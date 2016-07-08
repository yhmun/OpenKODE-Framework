/* --------------------------------------------------------------------------
 *
 *      File            android_window.c
 *      Description     Windows functions
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

#include "../kd_library.h"
	
#define GRAVITY_EARTH          (9.80665f)

static KDmode        l_mode    = 0;
static KDThread*     l_thread  = 0;
static JavaVM*       l_vm      = 0;
static JNIEnv*       l_env     = 0;
static jclass        l_class   = 0;
static KDbool        l_create  = KD_FALSE;
static KDTouch       l_touches [ KD_TOUCH_MAX ]; 
static KDbool        l_gps     = KD_FALSE;

extern KDint16*		 g_total_num_sats_in_view;

jint JNI_OnLoad ( JavaVM* vm, void* reserved )
{
	l_vm = vm;
	
	return JNI_VERSION_1_4;
}

static jmethodID getMethodID ( const char* methodName, const char *paramCode )
{
	jmethodID  ret = 0;

	if ( (*l_vm)->GetEnv ( l_vm, (void**) &l_env, JNI_VERSION_1_4) != JNI_OK)
	{
		kdLogMessagefKHR ( "Failed to get the environment using GetEnv()" );
		return 0;
	}

	if ( (*l_vm)->AttachCurrentThread ( l_vm, &l_env, 0 ) < 0 )
	{
		kdLogMessagefKHR ( "Failed to get the environment using AttachCurrentThread()" );
		return 0;
	}

	l_class = (*l_env)->FindClass ( l_env, "XMKode/XMNative" );
	if ( !l_class )
	{
		kdLogMessagefKHR ( "Failed to find class of XMKode/XMNative" );
		return 0;
	}

	if ( l_env != 0 && l_class != 0 )
	{
		ret = (*l_env)->GetStaticMethodID ( l_env, l_class, methodName, paramCode );
	}

	if ( !ret )
	{
		kdLogMessagefKHR ( "get method id of %s error", methodName );
	}

	return ret;
}

KDvoid xmPlatformExit ( KDint status )
{
	jmethodID  method = getMethodID ( "Exit", "(I)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, status );
	}
}

// xmPlatformSetFrameInterval : Set Frame Interval.
// If input value is 0 then no interval timing.
KDvoid xmPlatformSetFrameInterval ( KDdouble interval )
{
	jmethodID  method = getMethodID ( "SetFrameInterval", "(D)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, interval );
	}	
}


KDvoid xmPlatformEventAttach ( KDenum mode )
{
	jmethodID  method = getMethodID ( "SetAttach", "(I)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, (jint) mode );
	}	  
}

KDvoid xmPlatformEventDetach ( KDenum mode )
{
	jmethodID  method = getMethodID ( "SetDetach", "(I)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, (jint) mode );
	}	    
}

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundOpen", "(IILjava/lang/String;)I" );

	if ( method )
	{	
		jstring  jpath  = 0;
		jint     jasset = 0;
		
		if ( !kdStrcmp ( sound->vpath, "/res" ) )
		{
			jpath  = (*l_env)->NewStringUTF ( l_env, &sound->path [ 5 ] );
			jasset = 1;
		}
		else
		{
			jpath = (*l_env)->NewStringUTF ( l_env, sound->fpath );
			jasset = 0;
		}

		sound->device = (KDvoid*) (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) ( sound->mode & XM_SOUND_EFFECT ), jasset, jpath );
	}

	return sound->device ? 0 : -1;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundClose", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundPlay", "(IF)I" );

	if ( method )
	{
		KDfloat  volume = xmSoundGetVolumes ( sound->mode ) * sound->volume;

		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, volume );
	}

	return -1;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundStop", "(I)I" );
	
	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}	

	return -1;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundPause", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundResume", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

#ifdef XM_HAS_PLATFORM_SOUND_REPEAT
KDint xmPlatformSoundSetRepeat ( XMSound* sound, KDint on )
{
	jmethodID  method = getMethodID ( "SoundSetRepeat", "(II)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, (jint) on );
	}

	return -1;
}
#endif

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume )
{
	jmethodID  method = getMethodID ( "SoundSetVolume", "(IF)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, volume );
	}

	return -1;
}

KDint xmPlatformSoundSetPitch ( XMSound* sound, KDfloat pitch )
{
	jmethodID  method = getMethodID ( "SoundSetVolume", "(IF)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, pitch );
	}

	return -1;
}


KDint xmPlatformSoundSetPan ( XMSound* sound, KDfloat pan )
{
	jmethodID  method = getMethodID ( "SoundSetPan", "(IF)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, pan );
	}

	return -1;
}

KDfloat xmPlatformSoundGetVolume ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundGetVolume", "(I)F" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundGetDuration", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	jmethodID  method = getMethodID ( "SoundGetPosition", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device );
	}

	return -1;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint pos )
{
	jmethodID  method = getMethodID ( "SoundSetPosition", "(II)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) sound->device, (jint) pos );
	}

	return -1;
}

KDvoid xmPlatformSetIMEKeyboardState ( KDbool show )
{
	jmethodID  method = getMethodID ( "SetIMEKeyboardState", "(I)I" );
	
	if ( method )
	{
		(*l_env)->CallStaticIntMethod ( l_env, l_class, method, (jint) show );
	}	
}

KDvoid xmPlatformVibrate ( KDuint msec )
{
    jmethodID  method = getMethodID ( "Vibrate", "(I)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, msec );
	}
}

KDvoid xmPlatformSetBrightness ( KDfloat value )
{
    jmethodID  method = getMethodID ( "SetBrightness", "(F)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, value );
	}
}

KDvoid xmPlatformSetFullScreen ( KDbool enable )
{
    jmethodID  method = getMethodID ( "SetFullScreen", "(I)V" );

	if ( method )
	{
		(*l_env)->CallStaticVoidMethod ( l_env, l_class, method, enable );
	}
}

KDint xmPlatformTTSCreate ( KDvoid )
{
    jmethodID  method = getMethodID ( "TTSCreate", "()I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method );
	}

	return -1;
}

KDint xmPlatformTTSFree ( KDvoid )
{
    jmethodID  method = getMethodID ( "TTSFree", "()I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method );
	}

	return -1;
}

KDint xmPlatformTTSSpeak ( const KDchar* text, KDint mode )
{
    jmethodID  method = getMethodID ( "TTSSpeak", "(Ljava/lang/String;I)I" );

	if ( method )
	{
		jstring  jtext = (*l_env)->NewStringUTF ( l_env, text );
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, jtext, mode );
	}

	return -1;
}

KDint xmPlatformTTSStop ( KDvoid )
{
    jmethodID  method = getMethodID ( "TTSStop", "()I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method );
	}

	return -1;
}

KDint xmPlatformTTSSetLocale ( KDint locale )
{
    jmethodID  method = getMethodID ( "TTSSetLocale", "(I)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, locale );
	}

	return -1;
}

KDint xmPlatformTTSSetPitch ( KDfloat pitch )
{
    jmethodID  method = getMethodID ( "TTSSetPitch", "(F)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, pitch );
	}

	return -1;
}

KDint xmPlatformTTSSetRate ( KDfloat rate )
{
    jmethodID  method = getMethodID ( "TTSSetRate", "(F)I" );

	if ( method )
	{
		return (*l_env)->CallStaticIntMethod ( l_env, l_class, method, rate );
	}

	return -1;
}

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code )
{
	static struct XMKeyCode  codes[ ] =
	{	
		{ KD_INPUT_XMSOFT_PREV              , 0x00000004    },   
		{ KD_INPUT_XMSOFT_MENU              , 0x00000052    },   
        { KD_INPUT_XMSOFT_BACKSPACE         , KD_INPUT_XMSOFT_BACKSPACE },  

        { KD_INPUT_PHONEKEYPAD_0            , 0x00000090    },   
        { KD_INPUT_PHONEKEYPAD_1            , 0x00000091    },   
        { KD_INPUT_PHONEKEYPAD_2            , 0x00000092    },   
        { KD_INPUT_PHONEKEYPAD_3            , 0x00000093    },   
        { KD_INPUT_PHONEKEYPAD_4            , 0x00000094    },   
        { KD_INPUT_PHONEKEYPAD_5            , 0x00000095    },   
        { KD_INPUT_PHONEKEYPAD_6            , 0x00000096    },   
        { KD_INPUT_PHONEKEYPAD_7            , 0x00000097    },   
        { KD_INPUT_PHONEKEYPAD_8            , 0x00000098    },   
        { KD_INPUT_PHONEKEYPAD_9            , 0x00000099    },   
        { KD_INPUT_PHONEKEYPAD_STAR         , 0x0000009b    },   
        { KD_INPUT_PHONEKEYPAD_HASH         , 0x00000012    },  

        { KD_INPUT_GAMEKEYS_UP              , 0x00000013    },   
        { KD_INPUT_GAMEKEYS_LEFT            , 0x00000015    },   
        { KD_INPUT_GAMEKEYS_RIGHT           , 0x00000016    },   
        { KD_INPUT_GAMEKEYS_DOWN            , 0x00000014    },   
        { KD_INPUT_GAMEKEYS_FIRE            , 0x00000017    },  

		{ 0                                 , 0				},
	};	
	
	KDint      idx;
	
	for ( idx = 0; codes[ idx ].kd_code != 0; idx++ )
	{
		if( codes[ idx ].win_code == win_code )
		{
			return codes[ idx ].kd_code;
		}		
	}

	return 0;
}

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode )
{
	XMContext*      context;

	context = (XMContext *) xmQueueGet ( xmGetContexts ( ), 0 );

	l_mode = mode;	
	l_thread = context->thread;	
}

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid )
{

}

// xmJNIPostEvent : Post an event from Java Invoked into C system.
jint JNICALL Java_XMKode_XMNative_PostEvent ( JNIEnv* env, jobject obj, jint type, jobject ar )
{		
	switch ( type )
	{
		case KD_EVENT_INSERT_TEXT :
			{
				jboolean     copy = 0;
				jstring       str = (jstring) ar;
				const char*  text = ( *env )->GetStringUTFChars ( env, str, &copy );

				_xmEventInsertText ( text, kdStrlen ( text ) );

				( *env )->ReleaseStringUTFChars ( env, str, text );
			}
			break;

		case KD_EVENT_LOCATION + 1 :

			if ( *g_total_num_sats_in_view > 0 )
			{
				return 0;
			}

		case KD_EVENT_LOCATION :
			{
				jboolean     copy = 0;
				jstring       str = (jstring) ar;
				const char*  text = ( *env )->GetStringUTFChars ( env, str, &copy );

				xmParseNMEA ( KD_TRUE, text, kdStrlen ( text ) );

				( *env )->ReleaseStringUTFChars ( env, str, text );
			}
			break;

		case KD_EVENT_PROVIDER :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventProvider ( val[ 0 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_CREATE :

			l_thread->pthread = pthread_self ( );

			if ( l_create == KD_FALSE )
			{
				_xmEventCreate ( (XMWindow) l_vm );	
				l_create = KD_TRUE;
			}

			break;

		case KD_EVENT_DESTROY :

			_xmEventDestroy ( );

			break;

		case KD_EVENT_REDRAW :

			_xmEventRedraw ( );
            _xmEventUpdate ( );
            
			break;

		//case KD_EVENT_UPDATE :
		//	break;

		case KD_EVENT_RESIZE :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventResize ( val[ 0 ], val[ 1 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_VISIBLE :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventVisible ( val[ 0 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_FOCUS :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventFocus ( val[ 0 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_COMPLETED :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );
				
				switch ( val[ 0 ] )
				{
					case KD_SOUND_COMPLETED :
						xmSoundCompleted ( (KDvoid*) val[ 1 ] );
						break;

					case KD_TTS_COMPLETED :
						_xmEventCompleted ( val[ 0 ], (KDvoid*) val[ 1 ] );
						break;
				}
				
				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_KEY_RELEASED :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventKeyReleased ( val[ 0 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_KEY_PRESSED :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				_xmEventKeyPressed ( val[ 0 ] );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;     

		case KD_EVENT_TOUCH_BEGAN :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				l_touches [ 0 ].id = (KDvoid*) ( val [ 0 ] + 0xFF000000 );
				l_touches [ 0 ].x  = val [ 1 ];
				l_touches [ 0 ].y  = val [ 2 ];

				_xmEventTouchBegan ( l_touches, 1 );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_TOUCH_MOVED :
			{
				KDint       size = ( *env )->GetArrayLength ( env, ar ) / 3;
				KDint*       val = ( *env )->GetIntArrayElements ( env, ar, 0 );
				KDint          i;

				if ( size >= KD_TOUCH_MAX )
				{
					size = KD_TOUCH_MAX - 1;
				}

				for ( i = 0; i < size; i++ )
				{
					l_touches [ i ].id = (KDvoid*) ( val [ i * 3 + 0 ] + 0xFF000000 );
					l_touches [ i ].x  = val [ i * 3 + 1 ];
					l_touches [ i ].y  = val [ i * 3 + 2 ];
				}

				_xmEventTouchMoved ( l_touches, size );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_TOUCH_ENDED :
			{
				KDint*    val = ( *env )->GetIntArrayElements ( env, ar, 0 );

				l_touches [ 0 ].id = (KDvoid*) ( val [ 0 ] + 0xFF000000 );
				l_touches [ 0 ].x  = val [ 1 ];
				l_touches [ 0 ].y  = val [ 2 ];

				_xmEventTouchEnded ( l_touches, 1 );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_TOUCH_CANCELLED :
			{
				KDint       size = ( *env )->GetArrayLength ( env, ar ) / 3;
				KDint*       val = ( *env )->GetIntArrayElements ( env, ar, 0 );
				KDint          i;
				
				if ( size >= KD_TOUCH_MAX )
				{
					size = KD_TOUCH_MAX - 1;
				}

				for ( i = 0; i < size; i++ )
				{
					l_touches [ i ].id = (KDvoid*) ( val [ i * 3 + 0 ] + 0xFF000000 );
					l_touches [ i ].x  = val [ i * 3 + 1 ];
					l_touches [ i ].y  = val [ i * 3 + 2 ];
				}

				_xmEventTouchCancelled ( l_touches, size );

				( *env )->ReleaseIntArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_MAGNETOMETER :
			{
				KDfloat*  val = ( *env )->GetFloatArrayElements ( env, ar, 0 );
				KDEventMagnetometer  magnetometer = { val[ 0 ], val[ 1 ], val[ 2 ], val[ 3 ], val[ 4 ], val[ 5 ] };

				_xmEventMagnetometer ( magnetometer );

				( *env )->ReleaseFloatArrayElements ( env, ar, val, 0 );
			}
			break;

		case KD_EVENT_ACCELEROMETER :
			{
				KDfloat*    val = ( *env )->GetFloatArrayElements ( env, ar, 0 );

				KDfloat		x, y, z;

				if ( l_mode &  XM_SYS_LANDSCAPE )
				{
					x =  val [ 1 ];
					y = -val [ 0 ];
					z = -val [ 2 ];
				}
				else
				{
					x = -val [ 0 ];
					y = -val [ 1 ];
					z = -val [ 2 ];
				}		

				x /= GRAVITY_EARTH;
				y /= GRAVITY_EARTH;
				z /= GRAVITY_EARTH;

				_xmEventAccelerometer ( x, y, z );

				( *env )->ReleaseFloatArrayElements ( env, ar, val, 0 );
			}
			break;       

		case KD_EVENT_MEMORY_WARNING :
			{
				_xmEventMemoryWarning ( );
			}
			break;
	}

	return 0;
}
