/* --------------------------------------------------------------------------
 *
 *      File            kd.h
 *      Description     The OpenKODE Core with XMSoft's added APIs. 
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

#ifndef __kd_h__
#define __kd_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "kd_platform.h"

/*******************************************************
 * Introduction
 *******************************************************/

/*******************************************************
 * XMKode conformance
 *******************************************************/

/*******************************************************
 * Overview
 *******************************************************/

/*******************************************************
 * Programming environment
 *******************************************************/

#define KD_VERSION_1_0                    1

/*******************************************************
 * Errors
 *******************************************************/

#define KD_EACCES                         1
#define KD_EADDRINUSE                     2
#define KD_EADDRNOTAVAIL                  3
#define KD_EAFNOSUPPORT                   4
#define KD_EAGAIN                         5
#define KD_EALREADY                       6
#define KD_EBADF                          7
#define KD_EBUSY                          8 
#define KD_ECONNREFUSED                   9
#define KD_ECONNRESET                     10
#define KD_EDEADLK                        11
#define KD_EDESTADDRREQ                   12
#define KD_EEXIST                         13
#define KD_EFBIG                          14
#define KD_EHOSTUNREACH                   15
#define KD_EHOST_NOT_FOUND                16
#define KD_EINVAL                         17
#define KD_EIO                            18
#define KD_EILSEQ                         19
#define KD_EISCONN                        20
#define KD_EISDIR                         21
#define KD_EMFILE                         22
#define KD_ENAMETOOLONG                   23
#define KD_ENOENT                         24
#define KD_ENOMEM                         25
#define KD_ENOSPC                         26
#define KD_ENOSYS                         27
#define KD_ENOTCONN                       28
#define KD_ENO_DATA                       29
#define KD_ENO_RECOVERY                   30
#define KD_EOPNOTSUPP                     31
#define KD_EOVERFLOW                      32
#define KD_EPERM                          33
#define KD_ERANGE                         35
#define KD_ETIMEDOUT                      36
#define KD_ETRY_AGAIN                     37

// kdGetError : Get last error indication. 
KD_API KDint KD_APIENTRY kdGetError ( KDvoid );

// kdSetError : Set last error indication. 
KD_API KDvoid KD_APIENTRY kdSetError ( KDint error );

// kdStrerror : Get pointer to error message string.
// XMSoft's revision : Added API.
KD_API const KDchar* KD_APIENTRY kdStrerror ( KDint error );

/*******************************************************
 * Versioning and attribute queries
 *******************************************************/

#define KD_ATTRIB_VENDOR                  39
#define KD_ATTRIB_VERSION                 40
#define KD_ATTRIB_PLATFORM                41

// kdQueryAttribi : Obtain the value of a numeric XMKode attribute. 
KD_API KDint KD_APIENTRY kdQueryAttribi ( KDint attribute, KDint* value );

// kdQueryAttribcv : Obtain the value of a string XMKode attribute. 
KD_API const KDchar* KD_APIENTRY kdQueryAttribcv ( KDint attribute );

// kdQueryIndexedAttribcv : Obtain the value of an indexed string XMKode attribute.
KD_API const KDchar* KD_APIENTRY kdQueryIndexedAttribcv ( KDint attribute, KDint index );

/*******************************************************
 * Threads and synchronization
 *******************************************************/

typedef struct KDThreadAttr                 KDThreadAttr;
typedef struct KDThread                     KDThread;
typedef struct KDThreadMutex                KDThreadMutex;
typedef struct KDThreadCond                 KDThreadCond;
typedef struct KDThreadSem                  KDThreadSem;

typedef struct KDThreadOnce 
{
    KDvoid*    impl;
} KDThreadOnce;

#define KD_THREAD_ONCE_INIT                 { 0 }

#define KD_THREAD_CREATE_JOINABLE           0
#define KD_THREAD_CREATE_DETACHED           1

// kdThreadAttrCreate : Create a thread attribute object. 
KD_API KDThreadAttr* KD_APIENTRY kdThreadAttrCreate ( KDvoid );

// kdThreadAttrFree : Free a thread attribute object. 
KD_API KDint KD_APIENTRY kdThreadAttrFree ( KDThreadAttr* attr );

// kdThreadAttrSetDetachState : Set detachstate attribute.
KD_API KDint KD_APIENTRY kdThreadAttrSetDetachState ( KDThreadAttr* attr, KDint detachstate );

// kdThreadAttrSetStackSize : Set stacksize attribute.
KD_API KDint KD_APIENTRY kdThreadAttrSetStackSize ( KDThreadAttr* attr, KDsize stacksize );

// kdThreadCreate : Create a new thread. 
KD_API KDThread* KD_APIENTRY kdThreadCreate ( const KDThreadAttr* attr, KDvoid* (* start_routine)(KDvoid *), KDvoid* arg );

// kdThreadExit : Terminate this thread.
KD_API KD_NORETURN KDvoid KD_APIENTRY kdThreadExit ( KDvoid* retval );

// kdThreadJoin : Wait for termination of another thread. 
KD_API KDint KD_APIENTRY kdThreadJoin ( KDThread* thread, KDvoid** retval );

// kdThreadDetach : Allow resources to be freed as soon as a thread terminates.
KD_API KDint KD_APIENTRY kdThreadDetach ( KDThread* thread );

// kdThreadSelf : Return calling thread's ID. 
KD_API KDThread* KD_APIENTRY kdThreadSelf ( KDvoid );

// kdThreadOnce : Wrap initialization code so it is executed only once.
KD_API KDint KD_APIENTRY kdThreadOnce ( KDThreadOnce *once_control, KDvoid (* init_routine) ( KDvoid ) );

// kdThreadMutexCreate : Create a mutex.
KD_API KDThreadMutex* KD_APIENTRY kdThreadMutexCreate ( const KDvoid* mutexattr );

// kdThreadMutexFree : Free a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexFree ( KDThreadMutex* mutex );

// kdThreadMutexLock : Lock a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexLock ( KDThreadMutex* mutex );

// kdThreadMutexUnlock : Unlock a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexUnlock ( KDThreadMutex* mutex );

// kdThreadCondCreate : Create a condition variable.
KD_API KDThreadCond* KD_APIENTRY kdThreadCondCreate ( const KDvoid* attr );

// kdThreadCondFree: Free a condition variable. 
KD_API KDint KD_APIENTRY kdThreadCondFree ( KDThreadCond* cond );

// kdThreadCondSignal, kdThreadCondBroadcast : Signal a condition variable. 
KD_API KDint KD_APIENTRY kdThreadCondSignal ( KDThreadCond* cond );
KD_API KDint KD_APIENTRY kdThreadCondBroadcast ( KDThreadCond* cond );

// kdThreadCondWait : Wait for a condition variable to be signalled. 
KD_API KDint KD_APIENTRY kdThreadCondWait ( KDThreadCond* cond, KDThreadMutex* mutex );

// kdThreadSemCreate: Create a semaphore. 
KD_API KDThreadSem *KD_APIENTRY kdThreadSemCreate ( KDuint value );

// kdThreadSemFree : Free a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemFree ( KDThreadSem* sem );

// kdThreadSemWait : Lock a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemWait ( KDThreadSem* sem );

// kdThreadSemPost : Unlock a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemPost ( KDThreadSem* sem );

/*******************************************************
 * Events
 *******************************************************/

// KDEvent : Struct type containing an event.
#define KD_EVENT_USER                       0x40000000

typedef struct KDEvent                      KDEvent;
typedef KDvoid ( KD_APIENTRY KDCallbackFunc ) ( const KDEvent* event );

typedef struct KDEventUser 
{
    union
	{
        KDint64     i64;
        KDvoid*     p;
        struct
		{
            KDint32     a;
            KDint32     b;
        } i32pair;
    } value1;
    union
	{
        KDint64     i64;
        struct
		{
            union
			{
                KDint32     i32;
                KDvoid*     p;
            } value2;
            union 
			{
                KDint32     i32;
                void*       p;
            } value3;
        } i32orp;
    } value23;
} KDEventUser;

// kdWaitEvent : Get next event from thread's event queue.
KD_API const KDEvent* KD_APIENTRY kdWaitEvent ( KDust timeout );

// kdSetEventUserptr : Set the userptr for global events.
KD_API KDvoid KD_APIENTRY kdSetEventUserptr ( KDvoid* userptr );

// kdDefaultEvent : Perform default processing on an unrecognized event. 
KD_API KDvoid KD_APIENTRY kdDefaultEvent ( const KDEvent* event );

// kdPumpEvents : Pump the thread's event queue, performing callbacks.
KD_API KDint KD_APIENTRY kdPumpEvents ( KDvoid );

// kdInstallCallback : Install or remove a callback function for event processing. 
KD_API KDint KD_APIENTRY kdInstallCallback ( KDCallbackFunc* func, KDint eventtype, KDvoid* eventuserptr );

// kdCreateEvent : Create an event for posting.
KD_API KDEvent* KD_APIENTRY kdCreateEvent ( KDvoid );

// kdPostEvent, kdPostThreadEvent : Post an event into a queue.
KD_API KDint KD_APIENTRY kdPostEvent ( KDEvent* event );
KD_API KDint KD_APIENTRY kdPostThreadEvent ( KDEvent* event, KDThread* thread );

// kdFreeEvent : Abandon an event instead of posting it. 
KD_API KDvoid KD_APIENTRY kdFreeEvent ( KDEvent* event );

/*******************************************************
 * System 
 *******************************************************/

// kdGetpid : get process identification
// XMSoft's revision : Added API.
KD_API KDpid KD_APIENTRY kdGetpid ( KDvoid );

// kdFork : The kdFork() system call will spawn a new child process which is an identical process to the parent except that has a new system process ID
// XMSoft's revision : Added API.
KD_API KDpid KD_APIENTRY kdFork ( KDvoid );

// kdSigemptyset : initializes the signal set specified by set to an empty set. 
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSigemptyset ( KDsigset* set );

// kdSigaction : system call is used to change the action taken by a process on receipt of a specific signal.
// XMSoft's revision : Added API.
struct KDSigaction
{
    KDvoid		(*sa_handler_)   ( KDint );							// 시그널을 처리하기 위한 핸들러. SIG_DFL, SIG_IGN 또는 핸들러 함수
    KDvoid		(*sa_sigaction_) ( KDint, KDsiginfo*, KDvoid* );		// 밑의 sa_flags가 SA_SIGINFO일때
																	// sa_handler 대신에 동작하는 핸들러
    KDsigset	sa_mask;											// 시그널을 처리하는 동안 블록화할 시그널 집합의 마스크
    KDint		sa_flags;											// 아래 설명을 참고하세요.
    KDvoid		(*sa_restorer) ( KDvoid );							// 사용해서는 안됩니다.
};

KD_API KDint KD_APIENTRY kdSigaction ( KDint signum, const struct KDSigaction* act, struct KDSigaction* oldact );

// kdSignal : ANSI C signal handling
// XMSoft's revision : Added API.
#define KD_SIGABRT           22										// abnormal termination.
#define KD_SIGFPE            8										// floating point exception.
#define KD_SIGILL            4										// invalid instruction.
#define KD_SIGINT            2										// interactive attention request sent to the program.
#define KD_SIGSEGV           11										// invalid memory access.
#define KD_SIGTERM           15										// termination request sent to the program.

#define KD_SIGCHLD           31
#define KD_SIGPIPE           32
#define KD_SIGKILL           33

typedef KDvoid ( *KDSighandler ) ( KDint );

#define KD_SIG_DFL          (KDSighandler) 0						// default signal action 
#define KD_SIG_IGN          (KDSighandler) 1						// ignore signal 
#define KD_SIG_GET          (KDSighandler) 2						// return current value 
#define KD_SIG_SGE          (KDSighandler) 3						// signal gets error 
#define KD_SIG_ACK          (KDSighandler) 4						// acknowledge 

KD_API KDSighandler KD_APIENTRY kdSignal ( KDint signum, KDSighandler handler );

// kdSystem : Execute system command
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSystem ( const KDchar* string );

/*******************************************************
 * System events
 *******************************************************/

// KD_EVENT_QUIT : Event to request to quit application. 
#define KD_EVENT_QUIT                       43

// KD_EVENT_PAUSE : Application pause event.
#define KD_EVENT_PAUSE                      45

// KD_EVENT_RESUME : Application resume event.
#define KD_EVENT_RESUME                     46

// KD_EVENT_ORIENTATION : Orientation change event.
#define KD_EVENT_ORIENTATION                48

// KD_IOGROUP_EVENT : I/O group for XMKode system events implemented as state values.
#define KD_IOGROUP_EVENT			        0x100

#define KD_STATE_EVENT_USING_BATTERY        ( KD_IOGROUP_EVENT + 0 )
#define KD_STATE_EVENT_LOW_BATTERY          ( KD_IOGROUP_EVENT + 1 )

// KD_IOGROUP_ORIENTATION: I/O group for XMKode orientation state. 
#define KD_IOGROUP_ORIENTATION              0x200

#define KD_STATE_ORIENTATION_OVERALL        ( KD_IOGROUP_ORIENTATION + 0 )
#define KD_STATE_ORIENTATION_SCREEN         ( KD_IOGROUP_ORIENTATION + 1 )
#define KD_STATE_ORIENTATION_RENDERING      ( KD_IOGROUP_ORIENTATION + 2 )
#define KD_STATE_ORIENTATION_LOCKSURFACE    ( KD_IOGROUP_ORIENTATION + 3 )

/*******************************************************
 * Application startup and exit.
 *******************************************************/

// kdMain : The application-defined main function.
// XMSoft's revision : Deleted API.
KDint KD_APIENTRY kdMain ( KDint argc, const KDchar* const* argv );

// kdExit : Exit the application. 
KD_API KD_NORETURN KDvoid KD_APIENTRY kdExit ( KDint status );

/*******************************************************
 * Utility library functions
 *******************************************************/

#define KD_LTOSTR_MAXLEN                    ( ( sizeof ( KDint ) * 8 * 3 + 6 ) / 10 + 2 )
#define KD_ULTOSTR_MAXLEN                   ( ( sizeof ( KDint ) * 8 + 2 ) / 3 + 1 )
#define KD_FTOSTR_MAXLEN                    16

// XMSoft's revision : Added Macros.
#define KD_MAX( a, b )                      ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define KD_MIN( a, b )                      ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define KD_SWAP( a, b, t )                  { t tmp; tmp = a; a = b; b = tmp; }
#define KD_CLAMP( min, max, value )			( ( value < min ) ? min : ( value > max ) ? max : value )

#define KD_ABS( x ) 						( ( ( x ) > 0 ) ? ( x ) : -( x ) )

#define KD_LOWORD( l )						( (KDushort) ( (   (KDuintptr) ( l ) )         & 0xffff ) )
#define KD_HIWORD( l )						( (KDushort) ( ( ( (KDuintptr) ( l ) ) >> 16 ) & 0xffff ) )
#define KD_LOBYTE( w )						( (KDubyte)  ( (   (KDuintptr) ( w ) )         & 0xff   ) )
#define KD_HIBYTE( w )						( (KDubyte)  ( ( ( (KDuintptr) ( w ) ) >>  8 ) & 0xff   ) )

// kdAbs : Compute the absolute value of an integer.
KD_API KDint KD_APIENTRY kdAbs ( KDint i );

// kdStrtof : Convert a string to a floating point number.
KD_API KDfloat32 KD_APIENTRY kdStrtof ( const KDchar* s, KDchar** endptr );

// kdStrtol, kdStrtoul : Convert a string to an integer.
KD_API KDint KD_APIENTRY kdStrtol ( const KDchar* s, KDchar** endptr, KDint base );
KD_API KDuint KD_APIENTRY kdStrtoul ( const KDchar* s, KDchar** endptr, KDint base );

// kdLtostr, kdUltostr : Convert an integer to a string. 
KD_API KDssize KD_APIENTRY kdLtostr ( KDchar* buffer, KDsize buflen, KDint number );
KD_API KDssize KD_APIENTRY kdUltostr ( KDchar* buffer, KDsize buflen, KDuint number, KDint base );

// kdFtostr : Convert a float to a string.
KD_API KDssize KD_APIENTRY kdFtostr ( KDchar* buffer, KDsize buflen, KDfloat32 number );

// kdCryptoRandom : Return random data.
// XMSoft's revision : Only crypto supported with Win32. other platforms are normal random.
KD_API KDint KD_APIENTRY kdCryptoRandom ( KDuint8* buf, KDsize buflen );

// kdRand : Returns a pseudo-random integral number in the range 0 to KD_RAND_MAX
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdRand ( KDvoid );

// kdSrand : Initialize random number generator
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSrand ( KDuint seed );

// kdRandMax : Get Random max number
// XMSoft's revision : Added API.
KD_API KDuint KD_APIENTRY kdRandMax ( KDvoid );

// kdQsort : Sort elements of array
// XMSoft's revision : Added API.
KD_API KDvoid kdQsort ( KDvoid* base, KDsize num, KDsize size, KDint ( *comparator ) ( const KDvoid *, const KDvoid* ) );

// kdBsearch : Binary search in array
// XMSoft's revision : Added API.
KD_API KDvoid* KD_APIENTRY kdBsearch ( const KDvoid* key, const KDvoid* base, KDsize num, KDsize size, KDint ( *comparator ) ( const KDvoid*, const KDvoid* ) );

typedef struct 
{
	KDint		quot;	// Represents the quotient of the integral division operation performed by div, which is the integer of lesser magnitude that is nearest to the algebraic quotient.
	KDint		rem;	// Represents the remainder of the integral division operation performed by div, which is the integer resulting from subtracting quot to the numerator of the operation.
} KDDiv;

// kdDiv : Integral division
// XMSoft's revision : Added API.
KDDiv kdDiv ( KDint numer, KDint denom );


/*******************************************************
 * Locale specific functions
 *******************************************************/

// kdGetLocale : Determine the current language and locale.
// XMSoft's revision : Currently "Win32" and "iOS" platforms supported.
KD_API const KDchar* KD_APIENTRY kdGetLocale ( KDvoid );

/*******************************************************
 * Memory allocation
 *******************************************************/

// kdMalloc : Allocate memory.
KD_API KDvoid* KD_APIENTRY kdMalloc ( KDsize size );

// kdFree : Free allocated memory block. 
KD_API KDvoid KD_APIENTRY kdFree ( KDvoid* ptr );

// kdRealloc : Resize memory block. 
KD_API KDvoid* KD_APIENTRY kdRealloc ( KDvoid* ptr, KDsize size );

// kdCalloc : Allocate space for array in memory
//            Allocates a block of memory for an array of num elements,
//            each of them size bytes long, and initializes all its bits to zero.
// XMSoft's revision : Added API.
KD_API KDvoid* KD_APIENTRY kdCalloc ( KDsize num, KDsize size );

/*******************************************************
 * Thread-local storage.
 *******************************************************/

// XMSoft's revision : Added Macros.
typedef struct KDTLS                        KDTLS;
#define KD_GET_TLS( TYPE, VAL )             TYPE* VAL = (TYPE *) kdGetTLS ( )
#define KD_SET_TLS( TYPE )                                \
{                                                         \
    TYPE* tls = (TYPE *) kdMalloc ( sizeof ( TYPE ) );    \
    kdMemset ( tls, 0, sizeof ( TYPE ) );                 \
    kdSetTLS ( (KDvoid *) tls );                          \
}

// kdGetTLS : Get the thread-local storage pointer. 
KD_API KDvoid* KD_APIENTRY kdGetTLS ( KDvoid );

// kdSetTLS : Set the thread-local storage pointer. 
KD_API KDvoid KD_APIENTRY kdSetTLS ( KDvoid* ptr );

/*******************************************************
 * Mathematical functions
 *******************************************************/

#define KD_E_F                              2.718281750F
#define KD_PI_F                             3.141592740F
#define KD_PI_2_F                           1.570796370F
#define KD_2PI_F                            6.283185480F
#define KD_LOG2E_F                          1.442695020F
#define KD_LOG10E_F                         0.434294492F
#define KD_LN2_F                            0.693147182F
#define KD_LN10_F                           2.302585120F
#define KD_PI_4_F                           0.785398185F
#define KD_1_PI_F                           0.318309873F
#define KD_2_PI_F                           0.636619747F
#define KD_2_SQRTPI_F                       1.128379230F
#define KD_SQRT2_F                          1.414213540F
#define KD_SQRT1_2_F                        0.707106769F
#define KD_FLT_EPSILON                      1.19209290E-07F
#define KD_FLT_MAX                          3.40282346638528860e+38F
#define KD_FLT_MIN                          1.17549435e-38F
#define kdIsNan(x)                          ( ( ( x ) != ( x ) ) ? 1 : 0 )
#define KD_HUGE_VALF                        KD_INFINITY
#define KD_DEG_TO_RAD_F                     0.0174532924F
#define KD_RAD_TO_DEG_F                     57.2957802F

// XMSoft's revision : Added Macros.
#define KD_RAD2DEG( X )                     ( ( X ) * 57.2957795129f )
#define KD_DEG2RAD( X )                     ( ( X ) * 0.01745329252f )
#define	KD_F2X( X )                         ( (KDfixed) ( ( X ) * 65536.0f ) )
#define KD_X2F( X )                         ( (KDfloat) ( ( X ) * 0.0000152587890625f ) )
#define KD_I2X( X )                         ( ( (KDfixed) ( X ) ) << 16 )
#define KD_I2F( X )                         ( (KDfloat) ( X ) )
#define KD_F2I( X )                         ( (KDint) ( X ) )
#define KD_X2I( X )                         ( ( (KDint) ( X ) ) >> 16 )

// kdInfinity : Infinity function.
// XMSoft's revision : Added API.
KD_API KDfloat32 KD_APIENTRY kdInfinity ( KDvoid );

// kdAcosf : Arc cosine function. 
KD_API KDfloat32 KD_APIENTRY kdAcosf ( KDfloat32 x );

// kdAsinf : Arc sine function.
KD_API KDfloat32 KD_APIENTRY kdAsinf ( KDfloat32 x );

// kdAtanf : Arc tangent function.
KD_API KDfloat32 KD_APIENTRY kdAtanf ( KDfloat32 x );

// kdAtan2f : Arc tangent function. 
KD_API KDfloat32 KD_APIENTRY kdAtan2f ( KDfloat32 y, KDfloat32 x );

// kdCosf : Cosine function. 
KD_API KDfloat32 KD_APIENTRY kdCosf ( KDfloat32 x );

// kdSinf : Sine function. 
KD_API KDfloat32 KD_APIENTRY kdSinf ( KDfloat32 x );

// kdTanf : Tangent function.
KD_API KDfloat32 KD_APIENTRY kdTanf ( KDfloat32 x );

// kdExpf : Exponential function. 
KD_API KDfloat32 KD_APIENTRY kdExpf ( KDfloat32 x );

// kdLogf : Natural logarithm function.
KD_API KDfloat32 KD_APIENTRY kdLogf ( KDfloat32 x );

// kdFabsf : Absolute value.
KD_API KDfloat32 KD_APIENTRY kdFabsf ( KDfloat32 x );

// kdPowf : Power function.
// XMSoft's revision : Unexpectedly processed in "WinCE" and "Android" platforms ( Case : Infinity Input Value )
KD_API KDfloat32 KD_APIENTRY kdPowf ( KDfloat32 x, KDfloat32 y );

// kdSqrtf : Square root function.
KD_API KDfloat32 KD_APIENTRY kdSqrtf ( KDfloat32 x );

// kdCeilf : Return ceiling value.
KD_API KDfloat32 KD_APIENTRY kdCeilf ( KDfloat32 x );

// kdFloorf : Return floor value.
KD_API KDfloat32 KD_APIENTRY kdFloorf ( KDfloat32 x );

// kdRoundf : Round value to nearest integer.
KD_API KDfloat32 KD_APIENTRY kdRoundf ( KDfloat32 x );

// kdInvsqrtf : Inverse square root function.
KD_API KDfloat32 KD_APIENTRY kdInvsqrtf ( KDfloat32 x );

// kdFmodf : Calculate floating point remainder.
KD_API KDfloat32 KD_APIENTRY kdFmodf ( KDfloat32 x, KDfloat32 y );

/*******************************************************
 * String and memory functions
 *******************************************************/

// kdMemchr : Scan memory for a byte value.
KD_API KDvoid* KD_APIENTRY kdMemchr ( const KDvoid* src, KDint byte, KDsize len );

// kdMemcmp : Compare two memory regions.
KD_API KDint KD_APIENTRY kdMemcmp ( const KDvoid* src1, const KDvoid* src2, KDsize len );

// kdMemcpy : Copy a memory region, no overlapping.
KD_API KDvoid* KD_APIENTRY kdMemcpy ( KDvoid* buf, const KDvoid* src, KDsize len );

// kdMemmove : Copy a memory region, overlapping allowed.
KD_API KDvoid* KD_APIENTRY kdMemmove ( KDvoid* buf, const KDvoid* src, KDsize len );

// kdMemset : Set bytes in memory to a value.
KD_API KDvoid* KD_APIENTRY kdMemset ( KDvoid* buf, KDint byte, KDsize len );

// kdStrchr : Scan string for a byte value.
KD_API KDchar* KD_APIENTRY kdStrchr ( const KDchar* str, KDint ch );

// kdStrrchr : Locate last occurrence of character in string
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrrchr ( const KDchar* str, KDint ch );

// kdStrcmp : Compares two strings.
KD_API KDint KD_APIENTRY kdStrcmp ( const KDchar* str1, const KDchar* str2 );

// kdStrcasecmp, kdStrncasecmp : Compare two strings ignoring case.
KD_API KDint KD_APIENTRY kdStrcasecmp ( const KDchar* str1, const KDchar* str2 );
KD_API KDint KD_APIENTRY kdStrncasecmp ( const KDchar* str1, const KDchar* str2, KDsize maxlen );

// kdStrlen : Determine the length of a string.
KD_API KDsize KD_APIENTRY kdStrlen ( const KDchar* str );

// kdStrnlen : Determine the length of a string.
KD_API KDsize KD_APIENTRY kdStrnlen ( const KDchar* str, KDsize maxlen );

// kdStrcat, kdStrncat : Concatenate two strings.
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrcat ( KDchar* buf, const KDchar* src );
KD_API KDchar* KD_APIENTRY kdStrncat ( KDchar* buf, const KDchar* src, KDsize maxlen );

// kdStrncat_s : Concatenate two strings.
KD_API KDint KD_APIENTRY kdStrncat_s ( KDchar* buf, KDsize buflen, const KDchar* src, KDsize srcmaxlen );

// kdStrncmp : Compares two strings with length limit.
KD_API KDint KD_APIENTRY kdStrncmp ( const KDchar* str1, const KDchar* str2, KDsize maxlen );

// kdStrcpy, kdStrncpy : Copy a string
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrcpy ( KDchar* buf, const KDchar* src );
KD_API KDchar* KD_APIENTRY kdStrncpy ( KDchar* buf, const KDchar* src, KDsize maxlen );

// kdStrcpy_s : Copy a string with an overrun check.
KD_API KDint KD_APIENTRY kdStrcpy_s ( KDchar* buf, KDsize buflen, const KDchar* src );

// kdStrncpy_s : Copy a string with an overrun check.
KD_API KDint KD_APIENTRY kdStrncpy_s ( KDchar* buf, KDsize buflen, const KDchar* src, KDsize srclen );

// kdStrtok : A sequence of calls to this function split str into tokens, 
//            which are sequences of contiguous characters separated by any of the characters 
//            that are part of delimiters
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrtok ( KDchar* str, const KDchar* delim );

// kdStrstr : Locate substring
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrstr ( const KDchar* str1, const KDchar* str2 );

// kdStrspn : returns the length of the initial portion of str1 which consists only of 
//             characters that are part of str2.
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdStrspn ( const KDchar* str1, const KDchar* str2 );
    
// kdStrcspn : Get span until character in string
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdStrcspn ( const KDchar* str1, const KDchar* str2 );

// kdStrdup : returns a pointer to a new string which is a duplicate of the string s.
//            Memory for the new string is obtained with kdMalloc, and can be freed with kdFree
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrdup ( const KDchar* str );

// kdStrndup : The function is similar, but only copies at most n characters.
//             If s is longer than n, only n characters are copied, and a terminating
//             null byte ('\0') is added.
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrndup ( const KDchar* str, KDsize maxlen );

// kdTolower : Convert uppercase letter to lowercase
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdTolower ( KDint c );

// kdToupper : Convert lowercase letter to uppercase
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdToupper ( KDint c );

// kdIslower : Check if character is lowercase letter
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIslower ( KDint c );

// kdIsupper : Check if character is uppercase letter
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsupper ( KDint c );

// kdIsupper : Check if character is alphabetic
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsalpha ( KDint c );

// kdIsupper : Check if character is decimal digit
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsdigit ( KDint c );
    
// kdIsspace : Check if character is a white-space
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsspace ( KDint c );
    
// kdIsprint : Check if character is printable
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsprint ( KDint c );

// kdAtoi : Convert string to integer
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdAtoi ( const KDchar* str );

// kdAtof : Convert string to float
// XMSoft's revision : Added API.
KD_API KDfloat KD_APIENTRY kdAtof ( const KDchar* str );

// kdAtod : Convert string to double
// XMSoft's revision : Added API.
KD_API KDdouble KD_APIENTRY kdAtod ( const KDchar* str );

/*******************************************************
 * Time functions
 *******************************************************/

// XMSoft's revision : Added Macros.
#define KD_N2M_SEC( N )                   ( (KDuint) ( N / 1000000LL ) )
#define KD_GET_UST2MSEC                   ( kdGetTickCount ( ) )

typedef struct KDTm 
{
    KDint32     tm_sec;
    KDint32     tm_min;
    KDint32     tm_hour;
    KDint32     tm_mday;
    KDint32     tm_mon;
    KDint32     tm_year;
    KDint32     tm_wday;
    KDint32     tm_yday;
	KDint32		tm_isdst;
} KDTm;

typedef struct KDTimeval
{
	KDint32     tv_sec;
	KDint32     tv_usec;
} KDTimeval;

typedef struct KDTimezone
{
    KDint32     tz_minuteswest; 
    KDint32     tz_dsttime;      
} KDTimezone;

// kdGetTimeUST : Get the current unadjusted system time.
KD_API KDust KD_APIENTRY kdGetTimeUST ( KDvoid );

// kdTime : Get the current wall clock time.
KD_API KDtime KD_APIENTRY kdTime ( KDtime* timep );

// kdGmtime_r, kdLocaltime_r : Convert a seconds-since-epoch time into broken-down time.
KD_API KDTm* KD_APIENTRY kdGmtime_r ( const KDtime* timep, KDTm* result );
KD_API KDTm* KD_APIENTRY kdLocaltime_r ( const KDtime* timep, KDTm* result );

// kdUSTAtEpoch : Get the UST corresponding to KDtime 0.
// XMSoft's revision : Not supported API.
KD_API KDust KD_APIENTRY kdUSTAtEpoch ( KDvoid );

// kdGettimeofday : Get date and time.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdGettimeofday ( struct KDTimeval* tv, struct KDTimezone* tz );

// kdSettimeofday : Set date and time.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSettimeofday ( const struct KDTimeval* tv, const struct KDTimezone* tz );

// kdDifftime : Return difference between two times.
// XMSoft's revision : Added API.
KD_API KDdouble KD_APIENTRY kdDifftime ( KDtime end, KDtime beginning );

// Convert KDTm structure to KDtime ( local time )
// XMSoft's revision : Added API.
KD_API KDtime KD_APIENTRY kdMktime ( struct KDTm* timeptr );

// Convert KDTm structure to KDtime ( utc time )
// XMSoft's revision : Added API.
KD_API KDtime KD_APIENTRY kdTimegm ( struct KDTm* timeptr );

// Format time as string
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdStrftime ( KDchar* ptr, KDsize maxsize, const KDchar* format, const struct KDTm* timeptr );

// XMSoft's revision : Added API.
KD_API KDuint   KD_APIENTRY kdGetTickCount ( KDvoid );
KD_API KDvoid   KD_APIENTRY kdResetTickCount ( KDvoid );
KD_API KDdouble KD_APIENTRY kdGetMilliseconds ( KDvoid );

/*******************************************************
 * Timer functions
 *******************************************************/

#define KD_TIMER_ONESHOT                    61
#define KD_TIMER_PERIODIC_AVERAGE           62
#define KD_TIMER_PERIODIC_MINIMUM           63

typedef struct KDTimer                      KDTimer;

// KD_EVENT_TIMER : Timer fire event.
#define KD_EVENT_TIMER                      42

// kdSetTimer : Set timer.
KD_API KDTimer* KD_APIENTRY kdSetTimer ( KDint64 interval, KDint periodic, KDvoid* eventuserptr );

// kdCancelTimer : Cancel and free a timer.
KD_API KDint KD_APIENTRY kdCancelTimer ( KDTimer* timer );

// kdSleep : The function sleep gives a simple way to make the program wait for a short interval.
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSleep ( KDust ust );

#define KD_SLEEP_MSEC(x)					kdSleep ( x * 1000000ULL )

/*******************************************************
 * File system
 *******************************************************/

#define KD_EOF                              -1
#define KD_ISREG(m)                         ( ( m ) & 0x8000 )
#define KD_ISDIR(m)                         ( ( m ) & 0x4000 )

#define KD_F_OK								8
#define KD_R_OK                             4
#define KD_W_OK                             2
#define KD_X_OK                             1

typedef enum 
{
    KD_SEEK_SET =  0, 
    KD_SEEK_CUR =  1, 
    KD_SEEK_END =  2
} KDfileSeekOrigin;

#define KD_O_RDONLY                         0x0000  
#define KD_O_WRONLY                         0x0001  
#define KD_O_RDWR                           0x0002  
#define KD_O_APPEND                         0x0008  
#define KD_O_CREAT                          0x0100  
#define KD_O_TRUNC                          0x0200  
#define KD_O_EXCL                           0x0400  
#define KD_O_NOCTTY							0x0800
#define KD_O_NONBLOCK						0x1000

// Read, write, execute/search by owner 
#define	KD_S_IRWXU                          0000700		// RWX mask for owner 
#define	KD_S_IRUSR                          0000400		// R for owner 
#define	KD_S_IWUSR                          0000200		// W for owner 
#define	KD_S_IXUSR                          0000100		// X for owner 
// Read, write, execute/search by group 
#define	KD_S_IRWXG                          0000070		// RWX mask for group 
#define	KD_S_IRGRP                          0000040		// R for group 
#define	KD_S_IWGRP                          0000020		// W for group 
#define	KD_S_IXGRP                          0000010		// X for group 
// Read, write, execute/search by others 
#define	KD_S_IRWXO                          0000007		// RWX mask for other 
#define	KD_S_IROTH                          0000004		// R for other 
#define	KD_S_IWOTH                          0000002		// W for other 
#define	KD_S_IXOTH                          0000001		// X for other 

#define	KD_S_ISUID                          0004000		// set user id on execution 
#define	KD_S_ISGID                          0002000		// set group id on execution 
#define	KD_S_ISVTX                          0001000		// directory restrcted delete 


typedef struct KDFile                       KDFile;
typedef struct KDDir                        KDDir;

typedef struct KDStat 
{
    KDmode          st_mode;
    KDoff           st_size;
    KDtime          st_mtime;
} KDStat;

typedef struct KDDirent
{
    const KDchar*   d_name;
} KDDirent;

// kdFopen : Open a file from the file system.
KD_API KDFile* KD_APIENTRY kdFopen ( const KDchar* pathname, const KDchar* mode );

// kdFclose : Close an open file.
KD_API KDint KD_APIENTRY kdFclose ( KDFile* file );

// kdFsize : Get file size.
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdFsize ( KDFile* file );

// kdFflush : Flush an open file.
KD_API KDint KD_APIENTRY kdFflush ( KDFile* file );

// kdFread : Read from a file.
KD_API KDsize KD_APIENTRY kdFread ( KDvoid* buffer, KDsize size, KDsize count, KDFile* file );

// kdFwrite : Write to a file.
KD_API KDsize KD_APIENTRY kdFwrite ( const KDvoid* buffer, KDsize size, KDsize count, KDFile* file );

// kdGetc : Read next byte from an open file.
KD_API KDint KD_APIENTRY kdGetc ( KDFile* file );

// kdUngetc : Unget character from stream.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdUngetc ( KDint ch, KDFile* file );

// kdPutc : Write a byte to an open file.
KD_API KDint KD_APIENTRY kdPutc ( KDint c, KDFile* file );

// kdFgets : Read a line of text from an open file.
KD_API KDchar* KD_APIENTRY kdFgets ( KDchar* buffer, KDsize buflen, KDFile* file );

// kdGetline ( ) reads an entire line from stream, storing the address of the buffer containing the text into *lineptr.
// The buffer is null-terminated and includes the newline character, if one was found.
// XMSoft's revision : Added API.
KD_API KDssize kdGetline ( KDchar** lineptr, KDsize* buflen, KDFile* file );

// kdPuts : Write a line of text to an open file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdPuts ( const KDchar* str, KDFile* file );

// kdFEOF : Check for end of file.
KD_API KDint KD_APIENTRY kdFEOF ( KDFile* file );

// kdFerror : Check for an error condition on an open file.
// XMSoft's revision : Unexpectedly processed in "iOS" Platform. 
KD_API KDint KD_APIENTRY kdFerror ( KDFile* file );

// kdClearerr : Clear a file's error and end-of-file indicators.
KD_API KDvoid KD_APIENTRY kdClearerr ( KDFile* file );

// kdFseek : Reposition the file position indicator in a file.
KD_API KDint KD_APIENTRY kdFseek ( KDFile* file, KDoff offset, KDfileSeekOrigin origin );

// kdFtell : Get the file position of an open file.
KD_API KDoff KD_APIENTRY kdFtell ( KDFile* file );

// kdRewind : Set position indicator to the beginning
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdRewind ( KDFile* file );

// kdMkdir : Create new directory.
KD_API KDint KD_APIENTRY kdMkdir ( const KDchar* pathname );

// kdRmdir : Delete a directory.
KD_API KDint KD_APIENTRY kdRmdir ( const KDchar* pathname );

// kdRename : Rename a file.
KD_API KDint KD_APIENTRY kdRename ( const KDchar* src, const KDchar* dest );

// kdRemove : Delete a file.
KD_API KDint KD_APIENTRY kdRemove ( const KDchar* pathname );

// kdTruncate : Truncate or extend a file.
// XMSoft's revision : Unexpectedly processed in "Android" Platform. ( Case : Smaller )
KD_API KDint KD_APIENTRY kdTruncate ( const KDchar *pathname, KDoff length );

// kdStat, kdFstat : Return information about a file.
KD_API KDint KD_APIENTRY kdStat ( const KDchar* pathname, struct KDStat* buf );
KD_API KDint KD_APIENTRY kdFstat ( KDFile* file, struct KDStat* buf );
KD_API KDint KD_APIENTRY kdFstat_n ( KDint fileno, struct KDStat* buf );

// kdLstat The kdLstat() function shall be equivalent to kdStat(), except when path refers to a symbolic link. In that case lstat() shall return information about the link, while stat() shall return information about the file the link references.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdLstat ( const KDchar* path, struct KDStat* buf );

// kdAccess : Determine whether the application can access a file or directory.
// XMSoft's revision : Unexpectedly processed in each platform
KD_API KDint KD_APIENTRY kdAccess ( const KDchar* pathname, KDint amode );

// kdOpenDir : Open a directory ready for listing.
KD_API KDDir* KD_APIENTRY kdOpenDir ( const KDchar* pathname );

// kdReadDir : Return the next file in a directory.
KD_API KDDirent* KD_APIENTRY kdReadDir ( KDDir* dir );

// kdCloseDir : Close a directory.
KD_API KDint KD_APIENTRY kdCloseDir ( KDDir* dir );

// kdReadDir : Return the next file in a directory.
KD_API KDDirent* KD_APIENTRY kdReadDir ( KDDir* dir );

// kdGetFree : Get free space on a drive.
// XMSoft's revision : Not yet to be supported in "Android" platform.
KD_API KDoff KD_APIENTRY kdGetFree ( const KDchar* pathname );

// kdOpen : open a stream around a fixed-length string
// XMSoft's revision : Added API.
KD_API KDFile* KD_APIENTRY kdFmemopen ( KDvoid* buf, KDsize size, const KDchar* mode );

// kdOpen : Get current position in stream.
// XMSoft's revision : Added API.
KD_API KDint kdFgetpos ( KDFile* file, KDoff* pos );

// kdOpen : Open a file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdOpen ( const KDchar* pathname, KDint flags, KDint mode );

// kdClose : Close a file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdClose ( KDint fd );

// kdLseek : Set position in a file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdLseek ( KDint fd, KDint position, KDint startpoint );

// kdRead : Read from a file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdRead ( KDint fd, KDvoid* ptr, KDint len );

// kdWrite : Write to a file. 
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdWrite ( KDint fd, KDvoid* ptr, KDint len );

// kdChdir : Change working directory
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdChdir ( const KDchar* path );

// Provides an alternative pathname for the existing file, so that the file can be accessed by either the old or the new name.
// kdLink ( ) creates a link from the pathname newname to an existing file, with the pathname oldfile.
// XMSoft's revision : Added API.
KD_API KDint kdLink ( const KDchar* oldfile, const KDchar* newname );

// kdUnlink : Delete a name and possibly the file it refers to
// XMSoft's revision : Added API.
KD_API KDint kdUnlink ( const KDchar* pathname );

// kdSync :  synchronize data on disk with memory.
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSync ( KDvoid );

// kdIsatty : test whether a file descriptor refers to a terminal
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsatty ( KDint fd );

//KD_API KDFile* KD_APIENTRY kdStdin ( );
//KD_API KDFile* KD_APIENTRY kdStdout ( );
//KD_API KDFile* KD_APIENTRY kdStderr ( );
    
/*******************************************************
 * Network sockets
 *******************************************************/

#define KD_AF_INET                          70
#define KD_SOCK_TCP                         64
#define KD_SOCK_UDP                         65
#define KD_INADDR_ANY                       0
#define KD_INET_ADDRSTRLEN                  16
#define KD_EVENT_SOCKET_READABLE            49
#define KD_EVENT_NAME_LOOKUP_COMPLETE       53
#define KD_EVENT_SOCKET_WRITABLE            50
#define KD_EVENT_SOCKET_CONNECT_COMPLETE    51
#define KD_EVENT_SOCKET_INCOMING            52

typedef struct KDSocket                     KDSocket;

// KDSockaddr : Struct type for socket address.
typedef struct KDSockaddr
{
    KDuint16    family;
    union 
	{
        struct
		{
            KDuint16    port;
            KDuint32    address;
        } sin;
    } data;
} KDSockaddr;

typedef struct KDInAddr
{
    KDuint32    s_addr;
} KDInAddr;

// KD_EVENT_SOCKET_READABLE : Event to indicate that a socket is readable.
typedef struct KDEventSocketReadable 
{
    KDSocket*   socket;
} KDEventSocketReadable;

// KD_EVENT_SOCKET_WRITABLE : Event to indicate that a socket is writable.
typedef struct KDEventSocketWritable 
{
    KDSocket*   socket;
} KDEventSocketWritable;

// KD_EVENT_SOCKET_CONNECT_COMPLETE : Event generated when a socket connect is complete
typedef struct KDEventSocketConnect
{
    KDSocket*   socket;
    KDint32     error;
} KDEventSocketConnect;

// KD_EVENT_SOCKET_INCOMING : Event generated when a listening socket detects an incoming connection or an error.
typedef struct KDEventSocketIncoming
{
    KDSocket*   socket;
} KDEventSocketIncoming;

// KD_EVENT_NAME_LOOKUP_COMPLETE : kdNameLookup complete event.
typedef struct KDEventNameLookup
{
    KDint32     error;
    KDint32     resultlen;
    const KDSockaddr* result;
    KDboolean   more;
} KDEventNameLookup;

// kdNameLookup : Look up a hostname.
KD_API KDint KD_APIENTRY kdNameLookup ( KDint af, const KDchar* hostname, KDvoid* eventuserptr );

// kdNameLookupCancel : Selectively cancels ongoing kdNameLookup operations.
KD_API KDvoid KD_APIENTRY kdNameLookupCancel ( KDvoid* eventuserptr );

// kdSocketCreate : Creates a socket.
KD_API KDSocket* KD_APIENTRY kdSocketCreate ( KDint type, KDvoid* eventuserptr );

// kdSocketClose : Closes a socket.
KD_API KDint KD_APIENTRY kdSocketClose ( KDSocket* socket );

// kdSocketBind : Bind a socket.
KD_API KDint KD_APIENTRY kdSocketBind ( KDSocket* socket, const struct KDSockaddr* addr, KDboolean reuse );

// kdSocketGetName : Get the local address of a socket. 
KD_API KDint KD_APIENTRY kdSocketGetName ( KDSocket* socket, struct KDSockaddr* addr );

// kdSocketConnect : Connects a socket.
KD_API KDint KD_APIENTRY kdSocketConnect ( KDSocket* socket, const KDSockaddr* addr );

// kdSocketListen : Listen on a socket.
KD_API KDint KD_APIENTRY kdSocketListen ( KDSocket* socket, KDint backlog );

// kdSocketAccept : Accept an incoming connection.
KD_API KDSocket* KD_APIENTRY kdSocketAccept ( KDSocket* socket, KDSockaddr* addr, KDvoid* eventuserptr );

// kdSocketSend, kdSocketSendTo : Send data to a socket. 
KD_API KDint KD_APIENTRY kdSocketSend ( KDSocket* socket, const KDvoid* buf, KDint len );
KD_API KDint KD_APIENTRY kdSocketSendTo ( KDSocket* socket, const KDvoid* buf, KDint len, const KDSockaddr* addr );

// kdSocketRecv, kdSocketRecvFrom: Receive data from a socket.
KD_API KDint KD_APIENTRY kdSocketRecv ( KDSocket* socket, KDvoid* buf, KDint len );
KD_API KDint KD_APIENTRY kdSocketRecvFrom ( KDSocket* socket, KDvoid* buf, KDint len, KDSockaddr* addr );

// kdIsLittleEndian : If host byte order is little endian then return KD_TRUE
// XMSoft's revision : Added API.
KD_API KDbool KD_APIENTRY kdIsLittleEndian ( KDvoid );

// kdEndianSwap32, kdEndianSwap16 : Swap host byte order.
// XMSoft's revision : Added API.
KD_API KDuint32 KD_APIENTRY kdEndianSwap32 ( KDuint32 hostlong );
KD_API KDuint16 KD_APIENTRY kdEndianSwap16 ( KDuint16 hostshort );

// kdHtoll : Convert a 32-bit integer from host to little endian order.
// XMSoft's revision : Added API.
KD_API KDuint32 KD_APIENTRY kdHtoll ( KDuint32 hostlong );

// kdHtols : Convert a 16-bit integer from host to little endian order.
// XMSoft's revision : Added API.
KD_API KDuint16 KD_APIENTRY kdHtols ( KDuint16 hostshort );

// kdHtonl : Convert a 32-bit integer from host to network byte order.
KD_API KDuint32 KD_APIENTRY kdHtonl ( KDuint32 hostlong );

// kdHtons : Convert a 16-bit integer from host to network byte order.
KD_API KDuint16 KD_APIENTRY kdHtons ( KDuint16 hostshort );

// kdNtohl : Convert a 32-bit integer from network to host byte order.
KD_API KDuint32 KD_APIENTRY kdNtohl ( KDuint32 netlong );

// kdNtohs : Convert a 16-bit integer from network to host byte order.
KD_API KDuint16 KD_APIENTRY kdNtohs ( KDuint16 netshort );

// kdInetAton : Convert a dotted quad format address to an integer.
KD_API KDint KD_APIENTRY kdInetAton ( const KDchar* cp, KDuint32* inp );

// kdInetNtop : Convert a network address to textual form.
KD_API const KDchar *KD_APIENTRY kdInetNtop ( KDuint af, const KDvoid* src, KDchar* dst, KDsize cnt );

typedef struct 
{
	KDuint     fd_count;					 // how many are SET? 
	KDSOCKET   fd_array [ KD_FD_SETSIZE ];   // an array of SOCKETs 
} KDFd_set;

// kdSelect : kdSelect allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSelect ( KDint nfds, KDFd_set* readfds, KDFd_set* writefds, KDFd_set* exceptfds, struct KDTimeval* timeout );

#define KD_HOST_IS_BIG_ENDIAN				(KDbool) ( *(KDushort*) "\0\xff" < 0x100 ) 
#define KD_SWAP32(i)						( ( i & 0x000000ff ) << 24 | ( i & 0x0000ff00 ) << 8 | ( i & 0x00ff0000 ) >> 8 | ( i & 0xff000000 ) >> 24 )
#define KD_SWAP16(i)						( ( i & 0x00ff ) << 8 | ( i &0xff00 ) >> 8 )   
#define KD_SWAP_INT32_LITTLE_TO_HOST(i)		( ( KD_HOST_IS_BIG_ENDIAN == KD_TRUE ) ? KD_SWAP32 ( i ) : ( i ) )
#define KD_SWAP_INT16_LITTLE_TO_HOST(i)		( ( KD_HOST_IS_BIG_ENDIAN == KD_TRUE ) ? KD_SWAP16 ( i ) : ( i ) )
#define KD_SWAP_INT32_BIG_TO_HOST(i)		( ( KD_HOST_IS_BIG_ENDIAN == KD_TRUE ) ? (i) : KD_SWAP32 ( i ) )
#define KD_SWAP_INT16_BIG_TO_HOST(i)		( ( KD_HOST_IS_BIG_ENDIAN == KD_TRUE ) ? (i):  KD_SWAP16 ( i ) )

/*******************************************************
 * Input / output
 *******************************************************/

#define KD_EVENT_STATE                      55
#define KD_EVENT_INPUT                      56
#define KD_EVENT_INPUT_JOG                  71
#define KD_EVENT_INPUT_POINTER              57
#define KD_IO_CONTROLLER_STRIDE             64

// KD_IOGROUP_GAMEKEYS : I/O group for game keys. 
#define KD_IOGROUP_GAMEKEYS                 0x1000

#define KD_STATE_GAMEKEYS_AVAILABILITY      ( KD_IOGROUP_GAMEKEYS + 0 )
#define KD_INPUT_GAMEKEYS_UP                ( KD_IOGROUP_GAMEKEYS + 1 )
#define KD_INPUT_GAMEKEYS_LEFT              ( KD_IOGROUP_GAMEKEYS + 2 )
#define KD_INPUT_GAMEKEYS_RIGHT             ( KD_IOGROUP_GAMEKEYS + 3 )
#define KD_INPUT_GAMEKEYS_DOWN              ( KD_IOGROUP_GAMEKEYS + 4 )
#define KD_INPUT_GAMEKEYS_FIRE              ( KD_IOGROUP_GAMEKEYS + 5 )
#define KD_INPUT_GAMEKEYS_A                 ( KD_IOGROUP_GAMEKEYS + 6 )
#define KD_INPUT_GAMEKEYS_B                 ( KD_IOGROUP_GAMEKEYS + 7 )
#define KD_INPUT_GAMEKEYS_C                 ( KD_IOGROUP_GAMEKEYS + 8 )
#define KD_INPUT_GAMEKEYS_D                 ( KD_IOGROUP_GAMEKEYS + 9 )

// KD_IOGROUP_GAMEKEYSNC : I/O group for game keys, no chording.
#define KD_IOGROUP_GAMEKEYSNC               0x1100

#define KD_STATE_GAMEKEYSNC_AVAILABILITY    ( KD_IOGROUP_GAMEKEYSNC + 0 )
#define KD_INPUT_GAMEKEYSNC_UP              ( KD_IOGROUP_GAMEKEYSNC + 1 )
#define KD_INPUT_GAMEKEYSNC_LEFT            ( KD_IOGROUP_GAMEKEYSNC + 2 )
#define KD_INPUT_GAMEKEYSNC_RIGHT           ( KD_IOGROUP_GAMEKEYSNC + 3 )
#define KD_INPUT_GAMEKEYSNC_DOWN            ( KD_IOGROUP_GAMEKEYSNC + 4 )
#define KD_INPUT_GAMEKEYSNC_FIRE            ( KD_IOGROUP_GAMEKEYSNC + 5 )
#define KD_INPUT_GAMEKEYSNC_A               ( KD_IOGROUP_GAMEKEYSNC + 6 )
#define KD_INPUT_GAMEKEYSNC_B               ( KD_IOGROUP_GAMEKEYSNC + 7 )
#define KD_INPUT_GAMEKEYSNC_C               ( KD_IOGROUP_GAMEKEYSNC + 8 )
#define KD_INPUT_GAMEKEYSNC_D               ( KD_IOGROUP_GAMEKEYSNC + 9 )

// KD_IOGROUP_PHONEKEYPAD : I/O group for phone keypad.
#define KD_IOGROUP_PHONEKEYPAD              0x2000

#define KD_STATE_PHONEKEYPAD_AVAILABILITY   ( KD_IOGROUP_PHONEKEYPAD + 0 )
#define KD_INPUT_PHONEKEYPAD_0              ( KD_IOGROUP_PHONEKEYPAD + 1 )
#define KD_INPUT_PHONEKEYPAD_1              ( KD_IOGROUP_PHONEKEYPAD + 2 )
#define KD_INPUT_PHONEKEYPAD_2              ( KD_IOGROUP_PHONEKEYPAD + 3 )
#define KD_INPUT_PHONEKEYPAD_3              ( KD_IOGROUP_PHONEKEYPAD + 4 )
#define KD_INPUT_PHONEKEYPAD_4              ( KD_IOGROUP_PHONEKEYPAD + 5 )
#define KD_INPUT_PHONEKEYPAD_5              ( KD_IOGROUP_PHONEKEYPAD + 6 )
#define KD_INPUT_PHONEKEYPAD_6              ( KD_IOGROUP_PHONEKEYPAD + 7 )
#define KD_INPUT_PHONEKEYPAD_7              ( KD_IOGROUP_PHONEKEYPAD + 8 )
#define KD_INPUT_PHONEKEYPAD_8              ( KD_IOGROUP_PHONEKEYPAD + 9 )
#define KD_INPUT_PHONEKEYPAD_9              ( KD_IOGROUP_PHONEKEYPAD + 10 )
#define KD_INPUT_PHONEKEYPAD_STAR           ( KD_IOGROUP_PHONEKEYPAD + 11 )
#define KD_INPUT_PHONEKEYPAD_HASH           ( KD_IOGROUP_PHONEKEYPAD + 12 )
#define KD_INPUT_PHONEKEYPAD_LEFTSOFT       ( KD_IOGROUP_PHONEKEYPAD + 13 )
#define KD_INPUT_PHONEKEYPAD_RIGHTSOFT      ( KD_IOGROUP_PHONEKEYPAD + 14 )
#define KD_STATE_PHONEKEYPAD_ORIENTATION    ( KD_IOGROUP_PHONEKEYPAD + 15 )

// KD_IOGROUP_VIBRATE : I/O group for vibrate.
#define KD_IOGROUP_VIBRATE                  0x3000

#define KD_STATE_VIBRATE_AVAILABILITY       ( KD_IOGROUP_VIBRATE + 0 )
#define KD_STATE_VIBRATE_MINFREQUENCY       ( KD_IOGROUP_VIBRATE + 1 )
#define KD_STATE_VIBRATE_MAXFREQUENCY       ( KD_IOGROUP_VIBRATE + 2 )
#define KD_OUTPUT_VIBRATE_VOLUME            ( KD_IOGROUP_VIBRATE + 3 )
#define KD_OUTPUT_VIBRATE_FREQUENCY         ( KD_IOGROUP_VIBRATE + 4 )

// KD_IOGROUP_POINTER : I/O group for pointer.
#define KD_IOGROUP_POINTER                  0x4000

#define KD_STATE_POINTER_AVAILABILITY       ( KD_IOGROUP_POINTER + 0 )
#define KD_INPUT_POINTER_X                  ( KD_IOGROUP_POINTER + 1 )
#define KD_INPUT_POINTER_Y                  ( KD_IOGROUP_POINTER + 2 )
#define KD_INPUT_POINTER_SELECT             ( KD_IOGROUP_POINTER + 3 )

// KD_IOGROUP_BACKLIGHT : I/O group for backlight.
#define KD_IOGROUP_BACKLIGHT                0x5000

#define KD_STATE_BACKLIGHT_AVAILABILITY     ( KD_IOGROUP_BACKLIGHT + 0 )
#define KD_OUTPUT_BACKLIGHT_FORCE           ( KD_IOGROUP_BACKLIGHT + 1 )

// KD_IOGROUP_JOGDIAL : I/O group for a jog dial.
#define KD_IOGROUP_JOGDIAL                  0x6000

#define KD_STATE_JOGDIAL_AVAILABILITY       ( KD_IOGROUP_JOGDIAL + 0 )
#define KD_INPUT_JOGDIAL_UP                 ( KD_IOGROUP_JOGDIAL + 1 )
#define KD_INPUT_JOGDIAL_LEFT               ( KD_IOGROUP_JOGDIAL + 2 )
#define KD_INPUT_JOGDIAL_RIGHT              ( KD_IOGROUP_JOGDIAL + 3 )
#define KD_INPUT_JOGDIAL_DOWN               ( KD_IOGROUP_JOGDIAL + 4 )
#define KD_INPUT_JOGDIAL_SELECT             ( KD_IOGROUP_JOGDIAL + 5 )

// KD_IOGROUP_STICK : I/O group for joystick.
#define KD_IOGROUP_STICK                    0x7000

#define KD_STATE_STICK_AVAILABILITY         ( KD_IOGROUP_STICK + 0 )
#define KD_INPUT_STICK_X                    ( KD_IOGROUP_STICK + 1 )
#define KD_INPUT_STICK_Y                    ( KD_IOGROUP_STICK + 2 )
#define KD_INPUT_STICK_Z                    ( KD_IOGROUP_STICK + 3 )
#define KD_INPUT_STICK_BUTTON               ( KD_IOGROUP_STICK + 4 )
#define KD_IO_STICK_STRIDE 8

// KD_IOGROUP_DPAD : I/O group for D-pad.
#define KD_IOGROUP_DPAD                     0x8000

#define KD_STATE_DPAD_AVAILABILITY          ( KD_IOGROUP_DPAD + 0 )
#define KD_STATE_DPAD_COPY                  ( KD_IOGROUP_DPAD + 1 )
#define KD_INPUT_DPAD_UP                    ( KD_IOGROUP_DPAD + 2 )
#define KD_INPUT_DPAD_LEFT                  ( KD_IOGROUP_DPAD + 3 )
#define KD_INPUT_DPAD_RIGHT                 ( KD_IOGROUP_DPAD + 4 )
#define KD_INPUT_DPAD_DOWN                  ( KD_IOGROUP_DPAD + 5 )
#define KD_INPUT_DPAD_SELECT                ( KD_IOGROUP_DPAD + 6 )
#define KD_IO_DPAD_STRIDE 8

// KD_IOGROUP_BUTTONS : I/O group for buttons associated with joystick or D-pad.
#define KD_IOGROUP_BUTTONS                  0x9000

#define KD_STATE_BUTTONS_AVAILABILITY       ( KD_IOGROUP_BUTTONS + 0 )
#define KD_INPUT_BUTTONS_0                  ( KD_IOGROUP_BUTTONS + 1 )

// XMSoft's revision : Added macro.
#define KD_IOGROUP_XMSOFT                   0x10000

#define KD_INPUT_XMSOFT_ESCAPE              ( KD_IOGROUP_XMSOFT + 0 )
#define KD_INPUT_XMSOFT_PREV                ( KD_IOGROUP_XMSOFT + 1 )
#define KD_INPUT_XMSOFT_MENU                ( KD_IOGROUP_XMSOFT + 2 )
#define KD_INPUT_XMSOFT_RETURN              ( KD_IOGROUP_XMSOFT + 3 )
#define KD_INPUT_XMSOFT_CHAR                ( KD_IOGROUP_XMSOFT + 4 )
#define KD_INPUT_XMSOFT_BACKSPACE           ( KD_IOGROUP_XMSOFT + 5 )
#define KD_INPUT_XMSOFT_COMMA				( KD_IOGROUP_XMSOFT + 6 )
#define KD_INPUT_XMSOFT_PERIOD				( KD_IOGROUP_XMSOFT + 7 )
#define KD_INPUT_XMSOFT_LCONTROL			( KD_IOGROUP_XMSOFT + 8 )

#define KD_IOGROUP_KEY						0x100000

#define KD_INPUT_KEY_NONE					( KD_IOGROUP_KEY + 0      )
#define KD_INPUT_KEY_PAUSE					( KD_IOGROUP_KEY + 0x0013 )
#define KD_INPUT_KEY_SCROLL_LOCK			( KD_IOGROUP_KEY + 0x1014 )
#define KD_INPUT_KEY_PRINT					( KD_IOGROUP_KEY + 0x1061 )
#define KD_INPUT_KEY_SYSREQ            		( KD_IOGROUP_KEY + 0x106A )
#define KD_INPUT_KEY_BREAK             		( KD_IOGROUP_KEY + 0x106B )
#define KD_INPUT_KEY_ESCAPE            		( KD_IOGROUP_KEY + 0x001B )
#define KD_INPUT_KEY_BACKSPACE         		( KD_IOGROUP_KEY + 0x0008 )
#define KD_INPUT_KEY_TAB               		( KD_IOGROUP_KEY + 0x0009 )
#define KD_INPUT_KEY_BACK_TAB          		( KD_IOGROUP_KEY + 0x0089 )
#define KD_INPUT_KEY_RETURN           		( KD_IOGROUP_KEY + 0x000D )
#define KD_INPUT_KEY_CAPS_LOCK				( KD_IOGROUP_KEY + 0x00E5 )
#define KD_INPUT_KEY_SHIFT					( KD_IOGROUP_KEY + 0x00E1 )
#define KD_INPUT_KEY_CTRL					( KD_IOGROUP_KEY + 0x00E3 )
#define KD_INPUT_KEY_ALT					( KD_IOGROUP_KEY + 0x00E9 )
#define KD_INPUT_KEY_MENU					( KD_IOGROUP_KEY + 0x1067 )
#define KD_INPUT_KEY_HYPER					( KD_IOGROUP_KEY + 0x10ED )
#define KD_INPUT_KEY_INSERT					( KD_IOGROUP_KEY + 0x1063 )
#define KD_INPUT_KEY_HOME					( KD_IOGROUP_KEY + 0x1050 )
#define KD_INPUT_KEY_PG_UP					( KD_IOGROUP_KEY + 0x1055 )
#define KD_INPUT_KEY_DELETE					( KD_IOGROUP_KEY + 0x10FF )
#define KD_INPUT_KEY_END					( KD_IOGROUP_KEY + 0x1057 )
#define KD_INPUT_KEY_PG_DOWN				( KD_IOGROUP_KEY + 0x1056 )
#define KD_INPUT_KEY_LEFT_ARROW				( KD_IOGROUP_KEY + 0x1051 )
#define KD_INPUT_KEY_RIGHT_ARROW			( KD_IOGROUP_KEY + 0x1053 )
#define KD_INPUT_KEY_UP_ARROW				( KD_IOGROUP_KEY + 0x1052 )
#define KD_INPUT_KEY_DOWN_ARROW				( KD_IOGROUP_KEY + 0x1054 )
#define KD_INPUT_KEY_NUM_LOCK				( KD_IOGROUP_KEY + 0x107F )
#define KD_INPUT_KEY_KP_PLUS				( KD_IOGROUP_KEY + 0x10AB )
#define KD_INPUT_KEY_KP_MINUS				( KD_IOGROUP_KEY + 0x10AD )
#define KD_INPUT_KEY_KP_MULTIPLY			( KD_IOGROUP_KEY + 0x10AA )
#define KD_INPUT_KEY_KP_DIVIDE				( KD_IOGROUP_KEY + 0x10AF )
#define KD_INPUT_KEY_KP_ENTER				( KD_IOGROUP_KEY + 0x108D )
#define KD_INPUT_KEY_KP_HOME				( KD_IOGROUP_KEY + 0x10B7 )
#define KD_INPUT_KEY_KP_UP					( KD_IOGROUP_KEY + 0x10B8 )
#define KD_INPUT_KEY_KP_PG_UP				( KD_IOGROUP_KEY + 0x10B9 )
#define KD_INPUT_KEY_KP_LEFT				( KD_IOGROUP_KEY + 0x10B4 )
#define KD_INPUT_KEY_KP_FIVE				( KD_IOGROUP_KEY + 0x10B5 )
#define KD_INPUT_KEY_KP_RIGHT				( KD_IOGROUP_KEY + 0x10B6 )
#define KD_INPUT_KEY_KP_END					( KD_IOGROUP_KEY + 0x10B1 )
#define KD_INPUT_KEY_KP_DOWN				( KD_IOGROUP_KEY + 0x10B2 )
#define KD_INPUT_KEY_KP_PG_DOWN				( KD_IOGROUP_KEY + 0x10B3 )
#define KD_INPUT_KEY_KP_INSERT				( KD_IOGROUP_KEY + 0x10B0 )
#define KD_INPUT_KEY_KP_DELETE				( KD_IOGROUP_KEY + 0x10AE )
#define KD_INPUT_KEY_F1						( KD_IOGROUP_KEY + 0x00BE )
#define KD_INPUT_KEY_F2						( KD_IOGROUP_KEY + 0x00BF )
#define KD_INPUT_KEY_F3						( KD_IOGROUP_KEY + 0x00C0 )
#define KD_INPUT_KEY_F4						( KD_IOGROUP_KEY + 0x00C1 )
#define KD_INPUT_KEY_F5						( KD_IOGROUP_KEY + 0x00C2 )
#define KD_INPUT_KEY_F6						( KD_IOGROUP_KEY + 0x00C3 )
#define KD_INPUT_KEY_F7						( KD_IOGROUP_KEY + 0x00C4 )
#define KD_INPUT_KEY_F8						( KD_IOGROUP_KEY + 0x00C5 )
#define KD_INPUT_KEY_F9						( KD_IOGROUP_KEY + 0x00C6 )
#define KD_INPUT_KEY_F10					( KD_IOGROUP_KEY + 0x00C7 )
#define KD_INPUT_KEY_F11					( KD_IOGROUP_KEY + 0x00C8 )
#define KD_INPUT_KEY_F12					( KD_IOGROUP_KEY + 0x00C9 )
#define KD_INPUT_KEY_SPACE					( KD_IOGROUP_KEY + ' '    )
#define KD_INPUT_KEY_EXCLAM					( KD_IOGROUP_KEY + '!'    )
#define KD_INPUT_KEY_QUOTE					( KD_IOGROUP_KEY + '"'    )
#define KD_INPUT_KEY_NUMBER					( KD_IOGROUP_KEY + '#'    )
#define KD_INPUT_KEY_DOLLAR					( KD_IOGROUP_KEY + '$'    )
#define KD_INPUT_KEY_PERCENT				( KD_IOGROUP_KEY + '%'    )
#define KD_INPUT_KEY_CIRCUMFLEX				( KD_IOGROUP_KEY + '^'    )
#define KD_INPUT_KEY_AMPERSAND				( KD_IOGROUP_KEY + '&'    )
#define KD_INPUT_KEY_APOSTROPHE				( KD_IOGROUP_KEY + '\''   )
#define KD_INPUT_KEY_LEFT_PARENTHESIS		( KD_IOGROUP_KEY + '('    )
#define KD_INPUT_KEY_RIGHT_PARENTHESIS		( KD_IOGROUP_KEY + ')'    )
#define KD_INPUT_KEY_ASTERISK				( KD_IOGROUP_KEY + '*'    )
#define KD_INPUT_KEY_PLUS					( KD_IOGROUP_KEY + '+'    )
#define KD_INPUT_KEY_COMMA					( KD_IOGROUP_KEY + ','    )
#define KD_INPUT_KEY_MINUS					( KD_IOGROUP_KEY + '-'    )
#define KD_INPUT_KEY_PERIOD					( KD_IOGROUP_KEY + '.'    )
#define KD_INPUT_KEY_SLASH					( KD_IOGROUP_KEY + '/'    )
#define KD_INPUT_KEY_0						( KD_IOGROUP_KEY + '0'    )
#define KD_INPUT_KEY_1						( KD_IOGROUP_KEY + '1'    )
#define KD_INPUT_KEY_2						( KD_IOGROUP_KEY + '2'    )
#define KD_INPUT_KEY_3						( KD_IOGROUP_KEY + '3'    )
#define KD_INPUT_KEY_4						( KD_IOGROUP_KEY + '4'    )
#define KD_INPUT_KEY_5						( KD_IOGROUP_KEY + '5'    )
#define KD_INPUT_KEY_6						( KD_IOGROUP_KEY + '6'    )
#define KD_INPUT_KEY_7						( KD_IOGROUP_KEY + '7'    )
#define KD_INPUT_KEY_8						( KD_IOGROUP_KEY + '8'    )
#define KD_INPUT_KEY_9						( KD_IOGROUP_KEY + '9'    )
#define KD_INPUT_KEY_COLON					( KD_IOGROUP_KEY + ':'    )
#define KD_INPUT_KEY_SEMICOLON				( KD_IOGROUP_KEY + ';'    )
#define KD_INPUT_KEY_LESS_THAN				( KD_IOGROUP_KEY + '<'    )
#define KD_INPUT_KEY_EQUAL					( KD_IOGROUP_KEY + '='    )
#define KD_INPUT_KEY_GREATER_THAN			( KD_IOGROUP_KEY + '>'    )
#define KD_INPUT_KEY_QUESTION				( KD_IOGROUP_KEY + '?'    )
#define KD_INPUT_KEY_AT						( KD_IOGROUP_KEY + '@'    )
#define KD_INPUT_KEY_CAPITAL_A				( KD_IOGROUP_KEY + 'A'    )
#define KD_INPUT_KEY_CAPITAL_B				( KD_IOGROUP_KEY + 'B'    )
#define KD_INPUT_KEY_CAPITAL_C				( KD_IOGROUP_KEY + 'C'    )
#define KD_INPUT_KEY_CAPITAL_D				( KD_IOGROUP_KEY + 'D'    )
#define KD_INPUT_KEY_CAPITAL_E				( KD_IOGROUP_KEY + 'E'    )
#define KD_INPUT_KEY_CAPITAL_F				( KD_IOGROUP_KEY + 'F'    )
#define KD_INPUT_KEY_CAPITAL_G				( KD_IOGROUP_KEY + 'G'    )
#define KD_INPUT_KEY_CAPITAL_H				( KD_IOGROUP_KEY + 'H'    )
#define KD_INPUT_KEY_CAPITAL_I				( KD_IOGROUP_KEY + 'I'    )
#define KD_INPUT_KEY_CAPITAL_J				( KD_IOGROUP_KEY + 'J'    )
#define KD_INPUT_KEY_CAPITAL_K				( KD_IOGROUP_KEY + 'K'    )
#define KD_INPUT_KEY_CAPITAL_L				( KD_IOGROUP_KEY + 'L'    )
#define KD_INPUT_KEY_CAPITAL_M				( KD_IOGROUP_KEY + 'M'    )
#define KD_INPUT_KEY_CAPITAL_N				( KD_IOGROUP_KEY + 'N'    )
#define KD_INPUT_KEY_CAPITAL_O				( KD_IOGROUP_KEY + 'O'    )
#define KD_INPUT_KEY_CAPITAL_P				( KD_IOGROUP_KEY + 'P'    )
#define KD_INPUT_KEY_CAPITAL_Q				( KD_IOGROUP_KEY + 'Q'    )
#define KD_INPUT_KEY_CAPITAL_R				( KD_IOGROUP_KEY + 'R'    )
#define KD_INPUT_KEY_CAPITAL_S				( KD_IOGROUP_KEY + 'S'    )
#define KD_INPUT_KEY_CAPITAL_T				( KD_IOGROUP_KEY + 'T'    )
#define KD_INPUT_KEY_CAPITAL_U				( KD_IOGROUP_KEY + 'U'    )
#define KD_INPUT_KEY_CAPITAL_V				( KD_IOGROUP_KEY + 'V'    )
#define KD_INPUT_KEY_CAPITAL_W				( KD_IOGROUP_KEY + 'W'    )
#define KD_INPUT_KEY_CAPITAL_X				( KD_IOGROUP_KEY + 'X'    )
#define KD_INPUT_KEY_CAPITAL_Y				( KD_IOGROUP_KEY + 'Y'    )
#define KD_INPUT_KEY_CAPITAL_Z				( KD_IOGROUP_KEY + 'Z'    )
#define KD_INPUT_KEY_LEFT_BRACKET			( KD_IOGROUP_KEY + '['    )
#define KD_INPUT_KEY_BACK_SLASH				( KD_IOGROUP_KEY + '\\'   )
#define KD_INPUT_KEY_RIGHT_BRACKET			( KD_IOGROUP_KEY + ']'    )
#define KD_INPUT_KEY_UNDERSCORE				( KD_IOGROUP_KEY + '_'    )
#define KD_INPUT_KEY_GRAVE					( KD_IOGROUP_KEY + '`'    )
#define KD_INPUT_KEY_A						( KD_IOGROUP_KEY + 'a'    )
#define KD_INPUT_KEY_B						( KD_IOGROUP_KEY + 'b'    )
#define KD_INPUT_KEY_C						( KD_IOGROUP_KEY + 'c'    )
#define KD_INPUT_KEY_D						( KD_IOGROUP_KEY + 'd'    )
#define KD_INPUT_KEY_E						( KD_IOGROUP_KEY + 'e'    )
#define KD_INPUT_KEY_F						( KD_IOGROUP_KEY + 'f'    )
#define KD_INPUT_KEY_G						( KD_IOGROUP_KEY + 'g'    )
#define KD_INPUT_KEY_H						( KD_IOGROUP_KEY + 'h'    )
#define KD_INPUT_KEY_I						( KD_IOGROUP_KEY + 'i'    )
#define KD_INPUT_KEY_J						( KD_IOGROUP_KEY + 'j'    )
#define KD_INPUT_KEY_K						( KD_IOGROUP_KEY + 'k'    )
#define KD_INPUT_KEY_L						( KD_IOGROUP_KEY + 'l'    )
#define KD_INPUT_KEY_M						( KD_IOGROUP_KEY + 'm'    )
#define KD_INPUT_KEY_N						( KD_IOGROUP_KEY + 'n'    )
#define KD_INPUT_KEY_O						( KD_IOGROUP_KEY + 'o'    )
#define KD_INPUT_KEY_P						( KD_IOGROUP_KEY + 'p'    )
#define KD_INPUT_KEY_Q						( KD_IOGROUP_KEY + 'q'    )
#define KD_INPUT_KEY_R						( KD_IOGROUP_KEY + 'r'    )
#define KD_INPUT_KEY_S						( KD_IOGROUP_KEY + 's'    )
#define KD_INPUT_KEY_T						( KD_IOGROUP_KEY + 't'    )
#define KD_INPUT_KEY_U						( KD_IOGROUP_KEY + 'u'    )
#define KD_INPUT_KEY_V						( KD_IOGROUP_KEY + 'v'    )
#define KD_INPUT_KEY_W						( KD_IOGROUP_KEY + 'w'    )
#define KD_INPUT_KEY_X						( KD_IOGROUP_KEY + 'x'    )
#define KD_INPUT_KEY_Y						( KD_IOGROUP_KEY + 'y'    )
#define KD_INPUT_KEY_Z						( KD_IOGROUP_KEY + 'z'    )
#define KD_INPUT_KEY_LEFT_BRACE				( KD_IOGROUP_KEY + '{'    )
#define KD_INPUT_KEY_BAR					( KD_IOGROUP_KEY + '|'    )
#define KD_INPUT_KEY_RIGHT_BRACE			( KD_IOGROUP_KEY + '}'    )
#define KD_INPUT_KEY_TILDE					( KD_IOGROUP_KEY + '~'    )
#define KD_INPUT_KEY_EURO					( KD_IOGROUP_KEY + 0x20AC )
#define KD_INPUT_KEY_POUND					( KD_IOGROUP_KEY + 0x00A3 )
#define KD_INPUT_KEY_YEN					( KD_IOGROUP_KEY + 0x00A5 )
#define KD_INPUT_KEY_MIDDLE_DOT				( KD_IOGROUP_KEY + 0x0095 )
#define KD_INPUT_KEY_SEARCH					( KD_IOGROUP_KEY + 0xFFAA )

#define KD_IOGROUP_MOUSE					0x100000

#define KD_INPUT_MOUSE_BUTTON_1             ( KD_IOGROUP_MOUSE + 0 )
#define KD_INPUT_MOUSE_BUTTON_2             ( KD_IOGROUP_MOUSE + 1 )
#define KD_INPUT_MOUSE_BUTTON_3             ( KD_IOGROUP_MOUSE + 2 )
#define KD_INPUT_MOUSE_BUTTON_4             ( KD_IOGROUP_MOUSE + 3 )
#define KD_INPUT_MOUSE_BUTTON_5             ( KD_IOGROUP_MOUSE + 4 )
#define KD_INPUT_MOUSE_BUTTON_6             ( KD_IOGROUP_MOUSE + 5 )
#define KD_INPUT_MOUSE_BUTTON_7             ( KD_IOGROUP_MOUSE + 6 )
#define KD_INPUT_MOUSE_BUTTON_8             ( KD_IOGROUP_MOUSE + 7 )

#define KD_INPUT_MOUSE_LEFT					KD_INPUT_MOUSE_BUTTON_1
#define KD_INPUT_MOUSE_RIGHT                KD_INPUT_MOUSE_BUTTON_2
#define KD_INPUT_MOUSE_MIDDLE				KD_INPUT_MOUSE_BUTTON_3
#define KD_INPUT_MOUSE_LAST				    KD_INPUT_MOUSE_BUTTON_8

#define KD_IOGROUP_APNS                     0x20000

#define KD_APNS_DEVIDREG_OK                 ( KD_IOGROUP_APNS + 0 )
#define KD_APNS_DEVIDREG_FAIL               ( KD_IOGROUP_APNS + 1 )
#define KD_APNS_NOTIFY_RECEIVED             ( KD_IOGROUP_APNS + 2 )
	
#define KD_IOGROUP_NMEA	                    0x30000

#define KD_NMEA_UPDATED_GPS                 ( KD_IOGROUP_NMEA + 0 )
#define KD_NMEA_UPDATED_USER                ( KD_IOGROUP_NMEA + 1 )

#define KD_IOGROUP_SOUND                    0x40000

#define KD_SOUND_COMPLETED                  ( KD_IOGROUP_SOUND + 0 )

#define KD_IOGROUP_TTS                      0x80000

#define KD_TTS_COMPLETED                    ( KD_IOGROUP_TTS + 0 )


// KD_IO_UNDEFINED : I/O items reserved for implementation-dependent use.
#define KD_IO_UNDEFINED                     0x40000000

// KD_EVENT_STATE : State changed event.
typedef struct KDEventState 
{
    KDint32     index;
    union 
	{
        KDint32	    i;
        KDint64     l;
        KDfloat32   f;
    } value;
} KDEventState;

// KD_EVENT_INPUT : Input changed event.         
typedef struct KDEventInput 
{
    KDint32     index;
    union 
	{
        KDint32     i;
        KDint64     l;
        KDfloat32   f;
    } value;
} KDEventInput;

// KD_EVENT_INPUT_JOG : Jogdial jog event. 
typedef struct KDEventInputJog
{
    KDint32     index;
    KDint32     count;
} KDEventInputJog;

// KD_EVENT_INPUT_POINTER : Pointer input changed event.
typedef struct KDEventInputPointer 
{
    KDint32     index;
    KDint32     select;
    KDint32     x;
    KDint32     y;
} KDEventInputPointer;

// KD_EVENT_INPUT_STICK : Joystick stick changed event.
#define KD_EVENT_INPUT_STICK 58
typedef struct KDEventInputStick
{
    KDint32     index;
    KDint32     x;
    KDint32     y;
    KDint32     z;
} KDEventInputStick;

// kdStateGeti, kdStateGetl, kdStateGetf : get state value(s)
KD_API KDint KD_APIENTRY kdStateGeti ( KDint startidx, KDuint numidxs, KDint32* buffer );
KD_API KDint KD_APIENTRY kdStateGetl ( KDint startidx, KDuint numidxs, KDint64* buffer );
KD_API KDint KD_APIENTRY kdStateGetf ( KDint startidx, KDuint numidxs, KDfloat32* buffer );

// kdOutputSeti, kdOutputSetf : set outputs
KD_API KDint KD_APIENTRY kdOutputSeti ( KDint startidx, KDuint numidxs, const KDint32* buffer );
KD_API KDint KD_APIENTRY kdOutputSetf ( KDint startidx, KDuint numidxs, const KDfloat32* buffer );

/*******************************************************
 * Windowing
 *******************************************************/

/*
   XMSoft's revision : All window's APIs are not used.
                       Only a window is used. so see xm_kode.h.
*/

// KD_WINDOWPROPERTY_SIZE : Window client area width and height.
#define KD_WINDOWPROPERTY_SIZE              66

// KD_WINDOWPROPERTY_VISIBILITY : Window visibility status.
#define KD_WINDOWPROPERTY_VISIBILITY        67

// KD_WINDOWPROPERTY_FOCUS : Window input focus status.
#define KD_WINDOWPROPERTY_FOCUS             68

// KD_WINDOWPROPERTY_CAPTION : Window caption.
#define KD_WINDOWPROPERTY_CAPTION           69

// KD_EVENT_WINDOW_CLOSE : Event to request to close window.
#define KD_EVENT_WINDOW_CLOSE               44

// KD_EVENT_WINDOWPROPERTY_CHANGE : Notification about realized window property change.
#define KD_EVENT_WINDOWPROPERTY_CHANGE      47

// KD_EVENT_WINDOW_FOCUS : Event for change of window's focus state.
#define KD_EVENT_WINDOW_FOCUS               60

// KD_EVENT_WINDOW_REDRAW : Event to notify need to redraw the window.
#define KD_EVENT_WINDOW_REDRAW              59
    
#define KD_WINDOWPROPERTY_DEVICE_SIZE       70
#define KD_WINDOWPROPERTY_SCALE             71

typedef struct KDEventWindowFocus
{
    KDint32     focusstate;
} KDEventWindowFocus;

typedef struct KDEventWindowProperty
{
    KDint32     pname;
} KDEventWindowProperty;

typedef KDvoid*                             XMEGLDisplay;
typedef KDvoid*                             XMEGLConfig;
typedef KDvoid*                             XMEGLNativeWindowType;

typedef struct KDWindow                     KDWindow;

// kdCreateWindow : Create a window.
// XMSoft's revision : Not be used.
KD_API KDWindow* KD_APIENTRY kdCreateWindow ( XMEGLDisplay display, XMEGLConfig config, KDvoid* eventuserptr );

// kdDestroyWindow : Destroy a window.
// XMSoft's revision : Not be used.
KD_API KDint KD_APIENTRY kdDestroyWindow ( KDWindow* window );

// kdSetWindowPropertybv, kdSetWindowPropertyiv, kdSetWindowPropertycv: Set a window property to request a change in the on-screen representation of the window.
// XMSoft's revision : Not be used.
KD_API KDint KD_APIENTRY kdSetWindowPropertybv ( KDWindow* window, KDint pname, const KDboolean* param );
KD_API KDint KD_APIENTRY kdSetWindowPropertyiv ( KDWindow* window, KDint pname, const KDint32* param );
KD_API KDint KD_APIENTRY kdSetWindowPropertycv ( KDWindow* window, KDint pname, const KDchar* param );

// kdGetWindowPropertybv, kdGetWindowPropertyiv, kdGetWindowPropertycv: Get the current value of a window property.
// XMSoft's revision : Not be used.
KD_API KDint KD_APIENTRY kdGetWindowPropertybv ( KDWindow* window, KDint pname, KDboolean* param );
KD_API KDint KD_APIENTRY kdGetWindowPropertyiv ( KDWindow* window, KDint pname, KDint32* param );
KD_API KDint KD_APIENTRY kdGetWindowPropertycv ( KDWindow* window, KDint pname, KDchar* param, KDsize* size );

// kdRealizeWindow : Realize the window as a displayable entity and get the native window handle for passing to EGL.
// XMSoft's revision : Not be used.
KD_API KDint KD_APIENTRY kdRealizeWindow ( KDWindow* window, XMEGLNativeWindowType* nativewindow );

/*******************************************************
 * Assertions and logging
 *******************************************************/

// kdHandleAssertion : Handle assertion failure.
KD_API KDvoid KD_APIENTRY kdHandleAssertion ( const KDchar* condition, const KDchar* filename, KDint linenumber );

// kdLogMessage : Output a log message.
#ifdef KD_NDEBUG
#define kdLogMessage(s)
#else
KD_API KDvoid KD_APIENTRY kdLogMessage ( const KDchar* string );
#endif

#define KD_EVENT_NATIVE                   0x00000000
#define KD_EVENT_CREATE                   0x00000001
#define KD_EVENT_DESTROY                  0x00000002
#define KD_EVENT_RESIZE                   0x00000004
#define KD_EVENT_FOCUS                    0x00000008
#define KD_EVENT_VISIBLE                  0x00000010
#define KD_EVENT_REDRAW                   0x00000020
#define KD_EVENT_UPDATE                   0x00000040
#define KD_EVENT_MEMORY_WARNING           0x00000080
#define KD_EVENT_TOUCH_BEGAN              0x00000100
#define KD_EVENT_TOUCH_MOVED              0x00000200
#define KD_EVENT_TOUCH_ENDED              0x00000400
#define KD_EVENT_TOUCH_CANCELLED          0x00000800
#define KD_EVENT_KEY_RELEASED             0x00001000
#define KD_EVENT_KEY_PRESSED              0x00002000
#define KD_EVENT_MAGNETOMETER             0x00004000
#define KD_EVENT_ACCELEROMETER            0x00008000
#define KD_EVENT_LOCATION                 0x00010000
#define KD_EVENT_PROVIDER                 0x00020000
#define KD_EVENT_INSERT_TEXT              0x00040000
#define KD_EVENT_COMPLETED                0x00080000
#define KD_EVENT_MOUSE_PRESSED            0x00100000
#define KD_EVENT_MOUSE_RELEASED           0x00200000
#define KD_EVENT_MOUSE_MOVED              0x00400000
#define KD_EVENT_MOUSE_SCROLLED           0x00800000
#define KD_EVENT_IME_NOTIFICATION         0x01000000
#define KD_EVENT_SERIALIZE                0x80000000
	
/* CS Smart Game event */	
#define KD_EVENT_POINTING				  0x10000000
#define KD_EVENT_QUATERNION				  0x20000000
#define KD_EVENT_EULER					  0x40000000

typedef struct
{
	KDvoid*     p;
} KDEventNative;

typedef struct
{
	KDint       i;
} KDEventValue;

typedef struct
{
	KDuint      width;
	KDuint      height;
} KDEventSize;

typedef struct 
{
	KDuint      msec;
} KDEventUpdate;

typedef struct
{
	KDust       timestamp;
	KDvoid*     id;
	KDint       x;
	KDint       y;
	KDint       prev_x;
	KDint       prev_y;
    KDint       diff_x;
    KDint       diff_y;
} KDTouch;

typedef struct 
{
	KDint32     count;
	KDTouch*    touches;
} KDEventTouches;

typedef struct  
{
	KDint		x;
	KDint		y;
	KDint		code;
} KDEventMouse;

typedef struct  
{
	KDfloat     x;
	KDfloat     y;
	KDfloat     z;
} KDEventAccelerometer;

typedef struct
{
    KDfloat     azimuth;    
    KDfloat     pitch;
    KDfloat     roll;
    KDfloat     x;          // micro-tesla
    KDfloat     y;
    KDfloat     z;
} KDEventMagnetometer;
    
typedef struct
{
	KDint       code;
} KDEventKeypad;

typedef struct
{
	KDchar      utf8[8];
} KDEventInsert;

typedef struct
{
	KDint       type;
	KDvoid*     userptr;
} KDEventCompleted;

typedef struct
{
    KDint       status;         // 0 : Did Hide, 1 : Will Hide, 2 : Did Show, 3 : Will Show
    KDRect      begin;
    KDRect      end;
    KDfloat     duration;
} KDEventIMENotification;
    
typedef struct
{
	KDint       type;
	KDchar*     data;
	KDint       size;
} KDEventSerialize;
	
/* CS Smart game event */
typedef struct
{
	KDint		x;		// yaw
	KDint		y;		// pitch
} KDEventPointing;

typedef struct
{
	KDint		q1;
	KDint		q2;
	KDint		q3;
	KDint		q4;
} KDEventQuaternion;

typedef struct
{
	KDfloat     pitch;	// Y
	KDfloat		roll;	// X	
	KDfloat     yaw;	// Z
} KDEventEuler;						

// struct KDEvent delayed to the end as it uses event data structs from other parts of the .h file.
struct KDEvent 
{
    KDust       timestamp;
    KDint32     type;
    KDvoid*     userptr;
    union KDEventData
	{
        KDEventState           state;
        KDEventInput           input;
        KDEventInputJog        inputjog;
        KDEventInputPointer    inputpointer;
        KDEventInputStick      inputstick;
        KDEventSocketReadable  socketreadable;
        KDEventSocketWritable  socketwritable;
        KDEventSocketConnect   socketconnect;
        KDEventSocketIncoming  socketincoming;
        KDEventNameLookup      namelookup;
        KDEventWindowProperty  windowproperty;
        KDEventWindowFocus     windowfocus;
        KDEventUser            user;
 
        // XMSoft's revision
        KDEventNative          native;
		KDEventValue           value;
        KDEventSize            size;
        KDEventUpdate          update;
        KDEventTouches         touch;
		KDEventMouse           mouse;
        KDEventAccelerometer   accelerometer;
        KDEventMagnetometer    magnetometer;
        KDEventKeypad          keypad;
        KDEventInsert          insert;
        KDEventIMENotification imenotify;
		KDEventCompleted	   completed;
        KDEventSerialize       serialize;
		
		// CS Smart's revision 
		KDEventPointing        pointing;
		KDEventQuaternion	   quaternion;
		KDEventEuler		   euler;
    } data;
};
	
#ifdef __cplusplus
}
#endif

#endif 

