/* --------------------------------------------------------------------------
 *
 *      File            win32_kode.h
 *      Description     Win32-specific definitions and functions for XMKode
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

#ifndef __win32_kode_h__
#define __win32_kode_h__

#include <windows.h>
#include <wincrypt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <mmsystem.h>
#include <winsock.h>

#ifndef _WIN32_WCE

#include <WindowsX.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <io.h>
#include <fcntl.h>
#include <digitalv.h>
#include <signal.h>

#endif

#if _MSC_VER < 1600
#define EOVERFLOW               50
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOBUFS                 WSAENOBUFS
#define ECONNREFUSED			WSAECONNREFUSED
#define EISCONN					WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ENETDOWN				WSAENETDOWN
#define ENETUNREACH				WSAENETUNREACH
#define EINPROGRESS				WSAEINPROGRESS
#define EWOULDBLOCK				WSAEWOULDBLOCK
#endif

#define F_OK					8
#define	R_OK                    4
#define	W_OK                    2
#define	X_OK                    1

#define	vsscanf                 xmWinVsscanf
#define	vscanf                  xmWinVscanf
#define	vfscanf                 xmWinVfscanf
#define truncate                xmWinTruncate
#define opendir                 xmWinOpendir
#define readdir                 xmWinReaddir
#define closedir                xmWinClosedir
#define link					xmWinLink
#define sync					_flushall

#define copysign                _copysign

#define strcasecmp              _stricmp
#define strncasecmp             _strnicmp

#define unlink					_unlink
#define lstat					stat

#define gettimeofday            xmWinGettimeofday
#define settimeofday			xmWinSettimeofday

#undef  s_addr
#define s_addr_					S_un.S_addr
#define socklen_t               KDint

typedef	struct DIR              DIR;
typedef	struct dirent           dirent;
typedef	long long               off_t_;

typedef unsigned int			sigset_t;

struct dirent
{
	const char*                 d_name;
};

struct timezone 
{
     int						tz_minuteswest;			// of Greenwich 
     int						tz_dsttime;				// type of dst correction to apply 
};

#ifdef _WIN32_WCE

#define	_S_IREAD                0x0100					// read permission, owner 
#define	_S_IWRITE               0x0080					// write permission, owner 
#define	_S_IEXEC                0x0040					// execute / search permission, owner 

#define O_RDONLY                0x0000  
#define O_WRONLY                0x0001  
#define O_RDWR                  0x0002  
#define O_APPEND                0x0008  
#define O_CREAT                 0x0100  
#define O_TRUNC                 0x0200  
#define O_EXCL                  0x0400  

#define	fstat                   stat
#undef fileno
#define	fileno( X )             (const char*) ( file->path )

#define isatty( F )				1

struct stat 
{
	unsigned short              st_mode;
	long long                   st_size;	
	time_t                      st_mtime;	
};

#define	vsnprintf               _vsnprintf	
#define mkdir                   xmWinMkdir
#define rmdir                   xmWinRmdir
#define rename                  xmWinRename
#define remove                  xmWinRemove
#define stat( P, B )            xmWinStat( P, B )
#define access                  xmWinAccess

#define open                    xmWinOpen
#define close                   xmWinClose
#define tell					xmWinTell
#define lseek                   xmWinLseek
#define read                    xmWinRead    
#define write                   xmWinWrite
#define _unlink					xmWinRemove
#define chdir					xmWinChdir

#define time( T )               xmWinTime( T )
#define gmtime                  xmWinGmtime
#define localtime               xmWinLocaltime
#define mktime					xmWinMktime
#define difftime(_E, _B)		difftime( (__time32_t) (_E), (__time32_t) (_B) )

#define strftime				xmWinStrftime

#define bsearch                 xmWinBsearch

#define	 acosf( X )				(float)  acos ( X )
#define	 asinf( X )				(float)  asin ( X )
#define	 atanf( X )				(float)  atan ( X )
#define	atan2f( Y, X )			(float) atan2 ( Y, X )
#define	  cosf( X )				(float)   cos ( X )
#define	  sinf( X )				(float)   sin ( X )
#define	  tanf( X )				(float)   tan ( X )
#define	  expf( X )				(float)   exp ( X )
#define	  logf( X )				(float)   log ( X )
#define	 fabsf( X )				(float)  fabs ( X )
#define	  powf( X, Y )			(float)   pow ( X, Y )
#define	 sqrtf( X )				(float)  sqrt ( X )
#define	 ceilf( X )				(float)  ceil ( X )
#define	floorf( X ) 			(float) floor ( X )


#if   defined ( Platform_Nexus_s3c6410 )
void ChipSetting_init ( int x, int y, int width, int height, int screenWidth );
void ChipSetting_terminate ( void );
#endif

#else

#define	fileno                  _fileno
#define	access                  _access
#define	rmdir                   _rmdir
#define	mkdir(P, M)             _mkdir ( P )
#define open(P, F, M)           _open ( P, O_BINARY | F, M )
#define close                   _close
#define tell					_tell
#define lseek                   _lseek
#define read                    _read    
#define write                   _write
#define isatty					_isatty
#define chdir					_chdir

#endif

#ifdef _UNICODE

#define	XM_SET_STR(D, S)        D = xmWinWtoMStr ( S )
#define	XM_SET_TSTR(D, S)       D = xmWinMtoWStr ( S )

#define	KD_FREE_STR(S)          free ( S )
#define	KD_FREE_TSTR(S)         free ( S )

#else

#define	XM_SET_STR(D, S)        D = S
#define	XM_SET_TSTR(D, S)       D = S

#define	KD_FREE_STR(S)		
#define	KD_FREE_TSTR(S)

#endif

// xmWinMtoWStr, xmWinWtoMStr : Convert between Multi-Byte and Wide-Char string.
wchar_t* xmWinMtoWStr ( const char* mstr );
char* xmWinWtoMStr ( const wchar_t* wstr );

// xmWinVsscanf, xmWinVscanf, xmWinVfscanf : Read formatted input from a buffer or file. 
int xmWinVsscanf ( const char* str, const char* format, va_list arg_list );
int xmWinVscanf ( const char* format, va_list arg_list );
int xmWinVfscanf ( FILE* file, const char* format, va_list arg_list );

// xmWinDosMapErr : Map window error code to c standard error code.
KDvoid xmWinDosMapErr ( unsigned long oserrno );

// xmWinTruncate : Truncate or extend a file
int	xmWinTruncate ( const char* path, off_t_ length );

// xmWinOpendir : Open a directory ready for listing.
DIR* xmWinOpendir ( const char* dirname );

// xmWinReaddir : Return the next file in a directory.
dirent*	xmWinReaddir ( DIR* dirp );

// xmWinClosedir : Close a directory.
int	xmWinClosedir ( DIR* dirp );

// xmWinGettimeofday : Obtain the current time.
int xmWinGettimeofday ( struct timeval* tv, struct timezone* tz );

// xmWinGettimeofday : Obtain the current time.
int xmWinSettimeofday ( const struct timeval* tv, const struct timezone* tz );

// xmWinLink : Creates a link from the pathname newname to an existing file, with the pathname oldfile.
int xmWinLink ( const KDchar* oldfile, const KDchar* newname );

#ifdef _WIN32_WCE

// xmWinMkdir : Create new directory.
int	xmWinMkdir ( const char* path, mode_t mode );

// xmWinRmdir : Delete a directory.
int	xmWinRmdir ( const char* path );

// xmWinRename : Rename a file.
int	xmWinRename ( const char* _old, const char* _new );

// xmWinRemove : Delete a file.
int	xmWinRemove ( const char* path );

// xmWinStat : Return information about a file.
int	xmWinStat ( const char* path, struct stat* buf );

// xmWinAccess : Determine whether the application can access a file or directory.
// XMSoft's revision : Unexpectedly processed in each platform
int	xmWinAccess ( const char *path, int amode );

// xmWinOpen : Open a file.
int xmWinOpen ( const char* pathname, int flags, int mode );

// xmWinClose : Close a file.
int xmWinClose ( int fd );

// xmWinTell : Determine the current file position
KD_API KDint KD_APIENTRY xmWinTell ( KDint fd );

// xmWinLseek : Set position in a file.
int xmWinLseek ( int fd, int position, int startpoint );

// xmWinRead : Read from a file.
int xmWinRead ( int fd, void* ptr, int len );

// xmWinWrite : Write to a file. 
int xmWinWrite ( int fd, void* ptr, int len );

// xmWinChdir : Change working directory
KDint xmWinChdir ( const KDchar* path );

// xmWinTime :
time_t xmWinTime ( time_t* inTT );

// xmWinGmtime : 
struct tm* xmWinGmtime ( const time_t* _Time );

// xmWinLocaltime :
struct tm* xmWinLocaltime ( const time_t* _Time );

// xmWinMktime :
time_t xmWinMktime ( struct tm* timeptr );

// xmWinStrftime : 
size_t xmWinStrftime ( char *const s, const size_t maxsize, const char *const format, const struct tm * const t );

void* xmWinBsearch ( const void* key, const void* base, size_t num, size_t size, int ( *comparator ) ( const void*, const void* ) );

#endif

extern HINSTANCE         g_inst;

#endif 