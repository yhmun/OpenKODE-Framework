/* --------------------------------------------------------------------------
 *
 *      File            APSImageHolder.cpp
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
#include "extensions/CCArtPig/APSImageHolder.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSDeviceInfo.h"
#include "extensions/CCArtPig/APSResourceManager.h"
#include "textures/CCTextureCache.h"

NS_APS_BEGIN

const std::string& APSImageHolder::getModelTextureSizeKey ( KDvoid )
{
    static const std::string  sKey = "modelTextureSize";
    return sKey;
}

APSImageHolder::APSImageHolder ( const KDchar* szCode, APSResourceManager* pManager ) 
: APSMedium ( szCode, pManager )
{
	m_pRuntimeFullPath  = KD_NULL;
	m_pRuntimeFileName  = KD_NULL;
	m_tModelTextureSize = CCSizeZero;
}

APSImageHolder::APSImageHolder ( APSDictionary* pProperties ) 
{
	m_pRuntimeFullPath  = KD_NULL;
	m_pRuntimeFileName  = KD_NULL;
	m_tModelTextureSize = CCSizeZero;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSImageHolder::~APSImageHolder ( KDvoid )
{
    APS_SAFE_DELETE ( m_pRuntimeFileName );
    
    APS_SAFE_DELETE ( m_pRuntimeFullPath );
}

KDbool APSImageHolder::initWithProperties ( APSDictionary* pProperties )
{
    KDbool  bOk = APSMedium::initWithProperties ( pProperties );
    
    if ( bOk ) 
	{
        APSString*  pString = (APSString*) pProperties->getObjectForKey ( this->getModelTextureSizeKey ( ) );
        if ( pString ) 
		{
            this->setModelTextureSize ( pString->getSizeValue ( ) );
        }
    }
    
    return bOk;
}

KDvoid APSImageHolder::preloadData ( KDvoid ) 
{
    APSMedium::preloadData ( );
    
    // Simply calling this method populates the texture data.
    this->getTexture ( );
}

std::string APSImageHolder::getFilenameForDevice ( APSDevice* pDevice )
{
    if ( *this->getFilename ( ) && pDevice ) 
	{
        APSPathFields  tFields;
        APSString::getPathFields ( this->getFilename ( ), &tFields );

        tFields.sName.append ( "_" );
        tFields.sName.append ( pDevice->getIdentifier ( ) );
        tFields.sName.append ( "." );
        tFields.sName.append ( tFields.sExtension );
        return tFields.sName;
    }
    return "";
}

const std::string& APSImageHolder::getRuntimeFilename ( KDvoid ) 
{
    if ( !this->m_pRuntimeFileName )
	{
        // retrieve current device 
        APSDevice*  pDevice = this->getResourceManager ( )->getDeviceInfo ( )->getAppliedDevice ( );
        if ( pDevice ) 
		{
            this->m_pRuntimeFileName = new std::string ( this->getFilenameForDevice ( pDevice ) );
        }
    }

    return *this->m_pRuntimeFileName;
}

CCTexture2D* APSImageHolder::getTexture ( KDvoid ) 
{
    CCTexture2D*  pTexture = KD_NULL;
      
    pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( this->getRuntimeFullPath ( )->c_str ( ) );
    return pTexture;
}

const std::string* APSImageHolder::getRuntimeFullPath ( KDvoid )
{
    if ( !m_pRuntimeFullPath )
	{
		m_pRuntimeFullPath = new std::string ( this->getResourceManager ( )->getPathForFilename ( this->getRuntimeFilename ( ).c_str ( ) ) );
    }
    return m_pRuntimeFullPath;
}

NS_APS_END

