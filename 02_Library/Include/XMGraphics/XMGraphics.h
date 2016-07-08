/*
 *
 *      File            XMGraphics.h
 *      Description     XMGraphics library header
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

#ifndef __XMGraphics_h__
#define __XMGraphics_h__

#include <kode.h>

#define XMG_VENDOR                  XMSoft
#define XMG_VERSION                 0.20.0801

#define	XMG_F2X(X)                  ( (GLfixed) ( ( X ) * 65536.0f ) )
#define XMG_X2F(X)                  ( (GLfloat) ( ( X ) * 0.0000152587890625f ) )
#define XMG_I2X(X)                  ( ( (GLfixed) ( X ) ) << 16 )
#define XMG_I2F(X)                  ( (GLfloat) ( X ) )
#define XMG_F2I(X)                  ( (GLint) ( X ) )
#define XMG_X2I(X)                  ( ( (GLint) ( X ) ) >> 16 )

#define XMG_RAD2DEG(X)              ( ( X ) * 57.2957795129f )
#define XMG_DEG2RAD(X)              ( ( X ) * 0.01745329252f )

#define XMG_SHAPE_ALL               0xffffffff
#define XMG_FACET_ALL               0xffffffff
#define	XMG_HOLE_NULL               0

typedef enum 
{	
	XMG_TRUE                        = 1,
	XMG_FALSE                       = 0,		
} XMGBool;

typedef enum 
{	
	XMG_TYPE_FLOAT                  = 0x1406,
	XMG_TYPE_FIXED                  = 0x140C,		
} XMGType;

#include <XMGraphics/STLVector.h>
#include <XMGraphics/XMGColor.h>
#include <XMGraphics/XMGRect.h>
#include <XMGraphics/XMGVector.h>
#include <XMGraphics/XMGMatrix.h>

#include <XMGraphics/XMGCanvas.h>
#include <XMGraphics/XMGAnimate.h>
#include <XMGraphics/XMGView.h>
#include <XMGraphics/XMGTexture.h>
#include <XMGraphics/XMGFont.h>
#include <XMGraphics/XMGRender.h>
#include <XMGraphics/XMGText.h>
#include <XMGraphics/XMGQuad.h>
#include <XMGraphics/XMGPolygon.h>
#include <XMGraphics/XMGSphere.h>
#include <XMGraphics/XMGRipple.h>
//#include <XMGraphics/XMGPod.h>

#endif