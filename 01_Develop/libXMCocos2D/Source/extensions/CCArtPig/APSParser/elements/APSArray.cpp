/* --------------------------------------------------------------------------
 *
 *      File            APSArray.cpp
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
#include "extensions/CCArtPig/APSArray.h"
#include "extensions/CCArtPig/APSString.h"

NS_APS_BEGIN

APSArray::APSArray ( KDvoid )
{

}

APSArray::APSArray ( APSArray* pArray )
{
    this->addObjectsFromArray ( pArray );
}

APSArray::~APSArray ( KDvoid )
{
    this->clear ( );
}

APSArray* APSArray::array ( KDvoid )
{
    APSArray*  pRet = new APSArray ( );
    pRet->autorelease ( );
    return pRet;
}

APSObject* APSArray::getObjectAtIndex ( KDuint uIndex ) const
{
    return this->m_aArray [ uIndex ];
}

KDvoid APSArray::addObject ( APSObject* pObject )
{
    if ( pObject ) 
	{
        pObject->retain ( );
        this->m_aArray.push_back ( pObject );
    }
}

KDvoid APSArray::addObjectsFromArray ( APSArray* pArray )  
{
    if ( pArray ) 
	{
        APS_FOREACH ( APSArrayStorage, *pArray, iter )
		{
            if ( *iter )
			{
                this->addObject ( *iter );
            }
        }
    }
}

KDvoid APSArray::insertObjectAtIndex ( APSObject* pObject, KDuint uIndex ) 
{
    if ( pObject ) 
	{
        pObject->retain ( );
        this->m_aArray.insert ( this->m_aArray.begin ( ) + uIndex, pObject );
    }
}

KDvoid APSArray::removeObjectAtIndex ( KDuint uIndex )
{
    APS_SAFE_RELEASE ( this->m_aArray [ uIndex ] );
    this->m_aArray.erase ( this->m_aArray.begin ( ) + uIndex );
}

KDvoid APSArray::clear ( KDvoid ) 
{
	APS_FOREACH ( std::vector<APSObject*>, this->m_aArray, iter )
	{
        APS_SAFE_RELEASE ( (*iter) )
    }
    this->m_aArray.clear ( );
}

NS_APS_END


