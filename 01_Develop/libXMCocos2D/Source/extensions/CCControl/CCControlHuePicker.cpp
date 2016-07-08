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
#include "extensions/CCControl/CCControlHuePicker.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCControlHuePicker::CCControlHuePicker ( KDvoid )
{
	m_fHue			 = 0.0f;
	m_fHuePercentage = 0.0f;
	m_pBackground	 = KD_NULL;
	m_pSlider		 = KD_NULL;
}

CCControlHuePicker::~CCControlHuePicker ( KDvoid )
{
    removeAllChildrenWithCleanup ( KD_TRUE );
    CC_SAFE_RELEASE ( m_pBackground );
    CC_SAFE_RELEASE ( m_pSlider );
}

CCControlHuePicker* CCControlHuePicker::create ( CCNode* pTarget, CCPoint tPos )
{
	CCControlHuePicker*  pRet = new CCControlHuePicker ( );
	
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

KDbool CCControlHuePicker::initWithTargetAndPos ( CCNode* pTarget, CCPoint tPos )
{
	if ( CCControl::init ( ) )
    {
		setTouchEnabled ( KD_TRUE );

        // Add background and slider sprites

        this->setBackground ( CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "huePickerBackground.png", pTarget, tPos, ccp ( 0.0f, 0.0f ) ) );
        this->setSlider     ( CCControlUtils::addSpriteToTargetWithPosAndAnchor ( "colourPicker.png"       , pTarget, tPos, ccp ( 0.5f, 0.5f ) ) );
        
		m_pSlider->setPosition ( ccp ( tPos.x, tPos.y + m_pBackground->boundingBox ( ).size.cy * 0.5f ) );
		m_tStartPos = tPos;

        // Sets the default value
        m_fHue = 0.0f;
		m_fHuePercentage = 0.0f;
		return KD_TRUE;
    }
	
	return KD_FALSE;
}

KDvoid CCControlHuePicker::setHue ( KDfloat fHueValue )
{
	m_fHue = fHueValue;
	// Set the position of the slider to the correct hue
    // We need to divide it by 360 as its taken as an angle in degrees
    KDfloat  fHuePercentage	= fHueValue / 360.0f;
    setHuePercentage ( fHuePercentage );
}

KDvoid CCControlHuePicker::setHuePercentage ( KDfloat fHueValueInPercent )
{
	m_fHuePercentage = fHueValueInPercent;
	m_fHue			 = m_fHuePercentage * 360.0f;

    // Clamp the position of the icon within the circle
	CCRect   tRect = m_pBackground->boundingBox ( );

	// Get the center point of the background image
    KDfloat  fCenterX = m_tStartPos.x + tRect.size.cx * 0.5f;
    KDfloat  fCenterY = m_tStartPos.y + tRect.size.cy * 0.5f;
    
    // Work out the limit to the distance of the picker when moving around the hue bar
    KDfloat  fLimit = tRect.size.width * 0.5f - 15.0f;
    
    // Update angle
    KDfloat  fAngleDeg  = m_fHuePercentage * 360.0f - 180.0f;
    KDfloat  fAngle		= CC_DEGREES_TO_RADIANS ( fAngleDeg );
    
    // Set new position of the slider
    KDfloat  fX = fCenterX + fLimit * kdCosf ( fAngle );
    KDfloat  fY = fCenterY + fLimit * kdSinf ( fAngle );
	m_pSlider->setPosition ( ccp ( fX, fY ) );

}

KDvoid CCControlHuePicker::setEnabled ( KDbool bEnabled )
{
    CCControl::setEnabled ( bEnabled );

    if ( m_pSlider != KD_NULL )
    {
        m_pSlider->setOpacity ( bEnabled ? 255 : 128 );
    }
}

KDvoid CCControlHuePicker::updateSliderPosition ( CCPoint tLocation )
{    
    // Get the center point of the background image
    KDfloat  fCenterX = m_tStartPos.x + m_pBackground->boundingBox ( ).size.cx * 0.5f;
    KDfloat  fCenterY = m_tStartPos.y + m_pBackground->boundingBox ( ).size.cy * 0.5f;

    // Work out the distance difference between the location and center
    KDfloat  fDx = tLocation.x - fCenterX;
    KDfloat  fDy = tLocation.y - fCenterY;
    
    // Update angle by using the direction of the location
    KDfloat  fAngle		= kdAtan2f ( fDy, fDx );
    KDfloat  fAngleDeg  = CC_RADIANS_TO_DEGREES ( fAngle ) + 180.0f;
    
    // use the position / slider width to determin the percentage the dragger is at
    setHue ( fAngleDeg );
    
	// send CCControl callback
	sendActionsForControlEvents ( kCCControlEventValueChanged );
}

KDbool CCControlHuePicker::checkSliderPosition ( CCPoint tLocation )
{
    // compute the distance between the current location and the center
    KDfloat  fDistance = kdSqrtf ( kdPowf ( tLocation.x + 10, 2 ) + kdPowf ( tLocation.y, 2 ) );
 
    // check that the touch location is within the circle
    if ( 80 > fDistance && fDistance > 59 )
    {
		updateSliderPosition ( tLocation );
		return KD_TRUE;
    }

	return KD_FALSE;
}

KDbool CCControlHuePicker::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
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

KDvoid CCControlHuePicker::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	// Get the touch location
	CCPoint  tLocation = getTouchLocation ( pTouch );

	// Check the touch position on the slider
	checkSliderPosition ( tLocation );
}

NS_CC_END