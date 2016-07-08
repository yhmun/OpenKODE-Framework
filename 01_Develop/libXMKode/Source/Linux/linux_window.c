/* --------------------------------------------------------------------------
 *
 *      File            linux_window.c
 *      Description     Window fucntions.
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

#include "../kd_library.h"
#include "platform.h"

static KDint        l_wnd_x  = 0;
static KDint        l_wnd_y  = 0;
static KDuint       l_wnd_cx = 0;
static KDuint       l_wnd_cy = 0;

static Display*     l_display = 0;
static Window       l_root    = 0;
static Window       l_wnd     = 0;

static KDTouch      l_touch;
static KDbool       l_touch_pressed  = 0;

static Atom         XM_DELETE_WINDOW = 0;

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code )
{
	static struct XMKeyCode  codes[ ] =
	{
		// KD_IOGROUP_GAMEKEYS
		{ KD_INPUT_GAMEKEYS_LEFT            , 0x71 },	
		{ KD_INPUT_GAMEKEYS_UP              , 0x6F },			
		{ KD_INPUT_GAMEKEYS_RIGHT           , 0x72 },	
		{ KD_INPUT_GAMEKEYS_DOWN            , 0x74 },	
		{ KD_INPUT_GAMEKEYS_FIRE            , 0x41 }, // space
		{ KD_INPUT_GAMEKEYS_A               , 0x26 }, // a
		{ KD_INPUT_GAMEKEYS_B               , 0x27 }, // s
		{ KD_INPUT_GAMEKEYS_C               , 0x28 }, // d
		{ KD_INPUT_GAMEKEYS_D               , 0x19 }, // w

		// KD_IOGROUP_PHONEKEYPAD
		{ KD_INPUT_PHONEKEYPAD_0            , 0x5A },	    
		{ KD_INPUT_PHONEKEYPAD_1            , 0x57 },	    
		{ KD_INPUT_PHONEKEYPAD_2            , 0x58 },	    
		{ KD_INPUT_PHONEKEYPAD_3            , 0x59 },	    
		{ KD_INPUT_PHONEKEYPAD_4            , 0x53 },	    
		{ KD_INPUT_PHONEKEYPAD_5            , 0x54 },	    
		{ KD_INPUT_PHONEKEYPAD_6            , 0x55 },	    
		{ KD_INPUT_PHONEKEYPAD_7            , 0x4F },	    
		{ KD_INPUT_PHONEKEYPAD_8            , 0x50 },	    
		{ KD_INPUT_PHONEKEYPAD_9            , 0x51 },	    
		{ KD_INPUT_PHONEKEYPAD_STAR         , 0x3F },	    
		{ KD_INPUT_PHONEKEYPAD_HASH         , 0x6A },	    
		{ KD_INPUT_PHONEKEYPAD_LEFTSOFT     , 0x52 },	    
		{ KD_INPUT_PHONEKEYPAD_RIGHTSOFT    , 0x56 },
	
		{ KD_INPUT_XMSOFT_ESCAPE            , 0x09 },   
		{ KD_INPUT_XMSOFT_BACKSPACE         , 0x16 },  
		{ KD_INPUT_XMSOFT_MENU              , 0x3A },  
		{ 0                                 , 0	   },
	};	

	KDint      idx;
	
	for ( idx = 0; codes[ idx ].kd_code != 0; idx++ )
	{
		if( codes[ idx ].win_code == win_code )
		{
			return codes[ idx ].kd_code;
		}		
	}

	return 0;
}

static KDvoid xmWinGetMessage ( KDvoid )
{
	KDbool    refresh;

	while ( 1 )
	{
		if ( l_display )
		{
			refresh = KD_FALSE;

			if ( XPending ( l_display ) )
			{                
				XEvent    event;

				XNextEvent ( l_display, &event );				

				switch ( event.type )
				{
					case ClientMessage :

						if ( event.xclient.data.l[ 0 ] == XM_DELETE_WINDOW )
						{
							_xmEventDestroy ( );
						}

						break;

					case DestroyNotify :

						_xmEventDestroy ( );

						break;

					case ResizeRequest :

						_xmEventResize ( event.xresizerequest.width, event.xresizerequest.height );

						break;
					
					case ButtonPress :
						
						l_touch_pressed = 1;

						l_touch.id = (KDvoid*) l_wnd;
						l_touch.x  = event.xbutton.x;
						l_touch.y  = event.xbutton.y;

						_xmEventTouchBegan ( &l_touch, 1 );
						
						break;

					case ButtonRelease :

						l_touch_pressed = 0;

						l_touch.id = (KDvoid*) l_wnd;
						l_touch.x  = event.xbutton.x;
						l_touch.y  = event.xbutton.y;

						_xmEventTouchEnded ( &l_touch, 1 );
						
						break;

					case MotionNotify :

						if ( l_touch_pressed )
						{
							l_touch.id = (KDvoid*) l_wnd;
							l_touch.x  = event.xmotion.x;
							l_touch.y  = event.xmotion.y;

							_xmEventTouchMoved ( &l_touch, 1 );
						}
						
						break;

					case KeyRelease :	

                        //kdPrintf ( "%x", event.xkey.keycode );
						_xmEventKeyReleased ( event.xkey.keycode  );						

						break;

					case KeyPress :	
                        
						_xmEventKeyPressed ( event.xkey.keycode );						

						break;

					case VisibilityNotify :	

						_xmEventVisible ( 1 );

						break;

					case UnmapNotify :

						_xmEventVisible ( 0 );

						break;

					case FocusOut :		

						_xmEventFocus ( 0 );

						break;

					case FocusIn :		

						_xmEventFocus ( 1 );

						break;

					default : 

						refresh = KD_TRUE;
				}

				do
				{
					KDEventNativeLinux  native;
					KDEvent*  pEvent = _xmEventNew ( );

					native.display = l_display;
					native.root    = l_root;
					native.wnd     = l_wnd;
					native.event   = &event;
					pEvent->type   = KD_EVENT_NATIVE;
					pEvent->data.native.p = &native;

					_xmEventSend ( pEvent );

				} while ( 0 );
			}
			else
			{
				refresh = KD_TRUE;
			}
		}

		if ( l_wnd && refresh == KD_TRUE )
		{
			static KDuint old_msec;
			       KDuint new_msec = KD_GET_UST2MSEC;

			if ( g_interval == 0 || ( new_msec - old_msec ) >= g_interval || new_msec < old_msec )			
			{
				old_msec = new_msec;				

				_xmEventRedraw ( );
				_xmEventUpdate ( );	
			}
		}
	}
}

static KDvoid xmWinSetName ( KDvoid )
{
	KDchar    name[256] = "";
	KDchar*   path      = g_argv[ 0 ];
	KDint     i;

	for ( i = kdStrlen ( path ) - 1; i > 0; i-- )
	{
		if ( kdStrncmp ( &path[ i ], "/", 1 ) == 0 )
		{
			kdStrncat_s ( name, 256, &path[ i + 1 ], kdStrlen ( &path[ i + 1 ] ) );
			break;
		}
	}	

	XStoreName ( l_display, l_wnd, name ); 
}

// xmPlatformGetEGLNative : Get EGL native window and display type.
KDvoid xmPlatformGetEGLNative ( EGLNativeWindowType* native_window, EGLNativeDisplayType* native_display )
{
	*native_window  = l_wnd;
	*native_display = l_display;
}

// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDint x, KDint y, KDuint cx, KDuint cy )
{
	l_wnd_x  = x;
	l_wnd_y  = y;
	l_wnd_cx = cx;
	l_wnd_cy = cy;
}

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode )
{
	KDint     wnd_cx = 0;
	KDint     wnd_cy = 0;

	l_display = XOpenDisplay ( NULL );
	l_root    = DefaultRootWindow ( l_display );

	if ( !l_display )
	{
		kdLogMessage ( "XOpenDisplay( ) failed." );
		kdExit ( -3 );
	}

	if ( !l_root )
	{
		kdLogMessage ( "DefaultRootWindow( ) failed." );
		kdExit ( -3 );
	}

	XM_DELETE_WINDOW = XInternAtom ( l_display, "WM_DELETE_WINDOW", False );
	XAutoRepeatOff ( l_display );

	if ( mode & XM_SYS_LANDSCAPE )
	{
		wnd_cx = l_wnd_cy;
		wnd_cy = l_wnd_cx;
	}
	else
	{
		wnd_cx = l_wnd_cx;
		wnd_cy = l_wnd_cy;
	}

	l_wnd = XCreateSimpleWindow ( l_display, l_root, l_wnd_x, l_wnd_y, wnd_cx, wnd_cy, 1, BlackPixel ( l_display, 0 ), WhitePixel ( l_display, 0 ) );

	if ( !l_wnd )
	{
		kdLogMessage ( "XCreateSimpleWindow( ) failed." );
		kdExit ( -3 );
	}

	XSelectInput ( l_display, l_wnd, KeyPressMask | KeyReleaseMask | 
		                             ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
	                                 VisibilityChangeMask | ResizeRedirectMask | FocusChangeMask | StructureNotifyMask | PropertyChangeMask );

	XSetWMProtocols ( l_display, l_wnd, &XM_DELETE_WINDOW, 1 );	
	xmWinSetName ( );	

	_xmEventCreate ( (XMWindow*) l_wnd );

	XMapWindow ( l_display, l_wnd );	
	XFlush ( l_display );	

	_xmEventResize ( wnd_cx, wnd_cy );

	xmWinGetMessage ( );
}

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid )
{
	if ( l_display )
	{
		if ( l_wnd )
		{
			XDestroyWindow ( l_display, l_wnd );	
			l_wnd = 0;
		}

		XCloseDisplay ( l_display );
		l_display = 0;
	}
}

KDvoid xmPlatformEventAttach ( KDenum mode )
{
  
}

KDvoid xmPlatformEventDetach ( KDenum mode )
{
    
}

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume )
{
	return -1;
}

KDfloat xmPlatformSoundGetVolume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint pos )
{
	return -1;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	return -1;
}