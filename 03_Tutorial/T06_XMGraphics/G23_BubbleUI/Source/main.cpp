/*
 *
 *      File            main.cpp
 *      Description     XMGraphics : Bubble UI
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

#define TIME_BACK           5000
#define TIME_ROCK           400
#define TIME_ROCK_BEGIN     400
#define TIME_ROCK_END       2900
#define TIME_LIST           600
#define TIME_BUBBLE         300

#define INTERVAL            8000

#define	IMG_W1              240.f
#define	IMG_H1              400.f

#define	IMG_W2              45.f
#define	IMG_H2              46.f

static KDvoid SetBackground ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	XMGRectF   rc;
	GLuint     idx;
	XMGRotate  rot;

	rc = XMGRectF ( 0, 0, -IMG_W2, IMG_W1, IMG_H1 );

	tls->xmg_tex_back = new XMGTexture ( "/res/image/back.png" );
	tls->xmg_obj_back = new XMGQuad ( 1, XMG_GEO_BPILLAR );	

	tls->xmg_obj_back->SetVertexArray ( &rc );
	tls->xmg_obj_back->SetCull ( XMG_CULL_DISABLE );

	for ( idx = 0; idx < 5; idx++ )
	{
		switch ( idx )
		{
			case 0 :	rot = XMG_ROTATE_0;		rc = XMGRectF ( IMG_W2, IMG_H2, IMG_W1, IMG_H1 );				break;
			case 1 :	rot = XMG_ROTATE_270;	rc = XMGRectF ( 0, IMG_H2, IMG_W2, IMG_H1 );					break;
			case 2 :	rot = XMG_ROTATE_0;		rc = XMGRectF ( IMG_W2, 0, IMG_W1, IMG_H2 );					break;
			case 3 :	rot = XMG_ROTATE_90;	rc = XMGRectF ( IMG_W1 + IMG_W2, IMG_H2, IMG_W2, IMG_H1 );		break;
			case 4 :	rot = XMG_ROTATE_180;	rc = XMGRectF ( IMG_W2, IMG_H1 + IMG_H2, IMG_W1, IMG_H2 );		break;
		}

		tls->xmg_obj_back->TexIndices ( rot );
		tls->xmg_obj_back->SetTexture ( tls->xmg_tex_back, rc, XMG_TEX_UNIT_0, 0, idx );
	}	
}

static KDvoid SetList ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	KDchar  path[128];

	XMGRectF     rc;	
	XMGVector3F  pos;
	XMGVector2F  size;

	for ( GLuint idx = 0; idx < 4; idx++ )
	{
		kdSprintfKHR ( path, "/res/image/list_%02d.png", idx );
		
		tls->xmg_tex_list[ idx ] = new XMGTexture ( path );
		tls->xmg_obj_list[ idx ] = new XMGQuad ( );
		tls->xmg_ani_list[ idx ] = new XMGAnimate ( );

		tls->xmg_tex_list[ idx ]->GetImageSize ( size );
		rc = XMGRectF ( -size.m_x / 2.0f, -size.m_y / 2.0f, size.m_x, size.m_y );
		pos = XMGVector3F ( IMG_W1 / 2.0f, 0, 0 );

		tls->xmg_obj_list[ idx ]->SetVertexArray ( &rc );
		tls->xmg_obj_list[ idx ]->SetTexture ( tls->xmg_tex_list[ idx ] );
		tls->xmg_obj_list[ idx ]->SetPosition ( pos );
		tls->xmg_obj_list[ idx ]->SetBlend ( XMG_TRUE );
		tls->xmg_obj_list[ idx ]->SetDepth ( XMG_FALSE );		
	}
}

static KDvoid SetShadow ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	KDchar	path[128];

	XMGRectF	 rc;	
	XMGVector3F	 pos;
	XMGVector2F	 size;

	for ( GLuint idx = 0; idx < 2; idx++ )
	{
		kdSprintfKHR ( path, "/res/image/shadow_%02d.png", idx );

		tls->xmg_tex_shadow[ idx ] = new XMGTexture ( path );
		tls->xmg_obj_shadow[ idx ] = new XMGQuad ( );

		tls->xmg_tex_shadow[ idx ]->GetImageSize ( size );
		rc = XMGRectF ( -size.m_x / 2.0f, -size.m_y / 2.0f, size.m_x, size.m_y );
		pos = XMGVector3F ( IMG_W1 / 2.0f, 0, -IMG_W2 );

		tls->xmg_obj_shadow[ idx ]->SetVertexArray ( &rc );
		tls->xmg_obj_shadow[ idx ]->SetTexture ( tls->xmg_tex_shadow[ idx ] );
		tls->xmg_obj_shadow[ idx ]->SetPosition ( pos );
		tls->xmg_obj_shadow[ idx ]->SetBlend ( XMG_TRUE );
		tls->xmg_obj_shadow[ idx ]->SetDepth ( XMG_FALSE );
		tls->xmg_obj_shadow[ idx ]->SetColor ( XMGColorF ( 1.0f, 1.0f, 1.0f, 0.3f ) );	
		tls->xmg_obj_shadow[ idx ]->SetTexEnv ( XMG_TEX_MODULATE );
	}	
}

static KDvoid SetBubble ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	KDchar  path[128];

	XMGRectF     rc;	
	XMGVector3F  pos;
	XMGVector2F  size;

	for ( GLuint idx = 0; idx < 2; idx++ )
	{
		for ( GLuint idx2 = 0; idx2 < 6; idx2++ )
		{
			kdSprintfKHR ( path, "/res/image/bubble_%c_%02d.png", 'A' + idx, idx2 + 1 );

			tls->xmg_tex_bubble[ idx ][ idx2 ] = new XMGTexture ( path );
			tls->xmg_obj_bubble[ idx ][ idx2 ] = new XMGQuad ( );

			tls->xmg_tex_bubble[ idx ][ idx2 ]->GetImageSize ( size );
			rc = XMGRectF ( -size.m_x / 2.0f, -size.m_y / 2.0f, size.m_x, size.m_y );
			pos = XMGVector3F ( IMG_W1 / 2.0f, 0, 0 );

			tls->xmg_obj_bubble[ idx ][ idx2 ]->SetVertexArray ( &rc );
			tls->xmg_obj_bubble[ idx ][ idx2 ]->SetTexture ( tls->xmg_tex_bubble[ idx ][ idx2 ] );
			tls->xmg_obj_bubble[ idx ][ idx2 ]->SetPosition ( pos );
			tls->xmg_obj_bubble[ idx ][ idx2 ]->SetBlend ( XMG_TRUE );
		}
	}	
}

static KDvoid SetAnimate ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	XMGVector3F	 axis = XMGVector3F ( IMG_W1, IMG_H1, -IMG_W2 ) / 2.0f;

	GLfloat	 ani[] =
	{
		0.0f,
		0.0f, 0.0f, 0.0f,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 0.0f, 1.0f, 

		500.0f,
		0.0f, 0.0f, axis.m_z * 3,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 1.0f, 1.0f, 

		1000.0f,
		0.0f, 0.0f, axis.m_z * 3,
		axis.m_x, axis.m_y, axis.m_z,
		-30.0f, 0.0f, 1.0f, 1.0f, 

		2000.0f,
		0.0f, 0.0f, axis.m_z * 3,
		axis.m_x, axis.m_y, axis.m_z,
		60.0f, 1.0f, 1.0f, 0.0f, 

		3000.0f,
		0.0f, 0.0f, axis.m_z * 3,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 0.0f, 1.0f, 

		TIME_BACK - 500.0f,
		0.0f, 0.0f, axis.m_z * 3,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 0.0f, 1.0f, 

		TIME_BACK,
		0.0f, 0.0f, 0.0f,
		axis.m_x, axis.m_y, axis.m_z,
		360.0f, 0.0f, 0.0f, 1.0f, 
	};

	tls->xmg_ani[ 0 ]->SetKeyFrameByMask ( XMG_ANI_POSITION | XMG_ANI_AXIS | XMG_ANI_ROTATE, 7, ani );
}

static KDvoid SetAnimate2 ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );

	XMGVector3F	 axis = XMGVector3F ( IMG_W1, IMG_H1, 0 ) / 2.0f;

	GLfloat	 ani[] = 
	{
		0.0f,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 1.0f, 0.0f,

		TIME_ROCK * 0.25f,
		axis.m_x, axis.m_y, axis.m_z,
		-10.0f, 0.0f, 1.0f, 0.0f,

		TIME_ROCK * 0.5f,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 1.0f, 0.0f,

		TIME_ROCK * 0.75f,
		axis.m_x, axis.m_y, axis.m_z,
		10.0f, 0.0f, 1.0f, 0.0f,

		TIME_ROCK,
		axis.m_x, axis.m_y, axis.m_z,
		0.0f, 0.0f, 1.0f, 0.0f,
	};

	tls->xmg_ani[ 1 ]->SetKeyFrameByMask ( XMG_ANI_AXIS | XMG_ANI_ROTATE, 5, ani );
}

static KDvoid SetAniList ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	for ( GLuint idx = 0; idx < 4; idx++ )
	{
		if ( tls->app_show[ idx ] )
		{
			GLfloat	 ani[] = 
			{
				0.0f,
				0.0f, tls->app_prev_y[ idx ], 0.0f,

				TIME_LIST - 100.0f,
				0.0f, tls->app_post_y[ idx ], 0.0f,
			};

			tls->xmg_ani_list[ idx ]->SetKeyFrameByMask ( XMG_ANI_POSITION, 2, ani );
		}

		tls->app_prev_y[ idx ] = tls->app_post_y[ idx ];
	}

	tls->app_time_list = kdGetTimeUST ( );
}

KDvoid xmEventCreate ( KDvoid )
{	
	KD_SET_TLS ( KDTLS );
	KD_GET_TLS ( KDTLS, tls );

	tls->xmg_canvas   = new XMGCanvas ( );	
	tls->xmg_font     = new XMGFont ( "/res/font/COOPBL.TTF" );	
	tls->xmg_text     = new XMGText ( );
	tls->xmg_ani[ 0 ] = new XMGAnimate ( );
	tls->xmg_ani[ 1 ] = new XMGAnimate ( );

	SetBackground ( );
	SetList ( );
	SetShadow ( );
	SetBubble ( );

	SetAnimate ( );
	SetAnimate2 ( );

	tls->xmg_text->SetFont ( tls->xmg_font );
	tls->xmg_text->SetText ( "XMGraphics : Bubble UI" );	
}

KDvoid xmEventDestroy ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	GLuint  idx;
	GLuint  idx2;

	for ( idx = 0; idx < 4; idx++ )
	{
		delete tls->xmg_tex_list[ idx ];
		delete tls->xmg_ani_list[ idx ];
		delete tls->xmg_obj_list[ idx ];
	}

	for ( idx = 0; idx < 2; idx++ )
	{
		delete tls->xmg_tex_shadow[ idx ];
		delete tls->xmg_obj_shadow[ idx ];
		delete tls->xmg_ani[ idx ];
	}

	for ( idx = 0; idx < 2; idx++ )
	{
		for ( idx2 = 0; idx2 < 6; idx2++ )
		{
			delete tls->xmg_tex_bubble[ idx ][ idx2 ];
			delete tls->xmg_obj_bubble[ idx ][ idx2 ];
		}
	}

	delete tls->xmg_obj_back;
	delete tls->xmg_tex_back;

	delete tls->xmg_text;
	delete tls->xmg_font;

	delete tls->xmg_canvas;
}

KDvoid xmEventRedraw ( KDvoid )
{	
	KD_GET_TLS ( KDTLS, tls );

	KDust  time = kdGetTimeUST ( );
	KDust  msec = ( time - tls->app_time ) / 1000000UL;

	XMGMatrix4F  mat;
	
	GLuint  idx;
	GLuint  idx_obj;
	GLuint  idx_bubble;

	if ( msec > INTERVAL )
	{
		msec = 0;
		tls->app_time = time;
	}

	tls->xmg_canvas->Clear ( );

	tls->xmg_canvas->PushMatrix ( );

		tls->xmg_canvas->Scale ( tls->xmg_scale );

		if ( msec <= TIME_BACK )
		{
			tls->xmg_ani[ 0 ]->Update ( msec, &mat, 0 );
			tls->xmg_canvas->MulMatrix ( mat );
		}

		tls->xmg_obj_back->Render ( );

		tls->xmg_canvas->PushMatrix ( );
		
			if ( msec >= TIME_ROCK_BEGIN && msec <= TIME_ROCK_END )
			{
				tls->xmg_ani[ 1 ]->Update ( msec % 400, &mat, 0 );
				tls->xmg_canvas->MulMatrix ( mat );
			}

			for ( idx = 0; idx < 4; idx++ )
			{
				idx_obj = idx == 0 || idx == 3 ? 1 : 0;

				if ( tls->app_show[ idx ] )
				{
					if ( tls->app_time_list )
					{
						time = kdGetTimeUST ( );
						msec = ( time - tls->app_time_list ) / 1000000UL;

						if ( msec < TIME_LIST )
						{
							tls->xmg_ani_list[ idx ]->Update ( msec, &tls->app_mat[ idx ], 0 );
							tls->xmg_obj_list[ idx ]->SetMatrix ( tls->app_mat[ idx ] );							
						}
						else
						{
							tls->app_time_list = 0;
						}
					}

					tls->xmg_obj_shadow[ idx_obj ]->SetMatrix ( tls->app_mat[ idx ] );
					tls->xmg_obj_shadow[ idx_obj ]->Render ( );
					tls->xmg_obj_list[ idx ]->Render ( );	
				}
				else
				{
					if ( tls->app_time_bubble )
					{
						time = kdGetTimeUST ( );

						idx_obj = tls->app_select == 0 || tls->app_select == 3 ? 1 : 0;
						msec = ( time - tls->app_time_bubble ) / 1000000UL;						

						if ( msec < TIME_BUBBLE )
						{
							idx_bubble = (GLuint) msec / ( TIME_BUBBLE / 6 );

							tls->xmg_obj_bubble[ idx_obj ][ idx_bubble ]->SetMatrix ( tls->app_mat[ tls->app_select ] );
							tls->xmg_obj_bubble[ idx_obj ][ idx_bubble ]->Render ( );
						}
						else
						{
							tls->app_time_bubble = 0;
							SetAniList ( );
						}
					}
				}
			}

		tls->xmg_canvas->PopMatrix ( );

	tls->xmg_canvas->PopMatrix ( );

	tls->xmg_text->Render ( );	

	tls->xmg_canvas->Flush ( );
}

KDvoid xmEventUpdate ( KDuint msec )
{

}

KDvoid xmEventResize ( KDint width, KDint height )
{		
	KD_GET_TLS ( KDTLS, tls );

	tls->wnd_cx = width;
	tls->wnd_cy = height;

	tls->xmg_wnd.m_w = XMG_I2F ( width );
	tls->xmg_wnd.m_h = XMG_I2F ( height );

	tls->xmg_scale.m_x = tls->xmg_wnd.m_w / IMG_W1;
	tls->xmg_scale.m_y = tls->xmg_wnd.m_h / IMG_H1;
	tls->xmg_scale.m_z = tls->xmg_scale.m_x;

	tls->xmg_canvas->Viewport ( tls->xmg_wnd );
	tls->xmg_canvas->Perspective ( width, height );
		
	tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, tls->xmg_wnd.m_h - 20.0f ) );
	tls->xmg_text->SetLineLength ( tls->xmg_wnd.m_w - 5.0f );	
}

KDvoid xmEventTouchUp ( KDint id, KDint x, KDint y )
{

}

KDvoid xmEventTouchDown ( KDint id, KDint x, KDint y )
{
	KD_GET_TLS ( KDTLS, tls );

	GLuint          idx;
	GLuint          idx2;
	GLfloat         pos_y;	
	XMGVector2F     size;
	GLint           show;	

	XMGVector2F     touch;

	if ( id == 0 )
	{
		touch.m_x = XMG_I2F ( x ) * IMG_W1 / tls->xmg_wnd.m_w;
		touch.m_y = XMG_I2F ( y ) * IMG_H1 / tls->xmg_wnd.m_h;

		show = 0;
		for ( idx = 0; idx < 4; idx++ )
		{
			show |= tls->app_show[ idx ];

			if ( tls->app_show[ idx ] && tls->app_time_list == 0 && tls->app_time_bubble == 0 )
			{
				tls->xmg_tex_list[ idx ]->GetImageSize ( size );				
				size.m_y /= 2.f;

				if ( ( tls->app_prev_y[ idx ] - size.m_y ) < touch.m_y && ( tls->app_prev_y[ idx ] + size.m_y ) > touch.m_y )
				{
					tls->app_time_bubble = kdGetTimeUST ( );
					tls->app_show[ idx ] = 0;
					tls->app_select = idx;
					size.m_y *= 2.f;

					for ( idx2 = idx + 1; idx2 < 4; idx2++ )
					{ 
						tls->app_post_y[ idx2 ] += size.m_y;
					}

					return;
				}
			}
		}		

		if ( !show )
		{
			pos_y = 360.0f;

			for ( idx = 0; idx < 4; idx++ )
			{
				tls->xmg_tex_list[ idx ]->GetImageSize ( size );

				size.m_y /= 2.f;
				pos_y -= size.m_y;
				
				tls->app_prev_y[ idx ] = 400.f - size.m_y;
				tls->app_post_y[ idx ] = pos_y;
				tls->app_show[ idx ] = 1;		

				pos_y -= size.m_y;
			}

			SetAniList ( );
		}
	}
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
