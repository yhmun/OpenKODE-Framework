/* --------------------------------------------------------------------------
 *
 *      File            APSMedium.cpp
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
#include "extensions/CCArtPig/APSMedium.h"
#include "extensions/CCArtPig/APSResourceManager.h"

NS_APS_BEGIN

const std::string& APSMedium::getCodeKey ( KDvoid )
{
    static const std::string  sKey = "code";
    return sKey;
}

const std::string& APSMedium::getFilenameKey ( KDvoid )
{
    static const std::string  sKey = "filename";
    return sKey;
}

APSMedium::APSMedium ( const KDchar* szCode, APSResourceManager* pManager ) 
{
	m_sCode = szCode;
	m_pResourceManager = pManager;
}

APSMedium::APSMedium ( APSDictionary* pProperties ) 
{
	m_pResourceManager = KD_NULL;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSMedium::initWithProperties ( APSDictionary* pProperties )
{    
    // Set code
    APSString*  pString = (APSString*) pProperties->getObjectForKey ( this->getCodeKey ( ) );
    if ( pString ) 
	{
        this->setCode ( pString->c_str ( ) );
    }

    // Set filename
    pString = (APSString*) pProperties->getObjectForKey ( this->getFilenameKey ( ) );
    if ( pString )
	{
        this->setFilename ( pString->c_str ( ) );
    }
    
    return true;
}

KDvoid APSMedium::setResourceManager ( APSResourceManager* pManager ) 
{
    m_pResourceManager = pManager;
}

APSResourceManager* APSMedium::getResourceManager ( KDvoid ) const
{
    return m_pResourceManager;
}

NS_APS_END
