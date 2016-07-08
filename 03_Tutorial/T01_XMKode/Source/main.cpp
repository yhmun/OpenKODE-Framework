/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     XMKode Tutorial
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 

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

#include "main.h"
#include "platform.h"
#include "xmsoft.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE ( XM_SYS_DEFAULT | XM_SYS_LANDSCAPE )
XM_APP_MAIN_END

#define XM_TEST_BASIC          1
#define XM_TEST_ZLIB           0
#define XM_TEST_SOUND		   0

#if XM_TEST_BASIC == 1
static KDvoid xmXMKode ( KDvoid )
{
	PFNExample		pfn_example[ ] =
	{
		xmExample_01 ,        // 01. Application startup and exit
		xmExample_02 ,        // 02. Versioning and attribute queries
		xmExample_03 ,        // 03. Errors
		xmExample_04 ,        // 04. Assertions and logging
		xmExample_05 ,        // 05. Memory allocation
		xmExample_06 ,        // 06. String and memory functions
		xmExample_07 ,        // 07. Utility library functions
		xmExample_08 ,        // 08. Mathematical functions
		xmExample_09 ,        // 09. File system
		xmExample_10 ,        // 10. Events
		xmExample_11 ,        // 11. Time functions
		xmExample_12 ,        // 12. Locale specific functions
		xmExample_13 ,        // 13. Timer functions
		xmExample_14 ,        // 14. Threads and synchronization
		xmExample_15 ,        // 15. Thread-local storage
		xmExample_16 ,        // 16. Network sockets
		xmExample_17 ,        // 17. Windowing
		xmExample_18 ,        // 18. Input output
		xmExample_19 ,        // 19. System events 
		xmExample_20 ,        // 20. KD_KHR_float64
		xmExample_21 ,        // 21. KD_KHR_formatted
		xmExample_22 ,        // 22. KD_KHR_thread_storage
		xmExample_23 ,        // 23. KD_KHR_perfcounter
		xmExample_24 ,        // 24. XM_Queue 
		0 ,
	};

	KDint       idx  = 0;

	for ( ; pfn_example[ idx ] != 0; idx++ )
	{
		if ( idx == 12 || idx == 13 )
		{
			continue;
		}

		pfn_example[ idx ] ( );
		kdLogMessage ( " " );
	}
}
#endif

static KDvoid xmSetLogo ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

#if 0
	KDubyte*  pixels = (KDubyte*) kdCalloc ( g_img_xmsoft_w * g_img_xmsoft_h * 4, 1 );
	kdMemset ( pixels, 255, g_img_xmsoft_w * g_img_xmsoft_h * 2 );
	kdMemset ( pixels + g_img_xmsoft_w * g_img_xmsoft_h * 2, 128, g_img_xmsoft_w * g_img_xmsoft_h * 2 );

	xmSetQuad ( g_img_xmsoft_w, g_img_xmsoft_h, 0 );

	glGenTextures   ( 1, &tls->tex );
	glBindTexture   ( GL_TEXTURE_2D , tls->tex );
	glTexImage2D    ( GL_TEXTURE_2D , 0, GL_RGBA, g_img_xmsoft_w, g_img_xmsoft_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	kdFree ( pixels );
#else
	xmSetQuad ( g_img_xmsoft_w, g_img_xmsoft_h, 1 );

	glGenTextures   ( 1, &tls->tex );
	glBindTexture   ( GL_TEXTURE_2D , tls->tex );
	glTexImage2D    ( GL_TEXTURE_2D , 0, GL_RGBA, g_img_xmsoft_w, g_img_xmsoft_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, g_img_xmsoft );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_LINEAR );
#endif

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	kdLogMessage ( "* Set Logo Texture" );
}

static KDvoid xmDeleteObject ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	if ( tls->vertex )
	{
		glDeleteBuffers ( 1, &tls->vertex );
		tls->vertex = 0;
	}

	if ( tls->coord )
	{
		glDeleteBuffers ( 1, &tls->coord );
		tls->coord = 0;
	}

	if ( tls->tex )
	{
		glDeleteTextures ( 1, &tls->tex );
		tls->tex = 0;
	}
}

KDvoid xmSetQuad ( GLint ix, GLint iy, GLint flip_y )
{
	KD_GET_TLS ( KDTLS, tls );

	GLint      tx = 1;
	GLint      ty = 1;
	GLfloat    fx = 0;
	GLfloat    fy = 0;

	fx = (GLfloat) ix / tls->wnd_cx;
	fy = (GLfloat) iy / tls->wnd_cy;

	{
		GLfloat    v[ ] = 
		{
			-fx,  fy, 0.0f,
			 fx,  fy, 0.0f,
			-fx, -fy, 0.0f,
			 fx, -fy, 0.0f,
		};

		glGenBuffers ( 1, &tls->vertex );
		glBindBuffer ( GL_ARRAY_BUFFER, tls->vertex );
		glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * 4 * 3, v, GL_STATIC_DRAW );
	}

	while ( tx < ix ) tx = tx << 1;
	while ( ty < iy ) ty = ty << 1;

	fx = (GLfloat) ix / tx;
	fy = (GLfloat) iy / ty;

	{
		GLfloat    c[ ] = 
		{
			  0,  flip_y ? fy : 0,
			 fx,  flip_y ? fy : 0,
			  0,  flip_y ? 0 : fy, 
			 fx,  flip_y ? 0 : fy, 
		};
	
		glGenBuffers ( 1, &tls->coord );
		glBindBuffer ( GL_ARRAY_BUFFER, tls->coord );
		glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * 4 * 2, c, GL_STATIC_DRAW );
	}
}

static KDvoid xmEventCreate ( KDint width, KDint height )
{

#if XM_TEST_BASIC 
//	xmXMKode ( );
#endif
/*
#if XM_TEST_ZLIB  
	xmExample_ZLib ( );
#endif

#if XM_TEST_SOUND
	xmExample_Sound ( );
#endif
*/
    
	KD_SET_TLS ( KDTLS );

	{		
		KD_GET_TLS ( KDTLS, tls );
		tls->index = IMAGE_PATH_COUNT;
	}

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );	
	glClearColor ( 0.0f, 0.0f, 0.25f, 1.0f );	

	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

static KDvoid xmEventDestroy ( KDvoid )
{	
	xmExample_Sound_Free ( );

	xmDeleteObject ( );
}

static KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );

	tls->wnd_cx = width;
	tls->wnd_cy = height;

	glViewport ( 0, 0, width, height );
	glScissor  ( 0, 0, width, height );
}

static KDvoid xmEventRedraw ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( tls->vertex )
	{
		glEnable ( GL_TEXTURE_2D );	
		glBindTexture ( GL_TEXTURE_2D, tls->tex );	

		glLoadIdentity ( );

		glTranslatef ( 0.0f, kdSinf ( tls->trans ) / 4.0f, 0.0f );		

		glEnableClientState ( GL_VERTEX_ARRAY );
		glBindBuffer ( GL_ARRAY_BUFFER, tls->vertex );
		glVertexPointer ( 3, GL_FLOAT, 0, 0 );

		glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
		glBindBuffer ( GL_ARRAY_BUFFER, tls->coord );
		glTexCoordPointer ( 2, GL_FLOAT, 0, 0 );	

		glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
		glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState ( GL_VERTEX_ARRAY );

		glDisable ( GL_TEXTURE_2D );
		glBindTexture ( GL_TEXTURE_2D, 0 );
		
		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	}

	glFlush ( );
}

static KDvoid xmEventUpdate ( KDuint msec )
{
	KD_GET_TLS ( KDTLS, tls );

	if ( ( msec - tls->msec ) > 1000 )
	{		
		
		xmDeleteObject ( );

		if ( tls->index == IMAGE_PATH_COUNT )
		{
			tls->index = 0;
			xmSetLogo ( );
		}
		else
		{
			tls->index++;
			xmLoadTexture ( );
		}		

		tls->msec = msec;
	}

	if ( msec - tls->msec2 > 20 )
	{
		tls->msec2 = msec;
		tls->trans += 0.1f;
	}	
}

KD_API KDvoid KD_APIENTRY xmEventProc ( const KDEvent* event )
{
	switch ( event->type )
	{
		case KD_EVENT_NATIVE :  
			{
//				#if !defined ( SHP ) && defined ( _WIN32 ) 
//				KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
//				KDEventNativeLinux*  proc = (KDEventNativeLinux*) event->data.native.p;
//				#endif
			}
			break;
		
		case KD_EVENT_CREATE :
			{
				// event->data.size.width;
				// event->data.size.height;

				xmEventCreate ( event->data.size.width, event->data.size.height );
				xmEventResize ( event->data.size.width, event->data.size.height );
			}
			break;

		case KD_EVENT_DESTROY :  
			{
				xmEventDestroy ( );
			}
			break;

		case KD_EVENT_RESIZE :       
			{
				// event->data.size.width;
				// event->data.size.height;	
				xmEventResize ( event->data.size.width, event->data.size.height );
			}
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
				xmEventRedraw ( );
			}
			break;

		case KD_EVENT_UPDATE :      
			{
				// event->data.update.msec;

				xmEventUpdate ( event->data.update.msec );
			}
			break;

		case KD_EVENT_TOUCH_BEGAN :     
			{
				// event->data.touch.touches;
				// event->data.touch.count;

				xmExample_Sound_Event ( );
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
}
