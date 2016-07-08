/* --------------------------------------------------------------------------
 *
 *      File            CCScale9Sprite.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Neofect (Jung Sang-Taik on 12. 3. 16)
 *
 *         http://www.cocos2d-x.org   
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __CCScale9Sprite_h__
#define __CCScale9Sprite_h__

#include "../../sprite_nodes/CCSprite.h"

NS_CC_BEGIN

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
class CCScale9Sprite : public CCNodeRGBA
{
	public :
				 CCScale9Sprite ( KDvoid );
		virtual ~CCScale9Sprite ( KDvoid );

		/** 
		 *	Creates a 9-slice sprite with a texture file, a delimitation zone and
		 *	with the specified cap insets.
		 *
		 *	@see initWithFile:rect:centerRegion:
		 */
		static CCScale9Sprite*		create ( const KDchar* szFile, CCRect tRect, CCRect tCapInsets );

		/** 
		 *	Creates a 9-slice sprite with a texture file and a delimitation zone. The
		 *	texture will be broken down into a 3×3 grid of equal blocks.
		 *
		 *	@see initWithFile:rect:
		 */
		static CCScale9Sprite*		create ( const KDchar* szFile, CCRect tRect );

		/** 
		 *	Creates a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *
		 *	@see initWithFile:capInsets:
		 */
		static CCScale9Sprite*		create ( CCRect tCapInsets, const KDchar* szFile );

		/** 
		 *	Creates a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *
		 *	@see initWithFile:
		 */
		static CCScale9Sprite*		create ( const KDchar* szFile );

		/**
		 *	Creates a 9-slice sprite with an sprite frame and the centre of its zone.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrame:centerRegion:
		 */
		static CCScale9Sprite*		createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, CCRect tCapInsets );	

		/**
		 *	Creates a 9-slice sprite with an sprite frame.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrame:
		 */
		static CCScale9Sprite*		createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );	

		/**
		 *	Creates a 9-slice sprite with an sprite frame name and the centre of its
		 *	zone.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrameName:centerRegion:
		 */
		static CCScale9Sprite*		createWithSpriteFrameName ( const KDchar* szSpriteFrame, CCRect tCapInsets );	

		/**
		 *	Creates a 9-slice sprite with an sprite frame name.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@see initWithSpriteFrameName:
		 */
		static CCScale9Sprite*		createWithSpriteFrameName ( const KDchar* szSpriteFrame );

		static CCScale9Sprite*		create ( KDvoid );

	public :
				
		virtual KDbool		initWithBatchNode ( CCSpriteBatchNode* pBatchNode, CCRect tRect, KDbool bRotated, CCRect tCapInsets );
		virtual KDbool		initWithBatchNode ( CCSpriteBatchNode* pBatchNode, CCRect tRect, CCRect tCapInsets );

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
		virtual KDbool		initWithFile ( const KDchar* file, CCRect tRect, CCRect tCapInsets );		
		
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
		virtual KDbool		initWithFile ( const char* szFile, CCRect tRect );
		
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
		virtual KDbool		initWithFile ( CCRect tCapInsets, const KDchar* szFile );
		
		/**
		 *	Initializes a 9-slice sprite with a texture file. The whole texture will be
		 *	broken down into a 3×3 grid of equal blocks.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param file The name of the texture file.
		 */
		virtual KDbool		initWithFile ( const KDchar* szFile );
		
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
		virtual KDbool		initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, CCRect tCapInsets );

		/**
		 *	Initializes a 9-slice sprite with an sprite frame.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrame The sprite frame object.
		 */
		virtual KDbool		initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

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
		virtual KDbool		initWithSpriteFrameName ( const KDchar* szSpriteFrame, CCRect tCapInsets );

		/**
		 *	Initializes a 9-slice sprite with an sprite frame name.
		 *	Once the sprite is created, you can then call its "setContentSize:" method
		 *	to resize the sprite will all it's 9-slice goodness intract.
		 *	It respects the anchorPoint too.
		 *
		 *	@param spriteFrameName The sprite frame name.
		 */
		virtual KDbool		initWithSpriteFrameName ( const KDchar* szSpriteFrame );
		
		virtual KDbool		init ( KDvoid );

		/**
		 *	Creates and returns a new sprite object with the specified cap insets.
		 *	You use this method to add cap insets to a sprite or to change the existing
		 *	cap insets of a sprite. In both cases, you get back a new image and the 
		 *	original sprite remains untouched.
		 *
		 *	@param capInsets The values to use for the cap insets.
		 */
		CCScale9Sprite*		resizableSpriteWithCapInsets ( CCRect tCapInsets );

		// optional
		
		/**
		 *	sets the premultipliedAlphaOpacity property.
		 *	If set to NO then opacity will be applied as: glColor(R,G,B,opacity);
		 *	If set to YES then oapcity will be applied as: glColor(opacity, opacity, opacity, opacity );
		 *	Textures with premultiplied alpha will have this property by default on YES. Otherwise the default value is NO
		 *	@since v0.8
		 */
		virtual KDvoid		setOpacityModifyRGB ( KDbool bValue );
		
		/** 
		 *	returns whether or not the opacity will be applied using glColor(R,G,B,opacity) or glColor(opacity, opacity, opacity, opacity);
		 *	@since v0.8
		 */
		virtual KDbool		isOpacityModifyRGB ( KDvoid );
		virtual KDvoid		setOpacity ( GLubyte opacity );
		virtual KDvoid		setColor ( const ccColor3B& tColor );

		virtual KDbool		updateWithBatchNode ( CCSpriteBatchNode* pBatchnode, CCRect tRect, KDbool bRotated, CCRect tCapInsets );

		virtual KDvoid		setSpriteFrame ( CCSpriteFrame* pSpriteFrame );

	public :

		virtual KDvoid		setContentSize ( const CCSize& tSize );

		virtual KDvoid		visit ( KDvoid );

		/** Original sprite's size. */
		CC_SYNTHESIZE_READONLY ( CCSize, m_tOriginalSize, OriginalSize );
		/** Prefered sprite's size. By default the prefered size is the original size. */

		//if the preferredSize component is given as -1, it is ignored
		CC_PROPERTY ( CCSize, m_tPreferredSize, PreferredSize ); 

		/** 
		 * The end-cap insets. 
		 * On a non-resizeable sprite, this property is set to CGRectZero; the sprite 
		 * does not use end caps and the entire sprite is subject to stretching. 
		 */
		CC_PROPERTY ( CCRect, m_tCapInsets, CapInsets );

		/** Sets the left side inset */
		CC_PROPERTY ( KDfloat, m_fInsetLeft, InsetLeft );

		/** Sets the top side inset */
		CC_PROPERTY ( KDfloat, m_fInsetTop, InsetTop );

		/** Sets the right side inset */
		CC_PROPERTY ( KDfloat, m_fInsetRight, InsetRight );

		/** Sets the bottom side inset */
		CC_PROPERTY ( KDfloat, m_fInsetBottom, InsetBottom );

	protected :

		KDvoid				updateCapInset  ( KDvoid );
		KDvoid				updatePositions ( KDvoid );

	protected :

		KDbool					m_bSpritesGenerated;
		CCRect					m_tSpriteRect;
		KDbool					m_bSpriteFrameRotated;
		CCRect					m_tCapInsetsInternal;
		KDbool					m_bPositionsAreDirty;
	    
		CCSpriteBatchNode*		m_pScale9Image;
		CCSprite*				m_pTopLeft;
		CCSprite*				m_pTop;
		CCSprite*				m_pTopRight;
		CCSprite*				m_pLeft;
		CCSprite*				m_pCentre;
		CCSprite*				m_pRight;
		CCSprite*				m_pBottomLeft;
		CCSprite*				m_pBottom;
		CCSprite*				m_pBottomRight;
		
		KDbool					m_bOpacityModifyRGB;
		GLubyte					m_cOpacity;
		ccColor3B				m_tColor;	
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCScale9Sprite_h__
