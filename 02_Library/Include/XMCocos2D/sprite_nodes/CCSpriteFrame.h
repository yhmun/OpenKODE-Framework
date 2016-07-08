/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrame.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCSpriteFrame_h__
#define __CCSpriteFrame_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"

#include <string>

NS_CC_BEGIN
    
class CCTexture2D;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/**
 *	@brief A CCSpriteFrame has:
 *		- texture: A CCTexture2D that will be used by the CCSprite
 *		- rectangle: A rectangle of the texture
 *
 *
 *	You can modify the frame of a CCSprite by doing:
 *
 *		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(texture, rect, offset);
 *		sprite->setDisplayFrame(frame);
 */
class CCSpriteFrame : public CCObject
{
	public :

		virtual ~CCSpriteFrame ( KDvoid );

		/** 
		 *	Create a CCSpriteFrame with a texture filename, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		static  CCSpriteFrame*		create ( const KDchar* szFileName, const CCRect& tRect );

		/** 
		 *	Create a CCSpriteFrame with a texture filename, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in pixels of the frame before being trimmed.
		 */
		static  CCSpriteFrame*		create ( const KDchar* szFileName, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize );

		/**
		 *	Create a CCSpriteFrame with a texture, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		static  CCSpriteFrame*		createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		/**
		 *	Create a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in points of the frame before being trimmed.
		 */
		static  CCSpriteFrame*		createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize );

	public :
		
		/**
		 *	Initializes a CCSpriteFrame with a texture, rect in points.
		 *	It is assumed that the frame was not trimmed.
		 */
		virtual KDbool			initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		/** 
		 *	Initializes a CCSpriteFrame with a texture filename, rect in points;
		 *	It is assumed that the frame was not trimmed.
		 */
		virtual KDbool			initWithTextureFilename ( const KDchar* szFileName, const CCRect& tRect );


		/** 
		 *	Initializes a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in points of the frame before being trimmed.
		 */
		virtual KDbool			initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize );

		/** 
		 *	Initializes a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
		 *	The originalSize is the size in pixels of the frame before being trimmed.
		 *
		 *	@since v1.1
		 */
		virtual KDbool			initWithTextureFilename ( const KDchar* szFileName, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		// attributes

		inline const CCRect&	getRectInPixels ( KDvoid ) { return m_tRectInPixels; }
		KDvoid					setRectInPixels ( const CCRect& tRectInPixels );

		inline  KDbool			 isRotated ( KDvoid ) { return m_bRotated; }
		inline  KDvoid			setRotated ( KDbool bRotated ) { m_bRotated = bRotated;	}

		/** get rect of the frame */
		inline  const CCRect&	getRect ( KDvoid ) { return m_tRect; }	

		/** set rect of the frame */
        KDvoid					setRect ( const CCRect& tRect );
		
		/** get offset of the frame */
		const   CCPoint&		getOffsetInPixels ( KDvoid );
		
		/** set offset of the frame */
		KDvoid					setOffsetInPixels ( const CCPoint& tOffsetInPixels );

		/** get original size of the trimmed image */
		inline  const CCSize&	getOriginalSizeInPixels ( KDvoid) { return m_tOriginalSizeInPixels; }
    
		/** set original size of the trimmed image */
		inline  KDvoid			setOriginalSizeInPixels ( const CCSize& tSizeInPixels ) { m_tOriginalSizeInPixels = tSizeInPixels; }

		/** get original size of the trimmed image */
		inline  const CCSize&	getOriginalSize ( KDvoid ) { return m_tOriginalSize; }
		
		/** set original size of the trimmed image */
		inline  KDvoid			setOriginalSize ( const CCSize& tSizeInPixels ) { m_tOriginalSize = tSizeInPixels; }

		/** get texture of the frame */
		CCTexture2D*			getTexture ( KDvoid );	

		/** set texture of the frame, the texture is retained */
		KDvoid					setTexture ( CCTexture2D* pTexture );

		const   CCPoint&		getOffset ( KDvoid );
		KDvoid					setOffset ( const CCPoint& tOffsets );

	protected :

		CCPoint					m_tOffset;
		CCSize					m_tOriginalSize;
		CCRect					m_tRectInPixels;
		KDbool					m_bRotated;
		CCRect					m_tRect;
        CCPoint					m_tOffsetInPixels;
		CCSize					m_tOriginalSizeInPixels;		
		CCTexture2D*			m_pTexture;
		std::string				m_sTextureFile;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CCSpriteFrame_h__