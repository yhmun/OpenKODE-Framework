/* --------------------------------------------------------------------------
 *
 *      File            TestList.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "TestList.h"
#include "TestBasic.h"
#include "TestDef.h"

KDint				TestList::s_nTestIndex    = 0;
CCPoint				TestList::s_tPrevPosition = ccp ( -1, -1 );
ResolutionPolicy	TestList::s_eResolutionPolicy;
CCSize				TestList::s_tDesignSize; 

KDbool TestList::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCLayerGradient*  pBG = CCLayerGradient::create ( ccc4 ( 52, 84, 236, 200 ), ccc4 ( 149, 0, 202, 200 ) );
	pBG->setContentSize ( tLyrSize );
	this->addChild ( pBG, -10 );

	CCSprite*  pBottomLeft = CCSprite::create ( "ccb/jungle-left.png" );
	this->addChild ( pBottomLeft, 10 );
	pBottomLeft->setScale ( 1.5f );
	pBottomLeft->setOpacity ( 210 );
	pBottomLeft->setPositionWithParent ( kCCAlignmentBottomLeft );

	CCSprite*  pBottomRight = CCSprite::create ( "ccb/jungle-right.png" );
	this->addChild ( pBottomRight, 10 );
	pBottomRight->setScale ( 1.5f );
	pBottomRight->setOpacity ( 210 );
	pBottomRight->setPositionWithParent ( kCCAlignmentBottomRight );

	CCMenuItemImage*  pQuit = CCMenuItemImage::create
	(
		"xm_supports/CloseNormal.png", "xm_supports/CloseSelected.png", this, menu_selector ( TestList::onQuit ) 
	);
	pQuit->setScale ( 1.2f );
	pQuit->setPosition ( this, kCCAlignmentTopRight, ccp ( 10, 10 ) );
	this->addChild ( CCMenu::createWithItem ( pQuit ), 10 ); 

	CCTableView*  pTableView = CCTableView::create ( this, ccs ( tLyrSize.cx, tLyrSize.cy - 20.f ) );
	this->addChild ( pTableView );
	pTableView->setDelegate   ( this );	
	pTableView->setDataSource ( this );
	pTableView->setPosition   ( ccp ( 0, 10 ) );

	if ( s_tPrevPosition.x != -1 )
	{
		pTableView->setContentOffset ( s_tPrevPosition );
	}
	
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "fonts/ThonburiBold.ttf" );

	return KD_TRUE;
}

KDuint TestList::numberOfCellsInTableView ( CCTableView* pTable )
{
	return sizeof ( l_pTestNames ) / sizeof ( l_pTestNames [ 0 ] );
}

CCSize TestList::cellSizeForTable ( CCTableView* pTable )
{
	const CCSize&  tLyrSize = this->getContentSize ( );

	return CCSize ( tLyrSize.cx, 35.f );
}

CCTableViewCell* TestList::tableCellAtIndex ( CCTableView* pTable, KDuint uIndex )
{
	CCTableViewCell*  pCell = pTable->dequeueCell ( );

	if ( !pCell )
	{
		pCell = CCTableViewCell::create ( );
		
		CCMenuItemFont*   pItem = CCMenuItemFont::create ( l_pTestNames [ uIndex ] );	
		pCell->addChild ( pItem, 0, 1 );
		pItem->setPosition ( ccpMid ( cellSizeForTable ( pTable ) ) );
	}
	else
	{
		CCMenuItemFont*   pItem = (CCMenuItemFont*) pCell->getChildByTag ( 1 );
		pItem->setString ( l_pTestNames [ uIndex ] );	
	}

	return pCell;
}

KDvoid TestList::tableCellTouched ( CCTableView* pTable, CCTableViewCell* pCell )
{
	s_tPrevPosition = pTable->getContentOffset ( );

	TestList::setScene ( pCell->getIdx ( ), eDirNew );
}

KDvoid TestList::tableCellHighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1 ) )->selected ( );
}

KDvoid TestList::tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1 ) )->unselected ( );
}

KDvoid TestList::scrollViewDidScroll ( CCScrollView* pView )
{

}

KDvoid TestList::scrollViewDidZoom ( CCScrollView* pView )
{

}

KDvoid TestList::onQuit ( CCObject* pSender )
{
    CCDirector::sharedDirector ( )->end ( );
}

const KDchar* TestList::getName ( KDint nIndex )
{
	return l_pTestNames [ nIndex ];
}

KDvoid TestList::setScene ( KDint nIndex, EDirection eDirection )
{	
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	pDirector->setDepthTest ( KD_FALSE );
	
	s_nTestIndex = nIndex;

	CCLayer*  pLayer = KD_NULL;
	
	if ( nIndex == 41 || nIndex == 42 )
	{
		CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( s_tDesignSize, kResolutionShowAll );
	}
	else
	{
		CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( s_tDesignSize, s_eResolutionPolicy );
	}

	switch ( nIndex )
	{
		case -1 : pLayer = TestList					::create ( ); break;		
		case  0 : pLayer = TestActions				::create ( ); break;									
		case  1 : pLayer = TestTransitions			::create ( ); break;
		case  2	: pLayer = TestProgressActions		::create ( ); break;	
		case  3 : pLayer = TestEffects				::create ( ); break;	
		case  4	: pLayer = TestClickAndMove			::create ( ); break;	
		case  5 : pLayer = TestRotateWorld			::create ( ); break;
		case  6 : pLayer = TestParticle				::create ( ); break;
		case  7 : pLayer = TestEaseActions			::create ( ); break;	
		case  8 : pLayer = TestMotionStreak			::create ( ); break;
		case  9	: pLayer = TestDrawPrimitives		::create ( ); break;
		case 10 : pLayer = TestNode					::create ( ); break;
		case 11 : pLayer = TestTouches				::create ( ); break;
		case 12 : pLayer = TestMultiTouch			::create ( ); break;
		case 13 : pLayer = TestMenu					::create ( ); break;			
		case 14	: pLayer = TestActionManager		::create ( ); break;
		case 15 : pLayer = TestLayer				::create ( ); break;	
		case 16 : pLayer = TestScene				::create ( ); break;		
		case 17 : pLayer = TestParallax				::create ( ); break;
		case 18 : pLayer = TestParallaxScrollNode	::create ( ); break;
		case 19 : pLayer = TestTileMap				::create ( ); break;	
		case 20 : pLayer = TestDiamondTile			::create ( ); break;
		case 21 : pLayer = TestInterval				::create ( ); break;
		case 22 : pLayer = TestLabel				::create ( ); break;
		case 23 : pLayer = TestFont					::create ( ); break;
		case 24 : pLayer = TestTextInput			::create ( ); break;							
		case 25 : pLayer = TestSprite				::create ( ); break;
		case 26 : pLayer = TestScheduler			::create ( ); break;
		case 27 : pLayer = TestRenderTexture		::create ( ); break;
		case 28 : pLayer = TestTexture2D			::create ( ); break;
		case 29 : pLayer = TestTextureCache			::create ( ); break;
		case 30 : pLayer = TestBox2D				::create ( ); break;
		case 31 : pLayer = TestBox2DBed				::create ( ); break;
		case 32 : pLayer = TestChipmunk				::create ( ); break;
		case 33 : pLayer = TestChipmunk2			::create ( ); break;
		case 34 : pLayer = TestEffectAdvance		::create ( ); break;	
		case 35 : pLayer = TestAccelerometer		::create ( ); break;	
		case 36 : pLayer = TestKeypad				::create ( ); break;    		
		case 37 : pLayer = TestZwoptex				::create ( ); break;
		case 38 : pLayer = TestUserDefault			::create ( ); break;
		case 39 : pLayer = TestNotificationCenter	::create ( ); break;
		case 40 : pLayer = TestControl				::create ( ); break;
		case 41 : pLayer = TestEditBox				::create ( ); break;
		case 42 : pLayer = TestTableView			::create ( ); break;
		case 43 : pLayer = TestClippingNode			::create ( ); break;
		case 44 : pLayer = TestCocosBuilder			::create ( ); break;	
		case 45 : pLayer = TestArtPig				::create ( ); break;	
		case 46 : pLayer = TestSpine				::create ( ); break;
		case 47 : pLayer = TestGB2Shape				::create ( ); break;
		case 48 : pLayer = TestVRope				::create ( ); break;
		case 49 : pLayer = TestBlade				::create ( ); break;
		case 50 : pLayer = TestDenshion				::create ( ); break;
		case 51 : pLayer = TestTTS					::create ( ); break;
		case 52 : pLayer = TestCurl					::create ( ); break;	
		case 53 : pLayer = TestNetwork				::create ( ); break;
		case 54 : pLayer = TestSqlite				::create ( ); break;
		case 55 : pLayer = TestJson					::create ( ); break;
		case 56 : pLayer = TestLocation				::create ( ); break;
		case 57 : pLayer = TestCurrentLanguage		::create ( ); break;
		case 58 : pLayer = TestFileUtils			::create ( ); break;
		case 59 : pLayer = TestAssetsManager		::create ( ); break;
		case 60 : pLayer = TestPerformance			::create ( ); break;
		case 61 : pLayer = TestBugAndSupport		::create ( ); break;
#if defined ( USE_OPEN_GLES2 )	
		case 62 : pLayer = TestShader				::create ( ); break;
#else
		case 62 : pLayer = TestGraphics				::create ( ); break;
#endif
	}

	if ( !pLayer )
	{
		return;
	}
	
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( pLayer, nIndex );

	CCScene*  pTrans = pScene;
	if ( nIndex != 1 && nIndex != 24 ) // TestTransitions, TestSprite
	{
		/*
		switch ( eDirection )
		{
			case eDirNew	 :	pTrans = CCTransitionFadeTR			::create ( 0.5f, pScene );	break;
			case eDirClose	 :	pTrans = CCTransitionFadeBL			::create ( 0.5f, pScene );	break;
			case eDirPrev	 :	pTrans = CCTransitionMoveInL		::create ( 0.5f, pScene );	break;
			case eDirNext	 :	pTrans = CCTransitionMoveInR		::create ( 0.5f, pScene );	break;
			case eDirRestart :	pTrans = CCTransitionTurnOffTiles	::create ( 0.5f, pScene );	break;
		}
		*/
	}
	pDirector->replaceScene ( pTrans );		
}

