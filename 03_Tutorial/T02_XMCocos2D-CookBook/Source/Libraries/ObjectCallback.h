/* --------------------------------------------------------------------------
 *
 *      File            ObjectCallback.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __ObjectCallback_h__
#define __ObjectCallback_h__

class ObjectCallback : public CCObject
{
	public :
		
		static ObjectCallback*		create ( CCObject* pObject, SEL_CallFunc pCallback );

	public :

		virtual KDbool				initWithObject ( CCObject* pObject, SEL_CallFunc pCallback );

		virtual CCObject*			getObject ( KDvoid );
		virtual KDvoid				setObject ( CCObject* pObject );

		virtual SEL_CallFunc		getCallback ( KDvoid );
		virtual KDvoid				setCallback ( SEL_CallFunc pCallback );

	protected :

		CCObject*					m_pObject;
		SEL_CallFunc				m_pCallback;
};

#endif	// __ObjectCallback_h__
