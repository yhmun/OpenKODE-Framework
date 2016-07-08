/*
 *
 *      File            XMGRect.cpp
 *      Description     Rect implemation 
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

XMGRectF::XMGRectF ( void )
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 0;
	m_h = 0;
}

XMGRectF::XMGRectF ( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	m_x = x;
	m_y = y;
	m_z = 0;
	m_w = w;
	m_h = h;
}

XMGRectF::XMGRectF ( GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h )
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
	m_h = h;
}

void XMGRectF::SetLeft ( GLfloat l )
{ 
	m_x = l;
}

void XMGRectF::SetRight ( GLfloat r ) 
{
	m_w = r > m_x ? r - m_x : -( m_x - r );
}

void XMGRectF::SetBottom ( GLfloat b )
{
	m_h = m_y > b ? m_y - b : -( b - m_y );
}

void XMGRectF::SetTop ( GLfloat t ) 
{ 
	m_y = t;
}

GLfloat XMGRectF::GetLeft ( void )
{
	return m_x; 
}

GLfloat XMGRectF::GetRight ( void )
{
	return m_x + m_w; 
}

GLfloat XMGRectF::GetBottom ( void )
{
	return m_y; 
}

GLfloat XMGRectF::GetTop ( void )
{
	return m_y + m_h; 
}

GLfloat XMGRectF::GetCenterX ( void )
{
	return ( m_x + m_w ) * 0.5f;
}

GLfloat XMGRectF::GetCenterY ( void )
{
	return ( m_y + m_h ) * 0.5f;
}

XMGRectX::XMGRectX ( void )
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 0;
	m_h = 0;
}

XMGRectX::XMGRectX ( GLfixed x, GLfixed y, GLfixed w, GLfixed h )
{
	m_x = x;
	m_y = y;
	m_z = 0;
	m_w = w;
	m_h = h;
}

XMGRectX::XMGRectX ( GLfixed x, GLfixed y, GLfixed z, GLfixed w, GLfixed h )
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
	m_h = h;
}

void XMGRectX::SetLeft ( GLfixed l )
{ 
	m_x = l;
}

void XMGRectX::SetRight ( GLfixed r ) 
{
	m_w = r > m_x ? r - m_x : -( m_x - r );
}

void XMGRectX::SetBottom ( GLfixed b )
{
	m_h = m_y > b ? m_y - b : -( b - m_y );
}

void XMGRectX::SetTop ( GLfixed t ) 
{ 
	m_y = t;
}

GLfixed XMGRectX::GetLeft ( void )
{
	return m_x; 
}

GLfixed XMGRectX::GetRight ( void )
{
	return m_x + m_w; 
}

GLfixed XMGRectX::GetBottom ( void )
{
	return m_y; 
}

GLfixed XMGRectX::GetTop ( void )
{
	return m_y + m_h; 
}

GLfixed XMGRectX::GetCenterX ( void )
{
	return XMG_F2X ( XMG_X2F( m_x + m_w ) * 0.5f );
}

GLfixed XMGRectX::GetCenterY ( void )
{
	return XMG_F2X ( XMG_X2F( m_y + m_h ) * 0.5f );
}