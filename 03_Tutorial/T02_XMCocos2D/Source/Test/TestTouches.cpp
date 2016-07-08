/* --------------------------------------------------------------------------
 *
 *      File            TestTouches.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "TestTouches.h"

enum tagPlayer 
{
	kHighPlayer,
	kLowPlayer
} PlayerTouches;	

#define kStatusBarHeight    10.0f

enum 
{
	kSpriteTag
};

//------------------------------------------------------------------
//
// Paddle
//
//------------------------------------------------------------------
CCRect Paddle::rect ( KDvoid )
{
	CCSize  s = getTexture ( )->getContentSize ( );
	s.cx *= this->getScaleX ( );
	return CCRectMake ( -s.cx / 2, -s.cy / 2, s.cx, s.cy );
}

Paddle* Paddle::create ( CCTexture2D* aTexture )
{
	Paddle*  pRet = new Paddle ( );

	if ( pRet && pRet->initWithTexture ( aTexture ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	} 

	return pRet;
}

KDbool Paddle::initWithTexture ( CCTexture2D* aTexture )
{
	if ( CCSprite::initWithTexture ( aTexture ) ) 
	{
		m_state = kPaddleStateUngrabbed;
	}
	
	return KD_TRUE;
}

KDvoid Paddle::onEnter ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );

	CCSprite::onEnter ( );
}

KDvoid Paddle::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );

	CCSprite::onExit ( );
}	

KDbool Paddle::containsTouchLocation ( CCTouch* pTouch )
{
	return rect ( ).containsPoint ( convertTouchToNodeSpaceAR ( pTouch ) );
}

KDbool Paddle::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_state != kPaddleStateUngrabbed ) 
	{
		return KD_FALSE;
	}

	if ( !containsTouchLocation ( pTouch ) )
	{
		return KD_FALSE;
	}
	
	m_state = kPaddleStateGrabbed;

	return KD_TRUE;
}

KDvoid Paddle::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	// If it weren't for the TouchDispatcher, you would need to keep a reference
	// to the touch from touchBegan and check that the current touch is the same
	// as that one.
	// Actually, it would be even more complicated since in the Cocos dispatcher
	// you get CCSets instead of 1 UITouch, so you'd need to loop through the set
	// in each touchXXX method.
	
	CCAssert ( m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!" );	
	
	setPosition ( ccp ( this->getPosition ( ).x + pTouch->getDelta ( ).x, getPosition ( ).y ) );
}

KDvoid Paddle::ccTouchEnded ( CCTouch* touch, CCEvent* event )
{
	CCAssert ( m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!" );	
	
	m_state = kPaddleStateUngrabbed;
} 

KDvoid Paddle::touchDelegateRetain ( KDvoid )
{
	this->retain ( );
}

KDvoid Paddle::touchDelegateRelease ( KDvoid )
{
	this->release ( );
}

//------------------------------------------------------------------
//
// Ball
//
//------------------------------------------------------------------
KDfloat Ball::radius ( KDvoid )
{
	return getTexture ( )->getContentSize ( ).cx / 2;
}

Ball* Ball::create ( CCTexture2D* aTexture )
{
	Ball*  pRet = new Ball ( );

	if ( pRet && pRet->initWithTexture ( aTexture ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
		
	return pRet;
}

KDvoid Ball::move ( KDfloat fElapsed )
{
	const CCSize&  s = this->getParent ( )->getContentSize ( );

	setPosition ( ccpAdd ( getPosition ( ), ccpMult ( m_velocity, fElapsed ) ) );
	
	if ( getPosition ( ).x > s.cx - radius ( ) ) 
	{
		setPosition ( ccp ( s.cx - radius ( ), getPosition ( ).y ) );

		m_velocity.x *= -1;
	} 
	else if ( getPosition ( ).x < radius ( ) ) 
	{
		setPosition ( ccp ( radius ( ), getPosition ( ).y ) );

		m_velocity.x *= -1;
	}
}

KDvoid Ball::collideWithPaddle ( Paddle* paddle )
{
	CCRect    paddleRect = paddle->rect ( );
	paddleRect.origin.x += paddle->getPosition ( ).x;
	paddleRect.origin.y += paddle->getPosition ( ).y;
	
	KDfloat   lowY = paddleRect.getMinY ( );
	KDfloat   midY = paddleRect.getMidY ( );
	KDfloat  highY = paddleRect.getMaxY ( );

	KDfloat  leftX = paddleRect.getMinX ( );
	KDfloat rightX = paddleRect.getMaxX ( );
	
	if ( getPosition ( ).x > leftX && getPosition ( ).x < rightX )
	{	
		KDbool   hit = KD_FALSE;
		KDfloat  angleOffset = 0.0f; 
		
		if ( getPosition ( ).y > midY && getPosition ( ).y <= highY + radius ( ) ) 
		{
			setPosition ( ccp ( getPosition ( ).x, highY + radius ( ) ) );

			hit = KD_TRUE;
			angleOffset = KD_PI_F / 2.f;
		}
		else if ( getPosition ( ).y < midY && getPosition ( ).y >= lowY - radius ( ) ) 
		{
			setPosition ( ccp ( getPosition ( ).x, lowY - radius ( ) ) );

			hit = KD_TRUE;
			angleOffset = -KD_PI_F / 2.f;
		}
		
		if ( hit ) 
		{
			KDfloat  hitAngle = ccpToAngle ( ccpSub ( paddle->getPosition ( ), getPosition ( ) ) ) + angleOffset;
			
			KDfloat  scalarVelocity = ccpLength ( m_velocity ) * 1.05f;
			KDfloat  velocityAngle = -ccpToAngle ( m_velocity ) + 0.5f * hitAngle;
			
			m_velocity = ccpMult ( ccpForAngle ( velocityAngle ), scalarVelocity );
		}
	}	
} 

//------------------------------------------------------------------
//
// TestTouches
//
//------------------------------------------------------------------
KDbool TestTouches::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );
	CCSize  t;

	m_pBallStartingVelocity = ccp ( 20.0f, -100.0f );
	
    m_pBall = Ball::create ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/ball.png" ) );
	m_pBall->setPosition ( ccpMid ( s ) );
	m_pBall->setVelocity ( m_pBallStartingVelocity );
		
	this->addChild ( m_pBall );
	
	CCTexture2D*  paddleTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/paddle.png" );

	m_paddles = new CCArray ( 4 );

	for ( KDint i = 0; i < 4; i++ )
	{
		Paddle*  paddle = Paddle::create ( paddleTexture );

		t = paddle->getContentSize ( );
		paddle->setScaleX ( 2 );

		switch ( i )
		{
			case 0 : paddle->setPosition ( ccp ( s.cx / 2.f,        kStatusBarHeight + t.cy / 2.f ) ); break;
			case 1 : paddle->setPosition ( ccp ( s.cx / 2.f, s.cy - kStatusBarHeight - t.cy / 2.f ) ); break;
			case 2 : paddle->setPosition ( ccp ( s.cx / 2.f,        kStatusBarHeight + t.cy * 3.f ) ); break;
			case 3 : paddle->setPosition ( ccp ( s.cx / 2.f, s.cy - kStatusBarHeight - t.cy * 3.f ) ); break;
		}

		m_paddles->addObject ( paddle );
		this->addChild ( paddle );
	}

	this->schedule ( schedule_selector ( TestTouches::doStep ) );
	
	return KD_TRUE;
}

TestTouches::~TestTouches ( KDvoid )
{
	CC_SAFE_RELEASE ( m_paddles );
}

const KDchar* TestTouches::subtitle ( KDvoid )
{
	return "( tap and move paddle )";
}

KDvoid TestTouches::resetAndScoreBallForPlayer ( KDint player )
{
	const CCSize&  s = CCDirector::sharedDirector ( )->getWinSize ( );

	m_pBallStartingVelocity = ccpMult ( m_pBallStartingVelocity, -1.1f );

	m_pBall->setVelocity ( m_pBallStartingVelocity );
	m_pBall->setPosition ( ccpMid ( s ) );
}

KDvoid TestTouches::doStep ( KDfloat fElapsed )
{
	const CCSize&  s = this->getContentSize ( );

	m_pBall->move ( fElapsed );

	CCObject*   pObject = KD_NULL;
	CCARRAY_FOREACH ( m_paddles, pObject )
	{
		Paddle*  paddle = (Paddle*) pObject;

		if ( !paddle )
		{
			break;
		}

		m_pBall->collideWithPaddle ( paddle );
	}

	if ( m_pBall->getPosition ( ).y > s.cy - kStatusBarHeight + m_pBall->radius ( ) )
	{
		resetAndScoreBallForPlayer ( kLowPlayer );
	}
	else if ( m_pBall->getPosition ( ).y < -m_pBall->radius ( ) )
	{
		resetAndScoreBallForPlayer ( kHighPlayer );
	}

	m_pBall->draw ( );
} 


