/* --------------------------------------------------------------------------
 *
 *      File            MyContactListener.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
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
#include "MyContactListener.h"

MyContactListener::MyContactListener ( KDvoid ) : m_aContacts ( )
{

}

MyContactListener::~MyContactListener ( KDvoid )
{

}

KDvoid MyContactListener::BeginContact ( b2Contact* pContact )
{
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    MyContact	tMyContact = { pContact->GetFixtureA ( ), pContact->GetFixtureB ( ) };
    m_aContacts.push_back ( tMyContact );
}

KDvoid MyContactListener::EndContact ( b2Contact* pContact ) 
{
    MyContact	tMyContact = { pContact->GetFixtureA ( ), pContact->GetFixtureB ( ) };
    std::vector<MyContact>::iterator	itPos;

    itPos = std::find ( m_aContacts.begin ( ), m_aContacts.end ( ), tMyContact );
    if ( itPos != m_aContacts.end ( ) )
	{
        m_aContacts.erase ( itPos );
    }
}

KDvoid MyContactListener::PreSolve ( b2Contact* pContact, const b2Manifold* pOldManifold )
{

}

KDvoid MyContactListener::PostSolve( b2Contact* pContact, const b2ContactImpulse* pImpulse )
{

}
