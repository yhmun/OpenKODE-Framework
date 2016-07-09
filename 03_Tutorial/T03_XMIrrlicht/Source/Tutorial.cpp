/* --------------------------------------------------------------------------
 *
 *      File            Tutorial.cpp
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
#include "T01_HelloWorld.h"
#include "T02_Quake3Map.h"
#include "T03_CustomSceneNode.h"
#include "T04_Movement.h"
#include "T05_UserInterface.h"
#include "T06_2DGraphics.h"
#include "T07_Collision.h"
#include "T08_SpecialFX.h"
#include "T09_Meshviewer.h"
#include "T10_Shaders.h"
#include "T11_PerPixelLighting.h"
#include "T12_TerrainRendering.h"
#include "T13_RenderToTexture.h"
#include "T14_Win32Window.h"
#include "T15_LoadIrrFile.h"
#include "T16_Quake3MapShader.h"
#include "T17_HelloWorldMobile.h"
#include "T18_SplitScreen.h"
#include "T19_Mouse.h"
#include "T20_ManagedLights.h"
#include "T21_Quake3Explorer.h"
#include "T22_MaterialViewer.h"
#include "T23_SMeshHandling.h"
#include "T24_CursorControl.h"
#include "T25_XmlHandling.h"
#include "T26_OcclusionQuery.h"
#include "platform.h"

#define CNT_TUTORIAL  18

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE ( XM_SYS_LOG | XM_SYS_QUIT | XM_SYS_LANDSCAPE )
XM_APP_MAIN_END

KD_API KDvoid KD_APIENTRY xmEventProc ( const KDEvent* event )
{
	switch ( event->type )
	{
		case KD_EVENT_NATIVE :  
			{
//				#if !defined ( SHP ) && defined ( _WIN32 ) 
//				KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
//				#endif
			}
			break;
		
		case KD_EVENT_CREATE :
			{
				// event->data.size.width;
				// event->data.size.height;

				// create device and exit if creation failed
				CTutorial::s_pDevice = createDevice ( video::EDT_OGLES1, core::dimension2d<u32> ( event->data.size.width, event->data.size.height ), 32, false, false, false, 0 );
				kdAssert ( CTutorial::s_pDevice );

				CTutorial::s_pDriver = CTutorial::s_pDevice->getVideoDriver ( );
				CTutorial::SetTutorial ( 0 );
			}
			break;

		case KD_EVENT_DESTROY :      
			{
				/*
					In the end, delete the Irrlicht device.
				*/
				CTutorial::s_pDevice->drop ( );
				CTutorial::s_pDevice = KD_NULL;
			}
			break;

		case KD_EVENT_RESIZE :       

			break;

		case KD_EVENT_FOCUS :        
			{
				// event->data.value.i;
				// 1 : focus
			}
			break;

		case KD_EVENT_VISIBLE :    
			{
				// event->data.value.i;
				// 1 : visible
			}
			break;

		case KD_EVENT_REDRAW :     
			{
				CTutorial::Redraw ( );
			}
			break;

		case KD_EVENT_UPDATE :      
			{
				// event->data.update.msec;
			}
			break;

		case KD_EVENT_TOUCH_BEGAN :     
			{
				// event->data.touch.touches;
				// event->data.touch.count;
			}
			break;

		case KD_EVENT_TOUCH_MOVED :            
			{

			}
			break;

		case KD_EVENT_TOUCH_ENDED :            
			{

			}
			break;

		case KD_EVENT_TOUCH_CANCELLED :          
			{

			}
			break;

		case KD_EVENT_KEY_RELEASED :    
			{
				// event->data.keypad.code;		
			}
			break;

		case KD_EVENT_KEY_PRESSED :    
			{

			}
			break;

		case KD_EVENT_ACCELEROMETER :   
			{
				// event->data.accelerometer.x;
				// event->data.accelerometer.y;
				// event->data.accelerometer.z;
			}
			break;

		case KD_EVENT_LOCATION :                 
			{
				// event->data.value.i;
				// KD_NMEA_UPDATED_GPS, KD_NMEA_UPDATED_USER
			}
			break;

		case KD_EVENT_INSERT_TEXT :               
			{
				// event->data.insert.utf8;
			}
			break;

		case KD_EVENT_SERIALIZE :                
			{
				// event->data.serialize.type;
				// event->data.serialize.data;
				// event->data.serialize.size;
			}
			break;
	}

	if ( CTutorial::s_pDevice )
	{
		CTutorial::s_pDevice->EventProc ( event );
	}
}

IrrlichtDevice*			CTutorial::s_pDevice	= KD_NULL;
video::IVideoDriver*	CTutorial::s_pDriver	= KD_NULL;
CTutorial*				CTutorial::s_pTutorial	= KD_NULL;
KDuint					CTutorial::s_uIndex		= 0;

#define GUI_ID_BUTTON_PREV		1000
#define GUI_ID_BUTTON_NEXT		1001

#define GUI_ID_BUTTON_UP        2000
#define GUI_ID_BUTTON_DOWN      2001
#define GUI_ID_BUTTON_LEFT      2002
#define GUI_ID_BUTTON_RIGHT     2003
#define GUI_ID_BUTTON_JUMP      2004

CTutorial::CTutorial ( KDvoid )
{    
	/*
		Get a pointer to the video driver and the SceneManager so that
		we do not always have to call irr::IrrlichtDevice::getVideoDriver(),
		irr::IrrlichtDevice::getSceneManager() and irr::IrrlichtDevice::getGUIEnvironment().
	*/
	m_pScrMgr = s_pDevice->getSceneManager ( );
	m_pGuiEnv = s_pDevice->getGUIEnvironment ( );

	s_pDevice->setEventReceiver ( this );
}

CTutorial::~CTutorial ( KDvoid )
{
	m_pScrMgr->clear ( );
	m_pGuiEnv->clear ( );
}

KDvoid CTutorial::Init ( KDvoid )
{
	const core::dimension2d<u32>  tSize = s_pDriver->getScreenSize ( );

	s32  w = tSize.Width;
	s32  h = tSize.Height;

	m_pGuiEnv->addStaticText
	(
		this->getTitle ( ),
		_rect ( 20, 20, 120, 15 ),
		true, false, 0, -1, true 
	);

	m_pGuiEnv->addImage 
	(
		s_pDriver->getTexture ( "/res/media/irrlichtlogo2.png" ),
		core::position2d<s32> ( w - 130, 10 ) 
	);

	m_pGuiEnv->addButton 
	(
		_rect ( w / 2 - 110, h - 60, 100, 50 ),
		0, GUI_ID_BUTTON_PREV,
		L"Prev"
	);

	m_pGuiEnv->addButton 
	(
		_rect ( w / 2 +  10, h - 60, 100, 50 ),
		0, GUI_ID_BUTTON_NEXT,
		L"Next"
	);

	if ( !getVirPad ( ) )
	{
//		return;
	}
    
	m_pGuiEnv->addButton 
	(
        _rect ( 100, h - 180, 80, 80 ),
        0, GUI_ID_BUTTON_UP, 
        L"Up"
    );    
    
	m_pGuiEnv->addButton 
	(
        _rect ( 100, h - 90, 80, 80 ),
        0, GUI_ID_BUTTON_DOWN,
        L"Down"
    );   
    
	m_pGuiEnv->addButton 
	(
        _rect ( 10, h - 130, 80, 80 ),
        0, GUI_ID_BUTTON_LEFT,
        L"Left"
    );   
    
	m_pGuiEnv->addButton 
	(
        _rect ( 190, h - 130, 80, 80 ),
        0, GUI_ID_BUTTON_RIGHT,
        L"Right"
    );         

	m_pGuiEnv->addButton 
	(
        _rect ( w - 90, h - 90, 80, 80 ),
        0, GUI_ID_BUTTON_JUMP,
        L"Jump"
    );         
}

KDbool CTutorial::getVirPad ( KDvoid )
{
	return KD_FALSE;
}

bool CTutorial::OnEvent ( const SEvent& event )
{    
	if ( event.EventType == EET_GUI_EVENT )
	{
		s32     nID  = event.GUIEvent.Caller->getID ( );
		KDbool  bPad = KD_FALSE;
		SEvent  tEvent;
        
		switch ( event.GUIEvent.EventType )
		{
			case gui::EGET_BUTTON_CLICKED :

				switch ( nID )
				{
					case GUI_ID_BUTTON_PREV :

						s_uIndex = s_uIndex == 0 ? CNT_TUTORIAL - 1 : s_uIndex - 1;

						if ( s_uIndex == 13 || s_uIndex == 16 )
						{
							s_uIndex--;
						}

						SetTutorial ( s_uIndex );

						return true;

					case GUI_ID_BUTTON_NEXT :

						s_uIndex = s_uIndex == CNT_TUTORIAL - 1 ? 0 : s_uIndex + 1;

						if ( s_uIndex == 13 || s_uIndex == 16 )
						{
							s_uIndex++;
						}

						SetTutorial ( s_uIndex );

						return true;

					case GUI_ID_BUTTON_JUMP :

						bPad = KD_TRUE; 
						tEvent.KeyInput.Key = KEY_KEY_J;
				}

			case gui::EGET_ELEMENT_HOVERED :

				switch ( nID )
				{
					case GUI_ID_BUTTON_UP	 : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_UP;	break;
					case GUI_ID_BUTTON_DOWN  : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_DOWN;	break;
					case GUI_ID_BUTTON_LEFT  : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_LEFT;	break;
					case GUI_ID_BUTTON_RIGHT : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_RIGHT;	break;
				}

				if ( bPad )
				{
					tEvent.EventType = EET_KEY_INPUT_EVENT;
					tEvent.KeyInput.PressedDown = 1;	

					s_pDevice->postEventFromUser ( tEvent );
				}

				break;

			case gui::EGET_ELEMENT_LEFT :

				switch ( nID )
				{
					case GUI_ID_BUTTON_UP	 : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_UP;	break;
					case GUI_ID_BUTTON_DOWN  : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_DOWN;	break;
					case GUI_ID_BUTTON_LEFT  : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_LEFT;	break;
					case GUI_ID_BUTTON_RIGHT : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_RIGHT;	break;
					case GUI_ID_BUTTON_JUMP  : bPad = KD_TRUE; tEvent.KeyInput.Key = KEY_KEY_J; break;
				}

				if ( bPad )
				{
					tEvent.EventType = EET_KEY_INPUT_EVENT;
					tEvent.KeyInput.PressedDown = 0;

					s_pDevice->postEventFromUser ( tEvent );
				}

				break;
                
            default : ;
		}
	}

	return false;
}

KDvoid CTutorial::Draw ( KDvoid ) 
{
	m_pScrMgr->drawAll ( );
	m_pGuiEnv->drawAll ( );
}

video::SColor CTutorial::getClear ( KDvoid )
{
	return video::SColor ( 255, 0, 0, 0 );
}

const wchar_t* CTutorial::getTitle ( KDvoid )
{
	return L"No Title";
}

core::rect<s32> CTutorial::_rect ( s32 x, s32 y, s32 w, s32 h )
{
	return core::rect<s32> ( x, y, x + w, y + h );
}

KDvoid CTutorial::Redraw ( KDvoid )
{
	/*
		Ok, now we have set up the scene, lets draw everything: We run the
		device in a while() loop, until the device does not want to run any
		more. This would be when the user closes the window or presses ALT+F4
		(or whatever keycode closes a window).
	*/
	if ( s_pDevice && s_pDevice->run ( ) )
	{
		/*
			Anything can be drawn between a beginScene() and an endScene()
			call. The beginScene() call clears the screen with a color and
			the depth buffer, if desired. Then we let the Scene Manager and
			the GUI Environment draw their content. With the endScene()
			call everything is presented on the screen.
		*/
		s_pDriver->beginScene ( true, true, s_pTutorial->getClear ( ) );

		s_pTutorial->Draw ( );

		s_pDriver->endScene ( );
	}
	else
	{
		kdExit ( 0 );
	}
}

KDvoid CTutorial::SetTutorial ( KDuint uIndex )
{
	if ( s_pTutorial )
	{
		delete s_pTutorial;
		s_pTutorial = KD_NULL;
	}

	switch ( uIndex )
	{
		case  0 :	s_pTutorial = new CT01_HelloWorld		( );	break;
		case  1 :	s_pTutorial = new CT02_Quake3Map		( );	break;
		case  2 :	s_pTutorial = new CT03_CustomSceneNode	( );	break;
		case  3 :	s_pTutorial = new CT04_Movement			( );	break;
		case  4 :	s_pTutorial = new CT05_UserInterface	( );	break;
		case  5 :	s_pTutorial = new CT06_2DGraphics		( );	break;
		case  6 :	s_pTutorial = new CT07_Collision		( );	break;
		case  7 :	s_pTutorial = new CT08_SpecialFX		( );	break;
		case  8 :	s_pTutorial = new CT09_Meshviewer		( );	break;
		case  9 :	s_pTutorial = new CT10_Shaders			( );	break;
		case 10 :	s_pTutorial = new CT11_PerPixelLighting	( );	break;
		case 11 :	s_pTutorial = new CT12_TerrainRendering	( );	break;
		case 12 :	s_pTutorial = new CT13_RenderToTexture	( );	break;
		case 13 :	s_pTutorial = new CT14_Win32Window		( );	break;
		case 14 :	s_pTutorial = new CT15_LoadIrrFile		( );	break;
		case 15 :	s_pTutorial = new CT16_Quake3MapShader	( );	break;
		case 16 :	s_pTutorial = new CT17_HelloWorldMobile	( );	break;
		case 17 :	s_pTutorial = new CT18_SplitScreen		( );	break;
		case 18 :	s_pTutorial = new CT19_Mouse			( );	break;
		case 19 :	s_pTutorial = new CT20_ManagedLights	( );	break;
		case 20 :	s_pTutorial = new CT21_Quake3Explorer	( );	break;
		case 21 :	s_pTutorial = new CT22_MaterialViewer	( );	break;
		case 22 :	s_pTutorial = new CT23_SMeshHandling	( );	break;
		case 23 :	s_pTutorial = new CT24_CursorControl	( );	break;
		case 24 :	s_pTutorial = new CT25_XmlHandling		( );	break;
		case 25 :	s_pTutorial = new CT26_OcclusionQuery	( );	break;
	}	

	if ( s_pTutorial )
	{
		s_pTutorial->Init ( );
	}

	s_uIndex = uIndex;
}

