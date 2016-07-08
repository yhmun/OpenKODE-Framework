/* --------------------------------------------------------------------------
 *
 *      File            kd_library.h
 *      Description     Library-specific definitions and functions for XMKode 
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

#ifndef __kd_library_h__
#define __kd_library_h__

#define  XM_INCLUDE_NATIVE

#include "kode.h"

#if     defined ( ANDROID )

#define XM_HAS_PLATFORM_EGL
#define XM_HAS_PLATFORM_LOG
#define XM_HAS_PLATFORM_FREE_SPACE
#define XM_HAS_PLATFORM_KEYPAD
#define XM_HAS_PLATFORM_EXIT
#define XM_HAS_PLATFORM_FRAME_INTERVAL
#define XM_HAS_PLATFORM_IME_KEYBOARD
#define XM_HAS_PLATFORM_TOUCH_FLIP_Y
//#define XM_HAS_PLATFORM_SOUND_REPEAT
#define XM_HAS_PLATFORM_VIBRATE
#define XM_HAS_PLATFORM_BRIGHTNESS
#define XM_HAS_PLATFORM_FULLSCREEN
#define XM_HAS_PLATFORM_TTS

#include "Android/android_kode.h"

#elif   defined ( BLACKBERRY )

#define XM_HAS_PLATFORM_EGL
#define XM_HAS_PLATFORM_LOG
#define XM_HAS_PLATFORM_SLEEP

#include "BlackBerry/bb_kode.h"

#elif   defined ( TIZEN )

#define XM_HAS_PLATFORM_LOG
#define XM_HAS_PLATFORM_FREE_SPACE
#define XM_HAS_PLATFORM_KEYPAD
#define XM_HAS_PLATFORM_SLEEP
#define XM_HAS_PLATFORM_EXIT

#include "Tizen/tizen_kode.h"

#elif   defined ( SHP )

#define XM_HAS_PLATFORM_LOG
#define XM_HAS_PLATFORM_FREE_SPACE
#define XM_HAS_PLATFORM_KEYPAD
#define XM_HAS_PLATFORM_SLEEP
#define XM_HAS_PLATFORM_EXIT

#include "Bada/bada_kode.h"

#elif   defined ( WIN32 ) | defined ( _WIN32_WCE )

#define XM_HAS_PLATFORM_ERRNO
#define XM_HAS_PLATFORM_LOG
#define XM_HAS_PLATFORM_CRYPTO
#define XM_HAS_PLATFORM_FREE_SPACE
#define XM_HAS_PLATFORM_KEYPAD
#define XM_HAS_PLATFORM_SLEEP
#define XM_HAS_PLATFORM_AUDIO_TYPE
#define XM_HAS_VIRTUAL_ACCELEROMETER

#if defined ( USE_OPEN_GL )
#define XM_HAS_PLATFORM_EGL
#endif

#include "Win32/win32_kode.h"

#elif   defined ( MACOSX )

#include "Macosx/mac_kode.h"

#elif   defined ( __APPLE__ )

#define XM_HAS_PLATFORM_EGL
#define XM_HAS_PLATFORM_FRAME_INTERVAL
#define XM_HAS_PLATFORM_IME_KEYBOARD
#define XM_HAS_PLATFORM_VIBRATE
#define XM_HAS_PLATFORM_BRIGHTNESS
#define XM_HAS_PLATFORM_FULLSCREEN
//#define XM_HAS_PLATFORM_TTS			// XMOpenEars implemented instead of XMKode

#include "iOS/ios_kode.h"

#elif   defined ( Linux )

#define XM_HAS_PLATFORM_KEYPAD
#define XM_HAS_VIRTUAL_ACCELEROMETER

#include "Linux/linux_kode.h"

#elif   defined ( Crosstool )

#define XM_HAS_PLATFORM_EGL

#include "Crosstool/cross_kode.h"

#endif

#define	XM_LIBRARY_VENDER                 "XMSoft"
#define XM_LIBRARY_VERSION                "1.70.0103"

#if     defined ( Platform_Win32 )
#define XM_LIBRARY_PLATFORM               "Win32"		
#elif   defined ( Platform_Nexus_s3c6410 )
#define XM_LIBRARY_PLATFORM               "Nexus_s3c6410 (Armv4I)"	
#elif   defined ( Platform_Omnia_II )
#define XM_LIBRARY_PLATFORM               "Omnia_II (Armv4I)"
#elif   defined ( Platform_SiRFPrima_EVB_CE6 )
#define XM_LIBRARY_PLATFORM               "Platform_SiRFPrima_EVB_CE6_SDK (ARMV4I)"
#define XM_HAS_PLATFORM_FILE_LOG   
#elif   defined ( Platform_TCC8801 )
#define XM_LIBRARY_PLATFORM               "Platform_TCC8801_SDK (ARMV4I)"        
#elif   defined ( Platform_macosx )
#define XM_LIBRARY_PLATFORM               "Platform_macosx"
#elif   defined ( Platform_iphoneos )
#define XM_LIBRARY_PLATFORM               "iphoneos"
#elif   defined ( Platform_iphonesimulator )
#define XM_LIBRARY_PLATFORM               "iphonesimulator"
#elif   defined ( Platform_Linux )		
#define XM_LIBRARY_PLATFORM               "Linux"
#elif   defined ( Platform_Crosstool )
#define XM_LIBRARY_PLATFORM               "Crosstool"
#elif   defined ( Platform_Tizen_Simulator )
#define XM_LIBRARY_PLATFORM               "TizenSimulator"
#elif   defined ( Platform_BlackBerry10_Simulator )
#define XM_LIBRARY_PLATFORM               "BlackBerry10_Simulator"
#elif   defined ( Platform_BlackBerry10 )
#define XM_LIBRARY_PLATFORM               "BlackBerry10"
#elif   defined ( Platform_WaveWVGA )
#define XM_LIBRARY_PLATFORM               "WaveWVGA"
#elif   defined ( Platform_Wave )
#define XM_LIBRARY_PLATFORM               "Wave_LP1"
#elif   defined ( Platform_Wave_simulator )
#define XM_LIBRARY_PLATFORM               "Wave_LP1_Simulator"
#elif   defined ( Platform_Android_armeabi )
#define XM_LIBRARY_PLATFORM               "Android_armeabi"
#endif

#ifndef XM_HAS_PLATFORM_EGL

#include "EGL/egl.h"
#include "EGL/egl_ext.h"

#endif


#ifdef __cplusplus
extern "C"
{
#endif

#define XM_DIRECTORY_COUNT  7

typedef struct XMContext    XMContext;

struct KDFile
{
	FILE*                   ptr;
	KDchar*                 path;
	KDubyte*                mem;
	KDoff                   off;
	KDsize                  size;	
};

struct KDDir
{
	DIR*					ptr;
	KDuint					idx;
	KDuint					cnt;	
	KDchar					path[256];
	KDDirent				dirent;
};

struct KDThread
{
	pthread_t               pthread;
};

struct XMContext
{	
	KDThread*               thread;
	XMQueue*                timers;
	XMQueue*                events;
	XMQueue*                callbacks;		
	KDvoid*                 userptr;
	KDThreadStorageKeyKHR   key;
};

struct XMKeyCode 
{
	KDint                   kd_code;
	KDint                   win_code;
};

struct XMSound
{
	KDvoid*                 device;
	const KDchar*	        path;
	KDchar*			        fpath;
	KDchar*			        vpath;
	KDbool                  play;
	KDbool                  pause;
	KDbool                  repeat;
	KDint			        mode;
	KDfloat			        volume;
	KDfloat					pitch;
    KDfloat                 pan;
	KDCallbackFunc*         func;
};

// xmGetContexts : Get context queue.
XMQueue* xmGetContexts ( KDvoid );

// xmCreateContext : Create context.
XMContext* xmCreateContext ( KDvoid );

// xmFreeContext : Free context.
KDvoid xmFreeContext ( XMContext* context );

// xmFindContext : Find context with thread.
XMContext* xmFindContext ( KDThread* thread );

// xmCreateTSS : Create thread storage queue.
KDvoid xmCreateTSS ( KDvoid );

// xmFreeTSS : Free thread storage queue.
KDvoid xmFreeTSS ( KDvoid );

// xmFreeTS : Free thread storage.
KDvoid xmFreeTS ( KDThreadStorageKeyKHR key );

// xmGetErrno : Get last error indication. 
KDint xmGetErrno ( KDvoid );

// xmSetErrno : Set last error indication. 
KDvoid xmSetErrno ( KDint err );

// xmFireTimer : Fire timer.
KDvoid xmFireTimer ( XMContext* context );

// Initialize Directory Paths
KDvoid xmInitDirectory ( KDvoid );

// Release Directory Paths
KDvoid xmFreeDirectory ( KDvoid );

// Initialize NMEA
KDvoid xmInitNMEA ( KDvoid );

// Release NMEA
KDvoid xmFreeNMEA ( KDvoid );

// Initialize XMKode library in Platfrom specific.
KDvoid xmPlatformInitLibrary ( KDvoid );

// xmPlatformFreeLibrary : Release XMKode library's resources in platform specific.
KDvoid xmPlatformFreeLibrary ( KDvoid );

// xmPlatformMapDirectoryPath : Map absoulte path to virtual path.
KDvoid xmPlatformMapDirectoryPath ( KDchar** paths );

KDvoid xmInitTickCount ( KDvoid );

// xmPlatformGetTimeUST : Get the current unadjusted system time in platform specific.
KDust xmPlatformGetTimeUST ( KDvoid );

// xmPlatformUSTAtEpoch : Get the UST corresponding to KDtime 0 in platform specific.
KDust xmPlatformUSTAtEpoch ( KDvoid );

// kdGetLocale : Determine the current language and locale in platform specific.
const KDchar* xmPlatformGetLocale ( KDvoid );

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode );

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid );

#ifdef  XM_HAS_PLATFORM_EXIT
KDvoid xmPlatformExit ( KDint status );
#endif

#ifdef XM_HAS_PLATFORM_ERRNO

// xmPlatformGetErrno : Get last error indication in platform specific.
KDint  xmPlatformGetErrno ( KDvoid );
// xmPlatformSetErrno : Set last error indication in platform specific.
KDvoid xmPlatformSetErrno ( KDint err );

#endif

#ifdef XM_HAS_PLATFORM_LOG

// xmPlatformLogMessage : Output a log message in platform specific.
KDvoid xmPlatformLogMessage ( const KDchar* msg );

#endif

#ifdef XM_HAS_PLATFORM_FILE_LOG

// xmPlatformFileLog : Output a log message to file.
KDvoid xmPlatformFileLog ( const KDchar* str );

#endif

#ifdef XM_HAS_PLATFORM_SLEEP

// xmPlatformSleep : The function sleep gives a simple way to make the program wait for a short interval.
KDvoid xmPlatformSleep ( KDust ust );

#endif

#ifdef XM_HAS_PLATFORM_CRYPTO

KDint  xmPlatformCryptoInit	( KDvoid );
KDvoid xmPlatformCryptoFree	( KDvoid );
// xmPlatformCryptoRand : Return random data in in platform specific.
KDint  xmPlatformCryptoRand	( KDuint8* buf, KDsize buflen );

#endif

#ifdef XM_HAS_PLATFORM_FREE_SPACE

// xmPlatformGetFree : Get free space on a drive in in platform specific.
KDoff xmPlatformGetFree ( const KDchar* path );

#endif

#ifdef XM_HAS_PLATFORM_KEYPAD

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code ); 

#endif

#ifndef XM_HAS_PLATFORM_EGL

// xmPlatformGetEGLNative : Get EGL native window and display type.
KDvoid xmPlatformGetEGLNative ( EGLNativeWindowType* native_window, EGLNativeDisplayType* native_display ); 

#endif

#ifdef XM_HAS_PLATFORM_FRAME_INTERVAL

// xmPlatformSetFrameInterval : Set Frame Interval.
// If input value is 0 then no interval timing.
KDvoid xmPlatformSetFrameInterval ( KDdouble interval );

#endif

#ifdef  XM_HAS_PLATFORM_IME_KEYBOARD

KDvoid xmPlatformSetIMEKeyboardState ( KDbool show );

#endif

KDvoid  xmPlatformEventAttach ( KDenum mode );
KDvoid  xmPlatformEventDetach ( KDenum mode );

KDbool  xmIsNeededUpdateNMEA ( KDbool fromGPS );

KDvoid  xmCreateSounds ( KDvoid );
KDvoid  xmFreeSounds   ( KDvoid );

KDvoid  xmSoundAutoPause ( KDbool state );
KDvoid  xmSoundCompleted ( KDvoid* device );

KDint   xmPlatformSoundOpen   ( XMSound* sound );
KDint   xmPlatformSoundClose  ( XMSound* sound );

KDint   xmPlatformSoundPlay   ( XMSound* sound );
KDint   xmPlatformSoundStop   ( XMSound* sound );
KDint   xmPlatformSoundPause  ( XMSound* sound );
KDint   xmPlatformSoundResume ( XMSound* sound );

#ifdef  XM_HAS_PLATFORM_SOUND_REPEAT
KDint   xmPlatformSoundSetRepeat ( XMSound* sound, KDint on ); 
#endif

KDint   xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume );
KDfloat xmPlatformSoundGetVolume ( XMSound* sound ); 

KDint   xmPlatformSoundSetPitch ( XMSound* sound, KDfloat pitch );
KDfloat xmPlatformSoundGetPitch ( XMSound* sound ); 

KDint   xmPlatformSoundSetPan ( XMSound* sound, KDfloat pan );
KDfloat xmPlatformSoundGetPan ( XMSound* sound );
    
KDint   xmPlatformSoundGetDuration ( XMSound* sound ); 
KDint   xmPlatformSoundGetPosition ( XMSound* sound ); 
KDint	xmPlatformSoundSetPosition ( XMSound* sound, KDint pos ); 

#ifdef  XM_HAS_PLATFORM_VIBRATE
KDvoid  xmPlatformVibrate ( KDuint msec );
#endif

#ifdef	XM_HAS_PLATFORM_BRIGHTNESS
KDvoid  xmPlatformSetBrightness ( KDfloat value );
#endif

#ifdef	XM_HAS_PLATFORM_FULLSCREEN
KDvoid  xmPlatformSetFullScreen ( KDbool enable );
#endif

#ifdef XM_HAS_PLATFORM_OPEN_URL
KDvoid  xmPlatformOpenURL ( const KDchar* url );
#endif


#ifdef XM_HAS_PLATFORM_TTS
KDint   xmPlatformTTSCreate ( KDvoid );
KDint   xmPlatformTTSFree   ( KDvoid );

KDint   xmPlatformTTSSpeak ( const KDchar* text, KDint mode );
KDint   xmPlatformTTSStop  ( KDvoid );

KDint   xmPlatformTTSSetLocale ( KDint locale );
KDint   xmPlatformTTSSetPitch ( KDfloat pitch );
KDint   xmPlatformTTSSetRate  ( KDfloat rate );
#endif

KDEvent* _xmEventNew             ( KDvoid );
KDvoid   _xmEventSend            ( const KDEvent* event );
KDvoid   _xmEventCreate          ( XMWindow window );
KDvoid   _xmEventDestroy         ( KDvoid );
KDvoid   _xmEventRedraw          ( KDvoid );
KDvoid   _xmEventUpdate          ( KDvoid );
KDvoid   _xmEventMemoryWarning   ( KDvoid );
KDvoid   _xmEventResize          ( KDint width, KDint height );
KDvoid   _xmEventTouchBegan      ( KDTouch* touches, KDuint count );
KDvoid   _xmEventTouchMoved      ( KDTouch* touches, KDuint count );
KDvoid   _xmEventTouchEnded      ( KDTouch* touches, KDuint count );
KDvoid   _xmEventTouchCancelled  ( KDTouch* touches, KDuint count );
KDvoid   _xmEventMousePressed    ( KDint x, KDint y, KDint code );
KDvoid   _xmEventMouseReleased   ( KDint x, KDint y, KDint code );
KDvoid   _xmEventMouseMoved      ( KDint x, KDint y );
KDvoid   _xmEventMouseScrolled   ( KDint x, KDint y );
KDvoid   _xmEventKeyPressed      ( KDint code );
KDvoid   _xmEventKeyReleased     ( KDint code );
KDvoid   _xmEventVisible         ( KDbool state );
KDvoid   _xmEventFocus           ( KDbool state );
KDvoid   _xmEventProvider		 ( KDint provider );
KDvoid   _xmEventMagnetometer    ( KDEventMagnetometer magnetometer );
KDvoid   _xmEventAccelerometer   ( KDfloat x, KDfloat y, KDfloat z );
KDvoid   _xmEventInsertText      ( const KDchar* text, KDsize len );
KDvoid   _xmEventImeNotification ( KDint status, KDRect begin, KDRect end, KDfloat duration );
    
/* CS Smart Game event */
KDvoid	 _xmEventPointing		 ( KDint x, KDint y );
KDvoid	 _xmEventQuaternion		 ( KDint q1, KDint q2, KDint q3, KDint q4 );
KDvoid	 _xmEventEuler			 ( KDfloat roll, KDfloat pitch, KDfloat yaw );

extern  KDint       g_argc;
extern  KDchar**    g_argv;
extern  KDint       g_crypto;
extern  KDdouble    g_interval;  

// For Cocos2D
extern  KDPoint     g_contentScale;
extern  KDRect      g_viewportRect;
    
#ifdef __cplusplus
}
#endif

#endif
