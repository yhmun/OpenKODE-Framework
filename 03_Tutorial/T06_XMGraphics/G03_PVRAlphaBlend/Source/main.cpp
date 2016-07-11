/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : PVR AlphaBlend demo application
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

	tls->xmg_canvas  = new XMGCanvas ( );	
	tls->xmg_font[0] = new XMGFont ( "/res/font/COOPBL.TTF" );
	tls->xmg_font[1] = new XMGFont ( "/res/font/BrushScriptStd.otf" );
	tls->xmg_tex[0]  = new XMGTexture ( "/res/image/Background.pvr" );
	tls->xmg_tex[1]  = new XMGTexture ( "/res/image/Foreground.pvr" );
	tls->xmg_text    = new XMGText ( 3 );
	tls->xmg_quad    = new XMGQuad ( 2 );
	
	tls->xmg_font[ 1 ]->SetSize ( 18 );

	tls->xmg_text->SetFont ( tls->xmg_font[ 0 ], 0 );
	tls->xmg_text->SetText ( "XMGraphics : PVR AlphaBlend", 0 );

	for ( GLuint idx = 1; idx < 3; idx++ )
	{
		tls->xmg_text->SetFont ( tls->xmg_font[ 1 ], idx );
		tls->xmg_text->SetLineAlign ( XMG_ALIGN_CENTER, idx );				
		tls->xmg_text->SetColor ( XMGColorF ( 1.0f, 0.6f, 0.3f, 1.0f ), idx );
	}

	tls->xmg_quad->SetBlend ( XMG_TRUE );
	tls->xmg_quad->SetVertexArray ( tls->xmg_tex[ 1 ], 1 );
	tls->xmg_quad->SetTexture ( tls->xmg_tex[ 0 ], XMG_FLIP_NULL, XMG_TEX_UNIT_0, 0 );
	tls->xmg_quad->SetTexture ( tls->xmg_tex[ 1 ], XMG_FLIP_NULL, XMG_TEX_UNIT_0, 1 );

	tls->app_index = 3;
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_quad;
	delete tls->xmg_text;
	delete tls->xmg_tex[0];
	delete tls->xmg_tex[1];
	delete tls->xmg_font[0];
	delete tls->xmg_font[1];	
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_quad->Render ( );
	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	if ( msec - tls->app_time > 2000 )
	{
		tls->app_time  = msec;
		tls->app_index = tls->app_index == 3 ? 0 : tls->app_index + 1;

		switch ( tls->app_index )
		{
			case 0 :

				tls->xmg_quad->SetBlendOP ( XMG_BLEND_SRC_ALPHA, XMG_BLEND_ONE_MINUS_SRC_ALPHA, 1 );
				tls->xmg_text->SetText ( "TRANSPARENCY", 1 );
				tls->xmg_text->SetText ( "(SRC_ALPHA, 1 - SRC_ALPHA)", 2 );

				break;

			case 1 :
				
				tls->xmg_quad->SetBlendOP ( XMG_BLEND_ONE, XMG_BLEND_ONE, 1 );
				tls->xmg_text->SetText ( "ADDITIVE", 1 );
				tls->xmg_text->SetText ( "(ONE, ONE)", 2 );

				break;

			case 2 :
				
				tls->xmg_quad->SetBlendOP ( XMG_BLEND_DST_COLOR, XMG_BLEND_ZERO, 1 );
				tls->xmg_text->SetText ( "MODULATE", 1 );
				tls->xmg_text->SetText ( "(DST_COLOR, ZERO)", 2 );

				break;

			case 3 :
				
				tls->xmg_quad->SetBlendOP ( XMG_BLEND_DST_COLOR, XMG_BLEND_SRC_COLOR, 1 );	
				tls->xmg_text->SetText ( "MODULATE X2", 1 );
				tls->xmg_text->SetText ( "(DST_COLOR, SRC_COLOR)", 2 );

				break;
		}
	}	
}

KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );

	XMGVector3F  pos;
	XMGVector2F	 size;

	tls->xmg_tex[ 1 ]->GetImageSize ( size );
	pos = XMGVector3F ( ( width - size.m_x ) * 0.5f, ( height - size.m_y ) * 0.5f + 20.0f );

	tls->xmg_canvas->Viewport ( wnd );
	tls->xmg_canvas->Perspective ( width, height );
			
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, pos.m_y - 30.0f ), 1 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, pos.m_y - 60.0f ), 2 );
	
	tls->xmg_quad->SetVertexArray ( &wnd, 0 );
	tls->xmg_quad->SetPosition ( pos, 1 );
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