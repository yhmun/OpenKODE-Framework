/* --------------------------------------------------------------------------
 *
 *      File            ex_13.cpp
 *      Description     13. Timer functions
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

#define	XM_ONESHOT                 1000	 
#define XM_PERIODIC_AVERAGE        2000
#define XM_PERIODIC_MINIMUM        3000

KDvoid xmExample_13 ( KDvoid )
{	
	KDTimer*       timer[3];
	KDEvent*       event;
	//KDEvent*     quit;
	KDust          ust;
	KDtime         time;
	KDTm           tm;
	KDint          cnt;
	KDint          ret;

	kdLogMessage ( "Example 13. Timer functions\n\n" );

	cnt = 0;
	ust = kdGetTimeUST ( );
	
	//
	// 설명 : 타이머 설정
	// 인자 : 시간 간격 ( nano second 단위 )
	//		  반복 주기 ( KD_TIMER_ONESHOT, KD_TIMER_PERIODIC_AVERAGE, KD_TIMER_PERIODIC_MINIMUM )
	//		  사용자 정보
	// 반환 : 타이머 주소
	//

	timer[ 0 ] = kdSetTimer ( 1LL, KD_TIMER_ONESHOT, (KDvoid *) XM_ONESHOT );
	kdAssert ( ( timer[ 0 ] ) );

	time = kdTime ( 0 );
	kdLocaltime_r ( &time, &tm );

	kdLogMessagefKHR ( "kdSetTimer      : 0x%08x", &timer[ 0 ] );
	kdLogMessagefKHR ( "Current Time    : %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec );

	timer[ 1 ] = kdSetTimer (          1LL, KD_TIMER_PERIODIC_AVERAGE, (KDvoid *) XM_PERIODIC_AVERAGE );
	timer[ 2 ] = kdSetTimer ( 3000000000LL, KD_TIMER_PERIODIC_MINIMUM, (KDvoid *) XM_PERIODIC_MINIMUM );

	while ( ( event = (KDEvent *) kdWaitEvent ( -1 ) ) )
	{
		if ( event->type == KD_EVENT_TIMER )
		{
			kdLogMessagefKHR ( "Reached message : %d, %u us", event->userptr, ( kdGetTimeUST ( ) - ust ) / 1000 );
			
			switch( (KDint) event->userptr )
			{				
				case XM_ONESHOT :

					//
					//  설명 : 타이머 해제
					//  인자 : 타이머 주소
					//  반환 : 성공 = 0, 실패 = -1
					//
					ret = kdCancelTimer ( timer[0] );
					kdAssert ( !ret );	

					kdLogMessagefKHR ( "kdCancelTimer   : 0x%08x", &timer[ 0 ] );
					break;

				case XM_PERIODIC_AVERAGE :

					if ( cnt == 5 )
					{
						kdCancelTimer ( timer[ 1 ] );
					}
					
					cnt++;
					break;

				case XM_PERIODIC_MINIMUM :

					time = kdTime ( 0 );
					kdLocaltime_r ( &time, &tm );
					kdLogMessagefKHR ( "Current Time    : %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec );

					if ( kdGetTimeUST ( ) - ust > 10000000000LL )
					{
						kdCancelTimer ( timer[ 2 ] );						

						//quit = kdCreateEvent ( );
						//quit->type = KD_EVENT_QUIT;
						//kdPostEvent ( quit );
						
						return;
					}

					break;
			}
		}
		
		kdDefaultEvent ( event );
	}
}
