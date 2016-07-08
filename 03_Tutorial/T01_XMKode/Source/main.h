/* --------------------------------------------------------------------------
 *
 *      File            main.h
 *      Description     XMKode Tutorial
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
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
 * -------------------------------------------------------------------------- */

#ifndef __main_h__
#define __main_h__

#include <kode.h>

struct KDTLS
{
	GLint          wnd_cx;
	GLint          wnd_cy;

	GLuint         msec;
	GLuint         msec2;

	GLint          index;
	GLfloat        trans;

	GLuint         vertex;
	GLuint         coord;
	GLuint         tex;
};

typedef KDvoid ( *PFNExample ) ( KDvoid ); 

// OpenKODE Example
KDvoid xmExample_01 ( KDvoid );
KDvoid xmExample_02 ( KDvoid );
KDvoid xmExample_03 ( KDvoid );
KDvoid xmExample_04 ( KDvoid );
KDvoid xmExample_05 ( KDvoid );
KDvoid xmExample_06 ( KDvoid );
KDvoid xmExample_07 ( KDvoid );
KDvoid xmExample_08 ( KDvoid );
KDvoid xmExample_09 ( KDvoid );
KDvoid xmExample_10 ( KDvoid );
KDvoid xmExample_11 ( KDvoid );
KDvoid xmExample_12 ( KDvoid );
KDvoid xmExample_13 ( KDvoid );
KDvoid xmExample_14 ( KDvoid );
KDvoid xmExample_15 ( KDvoid );
KDvoid xmExample_16 ( KDvoid );
KDvoid xmExample_17 ( KDvoid );
KDvoid xmExample_18 ( KDvoid );
KDvoid xmExample_19 ( KDvoid );
KDvoid xmExample_20 ( KDvoid );
KDvoid xmExample_21 ( KDvoid );
KDvoid xmExample_22 ( KDvoid );
KDvoid xmExample_23 ( KDvoid );
KDvoid xmExample_24 ( KDvoid );

KDvoid xmExample_ZLib  ( KDvoid );
KDvoid xmExample_Sound ( KDvoid );
KDvoid xmExample_Sound_Free  ( KDvoid );
KDvoid xmExample_Sound_Event ( KDvoid );

// OpenGLES 
KDvoid xmSetQuad ( GLint ix, GLint iy, GLint flip_y );

// ATX Image decoder Example
#define KTX_COUNT 3
#define DDS_COUNT 10
#define PVR_COUNT 40
#define WEP_COUNT 3
#define TIF_COUNT 9
#define JPG_COUNT 10
#define BMP_COUNT 19
#define TGA_COUNT 7
#define PNG_COUNT 40

#define IMAGE_PATH_COUNT (KTX_COUNT + DDS_COUNT + PVR_COUNT + WEP_COUNT + TIF_COUNT + JPG_COUNT + BMP_COUNT + TGA_COUNT + PNG_COUNT)

KDvoid xmLoadTexture ( KDvoid );

#endif
