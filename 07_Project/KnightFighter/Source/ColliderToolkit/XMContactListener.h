/* --------------------------------------------------------------------------
 *
 *      File            XMContactListener.h
 *      Description     XMContactListener
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __XMContactListener_h__
#define __XMContactListener_h__

class Contact
{
	public:

	b2Fixture*			fixture [ 2 ];
};

class XMContactListener : public b2ContactListener
{
	public :

				 XMContactListener ( KDvoid );
		virtual ~XMContactListener ( KDvoid );

		virtual KDvoid		BeginContact ( b2Contact* pContact);

		Contact				getListFront ( KDvoid );
		KDvoid				clearContactList ( KDvoid );
		KDbool				isContactListEmpty ( KDvoid );
		KDint				getSize ( KDvoid );
		std::list<Contact>	getContactList ( KDvoid );

	protected :

		std::list<Contact>	m_aContactList;
};

#endif  // __XMContactListener_h__