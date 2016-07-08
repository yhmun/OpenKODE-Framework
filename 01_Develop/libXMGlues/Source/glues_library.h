/*
 *
 *      File            glues_library.h
 *      Description     Library-specific definitions and functions for XMGLU ( GLU ES )
 *      Version         1.4, 2009-05-17 
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2011 XMSoft. All rights reserved.
 * 
 *      Contact Email: chris@xmsoft.co.kr 
 *                     xmsoft77@gmail.com	
 * 
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
 */

#ifndef __glues_library_h__
#define __glues_library_h__

#include <kode.h>
#include <GLUES/glu.h>

#if defined ( SHP )
typedef KDint                           jmp_buf[16];
#define setjmp( B )                     ( 0 )
#define longjmp( B, V );
#else
#include <setjmp.h>
#endif

#ifndef LONG_MAX
#define LONG_MAX                        KDINT32_MAX
#endif

#ifndef NULL
#define NULL                            KD_NULL
#endif

#define size_t                          KDsize

#define assert                          kdAssert

#define malloc                          kdMalloc
#define realloc                         kdRealloc
#define free                            kdFree

#define memcpy                          kdMemcpy
#define strlen                          kdStrlen
#define strcmp                          kdStrcmp
#define strcpy                          kdStrcpy
#define strtok                          kdStrtok

#define sin                             kdSinKHR
#define cos                             kdCosKHR
#define sqrt                            kdSqrtKHR
#define floor                           kdFloorKHR

#endif
