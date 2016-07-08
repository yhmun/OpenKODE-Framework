/* --------------------------------------------------------------------------
 *
 *      File            CCControlHuePicker.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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
#include "extensions/CCControl/CCControlColourPicker.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCControlColourPicker::CCControlColourPicker ( KDvoid )
{
	m_pColourPicker = KD_NULL;
	m_pHuePicker	= KD_NULL;
	m_pBackground	= KD_NULL;
}

CCControlColourPicker::~CCControlColourPicker ( KDvoid )
{
    if ( m_pBackground )
    {
        m_pBackground->removeFromParentAndCleanup ( KD_TRUE );
    }
    
    if ( m_pHuePicker )
    {
        m_pHuePicker->removeFromParentAndCleanup ( KD_TRUE );
    }
    
    if ( m_pColourPicker )
    {
        m_pColourPicker->removeFromParentAndCleanup ( KD_TRUE );
    }
    
    m_pBackground     = KD_NULL;
    m_pHuePicker      = KD_NULL;
    m_pColourPicker   = KD_NULL;
}

KDbool CCControlColourPicker::init ( const KDchar* szSpriteSheet, const KDchar* szSpriteSheetImage )
{
	if ( CCControl::init ( ) )
	{
		setTouchEnabled ( KD_TRUE );

        // Cache the sprites
		CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( szSpriteSheet );
		
        // Create the sprite batch node
		CCSpriteBatchNode*  pSpriteSheet = CCSpriteBatchNode::create ( szSpriteSheetImage );
		addChild ( pSpriteSheet );      

        // Init default color
        m_tHsv.h = 0;
        m_tHsv.s = 0;
        m_tHsv.v = 0;
        
        // Add image
		m_pBackground = CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "menuColourPanelBackground.png", pSpriteSheet, CCPointZero, ccp ( 0.5f, 0.5f ) );            
		
		CCPoint  tBackgroundPointZero = ccpSub ( m_pBackground->getPosition ( ), ccpMid ( m_pBackground->getContentSize ( ) ) );
    
        // Setup panels
        KDfloat  fHueShift    = 8;
        KDfloat  fColourShift = 28;

        m_pHuePicker = new CCControlHuePicker ( );
        m_pHuePicker->initWithTargetAndPos ( pSpriteSheet, ccp ( tBackgroundPointZero.x + fHueShift, tBackgroundPointZero.y + fHueShift ) );

        m_pColourPicker = new CCControlSaturationBrightnessPicker ( );
        m_pColourPicker->initWithTargetAndPos ( pSpriteSheet, ccp ( tBackgroundPointZero.x + fColourShift, tBackgroundPointZero.y + fColourShift ) );
 
        // Setup events
		m_pHuePicker	->addTargetWithActionForControlEvents ( this, cccontrol_selector ( CCControlColourPicker::hueSliderValueChanged), kCCControlEventValueChanged );
		m_pColourPicker	->addTargetWithActionForControlEvents ( this, cccontrol_selector ( CCControlColourPicker::colourSliderValueChanged), kCCControlEventValueChanged );
       
        // Set defaults
		updateHueAndControlPicker ( );

		addChild ( m_pHuePicker );
		addChild ( m_pColourPicker );

		// Set content size
		setContentSize ( m_pBackground->getContentSize ( ) );

		return KD_TRUE;
	}
	
	return KD_FALSE;
}

CCControlColourPicker* CCControlColourPicker::create ( const KDchar* szSpriteSheet, const KDchar* szSpriteSheetImage )
{
	CCControlColourPicker*  pRet = new CCControlColourPicker ( );
	if ( pRet && pRet->init ( szSpriteSheet, szSpriteSheetImage ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	return pRet;
}

KDvoid CCControlColourPicker::setColor ( const ccColor3B& tColor )
{
	// XXX fixed me if not correct
    CCControl::setColor ( tColor );
    
    CCRgb  tRgb;
    tRgb.r = tColor.r / 255.0f;
    tRgb.g = tColor.g / 255.0f;
    tRgb.b = tColor.b / 255.0f;
    tRgb.a = 1.0f;
    
	m_tHsv = CCControlUtils::HSVfromRGB ( tRgb );
	updateHueAndControlPicker ( );
}

KDvoid CCControlColourPicker::setEnabled ( KDbool bEnabled )
{
    CCControl::setEnabled ( bEnabled );
    
	if ( m_pHuePicker != KD_NULL )
    {
        m_pHuePicker->setEnabled ( bEnabled );
    }

    if ( m_pColourPicker )
    {
        m_pColourPicker->setEnabled ( bEnabled );
    } 
}

// need two events to prevent an infinite loop! (can't update huePicker when the huePicker triggers the callback due to CCControlEventValueChanged)
KDvoid CCControlColourPicker::updateControlPicker ( KDvoid )
{
	m_pHuePicker->setHue ( (KDfloat) m_tHsv.h );
	m_pColourPicker->updateWithHSV ( m_tHsv );
}

KDvoid CCControlColourPicker::updateHueAndControlPicker ( KDvoid )
{
	m_pHuePicker->setHue ( (KDfloat) m_tHsv.h );
	m_pColourPicker->updateWithHSV ( m_tHsv );
	m_pColourPicker->updateDraggerWithHSV ( m_tHsv );
}

KDvoid CCControlColourPicker::hueSliderValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_tHsv.h = ( (CCControlHuePicker*) pSender )->getHue ( );

    // Update the value
	CCRgb  tRgb = CCControlUtils::RGBfromHSV ( m_tHsv );
	// XXX fixed me if not correct
    CCControl::setColor ( ccc3 ( (GLubyte) ( tRgb.r * 255.0f ), (GLubyte) ( tRgb.g * 255.0f ), (GLubyte) ( tRgb.b * 255.0f ) ) );
    
	// Send CCControl callback
	sendActionsForControlEvents ( kCCControlEventValueChanged );
	updateControlPicker ( );
}

KDvoid CCControlColourPicker::colourSliderValueChanged ( CCObject* pSender, CCControlEvent uControlEvent )
{
	m_tHsv.s = ( (CCControlSaturationBrightnessPicker*) pSender )->getSaturation ( );
	m_tHsv.v = ( (CCControlSaturationBrightnessPicker*) pSender )->getBrightness ( );

     // Update the value
	CCRgb  tRgb = CCControlUtils::RGBfromHSV ( m_tHsv );
	// XXX fixed me if not correct
	CCControl::setColor ( ccc3 ( (GLubyte) ( tRgb.r * 255.0f ), (GLubyte) ( tRgb.g * 255.0f ), (GLubyte) ( tRgb.b * 255.0f ) ) );
    
	// Send CCControl callback
	sendActionsForControlEvents ( kCCControlEventValueChanged );
}

//ignore all touches, handled by children
KDbool CCControlColourPicker::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_FALSE;
}

NS_CC_END