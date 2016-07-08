/* -----------------------------------------------------------------------------------
 *
 *      File            CCImage.h
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

#ifndef __CCImage_h__
#define __CCImage_h__

#include "../cocoa/CCObject.h"
#include "../textures/CCTexture2D.h"

NS_CC_BEGIN

/**
 * @addtogroup support
 * @{
 */

class CCImage : public CCObject
{
	public :
				 CCImage ( KDvoid );
        virtual ~CCImage ( KDvoid );

		static   CCImage*	create ( KDvoid );

		static   CCImage*	create ( const KDchar* szPath, CCTexture2DPixelFormat eFormat = kCCTexture2DPixelFormat_Automatic );

    public :
    
		/** 
		 *	@brief  Load the image from the specified path. 
		 *	@param  szPath  the file path.
		 *	@param  eFormat the type of image, currently only supporting two types.
		 *	@return true	if loaded correctly.
		 */
		virtual KDbool		initWithFile ( const KDchar* szPath, CCTexture2DPixelFormat eFormat = kCCTexture2DPixelFormat_Automatic );

		virtual KDbool		initWithData ( KDvoid* pData, KDuint uDataLen, CCTexture2DPixelFormat eFormat = kCCTexture2DPixelFormat_Automatic );

		virtual KDbool		initWithStream ( KDFile* pStream, CCTexture2DPixelFormat eFormat = kCCTexture2DPixelFormat_Automatic );

		/**
		 *	@brief   Load image from stream buffer.		 
		 *	@warning kFmtRawData only supports RGBA8888.
		 *	@param	 pBuffer  stream buffer which holds the image data.
		 *	@param	 nLength  data length expressed in (number of) bytes.
		 *	@param	 nWidth, nHeight, nBitsPerComponent are used for kFmtRawData.
		 *	@return  true if loaded correctly.
		 */
		virtual KDbool		initWithRawData ( KDvoid* pData, KDuint uDatalen, KDuint uWidth, KDuint uHeight, KDuint uBpp, KDbool bKeepData = KD_TRUE );		


		virtual KDbool		initWithStringShadowStroke ( const KDchar*	szText			   , 
														 KDuint			uWidth			= 0, 
														 KDuint			uHeight			= 0,
														 CCAlignment	eAlignMask		= kCCAlignmentCenter,
														 const KDchar*	szFontName		= 0,
														 KDuint			uSize			= 0,
														 KDfloat        fTextTintR		= 1,
														 KDfloat        fTextTintG		= 1,
														 KDfloat        fTextTintB		= 1,
														 KDbool			bShadow			= KD_FALSE,
														 KDfloat		fShadowOffsetX	= 0.0,
														 KDfloat		fShadowOffsetY	= 0.0,
														 KDfloat		fShadowOpacity	= 0.0,
														 KDfloat		fShadowBlur		= 0.0,
														 KDbool			bStroke			= KD_FALSE,
														 KDfloat		fStrokeR		= 1,
														 KDfloat		fStrokeG		= 1,
														 KDfloat		fStrokeB		= 1,
														 KDfloat		fStrokeSize		= 1 );

		KDbool				hasAlpha ( KDvoid )	{ return m_bHasAlpha; }

		KDbool				isPremultipliedAlpha ( KDvoid ) { return m_bPreMulti; }

		KDbool				isFlipY ( KDvoid ) { return m_bFlipY; }

		/**
		 *	@brief    Save CCImage data to the specified file, with specified format.
		 *	@param    szFilePath      the file's absolute path, including file suffix.
		 *	@param    bIsToRGB        whether the image is saved as RGB format.
		 */
		KDbool				saveToFile ( const KDchar* szFilePath, KDbool bIsToRGB = KD_TRUE );

	private :

		KDbool				initWithFontFile ( const KDchar* szText, KDuint uWidth, KDuint uHeight, CCAlignment eAlignMask, const KDchar* szFontFile, KDuint uSize );
		KDbool				initWithFontName ( const KDchar* szText, KDuint uWidth, KDuint uHeight, CCAlignment eAlignMask, const KDchar* szFontName, KDuint uSize );

	public :

		CC_SYNTHESIZE_READONLY ( KDubyte*    , m_pData     , Data    );
		CC_SYNTHESIZE_READONLY ( KDuint      , m_uDataLen  , DataLen );
		CC_SYNTHESIZE_READONLY ( KDuint		 , m_uWidth    , Width   );
		CC_SYNTHESIZE_READONLY ( KDuint		 , m_uHeight   , Height  );				
		CC_SYNTHESIZE_READONLY ( KDuint      , m_uLevel    , Level   );
		CC_SYNTHESIZE_READONLY ( KDuint		 , m_uBpp      , BitsPerComponent );
		CC_SYNTHESIZE_READONLY ( CCTexture2DPixelFormat, m_eFormat   , Format );

	protected :

		KDImageATX			m_pAtx;
		KDbool				m_bHasAlpha;
		KDbool				m_bPreMulti;
		KDbool				m_bKeepData;
		KDbool				m_bFlipY;
		    
	private :

		// noncopyable
		CCImage ( const CCImage& tImage );
		CCImage& operator= ( const CCImage& tImage );
};

// end of support group
/// @}

NS_CC_END

#endif // __CCImage_h__
