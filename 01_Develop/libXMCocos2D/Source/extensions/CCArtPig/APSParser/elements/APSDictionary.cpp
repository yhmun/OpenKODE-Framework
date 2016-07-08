/* --------------------------------------------------------------------------
 *
 *      File            APSDictionary.cpp
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
#include "extensions/CCArtPig/APSDictionary.h"
#include "extensions/CCArtPig/APSString.h"

NS_APS_BEGIN

APSDictionary::~APSDictionary ( KDvoid ) 
{
    this->clear ( );
}

APSDictionary* APSDictionary::dictionary ( KDvoid )
{
    APSDictionary*  pRet = new APSDictionary ( );
    pRet->autorelease ( );
    return pRet;
}

KDvoid APSDictionary::setObjectForKey ( APSObject* pObject, const std::string& sKey )
{
    APSDictionaryStorage::iterator  iter = this->m_aTable.find ( sKey );
    if ( iter != this->m_aTable.end ( ) )
	{
        if ( iter->second != pObject )
		{
            APS_SAFE_RETAIN  ( pObject );
            APS_SAFE_RELEASE ( iter->second );
            iter->second = pObject;
        }
    }
	else 
	{
        APS_SAFE_RETAIN ( pObject );
        this->m_aTable [ sKey ] = pObject;
    }
}

APSObject* APSDictionary::getObjectForKey ( const std::string& sKey ) 
{
    return this->m_aTable [ sKey ];
}

// Adds to the receiving dictionary the entries from another dictionary.
KDvoid APSDictionary::addEntriesFromDictionary ( const APSDictionary* pOtherDictionary )
{
    // ! Although otherDictionary is converted to none constant object, this loop only accesses its' properties without any change to otherDictionary.
    APS_FOREACH ( APSDictionaryStorage, ( (APSDictionary) *pOtherDictionary ), iter )
	{
        this->setObjectForKey ( ( *iter ).second, ( *iter ).first );
    }
}

KDvoid APSDictionary::removeForKey ( const std::string& sKey ) 
{
    APSDictionaryStorage::iterator  iter = this->m_aTable.find ( sKey );
    if ( iter != this->m_aTable.end ( ) ) 
	{
        APS_SAFE_RELEASE ( iter->second );
        this->m_aTable.erase ( iter );
    }
}

KDvoid APSDictionary::clear ( KDvoid )
{
    APS_FOREACH ( APSDictionaryStorage, this->m_aTable, iter )
	{
        APS_SAFE_RELEASE ( ( *iter ).second );
    }
    this->m_aTable.clear ( );
}

NS_APS_END











