// Copyright (C) 2002-2008 Nikolaus Gebhardt
// Copyright (C) 2008 Redshift Software, Inc.
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CIrrDevice.h"
#include "platform.h"

#include <stdlib.h>
#include "IEventReceiver.h"
#include "../os.h"
#include "../CTimer.h"
#include "irrString.h"
#include "Keycodes.h"
#include "../COSOperator.h"
#include "../../video/Null/CColorConverter.h"
#include "SIrrCreationParameters.h"
#include "SExposedVideoData.h"

//~ #include "irr/base/DeviceIPhone_Apple.h"

namespace irr
{
namespace video
{
	IVideoDriver* createOGLES1Driver(const SIrrlichtCreationParameters& params,
		video::SExposedVideoData& data, io::IFileSystem* io);
}
}

namespace irr
{

//! constructor
CIrrDevice::CIrrDevice(const SIrrlichtCreationParameters& params)
	: CIrrDeviceStub(params)
	, Close(false)
	, Closed(false)
	, WindowActive(false)
	, WindowMinimized(false)
{
	#ifdef _DEBUG
	setDebugName("CIrrDeviceIPhone");
	#endif

	// print version, distribution etc.
	Operator = new COSOperator((core::stringc("")).c_str());
	os::Printer::log(Operator->getOperationSystemVersion(), ELL_INFORMATION);

	// create display
	if (CreationParams.DriverType != video::EDT_NULL)
	{
		// create the display, only if we do not use the null device
		if (!createDisplay())
			return;
	}

	CursorControl = new CCursorControl ( this );

	// create driver
	createDriver();

	if (!VideoDriver)
		return;

	createKeyMap ( );

	createGUIAndScene();
	
	WindowActive = true;
}



//! destructor
CIrrDevice::~CIrrDevice()
{
}



bool CIrrDevice::createDisplay()
{
	return true;
}

#if defined ( _IRR_LINUX_PLATFORM_ )
#include <X11/keysym.h>
#include <X11/Xutil.h>

struct SKeyMap
{
	SKeyMap() {}
	SKeyMap(s32 x11, s32 win32)
		: X11Key(x11), Win32Key(win32)
	{
	}

	KeySym X11Key;
	s32 Win32Key;

	bool operator<(const SKeyMap& o) const
	{
		return X11Key<o.X11Key;
	}
};

static core::array<SKeyMap> KeyMap;

#endif

void CIrrDevice::createKeyMap()
{
#if defined ( _IRR_LINUX_PLATFORM_ )
	KeyMap.reallocate(84);
	KeyMap.push_back(SKeyMap(XK_BackSpace, KEY_BACK));
	KeyMap.push_back(SKeyMap(XK_Tab, KEY_TAB));
	KeyMap.push_back(SKeyMap(XK_ISO_Left_Tab, KEY_TAB));
	KeyMap.push_back(SKeyMap(XK_Linefeed, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_Clear, KEY_CLEAR));
	KeyMap.push_back(SKeyMap(XK_Return, KEY_RETURN));
	KeyMap.push_back(SKeyMap(XK_Pause, KEY_PAUSE));
	KeyMap.push_back(SKeyMap(XK_Scroll_Lock, KEY_SCROLL));
	KeyMap.push_back(SKeyMap(XK_Sys_Req, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_Escape, KEY_ESCAPE));
	KeyMap.push_back(SKeyMap(XK_Insert, KEY_INSERT));
	KeyMap.push_back(SKeyMap(XK_Delete, KEY_DELETE));
	KeyMap.push_back(SKeyMap(XK_Home, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_Left, KEY_LEFT));
	KeyMap.push_back(SKeyMap(XK_Up, KEY_UP));
	KeyMap.push_back(SKeyMap(XK_Right, KEY_RIGHT));
	KeyMap.push_back(SKeyMap(XK_Down, KEY_DOWN));
	KeyMap.push_back(SKeyMap(XK_Prior, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_Page_Up, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_Next, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_Page_Down, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_End, KEY_END));
	KeyMap.push_back(SKeyMap(XK_Begin, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_Num_Lock, KEY_NUMLOCK));
	KeyMap.push_back(SKeyMap(XK_KP_Space, KEY_SPACE));
	KeyMap.push_back(SKeyMap(XK_KP_Tab, KEY_TAB));
	KeyMap.push_back(SKeyMap(XK_KP_Enter, KEY_RETURN));
	KeyMap.push_back(SKeyMap(XK_KP_F1, KEY_F1));
	KeyMap.push_back(SKeyMap(XK_KP_F2, KEY_F2));
	KeyMap.push_back(SKeyMap(XK_KP_F3, KEY_F3));
	KeyMap.push_back(SKeyMap(XK_KP_F4, KEY_F4));
	KeyMap.push_back(SKeyMap(XK_KP_Home, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_KP_Left, KEY_LEFT));
	KeyMap.push_back(SKeyMap(XK_KP_Up, KEY_UP));
	KeyMap.push_back(SKeyMap(XK_KP_Right, KEY_RIGHT));
	KeyMap.push_back(SKeyMap(XK_KP_Down, KEY_DOWN));
	KeyMap.push_back(SKeyMap(XK_Print, KEY_PRINT));
	KeyMap.push_back(SKeyMap(XK_KP_Prior, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_KP_Page_Up, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_KP_Next, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_KP_Page_Down, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_KP_End, KEY_END));
	KeyMap.push_back(SKeyMap(XK_KP_Begin, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_KP_Insert, KEY_INSERT));
	KeyMap.push_back(SKeyMap(XK_KP_Delete, KEY_DELETE));
	KeyMap.push_back(SKeyMap(XK_KP_Equal, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_KP_Multiply, KEY_MULTIPLY));
	KeyMap.push_back(SKeyMap(XK_KP_Add, KEY_ADD));
	KeyMap.push_back(SKeyMap(XK_KP_Separator, KEY_SEPARATOR));
	KeyMap.push_back(SKeyMap(XK_KP_Subtract, KEY_SUBTRACT));
	KeyMap.push_back(SKeyMap(XK_KP_Decimal, KEY_DECIMAL));
	KeyMap.push_back(SKeyMap(XK_KP_Divide, KEY_DIVIDE));
	KeyMap.push_back(SKeyMap(XK_KP_0, KEY_KEY_0));
	KeyMap.push_back(SKeyMap(XK_KP_1, KEY_KEY_1));
	KeyMap.push_back(SKeyMap(XK_KP_2, KEY_KEY_2));
	KeyMap.push_back(SKeyMap(XK_KP_3, KEY_KEY_3));
	KeyMap.push_back(SKeyMap(XK_KP_4, KEY_KEY_4));
	KeyMap.push_back(SKeyMap(XK_KP_5, KEY_KEY_5));
	KeyMap.push_back(SKeyMap(XK_KP_6, KEY_KEY_6));
	KeyMap.push_back(SKeyMap(XK_KP_7, KEY_KEY_7));
	KeyMap.push_back(SKeyMap(XK_KP_8, KEY_KEY_8));
	KeyMap.push_back(SKeyMap(XK_KP_9, KEY_KEY_9));
	KeyMap.push_back(SKeyMap(XK_F1, KEY_F1));
	KeyMap.push_back(SKeyMap(XK_F2, KEY_F2));
	KeyMap.push_back(SKeyMap(XK_F3, KEY_F3));
	KeyMap.push_back(SKeyMap(XK_F4, KEY_F4));
	KeyMap.push_back(SKeyMap(XK_F5, KEY_F5));
	KeyMap.push_back(SKeyMap(XK_F6, KEY_F6));
	KeyMap.push_back(SKeyMap(XK_F7, KEY_F7));
	KeyMap.push_back(SKeyMap(XK_F8, KEY_F8));
	KeyMap.push_back(SKeyMap(XK_F9, KEY_F9));
	KeyMap.push_back(SKeyMap(XK_F10, KEY_F10));
	KeyMap.push_back(SKeyMap(XK_F11, KEY_F11));
	KeyMap.push_back(SKeyMap(XK_F12, KEY_F12));
	KeyMap.push_back(SKeyMap(XK_Shift_L, KEY_LSHIFT));
	KeyMap.push_back(SKeyMap(XK_Shift_R, KEY_RSHIFT));
	KeyMap.push_back(SKeyMap(XK_Control_L, KEY_LCONTROL));
	KeyMap.push_back(SKeyMap(XK_Control_R, KEY_RCONTROL));
	KeyMap.push_back(SKeyMap(XK_Caps_Lock, KEY_CAPITAL));
	KeyMap.push_back(SKeyMap(XK_Shift_Lock, KEY_CAPITAL));
	KeyMap.push_back(SKeyMap(XK_Meta_L, KEY_LWIN));
	KeyMap.push_back(SKeyMap(XK_Meta_R, KEY_RWIN));
	KeyMap.push_back(SKeyMap(XK_Alt_L, KEY_LMENU));
	KeyMap.push_back(SKeyMap(XK_Alt_R, KEY_RMENU));
	KeyMap.push_back(SKeyMap(XK_ISO_Level3_Shift, KEY_RMENU));
	KeyMap.push_back(SKeyMap(XK_Menu, KEY_MENU));
	KeyMap.push_back(SKeyMap(XK_space, KEY_SPACE));
	KeyMap.push_back(SKeyMap(XK_exclam, 0)); //?
	KeyMap.push_back(SKeyMap(XK_quotedbl, 0)); //?
	KeyMap.push_back(SKeyMap(XK_section, 0)); //?
	KeyMap.push_back(SKeyMap(XK_numbersign, 0)); //?
	KeyMap.push_back(SKeyMap(XK_dollar, 0)); //?
	KeyMap.push_back(SKeyMap(XK_percent, 0)); //?
	KeyMap.push_back(SKeyMap(XK_ampersand, 0)); //?
	KeyMap.push_back(SKeyMap(XK_apostrophe, 0)); //?
	KeyMap.push_back(SKeyMap(XK_parenleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_parenright, 0)); //?
	KeyMap.push_back(SKeyMap(XK_asterisk, 0)); //?
	KeyMap.push_back(SKeyMap(XK_plus, KEY_PLUS)); //?
	KeyMap.push_back(SKeyMap(XK_comma, KEY_COMMA)); //?
	KeyMap.push_back(SKeyMap(XK_minus, KEY_MINUS)); //?
	KeyMap.push_back(SKeyMap(XK_period, KEY_PERIOD)); //?
	KeyMap.push_back(SKeyMap(XK_slash, 0)); //?
	KeyMap.push_back(SKeyMap(XK_0, KEY_KEY_0));
	KeyMap.push_back(SKeyMap(XK_1, KEY_KEY_1));
	KeyMap.push_back(SKeyMap(XK_2, KEY_KEY_2));
	KeyMap.push_back(SKeyMap(XK_3, KEY_KEY_3));
	KeyMap.push_back(SKeyMap(XK_4, KEY_KEY_4));
	KeyMap.push_back(SKeyMap(XK_5, KEY_KEY_5));
	KeyMap.push_back(SKeyMap(XK_6, KEY_KEY_6));
	KeyMap.push_back(SKeyMap(XK_7, KEY_KEY_7));
	KeyMap.push_back(SKeyMap(XK_8, KEY_KEY_8));
	KeyMap.push_back(SKeyMap(XK_9, KEY_KEY_9));
	KeyMap.push_back(SKeyMap(XK_colon, 0)); //?
	KeyMap.push_back(SKeyMap(XK_semicolon, 0)); //?
	KeyMap.push_back(SKeyMap(XK_less, 0)); //?
	KeyMap.push_back(SKeyMap(XK_equal, 0)); //?
	KeyMap.push_back(SKeyMap(XK_greater, 0)); //?
	KeyMap.push_back(SKeyMap(XK_question, 0)); //?
	KeyMap.push_back(SKeyMap(XK_at, 0)); //?
	KeyMap.push_back(SKeyMap(XK_mu, 0)); //?
	KeyMap.push_back(SKeyMap(XK_EuroSign, 0)); //?
	KeyMap.push_back(SKeyMap(XK_A, KEY_KEY_A));
	KeyMap.push_back(SKeyMap(XK_B, KEY_KEY_B));
	KeyMap.push_back(SKeyMap(XK_C, KEY_KEY_C));
	KeyMap.push_back(SKeyMap(XK_D, KEY_KEY_D));
	KeyMap.push_back(SKeyMap(XK_E, KEY_KEY_E));
	KeyMap.push_back(SKeyMap(XK_F, KEY_KEY_F));
	KeyMap.push_back(SKeyMap(XK_G, KEY_KEY_G));
	KeyMap.push_back(SKeyMap(XK_H, KEY_KEY_H));
	KeyMap.push_back(SKeyMap(XK_I, KEY_KEY_I));
	KeyMap.push_back(SKeyMap(XK_J, KEY_KEY_J));
	KeyMap.push_back(SKeyMap(XK_K, KEY_KEY_K));
	KeyMap.push_back(SKeyMap(XK_L, KEY_KEY_L));
	KeyMap.push_back(SKeyMap(XK_M, KEY_KEY_M));
	KeyMap.push_back(SKeyMap(XK_N, KEY_KEY_N));
	KeyMap.push_back(SKeyMap(XK_O, KEY_KEY_O));
	KeyMap.push_back(SKeyMap(XK_P, KEY_KEY_P));
	KeyMap.push_back(SKeyMap(XK_Q, KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(XK_R, KEY_KEY_R));
	KeyMap.push_back(SKeyMap(XK_S, KEY_KEY_S));
	KeyMap.push_back(SKeyMap(XK_T, KEY_KEY_T));
	KeyMap.push_back(SKeyMap(XK_U, KEY_KEY_U));
	KeyMap.push_back(SKeyMap(XK_V, KEY_KEY_V));
	KeyMap.push_back(SKeyMap(XK_W, KEY_KEY_W));
	KeyMap.push_back(SKeyMap(XK_X, KEY_KEY_X));
	KeyMap.push_back(SKeyMap(XK_Y, KEY_KEY_Y));
	KeyMap.push_back(SKeyMap(XK_Z, KEY_KEY_Z));
	KeyMap.push_back(SKeyMap(XK_Adiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_Odiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_Udiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_bracketleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_backslash, 0)); //?
	KeyMap.push_back(SKeyMap(XK_bracketright, 0)); //?
	KeyMap.push_back(SKeyMap(XK_asciicircum, 0)); //?
	KeyMap.push_back(SKeyMap(XK_degree, 0)); //?
	KeyMap.push_back(SKeyMap(XK_underscore, 0)); //?
	KeyMap.push_back(SKeyMap(XK_grave, 0)); //?
	KeyMap.push_back(SKeyMap(XK_acute, 0)); //?
	KeyMap.push_back(SKeyMap(XK_quoteleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_a, KEY_KEY_A));
	KeyMap.push_back(SKeyMap(XK_b, KEY_KEY_B));
	KeyMap.push_back(SKeyMap(XK_c, KEY_KEY_C));
	KeyMap.push_back(SKeyMap(XK_d, KEY_KEY_D));
	KeyMap.push_back(SKeyMap(XK_e, KEY_KEY_E));
	KeyMap.push_back(SKeyMap(XK_f, KEY_KEY_F));
	KeyMap.push_back(SKeyMap(XK_g, KEY_KEY_G));
	KeyMap.push_back(SKeyMap(XK_h, KEY_KEY_H));
	KeyMap.push_back(SKeyMap(XK_i, KEY_KEY_I));
	KeyMap.push_back(SKeyMap(XK_j, KEY_KEY_J));
	KeyMap.push_back(SKeyMap(XK_k, KEY_KEY_K));
	KeyMap.push_back(SKeyMap(XK_l, KEY_KEY_L));
	KeyMap.push_back(SKeyMap(XK_m, KEY_KEY_M));
	KeyMap.push_back(SKeyMap(XK_n, KEY_KEY_N));
	KeyMap.push_back(SKeyMap(XK_o, KEY_KEY_O));
	KeyMap.push_back(SKeyMap(XK_p, KEY_KEY_P));
	KeyMap.push_back(SKeyMap(XK_q, KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(XK_r, KEY_KEY_R));
	KeyMap.push_back(SKeyMap(XK_s, KEY_KEY_S));
	KeyMap.push_back(SKeyMap(XK_t, KEY_KEY_T));
	KeyMap.push_back(SKeyMap(XK_u, KEY_KEY_U));
	KeyMap.push_back(SKeyMap(XK_v, KEY_KEY_V));
	KeyMap.push_back(SKeyMap(XK_w, KEY_KEY_W));
	KeyMap.push_back(SKeyMap(XK_x, KEY_KEY_X));
	KeyMap.push_back(SKeyMap(XK_y, KEY_KEY_Y));
	KeyMap.push_back(SKeyMap(XK_z, KEY_KEY_Z));
	KeyMap.push_back(SKeyMap(XK_ssharp, 0)); //?
	KeyMap.push_back(SKeyMap(XK_adiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_odiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_udiaeresis, 0)); //?

	KeyMap.sort();
#endif
}

//! create the driver
void CIrrDevice::createDriver()
{
	switch(CreationParams.DriverType)
	{
	case video::EDT_OGLES1:
	#ifdef _IRR_COMPILE_WITH_OGLES1_
		{
			video::SExposedVideoData data;
			VideoDriver = video::createOGLES1Driver(
				CreationParams, data, FileSystem);
		}
	#else
		os::Printer::log("No OpenGL-ES1 support compiled in.", ELL_ERROR);
	#endif
		break;

	case video::EDT_SOFTWARE:
	case video::EDT_BURNINGSVIDEO:
	case video::EDT_DIRECT3D8:
	case video::EDT_DIRECT3D9:
	case video::EDT_OPENGL:
		os::Printer::log("This driver is not available. Try OpenGL-ES1 renderer.",
			ELL_ERROR);
		break;

	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;

	default:
		os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
		break;
	}
}



//! runs the device. Returns false if device wants to be deleted
bool CIrrDevice::run()
{
	os::Timer::tick();
	yield();
	Closed = Close;
	return !Close;
}



//! Pause the current process for the minimum time allowed only to allow other processes to execute
void CIrrDevice::yield()
{

}



//! Pause execution and let other processes to run for a specified amount of time.
void CIrrDevice::sleep(u32 timeMs, bool pauseTimer=false)
{
	kdSleep ( (KDust) timeMs * 1000ULL );
}	

//! sets the caption of the window
void CIrrDevice::setWindowCaption(const wchar_t* text)
{
}



//! presents a surface in the client area
bool CIrrDevice::present(video::IImage* image, void * windowId, core::rect<s32>* src)
{
	return false;
}



//! notifies the device that it should close itself
void CIrrDevice::closeDevice()
{
	WindowActive = false;
	Close = true;

	kdExit ( 0 );
	//while (!Closed) yield();
}



//! returns if window is active. if not, nothing need to be drawn
bool CIrrDevice::isWindowActive() const
{
	return WindowActive;
}


bool CIrrDevice::isWindowFocused() const
{
	return isWindowActive();
}


bool CIrrDevice::isWindowMinimized() const
{
	return !isWindowActive();
}


//! Sets if the window should be resizeable in windowed mode.
void CIrrDevice::setResizable(bool resize)
{
}


//! Minimizes the window
void CIrrDevice::minimizeWindow()
{
}


//! Maximizes the window if possible.
void CIrrDevice::maximizeWindow()
{
}


//! Restore the window to normal size if possible.
void CIrrDevice::restoreWindow()
{
}


void CIrrDevice::setWindowActive(bool active)
{
	WindowActive = active;
}

KDvoid CIrrDevice::EventProc ( const KDEvent* event )
{
	SEvent  tEvent;

	switch ( event->type )
	{
		case KD_EVENT_NATIVE :
			
			EventNativeProc ( event );

			break;

		case KD_EVENT_TOUCH_BEGAN :
		case KD_EVENT_TOUCH_MOVED :
		case KD_EVENT_TOUCH_ENDED :
		case KD_EVENT_TOUCH_CANCELLED :

			switch ( event->type )
			{
				case KD_EVENT_TOUCH_BEGAN     : tEvent.MultiTouchInput.Event = irr::EMTIE_PRESSED_DOWN;	break;
				case KD_EVENT_TOUCH_MOVED     : tEvent.MultiTouchInput.Event = irr::EMTIE_MOVED;		break;
				case KD_EVENT_TOUCH_ENDED	  : 
				case KD_EVENT_TOUCH_CANCELLED : tEvent.MultiTouchInput.Event = irr::EMTIE_LEFT_UP;		break;
			}
			
			tEvent.EventType = irr::EET_MULTI_TOUCH_EVENT;
			tEvent.MultiTouchInput.clear ( );

			for ( KDint  i = 0; i < event->data.touch.count; i++ )
			{
				tEvent.MultiTouchInput.Touched [ i ] = 1;
				tEvent.MultiTouchInput.PrevX   [ i ] = event->data.touch.touches [ i ].prev_x;
				tEvent.MultiTouchInput.PrevY   [ i ] = event->data.touch.touches [ i ].prev_y;
				tEvent.MultiTouchInput.X       [ i ] = event->data.touch.touches [ i ].x;
				tEvent.MultiTouchInput.Y       [ i ] = event->data.touch.touches [ i ].y;

				postEventFromUser ( tEvent );
			}

#if !defined ( _IRR_WINDOWS_ ) && !defined ( _IRR_LINUX_PLATFORM_ )

			switch ( event->type )
			{
				case KD_EVENT_TOUCH_BEGAN     : tEvent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN; break;
				case KD_EVENT_TOUCH_MOVED     : tEvent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;		 break;
				case KD_EVENT_TOUCH_ENDED	  : 
				case KD_EVENT_TOUCH_CANCELLED : tEvent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;		 break;
			}
			
			tEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;
			tEvent.MouseInput.Shift = 0;
			tEvent.MouseInput.Control = 0;
			tEvent.MouseInput.ButtonStates = irr::EMBSM_LEFT;

			for ( KDint  i = 0; i < event->data.touch.count; i++ )
			{
				tEvent.MouseInput.X = event->data.touch.touches [ i ].x;
				tEvent.MouseInput.Y = CreationParams.WindowSize.Height - event->data.touch.touches [ i ].y;

				postEventFromUser ( tEvent );
			}
#endif
			break;


		case KD_EVENT_VISIBLE :

			WindowActive = event->data.value.i == 1 ? true : false;

			break;

	}
}

#if defined ( _IRR_WINDOWS_ )

HKL KEYBOARD_INPUT_HKL = 0;
unsigned int KEYBOARD_INPUT_CODEPAGE = 1252;
static unsigned int LocaleIdToCodepage(unsigned int lcid)
{
    switch ( lcid )
    {
        case 1098:  // Telugu
        case 1095:  // Gujarati
        case 1094:  // Punjabi
        case 1103:  // Sanskrit
        case 1111:  // Konkani
        case 1114:  // Syriac
        case 1099:  // Kannada
        case 1102:  // Marathi
        case 1125:  // Divehi
        case 1067:  // Armenian
        case 1081:  // Hindi
        case 1079:  // Georgian
        case 1097:  // Tamil
            return 0;
        case 1054:  // Thai
            return 874;
        case 1041:  // Japanese
            return 932;
        case 2052:  // Chinese (PRC)
        case 4100:  // Chinese (Singapore)
            return 936;
        case 1042:  // Korean
            return 949;
        case 5124:  // Chinese (Macau S.A.R.)
        case 3076:  // Chinese (Hong Kong S.A.R.)
        case 1028:  // Chinese (Taiwan)
            return 950;
        case 1048:  // Romanian
        case 1060:  // Slovenian
        case 1038:  // Hungarian
        case 1051:  // Slovak
        case 1045:  // Polish
        case 1052:  // Albanian
        case 2074:  // Serbian (Latin)
        case 1050:  // Croatian
        case 1029:  // Czech
            return 1250;
        case 1104:  // Mongolian (Cyrillic)
        case 1071:  // FYRO Macedonian
        case 2115:  // Uzbek (Cyrillic)
        case 1058:  // Ukrainian
        case 2092:  // Azeri (Cyrillic)
        case 1092:  // Tatar
        case 1087:  // Kazakh
        case 1059:  // Belarusian
        case 1088:  // Kyrgyz (Cyrillic)
        case 1026:  // Bulgarian
        case 3098:  // Serbian (Cyrillic)
        case 1049:  // Russian
            return 1251;
        case 8201:  // English (Jamaica)
        case 3084:  // French (Canada)
        case 1036:  // French (France)
        case 5132:  // French (Luxembourg)
        case 5129:  // English (New Zealand)
        case 6153:  // English (Ireland)
        case 1043:  // Dutch (Netherlands)
        case 9225:  // English (Caribbean)
        case 4108:  // French (Switzerland)
        case 4105:  // English (Canada)
        case 1110:  // Galician
        case 10249:  // English (Belize)
        case 3079:  // German (Austria)
        case 6156:  // French (Monaco)
        case 12297:  // English (Zimbabwe)
        case 1069:  // Basque
        case 2067:  // Dutch (Belgium)
        case 2060:  // French (Belgium)
        case 1035:  // Finnish
        case 1080:  // Faroese
        case 1031:  // German (Germany)
        case 3081:  // English (Australia)
        case 1033:  // English (United States)
        case 2057:  // English (United Kingdom)
        case 1027:  // Catalan
        case 11273:  // English (Trinidad)
        case 7177:  // English (South Africa)
        case 1030:  // Danish
        case 13321:  // English (Philippines)
        case 15370:  // Spanish (Paraguay)
        case 9226:  // Spanish (Colombia)
        case 5130:  // Spanish (Costa Rica)
        case 7178:  // Spanish (Dominican Republic)
        case 12298:  // Spanish (Ecuador)
        case 17418:  // Spanish (El Salvador)
        case 4106:  // Spanish (Guatemala)
        case 18442:  // Spanish (Honduras)
        case 3082:  // Spanish (International Sort)
        case 13322:  // Spanish (Chile)
        case 19466:  // Spanish (Nicaragua)
        case 2058:  // Spanish (Mexico)
        case 10250:  // Spanish (Peru)
        case 20490:  // Spanish (Puerto Rico)
        case 1034:  // Spanish (Traditional Sort)
        case 14346:  // Spanish (Uruguay)
        case 8202:  // Spanish (Venezuela)
        case 1089:  // Swahili
        case 1053:  // Swedish
        case 2077:  // Swedish (Finland)
        case 5127:  // German (Liechtenstein)
        case 1078:  // Afrikaans
        case 6154:  // Spanish (Panama)
        case 4103:  // German (Luxembourg)
        case 16394:  // Spanish (Bolivia)
        case 2055:  // German (Switzerland)
        case 1039:  // Icelandic
        case 1057:  // Indonesian
        case 1040:  // Italian (Italy)
        case 2064:  // Italian (Switzerland)
        case 2068:  // Norwegian (Nynorsk)
        case 11274:  // Spanish (Argentina)
        case 1046:  // Portuguese (Brazil)
        case 1044:  // Norwegian (Bokmal)
        case 1086:  // Malay (Malaysia)
        case 2110:  // Malay (Brunei Darussalam)
        case 2070:  // Portuguese (Portugal)
            return 1252;
        case 1032:  // Greek
            return 1253;
        case 1091:  // Uzbek (Latin)
        case 1068:  // Azeri (Latin)
        case 1055:  // Turkish
            return 1254;
        case 1037:  // Hebrew
            return 1255;
        case 5121:  // Arabic (Algeria)
        case 15361:  // Arabic (Bahrain)
        case 9217:  // Arabic (Yemen)
        case 3073:  // Arabic (Egypt)
        case 2049:  // Arabic (Iraq)
        case 11265:  // Arabic (Jordan)
        case 13313:  // Arabic (Kuwait)
        case 12289:  // Arabic (Lebanon)
        case 4097:  // Arabic (Libya)
        case 6145:  // Arabic (Morocco)
        case 8193:  // Arabic (Oman)
        case 16385:  // Arabic (Qatar)
        case 1025:  // Arabic (Saudi Arabia)
        case 10241:  // Arabic (Syria)
        case 14337:  // Arabic (U.A.E.)
        case 1065:  // Farsi
        case 1056:  // Urdu
        case 7169:  // Arabic (Tunisia)
            return 1256;
        case 1061:  // Estonian
        case 1062:  // Latvian
        case 1063:  // Lithuanian
            return 1257;
        case 1066:  // Vietnamese
            return 1258;
    }
    return 65001;   // utf-8
}

#elif defined ( _IRR_LINUX_PLATFORM_ )



#endif

KDvoid CIrrDevice::EventNativeProc ( const KDEvent* _event )
{
#if defined ( _IRR_WINDOWS_ )

    irr::SEvent  tEvent;
    
	const KDEventNativeWin32*  pEvent = (const KDEventNativeWin32*) _event->data.native.p;

	#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL    0x020A
	#endif
	#ifndef WHEEL_DELTA
	#define WHEEL_DELTA     120
	#endif

	static irr::s32  nClickCount = 0;
	if ( GetCapture ( ) != pEvent->hwnd && nClickCount > 0 )
	{
		nClickCount = 0;
	}

	struct TMsgMap
	{
		irr::s32	group;
		UINT		winMsg;
		irr::s32	irrMsg;
	};

	static TMsgMap  tMouseMap [ ] =
	{
		{  0, WM_LBUTTONDOWN, irr::EMIE_LMOUSE_PRESSED_DOWN	},
		{  1, WM_LBUTTONUP	, irr::EMIE_LMOUSE_LEFT_UP		},
		{  0, WM_RBUTTONDOWN, irr::EMIE_RMOUSE_PRESSED_DOWN	},
		{  1, WM_RBUTTONUP	, irr::EMIE_RMOUSE_LEFT_UP		},
		{  0, WM_MBUTTONDOWN, irr::EMIE_MMOUSE_PRESSED_DOWN	},
		{  1, WM_MBUTTONUP	, irr::EMIE_MMOUSE_LEFT_UP		},
		{  2, WM_MOUSEMOVE	, irr::EMIE_MOUSE_MOVED			},
		{  3, WM_MOUSEWHEEL	, irr::EMIE_MOUSE_WHEEL			},
		{ -1, 0, 0 }
	};

	// handle grouped events
	TMsgMap*  pMap = tMouseMap;
	while ( pMap->group >=0 && pMap->winMsg != pEvent->msg )
	{
		pMap += 1;
	}

	if ( pMap->group >= 0 )
	{
		if ( pMap->group == 0 )			// down
		{
			nClickCount++;
			SetCapture ( pEvent->hwnd );
		}
		else if ( pMap->group == 1 )	// up
		{
			nClickCount--;
			if ( nClickCount < 1 )
			{
				nClickCount = 0;
				ReleaseCapture ( );
			}
		}

		tEvent.EventType		  = irr::EET_MOUSE_INPUT_EVENT;
		tEvent.MouseInput.Event   = (irr::EMOUSE_INPUT_EVENT) pMap->irrMsg;
		tEvent.MouseInput.X		  = (short) LOWORD ( pEvent->lparam );
		tEvent.MouseInput.Y		  = (short) HIWORD ( pEvent->lparam );
		tEvent.MouseInput.Shift   = ( ( LOWORD ( pEvent->wparam ) & MK_SHIFT   ) != 0 );
		tEvent.MouseInput.Control = ( ( LOWORD ( pEvent->wparam ) & MK_CONTROL ) != 0 );
		// left and right mouse buttons
		tEvent.MouseInput.ButtonStates = pEvent->wparam & ( MK_LBUTTON | MK_RBUTTON);
		// middle and extra buttons
		if ( pEvent->wparam & MK_MBUTTON )
		{
			tEvent.MouseInput.ButtonStates |= irr::EMBSM_MIDDLE;
		}
#if(_WIN32_WINNT >= 0x0500)
		if ( pEvent->wparam & MK_XBUTTON1 )
		{
			tEvent.MouseInput.ButtonStates |= irr::EMBSM_EXTRA1;
		}
		if ( pEvent->wparam & MK_XBUTTON2 )
		{
			tEvent.MouseInput.ButtonStates |= irr::EMBSM_EXTRA2;
		}
#endif
		tEvent.MouseInput.Wheel = 0.f;

		// wheel
		if ( pMap->group == 3 )
		{
			POINT  tPos; // fixed by jox
			tPos.x = 0; 
			tPos.y = 0;

			ClientToScreen ( pEvent->hwnd, &tPos );
			tEvent.MouseInput.X -= tPos.x;
			tEvent.MouseInput.Y -= tPos.y;
			tEvent.MouseInput.Wheel = ((irr::f32) ( (short) HIWORD (pEvent->wparam) ) ) / (irr::f32) WHEEL_DELTA;
		}

		postEventFromUser ( tEvent );

		if ( tEvent.MouseInput.Event >= irr::EMIE_LMOUSE_PRESSED_DOWN && tEvent.MouseInput.Event <= irr::EMIE_MMOUSE_PRESSED_DOWN )
		{
			irr::u32  uClicks = checkSuccessiveClicks ( tEvent.MouseInput.X, tEvent.MouseInput.Y, tEvent.MouseInput.Event );
			if ( uClicks == 2 )
			{
				tEvent.MouseInput.Event = (irr::EMOUSE_INPUT_EVENT) ( irr::EMIE_LMOUSE_DOUBLE_CLICK + tEvent.MouseInput.Event - irr::EMIE_LMOUSE_PRESSED_DOWN );
				postEventFromUser ( tEvent );
			}
			else if ( uClicks == 3 )
			{
				tEvent.MouseInput.Event = (irr::EMOUSE_INPUT_EVENT) ( irr::EMIE_LMOUSE_TRIPLE_CLICK + tEvent.MouseInput.Event - irr::EMIE_LMOUSE_PRESSED_DOWN );
				postEventFromUser ( tEvent );
			}
		}

		return;
	}

	switch ( pEvent->msg )
	{
		case WM_SYSKEYDOWN :
		case WM_SYSKEYUP :
		case WM_KEYDOWN :
		case WM_KEYUP :
			{
				BYTE   allKeys[256];

				tEvent.EventType = irr::EET_KEY_INPUT_EVENT;
				tEvent.KeyInput.Key = (irr::EKEY_CODE) pEvent->wparam;
				tEvent.KeyInput.PressedDown = ( pEvent->msg == WM_KEYDOWN || pEvent->msg == WM_SYSKEYDOWN );

				const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.
				if ( tEvent.KeyInput.Key == irr::KEY_SHIFT )
				{
					// this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
					tEvent.KeyInput.Key = (irr::EKEY_CODE) MapVirtualKey ( ( ( pEvent->lparam >> 16 ) & 255 ), MY_MAPVK_VSC_TO_VK_EX );
				}

				if ( tEvent.KeyInput.Key == irr::KEY_CONTROL )
				{
					tEvent.KeyInput.Key = (irr::EKEY_CODE) MapVirtualKey( ( ( pEvent->lparam >> 16 ) & 255 ), MY_MAPVK_VSC_TO_VK_EX );
					// some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
					if ( pEvent->lparam & 0x1000000 )
					{
						tEvent.KeyInput.Key = irr::KEY_RCONTROL;
					}
				}

				if ( tEvent.KeyInput.Key == irr::KEY_MENU )
				{
					tEvent.KeyInput.Key = (irr::EKEY_CODE)MapVirtualKey ( ( ( pEvent->lparam >> 16 ) & 255 ), MY_MAPVK_VSC_TO_VK_EX );
					if ( pEvent->lparam & 0x1000000 )
					{
						tEvent.KeyInput.Key = irr::KEY_RMENU;
					}
				}
#ifndef _WIN32_WCE
				GetKeyboardState ( allKeys );
#endif
				tEvent.KeyInput.Shift = ( ( allKeys[VK_SHIFT] & 0x80 ) != 0 );
				tEvent.KeyInput.Control = ( ( allKeys[VK_CONTROL] & 0x80 )!= 0 );

				// Handle unicode and deadkeys in a way that works since Windows 95 and nt4.0
				// Using ToUnicode instead would be shorter, but would to my knowledge not run on 95 and 98.
				WORD  wKeyChars[2];
				UINT  uScanCode = HIWORD ( pEvent->lparam );
				
#ifdef _WIN32_WCE
				int   nConversionResult = 1;
#else
				int   nConversionResult = ToAsciiEx ( pEvent->wparam, uScanCode, allKeys, wKeyChars, 0, KEYBOARD_INPUT_HKL );
#endif
				if ( nConversionResult == 1 )
				{
					WORD unicodeChar;
					MultiByteToWideChar
					(
						KEYBOARD_INPUT_CODEPAGE,
						MB_PRECOMPOSED,   // default
						(LPCSTR) wKeyChars,
						sizeof ( wKeyChars ),
						(WCHAR*)&unicodeChar,
						1 
					);
					tEvent.KeyInput.Char = unicodeChar;
				}
				else
				{
					tEvent.KeyInput.Char = 0;
				}

				// allow composing characters like '@' with Alt Gr on non-US keyboards
				if ( ( allKeys[VK_MENU] & 0x80 ) != 0 )
				{
					tEvent.KeyInput.Control = 0;
				}

				postEventFromUser ( tEvent );
/*
				if ( message == WM_SYSKEYDOWN || message == WM_SYSKEYUP )
				{
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				else
					return 0;
*/
			}

	case WM_USER :
		
		tEvent.EventType = irr::EET_USER_EVENT;
		tEvent.UserEvent.UserData1 = (irr::s32) pEvent->wparam;
		tEvent.UserEvent.UserData2 = (irr::s32) pEvent->lparam;

		postEventFromUser ( tEvent );

		break;

		case WM_SETCURSOR :
			// because Windows forgot about that in the meantime
			getCursorControl ( )->setActiveIcon ( getCursorControl ( )->getActiveIcon ( ) );
			getCursorControl ( )->setVisible ( getCursorControl ( )->isVisible ( ) );
			
			break;

		case WM_INPUTLANGCHANGE :

			// get the new codepage used for keyboard input
			KEYBOARD_INPUT_HKL = GetKeyboardLayout ( 0 );
			KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage ( LOWORD ( KEYBOARD_INPUT_HKL ) );
			
			break;
	}

#elif defined ( _IRR_LINUX_PLATFORM_ )

	const KDEventNativeLinux*  pNative = (const KDEventNativeLinux*) _event->data.native.p;
	XEvent*  pEvent = pNative->event;
	irr::SEvent  tEvent;

	switch ( pEvent->type )
	{
		case MotionNotify :
			tEvent.EventType          = irr::EET_MOUSE_INPUT_EVENT;
			tEvent.MouseInput.Event   = irr::EMIE_MOUSE_MOVED;
			tEvent.MouseInput.X       = pEvent->xbutton.x;
			tEvent.MouseInput.Y       = pEvent->xbutton.y;
			tEvent.MouseInput.Control = ( pEvent->xkey.state & ControlMask ) != 0;
			tEvent.MouseInput.Shift   = ( pEvent->xkey.state & ShiftMask   ) != 0;

			// mouse button states
			tEvent.MouseInput.ButtonStates  = ( pEvent->xbutton.state & Button1Mask ) ? irr::EMBSM_LEFT   : 0;
			tEvent.MouseInput.ButtonStates |= ( pEvent->xbutton.state & Button3Mask ) ? irr::EMBSM_RIGHT  : 0;
			tEvent.MouseInput.ButtonStates |= ( pEvent->xbutton.state & Button2Mask ) ? irr::EMBSM_MIDDLE : 0;

			postEventFromUser ( tEvent );
			break;

		case ButtonPress   :
		case ButtonRelease :

			tEvent.EventType          = irr::EET_MOUSE_INPUT_EVENT;
			tEvent.MouseInput.X       = pEvent->xbutton.x;
			tEvent.MouseInput.Y       = pEvent->xbutton.y;
			tEvent.MouseInput.Control = ( pEvent->xkey.state & ControlMask ) != 0;
			tEvent.MouseInput.Shift   = ( pEvent->xkey.state & ShiftMask   ) != 0;

			// mouse button states
			// This sets the state which the buttons had _prior_ to the event.
			// So unlike on Windows the button which just got changed has still the old state here.
			// We handle that below by flipping the corresponding bit later.
			tEvent.MouseInput.ButtonStates  = ( pEvent->xbutton.state & Button1Mask ) ? irr::EMBSM_LEFT   : 0;
			tEvent.MouseInput.ButtonStates |= ( pEvent->xbutton.state & Button3Mask ) ? irr::EMBSM_RIGHT  : 0;
			tEvent.MouseInput.ButtonStates |= ( pEvent->xbutton.state & Button2Mask ) ? irr::EMBSM_MIDDLE : 0;

			tEvent.MouseInput.Event = irr::EMIE_COUNT;

			switch ( pEvent->xbutton.button )
			{
				case  Button1 :
					tEvent.MouseInput.Event =
						( pEvent->type == ButtonPress ) ? irr::EMIE_LMOUSE_PRESSED_DOWN : irr::EMIE_LMOUSE_LEFT_UP;
					tEvent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;
					break;

				case  Button3 :
					tEvent.MouseInput.Event =
						( pEvent->type == ButtonPress ) ? irr::EMIE_RMOUSE_PRESSED_DOWN : irr::EMIE_RMOUSE_LEFT_UP;
					tEvent.MouseInput.ButtonStates ^= irr::EMBSM_RIGHT;
					break;

				case  Button2 :
					tEvent.MouseInput.Event =
						( pEvent->type == ButtonPress ) ? irr::EMIE_MMOUSE_PRESSED_DOWN : irr::EMIE_MMOUSE_LEFT_UP;
					tEvent.MouseInput.ButtonStates ^= irr::EMBSM_MIDDLE;
					break;

				case  Button4 :
					if ( pEvent->type == ButtonPress )
					{
						tEvent.MouseInput.Event = EMIE_MOUSE_WHEEL;
						tEvent.MouseInput.Wheel = 1.0f;
					}
					break;

				case  Button5 :
					if ( pEvent->type == ButtonPress )
					{
						tEvent.MouseInput.Event = EMIE_MOUSE_WHEEL;
						tEvent.MouseInput.Wheel = -1.0f;
					}
					break;
			}

			if ( tEvent.MouseInput.Event != irr::EMIE_COUNT )
			{
				postEventFromUser ( tEvent );

				if ( tEvent.MouseInput.Event >= EMIE_LMOUSE_PRESSED_DOWN && tEvent.MouseInput.Event <= EMIE_MMOUSE_PRESSED_DOWN )
				{
					u32  clicks = checkSuccessiveClicks ( tEvent.MouseInput.X, tEvent.MouseInput.Y, tEvent.MouseInput.Event );
					if ( clicks == 2 )
					{
						tEvent.MouseInput.Event = (EMOUSE_INPUT_EVENT) ( EMIE_LMOUSE_DOUBLE_CLICK + tEvent.MouseInput.Event - EMIE_LMOUSE_PRESSED_DOWN );
						postEventFromUser ( tEvent );
					}
					else if ( clicks == 3 )
					{
						tEvent.MouseInput.Event = (EMOUSE_INPUT_EVENT) ( EMIE_LMOUSE_TRIPLE_CLICK + tEvent.MouseInput.Event - EMIE_LMOUSE_PRESSED_DOWN );
						postEventFromUser ( tEvent );
					}
				}
			}
			break;

		case KeyRelease :
			if ( XPending ( pNative->display ) > 0 )
			{
				// check for Autorepeat manually
				// We'll do the same as Windows does: Only send KeyPressed
				// So every KeyRelease is a real release
				XEvent   tNextEvent;
				XPeekEvent ( pEvent->xkey.display, &tNextEvent );
				if (( tNextEvent.type == KeyPress) &&
					( tNextEvent.xkey.keycode == pEvent->xkey.keycode ) &&
					( tNextEvent.xkey.time - pEvent->xkey.time ) < 2 )	// usually same time, but on some systems a difference of 1 is possible
				{
					// Ignore the key release event 
					break;
				}
			}
			// fall-through in case the release should be handled
			case KeyPress :
				{
					SKeyMap  tKeyMap;
					char     aBuf[8]={0};
					XLookupString ( &pEvent->xkey, aBuf, sizeof ( aBuf ), &tKeyMap.X11Key, 0 );

					const s32  nIdx = KeyMap.binary_search ( tKeyMap );

					if ( nIdx != -1 )
					{
						tEvent.KeyInput.Key = (EKEY_CODE) KeyMap [ nIdx ].Win32Key;
					}
					else
					{
						// Usually you will check keysymdef.h and add the corresponding key to createKeyMap.
						tEvent.KeyInput.Key = (EKEY_CODE) 0;
						os::Printer::log ( "Could not find win32 key for x11 key.", core::stringc ( (int) tKeyMap.X11Key ).c_str ( ), ELL_WARNING );
					}
					tEvent.EventType			= irr::EET_KEY_INPUT_EVENT;
					tEvent.KeyInput.PressedDown = ( pEvent->type == KeyPress );
//					mbtowc(&irrevent.KeyInput.Char, buf, sizeof(buf));
					tEvent.KeyInput.Char		= ( reinterpret_cast<wchar_t*> ( aBuf ) ) [ 0 ];
					tEvent.KeyInput.Control		= ( pEvent->xkey.state & ControlMask ) != 0;
					tEvent.KeyInput.Shift		= ( pEvent->xkey.state & ShiftMask   ) != 0;
					postEventFromUser ( tEvent );
				}

				break;
	}

#else

#endif
}

} // end namespace



