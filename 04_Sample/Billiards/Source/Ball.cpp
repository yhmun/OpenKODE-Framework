/* --------------------------------------------------------------------------
 *
 *      File            Ball.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/20
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "Ball.h"
#include "Board.h"
#include "BallTracer.h"

Ball* Ball::create ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable )
{
	Ball*	pRet = new Ball ( );

	if ( pRet && pRet->initWithWorld ( pWorld, tPos, bTouchable ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Ball* Ball::create ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor )
{
	Ball*	pRet = new Ball ( );

	if ( pRet && pRet->initWithWorld ( pWorld, tPos, tColor ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Ball::~Ball ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );
}

KDbool Ball::initWithWorld ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable )
{
	return this->initWithWorld ( pWorld, tPos, bTouchable, ccWHITE );
}

KDbool Ball::initWithWorld ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor )
{
	return this->initWithWorld ( pWorld, tPos, KD_FALSE, tColor );
}

KDbool Ball::initWithWorld ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable, const ccColor3B& tColor )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_bIsInHall = KD_FALSE;

	this->createBallInWorld ( pWorld, tPos, tColor );

	if ( bTouchable ) 
	{
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_FALSE );
	}

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid Ball::update ( KDfloat fDelta )
{
	Board*		pBoard = (Board*) this->getParent ( );

	if ( pBoard->isInHall ( m_pSprite->getPosition ( ) ) )
	{
		m_bIsInHall = KD_TRUE;
	}
}

KDbool Ball::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{	
	CCPoint		tLocation = Helper::locationFromTouch ( pTouch );

    m_bMoveToFinger = KD_TRUE;
    m_tFingerLocation = tLocation;

	m_pBody->SetLinearVelocity ( b2Vec2 ( 0.0f, 0.0f ) );
	m_pBody->SetAngularVelocity ( 0.0f );

	Ball*		pMainBall = (Ball*) this->getParent ( )->getChildByTag ( kTagMainBall );
	m_pBallTracer = BallTracer::create ( m_tFingerLocation, pMainBall->getSprite ( )->getPosition ( ) );
	this->getParent ( )->addChild ( m_pBallTracer );
	
	return KD_TRUE;
}

KDvoid Ball::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_bMoveToFinger )
	{
		m_tFingerLocation = Helper::locationFromTouch ( pTouch );
		m_pBallTracer->setTouchLocation ( m_tFingerLocation );
	}
}

KDvoid Ball::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_bMoveToFinger )
	{
		this->applyForceTowardsFinger ( );
		m_bMoveToFinger = KD_FALSE;	    
		this->getParent ( )->removeChild ( m_pBallTracer, KD_TRUE );
	}
}

KDfloat Ball::ballWidth ( KDvoid )
{
	return CCSprite::create ( "ball.png" )->getContentSize ( ).cx;
}

KDvoid Ball::resetColor ( KDvoid )
{
	m_pSprite->setColor ( m_tOriginalColor );
}

KDbool Ball::isMainBall ( KDvoid )
{
	return this->getTag ( ) == kTagMainBall;
}

KDbool Ball::isInHall ( KDvoid )
{
	return m_bIsInHall;
}

KDbool Ball::isTouchForMe ( const CCPoint& tLocation )
{
	const CCPoint&  tSpritePos = m_pSprite->getPosition ( );
	return kdFabsf ( tSpritePos.x - tLocation.x ) < m_fBallRadius * 2 && kdFabsf ( tSpritePos.y - tLocation.y ) < m_fBallRadius * 2;
}

KDvoid Ball::createBallInWorld ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor )
{
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position = Helper::toMeters ( tPos );

	tBodyDef.angularDamping = 0.3f;
	tBodyDef.linearDamping  = 0.3f;

	m_fBallRadius = Ball::ballWidth ( ) * 0.5f;
	KDfloat			fRadiusInMeters = ( m_fBallRadius / PTM_RATIO );

	b2CircleShape	tShape;
	tShape.m_radius = fRadiusInMeters;

	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tShape;
	tFixtureDef.density = 0.5f;
	tFixtureDef.friction = 1.0f;
	tFixtureDef.restitution = 1.0f;

	BodyNode::createBodyInWorld ( pWorld, &tBodyDef, &tFixtureDef, "ball.png" );

	m_pSprite->setPosition ( tPos );
	m_pSprite->setColor ( tColor );

	m_tOriginalColor = tColor;
}

KDvoid Ball::applyForceTowardsFinger ( KDvoid )
{
	b2Vec2		tBodyPos   = m_pBody->GetWorldCenter ( );
	b2Vec2		tFingerPos = Helper::toMeters ( m_tFingerLocation );
	
	b2Vec2		tBodyToFinger = tFingerPos - tBodyPos;
	
	b2Vec2		tForce = 5.0f * tBodyToFinger;

	m_pBody->ApplyForce ( tForce, m_pBody->GetWorldCenter ( ) );
}

