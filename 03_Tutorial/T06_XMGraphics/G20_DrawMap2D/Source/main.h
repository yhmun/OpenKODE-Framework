/*
 *
 *      File            main.h
 *      Description     XMGraphics : 2D Map demo application
 *      Version         0.20.0801, 2011-08-01
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 XMSoft. All rights reserved.
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

typedef struct
{
	GLuint          type;
	KDvoid*         primitive;
} XMGData;

typedef struct 
{
	KDuint          type_object;			
	KDuint          type_vertex;			
	KDuint          num_object;			
	KDuint          num_vertex;			 
	KDuint          reserved[6];
} MapHeader;

struct KDTLS
{
	XMGCanvas*      xmg_canvas;
	XMGView*        xmg_view[2];
	XMGFont*        xmg_font[2];	
	XMGText*        xmg_text;
	XMGData         xmg_data[8];

	KDuint          app_index;
	KDuint          app_count;
};

#endif
