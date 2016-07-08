/*
 *
 *      File            XMGRender.h
 *      Description     Render class
 *      Version         0.20.0801, 2011-08-01
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2011 XMSoft. All rights reserved.
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

#ifndef __XMGRender_h__
#define __XMGRender_h__
 
typedef enum 
{
	XMG_GEO_REGULAR                 = 0x0001,	
	XMG_GEO_SOLID                   = 0x0002,
	XMG_GEO_PILLAR                  = 0x0003,
	XMG_GEO_WALL                    = 0x0004,	
	XMG_GEO_BSOLID                  = 0x0005,	
	XMG_GEO_BPILLAR                 = 0x0006,
	XMG_GEO_BWALL                   = 0x0007,	
	XMG_GEO_CONE                    = 0x0008,
	XMG_GEO_CWALL                   = 0x0009,
} XMGGeoType;

typedef enum 
{
	XMG_DISP_POINT                  = 0x0001,			
	XMG_DISP_LINE                   = 0x0002,
	XMG_DISP_FILL                   = 0x0003,	
	XMG_DISP_TEXTURE                = 0x0004,
} XMGDispMode;

typedef	enum
{
	XMG_DISP_LINES                  = 0x0001,
	XMG_DISP_LINE_LOOP              = 0x0002,
	XMG_DISP_LINE_STRIP             = 0x0003,
} XMGDispLine;

typedef enum
{
	XMG_DISP_TRIANGLES              = 0x0004,
	XMG_DISP_TRIANGLE_STRIP         = 0x0005,
	XMG_DISP_TRIANGLE_FAN           = 0x0006,
} XMGDispTri;

typedef enum 
{		
	XMG_CULL_DISABLE                = 0x0000,
	XMG_CULL_FRONT                  = 0x0001,
	XMG_CULL_BACK                   = 0x0002,	
} XMGCull;

typedef enum
{
	XMG_WIND_CLOCK                  = 0x0900,
	XMG_WIND_ANTI_CLOCK             = 0x0901,	
} XMGWind;

typedef enum
{
	XMG_BLEND_ZERO                  = 0,
	XMG_BLEND_ONE                   = 1,
	XMG_BLEND_SRC_COLOR             = 0x0300,
	XMG_BLEND_ONE_MINUS_SRC_COLOR   = 0x0301,
	XMG_BLEND_SRC_ALPHA             = 0x0302,
	XMG_BLEND_ONE_MINUS_SRC_ALPHA   = 0x0303,
	XMG_BLEND_DST_ALPHA             = 0x0304,
	XMG_BLEND_ONE_MINUS_DST_ALPHA   = 0x0305,
	XMG_BLEND_DST_COLOR             = 0x0306,
	XMG_BLEND_ONE_MINUS_DST_COLOR   = 0x0307,
	XMG_BLEND_SRC_ALPHA_SATURATE    = 0x0308,
} XMGBlend;

typedef enum
{
	XMG_TEX_UNIT_0                  = 0,
	XMG_TEX_UNIT_1                  = 1,
} XMGTexUnit;

typedef enum
{
	XMG_TEX_REPLACE                 = 0x1E01,
	XMG_TEX_MODULATE                = 0x2100,				
} XMGTexEnv;

typedef enum
{
	XMG_COMBINE_MODULATE            = 0x2100,
	XMG_COMBINE_ADD                 = 0x0104,
	XMG_COMBINE_ADD_SIGNED          = 0x8574,
	XMG_COMBINE_REPLACE             = 0x1E01,
	XMG_COMBINE_SUBTRACT            = 0x84E7,
} XMGCombine;

class XMGRender 
{
	public :

		 XMGRender ( GLuint num_shape, GLuint num_vertex, XMGGeoType type = XMG_GEO_REGULAR );
		 XMGRender ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type = XMG_GEO_REGULAR );
		~XMGRender ( void );

		void	Render			( GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetDispMode		( XMGDispMode mode, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetDispLine		( XMGDispLine mode, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetDispTri		( XMGDispTri  mode, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetCull			( XMGCull mode, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetWind			( XMGWind mode, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetBlend		( XMGBool enable, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetDepth		( XMGBool enable, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetBlendOP		( XMGBlend sfactor, XMGBlend dfactor, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetPosition		( const XMGVector3F& position, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetPosition		( const XMGVector3X& position, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetMatrix		( const XMGMatrix4F& matrix, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetMatrix		( const XMGMatrix4X& matrix, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetAnimate		( const XMGAnimate* animate, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetColor		( const XMGColorF& color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetColor		( const XMGColorX& color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	SetVisible		( XMGBool    visible, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetTexEnv       ( XMGTexEnv  tex_env, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetTexCombine	( XMGCombine combine, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );		

		void	SetTexture		( GLuint id, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetTexture		( const XMGTexture* texture, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	SetVertexArray	( const XMGVector3F* arr_vertex, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetVertexArray	( const XMGVector3X* arr_vertex, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetCoordArray	( const XMGVector2F* arr_coord, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetCoordArray	( const XMGVector2X* arr_coord, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	SetColorArray	( const XMGColorF* arr_color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetColorArray	( const XMGColorX* arr_color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
			
		void	SetColors		( const XMGColorF* colors, XMGBool is_facet = XMG_FALSE );
		void	SetColors		( const XMGColorX* colors, XMGBool is_facet = XMG_FALSE );

		void	SetIndexArray	( const GLushort* arr_index, const GLuint* arr_num_index );

	protected :				
				
		void*	m_impl;
};

#endif 



