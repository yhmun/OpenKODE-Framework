/* --------------------------------------------------------------------------
 *
 *      File            CCJoyStick.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Solo on 13-8-14
 *      More info from  http://www.cocoachina.com/bbs/read.php?tid=122995
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft.  
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

#include "extensions/Joystick/CCJoyStick.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/actions/CCActionEase.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"

NS_CC_EXT_BEGIN

JoyStick* JoyStick::create ( KDint nBallRadius, KDint nMoveAreaRadius, KDbool bIsFollowTouch, KDbool bIsCanVisible, KDbool bIsAutoHide, KDbool bHasAnimation )
{
	JoyStick*	pRet = new JoyStick ( );
	if ( pRet && pRet->init ( nBallRadius, nMoveAreaRadius, bIsFollowTouch, bIsCanVisible, bIsAutoHide, bHasAnimation ) )
	{
		pRet->autorelease ( );           
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool JoyStick::init ( KDint nBallRadius, KDint nMoveAreaRadius, KDbool bIsFollowTouch, KDbool bIsCanVisible, KDbool bIsAutoHide, KDbool bHasAnimation )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	m_nBallRadius		= nBallRadius;
	m_nMoveAreaRadius	= nMoveAreaRadius;
	m_bIsFollowTouch	= bIsFollowTouch;
	m_bIsCanVisible		= bIsCanVisible;
	m_bIsAutoHide		= bIsAutoHide;
	m_bHasAnimation		= bHasAnimation;
        
	m_fPower = 0;
	m_fAngle = 0;
	Size	tSize = Director::getInstance ( )->getWinSize ( );

	setHitAreaWithRect ( Rect ( 0, 0, tSize.width / 2, tSize.height / 2 ) );
        
	m_pBall  = Sprite::create ( );
	m_pStick = Sprite::create ( );
	m_pDock  = Sprite::create ( );
        
	addChild ( m_pDock );
	addChild ( m_pStick );
	addChild ( m_pBall );
        
	if ( !m_bIsCanVisible )
	{
		setVisible ( false );
	}
	else 
	{
		if ( m_bIsAutoHide )
		{
			setVisible ( false );
		}
	}

	return true;	
}

JoyStick::JoyStick ( KDvoid ) : m_fPower ( 0 ), m_fAngle ( 0 ), m_pDelegate ( KD_NULL )
{

}

JoyStick::~JoyStick ( KDvoid ) 
{
    
}

KDvoid JoyStick::onEnter ( KDvoid )
{
	Layer::onEnter ( );

    // Register Touch Event
    auto	pListener = EventListenerTouchOneByOne::create ( );
    pListener->setSwallowTouches ( true );
    
    pListener->onTouchBegan = CC_CALLBACK_2 ( JoyStick::onTouchBegan, this );
    pListener->onTouchMoved = CC_CALLBACK_2 ( JoyStick::onTouchMoved, this );
    pListener->onTouchEnded = CC_CALLBACK_2 ( JoyStick::onTouchEnded, this );
    
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListener, this );
}

KDvoid JoyStick::onExit ( KDvoid )
{
	Layer::onExit ( );
}

KDbool JoyStick::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	if ( !containsTouchLocation ( pTouch ) ) 
	{
		return false;
	}
    
	Point	tTouchPoint = pTouch->getLocation ( );

	touchBegan ( tTouchPoint );
    
	return true;
}

KDvoid JoyStick::touchBegan ( Point tTouchPoint )
{
	m_tCurrentPoint = tTouchPoint;
	m_bIsTouched = true;
	if ( m_bIsAutoHide && m_bIsCanVisible )
	{
		setVisible ( true );
	}
    
	if ( m_bIsFollowTouch )
	{
		setPosition ( tTouchPoint );
	}
	m_pBall->stopAllActions ( );
	updateTouchPoint ( tTouchPoint );
	startTimer ( );
}

KDvoid JoyStick::updateTouchPoint ( Point tTouchPoint )
{
	Point	tSelfposition = getParent ( )->convertToWorldSpace ( getPosition ( ) );
	if ( tTouchPoint.getDistance ( tSelfposition ) > ( m_nMoveAreaRadius - m_nBallRadius ) )
	{
		m_tCurrentPoint = ( tTouchPoint - tSelfposition ).normalize ( ) * ( m_nMoveAreaRadius - m_nBallRadius );
	}
	else 
	{
		m_tCurrentPoint = tTouchPoint - tSelfposition; 
	}
	m_pBall->setPosition ( m_tCurrentPoint );
    
	m_fAngle = kdAtan2f ( m_pBall->getPositionY ( ), m_pBall->getPositionX ( ) ) / ( 3.14159f / 180 );
	m_fPower = m_pBall->getPosition ( ).getDistance ( Point::ZERO ) / ( m_nMoveAreaRadius - m_nBallRadius );
	m_tDirection = Point ( kdCosf ( m_fAngle * ( 3.14159f / 180 ) ), kdSinf ( m_fAngle * ( 3.14159f / 180 ) ) );
    
	Sprite*		pSticktexture = (Sprite*) m_pStick->getChildByTag ( 0 );
	m_pStick->setScaleX ( m_pBall->getPosition ( ).getDistance ( Point::ZERO ) / pSticktexture->getTextureRect ( ).size.width );
	m_pStick->setRotation ( -m_fAngle );
}

KDvoid JoyStick::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
	Point	tTouchPoint = pTouch->getLocation ( );
    
	if ( m_bIsTouched )
	{
		updateTouchPoint ( tTouchPoint );
	}
}

KDvoid JoyStick::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	if ( m_bIsTouched )
	{
		if ( m_bIsAutoHide && m_bIsCanVisible )
		{
			setVisible ( false );
		}
		m_bIsTouched = false;
		stopTimer ( );
		resetTexturePosition ( );
	}
}

KDbool JoyStick::containsTouchLocation ( Touch* pTouch )
{
	Point	tTouchPoint = pTouch->getLocation ( );
	if ( m_nActiveRadius > 0 )
	{
		if ( tTouchPoint .getDistance ( getParent ( )->convertToWorldSpace ( getPosition ( ) ) ) < m_nActiveRadius )
		{
			return true;
		}
	}
	if ( m_tActiveRect.size.width > 0 && m_tActiveRect.size.height > 0 )
	{
		if (   tTouchPoint.x > m_tActiveRect.origin.x
            && tTouchPoint.x < m_tActiveRect.origin.x + m_tActiveRect.size.width
            && tTouchPoint.y > m_tActiveRect.origin.y
            && tTouchPoint.y
            < m_tActiveRect.origin.y + m_tActiveRect.size.height )
		{
			return true;
		}
	}
	return false;
}

KDvoid JoyStick::resetTexturePosition ( KDvoid )
{
	m_fPower = 0;
	m_fAngle = 0;
    
	m_tCurrentPoint = Point::ZERO;
	m_pStick->setPosition ( Point::ZERO );
	m_pStick->setScaleX ( m_fPower );
    
	if ( !m_bIsAutoHide && m_bIsCanVisible && m_bHasAnimation )
	{
		ActionInterval*	pAction = MoveTo::create ( 0.5f, Point::ZERO );
		m_pBall->runAction ( EaseElasticOut::create ( pAction ) );
	}
	else 
	{
		m_pBall->setPosition ( Point::ZERO );
	}
}

KDvoid JoyStick::startTimer ( KDvoid )
{
	if ( m_pDelegate != KD_NULL )
	{
		m_pDelegate->onJoyStickActivated ( this );
	}
	schedule ( schedule_selector ( JoyStick::timerUpdate ) );
}

KDvoid JoyStick::stopTimer ( KDvoid )
{
	if ( m_pDelegate != KD_NULL )
	{
		m_pDelegate->onJoyStickDeactivated ( this );
	}

	unschedule ( schedule_selector ( JoyStick::timerUpdate ) );
}

KDvoid JoyStick::timerUpdate ( KDvoid )
{
	if ( m_pDelegate != KD_NULL )
	{
		m_pDelegate->onJoyStickUpdate ( this, m_fAngle, m_tDirection, m_fPower );
	}
}

KDvoid JoyStick::timerUpdate ( KDfloat dt )
{
	timerUpdate ( );
}

KDvoid JoyStick::setBallTexture ( const KDchar* pImageName )
{
	m_pBall->removeAllChildrenWithCleanup ( true );
    
	Sprite*		pBalltexture = Sprite::create ( pImageName );
    
	m_pBall->addChild ( pBalltexture );
}

KDvoid JoyStick::setStickTexture ( const KDchar* pImageName )
{
	m_pStick->removeAllChildrenWithCleanup ( true );
    
	Sprite*		pSticktexture = Sprite::create ( pImageName );
	pSticktexture->setPosition ( Point ( pSticktexture->getContentSize ( ).width / 2, 0 ) );
    
	m_pStick->addChild ( pSticktexture, 0, 0 );
    
	m_pStick->setScaleX ( m_pBall->getPosition ( ).getDistance ( Point::ZERO ) / pSticktexture->getTextureRect ( ).size.width );
}

KDvoid JoyStick::setDockTexture ( const KDchar* pImageName )
{
	m_pDock->removeAllChildrenWithCleanup ( true );
    
	Sprite*		pDocktexture = Sprite::create ( pImageName );
    
	m_pDock->addChild ( pDocktexture );
}

KDvoid JoyStick::setHitAreaWithRadius ( KDint nRadius )
{
	m_tActiveRect = Rect ( 0, 0, 0, 0 );
	m_nActiveRadius = nRadius;
}

KDvoid JoyStick::setHitAreaWithRect ( Rect tRect ) 
{
	m_tActiveRect   = tRect;
	m_nActiveRadius = 0;
}

NS_CC_EXT_END