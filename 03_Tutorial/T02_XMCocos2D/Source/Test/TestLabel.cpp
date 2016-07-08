/* --------------------------------------------------------------------------
 *
 *      File            TestLabel.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestLabel.h"

enum
{
	kTagTileMap       = 1, 
	kTagSpriteManager = 1, 
	kTagAnimation1    = 1, 
	kTagBitmapAtlas1  = 1, 
	kTagBitmapAtlas2  = 2, 
	kTagBitmapAtlas3  = 3, 
};

enum 
{
	kTagSprite1, 
	kTagSprite2, 
	kTagSprite3, 
	kTagSprite4, 
	kTagSprite5, 
	kTagSprite6, 
	kTagSprite7, 
	kTagSprite8, 
};

TestLabel* TestLabel::create ( KDvoid )
{
	TestLabel*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new LabelAtlasTest				( ); break;
		case  1 : pLayer = new LabelAtlasColorTest			( ); break;
		case  2 : pLayer = new Atlas3						( ); break;
		case  3 : pLayer = new Atlas4						( ); break;
		case  4 : pLayer = new Atlas5						( ); break;
		case  5 : pLayer = new Atlas6						( ); break;
		case  6 : pLayer = new AtlasBitmapColor				( ); break;
		case  7 : pLayer = new AtlasFastBitmap				( ); break;
		case  8 : pLayer = new BitmapFontMultiLine			( ); break;
		case  9 : pLayer = new LabelsEmpty					( ); break;
		case 10 : pLayer = new LabelBMFontHD				( ); break;
		case 11 : pLayer = new LabelAtlasHD					( ); break;
		case 12 : pLayer = new LabelGlyphDesigner			( ); break;

		// Not a label test. Should be moved to Atlas test
		case 13 : pLayer = new Atlas1						( ); break;
		case 14 : pLayer = new LabelTTFTest					( ); break;
		case 15 : pLayer = new LabelTTFMultiline			( ); break;
        case 16 : pLayer = new LabelTTFChinese				( ); break;
        case 17 : pLayer = new LabelBMFontChinese			( ); break;
        case 18 : pLayer = new BitmapFontMultiLineAlignment	( ); break;
        case 19 : pLayer = new LabelTTFA8Test				( ); break;
        case 20 : pLayer = new BMFontOneAtlas				( ); break;
        case 21 : pLayer = new BMFontUnicode				( ); break;
        case 22 : pLayer = new BMFontInit					( ); break;
        case 23 : pLayer = new TTFFontInit					( ); break;
		case 24 : pLayer = new LabelTTFAlignment			( ); break;
		case 25 : pLayer = new LabelBMFontBounds			( ); break;
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

KDuint TestLabel::count ( KDvoid )
{
	return 26;
}

//------------------------------------------------------------------
//
// Atlas1
//
//------------------------------------------------------------------
KDvoid Atlas1::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	m_pTextureAtlas = CCTextureAtlas::create ( "Images/atlastest.png", 3 ); 
	m_pTextureAtlas->retain ( );

	//
	// Notice: u, v tex coordinates are inverted
	//
	ccV3F_C4B_T2F_Quad quads[] = 
	{
		{
			{ {    0,    0, 0 }, ccc4 ( 0, 0, 255, 255 ), { 0.0f, 1.0f }, },			  // bottom left
			{ { s.cx,    0, 0 }, ccc4 ( 0, 0, 255,   0 ), { 1.0f, 1.0f }, },			  // bottom right
			{ {    0, s.cy, 0 }, ccc4 ( 0, 0, 255,   0 ), { 0.0f, 0.0f }, },			  // top left
			{ { s.cx, s.cy, 0 },      { 0, 0, 255, 255 }, { 1.0f, 0.0f }, },			  // top right
		}, 		
		{
			{ {  40,  40, 0 }, ccc4 ( 255, 255, 255, 255 ), { 0.0f, 0.2f }, },            // bottom left
			{ { 120,  80, 0 }, ccc4 ( 255,   0,   0, 255 ), { 0.5f, 0.2f }, },            // bottom right
			{ {  40, 160, 0 }, ccc4 ( 255, 255, 255, 255 ), { 0.0f, 0.0f }, },            // top left
			{ { 160, 160, 0 }, ccc4 (   0, 255,   0, 255 ), { 0.5f, 0.0f }, },            // top right
		}, 

		{
			{ { s.cx / 2     ,  40, 0 }, ccc4 ( 255,   0,   0, 255 ), { 0.0f, 1.0f }, },  // bottom left
			{ { s.cx         ,  40, 0 }, ccc4 (   0, 255,   0, 255 ), { 1.0f, 1.0f }, },  // bottom right
			{ { s.cx / 2 - 50, 200, 0 }, ccc4 (   0,   0, 255, 255 ), { 0.0f, 0.0f }, },  // top left
			{ { s.cx         , 100, 0 }, ccc4 ( 255, 255,   0, 255 ), { 1.0f, 0.0f }, },  // top right
		}, 
		
	};
	
	for ( KDuint it = 0; it < 3; it++ ) 
	{
		m_pTextureAtlas->updateQuad ( &quads [ it ], it );
	}
}

KDvoid Atlas1::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTextureAtlas );

	TestLabel::onExit ( );
}

KDvoid Atlas1::draw ( KDvoid )
{
	if ( m_pTextureAtlas )
	{
		m_pTextureAtlas->drawQuads ( );
	}
}

const KDchar* Atlas1::title ( KDvoid )
{
	return "CCTextureAtlas";
}

const KDchar* Atlas1::subtitle ( KDvoid )
{
	return "Manual creation of CCTextureAtlas";
}

//------------------------------------------------------------------
//
// LabelAtlasTest
//
//------------------------------------------------------------------
KDvoid LabelAtlasTest::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	m_fTime = 0;

	const CCSize&  s = this->getContentSize ( );	

	CCLabelAtlas*  pLabel1 = CCLabelAtlas::create ( "123 Test", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ' );
	pLabel1->setPosition ( ccp ( 50, s.cy / 2 - 70 ) );
	pLabel1->setOpacity ( 200 );
	this->addChild ( pLabel1, 0, kTagSprite1 );
	
	CCLabelAtlas*  pLabel2 = CCLabelAtlas::create ( "0123456789", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ' );
	pLabel2->setPosition ( ccp ( 50, s.cy / 2 + 10 ) );
	pLabel2->setOpacity ( 32 );
	this->addChild ( pLabel2, 0, kTagSprite2 );

	this->schedule ( schedule_selector ( LabelAtlasTest::step ) ); 
}

KDvoid LabelAtlasTest::step ( KDfloat fDelta )
{
	m_fTime += fDelta;
    
	CCLabelAtlas*  pLabel1 = (CCLabelAtlas*) getChildByTag ( kTagSprite1 );
	pLabel1->setString ( ccszf ( "%2.2f Test", m_fTime ) );

	CCLabelAtlas*  pLabel2 = (CCLabelAtlas*) getChildByTag ( kTagSprite2 );
	pLabel2->setString ( ccszf ( "%d", (KDint) m_fTime ) );
}

const KDchar* LabelAtlasTest::title ( KDvoid )
{
	return "LabelAtlas";
}

const KDchar* LabelAtlasTest::subtitle ( KDvoid )
{
	return "Updating label should be fast";
}

//------------------------------------------------------------------
//
// LabelAtlasColorTest
//
//------------------------------------------------------------------
KDvoid LabelAtlasColorTest::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	m_fTime = 0;

	const CCSize&  s = this->getContentSize ( );	

	CCLabelAtlas*  pLabel1 = CCLabelAtlas::create ( "123 Test", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ' );
	pLabel1->setPosition ( ccp ( 50, s.cy / 2 - 70 ) );
	pLabel1->setOpacity ( 200 );
	this->addChild ( pLabel1, 0, kTagSprite1 );
	
	CCLabelAtlas*  pLabel2 = CCLabelAtlas::create ( "0123456789", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ' );
	pLabel2->setPosition ( ccp ( 50, s.cy / 2 + 10 ) );
	pLabel2->setColor ( ccRED );
	this->addChild ( pLabel2, 0, kTagSprite2 );

	CCActionInterval*  fade = CCFadeOut::create ( 1.0f );
	CCSequence*  seq = CCSequence::create ( fade, fade->reverse ( ), KD_NULL );
	pLabel2->runAction ( CCRepeatForever::create ( seq ) );	
	
	this->schedule ( schedule_selector ( LabelAtlasColorTest::step ) ); 
}

KDvoid LabelAtlasColorTest::step ( KDfloat fDelta )
{
	m_fTime += fDelta;
    
	CCLabelAtlas*  pLabel1 = (CCLabelAtlas*) getChildByTag ( kTagSprite1 );
	pLabel1->setString ( ccszf ( "%2.2f Test", m_fTime ) );

	CCLabelAtlas*  pLabel2 = (CCLabelAtlas*) getChildByTag ( kTagSprite2 );
	pLabel2->setString ( ccszf ( "%d", (KDint) m_fTime ) );
}

KDvoid LabelAtlasColorTest::actionFinishCallback ( KDvoid )
{
	CCLOG ( "Action finished" );
}

const KDchar* LabelAtlasColorTest::title ( KDvoid )
{
	return "CCLabelAtlas";
}

const KDchar* LabelAtlasColorTest::subtitle ( KDvoid )
{
	return "Opacity + Color should work at the same time";
}

//------------------------------------------------------------------
//
// LabelTTFAlignment
//
//------------------------------------------------------------------
KDvoid LabelTTFAlignment::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

	CCLabelTTF*  ttf0 = CCLabelTTF::create ( "Alignment 0\nnew line", "fonts/Marker Felt.ttf", 18, CCSize ( s.cx / 2, 32 ), kCCTextAlignmentLeft );
    ttf0->setPosition ( ccp ( s.cx / 2, s.cy / 3 ) );
    this->addChild ( ttf0 );

    CCLabelTTF*  ttf1 = CCLabelTTF::create ( "Alignment 1\nnew line", "fonts/Marker Felt.ttf", 18, CCSize ( s.cx / 2, 32 ), kCCTextAlignmentCenter );
    ttf1->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
    this->addChild ( ttf1 );

    CCLabelTTF*  ttf2 = CCLabelTTF::create ( "Alignment 2\nnew line", "fonts/Marker Felt.ttf", 18, CCSize ( s.cx / 2, 32 ), kCCTextAlignmentRight );
    ttf2->setPosition ( ccp ( s.cx / 2, s.cy / 3 * 2 ) );
    this->addChild ( ttf2 );
}

const KDchar* LabelTTFAlignment::title ( KDvoid )
{
    return "CCLabelTTF alignment";
}

const KDchar* LabelTTFAlignment::subtitle ( KDvoid )
{
    return "Tests alignment values";
}

//------------------------------------------------------------------
//
// Atlas3
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid Atlas3::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	m_fTime = 0;

	const CCSize&  s = this->getContentSize ( );	

	CCActionInterval*  fade = CCFadeOut ::create ( 1.0f );
	CCSequence*  seq = CCSequence::create ( fade, fade->reverse ( ), KD_NULL );
	CCAction*  repeat = CCRepeatForever::create ( seq );	
	
	CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "Test", "fonts/bitmapFontTest2.fnt" );

	// testing anchors
	pLabel1->setAnchorPoint ( ccp ( 0, 0 ) );
	pLabel1->setPosition ( ccp ( 0, 0 ) );

	this->addChild ( pLabel1, 0, kTagBitmapAtlas1 );
	pLabel1->runAction ( repeat );

	// VERY IMPORTANT
	// color and opacity work OK because bitmapFontAltas2 loads a BMP image ( not a PNG image )
	// If you want to use both opacity and color, it is recommended to use NON premultiplied images like BMP images
	// Of course, you can also tell XCode not to compress PNG images, but I think it doesn't work as expected
	CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( "Test", "fonts/bitmapFontTest2.fnt" );

	// testing anchors
	pLabel2->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabel2->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	pLabel2->setColor ( ccRED );
	this->addChild ( pLabel2, 0, kTagBitmapAtlas2 );
	pLabel2->runAction (  (CCAction*) ( repeat->copy ( )->autorelease ( ) ) );

	CCLabelBMFont*  pLabel3 = CCLabelBMFont::create ( "Test", "fonts/bitmapFontTest2.fnt" );
	
	// testing anchors
	pLabel3->setAnchorPoint ( ccp ( 1, 1 ) );
	pLabel3->setPosition ( ccp ( s.cx, s.cy ) );
	this->addChild ( pLabel3, 0, kTagBitmapAtlas3 );

	this->schedule ( schedule_selector ( Atlas3::step ) );
}

KDvoid Atlas3::step ( KDfloat fDelta )
{
	m_fTime += fDelta;
	
	CCLabelBMFont*  pLabel1 = (CCLabelBMFont*) getChildByTag ( kTagBitmapAtlas1 );
	pLabel1->setString ( ccszf ( "%2.2f Test j", m_fTime ) );
	
	CCLabelBMFont*  pLabel2 = (CCLabelBMFont*) getChildByTag ( kTagBitmapAtlas2 );
	pLabel2->setString ( ccszf ( "%2.2f Test j", m_fTime ) );
	
	CCLabelBMFont*  pLabel3 = (CCLabelBMFont*) getChildByTag ( kTagBitmapAtlas3 );
	pLabel3->setString ( ccszf ( "%2.2f Test j", m_fTime ) );
}

const KDchar* Atlas3::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* Atlas3::subtitle ( KDvoid )
{
	return "Testing alignment.\nTesting opacity + tint";
}

//------------------------------------------------------------------
//
// Atlas4
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid Atlas4::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	m_fTime = 0;

	const CCSize&  s = this->getContentSize ( );

	// Upper Label
	CCLabelBMFont*  pLabel = CCLabelBMFont::create ( "Bitmap Font Atlas", "fonts/bitmapFontTest.fnt" );
	pLabel->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	pLabel->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	this->addChild ( pLabel );

	CCAction*      rot_4ever = CCRepeatForever::create ( CCRotateBy::create ( 2, 360 ) );
	CCActionInterval*  scale = CCScaleBy::create ( 2, 1.5f );
	CCAction*    scale_4ever = CCRepeatForever::create ( CCSequence::create ( scale, scale->reverse ( ), KD_NULL ) );
	CCAction*    jump_4ever  = CCRepeatForever::create ( CCJumpBy::create ( 0.5f, CCPointZero, 60, 1 ) );
	CCSequence*  seq = CCSequence::create ( CCFadeOut::create ( 1 ), CCFadeIn::create ( 1 ), KD_NULL );
	CCAction*    fade_4ever = CCRepeatForever::create ( seq );

	CCSprite*  BChar = (CCSprite*) pLabel->getChildByTag ( 0 );
	BChar->runAction ( rot_4ever );
	BChar->runAction ( scale_4ever );

	CCSprite*  FChar = (CCSprite*) pLabel->getChildByTag ( 7 );
	FChar->runAction ( jump_4ever );

	CCSprite*  AChar = (CCSprite*) pLabel->getChildByTag ( 12 );
	AChar->runAction ( fade_4ever );		

	// Bottom Label
	CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( "00.0", "fonts/bitmapFontTest.fnt" );
	pLabel2->setPosition ( ccp ( s.cx / 2.0f, s.cy / 2 - 80 ) );
	this->addChild ( pLabel2, 0, kTagBitmapAtlas2 );

	CCSprite*  pLastChar = (CCSprite*) pLabel2->getChildByTag ( 3 );
	pLastChar->runAction (  (CCAction*) ( rot_4ever->copy ( )->autorelease ( ) ) );
	
	this->schedule ( schedule_selector ( Atlas4::step ), 0.1f );
}

KDvoid Atlas4::draw ( KDvoid )
{
	const CCSize&  s = this->getContentSize ( );

	ccDrawLine ( ccp (        0, s.cy / 2 ), ccp ( s.cx    , s.cy / 2 ) );
	ccDrawLine ( ccp ( s.cx / 2,        0 ), ccp ( s.cx / 2, s.cy     ) );
}

KDvoid Atlas4::step ( KDfloat fDelta )
{
	m_fTime += fDelta;
	
	CCLabelBMFont*  pLabel1 = (CCLabelBMFont*) getChildByTag ( kTagBitmapAtlas2 );
	pLabel1->setString ( ccszf ( "%04.1f", m_fTime ) );
}

const KDchar* Atlas4::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* Atlas4::subtitle ( KDvoid )
{
	return "Using fonts as CCSprite objects.\nSome characters should rotate.";
}

//------------------------------------------------------------------
//
// Atlas5
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid Atlas5::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCLabelBMFont*  pLabel = CCLabelBMFont::create ( "abcdefg", "fonts/bitmapFontTest4.fnt" );
	pLabel->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	pLabel->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	this->addChild ( pLabel );
}

const KDchar* Atlas5::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* Atlas5::subtitle ( KDvoid )
{
	return "Testing padding";
}

//------------------------------------------------------------------
//
// Atlas6
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid Atlas6::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "FaFeFiFoFu", "fonts/bitmapFontTest5.fnt" );
	pLabel1->setPosition ( ccp ( s.cx / 2, s.cy / 2 + 50 ) );
	pLabel1->setAnchorPoint ( ccp ( 0.5f, 0.5f ) ) ;
	this->addChild ( pLabel1 );
	
	CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( "fafefifofu", "fonts/bitmapFontTest5.fnt" );
	pLabel2->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	pLabel2->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	this->addChild ( pLabel2 );

	CCLabelBMFont*  pLabel3 = CCLabelBMFont::create ( "aeiou", "fonts/bitmapFontTest5.fnt" );
	pLabel3->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 50 ) );
	pLabel3->setAnchorPoint ( ccp ( 0.5f, 0.5f ) ); 
	this->addChild ( pLabel3 );
}

const KDchar* Atlas6::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* Atlas6::subtitle ( KDvoid )
{
	return "Rendering should be OK.\nTesting offset";
}

//------------------------------------------------------------------
//
// AtlasBitmapColor
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid AtlasBitmapColor::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
	
	CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "Blue", "fonts/bitmapFontTest5.fnt" );
	pLabel1->setPosition ( ccp ( s.cx / 2, s.cy / 2 + 50 ) );
	pLabel1->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabel1->setColor ( ccBLUE );
	this->addChild ( pLabel1 );

	CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( "Red", "fonts/bitmapFontTest5.fnt" );
	pLabel2->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	pLabel2->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabel2->setColor ( ccRED );
	this->addChild ( pLabel2 );

	CCLabelBMFont*  pLabel3 = CCLabelBMFont::create ( "G", "fonts/bitmapFontTest5.fnt" );
	pLabel3->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 50 ) );
	pLabel3->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabel3->setColor ( ccGREEN );
	pLabel3->setString ( "Green" );
	this->addChild ( pLabel3 );
}

const KDchar* AtlasBitmapColor::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* AtlasBitmapColor::subtitle ( KDvoid )
{
	return "Testing color";
}

//------------------------------------------------------------------
//
// AtlasFastBitmap
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid AtlasFastBitmap::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	// Upper Label
	for ( KDint i = 0; i < 100; i++ ) 
	{
		CCLabelBMFont*  pLabel = CCLabelBMFont::create ( ccszf ( "-%d-", i ), "fonts/bitmapFontTest.fnt" );
		pLabel->setPosition ( ccp ( CCRANDOM_0_1 ( ) * s.cx, CCRANDOM_0_1 ( ) * s.cy ) );
		pLabel->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
		this->addChild ( pLabel );
	}
}

const KDchar* AtlasFastBitmap::title ( KDvoid )
{
	return "CCLabelBMFont";
}

const KDchar* AtlasFastBitmap::subtitle ( KDvoid )
{
	return "Creating several CCLabelBMFont with the same\n.fnt file should be fast";
}

//------------------------------------------------------------------
//
// BitmapFontMultiLine
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ ( Commercial, Mac OS X )
//     http://www.n4te.com/hiero/hiero.jnlp ( Free, Java )
//     http://slick.cokeandcode.com/demos/hiero.jnlp ( Free, Java )
//     http://www.angelcode.com/products/bmfont/ ( Free, Windows only )
//
//------------------------------------------------------------------
KDvoid BitmapFontMultiLine::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

    CCLabelBMFont*  pLeft = CCLabelBMFont::create ( "Multi line\nLeft", "fonts/bitmapFontTest3.fnt" );
	pLeft->setAnchorPoint ( ccp ( 0, 0 ) );
	pLeft->setPosition ( CCPointZero );
	this->addChild ( pLeft, 0, kTagBitmapAtlas1 );

    CCLabelBMFont*  pCenter = CCLabelBMFont::create ( "Multi line\nCenter", "fonts/bitmapFontTest3.fnt" );
	pCenter->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pCenter->setPosition ( ccpMid ( s ) );
	this->addChild ( pCenter, 0, kTagBitmapAtlas2 );

    CCLabelBMFont*  pRight = CCLabelBMFont::create ( "Multi line\nRight\nThree lines Three", "fonts/bitmapFontTest3.fnt" );
	pRight->setAnchorPoint ( ccp ( 1, 1 ) );
	pRight->setPosition ( s );
	this->addChild ( pRight, 0, kTagBitmapAtlas3 );
}

const KDchar* BitmapFontMultiLine::title ( KDvoid )
{
    return "CCLabelBMFont";
}

const KDchar* BitmapFontMultiLine::subtitle ( KDvoid )
{
    return "Multiline + anchor point";
}

//------------------------------------------------------------------
//
// LabelsEmpty
//
//------------------------------------------------------------------
KDvoid LabelsEmpty::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	m_bSetEmpty = KD_FALSE;

    CCSize   s = this->getContentSize ( );

    // CCLabelBMFont
    CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "", "fonts/bitmapFontTest3.fnt" );
	pLabel1->setPosition ( ccp ( s.cx / 2, s.cy / 3 * 2 ) );
	this->addChild ( pLabel1, 0, kTagBitmapAtlas1 );

    // CCLabelTTF
    CCLabelTTF*  pLabel2 = CCLabelTTF::create ( "", "fonts/Marker Felt.ttf", 24 );
	pLabel2->setPosition ( ccpMid ( s ) );
	this->addChild ( pLabel2, 0, kTagBitmapAtlas2 );

    // CCLabelAtlas
    CCLabelAtlas*  pLabel3 = CCLabelAtlas::create ( "", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ' );
	pLabel3->setPosition ( ccp ( s.cx / 2, s.cy / 3 ) );
	this->addChild ( pLabel3, 0, kTagBitmapAtlas3 );

    this->schedule ( schedule_selector ( LabelsEmpty::updateStrings ), 1.0f );
}

KDvoid LabelsEmpty::updateStrings ( KDfloat dt )
{
    CCLabelBMFont* pLabel1 = (CCLabelBMFont*) getChildByTag ( kTagBitmapAtlas1 );
    CCLabelTTF*    pLabel2 = (CCLabelTTF*)    getChildByTag ( kTagBitmapAtlas2 );
    CCLabelAtlas*  pLabel3 = (CCLabelAtlas*)  getChildByTag ( kTagBitmapAtlas3 );

    if ( !m_bSetEmpty )
    {
		pLabel1->setString ( "not empty" );
		pLabel2->setString ( "not empty" );
		pLabel3->setString ( "hi" );

		m_bSetEmpty = KD_TRUE;
    }
    else
    {
		pLabel1->setString ( "" );
		pLabel2->setString ( "" );
		pLabel3->setString ( "" );

		m_bSetEmpty = KD_FALSE;
    }
}

const KDchar* LabelsEmpty::title ( KDvoid )
{
    return "Testing empty labels";
}

const KDchar* LabelsEmpty::subtitle ( KDvoid )
{
    return "3 empty labels : LabelAtlas, LabelTTF and LabelBMFont";
}

//------------------------------------------------------------------
//
// LabelBMFontHD
//
//------------------------------------------------------------------
KDvoid LabelBMFontHD::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    CCSize   s = this->getContentSize ( );

    // CCLabelBMFont
    CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "TESTING RETINA DISPLAY", "fonts/konqa32.fnt" );
	pLabel1->setPosition ( ccpMid ( s ) );
	this->addChild ( pLabel1 );
}

const KDchar* LabelBMFontHD::title ( KDvoid )
{
    return "Testing Retina Display BMFont";
}

const KDchar* LabelBMFontHD::subtitle ( KDvoid )
{
    return "loading arista16 or arista16-hd";
}

//------------------------------------------------------------------
//
// LabelAtlasHD
//
//------------------------------------------------------------------
KDvoid LabelAtlasHD::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    // CCLabelBMFont
    CCLabelAtlas*  pLabel1 = CCLabelAtlas::create ( "TESTING RETINA DISPLAY", "fonts/larabie-16.png", 10, 20, 'A' );
	pLabel1->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabel1->setPosition ( ccpMid ( s ) );
	this->addChild ( pLabel1 );
}

const KDchar* LabelAtlasHD::title ( KDvoid )
{
    return "LabelAtlas with Retina Display";
}

const KDchar* LabelAtlasHD::subtitle ( KDvoid )
{
    return "loading larabie-16 / larabie-16-hd";
}

//------------------------------------------------------------------
//
// LabelGlyphDesigner
//
//------------------------------------------------------------------
KDvoid LabelGlyphDesigner::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    // CCLabelBMFont
    CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "Testing Glyph Designer", "fonts/futura-48.fnt" );
	pLabel1->setPosition ( ccpMid ( s ) );
	this->addChild ( pLabel1 );
}

const KDchar* LabelGlyphDesigner::title ( KDvoid )
{
    return "Testing Glyph Designer";
}

const KDchar* LabelGlyphDesigner::subtitle ( KDvoid )
{
    return "You should see a font\nwith shawdows and outline";
}

//------------------------------------------------------------------
//
// LabelTTFTest
//
//------------------------------------------------------------------
KDvoid LabelTTFTest::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

    m_pBG = CCLayerColor::create ( ccc4 ( 32, 32, 255, 64 ), ccpMid ( s ) );
	m_pBG->ignoreAnchorPointForPosition ( KD_FALSE );
    m_pBG->setPosition ( ccpMid ( s ) );
	this->addChild ( m_pBG );

    CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );

    CCMenu*  pMenu = CCMenu::create
	(
        CCMenuItemFont::create ( "Left"  , this, menu_selector ( LabelTTFTest::setAlignmentLeft   ) ),
        CCMenuItemFont::create ( "Center", this, menu_selector ( LabelTTFTest::setAlignmentCenter ) ),
        CCMenuItemFont::create ( "Right" , this, menu_selector ( LabelTTFTest::setAlignmentRight  ) ),
        NULL
	);
    pMenu->alignItemsVerticallyWithPadding ( 4 );
    pMenu->setPosition ( ccp ( 50, s.cy / 2 ) );
    this->addChild ( pMenu );

    pMenu = CCMenu::create 
	(
        CCMenuItemFont::create ( "Top"   , this, menu_selector ( LabelTTFTest::setAlignmentTop    ) ),
        CCMenuItemFont::create ( "Middle", this, menu_selector ( LabelTTFTest::setAlignmentMiddle ) ),
        CCMenuItemFont::create ( "Bottom", this, menu_selector ( LabelTTFTest::setAlignmentBottom ) ),
        NULL
	);
    pMenu->alignItemsVerticallyWithPadding ( 4 );
    pMenu->setPosition ( ccp ( s.cx - 50, s.cy / 2 ) );
    this->addChild ( pMenu );

    m_eHorizAlign = kCCTextAlignmentLeft;
    m_eVertAlign  = kCCVerticalTextAlignmentTop;

	this->updateAlignment ( );
}

const KDchar* LabelTTFTest::title ( KDvoid )
{
	return "Testing CCLabelTTF";
}

const KDchar* LabelTTFTest::subtitle ( KDvoid )
{
	return "Select the buttons on the sides to change alignment";
}

KDvoid LabelTTFTest::setAlignmentLeft ( CCObject* pSender )
{
    m_eHorizAlign = kCCTextAlignmentLeft;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::setAlignmentCenter ( CCObject* pSender )
{
    m_eHorizAlign = kCCTextAlignmentCenter;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::setAlignmentRight ( CCObject* pSender )
{
    m_eHorizAlign = kCCTextAlignmentRight;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::setAlignmentTop ( CCObject* pSender )
{
    m_eVertAlign = kCCVerticalTextAlignmentTop;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::setAlignmentMiddle ( CCObject* pSender )
{
    m_eVertAlign = kCCVerticalTextAlignmentCenter;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::setAlignmentBottom ( CCObject* pSender )
{
    m_eVertAlign = kCCVerticalTextAlignmentBottom;
    this->updateAlignment ( );
}

KDvoid LabelTTFTest::updateAlignment ( KDvoid )
{
	m_pBG->removeAllChildrenWithCleanup ( KD_TRUE );

	CCLabelTTF*  pLabel = CCLabelTTF::create ( this->getCurrentAlignment ( ), "fonts/Marker Felt.ttf", 26, m_pBG->getContentSize ( ), m_eHorizAlign, m_eVertAlign );
	pLabel->setPosition ( ccpMid ( m_pBG->getContentSize ( ) ) );
	m_pBG->addChild ( pLabel );
}

const KDchar* LabelTTFTest::getCurrentAlignment ( KDvoid )
{
    const KDchar*  szVert = NULL;
    const KDchar*  szHori = NULL;
    
	switch (m_eVertAlign) 
	{
        case kCCVerticalTextAlignmentTop    : szVert = "Top";    break;
        case kCCVerticalTextAlignmentCenter : szVert = "Middle"; break;
        case kCCVerticalTextAlignmentBottom : szVert = "Bottom"; break;
    }

    switch ( m_eHorizAlign ) 
	{
        case kCCTextAlignmentLeft   : szHori = "Left"; break;
        case kCCTextAlignmentCenter : szHori = "Center"; break;
        case kCCTextAlignmentRight  : szHori = "Right"; break;
    }

    return ccszf ( "Alignment %s %s", szVert, szHori );
}

//------------------------------------------------------------------
//
// LabelTTFMultiline
//
//------------------------------------------------------------------
KDvoid LabelTTFMultiline::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCLabelTTF*  pCenter = CCLabelTTF::create
	( 
		"word wrap \"testing\" (bla0) bla1 'bla2' [bla3] (bla4) {bla5} {bla6} [bla7] (bla8) [bla9] 'bla0' \"bla1\"",
		"fonts/American Typewriter.ttf", 26, CCSize ( s.cx / 3, 200 ), kCCTextAlignmentCenter
	);
	pCenter->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 20 ) );
	this->addChild ( pCenter );
}

const KDchar* LabelTTFMultiline::title ( KDvoid )
{
	return "Testing CCLabelTTF Word Wrap";
}

const KDchar* LabelTTFMultiline::subtitle ( KDvoid )
{
	return "Word wrap using CCLabelTTF";
}

//------------------------------------------------------------------
//
// LabelTTFChinese
//
//------------------------------------------------------------------

static const KDchar  l_szUTF8s [2][32] = 
#ifdef _MSC_VER
#pragma warning( disable: 4309 )
{	
	{ 0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0x00 },
	{ 0xed, 0x95, 0x9c, 0xea, 0xb8, 0x80, 0x00 } 
};
#else
{ "中国", "한글" };
#endif

KDvoid LabelTTFChinese::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    CCLabelTTF*  pLabel = CCLabelTTF::create ( l_szUTF8s [ 1 ], "xm_supports/NanumGothicBold.ttf", 32 );   
	pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );    
	this->addChild ( pLabel );
}

const KDchar* LabelTTFChinese::title ( KDvoid )
{
    return "Testing CCLabelTTF with Chinese character";
}

//------------------------------------------------------------------
//
// LabelBMFontChinese
//
//------------------------------------------------------------------
KDvoid LabelBMFontChinese::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    CCLabelBMFont*  pLabel = CCLabelBMFont::create ( l_szUTF8s [ 0 ], "fonts/bitmapFontChinese.fnt" );
	pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );    
	this->addChild ( pLabel );
}

const KDchar* LabelBMFontChinese::title ( KDvoid )
{
    return "Testing CCLabelBMFont with Chinese character";
}

//------------------------------------------------------------------
//
// BitmapFontMultiLineAlignment
//
//------------------------------------------------------------------

#define LongSentencesExample	"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
#define LineBreaksExample		"Lorem ipsum dolor\nsit amet\nconsectetur adipisicing elit\nblah\nblah"
#define MixedExample			"ABC\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt\nDEF"

#define ArrowsMax				0.95f
#define ArrowsMin				0.7f

#define LeftAlign				0
#define CenterAlign				1
#define RightAlign				2

#define LongSentences			0
#define LineBreaks				1
#define Mixed					2

static const KDfloat	alignmentItemPadding	= 50;
static const KDfloat	menuItemPaddingCenter	= 50;

KDvoid BitmapFontMultiLineAlignment::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    this->setTouchEnabled ( KD_TRUE );

    // ask director the the window size
    const CCSize&  s = this->getContentSize ( );

    // create and initialize a Label
    this->m_pLabelShouldRetain = CCLabelBMFont::create ( LongSentencesExample, "fonts/markerFelt.fnt", s.cx / 1.5f, kCCTextAlignmentCenter );
    this->m_pLabelShouldRetain->retain ( );

    this->m_pArrowsBarShouldRetain = CCSprite::create ( "Images/arrowsBar.png" );
    this->m_pArrowsBarShouldRetain->retain ( );
    this->m_pArrowsShouldRetain = CCSprite::create ( "Images/arrows.png" );
    this->m_pArrowsShouldRetain->retain ( );

    CCMenuItemFont::setFontSize ( 20 );
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );

    CCMenuItemFont*  pLongSentences = CCMenuItemFont::create ( "Long Flowing Sentences", this, menu_selector ( BitmapFontMultiLineAlignment::stringChanged ) );
    CCMenuItemFont*  pLineBreaks	= CCMenuItemFont::create ( "Short Sentences With Intentional Line Breaks", this, menu_selector ( BitmapFontMultiLineAlignment::stringChanged ) );
    CCMenuItemFont*  pMixed			= CCMenuItemFont::create ( "Long Sentences Mixed With Intentional Line Breaks", this, menu_selector ( BitmapFontMultiLineAlignment::stringChanged ) );
    CCMenu*          pStringMenu	= CCMenu::create ( pLongSentences, pLineBreaks, pMixed, KD_NULL );
    pStringMenu->alignItemsVertically ( );

    pLongSentences->setColor ( ccRED );
    m_pLastSentenceItem = pLongSentences;
    pLongSentences->setTag ( LongSentences );
    pLineBreaks->setTag ( LineBreaks );
    pMixed->setTag ( Mixed );

    CCMenuItemFont::setFontSize ( 30 );

    CCMenuItemFont*  pLeft   = CCMenuItemFont::create ( "Left"  , this, menu_selector ( BitmapFontMultiLineAlignment::alignmentChanged ) );
    CCMenuItemFont*  pCenter = CCMenuItemFont::create ( "Center", this, menu_selector ( BitmapFontMultiLineAlignment::alignmentChanged ) );
    CCMenuItemFont*  pRight  = CCMenuItemFont::create ( "Right" , this, menu_selector ( BitmapFontMultiLineAlignment::alignmentChanged ) );
    CCMenu*   pAlignmentMenu = CCMenu::create ( pLeft, pCenter, pRight, KD_NULL );
    pAlignmentMenu->alignItemsHorizontallyWithPadding ( alignmentItemPadding );

    pCenter->setColor ( ccRED );
    m_pLastAlignmentItem = pCenter;
    pLeft  ->setTag ( LeftAlign );
    pCenter->setTag ( CenterAlign );
    pRight ->setTag ( RightAlign );

    // position the label on the center of the screen
    this->m_pLabelShouldRetain->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
    this->m_pArrowsBarShouldRetain->setVisible ( KD_FALSE );

    KDfloat  fArrowsWidth = ( ArrowsMax - ArrowsMin ) * s.cx;
    this->m_pArrowsBarShouldRetain->setScaleX ( fArrowsWidth / this->m_pArrowsBarShouldRetain->getContentSize ( ).width );
    this->m_pArrowsBarShouldRetain->setPosition ( ccp ( ( ( ArrowsMax + ArrowsMin ) / 2 ) * s.cx, this->m_pLabelShouldRetain->getPosition ( ).y ) );

    this->snapArrowsToEdge ( );

    pStringMenu->setPosition ( ccp ( s.cx / 2, s.cy - menuItemPaddingCenter - 80 ) );
    pAlignmentMenu->setPosition ( ccp ( s.cx / 2, menuItemPaddingCenter + 60 ) );

    this->addChild ( this->m_pLabelShouldRetain );
    this->addChild ( this->m_pArrowsBarShouldRetain );
    this->addChild ( this->m_pArrowsShouldRetain );
    this->addChild ( pStringMenu );
    this->addChild ( pAlignmentMenu );
}

KDvoid BitmapFontMultiLineAlignment::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLabelShouldRetain );
	CC_SAFE_RELEASE ( m_pArrowsBarShouldRetain );
	CC_SAFE_RELEASE ( m_pArrowsShouldRetain );

	TestLabel::onExit ( );
}

const KDchar* BitmapFontMultiLineAlignment::title ( KDvoid )
{
    return "Bitmap Font Multi Line Alignment";
}

KDvoid BitmapFontMultiLineAlignment::stringChanged ( CCObject *sender )
{
    CCMenuItemFont*  pItem = (CCMenuItemFont*) sender;
    pItem->setColor ( ccRED );

    this->m_pLastAlignmentItem->setColor ( ccWHITE );
    this->m_pLastAlignmentItem = pItem;

    switch ( pItem->getTag ( ) )
    {
		case LongSentences	:	this->m_pLabelShouldRetain->setString ( LongSentencesExample );	break;
		case LineBreaks		:	this->m_pLabelShouldRetain->setString ( LineBreaksExample );	break;
		case Mixed			:	this->m_pLabelShouldRetain->setString ( MixedExample );			break;
    }

    this->snapArrowsToEdge ( );
}

KDvoid BitmapFontMultiLineAlignment::alignmentChanged ( CCObject* pSender )
{
    CCMenuItemFont*  pItem = (CCMenuItemFont*) pSender;

    pItem->setColor ( ccRED );
    this->m_pLastAlignmentItem->setColor ( ccWHITE );
    this->m_pLastAlignmentItem = pItem;

    switch ( pItem->getTag ( ) )
    {
		case LeftAlign		:	this->m_pLabelShouldRetain->setAlignment ( kCCTextAlignmentLeft   );	break;
		case CenterAlign	:	this->m_pLabelShouldRetain->setAlignment ( kCCTextAlignmentCenter );	break;
		case RightAlign		:	this->m_pLabelShouldRetain->setAlignment ( kCCTextAlignmentRight  );	break;
    }

    this->snapArrowsToEdge ( );
}

KDvoid BitmapFontMultiLineAlignment::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
    CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );

    if ( this->m_pArrowsShouldRetain->boundingBox ( ).containsPoint ( pTouch->getLocation ( ) ) )
    {
        m_bDrag = KD_TRUE;
        this->m_pArrowsBarShouldRetain->setVisible ( KD_TRUE );
    }
}

KDvoid BitmapFontMultiLineAlignment::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    m_bDrag = KD_FALSE;

    this->snapArrowsToEdge ( );

    this->m_pArrowsBarShouldRetain->setVisible ( KD_FALSE );
}

KDvoid BitmapFontMultiLineAlignment::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( !m_bDrag )
    {
        return;
    }

    CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint   tLocation = pTouch->getLocation ( );

    const CCSize&  tLyrSize = this->getContentSize ( );

    this->m_pArrowsShouldRetain->setPosition 
	(
		ccp ( KD_MAX ( KD_MIN ( tLocation.x, ArrowsMax * tLyrSize.cx ), ArrowsMin * tLyrSize.cx ), this->m_pArrowsShouldRetain->getPosition ( ).y ) 
	);

    KDfloat  fLabelW = kdFabsf ( this->m_pArrowsShouldRetain->getPosition ( ).x - this->m_pLabelShouldRetain->getPosition ( ).x ) * 2;
    this->m_pLabelShouldRetain->setWidth ( fLabelW );
}

KDvoid BitmapFontMultiLineAlignment::snapArrowsToEdge ( KDvoid )
{
    this->m_pArrowsShouldRetain->setPosition 
	(
		ccp ( this->m_pLabelShouldRetain->getPosition ( ).x + this->m_pLabelShouldRetain->getContentSize ( ).cx / 2, this->m_pLabelShouldRetain->getPosition ( ).y ) 
	);
}

//------------------------------------------------------------------
//
// LabelTTFA8Test
//
//------------------------------------------------------------------
KDvoid LabelTTFA8Test::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    // CCLabelBMFont
    CCLabelTTF*  pLabel1 = CCLabelTTF::create ( "Testing A8 Format", "fonts/Marker Felt.ttf", 48 );
    this->addChild ( pLabel1 );
    pLabel1->setColor ( ccRED );
    pLabel1->setPosition ( ccp ( s.width/2, s.height/2 ) );

    CCFadeOut*			fadeOut = CCFadeOut ::create ( 2 );
    CCFadeIn*			fadeIn	= CCFadeIn  ::create ( 2 );
    CCSequence*			seq		= CCSequence::create ( fadeOut, fadeIn, KD_NULL );
    CCRepeatForever*	forever = CCRepeatForever::create ( seq );
    pLabel1->runAction ( forever );
}

const KDchar* LabelTTFA8Test::title ( KDvoid )
{
    return "Testing A8 Format";
}

const KDchar* LabelTTFA8Test::subtitle ( KDvoid )
{
    return "RED label, fading In and Out in the center of the screen";
}

//------------------------------------------------------------------
//
// BMFontOneAtlas
//
//------------------------------------------------------------------
KDvoid BMFontOneAtlas::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( "This is Helvetica", "fonts/helvetica-32.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero );
    this->addChild ( pLabel1 );
    pLabel1->setPosition ( ccp ( s.cx / 2, s.cy / 2 + 50 ) );

    CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( "And this is Geneva", "fonts/geneva-32.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, ccp ( 0, 128 ) );
    this->addChild ( pLabel2 );
    pLabel2->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 50 ) );
}

const KDchar* BMFontOneAtlas::title ( KDvoid )
{
    return "CCLabelBMFont with one texture";
}

const KDchar* BMFontOneAtlas::subtitle ( KDvoid )
{
    return "Using 2 .fnt definitions that share the same texture atlas.";
}

//------------------------------------------------------------------
//
// BMFontUnicode
//
//------------------------------------------------------------------
KDvoid BMFontUnicode::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    CCDictionary*  pStrings = CCDictionary::createWithContentsOfFile ( "fonts/strings.xml" );

    const KDchar*  pChinese  = ( (CCString*) pStrings->objectForKey ( "chinese1" ) )->getCString ( );
    const KDchar*  pJapanese = ( (CCString*) pStrings->objectForKey ( "japanese" ) )->getCString ( );
    const KDchar*  pSpanish  = ( (CCString*) pStrings->objectForKey ( "spanish"  ) )->getCString ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelBMFont*  pLabel1 = CCLabelBMFont::create ( pSpanish, "fonts/arial-unicode-26.fnt", 200, kCCTextAlignmentLeft );
    this->addChild ( pLabel1 );
    pLabel1->setPosition ( ccp ( s.cx / 2, s.cy / 3 * 2 ) );

    CCLabelBMFont*  pLabel2 = CCLabelBMFont::create ( pChinese, "fonts/arial-unicode-26.fnt" );
    this->addChild ( pLabel2 );
    pLabel2->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );

    CCLabelBMFont*  pLabel3 = CCLabelBMFont::create ( pJapanese, "fonts/arial-unicode-26.fnt" );
    this->addChild ( pLabel3 );
    pLabel3->setPosition ( ccp ( s.cx / 2, s.cy / 3 * 1 ) );
}

const KDchar* BMFontUnicode::title ( KDvoid )
{
    return "CCLabelBMFont with Unicode support";
}

const KDchar* BMFontUnicode::subtitle ( KDvoid )
{
    return "You should see 3 differnt labels: In Spanish, Chinese and Korean";
}

//------------------------------------------------------------------
//
// BMFontInit
//
//------------------------------------------------------------------
KDvoid BMFontInit::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelBMFont*  pBmFont = new CCLabelBMFont ( );
    pBmFont->init ( );
    pBmFont->autorelease ( );
    pBmFont->setFntFile ( "fonts/helvetica-32.fnt" );
    pBmFont->setString ( "It is working!" );    
    pBmFont->setPosition ( ccpMid ( s ) );
	this->addChild ( pBmFont );
}

const KDchar* BMFontInit::title ( KDvoid )
{
    return "CCLabelBMFont init";
}

const KDchar* BMFontInit::subtitle ( KDvoid )
{
    return "Test for support of init method without parameters.";
}

//------------------------------------------------------------------
//
// TTFFontInit
//
//------------------------------------------------------------------
KDvoid TTFFontInit::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  pFont = new CCLabelTTF ( );
    pFont->init ( );
    pFont->autorelease ( );
    pFont->setFontName ( "fonts/Marker Felt.ttf" );
    pFont->setFontSize ( 48 );
    pFont->setString ( "It is working!" );    
    pFont->setPosition ( ccpMid ( s ) );
	this->addChild ( pFont );
}

const KDchar* TTFFontInit::title ( KDvoid )
{
    return "CCLabelTTF init";
}

const KDchar* TTFFontInit::subtitle ( KDvoid )
{
    return "Test for support of init method without parameters.";
}

//------------------------------------------------------------------
//
// LabelBMFontBounds
//
//------------------------------------------------------------------
KDvoid LabelBMFontBounds::onEnter ( KDvoid )
{
	TestLabel::onEnter ( );

    // CCLabelBMFont
    m_pLabel1 = CCLabelBMFont::create ( "Testing Glyph Designer", "fonts/boundsTestFont.fnt" );
    this->addChild ( m_pLabel1 );
    m_pLabel1->setPosition ( ccpMid ( this->getContentSize ( ) ) );
}

const KDchar* LabelBMFontBounds::title ( KDvoid )
{
    return "Testing LabelBMFont Bounds";
}

const KDchar* LabelBMFontBounds::subtitle ( KDvoid )
{
    return "You should see string enclosed by a box";
}

KDvoid LabelBMFontBounds::draw ( KDvoid )
{        
	const CCSize&  tSize = m_pLabel1->getContentSize ( );
	      CCSize  tOrgin = this->getContentSize ( );
    
    tOrgin.cx = tOrgin.cx * 0.5f - tSize.cx * 0.5f;
    tOrgin.cy = tOrgin.cy * 0.5f - tSize.cy * 0.5f;
    
    CCPoint  aVertices [ 4 ] =
    {
        ccp ( tOrgin.cx           , tOrgin.cy            ),
        ccp ( tOrgin.cx + tSize.cx, tOrgin.cy            ),
        ccp ( tOrgin.cx + tSize.cx, tOrgin.cy + tSize.cy ),
        ccp ( tOrgin.cx           , tOrgin.cy + tSize.cy )
    };

    ccDrawPoly ( aVertices, 4, KD_TRUE );
}