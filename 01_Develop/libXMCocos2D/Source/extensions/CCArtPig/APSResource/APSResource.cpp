/* --------------------------------------------------------------------------
 *
 *      File            APSResource.cpp
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
#include "extensions/CCArtPig/APSResource.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSResourceManager.h"

NS_APS_BEGIN

const std::string& APSResource::getTagKey ( KDvoid )
{
    static const std::string  sKey = "tag";
    return sKey;
}

const std::string& APSResource::getIndexKey ( KDvoid )
{
    static const std::string  sKey = "index";
    return sKey;
}

const std::string& APSResource::getParentSymbolKey ( KDvoid )
{
    static const std::string  sKey = "parentSymbol";
    return sKey;
}

APSResource::APSResource ( APSDictionary* pProperties )
{    
	m_uIndex = APS_NIL_INDEX;
	m_pParentSymbol = KD_NULL;

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSResource::initWithProperties ( APSDictionary* pProperties )
{
    // set index
    APSNumber*  pIndexNum = (APSNumber*) pProperties->getObjectForKey ( this->getIndexKey ( ) );
    if ( pIndexNum )
	{
        this->setIndex ( APSNumber::indexWithNumber ( pIndexNum ) );
    }
    
    // set tag
    APSString*  pTag = (APSString*) pProperties->getObjectForKey ( this->getTagKey ( ) );
    if ( pTag )
	{
        this->setTag ( pTag->c_str ( ) );
    }
    
    return KD_TRUE;
}

APSResource* APSResource::getResource ( APSIndex uIndex ) const 
{
    APSSymbol*  pSymbol = this->m_pParentSymbol;
    if ( pSymbol )
	{
        return pSymbol->getResourceAtIndex ( uIndex );
    }
    return NULL;
}

APSMedium* APSResource::getMediumForCode ( const std::string& sCode )
{
    APSSymbol*  pSymbol = this->m_pParentSymbol;
    if ( pSymbol ) 
	{
        return pSymbol->getResourceManager ( )->getMediumForCode ( sCode );
    }
    return KD_NULL;
}

NS_CC_END

