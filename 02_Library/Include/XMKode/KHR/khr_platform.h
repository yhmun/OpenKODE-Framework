/* --------------------------------------------------------------------------
 *
 *      File            khr_platform.h
 *      Description     Khronos platform-specific types and definitions 
 *      Version         Revision 9356, 2009-10-21
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

#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
 *
 * The types in khrplatform.h should only be used to define API-specific types.
 *
 *    khronos_int8_t              signed   8  bit
 *    khronos_uint8_t             unsigned 8  bit
 *    khronos_int16_t             signed   16 bit
 *    khronos_uint16_t            unsigned 16 bit
 *    khronos_int32_t             signed   32 bit
 *    khronos_uint32_t            unsigned 32 bit
 *    khronos_int64_t             signed   64 bit
 *    khronos_uint64_t            unsigned 64 bit
 *    khronos_intptr_t            signed   same number of bits as a pointer
 *    khronos_uintptr_t           unsigned same number of bits as a pointer
 *    khronos_ssize_t             signed   size
 *    khronos_usize_t             unsigned size
 *    khronos_float_t             signed   32 bit floating point
 *    khronos_time_ns_t           unsigned 64 bit time in nanoseconds
 *    khronos_utime_nanoseconds_t unsigned time interval or absolute time in nanoseconds                                       
 *    khronos_stime_nanoseconds_t signed time interval in nanoseconds
 *    khronos_boolean_enum_t      enumerated boolean type. This should
 *      only be used as a base type when a client API's boolean type is
 *      an enum. Client APIs which use an integer or other type for
 *      booleans cannot use this as the base type for their boolean.
 *
 * Tokens defined in khrplatform.h:
 *
 *    KHRONOS_FALSE, KHRONOS_TRUE Enumerated boolean false/true values.
 *
 *    KHRONOS_SUPPORT_INT64 is 1 if 64 bit integers are supported; otherwise 0.
 *    KHRONOS_SUPPORT_FLOAT is 1 if floats are supported; otherwise 0.
 *
 * Calling convention macros defined in this file:
 *    KHRONOS_APICALL
 *    KHRONOS_APIENTRY
 *    KHRONOS_APIATTRIBUTES
 *
 * These may be used in function prototypes as:
 *
 *      KHRONOS_APICALL void KHRONOS_APIENTRY funcname(
 *                                  int arg1,
 *                                  int arg2) KHRONOS_APIATTRIBUTES;
 */

/*-------------------------------------------------------------------------
 * basic type definitions
 *-----------------------------------------------------------------------*/

#if ( defined ( __STDC_VERSION__ ) && __STDC_VERSION__ >= 199901L ) || defined ( __GNUC__ ) || defined ( __SCO__ ) || defined ( __USLC__ )

#include <stdint.h>

typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined ( __VMS ) || defined ( __sgi )

#include <inttypes.h>

typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined ( _WIN32 ) && !defined ( __SCITECH_SNAP__ )

//
// Win32
//

typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined ( __sun__ ) || defined ( __digital__ )

//
// Sun or Digital
//

typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;

#if defined ( __arch64__ ) || defined ( _LP64 )

typedef long int                khronos_int64_t;
typedef unsigned long int       khronos_uint64_t;

#else

typedef long long int           khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;

#endif 

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif 0

//
// Hypothetical platform with no float or int64 support
//

typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;

#define KHRONOS_SUPPORT_INT64   0
#define KHRONOS_SUPPORT_FLOAT   0

#else

//
// Generic fallback
//

#include <stdint.h>

typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#endif

//
// Types that are (so far) the same on all platforms
//
typedef char                   khronos_boolean_t;
#ifdef __cplusplus
typedef bool                   khronos_bool_t;					
#else
typedef char                   khronos_bool_t;
#endif
typedef char                   khronos_char_t;
typedef unsigned char          khronos_uchar_t;
typedef char                   khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;

#if defined ( __APPLE__ )

typedef long                   khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
typedef long                   khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;

#else

typedef signed   long  int     khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
typedef signed   long  int     khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;

#endif

#if KHRONOS_SUPPORT_FLOAT

typedef float                  khronos_float_t;
typedef double                 khronos_double_t;

#endif

typedef long                   khronos_long_t;
typedef unsigned long          khronos_ulong_t;

#if KHRONOS_SUPPORT_INT64

//
// Time types
//
// These types can be used to represent a time interval in nanoseconds or
// an absolute Unadjusted System Time.  Unadjusted System Time is the number
// of nanoseconds since some arbitrary system event (e.g. since the last
// time the system booted).  The Unadjusted System Time is an unsigned
// 64 bit value that wraps back to 0 every 584 years.  Time intervals
// may be either signed or unsigned.
//

typedef khronos_uint64_t       khronos_utime_nanoseconds_t;
typedef khronos_int64_t        khronos_stime_nanoseconds_t;

#endif

#if defined ( _WIN32 )
typedef unsigned long          khronos_pid_t;
#else
typedef signed long            khronos_pid_t;
#endif

//
// Dummy value used to pad enum types to 32 bits.
//

#ifndef KHRONOS_MAX_ENUM
#define KHRONOS_MAX_ENUM       0x7FFFFFFF
#endif

//
// Enumerated boolean type
//
// Values other than zero should be considered to be true.  Therefore
// comparisons should not be made against KHRONOS_TRUE.
//

typedef enum 
{
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE  = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
} khronos_boolean_enum_t;

#include <stdarg.h>

#if defined ( SHP )

#elif defined ( _WIN32_WCE )

#elif defined ( _WIN32 )

    #define KHRONOS_APIENTRY              __stdcall

#elif defined ( ANDROID )

    #define KHRONOS_APICALL               __attribute__ ( ( visibility ( "default" ) ) )

#elif defined ( __APPLE__ )

    #define KDVaListKHR                   __builtin_va_list
    #define KD_VA_START_KHR(ap, parmN)    __builtin_va_start ( ap, parmN )	 
    #define KD_VA_END_KHR(ap)             __builtin_va_end ( ap )
    #define KD_VA_ARG_INT32_KHR(ap)       __builtin_va_arg ( ap, khronos_int32_t )
    #define KD_VA_ARG_INT_KHR(ap)         __builtin_va_arg ( ap, khronos_int32_t )
    #define KD_VA_ARG_INT64_KHR(ap)       __builtin_va_arg ( ap, khronos_int64_t )
    #define KD_VA_ARG_FLOAT32_KHR(ap)     __builtin_va_arg ( ap, khronos_double_t )
    #define KD_VA_ARG_PTR_KHR(ap)         __builtin_va_arg ( ap, void* )

#elif defined ( Linux )

    #define KD_VA_ARG_FLOAT32_KHR(ap)     va_arg(ap, khronos_double_t)
	#define KD_VA_COPY(dest, src)         va_copy(dest, src)

#elif defined ( __SYMBIAN32__ )

    #define KHRONOS_APICALL               IMPORT_C 

#endif

#ifndef KHRONOS_APICALL
#define KHRONOS_APICALL
#endif

#ifndef KHRONOS_APIENTRY
#define KHRONOS_APIENTRY
#endif


#ifndef KDVaListKHR
#define KDVaListKHR					va_list
#endif 

#ifndef KD_VA_START_KHR
#define KD_VA_START_KHR(ap, parmN)	va_start ( ap, parmN )	 
#endif

#ifndef KD_VA_END_KHR
#define KD_VA_END_KHR(ap)			va_end ( ap )
#endif

#ifndef KD_VA_ARG
#define KD_VA_ARG(ap, t)            va_arg ( ap, t ) 
#endif

#ifndef KD_VA_ARG_INT32_KHR
#define KD_VA_ARG_INT32_KHR(ap)		va_arg ( ap, khronos_int32_t )
#endif

#ifndef KD_VA_ARG_INT_KHR
#define KD_VA_ARG_INT_KHR(ap)		va_arg ( ap, khronos_int32_t )
#endif

#ifndef KD_VA_ARG_INT64_KHR
#define KD_VA_ARG_INT64_KHR(ap)		va_arg ( ap, khronos_int64_t )
#endif

#ifndef KD_VA_ARG_PTR_KHR
#define KD_VA_ARG_PTR_KHR(ap)		va_arg ( ap, void* )
#endif

#ifndef KD_VA_ARG_FLOAT32_KHR
#define KD_VA_ARG_FLOAT32_KHR(ap)	va_arg ( ap, khronos_float_t )
#endif

#ifndef KD_VA_COPY
#define KD_VA_COPY(dest, src)		( (dest) = (src) )
#endif

#endif
