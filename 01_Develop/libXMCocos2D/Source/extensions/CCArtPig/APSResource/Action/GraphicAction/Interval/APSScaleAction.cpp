/* --------------------------------------------------------------------------
 *
 *      File            APSScaleAction.cpp
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
#include "extensions/CCArtPig/APSScaleAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"

NS_APS_BEGIN

const std::string& APSScaleAction::getEndScaleKey ( KDvoid ) 
{
    static const std::string  sKey = "endScale";
    return sKey;
}

APSScaleAction::APSScaleAction ( APSDictionary* pProperties ) 
{
	m_tStartScale = CCPointZero;
	m_tEndScale   = CCPointZero;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSScaleAction::initWithProperties ( APSDictionary* pProperties ) 
{
    KDbool  bOk = APSIntervalGraphicAction::initWithProperties ( pProperties );
    
    // set endScale
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getEndScaleKey ( ) );
    if ( pStr )
	{
        this->m_tEndScale = pStr->getPointValue ( );
    }

    return bOk;   
}

//
// #### Protected ####
//
KDvoid APSScaleAction::saveNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        CCNode*  pNode = pGraphic->getNode ( );
        this->m_tStartScale.x = pNode->getScaleX ( );
        this->m_tStartScale.y = pNode->getScaleY ( );
    }
}

KDvoid APSScaleAction::restoreNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        CCNode*  pNode = pGraphic->getNode ( );
        pNode->setScaleX ( this->m_tStartScale.x );
        pNode->setScaleY ( this->m_tStartScale.y );
    }
}

CCActionInterval* APSScaleAction::makeGraphicCCAction ( KDbool bUniversal, KDfloat fScale )
{
    // If universal==false, APSScaleAction manages restoring graphic states, then there is no need CCAction to restore graphic state.
    KDbool  bFinishRecovery = bUniversal && this->isRestoringAction ( );
    if ( this->getRelative ( ) )
	{
        return APSCCScaleBy::actionWithDuration ( this->getDuration ( ), this->m_tEndScale.x, this->m_tEndScale.y, bFinishRecovery );
    } 
	else 
	{
        return APSCCScaleTo::actionWithDuration ( this->getDuration ( ), this->m_tEndScale.x, this->m_tEndScale.y, bFinishRecovery );
    }
}

NS_APS_END
