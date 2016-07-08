/* --------------------------------------------------------------------------
 *
 *      File            ex_05.cpp
 *      Description     05. Memory allocation
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

KDvoid xmExample_05 ( KDvoid )
{	
	const KDsize size[3] = { 1000, 100, 10000 };

	KDvoid*	    buf;
	KDint       i;
	
	kdLogMessage ( "Example 05. Memory allocation\n\n" );

	//
	// Case 1 : kdMalloc 
	//

	// 메모리 블럭을 할당한다.
	buf = kdMalloc ( size[ 0 ] );

	// 메모리 블럭 할당이 정상적인지 확인한다.
	if ( buf != KD_NULL )
	{
		kdLogMessage ( "Case 1 : Memory block is allocated." );
	}
	else
	{
		// 메모리 블럭 할당 문제가 무엇인지 확인한다.
		if ( kdGetError ( ) == KD_ENOMEM )
		{
			kdLogMessage ( "Case 1 : Not enough space." );
		}
		else
		{
			kdHandleAssertion ( "kdMalloc", __FILE__, __LINE__ );
		}
	}

	//
	// Case 2 : kdRealloc 
	//

	// 할당된 메모리 블럭 사이즈를 줄여다가 다시 크게한다.
	for ( i = 1; i < 3; i++ )
	{
		// 메모리 블럭을 리사이즈합니다.
		buf = kdRealloc ( buf, size[i] );

		// 메모리 블럭 리사이즈가 정상인지 확인한다.
		if ( buf != KD_NULL )
		{
			kdLogMessage ( "Case 2 : Memory block is resized." );
		}
		else
		{
			// 메모리 블럭 리사이즈 문제가 무엇인지 확인한다.
			if ( kdGetError ( ) == KD_ENOMEM )
			{
				kdLogMessage ( "Case 2 : Not enough space." );
			}
			else
			{
				kdHandleAssertion ( "kdRealloc", __FILE__, __LINE__);
			}
		}
	}

	//
	// Case 3 : kdFree 
	//

	// 메모리 블럭 해제한다.
	kdFree ( buf );
	kdLogMessage ( "Case 3 : Allocated memory block is freed." );
}
