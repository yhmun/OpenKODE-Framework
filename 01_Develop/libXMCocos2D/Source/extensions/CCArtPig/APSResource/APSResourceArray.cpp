/* --------------------------------------------------------------------------
 *
 *      File            APSResourceArray.cpp
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
#include "extensions/CCArtPig/APSResourceArray.h"
#include "extensions/CCArtPig/APSSymbol.h"

NS_APS_BEGIN

APSResourceArray::APSResourceArray ( APSResource* pContainer, KDuint uCapacity )
{
	m_pContainer = pContainer;
	m_bIsPreloaded = KD_FALSE;

    this->reserve ( uCapacity );
}

APSResource* APSResourceArray::getResourceAtIndex ( KDuint uIndex )
{
	std::pair<APSIndex, APSResource*> p = ( *this ) [ uIndex ];
    if ( !p.second )
	{
        p.second = this->getResource ( p.first );
        ( *this ) [ uIndex ] = p;
    }
    return p.second;
}

KDuint APSResourceArray::getIndexOfApsIndex ( APSIndex uIndex ) const 
{
    KDuint  uIdx = 0;
    APS_FOREACH_CONST ( APSResourceArray, *this, iter )
	{
        if ( ( *iter ).first == uIndex )
		{
            return uIdx;
        }
        uIdx++;
    }
    
    return APS_NIL_INDEX;
}

APSResource* APSResourceArray::getResource ( APSIndex uIndex ) const 
{
    if ( this->m_pContainer )
	{
        return this->m_pContainer->getResource ( uIndex );
    }
    return KD_NULL;
}

KDvoid APSResourceArray::pushBack ( APSIndex uApsIndex )
{
	this->push_back ( std::pair<APSIndex, APSResource*> ( uApsIndex, (APSResource*) KD_NULL ) );
}

KDvoid APSResourceArray::pushBack ( APSResource* pResource ) 
{
	this->push_back ( std::pair<APSIndex, APSResource*> ( pResource->getIndex ( ), pResource ) );
}

KDvoid APSResourceArray::insertResource ( KDint nIndex, APSIndex uApsIndex )
{
	this->insert ( this->begin ( ) + nIndex, std::pair<APSIndex, APSResource*> ( uApsIndex, (APSResource*) KD_NULL ) );
}

KDvoid APSResourceArray::insertResource ( KDint nIndex, APSResource* pResource )  
{
	this->insert ( this->begin ( ) + nIndex, std::pair<APSIndex, APSResource*> ( pResource->getIndex ( ), pResource ) );
}

KDvoid APSResourceArray::preloadAll ( KDvoid )
{
    if ( !m_bIsPreloaded )
	{
        KDuint  uSize = this->size ( );
        
        for ( KDuint i = 0; i < uSize; i++ )
		{
            this->getResourceAtIndex ( i );
        }
        
        m_bIsPreloaded = KD_TRUE;
    }
}

NS_APS_END

