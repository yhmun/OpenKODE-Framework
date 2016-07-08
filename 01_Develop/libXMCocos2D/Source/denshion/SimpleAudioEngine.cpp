/* -----------------------------------------------------------------------------------
 *
 *      File            SimpleAudioEngine.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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

#include "Precompiled.h"
#include "denshion/SimpleAudioEngine.h"
#include "support/CCFileUtils.h"
#include "support/ccUtils.h"

#include <map>
#include <vector>

NS_CC_BEGIN

#define HAS_PLATFORM_AUDIO	0

typedef std::map <KDuint, XMSound*>		SoundList;
typedef std::pair<KDuint, XMSound*>		Sound;

static Sound				l_tMusic;
static SoundList			l_aEffectList;
static SimpleAudioEngine*	l_pSharedEngine = KD_NULL;
static KDbool				l_bIsBackgroundPlaying = KD_FALSE;

static KDvoid background_callBack ( const KDEvent* event )
{
	l_bIsBackgroundPlaying = KD_FALSE;
}

SimpleAudioEngine::SimpleAudioEngine ( KDvoid )
{
	l_tMusic.first  = 0;
	l_tMusic.second = KD_NULL;
}

SimpleAudioEngine::~SimpleAudioEngine ( KDvoid )
{
	this->stopBackgroundMusic ( KD_TRUE );
	    
	for ( SoundList::iterator it = l_aEffectList.begin ( ); it != l_aEffectList.end ( ); it++ )
    {
		if ( it->second )
		{
			xmSoundClose ( it->second );

			it->second = KD_NULL;
		}
    }   

    l_aEffectList.clear ( );
}

SimpleAudioEngine*  SimpleAudioEngine::sharedEngine ( KDvoid )
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
}

KDint SimpleAudioEngine::getClassTypeInfo ( KDvoid )
{
	return getHashCodeByString ( typeid ( SimpleAudioEngine ).name ( ) );
}

//////////////////////////////////////////////////////////////////////////
// BackgroundMusic
//////////////////////////////////////////////////////////////////////////

KDvoid SimpleAudioEngine::playBackgroundMusic ( const KDchar* szFilePath ) 
{
	this->playBackgroundMusic ( szFilePath, KD_TRUE );
}

KDvoid SimpleAudioEngine::playBackgroundMusic ( const KDchar* szFilePath, KDbool bLoop )
{
    if ( !szFilePath )
    {
        return;
    }

	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );

	preloadBackgroundMusic ( sPath.c_str ( ) );

	if ( l_tMusic.second )
	{
		xmSoundSetRepeat ( l_tMusic.second, bLoop );

		this->rewindBackgroundMusic ( );
	}
}

KDvoid SimpleAudioEngine::stopBackgroundMusic ( KDvoid )
{
	this->stopBackgroundMusic ( KD_FALSE );
}

KDvoid SimpleAudioEngine::stopBackgroundMusic ( KDbool bReleaseData )
{
	if ( bReleaseData )
	{
		if ( l_tMusic.second )
		{
			xmSoundClose ( l_tMusic.second );
		}

		l_tMusic.first  = 0;
		l_tMusic.second = KD_NULL;
	}
	else
	{
		if ( l_tMusic.second )
		{
			xmSoundStop ( l_tMusic.second );			
		}
	}

	l_bIsBackgroundPlaying = KD_FALSE;
}

KDvoid SimpleAudioEngine::pauseBackgroundMusic ( KDvoid )
{
	if ( l_tMusic.second )
	{
		xmSoundPause ( l_tMusic.second );
	}  
}

KDvoid SimpleAudioEngine::resumeBackgroundMusic ( KDvoid )
{
	if ( l_tMusic.second )
	{
		xmSoundResume ( l_tMusic.second );
	}  
}

KDvoid SimpleAudioEngine::rewindBackgroundMusic ( KDvoid )
{
	if ( l_tMusic.second )
	{
		l_bIsBackgroundPlaying = KD_TRUE;
		xmSoundRewind ( l_tMusic.second );
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

KDuint SimpleAudioEngine::playEffect ( const KDchar* szFilePath )
{
	return this->playEffect ( szFilePath, KD_FALSE );
}

KDuint SimpleAudioEngine::playEffect ( const KDchar* szFilePath, KDbool bLoop )
{
    if ( !szFilePath )
    {
        return 0;
    }

	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );
	KDuint		 uRet  = getHashCodeByString ( sPath.c_str ( ) );

    preloadEffect ( szFilePath );

	SoundList::iterator it = l_aEffectList.find ( uRet );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundSetRepeat ( it->second, bLoop );
		xmSoundRewind    ( it->second );
    }	
	else
	{
		uRet = 0;
	}

    return uRet;
}

KDvoid SimpleAudioEngine::playEffect ( KDuint uSoundId, KDbool bLoop )
{
	SoundList::iterator it = l_aEffectList.find ( uSoundId );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundSetRepeat ( it->second, bLoop );
		xmSoundRewind ( it->second );
	}
}

KDvoid SimpleAudioEngine::stopEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find ( uSoundId );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundStop ( it->second );
    }
}

KDuint SimpleAudioEngine::preloadEffect ( const KDchar* szFilePath )
{
	return this->preloadEffect ( szFilePath, KD_NULL );
}

KDuint SimpleAudioEngine::preloadEffect ( const KDchar* szFilePath, const KDchar* szKey )
{
	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );
	KDuint		   uID = getHashCodeByString ( szKey ? szKey : sPath.c_str ( ) );

	SoundList::iterator it = l_aEffectList.find ( uID );
	if ( it == l_aEffectList.end ( ) )
	{
		XMSound*  pSound = xmSoundOpen ( sPath.c_str ( ), XM_SOUND_EFFECT );

		if ( pSound )
		{
			l_aEffectList.insert ( Sound ( uID, pSound ) );
		}
	}

	return uID;	
}

KDvoid SimpleAudioEngine::pauseEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find ( uSoundId );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundPause ( it->second );
    }
}

KDvoid SimpleAudioEngine::pauseAllEffects ( KDvoid )
{
	for ( SoundList::iterator it = l_aEffectList.begin ( ); it != l_aEffectList.end ( ); it++ )
    {
		if ( it->second )
		{
			xmSoundPause ( it->second );
		}
    }   
}

KDvoid SimpleAudioEngine::resumeEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find ( uSoundId );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundResume ( it->second );
    }
}

KDvoid SimpleAudioEngine::resumeAllEffects ( KDvoid )
{
	for ( SoundList::iterator it = l_aEffectList.begin ( ); it != l_aEffectList.end ( ); it++ )
    {
		if ( it->second )
		{
			xmSoundResume ( it->second );
		}
    }   
}

KDvoid SimpleAudioEngine::stopAllEffects ( KDvoid )
{
	for ( SoundList::iterator it = l_aEffectList.begin ( ); it != l_aEffectList.end ( ); it++ )
    {
		if ( it->second )
		{
			xmSoundStop ( it->second );
		}
    }  
}

KDvoid SimpleAudioEngine::preloadBackgroundMusic ( const KDchar* szFilePath )
{
    if ( !szFilePath )
    {
        return;
    }

	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );
	KDuint uID = getHashCodeByString ( sPath.c_str ( ) );

	this->stopBackgroundMusic ( KD_TRUE );	

	if ( uID != l_tMusic.first )
	{		
		l_tMusic.first  = uID;
		l_tMusic.second = xmSoundOpen ( sPath.c_str ( ), XM_SOUND_NORMAL );

		if ( l_tMusic.second )
		{			
			xmSoundSetCallback ( l_tMusic.second, background_callBack );
		}
	}
}

KDvoid SimpleAudioEngine::unloadEffect ( const KDchar* szFilePath )
{
    if ( !szFilePath )
    {
        return;
    }

	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );
	KDuint		   uID = getHashCodeByString ( sPath.c_str ( ) );

	SoundList::iterator it = l_aEffectList.find ( uID );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundClose ( it->second );
		l_aEffectList.erase ( uID );
    }	
}

KDvoid SimpleAudioEngine::unloadEffect ( KDuint uSoundId )
{
	SoundList::iterator it = l_aEffectList.find ( uSoundId );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundClose ( it->second );
		l_aEffectList.erase ( uSoundId );
    }
}

//////////////////////////////////////////////////////////////////////////
// volume interface
//////////////////////////////////////////////////////////////////////////

KDfloat SimpleAudioEngine::getBackgroundMusicVolume ( KDvoid )
{	
    return xmSoundGetVolumes ( XM_SOUND_NORMAL );
}

KDvoid SimpleAudioEngine::setBackgroundMusicVolume ( KDfloat fVolume )
{
	xmSoundSetVolumes ( XM_SOUND_NORMAL, fVolume );
}

KDfloat SimpleAudioEngine::getEffectsVolume ( KDvoid )
{
    return xmSoundGetVolumes ( XM_SOUND_EFFECT );
}

KDvoid SimpleAudioEngine::setEffectsVolume ( KDfloat fVolume )
{
	xmSoundSetVolumes ( XM_SOUND_EFFECT, fVolume );
}

KDfloat SimpleAudioEngine::getEffectVolume ( KDuint uID )
{
	SoundList::iterator it = l_aEffectList.find ( uID );
    if ( it != l_aEffectList.end ( ) )
    {
		return xmSoundGetVolume ( it->second );
    }	

	return 0;
}

KDvoid SimpleAudioEngine::setEffectVolume ( KDuint uID, KDfloat fVolume )
{
	SoundList::iterator it = l_aEffectList.find ( uID );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundSetVolume ( it->second, fVolume );
    }	
}

KDfloat SimpleAudioEngine::getEffectsPitch ( KDvoid )
{
	 return xmSoundGetPitches ( XM_SOUND_EFFECT );
}

KDvoid SimpleAudioEngine::setEffectsPitch ( KDfloat fPitch )
{
	xmSoundSetPitches ( XM_SOUND_EFFECT, fPitch );
}

KDfloat SimpleAudioEngine::getEffectPitch ( KDuint uID )
{
	SoundList::iterator it = l_aEffectList.find ( uID );
    if ( it != l_aEffectList.end ( ) )
    {
		return xmSoundGetPitch ( it->second );
    }	

	return 0;
}

KDvoid SimpleAudioEngine::setEffectPitch ( KDuint uID, KDfloat fPitch )
{
	SoundList::iterator it = l_aEffectList.find ( uID );
    if ( it != l_aEffectList.end ( ) )
    {
		xmSoundSetPitch ( it->second, fPitch );
    }	
}

NS_CC_END
