/* --------------------------------------------------------------------------
 *
 *      File            CCControlSaturationBrightnessPicker.cpp
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
#include "extensions/CCControl/CCControlSaturationBrightnessPicker.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCControlSaturationBrightnessPicker::CCControlSaturationBrightnessPicker ( KDvoid )
{
	m_fSaturation	= 0.0f;
	m_fBrightness	= 0.0f;
	m_pBackground	= KD_NULL;
	m_pOverlay		= KD_NULL;
	m_pShadow		= KD_NULL;
	m_pSlider		= KD_NULL;
	m_nBoxPos		= 0;
	m_nBoxSize		= 0;
}

CCControlSaturationBrightnessPicker::~CCControlSaturationBrightnessPicker ( KDvoid )
{
    removeAllChildrenWithCleanup ( KD_TRUE );

    m_pBackground = KD_NULL;
    m_pOverlay    = KD_NULL;
    m_pShadow     = KD_NULL;
    m_pSlider     = KD_NULL;
}
    

CCControlSaturationBrightnessPicker* CCControlSaturationBrightnessPicker::create ( CCNode* pTarget, CCPoint tPos )
{
	CCControlSaturationBrightnessPicker*  pRet = new CCControlSaturationBrightnessPicker ( );

	if ( pRet && pRet->initWithTargetAndPos ( pTarget, tPos ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCControlSaturationBrightnessPicker::initWithTargetAndPos ( CCNode* pTarget, CCPoint tPos )
{
	if ( CCControl::init ( ) )
    {
		setTouchEnabled ( KD_TRUE );

        // Add background and slider sprites
		m_pBackground = CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "colourPickerBackground.png", pTarget, tPos, ccp ( 0.0f, 0.0f ) );
		m_pOverlay	  = CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "colourPickerOverlay.png"	, pTarget, tPos, ccp ( 0.0f, 0.0f ) );
		m_pShadow	  = CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "colourPickerShadow.png"	, pTarget, tPos, ccp ( 0.0f, 0.0f ) );
		m_pSlider	  = CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "colourPicker.png"			, pTarget, tPos, ccp ( 0.5f, 0.5f ) );
				
		m_tStartPos   = tPos;														// starting position of the colour picker        
        m_nBoxPos     = 35;															// starting position of the virtual box area for picking a colour
        m_nBoxSize    = (KDint) ( m_pBackground->getContentSize ( ).width / 2 );	// the size (width and height) of the virtual box for picking a colour from
		return KD_TRUE;
    }
	
	return KD_FALSE;
}

KDvoid CCControlSaturationBrightnessPicker::updateWithHSV ( CCHsv tHsv )
{
    CCHsv   tHsvTemp;

    tHsvTemp.s = 1;
    tHsvTemp.h = tHsv.h;
    tHsvTemp.v = 1;
    
	CCRgb  tRgb = CCControlUtils::RGBfromHSV ( tHsvTemp );

	m_pBackground->setColor ( ccc3 ( (GLubyte) ( tRgb.r * 255.0f ), (GLubyte) ( tRgb.g * 255.0f ), (GLubyte) ( tRgb.b * 255.0f ) ) );
}

KDvoid CCControlSaturationBrightnessPicker::setEnabled ( KDbool bEnabled )
{
    CCControl::setEnabled ( bEnabled );

    if ( m_pSlider != NULL )
    {
        m_pSlider->setOpacity ( bEnabled ? 255 : 128 );
    }
}

KDvoid CCControlSaturationBrightnessPicker::updateDraggerWithHSV ( CCHsv tHsv )
{
    // Set the position of the slider to the correct saturation and brightness
	CCPoint  tPos = ccp ( m_tStartPos.x + m_nBoxPos + ( m_nBoxSize * ( 1 - tHsv.s ) ),
                          m_tStartPos.y + m_nBoxPos + ( m_nBoxSize * tHsv.v ) );
    
    // update
	updateSliderPosition ( tPos );
}

KDvoid CCControlSaturationBrightnessPicker::updateSliderPosition ( CCPoint tLocation )
{
    // Clamp the position of the icon within the circle
    
    // Get the center point of the bkgd image
    KDfloat  fCenterX = m_tStartPos.x + m_pBackground->boundingBox ( ).size.cx * 0.5f;
    KDfloat  fCenterY = m_tStartPos.y + m_pBackground->boundingBox ( ).size.cy * 0.5f;
    
    // Work out the distance difference between the location and center
    KDfloat  fDx	= tLocation.x - fCenterX;
    KDfloat  fDy	= tLocation.y - fCenterY;
    KDfloat  fDist	= kdSqrtf ( fDx * fDx + fDy * fDy );
    
    // Update angle by using the direction of the location
    KDfloat  fAngle = kdAtan2f ( fDy, fDx );
    
    // Set the limit to the slider movement within the colour picker
    KDfloat  fLimit = m_pBackground->boundingBox ( ).size.cx * 0.5f;
    
    // Check distance doesn't exceed the bounds of the circle
    if ( fDist > fLimit )
    {
        tLocation.x = fCenterX + fLimit * kdCosf ( fAngle );
        tLocation.y = fCenterY + fLimit * kdSinf ( fAngle );
    }
    
    // Set the position of the dragger
	m_pSlider->setPosition ( tLocation );
    
    
    // Clamp the position within the virtual box for colour selection
    if      ( tLocation.x < m_tStartPos.x + m_nBoxPos                  )	tLocation.x = m_tStartPos.x + m_nBoxPos;
    else if ( tLocation.x > m_tStartPos.x + m_nBoxPos + m_nBoxSize - 1 )	tLocation.x = m_tStartPos.x + m_nBoxPos + m_nBoxSize - 1;
    if      ( tLocation.y < m_tStartPos.y + m_nBoxPos                  )	tLocation.y = m_tStartPos.y + m_nBoxPos;
    else if ( tLocation.y > m_tStartPos.y + m_nBoxPos + m_nBoxSize     )	tLocation.y = m_tStartPos.y + m_nBoxPos + m_nBoxSize;
    
    // Use the position / slider width to determin the percentage the dragger is at
    m_fSaturation = 1.0f - kdFabsf ( ( m_tStartPos.x + (KDfloat) m_nBoxPos - tLocation.x ) / (KDfloat) m_nBoxSize );
    m_fBrightness = kdFabsf ( ( m_tStartPos.y + (KDfloat) m_nBoxPos - tLocation.y ) / (KDfloat) m_nBoxSize );
}

KDbool CCControlSaturationBrightnessPicker::checkSliderPosition ( CCPoint tLocation )
{
    // Clamp the position of the icon within the circle
    
    // get the center point of the bkgd image
    KDfloat  fCenterX = m_tStartPos.x + m_pBackground->boundingBox ( ).size.cx * 0.5f;
    KDfloat  fCenterY = m_tStartPos.y + m_pBackground->boundingBox ( ).size.cy * 0.5f;
    
    // work out the distance difference between the location and center
    KDfloat  fDx	= tLocation.x - fCenterX;
    KDfloat  fDy	= tLocation.y - fCenterY;
    KDfloat  fDist	= kdSqrtf ( fDx * fDx + fDy * fDy );
    
    // check that the touch location is within the bounding rectangle before sending updates
	if ( fDist <= m_pBackground->boundingBox ( ).size.cx * 0.5f )
    {
		updateSliderPosition ( tLocation );
		sendActionsForControlEvents ( kCCControlEventValueChanged );

		return KD_TRUE;
    }

	return KD_FALSE;
}

KDbool CCControlSaturationBrightnessPicker::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !isEnabled ( ) || !isVisible ( ) )
    {
        return KD_FALSE;
    }

	// Get the touch location
	CCPoint  tLocation = getTouchLocation ( pTouch );

    // Check the touch position on the slider
    return checkSliderPosition ( tLocation );
}

KDvoid CCControlSaturationBrightnessPicker::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	// Get the touch location
	CCPoint  tLocation = getTouchLocation ( pTouch );

	// Check the touch position on the slider
    checkSliderPosition ( tLocation );
}

NS_CC_END