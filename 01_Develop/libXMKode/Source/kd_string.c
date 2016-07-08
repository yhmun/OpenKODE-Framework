/* --------------------------------------------------------------------------
 *
 *      File            kd_string.c
 *      Description     String and memory functions
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

// kdMemchr : Scan memory for a byte value.
KD_API KDvoid* KD_APIENTRY kdMemchr ( const KDvoid* src, KDint byte, KDsize len )
{
	return memchr ( src, byte, len );
}

// kdMemcmp : Compare two memory regions.
KD_API KDint KD_APIENTRY kdMemcmp ( const KDvoid* src1, const KDvoid* src2, KDsize len )
{
	return memcmp ( src1, src2, len );
}

// kdMemcpy : Copy a memory region, no overlapping.
KD_API KDvoid* KD_APIENTRY kdMemcpy ( KDvoid* buf, const KDvoid* src, KDsize len )
{
	return memcpy ( buf, src, len );
}

// kdMemmove : Copy a memory region, overlapping allowed.
KD_API KDvoid* KD_APIENTRY kdMemmove ( KDvoid* buf, const KDvoid* src, KDsize len )
{
	return memmove ( buf, src, len );
}

// kdMemset : Set bytes in memory to a value.
KD_API KDvoid* KD_APIENTRY kdMemset ( KDvoid* buf, KDint byte, KDsize len )
{
	return memset ( buf, byte, len );
}

// kdStrchr : Scan string for a byte value.
KD_API KDchar* KD_APIENTRY kdStrchr ( const KDchar* str, KDint ch )
{
	return strchr ( str, ch );
}

// kdStrrchr : Locate last occurrence of character in string
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrrchr ( const KDchar* str, KDint ch )
{
	return strrchr ( str, ch );
}

// kdStrcmp : Compares two strings.
KD_API KDint KD_APIENTRY kdStrcmp ( const KDchar* str1, const KDchar* str2 )
{
	return strcmp ( str1, str2 );
}

// kdStrcasecmp, kdStrncasecmp : Compare two strings ignoring case.
KD_API KDint KD_APIENTRY kdStrcasecmp ( const KDchar* str1, const KDchar* str2 )
{
	return strcasecmp ( str1, str2 );
}

KD_API KDint KD_APIENTRY kdStrncasecmp ( const KDchar* str1, const KDchar* str2, KDsize maxlen )
{
	return strncasecmp ( str1, str2, maxlen );
}

// kdStrlen : Determine the length of a string.
KD_API KDsize KD_APIENTRY kdStrlen ( const KDchar* str )
{
	return (KDsize) strlen ( str );
}

// kdStrnlen : Determine the length of a string.
KD_API KDsize KD_APIENTRY kdStrnlen ( const KDchar* str, KDsize maxlen )
{
    KDsize      i;

    for ( i = 0; i < maxlen; ++i )
    {
        if ( !str[ i ] )
		{
            return i; 
		}
    }

    return maxlen;
}

// kdStrncat_s : Concatenate two strings.
// XMSoft's revision : Additional APIs.
KD_API KDchar* KD_APIENTRY kdStrcat ( KDchar* buf, const KDchar* src )
{
	return strcat ( buf, src );
}

KD_API KDchar* KD_APIENTRY kdStrncat ( KDchar* buf, const KDchar* src, KDsize maxlen )
{
	return strncat ( buf, src, maxlen );
}

// kdStrncat_s : Concatenate two strings.
KD_API KDint KD_APIENTRY kdStrncat_s ( KDchar* buf, KDsize buflen, const KDchar* src, KDsize srcmaxlen )
{
    KDsize      i          = 0;
    KDsize      d_len      = kdStrlen ( buf );
    KDchar*     append_pos = buf + d_len;
    KDsize      s_len      = kdStrnlen ( src, srcmaxlen );

    if ( !buflen )
	{
        return KD_ERANGE;
	}

    if ( buflen < ( d_len + s_len + 1 ) )
    {
        buf[0] = 0;

        return KD_ERANGE;
    }

    for ( i = 0; i < s_len; ++i )
	{
        append_pos[ i ] = src[ i ];
	}

    append_pos[ s_len ] = 0;

    return 0;
}

// kdStrncmp : Compares two strings with length limit.
KD_API KDint KD_APIENTRY kdStrncmp ( const KDchar* str1, const KDchar* str2, KDsize maxlen )
{	
	return strncmp ( str1, str2, maxlen );
}

// kdStrcpy, kdStrncpy : Copy a string
// XMSoft's revision : Additional APIs.
KD_API KDchar* KD_APIENTRY kdStrcpy ( KDchar* buf, const KDchar* src )
{
	return strcpy ( buf, src );
}

KD_API KDchar* KD_APIENTRY kdStrncpy ( KDchar* buf, const KDchar* src, KDsize maxlen )
{
	return strncpy ( buf, src, maxlen );
}

// kdStrcpy_s : Copy a string with an overrun check.
KD_API KDint KD_APIENTRY kdStrcpy_s ( KDchar* buf, KDsize buflen, const KDchar* src )
{	
     KDsize      i = 0;
    
    if ( !buflen )
	{
        return KD_EINVAL;
	}
    
    for ( i = 0; i < buflen; ++i )
    {
        buf[ i ] = src[ i ];

        if ( !src[ i ] )
		{
            return 0;
		}
    }

    buf[ 0 ] = 0;

    return KD_EINVAL;
}

// kdStrncpy_s : Copy a string with an overrun check.
KD_API KDint KD_APIENTRY kdStrncpy_s ( KDchar* buf, KDsize buflen, const KDchar* src, KDsize srclen )
{
    KDsize      i = 0;

    if ( !buflen )
	{
		return KD_EINVAL;
	}

    if ( buflen <= srclen )
    {
        for ( i = 0; i < buflen; ++i )
        {
            buf[ i ] = src[ i ];

            if ( !src[ i ] )
			{
                return 0;
			}
        }

        buf[ 0 ] = 0;
        
		return KD_EINVAL;
    }

    for ( i = 0; i < srclen; ++i )
    {
        buf[ i ] = src[ i ];

        if ( !src[ i ] )
		{
            return 0;
		}
    }

    buf[ srclen ] = 0;

    return 0;
}

// kdStrtok : A sequence of calls to this function split str into tokens, 
//            which are sequences of contiguous characters separated by any of the characters 
//            that are part of delimiters
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrtok ( KDchar* str, const KDchar* delim )
{
	return strtok ( str, delim );
}

// kdStrstr : Locate substring
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrstr ( const KDchar* str1, const KDchar* str2 )
{
	return strstr ( str1, str2 );
}

// kdStrspn : returns the length of the initial portion of str1 which consists only of 
//             characters that are part of str2.
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdStrspn ( const KDchar* str1, const KDchar* str2 )
{
    return (KDsize) strspn ( str1, str2 );
}

// kdStrcspn : Get span until character in string
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdStrcspn ( const KDchar* str1, const KDchar* str2 )
{
	return (KDsize) strcspn ( str1, str2 );
}

// kdStrdup : returns a pointer to a new string which is a duplicate of the string s.
//            Memory for the new string is obtained with kdMalloc, and can be freed with kdFree
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrdup ( const KDchar* str )
{
	KDchar*   ret = 0;
	KDsize    len = kdStrlen ( str );

	ret = (KDchar *) kdMalloc ( len + 1 );

	kdStrcpy ( ret, str );

	ret[ len ] = '\0';

	return ret;
}

// kdStrndup : The function is similar, but only copies at most n characters.
//             If s is longer than n, only n characters are copied, and a terminating
//             null byte ('\0') is added.
// XMSoft's revision : Added API.
KD_API KDchar* KD_APIENTRY kdStrndup ( const KDchar* str, KDsize maxlen )
{
	KDchar*   ret = 0;
	KDsize    len = kdStrlen ( str );

	if ( len > maxlen )
	{
		len = maxlen;
	}

	ret = (KDchar *) kdMalloc ( len + 1 );

    kdStrncpy ( ret, str, len );

	ret[ len ] = '\0';

	return ret;
}

// kdTolower : Convert uppercase letter to lowercase
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdTolower ( KDint c )
{
	return tolower ( c );
}

// kdToupper : Convert lowercase letter to uppercase
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdToupper ( KDint c )
{
	return toupper ( c );
}

// kdIslower : Check if character is lowercase letter
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIslower ( KDint c )
{
	return islower ( c );
}

// kdIsupper : Check if character is uppercase letter
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsupper ( KDint c )
{
	return isupper ( c );
}

// kdIsupper : Check if character is alphabetic
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsalpha ( KDint c )
{
	return isalpha ( c );
}

// kdIsupper : Check if character is decimal digit
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsdigit ( KDint c )
{
    return isdigit ( c );
}

// kdIsspace : Check if character is a white-space
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsspace ( KDint c )
{
	return isspace ( c );
}

// kdIsprint : Check if character is printable
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsprint ( KDint c )
{
    return isprint ( c );
}

// kdAtoi : Convert string to integer
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdAtoi ( const KDchar* str )
{
	return atoi ( str );
}

// kdAtof : Convert string to float
// XMSoft's revision : Added API.
KD_API KDfloat KD_APIENTRY kdAtof ( const KDchar* str )
{
	return (KDfloat) atof ( str );
}

// kdAtod : Convert string to double
// XMSoft's revision : Added API.
KD_API KDdouble KD_APIENTRY kdAtod ( const KDchar* str )
{
	return atof ( str );
}