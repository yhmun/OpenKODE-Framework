/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : 2.5D Map demo application
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

XM_DEF_APP_MAIN_DEFAULT ( XM_SYS_DEFAULT | XM_SYS_LANDSCAPE )

static KDvoid LoadData ( KDuint index, KDuint pattern, const KDchar* path )
{
	KD_GET_TLS ( KDTLS, tls );

	KDFile*         file;
	KDsize          size;
	KDuint8*        buffer;
	
	MapHeader*      ptr_map_header;
	GLuint*         ptr_num_vertex;
	GLuint*         ptr_arr_height;
	GLshort*        ptr_arr_vertex;
	GLubyte*        ptr_arr_color;	

	XMGVector3F*    arr_vertex;
	XMGVector2F*    arr_coord;
	XMGColorF*      arr_color;

	XMGData*        polydata;
	XMGRender*      polyline;
	XMGPolygon*     polygon;

	GLuint          idx_object;
	GLuint          idx_vertex;

	GLuint          num_vertex;

	GLuint          off_vertex;
	GLuint          off_coord;

	file = kdFopen ( path, "rb" );
	kdFseek  ( file, 0, KD_SEEK_END );
	size = (KDsize) kdFtell ( file );
	kdFseek  ( file, 0, KD_SEEK_SET );
	buffer = (KDuint8 *) kdMalloc ( size );
	kdFread ( (void *) buffer, size, 1, file );
	kdFclose ( file );

	ptr_map_header = (MapHeader *) buffer;	
	ptr_num_vertex = (GLuint *) ( buffer + 40 );

	polydata = (XMGData *) kdMalloc ( sizeof ( XMGData ) );
	arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * ptr_map_header->num_vertex );
	arr_color = (XMGColorF *) kdMalloc ( sizeof ( XMGColorF ) * ptr_map_header->num_object );

	switch ( ptr_map_header->type_object )
	{
		case  1 :
		case 20 :

			ptr_arr_color  = (GLubyte *) ( buffer + 40 + ptr_map_header->num_object * 4 * 1 );
			ptr_arr_vertex = (GLshort *) ( buffer + 40 + ptr_map_header->num_object * 4 * 2 );

			break;

		case 25 :

			arr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * ( ptr_map_header->num_vertex * 5 ) );

			ptr_arr_height = (GLuint  *) ( buffer + 40 + ptr_map_header->num_object * 4 * 1 );
			ptr_arr_color  = (GLubyte *) ( buffer + 40 + ptr_map_header->num_object * 4 * 2 );
			ptr_arr_vertex = (GLshort *) ( buffer + 40 + ptr_map_header->num_object * 4 * 3 );

			break;
	}
	
	for ( idx_object = 0, off_vertex = 0, off_coord = 0; idx_object < ptr_map_header->num_object; idx_object++ )
	{
		for ( idx_vertex = 0, num_vertex = ptr_num_vertex[ idx_object ]; idx_vertex < num_vertex; idx_vertex++, off_vertex++ )
		{
			arr_vertex[ off_vertex ].m_x = XMG_I2F ( ptr_arr_vertex[ off_vertex * 2 + 0 ] );	
			arr_vertex[ off_vertex ].m_y = XMG_I2F ( ptr_arr_vertex[ off_vertex * 2 + 1 ] );
			arr_vertex[ off_vertex ].m_z = ptr_map_header->type_object == 25 ? XMG_I2F ( ptr_arr_height[ idx_object ] / 3.0f ) : 0;
			
			if ( ptr_map_header->type_object == 25 )
			{
				arr_coord[ off_coord + idx_vertex ] = XMGVector2F ( 0.0f, 0.0f );

				arr_coord[ off_coord + num_vertex + idx_vertex * 4 + 0 ] = XMGVector2F ( 0.0f, 1.0f );
				arr_coord[ off_coord + num_vertex + idx_vertex * 4 + 1 ] = XMGVector2F ( 0.0f, 0.0f );
				arr_coord[ off_coord + num_vertex + idx_vertex * 4 + 2 ] = XMGVector2F ( 1.0f, 0.0f );
				arr_coord[ off_coord + num_vertex + idx_vertex * 4 + 3 ] = XMGVector2F ( 1.0f, 1.0f );
			}
		}

		arr_color[ idx_object ].m_r = ptr_arr_color[ idx_object * 4 + 2 ] / 255.f;
		arr_color[ idx_object ].m_g = ptr_arr_color[ idx_object * 4 + 1 ] / 255.f;
		arr_color[ idx_object ].m_b = ptr_arr_color[ idx_object * 4 + 0 ] / 255.f;
		arr_color[ idx_object ].m_a = ( 255 - ptr_arr_color[ idx_object * 4 + 3 ] ) / 255.f;

		off_coord += ptr_num_vertex[ idx_object ] * 5;
	}

	switch ( ptr_map_header->type_object )
	{
		case 1 :

			polyline = new XMGRender ( ptr_map_header->num_object, ptr_num_vertex );

			polyline->SetVertexArray ( arr_vertex );
			polyline->SetColors ( arr_color );
			polyline->SetDispMode ( XMG_DISP_LINE );
			polyline->SetDispLine ( XMG_DISP_LINE_STRIP );
	
			tls->xmg_data[ index ].type = 1;
			tls->xmg_data[ index ].primitive = (void *) polyline;
			break;

		case 20 :

			polygon = new XMGPolygon ( ptr_map_header->num_object, ptr_num_vertex );

			polygon->SetVertexArray ( arr_vertex );
			polygon->SetColors ( arr_color );
			polygon->SetWind ( XMG_WIND_CLOCK );

			tls->xmg_data[ index ].type = 2;
			tls->xmg_data[ index ].primitive = (void *) polygon;
			
			break;

		case 25 :

			polygon = new XMGPolygon ( ptr_map_header->num_object, ptr_num_vertex, XMG_GEO_BPILLAR );

			polygon->SetVertexArray ( arr_vertex );
			polygon->XMGRender::SetCoordArray ( arr_coord );
			polygon->SetColors ( arr_color );
			polygon->XMGRender::SetTexture ( tls->xmg_tex[ pattern ] );
			polygon->SetDispMode ( XMG_DISP_TEXTURE );
			polygon->SetWind ( XMG_WIND_CLOCK );
			polygon->SetTexEnv ( XMG_TEX_MODULATE );

			tls->xmg_data[ index ].type = 2;
			tls->xmg_data[ index ].primitive = (void *) polygon;
	}

	kdFree ( arr_vertex );
	kdFree ( arr_color );
	kdFree ( buffer );
}

KDvoid xmEventCreate ( KDvoid )
{		
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas  = new XMGCanvas ( );
	tls->xmg_view[0] = new XMGView ( );
	tls->xmg_view[1] = new XMGView ( );
	tls->xmg_font[0] = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_font[1] = new XMGFont ( "/res/font/BrushScriptStd.otf" );	
	tls->xmg_text    = new XMGText ( 3 );
	
	tls->xmg_text->SetFont ( tls->xmg_font[ 0 ], 0 );
	tls->xmg_text->SetText ( "XMGraphics : 2.5D Map", 0 );

	tls->xmg_text->SetFont ( tls->xmg_font[ 1 ], 1 );
	tls->xmg_text->SetColor ( XMGColorF ( 1.0f, 0.6f, 0.3f, 1.0f ), 1 );
	tls->xmg_text->SetLineAlign ( XMG_ALIGN_CENTER, 1 );

	tls->xmg_text->SetFont ( tls->xmg_font[ 0 ], 2 );
	tls->xmg_text->SetText ( "FPS : 0.0f", 2 );

	tls->xmg_view[ 0 ]->SetLookAt ( XMGVector3F ( 250.0f, 270.0f, 30.f ), XMGVector3F ( 0, 0, -1.0f ), XMGVector3F ( 0, 1.0f, 0 ) );
	tls->xmg_view[ 0 ]->Tilt ( 75.f );

	tls->xmg_tex[ 0 ] = new XMGTexture ( "/res/data/bld_06.jpg" );
	tls->xmg_tex[ 1 ] = new XMGTexture ( "/res/data/bld_06_1.jpg" );
	tls->xmg_tex[ 2 ] = new XMGTexture ( "/res/data/bld_06_2.jpg" );
	tls->xmg_tex[ 3 ] = new XMGTexture ( "/res/data/bld_06_3.jpg" );
	tls->xmg_tex[ 4 ] = new XMGTexture ( "/res/data/bld_07.jpg" );
	tls->xmg_tex[ 5 ] = new XMGTexture ( "/res/data/bld_07_1.jpg" );
	tls->xmg_tex[ 6 ] = new XMGTexture ( "/res/data/bld_07_2.jpg" );
	tls->xmg_tex[ 7 ] = new XMGTexture ( "/res/data/bld_07_3.jpg" );

	LoadData (  0, 0, "/res/data/polyline_00.jbt" );
	LoadData (  1, 0, "/res/data/polyline_01.jbt" );
	LoadData (  2, 0, "/res/data/polyline_02.jbt" );
	LoadData (  3, 0, "/res/data/polyline_03.jbt" );
	LoadData (  4, 0, "/res/data/polyline_04.jbt" );
	LoadData (  5, 0, "/res/data/polygon_00.jbt" );
	LoadData (  6, 0, "/res/data/polygon_01.jbt" );
	LoadData (  7, 0, "/res/data/polygon_02.jbt" );
	LoadData (  8, 0, "/res/data/polygon25_00.jbt" );
	LoadData (  9, 4, "/res/data/polygon25_01.jbt" );
	LoadData ( 10, 1, "/res/data/polygon25_03.jbt" );
	LoadData ( 11, 4, "/res/data/polygon25_04.jbt" );
	LoadData ( 12, 5, "/res/data/polygon25_05.jbt" );
	LoadData ( 13, 6, "/res/data/polygon25_06.jbt" );
	LoadData ( 14, 7, "/res/data/polygon25_07.jbt" );
	LoadData ( 15, 0, "/res/data/polygon25_09.jbt" );
	LoadData ( 16, 1, "/res/data/polygon25_11.jbt" );
	LoadData ( 17, 2, "/res/data/polygon25_12.jbt" );
	LoadData ( 18, 3, "/res/data/polygon25_17.jbt" );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	for ( GLuint idx = 0; idx < 19; idx++ )
	{
		switch ( tls->xmg_data[ idx ].type )
		{
			case 1 : delete (XMGRender  *) tls->xmg_data[ idx ].primitive; break;
			case 2 : delete (XMGPolygon *) tls->xmg_data[ idx ].primitive; break;
		}		
	}

	for ( GLuint idx = 0; idx < 8; idx++ )
	{
		delete tls->xmg_tex[ idx ];
	}

	delete tls->xmg_text;
	delete tls->xmg_font[0];
	delete tls->xmg_font[1];
	delete tls->xmg_view[0];	
	delete tls->xmg_view[1];
	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas->Clear ( );	

	tls->xmg_view[ 0 ]->Update ( );

	for ( GLuint idx = 0; idx < 19; idx++ )
	{
		switch ( tls->xmg_data[ idx ].type )
		{
			case 1 : ( (XMGRender  *) tls->xmg_data[ idx ].primitive )->Render ( );	break;
			case 2 : ( (XMGPolygon *) tls->xmg_data[ idx ].primitive )->Render ( );	break;
		}
	}	

	tls->xmg_view[ 1 ]->Update ( );
	tls->xmg_text->Render ( );

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{	
	KD_GET_TLS ( KDTLS, tls );

	XMGMatrix4F	 mat;

	switch ( tls->app_index )
	{
		case 0 :

			if ( tls->app_count == 0 )
			{
				tls->xmg_text->SetText ( "ZOOM IN", 1 );
			}
			else if ( tls->app_count < 100 )
			{
				tls->xmg_view[ 0 ]->Walk ( 0.8f );
			}
			else if ( tls->app_count == 100 )
			{
				tls->xmg_text->SetText ( "ZOOM OUT", 1 );
			}
			else if ( tls->app_count < 200 )
			{
				tls->xmg_view[ 0 ]->Walk ( -0.8f );
			}
			else
			{
				tls->app_index++;
				tls->app_count = 0;
				return;
			}

			break;

		case 1 :

			if ( tls->app_count == 0 )
			{
				tls->xmg_text->SetText ( "LATITUDE UP", 1 );
			}
			else if ( tls->app_count < 100 )
			{
				tls->xmg_view[ 0 ]->Fly ( 2.0f );
			}
			else if ( tls->app_count == 100 )
			{
				tls->xmg_text->SetText ( "LATITUDE DOWN", 1 );
			}
			else if ( tls->app_count < 200 )
			{
				tls->xmg_view[ 0 ]->Fly ( -2.0f );
			}
			else
			{
				tls->app_index++;
				tls->app_count = 0;
				return;
			}

			break;

		case 2 :

			if ( tls->app_count == 0 )
			{
				tls->xmg_text->SetText ( "LONGITUDE UP", 1 );
			}
			else if ( tls->app_count < 100 )
			{
				tls->xmg_view[ 0 ]->Strafe ( 2.0f );
			}
			else if ( tls->app_count == 100 )
			{
				tls->xmg_text->SetText ( "LONGITUDE DOWN", 1 );
			}
			else if ( tls->app_count < 200 )
			{
				tls->xmg_view[ 0 ]->Strafe ( -2.0f );
			}
			else
			{
				tls->app_index++;
				tls->app_count = 0;
				return;
			}

			break;

		case 3 :

			if ( tls->app_count == 0 )
			{
				tls->xmg_text->SetText ( "ROTATE RIGHT", 1 );
			}
			else if ( tls->app_count < 100 )
			{
				tls->xmg_view[ 0 ]->RotateZ ( -1.5f );
			}
			else if ( tls->app_count == 100 )
			{
				tls->xmg_text->SetText ( "ROTATE LEFT", 1 );
			}
			else if ( tls->app_count < 200 )
			{
				tls->xmg_view[ 0 ]->RotateZ ( 1.5f );
			}
			else
			{
				tls->app_index = 0;
				tls->app_count = 0;
				return;
			}

			break;

		case 4 :

			if ( tls->app_count == 0 )
			{
				tls->xmg_text->SetText ( "TILT UP", 1 );
			}
			else if ( tls->app_count < 100 )
			{
				tls->xmg_view[ 0 ]->Tilt ( 0.5f );
			}
			else if ( tls->app_count == 100 )
			{
				tls->xmg_text->SetText ( "TILT DOWN", 1 );
			}
			else if ( tls->app_count < 200 )
			{
				tls->xmg_view[ 0 ]->Tilt ( -0.5f );
			}
			else
			{
				tls->app_index = 0;
				tls->app_count = 0;
				return;
			}

			break;
	}

	tls->app_count++;

	KDchar  fps[128];
	kdSprintfKHR ( fps, "FPS : %.2f", xmMeasureFPS ( ) );
	tls->xmg_text->SetText ( fps, 2 );
}

KDvoid xmEventResize ( KDint width, KDint height )
{		
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF  wnd = XMGRectF ( 0, 0, 0, XMG_I2F ( width ), XMG_I2F ( height ) );	
	GLfloat	  aspect = wnd.m_w / wnd.m_h;
	
	XMGMatrix4F	 mat[2];

	mat[ 0 ].Perspective ( 45.0f, aspect, 0.1f, 500.0f );
	mat[ 1 ].Ortho ( 0.0f, wnd.m_w, 0.0f, wnd.m_h, -1.0f, 100.0f );

	for ( GLuint idx = 0; idx < 2; idx++ )
	{
		tls->xmg_view[ idx ]->SetViewport ( wnd );
		tls->xmg_view[ idx ]->SetProject ( mat[ idx ] );	
	}
		
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h / 5.0f - 20.0f ), 1 );	
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, 5.0f ), 2 );	
	tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );	
}

KDvoid xmEventTouchUp ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventTouchDown ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventTouchMove ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventKeyUp ( KDint code )
{

}

KDvoid xmEventKeyDown ( KDint code )
{

}

KDvoid xmEventAcceleration ( KDfloat x, KDfloat y, KDfloat z )
{

}

KDvoid xmEventVisible ( KDbool state )
{

}

KDvoid xmEventFocus ( KDbool state )
{

}

KDvoid xmEventUser ( const KDEvent* event )
{

}
