/* -----------------------------------------------------------------------------------
 *
 *      File            CCString.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCString_h__
#define __CCString_h__

#include <string>
#include <functional>
#include "CCObject.h"

NS_CC_BEGIN

/**
 *	@addtogroup data_structures
 *	@{
 */

class CC_DLL String : public Object, public Clonable
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		String ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		String ( const KDchar* pStr );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		String ( const std::string& rStr );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		String ( const String& rStr );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~String ( KDvoid );
    
		/**
		 *	override assignment operator 
		 *	@js NA
		 *	@lua NA
		 */
		String&				operator= ( const String& rOther );

		/**
		 *	init a string with format, it's similar with the c function 'sprintf' 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithFormat ( const KDchar* pFormat, ... ) CC_FORMAT_PRINTF ( 2, 3 );

		/** 
		 *	convert to int value 
		 *	@js NA
		 */
		KDint				intValue ( KDvoid ) const;

		/** 
		 *	convert to unsigned int value 
		 *	@js NA
		 */
		KDuint				uintValue ( KDvoid ) const;

		/**
		 *	convert to float value 
		 *	@js NA
		 */
		KDfloat				floatValue ( KDvoid ) const;

		/** 
		 *	convert to double value 
		 *	@js NA
		 */
		KDdouble			doubleValue ( KDvoid ) const;

		/** 
		 *	convert to bool value 
		 *	@js NA
		 */
		KDbool				boolValue ( KDvoid ) const;

		/** 
		 *	get the C string 
		 *	@js NA
		 */
		const KDchar*		getCString ( KDvoid ) const;

		/**
		 *	get the length of string 
		 *	@js NA
		 */
		KDuint				length ( KDvoid ) const;

		/** 
		 *	compare to a c string 
		 *	@js NA
		 */
		KDint				compare ( const KDchar* ) const;

		/** 
		 *	append additional characters at the end of its current value 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				append ( const std::string& rStr );

		/** 
		 *	append(w/ format) additional characters at the end of its current value 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				appendWithFormat ( const KDchar* pFormat, ... );

		/** 
		 *	split a string 
		 *	@js NA
		 *	@lua NA
		 */
		Array*				componentsSeparatedByString ( const KDchar* pDelimiter );
    
		/**
		 *	override functions 
		 *	@js NA
		 */
		virtual KDbool		isEqual ( const Object* pObject );

		/** 
		 *	create a string with std string, you can also pass a c string pointer because the default constructor of std::string can access a c string pointer. 
		 *  @return A String pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 *	@js NA
		 */
		static String*		create ( const std::string& rStr );

		/**
		 *	create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
		 *  if you want to change it, you should modify the kMaxStringLen macro in String.cpp file.
		 *  @return A String pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 *	@js NA
		 */
		static String*		createWithFormat ( const KDchar* pFormat, ... ) CC_FORMAT_PRINTF ( 1, 2 );

		/** 
		 *	create a string with binary data 
		 *  @return A String pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 *	@js NA
		 */
		static String*		createWithData ( const KDubyte* pData, KDsize uLen );

		/** 
		 *	create a string with a file, 
		 *  @return A String pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 *	@js NA
		 */
		static String*		createWithContentsOfFile ( const KDchar* pFilename );

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual KDvoid		acceptVisitor ( DataVisitor& tVisitor );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual String*		clone ( KDvoid ) const;
    
	private :

		/** only for internal use */
		KDbool				initWithFormatAndValist ( const KDchar* pFormat, KDVaListKHR pArgs );

	public :

		std::string			m_sString;
};

struct StringCompare : public std::binary_function<String*, String*, KDbool> 
{
    public :

        KDbool				operator ( ) ( String * a, String * b ) const 
		{
            return kdStrcmp ( a->getCString ( ), b->getCString ( ) ) < 0;
        }
};

#define StringMake( str )			String::create ( str )
#define ccs							StringMake
#define ccszf( _format_, ... )		String::createWithFormat ( _format_, ##__VA_ARGS__ )->getCString ( )

//	end of data_structure group
/// @}

NS_CC_END

#endif	// __CCString_h__
