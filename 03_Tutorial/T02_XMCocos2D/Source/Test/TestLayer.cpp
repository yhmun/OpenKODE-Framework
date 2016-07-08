/* --------------------------------------------------------------------------
 *
 *      File            TestLayer.cpp
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
#include "TestLayer.h"

enum 
{
	kTagLayer = 1,
};

TestLayer* TestLayer::create ( KDvoid )
{
	TestLayer*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 :	pLayer = new LayerTestCascadingOpacityA		( );	break;
		case  1 :	pLayer = new LayerTestCascadingOpacityB		( );	break;
		case  2 :	pLayer = new LayerTestCascadingOpacityC		( );	break;
		case  3 :	pLayer = new LayerTestCascadingColorA		( );	break;
		case  4 :	pLayer = new LayerTestCascadingColorB		( );	break;
		case  5 :	pLayer = new LayerTestCascadingColorC		( );	break;
		case  6 :	pLayer = new LayerTest1						( );	break;
		case  7 :	pLayer = new LayerTest2						( );	break;
		case  8 :	pLayer = new LayerTestBlend					( );	break;
		case  9 :	pLayer = new LayerGradient					( );	break;
        case 10 :	pLayer = new LayerIgnoreAnchorPointPos		( );	break;
        case 11 :	pLayer = new LayerIgnoreAnchorPointRot		( );	break;
        case 12 :	pLayer = new LayerIgnoreAnchorPointScale	( );	break;
		case 13 :	pLayer = new LayerExtendedBlendOpacityTest	( );	break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestLayer::count ( KDvoid )
{
	return 14;
}

static KDvoid setEnableRecursiveCascading ( CCNode* pNode, KDbool bEnable )
{
    CCRGBAProtocol*  pRgba = dynamic_cast<CCRGBAProtocol*> ( pNode );
    if ( pRgba )
    {
        pRgba->setCascadeColorEnabled   ( bEnable );
        pRgba->setCascadeOpacityEnabled ( bEnable );
    }
    
    CCObject*  pObj;
    CCArray*   pChildren = pNode->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pObj )
    {
        CCNode*  pChild = (CCNode*) pObj;
        setEnableRecursiveCascading ( pChild, bEnable );
    }
}

//------------------------------------------------------------------
//
// LayerTestCascadingOpacityA
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingOpacityA::onEnter ( KDvoid )
{
    TestLayer::onEnter ( );
    
    CCSize  s = this->getContentSize ( );
    CCLayerRGBA* layer1 = CCLayerRGBA::create();
    
    CCSprite*  sister1 = CCSprite::create ( "Images/grossinis_sister1.png" );
    CCSprite*  sister2 = CCSprite::create ( "Images/grossinis_sister2.png" );
    CCLabelBMFont*  label = CCLabelBMFont::create ( "Test", "fonts/bitmapFontTest.fnt" );
    
    layer1->addChild ( sister1 );
    layer1->addChild ( sister2 );
    layer1->addChild ( label );
    this->addChild ( layer1, 0, kTagLayer );
    
    sister1->setPosition ( ccp( s.cx*1/3, s.cy/2));
    sister2->setPosition ( ccp( s.cx*2/3, s.cy/2));
    label->setPosition ( ccp( s.cx/2, s.cy/2));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(4, 0),
       CCFadeTo::create(4, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCDelayTime::create(1),
       KD_NULL ) ) );
    
    
    // Enable cascading in scene
    setEnableRecursiveCascading ( this, KD_TRUE );
}

const KDchar* LayerTestCascadingOpacityA::title ( KDvoid )
{
    return "LayerRGBA: cascading opacity";
}

//------------------------------------------------------------------
//
// LayerTestCascadingOpacityB
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingOpacityB::onEnter ( KDvoid )
{
    TestLayer::onEnter();
        
    CCSize s = this->getContentSize ( );
    CCLayerColor* layer1 = CCLayerColor::create(ccc4(192, 0, 0, 255), s.cx, s.cy/2);
    layer1->setCascadeColorEnabled(KD_FALSE);
    
    layer1->setPosition( ccp(0, s.cy/2));
    
    CCSprite *sister1 = CCSprite::create("Images/grossinis_sister1.png");
    CCSprite *sister2 = CCSprite::create("Images/grossinis_sister2.png");
    CCLabelBMFont *label = CCLabelBMFont::create("Test", "fonts/bitmapFontTest.fnt");
    
    layer1->addChild(sister1);
    layer1->addChild(sister2);
    layer1->addChild(label);
    this->addChild( layer1, 0, kTagLayer);
    
    sister1->setPosition( ccp( s.cx*1/3, 0));
    sister2->setPosition( ccp( s.cx*2/3, 0));
    label->setPosition( ccp( s.cx/2, 0));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(4, 0),
       CCFadeTo::create(4, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCDelayTime::create(1),
       NULL)));
    
    // Enable cascading in scene
    setEnableRecursiveCascading(this, KD_TRUE);
}

const KDchar* LayerTestCascadingOpacityB::title ( KDvoid )
{
    return "CCLayerColor: cascading opacity";
}

//------------------------------------------------------------------
//
// LayerTestCascadingOpacityC
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingOpacityC::onEnter ( KDvoid )
{
    TestLayer::onEnter();
    
    CCSize s = this->getContentSize ( );
    CCLayerColor* layer1 = CCLayerColor::create(ccc4(192, 0, 0, 255), s.cx, s.cy/2);
    layer1->setCascadeColorEnabled(KD_FALSE);
    layer1->setCascadeOpacityEnabled(KD_FALSE);
    
    layer1->setPosition( ccp(0, s.cy/2));
    
    CCSprite *sister1 = CCSprite::create("Images/grossinis_sister1.png");
    CCSprite *sister2 = CCSprite::create("Images/grossinis_sister2.png");
    CCLabelBMFont *label = CCLabelBMFont::create("Test", "fonts/bitmapFontTest.fnt");
    
    layer1->addChild(sister1);
    layer1->addChild(sister2);
    layer1->addChild(label);
    this->addChild( layer1, 0, kTagLayer);
    
    sister1->setPosition( ccp( s.cx*1/3, 0));
    sister2->setPosition( ccp( s.cx*2/3, 0));
    label->setPosition( ccp( s.cx/2, 0));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(4, 0),
       CCFadeTo::create(4, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCFadeTo::create(2, 0),
       CCFadeTo::create(2, 255),
       CCDelayTime::create(1),
       NULL)));
}

const KDchar* LayerTestCascadingOpacityC::title ( KDvoid )
{
    return "CCLayerColor: non-cascading opacity";
}

//------------------------------------------------------------------
//
// LayerTestCascadingColorA
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingColorA::onEnter ( KDvoid )
{
    TestLayer::onEnter();
    
    CCSize s = this->getContentSize ( );
    CCLayerRGBA* layer1 = CCLayerRGBA::create();
    
    CCSprite *sister1 = CCSprite::create("Images/grossinis_sister1.png");
    CCSprite *sister2 = CCSprite::create("Images/grossinis_sister2.png");
    CCLabelBMFont *label = CCLabelBMFont::create("Test", "fonts/bitmapFontTest.fnt");
    
    layer1->addChild(sister1);
    layer1->addChild(sister2);
    layer1->addChild(label);
    this->addChild( layer1, 0, kTagLayer);
    
    sister1->setPosition( ccp( s.cx*1/3, s.cy/2));
    sister2->setPosition( ccp( s.cx*2/3, s.cy/2));
    label->setPosition( ccp( s.cx/2, s.cy/2));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(6, 255, 0, 255),
       CCTintTo::create(6, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(2, 255, 255, 0),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 0, 255, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 255, 0, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
    
    // Enable cascading in scene
    setEnableRecursiveCascading(this, KD_TRUE);
     
}

const KDchar* LayerTestCascadingColorA::title ( KDvoid )
{
    return "LayerRGBA: cascading color";
}


//------------------------------------------------------------------
//
// LayerTestCascadingColorB
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingColorB::onEnter ( KDvoid )
{
    TestLayer::onEnter ( );

    CCSize s = this->getContentSize ( );
    CCLayerColor* layer1 = CCLayerColor::create(ccc4(255, 255, 255, 255), s.cx, s.cy/2);
    
    layer1->setPosition( ccp(0, s.cy/2));
    
    CCSprite *sister1 = CCSprite::create("Images/grossinis_sister1.png");
    CCSprite *sister2 = CCSprite::create("Images/grossinis_sister2.png");
    CCLabelBMFont *label = CCLabelBMFont::create("Test", "fonts/bitmapFontTest.fnt");
    
    layer1->addChild(sister1);
    layer1->addChild(sister2);
    layer1->addChild(label);
    this->addChild( layer1, 0, kTagLayer);
    
    sister1->setPosition( ccp( s.cx*1/3, 0));
    sister2->setPosition( ccp( s.cx*2/3, 0));
    label->setPosition( ccp( s.cx/2, 0));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(6, 255, 0, 255),
       CCTintTo::create(6, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(2, 255, 255, 0),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 0, 255, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 255, 0, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
    
    // Enable cascading in scene
    setEnableRecursiveCascading(this, KD_TRUE);
}

const KDchar* LayerTestCascadingColorB::title ( KDvoid )
{
    return "CCLayerColor: cascading color";
}

//------------------------------------------------------------------
//
// LayerTestCascadingColorC
//
//------------------------------------------------------------------

KDvoid LayerTestCascadingColorC::onEnter ( KDvoid )
{
    TestLayer::onEnter ( );

    CCSize s = this->getContentSize ( );
    CCLayerColor* layer1 = CCLayerColor::create(ccc4(255, 255, 255, 255), s.cx, s.cy/2);
    layer1->setCascadeColorEnabled(KD_FALSE);
    layer1->setPosition( ccp(0, s.cy/2));
    
    CCSprite *sister1 = CCSprite::create("Images/grossinis_sister1.png");
    CCSprite *sister2 = CCSprite::create("Images/grossinis_sister2.png");
    CCLabelBMFont *label = CCLabelBMFont::create("Test", "fonts/bitmapFontTest.fnt");
    
    layer1->addChild(sister1);
    layer1->addChild(sister2);
    layer1->addChild(label);
    this->addChild( layer1, 0, kTagLayer);
    
    sister1->setPosition( ccp( s.cx*1/3, 0));
    sister2->setPosition( ccp( s.cx*2/3, 0));
    label->setPosition( ccp( s.cx/2, 0));
    
    layer1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(6, 255, 0, 255),
       CCTintTo::create(6, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
    
    sister1->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCTintTo::create(2, 255, 255, 0),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 0, 255, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCTintTo::create(2, 255, 0, 255),
       CCTintTo::create(2, 255, 255, 255),
       CCDelayTime::create(1),
       NULL)));
}

const KDchar* LayerTestCascadingColorC::title ( KDvoid )
{
    return "CCLayerColor: non-cascading color";
}

//------------------------------------------------------------------
//
// LayerTest1
//
//------------------------------------------------------------------

KDbool LayerTest1::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

	CCLayerColor*  layer = CCLayerColor::create ( ccc4 ( 0xFF, 0x00, 0x00, 0x80 ), CCSize ( 200, 200 ) ); 
	layer->ignoreAnchorPointForPosition ( KD_FALSE );
	layer->setPosition ( ccpMid ( s ) );
	
	this->addChild ( layer, 10, kTagLayer );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* LayerTest1::title ( KDvoid )
{
	return "ColorLayer resize";
}

const KDchar* LayerTest1::subtitle ( KDvoid )
{
	return "( tap & move )";
}

KDvoid LayerTest1::updateSize ( const CCPoint& tLocation )
{
	const CCSize&   tLyrSize = this->getContentSize ( );	

	CCSize  tNewSize = CCSize ( kdFabsf ( tLocation.x - tLyrSize.cx / 2 ) * 2, kdFabsf ( tLocation.y - tLyrSize.cy / 2 ) * 2 );
	
	CCLayerColor*  pLayer = (CCLayerColor*) getChildByTag ( kTagLayer );
	pLayer->setContentSize ( tNewSize );
}

KDvoid LayerTest1::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	ccTouchesMoved ( pTouches, pEvent );
}

KDvoid LayerTest1::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );

    updateSize ( this->convertTouchToNodeSpace ( pTouch ) );
}

KDvoid LayerTest1::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	ccTouchesMoved ( pTouches, pEvent );
}

//------------------------------------------------------------------
//
// LayerTest2
//
//------------------------------------------------------------------
KDbool LayerTest2::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

	CCLayerColor*  layer1 = CCLayerColor::create ( ccc4 ( 255, 255, 0, 80 ), CCSize ( 100, 300 ) );
	layer1->setPosition ( ccp ( s.cx / 3, s.cy / 2 ) );
	layer1->ignoreAnchorPointForPosition ( KD_FALSE );

	CCActionInterval*  actionTint = CCTintBy::create ( 2, -255, -127, 0 );
	layer1->runAction ( CCSequence::create ( actionTint, actionTint->reverse ( ), KD_NULL ) );

	this->addChild ( layer1, 1 );
	
	CCLayerColor*  layer2 = CCLayerColor::create ( ccc4 ( 0, 0, 255, 255 ), CCSize ( 100, 300 ) );
	layer2->setPosition ( ccp ( ( s.cx / 3 ) * 2, s.cy / 2 ) );
	layer2->ignoreAnchorPointForPosition ( KD_FALSE );
	
	CCActionInterval*  actionFade = CCFadeOut::create ( 2.0f );
	layer2->runAction ( CCSequence::create ( actionFade, actionFade->reverse ( ), KD_NULL ) );

	this->addChild ( layer2, 1 );

	return KD_TRUE;
}

const KDchar* LayerTest2::title ( KDvoid )
{
	return "ColorLayer : fade and tint";
}

//------------------------------------------------------------------
//
// LayerTestBlend
//
//------------------------------------------------------------------
KDbool LayerTestBlend::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

	CCLayerColor*  layer1 = CCLayerColor::create ( ccc4 ( 255, 255, 255, 80 ) );
	this->addChild ( layer1, 100, kTagLayer );

	CCSprite*  sister1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sister1->setPosition ( ccp ( s.cx / 3, s.cy / 2 ) );
	this->addChild ( sister1 );

	CCSprite*  sister2 = CCSprite::create ( "Images/grossinis_sister2.png" );
	sister2->setPosition ( ccp ( s.cx / 3 * 2, s.cy / 2 ) );
	this->addChild ( sister2 );
	
	this->schedule ( schedule_selector ( LayerTestBlend::newBlend ), 1.0f );

	return KD_TRUE;
}

KDvoid LayerTestBlend::newBlend ( KDfloat dt )
{
 	CCLayerColor*  layer = (CCLayerColor *) getChildByTag ( kTagLayer );

	GLenum src;
	GLenum dst;

	if ( layer->getBlendFunc ( ).dst == GL_ZERO )
	{
		src = GL_SRC_ALPHA;
		dst = GL_ONE_MINUS_SRC_ALPHA;
	}
	else
	{
		src = GL_ONE_MINUS_DST_COLOR;
		dst = GL_ZERO;
	}

	ccBlendFunc  bf = { src, dst };
	layer->setBlendFunc ( bf );

}

const KDchar* LayerTestBlend::title ( KDvoid )
{
	return "ColorLayer : blend";
}

//------------------------------------------------------------------
//
// LayerGradient
//
//------------------------------------------------------------------
KDbool LayerGradient::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

    CCLayerGradient*  gradient = CCLayerGradient::create ( ccc4 ( 255, 0, 0, 255 ), ccc4 ( 0, 255, 0, 255 ), ccp ( 0.9f, 0.9f ) );
	this->addChild ( gradient, 0, kTagLayer );
	
	CCMenuItemToggle*  item = CCMenuItemToggle::createWithTarget
	(
		this,
		menu_selector ( LayerGradient::toggleItem ), 
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Compressed Interpolation: Enabled" , "fonts/Marker Felt.ttf", 18 ) ), 
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Compressed Interpolation: Disabled", "fonts/Marker Felt.ttf", 18 ) ),
		KD_NULL
	);

	CCMenu*  menu = CCMenu::createWithItem ( item );
	menu->setPosition ( ccp ( s.cx / 2, 100 ) );
	this->addChild ( menu );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid LayerGradient::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    const CCSize&  tLyrSize = this->getContentSize ( );

    CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );   
    CCPoint    tDiff = ccpSub ( ccpMid ( tLyrSize ), this->convertTouchToNodeSpace ( pTouch ) );

    tDiff = ccpNormalize ( tDiff );

    CCLayerGradient*  pGradient = (CCLayerGradient*) getChildByTag ( 1 );
	pGradient->setVector ( tDiff );
}

const KDchar* LayerGradient::title ( KDvoid )
{
    return "LayerGradient";
}

const KDchar* LayerGradient::subtitle ( KDvoid )
{
    return "Touch the screen and move your finger";
}

KDvoid LayerGradient::toggleItem ( CCObject* sender )
{
	CCLayerGradient*  gradient = (CCLayerGradient*) getChildByTag ( kTagLayer );
	do
	{
		CC_BREAK_IF ( !gradient );

		gradient->setCompressedInterpolation ( !gradient->isCompressedInterpolation ( ) );

	} while ( 0 );

}

//------------------------------------------------------------------
//
// LayerIgnoreAnchorPointPos
//
//------------------------------------------------------------------

#define kLayerIgnoreAnchorPoint  1000

KDbool LayerIgnoreAnchorPointPos::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 150, 150);

    l->setAnchorPoint(ccp(0.5f, 0.5f));
    l->setPosition(ccp( s.cx/2, s.cy/2));

    CCMoveBy *move = CCMoveBy::create(2, ccp(100,2));
    CCMoveBy * back = (CCMoveBy*) move->reverse();
    CCSequence *seq = CCSequence::create(move, back, KD_NULL);
    l->runAction(CCRepeatForever::create(seq));
    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.cx/2, lsize.cy/2));

	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
    CCMenuItemFont *item = CCMenuItemFont::create("Toggle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointPos::onToggle));

    CCMenu *menu = CCMenu::create(item, KD_NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.cx/2, s.cy/2));

	return KD_TRUE;
}

const KDchar* LayerIgnoreAnchorPointPos::title ( KDvoid )
{
    return "IgnoreAnchorPoint - Position";
}

const KDchar* LayerIgnoreAnchorPointPos::subtitle ( KDvoid )
{
    return "Ignoring Anchor Point for position";
}

KDvoid LayerIgnoreAnchorPointPos::onToggle ( CCObject* sender )
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    KDbool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

//------------------------------------------------------------------
//
// LayerIgnoreAnchorPointRot
//
//------------------------------------------------------------------
KDbool LayerIgnoreAnchorPointRot::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );
	
    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 200, 200);

    l->setAnchorPoint(ccp(0.5f, 0.5f));
    l->setPosition(ccp( s.cx/2, s.cy/2));

    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCRotateBy *rot = CCRotateBy::create(2, 360);
    l->runAction(CCRepeatForever::create(rot));


    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.cx/2, lsize.cy/2));

	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
    CCMenuItemFont *item = CCMenuItemFont::create("Toogle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointRot::onToggle));

    CCMenu *menu = CCMenu::create(item, KD_NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.cx/2, s.cy/2));

	return KD_TRUE;
}

const KDchar* LayerIgnoreAnchorPointRot::title ( KDvoid )
{
    return "IgnoreAnchorPoint - Rotation";
}

const KDchar* LayerIgnoreAnchorPointRot::subtitle ( KDvoid )
{
    return "Ignoring Anchor Point for rotations";
}

KDvoid LayerIgnoreAnchorPointRot::onToggle ( CCObject* sender )
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    KDbool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

//------------------------------------------------------------------
//
// LayerIgnoreAnchorPointScale
//
//------------------------------------------------------------------
KDbool LayerIgnoreAnchorPointScale::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );
	
    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 200, 200);

    l->setAnchorPoint(ccp(0.5f, 1.0f));
    l->setPosition(ccp( s.cx/2, s.cy/2));


    CCScaleBy *scale = CCScaleBy::create(2, 2);
    CCScaleBy* back = (CCScaleBy*)scale->reverse();
    CCSequence *seq = CCSequence::create(scale, back, KD_NULL);

    l->runAction(CCRepeatForever::create(seq));

    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.cx/2, lsize.cy/2));

	CCMenuItemFont::setFontName ( "fonts/American Typewriter.ttf" );
    CCMenuItemFont *item = CCMenuItemFont::create("Toogle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointScale::onToggle));

    CCMenu *menu = CCMenu::create(item, KD_NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.cx/2, s.cy/2));

	return KD_TRUE;
}

const KDchar* LayerIgnoreAnchorPointScale::title ( KDvoid )
{
    return "IgnoreAnchorPoint - Scale";
}

const KDchar* LayerIgnoreAnchorPointScale::subtitle ( KDvoid )
{
    return "Ignoring Anchor Point for scale";
}

KDvoid LayerIgnoreAnchorPointScale::onToggle ( CCObject* sender )
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    KDbool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

//------------------------------------------------------------------
//
// LayerExtendedBlendOpacityTest
//
//------------------------------------------------------------------
KDbool LayerExtendedBlendOpacityTest::init ( KDvoid )
{
	if ( !TestLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	
    CCLayerGradient*  pLayer1 = CCLayerGradient::create ( ccc4 ( 255, 0, 0, 255 ), ccc4 ( 255, 0, 255, 255 ) );
    pLayer1->setContentSize ( ccs ( 80, 80 ) );
    pLayer1->setPosition    ( ccp ( tLyrSize.cx / 2 - 50, tLyrSize.cy / 2 - 50 ) );
	pLayer1->ignoreAnchorPointForPosition ( KD_FALSE );
    this->addChild ( pLayer1 );
    
    CCLayerGradient*  pLayer2 = CCLayerGradient::create ( ccc4 ( 0, 0, 0, 127 ), ccc4 ( 255, 255, 255, 127 ) );
    pLayer2->setContentSize ( ccs ( 80, 80 ) );
    pLayer2->setPosition    ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 ) );
	pLayer2->ignoreAnchorPointForPosition ( KD_FALSE );
    this->addChild ( pLayer2 );
    
    CCLayerGradient*  pLayer3 = CCLayerGradient::create ( );
    pLayer3->setContentSize  ( ccs (  80,  80 ) );
    pLayer3->setPosition     ( ccp ( tLyrSize.cx / 2 + 50, tLyrSize.cy / 2 + 50 ) );
    pLayer3->setStartColor   ( ccc3 ( 255, 0, 0   ) );
    pLayer3->setEndColor     ( ccc3 ( 255, 0, 255 ) );
    pLayer3->setStartOpacity ( 255 );
    pLayer3->setEndOpacity   ( 255 );
    ccBlendFunc  tBlend;
    tBlend.src = GL_SRC_ALPHA;
    tBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
    pLayer3->setBlendFunc ( tBlend );
	pLayer3->ignoreAnchorPointForPosition ( KD_FALSE );
    this->addChild ( pLayer3 );

	return KD_TRUE;
}

const KDchar* LayerExtendedBlendOpacityTest::title ( KDvoid )
{
    return "Extended Blend & Opacity";
}

const KDchar* LayerExtendedBlendOpacityTest::subtitle ( KDvoid )
{
    return "You should see 3 layers";
}