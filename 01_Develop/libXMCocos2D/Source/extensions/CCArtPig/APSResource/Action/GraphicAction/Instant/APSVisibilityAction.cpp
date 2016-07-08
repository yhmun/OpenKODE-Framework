/* --------------------------------------------------------------------------
 *
 *      File            APSVisibilityAction.cpp
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
#include "extensions/CCArtPig/APSVisibilityAction.h"
#include "extensions/CCArtPig/APSGraphic.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

const KDbool  kAPSVisibilityAction_defaultEndVisibility = KD_TRUE;

const std::string& APSVisibilityAction::getEndVisibilityKey ( KDvoid ) 
{
	static const std::string  sKey = "endVisibility";
    return sKey;
}

APSVisibilityAction::APSVisibilityAction ( APSDictionary* pProperties )
{
	m_bEndVisibility = kAPSVisibilityAction_defaultEndVisibility;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSVisibilityAction::initWithProperties ( APSDictionary* pProperties ) 
{
    KDbool  bOk = APSInstantGraphicAction::initWithProperties ( pProperties );
    
    // set endVisibility
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getEndVisibilityKey ( ) );
    if ( pNumber )
	{
        this->m_bEndVisibility = pNumber->getBoolean ( );
    }
    
    return bOk;   
}

CCFiniteTimeAction* APSVisibilityAction::universalCCAction ( KDfloat fScale ) 
{
    if ( this->getEndVisibility ( ) )
	{
        return CCShow::create ( );
    } 
	else
	{
        return CCHide::create ( );
    }
}

//
// #### Protected ####
//

CCFiniteTimeAction* APSVisibilityAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    if ( m_bEndVisibility )
	{
        return CCShow::create ( );
	}
    else
	{
        return CCHide::create ( );
	}
}

NS_APS_END
