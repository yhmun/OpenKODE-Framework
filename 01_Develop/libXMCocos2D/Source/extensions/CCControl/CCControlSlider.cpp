/* --------------------------------------------------------------------------
 *
 *      File            CCControlSlider.cpp
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
#include "extensions/CCControl/CCControlSlider.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCControlSlider::CCControlSlider ( KDvoid )
{
	m_fValue				= 0.0f;
	m_fMinimumValue			= 0.0f;
	m_fMaximumValue			= 0.0f;
	m_fMinimumAllowedValue	= 0.0f;
	m_fMaximumAllowedValue	= 0.0f;
	m_pThumbSprite			= KD_NULL;
	m_pProgressSprite		= KD_NULL;
	m_pBackgroundSprite		= KD_NULL;
}

CCControlSlider::~CCControlSlider ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pThumbSprite );
    CC_SAFE_RELEASE ( m_pProgressSprite );
    CC_SAFE_RELEASE ( m_pBackgroundSprite );
}

CCControlSlider* CCControlSlider::create ( const KDchar* szBackgroundFile, const KDchar* szProgressFile, const KDchar* szThumbFile )
{	
	return CCControlSlider::create
	(
		CCSprite::create ( szBackgroundFile ),
		CCSprite::create ( szProgressFile ), 
		CCSprite::create ( szThumbFile ) 
	);
}

CCControlSlider* CCControlSlider::create ( CCSprite* pBackgroundSprite, CCSprite* pProgressSprite, CCSprite* pThumbSprite )
{
	CCControlSlider*  pRet = new CCControlSlider ( );
	
	if ( pRet && pRet->initFromSprites ( pBackgroundSprite, pProgressSprite, pThumbSprite ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCControlSlider::initFromSprites ( CCSprite* pBackgroundSprite, CCSprite* pProgressSprite, CCSprite* pThumbSprite )
{
	if ( CCControl::init ( ) )
	{
		CCAssert ( pBackgroundSprite,  "Background sprite must be not nil" );
        CCAssert ( pProgressSprite,    "Progress sprite must be not nil" );
        CCAssert ( pThumbSprite,       "Thumb sprite must be not nil" );

		ignoreAnchorPointForPosition ( KD_FALSE );
		setTouchEnabled ( KD_TRUE );

		this->setBackgroundSprite ( pBackgroundSprite );
        this->setProgressSprite ( pProgressSprite );
        this->setThumbSprite ( pThumbSprite );

		// Defines the content size
		CCRect  tMaxRect = CCControlUtils::CCRectUnion ( pBackgroundSprite->boundingBox ( ), pThumbSprite->boundingBox ( ) );
        setContentSize ( CCSizeMake ( tMaxRect.size.cx, tMaxRect.size.cy ) );
        
		// Add the slider background
		m_pBackgroundSprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
		m_pBackgroundSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
		this->addChild ( m_pBackgroundSprite );

		// Add the progress bar
		m_pProgressSprite->setAnchorPoint ( ccp ( 0.0f, 0.5f ) );
		m_pProgressSprite->setPosition ( ccp ( 0.0f, this->getContentSize ( ).cy / 2 ) );
		this->addChild ( m_pProgressSprite );

		// Add the slider thumb  
		m_pThumbSprite->setPosition ( ccp ( 0.0f, this->getContentSize ( ).cy / 2 ) );
		this->addChild ( m_pThumbSprite );

		// Init default values
		m_fMinimumValue = 0.0f;
		m_fMaximumValue = 1.0f;
		
		setValue ( m_fMinimumValue );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCControlSlider::setEnabled ( KDbool bEnabled )
{
    CCControl::setEnabled ( bEnabled );
    if ( m_pThumbSprite != KD_NULL ) 
    {
        m_pThumbSprite->setOpacity ( ( bEnabled ) ? 255 : 128 );
    }
}

KDvoid CCControlSlider::setValue ( KDfloat fValue )
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

    this->needsLayout ( );

    this->sendActionsForControlEvents ( kCCControlEventValueChanged );	
}

KDvoid CCControlSlider::setMinimumValue ( KDfloat fMinimumValue )
{
	m_fMinimumValue = fMinimumValue;
	m_fMinimumAllowedValue = fMinimumValue;
	
	if ( m_fMinimumValue >= m_fMaximumValue )   
	{
		m_fMaximumValue = m_fMinimumValue + 1.0f;
	}

	setValue ( m_fValue );
}

KDvoid CCControlSlider::setMaximumValue ( KDfloat fMaximumValue )
{
	m_fMaximumValue = fMaximumValue;
	m_fMaximumAllowedValue = fMaximumValue;

	if ( m_fMaximumValue <= m_fMinimumValue )    
	{
		m_fMinimumValue = m_fMaximumValue - 1.0f;
	}

	setValue ( m_fValue );
}

KDbool CCControlSlider::isTouchInside ( CCTouch* pTouch )
{
	CCPoint  tLocation = pTouch->getLocation ( );
	tLocation = this->getParent ( )->convertToNodeSpace ( tLocation );

	CCRect  tRect = this->boundingBox();
	tRect.size.cx  += m_pThumbSprite->getContentSize ( ).cx;
	tRect.origin.x -= m_pThumbSprite->getContentSize ( ).cx / 2;

	return tRect.containsPoint ( tLocation );
}

CCPoint CCControlSlider::locationFromTouch ( CCTouch* pTouch )
{
	CCPoint  tLocation = pTouch->getLocation ( );			// Get the touch position
	tLocation = this->convertToNodeSpace ( tLocation );		// Convert to the node space of this class

	if ( tLocation.x < 0 )
	{
	  tLocation.x = 0;
	} 
	else if ( tLocation.x > m_pBackgroundSprite->getContentSize ( ).cx )
	{
	  tLocation.x = m_pBackgroundSprite->getContentSize ( ).cx;
	}

	return tLocation;
}

KDbool CCControlSlider::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !isTouchInside ( pTouch ) || !isEnabled ( ) || !isVisible ( ) )
    {
        return KD_FALSE;
    }

    CCPoint  tLocation = locationFromTouch ( pTouch );
    sliderBegan ( tLocation );

    return KD_TRUE;
}

KDvoid CCControlSlider::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tLocation = locationFromTouch ( pTouch );
	sliderMoved ( tLocation );
}

KDvoid CCControlSlider::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	sliderEnded ( CCPointZero );
}

KDvoid CCControlSlider::needsLayout ( KDvoid )
{
    if ( KD_NULL == m_pThumbSprite || KD_NULL == m_pBackgroundSprite || KD_NULL == m_pProgressSprite )
    {
        return;
    }

    // Update thumb position for new value
    KDfloat  fPercent = ( m_fValue - m_fMinimumValue ) / ( m_fMaximumValue - m_fMinimumValue );

    CCPoint  tPosition = m_pThumbSprite->getPosition ( );
    tPosition.x = fPercent * m_pBackgroundSprite->getContentSize ( ).cx;
    m_pThumbSprite->setPosition ( tPosition );

    // Stretches content proportional to newLevel
    CCRect   tTextureRect = m_pProgressSprite->getTextureRect ( );
    tTextureRect = CCRectMake ( tTextureRect.origin.x, tTextureRect.origin.y, tPosition.x, tTextureRect.size.cy );
    m_pProgressSprite->setTextureRect ( tTextureRect, m_pProgressSprite->isTextureRectRotated ( ), tTextureRect.size );
}

KDvoid CCControlSlider::sliderBegan ( CCPoint tLocation )
{
	this->setSelected ( KD_TRUE );
	this->getThumbSprite ( )->setColor ( ccGRAY );
	setValue ( valueForLocation ( tLocation ) );
}

KDvoid CCControlSlider::sliderMoved ( CCPoint tLocation )
{
	setValue ( valueForLocation ( tLocation ) );
}

KDvoid CCControlSlider::sliderEnded ( CCPoint tLocation )
{
	if ( this->isSelected ( ) )
	{
		setValue ( valueForLocation ( m_pThumbSprite->getPosition ( ) ) );
	}

	this->getThumbSprite ( )->setColor ( ccWHITE );
    this->setSelected ( KD_FALSE );
}

KDfloat CCControlSlider::valueForLocation ( CCPoint tLocation )
{
    KDfloat  fPercent = tLocation.x / m_pBackgroundSprite->getContentSize ( ).cx;
    return KD_MAX ( KD_MIN ( m_fMinimumValue + fPercent * ( m_fMaximumValue - m_fMinimumValue ), m_fMaximumAllowedValue ), m_fMinimumAllowedValue );

}

NS_CC_END