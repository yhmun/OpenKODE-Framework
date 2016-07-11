/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Ripple show demo application
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

static KDvoid SetAnimate ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	XMGVector2F  prev_pos;
	XMGVector2F  post_pos;
	GLfloat      prev_scale;
	GLfloat      post_scale;
	
	switch ( tls->app_mode )
	{
		case APP_MODE_LIST :

			prev_pos    = tls->xmg_pos[ tls->app_select ];
			prev_scale  = 0.25f;
			post_scale  = 1.0f;
			tls->app_mode = APP_MODE_RIPPLE;			
			tls->xmg_ripple->XMGRender::SetTexture ( tls->xmg_tex[ tls->app_select ] );

			break;

		case APP_MODE_RIPPLE :

			post_pos    = tls->xmg_pos[ tls->app_select ];
			prev_scale  = 1.0f;
			post_scale  = 0.25f;
			tls->app_mode = APP_MODE_LIST;

			break;
	}
	
	GLfloat  ani[ ] = 
	{
		0.0f,	
		prev_pos.m_x, prev_pos.m_y, 0.0f,		// position
		prev_scale, prev_scale, 1.0f,			// scale

		ANI_TIME,	
		post_pos.m_x, post_pos.m_y, 0.0f,		
		post_scale, post_scale, 1.0f,						
	};

	tls->xmg_ani->SetKeyFrameByMask ( XMG_ANI_POSITION | XMG_ANI_SCALE, 2, ani );
	tls->app_stat = APP_STAT_ANIMATE;
	tls->app_time = KD_GET_UST2MSEC;	
}

static KDvoid SetVertexArray ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLfloat  w = XMG_I2F ( tls->wnd_cx );
	GLfloat  h = XMG_I2F ( tls->wnd_cy );

	XMGRectF  rc = XMGRectF ( 0.0f, 0.0f, 0.0f, w, h );

	XMGVector2F  pos;			
	XMGMatrix4F  mat;

	GLuint  row;
	GLuint  col;
	GLuint  idx;

	tls->xmg_size.m_x = w / 4;
	tls->xmg_size.m_y = h / 4;

	tls->xmg_dist.m_x = w / 4 + w / 16;
	tls->xmg_dist.m_y = h / 4 + h / 16;

	pos.m_x = w - tls->xmg_dist.m_x * 3;
	pos.m_y = h - tls->xmg_dist.m_y * 3;

	idx = 0;
	for ( row = 0; row < 3; row++ )
	{
		for ( col = 0; col < 3; col++ )
		{
			tls->xmg_pos[ idx ].m_x = pos.m_x + tls->xmg_dist.m_x * col;
			tls->xmg_pos[ idx ].m_y = pos.m_y + tls->xmg_dist.m_y * ( 3 - row - 1 );

			mat.Identity ( );

			mat.Translate ( tls->xmg_pos[ idx ].m_x, tls->xmg_pos[ idx ].m_y, 0 );
			mat.Scale ( 0.25f, 0.25f, 1.0f );

			tls->xmg_quad->SetMatrix ( mat, idx );
			tls->xmg_quad->SetVertexArray ( &rc, idx );

			idx++;
		}
	}

	tls->xmg_ripple->SetVertexArray( w, h );
}

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );
	
	GLuint  idx;
	KDchar  path[128];

	tls->xmg_canvas = new XMGCanvas ( );	
	tls->xmg_font   = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_text   = new XMGText ( );
	tls->xmg_ani    = new XMGAnimate ( );
	tls->xmg_quad   = new XMGQuad ( 9 );
	tls->xmg_ripple = new XMGRipple ( );
	
	for ( idx = 0; idx < 9; idx++ )
	{
		kdSprintfKHR ( path, "/res/image/image_%02d.jpg", idx );
		tls->xmg_tex[ idx ] = new XMGTexture ( path );
		tls->xmg_quad->SetTexture ( tls->xmg_tex[ idx ], XMG_FLIP_NULL, XMG_TEX_UNIT_0, idx );
	}

	tls->xmg_ripple->SetTexture ( tls->xmg_tex[ 0 ] );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Ripple show" );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	for ( GLuint idx = 0; idx < 9; idx++ )
	{
		delete tls->xmg_tex[ idx ];
	}

	delete tls->xmg_ripple;
	delete tls->xmg_quad;
	delete tls->xmg_text;
	delete tls->xmg_ani;
	delete tls->xmg_font;
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	if ( tls->app_stat == APP_STAT_ANIMATE || tls->app_mode == APP_MODE_LIST )
	{
		tls->xmg_quad->Render ( );
		tls->xmg_quad->Render ( tls->app_select );
	}
	else
	{
		tls->xmg_ripple->Render ( );
	}
	
	tls->xmg_text->Render ( );		

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	XMGMatrix4F  mat;
	
	if ( tls->app_stat == APP_STAT_ANIMATE )
	{
		msec = msec - tls->app_time;

		if ( msec > ( ANI_TIME + 50 ) )
		{
			tls->app_stat = APP_STAT_IDLE;
		}	

		tls->xmg_ani->Update ( msec, &mat, 0 );
		tls->xmg_quad->SetMatrix ( mat, tls->app_select );
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

	SetVertexArray ( );
}

KDvoid xmEventTouchUp ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventTouchDown ( KDint id, KDint x, KDint y )
{
	KD_GET_TLS ( KDTLS, tls );

	XMGVector2F  touch ( XMG_I2F ( x ), XMG_I2F ( y ) );

	GLuint  idx;

	if ( tls->app_stat == APP_STAT_IDLE )
	{
		switch ( tls->app_mode )
		{
			case APP_MODE_LIST :

				for ( idx = 0; idx < 9; idx++ )
				{
					if ( tls->xmg_pos[ idx ].m_x <= touch.m_x && ( tls->xmg_pos[ idx ].m_x + tls->xmg_size.m_x ) >= touch.m_x &&
						 tls->xmg_pos[ idx ].m_y <= touch.m_y && ( tls->xmg_pos[ idx ].m_y + tls->xmg_size.m_y ) >= touch.m_y )
					{
						tls->app_select = idx;
						SetAnimate ( );
						break;
					}
				}

				break;

			case APP_MODE_RIPPLE :

				if ( touch.m_x < 64.f && touch.m_y < 64.f )
				{
					SetAnimate ( );
				}
				else
				{
					tls->xmg_ripple->SetRipple ( x, y );
				}

				break;
		}
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
			xmEventTouchDown(0, event->data.touch.touches[0].x, event->data.touch.touches[0].y);
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