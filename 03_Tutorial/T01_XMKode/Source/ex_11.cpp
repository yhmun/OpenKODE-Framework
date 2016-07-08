/* --------------------------------------------------------------------------
 *
 *      File            ex_11.cpp
 *      Description     11. Time functions
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

KDvoid xmExample_11 ( KDvoid )
{	
	KDust        ust;
	KDtime       time;	
	KDTm         tm;
	
	kdLogMessage ( "Example 11. Time functions\n\n" );

	//
	//	설명 : Unadjust System Time 가져오기 (시스템이 켜졌을 때부터 카운트 된값)
	//  반환 : 64비트 정수
	//
	ust = kdGetTimeUST ( );

	kdLogMessagefKHR ( "kdGetTimeUST  : %u ns", ust );

	//
	// 설명 : 현재 시간을 구한다. ( Epoch 이후 )
	// 인자 : 저장될 시간
	// 반환 : 현재 시간
	//
	time = kdTime ( KD_NULL );
	kdTime ( &time );

	kdLogMessagefKHR ( "kdTime        : %u", time );
	//
	// 설명 : 64비트 정수형 시간을 시간 구조체로 변환한다.
	// 인자 : 입력 시간, 반환될 시간 구조체
	// 반환 : 반환된 시간 구조체 주소
	//
	kdGmtime_r ( &time, &tm );

	kdLogMessagefKHR ( "kdGmtime_r    : %04d/%02d/%02d %02d:%02d:%02d", 
		1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );

	//
	// 설명 : 64비트 정수형 시간을 지역 시간 구조체로 변환한다.
	// 인자 : 입력 시간, 반환될 시간 구조체
	// 반환 : 반환된 시간 구조체 주소
	//
	kdLocaltime_r ( &time, &tm );		

	kdLogMessagefKHR ( "kdLocaltime_r : %04d/%02d/%02d %02d:%02d:%02d", 
		1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );

	// 
	// 설명 : Epoch(KDTime = 0)에 대응 되는 UST 값을 가져온다.
	// 반환 : 64비트 정수
	// 
	ust = kdUSTAtEpoch();	
	kdLogMessagefKHR ( "kdUSTAtEpoch  : %u ns", ust );
}
