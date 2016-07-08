/* --------------------------------------------------------------------------
 *
 *      File            ex_06.cpp
 *      Description     06. String and memory functions
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

KDvoid xmEx_kdMemchr ( KDvoid )
{
	KDchar*    pch   = 0;
	KDchar     str[] = "Hello";

	// 주어진 바이트값 (0~255)을 메모리 공간에서 찾아 주소값을 반환한다.
	pch = (KDchar *) kdMemchr ( str, 'l', kdStrlen ( str ) );

	kdAssert ( pch != KD_NULL );
	kdLogMessagefKHR ( "kdMemchr    : 'l' found at position %d.", pch - str + 1 );
}

KDvoid xmEx_kdMemcmp ( KDvoid )
{
	KDchar     str1[] = "Hi";
	KDchar     str2[] = "Hello";
	KDsize     len1	  = kdStrlen ( str1 );
	KDsize     len2	  = kdStrlen ( str2 );
	KDint      retval = 0;

	// 주어진 메모리 공간만큼 처음부터 KDuint8값 비교를 한다. 
	// 반환값이 양수인 경우 : 소스 1 > 소스 2 
	// 반환값이 음수인 경우 : 소스 1 < 소스 2
	// 반환값이    0인 경우 : 소스 1 = 소스 2
	retval = kdMemcmp ( str1, str2, ( len1 > len2 ) ? len1 : len2 );

	kdAssert ( retval > 0 );
	kdLogMessagefKHR ( "kdMemcmp    : '%s' is greater than '%s'.", str1, str2 );
}

KDvoid xmEx_kdMemcpy ( KDvoid )
{
	KDchar     str1[]   = "Hello world";
	KDchar     str2[20] = "";

	// 주어진 메모리 공간만큼 소스 2에 소스 1을 복사한다.
	kdMemcpy ( str2, str1, 5 );

	kdAssert ( kdMemcmp ( str2, "Hello", 5 ) == 0 );
	kdLogMessagefKHR ( "kdMemcpy    : '%s' correctly copied.", str2 );
}

KDvoid xmEx_kdMemmove ( KDvoid )
{
	KDchar     str[] = "Hello world .....";
	 
	// 오버랩이 허용된 메모리 복사를 한다. (kdMemcpy는 오버랩이 허용되지 않는다.)
	// 'world' 부분이 소스와 대상 주소의 오버랩 구간이다.
	kdMemmove ( str + 6, str, 11 );

	kdAssert ( kdMemcmp ( str, "Hello Hello world", 17 ) == 0 );
	kdLogMessagefKHR ( "kdMemmove   : '%s' correctly overlapped.", str );
}

KDvoid xmEx_kdMemset ( KDvoid )
{
	KDchar     str[] = "Hello world";
	 
	// 오버랩이 허용된 메모리 복사를 한다. (kdMemcpy는 오버랩이 허용되지 않는다.)
	// 'world' 부분이 소스와 대상 주소의 오버랩 구간이다.
	kdMemset ( str + 6, '.', 5 );

	kdAssert ( kdMemcmp ( str, "Hello .....", 11 ) == 0 );
	kdLogMessagefKHR ( "kdMemset    : '%s' correctly set bytes.", str );
}

KDvoid xmEx_kdStrchr ( KDvoid )
{
	KDchar*    pch   = 0;
	KDchar     str[] = "Hello";

	// 주어진 바이트값 (0~255)을 문자열에서 찾아 주소값을 반환한다.
	pch = kdStrchr ( str, 'e' );

	kdAssert ( pch );
	kdLogMessagefKHR ( "kdStrchr    : 'e' found at position %d.", pch - str + 1 );
}

KDvoid xmEx_kdStrcmp ( KDvoid )
{
	KDchar     str1[] = "Hello";
	KDchar     str2[] = "Hi";
	KDint      retval = 0;

	// 주어진 문자열에서 차례대로 KDuint8 문자값 비교를 한다. 
	// 반환값이 양수인 경우 : 문자열 1 > 문자열 2 
	// 반환값이 음수인 경우 : 문자열 1 < 문자열 2
	// 반환값이    0인 경우 : 문자열 1 = 문자열 2
	retval = kdStrcmp ( str1, str2 );

	kdAssert ( retval < 0 );
	kdLogMessagefKHR ( "kdStrcmp    : '%s' is smaller than '%s'.", str1, str2 );
}

KDvoid xmEx_kdStrlen ( KDvoid )
{
	KDchar     str[]  = "Hello";
	KDsize     retval = 0;
	
	// 문자열의 길이를 구한다. 단, 최대 사이즈가 지정된다.
	retval = kdStrlen ( str );

	kdAssert ( retval == 5 );
	kdLogMessagefKHR ( "kdStrlen    : '%s' string length is %d.", str, retval );
}

KDvoid xmEx_kdStrnlen ( KDvoid ) 
{
	KDchar     str[]  = "Hello world";
	KDsize     maxlen = 5;
	KDsize     retval = 0;
	
	// 문자열의 길이를 구한다.
	retval = kdStrnlen ( str, maxlen );

	kdAssert ( retval == 5 );
	kdLogMessagefKHR ( "kdStrnlen   : '%s' string length is %d. max length is %d", str, retval, maxlen );
}

KDvoid xmEx_kdStrncat_s ( KDvoid )
{
	KDchar     str[20] = "";

	// 두 문자열을 서로 결합한다.
	// 대상 문자열의 최대 사이즈를 정해야 한다.
	// 소스 문자열의 최대 사이즈를 정해야 한다. 
	// 두 최대 사이즈는 Null terminated 문자열에 대한 오버런 체크에 있다.
	kdStrncat_s ( str, sizeof ( str ), "Hello World", 5 );
	kdStrncat_s ( str, sizeof ( str ), " World...", 6 );

	kdAssert ( kdStrcmp ( str, "Hello World" ) == 0 );
	kdLogMessagefKHR ( "kdStrncat_s : '%s' correctly concatenated.", str );
}

KDvoid xmEx_kdStrncmp ( KDvoid )
{
	KDchar     str1[] = "Hello world";
	KDchar     str2[] = "Hello";
	KDint      retval = 0;

	// 주어진 두 문자열에서 차례대로 KDuint8 문자값 비교를 한다. 
	// 반환값이 양수인 경우 : 문자열 1 > 문자열 2 
	// 반환값이 음수인 경우 : 문자열 1 < 문자열 2
	// 반환값이    0인 경우 : 문자열 1 = 문자열 2
	// 비교 문자열의 최대 사이즈를 정해야 한다.
	retval = kdStrncmp ( str1, str2, 5 );

	kdAssert ( retval == 0 );
	kdLogMessagefKHR ( "kdStrncmp   : '%s' is same length as '%s'. compare max length is %d", str1, str2, 5 );
}

KDvoid xmEx_kdStrcpy_s ( KDvoid )
{
	KDchar     str1[]   = "Hello";
	KDchar     str2[20] = "";

	// 문자열 복사를 한다.
	// 대상 문자열의 최대 사이즈보다 소스 크기가 작아야 한다.
	// PS. 스펙 문서가 잘못된거 같다. kdStrncpy 함수명이 맞는거 같다. _s는 소스 최대 사이즈를 의미 하는데.
	kdStrcpy_s ( str2, 11, str1 );

	kdAssert ( kdStrcmp ( str2, "Hello" ) == 0 );
	kdLogMessagefKHR ( "kdStrcpy_s  : '%s' correctly copied.", str2 );
}

KDvoid xmEx_kdStrncpy_s ( KDvoid )
{
	KDchar     str1[]   = "Hello";
	KDchar     str2[20] = "";

	// 문자열 복사를 한다.
	// 대상 문자열의 최대 사이즈를 정해야 한다.
	// 소스 문자열의 최대 사이즈를 정해야 한다. 
	// 두 최대 사이즈는 Null terminated 문자열에 대한 오버런 체크에 있다.
	kdStrncpy_s ( str2, 11, str1, 5 );

	kdAssert ( kdStrcmp ( str2, "Hello" ) == 0 );
	kdLogMessagefKHR ( "kdStrncpy_s : '%s' correctly copied.", str2 );
}

KDvoid xmExample_06 ( KDvoid )
{	
	kdLogMessage ( "Example 06. String and memory functions\n\n" );

	xmEx_kdMemchr  ( );
	xmEx_kdMemcmp  ( );
	xmEx_kdMemcpy  ( );
	xmEx_kdMemmove ( );
	xmEx_kdMemset  ( );

	xmEx_kdStrchr    ( );
	xmEx_kdStrcmp    ( );
	xmEx_kdStrlen    ( );
	xmEx_kdStrnlen   ( );
	xmEx_kdStrncat_s ( );
	xmEx_kdStrncmp   ( );
	xmEx_kdStrcpy_s  ( );
	xmEx_kdStrncpy_s ( );
}
