/* --------------------------------------------------------------------------
 *
 *      File            bb_kode.h
 *      Description     Bb-specific definitions and functions for XMKode
 *      Author          H.S Lim
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

#ifndef __bb_kode_h__
#define __bb_kode_h__

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/statvfs.h>

#define	R_OK                        4
#define	W_OK                        2
#define	X_OK                        1
/*
#define PTHREAD_CREATE_JOINABLE     0
#define	PTHREAD_CREATE_DETACHED     1
#define	PTHREAD_ONCE_INIT           0
#define	PTHREAD_MUTEX_INITIALIZER   0
#define PTHREAD_COND_INITIALIZER    0

struct dirent
{
	const KDchar*     d_name;
};

class XMPthread;

typedef	struct DIR                  XMDir;
typedef	struct dirent               XMDirent;

typedef XMPthread*                  pthread_t;
typedef struct XMPthreadAttr*       pthread_attr_t;
typedef KDvoid*                     pthread_once_t;
typedef KDuint                      pthread_key_t;
typedef Mutex*                      pthread_mutex_t;
typedef KDvoid*                     pthread_mutexattr_t;
typedef Monitor*                    pthread_cond_t;
typedef KDvoid*                     pthread_condattr_t;
typedef Semaphore*                  sem_t;

#undef  putc
#define	putc                        fputc

#undef  getc
#define getc                        fgetc

#define mkdir                       xmBbMkdir
#define rmdir                       xmBbRmdir
#define rename                      xmBbRename
#define truncate                    xmBbTruncate
#define stat( P, B )                xmBbStat ( P, B )
#define	fileno( X )                 (const KDchar*) ( file->path )
#define	fstat                       stat
#define access                      xmBbAccess
#define opendir                     xmBbOpendir
#define readdir                     xmBbReaddir
#define closedir                    xmBbClosedir

#define vsprintf                    xmBbVsprintf
#define vsscanf                     xmBbVsscanf

#define gettimeofday                xmBbGettimeofday

#define pthread_attr_init           xmBbPthreadAttrInit
#define pthread_attr_destroy        xmBbPthreadAttrDestroy
#define pthread_attr_setdetachstate xmBbPthreadAttrSetdetachstate
#define pthread_attr_setstacksize   xmBbPthreadAttrSetstacksize

#define pthread_create              xmBbPthreadCreate
#define pthread_detach              xmBbPthreadDetach
#define pthread_equal               xmBbPthreadEqual
#define pthread_exit                xmBbPthreadExit
#define pthread_join                xmBbPthreadJoin
#define pthread_self                xmBbPthreadSelf
#define pthread_once                xmBbPthreadOnce

#define pthread_mutex_init          xmBbPthreadMutexInit
#define pthread_mutex_destroy       xmBbPthreadMutexDestroy
#define pthread_mutex_lock          xmBbPthreadMutexLock
#define pthread_mutex_unlock        xmBbPthreadMutexUnlock

#define pthread_cond_init           xmBbPthreadCondInit
#define pthread_cond_destroy        xmBbPthreadCondDestroy
#define pthread_cond_signal         xmBbPthreadCondSignal
#define pthread_cond_broadcast      xmBbPthreadCondBroadcast
#define pthread_cond_wait           xmBbPthreadCondWait

#define sem_init                    xmBbSemInit
#define sem_destroy                 xmBbSemDestroy
#define sem_wait                    xmBbSemWait
#define sem_post                    xmBbSemPost

#define pthread_key_create          xmBbPthreadKeyCreate
#define pthread_key_delete          xmBbPthreadKeyDelete
#define pthread_setspecific         xmBbPthreadSetspecific
#define pthread_getspecific         xmBbPthreadGetspecific

KDint     xmBbMkdir ( const KDchar* path, mode_t mode );
KDint     xmBbRmdir ( const KDchar* path );
KDint     xmBbRename ( const KDchar* _old, const KDchar* _new );
KDint     xmBbTruncate ( const KDchar* path, off_t length );
KDint     xmBbStat ( const KDchar* path, struct stat* buf );
KDint     xmBbAccess ( const KDchar* path, KDint amode );

XMDir*    xmBbOpendir  ( const KDchar* dirname );
XMDirent* xmBbReaddir  ( XMDir* dirp );
KDint     xmBbClosedir ( XMDir* dirp );

KDint     xmBbVsprintf ( KDchar *buf, const KDchar *fmt, va_list args );
KDint     xmBbVsscanf  ( const KDchar* str, const KDchar* format, va_list arg_list );

KDint     xmBbGettimeofday ( struct timeval* tv, struct timezone* tz );

KDvoid    xmBbPthreadInit ( KDvoid );
KDvoid    xmBbPthreadFree ( KDvoid );

KDint     xmBbPthreadAttrInit ( pthread_attr_t* attr );
KDint     xmBbPthreadAttrDestroy ( pthread_attr_t* attr );
KDint     xmBbPthreadAttrSetdetachstate ( pthread_attr_t* attr, KDint detachstate );
KDint     xmBbPthreadAttrSetstacksize ( pthread_attr_t* attr, KDsize stacksize );

KDint     xmBbPthreadCreate ( pthread_t* tid, const pthread_attr_t* attr, KDvoid* ( *start ) (KDvoid *), KDvoid* arg );
KDint     xmBbPthreadDetach ( pthread_t tid );
KDint     xmBbPthreadEqual ( pthread_t t1, pthread_t t2 );
KDvoid    xmBbPthreadExit ( KDvoid* value_ptr );
KDint     xmBbPthreadJoin ( pthread_t thread, KDvoid** value_ptr );
pthread_t xmBbPthreadSelf ( KDvoid );
KDint     xmBbPthreadOnce ( pthread_once_t* once_control, KDvoid ( *init_routine ) ( KDvoid ) );

KDint     xmBbPthreadMutexInit ( pthread_mutex_t* mutex, const pthread_mutexattr_t* attr );
KDint     xmBbPthreadMutexDestroy ( pthread_mutex_t* mutex );
KDint     xmBbPthreadMutexLock ( pthread_mutex_t* mutex );
KDint     xmBbPthreadMutexUnlock ( pthread_mutex_t* mutex );

KDint     xmBbPthreadCondInit ( pthread_cond_t* cond, const pthread_condattr_t* attr );
KDint     xmBbPthreadCondDestroy ( pthread_cond_t* cond );
KDint     xmBbPthreadCondSignal ( pthread_cond_t* cond );
KDint     xmBbPthreadCondBroadcast ( pthread_cond_t* cond );
KDint     xmBbPthreadCondWait ( pthread_cond_t* cond, pthread_mutex_t* mutex );

KDint     xmBbSemInit ( sem_t* sem, KDint pshared, KDuint value );
KDint     xmBbSemDestroy ( sem_t* sem );
KDint     xmBbSemWait ( sem_t* sem );
KDint     xmBbSemPost ( sem_t* sem );

KDint     xmBbPthreadKeyCreate ( pthread_key_t* key, KDvoid ( *destructor ) (KDvoid *) );
KDint     xmBbPthreadKeyDelete ( pthread_key_t key );
KDint     xmBbPthreadSetspecific ( pthread_key_t key, const KDvoid* value );
KDvoid*   xmBbPthreadGetspecific ( pthread_key_t key );
*/
#endif	// __bb_kode_h__

