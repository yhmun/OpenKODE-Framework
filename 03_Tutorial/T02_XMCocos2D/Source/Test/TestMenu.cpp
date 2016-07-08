/* --------------------------------------------------------------------------
 *
 *      File            TestMenu.cpp
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
#include "TestMenu.h"

enum 
{
	kTagMenu  = 1,
	kTagMenu0 = 0,					   
	kTagMenu1 = 1,
};

#define MID_CALLBACK       1000
#define MID_CALLBACK2      1001
#define MID_DISABLED       1002
#define MID_ENABLE         1003
#define MID_CONFIG         1004
#define MID_QUIT           1005
#define MID_OPACITY        1006
#define MID_ALIGN          1007
#define MID_CALLBACK3      1008
#define MID_BACKCALLBACK   1009

KDbool TestMenu::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

 	CCLayerMultiplex*  pLayer = CCLayerMultiplex::create 
	(
		MenuLayer1::create ( ), 
		MenuLayer2::create ( ),
		MenuLayer3::create ( ), 
		MenuLayer4::create ( ), 
		MenuLayer5::create ( ),
		MenuLayerPriorityTest::create  ( ),
		RemoveMenuItemWhenMove::create ( ),
		KD_NULL
	);
	this->addChild ( pLayer ); 

	return KD_TRUE;
}

//------------------------------------------------------------------
//
// MenuLayer1
//
//------------------------------------------------------------------
KDbool MenuLayer1::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_pDisabledItem = KD_NULL;

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

	// Sprite Item
	CCMenuItemSprite*  pItem1 = CCMenuItemSprite::create 
	(
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 2, 115, 23 ) ), 
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 1, 115, 23 ) ),
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 0, 115, 23 ) ),
		this, menu_selector ( MenuLayer1::menuCallback ) 
	);

	// Image Item
	CCMenuItem*  pItem2 = CCMenuItemImage::create 
	( 
		"Images/SendScoreButton.png", "Images/SendScoreButtonPressed.png",
		this, menu_selector ( MenuLayer1::menuCallback2 )
	);

	// Label Item ( LabelAtlas )
	CCMenuItemLabel*  pItem3 = CCMenuItemLabel::create
	(
		CCLabelAtlas::create ( "0123456789", "fonts/labelatlas.png", 16, 24, '.' ), 
		this, menu_selector ( MenuLayer1::menuCallbackDisabled ) 
	);
	m_pDisabledItem = pItem3; 
	pItem3->setDisabledColor ( ccc3 ( 32, 32, 64 ) );
	pItem3->setColor ( ccc3 ( 200, 200, 255 ) );
	pItem3->setEnabled ( KD_FALSE );

	// Font Item
	CCMenuItemFont*  pItem4 = CCMenuItemFont::create 
	(
		"I toggle enable items",
		this, menu_selector ( MenuLayer1::menuCallbackEnable ) 
	);
    pItem4->setFontSizeObj ( 20 );
    pItem4->setFontName ( "fonts/Marker Felt.ttf" );

	// Label Item ( BitmapFontAtlas )
	CCMenuItemLabel*  pItem5 = CCMenuItemLabel::create
	(
		CCLabelBMFont::create ( "configuration", "fonts/bitmapFontTest3.fnt" ),
		this, menu_selector ( MenuLayer1::menuCallbackConfig ) 
	);
	pItem5->setScale ( 0.8f );

    // Events
    CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
    CCMenuItemFont*  pItem6 = CCMenuItemFont::create
	(
		"Priority Test",
		this, menu_selector ( MenuLayer1::menuCallbackPriorityTest )
	);

	// Font Item
	CCMenuItemFont*  pItem7 = CCMenuItemFont::create
	(
		"Radio menus", 
		this, menu_selector ( MenuLayer1::menuCallbackRadio ) 
	);

    CCMenuItemFont*  pItem8 = CCMenuItemFont::create 
	(
		"Remove menu item when moving",
		this, menu_selector ( MenuLayer1::menuMovingCallback )
	);
 
	CCActionInterval*  pAction = CCTintBy::create ( 0.5f, 0, -255, -255 );
	CCSequence*  pSeq = CCSequence::create ( pAction, pAction->reverse ( ), KD_NULL );
	pItem7->runAction ( CCRepeatForever::create ( pSeq ) );

	CCMenu*  pMenu = CCMenu::create ( pItem1, pItem2, pItem3, pItem4, pItem5, pItem6, pItem7, pItem8, KD_NULL );
    pMenu->setContentSize ( tSize );
	pMenu->setPosition ( ccpMid ( tSize ) );
	pMenu->alignItemsVertically ( 10 );
	this->addChild ( pMenu );

	// elastic effect
	KDint            i = 0;
	CCArray*    pArray = pMenu->getChildren ( );
	CCObject*  pObject = KD_NULL;
	CCARRAY_FOREACH ( pArray, pObject )
	{
		if ( !pObject )
		{
			break;
		}

		CCNode*    pChild = (CCNode*) pObject;
		CCPoint    tPoint = pChild->getPosition ( );
		KDint     nOffset = (KDint) ( tSize.cx / 2 + 50 );

		if ( i % 2 == 0 )
		{
			nOffset = -nOffset;
		}
		
		pChild->setPosition ( ccp ( tPoint.x + nOffset, tPoint.y ) );
		pChild->runAction
		( 
			CCEaseElasticOut::create 
			(
				CCMoveBy::create ( 2, ccp ( tPoint.x - nOffset, 0 ) ), 0.35f
			) 
		);
		i++;
	}

	this->setTouchEnabled  ( KD_TRUE );
    this->setTouchPriority ( kCCMenuHandlerPriority + 1 );
    this->setTouchMode     ( kCCTouchesOneByOne );

	return KD_TRUE;
}

KDbool MenuLayer1::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

KDvoid MenuLayer1::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{

}

KDvoid MenuLayer1::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{

}

KDvoid MenuLayer1::menuCallback ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 1 );
}

KDvoid MenuLayer1::menuCallbackConfig ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 3 );
}

KDvoid MenuLayer1::allowTouches ( KDfloat dt )
{
    CCDirector::sharedDirector ( )->getTouchDispatcher ( )->setPriority ( kCCMenuHandlerPriority + 1, this );

    this->unscheduleAllSelectors ( );
    
	CCLOG ( "TOUCHES ALLOWED AGAIN" );
}

KDvoid MenuLayer1::menuCallbackDisabled ( CCObject* sender ) 
{
    // hijack all touch events for 5 seconds
    CCDirector::sharedDirector ( )->getTouchDispatcher ( )->setPriority ( kCCMenuHandlerPriority - 1, this );

    this->schedule ( schedule_selector ( MenuLayer1::allowTouches ), 5.0f );

    CCLOG ( "TOUCHES DISABLED FOR 5 SECONDS" );
}

KDvoid MenuLayer1::menuCallbackEnable ( CCObject* sender ) 
{
	m_pDisabledItem->setEnabled ( !m_pDisabledItem->isEnabled ( ) );
}

KDvoid MenuLayer1::menuCallback2 ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 2 );
}

KDvoid MenuLayer1::menuCallbackPriorityTest ( CCObject* pSender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 5 );
}

KDvoid MenuLayer1::menuCallbackRadio ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 4 );
}

KDvoid MenuLayer1::menuMovingCallback ( CCObject* pSender )
{
    ( (CCLayerMultiplex*) m_pParent )->switchTo ( 6 );
}

//------------------------------------------------------------------
//
// MenuLayer2
//
//------------------------------------------------------------------
KDbool MenuLayer2::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

	for ( KDint i = 0; i < 2; i++ ) 
	{
		CCMenuItemImage*  pItem1 = CCMenuItemImage::create ( "Images/btn-play-normal.png"       , "Images/btn-play-selected.png"      , this, menu_selector ( MenuLayer2::menuCallback ) );
		CCMenuItemImage*  pItem2 = CCMenuItemImage::create ( "Images/btn-highscores-normal.png" , "Images/btn-highscores-selected.png", this, menu_selector ( MenuLayer2::menuCallbackOpacity ) );
		CCMenuItemImage*  pItem3 = CCMenuItemImage::create ( "Images/btn-about-normal.png"      , "Images/btn-about-selected.png"     , this, menu_selector ( MenuLayer2::menuCallbackAlign ) );
		
		pItem1->setScaleX ( 1.5f );
		pItem2->setScaleX ( 0.5f );
		pItem3->setScaleX ( 0.5f );
		
		CCMenu*  pMenu = CCMenu::create ( pItem1, pItem2, pItem3, KD_NULL );

		pMenu->setPosition ( ccpMid ( tSize ) );
		pMenu->setTag ( kTagMenu );
			
		this->addChild ( pMenu, 0, 100 + i );

		m_tCenteredMenu = pMenu->getPosition ( );
	}

	m_bAlignedH = KD_TRUE;
	alignMenusH ( );

	return KD_TRUE;
}

KDvoid MenuLayer2::alignMenusH ( KDvoid )
{
	for ( KDint i = 0;i < 2; i++ ) 
	{
		CCMenu*  pMenu = (CCMenu*) getChildByTag ( 100 + i );
		pMenu->setPosition ( m_tCenteredMenu );

		if ( i == 0 ) 
		{
			// TIP: if no padding, padding = 5
			pMenu->alignItemsHorizontally ( );			
			pMenu->setPosition ( ccpAdd ( pMenu->getPosition ( ), ccp ( 0, 30 ) ) );
			
		} 
		else 
		{
			// TIP: but padding is configurable
			pMenu->alignItemsHorizontallyWithPadding ( 40 );
			pMenu->setPosition ( ccpSub ( pMenu->getPosition ( ), ccp ( 0, 30 ) ) );
		}		
	}
}

KDvoid MenuLayer2::alignMenusV ( KDvoid )
{
	for ( KDint i = 0; i < 2; i++ ) 
	{
		CCMenu*  pMenu = (CCMenu*) getChildByTag ( 100 + i );
		pMenu->setPosition ( m_tCenteredMenu );

		if ( i == 0 ) 
		{
			// TIP: if no padding, padding = 5
			pMenu->alignItemsVertically ( );			
			pMenu->setPosition ( ccpAdd ( pMenu->getPosition ( ), ccp ( 100, 0 ) ) );			
		} 
		else 
		{
			// TIP: but padding is configurable
			pMenu->alignItemsVerticallyWithPadding ( 40 );	
			pMenu->setPosition ( ccpSub ( pMenu->getPosition( ), ccp ( 100, 0 ) ) );
		}		
	}
}

KDvoid MenuLayer2::menuCallback ( CCObject* pSender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

KDvoid MenuLayer2::menuCallbackOpacity ( CCObject* pSender )
{
	CCMenu*  pMenu = (CCMenu*) ( ( (CCNode*) pSender )->getParent ( ) );
	
	pMenu->setOpacity ( pMenu->getOpacity ( ) == 128 ? 255 : 128 );
}

KDvoid MenuLayer2::menuCallbackAlign ( CCObject* sender )
{
	m_bAlignedH = !m_bAlignedH;
	
	m_bAlignedH ? alignMenusH ( ) : alignMenusV ( ); 
}

//------------------------------------------------------------------
//
// MenuLayer3
//
//------------------------------------------------------------------
KDbool MenuLayer3::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_pDisabledItem = KD_NULL;

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 28 );

	CCActionInterval*  spin = CCRotateBy::create ( 3, 360 );

	CCMenuItemLabel*  item1 = CCMenuItemLabel::create
	(
		CCLabelBMFont::create ( "Enable AtlasItem", "fonts/bitmapFontTest3.fnt" ), this, menu_selector ( MenuLayer3::menuCallback2 ) 
	);
	item1->setPosition ( ccp ( tSize.cx / 2 - 150, tSize.cy / 2 ) );
	item1->runAction ( CCRepeatForever::create ( spin ) );

	CCMenuItemFont*  item2 = CCMenuItemFont::create ( "--- Go Back ---", this, menu_selector ( MenuLayer3::menuCallback ) );		
	item2->setPosition ( ccp ( tSize.cx / 2 - 200, tSize.cy / 2 ) );

	CCJumpBy*  jump = CCJumpBy::create ( 3, ccp ( 400, 0 ), 50, 4 );
	item2->runAction ( CCRepeatForever::create
	(
		CCSequence::create ( jump, jump->reverse ( ), KD_NULL ) 
	) );

	item2->runAction ( CCRepeatForever::create ( (CCActionInterval*) ( spin->copy ( )->autorelease ( ) ) ) );

	CCMenuItemSprite* item3 = CCMenuItemSprite::create
	(
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 2, 115, 23 ) ), 
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 1, 115, 23 ) ),
		CCSprite::create ( "Images/menuitemsprite.png", CCRectMake ( 0, 23 * 0, 115, 23 ) ),
		this,
		menu_selector ( MenuLayer3::menuCallback3 )
	);

	m_pDisabledItem = item3;

	item3->setEnabled ( KD_FALSE );
	item3->setPosition ( ccp ( tSize.cx / 2, tSize.cy / 2 - 100 ) );
	item3->runAction ( CCRepeatForever::create ( (CCActionInterval*) ( spin->copy ( )->autorelease ( ) ) ) );

	this->addChild ( CCMenu::create ( item1, item2, item3, KD_NULL ) ); 

	return KD_TRUE;
}

KDvoid MenuLayer3::menuCallback ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

KDvoid MenuLayer3::menuCallback2 ( CCObject* sender )
{
	if ( m_pDisabledItem )
	{
		m_pDisabledItem->setEnabled ( !m_pDisabledItem->isEnabled ( ) );
		m_pDisabledItem->stopAllActions ( );
	}
}

KDvoid MenuLayer3::menuCallback3 ( CCObject* sender )
{
	
}

//------------------------------------------------------------------
//
// MenuLayer4
//
//------------------------------------------------------------------
KDbool MenuLayer4::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  title1 = CCMenuItemFont::create ( "Sound" );
	title1->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 34 );
	CCMenuItemToggle*  item1 = CCMenuItemToggle::createWithTarget 
	(
		this, 
		menu_selector ( MenuLayer4::menuCallback ), 
		CCMenuItemFont::create ( "On" ),
		CCMenuItemFont::create ( "Off"),
		KD_NULL 
	);
    
	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  title2 = CCMenuItemFont::create ( "Music" );
	title2->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 34 );
	CCMenuItemToggle* item2 = CCMenuItemToggle::createWithTarget
	(
		this, 
		menu_selector ( MenuLayer4::menuCallback ),
		CCMenuItemFont::create ( "On" ),
		CCMenuItemFont::create ( "Off"),
		KD_NULL
	);
    
	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  title3 = CCMenuItemFont::create ( "Quality" );
	title3->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );
	CCMenuItemToggle*  item3 = CCMenuItemToggle::createWithTarget
	(	
		this, 
		menu_selector ( MenuLayer4::menuCallback ), 
		CCMenuItemFont::create ( "High" ),
		CCMenuItemFont::create ( "Low" ),
		KD_NULL
	);
    
	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  title4 = CCMenuItemFont::create ( "Orientation" );
	title4->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 34 );
	CCMenuItemToggle* item4 = CCMenuItemToggle::createWithTarget
	(
		this, 
		menu_selector ( MenuLayer4::menuCallback ), 
		CCMenuItemFont::create ( "Off" ), 
		KD_NULL
	);

	// TIP: you can manipulate the items like any other CCMutableArray
	item4->getSubItems ( )->addObject ( CCMenuItemFont::create ( "33%" ) ); 
	item4->getSubItems ( )->addObject ( CCMenuItemFont::create ( "66%" ) ); 
	item4->getSubItems ( )->addObject ( CCMenuItemFont::create ( "100%" ) ); 

    // you can change the one of the items by doing this
	item4->setSelectedIndex ( 2 );
	    
    CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 34 );	
	CCMenuItemLabel*  back = CCMenuItemLabel::create ( CCLabelBMFont::create ( "go back", "fonts/bitmapFontTest3.fnt" ), this, menu_selector ( MenuLayer4::backCallback ) );
    
	CCMenu* menu = CCMenu::create
	(
		title1,  title2,
		 item1,   item2,
		title3,  title4,		
		 item3,   item4,
		back  , KD_NULL 
	); 

    menu->setContentSize ( tSize );
	menu->setPosition ( ccpMid ( tSize ) );
	menu->alignItemsInColumns ( 2, 2, 2, 2, 1, KD_NULL );
	this->addChild ( menu );

	return KD_TRUE;
}

KDvoid MenuLayer4::menuCallback ( CCObject* sender )
{

}

KDvoid MenuLayer4::backCallback ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

//------------------------------------------------------------------
//
// MenuLayer5
//
//------------------------------------------------------------------
KDbool MenuLayer5::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

    CCMenuRadio*  radio = CCMenuRadio::create ( this, menu_selector ( MenuLayer5::menuCallback ) );
	
	for ( KDuint it = 0; it < 5; it++ )
	{
        radio->addChild ( CCMenuItemImage::create ( "xm_supports/btn_radio_off.png", "xm_supports/btn_radio_off.png", "xm_supports/btn_radio_on.png" ) );
	}

	radio->alignItemsHorizontally ( 10 );
	radio->setPosition ( ccpMid ( tSize ) );	
	this->addChild ( radio );

	m_pLabel = CCLabelTTF::create ( "Radio menu not clicked.", "fonts/Marker Felt.ttf", 16 );
	m_pLabel->setPosition ( ccp ( tSize.cx / 2, tSize.cy / 2 + 50 ) );	
	this->addChild ( m_pLabel );
	
	CCMenuItemLabel*  back = CCMenuItemLabel::create ( CCLabelBMFont::create ( "go back", "fonts/bitmapFontTest3.fnt" ), this, menu_selector ( MenuLayer5::backCallback ) );

	back->setPosition ( ccp ( tSize.cx / 2, tSize.cy / 2 - 50 ) );	
	this->addChild ( CCMenu::createWithItem ( back ) );

	return KD_TRUE;
}

KDvoid MenuLayer5::menuCallback ( CCObject* sender )
{
	KDchar         msg [ 128 ];
	kdSprintfKHR ( msg, "Radio menu clicked %d.", ( (CCMenuRadio*) sender )->getSelectedIndex ( ) );
	
	m_pLabel->setString ( msg );
}

KDvoid MenuLayer5::backCallback ( CCObject* sender )
{
	( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

KDbool MenuLayerPriorityTest::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );

    // Testing empty menu
    m_pMenu1 = CCMenu::create ( );
    m_pMenu2 = CCMenu::create ( );

	m_pMenu1->setPosition ( ccpMid ( tSize ) );
	m_pMenu2->setPosition ( ccpMid ( tSize ) );

    // Menu 1
    CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
    CCMenuItemFont::setFontSize ( 18 );
    CCMenuItemFont*  pItem1 = CCMenuItemFont::create ( "Return to Main Menu" , this, menu_selector ( MenuLayerPriorityTest::menuCallback ) );
    CCMenuItemFont*  pItem2 = CCMenuItemFont::create ( "Disable menu for 5 seconds" , this, menu_selector ( MenuLayerPriorityTest::disableMenuCallback ) );

    m_pMenu1->addChild ( pItem1 );
    m_pMenu1->addChild ( pItem2 );

    m_pMenu1->alignItemsVerticallyWithPadding ( 5 );

    addChild ( m_pMenu1 );

    // Menu 2
    m_bPriority = KD_TRUE;
    CCMenuItemFont::setFontSize ( 48 );
    pItem1 = CCMenuItemFont::create ( "Toggle priority", this, menu_selector ( MenuLayerPriorityTest::togglePriorityCallback ) );
    pItem1->setColor ( ccc3 ( 0, 0, 255 ) );
    m_pMenu2->addChild ( pItem1 );
    addChild ( m_pMenu2 );

	return KD_TRUE;
}

KDvoid MenuLayerPriorityTest::menuCallback ( CCObject* pSender )
{
    ( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

KDvoid MenuLayerPriorityTest::disableMenuCallback ( CCObject* pSender )
{
    m_pMenu1->setEnabled ( KD_FALSE );

    CCSequence*  pAction = CCSequence::create 
	(
		CCDelayTime::create ( 5 ),
		CCCallFunc ::create ( this, callfunc_selector ( MenuLayerPriorityTest::enableMenuCallback ) ), 
		KD_NULL 
	);

    m_pMenu1->runAction ( pAction );
}

KDvoid MenuLayerPriorityTest::enableMenuCallback ( KDvoid )
{
    m_pMenu1->setEnabled ( KD_TRUE );
}

KDvoid MenuLayerPriorityTest::togglePriorityCallback ( CCObject* pSender )
{
    if ( m_bPriority) 
	{
        m_pMenu2->setHandlerPriority ( kCCMenuHandlerPriority + 20 );
        m_bPriority = KD_FALSE;
    }
	else 
	{
        m_pMenu2->setHandlerPriority ( kCCMenuHandlerPriority - 20 );
        m_bPriority = KD_TRUE;
    }
}


//------------------------------------------------------------------
//
// RemoveMenuItemWhenMove
//
//------------------------------------------------------------------

KDbool RemoveMenuItemWhenMove::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

    CCSize  tSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );
    
    CCLabelTTF*  pLabel = CCLabelTTF::create ( "click item and move, should not crash", "fonts/Marker Felt.ttf", 20 );
    pLabel->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 100 ) );
    addChild ( pLabel );
    
    m_pItem = CCMenuItemFont::create ( "item 1" );
    m_pItem->retain ( );
    
    CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
    CCMenuItemFont::setFontSize ( 20 );
    CCMenuItemFont*  pBack = CCMenuItemFont::create("go back", this, menu_selector ( RemoveMenuItemWhenMove::goBack ) );
    
    CCMenu*  pMenu = CCMenu::create ( m_pItem, pBack, KD_NULL );
    addChild ( pMenu );
    pMenu->alignItemsVertically ( );
    pMenu->setPosition ( ccp ( tSize.cx / 2, tSize.cy / 2 ) );
    
    setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid RemoveMenuItemWhenMove::goBack ( CCObject* pSender )
{
    ( (CCLayerMultiplex*) m_pParent )->switchTo ( 0 );
}

RemoveMenuItemWhenMove::~RemoveMenuItemWhenMove ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pItem );
}

void RemoveMenuItemWhenMove::registerWithTouchDispatcher ( KDvoid )
{
    CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, -129, KD_FALSE );
}

KDbool RemoveMenuItemWhenMove::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    return KD_TRUE;
}

KDvoid RemoveMenuItemWhenMove::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( m_pItem )
    {
        m_pItem->removeFromParentAndCleanup ( KD_TRUE );
        m_pItem->release ( );
        m_pItem = KD_NULL;
    }
}