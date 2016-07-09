/* --------------------------------------------------------------------------
 *
 *      File            MyContactListener.h
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

#ifndef __MyContactListener_h__
#define __MyContactListener_h__

struct MyContact 
{
    b2Fixture*		pFixtureA;
    b2Fixture*		pFixtureB;

    KDbool			operator == ( const MyContact& tOther ) const
    {
        return ( pFixtureA == tOther.pFixtureA ) && ( pFixtureB == tOther.pFixtureB );
    }
};
    
class MyContactListener : public b2ContactListener
{    
	public :
				 MyContactListener ( KDvoid );
		virtual ~MyContactListener ( KDvoid );
	    
	public :

		virtual KDvoid		BeginContact ( b2Contact* pContact );
		virtual KDvoid		EndContact	 ( b2Contact* pContact );
		virtual KDvoid		PreSolve	 ( b2Contact* pContact, const b2Manifold* pOldManifold );
		virtual KDvoid		PostSolve	 ( b2Contact* pContact, const b2ContactImpulse* pImpulse );

	public :

    	std::vector<MyContact>		m_aContacts;
};

#endif	// __MyContactListener_h__
