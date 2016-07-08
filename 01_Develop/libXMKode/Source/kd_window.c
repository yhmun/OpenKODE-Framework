/* --------------------------------------------------------------------------
 *
 *      File            kd_window.c
 *      Description     Windowing 
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

#include "kd_library.h"

extern  KDint   g_width;
extern	KDint   g_height;
extern  KDfloat g_scale;

/*
   XMSoft's revision : All window's APIs are not used.
                       Only a window is used. so see xm_kode.h.
*/

// kdCreateWindow : Create a window.
KD_API KDWindow* KD_APIENTRY kdCreateWindow ( XMEGLDisplay display, XMEGLConfig config, KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );

	return 0;
}

// kdDestroyWindow : Destroy a window.
KD_API KDint KD_APIENTRY kdDestroyWindow ( KDWindow* window )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdSetWindowPropertybv, kdSetWindowPropertyiv, kdSetWindowPropertycv: Set a window property to request a change in the on-screen representation of the window.
KD_API KDint KD_APIENTRY kdSetWindowPropertybv ( KDWindow* window, KDint pname, const KDboolean* param )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

KD_API KDint KD_APIENTRY kdSetWindowPropertyiv ( KDWindow* window, KDint pname, const KDint32* param )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

KD_API KDint KD_APIENTRY kdSetWindowPropertycv ( KDWindow* window, KDint pname, const KDchar* param )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdGetWindowPropertybv, kdGetWindowPropertyiv, kdGetWindowPropertycv: Get the current value of a window property.
KD_API KDint KD_APIENTRY kdGetWindowPropertybv ( KDWindow* window, KDint pname, KDboolean* param )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

KD_API KDint KD_APIENTRY kdGetWindowPropertyiv ( KDWindow* window, KDint pname, KDint32* param )
{
	switch ( pname )
	{
		case KD_WINDOWPROPERTY_SIZE :

			param[0] = g_width;
			param[1] = g_height;

			return 0;
            
        case KD_WINDOWPROPERTY_DEVICE_SIZE :
            
			param[0] = (KDint) ( g_width  * g_scale );
			param[1] = (KDint) ( g_height * g_scale );
            
            return 0;
            
        case KD_WINDOWPROPERTY_SCALE :
            
            param[0] = (KDint) g_scale;
            
            return 0;

		default :
			kdSetError ( KD_ENOSYS );
			break;
	}

	return -1;
}

KD_API KDint KD_APIENTRY kdGetWindowPropertycv ( KDWindow* window, KDint pname, KDchar* param, KDsize* size )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdRealizeWindow : Realize the window as a displayable entity and get the native window handle for passing to EGL.
KD_API KDint KD_APIENTRY kdRealizeWindow ( KDWindow* window, XMEGLNativeWindowType* nativewindow )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}
