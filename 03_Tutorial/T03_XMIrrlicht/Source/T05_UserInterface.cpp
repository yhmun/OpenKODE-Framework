/* --------------------------------------------------------------------------
 *
 *      File            T05_UserInterface.cpp
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

#include "Precompiled.h"
#include "T05_UserInterface.h"

CT05_UserInterface::CT05_UserInterface ( KDvoid )
{
	s_pDevice->getFileSystem ( )->changeWorkingDirectoryTo ( "/res" );

	/*
		To make the font a little bit nicer, we load an external font
		and set it as the new default font in the skin.
		To keep the standard font for tool tip text, we set it to
		the built-in font.
	*/

	gui::IGUISkin*  pSkin = m_pGuiEnv->getSkin ( );
	gui::IGUIFont*  pFont = m_pGuiEnv->getFont ( "/res/media/fonthaettenschweiler.bmp" );
	if ( pFont )
	{
		pSkin->setFont ( pFont );
	}

	pSkin->setFont ( m_pGuiEnv->getBuiltInFont ( ), gui::EGDF_TOOLTIP );

	/*
		We add three buttons. The first one closes the engine. The second
		creates a window and the third opens a file open dialog. The third
		parameter is the id of the button, with which we can easily identify
		the button in the event receiver.
	*/	
	m_pGuiEnv->addButton 
	(
		core::rect<s32> ( 10, 240, 110, 240 + 32 ),
		0,
		GUI_ID_QUIT_BUTTON,
		L"Quit",
		L"Exits Program"
	);

	m_pGuiEnv->addButton
	(
		core::rect<s32> ( 10, 280, 110, 280 + 32 ),
		0,
		GUI_ID_NEW_WINDOW_BUTTON,
		L"New Window", 
		L"Launches a new Window"
	);

	m_pGuiEnv->addButton 
	(
		core::rect<s32> ( 10, 320, 110, 320 + 32 ),
		0,
		GUI_ID_FILE_OPEN_BUTTON,
		L"File Open", 
		L"Opens a file" 
	);

	/*
		Now, we add a static text and a scrollbar, which modifies the
		transparency of all gui elements. We set the maximum value of
		the scrollbar to 255, because that's the maximal value for
		a color value.
		Then we create an other static text and a list box.
	*/
	m_pGuiEnv->addStaticText
	(
		L"Transparent Control:",
		core::rect<s32> ( 150, 20, 350, 40 ),
		true
	);

	gui::IGUIScrollBar*  pScrollbar = m_pGuiEnv->addScrollBar 
	(
		true,
		core::rect<s32> ( 150, 45, 350, 60 ),
		0,
		GUI_ID_TRANSPARENCY_SCROLL_BAR
	);

	pScrollbar->setMax ( 255 );

	// set scrollbar position to alpha value of an arbitrary element
	pScrollbar->setPos ( m_pGuiEnv->getSkin ( )->getColor ( gui::EGDC_WINDOW ).getAlpha ( ) );

	m_pGuiEnv->addStaticText
	(
		L"Logging ListBox:", 
		core::rect<s32> ( 50, 110, 250, 130 ),
		true 
	);

	gui::IGUIListBox*  pListbox = m_pGuiEnv->addListBox ( core::rect<s32> ( 50, 140, 250, 210 ) );
	m_pGuiEnv->addEditBox ( L"Editable Text", core::rect<s32> ( 350, 80, 550, 100 ) );

	m_nCount = 0;
	m_pListbox = pListbox;
}

CT05_UserInterface::~CT05_UserInterface ( KDvoid )
{
	
}

video::SColor CT05_UserInterface::getClear ( KDvoid )
{
	return video::SColor ( 0, 200, 200, 200 );
}

bool CT05_UserInterface::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	if ( event.EventType == EET_GUI_EVENT )
	{
		s32  nID = event.GUIEvent.Caller->getID ( );

		switch ( event.GUIEvent.EventType )
		{
			/*
				If a scrollbar changed its scroll position, and it is
				'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR), then we change
				the transparency of all gui elements. This is a very
				easy task: There is a skin object, in which all color
				settings are stored. We simply go through all colors
				stored in the skin and change their alpha value.
			*/
			case gui::EGET_SCROLL_BAR_CHANGED :
	
				if ( nID == GUI_ID_TRANSPARENCY_SCROLL_BAR )
				{
					s32  tPos = ( (gui::IGUIScrollBar*) event.GUIEvent.Caller )->getPos ( );
					
					for ( u32 i = 0; i < gui::EGDC_COUNT; ++i )
					{
						video::SColor  tCol = m_pGuiEnv->getSkin ( )->getColor ( (gui::EGUI_DEFAULT_COLOR) i );
						tCol.setAlpha ( tPos );
						m_pGuiEnv->getSkin ( )->setColor ( (gui::EGUI_DEFAULT_COLOR) i, tCol );
					}
				}
	
				break;

		/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
		*/
		case gui::EGET_BUTTON_CLICKED :

			switch ( nID )
			{
				case GUI_ID_QUIT_BUTTON :

					s_pDevice->closeDevice ( );
					
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
						m_pListbox->addItem ( L"Window created" );
						m_nCount += 30;
						if ( m_nCount > 200 )
						{
							m_nCount = 0;
						}

						gui::IGUIWindow*  pWindow = m_pGuiEnv->addWindow
						(
							core::rect<s32> ( 100 + m_nCount, 100 + m_nCount, 300 + m_nCount, 200 + m_nCount ),
							false, // modal?
							L"Test window"
						);

						m_pGuiEnv->addStaticText
						(
							L"Please close me",
							core::rect<s32> ( 35, 35, 140, 50 ),
							true,	 // border?
							false,	// wordwrap?
							pWindow
						);
					}
	
					return true;

				case GUI_ID_FILE_OPEN_BUTTON :
	
					m_pListbox->addItem ( L"File open" );
					m_pGuiEnv->addFileOpenDialog ( L"Please choose a file." );

					return true;

				default :

					return false;
			}

			break;

			default :
				
				break;
		}
	}

	return false;
}

const wchar_t* CT05_UserInterface::getTitle ( KDvoid )
{
	return L"05. Movement";
}