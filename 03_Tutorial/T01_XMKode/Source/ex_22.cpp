/* --------------------------------------------------------------------------
 *
 *      File            ex_22.cpp
 *      Description     22. KD_KHR_thread_storage
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
	KDint		index;
	KDchar		name[256];
} XMData;

#define KEY_ID		1000

KDvoid xmEx_SetTS ( KDvoid )
{
	KDThreadStorageKeyKHR   key;
	KDint                   ret;
	XMData*                 data;

	data = (XMData *) kdMalloc ( sizeof ( XMData ) );
	data->index = 100;
	kdStrcpy_s ( data->name, 256, (KDchar *) "Thread Storage Data" );

	//
	// 설명 : 고유 아이디를 가지고 쓰레드 스토리지 키 생성 및 맵핑
	// 인자 : 고유 아이디
	// 반환 : 키
	//
	key = kdMapThreadStorageKHR ( (KDvoid *) KEY_ID );
	kdLogMessagefKHR ( "kdMapThreadStorageKHR : key = %d", key );

	//
	// 설명 : 쓰레드 스토리지에 데이타를 저장한다.
	// 인자 : 키, 데이타
	// 반환 : 성공 = 0, 에러코드
	//
	ret = kdSetThreadStorageKHR ( key, data );

	kdAssert ( ret == 0 );
	kdLogMessagefKHR ( "kdSetThreadStorageKHR : index = %d, name = '%s'", data->index, data->name );
}

KDvoid xmEx_GetTS ( KDvoid )
{
	KDThreadStorageKeyKHR   key;
	XMData*                 data;

	key = kdMapThreadStorageKHR ( (KDvoid *) KEY_ID );

	//
	// 설명 : 쓰레드 스토리지에 데이타를 가져온다.
	// 인자 : 키
	// 반환 : 데이타
	//
	data = (XMData *) kdGetThreadStorageKHR ( key );

	kdAssert ( data );
	kdLogMessagefKHR ( "kdGetThreadStorageKHR : index = %d, name = '%s'", data->index, data->name );

	kdFree ( data );
	kdSetThreadStorageKHR ( key, KD_NULL );
}

KDvoid xmExample_22 ( KDvoid )
{	
	kdLogMessage ( "Example 22. KD_KHR_thread_storage\n\n" );

	xmEx_SetTS ( );
	xmEx_GetTS ( );	
}