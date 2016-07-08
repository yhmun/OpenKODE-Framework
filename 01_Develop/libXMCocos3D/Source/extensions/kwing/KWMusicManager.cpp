/* --------------------------------------------------------------------------
 *
 *      File            KWMusicManager.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/21 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
 *      
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "extensions/kwing/KWMusicManager.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"
#include "audio/SimpleAudioEngine.h"

USING_NS_CD;

NS_CC_EXT_BEGIN

static KWMusicManager*		l_pSharedManager = nullptr;

KWMusicManager*	KWMusicManager::sharedManager ( KDvoid )
{
	if ( !l_pSharedManager )
	{
		l_pSharedManager = new KWMusicManager ( );
		l_pSharedManager->init ( );
	}

	return l_pSharedManager;
}

KDbool KWMusicManager::init ( KDvoid )
{
	m_pIntroMusicCompletionListener = nullptr;

	return KD_TRUE;
}

KDvoid KWMusicManager::playMusic ( const KDchar* szFile, const KDchar* szIntroFile, KDbool bLoop )
{
	Scheduler*		pScheduler = Director::getInstance ( )->getScheduler ( );

	pScheduler->unscheduleSelector ( schedule_selector ( KWMusicManager::playNextMusic ), this );
	pScheduler->unscheduleSelector ( schedule_selector ( KWMusicManager::volumeDown ), this );

	this->stopMusic ( );

	this->setIntroMusicCompletionListener ( nullptr );

	SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( 1.f );
	
	m_bNextLoop = bLoop;

	if ( szIntroFile && kdStrlen ( szIntroFile ) > 0  )
	{
		m_sNextIntroMusic = szIntroFile;
		m_sNextLoopMusic  = szFile;

		SimpleAudioEngine::getInstance ( )->preloadBackgroundMusic ( szFile );
		SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( szIntroFile, false );
		SimpleAudioEngine::getInstance ( )->setBackgroundMusicMusicCompletionListener ( CC_CALLBACK_0 ( KWMusicManager::onIntroMusicCompletion, this ) );
	}
	else
	{
		m_sNextIntroMusic = "";
		m_sNextLoopMusic  = szFile;

		SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( szFile, bLoop );
	}
}

KDvoid KWMusicManager::playMusic ( const KDchar* szFile, KDbool bLoop )
{
	this->playMusic ( szFile, nullptr, bLoop );
}

KDvoid KWMusicManager::stopMusic ( KDvoid )
{
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( true );

	if ( m_sNextIntroMusic.size ( ) > 0 )
	{
		SimpleAudioEngine::getInstance ( )->unloadEffect ( m_sNextIntroMusic.c_str ( ) );
		m_sNextIntroMusic.clear ( );
	}

	if ( m_sNextLoopMusic.size ( ) > 0 )
	{
		SimpleAudioEngine::getInstance ( )->unloadEffect ( m_sNextLoopMusic .c_str ( ) );
		m_sNextLoopMusic .clear ( );
	}
}

KDvoid KWMusicManager::fadeout ( KDfloat s )
{
	Scheduler*	pScheduler = Director::getInstance ( )->getScheduler ( );

	pScheduler->scheduleSelector ( schedule_selector ( KWMusicManager::volumeDown ), this, s / 20.f, KD_FALSE );
}

KDvoid KWMusicManager::setIntroMusicCompletionListener ( const std::function<KDvoid()>& pFunc )
{
	m_pIntroMusicCompletionListener = pFunc;
}

KDvoid KWMusicManager::changeMusic ( const KDchar* szFile, const KDchar* szIntroFile, KDbool bLoop, KDfloat s )
{
	this->fadeout ( s );

	m_sNextIntroMusic = szIntroFile;
	m_sNextLoopMusic  = szFile;
	m_bNextLoop		  = bLoop;

	Scheduler*	pScheduler = Director::getInstance ( )->getScheduler ( );

	pScheduler->scheduleSelector ( schedule_selector ( KWMusicManager::playNextMusic ), this, s, KD_FALSE );
}

KDvoid KWMusicManager::onIntroMusicCompletion ( KDvoid )
{
	SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( m_sNextLoopMusic.c_str ( ), m_bNextLoop );

	if ( m_pIntroMusicCompletionListener )
	{
		m_pIntroMusicCompletionListener ( );	
		m_pIntroMusicCompletionListener = nullptr;
	}

	SimpleAudioEngine::getInstance ( )->setBackgroundMusicMusicCompletionListener ( nullptr );
}

KDvoid KWMusicManager::playNextMusic ( KDfloat fDelta )
{
	this->playMusic ( m_sNextLoopMusic.c_str ( ), m_sNextIntroMusic.c_str ( ), m_bNextLoop );
}

KDvoid KWMusicManager::volumeDown ( KDfloat fDelta )
{	
	KDfloat		fVolume = SimpleAudioEngine::getInstance ( )->getBackgroundMusicVolume ( );

	fVolume -= 1 / 20.f;

	SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( fVolume );

	if ( fVolume <= 0 )
	{
		Scheduler*	pScheduler = Director::getInstance ( )->getScheduler ( );

		pScheduler->unscheduleSelector ( schedule_selector ( KWMusicManager::volumeDown ), this );
	}
}

NS_CC_EXT_END