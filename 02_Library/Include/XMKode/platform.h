/* --------------------------------------------------------------------------
 *
 *      File            platform.h
 *      Description     Platform-specific types and definitions for XMKode
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

#ifndef __platform_h__
#define __platform_h__

#ifndef XM_EMULATOR_X
#define XM_EMULATOR_X                     50
#endif

#ifndef XM_EMULATOR_Y
#define XM_EMULATOR_Y                     50
#endif

#ifndef XM_EMULATOR_CX
#define XM_EMULATOR_CX                    480
#endif

#ifndef XM_EMULATOR_CY
#define XM_EMULATOR_CY                    800
#endif

// for Win32
#ifndef XM_USE_FULLSCREEN					  
#if defined ( WINCE )
#define XM_USE_FULLSCREEN				  1
#else
#define XM_USE_FULLSCREEN				  0
#endif
#endif  

// for iOS
#ifndef XM_USE_RETINA
#define XM_USE_RETINA                     1
#endif

#if defined ( ANDROID )

#include <jni.h>                    

#ifdef __cplusplus
extern "C"
{
#endif

// xmJNIInitLibrary : Initialize XMKode library in "Android" platform. 
jint JNICALL xmJNIInitLibrary ( JNIEnv* env, jobject obj, jobjectArray ar );           
jint JNICALL Java_XMKode_XMNative_InitLibrary ( JNIEnv* env, jobject obj, jobjectArray ar );

#ifdef __cplusplus
}
#endif

#define XM_APP_MAIN_BEGIN                                                                                      \
jint JNICALL Java_XMKode_XMNative_InitLibrary ( JNIEnv* env, jobject obj, jobjectArray ar )                    \
{                                                                                                              \
    xmJNIInitLibrary ( env, obj, ar );

#define XM_APP_MAIN_END                          return xmGetWindowMode ( ); }

// end of ANDROID
#elif defined ( SHP ) || defined ( TIZEN )

#include <osp/FOspConfig.h>

#ifdef __cplusplus
extern "C"
{
#endif 	// __cplusplus
_OSP_EXPORT_ int OspMain ( int argc, char *argv[] );
#ifdef __cplusplus
}
#endif	// __cplusplus

#define XM_APP_MAIN_BEGIN                                                                                      \
int OspMain ( int argc, char *argv[] )                                                                         \
{                                                                                                              \
    xmInitLibrary ( argc, argv );

// end of SHP
#elif defined ( WIN32 ) || defined ( _WIN32_WCE )
    
#include <windows.h>
#include <tchar.h>

#undef s_addr

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct
{
	HWND     hwnd;
	UINT     msg;
	WPARAM   wparam;
	LPARAM   lparam;
} KDEventNativeWin32;

// xmWinInitLibrary : Initialize XMKode library in "Win32" platform. 
KD_API KDvoid KD_APIENTRY xmWinInitLibrary ( HINSTANCE inst, HINSTANCE prev_inst, LPTSTR cmd_line, KDint cmd_show ); 

// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDint x, KDint y, KDuint cx, KDuint cy, KDbool fullscreen );

#ifdef __cplusplus
}
#endif

#define XM_APP_MAIN_BEGIN                                                                                      \
int WINAPI _tWinMain ( HINSTANCE inst, HINSTANCE prev_inst, LPTSTR cmd_line, int cmd_show )                    \
{                                                                                                              \
    xmWinInitLibrary ( inst, prev_inst, cmd_line, cmd_show );    

#define XM_APP_PROPERTY_SET                          xmWinSetProperty ( XM_EMULATOR_X, XM_EMULATOR_Y, XM_EMULATOR_CX, XM_EMULATOR_CY, XM_USE_FULLSCREEN );

// end of WIN32
#elif defined ( Linux ) 
    
#ifdef __cplusplus
extern "C"
{
#endif

#include <X11/Xlib.h>

typedef struct 
{
	Display*  display;
	Window    root;
	Window    wnd;
	XEvent*   event;
} KDEventNativeLinux;

// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDint x, KDint y, KDuint cx, KDuint cy );

#ifdef __cplusplus
}
#endif

#define XM_APP_PROPERTY_SET                          xmWinSetProperty ( XM_EMULATOR_X, XM_EMULATOR_Y, XM_EMULATOR_CX, XM_EMULATOR_CY );
 
// end of Linux
#elif defined ( __APPLE__ )
    
#ifdef __cplusplus
extern "C"
{
#endif
    
// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDbool use_retina );
    
#ifdef __cplusplus
}
#endif
    
#define XM_APP_PROPERTY_SET                          xmWinSetProperty ( XM_USE_RETINA );
    
#endif

#ifndef XM_APP_MAIN_BEGIN
  
#define XM_APP_MAIN_BEGIN                                                                                      \
int main ( int argc, char *argv[] )                                                                            \
{                                                                                                              \
    xmInitLibrary ( argc, argv );


#endif

#ifndef XM_APP_MAIN_END

#define XM_APP_MAIN_END                              xmFreeLibrary ( ); return 0; }

#endif

#ifndef XM_APP_PROPERTY_SET
#define XM_APP_PROPERTY_SET
#endif

#define XM_APP_WND_REALIZE( MODE )                   XM_APP_PROPERTY_SET; xmRealizeWindow ( XM_OPEN_GL_MODE | MODE, &xmEventProc );  

#define XMKODE_LAUNCH( _MODE_ )                                     \
XM_APP_MAIN_BEGIN													\
	XM_APP_PROPERTY_SET;											\
	xmRealizeWindow ( XM_OPEN_GL_MODE | _MODE_, &xmEventProc );		\
XM_APP_MAIN_END
    
#endif // __platform_h__
