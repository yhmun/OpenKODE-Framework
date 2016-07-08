/*
 *
 *      File            XMGText.h
 *      Description     Text class 
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

#ifndef __XMGText_h__
#define __XMGText_h__

typedef enum 
{		
	XMG_ALIGN_LEFT                  = 0x0000,
	XMG_ALIGN_CENTER                = 0x0001,		
	XMG_ALIGN_RIGHT                 = 0x0002,		
	XMG_ALIGN_JUSTIFY               = 0x0003,
} XMGAlign;	

class XMGText : public XMGRender
{
	public :

		 XMGText ( GLuint num_shape = 1 );
		~XMGText ( void );

		void	Render		  ( GLuint idx_shape = XMG_SHAPE_ALL );
		
		void	SetCull		  ( XMGCull   cull, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetFont		  ( XMGFont*  font, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetLineLength ( GLfloat length, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetLineLength ( GLfixed length, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetLineSpace  ( GLfloat  space, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetLineSpace  ( GLfixed  space, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetLineAlign  ( XMGAlign align, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetText ( const KDchar*   text, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetUTF8 ( const KDchar*   text, GLuint idx_shape = XMG_SHAPE_ALL );

	private :

		void*	m_impl;
};

#endif



