/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Polygon demo application
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

GLfloat  g_arr_vertex[] =
{	
	  0.0f, 110.0f, 0.0f,
     25.0f,  75.0f, 0.0f,
	  0.0f,  35.0f, 0.0f,
     50.0f,  35.0f, 0.0f,
	 75.0f,   0.0f, 0.0f,
	100.0f,  35.0f, 0.0f,
    150.0f,  35.0f, 0.0f,
	125.0f,  75.0f, 0.0f,
    150.0f, 110.0f, 0.0f,
	100.0f, 110.0f, 0.0f,
	 75.0f, 150.0f, 0.0f,
	 50.0f, 110.0f, 0.0f,	

	150.0f, 100.0f, 0.0f,
	  0.0f, 100.0f, 0.0f,
	135.0f,   0.0f, 0.0f,
	 75.0f, 150.0f, 0.0f,
	 15.0f,   0.0f, 0.0f,

	  0.0f,   0.0f, 0.0f,
	150.0f,   0.0f, 0.0f,
	150.0f, 150.0f, 0.0f,
	  0.0f, 150.0f, 0.0f,
	 25.0f,  25.0f, 0.0f,
     75.0f, 125.0f, 0.0f,
	125.0f,  25.0f, 0.0f,	
};

GLfloat	 g_arr_color[] =
{	
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,		
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,		

	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,	    	
};

GLuint  g_arr_coord[] =
{	
	  0, 110,
     25,  75,
	  0,  35,
     50,  35,
	 75,   0,
	100,  35,
    150,  35,
	125,  75,
    150, 110,
	100, 110,
	 75, 150,
	 50, 110,	

	150, 100,
	  0, 100,
	135,   0,
	 75, 150,
	 15,   0,

	  0,   0,
	150,   0,
	150, 150,
	  0, 150,
	 25,  25,
     75, 125,
	125,  25,
};

GLuint  g_num_vertex[] = { 12, 5, 7 };
GLuint  g_idx_hole[] = { 0, 0, 4 };

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas  = new XMGCanvas ( );	
	tls->xmg_font[0] = new XMGFont ( "/res/font/COOPBL.TTF" );
	tls->xmg_font[1] = new XMGFont ( "/res/font/BrushScriptStd.otf" );
	tls->xmg_tex     = new XMGTexture ( "/res/image/Album6.pvr" );
	tls->xmg_text    = new XMGText ( 3 );
	tls->xmg_poly    = new XMGPolygon ( 3, g_num_vertex );
	
	tls->xmg_text->SetFont ( tls->xmg_font[ 0 ], 0 );
	tls->xmg_text->SetText ( "XMGraphics : Polygon", 0 );

	for ( GLuint idx = 1; idx < 3; idx++ )
	{
		tls->xmg_text->SetFont ( tls->xmg_font[ 1 ], idx );
		tls->xmg_text->SetLineAlign ( XMG_ALIGN_CENTER, idx );				
		tls->xmg_text->SetColor ( XMGColorF ( 1.0f, 0.6f, 0.3f, 1.0f ), idx );
	}

	tls->xmg_poly->SetVertexArray ( (XMGVector3F *) g_arr_vertex, g_idx_hole );
	tls->xmg_poly->SetColorArray ( (XMGColorF *) g_arr_color );	
	tls->xmg_poly->SetTexture ( (XMGVector2 *) g_arr_coord, tls->xmg_tex );
	
	tls->app_index[ 0 ] = 2;
	tls->app_index[ 1 ] = 3;
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_poly;
	delete tls->xmg_text;
	delete tls->xmg_tex;
	delete tls->xmg_font[0];
	delete tls->xmg_font[1];	
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_poly->Render ( tls->app_index[ 0 ] );
	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	if ( msec - tls->app_time > 2000 )
	{
		tls->app_time = msec;
		tls->app_index[ 1 ] = tls->app_index[ 1 ] == 3 ? 0 : tls->app_index[ 1 ] + 1;

		switch ( tls->app_index [ 1 ] )
		{
			case 0 :

				tls->app_index[ 0 ] = tls->app_index[ 0 ] == 2 ? 0 : tls->app_index[ 0 ] + 1;

				switch ( tls->app_index [ 0 ] )
				{
					case 0 :	tls->xmg_text->SetText ( "SIMPLE POLYGON"     , 1 );	break;
					case 1 :	tls->xmg_text->SetText ( "INTERSECTED POLYGON", 1 );	break;
					case 2 :	tls->xmg_text->SetText ( "POLYGON WITH HOLE"  , 1 );	break;
				}

				tls->xmg_poly->SetDispMode ( XMG_DISP_TEXTURE, tls->app_index[ 0 ] );
				tls->xmg_text->SetText ( "DRAWING TEXTURE", 2 );

				break;

			case 1 :

				tls->xmg_poly->SetDispMode ( XMG_DISP_POINT, tls->app_index[ 0 ] );
				tls->xmg_text->SetText ( "DRAWING POINTS", 2 );

				break;

			case 2 :

				tls->xmg_poly->SetDispMode ( XMG_DISP_LINE, tls->app_index[ 0 ] );
				tls->xmg_text->SetText ( "DRAWING LINES", 2 );

				break;

			case 3 :

				tls->xmg_poly->SetDispMode ( XMG_DISP_FILL, tls->app_index[ 0 ] );
				tls->xmg_text->SetText ( "DRAWING FILL", 2 );

				break;
		}
	}
}

KDvoid xmEventResize ( KDint width, KDint height )
{
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );	

	tls->xmg_canvas->Viewport ( wnd );
	tls->xmg_canvas->Perspective ( width, height );
			
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h * 0.5f - 130.0f ), 1 );	
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h * 0.5f - 160.0f ), 2 );	
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	

	tls->xmg_poly->SetPosition ( XMGVector3F ( wnd.m_w * 0.5f - 75.0f, wnd.m_h * 0.5f - 50.0f ) );
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