/* -----------------------------------------------------------------------------------
 *
 *      File            CCString.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCString_h__
#define __CCString_h__

#include "CCObject.h"

#include <string>
#include <functional>

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class CCString : public CCObject
{
	public :		
				 CCString ( KDvoid );		
				 CCString ( const KDchar* szStr );
				 CCString ( const std::string& sStr );
				 CCString ( const CCString& tStr );
		virtual ~CCString ( KDvoid );
	
		/* static functions */

		/**
		 *	create a string with std string, you can also pass a c string pointer because the default constructor of std::string can access a c string pointer. 
		 *  @return A CCString pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 */
		static CCString*	create ( const std::string& sStr );

		/** 
		 *	create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
		 *  if you want to change it, you should modify the kMaxStringLen macro in CCString.cpp file.
		 *  @return A CCString pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 */ 
		static CCString*	createWithFormat ( const KDchar* szFormat, ... );

		/**
		 *	create a string with binary data 
		 *  @return A CCString pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 */
		static CCString*	createWithData ( const KDubyte* pData, KDsize nLen );

		/** 
		 *	create a string with a file, 
		 *  @return A CCString pointer which is an autorelease object pointer,
		 *          it means that you needn't do a release operation unless you retain it.
		 */
		static CCString*	createWithContentsOfFile ( const KDchar* szFileName );
	
	public :
			
		/* override assignment operator */
		CCString&			operator= ( const CCString& tOther );

		/** init a string with format, it's similar with the c function 'sprintf' */ 
		virtual KDbool		initWithFormat ( const KDchar* szFormat, ... );

		/** convert to int value */
		KDint				intValue ( KDvoid ) const;

		/** convert to unsigned int value */
		KDuint				uintValue ( KDvoid ) const;

		/** convert to float value */
		KDfloat				floatValue ( KDvoid ) const;

		/** convert to double value */
		KDdouble			doubleValue ( KDvoid ) const;

		/** convert to bool value */
		KDbool				boolValue ( KDvoid ) const;

		/** get the C string */
		const KDchar*		getCString ( KDvoid ) const;

		/** get the length of string */
		KDuint				length ( KDvoid ) const;

		/** compare to a c string */
		KDint				compare ( const KDchar* szStr ) const;

		/* override functions */
		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDbool		isEqual ( const CCObject* pObject );

		virtual KDvoid		acceptVisitor ( CCDataVisitor& tVisitor );

	private :

		/** only for internal use */
		KDbool				initWithFormatAndValist ( const KDchar* szFormat, KDVaListKHR pArgs );

	public :

		std::string			m_sString;
};

struct CCStringCompare : public std::binary_function<CCString*, CCString*, KDbool>
{
    public :

        KDbool		operator ( ) (CCString* pA, CCString* pB ) const 
		{
            return kdStrcmp ( pA->getCString ( ), pB->getCString ( ) ) < 0;
        }
};

#define CCStringMake( _str_ )		CCString::create ( _str_ )
#define ccstr						CCStringMake
#define ccszf( _format_, ... )		CCString::createWithFormat ( _format_, ##__VA_ARGS__ )->getCString ( )

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCString_h__
