/* --------------------------------------------------------------------------
 *
 *      File            ex_16.cpp
 *      Description     16. Network sockets
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

#include "main.h"

static KDvoid xmEx_ByteOrder ( KDvoid )
{
	KDuint32  netlong;
	KDuint16  netshort;
	KDuint32  hostlong;
	KDuint16  hostshort;
	KDuint8*  ptr;

	// 
	// 설명 : 호스트가 리틀 엔디안 바이트 오더인지 확인한다.
	// 반환 : 리틀 엔디안인 경우 KD_TRUE
	//
	kdLogMessagefKHR ( "kdIsLittleEndian : %s", kdIsLittleEndian ( ) ? "True" : "False" );

	// 
	// 설명 : 바이트 오더를 호스트 오더로 변경한다.
	// 인자 : 32비트 정수
	// 반환 : 32비트 정수
	//
	ptr = (KDuint8*) &netlong;
	ptr [ 0 ] = 0x00; ptr [ 1 ] = 0x00; ptr [ 2 ] = 0x01; ptr [ 3 ] = 0x00; 
	hostlong = kdNtohl ( netlong );
	
	kdLogMessagefKHR ( "kdNtohl : netlong   = %5u, hostlong  = %5u", netlong, hostlong );

	// 
	// 설명 : 바이트 오더를 호스트 오더로 변경한다.
	// 인자 : 16비트 정수
	// 반환 : 16비트 정수
	//
	ptr = (KDuint8*) &netshort;
	ptr [ 0 ] = 0x00; ptr [ 1 ] = 0x01;
	hostshort = kdNtohs ( netshort );
	
	kdLogMessagefKHR ( "kdNtohs : netshort  = %5u, hostshort = %5u", netshort, hostshort );

	// 
	// 설명 : 바이트 오더를 네트워크 오더 ( 빅엔디안 )로 변경한다.
	// 인자 : 32비트 정수
	// 반환 : 32비트 정수
	//
	netlong = kdHtonl ( hostlong );
	kdLogMessagefKHR ( "kdNtohl : hostlong  = %5u, netlong   = %5u", hostlong, netlong );

	// 
	// 설명 : 바이트 오더를 네트워크 오더 ( 빅엔디안 )로 변경한다.
	// 인자 : 16비트 정수
	// 반환 : 16비트 정수
	//
	netshort = kdHtons ( hostshort );
	kdLogMessagefKHR ( "kdHtons : hostshort = %5u, netshort  = %5u", hostshort, netshort );

	// 
	// 설명 : 바이트 오더를 리틀 엔디안로 변경한다.
	// 인자 : 32비트 정수
	// 반환 : 32비트 정수
	//
	netlong = kdHtoll ( hostlong );
	kdLogMessagefKHR ( "kdHtoll : hostlong  = %5u, netlong   = %5u", hostlong, netlong );

	// 
	// 설명 : 바이트 오더를 리틀 엔디안로 변경한다.
	// 인자 : 16비트 정수
	// 반환 : 16비트 정수
	//
	netshort = kdHtols ( hostshort );
	kdLogMessagefKHR ( "kdHtols : hostshort = %5u, netshort  = %5u", hostshort, netshort );

	// 
	// 설명 : 바이트 오더를 스왑한다.
	// 인자 : 32비트 정수
	// 반환 : 32비트 정수
	//
	kdLogMessagefKHR ( "kdEndianSwap32 : before = %5u, after = %5u", netlong, kdEndianSwap32 ( netlong ) );

	// 
	// 설명 : 바이트 오더를 스왑한다.
	// 인자 : 16비트 정수
	// 반환 : 16비트 정수
	//
	kdLogMessagefKHR ( "kdEndianSwap16 : before = %5u, after = %5u", netshort, kdEndianSwap16 ( netshort ) );
}

static KDvoid xmEx_Conversion ( KDvoid )
{
	KDuint32  ip_as_network_int;
	KDuint32  ip_as_local_int;
	KDInAddr  ip_as_in_addr;
	KDchar    ip_address[] = "5.122.224.33";
	KDchar    buf[KD_INET_ADDRSTRLEN];
	KDuint8   bytes[4];

	kdInetAton ( ip_address, &ip_as_network_int );
	ip_as_local_int = kdNtohl ( ip_as_network_int );

	bytes[0] = (KDuint8) ( ip_as_local_int >> 24 & 0xFF );
    bytes[1] = (KDuint8) ( ip_as_local_int >> 16 & 0xFF );
    bytes[2] = (KDuint8) ( ip_as_local_int >>  8 & 0xFF );
    bytes[3] = (KDuint8) ( ip_as_local_int       & 0xFF );

	kdAssert ( !( ( bytes[0] != 5 ) || ( bytes[1] != 122 ) || ( bytes[2] != 224 ) || ( bytes[3] !=  33 ) ) );    
	kdLogMessagefKHR ( "kdInetAton : %d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3] );

	ip_as_in_addr.s_addr = ip_as_network_int;
	kdInetNtop ( KD_AF_INET, &ip_as_in_addr, &buf[0], sizeof ( buf ) );

	kdAssert ( !kdMemcmp ( ip_address, &buf[0], sizeof ( ip_address ) ) );
	kdLogMessagefKHR ( "kdInetNtop : %s", buf );
}

static KDvoid xmEx_NameLookUp ( KDvoid )
{
	KDEvent const*  event;
	KDchar  buf[KD_INET_ADDRSTRLEN];

	kdNameLookup ( KD_AF_INET, "google.com", KD_NULL );

	while ( ( event = kdWaitEvent ( 0 ) ) )
	{
		if ( event->type == KD_EVENT_NAME_LOOKUP_COMPLETE )
		{
			kdInetNtop ( KD_AF_INET, &event->data.namelookup.result, &buf[0], sizeof ( buf ) );
			kdLogMessagefKHR ( "KD_EVENT_NAME_LOOKUP_COMPLETE : %s", buf );
		}
		
		kdDefaultEvent ( event );
	}
}

static KDvoid xmEx_GetName ( KDvoid )
{
	KDchar      buf[KD_INET_ADDRSTRLEN];

	KDSocket*   socket;
    KDSockaddr  addr;
    KDSockaddr  reported_addr;

	socket = kdSocketCreate ( KD_SOCK_TCP, &socket );
	addr.family = KD_AF_INET;
	kdInetAton ( "127.0.0.1", &addr.data.sin.address );

	addr.data.sin.port = kdHtons ( 80 );

	kdSocketBind ( socket, &addr, KD_FALSE );

	kdSocketGetName ( socket, &reported_addr );

	kdInetNtop ( KD_AF_INET, &reported_addr, &buf[0], sizeof ( buf ) );
	kdLogMessagefKHR ( "kdSocketGetName : %s", buf );

	kdSocketClose ( socket ); 
}

KDvoid xmExample_16 ( KDvoid )
{
	kdLogMessage ( "Example 16. Network sockets\n\n" );

	xmEx_ByteOrder  ( );
	xmEx_Conversion ( );
//	xmEx_NameLookUp ( );
//	xmEx_GetName    ( );
}