/* --------------------------------------------------------------------------
 *
 *      File            kd_platform.h 
 *      Description     Platform-specific types and definitions for XMKode
 *      Version         1.0.3
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

#ifndef __kd_platform_h_
#define __kd_platform_h_

#include <KHR/khr_platform.h>

#define KD_API                          KHRONOS_APICALL	
#define KD_APIENTRY

/*******************************************************
 * Types
 *******************************************************/

#define KDvoid                          void
typedef khronos_char_t                  KDchar;
typedef khronos_int8_t                  KDbyte;
typedef khronos_int16_t                 KDshort;
typedef khronos_long_t					KDlong;
typedef khronos_int32_t                 KDint;
typedef khronos_float_t                 KDfloat;
typedef khronos_double_t                KDdouble;
typedef khronos_int32_t                 KDfixed;
                  
typedef khronos_uchar_t                 KDuchar;
typedef khronos_uint8_t                 KDubyte;
typedef khronos_uint16_t                KDushort;
typedef khronos_ulong_t					KDulong;
typedef khronos_uint32_t                KDuint;

typedef khronos_int8_t                  KDint8;
typedef khronos_int16_t                 KDint16;
typedef khronos_int32_t                 KDint32;
typedef khronos_int64_t                 KDint64;
typedef khronos_float_t                 KDfloat32;
typedef khronos_double_t                KDfloat64;

typedef khronos_uint8_t                 KDuint8;
typedef khronos_uint16_t                KDuint16;
typedef khronos_uint32_t                KDuint32;
typedef khronos_uint64_t                KDuint64;

typedef khronos_intptr_t                KDintptr;	
typedef khronos_uintptr_t               KDuintptr;
typedef khronos_usize_t                 KDsize;
typedef khronos_ssize_t                 KDssize;

typedef khronos_boolean_t               KDboolean;
typedef khronos_bool_t                  KDbool;

typedef khronos_stime_nanoseconds_t     KDtime;
typedef khronos_utime_nanoseconds_t     KDust;
typedef khronos_intptr_t                KDoff;
typedef khronos_uint32_t                KDmode;
typedef khronos_uint64_t                KDmode64;
typedef khronos_uint32_t                KDenum;

typedef khronos_pid_t					KDpid;
typedef KDuintptr                       KDSOCKET;
typedef KDvoid                          KDsiginfo;
typedef khronos_uint32_t                KDsigset;

typedef struct
{
    KDfloat     x;
    KDfloat     y;
} KDPoint;

typedef struct
{
    KDfloat     x;
    KDfloat     y;
    KDfloat     w;
    KDfloat     h;
} KDRect;

#define KD_FD_SETSIZE                   64

#define KDINT8_MIN                      ( -0x7f - 1 )
#define KDINT8_MAX                      0x7f
#define KDUINT8_MAX                     0xffU

#define KDINT16_MIN                     ( -0x7fff - 1 )
#define KDINT16_MAX                     0x7fff
#define KDUINT16_MAX                    0xffffU

#define KDINT32_MIN                     ( -0x7fffffff - 1 )
#define KDINT32_MAX                     0x7fffffff
#define KDUINT32_MAX                    0xffffffffU

#define KDINT64_MIN                     ( -0x7fffffffffffffffLL - 1 )
#define KDINT64_MAX                     0x7fffffffffffffffLL
#define KDUINT64_MAX                    0xffffffffffffffffULL

#ifdef __cplusplus
#define KD_TRUE                         true
#define KD_FALSE                        false
#else
#define KD_TRUE                         1
#define KD_FALSE                        0
#endif

#define KDCHAR_MIN                      ( -0x7f - 1 )
#define KDCHAR_MAX                      0x7f
#define KDUCHAR_MAX                     0xffU

#define KDINT_MIN                       ( -0x7fffffff - 1 )
#define KDINT_MAX                       0x7fffffff
#define KDUINT_MAX                      0xffffffffU

#define KDSSIZE_MIN                     ( -0x7fffffff - 1 )
#define KDSSIZE_MAX                     0x7fffffff
#define KDSIZE_MAX                      0xffffffffU
#define KDUINTPTR_MAX                   0xffffffffU

#define KD_NORETURN             
#define KD_INFINITY                     kdInfinity ( )
#define KD_RAND_MAX                     kdRandMax ( )

#define KD_TOUCH_MAX                    10

#define KD_NULL                         0

#ifndef NULL
#define NULL							KD_NULL
#endif

#ifdef KD_NDEBUG
#define kdAssert(c)
#define kdAssertion(c)
#else
#define kdAssert(c)                     ( (KDvoid) ( ( c ) ? 0 : ( kdHandleAssertion ( #c, __FILE__, __LINE__ ), 0 ) ) )
#define kdAssertion(c)                  kdHandleAssertion ( c, __FILE__, __LINE__ )
#endif

#undef s_addr

#endif

