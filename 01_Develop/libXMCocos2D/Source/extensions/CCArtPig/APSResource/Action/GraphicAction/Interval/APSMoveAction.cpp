/* --------------------------------------------------------------------------
 *
 *      File            APSMoveAction.cpp
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
#include "extensions/CCArtPig/APSMoveAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"

NS_APS_BEGIN

const std::string& APSMoveAction::getEndPositionKey ( KDvoid )
{
    static const std::string  sKey = "endPosition";
    return sKey;
}

APSMoveAction::APSMoveAction ( APSDictionary* pProperties ) 
{
	m_tStartPosition = CCPointZero;
	m_tEndPosition   = CCPointZero;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSMoveAction::initWithProperties ( APSDictionary* pProperties ) 
{
    if ( !APSIntervalGraphicAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
        
    // set endPosition
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getEndPositionKey ( ) );
    if ( pStr ) 
	{
        this->setEndPosition ( pStr->getPointValue ( ) );
    }
    
    return KD_TRUE;    
}

//
// #### Protected ####
//

// Saves current state of target graphic
KDvoid APSMoveAction::saveNodeState ( KDvoid ) 
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        CCPoint  tPos = pGraphic->getNode ( )->getPosition ( );
        this->m_tStartPosition = CCPoint ( tPos.x, tPos.y );
    }
}

// Restores saved value to target graphic
KDvoid APSMoveAction::restoreNodeState ( KDvoid ) 
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        pGraphic->getNode ( )->setPosition ( CCPoint ( this->m_tStartPosition.x, this->m_tStartPosition.y ) );
    }
}

CCActionInterval* APSMoveAction::makeGraphicCCAction( KDbool bUniversal, KDfloat fScale )
{
    // If universal==false, APSMoveAction manages restoring graphic states, then there is no need CCAction to restore graphic state.
    KDbool   bFinishRecovery = bUniversal && this->isRestoringAction ( );
    CCPoint  tEndPosition = this->getEndPosition ( );
    tEndPosition.x *= fScale;
    tEndPosition.y *= fScale;
    
    if ( this->getRelative ( ) )
	{
        return APSCCMoveBy::actionWithDuration ( this->getDuration ( ), tEndPosition, bFinishRecovery );
    } 
	else 
	{
        return APSCCMoveTo::actionWithDuration ( this->getDuration ( ), tEndPosition, bFinishRecovery );
    }
}

NS_APS_END

