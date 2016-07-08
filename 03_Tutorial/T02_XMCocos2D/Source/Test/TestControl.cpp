/* --------------------------------------------------------------------------
 *
 *      File            TestControl.cpp
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
#include "TestControl.h"

TestControl* TestControl::create ( KDvoid )
{
	TestControl*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0	:	pLayer = new ControlSlider          ( );	break;
		case 1	:	pLayer = new ControlSwitch          ( );	break;			
		case 2	:	pLayer = new ControlColourPicker    ( );	break;
		case 3  :	pLayer = new ControlButton		    ( );	break;
		case 4  :	pLayer = new ControlButtonEvent		( );	break;
		case 5  :	pLayer = new ControlButtonStyling	( );	break;
		case 6	:	pLayer = new ControlPotentiometer	( );	break;
		case 7  :	pLayer = new ControlStepper			( );	break;
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

KDuint TestControl::count ( KDvoid )
{
	return 8;
}

//------------------------------------------------------------------
//
// ControlSlider
//
//------------------------------------------------------------------
KDbool ControlSlider::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
        
	m_pLabel = CCLabelTTF::create ( "Move the slider thumb!\nThe lower slider is restricted.", "fonts/Marker Felt.ttf", 24 );
	m_pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 40 ) );
	this->addChild ( m_pLabel );
	   
	CCControlSlider*  pSlider = CCControlSlider::create
	(
		"extensions/sliderTrack.png",
		"extensions/sliderProgress.png",
		"extensions/sliderThumb.png"
	);
	pSlider->setMinimumValue ( 0.0f );	// Sets the min value of range
	pSlider->setMaximumValue ( 5.0f );	// Sets the max value of range
    pSlider->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 10.f ) );
	pSlider->setTag ( 1 );

	pSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlSlider::onValueChanged ), kCCControlEventValueChanged );

	CCControlSlider*  pRestrictSlider = CCControlSlider::create 
	(
		"extensions/sliderTrack.png",
		"extensions/sliderProgress.png",
		"extensions/sliderThumb.png"
	);
    pRestrictSlider->setAnchorPoint ( ccp ( 0.5f, 1.0f ) );
    pRestrictSlider->setMinimumValue ( 0.0f ); // Sets the min value of range
    pRestrictSlider->setMaximumValue ( 5.0f ); // Sets the max value of range
	pRestrictSlider->setMaximumAllowedValue ( 4.0f );
	pRestrictSlider->setMinimumAllowedValue ( 1.5f );
	pRestrictSlider->setValue ( 3.0f );
    pRestrictSlider->setPosition ( ccp ( tLyrSize.cx / 2.0f, tLyrSize.cy / 2.0f - 30 ) );
	pRestrictSlider->setTag ( 2 );

	// same with restricted
	pRestrictSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlSlider::onValueChanged ), kCCControlEventValueChanged );

	this->addChild ( pSlider );
	this->addChild ( pRestrictSlider );
	
     
	return KD_TRUE;
}

const KDchar* ControlSlider::subtitle ( KDvoid )
{
    return "Control Slider";
}

KDvoid ControlSlider::onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	CCControlSlider*  pSlider = (CCControlSlider*) pSender;
	const KDchar*     szText = "";

	if ( pSlider->getTag ( ) == 1 )
	{
		szText = ccszf ( "Upper slider value = %.02f", pSlider->getValue ( ) );
	}
	else if( pSlider->getTag ( ) == 2 )
	{
		szText = ccszf ( "Lower slider value = %.02f", pSlider->getValue ( ) );
	}

	m_pLabel->setString ( szText );    
}

//------------------------------------------------------------------
//
// ControlSwitch
//
//------------------------------------------------------------------
KDbool ControlSwitch::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
        
	CCScale9Sprite*  pBG = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBG->setContentSize ( CCSize ( 100, 50 ) );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( pBG );

	m_pLabel = CCLabelTTF::create ( "Off", "fonts/Marker Felt.ttf", 24 );
	m_pLabel->setPosition ( pBG->getPosition ( ) );
	this->addChild ( m_pLabel );
 
	CCControlSwitch*  pSwitch = CCControlSwitch::create
	(
		CCSprite::create ( "extensions/switch-mask.png"  ),
		CCSprite::create ( "extensions/switch-on.png"    ),
		CCSprite::create ( "extensions/switch-off.png"   ),
		CCSprite::create ( "extensions/switch-thumb.png" ),
		CCLabelTTF::create ( "On" , "fonts/Marker Felt.ttf", 24 ),
		CCLabelTTF::create ( "Off", "fonts/Marker Felt.ttf", 24 )
	);
	pSwitch->setPosition ( ccp ( tLyrSize.cx / 2 + 100, tLyrSize.cy / 2 ) );
	pSwitch->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlSwitch::onSwitch ), kCCControlEventValueChanged );
	this->addChild ( pSwitch );

	onSwitch ( pSwitch, 0 );
     
	return KD_TRUE;
}

const KDchar* ControlSwitch::subtitle ( KDvoid )
{
    return "Control Switch";
}

KDvoid ControlSwitch::onSwitch ( CCObject* pSender, CCControlEvent uControlEvent )
{
	CCControlSwitch*  pSwitch = (CCControlSwitch*) pSender;

	m_pLabel->setString ( pSwitch->isOn ( ) ? "On" : "Off" );    
}

//------------------------------------------------------------------
//
// ControlColourPicker
//
//------------------------------------------------------------------
KDbool ControlColourPicker::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCScale9Sprite*  pBackground = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBackground->setContentSize ( CCSize ( 150, 50 ) );
	pBackground->setPosition ( ccp ( tLyrSize.cx / 2 + 90, tLyrSize.cy / 2 ) );
	this->addChild ( pBackground );

	m_pLabel = CCLabelTTF::create ( "#FFFFFF", "fonts/Marker Felt.ttf", 24 );
	m_pLabel->setPosition ( pBackground->getPosition ( ) );
	this->addChild ( m_pLabel );

	CCControlColourPicker*  pPicker = CCControlColourPicker::create 
	(
		"extensions/CCControlColourPickerSpriteSheet.plist",
		"extensions/CCControlColourPickerSpriteSheet.png"
	);
	this->addChild ( pPicker );
	pPicker->setColor ( ccc3 ( 37, 46, 252 ) );
	pPicker->setPosition ( ccp ( tLyrSize.cx / 2 - 90, tLyrSize.cy / 2 ) );
	pPicker->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlColourPicker::onColourValueChanged ), kCCControlEventValueChanged);

	onColourValueChanged ( pPicker, kCCControlEventValueChanged );
	
	return KD_TRUE;
}

const KDchar* ControlColourPicker::subtitle ( KDvoid )
{
    return "Control Colour Picker";
}

KDvoid ControlColourPicker::onColourValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	CCControlColourPicker*  pPicker = (CCControlColourPicker*) pSender;
	m_pLabel->setString ( ccszf ( "#%02X%02X%02X", pPicker->getColor ( ).r, pPicker->getColor ( ).g, pPicker->getColor ( ).b ) );
}

//------------------------------------------------------------------
//
// ControlButtonEvent
//
//------------------------------------------------------------------
KDbool ControlButtonEvent::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

    // Add the button
    CCScale9Sprite*  pBackgroundButton = CCScale9Sprite::create ( "extensions/button.png" );
    CCScale9Sprite*  pBackgroundHighlightedButton = CCScale9Sprite::create ( "extensions/buttonHighlighted.png" );
    
    CCLabelTTF*  pTitleButton = CCLabelTTF::create ( "Touch Me!", "fonts/Marker Felt.ttf", 30 );
    pTitleButton->setColor ( ccc3 ( 159, 168, 176 ) );
    
    CCControlButton*  pControlButton = CCControlButton::create ( pTitleButton, pBackgroundButton );
    pControlButton->setBackgroundSpriteForState ( pBackgroundHighlightedButton, kCCControlStateHighlighted );
    pControlButton->setTitleColorForState ( ccWHITE, kCCControlStateHighlighted );
    
    pControlButton->setAnchorPoint ( ccp ( 0.5f, 1 ) );
    pControlButton->setPosition ( ccpMid ( tLyrSize ) );
    addChild ( pControlButton, 1 );

    // Add the black background
    CCScale9Sprite*  pBackground = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
    pBackground->setContentSize ( CCSize ( 300, 170 ) );
    pBackground->setPosition ( ccpMid (  tLyrSize ) );
    addChild ( pBackground );
    
    // Sets up event handlers
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchDownAction	   ), kCCControlEventTouchDown		 );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchDragInsideAction ), kCCControlEventTouchDragInside  );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchDragOutsideAction), kCCControlEventTouchDragOutside );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchDragEnterAction  ), kCCControlEventTouchDragEnter   );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchDragExitAction   ), kCCControlEventTouchDragExit    );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchUpInsideAction   ), kCCControlEventTouchUpInside    );
    pControlButton->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlButtonEvent::touchUpOutsideAction  ), kCCControlEventTouchUpOutside   );

	m_pLabel = CCLabelTTF::create ( "No Event", "fonts/Marker Felt.ttf", 32 );
	m_pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 40 ) );
	this->addChild ( m_pLabel );

	return KD_TRUE;
}

const KDchar* ControlButtonEvent::subtitle ( KDvoid )
{
    return "Control Button Event";
}

KDvoid ControlButtonEvent::touchDownAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Touch Down" ) );
}

KDvoid ControlButtonEvent::touchDragInsideAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Drag Inside" ) );
}

KDvoid ControlButtonEvent::touchDragOutsideAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Drag Outside" ) );
}

KDvoid ControlButtonEvent::touchDragEnterAction	( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString (ccszf ( "Drag Enter" ) );
}

KDvoid ControlButtonEvent::touchDragExitAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Drag Exit" ) );
}

KDvoid ControlButtonEvent::touchUpInsideAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Touch Up Inside" ) );
}

KDvoid ControlButtonEvent::touchUpOutsideAction	( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_pLabel->setString ( ccszf ( "Touch Up Outside" ) );
}

//------------------------------------------------------------------
//
// ControlButton
//
//------------------------------------------------------------------
KDbool ControlButton::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	const KDchar*  aTitles [] = { "Hello", "Variable", "Size", "!" };

	KDfloat  fTotalWidth = 0, fHeight;

	CCLayer*  pLayer = CCLayer::create ( );
	this->addChild ( pLayer, 1 );

	for ( KDuint  i = 0; i < 4; i++ )
	{
		CCControlButton*  pButton = standardButtonWithTitle ( aTitles [ i ] );
		const CCSize&  tSize = pButton->getContentSize ( );

		pLayer->addChild ( pButton );
		pButton->setPosition ( ccp ( fTotalWidth + tSize.cx / 2, tSize.cy / 2 ) );

		fTotalWidth += tSize.cx;
		fHeight = tSize.cy;
	}

	pLayer->ignoreAnchorPointForPosition ( KD_FALSE );
	pLayer->setContentSize ( CCSize ( fTotalWidth, fHeight ) );
	pLayer->setPosition ( ccpMid ( tLyrSize ) );

	CCScale9Sprite*  pBackground = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBackground->setContentSize ( CCSize ( fTotalWidth + 14, fHeight + 14 ) );
	pBackground->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( pBackground );

	return KD_TRUE;
}

const KDchar* ControlButton::subtitle ( KDvoid )
{
    return "Control Button";
}

CCControlButton* ControlButton::standardButtonWithTitle ( const KDchar* szTitle )
{
	CCScale9Sprite*  pNormal = CCScale9Sprite::create ( "extensions/button.png" );
	CCScale9Sprite*  pSelect = CCScale9Sprite::create ( "extensions/buttonHighlighted.png" );
	CCLabelTTF*       pTitle = CCLabelTTF::create ( szTitle, "fonts/Marker Felt.ttf", 28 );

	pTitle->setColor ( ccc3 ( 159, 168, 176 ) );

	CCControlButton*  pButton = CCControlButton::create ( pTitle, pNormal );
	pButton->setBackgroundSpriteForState ( pSelect, kCCControlStateHighlighted );
	pButton->setTitleColorForState ( ccWHITE, kCCControlStateHighlighted );

	return pButton;
}

//------------------------------------------------------------------
//
// ControlButtonStyling
//
//------------------------------------------------------------------
KDbool ControlButtonStyling::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

    CCNode*  pLayer = CCNode::create ( );
    addChild ( pLayer, 1 );
    
    KDint   nSpace = 10; 
    KDfloat  fMaxW = 0, fMaxH = 0;

    for ( KDint i = 0; i < 3; i++ )
    {
        for ( KDint j = 0; j < 3; j++ )
        {
            // Add the buttons
            CCControlButton*  pButton = standardButtonWithTitle
			(
				CCString::createWithFormat ( "%d", kdRand ( ) % 30 )->getCString ( )
			);

            pButton->setAdjustBackgroundImage ( KD_FALSE );	// Tells the button that the background image must not be adjust
															// It'll use the prefered size of the background image
			const CCSize  tSize = pButton->getContentSize ( );
            pButton->setPosition ( ccp ( tSize.cx / 2 + ( tSize.cx + nSpace ) * i, tSize.cy / 2 + ( tSize.cy + nSpace ) * j ) );
            pLayer->addChild ( pButton );
            
            fMaxW = KD_MAX ( tSize.cx * ( i + 1 ) + nSpace * i, fMaxW );
            fMaxH = KD_MAX ( tSize.cy * ( j + 1 ) + nSpace * j, fMaxH );
        }
    }

	pLayer->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLayer->setContentSize ( CCSize ( fMaxW, fMaxH ) );
	pLayer->setPosition ( ccpMid ( tLyrSize ) );

	CCScale9Sprite*  pBackground = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBackground->setContentSize ( CCSize ( fMaxW + 14, fMaxH + 14 ) );
	pBackground->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( pBackground );

	return KD_TRUE;
}

const KDchar* ControlButtonStyling::subtitle ( KDvoid )
{
    return "Control Button Styling";
}

CCControlButton* ControlButtonStyling::standardButtonWithTitle ( const KDchar* szTitle )
{
	CCScale9Sprite*  pNormal = CCScale9Sprite::create ( "extensions/button.png" );
	pNormal->setPreferredSize ( CCSize ( 45, 45 ) );

	CCScale9Sprite*  pSelect = CCScale9Sprite::create ( "extensions/buttonHighlighted.png" );
	pSelect->setPreferredSize ( CCSize ( 45, 45 ) );

	CCLabelTTF*  pTitle = CCLabelTTF::create ( szTitle, "fonts/Marker Felt.ttf", 28 );
	pTitle->setColor ( ccc3 ( 159, 168, 176 ) );

	CCControlButton*  pButton = CCControlButton::create ( pTitle, pNormal );
	pButton->setBackgroundSpriteForState ( pSelect, kCCControlStateHighlighted );
	pButton->setTitleColorForState ( ccWHITE, kCCControlStateHighlighted );

	return pButton;
}

//------------------------------------------------------------------
//
// ControlPotentiometer
//
//------------------------------------------------------------------
KDbool ControlPotentiometer::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
        
	CCScale9Sprite*  pBG = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBG->setContentSize ( CCSize ( 100, 50 ) );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( pBG );

	m_pLabel = CCLabelTTF::create ( "Off", "fonts/Marker Felt.ttf", 24 );
	m_pLabel->setPosition ( pBG->getPosition ( ) );
	this->addChild ( m_pLabel );
 
	CCControlPotentiometer*  pPotentiometer = CCControlPotentiometer::create
	(
		"extensions/potentiometerTrack.png",
		"extensions/potentiometerProgress.png",
		"extensions/potentiometerButton.png"
	);
	pPotentiometer->setPosition ( ccp ( tLyrSize.cx / 2 + 100, tLyrSize.cy / 2 ) );
	pPotentiometer->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlPotentiometer::onValueChanged ), kCCControlEventValueChanged );
	this->addChild ( pPotentiometer );

	this->onValueChanged ( pPotentiometer, kCCControlEventValueChanged );
     
	return KD_TRUE;
}

const KDchar* ControlPotentiometer::subtitle ( KDvoid )
{
    return "Control Potentiometer";
}

KDvoid ControlPotentiometer::onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	CCControlPotentiometer*  pControl = (CCControlPotentiometer*) pSender;

	m_pLabel->setString ( ccszf ( "%0.02f", (KDfloat) pControl->getValue ( ) ) );    
}

//------------------------------------------------------------------
//
// ControlStepper
//
//------------------------------------------------------------------
KDbool ControlStepper::init ( KDvoid )
{
	if ( !TestControl::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
        
	CCScale9Sprite*  pBG = CCScale9Sprite::create ( "extensions/buttonBackground.png" );
	pBG->setContentSize ( CCSize ( 100, 50 ) );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( pBG );

	m_pLabel = CCLabelTTF::create ( "Off", "fonts/Marker Felt.ttf", 24 );
	m_pLabel->setPosition ( pBG->getPosition ( ) );
	this->addChild ( m_pLabel );
 
	CCControlStepper*  pStepper = CCControlStepper::create
	(
		CCSprite::create ( "extensions/stepper-minus.png" ),
		CCSprite::create ( "extensions/stepper-plus.png"  )
	);
	pStepper->setPosition ( ccp ( tLyrSize.cx / 2 + 100, tLyrSize.cy / 2 ) );
	pStepper->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ControlStepper::onValueChanged ), kCCControlEventValueChanged );
	this->addChild ( pStepper );

	this->onValueChanged ( pStepper, kCCControlEventValueChanged );
     
	return KD_TRUE;
}

const KDchar* ControlStepper::subtitle ( KDvoid )
{
    return "Control Stepper";
}

KDvoid ControlStepper::onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	CCControlStepper*  pControl = (CCControlStepper*) pSender;

	m_pLabel->setString ( ccszf ( "%0.02f", (KDfloat) pControl->getValue ( ) ) );    
}