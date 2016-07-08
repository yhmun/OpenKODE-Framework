/* --------------------------------------------------------------------------
 *
 *      File            ex_18.cpp
 *      Description     18. Input output
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

KDvoid xmExample_18 ( KDvoid )
{		
	KDint64      state = 0;
		
	kdLogMessage ( "Example 18. Input output\n\n" );
	
	//
	// 설명 : 입력 장치 가용성 확인
	// 인자 : startidx, numidxs, buffer ( Limit : 첫번째 인자 xxx_AVAILABILITY 입력만 허용, 두번째 인자는 무시됨 )
	// 반환 : 성공 = 0, 실패 = -1
	// 참고 : 함수 Limit가 있습니다. 단지 전체 가용성만 확인 가능합니다.
	//		  kdStateGeti, kdStateGetl, kdStateGetf 구현 동일 하지만 자료형만 다릅니다.
	//		  kdOutputSeti, kdOutputSetf 출력 장치는 지원 하지 않습니다.
	//
	kdStateGetl ( KD_STATE_GAMEKEYS_AVAILABILITY    , 1, &state );
	kdLogMessagefKHR ( "kdStateGetl : state = %d", state );

	kdStateGetl ( KD_STATE_GAMEKEYSNC_AVAILABILITY	, 1, &state );
	kdLogMessagefKHR ( "kdStateGetl : state = %d", state );

	kdStateGetl ( KD_STATE_PHONEKEYPAD_AVAILABILITY	, 1, &state );
	kdLogMessagefKHR ( "kdStateGetl : state = 0x%04x", state );

	kdStateGetl ( KD_STATE_POINTER_AVAILABILITY		, 1, &state );	
	kdLogMessagefKHR ( "kdStateGetl : state = %d", state );

	kdLogMessage ( "Edited for Window API to XMSoft's SPECs ( Only one Window, Application Entrance )" );
}
