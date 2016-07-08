/* --------------------------------------------------------------------------
 *
 *      File            TestBugAndSupport.cpp
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
#include "TestBugAndSupport.h"

TestBugAndSupport* TestBugAndSupport::create ( KDvoid )
{
	TestBugAndSupport*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new DeviceBrightness			( ); break;
		case  1 : pLayer = new SurfaceFullScreen		( ); break;
        case  2 : pLayer = new CCProgressTest           ( ); break;
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

KDuint TestBugAndSupport::count ( KDvoid )
{
	return 3;
}

//------------------------------------------------------------------
//
// DeviceBrightness
//
//------------------------------------------------------------------
KDvoid DeviceBrightness::onEnter ( KDvoid )
{
	TestBugAndSupport::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCControlSlider*  pSlider = CCControlSlider::create
	(
		"extensions/sliderTrack2.png","extensions/sliderProgress2.png" ,"extensions/sliderThumb.png" 
	);

	pSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( DeviceBrightness::sliderAction ), kCCControlEventValueChanged );

	// in case the parent view draws with a custom color or gradient, use a transparent color
	pSlider->setMinimumValue ( 0.0f );
	pSlider->setMaximumValue ( 1.0f );
	pSlider->setValue ( 0.5f );
	pSlider->setPosition ( ccpMid ( tLyrSize ) );

	this->addChild ( pSlider );
}

KDvoid DeviceBrightness::onExit ( KDvoid )
{
	kdSetBrightness ( -1.f );
	TestBugAndSupport::onExit ( );
}

const KDchar* DeviceBrightness::title ( KDvoid )
{
	return "Device Brightness";
}

const KDchar* DeviceBrightness::subtitle ( KDvoid )
{
	return "iOS and Android are supported.";
}

KDvoid DeviceBrightness::sliderAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
    CCControlSlider*  pSlider = (CCControlSlider*) pSender;
    KDfloat  fValue = pSlider->getValue ( );

	kdSetBrightness ( fValue );
}

//------------------------------------------------------------------
//
// SurfaceFullScreen
//
//------------------------------------------------------------------
KDvoid SurfaceFullScreen::onEnter ( KDvoid )
{
	TestBugAndSupport::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 32 );

	CCMenuItemToggle*  pToggle = CCMenuItemToggle::createWithTarget
	(
		this, 
		menu_selector ( SurfaceFullScreen::toggleCallback ), 
		CCMenuItemFont::create ( "Hide" ),
		CCMenuItemFont::create ( "Show" ),
		KD_NULL 
	);
	pToggle->setPosition ( ccpMid ( tLyrSize ) );	
	this->addChild ( CCMenu::createWithItem ( pToggle ) );	
}

const KDchar* SurfaceFullScreen::title ( KDvoid )
{
	return "Show or Hidde Status Bar";
}

const KDchar* SurfaceFullScreen::subtitle ( KDvoid )
{
	return "iOS and Android are supported.";
}

KDvoid SurfaceFullScreen::toggleCallback ( CCObject* pSender )
{
	CCMenuItemToggle*  pToggle = (CCMenuItemToggle*) pSender;

	if ( pToggle->getSelectedIndex ( ) == 1 )
	{
		kdSetFullScreen ( KD_TRUE );
	}
	else
	{
		kdSetFullScreen ( KD_FALSE );
	}
}


//------------------------------------------------------------------
//
// CCProgressTest
//
//------------------------------------------------------------------

const KDchar* CCProgressTest::subtitle ( KDvoid )
{
    return "CCProgress Test";
}

KDvoid CCProgressTest::onEnter ( KDvoid )
{
    TestBugAndSupport::onEnter ( );
    
    CCProgress*  uiPogress = CCProgress::create ( "xm_debug/etc_game_talkpop_02.9.png" );
    this->addChild ( uiPogress );
}