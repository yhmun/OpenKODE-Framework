/*
 *
 *      File            XMGMatrix.h
 *      Description     Matrix class
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

#ifndef __XMGMatrix_h__
#define __XMGMatrix_h__

class XMGMatrix4F;
class XMGMatrix4X;

class XMGMatrix4F
{
    public :

        XMGMatrix4F ( void );
		XMGMatrix4F ( const XMGMatrix4X & mat );
        
		XMGMatrix4F&	Identity ( void );

		XMGMatrix4F&	Translate ( GLfloat x, GLfloat y, GLfloat z );
		XMGMatrix4F&	Scale     ( GLfloat x, GLfloat y, GLfloat z );		
		XMGMatrix4F&	Rotate	  ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z ); 
		
		XMGMatrix4F&	Ortho       ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar );
		XMGMatrix4F&	Frustum     ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar );
		XMGMatrix4F&	Perspective ( GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar );

		XMGVector3F		Transform ( const XMGVector3F& vec ) const;

		XMGMatrix4F&	operator  = ( const XMGMatrix4X& mat );
		XMGMatrix4F		operator  * ( const XMGMatrix4F& mat );
		XMGMatrix4F&	operator *= ( const XMGMatrix4F& mat );

        GLfloat		m_m[16];
};

class XMGMatrix4X
{
    public :

        XMGMatrix4X ( void );
		XMGMatrix4X ( const XMGMatrix4F & mat );
        
		XMGMatrix4X&	Identity ( void );

		XMGMatrix4X&	Translate ( GLfixed x, GLfixed y, GLfixed z );
		XMGMatrix4X&	Scale     ( GLfixed x, GLfixed y, GLfixed z );		
		XMGMatrix4X&	Rotate	  ( GLfixed angle, GLfixed x, GLfixed y, GLfixed z ); 
	
		XMGMatrix4X&	Ortho       ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar );
		XMGMatrix4X&	Frustum     ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar );
		XMGMatrix4X&	Perspective ( GLfixed fovy, GLfixed aspect, GLfixed znear, GLfixed zfar );

		XMGVector3X		Transform ( const XMGVector3X& vec ) const;

		XMGMatrix4X&	operator  = ( const XMGMatrix4F& mat );
		XMGMatrix4X		operator  * ( const XMGMatrix4X& mat );
		XMGMatrix4X&	operator *= ( const XMGMatrix4X& mat );
		
        GLfixed		m_m[16];
};

typedef class XMGMatrix4X			XMGMatrix4;

#endif 



