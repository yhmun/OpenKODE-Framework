/* --------------------------------------------------------------------------
 *
 *      File            XMBullet.cpp
 *      Description     XMBullet
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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
#include "XMBullet.h"

KDbool XMBullet::initWithFile ( const std::string& sFilename )
{
	if ( !PhysicsSprite::initWithFile ( sFilename ) )
	{
		return KD_FALSE;
	}
	
	Point tPoint = this->getPosition ( );

	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	b2Body*			pBody = m_pParentWorld->CreateBody ( &tBodyDef );

	// Define another box shape for our dynamic body.
	b2PolygonShape	tDynamicBox;
	//the collision box size of bullet
	Size tSize = this->getContentSize ( );
	tDynamicBox.SetAsBox ( tSize.width / 2 / PT_RATIO, tSize.height / 2 / PT_RATIO );
	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tDynamicBox;
	tFixtureDef.isSensor = true;
	tFixtureDef.userData = this;

	pBody->CreateFixture ( &tFixtureDef );

	this->setB2Body ( pBody );
	this->setPTMRatio ( PT_RATIO );

	m_nDamage = 1;

	return KD_TRUE;
}

KDvoid XMBullet::shootLinear ( XMShootDirection eDirection, KDfloat fSpeed )
{
	Point tPoint = this->getPosition ( );
	switch ( eDirection )
	{
		case SHOOTDIRECTION_LEFT :
			
			tPoint.x -= 1;
			shootAiming ( tPoint, fSpeed );
			break;
		case SHOOTDIRECTION_RIGHT :
			tPoint.x += 1;
			shootAiming ( tPoint, fSpeed );
			break;
		default:
			break;
	}
}

KDvoid XMBullet::shootAiming ( Point tPoint, KDfloat fSpeed )
{
	const Size		tLyrSize = Director::getInstance ( )->getWinSize ( );

	Point			tPoint1 = tPoint;
	Point			tPoint2 = this->getPosition ( );

	KDfloat			fCheckX = 0;
	KDfloat			fCheckY = 0;

	if ( tPoint1.x > tPoint2.x )
	{
		fCheckX = tLyrSize.cx;
	}
	else
	{
		fCheckX = 0;
	}
	KDfloat			fOffX = tPoint2.x - tPoint1.x;
	KDfloat			fOffY = tPoint2.y - tPoint1.y;
	KDfloat			fRatio = fOffY / fOffX;
	KDfloat			fRealX;
	KDfloat			fRealY;

	fCheckY = ( ( fCheckX - tPoint1.x ) * fRatio ) + tPoint1.y;	

	if ( fCheckY < 0 )
	{
		fRealY = -( getContentSize ( ).height / 2 );
		fRealX = ( ( fRealY - tPoint1.y ) / fRatio ) + tPoint1.x;
	}
	else if ( fCheckY > tLyrSize.height )
	{
		fRealY = ( tLyrSize.height + getContentSize ( ).height / 2 );
		fRealX = ( ( fRealY - tPoint1.y ) / fRatio ) + tPoint1.x;
	}
	else
	{
		if ( tPoint1.x > tPoint2.x )
		{
			fRealX = tLyrSize.cx + ( this->getContentSize ( ).cx / 2 );
		}
		else
		{
			fRealX = -( this->getContentSize ( ).cx / 2 );
		}
		fRealY = ( ( fRealX - tPoint1.x ) * fRatio ) + tPoint1.y;
	}

	Point tRealDest = Point ( fRealX, fRealY );

	// Determine the fLength of how far we're shooting
	KDfloat			fOffRealX = tRealDest.x - this->getPosition ( ).x;
	KDfloat			fOffRealY = tRealDest.y - this->getPosition ( ).y;
	KDfloat			fLength = kdSqrtf ( ( fOffRealX * fOffRealX ) + ( fOffRealY * fOffRealY ) );
	KDfloat			fVelocity = fSpeed / 1; // 480pixels/1sec
	KDfloat			fRealMoveDufRation = fLength / fVelocity;

	b2Body*			pBody = this->getB2Body ( );

	ActionInterval* pAction = Sequence::create
	(
		MoveTo::create ( fRealMoveDufRation, tRealDest ),
		CallFunc::create ( CC_CALLBACK_0 ( XMBullet::destroyBullet, this, this, ( KDvoid* ) pBody ) ),
		nullptr
	);

	this->runAction ( pAction );
}

KDvoid XMBullet::destroyBullet ( Node* pSender, KDvoid* pData )
{
	m_pParentWorld->DestroyBody ( ( b2Body* ) pData );
	pSender->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid XMBullet::setDamage ( KDint nDamage )
{
	m_nDamage = nDamage;
}

KDint XMBullet::getDamage ( KDvoid )
{
	return m_nDamage;
}

KDvoid XMBullet::shootBulletMoveAndSpread ( Point tPoint, KDfloat fSpeed )
{
	const Size		tLyrSize = Director::getInstance ( )->getWinSize ( );

	Point tPoint1 = tPoint;
	Point tPoint2 = this->getPosition ( );

	KDfloat fCheckX = 0;
	KDfloat fCheckY = 0;

	if ( tPoint1.x > tPoint2.x )
	{
		fCheckX = tLyrSize.cx;
	}
	else
	{
		fCheckX = 0;
	}
	KDfloat			fOffX = tPoint2.x - tPoint1.x;
	KDfloat			fOffY = tPoint2.y - tPoint1.y;
	KDfloat			fRatio = fOffY / fOffX;
	KDfloat			fRealX;
	KDfloat			fRealY;

	fCheckY = ( ( fCheckX - tPoint1.x ) * fRatio ) + tPoint1.y;

	if ( fCheckY < 0 )
	{
		fRealY = -( getContentSize ( ).height / 2 );
		fRealX = ( ( fRealY - tPoint1.y ) / fRatio ) + tPoint1.x;
	}
	else if ( fCheckY > tLyrSize.height )
	{
		fRealY = ( tLyrSize.height + getContentSize ( ).height / 2 );
		fRealX = ( ( fRealY - tPoint1.y ) / fRatio ) + tPoint1.x;
	}
	else
	{
		if ( tPoint1.x > tPoint2.x )
		{
			fRealX = tLyrSize.cx + ( this->getContentSize ( ).cx / 2 );
		}
		else
		{
			fRealX = -( this->getContentSize ( ).cx / 2 );
		}
		fRealY = ( ( fRealX - tPoint1.x ) * fRatio ) + tPoint1.y;
	}

	Point			tRealDest = Point ( fRealX , fRealY );

	// Determine the fLength of how far we're shooting
	KDfloat			fOffRealX = tRealDest.x - this->getPosition ( ).x;
	KDfloat			fOffRealY = tRealDest.y - this->getPosition ( ).y;
	KDfloat			fLength = kdSqrtf ( ( fOffRealX * fOffRealX ) + ( fOffRealY * fOffRealY ) );
	KDfloat			fVelocity = fSpeed / 1; // 480pixels/1sec
	KDfloat			fRealMoveDufRation = fLength / fVelocity;

	b2Body*			pBody = this->getB2Body ( );

	ActionInterval* pAction = Sequence::create
	(
		Spawn::create 
		(
			MoveTo::create ( fRealMoveDufRation, tRealDest ),
			Sequence::create
			(
			    DelayTime::create ( 1.0f ),
				ScaleTo::create ( 0, 0 ),
			    CallFunc::create ( CC_CALLBACK_0 ( XMBullet::destroyBullet, this, this, ( KDvoid* ) pBody ) ),
				nullptr
			),
			nullptr
		),
		nullptr
	);

	this->runAction ( pAction );
}
