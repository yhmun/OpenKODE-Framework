/* --------------------------------------------------------------------------
 *
 *      File            T04_Movement.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __T04_Movement_h__
#define __T04_Movement_h__

class CT04_Movement : public CTutorial
{
	public :
	
		CT04_Movement ( KDvoid );

		virtual ~CT04_Movement ( KDvoid );

	public :

		virtual bool OnEvent ( const SEvent& event );

		virtual video::SColor  getClear ( KDvoid );	

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual KDvoid  Draw ( KDvoid );

	private :

		scene::ISceneNode*		m_pNode;
		u32						m_uThen;
		bool					m_bKeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif