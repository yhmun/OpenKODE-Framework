/*
 *
 *      File            XMGShape.h
 *      Description     Shape class 
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

#ifndef __XMGShape_h__
#define __XMGShape_h__

class XMGContour;
class XMGFacet;
class XMGShape;

typedef STLVector <XMGContour *>	VECContour;
typedef STLVector <XMGFacet *>		VECFacet;	
typedef STLVector <XMGShape *>		VECShape;

typedef enum 
{	
	XMG_LIMIT_NONE				= 0x0000,
	XMG_LIMIT_LINE				= 0x0001,
	XMG_LIMIT_TRI				= 0x0002,
} XMGDrawLimit;

class XMGContour
{
	public :

		XMGContour ( void )
		{
			m_draw_limit = XMG_LIMIT_NONE;
			m_disp_mode  = 0;
			m_num_vertex = 0;
			m_off_vertex = 0;
		}

		XMGDrawLimit	m_draw_limit;
		GLenum			m_disp_mode;
		GLuint			m_num_vertex;
		GLuint			m_off_vertex;
};

class XMGFacet
{
	public :

		XMGFacet ( void )
		{
			m_is_side	  = XMG_FALSE;
			m_num_vertex  = 0;
			m_off_vertex  = 0;

			m_num_ext     = 0;
			m_off_ext	  = 0;

			m_num_index   = 0;
			m_off_index   = 0;

			m_type_color  = XMG_TYPE_FIXED;
			m_visible     = XMG_TRUE;
			m_tex_env     = XMG_TEX_REPLACE;
			m_tex_combine = XMG_COMBINE_MODULATE;

			m_id_textures[ 0 ] = 0;
			m_id_textures[ 1 ] = 0;
		}

		~XMGFacet ( void )
		{
			ClearContour ( );
		}

		void	ClearContour ( void )
		{
			GLuint		idx_contour;
			GLuint		num_contour;
			
			for ( idx_contour = 0, num_contour = m_vec_contour.size ( ); idx_contour < num_contour; idx_contour++ )
			{
				delete m_vec_contour[ idx_contour ];
			}

			m_vec_contour.clear ( );
		}

		XMGBool			m_is_side;
		GLuint			m_num_vertex;
		GLuint			m_off_vertex;

		GLuint			m_num_ext;
		GLuint			m_off_ext;

		GLuint			m_num_index;
		GLuint			m_off_index;

		XMGType			m_type_color;
		XMGColor		m_color;

		XMGBool			m_visible;
		XMGTexEnv		m_tex_env;
		XMGCombine		m_tex_combine;	

		GLuint			m_id_textures[2];
		VECContour		m_vec_contour;
};

class XMGShape 
{
	public :

		XMGShape ( void )
		{
			m_num_input  = 0;
			m_num_basic  = 0;
			m_idx_hole   = XMG_HOLE_NULL;

			m_num_vertex = 0;
			m_off_vertex = 0;

			m_disp_mode	 = XMG_DISP_FILL;
			m_disp_line	 = XMG_DISP_LINES;
			m_disp_tri	 = XMG_DISP_TRIANGLES;

			m_cull		 = XMG_CULL_BACK;
			m_wind		 = XMG_WIND_ANTI_CLOCK;

			m_blend		 = XMG_FALSE;
			m_depth		 = XMG_TRUE;

			m_sfactor	 = XMG_BLEND_SRC_ALPHA;
			m_dfactor	 = XMG_BLEND_ONE_MINUS_SRC_ALPHA;

			m_type_position = (XMGType) 0;
			m_type_matrix   = (XMGType) 0;
		}

		~XMGShape ( void )
		{
			ClearFacet ( );
		}

		void	ClearFacet ( void )
		{
			GLuint		idx_facet;
			GLuint		num_facet;

			for ( idx_facet = 0, num_facet = m_vec_facet.size ( ); idx_facet < num_facet; idx_facet++ )
			{
				delete m_vec_facet[ idx_facet ];
			}
			
			m_vec_facet.clear ( );
		}
		
		GLuint			m_num_input;
		GLuint			m_num_basic;
		GLuint			m_idx_hole;

		GLuint			m_num_vertex;
		GLuint			m_off_vertex;

		XMGDispMode		m_disp_mode;
		XMGDispLine		m_disp_line;
		XMGDispTri		m_disp_tri;

		XMGCull			m_cull;
		XMGWind			m_wind;

		XMGBool			m_blend;
		XMGBool			m_depth;

		XMGBlend		m_sfactor;
		XMGBlend		m_dfactor;

		XMGVector3		m_position;
		XMGMatrix4		m_matrix;

		XMGType			m_type_position;
		XMGType			m_type_matrix;

		VECFacet		m_vec_facet;
};

#endif 