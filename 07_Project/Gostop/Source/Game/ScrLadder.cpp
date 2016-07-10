/*
 *
 *      File            ScrLadder.cpp
 *      Description     Ladder scene.
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#include "Precompiled.h"
#include "Dialog/DlgBase.h"
#include "Dialog/DlgNotice.h"
#include "ScrLadder.h"

//
//  Main layer of "CScrLadder" Scene.
//
KDbool CScrLadder::init ( KDvoid )
{
	m_uSelect	= 0;
	m_pFace		= KD_NULL;
	m_pNotice	= KD_NULL;

	kdMemset ( m_pButtons, KD_NULL, sizeof ( m_pButtons ) );

	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDuint  i;

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_Ladder )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );
	}

	g_pResManager->playBGM ( eSND_BGM_Ladder );

	CCSprite*  pNotice = CCSprite::create 
	(
		g_pResManager->getPath ( eIMG_ETC_Ladder_Notice )
	);
	{
		CC_ASSERT ( pNotice );

		this->addChild ( pNotice ); 

		pNotice->retain ( );
		pNotice->setPosition ( ccpMid ( tWinSize ) );

		m_pNotice = pNotice;
	} 

	for ( i = 0; i < 5; i++ )
	{
		CCMenuItemImage*  pButton = CCMenuItemImage::create 
		(
			g_pResManager->getPath ( eIMG_ETC_Ladder_Nor ), 
			g_pResManager->getPath ( eIMG_ETC_Ladder_Nor ),
			g_pResManager->getPath ( eIMG_ETC_Ladder_Sel )
		);

		CC_ASSERT ( pButton );

		pButton->retain ( );

		m_pButtons [ i ] = pButton;
	}

	CCMenuRadio*  pRadio = CCMenuRadio::create 
	(
		this, 
		menu_selector ( CScrLadder::onSelect ),
		m_pButtons [ 0 ], m_pButtons [ 1 ], m_pButtons [ 2 ], m_pButtons [ 3 ], m_pButtons [ 4 ],
		KD_NULL
	);
	{
		CC_ASSERT ( pRadio );

		this->addChild ( pRadio );

		pRadio->setPosition ( ccp ( tWinSize.cx / 2.f, tWinSize.cy - 75.f ) );
		pRadio->alignItemsHorizontallyWithPadding ( 103.f );
	}
	
	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CScrLadder::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pFace );
	CC_SAFE_RELEASE ( m_pNotice );

	for ( KDuint i = 0; i < 5; i++ )
	{
		CC_SAFE_RELEASE ( m_pButtons [ i ] );
	}

	CCLayer::onExit ( );
}

KDvoid CScrLadder::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority - 1, KD_TRUE );
}

KDbool CScrLadder::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCNode*  pNode = m_pButtons [ m_uSelect ];
	CCRect   tRect;
	
	tRect.size = pNode->getContentSize ( );

	if ( tRect.containsPoint ( pNode->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
	{
		g_pResManager->playSound ( eSND_Etc_Click );

		CCMenuRadio*  pRadio = (CCMenuRadio *) pNode->getParent ( );
		
		pRadio->setTouchEnabled ( KD_FALSE );
		this->setTouchEnabled ( KD_FALSE );

		this->onStart ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CScrLadder::onStart ( KDvoid )
{
	CCNode*  pNode;
	CCPoint  tPoint;
	CCSize   tSize;
	KDuint   i;

	kdSrand ( KD_GET_UST2MSEC );
	this->removeChild ( m_pNotice, KD_TRUE );

	for ( i = 0; i < 12; i++ )
	{
		KDuint  uIndex = kdRand ( ) % 4;

		pNode  = m_pButtons [ uIndex ];
		tPoint = pNode->convertToWorldSpace ( ccp ( 0, 0 ) );
		tSize  = pNode->getContentSize ( );

		CCLayerColor*  pLine = CCLayerColor::create ( ccc4 ( ccWHITE ), CCSize ( 160.f, 7.f ) );
		{
			CC_ASSERT ( pLine );

			this->addChild ( pLine );

			pLine->retain ( );
			pLine->setTag ( uIndex );
			pLine->setPosition ( ccp ( tPoint.x + tSize.cx / 2.f, i * 20.f + 120.f ) );

			m_vLines.push_back ( pLine );
		}
	}

	CCSprite*  pFace = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Ladder_KBM )
	);
	{
		CC_ASSERT ( pFace );

		this->addChild ( pFace );

		pNode  = m_pButtons [ m_uSelect ];
		tPoint = pNode->convertToWorldSpace ( ccp ( 0, 0 ) );

		pFace->retain ( );
		pFace->setTag ( m_uSelect );
		pFace->setPosition ( tPoint );
		pFace->ignoreAnchorPointForPosition ( KD_TRUE );

		m_pFace = pFace;
	}

	m_pFace->runAction ( CCCallFunc::create ( this, callfunc_selector ( CScrLadder::onMove ) ) );
}

KDvoid CScrLadder::onMove ( KDvoid )
{
	CCNode*  pNode   = KD_NULL;
	KDuint   uIndex1 = m_pFace->getTag ( );
	KDuint   uIndex2 = 0;
	KDuint   uCount  = 0;
	KDbool   bLoop   = KD_TRUE;
	CCPoint  tPoint1 = m_pFace->getPosition ( );
	CCPoint  tPoint2;
	CCSize   tSize;
	
	while ( bLoop )
	{
		if ( m_vLines.size ( ) == 0 )
		{
			m_pFace->runAction
			(
				(CCActionInterval *) CCSequence::create
				(
					CCMoveTo::create ( 0.1f, ccp ( tPoint1.x, 30.f ) ), 
					CCCallFunc::create ( this, callfunc_selector ( CScrLadder::onEnd ) ),
					KD_NULL
				)
			);

			break;
		}

		uCount++;

		pNode = m_vLines.back ( );
		uIndex2 = pNode->getTag ( );
		
		if ( uIndex1 == uIndex2 || uIndex1 - 1 == uIndex2 )
		{
			tSize = pNode->getContentSize ( );
			tPoint2 = pNode->getPosition ( );
			
			m_pFace->setTag
			(
				uIndex1 == uIndex2 ? uIndex1 + 1 : uIndex2
			);

			m_pFace->runAction
			(
				(CCActionInterval *) CCSequence::create
				(
					CCMoveTo::create ( 0.1f * uCount, ccp ( tPoint1.x, tPoint2.y ) ), 
					CCMoveTo::create ( 0.5f, ccp ( uIndex1 == uIndex2 ? tPoint1.x + tSize.cx : tPoint1.x - tSize.cx, tPoint2.y ) ), 
					CCCallFunc::create ( this, callfunc_selector ( CScrLadder::onMove ) ),
					KD_NULL
				)
			);

			bLoop = KD_FALSE;
		}

		m_vLines.pop_back ( );
		pNode->release ( );
	}
}

KDvoid CScrLadder::onEnd ( KDvoid )
{
	KDuint  uPercent = 0;

	switch ( m_pFace->getTag ( ) )
	{
		case 0 : uPercent = 10; break;
		case 1 : uPercent = 15; break;
		case 2 : uPercent = 20; break;
		case 3 : uPercent = 25; break;
		case 4 : uPercent = 30; break;
	}

	// Title Reward
	if ( g_pEnvManager->getTitle ( ) == 3 )
	{
		uPercent += 20;
	}

	CDlgLadder::s_uPercent = uPercent;

	g_pScrManager->setDialog ( eDlgLadder );
}

KDvoid CScrLadder::onSelect ( CCObject* pSender )
{
	CCMenuRadio*  pRadio = (CCMenuRadio *) pSender;
	
	g_pResManager->playSound ( eSND_Etc_Focus );

	m_uSelect = pRadio->getSelectedIndex ( );
}

