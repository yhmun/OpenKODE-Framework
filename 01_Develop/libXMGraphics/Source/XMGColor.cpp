/*
 *
 *      File            XMGColor.cpp
 *      Description     Color implemation
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

XMGColorF::XMGColorF ( void )
{
    m_r = 1.0f;
    m_g = 1.0f;
    m_b = 1.0f;
	m_a = 1.0f;
}

XMGColorF::XMGColorF ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    m_r = r;
    m_g = g;
    m_b = b;
	m_a = a;
}

XMGColorF::XMGColorF ( const XMGColorX& color )
{
	*this = color;
}

XMGColorF&	XMGColorF::operator = ( const XMGColorX& color )
{
	m_r = XMG_X2F ( color.m_r );
	m_g = XMG_X2F ( color.m_g );
	m_b = XMG_X2F ( color.m_b );
	m_a = XMG_X2F ( color.m_a );

	return *this;
}

XMGBool	XMGColorF::operator == ( const XMGColorF& color )
{
	return m_r == color.m_r &&  m_g == color.m_g && m_b == color.m_b && m_a == color.m_a ? XMG_TRUE : XMG_FALSE;
}

XMGBool	XMGColorF::operator != ( const XMGColorF& color )
{
	return m_r != color.m_r ||  m_g != color.m_g || m_b != color.m_b || m_a != color.m_a ? XMG_TRUE : XMG_FALSE;
}

XMGColorX::XMGColorX ( void )
{
    m_r = XMG_F2X ( 1.0f );
    m_g = XMG_F2X ( 1.0f );
    m_b = XMG_F2X ( 1.0f );
	m_a = XMG_F2X ( 1.0f );
}

XMGColorX::XMGColorX ( GLfixed r, GLfixed g, GLfixed b, GLfixed a )
{
    m_r = r;
    m_g = g;
    m_b = b;
	m_a = a;
}

XMGColorX::XMGColorX ( const XMGColorF& color )
{
	*this = color;
}

XMGColorX&	XMGColorX::operator = ( const XMGColorF& color )
{
	m_r = XMG_F2X ( color.m_r );
	m_g = XMG_F2X ( color.m_g );
	m_b = XMG_F2X ( color.m_b );
	m_a = XMG_F2X ( color.m_a );

	return *this;
}

XMGBool	XMGColorX::operator == ( const XMGColorX& color )
{
	return m_r == color.m_r &&  m_g == color.m_g && m_b == color.m_b && m_a == color.m_a ? XMG_TRUE : XMG_FALSE;
}

XMGBool	XMGColorX::operator != ( const XMGColorX& color )
{
	return m_r != color.m_r ||  m_g != color.m_g || m_b != color.m_b || m_a != color.m_a ? XMG_TRUE : XMG_FALSE;
}