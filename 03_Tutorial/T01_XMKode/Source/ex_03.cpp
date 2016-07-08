/* --------------------------------------------------------------------------
 *
 *      File            ex_03.cpp
 *      Description     03. Errors
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

KDvoid xmExample_03 ( KDvoid )
{	
	KDint        error;

	kdLogMessage ( "Example 03. Errors\n\n" );

	//
	// Case 1 : kdSetError 
	//
	
	// 프로그램 초기에 셋팅된 에러값을 가져온다.
	error = kdGetError ( );
	kdLogMessagefKHR ( "Case 1-1 : Initialized error value returned. ( %d )", error );

	// 사용자 에러값을 직접 입력하여 본다.
	kdSetError ( 100 );
	error = kdGetError ( );

	// 사용자 입력 에러값이 정확한지 알아본다.
	if ( error == 100 )
	{
		kdLogMessage ( "Case 1-2 : Correct error value returned. ( 100 )" );
	}
	else
	{
		kdLogMessage ( "Case 1-2 : Incorrect error value returned." );
	}

	//
	// Case 2 : kdGetError
	//

	// 함수 사용에 오류를 발생하는 인자값을 넣어 본다.
	if ( kdQueryAttribcv ( 0 ) == KD_NULL )
	{
		// 함수 오류시 발생된 에러 발생값이 KD_EINVAL로 변경되었는지 확인한다.
		if ( kdGetError ( ) == KD_EINVAL )
		{
			kdLogMessage ( "Case 2   : Correct error value returned. ( KD_EINVAL )" );
		}
		else
		{
			kdLogMessage ( "Case 2   : Incorrect error value returned. " );
		}
	}	
}
