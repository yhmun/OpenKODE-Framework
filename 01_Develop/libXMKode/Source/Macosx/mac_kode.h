/* --------------------------------------------------------------------------
 *
 *      File            ios_kode.h
 *      Description     iOS-specific definitions and functions for XMKode
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

#ifndef __ios_kode_h__
#define __ios_kode_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sched.h>
#include <pthread.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach/mach_error.h>
#include <mach/semaphore.h>
#include <mach/task.h>

#define sem_init                xmMacSemInit
#define sem_destroy             xmMacSemDestroy
#define sem_wait                xmMacSemWait
#define sem_post                xmMacSemPost

typedef semaphore_t             sem_t;

KDint  xmMacSemInit    ( sem_t* sem, KDint pshared, KDuint value );
KDint  xmMacSemDestroy ( sem_t* sem );
KDint  xmMacSemWait    ( sem_t* sem );
KDint  xmMacSemPost    ( sem_t* sem );

#define SOCKET_ERROR            -1
#define s_addr_                 s_addr
#define closesocket(fd)         shutdown ( fd, SHUT_RDWR )

#endif
