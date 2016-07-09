/* --------------------------------------------------------------------------
 *
 *      File            AddingImages.cpp
 *      Description     Simple linear blender ( dst = alpha * src1 + beta * src2 )
 *      Author          OpenCV team, Y.H Mun
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

#include "Precompiled.h"
#include "../Controller.h"

KDvoid AddingImages	( KDint nIdx )
{
	Mat			tSrc1;
	Mat			tSrc2;
	Mat			tDst;
	KDdouble	dAlpha;
	KDdouble	dBeta;

	// Read image ( same size, same type )
	tSrc1 = imread ( "/res/image/WindowsLogo.jpg" );
	tSrc2 = imread ( "/res/image/LinuxLogo.jpg" );

	dAlpha = 0.5;
	dBeta  = ( 1.0 - dAlpha );

	addWeighted ( tSrc1, dAlpha, tSrc2, dBeta, 0.0, tDst );

	g_pController->setFrame ( 0, tDst );
}
