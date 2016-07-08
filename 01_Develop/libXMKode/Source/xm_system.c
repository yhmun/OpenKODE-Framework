/* --------------------------------------------------------------------------
 *
 *      File            xm_system.c
 *      Description     System IO function.
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

#include "kd_library.h"

struct XMKeyName
{
	KDint			kd_code;
	const KDchar	key_name[64];
};

KDPoint     g_contentScale = { 1.f, 1.f };
KDRect      g_viewportRect = { 0, 0, 0, 0 };

// xmGetKeyName : Get key input name.
KD_API const KDchar* KD_APIENTRY xmGetKeyName ( KDint kd_code )
{
	static struct XMKeyName    names[ ] =	
	{
		{ KD_INPUT_MOUSE_BUTTON_1			, "KD_INPUT_MOUSE_BUTTON_LEFT"		},
		{ KD_INPUT_MOUSE_BUTTON_2			, "KD_INPUT_MOUSE_BUTTON_RIGHT"		},
		{ KD_INPUT_MOUSE_BUTTON_3			, "KD_INPUT_MOUSE_BUTTON_MIDDLE"	},
		{ KD_INPUT_MOUSE_BUTTON_4			, "KD_INPUT_MOUSE_BUTTON_4"			},
		{ KD_INPUT_MOUSE_BUTTON_5			, "KD_INPUT_MOUSE_BUTTON_5"			},
		{ KD_INPUT_MOUSE_BUTTON_6			, "KD_INPUT_MOUSE_BUTTON_6"			},
		{ KD_INPUT_MOUSE_BUTTON_7			, "KD_INPUT_MOUSE_BUTTON_7"			},
		{ KD_INPUT_MOUSE_BUTTON_8			, "KD_INPUT_MOUSE_BUTTON_8"			},

		{ KD_INPUT_PHONEKEYPAD_0			, "KD_INPUT_PHONEKEYPAD_0" 			},    
		{ KD_INPUT_PHONEKEYPAD_1			, "KD_INPUT_PHONEKEYPAD_1" 			},   
		{ KD_INPUT_PHONEKEYPAD_2			, "KD_INPUT_PHONEKEYPAD_2" 			},   
		{ KD_INPUT_PHONEKEYPAD_3			, "KD_INPUT_PHONEKEYPAD_3" 			},    
		{ KD_INPUT_PHONEKEYPAD_4			, "KD_INPUT_PHONEKEYPAD_4" 			},   
		{ KD_INPUT_PHONEKEYPAD_5			, "KD_INPUT_PHONEKEYPAD_5"			},      
		{ KD_INPUT_PHONEKEYPAD_6			, "KD_INPUT_PHONEKEYPAD_6"			},         
		{ KD_INPUT_PHONEKEYPAD_7			, "KD_INPUT_PHONEKEYPAD_7"			},      
		{ KD_INPUT_PHONEKEYPAD_8			, "KD_INPUT_PHONEKEYPAD_8"			},    
		{ KD_INPUT_PHONEKEYPAD_9			, "KD_INPUT_PHONEKEYPAD_9"			},    
		{ KD_INPUT_PHONEKEYPAD_STAR			, "KD_INPUT_PHONEKEYPAD_STAR"		}, 
		{ KD_INPUT_PHONEKEYPAD_HASH			, "KD_INPUT_PHONEKEYPAD_HASH"		}, 
		{ KD_INPUT_PHONEKEYPAD_LEFTSOFT		, "KD_INPUT_PHONEKEYPAD_LEFTSOFT"	}, 
		{ KD_INPUT_PHONEKEYPAD_RIGHTSOFT	, "KD_INPUT_PHONEKEYPAD_RIGHTSOFT"	},
		{ KD_INPUT_GAMEKEYS_UP				, "KD_INPUT_GAMEKEYS_UP"   			},
		{ KD_INPUT_GAMEKEYS_LEFT			, "KD_INPUT_GAMEKEYS_LEFT" 			},
		{ KD_INPUT_GAMEKEYS_RIGHT			, "KD_INPUT_GAMEKEYS_RIGHT"			},
		{ KD_INPUT_GAMEKEYS_DOWN			, "KD_INPUT_GAMEKEYS_DOWN"			},
		{ KD_INPUT_GAMEKEYS_FIRE			, "KD_INPUT_GAMEKEYS_FIRE"			}, 
		{ KD_INPUT_GAMEKEYS_A				, "KD_INPUT_GAMEKEYS_A"				},    
		{ KD_INPUT_GAMEKEYS_B				, "KD_INPUT_GAMEKEYS_B"				},    
		{ KD_INPUT_GAMEKEYS_C				, "KD_INPUT_GAMEKEYS_C"				},    
		{ KD_INPUT_GAMEKEYS_D				, "KD_INPUT_GAMEKEYS_D"				},
		{ KD_INPUT_GAMEKEYSNC_UP			, "KD_INPUT_GAMEKEYSNC_UP"			},  
		{ KD_INPUT_GAMEKEYSNC_LEFT			, "KD_INPUT_GAMEKEYSNC_LEFT"		}, 
		{ KD_INPUT_GAMEKEYSNC_RIGHT			, "KD_INPUT_GAMEKEYSNC_RIGHT"		},
		{ KD_INPUT_GAMEKEYSNC_DOWN			, "KD_INPUT_GAMEKEYSNC_DOWN"		}, 
		{ KD_INPUT_GAMEKEYSNC_FIRE			, "KD_INPUT_GAMEKEYSNC_FIRE"		},
		{ KD_INPUT_GAMEKEYSNC_A				, "KD_INPUT_GAMEKEYSNC_A"			},    
		{ KD_INPUT_GAMEKEYSNC_B				, "KD_INPUT_GAMEKEYSNC_B"			},    
		{ KD_INPUT_GAMEKEYSNC_C				, "KD_INPUT_GAMEKEYSNC_C"			},    
		{ KD_INPUT_GAMEKEYSNC_D				, "KD_INPUT_GAMEKEYSNC_D"			},   
		{ KD_INPUT_XMSOFT_ESCAPE            , "KD_INPUT_XMSOFT_ESCAPE"          },
		{ KD_INPUT_XMSOFT_PREV              , "KD_INPUT_XMSOFT_PREV"            },
		{ KD_INPUT_XMSOFT_MENU              , "KD_INPUT_XMSOFT_MENU"            },	
		{ KD_INPUT_XMSOFT_COMMA             , "KD_INPUT_XMSOFT_COMMA"           },
		{ KD_INPUT_XMSOFT_PERIOD            , "KD_INPUT_XMSOFT_PERIOD"          },
		{ KD_INPUT_XMSOFT_LCONTROL          , "KD_INPUT_XMSOFT_LCONTROL"        },
		{ -1								, ""								},							 
	};

	KDint   idx;

	for ( idx = 0; names[ idx ].kd_code != -1; idx++ )
	{
		if ( names[ idx ].kd_code == kd_code )
		{
			return names[ idx ].key_name;
		}
	}

	return (const KDchar *) "unknown";
}

// kdGetpid : get process identification
// XMSoft's revision : Added API.
KD_API KDpid KD_APIENTRY kdGetpid ( KDvoid )
{
	KDpid	pid = 0;
#if defined ( _WIN32 )
	pid = GetCurrentProcessId ( );
#else
	pid = getpid ( );
#endif

	return pid;
}

// kdFork : The kdFork() system call will spawn a new child process which is an identical process to the parent except that has a new system process ID
// XMSoft's revision : Added API.
KD_API KDpid KD_APIENTRY kdFork ( KDvoid )
{
	KDpid	pid = 0;
#if defined ( _WIN32 )
	
#else
	pid = fork ( );
#endif

	return pid;
}

// kdSigemptyset : initializes the signal set specified by set to an empty set.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSigemptyset ( KDsigset* set )
{
#if defined ( __GNUC__ )
	return sigemptyset ( (sigset_t*) set );
#else
	*set = 0;
	return 0;
#endif
}

// kdSigaction : system call is used to change the action taken by a process on receipt of a specific signal.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSigaction ( KDint signum, const struct KDSigaction* act, struct KDSigaction* oldact )
{
#if defined ( __GNUC__ )
	return sigaction ( signum, (struct sigaction*) act, (struct sigaction*) oldact );
#elif defined ( WIN32 ) && !defined ( _WIN32_WCE )
	signal ( signum, ( void (*) (int) ) act->sa_handler_ );
	return 0;
#else
	return -1;
#endif	
}

// kdSignal : ANSI C signal handling
// XMSoft's revision : Added API.
KD_API KDSighandler KD_APIENTRY kdSignal ( KDint signum, KDSighandler handler )
{
#if defined ( __GNUC__ ) || ( defined ( WIN32 ) && !defined ( _WIN32_WCE ) )
	switch ( signum )
	{
		case KD_SIGABRT	: signum = SIGABRT;	 break;								
		case KD_SIGFPE  : signum = SIGFPE;   break;							
		case KD_SIGILL  : signum = SIGILL;	 break;								
		case KD_SIGINT  : signum = SIGINT;   break;					
		case KD_SIGSEGV : signum = SIGSEGV;  break;											
		case KD_SIGTERM : signum = SIGTERM;  break;
	#ifdef SIGCHLD
        case KD_SIGCHLD : signum = SIGCHLD;  break;
	#endif
	#ifdef SIGPIPE	
        case KD_SIGPIPE : signum = SIGPIPE;  break;
	#endif
	#ifdef SIGKILL	
        case KD_SIGKILL : signum = SIGKILL;  break;
	#endif
	}

	if      ( handler == KD_SIG_DFL )	handler = SIG_DFL;
	else if ( handler == KD_SIG_IGN )	handler = SIG_IGN;
#ifdef SIG_GET
	else if ( handler == KD_SIG_GET )	handler = SIG_GET;
#endif
#ifdef SIG_SGE
	else if ( handler == KD_SIG_SGE )	handler = SIG_SGE;
#endif
#ifdef SIG_ACK
	else if ( handler == KD_SIG_ACK )	handler = SIG_ACK;
#endif

	return (KDSighandler) signal ( signum, handler );
#else
	return KD_NULL;
#endif	
}

// kdSystem : Execute system command
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSystem ( const KDchar* string )
{
#if defined ( __GNUC__ ) || ( defined ( WIN32 ) && !defined ( _WIN32_WCE ) )
	return system ( string );
#else 
	return -1;
#endif
}

// xmEventAttach, xmEventDetach : Attach System Event Callback. ( ACCELEROMETER, MAGNETOMETER )
KD_API KDvoid xmEventAttach ( KDenum mode )
{
    xmPlatformEventAttach ( mode );
}

KD_API KDvoid xmEventDetach ( KDenum mode )
{
    xmPlatformEventDetach ( mode );
}

// xmSetIMEKeyboardState 
KD_API KDvoid KD_APIENTRY xmSetIMEKeyboardState ( KDbool show )
{
#ifdef  XM_HAS_PLATFORM_IME_KEYBOARD
	xmPlatformSetIMEKeyboardState ( show );
#endif
}

// xmVibrate : Vibrate Device. ( Android and iOS Supported, iOS can not control time. )
KD_API KDvoid KD_APIENTRY xmVibrate ( KDuint msec )
{
#ifdef XM_HAS_PLATFORM_VIBRATE
    xmPlatformVibrate ( msec );
#endif
}

// xmSetBrightness : Set Device's Brightness.
KD_API KDvoid KD_APIENTRY xmSetBrightness ( KDfloat value )
{
#ifdef XM_HAS_PLATFORM_BRIGHTNESS
	xmPlatformSetBrightness ( value );
#endif
}

// xmSetFullScreen : Hide Status Bar.
KD_API KDvoid KD_APIENTRY xmSetFullScreen ( KDbool enable )
{
#ifdef XM_HAS_PLATFORM_FULLSCREEN
	xmPlatformSetFullScreen ( enable );
#endif
}

// xmOpenURL : Open Browser with url page. ( Currently iOS Supported )
KD_API KDvoid KD_APIENTRY xmOpenURL ( const KDchar* url )
{
#ifdef XM_HAS_PLATFORM_OPEN_URL
	xmPlatformOpenURL ( url );
#endif
}

// For Cocos2D Engine
KD_API KDvoid   KD_APIENTRY xmSetContentScaleFactor ( KDfloat scaleX, KDfloat scaleY )
{
    g_contentScale.x = scaleX;
    g_contentScale.y = scaleY;
}

KD_API KDvoid KD_APIENTRY xmSetViewportRect ( KDfloat x, KDfloat y, KDfloat w, KDfloat h )
{
    g_viewportRect.x = x;
    g_viewportRect.y = y;
    g_viewportRect.w = w;
    g_viewportRect.h = h;
}
