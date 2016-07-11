/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Panorama demo application
 *      Version         0.20.0801, 2011-08-01
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 XMSoft. All rights reserved.
 * 
 *      Contact Email: chris@xmsoft.co.kr 
 *                     xmsoft77@gmail.com	
 * 
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
 */

#include "main.h"
#include "platform.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE(XM_SYS_DEFAULT)
XM_APP_MAIN_END

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	GLfloat  radius = 1.0f;

	tls->xmg_canvas  = new XMGCanvas ( );	
	tls->xmg_view[0] = new XMGView ( );
	tls->xmg_view[1] = new XMGView ( );
	tls->xmg_font    = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_tex     = new XMGTexture ( "/res/image/pano1_1.jpg" );
	tls->xmg_text    = new XMGText ( );
	tls->xmg_sphere  = new XMGSphere ( );
	
	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Panorama" );

	tls->xmg_sphere->SetVertexArray ( &radius );	
	tls->xmg_sphere->SetPosition ( XMGVector3F ( 0.0f, 0.0f, -1.0f ) );
	tls->xmg_sphere->SetTexture ( tls->xmg_tex );
	tls->xmg_sphere->SetCull ( XMG_CULL_FRONT );
	
	tls->app_lon = -90.0f;
	tls->app_lat =   0.0f;
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_sphere;
	delete tls->xmg_text;
	delete tls->xmg_tex;
	delete tls->xmg_font;
	delete tls->xmg_view[0];	
	delete tls->xmg_view[1];
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_view[ 0 ]->Update ( );
	tls->xmg_sphere->Render ( );

	tls->xmg_view[ 1 ]->Update ( );
	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	XMGMatrix4F	 mat;
	
	mat.Rotate ( -tls->app_lat, 1.0f, 0.0f, 0.0f );	
	mat.Rotate (  tls->app_lon, 0.0f, 1.0f, 0.0f );	

	tls->xmg_sphere->SetMatrix ( mat );

	tls->app_lon += tls->app_rot_x;
	tls->app_lat += tls->app_rot_y;
}

KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );
	
	tls->wnd_cx = width;
	tls->wnd_cy = height;

	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );	
	GLfloat	  aspect = wnd.m_w / wnd.m_h;
	
	XMGMatrix4F	 mat[2];

	mat[ 0 ].Perspective ( 45.0f, aspect, 0.1f, 100.0f );
	mat[ 1 ].Ortho ( 0.0f, wnd.m_w, 0.0f, wnd.m_h, -1.0f, 100.0f );

	for ( GLuint idx = 0; idx < 2; idx++ )
	{
		tls->xmg_view[ idx ]->SetViewport ( wnd );
		tls->xmg_view[ idx ]->SetProject ( mat[ idx ] );	
	}
		
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	
}

KDvoid xmEventTouchUp ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventTouchDown ( KDint id, KDint x, KDint y )
{
	KD_GET_TLS ( KDTLS, tls );

	if ( tls->wnd_cx / 3 > x )
	{
		tls->app_rot_x = -1.0f;
	}
	else if ( x > tls->wnd_cx * 2 / 3 )
	{
		tls->app_rot_x =  1.0f;
	}
	else
	{
		tls->app_rot_x =  0.0f;
	}
	
	if ( tls->wnd_cy / 3 > y )
	{
		tls->app_rot_y = -1.0f;
	}
	else if ( y > tls->wnd_cy * 2 / 3 )
	{
		tls->app_rot_y = 1.0f;
	}
	else
	{
		tls->app_rot_y = 0.0f;
	}
}

KD_API KDvoid KD_APIENTRY xmEventProc(const KDEvent* event)
{
	switch (event->type)
	{
		case KD_EVENT_NATIVE:
		{
			//#if !defined ( SHP ) && defined ( _WIN32 ) 
			//KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
			//KDEventNativeLinux*  proc = (KDEventNativeLinux*) event->data.native.p;
			//#endif
		}	break;

		case KD_EVENT_CREATE:
		{
			// event->data.size.width;
			// event->data.size.height;
			xmEventCreate();
			xmEventResize(event->data.size.width, event->data.size.height);
		}	break;

		case KD_EVENT_DESTROY:
		{
			xmEventDestroy();
		}	break;

		case KD_EVENT_RESIZE:
		{
			// event->data.size.width;
			// event->data.size.height;	
			xmEventResize(event->data.size.width, event->data.size.height);
		}	break;

		case KD_EVENT_FOCUS:
		{
			// event->data.value.i;
			// 1 : focus
		}	break;

		case KD_EVENT_VISIBLE:
		{
			// event->data.value.i;
			// 1 : visible
		}	break;

		case KD_EVENT_REDRAW:
		{
			xmEventRedraw();
		}	break;

		case KD_EVENT_UPDATE:
		{
			// event->data.update.msec;
			xmEventUpdate(event->data.update.msec);
		}	break;

		case KD_EVENT_TOUCH_BEGAN:
		{
			// event->data.touch.touches;
			// event->data.touch.count;
		}	break;

		case KD_EVENT_TOUCH_MOVED:
		{

		}	break;

		case KD_EVENT_TOUCH_ENDED:
		{
			xmEventTouchDown (0,event->data.touch.touches[0].x, event->data.touch.touches[0].y);
		}	break;

		case KD_EVENT_TOUCH_CANCELLED:
		{

		}	break;

		case KD_EVENT_KEY_RELEASED:
		{
			// event->data.keypad.code;		
		}	break;

		case KD_EVENT_KEY_PRESSED:
		{

		}	break;

		case KD_EVENT_ACCELEROMETER:
		{
			// event->data.accelerometer.x;
			// event->data.accelerometer.y;
			// event->data.accelerometer.z;
		}	break;

		case KD_EVENT_LOCATION:
		{
			// event->data.value.i;
			// KD_NMEA_UPDATED_GPS, KD_NMEA_UPDATED_USER
		}	break;

		case KD_EVENT_INSERT_TEXT:
		{
			// event->data.insert.utf8;
		}	break;

		case KD_EVENT_SERIALIZE:
		{
			// event->data.serialize.type;
			// event->data.serialize.data;
			// event->data.serialize.size;
		}	break;
	}
}