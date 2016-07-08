/*
 *
 *      File            XMGColor.h
 *      Description     Color class
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

#ifndef __XMGColor_h__
#define __XMGColor_h__

class XMGColorF;
class XMGColorX;

class XMGColorF
{
    public :

        XMGColorF ( void );		
        XMGColorF ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
		XMGColorF ( const XMGColorX& color );

		XMGColorF&	operator  = ( const XMGColorX& color );
		XMGBool		operator == ( const XMGColorF& color );
		XMGBool		operator != ( const XMGColorF& color );

        GLfloat		m_r;
        GLfloat		m_g;
        GLfloat		m_b;
        GLfloat		m_a;
};

class XMGColorX
{
	 public :

        XMGColorX ( void );		
        XMGColorX ( GLfixed r, GLfixed _g, GLfixed _b, GLfixed _a );
		XMGColorX ( const XMGColorF& color );

		XMGColorX&	operator  = ( const XMGColorF& color );
		XMGBool		operator == ( const XMGColorX& color );
		XMGBool		operator != ( const XMGColorX& color );

        GLfixed		m_r;
        GLfixed		m_g;
        GLfixed		m_b;
        GLfixed		m_a;
};

typedef class XMGColorX			XMGColor;

#endif 



