/* --------------------------------------------------------------------------
 *
 *      File            GameSprite.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 03/05/2011
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
#include "GameSprite.h"
#include "Knight.h"
#include "Attacker.h"
#include "Player.h"
#include "GameScene.h"

static const KDint		l_nMaxSight = 400;

GameSprite::GameSprite ( KDvoid )
{
	m_pAnimation		= KD_NULL;
	m_pSpriteRunAction	= KD_NULL;
	m_pSpriteSheet		= KD_NULL;
}

GameSprite::~GameSprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAnimation );
	CC_SAFE_RELEASE ( m_pSpriteRunAction );
	CC_SAFE_RELEASE ( m_pSpriteSheet );
}

KDbool GameSprite::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_nDeathTurns = 0;

	return KD_TRUE;
}

CCPoint GameSprite::getLocation ( KDvoid )
{
	return ccp ( this->getPosition ( ).x, this->getPosition ( ).y - this->getContentSize ( ).cy / 4 );
}

KDvoid GameSprite::changeSpriteAnimation ( const KDchar* szDirection )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCSpriteFrame*			pFrame = pCache->spriteFrameByName ( ccszf ( "%s%s-1.png", m_sSpritesheetBaseFilename.c_str ( ), szDirection ) );
	
	this->setDisplayFrame ( pFrame );

	CCArray*				pAnimFrames = CCArray::create ( );
	KDint					nFrames = 7;

	if ( dynamic_cast <Knight*> ( this ) )
	{
		nFrames = 11;
	}
	
	CCLOG ( "Frames for %p = %d", this, nFrames );

	for ( KDint i = 0; i <= nFrames; i++ )
	{
		CCSpriteFrame*		pFrame = pCache->spriteFrameByName ( ccszf ( "%s%s-%d.png", m_sSpritesheetBaseFilename.c_str ( ), szDirection, i ) );
		pAnimFrames->addObject ( pFrame );
	}

	this->setAnimation ( CCAnimation::createWithSpriteFrames ( pAnimFrames, 0.1f ) );
}

KDvoid GameSprite::moveSpritePosition ( const CCPoint& tTargetPosition, CCObject* pSender )
{
	CCPoint		tSpritePos = this->getPosition ( );

	KDfloat		fAngle = GameMgr->getCoordinateFunctions ( )->getAngleBetweenPoints ( tSpritePos, tTargetPosition );

	KDint		nAngleInt = (KDint) ( fAngle / 22.5f );

	switch ( nAngleInt ) 
	{
		case  0 :	this->changeSpriteAnimation ( "W"  );	break;
		case  1 :	this->changeSpriteAnimation ( "SW" );	break;
		case  2 :	this->changeSpriteAnimation ( "SW" );	break;
		case  3 :	this->changeSpriteAnimation ( "S"  );	break;
		case  4 :	this->changeSpriteAnimation ( "S"  );	break;
		case  5 :	this->changeSpriteAnimation ( "SE" );	break;
		case  6 :	this->changeSpriteAnimation ( "SE" );	break;
		case  7 :	this->changeSpriteAnimation ( "E"  );	break;
		case  8 :	this->changeSpriteAnimation ( "E"  );	break;
		case  9 :	this->changeSpriteAnimation ( "NE" );	break;
		case 10 :	this->changeSpriteAnimation ( "NE" );	break;
		case 11 :	this->changeSpriteAnimation ( "N"  );	break;
		case 12 :	this->changeSpriteAnimation ( "N"  );	break;
		case 13 :	this->changeSpriteAnimation ( "NW" );	break;
		case 14 :	this->changeSpriteAnimation ( "NW" );	break;
		case 15 :	this->changeSpriteAnimation ( "W"  );	break;
	}

	CCPoint		tDiff = ccpSub ( tTargetPosition, tSpritePos );	
	KDfloat		fLengthOfMovement = kdSqrtf ( kdPowf ( tDiff.x, 2 ) + kdPowf ( tDiff.y, 2 ) );
	KDfloat		fSpriteVelocity   = this->getVelocity ( );
	KDfloat		fRealMoveDuration = fLengthOfMovement / fSpriteVelocity;

	this->setMoving ( KD_TRUE );

	CCLOG ( "moving sprite by %f %f for duration %f", tDiff.x, tDiff.y, fRealMoveDuration );

	this->stopAllActions ( );	// don't let old move continue - otherwise the player will stop at old target & start skating!

	this->runAction ( CCRepeatForever::create ( CCAnimate::create ( m_pAnimation ) ) );

	this->setSpriteRunAction 
	(
		CCSequence::create 
		(
			CCMoveBy::create ( fRealMoveDuration, tDiff ), 
			CCCallFuncN::create ( this, callfuncN_selector ( GameSprite::spriteMoveFinished ) ),
			KD_NULL 
		) 
	);
	this->runAction ( m_pSpriteRunAction );
}

KDvoid GameSprite::updateVertexZ ( const CCPoint& tTilePos, CCTMXTiledMap* pTileMap )
{
	KDfloat			fLowestZ  = -( pTileMap->getMapSize ( ).cx + pTileMap->getMapSize ( ).cy );
	KDfloat			fCurrentZ = tTilePos.x + tTilePos.y;
	
	this->setVertexZ ( fLowestZ + fCurrentZ + this->getOrderOffsetZ ( ) );
}

KDbool GameSprite::checkIfPointIsInSight ( const CCPoint& tTargetPos, GameSprite* pEnemy )
{
	CCPoint		tAttackerPos;
	if ( dynamic_cast <Knight*> ( pEnemy ) )
	{
		tAttackerPos = pEnemy->getPosition ( );
	} 
	else 
	{
		tAttackerPos = pEnemy->getLocation ( );
	}
	
	CCPoint		tDiff = ccpSub ( tTargetPos, tAttackerPos );
	if ( ( kdFabsf ( tDiff.x ) > l_nMaxSight ) || kdFabsf ( tDiff.y ) > l_nMaxSight )
	{
		return KD_FALSE;
	}

	CCArray*	pPoints = CCArray::create ( );
	this->getPointsOnLine ( (KDint) tAttackerPos.x, (KDint) tAttackerPos.y, (KDint) tAttackerPos.x, (KDint) tAttackerPos.y, pPoints );

	KDbool		bLineOfSight = KD_TRUE;
	for ( KDuint i = 0; i < pPoints->count ( ); i++ )
	{
		CCPoint		tThisPoint = ( (CCPointValue*) pPoints->objectAtIndex ( i ) )->getValue ( );
		CCPoint		tThisTile = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( tThisPoint );
		if ( GameMgr->getCoordinateFunctions ( )->isTilePosBlocked ( tThisTile ) )
		{
			bLineOfSight = KD_FALSE;
		}
	}

	return bLineOfSight;
}

KDvoid GameSprite::deathSequence ( KDvoid )
{
	m_bAlive = KD_FALSE;
	
	if ( dynamic_cast <Attacker*> ( this ) && GameMgr->isSoundOn ( ) )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "ghostdeath.wav" );
	}
	this->stopAllActions ( );
	this->schedule ( schedule_selector ( GameSprite::deathSpasm ), 0.1f ); 
}

KDvoid GameSprite::cacheFrames ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	const KDchar*			aDirections [ ] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW", KD_NULL };

	for ( KDint i = 0; aDirections [ i ] != KD_NULL; i++ )
	{
		const KDchar*	szPlistFilename = ccszf ( "%s%s.plist", m_sSpritesheetBaseFilename.c_str ( ), aDirections [ i ] );

		pCache->addSpriteFramesWithFile ( szPlistFilename );
		CCLOG ( "Added %s to the cache", szPlistFilename );
	}
}

KDvoid GameSprite::spriteMoveFinished ( CCNode* pSender )
{
	CCLOG ( "%p stopped moving", this );

	CCPoint		tTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( this->getPosition ( ) );
	CCLOG ( "Tile position %f %f", tTilePos.x, tTilePos.y );

	this->setMoving ( KD_FALSE );
	this->stopAllActions ( );
}

KDvoid GameSprite::deathSpasm ( KDfloat fDelta )
{
	const KDchar*	aDirections [ ] = { "N", "NW", "W", "SW", "S", "SE", "E", "NE" };

	this->changeSpriteAnimation ( aDirections [ m_nDeathTurns % 8 ] );
	
	m_nDeathTurns++;
	if ( m_nDeathTurns > 16 )
	{
		this->unschedule ( schedule_selector ( GameSprite::deathSpasm ) );

		if ( dynamic_cast <Player*> ( this ) )
		{
			CCLOG ( "Player died" );
		
			this->setVisible ( KD_FALSE );

			( (KnightFight*) this->getParent ( ) )->loseLife  ( );
			( (KnightFight*) this->getParent ( ) )->resetGame ( );
		} 
		else 
		{
			CCLOG ( "%p died, removing from parent", this );
			this->getParent ( )->removeChild ( this, KD_TRUE );			
		}
	}
}

/////////////////
//
// Bresenenham line drawing algorithm.
// ref: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
//
KDvoid GameSprite::getPointsOnLine ( KDint nX0, KDint nY0, KDint nX1, KDint nY1, CCArray* pPointsArray )
{
	KDint	nPointsOnLineGranularity = 1;
	
	KDint	nDx = nX1 - nX0; 
	KDint	nDy = nY1 - nY0;
	KDint	nSteep = ( kdAbs ( nDy ) >= kdAbs ( nDx ) );
	if ( nSteep )
	{
		// swap x and y values
		KDint	nTemp = nX0;
		nX0 = nY0;
		nY0 = nTemp;
		nTemp = nX1;
		nX1 = nY1;
		nY1 = nTemp;
		// recompute Dx, Dy after swap
		nDx = nX1 - nX0;
		nDy = nY1 - nY0;
	}
	KDint	nStepX = nPointsOnLineGranularity;
	if ( nDx < 0 )
	{
		nStepX = -nPointsOnLineGranularity;
		nDx = -nDx;
	}
	KDint	nStepY = nPointsOnLineGranularity;
	if ( nDy < 0 ) 
	{
		nStepY = -nPointsOnLineGranularity;		
		nDy = -nDy; 
	}
	KDint	nTwoDy = 2 * nDy; 
	KDint	nTwoDyTwoDx = nTwoDy - 2 * nDx;		// 2 * nDy - 2 * nDx
	KDint	nE = nTwoDy - nDx;					// 2 * nDy - nDx
	KDint	nY = nY0;
	KDint	nDrawX, nDrawY;	
	KDint	nX = nX0;
	while ( kdAbs ( nX - nX1 ) > nPointsOnLineGranularity )
	{
		nX += nStepX;
		
		if ( nSteep )
		{			
			nDrawX = nY;
			nDrawY = nX;
		}
		else 
		{			
			nDrawX = nX;
			nDrawY = nY;
		}

		// Add point to arrary.
		pPointsArray->addObject ( CCPointValue::create ( ccp ( nDrawX, nDrawY ) ) );
		
		// next
		if ( nE > 0 )
		{
			nE += nTwoDyTwoDx;					// nE += 2 * nDy - 2 * nDx;
			nY = nY + nStepY;
		} 
		else 
		{
			nE += nTwoDy;						// nE += 2 * nDy;
		}
	}
}
