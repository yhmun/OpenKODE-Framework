/* -----------------------------------------------------------------------------------
 *
 *      File            CCRenderTexture.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Jason Booth
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

#ifndef __CCRenderTexture_h__
#define __CCRenderTexture_h__

#include "../sprite_nodes/CCSprite.h"
#include "../kazmath/mat4.h"

NS_CC_BEGIN
   
/**
 * @addtogroup textures
 * @{
 */

typedef enum eImageFormat
{
	kCCImageFormatJPEG    = 0,
	kCCImageFormatPNG     = 1,
} tCCImageFormat;

/**
 *	@brief	CCRenderTexture is a generic rendering target. To render things into it,
 *	simply construct a render target, call begin on it, call visit on any cocos
 *	scenes or objects to render them, and call end. For convenience, render texture
 *	adds a sprite as it's display child with the results, so you can simply add
 *	the render texture to your scene and treat it like any other CocosNode.
 *	There are also functions for saving the render texture to disk in PNG or JPG format.
 *
 *	@since	v0.8.1
 */
class CCRenderTexture : public CCNode 
{
	public :
		         CCRenderTexture ( KDvoid );
		virtual ~CCRenderTexture ( KDvoid );

		/** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
		static   CCRenderTexture*	create ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat );

		/** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
		static   CCRenderTexture*	create ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat );

		/** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
		static   CCRenderTexture*	create ( KDuint uWidth, KDuint uHeight );

	public :

		/** initializes a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
		virtual KDbool		initWithWidthAndHeight ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat );

		/** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
		virtual KDbool		initWithWidthAndHeight ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat );

		virtual KDvoid		visit ( KDvoid );
		virtual KDvoid		draw  ( KDvoid );

		/** starts grabbing */
		KDvoid				begin ( KDvoid );

		/**
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid				beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha );

		/** 
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid				beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha, KDfloat fDepthValue );

		/** 
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid				beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha, KDfloat fDepthValue, KDint nStencilValue );

		/** end is key word of lua, use other name to export to lua. */
		inline  KDvoid		endToLua ( KDvoid ) { end ( ); }

		/** ends grabbing*/
		KDvoid				end ( KDvoid );

		/** clears the texture with a color */
		KDvoid				clear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha );

		/** clears the texture with a specified depth value */
		KDvoid				clearDepth ( KDfloat fDepthValue );

		/** clears the texture with a specified stencil value */
		KDvoid				clearStencil ( KDint nStencilValue );

		/*
		 *	creates a new CCImage from with the texture's data.
         *	Caller is responsible for releasing it by calling delete.
		 */
		CCImage*			newCCImage ( KDbool bFlipImage = KD_TRUE );

		/** 
		 *	saves the texture into a file using JPEG format. The file will be saved in the Documents folder.
         *	Returns YES if the operation is successful.
		 */
		KDbool				saveToFile ( const KDchar* szFilePath );

		/** 
		 *	saves the texture into a file. The format could be JPG or PNG. The file will be saved in the Documents folder.
		 *	Returns YES if the operation is successful.
		 */
		KDbool				saveToFile ( const KDchar* szFilePath, tCCImageFormat eFormat );

		/** 
		 *	Listen "come to background" message, and save render texture.
		 *	It only has effect on Android.
		 */
		KDvoid				listenToBackground ( CCObject* pObject );

		/** 
		 *	Listen "come to foreground" message and restore the frame buffer object
		 *	It only has effect on Android.
		 */
		KDvoid				listenToForeground ( CCObject* pObject );

		/** Valid flags: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT. They can be OR'ed. Valid when "autoDraw is YES. */
		KDuint				getClearFlags ( ) const;
		KDvoid				setClearFlags ( KDuint uClearFlags );
	    
		/** Clear color value. Valid only when "autoDraw" is true. */
		const ccColor4F&	getClearColor ( KDvoid ) const;
		KDvoid				setClearColor ( const ccColor4F& tClearColor );
	    
		/** Value for clearDepth. Valid only when autoDraw is true. */
		KDfloat				getClearDepth ( KDvoid ) const;
		KDvoid				setClearDepth ( KDfloat fClearDepth );
	    
		/** Value for clear Stencil. Valid only when autoDraw is true */
		KDint				getClearStencil ( KDvoid ) const;
		KDvoid				setClearStencil ( KDfloat fClearStencil );
	    
		/** 
		 *	When enabled, it will render its children into the texture automatically. Disabled by default for compatiblity reasons.
		 *	Will be enabled in the future.
		 */
		KDbool				 isAutoDraw ( KDvoid ) const;
		KDvoid				setAutoDraw ( KDbool bAutoDraw );

	private :

		KDvoid				beginWithClear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a, KDfloat fDepthValue, KDint fStencilValue, GLbitfield uFlags );

	public :

		/**
		 *	The CCSprite being used.
		 *	The sprite, by default, will use the following blending function: GL_ONE, GL_ONE_MINUS_SRC_ALPHA.
		 *	The blending function can be changed in runtime by calling:
		 *		- [[renderTexture sprite] setBlendFunc:(ccBlendFunc){GL_ONE, GL_ONE_MINUS_SRC_ALPHA}];
		 */
		CC_PROPERTY ( CCSprite*, m_pSprite, Sprite );

	protected :

		GLuint				m_uFBO;
		GLuint				m_uDepthRenderBufffer;
		GLint				m_nOldFBO;
		CCTexture2D*		m_pTexture;
		CCTexture2D*		m_pTextureCopy;				// a copy of m_pTexture
		CCImage*			m_pImage;
		GLenum				m_ePixelFormat;

		// code for "auto" update
		GLbitfield			m_uClearFlags;
		ccColor4F			m_tClearColor;
		GLclampf			m_fClearDepth;
		GLint				m_nClearStencil;
		KDbool				m_bAutoDraw;
};

// end of textures group
/// @}

NS_CC_END

#endif // __CCRenderTexture_h__