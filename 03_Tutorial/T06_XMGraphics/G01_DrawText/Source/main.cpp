/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : True type font demo application
 *      Version         0.20.0801, 2011-08-01
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2011 XMSoft. All rights reserved.
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

#if defined(_WIN32) && !defined(SHP)
	#include "kor_ansi.h"
#else
	#include "kor_utf8.h"
#endif

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE(XM_SYS_DEFAULT)
XM_APP_MAIN_END

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas = new XMGCanvas ( );
	tls->xmg_text   = new XMGText ( 3 );

	for ( GLuint idx = 0; idx < 3; idx++ )
	{
		switch ( idx )
		{
			case 0 :

				tls->xmg_font[ idx ] = new XMGFont ( "/res/font/COOPBL.TTF" );

				tls->xmg_text->SetFont ( tls->xmg_font[ idx ], idx );
				tls->xmg_text->SetText ( "XMGraphics : TTFont", idx );

				break;

			case 1 :

				tls->xmg_font[ idx ] = new XMGFont ( "/res/font/HMKMMAG.TTF" );
				tls->xmg_font[ idx ]->SetSize ( 40 );

			case 2 :

				tls->xmg_text->SetFont ( tls->xmg_font[ 1 ], idx );
				tls->xmg_text->SetLineAlign ( XMG_ALIGN_CENTER, idx );				
				tls->xmg_text->SetColor ( XMGColorF ( 1.0f, 0.6f, 0.3f, 1.0f ), idx );
				tls->xmg_text->SetCull ( XMG_CULL_DISABLE, idx );
				tls->xmg_text->SetText ( g_sz_test, idx );			

				break;
		}
	}
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_text;
	delete tls->xmg_font[ 1 ];
	delete tls->xmg_font[ 0 ];
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	XMGMatrix4F	 mat[2];

	mat[ 0 ].Translate (  tls->wnd_cx * 0.5f - 0.5f, 0.0f, 0.0f );
	mat[ 0 ].Rotate ( tls->app_rot, 0.0f, 1.0f, 0.0f );
	mat[ 0 ].Translate ( -tls->wnd_cx * 0.5f - 0.5f, 0.0f, 0.0f );

	mat[ 1 ].Translate ( 0.0f,  20.0f, 0.0f );
	mat[ 1 ].Rotate ( tls->app_rot, 1.0f, 0.0f, 0.0f );
	mat[ 1 ].Translate ( 0.0f, -20.0f, 0.0f );

	tls->xmg_text->SetMatrix ( mat[ 0 ], 1 );
	tls->xmg_text->SetMatrix ( mat[ 1 ], 2 );

	tls->app_rot += 2.0f;	
}

KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );

	tls->wnd_cx = width;
	tls->wnd_cy = height;	

	tls->xmg_canvas->Viewport ( wnd );
	tls->xmg_canvas->Perspective ( width, height );
			
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h * 0.5f + 20.0f ), 1 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h * 0.5f - 60.0f ), 2 );
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	
}

KD_API KDvoid KD_APIENTRY xmEventProc(const KDEvent* event)
{
	switch (event->type)
	{
		case KD_EVENT_NATIVE:
		{
			//				#if !defined ( SHP ) && defined ( _WIN32 ) 
			//				KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
			//				KDEventNativeLinux*  proc = (KDEventNativeLinux*) event->data.native.p;
			//				#endif
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