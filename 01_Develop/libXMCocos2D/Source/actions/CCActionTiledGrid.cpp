/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTiledGrid.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "actions/CCActionTiledGrid.h"
#include "cocoa/CCZone.h"
#include "effects/CCGrid.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
struct Tile
{
	CCPoint		tPosition;
	CCPoint		tStartPosition;
	CCSize		tDelta;
};

//
// implementation of ShakyTiles3D
//

CCShakyTiles3D* CCShakyTiles3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ )
{
	CCShakyTiles3D*  pRet = new CCShakyTiles3D ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, nRange, bShakeZ ) )
	{
		pRet->autorelease ( );
	}		
	else
	{
		CC_SAFE_DELETE ( pRet );	
	}

	return pRet;	
}

KDbool CCShakyTiles3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_nRandrange = nRange;
		m_bShakeZ = bShakeZ;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCShakyTiles3D::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCShakyTiles3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCShakyTiles3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCShakyTiles3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
	
	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_nRandrange, m_bShakeZ );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCShakyTiles3D::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
		{
			ccQuad3  tCoords = originalTile ( ccp ( i, j ) );

			// X
			tCoords.bl.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.br.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.tl.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.tr.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;

			// Y
			tCoords.bl.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.br.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.tl.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			tCoords.tr.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;

			if ( m_bShakeZ )
			{
				tCoords.bl.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.br.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tl.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tr.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
			}
						
			setTile ( ccp ( i, j ), tCoords );
		}
	}
}

//
// implementation of CCShatteredTiles3D
//

CCShatteredTiles3D* CCShatteredTiles3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShatterZ )
{
	CCShatteredTiles3D*  pRet = new CCShatteredTiles3D ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, nRange, bShatterZ ) )
	{		
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );	
	}

	return pRet;	
}

KDbool CCShatteredTiles3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShatterZ )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_bOnce = KD_FALSE;
		m_nRandrange = nRange;
		m_bShatterZ = bShatterZ;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCShatteredTiles3D::copyWithZone ( CCZone* pZone )
{
	CCZone*           pNewZone = KD_NULL;
	CCShatteredTiles3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pCopy = (CCShatteredTiles3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCShatteredTiles3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	// copy super class's member
	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_nRandrange, m_bShatterZ );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCShatteredTiles3D::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	if ( m_bOnce == KD_FALSE )
	{
		for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
		{
			for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
			{
				ccQuad3  tCoords = originalTile ( ccp ( i , j ) );
				
				// X
				tCoords.bl.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.br.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tl.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tr.x += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				
				// Y
				tCoords.bl.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.br.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tl.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				tCoords.tr.y += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;

				if ( m_bShatterZ ) 
				{
					tCoords.bl.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
					tCoords.br.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;				
					tCoords.tl.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
					tCoords.tr.z += ( kdRand ( ) % ( m_nRandrange * 2 ) ) - m_nRandrange;
				}
				
				setTile ( ccp ( i, j ), tCoords );
			}
		}
		
		m_bOnce = KD_TRUE;
	}
}

//
// implementation of CCShuffleTiles
//

CCShuffleTiles* CCShuffleTiles::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed )
{
	CCShuffleTiles*  pRet = new CCShuffleTiles ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uSeed ) )
	{		
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}	

	return pRet;	
}

KDbool CCShuffleTiles::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uSeed         = uSeed;
		m_pTilesOrder   = KD_NULL;
		m_pTiles        = KD_NULL;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCShuffleTiles::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCShuffleTiles*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pCopy = (CCShuffleTiles *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCShuffleTiles ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uSeed );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCShuffleTiles::~CCShuffleTiles( KDvoid )
{
	CC_SAFE_DELETE_ARRAY ( m_pTilesOrder );
	CC_SAFE_DELETE_ARRAY ( m_pTiles );
}

KDvoid CCShuffleTiles::shuffle ( KDuint* pArray, KDuint nLen )
{
	for ( KDint i = nLen - 1; i >= 0; i-- )
	{
		KDuint  j = kdRand ( ) % ( i + 1 );
		KDuint  v = pArray [ i ];

		pArray [ i ] = pArray [ j ];
		pArray [ j ] = v;
	}
}

CCSize CCShuffleTiles::getDelta ( const CCSize& tPos )
{
	CCPoint	 tPos2;
	KDuint   uIndex = (KDuint) ( tPos.cx * m_tGridSize.cy + tPos.cy );

	tPos2.x = (KDfloat) ( m_pTilesOrder [ uIndex ] / (KDint) m_tGridSize.cy );
	tPos2.y = (KDfloat) ( m_pTilesOrder [ uIndex ] % (KDint) m_tGridSize.cy );

	return ccs ( tPos2.x - tPos.cx, tPos2.y - tPos.cy );
}

KDvoid CCShuffleTiles::placeTile ( const CCPoint& tPos, Tile* pTile )
{
	ccQuad3  tCoords = originalTile ( tPos );
	CCPoint    tStep = m_pTarget->getGrid ( )->getStep ( );

	tCoords.bl.x += (KDint) ( pTile->tPosition.x * tStep.x );
	tCoords.bl.y += (KDint) ( pTile->tPosition.y * tStep.y );

	tCoords.br.x += (KDint) ( pTile->tPosition.x * tStep.x );
	tCoords.br.y += (KDint) ( pTile->tPosition.y * tStep.y );

	tCoords.tl.x += (KDint) ( pTile->tPosition.x * tStep.x );
	tCoords.tl.y += (KDint) ( pTile->tPosition.y * tStep.y );

	tCoords.tr.x += (KDint) ( pTile->tPosition.x * tStep.x );
	tCoords.tr.y += (KDint) ( pTile->tPosition.y * tStep.y );

	setTile ( tPos, tCoords );
}

KDvoid CCShuffleTiles::startWithTarget ( CCNode* pTarget )
{
	CCTiledGrid3DAction::startWithTarget ( pTarget );

	if ( m_uSeed != (KDuint) -1 )
	{
		kdSrand ( m_uSeed );
	}

	m_uTilesCount = (KDuint) ( m_tGridSize.cx * m_tGridSize.cy );
	m_pTilesOrder = new KDuint [ m_uTilesCount ];
	
	// Use k to loop. Because m_nTilesCount is unsigned int,
	// and i is used later for int.	
	for ( KDuint k = 0; k < m_uTilesCount; ++k )
	{
		m_pTilesOrder [ k ] = k;
	}

	shuffle ( m_pTilesOrder, m_uTilesCount );

	m_pTiles = (struct Tile *) new Tile[ m_uTilesCount ];
	Tile*  pTiles = (Tile *) m_pTiles;

	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
		{
			pTiles->tPosition = ccp ( i, j );
			pTiles->tStartPosition = ccp ( i, j );
			pTiles->tDelta = getDelta ( ccs ( i, j ) );
			++pTiles;
		}
	}
}

KDvoid CCShuffleTiles::update ( KDfloat fTime )
{
	Tile*  pTiles = (Tile*) m_pTiles;

	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
		{
			pTiles->tPosition = ccpMult ( ccp ( pTiles->tDelta.cx, pTiles->tDelta.cy ), fTime );
			placeTile ( ccp ( i, j ), pTiles );
			++pTiles;
		}
	}
}

//
// implementation of CCFadeOutTRTiles
//

CCFadeOutTRTiles* CCFadeOutTRTiles::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCFadeOutTRTiles*  pRet = new CCFadeOutTRTiles ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
	{	
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;	
}

KDfloat CCFadeOutTRTiles::testFunc ( const CCSize& tPos, KDfloat fTime )
{
	CCPoint  n = ccpMult ( ccp ( m_tGridSize.cx, m_tGridSize.cy ), fTime );

	if ( ( n.x + n.y ) == 0.0f )
	{
		return 1.0f;
	}

	return kdPowf ( ( tPos.cx + tPos.cy ) / ( n.x + n.y ), 6 );
}

KDvoid CCFadeOutTRTiles::turnOnTile ( const CCPoint& tPos )
{
	setTile ( tPos, originalTile ( tPos ) );
}

KDvoid CCFadeOutTRTiles::turnOffTile ( const CCPoint& tPos )
{
	ccQuad3  tCoords;

	kdMemset ( &tCoords, 0, sizeof ( ccQuad3 ) );
	setTile ( tPos, tCoords );
}

KDvoid CCFadeOutTRTiles::transformTile ( const CCPoint& tPos, KDfloat fDistance )
{
	ccQuad3  tCoords = originalTile ( tPos );
	CCPoint    tStep = m_pTarget->getGrid ( )->getStep ( );

	tCoords.bl.x += ( tStep.x / 2 ) * ( 1.0f - fDistance );
	tCoords.bl.y += ( tStep.y / 2 ) * ( 1.0f - fDistance );

	tCoords.br.x -= ( tStep.x / 2 ) * ( 1.0f - fDistance );
	tCoords.br.y += ( tStep.y / 2 ) * ( 1.0f - fDistance );

	tCoords.tl.x += ( tStep.x / 2 ) * ( 1.0f - fDistance );
	tCoords.tl.y -= ( tStep.y / 2 ) * ( 1.0f - fDistance );

	tCoords.tr.x -= ( tStep.x / 2 ) * ( 1.0f - fDistance );
	tCoords.tr.y -= ( tStep.y / 2 ) * ( 1.0f - fDistance );

	setTile ( tPos, tCoords );
}

KDvoid CCFadeOutTRTiles::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
		{
			KDfloat  fDistance = testFunc ( ccp ( i, j ), fTime );

			if ( fDistance == 0 )
			{
				turnOffTile ( ccp ( i, j ) );
			} 
			else if ( fDistance < 1 )
			{
				transformTile ( ccp ( i, j ), fDistance );
			}
			else
			{
				turnOnTile ( ccp( i, j ) );
			}
		}
	}
}


//
// implementation of CCFadeOutBLTiles
//

CCFadeOutBLTiles* CCFadeOutBLTiles::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCFadeOutBLTiles*  pRet = new CCFadeOutBLTiles ( );
		
	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDfloat CCFadeOutBLTiles::testFunc ( const CCSize& tPos, KDfloat fTime )
{
	CCPoint  n = ccpMult ( ccp ( m_tGridSize.cx, m_tGridSize.cy ), ( 1.0f - fTime ) );

	if ( ( n.x + n.y ) == 0 )
	{
		return 1.0f;
	}

	return kdPowf ( ( n.x + n.y ) / ( tPos.cx + tPos.cy ), 6 );
}


//
// implementation of CCFadeOutUpTiles
//

CCFadeOutUpTiles* CCFadeOutUpTiles::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCFadeOutUpTiles*  pRet = new CCFadeOutUpTiles ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDfloat CCFadeOutUpTiles::testFunc ( const CCSize& tPos, KDfloat fTime )
{
	CCPoint  n = ccpMult ( ccp ( m_tGridSize.cx, m_tGridSize.cy ), fTime );

	if ( n.y == 0.0f )
	{
		return 1.0f;
	}

	return kdPowf ( tPos.cy / n.y, 6 );
}

KDvoid CCFadeOutUpTiles::transformTile ( const CCPoint& tPos, KDfloat fDistance )
{
	ccQuad3  tCoords = originalTile ( tPos );
	CCPoint    tStep = m_pTarget->getGrid ( )->getStep ( );

	tCoords.bl.y += ( tStep.y / 2 ) * ( 1.0f - fDistance );
	tCoords.br.y += ( tStep.y / 2 ) * ( 1.0f - fDistance );
	tCoords.tl.y -= ( tStep.y / 2 ) * ( 1.0f - fDistance );
	tCoords.tr.y -= ( tStep.y / 2 ) * ( 1.0f - fDistance );

	setTile ( tPos, tCoords );
}


//
// implementation of CCFadeOutDownTiles
//

CCFadeOutDownTiles* CCFadeOutDownTiles::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCFadeOutDownTiles*  pRet = new CCFadeOutDownTiles ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDfloat CCFadeOutDownTiles::testFunc ( const CCSize& tPos, KDfloat fTime )
{
	CCPoint  n = ccpMult ( ccp ( m_tGridSize.cx, m_tGridSize.cy ), ( 1.0f - fTime ) );

	if ( n.y == 0 )
	{
		return 1;
	}

	return kdPowf ( n.y / tPos.cy, 6 );
}


//
// implementation of TurnOffTiles
//

CCTurnOffTiles* CCTurnOffTiles::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCTurnOffTiles*  pRet = new CCTurnOffTiles ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, 0 ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCTurnOffTiles* CCTurnOffTiles::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed )
{
	CCTurnOffTiles*  pRet = new CCTurnOffTiles ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uSeed ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTurnOffTiles::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uSeed = uSeed;
		m_pTilesOrder = KD_NULL;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCTurnOffTiles::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCTurnOffTiles*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pCopy = (CCTurnOffTiles*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCTurnOffTiles ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uSeed );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCTurnOffTiles::~CCTurnOffTiles ( KDvoid )
{
	CC_SAFE_DELETE_ARRAY ( m_pTilesOrder );
}

KDvoid CCTurnOffTiles::shuffle ( KDuint* pArray, KDuint nLen )
{
	for ( KDint i = nLen - 1; i >= 0; i-- )
	{
		KDuint  j = kdRand ( ) % ( i + 1 );
		KDuint  v = pArray [ i ];

		pArray [ i ] = pArray [ j ];
		pArray [ j ] = v;
	}
}

KDvoid CCTurnOffTiles::turnOnTile ( const CCPoint& tPos )
{
	setTile ( tPos, originalTile ( tPos ) );
}

KDvoid CCTurnOffTiles::turnOffTile ( const CCPoint& tPos )
{
	ccQuad3  tCoords;

	kdMemset ( &tCoords, 0, sizeof ( ccQuad3 ) );
	setTile ( tPos, tCoords );
}

KDvoid CCTurnOffTiles::startWithTarget ( CCNode* pTarget )
{
	CCTiledGrid3DAction::startWithTarget ( pTarget );

	if ( m_uSeed != (KDuint) -1 )
	{
		kdSrand ( m_uSeed );
	}

	m_uTilesCount = (KDuint) ( m_tGridSize.cx * m_tGridSize.cy );
	m_pTilesOrder = new KDuint [ m_uTilesCount ];

	for ( KDuint i = 0; i < m_uTilesCount; ++i )
	{
		m_pTilesOrder [ i ] = i;
	}

	shuffle ( m_pTilesOrder, m_uTilesCount );
}

KDvoid CCTurnOffTiles::update ( KDfloat fTime )
{
	KDuint  i, l, t;

    l = (KDuint) ( fTime * m_uTilesCount );

	for ( i = 0; i < m_uTilesCount; i++ )
	{
		t = m_pTilesOrder[ i ];
		CCPoint  tTilePos = ccp ( (KDuint) ( t / m_tGridSize.cy ), t % (KDuint) m_tGridSize.cy );

		if ( i < l )
		{
			turnOffTile ( tTilePos );
		}
		else
		{
			turnOnTile ( tTilePos );
		}
	}
}


//
// implementation of CCWavesTiles3D
//

CCWavesTiles3D* CCWavesTiles3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	CCWavesTiles3D*  pRet = new CCWavesTiles3D ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uWaves, fAmplitude ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}	

	return pRet;
}

KDbool CCWavesTiles3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uWaves = uWaves;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCWavesTiles3D::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCWavesTiles3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pCopy = (CCWavesTiles3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCWavesTiles3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uWaves, m_fAmplitude );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCWavesTiles3D::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; i++ )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; j++ )
		{
			ccQuad3  tCoords = originalTile ( ccp ( i, j ) );

			tCoords.bl.z = ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + ( tCoords.bl.y + tCoords.bl.x ) * 0.01f ) * m_fAmplitude * m_fAmplitudeRate );
			tCoords.br.z = tCoords.bl.z;
			tCoords.tl.z = tCoords.bl.z;
			tCoords.tr.z = tCoords.bl.z;

			setTile ( ccp ( i, j ), tCoords );
		}
	}
}

//
// implementation of CCJumpTiles3D
//

CCJumpTiles3D* CCJumpTiles3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uJumps, KDfloat fAmplitude )
{
	CCJumpTiles3D*  pRet = new CCJumpTiles3D ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uJumps, fAmplitude ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCJumpTiles3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uJumps, KDfloat fAmplitude )
{
	if ( CCTiledGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uJumps = uJumps;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCJumpTiles3D::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	CCJumpTiles3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pCopy = (CCJumpTiles3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCJumpTiles3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uJumps, m_fAmplitude );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCJumpTiles3D::update ( KDfloat fTime )
{
	KDfloat  fSinZ  = ( kdSinf ( KD_PI_F * ( fTime * m_uJumps * 2 + 0 ) ) * m_fAmplitude * m_fAmplitudeRate );
	KDfloat  fSinZ2 = ( kdSinf ( KD_PI_F * ( fTime * m_uJumps * 2 + 1 ) ) * m_fAmplitude * m_fAmplitudeRate );

	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; i++ )
	{
		for ( KDint j = 0; j < (KDint) m_tGridSize.cy; j++ )
		{
			ccQuad3  tCoords = originalTile ( ccp ( i, j ) );

			if ( ( ( i + j ) % 2 ) == 0 )
			{
				tCoords.bl.z += fSinZ;
				tCoords.br.z += fSinZ;
				tCoords.tl.z += fSinZ;
				tCoords.tr.z += fSinZ;
			}
			else
			{
				tCoords.bl.z += fSinZ2;
				tCoords.br.z += fSinZ2;
				tCoords.tl.z += fSinZ2;
				tCoords.tr.z += fSinZ2;
			}

			setTile ( ccp ( i, j ), tCoords );
		}
	}
}


//
// implementation of CCSplitRows
//

CCSplitRows* CCSplitRows::create ( KDfloat fDuration, KDuint uRows )
{
	CCSplitRows*  pRet = new CCSplitRows ( );

	if ( pRet && pRet->initWithDuration ( fDuration, uRows ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCSplitRows::initWithDuration ( KDfloat fDuration, KDuint uRows )
{
	m_uRows = uRows;

	return CCTiledGrid3DAction::initWithDuration ( fDuration, ccp ( 1, uRows ) );
}

CCObject* CCSplitRows::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCSplitRows*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pCopy = (CCSplitRows *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCSplitRows ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_uRows );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCSplitRows::startWithTarget ( CCNode* pTarget )
{
	CCTiledGrid3DAction::startWithTarget ( pTarget );

	m_tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
}

KDvoid CCSplitRows::update ( KDfloat fTime )
{
	for ( KDint j = 0; j < (KDint) m_tGridSize.cy; ++j )
	{
		ccQuad3  tCoords = originalTile ( ccp ( 0, j ) );
		KDfloat  fDirection = 1;

		if ( ( j % 2 ) == 0 )
		{
			fDirection = -1;
		}

		tCoords.bl.x += fDirection * m_tWinSize.cx * fTime;
		tCoords.br.x += fDirection * m_tWinSize.cx * fTime;
		tCoords.tl.x += fDirection * m_tWinSize.cx * fTime;
		tCoords.tr.x += fDirection * m_tWinSize.cx * fTime;

		setTile ( ccp ( 0, j ), tCoords );
	}
}


//
// implementation of CCSplitCols
//

CCSplitCols* CCSplitCols::create ( KDfloat fDuration, KDuint uCols )
{
	CCSplitCols*  pRet = new CCSplitCols ( );

	if ( pRet && pRet->initWithDuration ( fDuration, uCols ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCSplitCols::initWithDuration ( KDfloat fDuration, KDuint uCols )
{
	m_uCols = uCols;

	return CCTiledGrid3DAction::initWithDuration ( fDuration, ccp ( uCols, 1 ) );
}

CCObject* CCSplitCols::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCSplitCols*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pCopy = (CCSplitCols *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCSplitCols ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCTiledGrid3DAction::copyWithZone ( pZone );
	pCopy->initWithDuration ( m_fDuration, m_uCols );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCSplitCols::startWithTarget ( CCNode* pTarget )
{
	CCTiledGrid3DAction::startWithTarget ( pTarget );

	m_tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
}

KDvoid CCSplitCols::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) m_tGridSize.cx; ++i )
	{
		ccQuad3  tCoords = originalTile ( ccp ( i, 0 ) );
		KDfloat	 fDirection = 1;

		if ( ( i % 2 ) == 0 )
		{
			fDirection = -1;
		}

		tCoords.bl.y += fDirection * m_tWinSize.cy * fTime;
		tCoords.br.y += fDirection * m_tWinSize.cy * fTime;
		tCoords.tl.y += fDirection * m_tWinSize.cy * fTime;
		tCoords.tr.y += fDirection * m_tWinSize.cy * fTime;

		setTile ( ccp ( i, 0 ), tCoords );
	}
}
 
NS_CC_END
