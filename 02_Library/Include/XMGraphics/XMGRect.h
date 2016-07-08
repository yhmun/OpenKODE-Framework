/*
 *
 *      File            XMGRect.h
 *      Description     Rect class
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

#ifndef __XMGRect_h__
#define __XMGRect_h__

class XMGRectF
{
    public:

        XMGRectF ( void );        
		XMGRectF ( GLfloat x, GLfloat y, GLfloat w, GLfloat h );
        XMGRectF ( GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h );

		void		SetLeft	   ( GLfloat l );
		void		SetRight   ( GLfloat r );
		void		SetBottom  ( GLfloat b );
		void		SetTop	   ( GLfloat t );		

		GLfloat		GetLeft    ( void );
		GLfloat		GetBottom  ( void );
		GLfloat		GetTop     ( void );
		GLfloat		GetRight   ( void );
		GLfloat		GetCenterX ( void );
		GLfloat		GetCenterY ( void );

        GLfloat		m_x;
		GLfloat		m_y;
		GLfloat		m_z;
		GLfloat		m_w;
		GLfloat		m_h;
};

class XMGRectX
{
    public:

        XMGRectX ( void );        
		XMGRectX ( GLfixed x, GLfixed y, GLfixed w, GLfixed h );
        XMGRectX ( GLfixed x, GLfixed y, GLfixed z, GLfixed w, GLfixed h );

		void		SetLeft	   ( GLfixed l );
		void		SetRight   ( GLfixed r );
		void		SetBottom  ( GLfixed b );
		void		SetTop	   ( GLfixed t );		

		GLfixed		GetLeft    ( void );
		GLfixed		GetBottom  ( void );
		GLfixed		GetTop     ( void );
		GLfixed		GetRight   ( void );
		GLfixed		GetCenterX ( void );
		GLfixed		GetCenterY ( void );

        GLfixed		m_x;
		GLfixed		m_y;
		GLfixed		m_z;
		GLfixed		m_w;
		GLfixed		m_h;
};

typedef class XMGRectX			XMGRect;

#endif 