/* --------------------------------------------------------------------------
 *
 *      File            ex_24.cpp
 *      Description     24. XM_Queue
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

typedef struct XMData 
{
	KDint       index;
	KDchar      name[128];
} XMData;

static XMQueue*	    g_queue1 = 0;
static XMQueue*	    g_queue2 = 0;

static KDint xmFindQueue ( KDvoid* data, KDvoid* find )
{
	return ( (XMData *) data )->index >= *( (KDint *) find ) ? 0 : -1;
}

static KDvoid xmFreeQueue ( KDvoid* data )
{
	XMData*    _data = (XMData *) data;

	_data->index = 0;
	kdMemset ( _data->name, 0, 128 );

	kdFree ( data );
}

static KDvoid xmEx_Create ( KDvoid )
{
	//
	// 설명 : 큐를 생성한다.
	// 반환 : 큐 핸들
	//
	g_queue1 = xmQueueCreate ( );
	g_queue2 = xmQueueCreate ( );

	kdAssert ( g_queue1 );
	kdAssert ( g_queue2 );

	kdLogMessagefKHR ( "xmQueueCreate      : queue = 0x%08x", g_queue1 );
	kdLogMessagefKHR ( "xmQueueCreate      : queue = 0x%08x", g_queue2 );
}

static KDvoid xmEx_Push ( KDvoid )
{
	XMData*    data1  = 0;
	XMData*	   data2  = 0;
	KDint      i;
	KDint      ret;

	for ( i = 0; i < 7; i++ )
	{
		data1 = (XMData *) kdMalloc ( sizeof ( XMData ) );
		data2 = (XMData *) kdMalloc ( sizeof ( XMData ) );

		data1->index = i;
		data2->index = i;

		kdSprintfKHR ( data1->name, "data = 0x%08x, index = %d", data1, data1->index );
		kdSprintfKHR ( data2->name, "data = 0x%08x, index = %d", data2, data2->index );

		//
		// 설명 : 큐의 앞부분에 데이타를 넣는다. 
		// 인자 : 큐 핸들, 데이타
		// 반환 : 성공 = 0, 실패 = -1
		//
		ret = xmQueuePushFront ( g_queue1, ( KDvoid * ) data1 );

		kdAssert ( ret == 0 );
		kdLogMessagefKHR ( "xmQueuePushFront   : queue = 0x%08x, %s", g_queue1, data1->name );

		//
		// 설명 : 큐의 뒷부분에 데이타를 넣는다. 
		// 인자 : 큐 핸들, 데이타
		// 반환 : 성공 = 0, 실패 = -1
		//
		ret = xmQueuePushRear ( g_queue2, ( KDvoid * ) data2 );
		
		kdAssert ( ret == 0 );		
		kdLogMessagefKHR ( "xmQueuePushRear    : queue = 0x%08x, %s", g_queue2, data2->name );
	}
}

KDvoid xmEx_Pop ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;
	KDint      find  = 3;

	//
	// 설명 : 큐의 앞부분에 데이타를 꺼낸다.
	// 인자 : 큐 핸들
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueuePopFront ( g_queue1 );
	data2 = (XMData *) xmQueuePopFront ( g_queue2 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueuePopFront    : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueuePopFront    : queue = 0x%08x, %s", g_queue2, data2->name );

	//
	// 설명 : 큐의 뒷부분에 데이타를 꺼낸다.
	// 인자 : 큐 핸들
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueuePopRear ( g_queue1 );
	data2 = (XMData *) xmQueuePopRear ( g_queue2 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueuePopRear     : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueuePopRear     : queue = 0x%08x, %s", g_queue2, data2->name );

	//
	// 설명 : 큐의 조건에 의해 데이타를 꺼낸다.
	// 인자 : 큐 핸들, 조건 함수, 조건 변수
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueuePopFind ( g_queue1, xmFindQueue, &find );
	data2 = (XMData *) xmQueuePopFind ( g_queue2, xmFindQueue, &find );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueuePopFind     : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueuePopFind     : queue = 0x%08x, %s", g_queue2, data2->name );
}

static KDvoid xmEx_Free ( KDvoid )
{
	//
	// 설명 : 큐를 해제한다.
	// 인자 : 큐 핸들, 큐 삭제 함수 ( 널값이면 자동으로 데이타가 있는지 판단해서 데이타를 해제 한다. )
	//
	xmQueueFree ( g_queue1, xmFreeQueue );
	xmQueueFree ( g_queue2, 0 );

	kdLogMessagefKHR ( "xmQueueFree        : queue = 0x%08x", g_queue1 );
	kdLogMessagefKHR ( "xmQueueFree        : queue = 0x%08x", g_queue2 );
}

static KDvoid xmEx_Size ( KDvoid )
{
	KDsize    size1;
	KDsize    size2;

	//
	// 설명 : 큐 사이즈를 구한다.
	// 인자 : 큐 핸들
	// 반환 : 큐 사이즈
	//
	size1 = xmQueueSize ( g_queue1 );
	size2 = xmQueueSize ( g_queue2 );

	kdAssert ( size1 && size2 );
	kdLogMessagefKHR ( "xmQueueSize        : queue = 0x%08x, size = %d", g_queue1, size1 );
	kdLogMessagefKHR ( "xmQueueSize        : queue = 0x%08x, size = %d", g_queue2, size2 );
}

static KDvoid xmEx_Get ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;

	//
	// 설명 : 큐에서 데이타를 가져 온다. ( 큐에서 데이타만 접근한다. )
	// 인자 : 큐 핸들, 인덱스
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueueGet ( g_queue1, 1 );
	data2 = (XMData *) xmQueueGet ( g_queue2, 1 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueueGet         : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueueGet         : queue = 0x%08x, %s", g_queue2, data2->name );
}

static KDvoid xmEx_Erase ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;

	//
	// 설명 : 큐 해당 인덱스의 노드를 제거한다.
	// 인자 : 큐 핸들, 인덱스
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueueErase ( g_queue1, 1 );
	data2 = (XMData *) xmQueueErase ( g_queue2, 1 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueueErase       : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueueErase       : queue = 0x%08x, %s", g_queue2, data2->name );

	kdFree ( data1 );
	kdFree ( data2 );
}

static KDvoid xmEx_Find ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;
	KDint      find  = 1;

	//
	// 설명 : 큐의 앞부분 부터 조건에 의한 데이타를 찾는다. ( 단 조건 함수가 없으면, 첫번째 데이타를 반환한다. )
	// 인자 : 큐 핸들, 조건 함수, 조건 변수
	// 반환 : 데이타
	//
	data1 = (XMData *) xmQueueFind ( g_queue1, xmFindQueue, &find );

	//
	// 설명 : 큐의 뒷부분 부터 조건에 의한 데이타를 찾는다. ( 단 조건 함수가 없으면, 첫번째 데이타를 반환한다. )
	// 인자 : 큐 핸들, 조건 함수, 조건 변수
	// 반환 : 데이타
	//
	data2 = (XMData *) xmQueueReverseFind ( g_queue2, xmFindQueue, &find );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueueFind        : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueueReverseFind : queue = 0x%08x, %s", g_queue2, data2->name );	
}

/*
static KDvoid xmEx_FindNext ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;

	//
	// 설명 : Find로 찾은 데이타가 있으면, 그 다음 조건이 맞는 데이타를 찾는다. 
	// 인자 : 큐 핸들
	// 반환 : 데이타 
	//
	data1 = (XMData *) xmQueueFindNext ( g_queue1 );
	data2 = (XMData *) xmQueueFindNext ( g_queue2 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "xmQueueFindNext    : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "xmQueueFindNext    : queue = 0x%08x, %s", g_queue2, data2->name );	
}

static KDvoid xmEx_EraseFound ( KDvoid )
{
	XMData*    data1 = 0;
	XMData*    data2 = 0;

	//
	// 설명 : Find로 찾은 데이타가 있으면, 그 다음 조건이 맞는 데이타를 찾는다. 
	// 인자 : 큐 핸들
	// 반환 : 데이타 
	//
	data1 = (XMData *) xmQueueEraseFound ( g_queue1 );
	data2 = (XMData *) xmQueueEraseFound ( g_queue2 );

	kdAssert ( data1 && data2 );
	kdLogMessagefKHR ( "Example xmQueueEraseFound()  : queue = 0x%08x, %s", g_queue1, data1->name );
	kdLogMessagefKHR ( "Example xmQueueEraseFound()  : queue = 0x%08x, %s", g_queue2, data2->name );	

	kdFree ( data1 );
	kdFree ( data2 );
}
*/

KDvoid xmExample_24 ( KDvoid )
{	
	kdLogMessage ( "Example 24. XM_Queue\n\n" );

	//
	// * 사용 주의 사항
	//
	//   - 큐는 기본적으로 접근 상호 배제가 들어가져 있습니다.
	//   - 따라서, 하나의 큐에 동시 접근을 하는 경우 DeadLock이 발생 할 수 있습니다.
	//   - 쓰레드 함수 이용시 주의 바랍니다.
	//

	xmEx_Create ( );
	xmEx_Push   ( );
	xmEx_Pop    ( );
	xmEx_Size   ( );
	xmEx_Get    ( );
	xmEx_Erase  ( );
	xmEx_Find   ( );
	//xmEx_FindNext   ( );
	//xmEx_EraseFound ( );
	xmEx_Free   ( );
}