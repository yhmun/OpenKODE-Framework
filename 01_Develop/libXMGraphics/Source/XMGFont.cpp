/*
 *
 *      File            XMGFont.cpp
 *      Description     Font implemation
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

XMGFontImpl::XMGFontImpl ( const KDchar* path )
{
	XMGAssert ( m_font = new FTTextureFont ( path ) );	

	m_font->FaceSize ( 20 );
	m_font->CharMap ( ft_encoding_unicode );	
}

XMGFontImpl::~XMGFontImpl ( void )
{
	if ( m_font )
	{
		delete m_font;
	}	
}

XMGFont::XMGFont ( const KDchar* path ) 
{
	XMGAssert ( m_impl = new XMGFontImpl ( path ) );
}

XMGFont::~XMGFont ( void )
{	
	delete (XMGFontImpl *) m_impl;	
}

void XMGFont::SetSize ( GLsizei size )
{
	XMGFontImpl*	impl = (XMGFontImpl *) m_impl;

	impl->m_font->FaceSize ( (const unsigned int) size );
}