/* --------------------------------------------------------------------------
 *
 *      File            APSObject.cpp
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
#include "extensions/CCArtPig/APSObject.h"
#include "extensions/CCArtPig/APSAutoreleasePool.h"

NS_APS_BEGIN

const std::string& APSObject::getClassNameKey ( KDvoid )
{
	static const std::string  sKey = "className";
    return sKey;
}

APSObject::APSObject ( KDvoid ) 
{
	m_nRetainCount = 1;
}

APSObject* APSObject::retain ( KDvoid ) 
{
    m_nRetainCount++; 
    return this;
}

KDvoid APSObject::release ( KDvoid ) 
{
    if ( --m_nRetainCount <= 0 ) 
	{
        if ( m_nRetainCount < 0 ) 
		{
            APSLOG ( "retainCount is less than zero." );
        }
        delete this; 
    }
}

APSObject* APSObject::autorelease ( KDvoid )
{
    APSAutoreleasePool::addObject ( this );
    return this;
}

NS_APS_END
