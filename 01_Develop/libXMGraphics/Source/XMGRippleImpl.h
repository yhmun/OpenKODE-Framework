/*
 *
 *      File            XMGRippleImpl.h
 *      Description     Ripple effect implemation 
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

#ifndef __XMGRippleImpl_h__
#define __XMGRippleImpl_h__

#define	XMG_RIPPLE_COUNT		10

typedef struct XMGRippleData
{
	GLint		m_cx;
	GLint		m_cy;
	GLint		m_t;
	GLint		m_max;
	GLint		m_step;
} XMGRippleData;

class XMGRippleImpl
{	
	friend class XMGRipple;

	private : 

		 XMGRippleImpl ( void );
		~XMGRippleImpl ( void );	

		GLfloat		GetDistance ( GLint gx, GLint gy, GLint cx, GLint cy );
		GLint		GetMaxDistance ( GLint gx, GLint gy, GLint cx, GLint cy );

		XMGRippleData	m_data[ XMG_RIPPLE_COUNT ];
		XMGVector2F		m_size;

		XMGType			m_type_coord;
		XMGVector2*		m_arr_coord[2];
};

#endif 
