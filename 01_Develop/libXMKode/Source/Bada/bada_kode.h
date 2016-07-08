/* --------------------------------------------------------------------------
 *
 *      File            bada_kode.h
 *      Description     Bada-specific definitions and functions for XMKode
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

#ifndef __bada_kode_h__
#define __bada_kode_h__

#include <FBase.h>
#include <FGraphics.h>
#include <FApp.h>
#include <FSystem.h>
#include <FIo.h>
#include <FLocales.h>
#include <FUi.h>
#include <FMedia.h>

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
#include <sys/types.h>
#include <sys/stat.h>

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Base::Runtime;
using namespace Osp::Base::Utility;
using namespace Osp::Io;
using namespace Osp::System;
using namespace Osp::Locales;
using namespace Osp::Graphics;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Media;

#define	R_OK                        4
#define	W_OK                        2
#define	X_OK                        1

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

#define mkdir                       xmBadaMkdir
#define rmdir                       xmBadaRmdir
#define rename                      xmBadaRename
#define truncate                    xmBadaTruncate
#define stat( P, B )                xmBadaStat ( P, B )
#define	fileno( X )                 (const KDchar*) ( file->path )
#define	fstat                       stat
#define access                      xmBadaAccess
#define opendir                     xmBadaOpendir
#define readdir                     xmBadaReaddir
#define closedir                    xmBadaClosedir

#define vsprintf                    xmBadaVsprintf
#define vsscanf                     xmBadaVsscanf

#define gettimeofday                xmBadaGettimeofday

#define pthread_attr_init           xmBadaPthreadAttrInit
#define pthread_attr_destroy        xmBadaPthreadAttrDestroy
#define pthread_attr_setdetachstate xmBadaPthreadAttrSetdetachstate
#define pthread_attr_setstacksize   xmBadaPthreadAttrSetstacksize

#define pthread_create              xmBadaPthreadCreate
#define pthread_detach              xmBadaPthreadDetach
#define pthread_equal               xmBadaPthreadEqual
#define pthread_exit                xmBadaPthreadExit
#define pthread_join                xmBadaPthreadJoin
#define pthread_self                xmBadaPthreadSelf
#define pthread_once                xmBadaPthreadOnce

#define pthread_mutex_init          xmBadaPthreadMutexInit
#define pthread_mutex_destroy       xmBadaPthreadMutexDestroy
#define pthread_mutex_lock          xmBadaPthreadMutexLock
#define pthread_mutex_unlock        xmBadaPthreadMutexUnlock

#define pthread_cond_init           xmBadaPthreadCondInit
#define pthread_cond_destroy        xmBadaPthreadCondDestroy
#define pthread_cond_signal         xmBadaPthreadCondSignal
#define pthread_cond_broadcast      xmBadaPthreadCondBroadcast
#define pthread_cond_wait           xmBadaPthreadCondWait

#define sem_init                    xmBadaSemInit
#define sem_destroy                 xmBadaSemDestroy
#define sem_wait                    xmBadaSemWait
#define sem_post                    xmBadaSemPost

#define pthread_key_create          xmBadaPthreadKeyCreate
#define pthread_key_delete          xmBadaPthreadKeyDelete
#define pthread_setspecific         xmBadaPthreadSetspecific
#define pthread_getspecific         xmBadaPthreadGetspecific

KDint     xmBadaMkdir ( const KDchar* path, mode_t mode );
KDint     xmBadaRmdir ( const KDchar* path );
KDint     xmBadaRename ( const KDchar* _old, const KDchar* _new );
KDint     xmBadaTruncate ( const KDchar* path, off_t length );
KDint     xmBadaStat ( const KDchar* path, struct stat* buf );
KDint     xmBadaAccess ( const KDchar* path, KDint amode );

XMDir*    xmBadaOpendir  ( const KDchar* dirname );
XMDirent* xmBadaReaddir  ( XMDir* dirp );
KDint     xmBadaClosedir ( XMDir* dirp );

KDint     xmBadaVsprintf ( KDchar *buf, const KDchar *fmt, va_list args );
KDint     xmBadaVsscanf  ( const KDchar* str, const KDchar* format, va_list arg_list );

KDint     xmBadaGettimeofday ( struct timeval* tv, struct timezone* tz );

KDvoid    xmBadaPthreadInit ( KDvoid );
KDvoid    xmBadaPthreadFree ( KDvoid );

KDint     xmBadaPthreadAttrInit ( pthread_attr_t* attr );
KDint     xmBadaPthreadAttrDestroy ( pthread_attr_t* attr );
KDint     xmBadaPthreadAttrSetdetachstate ( pthread_attr_t* attr, KDint detachstate );
KDint     xmBadaPthreadAttrSetstacksize ( pthread_attr_t* attr, KDsize stacksize );

KDint     xmBadaPthreadCreate ( pthread_t* tid, const pthread_attr_t* attr, KDvoid* ( *start ) (KDvoid *), KDvoid* arg );
KDint     xmBadaPthreadDetach ( pthread_t tid );
KDint     xmBadaPthreadEqual ( pthread_t t1, pthread_t t2 );
KDvoid    xmBadaPthreadExit ( KDvoid* value_ptr );
KDint     xmBadaPthreadJoin ( pthread_t thread, KDvoid** value_ptr );
pthread_t xmBadaPthreadSelf ( KDvoid );
KDint     xmBadaPthreadOnce ( pthread_once_t* once_control, KDvoid ( *init_routine ) ( KDvoid ) );

KDint     xmBadaPthreadMutexInit ( pthread_mutex_t* mutex, const pthread_mutexattr_t* attr );
KDint     xmBadaPthreadMutexDestroy ( pthread_mutex_t* mutex );
KDint     xmBadaPthreadMutexLock ( pthread_mutex_t* mutex );
KDint     xmBadaPthreadMutexUnlock ( pthread_mutex_t* mutex );

KDint     xmBadaPthreadCondInit ( pthread_cond_t* cond, const pthread_condattr_t* attr );
KDint     xmBadaPthreadCondDestroy ( pthread_cond_t* cond );
KDint     xmBadaPthreadCondSignal ( pthread_cond_t* cond );
KDint     xmBadaPthreadCondBroadcast ( pthread_cond_t* cond );
KDint     xmBadaPthreadCondWait ( pthread_cond_t* cond, pthread_mutex_t* mutex );

KDint     xmBadaSemInit ( sem_t* sem, KDint pshared, KDuint value );
KDint     xmBadaSemDestroy ( sem_t* sem );
KDint     xmBadaSemWait ( sem_t* sem );
KDint     xmBadaSemPost ( sem_t* sem );

KDint     xmBadaPthreadKeyCreate ( pthread_key_t* key, KDvoid ( *destructor ) (KDvoid *) );
KDint     xmBadaPthreadKeyDelete ( pthread_key_t key );
KDint     xmBadaPthreadSetspecific ( pthread_key_t key, const KDvoid* value );
KDvoid*   xmBadaPthreadGetspecific ( pthread_key_t key );

#endif
