/* --------------------------------------------------------------------------
 *
 *      File            APSInstantFadeAction.cpp
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
#include "extensions/CCArtPig/APSInstantFadeAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

const KDuint kAPSInstantFadeAction_defaultEndOpacity = APS_MAX_OPACITY;

const std::string& APSInstantFadeAction::getEndOpacityKey ( KDvoid ) 
{
	static const std::string  sKey = "endOpacity";
    return sKey;
}

APSInstantFadeAction::APSInstantFadeAction ( APSDictionary* pProperties )
{
	m_uEndOpacity = kAPSInstantFadeAction_defaultEndOpacity;
    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSInstantFadeAction::initWithProperties ( APSDictionary* pProperties ) 
{
    if ( !APSInstantGraphicAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set endOpacity
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getEndOpacityKey ( ) );
    if ( pNumber )
	{
        this->m_uEndOpacity = pNumber->getUnsignedInteger ( );
    }
    
    return KD_TRUE;
    
}

//
// #### Protected ####
//

KDvoid APSInstantFadeAction::instantFade ( CCNode* pSender )
{
    APSGraphicGroup*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        pGraphic->setRunningOpacity ( this->m_uEndOpacity );
        pGraphic->updateAccOpacity ( );
    }
}


// Makes a CCFiniteTimeAction that runs between pre and post actions.
CCFiniteTimeAction* APSInstantFadeAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    return CCCallFuncN::create ( this, callfuncN_selector ( APSInstantFadeAction::instantFade ) );
}

NS_APS_END



