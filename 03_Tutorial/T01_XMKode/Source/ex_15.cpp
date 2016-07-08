/* --------------------------------------------------------------------------
 *
 *      File            ex_15.cpp
 *      Description     15. Thread-local storage
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
	KDchar        str[256];
	KDint         val;
} XMData;

KDvoid xmEx_SetTLS ( KDvoid )
{
	XMData*    data = 0;

	data = (XMData *) kdMalloc ( sizeof ( XMData ) );

	kdStrcpy_s ( data->str, 256, "hello" );
	data->val = 100;

	// 
	// 설명 : 쓰레드 로컬 스토리지 데이타 저장
	//
	kdSetTLS ( data );

	kdLogMessagefKHR ( "kdSetTLS : str = '%s', val = %d", data->str, data->val );
}

KDvoid xmEx_GetTLS ( KDvoid )
{
	XMData*    data = 0;

	//
	// 설명 : 쓰레드 로컬 스토리지 데이타 가져오기
	// 반환 : 쓰레드 로컬 스토리지 데이타
	//
	data = (XMData *) kdGetTLS ( );

	kdLogMessagefKHR ( "kdGetTLS : str = '%s', val = %d", data->str, data->val );
}

KDvoid xmExample_15 ( KDvoid )
{
	kdLogMessage ( "Example 15. Thread-local storage\n\n" );

	xmEx_SetTLS ( );
	xmEx_GetTLS ( );
}