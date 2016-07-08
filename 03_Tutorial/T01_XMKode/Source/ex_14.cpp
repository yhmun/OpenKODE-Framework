/* --------------------------------------------------------------------------
 *
 *      File            ex_14.cpp
 *      Description     14. Threads and synchronization
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

#define THREAD_QUIT_EVENT		1000
#define THREAD_EXIT_EVENT		2000

static KDvoid xmPostEvent ( KDThread* thread, KDint32 type )
{
	KDEvent* const    event = kdCreateEvent ( );

	event->type = type;
	kdPostThreadEvent ( event, thread );

	kdLogMessagefKHR ( "kdPostThreadEvent          : event  = '0x%08x', type = %d", event, event->type );
}

static void xmWaitProcess ( KDint64 ms )
{
	const KDEvent*    event = 0;
	      KDTimer*    timer = 0;
	      KDint       loop  = 1;
		
	timer = kdSetTimer ( ms * 1000LL * 1000LL, KD_TIMER_ONESHOT, 0 );
	
	while ( loop )
	{
		event = kdWaitEvent ( -1 );

		if ( event->type == KD_EVENT_TIMER ) 
		{
			kdCancelTimer ( timer );
			loop = 0;
		}

		kdDefaultEvent ( event );
	}
}

static KDvoid *xmThread ( KDvoid* arg )
{
	KDThread*    thread  = 0;
	KDint        waiting = 1;
	KDint*       retval  = (KDint *) arg;
	
	thread = kdThreadSelf ( );
	kdLogMessagefKHR ( "kdThreadSelf               : thread = '0x%08x'", thread );

	while ( waiting )
	{
		KDEvent const* event = kdWaitEvent ( 0 );

		if ( event )
		{
			switch( event->type )
			{
				case THREAD_EXIT_EVENT :

					kdDefaultEvent ( event );

					if ( retval )
					{
						*retval *= 100;
					}

					kdThreadExit ( retval ? (KDvoid *) *retval : 0 );

					break;

				case THREAD_QUIT_EVENT :

					waiting = 0;

					if ( retval )
					{
						*retval += 100;
					}

					kdLogMessagefKHR ( "kdWaitEvent                : event  = '0x%08x', type = %d", event, event->type );

					break;
			}
		}
		
		kdDefaultEvent ( event );
	}

	return retval ? (KDvoid *) *retval : 0;
}

static KDvoid xmEx_ThreadJoin ( KDvoid )
{
	KDThreadAttr*    attr   = 0;
	KDThread*        thread	= 0;
	KDint            retval	= 0;
	KDint            arg    = 0;	
	KDint            ret    = 0;
	
	// 
	// 설명 : 쓰레드 속성 생성
	// 반환 : 쓰레드 속성 핸들
	//
	attr = kdThreadAttrCreate ( );
	kdAssert ( attr != 0 );

	kdLogMessagefKHR ( "kdThreadAttrCreate         : attr   = '0x%08x'", attr );

	// 
	// 설명 : Detatch 상태 설정 ( 쓰레드 종료시 자원 회수 설정을 한다 )
	// 인자 : 쓰레드 속성 핸들, 상태 값
	// 반환 : 성공 = 0, 실패 = -1
	//
	ret = kdThreadAttrSetDetachState ( attr, KD_THREAD_CREATE_JOINABLE );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadAttrSetDetachState : attr   = '0x%08x'", attr );

	//
	// 설명 : 현재 쓰레드 ( 현재 쓰레드의 핸들을 가져 온다 )
	// 반환 : 쓰레드 핸들
	//
	thread = kdThreadSelf ( );
	kdAssert ( thread != 0 );

	kdLogMessagefKHR ( "kdThreadSelf               : thread = '0x%08x'", thread );

	// 
	// 설명 : 쓰레드 생성 
	// 인자 : 쓰레드 속성 (KD_NULL 허용), 쓰레드 함수 주소, 쓰레드 함수 인자 (KD_NULL 허용)
	// 반환 : 쓰레드 핸들
	//

	arg = 100;
	thread = kdThreadCreate ( attr, xmThread, (KDvoid *) &arg );
	kdAssert ( thread != 0 );

	kdLogMessagefKHR ( "kdThreadCreate             : thread = '0x%08x'", thread );

	// 
	// 설명 : 쓰레드 속성 해제
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	ret = kdThreadAttrFree ( attr );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadAttrFree           : attribute freed" );


	xmPostEvent ( thread, THREAD_QUIT_EVENT );

	// 
	// 설명 : 쓰레드 귀속 ( 쓰레드 수행이 완료 되길 기다린후, 현재 쓰레드에 귀속한다. )
	// 인자 : 쓰레드 핸들, 쓰레드 함수 반환값 (KD_NULL 허용)
	// 반환 : 성공 = 0, 실패 = -1
	// 
	ret = kdThreadJoin ( thread, (KDvoid **) &retval );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadJoin               : return value = '%d'", retval );
}

static KDvoid xmEx_ThreadDetatch ( KDvoid )
{
	KDThread*    thread = 0;
	KDint        ret    = 0;
	
	thread = kdThreadCreate ( 0, xmThread, 0 );

	xmPostEvent( thread, THREAD_QUIT_EVENT );

	xmWaitProcess ( 1000 );

	//
	// 설명 : 쓰레드 자원 회수 ( 이미 완료된 쓰레드의 자원을 회수 한다 )
	// 인자 : 쓰레드 핸들
	// 반환 : 성공 = 0, 실패 = -1
	//
	ret = kdThreadDetach ( thread );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadDetach             : thread detatched." );
}

static KDvoid xmEx_ThreadExit ( KDvoid )
{
	KDThread*    thread	= 0;
	KDint        retval	= 0;
	KDint        arg    = 0;		
	
	arg = 200;
	thread = kdThreadCreate ( 0, xmThread, (KDvoid *) &arg );

	xmPostEvent ( thread, THREAD_EXIT_EVENT );
	kdThreadJoin ( thread, (KDvoid **) &retval );	

	kdLogMessagefKHR ( "kdThreadJoin               : return value = '%d'", retval );
}

/*
static KDvoid xmEx_ThreadExit2 ( KDvoid )
{
	// 
	// 설명 : 쓰레드 나가기 ( 현재 쓰레드 나가며, 메인 쓰레드인 경우 종료 코드 0을 가진 어플리케이션 종료한다 )
	// 인자 : 쓰레드 함수 반환값 (KD_NULL 허용)
	//
	kdThreadExit ( 0 );

	kdLogMessagefKHR ( "kdThreadExit               : kdThreadExit is abnormal." );
}
*/

static KDint l_single_thread_value = 0;

static KDvoid xmSingleThread ( KDvoid )
{
	l_single_thread_value++;
}

static KDvoid xmEx_ThreadOnce ( KDvoid )
{
	static KDThreadOnce once_control = KD_THREAD_ONCE_INIT;

	KDint    value1 = 0;
	KDint    value2 = 0;

	// 
	// 설명 : 쓰레드 수행 한번만
	// 인자 : static KDThread ( KD_THREAD_ONCE_INT 으로 초기화 해야함 )
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	kdThreadOnce ( &once_control, xmSingleThread );
	value1 = l_single_thread_value;	

    kdThreadOnce ( &once_control, xmSingleThread );
	value2 = l_single_thread_value;
	
	kdAssert ( value1 == value2 );

	kdLogMessagefKHR ( "KDThreadOnce               : value1 = %d, value2 = %d", value1, value2 );
}

static KDThreadMutex*   l_mutex       = 0;
static KDint            l_mutex_value = 0;
static KDint            l_mutex_wait  = 1;

static KDvoid *xmMutexThread ( KDvoid* arg )
{	
	KDThread*	thread	= 0;
	KDint		ret		= 0;
	KDint		i;	
	
	thread = kdThreadSelf ( );
	
	if ( l_mutex_wait == 1 )
	{
		l_mutex_wait = 0;
		xmWaitProcess ( 500 );
	}

	//
	// 설명 : 뮤텍스 잠금
	// 인자 : 뮤텍스 핸들
	// 반환 : 성공 = 0, 실패 = -1
	//
	ret = kdThreadMutexLock ( l_mutex );
	kdAssert ( ret == 0 );
	
	xmWaitProcess ( 100 );
	kdLogMessagefKHR ( "kdThreadMutexLock          : thread = '0x%08x'", thread );

	xmWaitProcess ( 1000 );
	for ( i = 0; i < 10000; i++ )
	{
		l_mutex_value++;
	}	

	//
	// 설명 : 뮤텍스 품
	// 인자 : 뮤텍스 핸들
	// 반환 : 성공 = 0, 실패 = -1
	//
	ret = kdThreadMutexUnlock ( l_mutex );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadMutexUnlock        : thread = '0x%08x'", thread );

	return (KDvoid *) l_mutex_value;
}

static KDvoid xmEx_ThreadMutex ( KDvoid )
{
	KDThread*     thread1 = 0;
	KDThread*     thread2 = 0;
	KDint         retval1 = 0;
	KDint         retval2 = 0;
	KDint         ret     = 0;

	//
	// 설명 : 뮤텍스 생성 ( 뮤텍스 생성시 상태는 Unlock 이다 )
	// 인자 : 뮤텍스 속성 ( 스펙 확장용 : 현재는 반드시 KD_NULL 값만 인정된다 )
	// 반환 : 뮤텍스 핸들
	//
	l_mutex = kdThreadMutexCreate ( KD_NULL );
	kdAssert ( l_mutex != 0 );

	kdLogMessagefKHR ( "kdThreadMutexCreate        : mutex  = '0x%08x'", l_mutex );

	thread1 = kdThreadCreate ( 0, xmMutexThread, 0 );
	thread2 = kdThreadCreate ( 0, xmMutexThread, 0 );
		
	kdThreadJoin ( thread1, (void **) &retval1 );
	kdThreadJoin ( thread2, (void **) &retval2 );	

	kdLogMessagefKHR ( "kdThreadJoin               : return value 1 = '%d'", retval1 );
	kdLogMessagefKHR ( "kdThreadJoin               : return value 2 = '%d'", retval2 );

	//
	// 설명 : 뮤텍스 해제
	// 인자 : 뮤텍스 핸들
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	ret = kdThreadMutexFree ( l_mutex );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadMutexFree          : mutex freed.");
}

static KDThreadMutex*   l_sync_mutex = 0;
static KDThreadCond*    l_sync_cond  = 0;

static KDThreadMutex*   l_cond_mutex = 0;
static KDThreadCond*    l_cond  = 0;

static KDvoid* xmCondPing ( KDvoid* arg )
{
	KDuint8		rand  = 0;
	KDint		count = 1;

	kdThreadMutexLock ( l_sync_mutex );

	//
	// 설명 : 쓰레드 동기화 조건 신호 보내기
	// 인자 : 쓰레드 동기화 조건 핸들
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	kdThreadCondSignal ( l_sync_cond );
	kdThreadMutexUnlock ( l_sync_mutex );

	while ( count < 6 )
	{
		kdThreadMutexLock ( l_cond_mutex );		
		kdLogMessagefKHR ( "Condition Sync             : ping = %d", count );
		kdThreadCondSignal ( l_cond );

		//
		// 설명 : 쓰레드 동기화 조건 신호 대기
		// 인자 : 쓰레드 동기화 조건 핸들
		// 반환 : 성공 = 0, 실패 정의 없음
		//
		kdThreadCondWait ( l_cond, l_cond_mutex );
		kdThreadMutexUnlock ( l_cond_mutex );

		kdCryptoRandom ( &rand, 1 );
		xmWaitProcess ( ( (KDint64) rand ) % 100 * 10 );
		count++;
	}

	return 0;
}

static KDvoid* xmCondPong ( KDvoid* arg )
{	
	KDint     count = 1;

	kdThreadMutexLock ( l_sync_mutex );
	xmWaitProcess ( 1 );
	kdThreadCondSignal ( l_sync_cond );
	kdThreadMutexUnlock ( l_sync_mutex );

	while ( count < 6 )
	{
		kdThreadMutexLock ( l_cond_mutex );		
		kdThreadCondWait ( l_cond, l_cond_mutex );
		kdLogMessagefKHR ( "Condition Sync             : pong = %d", count );
		kdThreadCondSignal ( l_cond );		
		kdThreadMutexUnlock ( l_cond_mutex );
		count++;
	}

	return 0;
}

static KDvoid xmEx_ThreadCond ( KDvoid )
{
	KDThread*    thread[2];
	KDint        ret;
	KDint        i;
	
	l_sync_mutex = kdThreadMutexCreate ( KD_NULL );
	l_cond_mutex = kdThreadMutexCreate ( KD_NULL );

	//
	// 설명 : 쓰레드 동기화 조건 생성
	// 인자 : 쓰레드 동기화 조건 속성 ( 스펙 확장용 : 현재는 반드시 KD_NULL 값만 인정된다 )
	// 반환 : 쓰레드 동기화 조건 핸들
	//
	l_sync_cond = kdThreadCondCreate ( KD_NULL );	
	kdAssert ( l_sync_cond != 0 );

	kdLogMessagefKHR ( "kdThreadCondCreate         : cond = '0x%08x'", l_sync_cond );

	l_cond = kdThreadCondCreate ( KD_NULL );
	kdAssert ( l_cond != 0 );

	kdLogMessagefKHR ( "kdThreadCondCreate         : cond = '0x%08x'", l_cond );

	for ( i = 0; i < 2; i++ )
	{
		kdThreadMutexLock ( l_sync_mutex );
		
		thread[ i ] = kdThreadCreate ( 0, i == 0 ? xmCondPong : xmCondPing, 0 );

		kdThreadCondWait ( l_sync_cond, l_sync_mutex );
		kdThreadMutexUnlock ( l_sync_mutex );
	}

	for ( i = 0; i < 2; i++ )
	{
		kdThreadJoin ( thread[ i ], 0 );
	}
	
	kdThreadMutexFree ( l_sync_mutex );
	kdThreadMutexFree ( l_cond_mutex );

	//
	// 설명 : 쓰레드 동기화 조건 해제
	// 인자 : 쓰레드 동기화 조건 핸들
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	ret = kdThreadCondFree ( l_sync_cond );	
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadCondFree           : cond freed.");

	ret = kdThreadCondFree ( l_cond );
	kdAssert ( ret == 0 );
		
	kdLogMessagefKHR ( "kdThreadCondFree           : cond freed.");

}

#define XM_SEM_BUFFER_SIZE        3

static KDchar		l_sem_buffer[XM_SEM_BUFFER_SIZE];

static KDint        l_sem_in    = 0;
static KDint        l_sem_out   = 0;
static KDThreadSem*	l_sem_empty = 0;
static KDThreadSem*	l_sem_full  = 0;

static KDvoid* xmSetProducer ( KDvoid* arg )
{
	KDchar    item = 0;
	KDint     i;

	for ( i = 0; i < 7; i++ )
	{
		item = 'A' + i % 26;

		//
		// 설명 : 세마포어 대기
		// 인자 : 세마포어 핸들
		// 반환 : 성공 = 0, 실패 정의 없음
		//
		kdThreadSemWait ( l_sem_empty );
	
		l_sem_buffer[l_sem_in] = item;
		l_sem_in = ( l_sem_in + 1 ) % XM_SEM_BUFFER_SIZE;		

		kdLogMessagefKHR ( "Semaphore                  : producing = %c", item );
 
		if ( l_sem_in == 0 )
		{
			//
			// 설명 : 세마포어 전달
			// 인자 : 세마포어 핸들
			// 반환 : 성공 = 0, 실패 정의 없음
			//
			kdThreadSemPost ( l_sem_full );			
			xmWaitProcess ( 100 );
		}

		kdThreadSemPost ( l_sem_empty );
	}

	return 0;
}

static KDvoid* xmSemConsumer ( KDvoid* arg )
{
	KDchar     item = 0;
	KDint      i;

	for ( i = 0; i < 7; i++ )
	{
		kdThreadSemWait ( l_sem_full );

		item = l_sem_buffer[ l_sem_out ];
		l_sem_out = ( l_sem_out + 1 ) % XM_SEM_BUFFER_SIZE;

		kdLogMessagefKHR ( "Semaphore                  : consuming = %c", item );

		if ( l_sem_out == 0 )
		{
			xmWaitProcess ( 100 );
		}

		kdThreadSemPost ( l_sem_full );
	}

	return 0;
}

static KDvoid xmEx_ThreadSemaphore ( KDvoid )
{
	KDThread*    thread_producer = 0;
	KDThread*    thread_consumer = 0;
	KDint        ret;

	//
	// 설명 : 쓰레드 동기화 조건 생성
	// 인자 : 쓰레드 동기화 조건 속성 ( 스펙 확장용 : 현재는 반드시 KD_NULL 값만 인정된다 )
	// 반환 : 쓰레드 동기화 조건 핸들
	//
	l_sem_empty = kdThreadSemCreate ( 1 );
	kdAssert ( l_sem_empty != 0 );

	kdLogMessagefKHR ( "kdThreadSemCreate          : sem = '0x%08x'", l_sem_empty );

	l_sem_full  = kdThreadSemCreate( 0 );
	kdAssert ( l_sem_full != 0 );

	kdLogMessagefKHR ( "kdThreadSemCreate          : sem = '0x%08x'", l_sem_full );

	thread_producer = kdThreadCreate ( 0, xmSetProducer, 0 );
	thread_consumer = kdThreadCreate ( 0, xmSemConsumer, 0 );

	kdThreadJoin ( thread_producer, 0 );
	kdThreadJoin ( thread_consumer, 0 );

	//
	// 설명 : 세마포어 해제
	// 인자 : 세마포어 핸들
	// 반환 : 성공 = 0, 실패 정의 없음
	//
	ret = kdThreadSemFree ( l_sem_empty );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadSemFree            : semaphore freed.");

	ret = kdThreadSemFree ( l_sem_full );
	kdAssert ( ret == 0 );

	kdLogMessagefKHR ( "kdThreadSemFree            : semaphore freed.");
}

KDvoid xmExample_14 ( KDvoid )
{
	kdLogMessage ( "Example 14. Threads and synchronization\n\n" );

	xmEx_ThreadJoin      ( );
	xmEx_ThreadDetatch   ( );
	xmEx_ThreadExit      ( );
	xmEx_ThreadOnce      ( );	
#if !defined ( __APPLE__ )
	//xmEx_ThreadMutex     ( );		// iPhone Device 4.2 Example Error, not simulator
#endif
	xmEx_ThreadCond      ( );
#if !defined ( __APPLE__ )
	//xmEx_ThreadSemaphore ( );		// iPhone Device 4.2 Example Error, not simulator
#endif
	//xmEx_ThreadExit2     ( );
}
