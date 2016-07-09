/* --------------------------------------------------------------------------
 *
 *      File            Attacker.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 05/05/2011
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
#include "Attacker.h"
#include "Player.h"
#include "GameScene.h"
#include "Pathfinding.h"

Attacker::Attacker ( KDvoid )
{
	m_pPath = KD_NULL;
}

Attacker::~Attacker ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPath );
}

KDbool Attacker::init ( KDvoid )
{
	if ( !GameSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_bAlive					= KD_TRUE;
	m_nOrderOffsetZ				= 0;
	m_fVelocity					= 50.f + GameMgr->getLevel ( ) * 10;
	m_bChasingPlayer			= KD_FALSE;
	m_bFollowingPath			= KD_FALSE;
//	m_pThread					= KD_NULL;
	m_sSpritesheetBaseFilename	= "ghost";

	this->setPath ( CCArray::create ( ) );

	this->cacheFrames ( );

	return KD_TRUE;
}

KDvoid Attacker::spriteMoveFinished ( CCNode* pSender )
{
	if ( m_bFollowingPath )
	{
		if ( m_pPath->count ( ) > 1 )
		{
			CCLOG ( "Continuing along path, %d tiles left on path", m_pPath->count ( ) );
			this->followPath ( );
		} 
		else 
		{
			CCLOG ( "Reached end of path" );
			this->stopAllActions ( );

			// create another path
			this->createPathToPlayer ( );			
		}
	} 
	else 
	{
		this->stopAllActions ( );
		this->setMoving ( KD_FALSE );
		this->setChasingPlayer ( KD_FALSE );
	}
}

KDvoid Attacker::createPathToPlayer ( KDvoid )
{
	if ( m_bAlive && m_bFollowingPath )
	{		
		Player*		pPlayer = ( (KnightFight*) GameMgr->getGameScene ( ) )->getPlayer ( );

		CCPoint		tAttackerTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( this->getPosition ( ) );
		CCPoint		tPlayerTilePos   = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( pPlayer->getPosition ( ) );
		CCLOG ( "Creating path to player" );
		CCLOG ( "Attacker at %f %f", tAttackerTilePos.x, tAttackerTilePos.y );
		CCLOG ( "Player at %f %f", tPlayerTilePos.x, tPlayerTilePos.y );

		CCArray*	pTilePositions = CCArray::create 
		(
			CCPointValue::create ( tPlayerTilePos ), 
			CCPointValue::create ( tAttackerTilePos ), 
			KD_NULL 
		);
		/*	
		[self.thread release];
		self.thread = [[NSThread alloc] initWithTarget:self
												 selector:@selector(getPath:)
												   object:tilePositions];
		[self.thread setThreadPriority:0.0];
		[self.thread start];
		
		//[self performSelectorInBackground:@selector(getPath:) withObject:tilePositions];
		*/

		this->getPath ( pTilePositions );

		CCLOG ( "Following new path" );
	} 
	else 
	{
		CCLOG ( "Not creating path to player, because the attacker is dead or not following the player" );
	}
}

KDvoid Attacker::chasePlayer ( GameSprite* pPlayer )
{	
	if ( !m_bChasingPlayer && !m_bFollowingPath && GameMgr->isSoundOn ( ) )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "ghostbirth.wav" );
	}
	
	if ( m_bAlive )
	{		
		CCLOG ( "Chasing player" );
		CCLOG ( "Player at %f %f", pPlayer->getPosition ( ).x, pPlayer->getPosition ( ).y );
		CCLOG ( "Attacker at %f %f", this->getPosition ( ).x, this->getPosition ( ).y );

		this->moveSpritePosition ( pPlayer->getPosition ( ), this );

		m_bChasingPlayer = KD_TRUE;
	}	
}

KDvoid Attacker::updateAStarPath ( KDfloat fDelta )
{
	this->stopAllActions ( );
	this->createPathToPlayer ( );
}

KDvoid Attacker::followPath ( KDvoid )
{
	KDint	nNumberOfSquares = m_pPath->count ( );
	if ( nNumberOfSquares ) 
	{		
		// snake along the squares
		KDint				nPathIndex = m_pPath->count ( ) - 2;		// ignore the last position, it's the attacker's current square
		PathfindingNode*	pNode = (PathfindingNode*) m_pPath->objectAtIndex ( nPathIndex );
		m_pPath->removeLastObject ( );
				
		CCLOG ( "Moving sprite to tile pos %f %f", pNode->getTilePos ( ).x, pNode->getTilePos ( ).y );
	
		CCPoint		tNextPos = GameMgr->getCoordinateFunctions ( )->locationFromTilePos ( pNode->getTilePos ( ) );
		tNextPos = GameMgr->getCoordinateFunctions ( )->pointRelativeToCentreFromLocation ( tNextPos );

		CCLOG ( "Moving sprite to %f %f", tNextPos.x, tNextPos.y );
	
		this->moveSpritePosition ( tNextPos, this );		
	}
}

KDvoid Attacker::getPath ( CCArray* pTilePositions )
{
	CCLOG ( "in thread - getPath" );
//	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	CCPoint		tPlayerTilePos   = ( (CCPointValue*) pTilePositions->objectAtIndex ( 0 ) )->getValue ( );
	CCPoint		tAttackerTilePos = ( (CCPointValue*) pTilePositions->objectAtIndex ( 1 ) )->getValue ( );

	Pathfinding*	pPathfinding = Pathfinding::create ( );
	CCLOG ( "about to get path" );
	CCArray*		pReturnedPath = pPathfinding->search ( tAttackerTilePos, tPlayerTilePos );
	CCLOG ( "got path back, with %d nodes", pReturnedPath->count ( ) );
	
	this->setPathToAttacker ( pReturnedPath );
//	[self performSelectorOnMainThread:@selector(setPathToAttacker:) withObject:returnedPath waitUntilDone:NO];
	CCLOG ( "End of thread - draining pool" );
//	[pool drain];
	CCLOG ( "End of thread - pool drained" );	
}

KDvoid Attacker::setPathToAttacker ( CCArray* pAttackerPath )
{
	this->setPath ( pAttackerPath );
	this->followPath ( );
//	[self.thread release];
//	self.thread = nil;
}
