/* --------------------------------------------------------------------------
 *
 *      File            kode.h
 *      Description     XMSoft's revision of the OpenKODE Core v1.0.3
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

#ifndef __kode_h__
#define __kode_h__

#include "KD/kd.h"

#include "KHR/khr_float64.h"
#include "KHR/khr_formatted.h"
#include "KHR/khr_thread_storage.h"
#include "KHR/khr_perfcounter.h"

#include "ATX/atx_imgdec.h"
#include "ATX/atx_imgdec_jpeg.h"
#include "ATX/atx_imgdec_png.h"
#include "ATX/atx_imgdec_pvr.h"
#include "ATX/atx_dxtcomp.h"
#include "ATX/atx_bluetooth.h"

#include "XM/xm_queue.h"
#include "XM/xm_vector.h"
#include "XM/xm_matrix.h"
#include "XM/xm_sound.h"
#include "XM/xm_nmea.h"
#include "XM/xm_tts.h"
#include "XM/xm_system.h"

#define XM_SYS_DEFAULT						    XM_SYS_QUIT | XM_SYS_LOG

#define XM_SYS_PORTRAIT						    0x00000000
#define XM_SYS_LANDSCAPE					    0x10000000	
#define XM_SYS_LOCATION						    0x20000000	// GPS and Cell 
#define XM_SYS_ACCELEROMETER				    0x40000000	// Accelero meter sensor ( -1.0 ~ 1.0 )
#define XM_SYS_MAGNETOMETER					    0x80000000	// Magnetic meter sensor ( -359 ~ 360 )
#define XM_SYS_GL_NONE						    0x00000000	// No OpenGL
#define XM_SYS_GLES_V1                          0x01000000	// OpenGL ES 1.x
#define XM_SYS_GLES_V2                          0x02000000	// OpenGL ES 2.x
#define XM_SYS_GL                               0x04000000  // OpenGL
#define XM_SYS_RGB8888                          0x00000000
#define XM_SYS_RGB565                           0x00100000
#define XM_SYS_MSAA                             0x00200000	// Multi Sample Anti-Alias
#define XM_SYS_QUIT                             0x00000001	// When Top-Right Conner Clicked, Application will be ternimated.
#define XM_SYS_STATUS_BAR                       0x00000002	// Show Status Bar
#define XM_SYS_LOG                              0x00000008	

#if defined ( USE_OPEN_GLES2 )

#include <GLES2/gl2.h>
#include <GLES2/gl2_ext.h>

#define XM_OPEN_GL_MODE                         XM_SYS_GLES_V2

#elif defined ( USE_OPEN_GLES )

#include <GLES/gl.h>					        
#include <GLES/gl_ext.h>				        

#define XM_OPEN_GL_MODE                         XM_SYS_GLES_V1	

#elif defined ( USE_OPEN_GL ) 

//#include <windows.h>              
#include <GL/glew.h>
#include <GL/gl.h>

#undef  s_addr

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG		0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG		0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG		0x8C03
		 
#define GL_ATC_RGB_AMD							0x8C92
#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD			0x8C93
#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD		0x87EE

#define GL_ETC1_RGB8_OES						0x8D64

#define XM_OPEN_GL_MODE					        XM_SYS_GL						  	

#elif defined ( USE_OPEN_GL_NONE ) 

#include <GLES/gl.h>					        // Temp code
#include <GLES/gl_ext.h>				        // Temp code
		 
#define XM_OPEN_GL_MODE				            XM_SYS_GL_NONE

#else	

#include <GLES/gl.h>					        
#include <GLES/gl_ext.h>				        

#define XM_OPEN_GL_MODE                         XM_SYS_GLES_V1	

#endif // USE_OPEN_GL_MODE

#ifdef __cplusplus
extern "C"
{
#endif

typedef KDvoid*     XMWindow;	

// xmPlatformInitLibrary : Initialize XMKode library in platform specific.
KD_API KDvoid KD_APIENTRY xmInitLibrary ( KDint argc, KDchar *argv[] );

// xmFreeLibrary : Release XMKode library's resources.
KD_API KDvoid KD_APIENTRY xmFreeLibrary ( KDvoid );

// xmRealizeWindow : Realize window and ready event loop.
KD_API KDvoid KD_APIENTRY xmRealizeWindow ( KDmode mode, KDCallbackFunc* func ); 

// xmGetArgc : Get argument count.
KD_API KDint KD_APIENTRY xmGetArgc ( KDvoid );

// xmGetArgv : Get argument variables.
KD_API KDchar** KD_APIENTRY xmGetArgv ( KDvoid );

// xmGetWindow : Get window handle.
KD_API XMWindow KD_APIENTRY xmGetWindow ( KDvoid );
	
// xmGetWindowMode : Get window mode.
KD_API KDmode KD_APIENTRY xmGetWindowMode ( KDvoid );

// xmGetNativePath : Get native path.
KD_API KDchar* KD_APIENTRY xmGetNativePath ( const KDchar* path, KDchar** vpath );

// Application event callback functions' template.
KD_API KDvoid KD_APIENTRY xmEventProc ( const KDEvent* event );

// xmGetTouches : Get touches information.
KD_API KDTouch* KD_APIENTRY xmGetTouches ( KDvoid );

// xmGetTouchPressedCount : Get pressed touches count.
KD_API KDuint KD_APIENTRY xmGetPressedTouchesCount ( KDvoid );

// xmSetFrameInterval : Set Frame Interval. 
// If input value is 0 then no interval timing. 
KD_API KDvoid KD_APIENTRY xmSetFrameInterval ( KDdouble interval );

// xmMeasureFramePerSecond : Measure FPS during 1 second.
KD_API KDdouble KD_APIENTRY xmMeasureFramePerSecond ( KDvoid );

KD_API KDvoid KD_APIENTRY xmSetApnsMessage ( const KDchar* message );
KD_API const KDchar* KD_APIENTRY xmGetApnsMessage ( KDvoid );

typedef KDvoid* XMAREM;
KD_API XMAREM KD_APIENTRY xmGetAREM ( KDvoid );
    
#ifdef __cplusplus
}
#endif

#endif 
