/* -----------------------------------------------------------------------------------
 *
 *      File            CCRenderTexture.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCRenderTexture_h__
#define __CCRenderTexture_h__

#include "../base_nodes/CCNode.h"
#include "../sprite_nodes/CCSprite.h"
#include "../../platform/CCImage.h"
#include "XMKazmath/mat4.h"

NS_CC_BEGIN

/**
 *	@addtogroup textures
 *	@{
 */

/**
 *	@brief RenderTexture is a generic rendering target. To render things into it,
 *	simply construct a render target, call begin on it, call visit on any cocos
 *	scenes or objects to render them, and call end. For convenience, render texture
 *	adds a sprite as it's display child with the results, so you can simply add
 *	the render texture to your scene and treat it like any other CocosNode.
 *	There are also functions for saving the render texture to disk in PNG or JPG format.
 *
 *	@since v0.8.1
 */
class CC_DLL RenderTexture : public Node 
{
	public :

		/** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
		static RenderTexture*	create ( KDint w, KDint h, Texture2D::PixelFormat eFormat, GLuint uDepthStencilFormat );

		/** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
		static RenderTexture*	create ( KDint w, KDint h, Texture2D::PixelFormat eFormat );

		/** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
		static RenderTexture*	create ( KDint w, KDint h );

		/**
		 *	@js ctor
		 */
		RenderTexture ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~RenderTexture ( KDvoid );
    
		/** initializes a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
		KDbool					initWithWidthAndHeight ( KDint w, KDint h, Texture2D::PixelFormat eFormat );

		/** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
		KDbool					initWithWidthAndHeight ( KDint w, KDint h, Texture2D::PixelFormat eFormat, GLuint uDepthStencilFormat );

		/** starts grabbing */
		KDvoid					begin ( KDvoid );

		/**
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid					beginWithClear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a );

		/** 
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid					beginWithClear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a, KDfloat fDepthValue );

		/**
		 *	starts rendering to the texture while clearing the texture first.
		 *	This is more efficient then calling -clear first and then -begin 
		 */
		KDvoid					beginWithClear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a, KDfloat fDepthValue, KDint nStencilValue );

		/** end is key word of lua, use other name to export to lua. */
		inline KDvoid			endToLua ( KDvoid ) { end ( ); }

		/** ends grabbing*/
		KDvoid					end ( KDvoid );

		/** clears the texture with a color */
		KDvoid					clear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a );

		/** clears the texture with a specified depth value */
		KDvoid					clearDepth ( KDfloat fDepthValue );

		/** clears the texture with a specified stencil value */
		KDvoid					clearStencil ( KDint nStencilValue );

		/**
		 *	creates a new Image from with the texture's data.
		 *  Caller is responsible for releasing it by calling delete.
		 */    
		Image*					newImage ( KDbool bFlipImage = true );
    
		/** 
		 *	saves the texture into a file using JPEG format. The file will be saved in the Documents folder.
		 *	Returns true if the operation is successful.
		 */
		bool					saveToFile ( const KDchar* szFilePath );

		/** 
		 *	saves the texture into a file. The format could be JPG or PNG. The file will be saved in the Documents folder.
		 *	Returns true if the operation is successful.
		 */
		bool					saveToFile ( const KDchar* szName, Image::Format eFormat );
    
		/**
		 *	Listen "come to background" message, and save render texture.
		 *	It only has effect on Android.
		 */
		void					listenToBackground ( Object* pObj );
    
		/** 
		 *	Listen "come to foreground" message and restore the frame buffer object
		 *	It only has effect on Android.
		 */
		void					listenToForeground ( Object* pObj );
    
		/** Valid flags: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT. They can be OR'ed. Valid when "autoDraw" is true. */
		inline KDuint			getClearFlags ( KDvoid ) const { return m_uClearFlags; }
		inline KDvoid			setClearFlags ( KDuint uClearFlags ) { m_uClearFlags = uClearFlags; }
    
		/** Clear color value. Valid only when "autoDraw" is true. */
		inline const Color4F&	getClearColor ( KDvoid ) const { return m_tClearColor; }
		inline KDvoid			setClearColor ( const Color4F& tClearColor ) { m_tClearColor = tClearColor; }
    
		/** Value for clearDepth. Valid only when "autoDraw" is true. */
		inline KDfloat			getClearDepth ( KDvoid ) const { return m_fClearDepth; }
		inline KDvoid			setClearDepth ( KDfloat fClearDepth ) { m_fClearDepth = fClearDepth; }
    
		/** Value for clear Stencil. Valid only when "autoDraw" is true */
		inline KDint			getClearStencil ( KDvoid ) const { return m_nClearStencil; }
		inline KDvoid			setClearStencil ( KDint nClearStencil ) { m_nClearStencil = nClearStencil; }
    
		/**
		 *	When enabled, it will render its children into the texture automatically. Disabled by default for compatiblity reasons.
		 *	Will be enabled in the future.
		 */
		inline KDbool			 isAutoDraw ( KDvoid ) const { return m_bAutoDraw; }
		inline KDvoid			setAutoDraw ( KDbool bIsAutoDraw ) { m_bAutoDraw = bIsAutoDraw; }

		/** Gets the Sprite being used. */
		inline Sprite*			getSprite ( KDvoid ) const { return m_pSprite; }
    
		/** Sets the Sprite being used. */
		inline KDvoid			setSprite ( Sprite* pSprite )
		{
			CC_SAFE_RETAIN ( pSprite );
			CC_SAFE_RELEASE ( m_pSprite );
			m_pSprite = pSprite;
		}
    
		// Overrides
		virtual KDvoid			visit ( KDvoid ) override;
		virtual KDvoid			draw ( KDvoid ) override;

	private :

		KDvoid					beginWithClear ( KDfloat r, KDfloat g, KDfloat b, KDfloat a, KDfloat fDepthValue, KDint nStencilValue, GLbitfield uFlags );

	protected :

		GLuint					m_uFBO;
		GLuint					m_uDepthRenderBufffer;
		GLint					m_uOldFBO;
		Texture2D*				m_pTexture;
		Texture2D*				m_pTextureCopy;    // a copy of _texture
		Image*					m_pUITextureImage;
		Texture2D::PixelFormat	m_ePixelFormat;
    
		// code for "auto" update
		GLbitfield				m_uClearFlags;
		Color4F					m_tClearColor;
		GLclampf				m_fClearDepth;
		GLint					m_nClearStencil;
		KDbool					m_bAutoDraw;

		/**
		 *	The Sprite being used.
		 *	The sprite, by default, will use the following blending function: GL_ONE, GL_ONE_MINUS_SRC_ALPHA.
		 *	The blending function can be changed in runtime by calling:
		 *		- [[renderTexture sprite] setBlendFunc:(BlendFunc){GL_ONE, GL_ONE_MINUS_SRC_ALPHA}];
		 */
		Sprite*					m_pSprite;
};

// end of textures group
/// @}

NS_CC_END

#endif	// __CCRenderTexture_h__
