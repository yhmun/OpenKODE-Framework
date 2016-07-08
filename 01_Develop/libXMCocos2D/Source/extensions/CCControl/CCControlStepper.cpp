/* --------------------------------------------------------------------------
 *
 *      File            CCControlStepper.cpp
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
#include "extensions/CCControl/CCControlStepper.h"

NS_CC_BEGIN

#define CCControlStepperLabelColorEnabled		ccc3 (  55,  55,  55 )
#define CCControlStepperLabelColorDisabled		ccc3 ( 147, 147, 147 )

#define CCControlStepperLabelFont				"CourierNewPSMT"

#define kAutorepeatDeltaTime					0.15f
#define kAutorepeatIncreaseTimeIncrement		12

CCControlStepper::CCControlStepper ( KDvoid )
{
	m_dValue			= 0.0;
	m_bContinuous		= KD_FALSE;
	m_bAutorepeat		= KD_FALSE;
	m_bWraps			= KD_FALSE;
	m_dMinimumValue		= 0.0;
	m_dMaximumValue		= 0.0;
	m_dStepValue		= 0.0;
	m_pMinusSprite		= KD_NULL;
	m_pPlusSprite		= KD_NULL;
	m_pMinusLabel		= KD_NULL;
	m_pPlusLabel		= KD_NULL;
	m_bTouchInsideFlag	= KD_FALSE;
	m_eTouchedPart		= kCCControlStepperPartNone;
	m_nAutorepeatCount	= 0;
}

CCControlStepper::~CCControlStepper ( KDvoid )
{
    unscheduleAllSelectors ( );
    
    CC_SAFE_RELEASE ( m_pMinusSprite );
    CC_SAFE_RELEASE ( m_pPlusSprite );
    CC_SAFE_RELEASE ( m_pMinusLabel );
    CC_SAFE_RELEASE ( m_pPlusLabel );
}

KDbool CCControlStepper::initWithMinusSpriteAndPlusSprite ( CCSprite* pMinusSprite, CCSprite* pPlusSprite )
{
    if ( CCControl::init ( ) )
    {
        CCAssert ( pMinusSprite, "Minus sprite must be not nil" );
        CCAssert ( pPlusSprite , "Plus sprite must be not nil" );
        
        setTouchEnabled ( KD_TRUE );

        // Set the default values
        m_bAutorepeat	= KD_TRUE;
        m_bContinuous	= KD_TRUE;
        m_dMinimumValue	= 0;
        m_dMaximumValue	= 100;
        m_dValue		= 0;
        m_dStepValue	= 1;
        m_bWraps		= KD_FALSE;
        this->ignoreAnchorPointForPosition ( KD_FALSE );
    
        // Add the minus components
        this->setMinusSprite ( pMinusSprite );
		m_pMinusSprite->setPosition ( ccpMid ( pMinusSprite->getContentSize ( ) ) );
		this->addChild ( m_pMinusSprite );
        
        this->setMinusLabel ( CCLabelTTF::create ( "-", CCControlStepperLabelFont, 40 ) );
        m_pMinusLabel->setColor ( CCControlStepperLabelColorDisabled );
        m_pMinusLabel->setPosition ( ccpMid ( pMinusSprite->getContentSize ( ) ) );
        m_pMinusSprite->addChild ( m_pMinusLabel );
        
        // Add the plus components 
        this->setPlusSprite ( pPlusSprite );
		m_pPlusSprite->setPosition ( ccp ( pMinusSprite->getContentSize ( ).cx + pPlusSprite->getContentSize ( ).cx / 2, pMinusSprite->getContentSize().cy / 2 ) );
		this->addChild ( m_pPlusSprite );
        
        this->setPlusLabel ( CCLabelTTF::create ( "+", CCControlStepperLabelFont, 40 ) );
        m_pPlusLabel->setColor ( CCControlStepperLabelColorEnabled );
        m_pPlusLabel->setPosition ( ccpMid ( pPlusSprite->getContentSize ( ) ) );
        m_pPlusSprite->addChild ( m_pPlusLabel );
        
        // Defines the content size
        CCRect  tMaxRect = CCControlUtils::CCRectUnion ( m_pMinusSprite->boundingBox ( ), m_pPlusSprite->boundingBox ( ) );
        this->setContentSize ( CCSizeMake ( m_pMinusSprite->getContentSize ( ).cx + m_pPlusSprite->getContentSize ( ).cy, tMaxRect.size.cy ) );
    
		return KD_TRUE;
    }

    return KD_FALSE;
}

CCControlStepper* CCControlStepper::create ( CCSprite* pMinusSprite, CCSprite* pPlusSprite )
{
    CCControlStepper*  pRet = new CCControlStepper ( );
    
	if ( pRet && pRet->initWithMinusSpriteAndPlusSprite ( pMinusSprite, pPlusSprite ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

//#pragma mark Properties

KDvoid CCControlStepper::setWraps ( KDbool bWraps )
{
    m_bWraps = bWraps;
    
    if ( m_bWraps )
    {
        m_pMinusLabel->setColor ( CCControlStepperLabelColorEnabled );
        m_pPlusLabel ->setColor ( CCControlStepperLabelColorEnabled );
    }
    
    this->setValue ( m_dValue );
}

KDvoid CCControlStepper::setMinimumValue ( KDdouble dMinimumValue )
{
    if ( dMinimumValue >= m_dMaximumValue )
    {
        CCAssert ( 0, "Must be numerically less than maximumValue." );
    }
    
    m_dMinimumValue = dMinimumValue;
    this->setValue ( m_dValue );
}

KDvoid CCControlStepper::setMaximumValue ( KDdouble dMaximumValue )
{
    if ( dMaximumValue <= m_dMinimumValue)
    {
        CCAssert ( 0, "Must be numerically greater than minimumValue." );
    }
    
    m_dMaximumValue = dMaximumValue;
    this->setValue ( m_dValue );
}

KDvoid CCControlStepper::setValue ( KDdouble dValue )
{
    this->setValueWithSendingEvent ( dValue, KD_TRUE );
}

KDdouble CCControlStepper::getValue ( KDvoid )
{
    return m_dValue;
}

KDvoid CCControlStepper::setStepValue ( KDdouble dStepValue )
{
    if ( dStepValue <= 0 )
    {
        CCAssert ( 0, "Must be numerically greater than 0." );
    }

    m_dStepValue = dStepValue;
}

KDbool CCControlStepper::isContinuous ( KDvoid )
{
    return m_bContinuous;
}

//#pragma mark -
//#pragma mark CCControlStepper Public Methods

KDvoid CCControlStepper::setValueWithSendingEvent ( KDdouble dValue, KDbool bSend )
{
    if ( dValue < m_dMinimumValue )
    {
        dValue = m_bWraps ? m_dMaximumValue : m_dMinimumValue;
    } 
	else if ( dValue > m_dMaximumValue )
    {
        dValue = m_bWraps ? m_dMinimumValue : m_dMaximumValue;
    }
    
    m_dValue = dValue;
    
    if ( !m_bWraps )
    {
        m_pMinusLabel->setColor ( ( dValue == m_dMinimumValue ) ? CCControlStepperLabelColorDisabled : CCControlStepperLabelColorEnabled );
        m_pPlusLabel ->setColor ( ( dValue == m_dMaximumValue ) ? CCControlStepperLabelColorDisabled : CCControlStepperLabelColorEnabled );
    }
    
    if ( bSend )
    {
        this->sendActionsForControlEvents ( kCCControlEventValueChanged );
    }
}

KDvoid CCControlStepper::startAutorepeat ( KDvoid )
{
    m_nAutorepeatCount = -1;
    
    this->schedule ( schedule_selector ( CCControlStepper::update ), kAutorepeatDeltaTime, kCCRepeatForever, kAutorepeatDeltaTime * 3 );
}

/// Stop the autorepeat. 
KDvoid CCControlStepper::stopAutorepeat ( KDvoid )
{
    this->unschedule ( schedule_selector ( CCControlStepper::update ) );
}

KDvoid CCControlStepper::update ( KDfloat fDelta )
{
    m_nAutorepeatCount++;
    
    if ( ( m_nAutorepeatCount < kAutorepeatIncreaseTimeIncrement ) && ( m_nAutorepeatCount % 3 ) != 0 )
	{
        return;
	}
    
    if ( m_eTouchedPart == kCCControlStepperPartMinus )
    {
        this->setValueWithSendingEvent ( m_dValue - m_dStepValue, m_bContinuous );
    } 
	else if ( m_eTouchedPart == kCCControlStepperPartPlus )
    {
        this->setValueWithSendingEvent ( m_dValue + m_dStepValue, m_bContinuous );
    }
}

//#pragma mark CCControlStepper Private Methods

KDvoid CCControlStepper::updateLayoutUsingTouchLocation ( CCPoint tLocation )
{
    if ( tLocation.x < m_pMinusSprite->getContentSize ( ).cx && m_dValue > m_dMinimumValue )
    {
        m_eTouchedPart = kCCControlStepperPartMinus;
        
        m_pMinusSprite->setColor ( ccGRAY );
        m_pPlusSprite->setColor ( ccWHITE );
    }
	else if ( tLocation.x >= m_pMinusSprite->getContentSize ( ).cx && m_dValue < m_dMaximumValue )
    {
        m_eTouchedPart = kCCControlStepperPartPlus;
        
        m_pMinusSprite->setColor ( ccWHITE );
        m_pPlusSprite ->setColor ( ccGRAY );
    } 
	else
    {
        m_eTouchedPart = kCCControlStepperPartNone;
        
        m_pMinusSprite->setColor ( ccWHITE );
        m_pPlusSprite ->setColor ( ccWHITE );
    }
}

KDbool CCControlStepper::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !isTouchInside ( pTouch ) || !isEnabled ( ) || !isVisible ( ) )
    {
        return KD_FALSE;
    }
    
    CCPoint  tLocation = this->getTouchLocation ( pTouch );
    this->updateLayoutUsingTouchLocation ( tLocation );
    
    m_bTouchInsideFlag = KD_TRUE;
    
    if ( m_bAutorepeat )
    {
        this->startAutorepeat ( );
    }
    
    return KD_TRUE;
}

KDvoid CCControlStepper::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( this->isTouchInside ( pTouch ) )
    {
        CCPoint  tLocation = this->getTouchLocation ( pTouch );
        this->updateLayoutUsingTouchLocation ( tLocation );
        
        if ( !m_bTouchInsideFlag )
        {
            m_bTouchInsideFlag = KD_TRUE;
            
            if ( m_bAutorepeat )
            {
                this->startAutorepeat ( );
            }
        }
    } else
    {
        m_bTouchInsideFlag = KD_FALSE;
        
        m_eTouchedPart = kCCControlStepperPartNone;
        
        m_pMinusSprite->setColor ( ccWHITE );
        m_pPlusSprite ->setColor ( ccWHITE );
        
        if ( m_bAutorepeat )
        {
            this->stopAutorepeat ( );
        }
    }
}

KDvoid CCControlStepper::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    m_pMinusSprite->setColor ( ccWHITE );
    m_pPlusSprite ->setColor ( ccWHITE );
    
    if ( m_bAutorepeat )
    {
        this->stopAutorepeat ( );
    }
    
    if ( this->isTouchInside ( pTouch ) )
    {
        CCPoint  tLocation = this->getTouchLocation ( pTouch );
        
        this->setValue ( m_dValue + ( ( tLocation.x < m_pMinusSprite->getContentSize ( ).cx ) ? ( 0.0 - m_dStepValue ) : m_dStepValue ) );
    }
}

NS_CC_END
