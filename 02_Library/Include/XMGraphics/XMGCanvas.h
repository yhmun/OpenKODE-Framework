/*
 *
 *      File            XMGCanvas.h
 *      Description     XMGraphics : Canvas 
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

#ifndef __XMGCanvas_h__
#define __XMGCanvas_h__

class XMGCanvas
{
	public :

		 XMGCanvas ( void );
		~XMGCanvas ( void );

		void	Clear ( void );
		void	Flush ( void );
		
		void	ClearColor ( const XMGColorF& color );	
		void	ClearColor ( const XMGColorX& color );	

		void	LoadIdentity ( void );

		void	PushMatrix ( void );
		void	PopMatrix  ( void );

		void	MulMatrix ( const XMGMatrix4F& mat );
		void	MulMatrix ( const XMGMatrix4X& mat );

		void	Translate ( const XMGVector3F& vec );
		void	Translate ( const XMGVector3X& vec );

		void	Rotate ( GLfloat angle, const XMGVector3F& vec );
		void	Rotate ( GLfixed angle, const XMGVector3X& vec );

		void	Scale ( const XMGVector3F& vec );
		void	Scale ( const XMGVector3X& vec );
		
		void	Viewport ( const XMGRectF& rect );
		void	Viewport ( const XMGRectX& rect );

		void	Ortho ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar );
		void	Ortho ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar );
		
		void	Perspective ( GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar );
		void	Perspective ( GLfixed fovy, GLfixed aspect, GLfixed znear, GLfixed zfar );
		void	Perspective ( GLuint width, GLuint height );

	protected :

		void*	m_impl;
};

#endif 



