/* --------------------------------------------------------------------------
 *
 *      File            APSInstantMoveAction.cpp
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
#include "extensions/CCArtPig/APSInstantMoveAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "actions/CCActionInstant.h"
#include "base_nodes/CCNode.h"

NS_APS_BEGIN

const std::string& APSInstantMoveAction::getEndPositionKey ( KDvoid )
{
	static const std::string  sKey = "endPosition";
    return sKey;
}

APSInstantMoveAction::APSInstantMoveAction ( APSDictionary* pProperties )
{
	m_tEndPosition = CCPointZero;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSInstantMoveAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSInstantGraphicAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
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
KDvoid APSInstantMoveAction::instantMove ( CCNode* pSender )
{
    if ( this->getRelative ( ) )
	{
        CCPoint  tSS = this->getTargetGraphic ( )->getNode ( )->getPosition ( );
        CCPoint  tES = this->getEndPosition ( );
        pSender->setPosition ( CCPoint ( tSS.x + tES.x, tSS.y + tES.y ) );
    } 
	else 
	{
        pSender->setPosition ( this->getEndPosition ( ) );
    }
}

// Makes a CCFiniteTimeAction that runs between pre and post actions.
CCFiniteTimeAction* APSInstantMoveAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale ) 
{
    return CCCallFuncN::create ( this, callfuncN_selector ( APSInstantMoveAction::instantMove ) );
}

NS_APS_END
