/* --------------------------------------------------------------------------
 *
 *      File            egl_platform.h
 *      Description     Platform-specific types and definitions for EGL 1.4
 *      Version         1.4, 2009-10-21 
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __eglplatform_h_
#define __eglplatform_h_

#include <KHR/khr_platform.h>

/*
 * Macros used in EGL function prototype declarations.
 *
 * EGL functions should be prototyped as:
 *
 * EGLAPI return-type EGLAPIENTRY eglFunction(arguments);
 * typedef return-type (EXPAPIENTRYP PFNEGLFUNCTIONPROC) (arguments);
 *
 * KHRONOS_APICALL and KHRONOS_APIENTRY are defined in KHR/khr_platform.h
 */

#ifndef EGLAPI
#define EGLAPI                          KHRONOS_APICALL
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY                     KHRONOS_APIENTRY
#endif
#define EGLAPIENTRYP                    KHRONOS_APIENTRY*

/*
 * The types NativeDisplayType, NativeWindowType, and NativePixmapType
 * are aliases of window-system-dependent types, such as X Display * or
 * Windows Device Context. They must be defined in platform-specific
 * code below. The EGL-prefixed versions of Native*Type are the same
 * types, renamed in EGL 1.3 so all types in the API start with "EGL".
 */

#if defined ( SHP ) || defined ( TIZEN )

#include "egl_macro.h"

typedef int                             EGLNativeDisplayType;
typedef void*                           EGLNativeWindowType;
typedef void*                           EGLNativePixmapType;

#elif defined ( BLACKBERRY )

typedef int                             EGLNativeDisplayType;
typedef void*                           EGLNativeWindowType;
typedef void*                           EGLNativePixmapType;

#elif defined ( _WIN32 ) || defined ( __VC32__ ) && !defined ( __CYGWIN__ ) && !defined( __SCITECH_SNAP__ ) 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>

typedef HDC                             EGLNativeDisplayType;
typedef HBITMAP                         EGLNativePixmapType;
typedef HWND                            EGLNativeWindowType;

#elif defined ( __WINSCW__ ) || defined ( __SYMBIAN32__ )  

typedef int                             EGLNativeDisplayType;
typedef void*                           EGLNativeWindowType;
typedef void*                           EGLNativePixmapType;

#elif defined(__ANDROID__) || defined(ANDROID)

#include <android/native_window.h>

struct egl_native_pixmap_t;

typedef struct ANativeWindow*           EGLNativeWindowType;
typedef struct egl_native_pixmap_t*     EGLNativePixmapType;
typedef void*                           EGLNativeDisplayType;

#elif defined ( __unix__ ) || defined ( Linux )

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef Display*                        EGLNativeDisplayType;
typedef Pixmap                          EGLNativePixmapType;
typedef Window                          EGLNativeWindowType;

#else

typedef void*                           EGLNativeDisplayType;
typedef void*                           EGLNativeWindowType;
typedef void*                           EGLNativePixmapType;

#endif

// EGL 1.2 types, renamed for consistency in EGL 1.3
typedef EGLNativeDisplayType            NativeDisplayType;
typedef EGLNativePixmapType             NativePixmapType;
typedef EGLNativeWindowType             NativeWindowType;

// EGL Types
typedef khronos_int32_t                 EGLint;
typedef khronos_uint32_t                EGLBoolean;
typedef khronos_uint32_t                EGLenum;
typedef void*                           EGLConfig;
typedef void*                           EGLContext;
typedef void*                           EGLDisplay;
typedef void*                           EGLSurface;
typedef void*                           EGLClientBuffer;

#endif
