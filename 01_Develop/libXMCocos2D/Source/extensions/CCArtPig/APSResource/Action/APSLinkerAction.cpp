/* --------------------------------------------------------------------------
 *
 *      File            APSLinkerAction.cpp
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
#include "extensions/CCArtPig/APSLinkerAction.h"
#include "extensions/CCArtPig/APSActionGroup.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

const APSLinkerType kAPSLinkerAction_defaultType = kAPSLinkerTypeTrigger;
const KDbool		kAPSLinkerAction_defaultNoTriggerRunningTarget = KD_FALSE;

const std::string& APSLinkerAction::getTargetActionIndexKey ( KDvoid )
{
    static const std::string  sKey = "tActionIdx";
    return sKey;
}

const std::string& APSLinkerAction::getTypeKey ( KDvoid ) 
{
    static const std::string  sKey = "type";
    return sKey;
}

const std::string& APSLinkerAction::getNoTriggerRunningTargetKey ( KDvoid )
{
    static const std::string  sKey = "noTrigRT";
    return sKey;
}

APSLinkerAction::APSLinkerAction ( APSDictionary* pProperties ) 
{
	m_uType					  = kAPSLinkerAction_defaultType;
	m_pTargetAction			  = KD_NULL;
	m_uTargetActionIndex	  = APS_NIL_INDEX;
	m_bNoTriggerRunningTarget = kAPSLinkerAction_defaultNoTriggerRunningTarget;

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSLinkerAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set targetActionIndex
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTargetActionIndexKey ( ) );
    if ( pNumber )
	{
        this->setTargetActionIndex ( APSNumber::indexWithNumber ( pNumber ) );
    }
    
    // set type
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTypeKey ( ) );
    if ( pNumber ) 
	{
        this->setType ( (APSLinkerType) pNumber->getInteger ( ) );
    }
    
    // set noTriggerRunningTarget
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getNoTriggerRunningTargetKey ( ) );
    if ( pNumber )
	{
        this->setNoTriggerRunningTarget ( pNumber->getBoolean ( ) );
    }
    
    return KD_TRUE;
}

KDvoid APSLinkerAction::_trigger ( KDvoid ) 
{    
    if ( !this->getTargetGraphic ( )->getNode ( )->getActionByTag ( this->getCCAction ( )->getTag ( ) ) )
	{
        this->getTargetGraphic()->runAction(this);
    }
}

KDvoid APSLinkerAction::sendLinkMessage ( KDvoid ) 
{
    this->_sendLinkMessage ( this->getParentSymbol ( ) );
}

// Sends link message.
// !!! Fixme: somehow sendLinkMessage is called twice per linker action in Cocos2D-iPhone 2.0.
KDvoid APSLinkerAction::_sendLinkMessage ( APSSymbol* pSymbol )
{
    APSActionGroup*  pTargetAction = (APSActionGroup*) this->getTargetAction ( );
    
    switch ( (KDint) this->m_uType ) 
	{
        case kAPSLinkerTypeTrigger :
            if ( pTargetAction && pTargetAction->getEnabled ( ) ) 
			{
                // Do not trigger if the target is running.
                if ( !this->getNoTriggerRunningTarget ( ) || !pTargetAction->getRunning ( ) )
				{
                    pTargetAction->trigger ( );
                }
            }
            break;

        case kAPSLinkerTypeInterrupt :
            if ( pTargetAction )
			{
                pTargetAction->interrupt ( );
            }
            break;

        case kAPSLinkerTypeEnable :
            if ( pTargetAction ) 
			{
                pTargetAction->setEnabled ( KD_TRUE );
            }
            break;

        case kAPSLinkerTypeDisable :
            if ( pTargetAction ) 
			{
                pTargetAction->setEnabled ( KD_FALSE );
            }
            break;

        case kAPSLinkerTypeInterruptAll :
            pSymbol->interruptAllActions ( );
            break;

        default :
            break;
    }
}

APSAction* APSLinkerAction::getTargetAction ( KDvoid ) 
{
    if ( !m_pTargetAction )
	{
        m_pTargetAction = (APSAction*) this->getResource ( this->m_uTargetActionIndex );
    }
    return m_pTargetAction;
}

//
// #### Protected ####
//

CCFiniteTimeAction* APSLinkerAction::makeCCAction ( KDvoid ) 
{
    //!!! Fixme: If LinkerAction objects are created indefinetely,  the same tag could be reused.
    static KDint  nActionTagCount = 0;
    
    CCFiniteTimeAction*  pAction = APSAction::makeCCAction ( );
    
    pAction->setTag ( nActionTagCount++ );
    return pAction;
}

// Makes a CCFiniteTimeAction that runs between pre and post actions.
CCFiniteTimeAction* APSLinkerAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    CCCallFunc::create ( this, callfunc_selector ( APSLinkerAction::sendLinkMessage ) );
    return 0; //(CCFiniteTimeAction*) CCCallFunc::create ( this, callfunc_selector ( APSLinkerAction::sendLinkMessage ) );
}

NS_APS_END


