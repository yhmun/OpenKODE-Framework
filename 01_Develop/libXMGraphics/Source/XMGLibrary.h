/*
 *
 *      File            XMGLibrary.h
 *      Description     XMGraphics library header
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

#ifndef __XMGLibrary_h__
#define __XMGLibrary_h__

#define REAL				KDfloat
#define XMGAssert(c)		if ( !(c) ) { kdLogMessagefKHR ( "Error code = %d", kdGetError( ) ); kdExit ( 0 ); } 

#include <XMGraphics/XMGraphics.h>

#include <GLUES/glu.h>
#include <FTGL/ftgles.h>
#include <FTGL/ftglesGlue.h>

#include "XMGCanvasImpl.h"
#include "XMGAnimateImpl.h"
#include "XMGViewImpl.h"
#include "XMGFontImpl.h"
#include "XMGTextureImpl.h"
#include "XMGShape.h"
#include "XMGRenderImpl.h"
#include "XMGTextImpl.h"
#include "XMGQuadImpl.h"
#include "XMGPolygonImpl.h"
#include "XMGSphereImpl.h"
#include "XMGRippleImpl.h"
//#include "XMGPodImpl.h"

#endif