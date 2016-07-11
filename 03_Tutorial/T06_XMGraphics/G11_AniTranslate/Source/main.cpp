/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Translate animation demo
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

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE(XM_SYS_DEFAULT)
XM_APP_MAIN_END

#define	GRID_Y              9
#define INTERVAL            3000

static KDvoid SetAnimate ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLfloat  ani[ ] = 
	{
		0.0f,	
		XMG_I2F ( tls->wnd_cx  ), 0.0f, 0.0f,				// position
		1.0f,												// alpha

		0.0f,	
		0.0f, 0.0f, 0.0f, 
		0.0f,
	};

	GLfloat  base = 500.0f;
	GLfloat  inc  = 250.0f;

	GLuint  row;

	for ( row = 0; row < GRID_Y; row++ )
	{
		switch ( row )
		{
			case 0			:
			case GRID_Y - 1 :	ani[ 5 ] = base;	break;
			default			:	ani[ 5 ] = base + inc; inc += 150.f;	break;
		}

		tls->xmg_ani[ GRID_Y - row - 1 ]->SetKeyFrameByMask ( XMG_ANI_POSITION | XMG_ANI_ALPHA, 2, ani );
	}	
}

KDvoid SetQuad ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLuint  row;

	XMGRectF  v;
	XMGRectF  c;

	GLfloat  h_v;
	GLfloat  h_c;
	GLfloat  mul;

	XMGVector3F  pos;
	XMGVector2F  size;
	
	v.m_w = XMG_I2F ( tls->wnd_cx );
	v.m_h = XMG_I2F ( tls->wnd_cy );

	tls->xmg_tex[ 0 ]->GetImageSize ( size );

	c.m_w = size.m_x;
	c.m_h = size.m_y;

	h_v = v.m_h / GRID_SIZE;
	h_c = c.m_h / GRID_SIZE;

	for ( row = 0; row < GRID_Y; row++ )
	{
		switch ( row )
		{
			case 0			:
			case GRID_Y - 1 :	mul = 3.f; break;
			default			:	mul = 2.f; break;
		}

		v.m_h = h_v * mul;
		c.m_h = h_c * mul;

		tls->xmg_quad[ 0 ]->SetVertexArray( &v, row );
		tls->xmg_quad[ 1 ]->SetVertexArray( &v, row );

		tls->xmg_quad[ 0 ]->SetPosition ( pos, row );
		tls->xmg_quad[ 1 ]->SetPosition ( pos, row );

		tls->xmg_quad[ 0 ]->SetTexture ( tls->xmg_tex[ 0 ], c, XMG_TEX_UNIT_0, row );
		tls->xmg_quad[ 1 ]->SetTexture ( tls->xmg_tex[ 1 ], c, XMG_TEX_UNIT_0, row );

		pos.m_y += v.m_h;
		c.m_y += c.m_h;
	}
}

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	GLuint  idx;

	tls->xmg_canvas    = new XMGCanvas ( );	
	tls->xmg_font      = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_tex[0]    = new XMGTexture ( "/res/image/menu.jpg" );
	tls->xmg_tex[1]    = new XMGTexture ( "/res/image/list.jpg" );
	tls->xmg_text      = new XMGText ( );
	tls->xmg_quad[ 0 ] = new XMGQuad ( GRID_Y );
	tls->xmg_quad[ 1 ] = new XMGQuad ( GRID_Y );
	
	for ( idx = 0; idx < GRID_Y; idx++ )
	{
		tls->xmg_ani[ idx ] = new XMGAnimate ( );
	}

	tls->xmg_canvas->ClearColor ( XMGColorF ( 0, 0, 0, 1.0f ) );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Translate" );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	for ( GLuint idx = 0; idx < GRID_Y; idx++ )
	{
		delete tls->xmg_ani[ idx ];
	}

	delete tls->xmg_quad[0];
	delete tls->xmg_quad[1];
	delete tls->xmg_text;
	delete tls->xmg_tex[0];
	delete tls->xmg_tex[1];
	delete tls->xmg_font;
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_quad[ 0 ]->Render ( );
	tls->xmg_quad[ 1 ]->Render ( );	
	
	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	KDust  t = msec - tls->app_msec;

	GLuint row;

	XMGMatrix4F  mat;
	XMGColorF    col;

	if ( t > INTERVAL )
	{
		t = 0;
		tls->app_msec = msec;
	}

	for ( row = 0; row < GRID_Y; row++ )
	{
		tls->xmg_ani[ row ]->Update ( t, &mat, &col );

		tls->xmg_quad[ 0 ]->SetColor ( col, row );
		tls->xmg_quad[ 1 ]->SetMatrix ( mat, row );
	}
}

KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );

	tls->wnd_cx = width;
	tls->wnd_cy = height;

	tls->xmg_canvas->Viewport ( wnd );
	tls->xmg_canvas->Perspective ( width, height );
		
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ) );
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	

	SetQuad ( );
	SetAnimate ( );
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