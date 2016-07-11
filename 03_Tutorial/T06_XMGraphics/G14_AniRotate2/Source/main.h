/*
 *
 *      File            main.h
 *      Description     XMGraphics : Rotation animation demo 2
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

#ifndef __main_h__
#define __main_h__

#include <XMGraphics/XMGraphics.h>

struct KDTLS
{
	GLint           wnd_cx;
	GLint           wnd_cy;

	XMGCanvas*      xmg_canvas;
	XMGFont*        xmg_font;	
	XMGAnimate*     xmg_ani;	
	XMGTexture*     xmg_tex[4];	
	XMGText*        xmg_text;
	XMGQuad*        xmg_quad;

	KDuint          app_msec;
};

#endif
