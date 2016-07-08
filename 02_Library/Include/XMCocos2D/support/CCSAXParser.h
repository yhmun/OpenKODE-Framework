/* -----------------------------------------------------------------------------------
 *
 *      File            CCSAXParser.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      ¬®¬Ñ¬Ü¬ã¬Ú¬Þ ¬¡¬Ü¬ã¬Ö¬ß¬à¬Ó
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

#ifndef __CCSAXParser_h__
#define __CCSAXParser_h__

#include "../ccTypes.h"

NS_CC_BEGIN
    
/**
 * @addtogroup support
 * @{
 */

class CCSAXDelegator
{
	public :

		virtual KDvoid		startElement ( KDvoid* pCtx, const KDchar* szFilePath, const KDchar** pAtts ) = 0;
		virtual KDvoid		endElement   ( KDvoid* pCtx, const KDchar* szFilePath ) = 0;
		virtual KDvoid		textHandler  ( KDvoid* pCtx, const KDchar* szFilePath, KDint nLen ) = 0;
};

class CCSAXParser
{
	public :
				 CCSAXParser ( KDvoid );
		virtual ~CCSAXParser ( KDvoid );

	public :

		virtual KDbool		init ( const KDchar* szEncoding );

		KDbool				parse ( const KDchar* pXMLData, KDuint uDataLength );
		KDbool				parse ( const KDchar* szFilePath );

		KDvoid				setDelegator ( CCSAXDelegator* pDelegator );

		static  KDvoid		startElement ( KDvoid* pCtx, const KDchar* szFilePath, const KDchar** pAtts );
		static  KDvoid		endElement   ( KDvoid* pCtx, const KDchar* szFilePath );
		static  KDvoid		textHandler  ( KDvoid* pCtx, const KDchar* szFilePath, KDint nLen );

	protected :

		CCSAXDelegator*		 m_pDelegator;
};

// end of support group
/// @}

NS_CC_END

#endif // __CCSAXParser_h__