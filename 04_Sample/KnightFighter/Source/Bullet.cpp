/* --------------------------------------------------------------------------
 *
 *      File            Bullet.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 06/05/2011
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
#include "Bullet.h"
#include "Player.h"

KDbool Bullet::init ( KDvoid )
{
	if ( !GameSprite::init ( ) )
	{
		return KD_FALSE;
	}

	this->initWithFile ( "bullet.png" );

	m_nOrderOffsetZ	= 2;
	m_fVelocity		= 360.f;

	this->setVisible ( KD_FALSE );

	return KD_TRUE;
}

KDvoid Bullet::shootBullet ( const CCPoint& tTargetPosition, Player* pPlayer )
{
	CCPoint		tPlayerPosition = pPlayer->getPosition ( );
	this->setVisible ( KD_TRUE );

	KDint		nRealX, nRealY;
	CCPoint		tDiff = ccpSub ( tTargetPosition, tPlayerPosition );

	CCLOG ( "Diff x = %f", tDiff.x );
	
	if ( tDiff.x > 0 ) 
	{
		nRealX = (KDint) ( ( GameMgr->getTileMap ( )->getMapSize ( ).cx * GameMgr->getTileMap ( )->getTileSize ( ).cx ) + this->getContentSize ( ).cx / 2 );
	}
	else 
	{
		nRealX = (KDint) ( -( GameMgr->getTileMap ( )->getMapSize ( ).cx * GameMgr->getTileMap ( )->getTileSize ( ).cx ) - this->getContentSize ( ).cx / 2 );
	}

	KDfloat		fRatio = tDiff.y / tDiff.x;
	nRealY = (KDint) ( ( ( nRealX - tPlayerPosition.x ) * fRatio ) + tPlayerPosition.y );

	CCPoint		tRealDest = ccp ( nRealX, nRealY );

	// determine length
	KDint		nOffRealX = (KDint) ( nRealX - tPlayerPosition.x );
	KDint		nOffRealY = (KDint) ( nRealY - tPlayerPosition.y );
	KDfloat		fLength = kdSqrtf ( (KDfloat) ( nOffRealX * nOffRealX ) + ( nOffRealY * nOffRealY ) );
	KDfloat		fRealMoveDuration = fLength / m_fVelocity;	
	
	this->setMoving ( KD_TRUE );

	this->stopAllActions ( );		// don't let old move continue - otherwise the player will stop at old target & start skating!

	this->setSpriteRunAction 
	(
		CCSequence::create 
		(
			CCMoveBy::create ( fRealMoveDuration, tRealDest ),
			CCCallFuncN::create ( this, callfuncN_selector ( Bullet::spriteMoveFinished ) ),
			KD_NULL 
		)
	);

	this->runAction ( m_pSpriteRunAction );
}
