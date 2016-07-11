/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Transparent animation demo
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

#define	GRID_X            30
#define	GRID_Y            30
#define INTERVAL          3000

static KDvoid SetAnimate ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLfloat  ani[ ] = 
	{
		0.0f,	
		1.0f,												// alpha

		500.0f,	
		0.0f,
	};

	tls->xmg_ani->SetKeyFrameByMask ( XMG_ANI_ALPHA, 2, ani );
}

KDvoid SetQuad ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	GLuint  row = 0;
	GLuint  col = 0;
	GLuint  idx = 0;

	XMGRectF  v;
	XMGRectF  c;		

	XMGVector2F  size;
	
	tls->xmg_tex[ 0 ]->GetImageSize ( size );

	v.m_w = XMG_I2F ( tls->wnd_cx ) / GRID_X;
	v.m_h = XMG_I2F ( tls->wnd_cy ) / GRID_Y;

	c.m_w = size.m_x / GRID_X;
	c.m_h = size.m_y / GRID_Y;

	for ( row = 0; row < GRID_Y; row++ )
	{
		for ( col = 0; col < GRID_X; col++ )
		{
			tls->xmg_quad[ 0 ]->SetVertexArray ( &v, idx );
			tls->xmg_quad[ 1 ]->SetVertexArray ( &v, idx );

			tls->xmg_quad[ 0 ]->SetTexture ( tls->xmg_tex[ 0 ], c, XMG_TEX_UNIT_0, idx );
			tls->xmg_quad[ 1 ]->SetTexture ( tls->xmg_tex[ 1 ], c, XMG_TEX_UNIT_0, idx );

			v.m_x += v.m_w;
			c.m_x += c.m_w;

			idx++;
		}

		v.m_x = 0;
		c.m_x = 0;

		v.m_y += v.m_h;
		c.m_y += c.m_h;
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
	tls->xmg_ani       = new XMGAnimate ( );
	tls->xmg_quad[ 0 ] = new XMGQuad ( GRID_X * GRID_Y );
	tls->xmg_quad[ 1 ] = new XMGQuad ( GRID_X * GRID_Y );
	
	tls->xmg_canvas->ClearColor ( XMGColorF ( 0, 0, 0, 1.0f ) );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Transparent" );

	tls->xmg_quad[ 0 ]->SetBlend ( XMG_TRUE );
	tls->xmg_quad[ 1 ]->SetBlend ( XMG_TRUE );
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	delete tls->xmg_quad[0];
	delete tls->xmg_quad[1];
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

	KDust  update;
	KDint  flip;
	KDint  i;

	GLuint  row;
	GLuint  col;

	GLuint  row2;
	GLuint  col2;
	
	KDint   idx[4];

	XMGColorF  color[2];

	flip = tls->app_flip;

	if ( t > INTERVAL )
	{
		t = 0;
		tls->app_msec = msec;
		tls->app_flip = !tls->app_flip;

		tls->xmg_quad[ 0 ]->XMGRender::SetTexture ( tls->xmg_tex[ flip ? 0 : 1 ] );
		tls->xmg_quad[ 1 ]->XMGRender::SetTexture ( tls->xmg_tex[ flip ? 1 : 0 ] );
	}

	for ( row = 0; row < GRID_Y / 2; row++ )
	{
		update = t - row * 30;
		row2 = flip ? GRID_Y / 2 - row - 1 : row;

		for ( col = 0; col < GRID_X / 2; col++ )
		{
			update = t - row * 30 - col * 30;

			tls->xmg_ani->Update ( update < 0 ? 0 : update, 0, &color[0] );
			color[1].m_a = 1.0f - color[0].m_a;
	
			col2 = flip ? GRID_X / 2 - col - 1 : col;

			idx[0] = row2 * GRID_X + col2;
			idx[1] = row2 * GRID_X + GRID_X - col2 - 1;
			idx[2] = ( GRID_Y - row2 - 1 ) * GRID_X + col2;
			idx[3] = ( GRID_Y - row2 - 1 ) * GRID_X + GRID_X - col2 - 1;

			for ( i = 0; i < 4; i++ )
			{
				tls->xmg_quad[ 0 ]->SetColor ( color[ 0 ], idx[i] );
				tls->xmg_quad[ 1 ]->SetColor ( color[ 1 ], idx[i] );
			}
		}
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
