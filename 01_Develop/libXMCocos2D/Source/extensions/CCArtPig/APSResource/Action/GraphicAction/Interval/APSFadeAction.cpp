/* --------------------------------------------------------------------------
 *
 *      File            APSFadeAction.cpp
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
#include "extensions/CCArtPig/APSFadeAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"

NS_APS_BEGIN

const KDuint  kAPSFadeAction_defautEndOpacity = APS_MAX_OPACITY;

const std::string& APSFadeAction::getEndOpacityKey ( KDvoid ) 
{
    static const std::string  sKey = "endOpacity";
    return sKey;
}

APSFadeAction::APSFadeAction ( APSDictionary* pProperties ) 
{
	m_uStartOpacity = APS_MAX_OPACITY;
	m_uEndOpacity   = kAPSFadeAction_defautEndOpacity;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSFadeAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSIntervalGraphicAction::initWithProperties ( pProperties ) )
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

CCActionInterval* APSFadeAction::makeGraphicCCAction ( KDbool bUniversal, KDfloat fScale ) 
{
    if ( bUniversal ) 
	{
        return APSCCFadeTo::actionWithDuration ( this->getDuration ( ), this->getEndOpacity ( ), this->isRestoringAction ( ) );
    } 
	else 
	{
        // If universal==false, APSFadeAction manages restoring graphic states, then there is no need CCAction to restore graphic state.
        return APSCCFadeChildrenTo::actionWithDuration ( this->getDuration ( ), this->getEndOpacity ( ), this->getTargetGraphic ( ) );
    }
}

KDvoid APSFadeAction::saveNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        this->m_uStartOpacity = pGraphic->getRunningOpacity ( );
    }
}

KDvoid APSFadeAction::restoreNodeState ( KDvoid ) 
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        pGraphic->setRunningOpacity ( this->m_uStartOpacity );
        pGraphic->updateAccOpacity ( );
    }
}

NS_APS_END


