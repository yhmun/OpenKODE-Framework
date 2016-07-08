/* --------------------------------------------------------------------------
 *
 *      File            APSSymbolControlAction.cpp
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
#include "extensions/CCArtPig/APSSymbolControlAction.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

KDvoid APSSymbolControlAction::resetTarget ( KDvoid )
{
    CCLOG ( "reset : %s", this->getParentSymbol ( )->getTag ( ) );
    this->getParentSymbol ( )->initNode ( );
}

KDvoid APSSymbolControlAction::triggerPrologues ( KDvoid ) 
{
    CCLOG ( "trigger prologue : %s", this->getParentSymbol ( )->getTag ( ) );
    this->getParentSymbol ( )->triggerPrologueActionGroups ( );
}

KDvoid APSSymbolControlAction::interruptAllActions ( KDvoid )
{
    CCLOG ( "interrupt all : %s", this->getParentSymbol ( )->getTag ( ) );
    this->getParentSymbol ( )->interruptAllActions ( );
}

//
// #### Protected ####
//

// Makes a CCFiniteTimeAction that runs between pre and post actions.
CCFiniteTimeAction* APSSymbolControlAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{        
    switch ( this->getIndex ( ) )
	{
        case kAPSResetSymbolActionIndex			:	return CCCallFunc::create ( this, callfunc_selector ( APSSymbolControlAction::resetTarget		  ) );
        case kAPSTriggerProloguesActionIndex	:	return CCCallFunc::create ( this, callfunc_selector ( APSSymbolControlAction::triggerPrologues	  ) );
        case kAPSInterruptAllActionIndex		:	return CCCallFunc::create ( this, callfunc_selector ( APSSymbolControlAction::interruptAllActions ) );
    }

    return KD_NULL;        
}

NS_APS_END

