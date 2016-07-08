/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrame.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2011 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCSpriteFrame_h__
#define __CCSpriteFrame_h__

#include "../base_nodes/CCNode.h"
#include "../../base/CCGeometry.h"

NS_CC_BEGIN

class Texture2D;

/**
 *	@addtogroup sprite_nodes
 *	@{
 */

/** 
 *	@brief A SpriteFrame has:
 *	   - texture: A Texture2D that will be used by the Sprite
 *	   - rectangle: A rectangle of the texture
 *
 *
 *	You can modify the frame of a Sprite by doing:
 *
 *  SpriteFrame *frame = SpriteFrame::frameWithTexture(texture, rect, offset);
 *  sprite->setDisplayFrame(frame);
 */
class CC_DLL SpriteFrame : public Object, public Clonable
{
	public :

		/** 
		 *	Create a SpriteFrame with a texture filename, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		static SpriteFrame*		create ( const std::string& sFilename, const Rect& tRect );
    
		/** 
		 *	Create a SpriteFrame with a texture filename, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in pixels of the frame before being trimmed.
		 */
		static SpriteFrame*		create ( const std::string& sFilename, const Rect& tRect, KDbool bRotated, const Point& tOffset, const Size& tOriginalSize );
    
		/** 
		 *	Create a SpriteFrame with a texture, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		static SpriteFrame*		createWithTexture ( Texture2D* pTexture, const Rect& tRect );

		/** 
		 *	Create a SpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in points of the frame before being trimmed.
		 */
		static SpriteFrame*		createWithTexture ( Texture2D* pTexture, const Rect& tRect, KDbool bRotated, const Point& tOffset, const Size& tOriginalSize );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~SpriteFrame ( KDvoid );

		/**
		 *	Initializes a SpriteFrame with a texture, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		KDbool					initWithTexture ( Texture2D* pTexture, const Rect& tRect );

		/**
		 *	Initializes a SpriteFrame with a texture filename, rect in points;
		 *	It is assumed that the frame was not trimmed.
		 */
		KDbool					initWithTextureFilename ( const std::string& sFilename, const Rect& tRect );

		/** 
		 *	Initializes a SpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in points of the frame before being trimmed.
		 */
		KDbool					initWithTexture ( Texture2D* pTexture, const Rect& tRect, KDbool bRotated, const Point& tOffset, const Size& tOriginalSize );

		/**
		 *	Initializes a SpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in pixels of the frame before being trimmed.
		 *
		 *	@since v1.1
		 */
		KDbool					initWithTextureFilename ( const std::string& sFilename, const Rect& tRect, KDbool bRotated, const Point& tOffset, const Size& tOriginalSize );


		// attributes
		inline const Rect&		getRectInPixels ( KDvoid ) const { return m_tRectInPixels; }
		KDvoid					setRectInPixels ( const Rect& tRectInPixels );

		inline KDbool			 isRotated ( KDvoid ) const { return m_bRotated; }
		inline KDvoid			setRotated ( KDbool bRotated ) { m_bRotated = bRotated; }

		/** get rect of the frame */
		inline const Rect&		getRect ( KDvoid ) const { return m_tRect; }
		/** set rect of the frame */
		KDvoid					setRect ( const Rect& tRect );

		/** get offset of the frame */
		const Point&			getOffsetInPixels ( KDvoid ) const;
		/** set offset of the frame */
		KDvoid					setOffsetInPixels ( const Point& tOffsetInPixels );

		/** get original size of the trimmed image */
		inline const Size&		getOriginalSizeInPixels ( KDvoid ) const { return m_tOriginalSizeInPixels; }
		/** set original size of the trimmed image */
		inline KDvoid			setOriginalSizeInPixels ( const Size& tSizeInPixels ) { m_tOriginalSizeInPixels = tSizeInPixels; }

		/** get original size of the trimmed image */
		inline const Size&		getOriginalSize ( KDvoid ) const { return m_tOriginalSize; }
		/** set original size of the trimmed image */
		inline KDvoid			setOriginalSize ( const Size& tSizeInPixels ) { m_tOriginalSize = tSizeInPixels; }

		/** get texture of the frame */
		Texture2D*				getTexture ( KDvoid );
		/** set texture of the frame, the texture is retained */
		KDvoid					setTexture ( Texture2D* pTexture );

		const Point&			getOffset ( KDvoid ) const;
		KDvoid					setOffset ( const Point& tOffsets );

		// Overrides
		virtual SpriteFrame*	clone ( KDvoid ) const override;

	protected :

		Point					m_tOffset;
		Size					m_tOriginalSize;
		Rect					m_tRectInPixels;
		KDbool					m_bRotated;
		Rect					m_tRect;
		Point					m_tOffsetInPixels;
		Size					m_tOriginalSizeInPixels;
		Texture2D*				m_pTexture;
		std::string				m_sTextureFilename;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif	// __CCSpriteFrame_h__
