/* --------------------------------------------------------------------------
 *
 *      File            KWStateManager.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/23
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWStateManager_h__
#define __KWStateManager_h__

class KWState;

class KWStateManager : public CCObject
{
	public :		
				 KWStateManager ( KDvoid );
		virtual ~KWStateManager ( KDvoid );

		static KWStateManager*	create ( KWState* pState );

		static KWStateManager*	create ( KWState* pState, CCDictionary* pUserData );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		initWithInitialState ( KWState* pState );
		virtual KDbool		initWithInitialState ( KWState* pState, CCDictionary* pUserData );

		virtual KDvoid		pushState ( KWState* pState );
		virtual KDvoid		pushState ( KWState* pState, CCDictionary* pUserData );

		virtual KDvoid		replaceState ( KWState* pState );
		virtual KDvoid		replaceState ( KWState* pState, CCDictionary* pUserData );

		virtual KDvoid		popState ( KDvoid );

	protected :

	  CCArray*				m_pStateStack;
	  KWState*				m_pRunningState;
};

#endif	// __KWStateManager_h__
