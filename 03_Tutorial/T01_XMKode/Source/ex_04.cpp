/* --------------------------------------------------------------------------
 *
 *      File            ex_04.cpp
 *      Description     04. Assertions and logging
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

KDvoid xmExample_04 ( KDvoid )
{	
	kdLogMessage ( "Example 04. Assertions and logging\n\n" );

	// IDE에서 Release모드로 빌드할 경우, Case 3만 동작된다.
	
	//
	// Case 1 : kdLogMessage 
	//

	kdLogMessage ( "Case 1 : Log message is printed." );

	//
	// Case 2 : kdAssert 
	//

	// Condition이 KD_TRUE 면, 매크로 정의 구문이 공백으로 치환된다.
	// KD_FALSE이면 kdHandleAssertion 매크로 치환된다.
	kdAssert ( KD_TRUE );
	kdLogMessage ( "Case 2 : Assert condition is KD_TRUE." ); 

	//
	// Case 3 : kdHandleAssertion 
	//

	// 사용자 Assert 명령을 내린다. 
	//kdHandleAssertion ( "Case 3", __FILE__, __LINE__);
	
	// 다음 로그 메세지가 출력되면 사용자 Assert가 실패됨을 알 수 있다.
	//kdLogMessage ( "Case 3 : Application is not terminated by kdHandleAssertion" ); 
}
