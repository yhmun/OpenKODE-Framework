/* --------------------------------------------------------------------------
 *
 *      File            SMapLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
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
#include "SMapLayer.h"
#include "SLevel.h"

const SIndexPath	SIndexPathZero = { 0, 0 };

inline SIndexPath SIndexPathMake ( KDint nRow, KDint nCol ) 
{
    SIndexPath		tIndexPath;
    tIndexPath.nRow = nRow;
    tIndexPath.nCol = nCol;
    return tIndexPath;
}

inline KDbool SIndexPathEqual ( SIndexPath tIndexPath1, SIndexPath tIndexPath2 )
{
    return ( tIndexPath1.nRow == tIndexPath2.nRow ) && ( tIndexPath1.nCol == tIndexPath2.nCol );
}

inline CCString* CCStringFromIndexPath ( SIndexPath tIndexPath )
{
	return CCString::createWithFormat ( "[%d,%d]", tIndexPath.nRow, tIndexPath.nCol );
}

inline SIndexPath SIndexPathFromCCString ( CCString* pString )
{
    SIndexPath		tIndexPath = SIndexPathMake ( 0, 0 );

	KDchar*		szTemp = kdStrdup ( pString->getCString ( ) );
	CCArray*	pArray = CCArray::create ( );
	KDchar*		szPtr  = kdStrtok ( szTemp, "[,]" );

	do
	{
		pArray->addObject ( CCString::create ( szPtr ) );

	} while ( ( szPtr = kdStrtok ( KD_NULL, "[,]" ) ) );

	kdFree ( szTemp );

	if ( pArray->count ( ) >= 2 )
	{
		tIndexPath.nRow = ((CCString*) pArray->objectAtIndex ( 0 ) )->intValue ( );
		tIndexPath.nCol = ((CCString*) pArray->objectAtIndex ( 1 ) )->intValue ( );
	}

    return tIndexPath;
}

inline CCString* ValueFromArrayAtIndexPath ( CCArray* pArray, SIndexPath tIndexPath ) 
{
    CCString*	pObject = KD_NULL;

	if ( (KDint) pArray->count ( ) > tIndexPath.nRow )
	{
		CCArray*	pTempArray = dynamic_cast<CCArray*> ( pArray->objectAtIndex ( tIndexPath.nRow ) );
		if ( pTempArray ) 
		{
			if ( (KDint) pTempArray->count ( ) > tIndexPath.nCol )
			{
				pObject = (CCString*) pTempArray->objectAtIndex ( tIndexPath.nCol );
			}
		}
	}

	return pObject;
}

inline KDint IntValueFromArrayAtIndexPath ( CCArray* pArray, SIndexPath tIndexPath ) 
{
	CCString*	pObject = ValueFromArrayAtIndexPath ( pArray, tIndexPath );

	return pObject ? pObject->intValue ( ) : -1;
}

inline KDint IntValueFromArrayAtRowAndColumn ( CCArray* pArray, KDint nRow, KDint nCol ) 
{
    return IntValueFromArrayAtIndexPath ( pArray, SIndexPathMake ( nRow, nCol ) );
}

inline KDbool SetArrayAtIndexPath ( CCArray* pArray, SIndexPath tIndexPath, CCString* pNewValue )
{
    KDbool	bResult = KD_FALSE;
	if ( (KDint) pArray->count ( ) > tIndexPath.nRow )
	{
		CCArray*	pTempArray = dynamic_cast<CCArray*> ( pArray->objectAtIndex ( tIndexPath.nRow ) );
		if ( pTempArray ) 
		{
			if ( (KDint) pTempArray->count ( ) > tIndexPath.nCol )
			{
				pTempArray->replaceObjectAtIndex ( tIndexPath.nCol, pNewValue );
				bResult = KD_TRUE;
			}
		}
	}
	return bResult;
}

inline KDbool SetArrayAtRowAndColumn ( CCArray* pArray, KDint nRow, KDint nCol, CCString* pNewValue )
{
    return SetArrayAtIndexPath ( pArray, SIndexPathMake ( nRow, nCol ), pNewValue );
}

SMapLayer::SMapLayer ( KDvoid )
{
	m_pBoxes		= KD_NULL;
	m_pBoxArray		= KD_NULL;
	m_pDestArray	= KD_NULL;
	m_pLevel		= KD_NULL;
	m_pBoxMan		= KD_NULL;
}

SMapLayer::~SMapLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBoxes );
	CC_SAFE_RELEASE ( m_pBoxArray );
	CC_SAFE_RELEASE ( m_pDestArray );
}

KDbool SMapLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_pDelegate		= KD_NULL;
	m_pLevel		= KD_NULL;

	m_pBoxes		= new CCArray ( 5 );
	m_pBoxArray		= new CCArray ( 5 );
	m_pDestArray	= new CCArray ( 5 );
	m_tStart		= ccpz;
	m_tFinish		= ccpz;
	
	return KD_TRUE;
}

KDvoid SMapLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	m_tStart = this->convertTouchToNodeSpace ( pTouch );
}

KDvoid SMapLayer::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{

}

KDvoid SMapLayer::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	m_tFinish = this->convertTouchToNodeSpace ( pTouch );

    SDirection	eDirection = this->directionFromPoint ( m_tStart, m_tFinish );

    if ( eDirection != SDirectionUnknown )
	{
        this->setTouchEnabled ( KD_FALSE );

		m_pBoxMan->stopAllActions ( );
		m_pBoxMan->setDirection ( eDirection );

		SIndexPath	tIndexPath = this->indexPathAtPoint ( m_pBoxMan->getPosition ( ) );
		SMoveType	eMoveType  = this->canMoveWithDirection ( eDirection );
       
        if ( eMoveType == SMoveEnabledWithMan )
		{
			this->pushManWithDirection ( eDirection, tIndexPath );
        }
		else if ( eMoveType == SMoveEnabledWithBoxMan ) 
		{
            this->pushBoxWithDirection ( eDirection, tIndexPath );
        }

		this->scheduleOnce ( schedule_selector ( SMapLayer::enabledTouch ), kMoveDuration );		
    }	
}

KDvoid SMapLayer::loadMapWithLevel ( SLevel* pLevel )
{
	CC_SAFE_RELEASE ( m_pLevel );

    m_pLevel = pLevel;
	m_pLevel->retain ( );

    this->reloadMap ( );
}

KDvoid SMapLayer::reloadMap ( KDvoid )
{
	m_pBoxes->removeAllObjects ( );
	m_pBoxArray->removeAllObjects ( );
	m_pDestArray->removeAllObjects ( );

	if ( m_pBoxMan )
	{
		m_pBoxMan->removeFromParentAndCleanup ( KD_TRUE );
		m_pBoxMan = KD_NULL;
	}

    while ( this->getChildByTag ( 2 ) )
	{
        this->removeChildByTag ( 2, KD_TRUE ); 
    }
    m_pLevel->resetMapElements ( );
    m_pMapElements = m_pLevel->getMapElements ( );
    m_nRowCount = m_pMapElements->count ( );
    if ( m_nRowCount > 0 )
	{
		m_nColumnCount = ( (CCArray*) m_pMapElements->objectAtIndex ( 0 ) )->count ( );	
    }

	this->loadMap ( );
}

KDvoid SMapLayer::loadMap ( KDvoid )
{
    this->setContentSize ( ccs ( kBoxManLength  * m_nColumnCount, kBoxManLength * m_nRowCount ) );

	CCObject*	pObject1;
	CCObject*	pObject2;

	KDint		nIdx1 = 0;
	KDint		nIdx2 = 0;

	CCARRAY_FOREACH ( m_pMapElements, pObject1 )
	{
		CCArray*	pRows = (CCArray*) pObject1;		

		nIdx2 = 0;
		CCARRAY_FOREACH ( pRows, pObject2 )
		{
			KDint		nType = ( (CCString*) pObject2 )->intValue ( );

			CCPoint		tPosition = this->pointAtIndexPath ( SIndexPathMake ( nIdx1, nIdx2 ) );

			CCSprite*	pSprite = CCSprite::create ( "green_road.png" );
            pSprite->setPosition ( tPosition );
            this->addChild ( pSprite, 99, 2 ); 

			switch ( nType )
			{
				case SMapElementRedHouse :
					pSprite = CCSprite::create ( "house_red.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBlueHouse :
					pSprite = CCSprite::create ( "house_blue.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementYellowHouse :
					pSprite = CCSprite::create ( "house_yellow.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBluePoolUp :
					pSprite = CCSprite::create ( "pool_up.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBluePoolDown :
					pSprite = CCSprite::create ( "pool_down.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBluePoolLeft :
					pSprite = CCSprite::create ( "pool_left.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBluePoolRight :
					pSprite = CCSprite::create ( "pool_right.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_5 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementGreenTree :
					pSprite = CCSprite::create ( "green_tree.png" );
					pSprite->setPosition ( ccp ( tPosition.x, tPosition.y + kBoxManLength_4 ) );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementScenryTree :
					pSprite = CCSprite::create ( "scenery_tree.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementGreenShrub :
					pSprite = CCSprite::create ( "green_shrub.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementYellowFlower :
					pSprite = CCSprite::create ( "yellow_flower.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementRedWall :
					pSprite = CCSprite::create ( "red_wall.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 990, 2 );
					break;

				case SMapElementRedBarricade :
					pSprite = CCSprite::create ( "red_barricade.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 991, 2 );
					break;

				case SMapElementBox :
					pSprite = CCSprite::create ( "box.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 992, 2 );
					m_pBoxes->addObject ( pSprite );
					m_pBoxArray->addObject ( CCStringFromIndexPath ( SIndexPathMake ( nIdx1, nIdx1 ) ) );
					break;

				case SMapElementDst :
					pSprite = CCSprite::create ( "balloon.png" );
					pSprite->setPosition ( tPosition );
					this->addChild ( pSprite, 990, 2 );					
					m_pDestArray->addObject ( CCStringFromIndexPath ( SIndexPathMake ( nIdx1, nIdx2 ) ) );
					break;

				case SMapElementMan :
					if ( !m_pBoxMan )
					{
						m_pBoxMan = SBoxMan::create ( tPosition );
						this->addChild ( m_pBoxMan, 993, 2 );
					}
					break;
			}

			nIdx2++;
		}
		
		nIdx1++;
	}
 
	SimpleAudioEngine*		pSE = SimpleAudioEngine::sharedEngine ( );

	if ( pSE->isBackgroundMusicPlaying ( ) )
	{
		pSE->stopBackgroundMusic ( );
	}
    
	pSE->preloadEffect ( m_pLevel->getPushEffect ( ) );
	pSE->preloadEffect ( m_pLevel->getWinEffect  ( ) );
	pSE->preloadEffect ( "start.wav" );

	pSE->playBackgroundMusic ( m_pLevel->getBackgroundMusic ( ) );

	if ( m_pDelegate )
	{
		m_pDelegate->gameDidStart ( );
	}

	this->setTouchEnabled ( KD_TRUE );
}

KDvoid SMapLayer::setDelegate ( SGameDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

SIndexPath SMapLayer::indexPathAtPoint ( const CCPoint& tPoint )
{
    KDint	x = (KDint) ( tPoint.x );
    KDint	y = (KDint) ( m_tContentSize.cy - tPoint.y );
    KDint	nLength = (KDint) kBoxManLength;

    return SIndexPathMake ( y / nLength, x / nLength );
}

CCPoint SMapLayer::pointAtIndexPath ( SIndexPath tIndexPath )
{
    KDfloat		x = tIndexPath.nCol * kBoxManLength;
    KDfloat		y = ( m_nRowCount - tIndexPath.nRow ) * kBoxManLength;

    return ccp ( x, y );
}

SDirection SMapLayer::directionFromPoint ( const CCPoint& tStart, const CCPoint& tFinish )
{
	SDirection		eDirection = SDirectionUnknown;

	if ( !m_tStart.equals ( m_tFinish ) )
	{
		KDfloat		fOffsetX = m_tFinish.x - m_tStart.x;
		KDfloat		fOffsetY = m_tFinish.y - m_tStart.y;
		if ( kdFabsf ( fOffsetX ) >= kdFabsf ( fOffsetY ) )
		{
			if ( fOffsetX > kBoxManLength ) 
			{
				eDirection = SDirectionRight;
			}
			else if ( fOffsetX < -kBoxManLength )
			{
				eDirection = SDirectionLeft;
			}
		} 
		else 
		{
			if ( fOffsetY > kBoxManLength )
			{
				eDirection = SDirectionUp;
			}
			else if ( fOffsetY < -kBoxManLength )
			{
				eDirection = SDirectionDown;
			}
		}
	}
	return eDirection;
}

SMoveType SMapLayer::canMoveWithDirection ( SDirection eDirection )
{
    SMoveType	eType = SMoveDisabled;    
    CCArray*	pArray = m_pMapElements;
	SIndexPath	tIndexPath = this->indexPathAtPoint ( m_pBoxMan->getPosition ( ) );
    
    switch ( eDirection ) 
	{
        case SDirectionUp:
            
            if ( tIndexPath.nRow >= 1 )
			{
                if ( IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow - 1, tIndexPath.nCol ) > SMapElementBox )
				{
                    eType = SMoveEnabledWithMan;
                }
				else if ( tIndexPath.nRow >= 2 && 
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow - 1, tIndexPath.nCol ) == SMapElementBox &&
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow - 2, tIndexPath.nCol )  > SMapElementBox )
				{
                    eType = SMoveEnabledWithBoxMan;
                }
            }
            break;

        case SDirectionDown :
            if ( tIndexPath.nRow <= m_nRowCount - 2 )
			{
				if ( IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow + 1, tIndexPath.nCol ) > SMapElementBox )
				{
                    eType = SMoveEnabledWithMan;
                }
				else if ( tIndexPath.nRow <= m_nRowCount - 3 && 
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow + 1, tIndexPath.nCol ) == SMapElementBox &&
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow + 2, tIndexPath.nCol )  > SMapElementBox )
				{
                    eType = SMoveEnabledWithBoxMan;
                }
            } 
            break;

        case SDirectionLeft:
            if ( tIndexPath.nCol >= 1 )
			{               
                if ( IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol - 1 ) > SMapElementBox )
				{
                    eType = SMoveEnabledWithMan;                   
                }
				else if ( tIndexPath.nCol >= 2 && 
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol - 1 ) == SMapElementBox && 
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol - 2 )  > SMapElementBox )
				{                   
                    eType = SMoveEnabledWithBoxMan;
                }
            }
            break;

        case SDirectionRight :            
            if ( tIndexPath.nCol <= m_nColumnCount - 2 )
			{                
                if ( IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol + 1 ) > SMapElementBox )
				{
                    eType = SMoveEnabledWithMan;
                } 
				else if ( tIndexPath.nCol <= m_nColumnCount - 3 &&
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol + 1 ) == SMapElementBox && 
						  IntValueFromArrayAtRowAndColumn ( pArray, tIndexPath.nRow, tIndexPath.nCol + 2 )  > SMapElementBox )
				{
                    eType = SMoveEnabledWithBoxMan;
                }
            }
            break;

        default :
            break;
    }

    return eType;
}

KDvoid SMapLayer::pushManWithDirection ( SDirection eDirection, SIndexPath tIndexPath )
{
    SIndexPath	tIndexPath1;

	switch ( eDirection )
	{
        case SDirectionUp		:	tIndexPath1 = SIndexPathMake ( tIndexPath.nRow - 1, tIndexPath.nCol );	break;
        case SDirectionDown		:	tIndexPath1 = SIndexPathMake ( tIndexPath.nRow + 1, tIndexPath.nCol );	break;
        case SDirectionLeft		:	tIndexPath1 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol - 1 );	break;
        case SDirectionRight	:	tIndexPath1 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol + 1 );	break;
        default					:	return;
    }
 
    CCPoint		tPosition = this->pointAtIndexPath ( tIndexPath1 );
	m_pBoxMan->runAction ( CCMoveTo::create ( kMoveDuration, tPosition ) );
    
	SetArrayAtIndexPath ( m_pMapElements, tIndexPath , CCString::createWithFormat ( "%d", SMapElementGreenRoad ) );
	SetArrayAtIndexPath ( m_pMapElements, tIndexPath1, CCString::createWithFormat ( "%d", SMapElementMan ) );

	if ( m_pDelegate )
	{
		m_pDelegate->boxManDidMovedWithBox ( KD_FALSE );
	}
}

KDvoid SMapLayer::pushBoxWithDirection ( SDirection eDirection, SIndexPath tIndexPath )
{
	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );
	
	pSE->playEffect ( m_pLevel->getPushEffect ( ) );

	SIndexPath		tIndexPath1;
	SIndexPath		tIndexPath2;

	switch ( eDirection ) 
	{
		case SDirectionUp :
			tIndexPath1 = SIndexPathMake ( tIndexPath.nRow - 1, tIndexPath.nCol );
			tIndexPath2 = SIndexPathMake ( tIndexPath.nRow - 2, tIndexPath.nCol );
			break;

		case SDirectionDown :
			tIndexPath1 = SIndexPathMake ( tIndexPath.nRow + 1, tIndexPath.nCol );
			tIndexPath2 = SIndexPathMake ( tIndexPath.nRow + 2, tIndexPath.nCol );
			break;

		case SDirectionLeft :
			tIndexPath1 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol - 1 );
			tIndexPath2 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol - 2 );
			break;

		case SDirectionRight :
			tIndexPath1 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol + 1 );
			tIndexPath2 = SIndexPathMake ( tIndexPath.nRow, tIndexPath.nCol + 2 );
			break;

		default :
			return;
	}

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBoxes, pObject )
	{
		CCSprite*		pSprite  = (CCSprite*) pObject;
		SIndexPath		tIdxPath = this->indexPathAtPoint ( pSprite->getPosition ( ) );

		if ( SIndexPathEqual ( tIndexPath1, tIdxPath ) ) 
		{
            CCPoint		tManPosition = this->pointAtIndexPath ( tIndexPath1 );
			m_pBoxMan->runAction ( CCMoveTo::create ( kMoveDuration, tManPosition ) );

            CCPoint		tBoxPosition = this->pointAtIndexPath ( tIndexPath2 );          
            pSprite->runAction ( CCMoveTo::create ( kMoveDuration, tBoxPosition ) );

            KDint		nIndex = m_pBoxes->indexOfObject ( pSprite );
            m_pBoxArray->replaceObjectAtIndex ( nIndex, CCStringFromIndexPath ( tIndexPath2 ) );
			break;
		}
	}

	SetArrayAtIndexPath ( m_pMapElements, tIndexPath, CCString::createWithFormat ( "%d", SMapElementGreenRoad ) );
    SetArrayAtIndexPath ( m_pMapElements, tIndexPath1, CCString::createWithFormat ( "%d", SMapElementMan ) );
    SetArrayAtIndexPath ( m_pMapElements, tIndexPath2, CCString::createWithFormat ( "%d", SMapElementBox ) );

	if ( m_pDelegate )
	{
		m_pDelegate->boxManDidMovedWithBox ( KD_TRUE );
	}

	KDbool		bFinish = KD_FALSE;

	CCARRAY_FOREACH ( m_pBoxArray, pObject )
	{
		CCString*		pString = (CCString*) pObject;
		SIndexPath		tIdxP1  = SIndexPathFromCCString ( pString );

		KDbool			bContains = KD_FALSE;
		CCObject*		pObject2;
		CCARRAY_FOREACH ( m_pDestArray, pObject2 )
		{
			CCString*		pString2 = (CCString*) pObject2;
			SIndexPath		tIdxP2   = SIndexPathFromCCString ( pString2 );

			bContains = SIndexPathEqual ( tIdxP1, tIdxP2 );
			if ( bContains )
			{
				break;
			}
		}

		if ( !bContains )
		{
			bFinish = KD_FALSE;
			break;
		}
		else
		{
			bFinish = KD_TRUE;
		}
	}

    if ( bFinish )
	{
        this->setTouchEnabled ( KD_FALSE );
		pSE->stopBackgroundMusic ( );
        
		this->scheduleOnce ( schedule_selector ( SMapLayer::win ), kMoveDuration );
    }	
}

KDvoid SMapLayer::win ( KDfloat fDelta )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( m_pLevel->getWinEffect ( ) );

	if ( m_pDelegate )
	{
		m_pDelegate->gameDidFinish ( );
	}
}

KDvoid SMapLayer::enabledTouch ( KDfloat fDelta )
{
	this->setTouchEnabled ( KD_TRUE );
}
