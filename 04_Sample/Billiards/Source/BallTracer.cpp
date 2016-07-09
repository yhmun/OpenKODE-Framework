/* --------------------------------------------------------------------------
 *
 *      File            BallTracer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/24
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
#include "BallTracer.h"
#include "Ball.h"

BallTracer* BallTracer::create ( const CCPoint& tTouchLoaction, const CCPoint& tBallLocation )
{
	BallTracer*	pRet = new BallTracer ( );

	if ( pRet && pRet->initWithTouchLocation ( tTouchLoaction, tBallLocation ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool BallTracer::initWithTouchLocation ( const CCPoint& tTouchLoaction, const CCPoint& tBallLocation )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_tTouchLocation = tTouchLoaction;
	m_tBallLocation  = tBallLocation;

	m_fBallWidth = Ball::ballWidth ( );

	m_pBatch = CCSpriteBatchNode::create ( "ball.png" );
	this->addChild ( m_pBatch );

	this->addBallShadow ( );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid BallTracer::update ( KDfloat fDelta )
{
	KDfloat		dx = m_tBallLocation.x - m_tTouchLocation.x;
	KDfloat		dy = m_tBallLocation.y - m_tTouchLocation.y;
	KDfloat		fDistance = kdSqrtf ( dx * dx + dy * dy );

	KDuint		uShadowCount = (KDuint) ( fDistance / m_fBallWidth );

	if ( uShadowCount > m_pBatch->getChildren ( )->count ( ) )
	{
		while ( uShadowCount > m_pBatch->getChildren ( )->count ( ) )
		{
			this->addBallShadow ( );
		}
	}
	else 
	{
		while ( uShadowCount < m_pBatch->getChildren ( )->count ( ) )
		{
			CCSprite*	pLast = (CCSprite*) m_pBatch->getChildren ( )->lastObject ( );
			m_pBatch->removeChild ( pLast, KD_TRUE );
		}
	}

	for ( KDuint i = 0; i < uShadowCount; i++ )
	{
		CCSprite*	pShadowSprite = (CCSprite*) m_pBatch->getChildren ( )->objectAtIndex ( i );
	    
		KDfloat		d  = m_fBallWidth * ( i + 1 );
		KDfloat		sx = dx * ( d / fDistance ) + m_tTouchLocation.x;
		KDfloat		sy = dy * ( d / fDistance ) + m_tTouchLocation.y;
		pShadowSprite->setPosition ( ccp ( sx, sy ) );
	}
}

KDvoid BallTracer::setTouchLocation ( const CCPoint& tTouchLoaction )
{
	m_tTouchLocation = tTouchLoaction;
}

KDvoid BallTracer::addBallShadow ( KDvoid )
{
	CCSprite*	pBall = CCSprite::create ( "ball.png" );
    pBall->setOpacity ( 128 );
    m_pBatch->addChild ( pBall );
}
