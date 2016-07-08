/* -----------------------------------------------------------------------------------
 *
 *      File            AWTextureFilter.cpp
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

#include "Precompiled.h"
#include "AWTextureFilter.h"
#include "CCTexture2DMutable.h"

KDvoid AWTextureFilter::blurInput ( KDvoid* pInput, KDvoid* pOutput, CCTexture2DPixelFormat eFormat, const CCSize& tContentSize, KDint nRadius, const CCRect tRect )
{
    KDint			i, xl, yl, yi, ym, ri, riw;
	KDint			cx = (KDint) tContentSize.cx;
	KDint			cy = (KDint) tContentSize.cy;
	KDint			w  = (KDint) ( tRect.size.cx );
	KDint			h  = (KDint) ( tRect.size.cy );
//	const KDint		wh = (KDint) ( tContentSize.cx * tContentSize.cy );
	KDint			nRead;

	w = ( w == 0 ) ? cx : w;
	h = ( h == 0 ) ? cy : h;
	
	// Check data
	KDint			px = KD_MAX ( 0, (KDint) tRect.origin.x );
	KDint			py = KD_MAX ( 0, (KDint) tRect.origin.y );
    w = px + w - KD_MAX ( 0, ( px + w ) - cx );
	h = py + h - KD_MAX ( 0, ( py + h ) - cy );
    yi = py * cx;
	
	// Generate Gaussian kernel
    nRadius = KD_MIN ( KD_MAX ( 1, nRadius ), 248 );
    KDint	nKernelSize = 1 + nRadius * 2;
    KDint*	pKernel = new KDint [ nKernelSize ];	
    KDint	g = 0, nSum = 0;
	
	// Gaussian filter	
    for ( i = 0; i < nRadius; i++ )
	{
		g = i * i * i + 1;
		pKernel [ i ] = pKernel [ nKernelSize - i - 1 ] = g;
		nSum += g * 2;
    }
	g = nRadius * nRadius;
    pKernel [ nRadius ] = g;
    nSum += g;

	if ( eFormat == kCCTexture2DPixelFormat_RGBA8888 )
	{
		KDint				cr, cg, cb, ca;

		const ccColor4B*	pOriginalData = (ccColor4B*) pInput;
		ccColor4B*			pData = (ccColor4B*) pOutput;
		ccColor4B*			pTemp = (ccColor4B*) kdMalloc ( cx * cy * 4 );
		ccColor4B*			pPixel;
		
		// Horizontal blur
		for ( yl = py; yl < h; yl++ )
		{
			for ( xl = px; xl < w; xl++ )
			{
				cb = cg = cr = ca = 0; 
				ri = xl - nRadius;

				for ( i = 0; i < nKernelSize; i++ )
				{
					nRead = ri + i;
					if ( nRead >= px && nRead < w )
					{
						nRead += yi;
						pPixel = (ccColor4B*) &pOriginalData [ nRead ];
						cr += pPixel->r * pKernel [ i ];
						cg += pPixel->g * pKernel [ i ];
						cb += pPixel->b * pKernel [ i ];
						ca += pPixel->a * pKernel [ i ];
					}
				}

				ri = yi + xl;
				pPixel = &pTemp [ ri ];
				pPixel->r = cr / nSum;
				pPixel->g = cg / nSum;
				pPixel->b = cb / nSum;
				pPixel->a = ca / nSum;
			}
			yi += cx;
		}
		yi = py * cx;
		
		// Vertical blur
		for ( yl = py; yl < h; yl++ )
		{
			ym = yl - nRadius;
			riw = ym * cx;
			for ( xl = px; xl < w; xl++ )
			{
				cb = cg = cr = ca = 0;
				ri = ym;
				nRead = xl + riw;
				for ( i = 0; i < nKernelSize; i++ )
				{
					if ( ri < h && ri >= py )
					{
						pPixel = &pTemp [ nRead ];
						cr += pPixel->r * pKernel [ i ];
						cg += pPixel->g * pKernel [ i ];
						cb += pPixel->b * pKernel [ i ];
						ca += pPixel->a * pKernel [ i ];
					}
					ri++;
					nRead += cx;
				}
				pPixel = &pData [ xl + yi ];
				pPixel->r = cr / nSum;
				pPixel->g = cg / nSum;
				pPixel->b = cb / nSum;
				pPixel->a = ca / nSum;
			}
			yi += cx;
		}

		// Free temp data
		kdFree ( pTemp );		
	}
	else if ( eFormat == kCCTexture2DPixelFormat_A8 )
	{
		KDint		ca;

		const KDubyte*	pOriginalData = (const KDubyte*) pInput;
		KDubyte*		pData = (KDubyte*) pOutput;
		KDubyte*		pTemp = (KDubyte*) kdMalloc ( cx * cy );
		
		// Horizontal blur
		for ( yl = py; yl < h; yl++ )
		{
			for ( xl = px; xl < w; xl++ )
			{
				ca = 0;
				ri = xl - nRadius;
				for ( i = 0; i < nKernelSize; i++ )
				{
					nRead = ri + i;
					if ( nRead >= px && nRead < w )
					{
						nRead += yi;
						ca += pOriginalData [ nRead ] * pKernel [ i ];
					}
				}
				ri = yi + xl;
				pTemp [ ri ] = ca / nSum;
			}
			yi += cx;
		}
		yi = py * cx;
		
		// Vertical blur
		for ( yl = py; yl < h; yl++ )
		{
			ym = yl - nRadius;
			riw = ym * cx;
			for ( xl = px; xl < w; xl++ )
			{
				ca = 0;
				ri = ym;
				nRead = xl + riw;
				for ( i = 0; i < nKernelSize; i++ )
				{
					if ( ri < h && ri >= py )
					{
						ca += pTemp [ nRead ] * pKernel [ i ];
					}
						
					ri++;
					nRead += cx;
				}
				pData [ xl + yi ] = ca / nSum;
			}
			yi += cx;
		}
		
		// Free temp data
		kdFree ( pTemp );		
	}
	else
	{
		CCAssert ( KD_FALSE, "AWTextureFilter : Pixel format don't supported. It should be RGBA8888 or A8" );
	}

	delete [] pKernel;
}

CCTexture2DMutable* AWTextureFilter::blur ( CCTexture2DMutable* pTexture, KDint nRadius )
{
	return AWTextureFilter::blur ( pTexture, nRadius, CCRectZero );
}

CCTexture2DMutable* AWTextureFilter::blur ( CCTexture2DMutable* pTexture, KDint nRadius, const CCRect& tRect )
{
	if ( !pTexture )
	{
		return KD_NULL;
	}
	
	KDvoid*		pInput = pTexture->getTexData ( );
	
	// Apply the effect to the texture
	AWTextureFilter::blurInput
	(
		pInput,
		pInput,
		pTexture->getPixelFormat ( ),
		pTexture->getContentSize ( ),
		nRadius,
		tRect
	);
	
	// Update the GPU data
	pTexture->apply ( );
	
	return pTexture;	
}
