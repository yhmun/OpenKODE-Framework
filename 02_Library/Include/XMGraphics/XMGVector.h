/*
 *
 *      File            XMGVector.h
 *      Description     Vector class
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

#ifndef __XMGVector_h__
#define __XMGVector_h__

class XMGVector2F;
class XMGVector2X;
class XMGVector3F;
class XMGVector3X;

class XMGVector2F
{
	public :
		
		XMGVector2F ( void );
		XMGVector2F ( GLfloat x, GLfloat y );
		XMGVector2F ( const XMGVector2X& vec );

		XMGVector2F&	operator  = ( const XMGVector2X& vec );
		XMGVector2F		operator  - ( const XMGVector2F& vec ) const;
		XMGBool			operator == ( const XMGVector2F& vec ) const;

		GLfloat		m_x;
		GLfloat		m_y;
};

class XMGVector2X
{
	public :
		
		XMGVector2X ( void );
		XMGVector2X ( GLfixed x, GLfixed y );
		XMGVector2X ( const XMGVector2F& vec );

		XMGVector2X&	operator  = ( const XMGVector2F& vec );
		XMGVector2X		operator  - ( const XMGVector2X& vec ) const;
		XMGBool			operator == ( const XMGVector2X& vec ) const;

		GLfixed		m_x;
		GLfixed		m_y;
};

class XMGVector3F
{
	public :

		XMGVector3F ( void );
		XMGVector3F ( GLfloat x, GLfloat y, GLfloat z = 0 );
		XMGVector3F ( const XMGVector3X& vec );

		GLfloat			Length ( void ) const;		
		XMGVector3F		Normalize ( void ) const;
		GLfloat			Dot ( const XMGVector3F& vec ) const;
		XMGVector3F		Cross ( const XMGVector3F& vec ) const;
				
		XMGVector3F&	operator = ( const XMGVector3X& vec );
		XMGVector3F		operator + ( const XMGVector3F& vec ) const;
		XMGVector3F		operator - ( const XMGVector3F& vec ) const;
		XMGVector3F		operator * ( const GLfloat& val ) const;	
		XMGVector3F		operator / ( const GLfloat& val ) const;	

		XMGVector3F&	operator += ( const XMGVector3F& vec );
		XMGVector3F&	operator -= ( const XMGVector3F& vec );
		XMGVector3F&	operator *= ( const GLfloat& val );
		XMGVector3F&	operator /= ( const GLfloat& val );


		GLfloat		m_x;
		GLfloat		m_y;
		GLfloat		m_z;
};

class XMGVector3X
{
	public :

		XMGVector3X ( void );
		XMGVector3X ( GLfixed x, GLfixed y, GLfixed z = 0 );
		XMGVector3X ( const XMGVector3F& vec );

		GLfixed			Length ( void ) const;
		XMGVector3X		Normalize ( void ) const;
		GLfixed			Dot ( const XMGVector3X& vec ) const;
		XMGVector3X		Cross ( const XMGVector3X& vec ) const;

		XMGVector3X&	operator = ( const XMGVector3F& vec );
		XMGVector3X		operator + ( const XMGVector3X& vec ) const;
		XMGVector3X		operator - ( const XMGVector3X& vec ) const;
		XMGVector3X		operator * ( const GLfixed& val ) const;
		XMGVector3X		operator / ( const GLfixed& val ) const;
		
		XMGVector3X&	operator += ( const XMGVector3X& vec );
		XMGVector3X&	operator -= ( const XMGVector3X& vec );
		XMGVector3X&	operator *= ( const GLfixed& val );
		XMGVector3X&	operator /= ( const GLfixed& val );

		GLfixed		m_x;
		GLfixed		m_y;
		GLfixed		m_z;
};

typedef class XMGVector2X			XMGVector2;
typedef class XMGVector3X			XMGVector3;

#endif



