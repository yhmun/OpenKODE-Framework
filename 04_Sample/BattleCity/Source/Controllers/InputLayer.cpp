/* --------------------------------------------------------------------------
 *
 *      File            InputLayer.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/07/13). All rights reserved.
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
#include "InputLayer.h"
#include "Views/MapLayer.h"
#include "SneakyInput/SneakyJoystick.h"
#include "SneakyInput/SneakyJoystickSkinnedBase.h"
#include "SneakyInput/SneakyButtonSkinnedBase.h"

InputLayer* InputLayer::create ( KDvoid )
{
	InputLayer*		pRet = new InputLayer ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool InputLayer::init ( KDvoid )
{
	m_pMapLayer = KD_NULL;

	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	this->setPosition ( ccpz );

	this->addFireButton ( );
	
	this->addJoystick ( );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid InputLayer::setMapLayer ( MapLayer* pLayer )
{
	m_pMapLayer = pLayer;
}

KDvoid InputLayer::update ( KDfloat fDelta )
{
    if ( !m_pMapLayer )
	{
		return;
	}
    
    Tank90Sprite*	pTank = m_pMapLayer->getTank ( );    
    if ( KD_NULL == pTank ) 
	{
		return;
	}

	CCPoint		tPos = ccpMult ( m_pJoystick->getVelocity ( ), 50 );

	if ( m_pFireButton->isActive ( ) )
	{
		pTank->onFire ( );
	}
    
    if ( ( tPos.x  >  0)  && ( tPos.x - tPos.y ) > 0 && ( tPos.x + tPos.y ) > 0 )
	{
        pTank->moveRight ( );
    }
	else if ( ( tPos.x < 0)  && ( tPos.x + tPos.y ) < 0 && ( tPos.x - tPos.y ) < 0 ) 
	{    
		pTank->moveLeft ( );
    }
	else if ( ( tPos.y > 0) && ( tPos.y - tPos.x ) > 0 && ( tPos.y + tPos.x ) > 0 )
	{    
		pTank->moveUp ( );
    }
	else if ( ( tPos.y < 0) && ( tPos.y - tPos.x ) < 0 && ( tPos.y + tPos.x ) < 0 ) 
	{    
		pTank->moveDown ( );
    }
}

KDvoid InputLayer::addFireButton ( KDvoid )
{
    KDfloat		fButtonRadius = 80;
    
	const CCSize&	tLyrSize = this->getContentSize ( );

	m_pFireButton = SneakyButton::create ( );

	SneakyButtonSkinnedBase*	pSkinFireButton = SneakyButtonSkinnedBase::create ( );
	pSkinFireButton->setPosition ( ccp ( tLyrSize.cx - fButtonRadius / 2, fButtonRadius / 2 ) );
	pSkinFireButton->setDefaultSprite ( CCSprite::create ( "Images/fire_button_default.png" ) );
	pSkinFireButton->setPressSprite ( CCSprite::create ( "Images/fire_button_press.png" ) );
	pSkinFireButton->setButton ( m_pFireButton );
	pSkinFireButton->setScale ( 0.5f );
	pSkinFireButton->getDefaultSprite ( )->setOpacity ( 100 );
	pSkinFireButton->getPressSprite ( )->setOpacity ( 100 );
	this->addChild ( pSkinFireButton );
}
 
KDvoid InputLayer::addJoystick ( KDvoid )
{
	KDfloat		fStickRadius = 70;
	
	m_pJoystick = SneakyJoystick::create ( CCRect ( 0, 0, fStickRadius, fStickRadius ) );
    m_pJoystick->setAutoCenter ( KD_TRUE );
    m_pJoystick->setDeadZone ( KD_TRUE );
    m_pJoystick->setDeadRadius ( 5 );

	SneakyJoystickSkinnedBase*	pSkinStick = SneakyJoystickSkinnedBase::create ( );
	pSkinStick->setPosition ( ccp ( fStickRadius, fStickRadius ) );
	pSkinStick->setScale ( 0.6f );
	pSkinStick->setBackgroundSprite ( CCSprite::create ( "Images/control_bg.png" ) );
	pSkinStick->setThumbSprite ( CCSprite::create ( "Images/cen.png" ) );
	pSkinStick->getThumbSprite ( )->setScale ( 1.f );
	pSkinStick->setJoystick ( m_pJoystick );
	pSkinStick->getBackgroundSprite ( )->setOpacity ( 100 );
	pSkinStick->getThumbSprite ( )->setOpacity ( 100 );
	
	this->addChild ( pSkinStick );
}
