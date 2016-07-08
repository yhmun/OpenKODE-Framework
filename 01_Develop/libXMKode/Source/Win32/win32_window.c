/* --------------------------------------------------------------------------
 *
 *      File            win32_window.c
 *      Description     Window functions.
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

#ifdef XM_HAS_PLATFORM_EGL

#include <windows.h>              
#include <GL/glew.h>
#include <GL/gl.h>

#endif

#include "../kd_library.h"
#include "platform.h"

#define XM_IDI_ICON                     100
#define XM_KEY_MASK_REPEAT              ( 1 << 30 )
#define XM_GPS_TIMER                    1000
#define XM_SOUND_EFFECT_WAV				0x0002		// For Win32 Wave

static KDint        l_wnd_x    = CW_USEDEFAULT;
static KDint        l_wnd_y    = CW_USEDEFAULT;
static KDuint       l_wnd_cx   = CW_USEDEFAULT;
static KDuint       l_wnd_cy   = CW_USEDEFAULT;
static KDuint		l_wnd_full = 0;
static HWND			l_wnd      = 0;
static HDC          l_wnd_dc   = 0;
static KDint        l_wnd_reg  = 0;
static TCHAR        l_wnd_class[256];
static KDmode		l_wnd_mode = 0;
#ifndef _WIN32_WCE
static HGLRC		l_wnd_rc   = 0;
#endif

static KDshort		l_virtKey  = 0;
static KDTouch		l_touch [ 2 ];

static HANDLE       l_gps		  = INVALID_HANDLE_VALUE;
static KDThread*    l_gps_thread  = KD_NULL;
static KDThreadSem* l_gps_lock    = KD_NULL;
static KDbool       l_gps_running = KD_FALSE;

static KDvoid PVRFrameEnableControlWindow ( KDbool bEnable )
{
#ifndef _WIN32_WCE
    HKEY          hKey = 0;
	const TCHAR*  tszValue = _T( "hide_gui" );
	const TCHAR*  tszNewData = ( bEnable ) ? _T( "NO" ) : _T( "YES" );
	TCHAR         tszOldData[256] = { 0 };
	DWORD         dwSize = sizeof ( tszOldData );
	LSTATUS       status;

    // Open PVRFrame control key, if not exist create it.
    if ( ERROR_SUCCESS != RegCreateKeyEx ( HKEY_CURRENT_USER,
        _T ( "Software\\Imagination Technologies\\PVRVFRame\\STARTUP\\" ),
        0,
        0,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        0,
        &hKey,
        KD_NULL ) )
    {
        return;
    }
    
    status = RegQueryValueEx ( hKey, tszValue, 0, KD_NULL, (LPBYTE) tszOldData, &dwSize );
    if ( ERROR_FILE_NOT_FOUND == status || ( ERROR_SUCCESS == status && 0 != _tcscmp ( tszNewData, tszOldData ) ) )   
    {
        dwSize = sizeof ( TCHAR ) * ( _tcslen ( tszNewData ) + 1 );
        RegSetValueEx ( hKey, tszValue, 0, REG_SZ, (const BYTE *) tszNewData, dwSize );
    }

    RegCloseKey ( hKey );
#endif
}

static  KDvoid*	 xmGPSSerial ( KDvoid* arg )
{
	l_gps_running = KD_TRUE;

	while ( l_gps_running )
	{
		DWORD    bytes = 0;
		DWORD	 error = 0;
		
		KDchar   buff [ 1024 ];
		COMSTAT  comstat;

		kdThreadSemWait ( l_gps_lock );

		ReadFile ( l_gps, buff,	1024, &bytes, NULL );			

		if ( bytes )
		{
			xmParseNMEA ( KD_TRUE, buff, (KDsize) bytes );
		}

		error = GetLastError ( );
		ClearCommError ( l_gps, &error, &comstat );	

		kdThreadSemPost ( l_gps_lock );

		Sleep ( 1000 );
	}

	return 0;
}

static KDbool xmGPSOpen ( KDuint32 port_number, KDuint32 baud_rate, KDuint8 parity_bit, KDuint8 stop_bits, KDuint8 byte_size )
{
	TCHAR  szPort[6] = _T( "" );
	_stprintf ( szPort, _T( "COM%d:" ), port_number );

	l_gps = CreateFile ( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( l_gps == INVALID_HANDLE_VALUE )
	{
		DWORD  error = GetLastError ( );

		switch ( error )
		{
			case ERROR_ACCESS_DENIED :

				kdLogMessagefKHR ( "xmGPSOpen : Com %d - ERROR_ACCESS_DENIED", port_number );

				break;

			case ERROR_FILE_NOT_FOUND :

				kdLogMessagefKHR ( "xmGPSOpen : Com %d - ERROR_FILE_NOT_FOUND", port_number );

				break;

			default :

				kdLogMessagefKHR ( "xmGPSOpen : Com %d - ERROR_UNKNOWN", port_number );

				break;
		}
	}
	else
	{
		DCB  dcb;

		dcb.DCBlength = sizeof ( DCB );

		// Get the default port setting information.
		GetCommState ( l_gps, &dcb );	

		// Need to set baud separately
		dcb.BaudRate			= baud_rate;
		dcb.Parity				= parity_bit;
		
		dcb.StopBits			= stop_bits;
		dcb.ByteSize			= byte_size;

		dcb.fBinary				= TRUE;					// binary mode, no EOF check 
		dcb.fParity				= TRUE;
		dcb.fOutxCtsFlow		= FALSE;				// CTS output flow control 
		dcb.fOutxDsrFlow		= FALSE;				// DSR output flow control 
		dcb.fDtrControl			= DTR_CONTROL_ENABLE;	// DTR flow control type 
		dcb.fDsrSensitivity		= FALSE;				// DSR sensitivity 
		dcb.fTXContinueOnXoff	= TRUE;
		dcb.fOutX				= FALSE;				// XON/XOFF out flow control 
		dcb.fInX				= FALSE;				// XON/XOFF in flow control 
		dcb.fErrorChar			= FALSE;
		dcb.fNull				= FALSE;				// enable null stripping 
		dcb.fRtsControl			= RTS_CONTROL_ENABLE;	// RTS flow control 
		dcb.fAbortOnError		= FALSE;				// abort reads/writes on error 

		// Configure the port according to the specifications of the DCB 
		// structure.
		if ( !SetCommState ( l_gps, &dcb ) )					// PMI Code
		{
			kdLogMessage ( "xmGPSOpen : Unable to configure the serial port." );
		}
		else
		{
			// Setup the comm timeouts
			COMMTIMEOUTS  timeout;
			GetCommTimeouts ( l_gps, &timeout);					// PMI Code

			timeout.ReadIntervalTimeout		    = MAXDWORD;
			timeout.ReadTotalTimeoutMultiplier  = 0;
			timeout.ReadTotalTimeoutConstant	= 0;
			timeout.WriteTotalTimeoutMultiplier = 0;
			timeout.WriteTotalTimeoutConstant	= 1000;

			// Set the time-out parameters for all read and write operations
			// on the port. 
			if ( !SetCommTimeouts ( l_gps, &timeout ) )		// PMI Code
			{
				kdLogMessage ( "xmGPSOpen : Unable to set the time-out parameters." );
			}
			else
			{
				DWORD     error;
				COMSTAT   comstat;

				ClearCommError ( l_gps, &error, &comstat );
				SetupComm ( l_gps, 16384, 16384 ); 
				PurgeComm ( l_gps, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );		// PMI Code
				SetCommMask ( l_gps, EV_RXCHAR );

				kdLogMessagefKHR ( "xmGPSOpen : Com %d Baud Rate %d - SUCCESS.", port_number, baud_rate );

				l_gps_thread = kdThreadCreate ( KD_NULL, xmGPSSerial, 0 );
				l_gps_lock = kdThreadSemCreate ( 1 );

				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

static KDvoid xmGPSClose ( KDvoid )
{
	if ( l_gps != INVALID_HANDLE_VALUE )
	{
		l_gps_running = KD_FALSE;
		kdThreadJoin ( l_gps_thread, KD_NULL );
		l_gps_thread = KD_NULL;
		
		kdThreadSemFree ( l_gps_lock );
		l_gps_lock = KD_NULL;

		CloseHandle ( l_gps );
		l_gps = INVALID_HANDLE_VALUE;

		Sleep ( 100 );
	}
}

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code )
{
	static struct XMKeyCode  codes[ ] =
	{
		// KD_IOGROUP_GAMEKEYS
		{ KD_INPUT_GAMEKEYS_LEFT			, VK_LEFT		},	
		{ KD_INPUT_GAMEKEYS_UP				, VK_UP			},			
		{ KD_INPUT_GAMEKEYS_RIGHT			, VK_RIGHT		},	
		{ KD_INPUT_GAMEKEYS_DOWN			, VK_DOWN		},	
		{ KD_INPUT_GAMEKEYS_FIRE			, VK_SPACE		},	
		{ KD_INPUT_GAMEKEYS_A				, 'A'			},	
		{ KD_INPUT_GAMEKEYS_B				, 'S'			},	
		{ KD_INPUT_GAMEKEYS_C				, 'D'			},	
		{ KD_INPUT_GAMEKEYS_D				, 'W'			},	

		// KD_IOGROUP_PHONEKEYPAD
		{ KD_INPUT_PHONEKEYPAD_0			, VK_NUMPAD0	},	    
		{ KD_INPUT_PHONEKEYPAD_1			, VK_NUMPAD1	},	    
		{ KD_INPUT_PHONEKEYPAD_2			, VK_NUMPAD2	},	    
		{ KD_INPUT_PHONEKEYPAD_3			, VK_NUMPAD3	},	    
		{ KD_INPUT_PHONEKEYPAD_4			, VK_NUMPAD4	},	    
		{ KD_INPUT_PHONEKEYPAD_5			, VK_NUMPAD5	},	    
		{ KD_INPUT_PHONEKEYPAD_6			, VK_NUMPAD6	},	    

		{ KD_INPUT_PHONEKEYPAD_7			, VK_NUMPAD7	},	    
		{ KD_INPUT_PHONEKEYPAD_8			, VK_NUMPAD8	},	    
		{ KD_INPUT_PHONEKEYPAD_9			, VK_NUMPAD9	},	    
		{ KD_INPUT_PHONEKEYPAD_STAR			, VK_MULTIPLY	},	    
		{ KD_INPUT_PHONEKEYPAD_HASH			, VK_DIVIDE		},	    
		{ KD_INPUT_PHONEKEYPAD_LEFTSOFT		, VK_SUBTRACT	},	    
		{ KD_INPUT_PHONEKEYPAD_RIGHTSOFT	, VK_ADD		},	  

		{ KD_INPUT_XMSOFT_ESCAPE			, VK_ESCAPE		},
		{ KD_INPUT_XMSOFT_BACKSPACE	    	, VK_BACK		},
		{ KD_INPUT_XMSOFT_MENU   			, 'M'    		},	
		{ KD_INPUT_XMSOFT_COMMA   			, ','    		},	
		{ KD_INPUT_XMSOFT_PERIOD  			, '.'    		},	
		{ KD_INPUT_XMSOFT_COMMA  			, VK_OEM_COMMA	},
		{ KD_INPUT_XMSOFT_PERIOD  			, VK_OEM_PERIOD	},
		{ 0									, 0				},
	};	
	
	KDint      kd_code = 0;
	KDint      idx = 0;
	
	for ( idx = 0; codes[ idx ].kd_code != 0; idx++ )
	{
		if( codes[ idx ].win_code == win_code )
		{
			return codes[ idx ].kd_code;
		}		
	}

	return 0;
}

static LRESULT CALLBACK xmWndProc ( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch ( msg )
	{
		case WM_CREATE :

			l_wnd = hwnd;

			l_wnd_dc = GetDC ( l_wnd );

		#ifdef XM_HAS_PLATFORM_EGL
			
			{
				// Setup Pixel Format
				KDint	nPixelFormat;

				PIXELFORMATDESCRIPTOR	tPFD =
				{
					sizeof ( PIXELFORMATDESCRIPTOR ),	// size
					1,									// version
					PFD_SUPPORT_OPENGL |				// OpenGL window
					PFD_DRAW_TO_WINDOW |				// render to window
					PFD_DOUBLEBUFFER,					// support double-buffering
					PFD_TYPE_RGBA,						// color type
					32,									// preferred color depth
					0, 0, 0, 0, 0, 0,					// color bits (ignored)
					0,									// no alpha buffer
					0,									// alpha bits (ignored)
					0,									// no accumulation buffer
					0, 0, 0, 0,							// accum bits (ignored)
					24,									// depth buffer
					8,									// no stencil buffer
					0,									// no auxiliary buffers
					PFD_MAIN_PLANE,						// main layer
					0,									// reserved
					0, 0, 0,							// no layer, visible, damage masks
				};

				nPixelFormat = ChoosePixelFormat ( l_wnd_dc, &tPFD );
				SetPixelFormat ( l_wnd_dc, nPixelFormat, &tPFD );

				// Create WGL Context
				l_wnd_rc = wglCreateContext ( l_wnd_dc );
				wglMakeCurrent ( l_wnd_dc, l_wnd_rc );

				// Version Check
				{
					const KDchar*	szVersion = (const KDchar*) glGetString ( GL_VERSION );
					const KDchar*   szExtensions = (const KDchar*) glGetString ( GL_EXTENSIONS );
					KDchar			szMessage [ 256 ] = "";
					TCHAR*			tszMessage = KD_NULL;
					GLenum			nResult = 0;

					kdPrintf ( "* OpenGL Version = %s", szVersion );

					if ( kdAtof ( szVersion ) < 1.5f )
					{				
						kdSprintf ( szMessage, "OpenGL 1.5 or higher is required (your version is %s). Please upgrade the driver of your video card.", szVersion );

						XM_SET_TSTR ( tszMessage, szMessage );
						MessageBox ( l_wnd, tszMessage, _T( "OpenGL version too old" ), MB_OK );
						KD_FREE_TSTR ( tszMessage );

						kdExit ( 0 );
					}

					// Initialize GLEW
					nResult = glewInit ( );
					if ( GLEW_OK != nResult )
					{
						kdSprintf ( szMessage, "%s", (const KDchar*) glewGetErrorString ( nResult ) );
						XM_SET_TSTR ( tszMessage, szMessage );
						MessageBox ( l_wnd, tszMessage, _T( "OpenGL error" ), MB_OK );
						KD_FREE_TSTR ( tszMessage );

						kdExit ( 0 );
					}

					if ( GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader )
					{
						kdPrintf ( "* Ready for GLSL" );
					}
					else
					{
						kdPrintf ( "* Not totally ready :( %s )", glewIsSupported ( "GL_VERSION_2_0" ) ? "Ready for OpenGL 2.0" : "OpenGL 2.0 not supported" );
					}

					// GLEW Dynamic Binding
					if ( glGenFramebuffers == KD_NULL )
					{
						kdPrintf ( "* OpenGL : glGenFramebuffers is NULL, try to detect an extension" );
						if ( kdStrstr ( szExtensions, "ARB_framebuffer_object" ) )
						{
							kdPrintf ( "* OpenGL : ARB_framebuffer_object is supported" );

							glIsRenderbuffer						= (PFNGLISRENDERBUFFERPROC)							wglGetProcAddress ( "glIsRenderbuffer"							);
							glBindRenderbuffer						= (PFNGLBINDRENDERBUFFERPROC)						wglGetProcAddress ( "glBindRenderbuffer"						);
							glDeleteRenderbuffers					= (PFNGLDELETERENDERBUFFERSPROC)					wglGetProcAddress ( "glDeleteRenderbuffers"						);
							glGenRenderbuffers						= (PFNGLGENRENDERBUFFERSPROC)						wglGetProcAddress ( "glGenRenderbuffers"						);
							glRenderbufferStorage					= (PFNGLRENDERBUFFERSTORAGEPROC)					wglGetProcAddress ( "glRenderbufferStorage"						);
							glGetRenderbufferParameteriv			= (PFNGLGETRENDERBUFFERPARAMETERIVPROC)				wglGetProcAddress ( "glGetRenderbufferParameteriv"				);
							glIsFramebuffer							= (PFNGLISFRAMEBUFFERPROC)							wglGetProcAddress ( "glIsFramebuffer"							);
							glBindFramebuffer						= (PFNGLBINDFRAMEBUFFERPROC)						wglGetProcAddress ( "glBindFramebuffer"							);
							glDeleteFramebuffers					= (PFNGLDELETEFRAMEBUFFERSPROC)						wglGetProcAddress ( "glDeleteFramebuffers"						);
							glGenFramebuffers						= (PFNGLGENFRAMEBUFFERSPROC)						wglGetProcAddress ( "glGenFramebuffers"							);
							glCheckFramebufferStatus				= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)					wglGetProcAddress ( "glCheckFramebufferStatus"					);
							glFramebufferTexture1D					= (PFNGLFRAMEBUFFERTEXTURE1DPROC)					wglGetProcAddress ( "glFramebufferTexture1D"					);
							glFramebufferTexture2D					= (PFNGLFRAMEBUFFERTEXTURE2DPROC)					wglGetProcAddress ( "glFramebufferTexture2D"					);
							glFramebufferTexture3D					= (PFNGLFRAMEBUFFERTEXTURE3DPROC)					wglGetProcAddress ( "glFramebufferTexture3D"					);
							glFramebufferRenderbuffer				= (PFNGLFRAMEBUFFERRENDERBUFFERPROC)				wglGetProcAddress ( "glFramebufferRenderbuffer"					);
							glGetFramebufferAttachmentParameteriv	= (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)	wglGetProcAddress ( "glGetFramebufferAttachmentParameteriv"		);
							glGenerateMipmap						= (PFNGLGENERATEMIPMAPPROC)							wglGetProcAddress ( "glGenerateMipmap"							);
						}
						else if ( kdStrstr ( szExtensions, "EXT_framebuffer_object" ) )
						{
							kdPrintf ( "* OpenGL: EXT_framebuffer_object is supported" );

							glIsRenderbuffer						= (PFNGLISRENDERBUFFERPROC)							wglGetProcAddress ( "glIsRenderbufferEXT"						);
							glBindRenderbuffer						= (PFNGLBINDRENDERBUFFERPROC)						wglGetProcAddress ( "glBindRenderbufferEXT"						);
							glDeleteRenderbuffers					= (PFNGLDELETERENDERBUFFERSPROC)					wglGetProcAddress ( "glDeleteRenderbuffersEXT"					);
							glGenRenderbuffers						= (PFNGLGENRENDERBUFFERSPROC)						wglGetProcAddress ( "glGenRenderbuffersEXT"						);
							glRenderbufferStorage					= (PFNGLRENDERBUFFERSTORAGEPROC)					wglGetProcAddress ( "glRenderbufferStorageEXT"					);
							glGetRenderbufferParameteriv			= (PFNGLGETRENDERBUFFERPARAMETERIVPROC)				wglGetProcAddress ( "glGetRenderbufferParameterivEXT"			);
							glIsFramebuffer							= (PFNGLISFRAMEBUFFERPROC)							wglGetProcAddress ( "glIsFramebufferEXT"						);
							glBindFramebuffer						= (PFNGLBINDFRAMEBUFFERPROC)						wglGetProcAddress ( "glBindFramebufferEXT"						);
							glDeleteFramebuffers					= (PFNGLDELETEFRAMEBUFFERSPROC)						wglGetProcAddress ( "glDeleteFramebuffersEXT"					);
							glGenFramebuffers						= (PFNGLGENFRAMEBUFFERSPROC)						wglGetProcAddress ( "glGenFramebuffersEXT"						);
							glCheckFramebufferStatus				= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)					wglGetProcAddress ( "glCheckFramebufferStatusEXT"				);
							glFramebufferTexture1D					= (PFNGLFRAMEBUFFERTEXTURE1DPROC)					wglGetProcAddress ( "glFramebufferTexture1DEXT"					);
							glFramebufferTexture2D					= (PFNGLFRAMEBUFFERTEXTURE2DPROC)					wglGetProcAddress ( "glFramebufferTexture2DEXT"					);
							glFramebufferTexture3D					= (PFNGLFRAMEBUFFERTEXTURE3DPROC)					wglGetProcAddress ( "glFramebufferTexture3DEXT"					);
							glFramebufferRenderbuffer				= (PFNGLFRAMEBUFFERRENDERBUFFERPROC)				wglGetProcAddress ( "glFramebufferRenderbufferEXT"				);
							glGetFramebufferAttachmentParameteriv	= (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)	wglGetProcAddress ( "glGetFramebufferAttachmentParameterivEXT"	);
							glGenerateMipmap						= (PFNGLGENERATEMIPMAPPROC)							wglGetProcAddress ( "glGenerateMipmapEXT"						);
						}
						else
						{
							kdPrintf ( "* OpenGL: No framebuffers extension is supported" );
							kdPrintf ( "* OpenGL: Any call to Fbo will crash!" );
							
							XM_SET_TSTR ( tszMessage, "No OpenGL framebuffer support. Please upgrade the driver of your video card." );
							MessageBox ( l_wnd, tszMessage, _T( "OpenGL error" ), MB_OK );
							KD_FREE_TSTR ( tszMessage );

							kdExit ( 0 );
						}

					}

					glEnable ( GL_VERTEX_PROGRAM_POINT_SIZE );
				}
			}

		#endif	// XM_HAS_PLATFORM_EGL

			_xmEventCreate ( (XMWindow) hwnd );

			break;

		case WM_DESTROY	:

			_xmEventDestroy ( );

			break;

		case WM_SIZE :
		
			{
				RECT        rect;

				GetClientRect ( hwnd, &rect );

				_xmEventResize ( rect.right - rect.left, rect.bottom - rect.top );   
			}
			
			break; 
						
		case WM_RBUTTONDOWN :
		case WM_MBUTTONDOWN :
		case WM_XBUTTONDOWN :
		case WM_LBUTTONDOWN :
		{
			KDint	x = GET_X_LPARAM ( lparam );
			KDint	y = GET_Y_LPARAM ( lparam );
			KDint	code = 0;

			SetCapture ( hwnd );	

			switch ( msg )
			{
				case WM_RBUTTONDOWN :
					code = KD_INPUT_MOUSE_BUTTON_2;
					break;

				case WM_MBUTTONDOWN :
					code = KD_INPUT_MOUSE_BUTTON_3;
					break;

				case WM_XBUTTONDOWN :
					code = HIWORD ( wparam ) == XBUTTON1 ? KD_INPUT_MOUSE_BUTTON_4 : KD_INPUT_MOUSE_BUTTON_5;
					break;

				case WM_LBUTTONDOWN :	
					code = KD_INPUT_MOUSE_BUTTON_1;

					l_touch [ 0 ].id = (KDvoid*) hwnd;
					l_touch [ 0 ].x  = x;
					l_touch [ 0 ].y  = y;

					if ( l_virtKey )
					{
						l_touch [ 1 ].id = (KDvoid*) ( (KDint*) hwnd + 1 );
						l_touch [ 1 ].x  = l_wnd_cx - x;
						l_touch [ 1 ].y  = l_wnd_cy - y;

						_xmEventTouchBegan ( l_touch, 2 );
					}
					else
					{
						_xmEventTouchBegan ( l_touch, 1 );
					}
					break;
			}

			_xmEventMousePressed ( x, y, code );

		}	break;

		case WM_LBUTTONUP :
		case WM_RBUTTONUP :
		case WM_MBUTTONUP :
		case WM_XBUTTONUP :
		{
			KDint16    x = GET_X_LPARAM ( lparam );
			KDint16    y = GET_Y_LPARAM ( lparam );
			KDint      code = 0;

			if ( GetCapture ( ) == hwnd )
			{
				ReleaseCapture ( );
			}
			
			switch ( msg )
			{
				case WM_RBUTTONUP :
					code = KD_INPUT_MOUSE_BUTTON_2;
					break;

				case WM_MBUTTONUP :
					code = KD_INPUT_MOUSE_BUTTON_3;
					break;

				case WM_XBUTTONUP :
					code = HIWORD ( wparam ) == XBUTTON1 ? KD_INPUT_MOUSE_BUTTON_4 : KD_INPUT_MOUSE_BUTTON_5;
					break;
			
				case WM_LBUTTONUP :
					code = KD_INPUT_MOUSE_BUTTON_1;
				
					l_touch [ 0 ].id = (KDvoid*) hwnd;
					l_touch [ 0 ].x  = x;
					l_touch [ 0 ].y  = y;

					if ( l_touch [ 1 ].id != 0 )
					{
						l_touch [ 1 ].id = (KDvoid*) ( (KDint*) hwnd + 1 );
						l_touch [ 1 ].x  = l_wnd_cx - x;
						l_touch [ 1 ].y  = l_wnd_cy - y;

						_xmEventTouchEnded ( l_touch, 2 );					
					}
					else
					{
						_xmEventTouchEnded ( l_touch, 1 );
					}

					kdMemset ( l_touch, 0, sizeof ( l_touch ) );
					break;
			}

			_xmEventMouseReleased ( x, y, code );

		}	break;

		case WM_MOUSEMOVE :
		{
			KDint16    x = GET_X_LPARAM ( lparam );
			KDint16    y = GET_Y_LPARAM ( lparam );

			if ( wparam & MK_LBUTTON )
			{								
				l_touch [ 0 ].id = (KDvoid*) hwnd;
				l_touch [ 0 ].x  = x;
				l_touch [ 0 ].y  = y;

				if ( l_touch [ 1 ].id != 0 )
				{
					l_touch [ 1 ].id = (KDvoid*) ( (KDint*) hwnd + 1 );
					l_touch [ 1 ].x  = l_wnd_cx - x;
					l_touch [ 1 ].y  = l_wnd_cy - y;

					_xmEventTouchMoved ( l_touch, 2 );
				}
				else
				{
					_xmEventTouchMoved ( l_touch, 1 );
				}
			}		
	
			_xmEventMouseMoved ( x, y );

		}	break;

		case WM_MOUSEWHEEL :		
			_xmEventMouseScrolled ( 0, (SHORT) HIWORD ( wparam ) / WHEEL_DELTA );
			break;

		case WM_MOUSEHWHEEL :		
			_xmEventMouseScrolled ( (SHORT) HIWORD ( wparam ) / WHEEL_DELTA, 0 );
			break;
		
		case WM_CHAR :

			if ( wparam < 0x20 )
			{
				if      ( wparam == VK_BACK )
				{
					_xmEventKeyPressed ( KD_INPUT_XMSOFT_PREV );	
				}
				else if ( wparam == VK_BACK )
				{
					_xmEventKeyPressed ( KD_INPUT_XMSOFT_RETURN );	
				}
			}
			else if ( wparam < 128 )
			{
				_xmEventInsertText ( (const KDchar*) &wparam, 1 );
			}
			else
			{
				char szUtf8[8] = { 0 };
				int  nLen = WideCharToMultiByte ( CP_UTF8, 0, (LPCWSTR) &wparam, 1, szUtf8, sizeof ( szUtf8 ), NULL, NULL );

				_xmEventInsertText ( szUtf8, nLen );
			}

			if ( lparam & XM_KEY_MASK_REPEAT )			
			{								
				break;
			}

			if ( !( ( wparam >= 'A' && wparam <= 'Z' ) || ( wparam >= 'a' && wparam <= 'z' ) ) )
			{
				break;
			}

			if ( wparam >= 'a' )
			{
				wparam -= 0x20;
			} 	

			_xmEventKeyPressed ( (KDint) wparam );	

			break;

		case WM_KEYDOWN :
		{			
			if ( wparam == VK_CONTROL )
			{
				l_virtKey = GetKeyState ( VK_LCONTROL ) & 0x8000;

				if ( l_virtKey && l_touch [ 0 ].id != 0 && l_touch [ 1 ].id == 0 )
				{
					l_touch [ 1 ].id = (KDvoid*) ( (KDint*) l_touch [ 0 ].id + 1 );
					l_touch [ 1 ].x  = l_wnd_cx - l_touch [ 0 ].x;
					l_touch [ 1 ].y  = l_wnd_cy - l_touch [ 0 ].y;

					_xmEventTouchBegan ( &l_touch [ 1 ], 1 );
				}
			}

			if ( lparam & XM_KEY_MASK_REPEAT )			
			{								
				break;
			}

			if ( wparam >= 'A' && wparam <= 'Z' )
			{
				break;
			}

			_xmEventKeyPressed ( (KDint) wparam );		

		}	break;

		case WM_KEYUP :

			if ( wparam == VK_CONTROL )
			{
				l_virtKey = GetKeyState ( VK_LCONTROL ) & 0x8000;

				if ( !l_virtKey && l_touch [ 1 ].id != 0 )
				{					
					l_touch [ 1 ].x  = l_wnd_cx - l_touch [ 0 ].x;
					l_touch [ 1 ].y  = l_wnd_cy - l_touch [ 0 ].y;

					_xmEventTouchEnded ( &l_touch [ 1 ], 1 );

					l_touch [ 1 ].id = 0;
				}				
			}

			if ( wparam == 0xe5 )
			{
				break;
			}				

			_xmEventKeyReleased ( (KDint) wparam );		

			break;

		case WM_SHOWWINDOW :
			
			_xmEventVisible ( wparam == TRUE ? 1 : 0 );    
			
			break;			

		case WM_SETFOCUS :
			
			_xmEventFocus ( 1 );    
			
			break;	

		case WM_KILLFOCUS :
			
			_xmEventFocus ( 0 );   
			
			break;	

#ifndef _WIN32_WCE
		case MM_MCINOTIFY :

			if ( wparam == MCI_NOTIFY_SUCCESSFUL )
			{
				xmSoundCompleted ( (KDvoid*) lparam );
			}

			break;
#endif

//#ifdef Platform_SiRFPrima_EVB_CE6
		case WM_PAINT :
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint ( hwnd, &ps );
				EndPaint ( hwnd, &ps );
			}
			break;
//#endif

	}

	do
	{
		KDEventNativeWin32  native = { hwnd, msg, wparam, lparam };
		KDEvent*  event = _xmEventNew ( );

		event->type = KD_EVENT_NATIVE;
		event->data.native.p = &native;

		_xmEventSend ( event );

	} while ( 0 );
	 
	return DefWindowProc ( hwnd, msg, wparam, lparam );
}

static KDvoid xmRegisterClass ( KDvoid )
{	
	WNDCLASS	wc;	

#ifndef _WIN32_WCE
	swprintf_s ( l_wnd_class, 256, _T( "XMKode : 0x%08x" ), &wc );	
#else
	swprintf ( l_wnd_class, _T( "XMKode : 0x%08x" ), &wc );	
#endif

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = xmWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = g_inst;
	wc.hIcon         = LoadIcon ( g_inst, MAKEINTRESOURCE ( XM_IDI_ICON ) );
	wc.hCursor       = LoadCursor ( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH) GetStockObject ( BLACK_BRUSH );		
	wc.lpszMenuName  = 0;
	wc.lpszClassName = l_wnd_class;	
		
	if ( RegisterClass ( &wc ) == 0 )		
	{
		kdLogMessage ( "Window class register failed." );
		kdExit ( 3 );
	}

	l_wnd_reg = 1;

	kdMemset ( l_touch, 0, sizeof ( l_touch ) );
}

static KDvoid xmUnregisterClass ( KDvoid )
{
	if ( l_wnd_reg )
	{
		UnregisterClass( l_wnd_class, g_inst );
		l_wnd_reg = 0;
	}
}

static KDvoid xmCreateWindow ( KDmode mode )
{
	DWORD       style    = 0;
	DWORD       exstyle  = 0;

	KDuint      frame_cx = 0;
	KDuint      frame_cy = 0;

	TCHAR       tpath[MAX_PATH] = _T( "" );
	TCHAR       tname[MAX_PATH] = _T( "" );
	KDint       i;

	GetModuleFileName ( NULL, tpath, MAX_PATH );

	for ( i = _tcslen ( tpath ) - 1; i > 0; i-- )
	{
		if ( _tcsncmp ( &tpath[ i ], _T( "\\" ), 1 ) == 0 )
		{
			_tcsncat ( tname, &tpath[ i + 1 ], _tcslen ( &tpath[ i + 1 ] ) - 4 );
			break;
		}
	}	

	if ( l_wnd_full )
	{
		style   = WS_VISIBLE;
		exstyle = WS_EX_TOPMOST;

		l_wnd_x = 0;
		l_wnd_y = 0;

		l_wnd_cx  = GetSystemMetrics ( SM_CXSCREEN );
		l_wnd_cy  = GetSystemMetrics ( SM_CYSCREEN );

	#if defined ( Platform_Nexus_s3c6410 )
		ChipSetting_init ( 0, 0, wnd_cx, wnd_cy, wnd_cx );
	#endif
	}
	else
	{
		style    = WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;

	#ifdef _WIN32_WCE

		l_wnd_x = 0;
		l_wnd_y = 0;

		frame_cx = GetSystemMetrics ( SM_CXBORDER ) * 2;
		frame_cy = GetSystemMetrics ( SM_CYBORDER ) * 2 + GetSystemMetrics ( SM_CYCAPTION );

		l_wnd_cx = GetSystemMetrics ( SM_CXSCREEN ) - frame_cx;
		l_wnd_cy = GetSystemMetrics ( SM_CYSCREEN ) - frame_cy;

	#else

		PVRFrameEnableControlWindow ( KD_FALSE );

		if ( mode & XM_SYS_LANDSCAPE )
		{
			KD_SWAP ( l_wnd_cx, l_wnd_cy, KDuint );
		}
		{
			RECT	rect = { 0, 0, l_wnd_cx, l_wnd_cy };

			AdjustWindowRectEx ( &rect, style, FALSE, exstyle );

			l_wnd_cx = rect.right - rect.left;
			l_wnd_cy = rect.bottom - rect.top;
		}

	#endif
	}

	CreateWindowEx ( exstyle, l_wnd_class, tname, style, l_wnd_x, l_wnd_y, l_wnd_cx + frame_cx, l_wnd_cy + frame_cy, NULL, NULL, g_inst, NULL );

    ShowWindow ( l_wnd, SW_SHOW );
    UpdateWindow ( l_wnd );
	SetFocus ( l_wnd );	

	if ( mode & XM_SYS_LOCATION )
	{
		KDint  specific_port = 0;
		KDint  port = 0;
		KDbool ret  = KD_FALSE;

		#if defined ( Platform_SiRFPrima_EVB_CE6 ) || defined ( Platform_TCC8801 )

		specific_port = 1;

		#endif

		if ( specific_port != 0 )
		{
			ret = xmGPSOpen ( specific_port, 9600, NOPARITY, ONESTOPBIT, 8 );
		}

		if ( ret == KD_FALSE )
		{
			for ( port = 1; port < 9; port++ )
			{
				if ( specific_port == port )
				{
					continue;
				}

				ret = xmGPSOpen ( port, 9600, NOPARITY, ONESTOPBIT, 8 );
				if ( ret == KD_TRUE )
				{
					break;
				}
			}
		}
	}
}

static KDvoid xmDestroyWindow ( KDvoid )
{
#ifdef XM_HAS_PLATFORM_EGL

	if ( l_wnd_dc && l_wnd_rc )
	{
		wglMakeCurrent ( l_wnd_dc, KD_NULL );
		wglDeleteContext ( l_wnd_rc );
	}
	
#endif	// XM_HAS_PLATFORM_EGL

	if ( l_wnd_dc )
	{
		ReleaseDC ( l_wnd, l_wnd_dc );
		l_wnd_dc = 0;
	}

	if ( l_wnd )
	{
		DestroyWindow ( l_wnd );
		l_wnd = 0;
	}

#if defined ( Platform_Nexus_s3c6410 )
	ChipSetting_terminate ( );
#endif
}

static KDvoid xmWinGetMessage ( KDvoid )
{
	while ( 1 )
	{
		MSG	 msg;

		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) == TRUE )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}	

		if ( l_wnd )
		{			
			static KDdouble old_msec;
			       KDdouble new_msec = kdGetMilliseconds ( );
					
			if ( g_interval == 0 || ( new_msec - old_msec ) >= g_interval || new_msec < old_msec )		
			{
				old_msec = new_msec;				

				_xmEventRedraw ( );
				_xmEventUpdate ( );	

			#ifdef XM_HAS_PLATFORM_EGL

				SwapBuffers ( l_wnd_dc );
				
			#endif	// XM_HAS_PLATFORM_EGL

			}
			else
			{				
				Sleep ( 1 );
			}
		}
	}
}

#ifndef XM_HAS_PLATFORM_EGL
// xmPlatformGetEGLNative : Get EGL native window and display type.
KDvoid xmPlatformGetEGLNative ( EGLNativeWindowType* native_window, EGLNativeDisplayType* native_display )
{
	*native_window  = l_wnd;
	*native_display = l_wnd_dc;
}
#endif

// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDint x, KDint y, KDuint cx, KDuint cy, KDbool fullscreen )
{
	l_wnd_full = fullscreen;
	l_wnd_x    = x;
	l_wnd_y    = y;
	l_wnd_cx   = cx;
	l_wnd_cy   = cy;
}

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode )
{
	l_wnd_mode = xmGetWindowMode ( );

	xmRegisterClass ( );

	xmCreateWindow ( mode );

	xmWinGetMessage ( );
}

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid )
{
	xmGPSClose ( );

	xmDestroyWindow ( );

	xmUnregisterClass ( );
}

KDvoid xmPlatformEventAttach ( KDenum mode )
{
  
}

KDvoid xmPlatformEventDetach ( KDenum mode )
{
    
}

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( sound->mode & XM_SOUND_EFFECT )
	{
		KDsize   size  = kdStrlen ( sound->fpath );
		KDchar*  ext   = KD_NULL;

		if ( size > 3 )
		{
			ext = kdStrdup ( &sound->fpath [ size - 4 ] );
			if ( !kdStrcmp ( ext, "WAV" ) )
			{
				sound->mode |= XM_SOUND_EFFECT_WAV;
				return 0;
			}
		}
	}

	{
		MCI_OPEN_PARMS  mciOpen = { 0 };
		MCIERROR        mciError;
		TCHAR*          tpath;

		XM_SET_TSTR ( tpath, sound->fpath );
		mciOpen.lpstrDeviceType  = (LPCTSTR) MCI_ALL_DEVICE_ID;
		mciOpen.lpstrElementName = tpath;

		mciError = mciSendCommand ( 0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD) &mciOpen );
		KD_FREE_TSTR ( tpath );

		if ( !mciError )
		{
			sound->device = (KDvoid*) mciOpen.wDeviceID;
		}
		else
		{
			nRet = -1;
		}
	}
	
#endif

	return nRet;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_CLOSE, 0, 0 ) )
		{
			nRet = -1;
		}
	}
#endif

	return nRet;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_PLAY_PARMS   mciPlay = {0};

		mciPlay.dwCallback = (DWORD_PTR) l_wnd;
		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_PLAY, MCI_NOTIFY, (DWORD) &mciPlay ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		TCHAR*  tpath;

		XM_SET_TSTR ( tpath, sound->fpath );
		nRet = sndPlaySound ( tpath, SND_ASYNC || ( sound->repeat ? SND_LOOP : 0 ) ) ? 0 : -1;
		KD_FREE_TSTR ( tpath );
	}

	return nRet;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		mciSendCommand ( (MCIDEVICEID) sound->device, MCI_STOP, 0, 0 );
		nRet = mciSendCommand ( (MCIDEVICEID) sound->device, MCI_SEEK, MCI_SEEK_TO_START, 0 );
		if ( nRet )
		{
			nRet = -1;
		}
	}
#endif

	return nRet;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_PAUSE, 0, 0 ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		nRet = - 1;
	}

	return nRet;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_RESUME, 0, 0 ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		nRet = - 1;
	}

	return nRet;
}

#ifdef XM_HAS_PLATFORM_SOUND_REPEAT
KDint xmPlatformSoundSetRepeat ( XMSound* sound, KDint on )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		DWORD  dwFlags = MCI_NOTIFY;
		if ( on )
		{
			dwFlags |= MCI_DGV_PLAY_REPEAT;
		}

		if ( mciSendCommand ( (MCIDEVICEID) sound->device, dwFlags, 0, 0 ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		nRet = - 1;
	}

	return nRet;
}
#endif

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_DGV_SETAUDIO_PARMS  param; 

		param.dwCallback		= 0; 
		param.dwItem			= MCI_DGV_SETAUDIO_VOLUME; 
		param.dwValue			= (DWORD) ( volume * 1000 ); 
		param.dwOver			= 0; 
		param.lpstrAlgorithm	= NULL; 
		param.lpstrQuality		= NULL; 

		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID) &param ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		nRet = - 1;
	}

	return nRet;
}

KDfloat xmPlatformSoundGetVolume ( XMSound* sound )
{
	KDfloat  fRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_STATUS_PARMS  param; 

		param.dwCallback = 0; 
		param.dwItem     = MCI_DGV_STATUS_VOLUME; 

		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID) &param ) )
		{
			fRet = -1;
		}
		else
		{
			fRet = param.dwReturn / 1000.f;  // Volume: 0 - 1000 
		}
	}
	else
#endif
	{
		fRet = -1;
	}

	return fRet;
}

KDint xmPlatformSoundSetPitch ( XMSound* sound, KDfloat pitch )
{
	KDint  nRet = - 1;

	return nRet;
}

KDfloat xmPlatformSoundGetPitch ( XMSound* sound )
{
	KDfloat  fRet = -1;

	return fRet;
}

KDint xmPlatformSoundSetPan ( XMSound* sound, KDfloat pan )
{
	KDint  nRet = - 1;

	return nRet;
}

KDfloat xmPlatformSoundGetPan ( XMSound* sound )
{
	KDfloat  fRet = -1;

	return fRet;
}



KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_STATUS_PARMS  param; 

		param.dwItem = MCI_STATUS_LENGTH; 

		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID) &param ) )
		{
			nRet = -1;
		}
		else
		{
			nRet = (KDint) param.dwReturn;
		}
	}
	else
#endif
	{
		nRet = -1;
	}

	return nRet;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_STATUS_PARMS  param; 

		param.dwItem = MCI_STATUS_POSITION; 

		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID) &param ) )
		{
			nRet = -1;
		}
		else
		{
			nRet = (KDint) param.dwReturn;
		}
	}
	else
#endif
	{
		nRet = -1;
	}

	return nRet;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint pos )
{
	KDint  nRet = 0;

#ifndef _WIN32_WCE
	if ( !( sound->mode & XM_SOUND_EFFECT_WAV ) )
	{
		MCI_SEEK_PARMS  param;

		param.dwTo = (DWORD) pos;
		
		if ( mciSendCommand ( (MCIDEVICEID) sound->device, MCI_SEEK, MCI_TO|MCI_WAIT, (DWORD)(LPVOID) &param ) )
		{
			nRet = -1;
		}
	}
	else
#endif
	{
		nRet = -1;
	}

	return nRet;
}

// xmGPSReset : Reset GPS ( only for Win32 platform )
KD_API KDvoid KD_APIENTRY xmGPSReset ( KDint mode )
{
	KDchar  temp [ 256 ] = "";
	KDchar  chk  [ 256 ] = "";

	KDuint  i;
	KDint   checksum;

	switch ( mode )
	{
		case XM_GPS_RESET_HOT  : kdStrcpy ( temp, "$PSRF101,-2686700,-4304200,3851624,96000,497260,921,12,1" ); break;
		case XM_GPS_RESET_WARM : kdStrcpy ( temp, "$PSRF101,-2686700,-4304200,3851624,96000,497260,921,12,2" ); break;
		case XM_GPS_RESET_COLD : kdStrcpy ( temp, "$PSRF101,-2686700,-4304200,3851624,96000,497260,921,12,4" ); break;

		default : 

			return;
	}

	// checksum
	checksum = 0;
	for ( i = 1; i < kdStrlen ( temp ); i++ ) 
	{
		checksum = checksum ^ temp [ i ];   
	}
	kdSprintfKHR ( chk, "*%02x\r\n", checksum );
	kdStrcat ( temp, chk );

	if ( l_gps )
	{
		DWORD   bytes;

		kdThreadSemWait ( l_gps_lock );

		WriteFile ( l_gps, (BYTE*) temp, kdStrlen ( temp ), &bytes, NULL );
		if ( bytes )
		{
			Sleep ( 1000 );
		}

		kdThreadSemPost ( l_gps_lock );
	}
}

KD_API XMAREM KD_APIENTRY xmGetAREM ( KDvoid )
{
	return l_wnd;
}