/* -----------------------------------------------------------------------------------
 *
 *      File            base64.cpp
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

#include "Precompiled.h"
#include "support/base64.h"

NS_CC_BEGIN
    
static KDubyte  l_szAlphabet [ ] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

KDint _base64Decode( KDubyte* pSrc, KDuint uLenSrc, KDubyte* pDst, KDuint* pLenDst )
{
    static KDchar  szAlphabet [ 256 ], szDecoder[256];
    
    KDint   i, nBits, nChar = 0, nCharCount, nErrors = 0;
	KDuint  uIndexSrc  = 0;
	KDuint  uIndexDst  = 0;

    for ( i = ( sizeof ( l_szAlphabet ) ) - 1; i >= 0 ; i-- )
	{
		szAlphabet [ l_szAlphabet[ i ] ] = 1;
         szDecoder [ l_szAlphabet[ i ] ] = i;
    }

    nCharCount = 0;
    nBits = 0;
	for ( uIndexSrc = 0; uIndexSrc < uLenSrc ; uIndexSrc++ ) 
	{
		nChar = pSrc [ uIndexSrc ];
		if ( nChar == '=' )
		{
			break;
		}

		if ( nChar > 255 || ! szAlphabet [ nChar ] )
		{
			continue;
		}

		nBits += szDecoder [ nChar ];
		nCharCount++;
		if ( nCharCount == 4 )
		{
			pDst [ uIndexDst++ ] = (   nBits >> 16 );
			pDst [ uIndexDst++ ] = ( ( nBits >> 8 ) & 0xff );
			pDst [ uIndexDst++ ] = (   nBits & 0xff );
			nBits = 0;
			nCharCount = 0;
		} 
		else
		{
			nBits <<= 6;
		}
    }
	
	if ( nChar == '=' )
	{
		switch ( nCharCount ) 
		{
			case 1 :
				kdPrintf ( "base64Decode: encoding incomplete: at least 2 bits missing" );
				nErrors++;
				break;

			case 2 :
				pDst [ uIndexDst++ ] = ( nBits >> 10 );
				break;

			case 3 :
				pDst [ uIndexDst++ ] = ( nBits >> 16 );
				pDst [ uIndexDst++ ] = ( ( nBits >> 8 ) & 0xff );
				break;
		}
	}
	else if ( uIndexSrc < uLenSrc )
	{
		if ( nCharCount ) 
		{
			kdPrintf ( "base64 encoding incomplete: at least %d bits truncated", ( ( 4 - nCharCount ) * 6 ) );
			nErrors++;
		}
    }
	
	*pLenDst = uIndexDst;
	return nErrors;
}

KDint base64Decode ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst )
{
	KDuint   uLenDst = 0;
	
	//should be enough to store 6-bit buffers in 8-bit buffers
	*ppDst = new KDubyte [ (KDsize) ( uLenSrc * 3.0f / 4.0f + 1 ) ];
	if ( *ppDst )
	{
		KDint ret = _base64Decode ( pSrc, uLenSrc, *ppDst, &uLenDst );
		
		if ( ret > 0 )
		{			
			kdPrintf ( "Base64Utils: error decoding" );

			CC_SAFE_DELETE_ARRAY ( *ppDst );		
			uLenDst = 0;
		}
	}

    return uLenDst;
}


NS_CC_END