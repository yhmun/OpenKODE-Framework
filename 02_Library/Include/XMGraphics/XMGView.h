/*
 *
 *      File            XMGView.h
 *      Description     View class
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

#ifndef __XMGView_h__
#define __XMGView_h__

class XMGView 
{
	public :

		 XMGView ( void );
		~XMGView ( void );

		void	Update ( void );
		
		void	SetViewport ( const XMGRectF& rect );
		void	SetViewport ( const XMGRectX& rect );

		void	SetProject ( const XMGMatrix4F& mat );
		void	SetProject ( const XMGMatrix4X& mat );

		void	SetLookAt ( const XMGVector3F& eye, const XMGVector3F& at, const XMGVector3F& up );
		void	SetLookAt ( const XMGVector3X& eye, const XMGVector3X& at, const XMGVector3X& up );
		 
		void	Walk ( GLfloat val );
		void	Walk ( GLfixed val );

		void	Fly ( GLfloat val );
		void	Fly ( GLfixed val );

		void	Strafe ( GLfloat val );
		void	Strafe ( GLfixed val );

		void	Tilt ( GLfloat val );
		void	Tilt ( GLfixed val );

		void	RotateX ( GLfloat val );
		void	RotateX ( GLfixed val );

		void	RotateY ( GLfloat val );
		void	RotateY ( GLfixed val );

		void	RotateZ ( GLfloat val );
		void	RotateZ ( GLfixed val );

		void	Rotate ( GLfloat val, const XMGVector3F& vec );
		void	Rotate ( GLfixed val, const XMGVector3X& vec );

	protected :

		void*	m_impl;
};

#endif 



