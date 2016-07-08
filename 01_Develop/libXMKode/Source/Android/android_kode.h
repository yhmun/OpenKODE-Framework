/* --------------------------------------------------------------------------
 *
 *      File            android_kode.h
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

#ifndef __android_kode_h__
#define __android_kode_h__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <android/log.h>
#include <jni.h>	
#include <zip.h>

#define pthread_key_create         xmAndroidPthreadKeyCreate
#define pthread_key_delete         xmAndroidPthreadKeyDelete
#define pthread_setspecific        xmAndroidPthreadSetSpecific
#define pthread_getspecific        xmAndroidPthreadGetSpecific

KDvoid  xmAndroidResInit ( KDvoid );
KDvoid  xmAndroidResFree ( KDvoid );

KDFile* xmAndroidFopen ( const KDchar* filename, const KDchar* mode );
//FILE*   xmAndroidFopen ( const KDchar* filename, const KDchar* mode );

KDint   xmAndroidStat ( const KDchar* pathname, struct KDStat* buf );

KDDir*    xmAndroidOpenDir ( const KDchar* pathname );
KDDirent* xmAndroidReadDir ( KDDir* dir );

KDint   xmAndroidOpen ( const KDchar* pathname, KDint flags, KDint mode );

KDvoid  xmAndroidPthreadKeyInit ( KDvoid );
KDint   xmAndroidPthreadKeyCreate ( pthread_key_t* key, KDvoid ( *destructor ) (KDvoid *) );
KDint   xmAndroidPthreadKeyDelete ( pthread_key_t key );
KDint   xmAndroidPthreadSetSpecific ( pthread_key_t key, const KDvoid* value );
KDvoid* xmAndroidPthreadGetSpecific ( pthread_key_t key );

#define SOCKET_ERROR            -1
#define s_addr_                 s_addr
#define closesocket(fd)         shutdown ( fd, SHUT_RDWR )

#endif
