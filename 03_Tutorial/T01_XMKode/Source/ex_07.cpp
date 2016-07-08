/* --------------------------------------------------------------------------
 *
 *      File            ex_07.cpp
 *      Description     07. Utility library functions
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

#define XM_IS_EQUAL_FLOAT(F1, F2)        ( ( ( F1 - F2 ) <= ( 0.000001f * F1 ) ) && ( ( F1 - F2 ) >= -( 0.000001f * F2 ) ) )

KDvoid xmEx_kdAbs ( KDvoid )
{	
	KDint        out =      0;
	KDint        in1 =  12345;
	KDint        in2 = -67890;

	// 양의 정수의 절대값을 구한다.
	out = kdAbs ( in1 );

	kdAssert ( out == 12345 );
	kdLogMessagefKHR ( "kdAbs     : in = %d   , out = %d", in1, out );

	// 음의 정수의 절대값을 구한다.
	out = kdAbs( in2 );

	kdAssert ( out == 67890 );
	kdLogMessagefKHR ( "kdAbs     : in = %d  , out = %d", in2, out );
}

KDvoid xmEx_kdStrtof ( KDvoid )
{
	KDfloat32    out   = 0;
	KDchar       in1[] = "3.14159f";
	KDchar       in2[] = "5.9e-39f";

	// 일반 실수형 문자열을 실수형으로 변환
	out = kdStrtof ( in1, KD_NULL );

	kdAssert ( XM_IS_EQUAL_FLOAT ( out, 3.14159f ) );
	kdLogMessagefKHR ( "kdStrtof  : in = %s, out = %.9g", in1, out );

	// 지수 실수형 문자열을 실수형으로 변환
	out = kdStrtof ( in2, KD_NULL );
	
	kdAssert ( out == 5.9e-39f );	
	kdLogMessagefKHR ( "kdStrtof  : in = %s, out = %.9g", in2, out );
}

KDvoid xmEx_kdStrtol ( KDvoid )
{
	KDint        out   = 0;
	KDchar       in1[] = "-12345";
	KDchar       in2[] = "-10000";
	KDchar       in3[] = "-0xff";

	// 10진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환 (Base : 0 or 10)
	out = kdStrtol ( in1, KD_NULL, 0 );
	
	kdAssert ( out == -12345 );
	kdLogMessagefKHR ( "kdStrtol  : in = %s  , out = %d", in1, out );

	// 8진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환
	out = kdStrtol( in2, KD_NULL, 8 );
	
	kdAssert ( out == -4096 );	
	kdLogMessagefKHR ( "kdStrtol  : in = %s  , out = %d", in2, out );

	// 16진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환
	out = kdStrtol ( in3, KD_NULL, 16 );
	
	kdAssert ( out == -0xff );	
	kdLogMessagefKHR ( "kdStrtol  : in = %s   , out = %d", in3, out );
}

KDvoid xmEx_kdStrtoul ( KDvoid )
{
	KDuint       out   = 0;
	KDchar       in1[] = "12345";
	KDchar       in2[] = "10000";
	KDchar       in3[] = "0xff";

	// 10진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환 (Base : 0 or 10)
	out = kdStrtoul ( in1, KD_NULL, 0 );
	
	kdAssert ( out == 12345 );
	kdLogMessagefKHR ( "kdStrtoul : in = '%s' , out = '%u'", in1, out );

	// 8진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환
	out = kdStrtoul ( in2, KD_NULL, 8 );
	
	kdAssert ( out == 4096 );	
	kdLogMessagefKHR ( "kdStrtoul : in = '%s' , out = '%u'", in2, out );

	// 16진수 정수형 문자열(signed)을 정수형(KDuint)으로 변환
	out = kdStrtoul ( in3, KD_NULL, 16 );
	
	kdAssert ( out == 0xff );	
	kdLogMessagefKHR ( "kdStrtoul : in = '%s'  , out = '%u'", in3, out );
}

KDvoid xmEx_kdLtostr ( KDvoid )
{	
	KDchar       out[KD_LTOSTR_MAXLEN] = "";
	KDsize       len = 0;
	KDint        in  = -12345;

	// 정수형(KDint)을 10진수 문자열(signed)로 변환
	len = kdLtostr( out, KD_LTOSTR_MAXLEN, in );

	kdAssert ( len != (KDsize) -1 && !kdStrcmp( out, "-12345" ) );
	kdLogMessagefKHR ( "kdLtostr  : in = '%d', out = '%s'", in, out );
}

KDvoid xmEx_kdUltostr ( KDvoid )
{
	KDchar       out[KD_ULTOSTR_MAXLEN] = "";
	KDsize       len = 0;
	KDuint       in1 = 12345;
	KDuint       in2 = 4096;
	KDuint       in3 = 0xff;
	
	// 정수형(KDuint)을 10진수 문자열(unsigned)로 변환 (Base : 0 or 10)
	len = kdUltostr ( out, KD_ULTOSTR_MAXLEN, in1, 0 );
	
	kdAssert ( len != (KDsize) -1 && !kdStrcmp ( out, "12345" ) );
	kdLogMessagefKHR ( "kdUltostr : in = '%u' , out = '%s'", in1, out );

	// 정수형(KDuint)을 8진수 문자열(unsigned)로 변환
	len = kdUltostr ( out, KD_ULTOSTR_MAXLEN, in2, 8 );

	kdAssert ( len != (KDsize) -1 && !kdStrcmp ( out, "10000" ) );
	kdLogMessagefKHR ( "kdUltostr : in = '%u'  , out = '%s'", in2, out );

	// 정수형(KDuint)을 16진수 문자열(unsigned)로 변환
	len = kdUltostr ( out, KD_ULTOSTR_MAXLEN, in3, 16 );

	kdAssert ( len != (KDsize) -1 && !kdStrcmp ( out, "ff" ) );
	kdLogMessagefKHR ( "kdUltostr : in = '%u'   , out = '%s'", in3, out );
}

KDvoid xmEx_kdFtostr ( KDvoid )
{
	KDchar       out[KD_FTOSTR_MAXLEN] = "";
	KDsize       len = 0;
	KDfloat32    in1 = -3.14159274f;
	KDfloat32    in2 = 5.9e-39f;
	
	// 일반 실수형 문자열을 실수형으로 변환
	len = kdFtostr ( out, KD_FTOSTR_MAXLEN, in1 );

	kdAssert ( len != (KDsize) -1 && !kdStrcmp ( out, "-3.14159274" ) );
	kdLogMessagefKHR ( "kdFtostr  : in = '%.9g'   , out = '%s'", in1, out );

	// 지수 실수형 문자열을 실수형으로 변환
	len = kdFtostr ( out, KD_FTOSTR_MAXLEN, in2 );

	kdAssert ( len != (KDsize) -1 && XM_IS_EQUAL_FLOAT ( kdStrtof ( out, KD_NULL ), 5.9e-39f ) );
	kdLogMessagefKHR ( "kdFtostr  : in = '%.9g', out = '%s'", in2, out );
}

KDvoid xmEx_kdCryptoRandom ( KDvoid )
{
	KDuint8      buf[4];
	KDint        out = 0;	
	KDint        ret = 0;
	KDint        i   = 0;

	// Crypto Random을 8회 구한다.
	for ( i = 0; i < 8; i++ )
	{
		ret = kdCryptoRandom ( buf, 4 );
		kdMemcpy ( &out, buf, 4 );	
			
		kdAssert ( ret == 0 );
		kdLogMessagefKHR ( "kdCryptoRandom : out = '%d'", out );
	}
}

static KDint compare ( const KDvoid* a, const KDvoid* b )
{
	return ( *(KDint*) a - *(KDint*) b );
}

KDvoid xmEx_kdQsort ( KDvoid )
{
	KDint   val  [ ] = { 40, 10, 95, 90, 20, 25 };
	KDchar  str  [ 128 ];
	KDchar  str2 [ 128 ];
	KDint   n;
	
	kdStrcpy ( str, "kdQsort        : before = " );
	for ( n = 0; n < 6; n++ )
	{
		kdSprintfKHR ( str2, "%d ", val [ n ] );
		kdStrcat ( str, str2 );
	}
	kdLogMessage ( str );

	kdQsort ( val, 6, sizeof ( KDint ), compare );

	kdStrcpy ( str, "kdQsort        : after  = " );
	for ( n = 0; n < 6; n++ )
	{
		kdSprintfKHR ( str2, "%d ", val [ n ] );
		kdStrcat ( str, str2 );
	}
	kdLogMessage ( str );
}

KDvoid xmExample_07 ( KDvoid )
{	
	kdLogMessage ( "Example 07. Utility library functions\n\n" );

	xmEx_kdAbs     ( );
	xmEx_kdStrtof  ( );
	xmEx_kdStrtol  ( );
	xmEx_kdStrtoul ( );
	xmEx_kdLtostr  ( );
   	xmEx_kdUltostr ( );
	xmEx_kdFtostr  ( );
	xmEx_kdCryptoRandom ( );
	xmEx_kdQsort   ( );
}
