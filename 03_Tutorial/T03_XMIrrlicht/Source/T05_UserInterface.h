/* --------------------------------------------------------------------------
 *
 *      File            T05_UserInterface.h
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

#ifndef __T05_UserInterface_h__
#define __T05_UserInterface_h__

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON				= 101,
	GUI_ID_NEW_WINDOW_BUTTON			 ,
	GUI_ID_FILE_OPEN_BUTTON				 ,
	GUI_ID_TRANSPARENCY_SCROLL_BAR		 ,
};

class CT05_UserInterface : public CTutorial
{
	public :
	
		CT05_UserInterface ( KDvoid );

		virtual ~CT05_UserInterface ( KDvoid );

	public :

		virtual video::SColor  getClear ( KDvoid );	

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual bool OnEvent ( const SEvent& event );

	private :

		s32						m_nCount;
		gui::IGUIListBox*		m_pListbox;
};

#endif