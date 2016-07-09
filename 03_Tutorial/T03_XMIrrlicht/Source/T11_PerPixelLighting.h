/* --------------------------------------------------------------------------
 *
 *      File            T11_PerPixelLighting.h
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

#ifndef __T11_PerPixelLighting_h__
#define __T11_PerPixelLighting_h__

/** Example 011 Per-Pixel Lighting

	This tutorial shows how to use one of the built in more complex materials in
	irrlicht: Per pixel lighted surfaces using normal maps and parallax mapping. It
	will also show how to use fog and moving particle systems. And don't panic: You
	dont need any experience with shaders to use these materials in Irrlicht.

	At first, we need to include all headers and do the stuff we always do, like in
	nearly all other tutorials.
*/
class CT11_PerPixelLighting : public CTutorial
{
	public :
	
		CT11_PerPixelLighting ( KDvoid );

		virtual ~CT11_PerPixelLighting ( KDvoid );

	public :

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual bool OnEvent ( const SEvent& event );

	protected :

		void  setMaterial ( );

	private :

		scene::ISceneNode*      m_pRoom;
		scene::ISceneNode*      m_pEarth;
		gui::IGUIListBox*		m_pListbox;
		gui::IGUIStaticText*    m_pProblem;
};

#endif