/* -----------------------------------------------------------------------------------
 *
 *      File            ccUTF8.h
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

#ifndef __ccUTF8_h__
#define __ccUTF8_h__

#include "../ccTypes.h"
#include <vector>

NS_CC_BEGIN

KDint		cc_wcslen ( const KDushort* str );

KDvoid		cc_utf8_trim_ws ( std::vector<KDushort>* str );

/**
 *	@ch is the unicode character whitespace?
 *
 *	Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 *	Return value: weather the character is a whitespace character.
 */
KDbool		isspace_unicode ( KDushort ch );

/**
 *  cc_utf8_strlen:
 *  @p: pointer to the start of a UTF-8 encoded string.
 *  @max: the maximum number of bytes to examine. If @max
 *        is less than 0, then the string is assumed to be
 *        null-terminated. If @max is 0, @p will not be examined and
 *        may be %NULL.
 *
 *  Returns the length of the string in characters.
 *
 *  Return value: the length of the string in characters
 **/
KDint		cc_utf8_strlen ( const KDchar* p, KDint max );


/**
 *  @str:    the string to search through.
 *  @c:        the character to not look for.
 *
 *  Return value: the index of the last character that is not c.
 */
KDuint		cc_utf8_find_last_not_char ( std::vector<KDushort> str, KDushort c );

std::vector<KDushort>	cc_utf16_vec_from_utf16_str ( const KDushort* str );

/**
 *  cc_utf8_to_utf16:
 *  @str_old: pointer to the start of a C string.
 *
 *  Creates a utf8 string from a cstring.
 * 
 *  Return value: the newly created utf8 string.
 */
KDushort*	cc_utf8_to_utf16 ( const KDchar* str_old, KDint length = -1, KDint* rUtf16Size = KD_NULL );

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
KDchar*		cc_utf16_to_utf8 ( const KDushort* str, KDint len, KDint* items_read, KDint* items_written );


KDint		cc_unichar_to_utf8 ( KDushort c, KDchar* outbuf );

KDuint		cc_utf8_find_char ( std::vector<KDushort> str, KDushort c );

KDushort*	cc_utf16_from_utf8 ( const KDchar* str_old );

NS_CC_END

#endif	// __ccUTF8_h__
