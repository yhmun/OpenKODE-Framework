/* --------------------------------------------------------------------------
 *
 *      File            APSRotationAction.cpp
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
#include "extensions/CCArtPig/APSRotationAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"

NS_APS_BEGIN

const std::string& APSRotationAction::getEndRotationKey ( KDvoid ) 
{
    static const std::string  sKey = "endRotation";
    return sKey;
}

APSRotationAction::APSRotationAction ( APSDictionary* pProperties ) 
{
	m_fStartRotation = 0.f;
	m_fEndRotation	 = 0.f;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSRotationAction::initWithProperties ( APSDictionary* pProperties )
{
    KDbool  bOk = APSIntervalGraphicAction::initWithProperties ( pProperties );
    
    // set endRotation
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getEndRotationKey ( ) );
    if ( pNumber ) 
	{
        this->m_fEndRotation = pNumber->getFloat ( );
    }
    
    return bOk;
}

//
// #### Protected ####
//

KDvoid APSRotationAction::saveNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        this->m_fStartRotation = pGraphic->getNode ( )->getRotation ( );
    }
}

KDvoid APSRotationAction::restoreNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        pGraphic->getNode ( )->setRotation ( this->m_fStartRotation );
    }
}

CCActionInterval* APSRotationAction::makeGraphicCCAction ( KDbool bUniversal, KDfloat fScale ) 
{
    // If universal==false, APSRotationAction manages restoring graphic states, then there is no need CCAction to restore graphic state.
    KDbool  bFinishRecovery = bUniversal && this->isRestoringAction ( );
    if ( this->getRelative ( ) )
	{
        return APSCCRotateBy::actionWithDuration ( this->getDuration ( ), this->getEndRotation ( ), bFinishRecovery );
    }
	else 
	{
        return APSCCRotateTo::actionWithDuration ( this->getDuration ( ), this->getEndRotation ( ), bFinishRecovery );
    }
}

NS_APS_END
