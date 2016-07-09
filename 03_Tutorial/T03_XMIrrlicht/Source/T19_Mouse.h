/* --------------------------------------------------------------------------
 *
 *      File            T19_Mouse.h
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

#ifndef __T19_Mouse_h__
#define __T19_Mouse_h__

/** Example 019 Mouse and Joystick

	This tutorial builds on example 04.Movement which showed how to
	handle keyboard events in Irrlicht.  Here we'll handle mouse events
	and joystick events, if you have a joystick connected and a device
	that supports joysticks.  These are currently Windows, Linux and SDL
	devices.
*/

class CT19_Mouse : public CTutorial
{
	public :
	
		CT19_Mouse ( KDvoid );

		virtual ~CT19_Mouse ( KDvoid );

	public :

		// This is the one method that we have to implement
		virtual bool  OnEvent ( const SEvent& event );

		virtual KDvoid  Draw ( KDvoid );

		virtual video::SColor  getClear ( KDvoid );	

		virtual const wchar_t*  getTitle ( KDvoid );

	private :

		// We'll create a struct to record info on the mouse state
		struct SMouseState
		{
			core::position2di		Position;
			bool					LeftButtonDown;

			SMouseState ( KDvoid ) : LeftButtonDown ( false ) { }
		} m_tMouseState;

		u32							m_uThen;
		scene::ISceneNode*			m_pNode;
		scene::ICameraSceneNode*	m_pCamera;
};



#endif