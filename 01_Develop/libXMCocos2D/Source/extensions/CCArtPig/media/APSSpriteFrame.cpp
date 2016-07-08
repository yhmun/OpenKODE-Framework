/* --------------------------------------------------------------------------
 *
 *      File            APSSpriteFrame.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Created by Kim Kiyoung on 5/7/12.
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
#include "extensions/CCArtPig/APSSpriteFrame.h"
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"

NS_APS_BEGIN

const std::string& APSSpriteFrame::getNameKey ( KDvoid ) 
{
    static const std::string  sKey = "name";
    return sKey;
}

const std::string& APSSpriteFrame::getFrameIndexKey ( KDvoid )
{
    static const std::string  sKey = "frameIndex";
    return sKey;
}

const std::string& APSSpriteFrame::getInternalNameKey ( KDvoid ) 
{
    static const std::string  sKey = "itnName";
    return sKey;
}

APSSpriteFrame::APSSpriteFrame ( KDvoid )
{
	m_pSpriteFrame		 = KD_NULL;
	m_pSpriteSheetHolder = KD_NULL;
	m_uFrameIndex		 = 0;
}

APSSpriteFrame::APSSpriteFrame ( APSDictionary* pProperties, APSSpriteSheetHolder* pSprietSheetHolder ) 
{
	m_pSpriteFrame		 = KD_NULL;
	m_pSpriteSheetHolder = pSprietSheetHolder;
	m_uFrameIndex		 = 0;

    // Set name
    APSString*  pString = (APSString*) pProperties->getObjectForKey ( this->getNameKey ( ) );
    if ( pString )
	{
        this->setName ( pString->c_str ( ) );
    }
    
    // Set internalName
    pString = (APSString*) pProperties->getObjectForKey ( this->getInternalNameKey ( ) );
    if ( pString ) 
	{
        this->setInternalName ( pString->c_str ( ) );
    }
    
    // Set frameIndex
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getFrameIndexKey ( ) );
    if ( pNumber ) 
	{
        this->setFrameIndex ( pNumber->getUnsignedInteger ( ) );
    }
}

NS_APS_END
