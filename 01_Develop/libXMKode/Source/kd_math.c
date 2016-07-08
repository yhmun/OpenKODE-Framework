/* --------------------------------------------------------------------------
 *
 *      File            kd_math.c
 *      Description     Mathematical functions
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

#define XM_MINUS_ZERO_BITMASK_32       0x80000000

static KDfloat32 xmComposeFloat ( KDuint sign, KDuint exponent, KDuint mantissa )
{
	KDuint32    float_as_int = 0;
	KDfloat32   result;

	float_as_int += mantissa & 0x7FFFFF;
	float_as_int += ( exponent & 0xFF ) << 23;
	float_as_int += ( sign & 1 ) << 31;

	kdMemcpy ( &result, &float_as_int, sizeof ( result ) );

	return result;
}

// kdInfinity : Infinity function.
// XMSoft's revision SPEC
KD_API KDfloat KD_APIENTRY kdInfinity ( KDvoid )
{
	KDfloat32   infinity;
	KDuint32    bitpattern = 0x7f800000;

	kdMemcpy ( &infinity, &bitpattern, sizeof ( KDfloat32 ) );

	return infinity;
}

// kdAcosf : Arc cosine function. 
KD_API KDfloat32 KD_APIENTRY kdAcosf ( KDfloat32 x )
{
	if ( x == +1.0f ) return +0.0f;
	if ( kdIsNan(x) ) return x;
	if ( x < -1.0f  ) return xmComposeFloat ( 1, 0xFF, 12 );	
	if ( x > +1.0f  ) return xmComposeFloat ( 0, 0xFF,  1 );	

	return acosf ( x );	
}

// kdAsinf : Arc sine function.
KD_API KDfloat32 KD_APIENTRY kdAsinf ( KDfloat32 x )
{
	if ( x == 0	    ) return x;
	if ( kdIsNan(x) ) return x;
	if ( x < -1.0f  ) return xmComposeFloat ( 1, 0xFF, 12 );	
	if ( x > +1.0f  ) return xmComposeFloat ( 0, 0xFF,  1 );	

	return asinf ( x );
}

// kdAtanf : Arc tangent function.
KD_API KDfloat32 KD_APIENTRY kdAtanf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( x == 0     ) return x;
	if ( kdIsNan(x) ) return x;
	if ( x == -inf  ) return -KD_PI_2_F;	
	if ( x == +inf  ) return +KD_PI_2_F;	

	return atanf ( x );
}

// kdAtan2f : Arc tangent function. 
KD_API KDfloat32 KD_APIENTRY kdAtan2f ( KDfloat32 y, KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(y) ) return y;
	if ( kdIsNan(x) ) return x;
	
	if ( y == 0 )
	{		
		KDuint32    bitpattern_y;
		kdMemcpy ( &bitpattern_y, &y, sizeof ( KDfloat32 ) );

		if ( x == 0 )
		{
			KDuint32    bitpattern_x;
			kdMemcpy ( &bitpattern_x, &x, sizeof ( KDfloat32 ) );

			if ( bitpattern_x & XM_MINUS_ZERO_BITMASK_32 )	return ( bitpattern_y & XM_MINUS_ZERO_BITMASK_32 ) ? -KD_PI_F : KD_PI_F;		
			else										    return ( bitpattern_y & XM_MINUS_ZERO_BITMASK_32 ) ? -0.0f : +0.0f;		
		}

		if ( x > 0 ) return ( bitpattern_y & XM_MINUS_ZERO_BITMASK_32 ) ? -0.0f : +0.0f;				
		if ( x < 0 ) return ( bitpattern_y & XM_MINUS_ZERO_BITMASK_32 ) ? -KD_PI_F : KD_PI_F;		
	}

	if ( y == inf )
	{
		if ( x !=  inf && x != -inf )	return KD_PI_2_F;
		if ( x ==  inf )				return KD_PI_4_F;
		if ( x == -inf )				return 3 * KD_PI_4_F;		
	}

	if ( y == -inf )
	{
		if ( x !=  inf && x != -inf )	return -KD_PI_2_F;
		if ( x ==  inf )				return -KD_PI_4_F;
		if ( x == -inf )				return -3 * KD_PI_4_F;
	}

	if ( x == 0 )
	{
		if ( y < 0 ) return -KD_PI_2_F;
		if ( y > 0 ) return  KD_PI_2_F;		
	}

	if ( x == -inf )
	{
		if ( y > 0 ) return  KD_PI_F;
		if ( y < 0 ) return -KD_PI_F;
	}
	
	if ( x == inf )
	{
		if ( y > 0 ) return +0.0f;
		if ( y < 0 ) return -0.0f;
	}

	return atan2f ( y, x );
}

// kdCosf : Cosine function. 
KD_API KDfloat32 KD_APIENTRY kdCosf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == inf   ) return xmComposeFloat ( 0, 0xFF, 1 );	

	return cosf ( x );
}

// kdSinf : Sine function. 
KD_API KDfloat32 KD_APIENTRY kdSinf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if( kdIsNan(x) ) return x;
	if( x == inf   ) return xmComposeFloat ( 0, 0xFF, 1 );	

	return sinf ( x );
}

// kdTanf : Tangent function.
KD_API KDfloat32 KD_APIENTRY kdTanf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return x;
	if ( x == -inf  ) return xmComposeFloat ( 1, 0xFF, 12 );
	if ( x == +inf  ) return xmComposeFloat ( 0, 0xFF,  1 );	

	return tanf ( x );
}

// kdExpf : Exponential function. 
KD_API KDfloat32 KD_APIENTRY kdExpf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == -inf  ) return +0.0f;
	if ( x == +inf  ) return +inf;	

	return expf ( x );
}

// kdLogf : Natural logarithm function.
KD_API KDfloat32 KD_APIENTRY kdLogf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return -inf;
	if ( x <  0	    ) return xmComposeFloat ( 1, 0xFF, 12 );

	return logf ( x );
}

// kdFabsf : Absolute value.
KD_API KDfloat32 KD_APIENTRY kdFabsf ( KDfloat32 x )
{
	if ( kdIsNan(x) ) return x;

	return fabsf ( x );
}

// kdPowf : Power function.
// XMSoft's revision : Unexpectedly processed in "WinCE" and "Android" platforms ( Case : Infinity Input Value )
KD_API KDfloat32 KD_APIENTRY kdPowf ( KDfloat32 x, KDfloat32 y )
{
	KDfloat32   inf  = kdInfinity ( );
	KDfloat32   abs  = 0;
	KDfloat32   _int = 0;

	if ( kdIsNan(x) && y !=  0.0f ) return x;
	if ( kdIsNan(y) && x != +1.0f ) return y;
	if ( y ==     0 ) return +1.0f;
	if ( x == +1.0f ) return +1.0f;
	if ( x == -1.0f ) 
	{
		if ( y == +inf ) return +1.0f;
		if ( y == -inf ) return +1.0f;
	}
	
	_int = kdCeilf ( y );
	if ( x == 0 )
	{
		if ( y == _int )
		{
			if ( y < 0 )
			{
				if ( ( KDint32 ) y % 2 )
				{
					KDuint32    bitpattern_x;
					kdMemcpy ( &bitpattern_x, &x, sizeof ( KDfloat32 ) );
					return ( bitpattern_x & XM_MINUS_ZERO_BITMASK_32 ) ? -inf : +inf;
				}
				else
				{
					return +inf;
				}
			}

			if ( y > 0 )
			{
				return ( (KDint32) y % 2) ? x : +0.0f;
			}
		}
	}

	if ( x == -inf )
	{
		if ( y == _int )
		{
			if ( y < 0 )
			{
				return ( (KDint32) y % 2 ) ? -0.0f : +0.0f;
			}
			
			if ( y > 0 )
			{
				return ( (KDint32) y % 2 ) ? -inf : +inf;
			}
		}
	}

	if ( x == +inf )
	{
		if ( y < 0 ) return +0.0f;
		if ( y > 0 ) return +inf;
	}

	if ( ( x < 0 && x != -inf ) && ( ( y != inf && y != -inf ) && y != _int ) )
	{
		return xmComposeFloat ( 1, 0xFF, 12 );
	}

	abs = kdFabsf ( x );
	if ( abs < 1.0f ) 
	{
		if ( y == -inf ) return +inf;
		if ( y == +inf ) return +0.0f;		
	}

	if ( abs > 1.0f )
	{
		if ( y == -inf ) return +0.0f;
		if ( y == +inf ) return +inf;		
	}

	return powf ( x, y );
}

// kdSqrtf : Square root function.
KD_API KDfloat32 KD_APIENTRY kdSqrtf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return x;
	if ( x == +inf  ) return x;
	if ( x <	0	) return xmComposeFloat ( 1, 0xFF, 12 );

	return sqrtf ( x );
}

// kdCeilf : Return ceiling value.
KD_API KDfloat32 KD_APIENTRY kdCeilf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return x;
	if ( x == +inf  ) return x;
	if ( x == -inf  ) return x;

	return ceilf ( x );
}

// kdFloorf : Return floor value.
KD_API KDfloat32 KD_APIENTRY kdFloorf ( KDfloat32 x )
{
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return x;
	if ( x == +inf  ) return x;
	if ( x == -inf  ) return x;

	return floorf ( x );
}

// kdRoundf : Round value to nearest integer.
KD_API KDfloat32 KD_APIENTRY kdRoundf ( KDfloat32 x )
{		
	KDfloat32   inf = kdInfinity ( );

	if ( kdIsNan(x) ) return x;
	if ( x == 0     ) return x;
	if ( x == +inf  ) return x;
	if ( x == -inf  ) return x;

	return floorf ( x + 0.5f );	
}

// kdInvsqrtf : Inverse square root function.
KD_API KDfloat32 KD_APIENTRY kdInvsqrtf ( KDfloat32 x )
{
	if ( kdIsNan(x) ) return x;
	if ( x < 0      ) return xmComposeFloat ( 1, 0xFF, 12 );

	return 1.0f / sqrtf ( x );
}

// kdFmodf : Calculate floating point remainder.
KD_API KDfloat32 KD_APIENTRY kdFmodf ( KDfloat32 x, KDfloat32 y )
{
	KDfloat32   inf = kdInfinity ( );

	if ( y == 0     ) return xmComposeFloat ( 0, 0xFF,  1 );	
	if ( x == +inf  ) return xmComposeFloat ( 0, 0xFF,  1 );	
	if ( x == -inf  ) return xmComposeFloat ( 1, 0xFF, 12 );
	if ( kdIsNan(x) ) return x;
	if ( kdIsNan(y) ) return y;

	if ( x == 0   && y != 0    ) return x;
	if ( y == inf || y == -inf ) return x;

	return fmodf ( x, y );
}
