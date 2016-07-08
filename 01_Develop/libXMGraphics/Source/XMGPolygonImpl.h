/*
 *
 *      File            XMGPolygonImpl.h
 *      Description     Polygon implemation
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

#ifndef __XMGPolygonImpl_h__
#define __XMGPolygonImpl_h__

typedef enum 
{	
	XMG_TESS_VERTEX					= 0x0000,
	XMG_TESS_COLOR					= 0x0001,
	XMG_TESS_COORD					= 0x0002,
} XMGTessMode;

class XMGTess
{
	public :

		XMGTess ( GLuint num_vertex )
		{
			m_tess_mode  = XMG_TESS_VERTEX;	
			XMGAssert ( m_arr_vertex = (GLfloat *) kdMalloc ( sizeof ( GLfloat ) * 8 * num_vertex ) );
		}

		~XMGTess ( void )
		{
			ClearVector ( );
			kdFree ( m_arr_vertex );
		}

		void	Init ( XMGTessMode mode )
		{
			ClearVector ( );
			m_tess_mode = mode;
		}

		void	ClearVector ( void )
		{
			m_vec_mode.clear ( );
			m_vec_num_vertex.clear ( );
			m_vec_vertex.clear ( );
			m_vec_coord.clear ( );
			m_vec_color.clear ( );
		}

		STLVector<GLenum>		m_vec_mode;
		STLVector<GLuint>		m_vec_num_vertex;
		STLVector<XMGVector3F>	m_vec_vertex;
		STLVector<XMGVector2F>	m_vec_coord;
		STLVector<XMGColorF>	m_vec_color;

		XMGTessMode		m_tess_mode;
		GLfloat*		m_arr_vertex;		
};

class XMGPolygonImpl
{	
	friend class XMGPolygon;

	private :

		 XMGPolygonImpl ( GLuint num_shape, const GLuint* arr_num_vertex, XMGBool* has_face );
		~XMGPolygonImpl ( void );	

	public :

		GLuint			m_num_shape;
		XMGBool			m_need_tess;

		GLUtesselator*	m_tess[2];
		XMGTess**		m_data[2];
};

#endif 
