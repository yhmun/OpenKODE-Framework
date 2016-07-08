/* --------------------------------------------------------------------------
 *
 *      File            khr_float64.c
 *      Description     64-bit floating point data type and associated functions. 
 *      Version         7, 2008-09-12
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

#include "kd_library.h"

#define XM_MINUS_ZERO_BITMASK_64    0x8000000000000000LL

static KDfloat64KHR kdGetNan ( KDint sign )
{
	KDuint64	    l_nan = ( ( (KDuint64) 0xffffffff ) << 32 ) | 0xffffffff;
	KDfloat64KHR    d_nan = *( (KDfloat64KHR *) &l_nan );

	return sign > 0 ? -d_nan : d_nan;
}

// kdAcosKHR : Arc cosine function. 
KD_API KDfloat64KHR KD_APIENTRY kdAcosKHR ( KDfloat64KHR x )
{
	if ( x == +1.0  )	return +0.0;
	if ( kdIsNan(x) )	return x;
 	if ( x < -1.0   )	return kdGetNan ( -1 );	
 	if ( x > +1.0   )	return kdGetNan (  1 );

	return acos ( x );
}

// kdAsinKHR : Arc sine function.
KD_API KDfloat64KHR KD_APIENTRY kdAsinKHR ( KDfloat64KHR x )
{
	if ( x == 0	    )	return x;
	if ( kdIsNan(x) )	return x;
 	if ( x < -1.0   )	return kdGetNan ( -1 );	
 	if ( x > +1.0   )	return kdGetNan (  1 );

	return asin ( x );	
}

// kdAtanKHR : Arc tangent function.
KD_API KDfloat64KHR KD_APIENTRY kdAtanKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( x == 0     )	return x;
	if ( kdIsNan(x) )	return x;
	if ( x == -inf  )	return -KD_PI_2_KHR;	
	if ( x == +inf  )	return +KD_PI_2_KHR;	

	return atan ( x );	
}	

// kdAtan2KHR : Arc tangent function. 
KD_API KDfloat64KHR KD_APIENTRY kdAtan2KHR ( KDfloat64KHR y, KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(y) ) return y;
	if ( kdIsNan(x) ) return x;

	if ( y == 0 )
	{		
		KDuint64    bitpattern_y;
		kdMemcpy ( &bitpattern_y, &y, sizeof ( KDfloat64KHR ) );

		if ( x == 0 )
		{
			KDuint64    bitpattern_x;
			kdMemcpy ( &bitpattern_x, &x, sizeof ( KDfloat64KHR ) );

			if ( bitpattern_x & XM_MINUS_ZERO_BITMASK_64 )	return bitpattern_y & XM_MINUS_ZERO_BITMASK_64 ? -KD_PI_KHR : KD_PI_KHR;		
			else										    return bitpattern_y & XM_MINUS_ZERO_BITMASK_64 ? -0.0 : +0.0;		
		}

		if ( x > 0 )	return bitpattern_y & XM_MINUS_ZERO_BITMASK_64 ? -0.0 : +0.0;				
		if ( x < 0 )	return bitpattern_y & XM_MINUS_ZERO_BITMASK_64 ? -KD_PI_KHR : KD_PI_KHR;		
	}

	if ( y == inf )
	{
		if ( x !=  inf && x != -inf )	return KD_PI_2_KHR;
		if ( x ==  inf )				return KD_PI_4_KHR;
		if ( x == -inf )				return 3 * KD_PI_4_KHR;		
	}

	if ( y == -inf )
	{
		if ( x !=  inf && x != -inf )	return -KD_PI_2_KHR;
		if ( x ==  inf )				return -KD_PI_4_KHR;
		if ( x == -inf )				return -3 * KD_PI_4_KHR;
	}

	if ( x == 0 )
	{
		if ( y < 0 )	return -KD_PI_2_KHR;
		if ( y > 0 )	return  KD_PI_2_KHR;		
	}

	if ( x == -inf )
	{
		if ( y > 0 )	return  KD_PI_KHR;
		if ( y < 0 )	return -KD_PI_KHR;
	}

	if ( x == inf )
	{
		if ( y > 0 )	return +0.0;
		if ( y < 0 )	return -0.0;
	}

	return atan2 ( y, x );	
}

// kdCosKHR : Cosine function. 
KD_API KDfloat64KHR KD_APIENTRY kdCosKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == inf   )	return kdGetNan ( 1 );

	return cos ( x );
}

// kdSinKHR : Sine function.
KD_API KDfloat64KHR KD_APIENTRY kdSinKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == inf   )	return kdGetNan ( 1 );

	return sin ( x );
}

// kdTanKHR : Tangent function.
KD_API KDfloat64KHR KD_APIENTRY kdTanKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return x;
 	if ( x == -inf  )	return kdGetNan ( -1 );
 	if ( x == +inf  )	return kdGetNan (  1 );

	return tan ( x );
}

// kdExpKHR : Exponential function. 
KD_API KDfloat64KHR KD_APIENTRY kdExpKHR ( KDfloat64KHR x )
{	
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == -inf  )	return +0.0;
	if ( x == +inf  )	return +inf;	

	return exp ( x );
}	

// kdLogKHR : Natural logarithm function.
KD_API KDfloat64KHR KD_APIENTRY kdLogKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return -inf;
	if ( x <  0	    )	return kdGetNan ( -1 );

	return log ( x );
}

// kdFabsKHR : Absolute value.
KD_API KDfloat64KHR KD_APIENTRY kdFabsKHR ( KDfloat64KHR x )
{
	if ( kdIsNan(x) )	return x;

	return fabs ( x );
}

// kdPowKHR : Power function.
KD_API KDfloat64KHR KD_APIENTRY kdPowKHR ( KDfloat64KHR x, KDfloat64KHR y )
{
	KDfloat64KHR    inf  = kdInfinity ( );
	KDfloat64KHR    abs  = 0;
	KDfloat64KHR    _int = 0;

	if ( kdIsNan(x) && y !=  0.0 )	return x;
	if ( kdIsNan(y) && x != +1.0 )	return y;
	if ( y ==  0.0 )	return +1.0;
	if ( x == +1.0 )	return +1.0;
	if ( x == -1.0 ) 
	{
		if ( y == +inf )	return +1.0;
		if ( y == -inf )	return +1.0;
	}

	_int = kdCeilKHR( y );
	if ( x == 0 )
	{
		if ( y == _int )
		{
			if ( y < 0 )
			{
				if ( (KDint32) y % 2 )
				{
					KDuint64    bitpattern_x;
					kdMemcpy ( &bitpattern_x, &x, sizeof ( KDfloat64KHR ) );

					return bitpattern_x & XM_MINUS_ZERO_BITMASK_64 ? -inf : +inf;
				}
				else
				{
					return +inf;
				}
			}

			if( y > 0 )
			{
				return (KDint32) y % 2 ? x : +0.0;
			}
		}
	}

	if ( x == -inf )
	{
		if ( y == _int )
		{
			if ( y < 0 )	return (KDint32) y % 2 ? -0.0 : +0.0;
			if ( y > 0 )	return (KDint32) y % 2 ? -inf : +inf;			
		}
	}

	if ( x == +inf )
	{
		if ( y < 0 )	return +0.0;
		if ( y > 0 )	return +inf;
	}

	if ( ( x < 0 && x != -inf ) && ( ( y != inf && y != -inf ) && y != _int ) )
	{
		return kdGetNan ( -1 );		
	}

	abs = kdFabsKHR ( x );
	if ( abs < 1.0f ) 
	{
		if ( y == -inf )	return +inf;
		if ( y == +inf )	return +0.0;		
	}

	if ( abs > 1.0f )
	{
		if ( y == -inf )	return +0.0;
		if ( y == +inf )	return +inf;		
	}

	return pow ( x, y );
}

// kdSqrtKHR : Square root function.
KD_API KDfloat64KHR KD_APIENTRY kdSqrtKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return x;
	if ( x == +inf  )	return x;
	if ( x <	0	)	return kdGetNan( -1 );

	return sqrt ( x );
}

// kdCeilKHR : Return ceiling value.
KD_API KDfloat64KHR KD_APIENTRY kdCeilKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return x;
	if ( x == +inf  )	return x;
	if ( x == -inf  )	return x;

	return ceil ( x );
}

// kdFloorKHR : Return floor value.
KD_API KDfloat64KHR KD_APIENTRY kdFloorKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return x;
	if ( x == +inf  )	return x;
	if ( x == -inf  )	return x;

	return floor ( x );
}

// kdRoundKHR : Round value to nearest integer.
KD_API KDfloat64KHR KD_APIENTRY kdRoundKHR ( KDfloat64KHR x )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if ( kdIsNan(x) )	return x;
	if ( x == 0     )	return x;
	if ( x == +inf  )	return x;
	if ( x == -inf  )	return x;

	return kdFloorKHR ( x + 0.5 );	
}

// kdInvsqrtKHR : Inverse square root function.
KD_API KDfloat64KHR KD_APIENTRY kdInvsqrtKHR ( KDfloat64KHR x )
{
	if ( kdIsNan(x) )	return x;
	if ( x < 0      )	return kdGetNan ( -1 );

	return 1.0 / kdSqrtKHR ( x );
}

// kdFmodKHR : Calculate floating point remainder.
KD_API KDfloat64KHR KD_APIENTRY kdFmodKHR ( KDfloat64KHR x, KDfloat64KHR y )
{
	KDfloat64KHR    inf = kdInfinity ( );

 	if ( y == 0     )	return kdGetNan (  1 );
 	if ( x == +inf  )	return kdGetNan (  1 );
 	if ( x == -inf  )	return kdGetNan ( -1 );
	if ( kdIsNan(x) )	return x;
	if ( kdIsNan(y) )	return y;

	if ( x == 0   && y != 0    )	return x;
	if ( y == inf || y == -inf )	return x;

	return fmod ( x, y );	
}

// kdStrtodKHR : Convert a string to a 64-bit floating point number.
KD_API KDfloat64KHR KD_APIENTRY kdStrtodKHR ( const KDchar* s, KDchar** endptr )
{
	KDfloat64KHR    ret = 0;
	
	if ( ( ret = strtod ( s, endptr ) ) == 0 )
	{
		ret = ( KDfloat64KHR ) kdStrtof ( s, endptr );
	}
 
	return ret;
}

// kdDtostrKHR : Convert a 64-bit float to a string.
KD_API KDssize KD_APIENTRY kdDtostrKHR ( KDchar* buffer, KDsize buflen, KDfloat64KHR number )
{
	KDfloat64KHR    inf = kdInfinity ( );

	if	    ( number ==  inf  )	kdSnprintfKHR ( buffer, buflen, "inf" );	
	else if ( number == -inf  )	kdSnprintfKHR ( buffer, buflen, "-inf" );	
	else if ( kdIsNan(number) )	kdSnprintfKHR ( buffer, buflen, "nan" );	
	else						kdSnprintfKHR ( buffer, buflen, "%.18g", number );	
	
	return kdStrlen ( buffer );
}

// kdCopysignKHR :a value whose absolute value matches that of x, but whose sign bit matches that of y
// XMSoft's revision : Added API.
KD_API KDfloat64KHR KD_APIENTRY kdCopysignKHR ( KDfloat64KHR x, KDfloat64KHR y )
{
	return copysign ( x, y );
}
