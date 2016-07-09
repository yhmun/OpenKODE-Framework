/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/10
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
#include "GameScene.h"
#include "OverScene.h"

KDbool GameScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*			pBg = CCSprite::create ( "bg.png" );
	pBg->setAnchorPoint ( ccp ( 0, 0 ) );
	pBg->setPosition ( ccp ( -43, 0 ) );
	this->addChild ( pBg, -1 ); 

	m_pSheet = CCSpriteBatchNode::create ( "bubble2.png", 50 ); 
	this->addChild ( m_pSheet, 0, 100 );

	m_pBubbles		= new CCArray ( 70 );	
	m_pRemain		= new CCArray ( 70 );

	m_pActionList	= new CCArray ( 4 );
	m_pUpArray		= new CCArray ( 4 );
	m_pDownArray	= new CCArray ( 4 );
	m_pLeftArray	= new CCArray ( 4 );
	m_pRightArray	= new CCArray ( 4 );
	
	m_pNo = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 0, 43, 43, 43 ) );
	m_pNo->setTag ( NoType );
	m_pNo->retain ( );
	
	for ( KDint i = 0; i < 70; i++ ) 
	{
		m_pRemain->addObject ( m_pNo );
	}

	// timeline and timemask
	m_pTimeLine = CCLayerColor::create ( ccc4 ( 244, 208, 208, 255 ) ); 
	m_pTimeLine->setPosition ( ccp ( 5, 459 ) );
	m_pTimeLine->setAnchorPoint ( ccp ( 0, 0 ) );
	m_pTimeLine->setContentSize ( ccs ( 190, 12 ) );
	this->addChild ( m_pTimeLine, 3, 5 ); 

	m_pTimeMask = CCSprite::create ( "timeMask.png" );
	m_pTimeMask->setAnchorPoint ( ccp ( 0, 0 ) );
	m_pTimeMask->setPosition ( ccp ( 4, 458 ) );
	this->addChild ( m_pTimeMask, 4, 5 );

	// open button
	m_pOpen = CCSprite::create ( "open.png" );
	m_pOpen->setAnchorPoint ( ccp ( 0, 0 ) );
	m_pOpen->setPosition ( ccp ( 280, 450 ) );
	this->addChild ( m_pOpen, 5, 7 );
	m_pOpen->setVisible ( CCUserDefault::sharedUserDefault ( )->getBoolForKey ( "BBSoundBOOL", KD_TRUE ) ); 

	// close button
	m_pClose = CCSprite::create ( "close.png" );
	m_pClose->setAnchorPoint ( ccp ( 0, 0 ) );
	m_pClose->setPosition ( ccp ( 280, 450 ) );
	this->addChild ( m_pClose, 5, 7 );
	m_pClose->setVisible ( !m_pOpen->isVisible ( ) );
	Common::sharedCommon ( )->setPlay ( m_pOpen->isVisible ( ) );

	// score label
	m_pScoreLabel = CCLabelTTF::create ( "0", "Courier.ttf", 24 );
	m_pScoreLabel->setPosition ( ccp ( 250, 465 )  );
	m_pScoreLabel->setColor ( ccc3 ( 225, 79, 0 ) );
	this->addChild ( m_pScoreLabel, 6, 8 );

	// blue * 10
	for ( KDint i = 0; i < 10; i ++ )
	{
		CCSprite*	pBlue = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 43 * 8, 0, 43, 43 ) );
		pBlue->setTag ( BlueType );
		m_pBubbles->addObject ( pBlue );
		m_pSheet->addChild ( pBlue, 0 );
	}

	// green * 10
	for ( KDint i = 0; i < 10; i ++ )
	{
		CCSprite*	pGreen = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 43 * 6, 0, 43, 43 ) );
		pGreen->setTag ( GreenType );
		m_pBubbles->addObject ( pGreen );
		m_pSheet->addChild ( pGreen, 0 );
	}

	// red * 10
	for ( KDint i = 0; i < 10; i ++ )
	{
		CCSprite*	pRed = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 43 * 1, 0, 43, 43 ) );
		pRed->setTag ( RedType );
		m_pBubbles->addObject ( pRed );
		m_pSheet->addChild ( pRed, 0 );
	}

	// orange * 10
	for ( KDint i = 0; i < 10; i ++ )
	{
		CCSprite*	pOrange = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 43 * 4, 0, 43, 43 ) );
		pOrange->setTag ( OrangeType );
		m_pBubbles->addObject ( pOrange );
		m_pSheet->addChild ( pOrange, 0 );
	}

	// purple * 10
	for ( KDint i = 0; i < 10; i ++ )
	{
		CCSprite*	pPurple = CCSprite::createWithSpriteBatchNode ( m_pSheet, CCRect ( 43 * 2, 0, 43, 43 ) );
		pPurple->setTag ( PurpleType );
		m_pBubbles->addObject ( pPurple );
		m_pSheet->addChild ( pPurple, 0 );
	}

	this->initBubble ( );
	this->setTouchEnabled ( KD_TRUE );
	this->setTouchMode ( kCCTouchesOneByOne );

	m_nTopScore = CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( "BBTopScoreInteger" );

	m_pOver = OverScene::create ( );
	m_pOver->retain ( );
	m_pOver->setGame ( this );

	return KD_TRUE;
}

GameScene::~GameScene ( KDvoid )
{
	m_pTiles		->release ( );
	m_pBubbles		->release ( );
	m_pRemain		->release ( );
	m_pUpArray		->release ( );
	m_pDownArray	->release ( );
	m_pLeftArray	->release ( );
	m_pRightArray	->release ( );
	m_pActionList	->release ( );

	m_pNo			->release ( );
	m_pOver			->release ( );
}

KDvoid GameScene::initBubble ( KDvoid )
{
	m_pRemain->removeAllObjects ( );

	for ( KDint i = 0; i < 70; i ++ )
	{
		m_pRemain->addObject ( m_pNo );
	}
	
	this->initTiles ( 70 );

	CCObject*	pObject;
	KDint i = 0;
	CCARRAY_FOREACH ( m_pBubbles, pObject )
	{
		CCSprite*	pBubble = (CCSprite*) pObject;

		KDint		nTile = this->getTile ( );
		KDint		nX = nTile % 7;
		KDint		nY = nTile / 7;

//		CCLOG ( "%02d, %d, %d", nTile, nX, nY );

		pBubble->setPosition ( ccp ( nX * 45 + 22.5f + 2, nY * 45 + 22.5f ) );
		pBubble->setTag ( pBubble->getTag ( ) | nTile );
		m_pSheet->reorderChild ( pBubble, 0 );
		m_pRemain->replaceObjectAtIndex ( nTile, pBubble );
	}

	m_nTime = 20;
	m_nScore = 0;
	m_pScoreLabel->setString ( "0" );

	// time timer
	this->schedule ( schedule_selector ( GameScene::tick ), 1.f ); 
}

KDvoid GameScene::initTiles ( KDint nCapacity )
{
	m_pTiles = new CCArray ( nCapacity );

	for ( KDint i = 0; i < nCapacity; i++ )
	{
		m_pTiles->addObject ( CCInteger::create ( i ) );
	}
}

KDint GameScene::getTile ( KDvoid )
{
	KDint	nRand = kdRand ( ) % m_pTiles->count ( );
	KDint	nTile = ( (CCInteger*) m_pTiles->objectAtIndex ( nRand ) )->getValue ( );
	m_pTiles->removeObjectAtIndex ( nRand );
	return nTile;
}

KDvoid GameScene::tick ( KDfloat fDelta )
{
	m_nTime--;

	m_pTimeLine->setContentSize ( ccs ( m_nTime * 10.f, 12.f ) );

	if ( m_nTime == 0 )
	{
		this->unschedule ( schedule_selector ( GameScene::tick ) ); 

		if ( m_nScore > m_nTopScore ) 
		{
			m_nTopScore = m_nScore;
			CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "BBTopScoreInteger", m_nScore );		
		}

		m_pOver->initTop ( );

		Common::sharedCommon ( )->playOverSound ( );

		CCScene*	pScene = CCScene::create ( );
		pScene->addChild ( m_pOver );
		CCDirector::sharedDirector ( )->pushScene ( CCTransitionSlideInL::create ( 1, pScene ) );
	}
}

KDbool GameScene::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tp = pTouch->getLocation ( );

	KDint		x = (KDint) ( ( tp.x - 2 ) / 45 );
	KDint		y = (KDint) ( tp.y / 45 );
	KDint		p = y * 7 + x;

	if ( p > 75 )
	{
		if ( m_pOpen->isVisible ( ) )
		{
			m_pOpen ->setVisible ( KD_FALSE );
			m_pClose->setVisible ( KD_TRUE  );
		}
		else 
		{
			m_pOpen ->setVisible ( KD_TRUE  );
			m_pClose->setVisible ( KD_FALSE );
		}
		Common::sharedCommon ( )->setPlay ( m_pOpen->isVisible ( ) );
		CCUserDefault::sharedUserDefault ( )->setBoolForKey ( "BBSoundBOOL", m_pOpen->isVisible ( ) ); 
		return KD_TRUE;
	}
	else if ( p >= 70 )
	{
		return KD_TRUE;
	}

	CCSprite*	s = (CCSprite*) m_pRemain->objectAtIndex ( p );
	if ( s && s->getTag ( ) == 0xFFF00000 ) 
	{
		if ( this->shouldHit ( x, y ) )
		{
			this->playAnimationWithBeans ( );
			// hit
		}
		else 
		{
			// time - 1;
			m_pUpArray   ->removeAllObjects ( );
			m_pDownArray ->removeAllObjects ( );
			m_pLeftArray ->removeAllObjects ( );
			m_pActionList->removeAllObjects ( );
			m_nTime--;
			m_pTimeLine->setContentSize ( ccs ( m_nTime * 10.0f, 12.0f ) );
			if ( m_nTime == 0 ) 
			{
				this->unschedule ( schedule_selector ( GameScene::tick ) ); 

				if ( m_nScore > m_nTopScore )
				{
					m_nTopScore = m_nScore;
					CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "BBTopScoreInteger", m_nScore );	
				}
				m_pOver->initTop ( );

				Common::sharedCommon ( )->playOverSound ( );

				CCScene*	pScene = CCScene::create ( );
				pScene->addChild ( m_pOver );
				CCDirector::sharedDirector ( )->pushScene ( CCTransitionSlideInL::create ( 1, pScene ) );
			}
			else 
			{
				Common::sharedCommon ( )->playWrongSound ( );
			}
		}
	}

	return KD_TRUE;
}

KDbool GameScene::shouldHit ( KDint x, KDint y )
{
	KDint		nUpX, nUpY, nDownX, nDownY, nLeftX, nLeftY, nRightX, nRightY;
	KDint		nUpType, nDownType, nLeftType, nRightType;

	// up
	nUpX = x;
	nUpY = y;
	if ( nUpY < 10 - 1 )
	{
		nUpY++;
		while ( ( (CCNode*) m_pRemain->objectAtIndex ( nUpY * 7 + nUpX ) )->getTag ( ) == 0xFFF00000 )
		{
			nUpY++;
			if ( nUpY >= 10 )
			{
				nUpY = -1;
				nUpX = -1;
				break;
			}
		}	
	}
	else 
	{
		nUpX = -1;
		nUpY = -1;
	}

	if ( nUpX != -1 || nUpY != -1 )
	{
		nUpType = ( (CCNode*) m_pRemain->objectAtIndex ( nUpY * 7 + nUpX ) )->getTag ( )  & 0xFFFF0000;
	}
	else 
	{
		nUpType = -1;
	}


	// down
	nDownX = x;
	nDownY = y;
	if ( nDownY > 0 )
	{
		nDownY--;
		while ( ( (CCNode*) m_pRemain->objectAtIndex ( nDownY * 7 + nDownX ) )->getTag ( ) == 0xFFF00000 )
		{
			nDownY --;
			if ( nDownY <= -1 )
			{
				nDownY = -1;
				nDownX = -1;
				break;
			}
		}	
	}
	else 
	{
		nDownX = -1;
		nDownY = -1;
	}

	if ( nDownX != -1 || nDownY != -1 )
	{
		nDownType = ( (CCNode*) m_pRemain->objectAtIndex ( nDownY * 7 + nDownX ) )->getTag ( ) & 0xFFFF0000;
	}
	else 
	{
		nDownType = -1;
	}

	// left
	nLeftX = x;
	nLeftY = y;
	if ( nLeftX > 0 )
	{
		nLeftX--;
		while ( ( (CCNode*) m_pRemain->objectAtIndex ( nLeftY * 7 + nLeftX ) )->getTag ( ) == 0xFFF00000 )
		{
			nLeftX--;
			if ( nLeftX <= -1 )
			{
				nLeftY = -1;
				nLeftX = -1;
				break;
			}
		}	
	}
	else 
	{
		nLeftX = -1;
		nLeftY = -1;
	}
	
	if ( nLeftX != -1 || nLeftY != -1 )
	{
		nLeftType = ( (CCNode*) m_pRemain->objectAtIndex ( nLeftY * 7 + nLeftX ) )->getTag ( ) & 0xFFFF0000;
	}
	else 
	{
		nLeftType = -1;
	}

	//right
	nRightX = x;
	nRightY = y;
	if ( nRightX < 7 - 1 )
	{
		nRightX++;
		while ( ( (CCNode*) m_pRemain->objectAtIndex ( nRightY * 7 + nRightX ) )->getTag ( ) == 0xFFF00000 )
		{
			nRightX++;
			if ( nRightX >= 7 )
			{
				nRightY = -1;
				nRightX = -1;
				break;
			}
		}	
	}
	else 
	{
		nRightX = -1;
		nRightY = -1;
	}

	if ( nRightX != -1 || nRightY != -1 ) 
	{
		nRightType = ( (CCNode*) m_pRemain->objectAtIndex ( nRightY * 7 + nRightX ) )->getTag ( ) & 0xFFFF0000;
	}
	else 
	{
		nRightType = -1;
	}
	
	// group strategy
	KDint		nUp, nDown, nLeft;
	KDbool		bIsAddDown, bIsAddLeft, bIsAddRight;
	bIsAddDown = bIsAddLeft = bIsAddRight = KD_FALSE;

	if ( nUpType != -1 )
	{
		nUp = 1;
		m_pUpArray->addObject ( m_pRemain->objectAtIndex ( nUpY * 7 + nUpX ) );

		KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
		KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );
		
		CCJumpTo*	pActionUp = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );
		m_pActionList->addObject ( pActionUp );

		if ( nUpType == nDownType ) 
		{
			nUp++;
			bIsAddDown = KD_TRUE;
			m_pUpArray->addObject ( m_pRemain->objectAtIndex ( nDownY * 7 + nDownX ) );

			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionDown = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );
			m_pActionList->addObject ( pActionDown );
		}

		if ( nUpType == nLeftType )
		{
			nUp++;
			bIsAddLeft = KD_TRUE;
			m_pUpArray->addObject ( m_pRemain->objectAtIndex ( nLeftY * 7 + nLeftX ) );

			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionLeft = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
			m_pActionList->addObject ( pActionLeft );
		}

		if ( nUpType == nRightType )
		{
			nUp++;
			bIsAddRight = KD_TRUE;
			m_pUpArray->addObject ( m_pRemain->objectAtIndex ( nRightY * 7 + nRightX ) );

			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionRight = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
			m_pActionList->addObject ( pActionRight );			
		}
	}
	else 
	{
		nUp = 0;
	}

	if ( nDownType != -1 && !bIsAddDown ) 
	{
		nDown = 1;
		m_pDownArray->addObject ( m_pRemain->objectAtIndex ( nDownY * 7 + nDownX ) );		

		KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
		KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

		CCJumpTo*	pActionDown = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
		m_pActionList->addObject ( pActionDown );

		if ( nDownType == nLeftType && !bIsAddLeft )
		{
			nDown++;
			bIsAddLeft = KD_TRUE;
			m_pDownArray->addObject ( m_pRemain->objectAtIndex ( nLeftY * 7 + nLeftX ) );	
			
			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionLeft = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
			m_pActionList->addObject ( pActionLeft );
		}
		if ( nDownType == nRightType && !bIsAddRight )
		{
			m_pDownArray->addObject ( m_pRemain->objectAtIndex ( nRightY * 7 + nRightX ) );	
			
			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionRight = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
			m_pActionList->addObject ( pActionRight );	

			bIsAddRight = KD_TRUE;
			nDown++;
		}
	}
	else 
	{
		nDown = 0;
	}

	if ( nLeftType != -1 && !bIsAddLeft ) 
	{
		nLeft = 1;
		m_pLeftArray->addObject ( m_pRemain->objectAtIndex ( nLeftY * 7 + nLeftX ) );

		KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
		KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );
		
		CCJumpTo*	pActionLeft = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
		m_pActionList->addObject ( pActionLeft );

		if ( nLeftType == nRightType && !bIsAddRight )
		{
			nLeft++;
			bIsAddRight = KD_TRUE;
			m_pLeftArray->addObject ( m_pRemain->objectAtIndex ( nRightY * 7 + nRightX ) );	

			KDfloat		x = 160.f + ( kdRand ( ) % 400 - 200 );
			KDfloat		h = 200.f + ( kdRand ( ) % 100 -  50 );

			CCJumpTo*	pActionRight = CCJumpTo::create ( 1, ccp ( x, -50 ), h, 1 );			
			m_pActionList->addObject ( pActionRight );		
		}
	}
	else 
	{
		nLeft = 0;
	}

	if ( nUp > 1 || nDown > 1 || nLeft > 1 )
	{
		return KD_TRUE;
	}
	else 
	{
		return KD_FALSE;
	}
}

KDvoid GameScene::playAnimationWithBeans ( KDvoid )
{
	KDint		j = 0;
	CCObject*	pObject;
	
	if ( m_pUpArray->count ( ) > 1 )
	{
		CCARRAY_FOREACH ( m_pUpArray, pObject )
		{
			CCSprite*	pSprite = (CCSprite*) pObject;
			m_pSheet->reorderChild ( pSprite, 2 );
			pSprite->runAction ( ( (CCAction*) m_pActionList->objectAtIndex ( j++ ) ) );

			KDint		nIndex = m_pRemain->indexOfObject ( pSprite );			
			m_pRemain->replaceObjectAtIndex ( nIndex, m_pNo );
		}
		m_nScore += m_pUpArray->count ( );
	}
		
	if ( m_pDownArray->count ( ) > 1 )
	{
		CCARRAY_FOREACH ( m_pDownArray, pObject )
		{
			CCSprite*	pSprite = (CCSprite*) pObject;
			m_pSheet->reorderChild ( pSprite, 2 );
			pSprite->runAction ( ( (CCAction*) m_pActionList->objectAtIndex ( j++ ) ) );

			KDint		nIndex = m_pRemain->indexOfObject ( pSprite );					
			m_pRemain->replaceObjectAtIndex ( nIndex, m_pNo );
		}
		m_nScore += m_pDownArray->count ( );
	}
	
	if ( m_pLeftArray->count ( ) > 1 )
	{
		CCARRAY_FOREACH ( m_pLeftArray, pObject )
		{
			CCSprite*	pSprite = (CCSprite*) pObject;
			m_pSheet->reorderChild ( pSprite, 2 );
			pSprite->runAction ( ( (CCAction*) m_pActionList->objectAtIndex ( j++ ) ) );

			KDint		nIndex = m_pRemain->indexOfObject ( pSprite );					
			m_pRemain->replaceObjectAtIndex ( nIndex, m_pNo );
		}
		m_nScore += m_pLeftArray->count ( );
	}
	m_pScoreLabel->setString ( ccszf ( "%d", m_nScore ) );

	m_pUpArray   ->removeAllObjects ( );
	m_pDownArray ->removeAllObjects ( );
	m_pLeftArray ->removeAllObjects ( );
	m_pActionList->removeAllObjects ( );
	
	Common::sharedCommon ( )->playRightSound ( );	
}

KDint GameScene::getTopScore ( KDvoid )
{
	return m_nTopScore;
}

KDint GameScene::getScore ( KDvoid )
{
	return m_nScore;
}