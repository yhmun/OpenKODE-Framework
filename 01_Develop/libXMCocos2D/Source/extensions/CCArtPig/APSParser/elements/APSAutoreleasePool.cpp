/* --------------------------------------------------------------------------
 *
 *      File            APSAutoreleasePool.cpp
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
#include "extensions/CCArtPig/APSAutoreleasePool.h"
#include "extensions/CCArtPig/APSObject.h"

NS_APS_BEGIN

static std::vector<APSAutoreleasePool*>  m_aPoolStack;

// Adds an APSObject object at the top pool
KDvoid APSAutoreleasePool::addObject ( APSObject* pObject ) 
{
    if ( m_aPoolStack.size ( ) == 0 ) 
	{
        pushPool ( );
    }
    APSAutoreleasePool*  pPool = *m_aPoolStack.rbegin ( );
    pPool->push_back ( pObject );
}

// Drains the top pool and removes from the pool stack.
KDvoid APSAutoreleasePool::popPool ( KDvoid )
{
    APSAutoreleasePool*  pPool = *m_aPoolStack.rbegin ( );
    pPool->drain ( );
    
    if ( m_aPoolStack.size ( ) > 1 ) 
	{
        m_aPoolStack.resize ( m_aPoolStack.size ( ) -1 );
        delete pPool;
    }
}

// Adds a release pool at the top of the pool stack.
KDvoid APSAutoreleasePool::pushPool ( KDvoid ) 
{
    m_aPoolStack.push_back ( new APSAutoreleasePool );
}

// Drains all objects in this pool.
KDvoid APSAutoreleasePool::drain ( KDvoid )
{
    APS_FOREACH ( APSAutoreleasePool, *this, iter )
	{
        APS_SAFE_RELEASE ( (*iter) );
    }

    this->clear ( );
}

NS_APS_END
