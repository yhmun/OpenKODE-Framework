/*
 *
 *      File            XMGVector.cpp
 *      Description     Vector implemation 
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

#include "XMGLibrary.h"

XMGVector2F::XMGVector2F ( void )
{
	m_x = 0;
	m_y = 0;
}

XMGVector2F::XMGVector2F ( GLfloat x, GLfloat y )
{
	m_x = x;
	m_y = y;
}

XMGVector2F::XMGVector2F ( const XMGVector2X& vec )
{
	*this = vec;
}

XMGVector2F& XMGVector2F::operator = ( const XMGVector2X& vec )
{
	m_x = XMG_X2F ( vec.m_x );
	m_y = XMG_X2F ( vec.m_y );

	return *this;
}

XMGVector2F	XMGVector2F::operator - ( const XMGVector2F& vec ) const
{
	XMGVector2F		ret;

	ret.m_x = m_x - vec.m_x;
	ret.m_y = m_y - vec.m_y;

	return ret;	
}

XMGBool	XMGVector2F::operator == ( const XMGVector2F& vec ) const
{
	return m_x == vec.m_x && m_y == vec.m_y ? XMG_TRUE : XMG_FALSE;
}

XMGVector2X::XMGVector2X ( void )
{
	m_x = 0;
	m_y = 0;
}

XMGVector2X::XMGVector2X ( GLfixed x, GLfixed y )
{
	m_x = x;
	m_y = y;
}

XMGVector2X::XMGVector2X ( const XMGVector2F& vec )
{
	*this = vec;
}

XMGVector2X& XMGVector2X::operator = ( const XMGVector2F& vec )
{
	m_x = XMG_F2X ( vec.m_x );
	m_y = XMG_F2X ( vec.m_y );

	return *this;
}

XMGVector2X	XMGVector2X::operator - ( const XMGVector2X& vec ) const
{
	XMGVector2X		ret;

	ret.m_x = m_x - vec.m_x;
	ret.m_y = m_y - vec.m_y;

	return ret;	
}

XMGBool	XMGVector2X::operator == ( const XMGVector2X& vec ) const
{
	return m_x == vec.m_x && m_y == vec.m_y ? XMG_TRUE : XMG_FALSE;
}

XMGVector3F::XMGVector3F ( void )
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

XMGVector3F::XMGVector3F ( GLfloat x, GLfloat y, GLfloat z )
{
	m_x = x;
	m_y = y;
	m_z = z;
}

XMGVector3F::XMGVector3F ( const XMGVector3X& vec )
{
	*this = vec;
}

GLfloat	XMGVector3F::Length ( void ) const
{
	return kdSqrtf ( m_x * m_x + m_y * m_y + m_z * m_z );
}

XMGVector3F XMGVector3F::Normalize ( void ) const
{
	XMGVector3F		ret;
	GLfloat			len = Length ( );
	
	ret.m_x = m_x / len;
	ret.m_y = m_y / len;
	ret.m_z = m_z / len;

	return ret;
}

GLfloat	XMGVector3F::Dot ( const XMGVector3F& vec ) const
{
	return m_x * vec.m_x + m_y * vec.m_y + m_z * vec.m_z;
}

XMGVector3F	XMGVector3F::Cross ( const XMGVector3F& vec ) const
{
	XMGVector3F		ret;
	
	ret.m_x = m_y * vec.m_z - m_z * vec.m_y;
	ret.m_y = m_z * vec.m_x - m_x * vec.m_z;
	ret.m_z = m_x * vec.m_y - m_y * vec.m_x;

	return ret;
}

XMGVector3F& XMGVector3F::operator = ( const XMGVector3X& vec )
{
	m_x = XMG_X2F ( vec.m_x );
	m_y = XMG_X2F ( vec.m_y );
	m_z = XMG_X2F ( vec.m_z );

	return *this;
}

XMGVector3F XMGVector3F::operator + ( const XMGVector3F& vec ) const
{
	XMGVector3F		ret;

	ret.m_x = m_x + vec.m_x;
	ret.m_y = m_y + vec.m_y;
	ret.m_z = m_z + vec.m_z;

	return ret;
}

XMGVector3F XMGVector3F::operator - ( const XMGVector3F& vec ) const
{
	XMGVector3F		ret;

	ret.m_x = m_x - vec.m_x;
	ret.m_y = m_y - vec.m_y;
	ret.m_z = m_z - vec.m_z;

	return ret;
}

XMGVector3F	XMGVector3F::operator * ( const GLfloat& val ) const
{
	XMGVector3F		ret;

	ret.m_x = m_x * val;
	ret.m_y = m_y * val;
	ret.m_z = m_z * val;

	return ret;
}

XMGVector3F	XMGVector3F::operator / ( const GLfloat& val ) const
{
	XMGVector3F		ret;

	ret.m_x = m_x / val;
	ret.m_y = m_y / val;
	ret.m_z = m_z / val;

	return ret;
}

XMGVector3F& XMGVector3F::operator += ( const XMGVector3F& vec )
{
	m_x += vec.m_x;
	m_y += vec.m_y;
	m_z += vec.m_z;

	return *this;
}

XMGVector3F& XMGVector3F::operator -= ( const XMGVector3F& vec )
{
	m_x -= vec.m_x;
	m_y -= vec.m_y;
	m_z -= vec.m_z;

	return *this;
}

XMGVector3F& XMGVector3F::operator *= ( const GLfloat& val )
{
	m_x *= val;
	m_y *= val;
	m_z *= val;

	return *this;
}

XMGVector3F& XMGVector3F::operator /= ( const GLfloat& val )
{
	m_x /= val;
	m_y /= val;
	m_z /= val;

	return *this;
}

XMGVector3X::XMGVector3X ( void )
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

XMGVector3X::XMGVector3X ( GLfixed x, GLfixed y, GLfixed z )
{
	m_x = x;
	m_y = y;
	m_z = z;
}

XMGVector3X::XMGVector3X ( const XMGVector3F& vec )
{
	*this = vec;
}

GLfixed	XMGVector3X::Length ( void ) const
{
	return XMG_F2X ( kdSqrtf ( XMG_X2F ( m_x ) * XMG_X2F ( m_x ) + XMG_X2F ( m_y ) * XMG_X2F ( m_y ) + XMG_X2F ( m_z ) * XMG_X2F ( m_z ) ) );
}

XMGVector3X XMGVector3X::Normalize ( void ) const
{
	XMGVector3X		ret;
	GLfloat			len = XMG_X2F ( Length ( ) );

	ret.m_x = XMG_F2X ( XMG_X2F ( m_x ) / len );
	ret.m_y = XMG_F2X ( XMG_X2F ( m_y ) / len );
	ret.m_z = XMG_F2X ( XMG_X2F ( m_z ) / len );

	return ret;
}

GLfixed	XMGVector3X::Dot ( const XMGVector3X& vec ) const
{
	return XMG_F2X ( XMG_X2F ( m_x ) * XMG_X2F ( vec.m_x ) + XMG_X2F ( m_y ) * XMG_X2F ( vec.m_y ) + XMG_X2F ( m_z ) * XMG_X2F ( vec.m_z ) );	
}

XMGVector3X	XMGVector3X::Cross ( const XMGVector3X& vec ) const
{
	XMGVector3X		ret;
	
	ret.m_x = XMG_F2X ( XMG_X2F ( m_y ) * XMG_X2F ( vec.m_z ) - XMG_X2F ( m_z ) * XMG_X2F ( vec.m_y ) );
	ret.m_y = XMG_F2X ( XMG_X2F ( m_z ) * XMG_X2F ( vec.m_x ) - XMG_X2F ( m_x ) * XMG_X2F ( vec.m_z ) );
	ret.m_z = XMG_F2X ( XMG_X2F ( m_x ) * XMG_X2F ( vec.m_y ) - XMG_X2F ( m_y ) * XMG_X2F ( vec.m_x ) );

	return ret;
}

XMGVector3X& XMGVector3X::operator = ( const XMGVector3F& vec )
{
	m_x = XMG_F2X ( vec.m_x );
	m_y = XMG_F2X ( vec.m_y );
	m_z = XMG_F2X ( vec.m_z );

	return *this;
}

XMGVector3X XMGVector3X::operator + ( const XMGVector3X& vec ) const
{
	XMGVector3X		ret;

	ret.m_x = m_x + vec.m_x;
	ret.m_y = m_y + vec.m_y;
	ret.m_z = m_z + vec.m_z;

	return ret;
}

XMGVector3X XMGVector3X::operator - ( const XMGVector3X& vec ) const
{
	XMGVector3X		ret;

	ret.m_x = m_x - vec.m_x;
	ret.m_y = m_y - vec.m_y;
	ret.m_z = m_z - vec.m_z;

	return ret;
}

XMGVector3X	XMGVector3X::operator * ( const GLfixed& val ) const
{
	XMGVector3X		ret;

	ret.m_x = XMG_F2X ( XMG_X2F ( m_x ) * XMG_X2F ( val ) );
	ret.m_y = XMG_F2X ( XMG_X2F ( m_y ) * XMG_X2F ( val ) );
	ret.m_z = XMG_F2X ( XMG_X2F ( m_z ) * XMG_X2F ( val ) );

	return ret;
}

XMGVector3X	XMGVector3X::operator / ( const GLfixed& val ) const
{
	XMGVector3X		ret;

	ret.m_x = XMG_F2X ( XMG_X2F ( m_x ) / XMG_X2F ( val ) );
	ret.m_y = XMG_F2X ( XMG_X2F ( m_y ) / XMG_X2F ( val ) );
	ret.m_z = XMG_F2X ( XMG_X2F ( m_z ) / XMG_X2F ( val ) );

	return ret;
}

XMGVector3X& XMGVector3X::operator += ( const XMGVector3X& vec )
{
	m_x += vec.m_x;
	m_y += vec.m_y;
	m_z += vec.m_z;

	return *this;
}

XMGVector3X& XMGVector3X::operator -= ( const XMGVector3X& vec )
{
	m_x -= vec.m_x;
	m_y -= vec.m_y;
	m_z -= vec.m_z;

	return *this;
}

XMGVector3X&	XMGVector3X::operator *= ( const GLfixed& val )
{
	m_x = XMG_F2X ( XMG_X2F ( m_x ) * XMG_X2F ( val ) );
	m_y = XMG_F2X ( XMG_X2F ( m_y ) * XMG_X2F ( val ) );
	m_z = XMG_F2X ( XMG_X2F ( m_z ) * XMG_X2F ( val ) );

	return *this;
}

XMGVector3X&	XMGVector3X::operator /= ( const GLfixed& val )
{
	m_x = XMG_F2X ( XMG_X2F ( m_x ) / XMG_X2F ( val ) );
	m_y = XMG_F2X ( XMG_X2F ( m_y ) / XMG_X2F ( val ) );
	m_z = XMG_F2X ( XMG_X2F ( m_z ) / XMG_X2F ( val ) );

	return *this;
}