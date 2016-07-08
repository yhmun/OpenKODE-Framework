/* --------------------------------------------------------------------------
 *
 *      File            APSIntervalGraphicAction.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
#include "extensions/CCArtPig/APSIntervalGraphicAction.h"
#include "extensions/CCArtPig/APSActionGroup.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"

NS_APS_BEGIN

const KDbool   kAPSIntervalGraphicAction_defaultAutoReverse			= KD_FALSE;
const KDfloat  kAPSIntervalGraphicAction_defaultAutoReverseDelay	= 0.f;
const KDuint   kAPSIntervalGraphicAction_defaultRepeat				= 1;
const KDint    kAPSIntervalGraphicAction_defaultTimingMode			= kAPSTimingModeLinear;
const KDfloat  kAPSIntervalGraphicAction_defaultTimingFactor		= 0.f;
const KDbool   kAPSIntervalGraphicAction_defaultFinishRecovery		= KD_FALSE;

const std::string& APSIntervalGraphicAction::getAutoReverseKey ( KDvoid ) 
{
    static const std::string  sKey = "autoReverse";
    return sKey;
}

const std::string& APSIntervalGraphicAction::getAutoReverseDelayKey ( KDvoid )
{
    static const std::string  sKey = "autoReverseDelay";
    return sKey;
}

const std::string& APSIntervalGraphicAction::getRepeatKey ( KDvoid )
{
    static const std::string  sKey = "repeat";
    return sKey;
}

const std::string& APSIntervalGraphicAction::getTimingModeKey ( KDvoid ) 
{
    static const std::string  sKey = "timingMode";
    return sKey;
}

const std::string& APSIntervalGraphicAction::getTimingFactorKey ( KDvoid )
{
    static const std::string  sKey = "timingFactor";
    return sKey;
}

const std::string& APSIntervalGraphicAction::getFinishRecoveryKey ( KDvoid ) 
{
    static const std::string  sKey = "finishRecovery";
    return sKey;
}

APSIntervalGraphicAction::APSIntervalGraphicAction ( APSDictionary* pProperties ) 
{
	m_bAutoReverse		= kAPSIntervalGraphicAction_defaultAutoReverse;
	m_fAutoReverseDelay	= kAPSIntervalGraphicAction_defaultAutoReverseDelay;
	m_uRepeat			= kAPSIntervalGraphicAction_defaultRepeat;
	m_nTimingMode		= kAPSIntervalGraphicAction_defaultTimingMode;
	m_fTimingFactor		= kAPSIntervalGraphicAction_defaultTimingFactor;
	m_bFinishRecovery	= kAPSIntervalGraphicAction_defaultFinishRecovery;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSIntervalGraphicAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSGraphicAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    APSNumber*  pNumber = KD_NULL;
    
    // set autoReverse
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getAutoReverseKey ( ) );
    if ( pNumber )
	{
        this->setAutoReverse ( pNumber->getBoolean ( ) );
    }
    
    // set autoReverseDelay
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getAutoReverseDelayKey ( ) );
    if ( pNumber ) 
	{
        this->setAutoReverseDelay ( pNumber->getFloat ( ) );
    }
    
    // set repeat
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getRepeatKey ( ) );
    if ( pNumber ) 
	{
        this->setRepeat ( pNumber->getUnsignedInteger ( ) );
    }
    
    // set timingMode
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTimingModeKey ( ) );
    if ( pNumber ) 
	{
        this->setTimingMode ( pNumber->getInteger ( ) );
    }
    
    // set timingFactor
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTimingFactorKey ( ) );
    if ( pNumber )
	{
        this->setTimingFactor ( pNumber->getFloat ( ) );
    }
    
    // set finishRecovery
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getFinishRecoveryKey ( ) );
    if ( pNumber ) 
	{
        this->setFinishRecovery ( pNumber->getBoolean ( ) );
    }
    
    return KD_TRUE;
}

KDfloat APSIntervalGraphicAction::getEndTime ( KDvoid ) 
{
    return this->m_fStartTime + this->m_fDuration * this->m_uRepeat;
}

KDbool APSIntervalGraphicAction::isRestoringAction ( KDvoid ) const
{
    return !this->m_bAutoReverse && ( this->m_bFinishRecovery || ( this->m_uRepeat > 1 && !this->m_bRelative ) );
}

CCFiniteTimeAction *APSIntervalGraphicAction::universalCCAction ( KDfloat fScale )
{   
    return this->makeMidCCAction ( KD_TRUE, fScale );
}

KDvoid APSIntervalGraphicAction::startFunc ( KDvoid ) 
{
    if ( this->isRestoringAction ( ) )
	{
        this->saveNodeState ( );
    }
    APSAction::startFunc ( );   
}

KDvoid APSIntervalGraphicAction::stopFunc ( KDvoid ) 
{
	if ( this->isRestoringAction ( ) )
	{
		this->restoreNodeState ( );
	}
    APSAction::stopFunc ( );    
}

CCFiniteTimeAction* APSIntervalGraphicAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    m_pGraphicCCAction = this->makeGraphicCCAction ( bUniversal, fScale );
    CCActionInterval*  pAction = this->attachTimingMode ( m_pGraphicCCAction );
    
    if ( this->m_bAutoReverse )
	{        
        if ( this->getAutoReverseDelay ( ) > KD_FLT_EPSILON )
		{
            pAction = CCSequence::createWithTwoActions ( pAction, CCDelayTime::create ( this->getAutoReverseDelay ( ) ) );
        }
       
        pAction = CCSequence::createWithTwoActions ( pAction, pAction->reverse ( ) );
    }

    return this->attachRepeat ( pAction );
}

CCActionInterval* APSIntervalGraphicAction::attachRepeat ( CCActionInterval* pAction )
{
    if ( this->m_uRepeat > 1 )
	{
        CCActionInterval*  pActionWithRestoring = KD_NULL;
        
        if ( this->isRestoringAction ( ) )
		{
            // For repeating non-relative and non-autoreverse action, we should bring graphic's state to origin.
            pActionWithRestoring = CCSequence::createWithTwoActions ( pAction, CCCallFunc::create ( this, callfunc_selector ( APSGraphicAction::restoreNodeState ) ) );
        }
        
        if ( this->m_uRepeat > 1 )
		{
            if ( pActionWithRestoring ) 
			{
                // The last action should not restore state, so we append the original action.
                pAction = CCSequence::createWithTwoActions ( CCRepeat::create ( pActionWithRestoring, this->m_uRepeat - 1 ), pAction );
            } 
			else 
			{
                pAction = CCRepeat::create ( pAction, this->m_uRepeat );
            }
        }
    }
    return pAction;
}

CCActionInterval* APSIntervalGraphicAction::attachTimingMode ( CCActionInterval* pAction )
{
    switch ( this->m_nTimingMode )
	{
        case kAPSTimingModeEaseIn				:	pAction = CCEaseIn				::create ( pAction, this->m_fTimingFactor );	break;
        case kAPSTimingModeEaseOut				:	pAction = CCEaseOut				::create ( pAction, this->m_fTimingFactor );	break;
        case kAPSTimingModeEaseInEaseOut		:	pAction = CCEaseInOut			::create ( pAction, this->m_fTimingFactor );	break;
        case kAPSTimingModeEaseExponentialIn	:	pAction = CCEaseExponentialIn	::create ( pAction );	break;
        case kAPSTimingModeEaseExponentialOut	:	pAction = CCEaseExponentialOut	::create ( pAction );	break;
        case kAPSTimingModeEaseExponentialInOut	:	pAction = CCEaseExponentialInOut::create ( pAction );	break;
        case kAPSTimingModeEaseSineIn			:	pAction = CCEaseSineIn			::create ( pAction );	break;
        case kAPSTimingModeEaseSineOut			:	pAction = CCEaseSineOut			::create ( pAction );	break;
        case kAPSTimingModeEaseSineInOut		:	pAction = CCEaseSineInOut		::create ( pAction );	break;
        case kAPSTimingModeEaseElasticIn		:	pAction = CCEaseElasticIn		::create ( pAction , easeElasticPeriod ( this->m_fTimingFactor ) );	break;
        case kAPSTimingModeEaseElasticOut		:	pAction = CCEaseElasticOut		::create ( pAction , easeElasticPeriod ( this->m_fTimingFactor ) );	break;
        case kAPSTimingModeEaseElasticInOut		:	pAction = CCEaseElasticInOut	::create ( pAction , easeElasticPeriod ( this->m_fTimingFactor ) );	break;
        case kAPSTimingModeEaseBounceIn			:	pAction = CCEaseBounceIn		::create ( pAction );	break;
        case kAPSTimingModeEaseBounceOut		:	pAction = CCEaseBounceOut		::create ( pAction );	break;
        case kAPSTimingModeEaseBounceInOut		:	pAction = CCEaseBounceInOut		::create ( pAction );	break;
        case kAPSTimingModeEaseBackIn			:	pAction = CCEaseBackIn			::create ( pAction );	break;
        case kAPSTimingModeEaseBackOut			:	pAction = CCEaseBackOut			::create ( pAction );	break;
        case kAPSTimingModeEaseBackInOut		:	pAction = CCEaseBackInOut		::create ( pAction );	break;
            
        default	: break;
    }

    return pAction;    
}

NS_APS_END

