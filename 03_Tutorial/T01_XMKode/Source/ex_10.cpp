/* --------------------------------------------------------------------------
 *
 *      File            ex_10.cpp
 *      Description     10. Events
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

#define XM_FOR_PUMP_EVENT        1000
#define XM_FOR_WAIT_EVENT        2000

KDvoid xmRegularCallback ( const KDEvent *event )
{
	kdLogMessagefKHR ( "regular_callback   : event = 0x%08x, type = %04d, userptr = 0x%08x", event, event->type, event->userptr );
}

KDvoid xmRecursiveCallback ( const KDEvent *event )
{
	kdLogMessagefKHR ( "recursive_callback : event = 0x%08x, type = %04d, userptr = 0x%08x", event, event->type, event->userptr );

	if ( event->userptr == 0 )
	{
		const KDEvent* wait;
		
		KDEvent*	event[3];
		KDint		i = 0;

		event[i] = kdCreateEvent ();	
		event[i]->type = XM_FOR_PUMP_EVENT;	
		event[i]->userptr = (KDvoid *) xmRecursiveCallback;
		kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
		
		kdPostEvent ( event[i] );
		kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

		i++;

		kdLogMessagefKHR ( "kdWaitEvent        : waiting message." );	
		wait = kdWaitEvent ( 0 );	
		kdLogMessagefKHR ( "kdWaitEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", wait, wait->type, wait->userptr );

		kdDefaultEvent( wait );	
		kdLogMessagefKHR ( "kdDefaultEvent     : event = 0x%08x", wait );

		event[i] = kdCreateEvent ();	
		event[i]->type = XM_FOR_PUMP_EVENT;	
		event[i]->userptr = (KDvoid *) xmRecursiveCallback;
		kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

		kdPostEvent ( event[i] );
		kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

		i++;

		event[i] = kdCreateEvent ();	
		event[i]->type = XM_FOR_WAIT_EVENT;	
		event[i]->userptr = (KDvoid *) xmRecursiveCallback;
		kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );		

		kdPostEvent ( event[i] );
		kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	}
}

KDvoid xmEx_Regular ( KDvoid )
{
	const KDEvent* wait;

	KDEvent*	event[4];	
	KDint		ret = 0;
	KDint		i   = 0;
	
	kdLogMessage ( "* Case Regular\n\n" );

	//
	// 설명 : 이벤트 생성
	// 반환 : 이벤트 
	//
	event[i] = kdCreateEvent ();
	kdAssert ( ( event[i] ) );

	event[i]->type = XM_FOR_PUMP_EVENT;	// 펌프 테스트 용
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	i++;

	event[i] = kdCreateEvent ();	
	event[i]->type = XM_FOR_WAIT_EVENT;	// 메세지 대기 테스트 용
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	i++;

	event[i] = kdCreateEvent ();
	event[i]->type = XM_FOR_PUMP_EVENT;	// 이벤트 제거 테스트 용
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	i++;

	event[i] = kdCreateEvent ();	
	event[i]->type = XM_FOR_PUMP_EVENT;	// 메세지 대기시 펌프 테스트 용
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

	i = 0;
	//
	// 설명 : 이벤트 전달
	// 인자 : 이벤트
	// 반환 : 성공 = 0, 실패 = -1
	//
	ret = kdPostEvent ( event[i] );
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	i++;

	//
	// 설명 : 이벤트 전달 ( 해당 쓰레드로 이벤트 전달 )
	// 인자 : 이벤트, 쓰레드 핸들 
	// 반환 : 성공 = 0, 실패 = -1
	//	
	ret = kdPostThreadEvent ( event[i], kdThreadSelf() );			// 현재 쓰레드 구현 안됨. 	
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdPostThreadEvent  : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	i++;

	// 
	// 설명 : 이벤트 제거 ( 이미 포스팅 또는 제거를 한 경우에는 예기치 않은 오류 발생 가능 )
	// 인자 : 이벤트
	//
	kdFreeEvent ( event[i] );

	kdLogMessagefKHR ( "kdFreeEvent        : event = 0x%08x", event[i] );
	i++;

	//
	// 설명 : 콜백 함수 등록 ( 이벤트 및 사용자 포인터를 이중으로 등록 가능 )
	// 인자 : 콜백 함수, 이벤트 종류, 사용자 포인터
	// 반환 : 성공 = 0, 실패 = -1
	//	
	ret = kdInstallCallback ( xmRegularCallback, XM_FOR_PUMP_EVENT, KD_NULL );	
	kdAssert ( !ret );

	kdLogMessagefKHR ( "kdInstallCallback  : func  = 0x%08x, type = %04d, userptr = 0x%08x", xmRegularCallback, XM_FOR_PUMP_EVENT, KD_NULL );

	//
	// 설명 : 이벤트 펌프( 등록된 콜백 함수 조건에 성립된 메세지들을 큐에서 꺼내 전부 콜백 함수를 호출한다 )
	// 반환 : 성공 = 0, 실패 = -1
	//
	kdLogMessagefKHR ( "kdPumpEvents       : pumping messages." );

	ret = kdPumpEvents ( );
	
	kdAssert ( !ret );

	//
	// 설명 : 메세지 대기
	// 인자 : 시간제한 ( -1은 무한 )
	// 반환 : 이벤트
	//		
	i = 3;
	kdPostEvent ( event[i] );
	kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	kdLogMessagefKHR ( "kdWaitEvent        : waiting message." );	
	
	wait = kdWaitEvent ( -1 );	
	kdAssert ( wait->type == XM_FOR_WAIT_EVENT );

	kdLogMessagefKHR ( "kdWaitEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", wait, wait->type, wait->userptr );

	//
	// 설명 : 디폴트 처리 ( 전달된 이벤트는 삭제 됨 )
	// 인자 : 이벤트
	// 
	kdDefaultEvent ( wait );	

	kdLogMessagefKHR ( "kdDefaultEvent     : event = 0x%08x", wait );
}

KDvoid xmEx_Recursive ( KDvoid )
{
	const KDEvent*    wait;

	KDEvent*     event[2];		
	KDint        i = 0;

	kdLogMessage ( "\n* Case Recursive\n\n" );

	event[i] = kdCreateEvent ();	
	event[i]->type = XM_FOR_PUMP_EVENT;	
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

	kdPostEvent ( event[i] );
	kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

	i++;

	event[i] = kdCreateEvent ();	
	event[i]->type = XM_FOR_WAIT_EVENT;	
	kdLogMessagefKHR ( "kdCreateEvent      : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );
	
	kdPostEvent ( event[i] );
	kdLogMessagefKHR ( "kdPostEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", event[i], event[i]->type, event[i]->userptr );

	kdInstallCallback ( xmRecursiveCallback, XM_FOR_PUMP_EVENT, KD_NULL );
	kdLogMessagefKHR ( "kdInstallCallback  : func  = 0x%08x, type = %04d, userptr = 0x%08x", xmRecursiveCallback, XM_FOR_PUMP_EVENT, KD_NULL );

	kdLogMessagefKHR ( "kdWaitEvent        : waiting message." );	
	wait = kdWaitEvent ( -1 );	
	kdLogMessagefKHR ( "kdWaitEvent        : event = 0x%08x, type = %04d, userptr = 0x%08x", wait, wait->type, wait->userptr );

	kdDefaultEvent( wait );	
	kdLogMessagefKHR ( "kdDefaultEvent     : event = 0x%08x", wait );
}

KDvoid xmEx_RemoveCallback ( KDvoid )
{
	kdLogMessage ( "\n* Case RemoveCallback\n\n" );

	//
	// 콜백 등록 제거
	//
	kdLogMessagefKHR ( "kdInstallCallback  : uninstall all callbacks." );
	kdInstallCallback ( KD_NULL, (KDint) KD_NULL, KD_NULL );	
}

KDvoid xmExample_10 ( KDvoid )
{	
	kdLogMessage ( "Example 10. Events\n\n" );

	xmEx_Regular ( );
	xmEx_Recursive ( );
	xmEx_RemoveCallback ( );	
}
