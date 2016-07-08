/*
 *
 *      File            XMGQuadImpl.h
 *      Description     Quadrilateral implemation 
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

#ifndef __XMGQuadImpl_h__
#define __XMGQuadImpl_h__

class XMGQuadImpl
{	
	friend class XMGQuad;

	private :

		 XMGQuadImpl ( XMGGeoType type );
		~XMGQuadImpl ( void );	

	public :

		void	Rect2Vector ( XMGVector3F* vec, XMGRectF& rect, XMGWind wind );
		void	Rect2Vector ( XMGVector3X* vec, XMGRectX& rect, XMGWind wind );

		XMGGeoType		m_geo_type;
		GLuint			m_tex_indices[4];

};

#endif 
