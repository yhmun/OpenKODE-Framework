/* --------------------------------------------------------------------------
 *
 *      File            ex_08.cpp
 *      Description     08. Mathematical functions
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

#define XM_ERROR_TRESHOLD 0.001

static KDfloat32 xmGetRel ( KDfloat32 a, KDfloat32 b )
{
    KDfloat32    rv;

    if ( a > b )
    {
        rv = ( a - b ) / a;
    }
    else if ( b > a )
    {
        rv = ( b - a ) / b;
    }
    else
    {
        rv = 0.0f;
    }

    return rv;
}

KDvoid xmEx_kdAcosf ( KDvoid )
{
	// Radian 값을 입력 받아 Arc cosine을 구한다. range [0,PI]

	KDfloat32        x		= 3.4450960159301758e-001f;
	KDfloat32        expect = 1.2190799713134766e+000f;
	KDfloat32        retval = kdAcosf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdAcosf    : acos ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdAsinf ( KDvoid )
{	
	// Radian 값을 입력 받아 Arc sine을 구한다. range [-PI/2,PI/2]	

	KDfloat32        x		= 2.7052211761474609e-001f;
	KDfloat32        expect = 2.7393531799316406e-001f;
	KDfloat32        retval = kdAsinf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdAsinf    : asin ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdAtanf ( KDvoid )
{
	// Radian 값을 입력 받아 Arc tangent을 구한다. range [-PI/2,PI/2]

	KDfloat32        x      = 7.3333752155303955e-001f;
	KDfloat32        expect = 6.3275158405303955e-001f;
	KDfloat32        retval = kdAtanf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdAtanf    : atan ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdAtan2f ( KDvoid )
{
	//  y / x Radian 값을 입력 받아 Arc tangent을 구한다. range [-PI,PI]

	KDfloat32        x		= -2.2397613525390625e-001f;
	KDfloat32        y		= -4.0733156204223633e+000f;
	KDfloat32        expect = -1.6257271766662598e+000f;
	KDfloat32        retval = kdAtan2f ( y, x );
	KDfloat32        diff   = xmGetRel ( retval, expect );
	
	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdAtan2f   : atan2 ( %.9g, %.9g ) = %.9g", y, x, retval );
}

KDvoid xmEx_kdCosf ( KDvoid ) 
{
	// Radian 값을 입력 받아 Cosine을 구한다. range [-1,+1]

	KDfloat32        x      = -7.3246250152587891e+000f;
	KDfloat32        expect =  5.0497812032699585e-001f;
	KDfloat32        retval = kdCosf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdCosf     : cos ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdSinf ( KDvoid )
{
	// Radian 값을 입력 받아 Sine을 구한다. range [-1,+1]

	KDfloat32        x      = 2.6337766647338867e+001f;
	KDfloat32        expect = 9.3384832143783569e-001f;
	KDfloat32        retval = kdSinf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdSinf     : sin ( %.9g ) = %.9g", x, retval );	
}

KDvoid xmEx_kdTanf ( KDvoid )
{
	// Radian 값을 입력 받아 Tangent을 구한다. 

	KDfloat32        x      = -4.5864124298095703e+000f;
	KDfloat32        expect = -7.8959484100341797e+000f;
	KDfloat32        retval = kdTanf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdTanf     : tan ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdExpf ( KDvoid )
{	
	// 지수를 구한다. 

	KDfloat32        x      = 2.2853698730468750e+000f;
	KDfloat32        expect = 9.8293209075927734e+000f;
	KDfloat32        retval = kdExpf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdExpf     : exp ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdLogf ( KDvoid )
{
	// 자연 로그를 구한다. 

	KDfloat32        x      =  8.4225404428650563e-010f;
	KDfloat32        expect = -2.0894939422607422e+001f;
	KDfloat32        retval = kdLogf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdLogf     : log ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdFabsf ( KDvoid )
{
	// 실수 절대값을 구한다. 

	KDfloat32        x      = -3.5541311372071505e-003f;
	KDfloat32        expect =  3.5541311372071505e-003f;
	KDfloat32        retval = kdFabsf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdFabsf    : fabs ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdPowf(void)
{
	// 제곱을 구한다. 

	KDfloat32        x      =  5.4460234642028809e+000f;
	KDfloat32        y		= -1.8096313476562500e+000f;
	KDfloat32        expect =  4.6555120497941971e-002f;
	KDfloat32        retval = kdPowf ( x, y );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdPowf     : pow ( %.9g, %.9g ) = %.9g", x, y, retval );
}

KDvoid xmEx_kdSqrtf(void)
{
	// 제곱근을 구한다. 

	KDfloat32        x      = 2.5649807500000000e+006f;
	KDfloat32        expect = 1.6015557861328125e+003f;
	KDfloat32        retval = kdSqrtf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdSqrtf    : sqrt ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdCeilf(void)
{
	// 정수 오름을 구한다.

	KDfloat32        x      = -2.4439306640625000e+002f;
	KDfloat32        expect = -2.4400000000000000e+002f;
	KDfloat32        retval = kdCeilf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdCeilf    : ceil ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdFloorf ( KDvoid )
{
	// 정수 내림을 구한다.

	KDfloat32        x      = 6.4110289062500000e+004f;
	KDfloat32        expect = 6.4110000000000000e+004f;
	KDfloat32        retval = kdFloorf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdFloorf   : floor ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdRoundf ( KDvoid )
{
	// 정수 반올림을 구한다.

	KDfloat32        x      = 7.5617235898971558e-001f;
	KDfloat32        expect = 1.0000000000000000e+000f;
	KDfloat32        retval = kdRoundf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdRoundf   : round ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdInvsqrtf ( KDvoid )
{
	// 역제곱근을 구한다.
	
	KDfloat32        x      = 4.3778949355162448e+026f;
	KDfloat32        expect = 4.7793335993450911e-014f;
	KDfloat32        retval = kdInvsqrtf ( x );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdInsqrtf  : invsqrt ( %.9g ) = %.9g", x, retval );
}

KDvoid xmEx_kdFmodf ( KDvoid )
{
	// 실수 나머지를 구한다.

	KDfloat32        x      = 7.3997540950623830e-013f;
	KDfloat32        y 	    = 4.1420369196153162e-013f;
	KDfloat32        expect = 3.2577171754470668e-013f;
	KDfloat32        retval = kdFmodf ( x, y );
	KDfloat32        diff   = xmGetRel ( retval, expect );

	kdAssert ( diff <= XM_ERROR_TRESHOLD );
	kdLogMessagefKHR ( "kdFmodf    : fmod ( %.9g, %.9g ) = %.9g", x, y, retval );
}

KDvoid xmExample_08 ( KDvoid )
{	
	kdLogMessage ( "Example 08. Mathematical functions\n\n" );

	// 실수 오차 범위 비교를 사용하지 않았습니다. 참고 하시길 바랍니다.
	// Speacial Value 입력 값에 대한 예시는 보여 주지 않습니다. 
	// 스펙 문서를 참고 하시길 바랍니다.

	xmEx_kdAcosf    ( );
	xmEx_kdAsinf    ( );
	xmEx_kdAtanf    ( );
	xmEx_kdAtan2f   ( );
	xmEx_kdCosf     ( );
   	xmEx_kdSinf     ( );
	xmEx_kdTanf     ( );
	xmEx_kdExpf     ( );
	xmEx_kdLogf     ( );
	xmEx_kdFabsf    ( );
	xmEx_kdPowf     ( );
	xmEx_kdSqrtf    ( );
	xmEx_kdCeilf    ( );
	xmEx_kdFloorf   ( );
	xmEx_kdRoundf   ( );
	xmEx_kdInvsqrtf ( );
	xmEx_kdFmodf    ( );
}
