/* --------------------------------------------------------------------------
 *
 *      File            xm_kode.c
 *      Description     XMKode APIs ( Windowing, Application Entrance )
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
#include "platform.h"

       KDdouble                     g_interval			= 0;
	   KDint                        g_width				= 0;
	   KDint                        g_height			= 0;
       KDfloat                      g_scale				= 1;
       KDint					    g_provider			= 0;
static KDuint                       l_frame_count		= 0;
static KDuint                       l_time_elapsed		= 0;
static KDuint                       l_time_old			= 0;
static KDdouble                     l_fps				= 0;
static KDuint                       l_msec				= 0;

static XMWindow                     l_window			= 0;
static KDmode                       l_mode				= 0;
static KDCallbackFunc*              l_event				= 0;

static KDbool                       l_launch			= KD_FALSE;
static KDbool                       l_visible			= KD_FALSE;
static KDbool                       l_focus				= KD_FALSE;

static KDTouch                      l_touches			[ KD_TOUCH_MAX ];
static KDuint                       l_touches_cnt		= 0;

static const KDchar*                l_szApnsMessage		= KD_NULL;

#ifdef XM_HAS_VIRTUAL_ACCELEROMETER
static KDfloat						l_accelX			=  0.0f;
static KDfloat						l_accelY			=  0.0f;
static KDfloat						l_accelZ			=  0.0f;
#endif
static const KDfloat				l_accelerationStep	=  0.1f;
static const KDfloat				l_minAcceleration	= -1.0f;
static const KDfloat				l_maxAcceleration	=  1.0f;

#ifndef XM_HAS_PLATFORM_EGL

static EGLNativeWindowType          l_native_window		= 0;
static EGLNativeDisplayType         l_native_display	= 0;

static EGLDisplay                   l_egl_display		= 0;
static EGLConfig                    l_egl_config		= 0;
static EGLSurface                   l_egl_surface		= 0;
static EGLContext                   l_egl_context		= 0;

static KDvoid xmEGLInit ( KDvoid )
{
	KDuint      i       = 0;
	EGLint      major   = 0;
	EGLint      minor   = 0;
	EGLint      configs = 0;
	EGLint      color_buffer[4];
	EGLint      config_attrs[32];
	EGLint      context_attrs[3];

	kdLogMessage ( "\n* EGL intializing.\n\n" );

	if ( l_mode & XM_SYS_RGB565 )
	{
		color_buffer [ 0 ] = 5;
		color_buffer [ 1 ] = 6;
		color_buffer [ 2 ] = 5;
		color_buffer [ 3 ] = 0;
	}
	else
	{
		color_buffer [ 0 ] = 8;
		color_buffer [ 1 ] = 8;
		color_buffer [ 2 ] = 8;
		color_buffer [ 3 ] = 8;
	}

	i = 0;
	config_attrs[i++] = EGL_RED_SIZE;
	config_attrs[i++] = color_buffer[ 0 ];
	config_attrs[i++] = EGL_GREEN_SIZE;
	config_attrs[i++] = color_buffer[ 1 ];
	config_attrs[i++] = EGL_BLUE_SIZE;
	config_attrs[i++] = color_buffer[ 2 ];
	config_attrs[i++] = EGL_ALPHA_SIZE;
	config_attrs[i++] = color_buffer[ 3 ];
	config_attrs[i++] = EGL_DEPTH_SIZE;
	config_attrs[i++] = 16;

#if !defined ( Linux ) && !defined ( SHP )

	if ( l_mode & XM_SYS_MSAA )
	{
		config_attrs[i++] = EGL_SAMPLE_BUFFERS;
		config_attrs[i++] = 1;
		config_attrs[i++] = EGL_SAMPLES;
		config_attrs[i++] = 2;
	}

#endif

	config_attrs[i++] = EGL_SURFACE_TYPE;
	config_attrs[i++] = EGL_WINDOW_BIT;

#if  !defined ( Platform_Nexus_s3c6410 ) && !defined ( Platform_SiRFPrima_EVB_CE6 )

	config_attrs[i++] = EGL_RENDERABLE_TYPE;
	config_attrs[i++] = l_mode & XM_SYS_GLES_V2 ? EGL_OPENGL_ES2_BIT : EGL_OPENGL_ES_BIT;

#endif

	config_attrs[i++] = EGL_NONE;

	context_attrs[0] = EGL_CONTEXT_CLIENT_VERSION;	
	context_attrs[1] = l_mode & XM_SYS_GLES_V2 ? 2 : 1;
	context_attrs[2] = EGL_NONE;

#ifndef WINCE
	eglBindAPI ( EGL_OPENGL_ES_API );
#endif
	l_egl_display = eglGetDisplay ( l_native_display );
	if ( l_egl_display == EGL_NO_DISPLAY )
	{
		kdAssertion ( "eglGetDisplay ( ) failed." );
	}

	if ( !eglInitialize ( l_egl_display, &major, &minor ) )
	{
		kdAssertion ( "eglInitialize ( ) failed." );
	}

	if ( !eglChooseConfig ( l_egl_display, config_attrs, &l_egl_config, 1, &configs ) || ( configs != 1 ) )
    {
		kdAssertion ( "eglChooseConfig ( ) failed." );
        return ;
    }

    l_egl_context = eglCreateContext ( l_egl_display, l_egl_config, 0,
#if defined ( Platform_SiRFPrima_EVB_CE6 )
		0
#else
		context_attrs 
#endif
	);
 	if ( eglGetError ( ) != EGL_SUCCESS )
	{
		kdAssertion ( "eglCreateContext ( ) failed." );
	}

	l_egl_surface = eglCreateWindowSurface ( l_egl_display, l_egl_config, l_native_window, 0 );
	if ( eglGetError ( ) != EGL_SUCCESS )
	{
		kdAssertion ( "eglCreateWindowSurface ( ) failed." );
	}

	eglMakeCurrent ( l_egl_display, l_egl_surface, l_egl_surface, l_egl_context );
	if ( eglGetError ( ) != EGL_SUCCESS )
	{
		kdAssertion ( "eglMakeCurrent ( ) failed." );
	}
}

#endif

KDEvent* _xmEventNew  ( KDvoid )
{
	KDEvent*  event = kdCreateEvent ( );
	kdAssert ( event );
	return event;
}

KDvoid _xmEventSend ( const KDEvent* event )
{
	l_event ( event );
	kdDefaultEvent ( event );
}

KDvoid _xmEventCreate ( XMWindow window )
{	
	l_window   = window;
	
	l_launch   = KD_FALSE;
	l_visible  = KD_FALSE;
	l_focus    = KD_FALSE;
	g_width    = 0;
	g_height   = 0;

	kdMemset ( l_touches, 0, sizeof ( KDTouch ) * KD_TOUCH_MAX );
	l_touches_cnt = 0;

	#ifndef XM_HAS_PLATFORM_EGL

	xmPlatformGetEGLNative ( &l_native_window, &l_native_display );

	if ( ( l_mode & XM_SYS_GLES_V1 ) || ( l_mode & XM_SYS_GLES_V2 ) )
	{
		xmEGLInit ( );
	}

	#endif

	kdLogMessage ( "\n\n" );

    l_msec = KD_GET_UST2MSEC;
}

KDvoid _xmEventDestroy ( KDvoid )
{
	KDEvent*  event = KD_NULL;
	
	if ( l_launch == KD_FALSE )
	{
		return;
	}

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessage ( "KD_EVENT_DESTROY" );
	}

    xmEventDetach ( XM_SYS_MAGNETOMETER );
    xmEventDetach ( XM_SYS_ACCELEROMETER );
    
	l_launch = KD_FALSE;

	event = _xmEventNew ( );
	event->type = KD_EVENT_DESTROY;

	_xmEventSend ( event );

	#ifndef XM_HAS_PLATFORM_EGL
	if ( ( l_mode & XM_SYS_GLES_V1 ) || ( l_mode & XM_SYS_GLES_V2 ) )
	{
		kdLogMessage ( "\n* EGL terninating.\n\n" );

	#if !defined ( Linux )

		eglSwapBuffers ( l_egl_display, l_egl_surface );
		eglMakeCurrent ( l_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
		eglTerminate ( l_egl_display );

	#endif

	}
	#endif

	kdExit ( 0 );
}

KDvoid _xmEventRedraw ( KDvoid )
{
	KDEvent*  event = KD_NULL;

	KDuint  time_delta = 0;
	KDuint  time_new   = 0;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	if ( l_mode & XM_SYS_LOG )
	{
		//kdLogMessage ( "KD_EVENT_REDRAW" );
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_REDRAW;
	
	_xmEventSend ( event );

	#ifndef XM_HAS_PLATFORM_EGL
	if ( ( l_mode & XM_SYS_GLES_V1 ) || ( l_mode & XM_SYS_GLES_V2 ) )
	{
		eglSwapBuffers ( l_egl_display, l_egl_surface );
	}
	#endif

	time_new   = kdGetTickCount ( );
	time_delta = time_new - l_time_old;
	l_time_old = time_new;

	l_frame_count++;
	l_time_elapsed += time_delta;

	if ( l_time_elapsed >= 1000 )
	{
		l_fps = 1000.0 * l_frame_count / l_time_elapsed;
		l_time_elapsed = 0;
		l_frame_count  = 0;
	}
}

KDvoid _xmEventUpdate ( KDvoid )
{
	KDEvent*  event = KD_NULL;
    KDuint    msec  = 0;
	KDint     i;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	// For Notify NMEA State chaged.
	for ( i = 0; i < 2; i++ )
	{
		if ( xmIsNeededUpdateNMEA ( i == 0 ? KD_TRUE : KD_FALSE ) )
		{
			if ( l_mode & XM_SYS_LOG )
			{
				//kdLogMessagefKHR ( "KD_EVENT_LOCATION        : from = %s", i == 0 ? "GPS" : "User" );
			}

			event = _xmEventNew ( );
			event->type = KD_EVENT_LOCATION;
			event->data.value.i = i == 0 ? KD_NMEA_UPDATED_GPS : KD_NMEA_UPDATED_USER;

			_xmEventSend ( event );
		}
	}
	
    msec = KD_GET_UST2MSEC - l_msec;

#ifdef XM_HAS_VIRTUAL_ACCELEROMETER
	if ( l_mode & XM_SYS_ACCELEROMETER )
	{
		static KDuint old_msec;
		       KDuint new_msec = msec;
			   KDuint interval = 1000 / 10;
				
		if ( ( new_msec - old_msec ) >= interval || new_msec < old_msec )		
		{
			old_msec = new_msec;	

			_xmEventAccelerometer ( l_accelX, l_accelY, l_accelZ );
		}
	}
#endif

	if ( l_mode & XM_SYS_LOG )
	{
		//kdLogMessagefKHR ( "KD_EVENT_UPDATE          : msec = %d", msec );
	}
    
	event = _xmEventNew ( );
	event->type = KD_EVENT_UPDATE;
	event->data.update.msec = msec;

	_xmEventSend ( event );
}

KDvoid _xmEventResize ( KDint width, KDint height )
{
	KDEvent*  event = KD_NULL;

	g_width  = width;
	g_height = height;

	if ( !l_launch )
	{
		l_launch = KD_TRUE;

		if ( l_mode & XM_SYS_LOG )
		{
			kdLogMessagefKHR ( "KD_EVENT_CREATE          : width = %d, height = %d", width, height );
		}

		event = _xmEventNew ( );
		event->type = KD_EVENT_CREATE;
		event->data.size.width  = width;
		event->data.size.height = height;

		_xmEventSend ( event );

		_xmEventVisible ( 1 );
		_xmEventFocus ( 1 );

		xmEventAttach ( l_mode );
	}
	else
	{
		if ( l_mode & XM_SYS_LOG )
		{
			kdLogMessagefKHR ( "KD_EVENT_RESIZE          : width = %d, height = %d", width, height );
		}

		event = _xmEventNew ( );
		event->type = KD_EVENT_RESIZE;
		event->data.size.width  = width;
		event->data.size.height = height;

		_xmEventSend ( event );
	}
}

static KDvoid _xmEventTouchFail ( KDvoid )
{
	KDEvent*  event = KD_NULL;
	KDTouch   touches_post [ KD_TOUCH_MAX ];
	KDuint    i, post_cnt = 0;

	for ( i = 0; i < KD_TOUCH_MAX; i++ )
	{
		if ( l_touches [ i ].id != 0 )
		{
			touches_post [ i ] = l_touches [ i ];
			touches_post [ i ].timestamp = kdGetTimeUST ( );

			kdMemset ( &l_touches [ i ], 0, sizeof ( KDTouch ) );

			if ( l_mode & XM_SYS_LOG )
			{
				kdLogMessagefKHR ( "%s id = %p, x = %3d, y = %3d", 
					post_cnt == 0 ? "KD_EVENT_TOUCH_CANCELLED :" : "                          ",
					touches_post [ i ].id, touches_post [ i ].x, touches_post [ i ].y );
			}

			post_cnt++;
		}
	}

	if ( post_cnt )
	{
		event = _xmEventNew ( );
		event->type = KD_EVENT_TOUCH_CANCELLED;
		event->data.touch.touches = touches_post;
		event->data.touch.count = post_cnt;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventTouchBegan ( KDTouch* touches, KDuint count )
{
	KDEvent*  event = KD_NULL;
	KDTouch   touches_post [ KD_TOUCH_MAX ];
	KDuint    i, j, post_cnt = 0;
	
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		kdMemset ( l_touches, 0, sizeof ( KDTouch ) * KD_TOUCH_MAX );
		l_touches_cnt = 0;
		return;
	}

	for ( i = 0, j = 0; i < count; i++ )
	{
		if ( l_mode & XM_SYS_QUIT )
		{
			if ( ( ( g_width - 64 ) < touches [ i ].x ) && ( touches [ i ].y < 64 ) )
			{
				kdLogMessage ( "KD_EVENT_SYS_QUIT" );

				_xmEventDestroy ( );				

				return;
			}
		}

		if      ( touches [ i ].x < 0        ) { touches [ i ].x = 0;        }
		else if ( touches [ i ].x > g_width  ) { touches [ i ].x = g_width;  }

		if      ( touches [ i ].y < 0        ) { touches [ i ].y = 0;        }
		else if ( touches [ i ].y > g_height ) { touches [ i ].y = g_height; }

		touches [ i ].y = g_height - touches [ i ].y;

		for ( ; j < KD_TOUCH_MAX; j++ )
		{
			if ( touches [ i ].id == l_touches [ j ].id )
			{
				kdLogMessagefKHR ( "KD_EVENT_TOUCH_BEGAN     : collided touch id %p", touches [ i ].id );
				_xmEventTouchFail ( );
				return;
			}

			if ( j > l_touches_cnt )
			{
				break;
			}

			if ( l_touches [ j ].id == KD_NULL )
			{
                l_touches [ j ].id     = touches [ i ].id;
                
                l_touches [ j ].prev_x = touches [ i ].x;
                l_touches [ j ].prev_y = touches [ i ].y;
                
                l_touches [ j ].x      = touches [ i ].x;
                l_touches [ j ].y      = touches [ i ].y;
                
                l_touches [ j ].diff_x = 0;
                l_touches [ j ].diff_y = 0;
                
				l_touches [ j ].timestamp = kdGetTimeUST ( );
				l_touches_cnt++;	
				
				if ( l_mode & XM_SYS_LOG )
				{
					kdLogMessagefKHR ( "%s id = %p, x = %3d, y = %3d", 
						post_cnt == 0 ? "KD_EVENT_TOUCH_BEGAN     :" : "                          ",
						touches [ i ].id, touches [ i ].x, touches [ i ].y );
				}

				touches_post [ post_cnt ] = l_touches [ j ];
				post_cnt++;

				break;
			}
		}

		if ( j == KD_TOUCH_MAX )
		{
			kdLogMessagefKHR ( "KD_EVENT_TOUCH_BEGAN     : limited touch number less than %d", KD_TOUCH_MAX );
			_xmEventTouchFail ( );
			return;
		}
	}

	if ( post_cnt )
	{
		event = _xmEventNew ( );
		event->type = KD_EVENT_TOUCH_BEGAN;
		event->data.touch.touches = touches_post;
		event->data.touch.count = post_cnt;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventTouchMoved ( KDTouch* touches, KDuint count )
{
	KDEvent*  event = KD_NULL;
	KDTouch   touches_post [ KD_TOUCH_MAX ];
	KDuint    i, j, post_cnt = 0;
	
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	for ( i = 0; i < count; i++ )
	{
		if      ( touches [ i ].x < 0        ) { touches [ i ].x = 0;        }
		else if ( touches [ i ].x > g_width  ) { touches [ i ].x = g_width;  }

		if      ( touches [ i ].y < 0        ) { touches [ i ].y = 0;        }
		else if ( touches [ i ].y > g_height ) { touches [ i ].y = g_height; }

		touches [ i ].y = g_height - touches [ i ].y;

		for ( j = 0; j < l_touches_cnt + 1; j++ )
		{
			if ( touches [ i ].id == l_touches [ j ].id )
			{
				if ( ( touches [ i ].x != l_touches [ j ].x ) || ( touches [ i ].y != l_touches [ j ].y ) )
				{
                    l_touches [ j ].prev_x = l_touches [ j ].x;
                    l_touches [ j ].prev_y = l_touches [ j ].y;
                    
                    l_touches [ j ].x      = touches [ i ].x;
                    l_touches [ j ].y      = touches [ i ].y;
                    
                    l_touches [ j ].diff_x = l_touches [ j ].x - l_touches [ j ].prev_x;
                    l_touches [ j ].diff_y = l_touches [ j ].y - l_touches [ j ].prev_y;                                        
                    
					l_touches [ j ].timestamp = kdGetTimeUST ( );

					if ( l_mode & XM_SYS_LOG )
					{
						kdLogMessagefKHR ( "%s id = %p, x = %3d, y = %3d", 
							post_cnt == 0 ? "KD_EVENT_TOUCH_MOVED     :" : "                          ",
							touches [ i ].id, touches [ i ].x, touches [ i ].y );
					}

					touches_post [ post_cnt ] = l_touches [ j ];
					post_cnt++;
				}

				break;
			}
		}

		if ( j == l_touches_cnt + 1 )
		{
			kdLogMessagefKHR ( "KD_EVENT_TOUCH_MOVED     : invalid touch id %p", touches [ i ].id );
			_xmEventTouchFail ( );
			return;
		}
	}

	if ( post_cnt )
	{
		event = _xmEventNew ( );
		event->type = KD_EVENT_TOUCH_MOVED;
		event->data.touch.touches = touches_post;
		event->data.touch.count = post_cnt;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventTouchEnded ( KDTouch* touches, KDuint count )
{
	KDEvent*  event = KD_NULL;
	KDTouch  touches_post [ KD_TOUCH_MAX ];
	KDuint   i, j, post_cnt = 0;
	KDbool   found = KD_FALSE;
	
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	for ( i = 0; i < count; i++ )
	{
		if      ( touches [ i ].x < 0        ) { touches [ i ].x = 0;        }
		else if ( touches [ i ].x > g_width  ) { touches [ i ].x = g_width;  }

		if      ( touches [ i ].y < 0        ) { touches [ i ].y = 0;        }
		else if ( touches [ i ].y > g_height ) { touches [ i ].y = g_height; }

		touches [ i ].y = g_height - touches [ i ].y;

		for ( j = 0; j < l_touches_cnt + 1; j++ )
		{
			if ( touches [ i ].id == l_touches [ j ].id )
			{
				found = KD_TRUE;

                l_touches [ j ].prev_x = l_touches [ j ].x;
                l_touches [ j ].prev_y = l_touches [ j ].y;
                
                l_touches [ j ].x      = touches [ i ].x;
                l_touches [ j ].y      = touches [ i ].y;
                
                l_touches [ j ].diff_x = l_touches [ j ].x - l_touches [ j ].prev_x;
                l_touches [ j ].diff_y = l_touches [ j ].y - l_touches [ j ].prev_y;
                
				l_touches [ j ].timestamp = kdGetTimeUST ( );

				if ( l_mode & XM_SYS_LOG )
				{
					kdLogMessagefKHR ( "%s id = %p, x = %3d, y = %3d", 
						post_cnt == 0 ? "KD_EVENT_TOUCH_ENDED     :" : "                          ",
						touches [ i ].id, touches [ i ].x, touches [ i ].y );
				}

				touches_post [ post_cnt ] = l_touches [ j ];
				post_cnt++;

				if ( l_touches_cnt > 1 )
				{
					l_touches [ j ] = l_touches [ l_touches_cnt - 1 ];
				}
				kdMemset ( &l_touches [ l_touches_cnt - 1 ], 0, sizeof ( KDTouch ) );
				l_touches_cnt--;

				break;
			}
		}

		if ( found == KD_FALSE )
		{
			kdLogMessagefKHR ( "KD_EVENT_TOUCH_ENDED     : invalid touch id %p", touches [ i ].id );
			_xmEventTouchFail ( );
			return;
		}
	}

	if ( post_cnt )
	{
		event = _xmEventNew ( );
		event->type = KD_EVENT_TOUCH_ENDED;
		event->data.touch.touches = touches_post;
		event->data.touch.count = post_cnt;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventTouchCancelled ( KDTouch* touches, KDuint count )
{
	KDEvent*  event = KD_NULL;
	KDTouch   touches_post [ KD_TOUCH_MAX ];
	KDuint    i, j, post_cnt = 0;
	KDbool    found = KD_FALSE;
	
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	for ( i = 0; i < count; i++ )
	{
		if      ( touches [ i ].x < 0        ) { touches [ i ].x = 0;        }
		else if ( touches [ i ].x > g_width  ) { touches [ i ].x = g_width;  }

		if      ( touches [ i ].y < 0        ) { touches [ i ].y = 0;        }
		else if ( touches [ i ].y > g_height ) { touches [ i ].y = g_height; }

		touches [ i ].y = g_height - touches [ i ].y;

		for ( j = 0; j < l_touches_cnt + 1; j++ )
		{
			if ( touches [ i ].id == l_touches [ j ].id )
			{
				found = KD_TRUE;

                l_touches [ j ].prev_x = l_touches [ j ].x;
                l_touches [ j ].prev_y = l_touches [ j ].y;
                
                l_touches [ j ].x      = touches [ i ].x;
                l_touches [ j ].y      = touches [ i ].y;
                
                l_touches [ j ].diff_x = l_touches [ j ].x - l_touches [ j ].prev_x;
                l_touches [ j ].diff_y = l_touches [ j ].y - l_touches [ j ].prev_y;
                
				l_touches [ j ].timestamp = kdGetTimeUST ( );

				if ( l_mode & XM_SYS_LOG )
				{
					kdLogMessagefKHR ( "%s id = %p, x = %3d, y = %3d", 
						post_cnt == 0 ? "KD_EVENT_TOUCH_CANCELLED :" : "                          ",
						touches [ i ].id, touches [ i ].x, touches [ i ].y );
				}

				touches_post [ post_cnt ] = l_touches [ j ];
				post_cnt++;

				if ( l_touches_cnt > 1 )
				{
					l_touches [ j ] = l_touches [ l_touches_cnt - 1 ];
				}
				kdMemset ( &l_touches [ l_touches_cnt - 1 ], 0, sizeof ( KDTouch ) );
				l_touches_cnt--;

				break;
			}
		}

		if ( found == KD_FALSE )
		{
			kdLogMessagefKHR ( "KD_EVENT_TOUCH_CANCELLED : invalid touch id %p", touches [ i ].id );
			_xmEventTouchFail ( );
			return;
		}
	}

	if ( post_cnt )
	{
		event = _xmEventNew ( );
		event->type = KD_EVENT_TOUCH_CANCELLED;
		event->data.touch.touches = touches_post;
		event->data.touch.count = post_cnt;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventMousePressed ( KDint x, KDint y, KDint code )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_MOUSE_PRESSED;
	event->data.mouse.x = x;
	event->data.mouse.y = y;
	event->data.mouse.code = code;

	_xmEventSend ( event );
}

KDvoid _xmEventMouseReleased ( KDint x, KDint y, KDint code )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_MOUSE_RELEASED;
	event->data.mouse.x = x;
	event->data.mouse.y = y;
	event->data.mouse.code = code;

	_xmEventSend ( event );
}

KDvoid _xmEventMouseMoved ( KDint x, KDint y )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_MOUSE_MOVED;
	event->data.mouse.x = x;
	event->data.mouse.y = g_height - y;
	event->data.mouse.code = -1;

	_xmEventSend ( event );
}

KDvoid _xmEventMouseScrolled ( KDint x, KDint y )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_MOUSE_SCROLLED;
	event->data.mouse.x = x;
	event->data.mouse.y = y;
	event->data.mouse.code = -1;

	_xmEventSend ( event );
}

KDvoid _xmEventKeyReleased ( KDint win_code )
{
	KDEvent*  event = KD_NULL;
	KDint     kd_code = 0;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}

#ifdef XM_HAS_PLATFORM_KEYPAD
	kd_code = xmPlatformGetKeyCode ( win_code );
#else
    kd_code = win_code;
#endif
    
	if ( kd_code == 0 )
	{
		return;
	}

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_KEY_RELEASED    : %s", xmGetKeyName ( kd_code ) );
	}

#ifdef XM_HAS_VIRTUAL_ACCELEROMETER
	if ( l_mode & XM_SYS_ACCELEROMETER )
	{
		switch ( kd_code )
		{
			case KD_INPUT_GAMEKEYS_LEFT		:	l_accelX = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelX - l_accelerationStep );	break;
			case KD_INPUT_GAMEKEYS_RIGHT	:	l_accelX = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelX + l_accelerationStep );	break;
			case KD_INPUT_GAMEKEYS_UP		:	l_accelY = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelY + l_accelerationStep );	break;
			case KD_INPUT_GAMEKEYS_DOWN		:	l_accelY = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelY - l_accelerationStep );	break;
			case KD_INPUT_XMSOFT_PERIOD		:	l_accelZ = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelZ + l_accelerationStep );	break;
			case KD_INPUT_XMSOFT_COMMA		:	l_accelZ = KD_CLAMP ( l_minAcceleration, l_maxAcceleration, l_accelZ - l_accelerationStep );	break;
			case KD_INPUT_GAMEKEYS_FIRE		:	
				l_accelX = 0;
				l_accelY = 0;
				l_accelZ = 0;	
				break;
		}
	}
#endif

	event = _xmEventNew ( );
	event->type = KD_EVENT_KEY_RELEASED;
	event->data.keypad.code = kd_code;

	_xmEventSend ( event );

	if ( kd_code & KD_IOGROUP_GAMEKEYS )
	{
		kd_code += 0x0100;

		if ( l_mode & XM_SYS_LOG )
		{
			kdLogMessagefKHR ( "KD_EVENT_KEY_RELEASED    : %s", xmGetKeyName ( kd_code ) );
		}

		event = _xmEventNew ( );
		event->type = KD_EVENT_KEY_RELEASED;
		event->data.keypad.code = kd_code;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventKeyPressed ( KDint win_code )
{
	KDEvent*  event = KD_NULL;
	KDint     kd_code = 0;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}
    
#ifdef XM_HAS_PLATFORM_KEYPAD
	kd_code = xmPlatformGetKeyCode ( win_code );
#else
    kd_code = win_code;
#endif
    
	if ( kd_code == 0 )
	{
		return;
	}

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_KEY_PRESSED     : %s", xmGetKeyName ( kd_code ) );
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_KEY_PRESSED;
	event->data.keypad.code = kd_code;

	_xmEventSend ( event );

	if ( kd_code & KD_IOGROUP_GAMEKEYS )
	{
		kd_code += 0x0100;

		if ( l_mode & XM_SYS_LOG )
		{
			kdLogMessagefKHR ( "KD_EVENT_KEY_PRESSED     : %s", xmGetKeyName ( kd_code ) );
		}

		event = _xmEventNew ( );
		event->type = KD_EVENT_KEY_PRESSED;
		event->data.keypad.code = kd_code;

		_xmEventSend ( event );
	}
}

KDvoid _xmEventMagnetometer ( KDEventMagnetometer magnetometer )
{
	KDEvent*  event = KD_NULL;
    
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	
    
	if ( l_mode & XM_SYS_LOG )
	{
//       kdLogMessagefKHR ( "KD_EVENT_MAGNETOMETER    : azimuth = %.2f, pitch = %.2f, roll = %.2f, x = %.2f, y = %.2f, z = %.2f", 
//                          magnetometer.azimuth, magnetometer.pitch, magnetometer.roll, magnetometer.x, magnetometer.y, magnetometer.z );
	}
    
    event = _xmEventNew ( );
    event->type = KD_EVENT_MAGNETOMETER;
    event->data.magnetometer = magnetometer;
    
    _xmEventSend ( event );    
}

KDvoid _xmEventAccelerometer ( KDfloat x, KDfloat y, KDfloat z )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	

	if ( l_mode & XM_SYS_LOG )
	{
//		kdLogMessagefKHR ( "KD_EVENT_ACCELEROMETER   : x = %.2f, y = %.2f, z = %.2f", x, y, z );
	}
	
	event = _xmEventNew ( );
	event->type = KD_EVENT_ACCELEROMETER;
	event->data.accelerometer.x = x;
	event->data.accelerometer.y = y;
	event->data.accelerometer.z = z;

	_xmEventSend ( event );
}

KDvoid  _xmEventInsertText ( const KDchar* text, KDsize len )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	

	event = _xmEventNew ( );
	event->type = KD_EVENT_INSERT_TEXT;
	kdStrncpy_s ( event->data.insert.utf8, 8, text, len );

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_INSERT_TEXT     : %s", event->data.insert.utf8 );
	}

	_xmEventSend ( event );
}

KDvoid _xmEventFocus ( KDbool state )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE )
	{
		l_focus = KD_FALSE;

		return;
	}

	if ( l_focus == state )
	{
		return;
	}

	l_focus = state;

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_FOCUS           : state = %d", state );
	}

	event = _xmEventNew ( );
	event->type = KD_EVENT_FOCUS;
	event->data.value.i = state;

	_xmEventSend ( event );
}

KDvoid _xmEventProvider ( KDint provider )
{
	KDEvent*  event = KD_NULL;

	g_provider = provider;

	if ( l_launch == KD_FALSE )
	{
		return;
	}

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_PROVIDER        : provider = %d", provider );
	}	

	event = _xmEventNew ( );
	event->type = KD_EVENT_PROVIDER;
	event->data.value.i = provider;

	_xmEventSend ( event );
}

KDvoid _xmEventVisible ( KDbool state )
{
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE )
	{
		l_visible = KD_FALSE;

		return;
	}

	if ( l_visible == state )
	{
		return;
	}

	l_visible = state;

	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_VISIBLE         : state = %d", state );
	}	

	event = _xmEventNew ( );
	event->type = KD_EVENT_VISIBLE;
	event->data.value.i = state;

	_xmEventSend ( event );

	xmSoundAutoPause ( state );
}


KDvoid _xmEventMemoryWarning ( KDvoid )
{
    KDEvent*  event = KD_NULL;
    
	if ( l_launch == KD_FALSE )
	{
		return;
	}
    
	if ( l_mode & XM_SYS_LOG )
	{
        kdLogMessagefKHR ( "KD_EVENT_MEMORY_WARNING" );
	}
    
	event = _xmEventNew ( );
	event->type = KD_EVENT_MEMORY_WARNING;
    
	_xmEventSend ( event );
}

KDvoid _xmEventCompleted ( KDint type, KDvoid* userptr )
{
    KDEvent*  event = KD_NULL;
    
	if ( l_launch == KD_FALSE )
	{
		return;
	}
    
	if ( l_mode & XM_SYS_LOG )
	{
		kdLogMessagefKHR ( "KD_EVENT_COMPLETED       : type = %d, userptr = %p", type, userptr );
	}
    
	event = _xmEventNew ( );
	event->type = KD_EVENT_COMPLETED;
	event->data.completed.type = type;
	event->data.completed.userptr = userptr;
    
	_xmEventSend ( event );
}

KDvoid _xmEventImeNotification ( KDint status, KDRect begin, KDRect end, KDfloat duration )
{
	KDEvent*  event = KD_NULL;
    
	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}
    
	if ( l_mode & XM_SYS_LOG )
	{
        kdLogMessagefKHR ( "KD_EVENT_IME_NOTIFICATION : status = %d, begin ( %.2f, %.2f, %.2f, %.2f ), end ( %.2f, %.2f, %.2f, %.2f ), duration = %.2f",
                          status, begin.x, begin.y, begin.w, begin.h, end.x, end.y, end.w, end.h, duration );
	}
	
	event = _xmEventNew ( );
	event->type = KD_EVENT_IME_NOTIFICATION;
    event->data.imenotify.status    = status;
    event->data.imenotify.begin     = begin;
    event->data.imenotify.end       = end;
    event->data.imenotify.duration  = duration;
    
	_xmEventSend ( event );
}

/* CS Smart Game event */
KDvoid _xmEventPointing ( KDint x, KDint y )
{	
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	

	event = _xmEventNew ( );

	event->type = KD_EVENT_POINTING;
	event->data.pointing.x = x;
	event->data.pointing.y = y;

	if ( l_mode & XM_SYS_LOG )
	{
//		kdLogMessagefKHR ( "KD_EVENT_POINTING        : %.2f, %.2f", x, y );
	}

	_xmEventSend ( event );
}

KDvoid _xmEventQuaternion ( KDint q1, KDint q2, KDint q3, KDint q4 )
{	
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	

	event = _xmEventNew ( );

	event->type = KD_EVENT_QUATERNION;
	event->data.quaternion.q1 = q1;
	event->data.quaternion.q2 = q2;
	event->data.quaternion.q3 = q3;
	event->data.quaternion.q4 = q4;

	if ( l_mode & XM_SYS_LOG )
	{
//		kdLogMessagefKHR ( "KD_EVENT_QUATERNION      : %.2f, %.2f, %.2f, %.2f", q1, q2, q3, q4 );
	}

	_xmEventSend ( event );
}

KDvoid _xmEventEuler ( KDfloat roll, KDfloat pitch, KDfloat yaw )
{	
	KDEvent*  event = KD_NULL;

	if ( l_launch == KD_FALSE || l_visible == KD_FALSE )
	{
		return;
	}	

	event = _xmEventNew ( );
	event->type = KD_EVENT_EULER;
	event->data.euler.pitch = pitch;
	event->data.euler.roll  = roll;
	event->data.euler.yaw   = yaw;
	
	if ( l_mode & XM_SYS_LOG )
	{
//		kdLogMessagefKHR ( "KD_EVENT_EULER           : roll = %.2f, pitch = %.2f, yaw = %.2f", roll, pitch, yaw );
	}

	_xmEventSend ( event );
}

// xmRealizeWindow : Realize window and ready event loop.
KD_API KDvoid KD_APIENTRY xmRealizeWindow ( KDmode mode, KDCallbackFunc* func )
{
	l_mode  = mode;
	l_event = func;

	xmSetFrameInterval ( 1 / 60.0 );
	xmPlatformRealizeWindow ( mode );
}

// xmGetArgc : Get argument count.
KD_API KDint KD_APIENTRY xmGetArgc ( KDvoid )
{
	return g_argc;
}

// xmGetArgv : Get argument variables.
KD_API KDchar** KD_APIENTRY xmGetArgv ( KDvoid )
{
	return g_argv;
}

// xmGetWindow : Get window handle.
KD_API XMWindow KD_APIENTRY xmGetWindow ( KDvoid )
{
	return l_window;
}

// xmGetWindowMode : Get window mode.
KD_API KDmode KD_APIENTRY xmGetWindowMode ( KDvoid )
{
	return l_mode;
}

// xmSetFrameInterval : Set Frame Interval. 
// If input value is 0 then no interval timing. 
KD_API KDvoid KD_APIENTRY xmSetFrameInterval ( KDdouble interval )
{
	g_interval = interval * 1000;
    
#ifdef XM_HAS_PLATFORM_FRAME_INTERVAL
    xmPlatformSetFrameInterval ( interval );
#endif
}

// xmMeasureFramePerSecond : Measure FPS during 1 second.
KD_API KDdouble KD_APIENTRY xmMeasureFramePerSecond ( KDvoid )
{
	return l_fps;
}

// xmGetTouches : Get touches information. if id exist, hold state.
KD_API KDTouch* KD_APIENTRY xmGetTouches ( KDvoid )
{
	return l_touches;
}

// xmGetTouchPressedCount : Get pressed touches count.
KD_API KDuint KD_APIENTRY xmGetPressedTouchesCount ( KDvoid )
{
	return l_touches_cnt;
}

KD_API KDvoid KD_APIENTRY xmSetApnsMessage ( const KDchar* message )
{
	l_szApnsMessage = message;
}

KD_API const KDchar* KD_APIENTRY xmGetApnsMessage ( KDvoid )
{
	return l_szApnsMessage;
}