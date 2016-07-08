/* -----------------------------------------------------------------------------------
 *
 *      File            CCScale9Sprite.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Jung Sang-Taik on 12. 3. 16
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Neofect
 *
 *          http://www.cocos2d-x.org
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
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCScale9Sprite_h__
#define __CCScale9Sprite_h__

#include "../../ExtensionMacros.h"
#include "../../../2d/sprite_nodes/CCSpriteBatchNode.h"
#include "../../../2d/sprite_nodes/CCSpriteFrame.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

/**
 *	A 9-slice sprite for cocos2d.
 *
 *	9-slice scaling allows you to specify how scaling is applied
 *	to specific areas of a sprite. With 9-slice scaling (3x3 grid),
 *	you can ensure that the sprite does not become distorted when
 *	scaled.
 *
 *	@see http://yannickloriot.com/library/ios/cccontrolextension/Classes/CCScale9Sprite.html
 */
class Scale9Sprite : public NodeRGBA
{
	public :

		/**
		 *	@js ctor
		 */
		Scale9Sprite ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Scale9Sprite ( KDvoid );

	public :

		static Scale9Sprite*	create ( KDvoid );

		/**
		 *	Creates a 9-slice sprite with a texture file, a delimitation zone and
		 *	with the specified cap insets.
		 *
		 *	@see initWithFile(const char *file, const Rect& rect, const Rect& capInsets)
		 */
		static Scale9Sprite*	create ( const KDchar* pFile, const Rect& tRect,  const Rect& tCapInsets );

		/**
		 *	Creates a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *
		 *	@see initWithFile(const Rect& capInsets, const char *file)
		 */
		static Scale9Sprite*	create ( const Rect& tCapInsets, const KDchar* pFile );

		/**
		 *	Creates a 9-slice sprite with a texture file and a delimitation zone. The
		 *	texture will be broken down into a 3×3 grid of equal blocks.
		 *	
		 *	@see initWithFile(const char *file, const Rect& rect)
		 */
		static Scale9Sprite*	create ( const KDchar* pFile, const Rect& tRect );

		/**
		 *	Creates a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *
		 *	@see initWithFile(const char *file)
		 */
		static Scale9Sprite*	create ( const KDchar* pFile );

		/**
		 *	Creates a 9-slice sprite with an sprite frame.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrame(SpriteFrame *spriteFrame)
		 */
		static Scale9Sprite*	createWithSpriteFrame ( SpriteFrame* pSpriteFrame );

		/**
		 *	Creates a 9-slice sprite with an sprite frame and the centre of its zone.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrame(SpriteFrame *spriteFrame, const Rect& capInsets)
		 */
		static Scale9Sprite*	createWithSpriteFrame ( SpriteFrame* pSpriteFrame, const Rect& tCapInsets );

		/**
		 *	Creates a 9-slice sprite with an sprite frame name.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrameName(const char *spriteFrameName)
		 */
		static Scale9Sprite*	createWithSpriteFrameName ( const KDchar* pSpriteFrameName );

		/**
		 *	Creates a 9-slice sprite with an sprite frame name and the centre of its
		 *	zone.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrameName(const char *spriteFrameName, const Rect& capInsets)
		 */
		static Scale9Sprite*	createWithSpriteFrameName ( const KDchar* pSpriteFrameName, const Rect& tCapInsets );

		/**
		 *	Initializes a 9-slice sprite with a texture file, a delimitation zone and
		 *	with the specified cap insets.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param file The name of the texture file.
		 *	@param rect The rectangle that describes the sub-part of the texture that
		 *	is the whole image. If the shape is the whole texture, set this to the 
		 *	texture's full rect.
		 *	@param capInsets The values to use for the cap insets.
		 */
		virtual KDbool			initWithFile ( const KDchar* pFile, const Rect& tRect,  const Rect& tCapInsets );
    
		/**
		 *	Initializes a 9-slice sprite with a texture file and a delimitation zone. The
		 *	texture will be broken down into a 3×3 grid of equal blocks.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param file The name of the texture file.
		 *	@param rect The rectangle that describes the sub-part of the texture that
		 *	is the whole image. If the shape is the whole texture, set this to the 
		 *	texture's full rect.
		 */
		virtual KDbool			initWithFile ( const KDchar* pFile, const Rect& tRect );
    
		/**
		 *	Initializes a 9-slice sprite with a texture file and with the specified cap
		 *	insets.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param file The name of the texture file.
		 *	@param capInsets The values to use for the cap insets.
		 */
		virtual KDbool			initWithFile ( const Rect& tCapInsets, const KDchar* pFile );
    
		/**
		 *	Initializes a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param file The name of the texture file.
		 */
		virtual KDbool			initWithFile ( const KDchar* pFile );
    
		/**
		 *	Initializes a 9-slice sprite with an sprite frame and with the specified 
		 *	cap insets.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrame The sprite frame object.
		 *	@param capInsets The values to use for the cap insets.
		 */
		virtual KDbool			initWithSpriteFrame ( SpriteFrame* pSpriteFrame, const Rect& tCapInsets );

		/**
		 *	Initializes a 9-slice sprite with an sprite frame.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrame The sprite frame object.
		 */
		virtual KDbool			initWithSpriteFrame(SpriteFrame* spriteFrame);

		/**
		 *	Initializes a 9-slice sprite with an sprite frame name and with the specified 
		 *	cap insets.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrameName The sprite frame name.
		 *	@param capInsets The values to use for the cap insets.
		 */
		virtual KDbool			initWithSpriteFrameName ( const KDchar* pSpriteFrameName, const Rect& tCapInsets );

		/**
		 *	Initializes a 9-slice sprite with an sprite frame name.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrameName The sprite frame name.
		 */
		virtual KDbool			initWithSpriteFrameName ( const KDchar* pSpriteFrameName );

		virtual KDbool			init ( KDvoid );
		virtual KDbool			initWithBatchNode ( SpriteBatchNode* pBatchnode, const Rect& tRect, KDbool bRotated, const Rect& tCapInsets );
		virtual KDbool			initWithBatchNode ( SpriteBatchNode* pBatchnode, const Rect& tRect, const Rect& tCapInsets );

		/**
		 *	Creates and returns a new sprite object with the specified cap insets.
		 *	You use this method to add cap insets to a sprite or to change the existing
		 *	cap insets of a sprite. In both cases, you get back a new image and the 
		 *	original sprite remains untouched.
		 *
		 *	@param capInsets The values to use for the cap insets.
		 */
		Scale9Sprite*			resizableSpriteWithCapInsets ( const Rect& tCapInsets );
    
		virtual KDbool			updateWithBatchNode ( SpriteBatchNode* pBatchnode, const Rect& tRect, KDbool bRotated, const Rect& tCapInsets );
		virtual KDvoid			setSpriteFrame ( SpriteFrame* pSpriteFrame );

		// overrides
		virtual KDvoid			setContentSize ( const Size& tSize ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			visit ( KDvoid ) override;
		virtual KDvoid			setOpacityModifyRGB ( KDbool bValue ) override;
		virtual KDbool			 isOpacityModifyRGB ( KDvoid ) const override;
		virtual KDvoid			setOpacity ( GLubyte cOpacity ) override;
		virtual GLubyte			getOpacity ( KDvoid ) const override;
		virtual KDvoid			setColor ( const Color3B& tColor ) override;
		virtual const Color3B&	getColor ( KDvoid ) const override;
		virtual KDvoid			updateDisplayedOpacity ( GLubyte cParentOpacity ) override;
		virtual KDvoid			updateDisplayedColor ( const Color3B& tParentColor ) override;

	protected :

		KDvoid					updateCapInset ( KDvoid );
		KDvoid					updatePositions ( KDvoid );

		KDbool					m_bSpritesGenerated;
		Rect					m_tSpriteRect;
		KDbool					m_bSpriteFrameRotated;
		Rect					m_tCapInsetsInternal;
		KDbool					m_bPositionsAreDirty;

		SpriteBatchNode*		m_pScale9Image;
		Sprite*					m_pTopLeft;
		Sprite*					m_pTop;
		Sprite*					m_pTopRight;
		Sprite*					m_pLeft;
		Sprite*					m_pCentre;
		Sprite*					m_pRight;
		Sprite*					m_pBottomLeft;
		Sprite*					m_pBottom;
		Sprite*					m_pBottomRight;

		KDbool					m_bOpacityModifyRGB;

		/** Original sprite's size. */
		CC_SYNTHESIZE_READONLY ( Size, m_tOriginalSize, OriginalSize );

		/** Prefered sprite's size. By default the prefered size is the original size. */

		//if the preferredSize component is given as -1, it is ignored
		CC_PROPERTY ( Size, m_tPreferredSize, PreferredSize );
		
		/**
		 *	The end-cap insets.
		 *	On a non-resizeable sprite, this property is set to CGRect::ZERO; the sprite
		 *	does not use end caps and the entire sprite is subject to stretching.
		 */
		CC_PROPERTY ( Rect, m_tCapInsets, CapInsets );
		/** Sets the left side inset */
		CC_PROPERTY ( KDfloat, m_fInsetLeft, InsetLeft );
		/** Sets the top side inset */
		CC_PROPERTY ( KDfloat, m_fInsetTop, InsetTop );
		/** Sets the right side inset */
		CC_PROPERTY ( KDfloat, m_fInsetRight, InsetRight );
		/** Sets the bottom side inset */
		CC_PROPERTY ( KDfloat, m_fInsetBottom, InsetBottom );
};

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif // __CCScale9Sprite_h__
