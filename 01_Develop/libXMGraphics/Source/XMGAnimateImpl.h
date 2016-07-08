/*
 *
 *      File            XMGAnimateImpl.h
 *      Description     Animation implemation
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

#ifndef __XMGAnimateImpl_h__
#define __XMGAnimateImpl_h__

class XMGAnimateImpl
{
	friend class XMGAnimate;	

	private :

		 XMGAnimateImpl ( void );
		~XMGAnimateImpl ( void );	

		void	Update ( KDust msec, GLuint* out_frame, XMGType out_type );
		void	Update ( KDust msec, XMGMatrix4* out_matrix, XMGColor* out_color, XMGType out_type );

		void	SetKeyFrame ( GLuint size, GLuint count, const GLuint* arr_frame, XMGType arr_type );
		void	SetKeyFrameByMask ( GLbitfield mask, GLuint count, const GLuint* arr_frame, XMGType arr_type );

		GLfloat*		m_frame;			
		GLuint			m_count;
		GLuint			m_size;
		GLbitfield		m_mask;
};

#endif 
