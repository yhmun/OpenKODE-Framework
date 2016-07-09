/* -----------------------------------------------------------------------------------
 *
 *      File            AWTextureFilter.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      ForzeField Studios S.L. All rights reserved.
 *
 *      AWSuite : http://forzefield.com
 * 
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * -------------------------------------------------------------------------------- */ 

#ifndef __AWTextureFilter_h__
#define __AWTextureFilter_h__

class CCTexture2DMutable;

class AWTextureFilter
{
	public :

		/**
		 *	@param input: Original texture data
		 *	@param output: Empty (or not) buffer
		 *  @param format: Pixel format of the data
		 *	@param width: Real width (is a power of two)
		 *	@param height: Real height (is a power of two)
		 *	@param position: Top left vertex of the blur effect
		 *	@param size: The size of the blur effect
		 *	@param contentSize: 
		 *	@param radios: It's the radius of the blur effect
		 */   

		static  KDvoid					blurInput ( KDvoid* pInput, KDvoid* pOutput, CCTexture2DPixelFormat eFormat, const CCSize& tContentSize, KDint nRadius, const CCRect tRect );

		static  CCTexture2DMutable*		blur ( CCTexture2DMutable* pTexture, KDint nRadius, const CCRect& tRect );
		static  CCTexture2DMutable*		blur ( CCTexture2DMutable* pTexture, KDint nRadius );
};

#endif	// __AWTextureFilter_h__
