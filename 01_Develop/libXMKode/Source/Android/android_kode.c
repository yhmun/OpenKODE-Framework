/* --------------------------------------------------------------------------
 *
 *      File            android_kode.c
 *      Description     Android-specific definitions and functions for XMKode
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

static jint        l_argc = 0;
static jchar**     l_argv = 0;

PFNGLGENVERTEXARRAYSOESPROC     glGenVertexArraysOESEXT     = 0;
PFNGLBINDVERTEXARRAYOESPROC     glBindVertexArrayOESEXT     = 0;
PFNGLDELETEVERTEXARRAYSOESPROC  glDeleteVertexArraysOESEXT  = 0;

// xmPlatformInitLibrary : Initialize XMKode library in platform specific.
KDvoid xmPlatformInitLibrary ( KDvoid )
{
	xmAndroidPthreadKeyInit ( );
	xmAndroidResInit ( );
    
    glGenVertexArraysOESEXT    = (PFNGLGENVERTEXARRAYSOESPROC)    eglGetProcAddress ( "glGenVertexArraysOES" );
    glBindVertexArrayOESEXT    = (PFNGLBINDVERTEXARRAYOESPROC)    eglGetProcAddress ( "glBindVertexArrayOES" );
    glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC) eglGetProcAddress ( "glDeleteVertexArraysOES" );
}

// xmPlatformFreeLibrary : Release XMKode library's resources in platform specific.
KDvoid xmPlatformFreeLibrary ( KDvoid )
{
	xmAndroidResFree ( );
}

// xmJNIInitLibrary : Initialize XMKode library in "Android" platform. 
jint JNICALL xmJNIInitLibrary ( JNIEnv* env, jobject obj, jobjectArray ar )
{
	jclass        cls;
	jmethodID     mid;

	jint    i;

	if ( l_argc )
	{
		return;
	}

	l_argc = ( *env )->GetArrayLength ( env, ar );																		
	l_argv = (jchar **) malloc ( sizeof ( jchar* ) * l_argc );															

	for ( i = 0; i < l_argc; i++ )																									
	{																														
		l_argv[ i ] = (jchar *) (*env)->GetStringUTFChars ( env, (jstring) (*env)->GetObjectArrayElement ( env, ar, i ), 0 );	
	}		

	xmInitLibrary ( l_argc, (KDchar **) l_argv );

	return 0;
}