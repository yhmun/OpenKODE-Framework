/* --------------------------------------------------------------------------
 *
 *      File            Ch5_Minimap.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch5_Minimap.h"

KDbool Ch5_Minimap::init ( KDvoid )
{	
	// Initialize the Minimap object
	m_pMinimap = Minimap::create ( );
	m_pMinimap->setPosition ( ccp ( 300, 140 ) );
	this->addChild ( m_pMinimap, 10 );

	// Run our top-down isometric game recipe
	if ( !Ch4_TopDownIsometric::init ( ) )
	{
		return KD_FALSE;
	}
	
	// Add trees as static objects
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pTrees, pObject )
	{
		GameObject*		pTree = (GameObject*) pObject;
		b2Vec2			tBodyPos = pTree->getBody ( )->GetPosition ( );

		m_pMinimap->addStaticObject ( ccp ( tBodyPos.x, tBodyPos.y ) );
	}
	
	return KD_TRUE;
}

KDvoid Ch5_Minimap::step ( KDfloat fDelta )
{
	Ch4_TopDownIsometric::step ( fDelta );
	
	// Set the actor position
	b2Vec2		tGunmanPos = m_pGunman->getBody ( )->GetPosition ( );
	m_pMinimap->setActor ( ccp ( tGunmanPos.x, tGunmanPos.y ) );
	
	// Set individual projectile positions
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pBalls, pObject )
	{
		GameObject*		pBall = (GameObject*) pObject;
		b2Vec2			tBallPos = pBall->getBody ( )->GetPosition ( );

		m_pMinimap->setProjectile ( ccp ( tBallPos.x, tBallPos.y ), ccszf ( "%d", pBall->getTag ( ) ) );
	}
}

// We overload this method to automatically add walls to the minimap
KDvoid Ch5_Minimap::addBrickWallFrom ( CCPoint tPoint1, CCPoint tPoint2, KDfloat fHeight )
{
	// Convert wall vertex positions to the properly scaled Box2D coordinates
	CCPoint		tVert1 = ccp ( tPoint1.x / PTM_RATIO, tPoint1.y / PTM_RATIO / PERSPECTIVE_RATIO );
	CCPoint		tVert2 = ccp ( tPoint2.x / PTM_RATIO, tPoint2.y / PTM_RATIO / PERSPECTIVE_RATIO );
	
	// Add both wall vertices
	m_pMinimap->addWallWithVertex1 ( tVert1, tVert2 ); 

	Ch4_TopDownIsometric::addBrickWallFrom ( tPoint1, tPoint2, fHeight );
}