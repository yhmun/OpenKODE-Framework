/* --------------------------------------------------------------------------
 *
 *      File            APSGraphicAction.cpp
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
#include "extensions/CCArtPig/APSGraphicAction.h"

NS_APS_BEGIN

const KDbool kAPSGraphicAction_defaultRelative = KD_TRUE;

const std::string& APSGraphicAction::getRelativeKey ( KDvoid )
{
	static const std::string  sKey = "relative";
    return sKey;
}

APSGraphicAction::APSGraphicAction ( APSDictionary* pProperties )
{
	m_bRelative = kAPSGraphicAction_defaultRelative;
    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSGraphicAction::isGraphicAction ( APSAction* pAction )
{
    switch ( pAction->getActionType ( ) )
	{
        case kAPSActionTypeMove			:                    
        case kAPSActionTypeScale		:
        case kAPSActionTypeFade			:
        case kAPSActionTypeVisibility	:
        case kAPSActionTypeRotation		:
            return KD_TRUE;
    }
    return KD_FALSE;
}

KDbool APSGraphicAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    APSNumber*  pNumber = KD_NULL;
    // set relative
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getRelativeKey ( ) );
    if ( pNumber )
	{
        this->setRelative ( pNumber->getBoolean ( ) );
    }
    
    return KD_TRUE;
}

CCFiniteTimeAction* APSGraphicAction::universalCCAction ( KDfloat fScale ) 
{
    return this->makeMidCCAction ( KD_TRUE, fScale );
}

NS_APS_END
