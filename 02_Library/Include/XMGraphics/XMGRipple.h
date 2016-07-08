/*
 *
 *      File            XMGRipple.h
 *      Description     Ripple Effect class
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

#ifndef __XMGRipple_h__
#define __XMGRipple_h__

class XMGRipple : public XMGRender
{
	public :

		 XMGRipple ( void );
		~XMGRipple ( void );

		void	Render ( void );

		void	SetRipple ( GLint x, GLint y, GLuint step = 7 );

		void	SetVertexArray ( GLfloat width, GLfloat height );
		void	SetVertexArray ( GLfixed width, GLfixed height );

		void	SetTexture ( const XMGTexture* texture, XMGFlip flip = XMG_FLIP_NULL );
		void	SetTexture ( const XMGTexture* texture, const XMGRectF& offset );
		void	SetTexture ( const XMGTexture* texture, const XMGRectX& offset );

	protected :

		void*	m_impl;
};

#endif 



