/* --------------------------------------------------------------------------
 *
 *      File            CompanyScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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

#include "Precompiled.h"
#include "CompanyScene.h"
#include "MenuMainScene.h"

KDbool CompanyScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*		pCompany = CCSprite::create ( "shinobigames_logo.pvr" );
	pCompany->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pCompany );

	this->scheduleOnce ( schedule_selector ( CompanyScene::onMainMenu ), 0.5f );

	return KD_TRUE;
}

KDvoid CompanyScene::onMainMenu ( KDfloat fDelta )
{
	SoundManager::sharedSoundManager ( )->showVersion ( );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ) ) );

	SoundManager::sharedSoundManager ( )->playSoundMusicMenu ( );
}
