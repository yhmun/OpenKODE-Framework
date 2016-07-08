/* --------------------------------------------------------------------------
 *
 *      File            ex_20.cpp
 *      Description     20. KD_KHR_float64
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

static KDint xmIsEqualDouble ( KDfloat64KHR v1, KDfloat64KHR v2 )
{
	KDchar    s1[KD_DTOSTR_MAXLEN_KHR];
	KDchar    s2[KD_DTOSTR_MAXLEN_KHR];

	kdSprintfKHR ( s1, "%0.18g", v1 );
	kdSprintfKHR ( s2, "%0.18g", v2 );

	return kdStrncmp ( s1, s2, 16 ) ? 0 : 1;
}

KDvoid EX_kdAcosKHR ( KDvoid )
{
	KDfloat64KHR    r = 0;
	KDfloat64KHR    x = 9.99390827019095760e-01;

	// Radian 값을 입력 받아 Arc cosine을 구한다. 
	r = kdAcosKHR ( x );
	
	kdAssert ( xmIsEqualDouble ( r, 3.49065850398856680e-02 ) );
	kdLogMessagefKHR ( "kdAcosKHR    : acos ( %.18g ) = %.18g", x, r );
}

void EX_kdAsinKHR ( KDvoid )
{
	KDfloat64KHR    r = 0;
	KDfloat64KHR    x = 3.48994967025008230e-02;

	// Radian 값을 입력 받아 Arc sine을 구한다. 
	r = kdAsinKHR ( x );
	
	kdAssert ( xmIsEqualDouble ( r, 3.49065850398864450e-02 ) );
	kdLogMessagefKHR ( "kdAsinKHR    : asin ( %.18g ) = %.18g", x, r );
}

void EX_kdAtanKHR ( KDvoid )
{
	KDfloat64KHR    r = 0;
	KDfloat64KHR    x = 3.49207694917475770e-02;

	// Radian 값을 입력 받아 Arc tangent을 구한다. 
	r = kdAtanKHR( x );
	
	kdAssert ( xmIsEqualDouble ( r, 3.49065850398864380e-02 ) );
	kdLogMessagefKHR ( "kdAtanKHR    : atan ( %.18g ) = %.18g", x, r );
}

void EX_kdAtan2KHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = -7.35212670560393110e+286;
	KDfloat64KHR	y = 2.35582254963331620e+129;

	//  y / x Radian 값을 입력 받아 Arc tangent을 구한다. 
	r = kdAtan2KHR ( y, x );

	kdAssert ( xmIsEqualDouble ( r, 3.14159265358979310e+00 ) );
	kdLogMessagefKHR ( "kdAtan2KHR   : atan2 ( %.18g, %.18g ) = %.18g", y, x, r );
}

void EX_kdCosKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = -6.24827872213970000e+00;

	// Radian 값을 입력 받아 Cosine을 구한다.
	r = kdCosKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 9.99390827019095760e-01 ) );
	kdLogMessagefKHR ( "kdCosKHR     : cos ( %.18g ) = %.18g", x, r );
}

void EX_kdSinKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = -6.24827872213970000e+00;

	// Radian 값을 입력 받아 Sine을 구한다. 
	r = kdSinKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 3.48994967025008230e-02 ) );
	kdLogMessagefKHR ( "kdSinKHR     : sin ( %.18g ) = %.18g", x, r );	
}

void EX_kdTanKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = -6.24827872213970000e+00;

	// Radian 값을 입력 받아 Tangent을 구한다. 
	r = kdTanKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 3.49207694917475770e-02 ) );
	kdLogMessagefKHR ( "kdTanKHR     : tan ( %.18g ) = %.18g", x, r );
}

void EX_kdExpKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 6.27760367133665260e-03;

	// 지수를 구한다. 
	r = kdExpKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 1.00629734912167710e+00 ) );
	kdLogMessagefKHR ( "kdExpKHR     : exp ( %.18g ) = %.18g", x, r );
}

void EX_kdLogKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 3.83953083946810470e-299;

	// 자연 로그를 구한다. 
	r = kdLogKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, -6.87127592623306100e+02 ) );
	kdLogMessagefKHR ( "kdLogKHR     : log ( %.18g ) = %.18g", x, r );
}

void EX_kdFabsKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 3.83953083946810470e-299;

	// 실수 절대값을 구한다. 
	r = kdFabsKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 3.83953083946810470e-299 ) );
	kdLogMessagefKHR ( "kdFabsKHR    : fabs ( %.18g ) = %.18g", x, r );
}

void EX_kdPowKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 2.12302835763627940e+262;
	KDfloat64KHR	y = 2.32262292350669250e-01;

	// 제곱을 구한다. 
	r = kdPowKHR ( x, y );

	kdAssert ( r >= 8.48516121857668000e+60 && r <= 8.48516121857669999e+60 );		// 8.48516121857668770e+60
	kdLogMessagefKHR ( "kdPowKHR     : pow ( %.18g, %.18g ) = %.18g", x, y, r );
}

void EX_kdSqrtKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 3.83953083946810470e-299;

	// 제곱근을 구한다. 
	r = kdSqrtKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 6.19639479009214230e-150 ) );
	kdLogMessagefKHR ( "kdSqrtKHR    : sqrt ( %.18g ) = %.18g", x, r );
}

void EX_kdCeilKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 6.42826615944873230e+00;

	// 정수 오름을 구한다.
	r = kdCeilKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 7.00000000000000000e+00 ) );
	kdLogMessagefKHR ( "kdCeilKHR    : ceil ( %.18g ) = %.18g", x, r );
}

void EX_kdFloorKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 6.42826615944873230e+00;

	// 정수 내림을 구한다.
	r = kdFloorKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 6.00000000000000000e+00 ) );
	kdLogMessagefKHR ( "kdFloorKHR   : floor ( %.18g ) = %.18g", x, r );
}

void EX_kdRoundKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 1.96175114729270390e-04;

	// 정수 반올림을 구한다.
	r = kdRoundKHR ( x );

	kdAssert ( xmIsEqualDouble ( r, 0.00000000000000000e+00 ) );
	kdLogMessagefKHR ( "kdRoundKHR   : round ( %.18g ) = %.18g", x, r );
}

void EX_kdInvsqrtKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = 1.82702312925150980e-13;

	// 역제곱근을 구한다.
	r = kdInvsqrtKHR( x );

	kdAssert ( xmIsEqualDouble ( r, 2.33952654568647310e+06 ) );
	kdLogMessagefKHR ( "kdInsqrtKHR  : invsqrt ( %.18g ) = %.18g", x, r );
}

void EX_kdFmodKHR ( KDvoid )
{
	KDfloat64KHR	r = 0;
	KDfloat64KHR	x = -1.88199041020884560e+43;
	KDfloat64KHR	y = 4.02719003854292400e+164;

	// 실수 나머지를 구한다.
	r = kdFmodKHR ( x, y );

	kdAssert ( xmIsEqualDouble ( r, -1.88199041020884560e+43 ) );
	kdLogMessagefKHR ( "kdFmodKHR    : fmod ( %.18g, %.18g ) = %.18g", x, y, r );
}

void EX_kdStrtodKHR ( KDvoid )
{
	KDfloat64KHR	out  = 0;
	KDchar			in[] = "3.839530839468104729e-299";

	// 문자열을 실수형으로 변환
	out = kdStrtodKHR ( in, KD_NULL );
	
	kdAssert ( xmIsEqualDouble ( out, 3.83953083946810470e-299 ) );
	kdLogMessagefKHR ( "kdStrtodKHR  : in = '%s', out = %.18g", in, out );
}

void EX_kdDtostrKHR ( KDvoid )
{	
	KDchar			out[KD_DTOSTR_MAXLEN_KHR] = "";
	KDchar			r[KD_DTOSTR_MAXLEN_KHR] = "";
	KDsize			len = 0;
	KDfloat64KHR	in  = 3.83953083946810470e-299;

	// 실수를 문자열로 변환
	len = kdDtostrKHR ( out, KD_DTOSTR_MAXLEN_KHR, in );

	kdSprintfKHR ( r, "%.18g", 3.839530839468104729e-299 );
	kdAssert ( len != (KDsize) -1 && !kdStrncmp( out, r, 17 ) );
	kdLogMessagefKHR ( "kdDtostrKHR  : in = %.18g, out = '%s'", in, out );
}

KDvoid xmExample_20 ( KDvoid )
{	
	kdLogMessage ( "Example 20. KD_KHR_float64\n\n" );

	// 실수 오차 범위 비교를 사용하지 않았습니다. 참고 하시길 바랍니다.
	// Speacial Value 입력 값에 대한 예시는 보여 주지 않습니다. 
	// 스펙 문서를 참고 하시길 바랍니다.

	EX_kdAcosKHR    ( );
	EX_kdAsinKHR    ( );
	EX_kdAtanKHR    ( );
	EX_kdAtan2KHR   ( );
	EX_kdCosKHR     ( );
   	EX_kdSinKHR     ( );
	EX_kdTanKHR     ( );
	EX_kdExpKHR     ( );
	EX_kdLogKHR     ( );
	EX_kdFabsKHR    ( );
	EX_kdPowKHR     ( );
	EX_kdSqrtKHR    ( );
	EX_kdCeilKHR    ( );
	EX_kdFloorKHR   ( );
	EX_kdRoundKHR   ( );
	EX_kdInvsqrtKHR ( );
	EX_kdFmodKHR    ( );

	EX_kdStrtodKHR  ( );
	EX_kdDtostrKHR  ( );
}