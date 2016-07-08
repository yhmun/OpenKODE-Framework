/* --------------------------------------------------------------------------
 *
 *      File            CCControlPotentiometer.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Yannick Loriot
 *
 *         http://www.cocos2d-x.org   
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
#include "extensions/CCControl/CCControlPotentiometer.h"

NS_CC_BEGIN

CCControlPotentiometer::CCControlPotentiometer ( KDvoid )
{
	m_fValue		 = 0.0f;
	m_fMinimumValue  = 0.0f;
	m_fMaximumValue  = 0.0f;
	m_pThumbSprite	 = KD_NULL;
	m_pProgressTimer = KD_NULL;
}

CCControlPotentiometer::~CCControlPotentiometer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pThumbSprite );
    CC_SAFE_RELEASE ( m_pProgressTimer );
}

CCControlPotentiometer* CCControlPotentiometer::create ( const KDchar* szBackgroundFile, const KDchar* szProgressFile, const KDchar* szThumbFile )
{
	return CCControlPotentiometer::create 
	(
		CCSprite	   ::create ( szBackgroundFile ),
		CCProgressTimer::create ( szProgressFile ),
		CCSprite	   ::create ( szThumbFile )
	);
}

CCControlPotentiometer* CCControlPotentiometer::create ( CCSprite* pBackgroundSprite, CCProgressTimer* pProgressTimer, CCSprite* pThumbSprite )
{
    CCControlPotentiometer*  pRet = new CCControlPotentiometer ( );

    if ( pRet && pRet->initWithTrackSprite_ProgressTimer_ThumbSprite ( pBackgroundSprite, pProgressTimer, pThumbSprite ) )
    {
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool CCControlPotentiometer::initWithTrackSprite_ProgressTimer_ThumbSprite ( CCSprite* pTrackSprite, CCProgressTimer* pProgressTimer, CCSprite* pThumbSprite )
{
    if ( CCControl::init ( ) )
    {
        setTouchEnabled ( KD_TRUE );

        setProgressTimer ( pProgressTimer );
        setThumbSprite ( pThumbSprite );
        pThumbSprite->setPosition ( pProgressTimer->getPosition ( ) );
        
        this->addChild ( pThumbSprite, 2 );
        this->addChild ( pProgressTimer, 1 );
        this->addChild ( pTrackSprite );
        
        setContentSize ( pTrackSprite->getContentSize ( ) );
        
        // Init default values
        m_fMinimumValue = 0.0f;
        m_fMaximumValue = 1.0f;
        setValue ( m_fMinimumValue );
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid CCControlPotentiometer::setEnabled ( KDbool bEnabled )
{
    CCControl::setEnabled ( bEnabled );
    if ( m_pThumbSprite != KD_NULL )
    {
        m_pThumbSprite->setOpacity ( ( bEnabled ) ? 255 : 128 );
    }
}

KDvoid CCControlPotentiometer::setValue ( KDfloat fValue )
{
    // set new value with sentinel
    if ( fValue < m_fMinimumValue )
    {
        fValue = m_fMinimumValue;
    }
	
    if ( fValue > m_fMaximumValue ) 
    {
        fValue = m_fMaximumValue;
    }
    
    m_fValue = fValue;
    
    // Update thumb and progress position for new value
    KDfloat  fPercent = ( fValue - m_fMinimumValue ) / ( m_fMaximumValue - m_fMinimumValue );
    m_pProgressTimer->setPercentage ( fPercent * 100.0f );
    m_pThumbSprite->setRotation ( fPercent * 360.0f );
    
    sendActionsForControlEvents ( kCCControlEventValueChanged );    
}

KDfloat CCControlPotentiometer::getValue ( KDvoid )
{
    return m_fValue;
}

KDvoid CCControlPotentiometer::setMinimumValue ( KDfloat fMinimumValue )
{
    m_fMinimumValue = fMinimumValue;
    
    if ( m_fMinimumValue >= m_fMaximumValue )
    {
        m_fMaximumValue = m_fMinimumValue + 1.0f;
    }
    
    setValue ( m_fMaximumValue );
}

KDfloat CCControlPotentiometer::getMinimumValue ( KDvoid )
{
    return m_fMinimumValue;
}

KDvoid CCControlPotentiometer::setMaximumValue ( KDfloat fMaximumValue )
{
    m_fMaximumValue = fMaximumValue;
    
    if ( m_fMaximumValue <= m_fMinimumValue )
    {
        m_fMinimumValue = m_fMaximumValue - 1.0f;
    }
    
    setValue ( m_fMinimumValue );
}

KDfloat CCControlPotentiometer::getMaximumValue ( KDvoid )
{
    return m_fMaximumValue;
}

KDbool CCControlPotentiometer::isTouchInside ( CCTouch* pTouch )
{
    CCPoint  tLocation = this->getTouchLocation ( pTouch );
    
    KDfloat  fDistance = this->distanceBetweenPointAndPoint ( m_pProgressTimer->getPosition ( ), tLocation );

    return fDistance < KD_MIN ( getContentSize ( ).cx / 2, getContentSize ( ).cy / 2 );
}

KDbool CCControlPotentiometer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isTouchInside ( pTouch ) || !this->isEnabled ( ) || !isVisible ( ) )
    {
        return KD_FALSE;
    }
    
    m_tPreviousLocation = this->getTouchLocation ( pTouch );
    
    this->potentiometerBegan ( m_tPreviousLocation );
    
    return KD_TRUE;
}

KDvoid CCControlPotentiometer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    this->potentiometerMoved ( this->getTouchLocation ( pTouch ) );
}

KDvoid CCControlPotentiometer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    this->potentiometerEnded ( CCPointZero );
}

KDfloat CCControlPotentiometer::distanceBetweenPointAndPoint ( CCPoint tPoint1, CCPoint tPoint2 )
{
    KDfloat  fx = tPoint1.x - tPoint2.x;
    KDfloat  fy = tPoint1.y - tPoint2.y;
    return kdSqrtf ( fx * fx + fy * fy );
}

KDfloat CCControlPotentiometer::angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint ( CCPoint tBeginLineA, CCPoint tEndLineA, CCPoint tBeginLineB, CCPoint tEndLineB )
{
    KDfloat  a = tEndLineA.x - tBeginLineA.x;
    KDfloat  b = tEndLineA.y - tBeginLineA.y;
    KDfloat  c = tEndLineB.x - tBeginLineB.x;
    KDfloat  d = tEndLineB.y - tBeginLineB.y;
    
    KDfloat  atanA = kdAtan2f ( a, b );
    KDfloat  atanB = kdAtan2f ( c, d );
    
    // convert radiants to degrees
    return ( atanA - atanB ) * 180 / KD_PI_F;
}

KDvoid CCControlPotentiometer::potentiometerBegan ( CCPoint tLocation )
{
    setSelected ( KD_TRUE );
    getThumbSprite ( )->setColor ( ccGRAY );
}

KDvoid CCControlPotentiometer::potentiometerMoved ( CCPoint tLocation )
{
    KDfloat  fAngle = this->angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint 
	(
        m_pProgressTimer->getPosition ( ),
        tLocation,
        m_pProgressTimer->getPosition ( ),
        m_tPreviousLocation
	);
    
    // fix value, if the 12 o'clock position is between location and previousLocation
    if ( fAngle > 180 )
    {
        fAngle -= 360;
    }
    else if ( fAngle < -180 )
    {
        fAngle += 360;
    }

    setValue ( m_fValue + fAngle / 360.0f * ( m_fMaximumValue - m_fMinimumValue ) );
    
    m_tPreviousLocation = tLocation;
}

KDvoid CCControlPotentiometer::potentiometerEnded ( CCPoint tLocation )
{
    getThumbSprite ( )->setColor ( ccWHITE );
    setSelected ( KD_FALSE );
}

NS_CC_END
