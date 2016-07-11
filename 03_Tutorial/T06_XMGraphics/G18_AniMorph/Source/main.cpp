/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Morph animation demo 1
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
#include "morph.h"
#include "platform.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE(XM_SYS_DEFAULT)
XM_APP_MAIN_END

#define ANI_TIME      1500
#define INTERVAL      3000

KDvoid SetMorph ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLfloat*        arr_morph;
	XMGVector2F*    arr_coord;

	XMGMatrix4F     mat;
	XMGRectF        rc;

	XMGVector2F     img;
	XMGVector2F     tex;
	XMGVector2F     rate;
	XMGVector2F     half;

	GLuint          idx;
	GLuint          off;
	GLuint          frame;

	rc.m_w = XMG_I2F ( tls->wnd_cx );
	rc.m_h = XMG_I2F ( tls->wnd_cy );

	tls->xmg_quad->SetVertexArray ( &rc );
	tls->xmg_quad->SetTexture ( tls->xmg_tex[ 0 ] );

	tls->xmg_tex[ 0 ]->GetSize ( tex );
	tls->xmg_tex[ 0 ]->GetImageSize ( img );

	rate.m_x = img.m_x / tex.m_x;
	rate.m_y = img.m_y / tex.m_y;

	arr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * g_morph_num_vertex );	

	for ( idx = 0; idx < g_morph_num_vertex; idx++ )
	{
		arr_coord[ idx ].m_x = g_morph_arr_coord[ idx * 2 + 0 ] * rate.m_x;
		arr_coord[ idx ].m_y = g_morph_arr_coord[ idx * 2 + 1 ] * rate.m_y;
	}

	tls->xmg_morph->SetDispMode ( XMG_DISP_TEXTURE );
	tls->xmg_morph->SetCoordArray ( arr_coord );	

	kdFree ( arr_coord );

	arr_morph = (GLfloat *) kdMalloc ( sizeof ( GLfloat ) * ( g_morph_num_frame + g_morph_num_frame * g_morph_num_vertex * 3 ) );

	off = 0;
	for ( frame = 0; frame < g_morph_num_frame; frame++ )
	{
		arr_morph[ off ] = XMG_I2F ( ( ANI_TIME / g_morph_num_frame ) * frame );	off++;

		for ( idx = 0; idx < g_morph_num_vertex; idx++ )
		{
			arr_morph[ off ] =  g_morph_arr_vertex[ frame ][ idx * 3 + 0 ];			off++;
			arr_morph[ off ] = -g_morph_arr_vertex[ frame ][ idx * 3 + 2 ];			off++;
			arr_morph[ off ] =  g_morph_arr_vertex[ frame ][ idx * 3 + 1 ];			off++;
		}
	}

	tls->xmg_ani->SetKeyFrame ( g_morph_num_vertex * 3, g_morph_num_frame, arr_morph );

	kdFree ( arr_morph );
	
	tls->xmg_morph->SetIndexArray ( g_morph_arr_index, (const GLuint*) &g_morph_num_index );

	half.m_x = rc.m_w / 2.0f;
	half.m_y = rc.m_h / 2.0f;

	mat.Translate ( half.m_x, half.m_y, 0.0f );
	mat.Scale ( half.m_x / 54.1f, half.m_y / 76.6f, 1.0f );

	tls->xmg_morph->SetMatrix ( mat );
}

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );
	
	tls->xmg_canvas = new XMGCanvas ( );	
	tls->xmg_font   = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_tex[0] = new XMGTexture ( "/res/image/idle.jpg" );
	tls->xmg_tex[1] = new XMGTexture ( "/res/image/menu.jpg" );
	tls->xmg_text   = new XMGText ( );
	tls->xmg_ani    = new XMGAnimate ( );
	tls->xmg_quad   = new XMGQuad ( 1 );
	tls->xmg_morph  = new XMGRender ( 1, g_morph_num_vertex );	
	tls->xmg_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * g_morph_num_vertex );

	tls->xmg_canvas->ClearColor ( XMGColorF ( 0, 0, 0, 1.0f ) );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Morphing 1" );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	kdFree ( tls->xmg_vertex );

	delete tls->xmg_quad;
	delete tls->xmg_morph;
	delete tls->xmg_text;
	delete tls->xmg_ani;
	delete tls->xmg_tex[0];
	delete tls->xmg_tex[1];
	delete tls->xmg_font;
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	// Updating
	KDuint  msec = KD_GET_UST2MSEC;
	KDust   t = msec - tls->app_msec;
	KDbool  flip = tls->app_flip;

	if ( t > INTERVAL )
	{
		t = 0;
		tls->app_msec = msec;
		tls->app_flip = tls->app_flip ? KD_FALSE : KD_TRUE;

		tls->xmg_morph->SetTexture ( tls->xmg_tex[ flip ? 0 : 1 ] );
		tls->xmg_quad->XMGRender::SetTexture ( tls->xmg_tex[ flip ? 1 : 0 ] );
	}

	// Rendering
	tls->xmg_canvas->Clear ( );	

	tls->xmg_quad->Render ( );

	if ( t < ANI_TIME )
	{
		tls->xmg_ani->Update ( t, (GLfloat *) tls->xmg_vertex );
		tls->xmg_morph->SetVertexArray ( tls->xmg_vertex );

		tls->xmg_morph->Render ( );
	}
	
	tls->xmg_text->Render ( );		
	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{

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

	SetMorph ( );
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