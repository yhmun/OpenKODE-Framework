/*
 *
 *      File            XMGMatrix.cpp
 *      Description     Matrix implemation
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

XMGMatrix4F::XMGMatrix4F ( void )
{
	Identity ( );
}

XMGMatrix4F::XMGMatrix4F ( const XMGMatrix4X& mat )
{
	*this = mat;
}

XMGMatrix4F& XMGMatrix4F::Identity ( void )
{
	m_m[ 0] = 1.f;	m_m[ 1] = 0;	m_m[ 2] = 0;	m_m[ 3] = 0;
	m_m[ 4] = 0;	m_m[ 5] = 1.f;	m_m[ 6] = 0;	m_m[ 7] = 0;
	m_m[ 8] = 0;	m_m[ 9] = 0;	m_m[10] = 1.f;	m_m[11] = 0;
	m_m[12] = 0;	m_m[13] = 0;	m_m[14] = 0;	m_m[15] = 1.f;

	return *this;
}

XMGMatrix4F& XMGMatrix4F::Translate ( GLfloat x, GLfloat y, GLfloat z )
{
	XMGMatrix4F		mat;

	mat.m_m[12] = x;	
	mat.m_m[13] = y;		
	mat.m_m[14] = z;

	return *this = mat * *this;
}

XMGMatrix4F& XMGMatrix4F::Scale ( GLfloat x, GLfloat y, GLfloat z )
{
	XMGMatrix4F		mat;
	
	mat.m_m[ 0] = x;
	mat.m_m[ 5] = y;
	mat.m_m[10] = z;

	return *this = mat * *this;
}

XMGMatrix4F& XMGMatrix4F::Rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	XMGMatrix4F		mat;

	GLfloat			s  = kdSinf ( XMG_DEG2RAD ( angle ) );
	GLfloat			c  = kdCosf ( XMG_DEG2RAD ( angle ) );
	GLfloat			l  = kdSqrtf ( x * x + y * y + z * z );
	GLfloat			ux = x / l;
	GLfloat			uy = y / l;
	GLfloat			uz = z / l;
	GLfloat			c1 = 1 - c;

	mat.m_m[ 0] = ux * ux * c1 + c;
	mat.m_m[ 1] = uy * ux * c1 + uz * s;
	mat.m_m[ 2] = ux * uz * c1 - uy * s;

	mat.m_m[ 4] = ux * uy * c1 - uz * s;
	mat.m_m[ 5] = uy * uy * c1 + c;
	mat.m_m[ 6] = uy * uz * c1 + ux * s;
	
	mat.m_m[ 8] = ux * uz * c1 + uy * s;
	mat.m_m[ 9] = uy * uz * c1 - ux * s;
	mat.m_m[10] = uz * uz * c1 + c;

	return *this = mat * *this;
}

XMGMatrix4F& XMGMatrix4F::Ortho ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar )
{
	m_m[ 0] = 2.f / ( right - left );	
	m_m[ 1] = 0;	
	m_m[ 2] = 0;
	m_m[ 3] = 0;

	m_m[ 4] = 0;
	m_m[ 5] = 2.f / ( top - bottom );
	m_m[ 6] = 0;
	m_m[ 7] = 0;

	m_m[ 8] = 0;		
	m_m[ 9] = 0;
	m_m[10] = -2.f / ( zfar - znear );
	m_m[11] = 0;

	m_m[12] = -( right + left ) / ( right - left );	
	m_m[13] = -( top + bottom ) / ( top - bottom );
	m_m[14] = -( zfar + znear ) / ( zfar - znear );
	m_m[15] = 1.f;

	return *this;
}

XMGMatrix4F& XMGMatrix4F::Frustum ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar )
{
	m_m[ 0] = 2.f * znear / ( right - left );	
	m_m[ 1] = 0;	
	m_m[ 2] = 0;
	m_m[ 3] = 0;

	m_m[ 4] = 0;
	m_m[ 5] = 2.f * znear / ( top - bottom );
	m_m[ 6] = 0;
	m_m[ 7] = 0;

	m_m[ 8] = ( right + left ) / ( right - left );		
	m_m[ 9] = ( top + bottom ) / ( top - bottom );
	m_m[10] = ( -zfar - znear ) / ( zfar - znear );
	m_m[11] = -1.f;

	m_m[12] = 0;	
	m_m[13] = 0;
	m_m[14] = ( -( 2.f * znear) * zfar ) / ( zfar - znear );
	m_m[15] = 0;

	return *this;
}

XMGMatrix4F& XMGMatrix4F::Perspective ( GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar )
{
	GLfloat		h  = 2.0f * znear * kdTanf ( XMG_DEG2RAD ( fovy ) * 0.5f );
	GLfloat		w  = aspect * h;
	GLfloat		h2 = h * 0.5f;
	GLfloat		w2 = w * 0.5f;

	Frustum ( -w2, w2, -h2, h2, znear, zfar );

	return *this;
}

XMGVector3F XMGMatrix4F::Transform ( const XMGVector3F& vec ) const
{
	XMGVector3F		ret;

	ret.m_x = vec.m_x * m_m[0] + vec.m_y * m_m[4] + vec.m_z * m_m[8];
    ret.m_y = vec.m_x * m_m[1] + vec.m_y * m_m[5] + vec.m_z * m_m[9];
    ret.m_z = vec.m_x * m_m[2] + vec.m_y * m_m[6] + vec.m_z * m_m[10];

	return ret;
}

XMGMatrix4F& XMGMatrix4F::operator = ( const XMGMatrix4X& mat )
{
	m_m[ 0] = XMG_X2F ( mat.m_m[ 0] );	m_m[ 1] = XMG_X2F ( mat.m_m[ 1] );	m_m[ 2] = XMG_X2F ( mat.m_m[ 2] );	m_m[ 3] = XMG_X2F ( mat.m_m[ 3] );
	m_m[ 4] = XMG_X2F ( mat.m_m[ 4] );	m_m[ 5] = XMG_X2F ( mat.m_m[ 5] );	m_m[ 6] = XMG_X2F ( mat.m_m[ 6] );	m_m[ 7] = XMG_X2F ( mat.m_m[ 7] );
	m_m[ 8] = XMG_X2F ( mat.m_m[ 8] );	m_m[ 9] = XMG_X2F ( mat.m_m[ 9] );	m_m[10] = XMG_X2F ( mat.m_m[10] );	m_m[11] = XMG_X2F ( mat.m_m[11] );
	m_m[12] = XMG_X2F ( mat.m_m[12] );	m_m[13] = XMG_X2F ( mat.m_m[13] );	m_m[14] = XMG_X2F ( mat.m_m[14] );	m_m[15] = XMG_X2F ( mat.m_m[15] );

	return *this;
}

XMGMatrix4F XMGMatrix4F::operator * ( const XMGMatrix4F& mat )
{
	XMGMatrix4F		ret;

	ret.m_m[ 0] = m_m[ 0] * mat.m_m[ 0] + m_m[ 1] * mat.m_m[ 4] + m_m[ 2] * mat.m_m[ 8] + m_m[ 3] * mat.m_m[12];
	ret.m_m[ 1] = m_m[ 0] * mat.m_m[ 1] + m_m[ 1] * mat.m_m[ 5] + m_m[ 2] * mat.m_m[ 9] + m_m[ 3] * mat.m_m[13];
	ret.m_m[ 2] = m_m[ 0] * mat.m_m[ 2] + m_m[ 1] * mat.m_m[ 6] + m_m[ 2] * mat.m_m[10] + m_m[ 3] * mat.m_m[14];
	ret.m_m[ 3] = m_m[ 0] * mat.m_m[ 3] + m_m[ 1] * mat.m_m[ 7] + m_m[ 2] * mat.m_m[11] + m_m[ 3] * mat.m_m[15];

	ret.m_m[ 4] = m_m[ 4] * mat.m_m[ 0] + m_m[ 5] * mat.m_m[ 4] + m_m[ 6] * mat.m_m[ 8] + m_m[ 7] * mat.m_m[12];
	ret.m_m[ 5] = m_m[ 4] * mat.m_m[ 1] + m_m[ 5] * mat.m_m[ 5] + m_m[ 6] * mat.m_m[ 9] + m_m[ 7] * mat.m_m[13];
	ret.m_m[ 6] = m_m[ 4] * mat.m_m[ 2] + m_m[ 5] * mat.m_m[ 6] + m_m[ 6] * mat.m_m[10] + m_m[ 7] * mat.m_m[14];
	ret.m_m[ 7] = m_m[ 4] * mat.m_m[ 3] + m_m[ 5] * mat.m_m[ 7] + m_m[ 6] * mat.m_m[11] + m_m[ 7] * mat.m_m[15];

	ret.m_m[ 8] = m_m[ 8] * mat.m_m[ 0] + m_m[ 9] * mat.m_m[ 4] + m_m[10] * mat.m_m[ 8] + m_m[11] * mat.m_m[12];
	ret.m_m[ 9] = m_m[ 8] * mat.m_m[ 1] + m_m[ 9] * mat.m_m[ 5] + m_m[10] * mat.m_m[ 9] + m_m[11] * mat.m_m[13];
	ret.m_m[10] = m_m[ 8] * mat.m_m[ 2] + m_m[ 9] * mat.m_m[ 6] + m_m[10] * mat.m_m[10] + m_m[11] * mat.m_m[14];
	ret.m_m[11] = m_m[ 8] * mat.m_m[ 3] + m_m[ 9] * mat.m_m[ 7] + m_m[10] * mat.m_m[11] + m_m[11] * mat.m_m[15];

	ret.m_m[12] = m_m[12] * mat.m_m[ 0] + m_m[13] * mat.m_m[ 4] + m_m[14] * mat.m_m[ 8] + m_m[15] * mat.m_m[12];
	ret.m_m[13] = m_m[12] * mat.m_m[ 1] + m_m[13] * mat.m_m[ 5] + m_m[14] * mat.m_m[ 9] + m_m[15] * mat.m_m[13];
	ret.m_m[14] = m_m[12] * mat.m_m[ 2] + m_m[13] * mat.m_m[ 6] + m_m[14] * mat.m_m[10] + m_m[15] * mat.m_m[14];
	ret.m_m[15] = m_m[12] * mat.m_m[ 3] + m_m[13] * mat.m_m[ 7] + m_m[14] * mat.m_m[11] + m_m[15] * mat.m_m[15];

	return ret;
}

XMGMatrix4F& XMGMatrix4F::operator *= ( const XMGMatrix4F& mat ) 
{
	return *this = *this * mat;
}

XMGMatrix4X::XMGMatrix4X ( void )
{
	Identity ( );
}

XMGMatrix4X::XMGMatrix4X ( const XMGMatrix4F& mat )
{
	*this = mat;
}

XMGMatrix4X& XMGMatrix4X::Identity ( void )
{
	m_m[ 0] = 65536;	m_m[ 1] = 0;		m_m[ 2] = 0;		m_m[ 3] = 0;
	m_m[ 4] = 0;		m_m[ 5] = 65536;	m_m[ 6] = 0;		m_m[ 7] = 0;
	m_m[ 8] = 0;		m_m[ 9] = 0;		m_m[10] = 65536;	m_m[11] = 0;
	m_m[12] = 0;		m_m[13] = 0;		m_m[14] = 0;		m_m[15] = 65536;

	return *this;
}

XMGMatrix4X& XMGMatrix4X::Translate ( GLfixed x, GLfixed y, GLfixed z )
{
	XMGMatrix4X		mat;

	mat.m_m[12] = x;	
	mat.m_m[13] = y;		
	mat.m_m[14] = z;

	return *this = mat * *this;
}

XMGMatrix4X& XMGMatrix4X::Scale ( GLfixed x, GLfixed y, GLfixed z )
{
	XMGMatrix4X		mat;
	
	mat.m_m[ 0] = x;
	mat.m_m[ 5] = y;
	mat.m_m[10] = z;

	return *this = mat * *this;
}

XMGMatrix4X& XMGMatrix4X::Rotate ( GLfixed angle, GLfixed x, GLfixed y, GLfixed z )
{
	XMGMatrix4X		mat;

	GLfloat			s  = kdSinf ( XMG_DEG2RAD ( XMG_X2F ( angle ) ) );
	GLfloat			c  = kdCosf ( XMG_DEG2RAD ( XMG_X2F ( angle ) ) );
	GLfloat			fx = XMG_X2F ( x );
	GLfloat			fy = XMG_X2F ( y );
	GLfloat			fz = XMG_X2F ( z );
	GLfloat			fl = kdSqrtf ( fx * fx + fy * fy + fz * fz );
	GLfloat			ux = fx / fl;
	GLfloat			uy = fy / fl;
	GLfloat			uz = fz / fl;
	GLfloat			c1 = 1 - c;

	mat.m_m[ 0] = XMG_F2X ( ux * ux * c1 + c );
	mat.m_m[ 1] = XMG_F2X ( uy * ux * c1 + uz * s );
	mat.m_m[ 2] = XMG_F2X ( ux * uz * c1 - uy * s );

	mat.m_m[ 4] = XMG_F2X ( ux * uy * c1 - uz * s );
	mat.m_m[ 5] = XMG_F2X ( uy * uy * c1 + c );
	mat.m_m[ 6] = XMG_F2X ( uy * uz * c1 + ux * s );
	
	mat.m_m[ 8] = XMG_F2X ( ux * uz * c1 + uy * s );
	mat.m_m[ 9] = XMG_F2X ( uy * uz * c1 - ux * s );
	mat.m_m[10] = XMG_F2X ( uz * uz * c1 + c );

	return *this = mat * *this;
}

XMGMatrix4X& XMGMatrix4X::Ortho ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar )
{
	m_m[ 0] = XMG_F2X ( 2.f / XMG_X2F ( right - left ) );	
	m_m[ 1] = 0;	
	m_m[ 2] = 0;
	m_m[ 3] = 0;

	m_m[ 4] = 0;
	m_m[ 5] = XMG_F2X ( 2.f / XMG_X2F ( top - bottom ) );
	m_m[ 6] = 0;
	m_m[ 7] = 0;

	m_m[ 8] = 0;		
	m_m[ 9] = 0;
	m_m[10] = XMG_F2X ( -2.f / XMG_X2F ( zfar - znear ) );
	m_m[11] = 0;

	m_m[12] = XMG_F2X ( -XMG_X2F ( right + left ) / XMG_X2F ( right - left ) );	
	m_m[13] = XMG_F2X ( -XMG_X2F ( top + bottom ) / XMG_X2F ( top - bottom ) );
	m_m[14] = XMG_F2X ( -XMG_X2F ( zfar + znear ) / XMG_X2F ( zfar - znear ) );
	m_m[15] = 65536;

	return *this;
}

XMGMatrix4X& XMGMatrix4X::Frustum ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar )
{
	m_m[ 0] = XMG_F2X ( 2.f * XMG_X2F ( znear ) / XMG_X2F ( right - left ) );	
	m_m[ 1] = 0;	
	m_m[ 2] = 0;
	m_m[ 3] = 0;

	m_m[ 4] = 0;
	m_m[ 5] = XMG_F2X ( 2.f * XMG_X2F ( znear ) / XMG_X2F ( top - bottom ) );
	m_m[ 6] = 0;
	m_m[ 7] = 0;

	m_m[ 8] = XMG_F2X ( XMG_X2F ( right + left ) / XMG_X2F ( right - left ) );		
	m_m[ 9] = XMG_F2X ( XMG_X2F ( top + bottom ) / XMG_X2F ( top - bottom ) );
	m_m[10] = XMG_F2X ( XMG_X2F ( -zfar - znear ) / XMG_X2F ( zfar - znear ) );
	m_m[11] = -65536;

	m_m[12] = 0;	
	m_m[13] = 0;
	m_m[14] = XMG_F2X ( ( -( 2.f * XMG_X2F ( znear ) ) * XMG_X2F ( zfar ) ) / XMG_X2F ( zfar - znear ) );
	m_m[15] = 0;

	return *this;
}

XMGMatrix4X& XMGMatrix4X::Perspective ( GLfixed fovy, GLfixed aspect, GLfixed znear, GLfixed zfar )
{
	GLfloat		fh  = 2.0f * XMG_X2F ( znear ) * kdTanf ( XMG_DEG2RAD ( XMG_X2F ( fovy ) ) * 0.5f );	
	GLfloat		fw  = XMG_X2F ( aspect ) * fh;
	GLfixed		xh2 = XMG_F2X ( fh * 0.5f );
	GLfixed		xw2 = XMG_F2X ( fw * 0.5f );
	
	Frustum ( -xw2, xw2, -xh2, xh2, znear, zfar );

	return *this;
}

XMGVector3X XMGMatrix4X::Transform ( const XMGVector3X& vec ) const
{
	XMGVector3X		ret;

	ret.m_x = XMG_F2X ( XMG_X2F ( vec.m_x ) * XMG_X2F ( m_m[0] ) + XMG_X2F ( vec.m_y ) * XMG_X2F ( m_m[4] ) + XMG_X2F ( vec.m_z ) * XMG_X2F ( m_m[ 8] ) );
    ret.m_y = XMG_F2X ( XMG_X2F ( vec.m_x ) * XMG_X2F ( m_m[1] ) + XMG_X2F ( vec.m_y ) * XMG_X2F ( m_m[5] ) + XMG_X2F ( vec.m_z ) * XMG_X2F ( m_m[ 9] ) );
    ret.m_z = XMG_F2X ( XMG_X2F ( vec.m_x ) * XMG_X2F ( m_m[2] ) + XMG_X2F ( vec.m_y ) * XMG_X2F ( m_m[6] ) + XMG_X2F ( vec.m_z ) * XMG_X2F ( m_m[10] ) );

	return ret;
}

XMGMatrix4X& XMGMatrix4X::operator = ( const XMGMatrix4F& mat )
{
	m_m[ 0] = XMG_F2X ( mat.m_m[ 0] );	m_m[ 1] = XMG_F2X ( mat.m_m[ 1] );	m_m[ 2] = XMG_F2X ( mat.m_m[ 2] );	m_m[ 3] = XMG_F2X ( mat.m_m[ 3] );
	m_m[ 4] = XMG_F2X ( mat.m_m[ 4] );	m_m[ 5] = XMG_F2X ( mat.m_m[ 5] );	m_m[ 6] = XMG_F2X ( mat.m_m[ 6] );	m_m[ 7] = XMG_F2X ( mat.m_m[ 7] );
	m_m[ 8] = XMG_F2X ( mat.m_m[ 8] );	m_m[ 9] = XMG_F2X ( mat.m_m[ 9] );	m_m[10] = XMG_F2X ( mat.m_m[10] );	m_m[11] = XMG_F2X ( mat.m_m[11] );
	m_m[12] = XMG_F2X ( mat.m_m[12] );	m_m[13] = XMG_F2X ( mat.m_m[13] );	m_m[14] = XMG_F2X ( mat.m_m[14] );	m_m[15] = XMG_F2X ( mat.m_m[15] );

	return *this;
}

XMGMatrix4X XMGMatrix4X::operator * ( const XMGMatrix4X& mat )
{
	return XMGMatrix4F ( *this ) * XMGMatrix4F ( mat );
}

XMGMatrix4X& XMGMatrix4X::operator *= ( const XMGMatrix4X& mat )
{
	return *this = *this * mat;
}