/* --------------------------------------------------------------------------
 *
 *      File            kd_error.c
 *      Description     Errors 
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

#include "kd_library.h"

static KDint    l_error = 0;

// xmGetErrno : Get last error indication. 
KDint xmGetErrno ( KDvoid )
{
	KDint   err;
	
	#ifdef XM_HAS_PLATFORM_ERRNO

	err = xmPlatformGetErrno ( );	

	#else	

	err = errno;	

	#endif

	return err;
}

// xmSetErrno : Set last error indication. 
KDvoid xmSetErrno ( KDint err )
{
	#ifdef XM_HAS_PLATFORM_ERRNO
	
	xmPlatformSetErrno ( err );
	
	#else
	
	errno = err;
	
	#endif
}

// kdGetError : Get last error indication. 
KD_API KDint KD_APIENTRY kdGetError ( KDvoid )
{	
	return l_error;
}

// kdSetError : Set last error indication. 
KD_API KDvoid KD_APIENTRY kdSetError ( KDint error )
{
	l_error = error;
}

// kdStrerror : Get pointer to error message string.
// XMSoft's revision : Added API.
KD_API const KDchar* KD_APIENTRY kdStrerror ( KDint error )
{
	switch ( error )
	{
		case KD_EACCES			:	return "Permission denied.";
		case KD_EADDRINUSE		:	return "Address in use.";
		case KD_EADDRNOTAVAIL	:	return "Address not available on the local platform.";
		case KD_EAFNOSUPPORT	:	return "Address family not supported.";
		case KD_EAGAIN			:	return "Resource unavailable, try again.";
		case KD_EALREADY		:	return "A connection attempt is already in progress for this socket.";
		case KD_EBADF			:	return "File not opened in the appropriate mode for the operation.";
		case KD_EBUSY			:	return "Device or resource busy.";
		case KD_ECONNREFUSED	:	return "Connection refused.";
		case KD_ECONNRESET		:	return "Connection reset.";
		case KD_EDEADLK			:	return "Resource deadlock would occur.";
		case KD_EDESTADDRREQ	:	return "Destination address required.";
		case KD_EEXIST			:	return "File exists.";
		case KD_EFBIG			:	return "File too large.";
		case KD_EHOSTUNREACH	:	return "Host is unreachable.";
		case KD_EHOST_NOT_FOUND	:	return "The specified name is not known.";
		case KD_EINVAL			:	return "Invalid argument.";
		case KD_EIO				:	return "I/O error.";
		case KD_EILSEQ			:	return "Illegal byte sequence.";
		case KD_EISCONN			:	return "Socket is connected.";
		case KD_EISDIR			:	return "Is a directory.";
		case KD_EMFILE			:	return "Too many open files.";
		case KD_ENAMETOOLONG	:	return "Filename too long.";
		case KD_ENOENT			:	return "No such file or directory.";
		case KD_ENOMEM			:	return "Not enough space.";
		case KD_ENOSPC			:	return "No space left on device.";
		case KD_ENOSYS			:	return "Function not supported.";
		case KD_ENOTCONN		:	return "The socket is not connected.";
		case KD_ENO_DATA		:	return "The specified name is valid but does not have an address.";
		case KD_ENO_RECOVERY	:	return "A non-recoverable error has occurred on the name server.";
		case KD_EOPNOTSUPP		:	return "Operation not supported.";
		case KD_EOVERFLOW		:	return "Overflow.";
		case KD_EPERM			:	return "Operation not permitted.";
		case KD_ERANGE			:	return "Result out of range.";
		case KD_ETIMEDOUT		:	return "Connection timed out.";
		case KD_ETRY_AGAIN		:	return "A temporary error has occurred on an authoratitive name server, and the lookup may succeed if retried later.";
		default					:	return "Unkwon Error.";
	}
}