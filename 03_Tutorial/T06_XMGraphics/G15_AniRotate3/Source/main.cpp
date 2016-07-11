/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Rotation animation demo 3
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

XM_DEF_APP_MAIN_DEFAULT ( XM_SYS_DEFAULT )

#define	GRID_X            4
#define	GRID_Y            6

static KDvoid SetAnimate ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLfloat  axis_x = XMG_I2F ( tls->wnd_cx ) / GRID_X / 2;
	GLfloat  axis_y = XMG_I2F ( tls->wnd_cy ) / GRID_Y / 2;

	GLfloat  ani[ ] = 
	{
		0.0f,	
		axis_x, axis_y, 0.0f,						// axis
		0.0f, 0.0f, 1.0f, 0.0f,						// rotate

		700.0f,	
		axis_x, axis_y, 0.0f,
		220.0f, 0.0f, 1.0f, 0.0f,				

		800.0f,	
		axis_x, axis_y, 0.0f,
		160.0f, 0.0f, 1.0f, 0.0f,		

		900.0f,	
		axis_x, axis_y, 0.0f,
		180.0f, 0.0f, 1.0f, 0.0f,		
	};

	tls->xmg_ani[ 0 ]->SetKeyFrameByMask ( XMG_ANI_AXIS | XMG_ANI_ROTATE, 4, ani );

	for ( GLuint idx = 0; idx < 4; idx++ )
	{
		ani[ idx * 8 + 4 ] = -ani[ idx * 8 + 4 ];
	}

	tls->xmg_ani[ 1 ]->SetKeyFrameByMask ( XMG_ANI_AXIS | XMG_ANI_ROTATE, 4, ani );
}

KDvoid SetQuad ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );


	GLuint  row = 0;
	GLuint  col = 0;
	GLuint  idx = 0;

	XMGRectF     vertex;
	XMGRectF     coord;
	XMGVector3F  pos;	
	XMGVector2F  size;
	
	vertex.m_w = XMG_I2F ( tls->wnd_cx ) / GRID_X;
	vertex.m_h = XMG_I2F ( tls->wnd_cy ) / GRID_Y;

	tls->xmg_tex[ 0 ]->GetImageSize ( size );

	coord.m_w = size.m_x / GRID_X;
	coord.m_h = size.m_y / GRID_Y;

	for ( row = 0; row < GRID_Y; row++ )
	{
		pos.m_y = row * vertex.m_h;
		coord.m_y = row * coord.m_h;

		for ( col = 0; col < GRID_X; col++ )
		{
			pos.m_x = col * vertex.m_w;
			coord.m_x = col * coord.m_w;

			tls->xmg_quad[ 0 ]->SetVertexArray( &vertex, idx );
			tls->xmg_quad[ 1 ]->SetVertexArray( &vertex, idx );

			tls->xmg_quad[ 0 ]->SetPosition ( pos, idx );
			tls->xmg_quad[ 1 ]->SetPosition ( pos, idx );

			tls->xmg_quad[ 0 ]->SetTexture ( tls->xmg_tex[ 0 ], coord, XMG_TEX_UNIT_0, idx );
			tls->xmg_quad[ 1 ]->SetTexture ( tls->xmg_tex[ 1 ], coord, XMG_TEX_UNIT_0, idx );

			idx++;
		}
	}
}

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas    = new XMGCanvas ( );	
	tls->xmg_font      = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_tex[0]    = new XMGTexture ( "/res/image/idle.jpg" );
	tls->xmg_tex[1]    = new XMGTexture ( "/res/image/menu.jpg" );
	tls->xmg_text      = new XMGText ( );
	tls->xmg_ani[0]    = new XMGAnimate ( );
	tls->xmg_ani[1]    = new XMGAnimate ( );
	tls->xmg_quad[ 0 ] = new XMGQuad ( GRID_X * GRID_Y );
	tls->xmg_quad[ 1 ] = new XMGQuad ( GRID_X * GRID_Y );
	
	tls->xmg_canvas->ClearColor ( XMGColorF ( 0, 0, 0, 1.0f ) );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Rotate 3" );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_quad[0];
	delete tls->xmg_quad[1];
	delete tls->xmg_text;
	delete tls->xmg_tex[0];
	delete tls->xmg_tex[1];
	delete tls->xmg_ani[0];	
	delete tls->xmg_ani[1];
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

	GLuint  row  = 0;
	GLuint  col  = 0;
	GLuint  idx  = 0;
	GLuint  flip = 0;

	XMGMatrix4F  mat_ani[ 2 ];
	XMGMatrix4F  mat_inverse;

	if ( t > 2000 )
	{
		t = 0;
		tls->app_msec = msec;

		tls->xmg_quad[ 0 ]->XMGRender::SetTexture ( tls->xmg_tex [ tls->app_flip ? 1 : 0 ] );
		tls->xmg_quad[ 1 ]->XMGRender::SetTexture ( tls->xmg_tex [ tls->app_flip ? 0 : 1 ] );

		tls->app_flip = !tls->app_flip;
	}

	tls->xmg_ani[ 0 ]->Update ( t, &mat_ani[ 0 ], 0 );
	tls->xmg_ani[ 1 ]->Update ( t, &mat_ani[ 1 ], 0 );

	mat_inverse.Translate ( XMG_I2F ( tls->wnd_cx ) / GRID_X / 2, 0, 0 );
	mat_inverse.Rotate ( 180.f, 0.0f, 1.0f, 0.0f );
	mat_inverse.Translate ( -XMG_I2F ( tls->wnd_cx ) / GRID_X / 2, 0, 0 );

	for ( row = 0; row < GRID_Y; row++ )
	{
		for ( col = 0; col < GRID_X; col++ )
		{
			tls->xmg_quad[ 0 ]->SetMatrix ( mat_ani[ flip ], idx );
			tls->xmg_quad[ 1 ]->SetMatrix ( mat_ani[ flip ] * mat_inverse, idx );
	
			flip = !flip;
			idx++;
		}

		flip = !flip;
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
