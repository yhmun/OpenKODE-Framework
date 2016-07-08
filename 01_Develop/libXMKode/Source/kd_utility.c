/* --------------------------------------------------------------------------
 *
 *      File            kd_utility.c
 *      Description     Utility library functions
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

#define	XM_MAX_DIGITS              ( 1 + 4 * sizeof ( KDint ) )
#define XM_DIGIT_TO_CHAR(digit)    ( ( digit ) < 10 ? (KDchar) ( ( digit ) + 48 ) : (KDchar) ( ( digit ) + 87 ) )

typedef struct XMTwoFactored
{
    KDint       power;
    KDuint64    factor;
} XMTwoFactored;

typedef struct XMTenFactored
{
	KDint       power;
	KDuint64    factor;
} XMTenFactored;

static KDvoid xmDecomposeFloat ( KDfloat32 f, KDint* sign, KDint* exponent, KDuint* mantissa )
{
	KDuint32    float_as_int;

	kdMemcpy ( &float_as_int, &f, sizeof ( float_as_int ) );

	*mantissa = float_as_int & 0x7FFFFF;    
	*exponent = float_as_int >> 23 & 0xFF;
	*sign     = float_as_int >> 31 & 1;
}

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

static KDvoid xmGetDecimalsForPowerOfTwo ( KDint power, XMTenFactored* result )
{
	XMTenFactored some_powers_of_two[] = 
	{
		{ -45-11, (KDuint64) 140 * 1000000000 + 129846432 }, // 2^-149 = 1,401 298 464 324 e-45 
		{ -38-11, (KDuint64) 235 * 1000000000 +  98870164 }, // 2^-125 = 2,350 988 701 644 e-38 
		{ -31-11, (KDuint64) 394 * 1000000000 + 430452611 }, // 2^-101 = 3,944 304 526 105 e-31 
		{ -24-11, (KDuint64) 661 * 1000000000 + 744490042 }, // 2^-77  = 6,617 444 900 424 e-24 
		{ -16-11, (KDuint64) 111 * 1000000000 +  22302463 }, // 2^-53  = 1,110 223 024 625 e-16 
		{ - 9-11, (KDuint64) 186 * 1000000000 + 264514923 }, // 2^-29  = 1,862 645 149 230 e-09 
		{ - 2-11, (KDuint64) 312 * 1000000000 + 500000000 }, // 2^-5   = 3,125 000 000 000 e-02 
		{ + 5-11, (KDuint64) 524 * 1000000000 + 288000000 }, // 2^19   = 5,242 880 000 000 e+05 
		{ +12-11, (KDuint64) 879 * 1000000000 + 609302221 }, // 2^43   = 8,796 093 022 208 e+12 
		{ +20-11, (KDuint64) 147 * 1000000000 + 573952590 }, // 2^67   = 1,475 739 525 896 e+20 
		{ +27-11, (KDuint64) 247 * 1000000000 + 588007857 }, // 2^91   = 2,475 880 078 570 e+27 
		{ +34-11, (KDuint64) 415 * 1000000000 + 383748683 }  // 2^115  = 4,153 837 486 827 e+34 
	};

	KDint		index;
	KDint		modulus;
	KDuint64	factor;

	index	= ( power + 149 ) / 24; 
	modulus	= ( power + 149 ) % 24;

	factor	= some_powers_of_two[ index ].factor << modulus;
	power	= some_powers_of_two[ index ].power;

	// truncate to 12 decimals
	while ( factor >= (KDuint64) 1000 * 1000000000 )
	{
		factor /= 10;
		++power;
	}

	result->factor = factor;
	result->power  = power;
}

static KDvoid TwoFactorPowerOfTen ( KDint power, XMTwoFactored* result )
{
    XMTwoFactored some_powers_of_ten[] = 
	{
		{ -235, (KDuint64) 552 * 1000000000 + 139707743 }, // 10^-59 = 552 139 707 743 * 2^-235 
		{ -212, (KDuint64) 658 * 1000000000 + 201822928 }, // 10^-52 = 658 201 822 928 * 2^-212  
		{ -189, (KDuint64) 784 * 1000000000 + 637716923 }, // 10^-45 = 784 637 716 923 * 2^-189  
		{ -166, (KDuint64) 935 * 1000000000 + 361047891 }, // 10^-38 = 935 361 047 891 * 2^-166 
		{ -142, (KDuint64) 557 * 1000000000 + 518629963 }, // 10^-31 = 557 518 629 963 * 2^-142 
		{ -119, (KDuint64) 664 * 1000000000 + 613997892 }, // 10^-24 = 664 613 997 892 * 2^-119 
		{ - 96, (KDuint64) 792 * 1000000000 + 281625142 }, // 10^-17 = 792 281 625 142 * 2^-96  
		{ - 73, (KDuint64) 944 * 1000000000 + 473296573 }, // 10^-10 = 944 473 296 573 * 2^-73 
		{ - 49, (KDuint64) 562 * 1000000000 + 949953421 }, // 10^-3  = 562 949 953 421 * 2^-49 
		{ - 26, (KDuint64) 671 * 1000000000 +  88640000 }, // 10^+4  = 671 088 640 000 * 2^-26 
		{ -  3, (KDuint64) 800 * 1000000000 +         0 }, // 10^+11 = 800 000 000 000 * 2^-3 
		{ + 20, (KDuint64) 953 * 1000000000 + 674316406 }, // 10^+18 = 953 674 316 406 * 2^20 
		{ + 44, (KDuint64) 568 * 1000000000 + 434188608 }, // 10^+25 = 568 434 188 608 * 2^44 
		{ + 67, (KDuint64) 677 * 1000000000 + 626357803 }  // 10^+32 = 677 626 357 803 * 2^67 
	};

    KDint       index;
    KDint       modulus;
    KDuint64    factor;
    KDint       i;
    
    index   = ( power + 59 ) / 7; 
    modulus = ( power + 59 ) % 7;
    
    power  = some_powers_of_ten[ index ].power;
    factor = some_powers_of_ten[ index ].factor;
    
	for( i = 0; i < modulus; ++i )
	{
        factor *= 10;
	}
    
    // truncate to 30 bits 
    while ( factor >= ( 1 << 30 ) )
    {
        factor >>= 1;
        ++power;
    }

    result->factor = factor;
    result->power  = power;
}

static KDint xmDecimalFloatToString ( KDint sign, KDint power, KDuint64 factor, KDchar* result )
{
	KDchar	    buf[9];
	KDint       buf_pos = 0;
	KDint       pos     = 0;
	KDint       i;
	KDint       modulus;

	power += 11;

	// rounding to 9 decimals 
	if ( factor % 1000 >= 500 )
	{
		factor += 1000;
	}
	factor /= 1000;

	if ( factor >= 1000000000 )
	{
		factor /= 10;
		++power;
	}

	for ( i = 8; i >= 0; --i )
	{
		modulus = (KDint) ( factor % 10 );
		buf[i]  = XM_DIGIT_TO_CHAR ( modulus );
		factor /= 10;
	}

	if ( ( power >= 9 ) || ( power < -4 ) )
	{
		// exp form 
		if ( sign )
		{
			result[ pos++ ] = '-';
		}

		result[ pos++ ] = buf[ 0 ];
		result[ pos++ ] = '.';
		kdMemcpy ( result + pos, buf + 1, 8 );
		pos += 8;

		// trim
		while ( result[ pos - 1 ] == '0' )
		{
			pos--;
		}

		if ( result[ pos - 1 ] == '.' )
		{
			pos--;
		}

		result[ pos++ ] = 'e';
		result[ pos++ ] = power >= 0 ? '+' : '-';
		
		if ( power < 0 )
		{
			power *= -1;
		}
		result[ pos++ ] = XM_DIGIT_TO_CHAR ( power / 10 );
		result[ pos++ ] = XM_DIGIT_TO_CHAR ( power % 10 );
		result[ pos++ ] = 0;

		return pos - 1;
	}
	else
	{
		if ( sign )
		{
			result[ pos++ ] = '-';
		}

		if (power >= 0 )
		{
			kdMemcpy ( result + pos, buf + buf_pos, power + 1 );
			pos		+= power + 1;
			buf_pos += power + 1;
		}
		else
		{
			result[ pos++ ] = '0';
		}
		result[ pos++ ] = '.';

		for ( i = 0; i < -power - 1; ++i )
		{
			result[ pos++ ] = '0';
		}
		kdMemcpy ( result + pos, buf + buf_pos, 9 - buf_pos );
		pos += 9 - buf_pos;        

		// trim 
		while ( result[ pos - 1 ] == '0' )
		{
			pos--;
		}

		if ( result[ pos - 1 ] == '.' )
		{
			pos--;  
		}

		result[ pos++ ] = 0;

		return pos - 1;
	}
}

static KDbool xmIsWhiteSpace ( KDchar const ch )
{
	switch ( ch )
    {
		case ' ' : case '\f' : case '\n' : case '\r' : case '\t' : case '\v' :
			return KD_TRUE;

        default :
            return KD_FALSE;
    }
}

static KDboolean xmIsDigitForBase ( KDchar const ch, KDint base )
{
	switch ( ch )
	{
		case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' :
			return KD_TRUE;

		case '8' : case '9' :
			return base >= 10 ? KD_TRUE : KD_FALSE;

		case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
		case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' :       
			return base >= 16 ? KD_TRUE : KD_FALSE;

		default:
			return KD_FALSE;
	}                
}

static KDint xmCharToDigit ( KDchar ch )
{
    if ( ch >= 97 )
	{
        return ch - 97 + 10;
	}

    if ( ch >= 65 )
	{
        return ch - 65 + 10;
	}

    return ch - 48;
} 

static KDchar ToLowerCaseHex ( KDchar ch )
{
    if ( ch >= 65 && ch <= 70 )
	{
        ch += 32;
	}

    return ch;
}       

// kdAbs : Compute the absolute value of an integer.
KD_API KDint KD_APIENTRY kdAbs ( KDint i )
{		
	// If i is KDINT_MIN, the returned value is undefined.
 	if ( i == KDINT_MIN )
 	{		
		return KDINT_MAX;
 	}

	return abs ( i );
}

// kdStrtof : Convert a string to a floating point number.
KD_API KDfloat32 KD_APIENTRY kdStrtof ( const KDchar* s, KDchar** endptr )
{
    KDchar*         pos = (KDchar*) s;
    KDint           point_pos = -1;
    KDint           base = 10;
    KDint           base_to_i;
    KDint           i;
    XMTwoFactored   tf;
    KDchar*         mantissa_start_pos;
    KDint           mantissa_length = 0;
    KDint           mantissa_read_length = 0;
    KDchar*         exponent_start_pos = KD_NULL;
    KDint           exponent_length = 0;
    KDint           exponent_sign = 1;
    KDint           exponent = 0;
    KDint           mantissa_sign = 1;
    KDuint          sign;
    KDuint64        mantissa = 0;
    KDint           mantissa_power;
    KDint           number_of_postdecimal_zeros = 0;
    KDfloat32       result;
    KDint           round;
        
    while ( xmIsWhiteSpace ( *pos ) )
	{
        ++pos;
	}
        
    if ( *pos == '+' )
    {
        ++pos;
    }        
    else if ( *pos == '-' )
    {
        mantissa_sign = -1;
        ++pos;
    }
    
    // inf 
    if ( (pos[0] == 'i' || pos[0] == 'I' ) && ( pos[1] == 'n' || pos[1] == 'N' ) && ( pos[2] == 'f' || pos[2] == 'F' ) )
    {
        pos += 3;
        result = xmComposeFloat ( mantissa_sign == 1 ? 0 : 1, 0xFF, 0 );
        if ( endptr )
		{
            *endptr = pos;
		}

        return result;            
    }
    
    // nan 
    if ( (pos[0] == 'n' || pos[0] == 'N' ) && ( pos[1] == 'a' || pos[1] == 'A' ) && ( pos[2] == 'n' || pos[2] == 'N' ) )
    {
        pos += 3;
        result = xmComposeFloat ( 0, 0xFF, 1 );
        if ( endptr )
		{
            *endptr = pos;
		}

        return result;
    }
    
    mantissa_start_pos = pos;
    for ( ; ; )
    {
        if ( xmIsDigitForBase ( mantissa_start_pos[ mantissa_length ], base ) )
		{
            ++mantissa_length;
		}
        else if ( mantissa_start_pos[ mantissa_length ] == '.' )
		{
            point_pos = mantissa_length++;
		}
        else
		{
            break;
		}
    }
    
    // if no mantissa at all.. 
    if ( !mantissa_length )
    {
        if ( endptr )
		{
            *endptr = (KDchar *) s;
		}

        return 0.0f;
    }
    
    // predecimal zeros 
    while ( mantissa_start_pos[0] == '0' )
    {
        ++mantissa_start_pos;
        --mantissa_length;
        if ( point_pos != -1 )
		{
            --point_pos;
		}
    }
    
    // postdecimal zeros 
    if ( mantissa_start_pos[0] == '.' )
    {
        while ( mantissa_start_pos[1] == '0' )
        {
            ++mantissa_start_pos;
            --mantissa_length;
            ++number_of_postdecimal_zeros;
        }
    }
    
    mantissa_read_length = mantissa_length;
    
    // cap mantissa reading 
    if ( mantissa_read_length > 10 && point_pos == -1 )
	{
        mantissa_read_length = 10;
	}
    else if ( mantissa_read_length > 11 && point_pos != -1 )
	{
        mantissa_read_length = 11;
	}
    
    if ( point_pos != -1 )
	{
        mantissa_power = point_pos - mantissa_read_length + 1 - number_of_postdecimal_zeros;
	}
    else
	{
        mantissa_power = - number_of_postdecimal_zeros;        
	}
    
    base_to_i = 1;
    mantissa  = 0;
    for ( i = mantissa_read_length - 1; i >= 0; --i )
    {
        if ( i != point_pos )
        {
            mantissa  += xmCharToDigit ( mantissa_start_pos[ i ] ) * base_to_i;
            base_to_i *= base;
        }
    }
    
    pos = mantissa_start_pos + mantissa_length;
    
    if ( ( base == 10 && ( *pos == 'e' || *pos == 'E' ) ) || ( base == 16 && ( *pos == 'p' || *pos == 'P' ) ) )
    {
        KDchar *rewind = pos;
        ++pos;
        
        if ( *pos == '+' )
        {
            ++pos;
        }
        else if ( *pos == '-' )
        {
            exponent_sign = -1;
            ++pos;
        }

        if ( (unsigned) *pos - '0' >= 10 )
        {
            // No exponent digits -- rewind. 
            pos = rewind;
        }
        else
        {
			while( *pos == '0' )
			{
                ++pos;
			}
            
            exponent_start_pos = pos;
            
            while ( xmIsDigitForBase ( exponent_start_pos[ exponent_length ], base ) )
			{
                ++exponent_length;
			}
            
            base_to_i = 1;
            exponent  = 0;
            for ( i = exponent_length - 1; i >= 0; --i )
            {
                exponent  += xmCharToDigit ( exponent_start_pos[ i ] ) * base_to_i;
                base_to_i *= 10;
            }
        }
    }
    
    if ( endptr )
	{
        *endptr = exponent_start_pos ? exponent_start_pos + exponent_length : pos;                             
	}
    
    // +/- 0 
    if ( mantissa == 0 )
	{
        return mantissa_sign == 1 ? +0.0f : -0.0f;        
	}
    
    // hexadecimal number whith neither hexadecimal point or binary exponent present 
    if ( base == 16 && point_pos == -1 && !exponent_length )
    {
        if ( endptr )
		{
            *endptr = (KDchar*) s;
		}

        return +0.0f;
    }
    
    exponent *= exponent_sign;
    
    if ( base == 10 )
    {        
        TwoFactorPowerOfTen ( mantissa_power + exponent, &tf );
        tf.factor *= mantissa;
    }
    else
    {
        tf.factor = mantissa;
        tf.power  = mantissa_power + exponent;
    }
        
    // truncate to 24 bits 
    while ( tf.factor >= ( 1 << 24 ) )
    {
        round = (KDint) ( tf.factor & 1 );
        tf.factor >>= 1;
        ++tf.power;
    }
    
    tf.factor += 1;
    if ( tf.factor >= ( 1 << 24 ) )
    {
        tf.factor >>= 1;
        ++tf.power;
    }        
    
    sign = mantissa_sign == 1 ? 0 : 1;
    
    tf.power += 127;
    tf.power += 23;
    
    // denorm. 
    if ( tf.power < 1 )       
    { 
        while ( tf.power < 1 )
        {
            ++tf.power;
            tf.factor >>= 1;
        }
        tf.power = 0;
        if ( !tf.factor )
        {
            // Underflow. 
            kdSetError ( KD_ERANGE );	
        }
    }        
    
    if ( tf.power >= 255 )
    {
        // Overflow. 
        tf.factor = 0;
        tf.power  = 255;

        kdSetError ( KD_ERANGE );	
    }

    result = xmComposeFloat ( sign, tf.power, (KDuint) tf.factor );
    return result;        
}

// kdStrtol, kdStrtoul : Convert a string to an integer.
KD_API KDint KD_APIENTRY kdStrtol ( const KDchar* s, KDchar** endptr, KDint base )
{	
    KDchar*      pos = (KDchar*) s;
    KDint        sign = 1;
    KDint        result;
    KDint        number_of_digits = 0;    
    KDchar       buf[ XM_MAX_DIGITS + 1 ];
    KDssize      max_number_of_digits;
    KDint        base_to_i;
    KDint        i;
    KDuint       max;
    KDint        diff;
    
	while ( xmIsWhiteSpace ( *pos ) )
	{
        ++pos;
	}
        
    if ( *pos == '+' )
    {
        ++pos;
    }        
    else if ( *pos == '-' )
    {
        sign = -1;
        ++pos;
    }
     
    if ( *pos == '0' && ( *( pos + 1 ) == 'x' || *( pos + 1 ) == 'X' ) && ( base == 16 || base == 0 ) )
    {
        base = 16;
        pos += 2;
    }
    else if ( *pos == '0' && base == 0 )
    {
        base = 8;
        ++pos;
    }
    else if ( base == 0 )
    {
        base = 10;
    }
    
    while ( *pos == '0' )
	{
        ++pos;     
	}
    
    number_of_digits = 0;
    while ( xmIsDigitForBase ( pos[ number_of_digits ], base ) )
	{
        ++number_of_digits;
	}
    
    if ( endptr )
	{
        *endptr = pos + number_of_digits;                 
	}
        
    if ( !number_of_digits )        
	{
        return 0;
	}
        
    if ( base == 16 )
	{
        for ( i = 0; i < number_of_digits; ++i )
		{
            pos[ i ] = ToLowerCaseHex ( pos[ i ] );
		}
	}

    max = sign == 1 ? KDINT_MAX : ( (KDuint) ( KDINT_MAX ) ) + 1;
        
    max_number_of_digits = kdUltostr( buf, sizeof( buf ), max, base ); 
    if ( number_of_digits > max_number_of_digits )
    {
		kdSetError ( KD_ERANGE );		
        return sign == 1 ? KDINT_MAX : KDINT_MIN;
    }        
    else if ( number_of_digits == max_number_of_digits )
    {
        diff = kdStrncmp ( pos, buf, max_number_of_digits );
        if ( diff >= 0 )
        {            
			if ( diff > 0 )
			{
				kdSetError ( KD_ERANGE );		
			}
            
            return sign == 1 ? KDINT_MAX : KDINT_MIN;
        }
    }
        
    base_to_i = 1;
    result = 0;        
        
    for ( i = number_of_digits - 1; i >= 0; --i )
    {        
        result += xmCharToDigit ( pos[ i ] ) * base_to_i;
        base_to_i *= base;
    }
    
    result *= sign;
    return result;
}

KD_API KDuint KD_APIENTRY kdStrtoul ( const KDchar *s, KDchar **endptr, KDint base )
{	
	KDchar*	     pos = (KDchar*) s;
    KDuint       result;
    KDint        number_of_digits = 0;    
    KDchar       buf[ XM_MAX_DIGITS + 1 ];
    KDssize      max_number_of_digits;
    KDint        base_to_i;
    KDint        i;
    KDint        sign = 1;
    
    while ( xmIsWhiteSpace ( *pos ) )
	{
        ++pos;
	}
        
    if ( *pos == '+' )
    {
        ++pos;
    }        
    else if ( *pos == '-' )
    {
        sign = -1;
        ++pos;
    }
     
    if ( *pos == '0' && ( *( pos + 1 ) == 'x' || * ( pos + 1 ) == 'X' ) && ( base == 16 || base == 0 ) )
    {
        base = 16;
        pos += 2;
    }
    else if ( *pos == '0' && base == 0 )
    {
        base = 8;
        ++pos;
    }
    else if ( base == 0 )
    {
        base = 10;
    }
    
    while ( *pos == '0' )
	{
        ++pos;     
	}
    
    number_of_digits = 0;
    while ( xmIsDigitForBase ( pos[ number_of_digits ], base ) )
	{
        ++number_of_digits;
	}
    
    if ( endptr )
	{
        *endptr = pos + number_of_digits;    
	}
        
    if ( !number_of_digits )
	{
        return 0;
	}
        
    if ( base == 16 )
	{
        for ( i = 0; i < number_of_digits; ++i )
		{
            pos[ i ] = ToLowerCaseHex ( pos[ i ] );
		}
	}
        
    max_number_of_digits = kdUltostr ( buf, sizeof ( buf ), KDUINT_MAX, base );
    if ( number_of_digits > max_number_of_digits )
    {       
		kdSetError ( KD_ERANGE );		

        return KDUINT_MAX;
    }        
    else if ( number_of_digits == max_number_of_digits )
    {
        if ( kdStrncmp ( pos, buf, max_number_of_digits ) > 0 )
        {            
			kdSetError ( KD_ERANGE );		

            return KDUINT_MAX;
        }            
    }
        
    base_to_i = 1;
    result = 0;        
        
    for ( i = number_of_digits - 1; i >= 0; --i )
    {                 
        result += sign * xmCharToDigit ( pos[ i ] ) * base_to_i;
        base_to_i *= base;
    }
    
    return result;
}

// kdLtostr, kdUltostr : Convert an integer to a string. 
KD_API KDssize KD_APIENTRY kdLtostr ( KDchar* buffer, KDsize buflen, KDint number )
{	
	KDssize     len;

	if ( buflen > KD_LTOSTR_MAXLEN )
	{
		buflen = KD_LTOSTR_MAXLEN;
	}

	if ( number >= 0 )
	{
		len = kdUltostr ( buffer, buflen, number, 0 );
		
		if( len == -1 )
		{
			return -1;
		}
	}
	else
	{
		len = kdUltostr ( buffer + 1, buflen - 1, -number, 0 );

		if( len == -1 )
		{
			return -1;
		}

		len++;
		buffer[ 0 ] = '-';
	}

	return len;
}

KD_API KDssize KD_APIENTRY kdUltostr ( KDchar* buffer, KDsize buflen, KDuint number, KDint base )
{	
	KDchar      buf[ XM_MAX_DIGITS + 1 ];	
	KDint       modulus;
	KDint       start = XM_MAX_DIGITS - 1;	
	KDint       i;

	if ( base == 0 )
	{
		base = 10;
	}

	if ( buflen == 0 )
	{
		return -1;
	}

	if ( buflen > KD_ULTOSTR_MAXLEN )
	{
		buflen = KD_ULTOSTR_MAXLEN;
	}

	buf[ XM_MAX_DIGITS ] = 0;
	for ( i = XM_MAX_DIGITS - 1; i >= 0; --i )
	{
		modulus = number % base;

		// update string starting point 
		if ( modulus )
		{
			start = i;
		}

		buf[i]  = XM_DIGIT_TO_CHAR ( modulus );
		number /= base;
	}

	if ( kdStrncpy_s ( buffer, buflen, buf + start, XM_MAX_DIGITS + 1 - start ) )
	{
		return -1;
	}

	return kdStrlen ( buffer );
}

// kdFtostr : Convert a float to a string.
KD_API KDssize KD_APIENTRY kdFtostr ( KDchar* buffer, KDsize buflen, KDfloat32 number )
{	
	KDchar		    buf[KD_FTOSTR_MAXLEN] = "";
	KDint           sign;
	KDint           exponent;
	KDuint          mantissa;
	XMTenFactored   tf;
	KDint           chars;

	if ( buflen == 0 )
	{
		return -1;
	}
	
	if ( buflen > KD_FTOSTR_MAXLEN )
	{
		buflen = KD_FTOSTR_MAXLEN;
	}

	xmDecomposeFloat ( number, &sign, &exponent, &mantissa );

	// nans & infs 
	if ( exponent == 255 )
	{
		if ( !mantissa )
		{
			if ( sign )
			{				
				kdStrcpy_s ( buf, 5, "-inf" );
				chars = 4;
				goto result;				
			}
			else
			{
				kdStrcpy_s ( buf, 4, "inf" );				
				chars = 3;
				goto result;	
			}
		}
		else
		{
			kdStrcpy_s ( buf, 4, "nan" );
			chars = 3;
			goto result;
		}
	}

	// zeros 
	if ( ( exponent == 0 ) && ( mantissa == 0 )	)
	{
		if ( sign )
		{
			kdStrcpy_s ( buf, 3, "-0" );
			chars = 2;
			goto result;
		}
		else
		{
			kdStrcpy_s ( buf, 2, "0" );
			chars = 1;
			goto result;
		}
	} 

	if ( exponent == 0 )
	{
		++exponent;
	}
	else
	{
		mantissa += 1 << 23;
	}

	xmGetDecimalsForPowerOfTwo ( exponent - 127 - 23, &tf );

	tf.factor = mantissa * tf.factor;

	// truncate to 12 decimals 
	while ( tf.factor >= (KDuint64) 1000 * 1000000000 )
	{
		tf.factor /= 10;
		++tf.power;
	}

	chars = xmDecimalFloatToString ( sign, tf.power, tf.factor, buf );

result :

	if ( buflen <= kdStrlen ( buf ) )
	{
		return -1;
	}
	
	kdStrcpy_s ( buffer, buflen, buf );

	return chars;
}

// kdCryptoRandom : Return random data.
// XMSoft's revision : Only crypto supported with Win32. other platforms are normal random.
KD_API KDint KD_APIENTRY kdCryptoRandom ( KDuint8* buf, KDsize buflen )
{
	KDint     ret = 0;
	KDsize    i;

	if ( g_crypto )
	{
		#ifdef XM_HAS_PLATFORM_CRYPTO
		
		if ( ret = xmPlatformCryptoRand ( buf, buflen ) )
		{
			kdSetError ( KD_ENOMEM );
		}

		#endif
	}
	else
	{
		for ( i = 0; i < buflen ; i++ )
		{
			buf[ i ] = (KDuint8) ( rand ( ) % 256 );
		}
	}

	return ret;
}

// kdRand : Returns a pseudo-random integral number in the range 0 to KD_RAND_MAX
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdRand ( KDvoid )
{
	return rand ( );
}

// kdSrand : Initialize random number generator
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSrand ( KDuint seed )
{
	srand ( seed );
}

// kdRandMax : Get Random max number
// XMSoft's revision : Added API.
KD_API KDuint KD_APIENTRY kdRandMax ( KDvoid )
{
	return RAND_MAX;
}

// kdQsort : Sort elements of array
// XMSoft's revision : Added API.
KD_API KDvoid kdQsort ( KDvoid* base, KDsize num, KDsize size, KDint ( *comparator ) ( const KDvoid *, const KDvoid* ) )
{
	qsort ( base, num, size, (int (*) ( const void*, const void* ) ) comparator );
}

// kdBsearch : Binary search in array
// XMSoft's revision : Added API.
KD_API KDvoid* KD_APIENTRY kdBsearch ( const KDvoid* key, const KDvoid* base, KDsize num, KDsize size, KDint ( *comparator ) ( const KDvoid*, const KDvoid* ) )
{
	return bsearch ( (const void*) key, (const void*) base, num, size, (int (*) ( const void*, const void* ) ) comparator );
}

// kdDiv : Integral division
// XMSoft's revision : Added API.
KDDiv kdDiv ( KDint numer, KDint denom )
{
	div_t	d;
	KDDiv	r;

	d = div ( numer, denom );

	r.quot = d.quot;
	r.rem  = d.rem;

	return r;
}