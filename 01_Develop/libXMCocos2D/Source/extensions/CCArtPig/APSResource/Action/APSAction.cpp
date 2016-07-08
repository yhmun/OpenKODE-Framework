/* --------------------------------------------------------------------------
 *
 *      File            APSAction.cpp
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
#include "extensions/CCArtPig/APSAction.h"
#include "extensions/CCArtPig/APSActionGroup.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

const KDbool kAPSAction_defaultEnabled  = KD_TRUE;
const KDint  kAPSAction_defaultPriority = 0;

const std::string& APSAction::getTargetGraphicIndexKey ( KDvoid )
{
	static const std::string  sKey = "targetGraphicIndex";
    return sKey;
}

const std::string& APSAction::getActionGroupIndexKey ( KDvoid )
{
	static const std::string  sKey = "actionGroupIndex";
    return sKey;
}

const std::string& APSAction::getPriorityKey ( KDvoid ) 
{
	static const std::string  sKey = "priority";
    return sKey;
}

const std::string& APSAction::getEnabledKey ( KDvoid ) 
{
	static const std::string  sKey = "enabled";
    return sKey;
}

const std::string& APSAction::getStartTimeKey ( KDvoid )
{
	static const std::string  sKey = "startTime";
    return sKey;
}

const std::string& APSAction::getDurationKey ( KDvoid )
{
	static const std::string  sKey = "duration";
    return sKey;
}

const std::string& APSAction::getRunningKey ( KDvoid )
{
	static const std::string  sKey = "running";
    return sKey;
}

APSAction::APSAction ( APSDictionary* pProperties )
{
	m_pTargetGraphic		= KD_NULL;
	m_uTargetGraphicIndex	= APS_NIL_INDEX;
	m_pActionGroup			= KD_NULL;
	m_uActionGroupIndex		= APS_NIL_INDEX;
	m_pCCAction				= KD_NULL;
	m_nPriority				= kAPSAction_defaultPriority;
	m_fStartTime			= 0.f;
	m_fDuration				= 0.f;
	m_bEnabled				= kAPSAction_defaultEnabled;
	m_bRunning				= KD_FALSE;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSAction::~APSAction ( KDvoid )
{
    CC_SAFE_RELEASE ( this->m_pCCAction );
}

KDbool APSAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSResource::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set targetGraphicIndex
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTargetGraphicIndexKey ( ) );
    if ( pNumber )
	{
        this->setTargetGraphicIndex ( APSNumber::indexWithNumber ( pNumber ) );
    }
    
    // set actionGroupIndex
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getActionGroupIndexKey ( ) );
    if ( pNumber ) 
	{
        this->setActionGroupIndex ( APSNumber::indexWithNumber ( pNumber ) );
    }
    
    // set priority
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getPriorityKey ( ) );
    if ( pNumber ) 
	{
        this->setPriority ( pNumber->getInteger ( ) );
    }
    
    // set enabled
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getEnabledKey ( ) );
    if ( pNumber ) 
	{
        this->setEnabled ( pNumber->getBoolean ( ) );
    }
    
    // set startTime
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getStartTimeKey ( ) );
    if ( pNumber ) 
	{
        this->setStartTime ( pNumber->getFloat ( ) );
    }
    
    // set duration
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getDurationKey ( ) );
    if ( pNumber ) 
	{
        this->setDuration ( pNumber->getFloat ( ) );
    }

    return KD_TRUE;
}


KDfloat APSAction::getEndTime ( KDvoid ) 
{
    return this->m_fStartTime + this->m_fDuration;
}

CCAction* APSAction::getCCAction ( KDvoid )
{
    if ( !this->m_pCCAction )
	{
        // create if does not have one.
        this->m_pCCAction = this->makeCCAction ( );
        CC_SAFE_RETAIN ( this->m_pCCAction );
    }
    return this->m_pCCAction;
}

KDvoid APSAction::resetCCAction ( KDvoid )
{
    // Force to stop currently running action.
    if ( this->getRunning ( ) )
	{
        this->interrupt ( );
    }
    
    CC_SAFE_RELEASE ( this->m_pCCAction );
    
    this->getCCAction ( );
}

KDvoid APSAction::clearCachedData ( KDvoid )
{ 
    if ( this->m_pCCAction )
	{
        if ( this->m_pCCAction->isDone ( ) )
		{
            CC_SAFE_RELEASE ( this->m_pCCAction );
            this->m_pCCAction = KD_NULL;
        }
    }
}

KDvoid APSAction::preloadData ( KDvoid )
{
    if ( !this->m_pCCAction )
	{
        this->getCCAction ( );
    }
}

KDvoid APSAction::startFunc ( KDvoid )
{
    this->m_bRunning = KD_TRUE;
    this->getParentSymbol ( )->notifyStart ( this );
}

KDvoid APSAction::stopFunc ( KDvoid )
{
    APSGraphicGroup*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        pGraphic->stopAction(this);
    }
    this->m_bRunning = KD_FALSE;
    this->getParentSymbol ( )->notifyStop ( this );
}

KDvoid APSAction::trigger ( KDvoid )
{
    this->getParentSymbol ( )->triggerAction ( this );
}

KDvoid APSAction::interrupt ( KDvoid )
{
    this->getParentSymbol ( )->interruptAction ( this );
}

KDvoid APSAction::_trigger ( KDvoid )
{
    this->getTargetGraphic ( )->runAction ( this );
}

KDvoid APSAction::_interrupt ( KDvoid )
{
    // Since CCAction is stopped, stopFunc() should be called manually.
    this->stopFunc ( );
}

CCFiniteTimeAction* APSAction::makeCCAction ( KDvoid )
{
    CCFiniteTimeAction*  pActions[3] = { this->makePreCCAction ( ), this->makeMidCCAction ( KD_FALSE, 1 ), this->makePostCCAction ( ) };    
    CCFiniteTimeAction*  pSeq = KD_NULL;
    for ( KDint i = 0; i < 3; i++ )
	{
        if ( pActions[i] )
		{
            if ( pSeq ) 
			{
				pSeq = CCSequence::createWithTwoActions ( pSeq, pActions [i] );
			}
			else 
			{
                pSeq = pActions [i];
            }
        }
    }

    CCFiniteTimeAction*  pAction = pSeq ? pSeq : CCMoveBy::create ( KD_FLT_EPSILON, CCPointZero );
   
    return pAction;
}

CCFiniteTimeAction* APSAction::makePreCCAction ( KDvoid )
{
    return CCCallFunc::create ( this, callfunc_selector ( APSAction::startFunc ) );
}

CCFiniteTimeAction* APSAction::makePostCCAction ( KDvoid )
{
    return CCCallFunc::create ( this, callfunc_selector ( APSAction::stopFunc ) );
}

APSGraphicGroup* APSAction::getTargetGraphic ( KDvoid )
{
    if ( this->getTargetGraphicIndex ( ) == APS_NIL_INDEX )
	{
        return KD_NULL;
    }
    
    if ( !m_pTargetGraphic )
	{
        m_pTargetGraphic = (APSGraphicGroup*) this->getResource ( this->getTargetGraphicIndex ( ) );
    }
    
    return m_pTargetGraphic;
}

APSActionGroup* APSAction::getActionGroup ( KDvoid )
{
    if ( this->getActionGroupIndex ( ) == APS_NIL_INDEX )
	{
        return KD_NULL;
    }
    
    if ( !m_pActionGroup )
	{
        m_pActionGroup = (APSActionGroup*) this->getResource ( this->getActionGroupIndex ( ) );
    }
    
    return m_pActionGroup;
}

NS_APS_END


