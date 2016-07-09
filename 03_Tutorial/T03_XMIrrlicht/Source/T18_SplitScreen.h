/* --------------------------------------------------------------------------
 *
 *      File            T18_SplitScreen.h
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

#ifndef __T18_SplitScreen_h__
#define __T18_SplitScreen_h__

/** Example 018 Splitscreen

	A tutorial by Max Winkel.

	In this tutorial we'll learn how to use splitscreen (e.g. for racing-games)
	with Irrlicht. We'll create a viewport divided
	into 4 parts, wtih 3 fixed cameras and one user-controlled.

	Ok, let's start with the headers (I think there's
	nothing to say about it)
*/
class CT18_SplitScreen : public CTutorial
{
	public :
	
		CT18_SplitScreen ( KDvoid );

		virtual ~CT18_SplitScreen ( KDvoid );

	public :

		virtual KDvoid  Draw ( KDvoid );

		virtual video::SColor  getClear ( KDvoid );	

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual bool OnEvent ( const SEvent& event );

	private :

		bool						m_bInit;
		bool						m_bSplitScreen;
		scene::ICameraSceneNode*	m_pCamera [ 4 ];
};

#endif