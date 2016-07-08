/* --------------------------------------------------------------------------
 *
 *      File            kd_network.c
 *      Description     Network sockets
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

struct KDSocket
{
	KDint		type;
	KDint		sockfd;
	KDvoid*		userptr;
};

#ifndef SHP
static KDint kdSocketBlock ( KDint sockfd, KDbool enable )
{
#ifdef WIN32
	
	KDint  block = enable ? 1 : 0;
    ioctlsocket ( sockfd, FIONBIO, (unsigned long*) &block );

#else

    KDint  flags = fcntl ( sockfd, F_GETFL, 0 );
	
	flags = enable ? flags | O_NONBLOCK : flags & ( ~O_NONBLOCK );

	fcntl ( sockfd, F_SETFL, flags ); 

#endif

	return 0;
}

static KDint kdSocketError ( KDvoid )
{
#ifdef WIN32

	return WSAGetLastError ( );

#else

	return xmGetErrno ( );

#endif
}

static KDint xmConnectNonBlocking ( KDint sockfd, const struct sockaddr *saptr, socklen_t salen, struct timeval tval )
{
	KDint	ret = 0;

    fd_set	rset;
	fd_set	wset;
    KDint	n;

    if ( ( n = connect ( sockfd, (struct sockaddr *) saptr, salen ) ) < 0 )
    {
		KDint  err = kdSocketError ( );

        if ( err != EINPROGRESS && err != EWOULDBLOCK )
        {
            return -1;
        }
    }

    if ( n == 0 )
	{
        goto done;    
	}

    FD_ZERO ( &rset );
    FD_SET ( sockfd, &rset );
    wset = rset;

    if ( (n = select ( sockfd + 1, &rset, &wset, KD_NULL, ( ( tval.tv_sec > 0 ) || ( tval.tv_usec > 0 ) ) ? &tval : KD_NULL ) ) == 0 ) 
    {      
		xmSetErrno ( ETIMEDOUT );
        return -1;
    }

    if ( FD_ISSET ( sockfd, &rset ) || FD_ISSET ( sockfd, &wset ) ) 
    {
#ifndef WIN32
        socklen_t  len = sizeof ( ret );
        if ( getsockopt ( sockfd, SOL_SOCKET, SO_ERROR, &ret, &len ) < 0 )
		{
            return -1;            
		}
#endif
    }
	else
	{
        return -1; 
	}

done :

    if ( ret ) 
	{
        xmSetErrno ( ret );
        return -1;
    }

    return 0;
}
#endif

// kdNameLookup : Look up a hostname.
KD_API KDint KD_APIENTRY kdNameLookup ( KDint af, const KDchar* hostname, KDvoid* eventuserptr )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct hostent*  host = KD_NULL;
	KDint			 n;

	host = gethostbyname ( hostname );

	if ( !host )
	{
		return -1;
	}

	for ( n = 0 ; host->h_addr_list[n] ; n++ )
	{
		KDEvent*   event = kdCreateEvent ( );
		KDSockaddr* addr = kdMalloc ( sizeof ( KDSockaddr ) );

		event->type = KD_EVENT_NAME_LOOKUP_COMPLETE;
		event->data.namelookup.resultlen = 1;
		event->data.namelookup.result = addr;
		event->userptr = eventuserptr;

		addr->family = af;
		kdMemcpy ( &addr->data.sin.address, host->h_addr_list[n], host->h_length );

		kdPostEvent ( event );
	}

	return 0;
#else
	kdSetError ( KD_ENOSYS );
	return -1;
#endif
}

// kdNameLookupCancel : Selectively cancels ongoing kdNameLookup operations.
KD_API KDvoid KD_APIENTRY kdNameLookupCancel ( KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );
}

// kdSocketCreate : Creates a socket.
KD_API KDSocket* KD_APIENTRY kdSocketCreate ( KDint type, KDvoid* eventuserptr )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	KDSocket*  _sock = 0;
	
	if ( ( _sock = (KDSocket*) kdCalloc ( 1, sizeof ( KDSocket ) ) ) )
	{
		_sock->type = type;
		_sock->userptr = eventuserptr;
		
		switch ( type )
		{
			case KD_SOCK_TCP : _sock->sockfd = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP ); break;
			case KD_SOCK_UDP : _sock->sockfd = socket ( AF_INET, SOCK_DGRAM , IPPROTO_UDP ); break; 

			default :

				kdSetError ( KD_EINVAL );
				kdFree ( _sock );
				return 0;
		}

		if ( _sock->sockfd == -1 )
		{
			switch ( xmGetErrno ( ) )
			{
				case EAFNOSUPPORT	 :
				case EPROTONOSUPPORT :
				case EINVAL			 :  kdSetError ( KD_EINVAL );	break;
				case ENOBUFS		 :  
				case ENOMEM			 :  kdSetError ( KD_ENOMEM );	break;
				case EMFILE			 :
				case ENFILE			 :	kdSetError ( KD_EMFILE );	break;
				case EACCES			 :	kdSetError ( KD_EACCES );   break;
				default				 :	kdSetError ( KD_EIO	   );	break;
			}

			kdFree ( _sock );
			_sock = 0;
		}
		else
		{
			KDEvent*  event = kdCreateEvent ( );

			event->type = KD_EVENT_SOCKET_WRITABLE;
			event->userptr = eventuserptr;
			event->data.socketwritable.socket = _sock;

			kdPostEvent ( event );
		}

		kdSocketBlock ( _sock->sockfd, KD_TRUE ); 
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}		

	return _sock;
#else
	kdSetError ( KD_ENOSYS );
	return KD_NULL;
#endif
}

// kdSocketClose : Closes a socket.
KD_API KDint KD_APIENTRY kdSocketClose ( KDSocket* socket )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	if ( closesocket ( socket->sockfd ) != 0 )
	{
		kdFree ( socket );

		kdSetError ( KD_EIO );
		return -1;
	}

	kdFree ( socket );

	return 0;
#else
	kdSetError ( KD_ENOSYS );
	return -1;
#endif
}

// kdSocketBind : Bind a socket.
KD_API KDint KD_APIENTRY kdSocketBind ( KDSocket* socket, const struct KDSockaddr* addr, KDboolean reuse )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct sockaddr_in  _addr;

	kdMemset ( &_addr, 0x00, sizeof ( _addr ) ); 

	if ( addr->family != KD_AF_INET )
	{
		kdSetError ( KD_EAFNOSUPPORT );
		return -1;
	}

	_addr.sin_family       = AF_INET;
	_addr.sin_addr.s_addr_ = addr->data.sin.address; 
	_addr.sin_port         = addr->data.sin.port;

	if ( bind ( socket->sockfd, (struct sockaddr*) &_addr, sizeof( struct sockaddr_in) ) == SOCKET_ERROR )
	{
		kdSetError ( KD_EIO	);

		return -1;
	}
	else
	{
		KDEvent*  event = kdCreateEvent ( );

		event->type = KD_EVENT_SOCKET_READABLE;
		event->userptr = socket->userptr;
		event->data.socketreadable.socket = socket;

		kdPostEvent ( event );
	}

	return 0;

#else

	kdSetError ( KD_ENOSYS );
	return -1;

#endif
}

// kdSocketGetName : Get the local address of a socket. 
KD_API KDint KD_APIENTRY kdSocketGetName ( KDSocket* socket, struct KDSockaddr* addr )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct sockaddr_in  _addr;
	socklen_t  len = sizeof ( _addr );

	if ( socket->type != KD_SOCK_TCP && socket->type != IPPROTO_UDP )
	{
		kdSetError ( KD_EOPNOTSUPP );
		return -1;
	}

	if ( getsockname ( socket->sockfd, (struct sockaddr*) &_addr, &len ) == SOCKET_ERROR )
	{
		kdSetError ( KD_EIO );
		return -1;
	}
	
	addr->family           = _addr.sin_family == AF_INET ? KD_AF_INET : _addr.sin_family;
	addr->data.sin.address = _addr.sin_addr.s_addr_; 
	addr->data.sin.port    = _addr.sin_port;

	return 0;
#else
	kdSetError ( KD_ENOSYS );
	return -1;
#endif
}

// kdSocketConnect : Connects a socket.
KD_API KDint KD_APIENTRY kdSocketConnect ( KDSocket* socket, const KDSockaddr* addr )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct sockaddr_in  _addr;
	struct timeval      tv;
	KDint  error;
	
	tv.tv_sec  = 10;
	tv.tv_usec = 0;

	kdMemset ( &_addr, 0x00, sizeof ( _addr ) ); 

	if ( addr->family != KD_AF_INET )
	{
		kdSetError ( KD_EAFNOSUPPORT );
		return -1;
	}

	_addr.sin_family       = AF_INET;
	_addr.sin_addr.s_addr_ = addr->data.sin.address;
	_addr.sin_port         = addr->data.sin.port;

	if ( xmConnectNonBlocking ( socket->sockfd, (struct sockaddr*) &_addr, sizeof ( _addr ), tv ) == SOCKET_ERROR )
	{
		error = xmGetErrno ( );
		switch ( error )
		{
			case EAGAIN		  :
			case EACCES		  :
			case EEXIST		  : 
			case EPERM		  :	
			case EBADF		  :
			case EFAULT		  :
			case EINVAL		  :	kdSetError ( KD_EINVAL );		break;
			case ENOMEM		  : kdSetError ( KD_ENOMEM );		break;
			case ECONNREFUSED :	kdSetError ( KD_ECONNREFUSED );	break;
			case ETIMEDOUT	  :	kdSetError ( KD_ETIMEDOUT );	break;
			case EISCONN	  :	kdSetError ( KD_EALREADY );		break;
			case EPIPE		  :	kdSetError ( KD_ENOTCONN );		break;
			case ENETDOWN	  :
			case ENETUNREACH  :
			default			  :	kdSetError ( KD_EIO	     );		break;
		}

		return -1;
	}
	else
	{
		KDEvent*  event = kdCreateEvent ( );

		event->type = KD_EVENT_SOCKET_CONNECT_COMPLETE;
		event->userptr = socket->userptr;
		event->data.socketconnect.socket = socket;
		
		kdPostEvent ( event );
	}

	return 0;
#else
	kdSetError ( KD_ENOSYS );
	return -1;
#endif
}

// kdSocketListen : Listen on a socket.
KD_API KDint KD_APIENTRY kdSocketListen ( KDSocket* socket, KDint backlog )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	if ( listen ( socket->sockfd, backlog ) == SOCKET_ERROR )
	{
		kdSetError ( KD_EIO );
		return -1; 
	}
	else
	{
		KDEvent*  event = kdCreateEvent ( );

		event->type = KD_EVENT_SOCKET_INCOMING;
		event->userptr = socket->userptr;
		event->data.socketincoming.socket = socket;

		kdPostEvent ( event );
	}

	return 0;
#else
	kdSetError ( KD_ENOSYS );
	return -1;
#endif
}

// kdSocketAccept : Accept an incoming connection.
KD_API KDSocket* KD_APIENTRY kdSocketAccept ( KDSocket* socket, KDSockaddr* addr, KDvoid* eventuserptr )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct sockaddr_in  _addr;
	socklen_t  size = sizeof ( _addr );
	KDint      sockfd = 0;

	if ( ( sockfd = accept ( socket->sockfd, (struct sockaddr*) &_addr, &size ) ) == -1  )
	{
		switch ( xmGetErrno ( ) )
		{
			case EPIPE		:	kdSetError ( KD_ENOTCONN );		break;
			default			:	kdSetError ( KD_EIO	     );		break;
		}

		return KD_NULL;
	}
	else
	{
		KDSocket*  _sock = 0;
	
		if ( ( _sock = (KDSocket*) kdCalloc ( 1, sizeof ( KDSocket ) ) ) )
		{
			_sock->type = socket->type;
			_sock->sockfd = sockfd;

			addr->family           = _addr.sin_family == AF_INET ? KD_AF_INET : _addr.sin_family;
			addr->data.sin.address = _addr.sin_addr.s_addr_; 
			addr->data.sin.port    = _addr.sin_port;

			{
				KDEvent*  event = kdCreateEvent ( );

				event->type = KD_EVENT_SOCKET_WRITABLE;
				event->userptr = eventuserptr;
				event->data.socketwritable.socket = _sock;

				kdPostEvent ( event );
			}

			{	
				KDEvent*  event = kdCreateEvent ( );

				event->type = KD_EVENT_SOCKET_READABLE;
				event->userptr = socket->userptr;
				event->data.socketreadable.socket = _sock;

				kdPostEvent ( event );
			}

			return _sock;
		}
		else
		{
			kdSetError ( KD_ENOMEM );
			return KD_NULL;
		}
	}
#else
	kdSetError ( KD_ENOSYS );
	return 0;
#endif
}

// kdSocketSend, kdSocketSendTo : Send data to a socket. 
KD_API KDint KD_APIENTRY kdSocketSend ( KDSocket* socket, const KDvoid* buf, KDint len )
{
	KDint  ret = -1;

#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	ret = (KDint) send ( socket->sockfd, buf, len, 0 );
	if ( ret == -1 )
	{
		KDint  error = xmGetErrno ( );
        
        if ( error == EWOULDBLOCK || error == EAGAIN )
        {
            kdSetError ( KD_EAGAIN );
        }
        else if ( error == ENOTCONN || error == EPIPE )
        {
            kdSetError ( KD_ENOTCONN );
        }
        else if ( error == ENOMEM )
        {
            kdSetError ( KD_ENOMEM );	
        }
        else
        {
            kdSetError ( KD_EIO );
        }
	}

#else

	kdSetError ( KD_ENOSYS );
	
#endif

	return ret;
}

KD_API KDint KD_APIENTRY kdSocketSendTo ( KDSocket* socket, const KDvoid* buf, KDint len, const KDSockaddr* addr )
{
	KDint  ret = -1;

#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )

	struct sockaddr_in  _addr;

	kdMemset ( &_addr, 0, sizeof ( _addr ) ); 

	if ( addr )
	{
		if ( addr->family != KD_AF_INET )
		{
			kdSetError ( KD_EAFNOSUPPORT );
			return -1;
		}

		_addr.sin_family       = AF_INET;
		_addr.sin_addr.s_addr_ = addr->data.sin.address;
		_addr.sin_port	       = addr->data.sin.port;
	}

	ret = (KDint) sendto ( socket->sockfd, buf, len, 0, addr ? (struct sockaddr*) &_addr : KD_NULL, sizeof ( _addr ) );
	if ( ret == -1 )
	{
		KDint  error = xmGetErrno ( );
        
        if ( error == EWOULDBLOCK || error == EAGAIN )
        {
            kdSetError ( KD_EAGAIN );
        }
        else if ( error == ENOTCONN || error == EPIPE )
        {
            kdSetError ( KD_ENOTCONN );
        }
        else if ( error == ENOMEM )
        {
            kdSetError ( KD_ENOMEM );	
        }
        else
        {
            kdSetError ( KD_EIO );
        }
	}
	
#else

	kdSetError ( KD_ENOSYS );

#endif

	return ret;
}

// kdSocketRecv, kdSocketRecvFrom: Receive data from a socket.
KD_API KDint KD_APIENTRY kdSocketRecv ( KDSocket* socket, KDvoid* buf, KDint len )
{
	KDint  ret = -1;

#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
/*
	struct  timeval  tv; 
	fd_set  rd_set;

	tv.tv_sec  = 5;
	tv.tv_usec = 0;

	FD_ZERO ( &rd_set );
	FD_SET  ( socket->sockfd, &rd_set);
 
	ret = select ( socket->sockfd + 1, &rd_set, KD_NULL, KD_NULL, &tv );
	if ( ret == -1 )
	{
		return ret;
	}
*/
	ret = (KDint) recv ( socket->sockfd, buf, len, 0 );
	if ( ret == -1 )
	{
        KDint  error = xmGetErrno ( );
        
        if ( error == EWOULDBLOCK || error == EAGAIN || error == EEXIST )
        {
            kdSetError ( KD_EAGAIN );
        }
        else if ( error == ENOTCONN || error == EPIPE )
        {
            kdSetError ( KD_ENOTCONN );
        }
        else if ( error == ENOMEM )
        {
            kdSetError ( KD_ENOMEM );	
        }
        else
        {
            kdSetError ( KD_EIO );
        }
	}

#else

	kdSetError ( KD_ENOSYS );

#endif

	return ret;
}

KD_API KDint KD_APIENTRY kdSocketRecvFrom ( KDSocket* socket, KDvoid* buf, KDint len, KDSockaddr* addr )
{
	KDint  ret = -1;

#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )

	struct sockaddr_in  _addr;
	socklen_t  size = sizeof ( _addr );

	ret = (KDint) recvfrom ( socket->sockfd, buf, len, 0, addr ? (struct sockaddr*) &_addr : KD_NULL, &size );
	if ( ret == -1 )
	{
        KDint  error = xmGetErrno ( );
        
        if ( error == EWOULDBLOCK || error == EAGAIN || error == EEXIST )
        {
            kdSetError ( KD_EAGAIN );
        }
        else if ( error == ENOTCONN || error == EPIPE )
        {
            kdSetError ( KD_ENOTCONN );
        }
        else if ( error == ENOMEM )
        {
            kdSetError ( KD_ENOMEM );	
        }
        else
        {
            kdSetError ( KD_EIO );
        }
	}
	else 
	{
		if ( addr )
		{
			addr->family           = _addr.sin_family == AF_INET ? KD_AF_INET : _addr.sin_family;
			addr->data.sin.address = _addr.sin_addr.s_addr_;
			addr->data.sin.port    = _addr.sin_port;
		}
	}

#else

	kdSetError ( KD_ENOSYS );

#endif

	return ret;
}

// kdIsLittleEndian : If host byte order is little endian then return KD_TRUE
// XMSoft's revision : Added API.
KD_API KDbool KD_APIENTRY kdIsLittleEndian ( KDvoid )
{
	KDuint16  endian_test = 1;

	return *( (KDuint8*) &endian_test ) == 1 ? KD_TRUE : KD_FALSE;
}

// kdEndianSwap32, kdEndianSwap16 : Swap host byte order.
// XMSoft's revision : Added API.
KD_API KDuint32 KD_APIENTRY kdEndianSwap32 ( KDuint32 hostlong )
{
	return ( ( hostlong & 0xff   ) << 24 ) | 
		   ( ( hostlong & 0xff00 ) <<  8 ) | 
		   ( ( hostlong >>  8 ) & 0xff00 ) | 
		   ( ( hostlong >> 24 ) & 0xff   );
}

KD_API KDuint16 KD_APIENTRY kdEndianSwap16 ( KDuint16 hostshort )
{
	return ( ( hostshort & 0xff ) << 8 ) |
		   ( ( hostshort >> 8 ) & 0xff );
}

// kdHtoll : Convert a 32-bit integer from host to little endian order.
// XMSoft's revision : Added API.
KD_API KDuint32 KD_APIENTRY kdHtoll ( KDuint32 hostlong )
{
	if ( kdIsLittleEndian ( ) == KD_FALSE )
	{
		kdEndianSwap32 ( hostlong );
	}

	return hostlong;
}

// kdHtols : Convert a 16-bit integer from host to little endian order.
// XMSoft's revision : Added API.
KD_API KDuint16 KD_APIENTRY kdHtols ( KDuint16 hostshort )
{
	if ( kdIsLittleEndian ( ) == KD_FALSE )
	{
		kdEndianSwap16 ( hostshort );
	}

	return hostshort;
}

// kdHtonl : Convert a 32-bit integer from host to network byte order.
KD_API KDuint32 KD_APIENTRY kdHtonl ( KDuint32 hostlong )
{
	if ( kdIsLittleEndian ( ) == KD_TRUE )
	{
		return kdEndianSwap32 ( hostlong );
	}

	return hostlong;
}

// kdHtons : Convert a 16-bit integer from host to network byte order.
KD_API KDuint16 KD_APIENTRY kdHtons ( KDuint16 hostshort )
{
	if ( kdIsLittleEndian ( ) == KD_TRUE )
	{
		return kdEndianSwap16 ( hostshort );
	}

	return hostshort;
}

// kdNtohl : Convert a 32-bit integer from network to host byte order.
KD_API KDuint32 KD_APIENTRY kdNtohl ( KDuint32 netlong )
{
	if ( kdIsLittleEndian ( ) == KD_TRUE )
	{
		return kdEndianSwap32 ( netlong );
	}

	return netlong;
}

// kdNtohs : Convert a 16-bit integer from network to host byte order.
KD_API KDuint16 KD_APIENTRY kdNtohs ( KDuint16 netshort )
{
	if ( kdIsLittleEndian ( ) == KD_TRUE )
	{
		return kdEndianSwap16 ( netshort );
	}

	return netshort;
}

// kdInetAton : Convert a dotted quad format address to an integer.
KD_API KDint KD_APIENTRY kdInetAton ( const KDchar* cp, KDuint32* inp )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	*inp = inet_addr ( cp ); 
	return *inp;
#else
	return 0;
#endif
}

// kdInetNtop : Convert a network address to textual form.
KD_API const KDchar* KD_APIENTRY kdInetNtop ( KDuint af, const KDvoid* src, KDchar* dst, KDsize cnt )
{
#if !defined ( SHP ) && !defined ( TIZEN ) && !defined ( BLACKBERRY )
	struct in_addr   _addr;
	const KDchar*    saddr;

	if ( af != KD_AF_INET )
	{
		kdSetError ( KD_EAFNOSUPPORT );
		return KD_NULL;
	}

	if ( cnt < KD_INET_ADDRSTRLEN )
	{
		kdSetError ( KD_ENOSPC );
		return KD_NULL;
	}

	_addr.s_addr_ = ( (KDInAddr*) src )->s_addr;
	saddr = inet_ntoa ( _addr );

	kdStrncpy ( dst, saddr, cnt );

	return dst;
#else
	return 0;
#endif
}

// kdSelect : kdSelect allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSelect ( KDint nfds, KDFd_set* readfds, KDFd_set* writefds, KDFd_set* exceptfds, struct KDTimeval* timeout )
{
	return select ( nfds, (fd_set*) readfds, (fd_set*) writefds, (fd_set*) exceptfds, (struct timeval*) timeout );
}
