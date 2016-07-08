/* -----------------------------------------------------------------------------------
 *
 *      File            CCSAXParser.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Максим Аксенов
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

#ifndef __CCSAXParser_h__
#define __CCSAXParser_h__

#include "CCPlatformConfig.h"
#include "platform/CCCommon.h"

NS_CC_BEGIN

/**
 *	@addtogroup platform
 *	@{
 */

typedef KDubyte		CC_XML_CHAR;

class CC_DLL SAXDelegator
{
	public :

		virtual ~SAXDelegator ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid		startElement ( KDvoid *ctx, const KDchar* name, const KDchar** atts ) = 0;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid		endElement ( KDvoid* ctx, const KDchar* name ) = 0;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid		textHandler ( KDvoid* ctx, const KDchar* s, KDint len ) = 0;
};

class CC_DLL SAXParser
{
		SAXDelegator*		m_pDelegator;

	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		SAXParser ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~SAXParser ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				init ( const KDchar* pszEncoding );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				parse ( const KDchar* pXMLData, KDuint uDataLength );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				parse ( const KDchar* pszFile );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				setDelegator ( SAXDelegator* pDelegator );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid		startElement ( KDvoid* ctx, const CC_XML_CHAR* name, const CC_XML_CHAR** atts );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid		endElement ( KDvoid* ctx, const CC_XML_CHAR* name );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid		textHandler ( KDvoid* ctx, const CC_XML_CHAR* name, KDint len );
};

//	end of platform group
/// @}

NS_CC_END

#endif	// __CCSAXParser_h__
