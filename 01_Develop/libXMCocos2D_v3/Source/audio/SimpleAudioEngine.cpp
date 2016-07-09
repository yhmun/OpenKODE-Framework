/* -----------------------------------------------------------------------------------
 *
 *      File            SimpleAudioEngine.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Steve Oldmeadow
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */

#include "audio/SimpleAudioEngine.h"
#include "platform/CCFileUtils.h"
#include "support/ccUtils.h"

USING_NS_CC;

using namespace std;

namespace CocosDenshion {

typedef std::map <KDuint, XMSound*>		SoundList;
typedef std::pair<KDuint, XMSound*>		Sound;

static SoundList					l_aMusicList;
static SoundList					l_aEffectList;
static KDuint						l_uCurrentMusic = 0;
static SimpleAudioEngine*			l_pSharedEngine = nullptr;
static KDbool						l_bIsBackgroundPlaying = KD_FALSE;
static std::function<KDvoid()>		l_pBackgroundMusicMusicCompletionListener = nullptr;

//////////////////////////////////////////////////////////////////////////
// static function
//////////////////////////////////////////////////////////////////////////

static KDuint _Hash(const KDchar *key)
{
	unsigned int len = strlen(key);
	const char *end = key + len;
	unsigned int hash;

	for (hash = 0; key < end; key++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)(unsigned char)toupper(*key);
	}
	return (hash);
}

static KDvoid background_callBack ( const KDEvent* pEvent )
{
	l_bIsBackgroundPlaying = KD_FALSE;

    if ( l_pBackgroundMusicMusicCompletionListener )
	{
        l_pBackgroundMusicMusicCompletionListener ( );
    }
}

SimpleAudioEngine::SimpleAudioEngine ( KDvoid )
{

}

SimpleAudioEngine::~SimpleAudioEngine ( KDvoid )
{
	for ( SoundList::iterator it = l_aMusicList.begin ( ); it != l_aMusicList.end ( ); it++ )
	{
		if ( it->second )
		{
			xmSoundClose ( it->second );
			it->second = nullptr;
		}
	}
	l_aMusicList.clear ( );

	for ( SoundList::iterator it = l_aEffectList.begin ( ); it != l_aEffectList.end ( ); it++ )
	{
		if ( it->second )
		{
			xmSoundClose ( it->second );
			it->second = nullptr;
		}
	}
	l_aEffectList.clear ( );
}

SimpleAudioEngine* SimpleAudioEngine::getInstance ( KDvoid )
{
	if ( !l_pSharedEngine )
	{
		l_pSharedEngine = new SimpleAudioEngine ( );
	}

	return l_pSharedEngine;
}

KDvoid SimpleAudioEngine::end ( KDvoid )
{
	CC_SAFE_DELETE ( l_pSharedEngine );
    return;
}

//////////////////////////////////////////////////////////////////////////
// BackgroundMusic
//////////////////////////////////////////////////////////////////////////

KDvoid SimpleAudioEngine::playBackgroundMusic ( const KDchar* szFilePath, bool bLoop )
{
	if ( !szFilePath )
	{
		return;
	}

	std::string  sPath = FileUtils::getInstance ( )->fullPathForFilename ( szFilePath );
	KDuint		 uRet  = _Hash ( sPath.c_str ( ) );

	preloadBackgroundMusic ( sPath.c_str ( ) );

	SoundList::iterator it = l_aMusicList.find ( uRet );
	if ( it != l_aMusicList.end ( ) )
	{
		l_uCurrentMusic = uRet;
		xmSoundSetRepeat ( it->second, bLoop );	
		this->rewindBackgroundMusic ( );
	}
}

KDvoid SimpleAudioEngine::stopBackgroundMusic ( KDbool bReleaseData )
{
	SoundList::iterator it = l_aMusicList.find ( l_uCurrentMusic );

	if ( it != l_aMusicList.end ( ) )
	{
		if ( bReleaseData )
		{
			if ( it->second )
			{
				xmSoundClose ( it->second );
			}

			l_uCurrentMusic = 0;
			l_aEffectList.erase ( l_uCurrentMusic );
		}
		else
		{
			if ( it->second )
			{
				xmSoundStop ( it->second );
			}
		}
	}

	l_bIsBackgroundPlaying = KD_FALSE;
}

KDvoid SimpleAudioEngine::pauseBackgroundMusic ( KDvoid )
{
	SoundList::iterator it = l_aMusicList.find ( l_uCurrentMusic );

	if ( it != l_aMusicList.end ( ) )
	{
		if ( it->second )
		{
			xmSoundPause ( it->second );
		}
	}
}

KDvoid SimpleAudioEngine::resumeBackgroundMusic ( KDvoid )
{
	SoundList::iterator it = l_aMusicList.find ( l_uCurrentMusic );

	if ( it != l_aMusicList.end ( ) )
	{
		if ( it->second )
		{
			xmSoundResume ( it->second );
		}
	}
}

KDvoid SimpleAudioEngine::rewindBackgroundMusic ( KDvoid )
{
	SoundList::iterator it = l_aMusicList.find ( l_uCurrentMusic );

	if ( it != l_aMusicList.end ( ) )
	{
		if ( it->second )
		{
			l_bIsBackgroundPlaying = KD_TRUE;
			xmSoundRewind ( it->second );
		}
	}
}

KDbool SimpleAudioEngine::willPlayBackgroundMusic ( KDvoid )
{
	return KD_FALSE;
}

KDbool SimpleAudioEngine::isBackgroundMusicPlaying ( KDvoid )
{
	return l_bIsBackgroundPlaying;
}

//////////////////////////////////////////////////////////////////////////
// effect function
//////////////////////////////////////////////////////////////////////////

KDuint SimpleAudioEngine::playEffect ( const KDchar* szFilePath, KDbool bLoop, KDfloat fPitch, KDfloat fPan, KDfloat fGain )
{
	if (!szFilePath)
	{
		return 0;
	}

	std::string  sPath = FileUtils::getInstance()->fullPathForFilename(szFilePath);
	KDuint		 uRet = _Hash(sPath.c_str());

	preloadEffect(szFilePath);

	SoundList::iterator it = l_aEffectList.find(uRet);
	if (it != l_aEffectList.end())
	{
		xmSoundSetRepeat(it->second, bLoop);		
		xmSoundSetVolume(it->second, fGain);
		xmSoundSetPitch(it->second, fPitch);
		xmSoundSetPan(it->second, fPan);
		xmSoundRewind(it->second);
	}
	else
	{
		uRet = 0;
	}

	return uRet;
}

KDvoid SimpleAudioEngine::playEffect ( KDuint uSoundId, KDbool bLoop, KDfloat fPitch, KDfloat fPan, KDfloat fGain )
{
	SoundList::iterator it = l_aEffectList.find(uSoundId);
	if (it != l_aEffectList.end())
	{
		xmSoundSetRepeat(it->second, bLoop);
		xmSoundSetVolume(it->second, fGain);
		xmSoundSetPitch(it->second, fPitch);
		xmSoundSetPan(it->second, fPan);
		xmSoundRewind(it->second);
	}
}

KDvoid SimpleAudioEngine::stopEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find(uSoundId);
	if (it != l_aEffectList.end())
	{
		xmSoundStop(it->second);
	}
}

KDuint SimpleAudioEngine::preloadEffect(const KDchar* szFilePath, const KDchar* szKey)
{
	std::string  sPath = FileUtils::getInstance()->fullPathForFilename(szFilePath);
	KDuint		   uID = _Hash(szKey ? szKey : sPath.c_str());

	SoundList::iterator it = l_aEffectList.find(uID);
	if (it == l_aEffectList.end())
	{
		XMSound*  pSound = xmSoundOpen(sPath.c_str(), XM_SOUND_EFFECT);

		if (pSound)
		{
			l_aEffectList.insert(Sound(uID, pSound));
		}
	}

	return uID;
}

KDvoid SimpleAudioEngine::pauseEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find(uSoundId);
	if (it != l_aEffectList.end())
	{
		xmSoundPause(it->second);
	}
}

KDvoid SimpleAudioEngine::pauseAllEffects ( KDvoid )
{
	for (SoundList::iterator it = l_aEffectList.begin(); it != l_aEffectList.end(); it++)
	{
		if (it->second)
		{
			xmSoundPause(it->second);
		}
	}
}

KDvoid SimpleAudioEngine::resumeEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find(uSoundId);
	if (it != l_aEffectList.end())
	{
		xmSoundResume(it->second);
	}
}

KDvoid SimpleAudioEngine::resumeAllEffects ( KDvoid )
{
	for (SoundList::iterator it = l_aEffectList.begin(); it != l_aEffectList.end(); it++)
	{
		if (it->second)
		{
			xmSoundResume(it->second);
		}
	}
}

KDvoid SimpleAudioEngine::stopAllEffects ( KDvoid )
{
	for (SoundList::iterator it = l_aEffectList.begin(); it != l_aEffectList.end(); it++)
	{
		if (it->second)
		{
			xmSoundStop(it->second);
		}
	}
}

KDvoid SimpleAudioEngine::preloadBackgroundMusic ( const KDchar* szFilePath )
{
	if ( !szFilePath )
	{
		return;
	}

	std::string  sPath = FileUtils::getInstance ( )->fullPathForFilename ( szFilePath );
	KDuint		 uID   = _Hash ( sPath.c_str ( ) );

	this->stopBackgroundMusic ( true );

	SoundList::iterator it = l_aMusicList.find ( uID );
	if ( it == l_aMusicList.end ( ) )
	{
		XMSound*  pSound = xmSoundOpen ( sPath.c_str ( ), XM_SOUND_NORMAL );

		if ( pSound )
		{
			xmSoundSetCallback ( pSound, background_callBack );

			l_aMusicList.insert ( Sound ( uID, pSound ) );			
		}
	}
}

KDvoid SimpleAudioEngine::unloadEffect ( const KDchar* szFilePath )
{
	if (!szFilePath)
	{
		return;
	}

	std::string  sPath = FileUtils::getInstance()->fullPathForFilename(szFilePath);
	KDuint		   uID = _Hash(sPath.c_str());

	SoundList::iterator it = l_aEffectList.find(uID);
	if (it != l_aEffectList.end())
	{
		xmSoundClose(it->second);
		l_aEffectList.erase(uID);
	}
}

KDvoid SimpleAudioEngine::unloadEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find(uSoundId);
	if (it != l_aEffectList.end())
	{
		xmSoundClose(it->second);
		l_aEffectList.erase(uSoundId);
	}
}

//////////////////////////////////////////////////////////////////////////
// volume interface
//////////////////////////////////////////////////////////////////////////

KDfloat SimpleAudioEngine::getBackgroundMusicVolume ( KDvoid )
{
	return xmSoundGetVolumes(XM_SOUND_NORMAL);
}

KDvoid SimpleAudioEngine::setBackgroundMusicVolume ( KDfloat fVolume )
{
	xmSoundSetVolumes(XM_SOUND_NORMAL, fVolume);
}

KDfloat SimpleAudioEngine::getEffectsVolume ( KDvoid )
{
	return xmSoundGetVolumes(XM_SOUND_EFFECT);
}

KDvoid SimpleAudioEngine::setEffectsVolume ( KDfloat fVolume )
{
	xmSoundSetVolumes(XM_SOUND_EFFECT, fVolume);
}

KDfloat SimpleAudioEngine::getEffectVolume ( KDuint uID )
{
	SoundList::iterator it = l_aEffectList.find(uID);
	if (it != l_aEffectList.end())
	{
		return xmSoundGetVolume(it->second);
	}

	return 0;
}

KDvoid SimpleAudioEngine::setEffectVolume ( KDuint uID, KDfloat fVolume )
{
	SoundList::iterator it = l_aEffectList.find(uID);
	if (it != l_aEffectList.end())
	{
		xmSoundSetVolume(it->second, fVolume);
	}
}

KDfloat SimpleAudioEngine::getEffectsPitch ( KDvoid )
{
	return xmSoundGetPitches(XM_SOUND_EFFECT);
}

KDvoid SimpleAudioEngine::setEffectsPitch ( KDfloat fPitch )
{
	xmSoundSetPitches ( XM_SOUND_EFFECT, fPitch );
}

KDfloat SimpleAudioEngine::getEffectPitch ( KDuint uID )
{
	SoundList::iterator it = l_aEffectList.find(uID);
	if (it != l_aEffectList.end())
	{
		return xmSoundGetPitch(it->second);
	}

	return 0;
}

KDvoid SimpleAudioEngine::setEffectPitch(KDuint uID, KDfloat fPitch)
{
	SoundList::iterator it = l_aEffectList.find(uID);
	if (it != l_aEffectList.end())
	{
		xmSoundSetPitch(it->second, fPitch);
	}
}

KDvoid SimpleAudioEngine::unloadMusic ( const KDchar* szFilePath )
{
	if ( !szFilePath )
	{
		return;
	}

	std::string  sPath = FileUtils::getInstance()->fullPathForFilename ( szFilePath );
	KDuint		   uID = _Hash ( sPath.c_str ( ) );

	SoundList::iterator it = l_aMusicList.find ( uID );
	if ( it != l_aMusicList.end ( ) )
	{
		xmSoundClose ( it->second );
		l_aMusicList.erase ( uID );
	}

	if ( l_uCurrentMusic == uID )
	{
		l_uCurrentMusic = 0;
	}
}

KDvoid SimpleAudioEngine::setBackgroundMusicMusicCompletionListener ( const std::function<KDvoid()>& pFunc )
{
	l_pBackgroundMusicMusicCompletionListener = pFunc;
}


} // end of namespace CocosDenshion
