/* -----------------------------------------------------------------------------------
 *
 *      File            ccUTF8.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *		Copyright (c) 2013      James Chen ( 2/27/13 )
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/ccUTF8.h"

NS_CC_BEGIN
    
KDint cc_wcslen ( const KDushort* str )
{
    KDint  i = 0;
    
	while ( *str++ )
	{
		i++;
	}

    return i;
}

/* Code from GLIB gutf8.c starts here. */
#define UTF8_COMPUTE( Char, Mask, Len )		\
	if ( Char < 128 )						\
	{										\
		Len = 1;							\
		Mask = 0x7f;						\
    }										\
	else if ( ( Char & 0xe0 ) == 0xc0 )		\
	{										\
		Len = 2;							\
		Mask = 0x1f;						\
	}										\
	else if ( ( Char & 0xf0 ) == 0xe0 )		\
	{										\
		Len = 3;							\
		Mask = 0x0f;						\
	}										\
	else if ( ( Char & 0xf8 ) == 0xf0 )		\
	{										\
		Len = 4;							\
		Mask = 0x07;						\
	}										\
	else if ( ( Char & 0xfc ) == 0xf8 )		\
	{										\
		Len = 5;							\
		Mask = 0x03;						\
	}										\
	else if ( ( Char & 0xfe ) == 0xfc )		\
	{										\
		Len = 6;							\
		Mask = 0x01;						\
	}										\
	else									\
	{										\
		Len = -1;							\
	}

#define UTF8_LENGTH( Char )					\
	( (Char) < 0x80 ? 1 :					\
	( (Char) < 0x800 ? 2 :					\
	( (Char) < 0x10000 ? 3 :				\
	( (Char) < 0x200000 ? 4 :				\
	( (Char) < 0x4000000 ? 5 : 6 ) ) )  ) )


#define UTF8_GET( Result, Chars, Count, Mask, Len )		\
	(Result) = (Chars) [0] & (Mask);					\
	for ( (Count) = 1; (Count) < (Len); ++(Count) )		\
	{													\
		if ( ( (Chars) [(Count)] & 0xc0 ) != 0x80 )		\
		{												\
			(Result) = -1;								\
			break;										\
		}												\
		(Result) <<= 6;									\
		(Result) |= ( (Chars) [(Count)] & 0x3f );		\
	}

#define UNICODE_VALID( Char )							\
	( (Char) < 0x110000 &&								\
	( ( (Char) & 0xFFFFF800) != 0xD800 ) &&				\
	( (Char) < 0xFDD0 || (Char) > 0xFDEF ) &&			\
	( (Char) & 0xFFFE) != 0xFFFE )


static const KDchar utf8_skip_data [256] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
	5, 5, 5, 6, 6, 1, 1
};

static const KDchar* const g_utf8_skip = utf8_skip_data;

#define cc_utf8_next_char( p )	(KDchar*) ( (p) + g_utf8_skip [*(KDubyte*) (p)] )

/**
 *	@str:	the string to search through.
 *	@c:		the character to not look for.
 * 
 *	Return value: the index of the last character that is not c.
 */
KDuint cc_utf8_find_last_not_char ( std::vector<KDushort> str, KDushort c )
{
	KDint  len = str.size ( );

	KDint  i = len - 1;
	for ( ; i >= 0; --i )
	{
		if ( str[i] != c )
		{
			return i;
		}
	}

	return i;
}

/**
 *	@str:	the string to trim
 *	@index:	the index to start trimming from.
 * 
 *	Trims str st str=[0, index) after the operation.
 * 
 *	Return value: the trimmed string.
 */
KDvoid cc_utf8_trim_from ( std::vector<KDushort>* str, KDint index )
{
	KDint  size = str->size ( );
	if ( index >= size || index < 0 )
	{
		return;
	}

	str->erase ( str->begin ( ) + index, str->begin ( ) + size );
}

/**
 *	@ch is the unicode character whitespace?
 * 
 *	Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 * 
 *	Return value: weather the character is a whitespace character.
 */
KDbool isspace_unicode ( KDushort ch )
{
	return ( ch >= 0x0009 && ch <= 0x000D ) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
		|| ( ch >= 0x2000 && ch <= 0x200A ) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
		||   ch == 0x205F || ch == 0x3000;
}

KDvoid  cc_utf8_trim_ws ( std::vector<KDushort>* str )
{
	KDint  len = str->size ( );

	if ( len <= 0 )
	{
		return;
	}

	KDint  last_index = len - 1;

	// Only start trimming if the last character is whitespace..
	if ( isspace_unicode ( (*str)[last_index] ) )
	{
		for ( KDint i = last_index - 1; i >= 0; --i )
		{
			if ( isspace_unicode ( (*str)[i] ) )
			{
				last_index = i;
			}
			else
			{
				break;
			}
		}

		cc_utf8_trim_from ( str, last_index );
	}
}

/**
 *	g_utf8_strlen:
 *	@p: pointer to the start of a UTF-8 encoded string.
 *	@max: the maximum number of bytes to examine. If @max
 *        is less than 0, then the string is assumed to be
 *        null-terminated. If @max is 0, @p will not be examined and
 *        may be %NULL.
 *
 *	Returns the length of the string in characters.
 *
 *	Return value: the length of the string in characters
 **/
KDint cc_utf8_strlen ( const KDchar* p, KDint max )
{
	KDint			len = 0;
	const KDchar*	start = p;

	if ( !( p != KD_NULL || max == 0 ) )
	{
		return 0;
	}

	if ( max < 0 )
    {
		while ( *p )
		{
			p = cc_utf8_next_char ( p );
			++len;
		}
    }
	else
    {
		if ( max == 0 || !*p )
		{
			return 0;
		}

		p = cc_utf8_next_char ( p );

		while ( p - start < max && *p )
		{
			++len;
			p = cc_utf8_next_char ( p );
		}

		/* only do the last len increment if we got a complete
		 * char (don't count partial chars)
		 */
		if ( p - start == max )
		{
			++len;
		}
    }

	return len;
}

/**
 *  g_utf8_get_char:
 *  @p: a pointer to Unicode character encoded as UTF-8
 *
 *  Converts a sequence of bytes encoded as UTF-8 to a Unicode character.
 *  If @p does not point to a valid UTF-8 encoded character, results are
 *  undefined. If you are not sure that the bytes are complete
 *  valid Unicode characters, you should use g_utf8_get_char_validated()
 *  instead.
 *
 *  Return value: the resulting character
 **/
KDuint cc_utf8_get_char ( const KDchar* p )
{
	KDint	i, mask = 0, len;
	KDuint	result;
	KDubyte c = (KDubyte) *p;

	UTF8_COMPUTE ( c, mask, len );
	if ( len == -1 )
	{
		return (KDuint) -1;
	}

	UTF8_GET ( result, p, i, mask, len );

	return result;
}

KDushort* cc_utf8_to_utf16 ( const KDchar* str_old, KDint length, KDint* rUtf16Size )
{
    KDint     len = cc_utf8_strlen ( str_old, length );
    if ( rUtf16Size != KD_NULL )
	{
        *rUtf16Size = len;
    }

    KDushort* str_new = new KDushort [ len + 1 ];
    str_new [ len ] = 0;
    
    for ( KDint i = 0; i < len; ++i )
    {
        str_new[i] = cc_utf8_get_char ( str_old );
        str_old = cc_utf8_next_char ( str_old );
    }
    
    return str_new;
}

std::vector<KDushort> cc_utf16_vec_from_utf16_str ( const KDushort* str )
{
	KDint  len = cc_wcslen ( str );
	std::vector<KDushort>  str_new;

	for ( KDint i = 0; i < len; ++i )
	{
		str_new.push_back ( str[i] );
	}

	return str_new;
}

/**
 *  cc_unichar_to_utf8:
 *  @c: a ISO10646 character code
 *  @outbuf: output buffer, must have at least 6 bytes of space.
 *        If %NULL, the length will be computed and returned
 *        and nothing will be written to @outbuf.
 * 
 *  Converts a single character to UTF-8.
 *
 *  Return value: number of bytes written
 **/
KDint cc_unichar_to_utf8 ( KDushort c, KDchar* outbuf )
{
    KDuint  len = 0;
    KDint   first;
    KDint   i;
    
    if ( c < 0x80 )
    {
        first = 0;
        len = 1;
    }
    else if ( c < 0x800 )
    {
        first = 0xc0;
        len = 2;
    }
    else if ( c < 0x10000 )
    {
        first = 0xe0;
        len = 3;
    }
    else if ( c < 0x200000 )
    {
        first = 0xf0;
        len = 4;
    }
    else if ( c < 0x4000000 )
    {
        first = 0xf8;
        len = 5;
    }
    else
    {
        first = 0xfc;
        len = 6;
    }
    
    if ( outbuf )
    {
        for ( i = len - 1; i > 0; --i )
        {
            outbuf[i] = ( c & 0x3f ) | 0x80;
            c >>= 6;
        }
        outbuf[0] = c | first;
    }
    
    return len;
}

#define SURROGATE_VALUE(h,l) (((h) - 0xd800) * 0x400 + (l) - 0xdc00 + 0x10000)

/**
 *  cc_utf16_to_utf8:
 *  @str: a UTF-16 encoded string
 *  @len: the maximum length of @str to use. If @len < 0, then
 *        the string is terminated with a 0 character.
 *  @items_read: location to store number of words read, or %NULL.
 *               If %NULL, then %G_CONVERT_ERROR_PARTIAL_INPUT will be
 *               returned in case @str contains a trailing partial
 *               character. If an error occurs then the index of the
 *               invalid input is stored here.
 *  @items_written: location to store number of bytes written, or %NULL.
 *                  The value stored here does not include the trailing
 *                  0 byte.
 *  @error: location to store the error occuring, or %NULL to ignore
 *          errors. Any of the errors in #GConvertError other than
 *          %G_CONVERT_ERROR_NO_CONVERSION may occur.
 * 
 *  Convert a string from UTF-16 to UTF-8. The result will be
 *  terminated with a 0 byte.
 * 
 *  Return value: a pointer to a newly allocated UTF-8 string.
 *                This value must be freed with free(). If an
 *                error occurs, %NULL will be returned and
 *                @error set.
 **/
KDchar* cc_utf16_to_utf8 ( const KDushort* str, KDint len, KDint* items_read, KDint* items_written )
{ 
    /* This function and g_utf16_to_ucs4 are almost exactly identical - The lines that differ
     * are marked.
     */
    const KDushort*	in;
    KDchar*			out;
    KDchar*			result = KD_NULL;
    KDint			n_bytes;
    KDushort		high_surrogate;
    
    if ( str == 0 ) 
	{
		return KD_NULL;
	}
    
    n_bytes = 0;
    in = str;
    high_surrogate = 0;
    while ( ( len < 0 || in - str < len ) && *in )
    {
        KDushort  c = *in;
        KDushort  wc;
        
        if ( c >= 0xdc00 && c < 0xe000 )		// low surrogate 
        {
            if ( high_surrogate )
            {
                wc = SURROGATE_VALUE ( high_surrogate, c );
                high_surrogate = 0;
            }
            else
            {
                CCLOGERROR ( "Invalid sequence in conversion input" );
                goto err_out;
            }
        }
        else
        {
            if ( high_surrogate )
            {
                CCLOGERROR ( "Invalid sequence in conversion input" );
                goto err_out;
            }
            
            if ( c >= 0xd800 && c < 0xdc00 )	// high surrogate 
            {
                high_surrogate = c;
                goto next1;
            }
            else
			{
                wc = c;
			}
        }
        
        // DIFFERENT for UTF8/UCS4 
        n_bytes += UTF8_LENGTH ( wc );
        
    next1 :

        in++;
    }
    
    if ( high_surrogate && !items_read )
    {        
        CCLOGERROR ( "Partial character sequence at end of input" );
        goto err_out;
    }
    
    // At this point, everything is valid, and we just need to convert
    
    // DIFFERENT for UTF8/UCS4 
    result = new KDchar [ n_bytes + 1 ];
    
    high_surrogate = 0;
    out = result;
    in = str;
    while ( out < result + n_bytes )
    {
        KDushort  c = *in;
        KDushort  wc;
        
        if ( c >= 0xdc00 && c < 0xe000 )			// low surrogate 
        {
            wc = SURROGATE_VALUE ( high_surrogate, c );
            high_surrogate = 0;
        }
        else if ( c >= 0xd800 && c < 0xdc00 )		// high surrogate 
        {
            high_surrogate = c;
            goto next2;
        }
        else
		{
            wc = c;
		}
        
        // DIFFERENT for UTF8/UCS4 
        out += cc_unichar_to_utf8 ( wc, out );
        
    next2:
        in++;
    }
    
    // DIFFERENT for UTF8/UCS4 
    *out = '\0';
    
    if ( items_written )
	{
		// DIFFERENT for UTF8/UCS4 
        *items_written = out - result;
	}
    
err_out :

    if ( items_read )
	{
        *items_read = in - str;
	}
    
    return result;
}



/**
 *  @str:	the string to search through.
 *  @c:		the character to find.
 * 
 *  Returns the index of the first occurrence of the character, if found.  Otherwise -1 is returned.
 * 
 *  Return value: the index of the first occurrence of the character if found or -1 otherwise.
 */
KDuint cc_utf8_find_char ( std::vector<KDushort> str, KDushort c )
{
	KDuint  len = str.size ( );

	for ( KDuint i = 0; i < len; ++i )
	{
		if ( str[i] == c )
		{
			return i;
		}
	}

	return -1;
}


/**
 *  cc_utf16_from_utf8:
 *  @str_old: pointer to the start of a C string.
 * 
 *  Creates a utf8 string from a cstring.
 * 
 *  Return value: the newly created utf8 string.
 */
KDushort* cc_utf16_from_utf8 ( const KDchar* str_old )
{
	KDint	len = cc_utf8_strlen ( str_old, -1 );

	KDushort* str_new = new KDushort [len + 1];
	str_new [len] = 0;

	for ( KDint i = 0; i < len; ++i )
	{
		str_new [i] = cc_utf8_get_char ( str_old );
		str_old = cc_utf8_next_char ( str_old );
	}

	return str_new;
}

NS_CC_END