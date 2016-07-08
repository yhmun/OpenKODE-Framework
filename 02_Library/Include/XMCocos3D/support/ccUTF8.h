/* -----------------------------------------------------------------------------------
 *
 *      File            ccUTF8.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __ccUTF8_h__
#define __ccUTF8_h__

#include "../ccMacros.h"
#include <vector>

NS_CC_BEGIN

CC_DLL KDint					cc_wcslen ( const KDushort* str );

CC_DLL KDvoid					cc_utf8_trim_ws ( std::vector<KDushort>* str );

/**
 *	Whether the character is a whitespace character.
 *
 *	@param ch    the unicode character
 *	@returns     whether the character is a white space character.
 *
 *	@see http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 * 
 */
CC_DLL KDbool					isspace_unicode ( KDushort ch );

/**
 *	Returns the length of the string in characters.
 *
 *	@param p     pointer to the start of a UTF-8 encoded string.
 *	@param max   the maximum number of bytes to examine. If \p max is less than
 *               0, then the string is assumed to be null-terminated. If \p max
 *               is 0, \p p will not be examined and my be %NULL.
 *
 *	@returns the length of the string in characters
 **/
CC_DLL KDint32					cc_utf8_strlen ( const KDchar* p, KDint max );

/**
 *	Find the last character that is not equal to the character given.
 *
 *	@param str   the string to be searched.
 *	@param c     the character to be searched for.
 *
 *	@returns the index of the last character that is not \p c.
 * 
 */
CC_DLL KDuint					cc_utf8_find_last_not_char ( std::vector<KDushort> str, KDushort c );

CC_DLL std::vector<KDushort>	cc_utf16_vec_from_utf16_str ( const KDushort* str );

/**
 *	Creates a utf8 string from a cstring.
 *
 *	@param str_old   pointer to the start of a C string.
 *
 *	@returns the newly created utf8 string.
 * */
CC_DLL KDushort*				cc_utf8_to_utf16 ( const KDchar* str_old, KDint length = -1, KDint* rUtf16Size = KD_NULL );

/**
 *	Convert a string from UTF-16 to UTF-8. The result will be null terminated.
 *
 *	@param str   a UTF-16 encoded string
 *	@param len   the maximum length of \p str to use. If \p len < 0, then the
 *               string is null terminated.
 *	@param items_read    location to store number of words read, or %NULL.
 *                       If %NULL, then %G_CONVERT_ERROR_PARTIAL_INPUT will be
 *                       returned in case \p str contains a trailing partial
 *                       character. If an error occurs then the index of the
 *                       invalid input is stored here.
 *  @param items_written location to store number of bytes written, or %NULL.
 *                       The value stored here does not include the trailing null.
 *
 *  @returns a pointer to a newly allocated UTF-8 string. This value must be
 *           freed with free(). If an error occurs, %NULL will be returned.
 **/
CC_DLL KDchar*					cc_utf16_to_utf8 ( const KDushort* str, KDint32 len, KDint32* items_read, KDint32* items_written );

NS_CC_END

#endif	// __ccUTF8_h__
