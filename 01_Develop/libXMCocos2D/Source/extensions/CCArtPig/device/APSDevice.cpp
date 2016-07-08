/* --------------------------------------------------------------------------
 *
 *      File            APSDevice.cpp
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
#include "extensions/CCArtPig/APSDevice.h"

NS_APS_BEGIN

const std::string& APSDevice::getIdentifierKey ( KDvoid ) 
{
    static const std::string  sKey = "identifier";
    return sKey;
}

const std::string& APSDevice::getEnabledKey ( KDvoid )
{
    static const std::string  sKey = "enabled";
    return sKey;
}

const std::string& APSDevice::getScreenSizeKey ( KDvoid )
{
    static const std::string  sKey = "screenSize";
    return sKey;
}

APSDevice::APSDevice ( const KDchar* szIdentifier, CCSize tScreenSize )
{
	m_tScreenSize	= tScreenSize;
	m_bEnabled		= KD_FALSE;
	m_sIdentifier	= "";

    this->setIdentifier ( szIdentifier );
}

APSDevice::APSDevice(const APSDevice& tDevice )
{
	m_tScreenSize	= tDevice.getScreenSize ( );
	m_bEnabled		= tDevice.getEnabled ( );
	m_sIdentifier	= "";

    this->setIdentifier ( tDevice.getIdentifier ( ) );
}

APSDevice::APSDevice ( APSDictionary* pProperties )
{
	m_tScreenSize	= CCSizeZero;
	m_bEnabled		= KD_FALSE;
	m_sIdentifier	= "";

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSDevice::initWithProperties ( APSDictionary* pProperties )
{
    // set identifier
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getIdentifierKey ( ) );
    if ( pStr )
	{
        this->setIdentifier ( pStr->c_str ( ) );
    }
    
    // set enabled
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getEnabledKey ( ) );
    if ( pNumber ) 
	{
        this->setEnabled ( pNumber->getBoolean ( ) );
    }
    
    // set screenSize
    pStr = (APSString*) pProperties->getObjectForKey ( this->getScreenSizeKey ( ) );
    if ( pStr ) 
	{
        this->setScreenSize ( pStr->getSizeValue ( ) );
    }
    
    return KD_TRUE;
}

NS_APS_END
