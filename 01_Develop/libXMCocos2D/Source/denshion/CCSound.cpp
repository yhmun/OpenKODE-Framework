/* -----------------------------------------------------------------------------------
 *
 *      File            CCSound.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "denshion/CCSound.h"
#include "support/CCFileUtils.h"
#include "ccTypeInfo.h"
#include <map>

NS_CC_BEGIN
    
typedef struct STSound
{
	STSound ( KDvoid )
	{
		m_pSound = KD_NULL;
		m_pTarget = KD_NULL;
		m_pSelector = KD_NULL;
	}

	XMSound*		m_pSound;
	CCObject*		m_pTarget;
	SEL_SOUND		m_pSelector;

} STSound;

static CCSound*                    l_pSharedSound = KD_NULL;
static std::map<KDuint, STSound>   l_aSounds;

static KDvoid _CallBack ( const KDEvent* event )
{
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.begin ( );

	for ( ; iter != l_aSounds.end ( ); iter++ )
	{
		if ( iter->second.m_pSound == event->userptr )
		{
			( iter->second.m_pTarget->*iter->second.m_pSelector ) ( iter->first, event->type );
		}
	}
}

CCSound::CCSound ( KDvoid )
{

}

CCSound::~CCSound ( KDvoid )
{

}

CCSound* CCSound::sharedSound ( KDvoid )
{
	if ( !l_pSharedSound )
	{
		l_pSharedSound = new CCSound ( );
	}

	return l_pSharedSound;
}

KDvoid CCSound::purgeSound ( KDvoid )
{
	std::map<KDuint, STSound>::iterator  iter;
	for ( iter = l_aSounds.begin ( ); iter != l_aSounds.end(); iter++ )
	{
		xmSoundClose ( iter->second.m_pSound );
	}

	l_aSounds.clear ( );

	CC_SAFE_DELETE ( l_pSharedSound );
}

KDuint CCSound::Open ( const KDchar* szFilePath, KDuint nMode )
{
	std::string  sFilePath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );

	KDint     uRet = 0;
	XMSound*  pSound = KD_NULL;
	do 
	{
		CC_BREAK_IF ( !sFilePath.c_str ( ) );

		uRet = getHashCodeByString ( sFilePath.c_str ( ) );

		CC_BREAK_IF ( l_aSounds.end ( ) != l_aSounds.find ( uRet ) );

		if ( ( pSound = xmSoundOpen ( sFilePath.c_str ( ), nMode ) ) )
		{
			STSound  tSound;
			tSound.m_pSound = pSound;

			l_aSounds.insert ( std::pair<KDuint, STSound> ( uRet, tSound ) );
		}
		else
		{
			uRet = 0;
		}

	} while ( 0 );

	return uRet;
}

KDint CCSound::Close ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundClose ( iter->second.m_pSound );
		l_aSounds.erase ( uSoundID );
    }

	return ret;
}

KDint CCSound::Play ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundPlay ( iter->second.m_pSound );
    }

	return ret;
}

KDint CCSound::Stop ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundStop ( iter->second.m_pSound );
    }

	return ret;
}

KDint CCSound::Pause ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );
    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundPause ( iter->second.m_pSound );
    }

	return ret;
}

KDint CCSound::Resume ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundResume ( iter->second.m_pSound );
    }

	return ret;
}

KDint CCSound::Rewind ( KDuint uSoundID )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundRewind ( iter->second.m_pSound );
    }

	return ret;
}

KDint CCSound::setRepeat ( KDuint uSoundID, KDbool bOn )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		ret = xmSoundSetRepeat ( iter->second.m_pSound, bOn );
    }

	return ret;
}

KDfloat CCSound::getVolume ( KDuint uSoundID )
{
	KDfloat  ret = -1;
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

	if ( iter != l_aSounds.end ( ) )
	{
		ret = xmSoundGetVolume ( iter->second.m_pSound );
	}

	return ret;
}

KDint CCSound::setVolume ( KDuint uSoundID, KDfloat fVolume )
{
	KDint  ret = -1;
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

	if ( iter != l_aSounds.end ( ) )
	{
		ret = xmSoundSetVolume ( iter->second.m_pSound, fVolume );
	}

	return ret;
}

KDfloat CCSound::getVolumes ( KDuint nMode )
{
	return xmSoundGetVolumes ( nMode );
}

KDint CCSound::setVolumes ( KDuint nMode, KDfloat fVolume )
{
	return xmSoundSetVolumes ( nMode, fVolume );
}

/////////////////////////////////////////////////////////////////////////////////////
// Effect Mode : Not worked.
KDint CCSound::getDuration ( KDuint uSoundID )
{
	KDint  ret = -1;
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

	if ( iter != l_aSounds.end ( ) )
	{
		ret = xmSoundGetDuration ( iter->second.m_pSound );
	}

	return ret;
}

KDint CCSound::getPosition ( KDuint uSoundID )
{
	KDint  ret = -1;
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

	if ( iter != l_aSounds.end ( ) )
	{
		ret = xmSoundGetPosition ( iter->second.m_pSound );
	}

	return ret;
}

KDint CCSound::setPosition ( KDuint uSoundID, KDint nPos )
{
	KDint  ret = -1;
	std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

	if ( iter != l_aSounds.end ( ) )
	{
		ret = xmSoundSetPosition ( iter->second.m_pSound, nPos );
	}

	return ret;
}

KDint CCSound::setCallFunc ( KDuint uSoundID, CCObject* pTarget, SEL_SOUND pSelector )
{
	KDint  ret = -1;
    std::map<KDuint, STSound>::iterator  iter = l_aSounds.find ( uSoundID );

    if ( iter != l_aSounds.end ( ) )
    {
		iter->second.m_pTarget = pTarget;
		iter->second.m_pSelector = pSelector;

		KDCallbackFunc*  pFunc = ( pTarget && pSelector ) ? _CallBack : KD_NULL;
		ret = xmSoundSetCallback ( iter->second.m_pSound, pFunc );
    }

	return ret;
}


NS_CC_END