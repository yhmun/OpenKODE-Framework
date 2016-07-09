/* --------------------------------------------------------------------------
 *
 *      File            Tank.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-11-04
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
#include "Tank.h"

KDbool Tank::init ( KDvoid )
{
	if ( !EnemyBase::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pSprite1 = CCSprite::create ( "Image 320 at frame 10.png" );
	CCSprite*	pSprite2 = CCSprite::create ( "Image 321 at frame 10.png" );
	CCSprite*	pSprite4 = CCSprite::create ( "Image 324 at frame 10.png" );

	pSprite1->setPosition ( ccp (   0, -10 ) );
	pSprite2->setPosition ( ccp (   0,   0 ) );
	pSprite4->setPosition ( ccp ( -15,  18 ) );

	this->addChild ( pSprite1 );
	this->addChild ( pSprite2 );
	this->addChild ( pSprite4 );

	this->setScale  ( IOSSCALE );
	this->setActive ( KD_FALSE );

	return KD_TRUE;
}

KDvoid Tank::active ( KDvoid )
{
	if ( this->isActive ( ) )
	{
		return;
	}
	this->setActive ( KD_TRUE );

    if ( this->getType ( ) == 0 ) 
	{
//		CCArray*	pFrames = CCArray::create 
//		(
//			CCSpriteFrame::create ( "Image 904 at frame 10.png", CCRect ( 0, 0, 46, 21 ) ),
//			CCSpriteFrame::create ( "Image 905 at frame 10.png", CCRect ( 0, 0, 46, 22 ) ),
//			KD_NULL
//		);

		this->schedule ( schedule_selector ( Tank::tankFire ), 2 );
	}
}

KDbool Tank::beenAttack ( KDint nType )
{
	return KD_TRUE;
}

KDvoid Tank::tankFire ( KDfloat fDelta )
{
	CCSprite*	pBullet = CCSprite::create ( "Image 904 at frame 10.png" );
	CCLayer*	pLayer  = (CCLayer*) this->getParent ( )->getParent ( );
	CCPoint		tHPos = this->getPosition ( );
	CCPoint		tSPos = this->getParent ( )->getPosition ( );
	CCPoint		tPos  = ccp ( tHPos.x + tSPos.x - 80, tHPos.y + tSPos.y + 25 );
	pBullet->setPosition ( tPos );
	pLayer->addChild ( pBullet, 2 );
	pBullet->setTag ( 851137 );
	pBullet->runAction ( CCMoveBy::create ( 1.5f, ccp ( -400, 0 ) ) );
}
