/* -----------------------------------------------------------------------------------
 *
 *      File            CCSprite.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __CCSprite_h__
#define __CCSprite_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"
#include "../textures/CCTextureAtlas.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN
    
class  CCSpriteBatchNode;
class  CCSpriteFrame;
class  CCAnimation;
class  CCRect;
class  CCPoint;
class  CCSize;
class  CCTexture2D;
struct transformValues_;

/**
 * @addtogroup sprite_nodes
 * @{
 */

#define CCSpriteIndexNotInitialized     0xffffffff		/// CCSprite invalid index on the CCSpriteBatchNode

/**
 *	CCSprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) )
 *
 *	CCSprite can be created with an image, or with a sub-rectangle of an image.
 *
 *	If the parent or any of its ancestors is a CCSpriteBatchNode then the following features/limitations are valid
 *		- Features when the parent is a CCBatchNode:
 *			- MUCH faster rendering, specially if the CCSpriteBatchNode has many children. All the children will be drawn in a single batch.
 *
 *		- Limitations
 *			- Camera is not supported yet (eg: CCOrbitCamera action doesn't work)
 *			- GridBase actions are not supported (eg: CCLens, CCRipple, CCTwirl)
 *			- The Alias/Antialias property belongs to CCSpriteBatchNode, so you can't individually set the aliased property.
 *			- The Blending function property belongs to CCSpriteBatchNode, so you can't individually set the blending function property.
 *			- Parallax scroller is not supported, but can be simulated with a "proxy" sprite.
 *	
 *	If the parent is an standard CCNode, then CCSprite behaves like any other CCNode:
 *		- It supports blending functions
 *	    - It supports aliasing / antialiasing
 *	    - But the rendering will be slower: 1 draw per children.
 *
 *	The default anchorPoint in CCSprite is (0.5, 0.5).
 */
class CCSprite : public CCNodeRGBA, public CCTextureProtocol
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/**
		 *	Default constructor
		 */
		CCSprite ( KDvoid );

		/**
		 *	Default destructor
		 */
		virtual ~CCSprite ( KDvoid );

	public :

    /// @{
    /// @name Creators

		/**
		 *	Creates an empty sprite without texture. You can call setTexture method subsequently.
		 *
		 *	@return An empty sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			create ( KDvoid );

		/**
		 *	Creates a sprite with an image filename.
		 *
		 *	After creation, the rect of sprite will be the size of the image,
		 *	and the offset will be (0,0).
		 *
		 *	@param   pszFileName The string which indicates a path to image file, e.g., "scene1/monster.png".
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			create ( const KDchar* szFilePath );

		/**
		 *	Creates a sprite with an image filename and a rect.
		 *
		 *	@param   pszFileName The string wich indicates a path to image file, e.g., "scene1/monster.png"
		 *	@param   rect        Only the contents inside rect of pszFileName's texture will be applied for this sprite.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			create ( const KDchar* szFilePath, const CCRect& tRect );

		/**
		 *	Creates a sprite with an exsiting texture contained in a CCTexture2D object
		 *	After creation, the rect will be the size of the texture, and the offset will be (0,0).
		 *
		 *	@param   pTexture    A pointer to a CCTexture2D object.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			createWithTexture ( CCTexture2D* pTexture );

		/**
		 *	Creates a sprite with a texture and a rect.
		 *
		 *	After creation, the offset will be (0,0).
		 *
		 *	@param   pTexture    A pointer to an existing CCTexture2D object.
		 *                      You can use a CCTexture2D object for many sprites.
		 *	@param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		/**
		 *	Creates a sprite with an sprite frame.
		 *
		 *	@param   pSpriteFrame    A sprite frame which involves a texture and a rect
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

		/**
		 *	Creates a sprite with an sprite frame name.
		 *
		 *	A CCSpriteFrame will be fetched from the CCSpriteFrameCache by pszSpriteFrameName param.
		 *	If the CCSpriteFrame doesn't exist it will raise an exception.
		 *
		 *	@param   pszSpriteFrameName A null terminated string which indicates the sprite frame name.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static  CCSprite*			createWithSpriteFrameName ( const KDchar* szSpriteFrameName );


		static	CCSprite*			createWithSpriteBatchNode ( CCSpriteBatchNode* pSpriteBatchNode );

		static	CCSprite*			createWithSpriteBatchNode ( CCSpriteBatchNode* pSpriteBatchNode, const CCRect& tRect );


    /// @}  end of creators group
	    
	public :

    /// @{
    /// @name Initializers

		/**
		 *	Initializes an empty sprite with nothing init.
		 */
		virtual KDbool				init ( KDvoid );	

		/**
		 *	Initializes a sprite with a texture.
		 *
		 *	After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
		 *
		 *	@param   pTexture    A pointer to an existing CCTexture2D object.
		 *	                     You can use a CCTexture2D object for many sprites.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( CCTexture2D* pTexture );

		/**
		 *	Initializes a sprite with a texture and a rect.
		 *	
		 *	 After initialization, the offset will be (0,0).
		 *
		 *	@param   pTexture    A pointer to an exisiting CCTexture2D object.
		 *	                     You can use a CCTexture2D object for many sprites.
		 *	@param   rect        Only the contents inside rect of this texture will be applied for this sprite.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		/**
		 *	Initializes a sprite with a texture and a rect in points, optionally rotated.
		 *
		 *	After initialization, the offset will be (0,0).
		 *	@note    This is the designated initializer.
		 *
		 *	@param   pTexture    A CCTexture2D object whose texture will be applied to this sprite.
		 *	@param   rect        A rectangle assigned the contents of texture.
		 *	@param   rotated     Whether or not the texture rectangle is rotated.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated );

		/**
		 *	Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
		 *
		 *	@param   pSpriteFrame  A CCSpriteFrame object. It should includes a valid texture and a rect
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

		/**
		 *	Initializes a sprite with an sprite frame name.
		 *
		 *	A CCSpriteFrame will be fetched from the CCSpriteFrameCache by name.
		 *	If the CCSpriteFrame doesn't exist it will raise an exception.
		 *
		 *	@param   pszSpriteFrameName  A key string that can fected a volid CCSpriteFrame from CCSpriteFrameCache
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithSpriteFrameName ( const KDchar* szSpriteFrameName );

		/**
		 *	Initializes a sprite with an image filename.
		 *
		 *	This method will find pszFilename from local file system, load its content to CCTexture2D,
		 *	then use CCTexture2D to create a sprite.
		 *	After initialization, the rect used will be the size of the image. The offset will be (0,0).
		 *
		 *	@param   pszFilename The path to an image file in local file system
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithFile ( const KDchar* szFilePath );

		/**
		 *	Initializes a sprite with an image filename, and a rect.
		 *
		 *	This method will find pszFilename from local file system, load its content to CCTexture2D,
		 *	then use CCTexture2D to create a sprite.
		 *	After initialization, the offset will be (0,0).
		 *
		 *	@param   pszFilename The path to an image file in local file system.
		 *	@param   rect        The rectangle assigned the content area from texture.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithFile ( const KDchar* szFilePath, const CCRect& tRect );

    /// @} end of initializers


    /// @{
    /// @name Functions inherited from CCTextureProtocol

		virtual KDvoid				setTexture ( CCTexture2D* pTexture );
		virtual CCTexture2D*		getTexture ( KDvoid );

		/** conforms to CCTextureProtocol protocol */
		inline ccBlendFunc			getBlendFunc ( KDvoid ) { return m_tBlendFunc; }

		/** conforms to CCTextureProtocol protocol */
		inline KDvoid				setBlendFunc ( ccBlendFunc tBlendFunc ) { m_tBlendFunc = tBlendFunc; }

    /// @}


    /// @{
    /// @name Functions inherited from CCNode

		virtual KDvoid				setScale  ( KDfloat fScale );
		virtual KDvoid				setScaleX ( KDfloat fScaleX );
		virtual KDvoid				setScaleY ( KDfloat fScaleY );

		virtual KDvoid				setPosition ( const CCPoint& tPosition );
		
		virtual KDvoid				setRotation  ( KDfloat fRotation  );
		virtual KDvoid				setRotationX ( KDfloat fRotationX );
		virtual KDvoid				setRotationY ( KDfloat fRotationY );
		
		virtual KDvoid				setSkewX ( KDfloat fSkewX );
		virtual KDvoid				setSkewY ( KDfloat sSkewY );

		virtual KDvoid				removeChild ( CCNode* pChild, KDbool bCleanup );
		virtual KDvoid				removeAllChildrenWithCleanup ( KDbool bCleanup );

		virtual KDvoid				reorderChild ( CCNode* pChild, KDint nZOrder );

		virtual KDvoid				addChild ( CCNode* pChild );
		virtual KDvoid				addChild ( CCNode* pChild, KDint nZOrder );
		virtual KDvoid				addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );

		virtual KDvoid				sortAllChildren ( KDvoid );

		virtual KDvoid				setVertexZ ( KDfloat fVertexZ );
		
		virtual KDvoid				setAnchorPoint ( const CCPoint& tAnchorPoint );
		virtual KDvoid				ignoreAnchorPointForPosition ( KDbool bIgnore );
		
		virtual KDvoid				setVisible ( KDbool bVisible );

		virtual KDvoid				draw ( KDvoid );

    /// @}


    /// @{
    /// @name Functions inherited from CCNodeRGBA

		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tParentColor );

		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		virtual KDbool				 isOpacityModifyRGB ( KDvoid );
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB ); 

		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity );

    /// @}


    /// @{
    /// @name BatchNode methods

		/**
		 *	Updates the quad according the rotation, position, scale values. 
		 */
		virtual KDvoid				updateTransform ( KDvoid );

		/**
		 *	Returns the batch node object if this sprite is rendered by CCSpriteBatchNode
		 *
		 *	@return The CCSpriteBatchNode object if this sprite is rendered by CCSpriteBatchNode,
		 *          NULL if the sprite isn't used batch node.
		 */
		virtual CCSpriteBatchNode*	getBatchNode ( KDvoid );

		/**
		 *	Sets the batch node to sprite
		 *	@warning This method is not recommended for game developers. Sample code for using batch node
		 *	@code
		 *	CCSpriteBatchNode *batch = CCSpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
		 *	CCSprite *sprite = CCSprite::createWithTexture(batch->getTexture(), CCRectMake(0, 0, 57, 57));
		 *	batch->addChild(sprite);
		 *	layer->addChild(batch);
		 *	@endcode
		 */
		virtual KDvoid				setBatchNode ( CCSpriteBatchNode* pSpriteBatchNode );

    /// @} end of BatchNode methods


    /// @{
    /// @name Texture methods

		/**
		 *	Updates the texture rect of the CCSprite in points.
		 *	It will call setTextureRect:rotated:untrimmedSize with rotated = NO, and utrimmedSize = rect.size.
		 */
		virtual KDvoid				setTextureRect ( const CCRect& tRect );

		/**
		 *	Sets the texture rect, rectRotated and untrimmed size of the CCSprite in points.
		 *	It will update the texture coordinates and the vertex rectangle.
		 */
		virtual KDvoid				setTextureRect ( const CCRect& tRect, KDbool bRotated, const CCSize& tUntrimmedSize );

		/**
		 *	Sets the vertex rect.
		 *	It will be called internally by setTextureRect.
		 *	Useful if you want to create 2x images from SD images in Retina Display.
		 *	Do not call it manually. Use setTextureRect instead.
		 */
		virtual KDvoid				setVertexRect ( const CCRect& tRect );

    /// @} end of texture methods


    /// @{
    /// @name Frames methods

		/**
		 *	Sets a new display frame to the CCSprite.
		 */
		virtual KDvoid				setDisplayFrame ( CCSpriteFrame* pNewFrame );

		/**
		 *	Returns whether or not a CCSpriteFrame is being displayed
		 */
		virtual KDbool				isFrameDisplayed ( CCSpriteFrame* pFrame );

		/**
		 *	Returns the current displayed frame.
		 */
		virtual CCSpriteFrame*		displayFrame ( KDvoid );

    /// @} End of frames methods


    /// @{
    /// @name Animation methods

		/**
		 *	Changes the display frame with animation name and index.
		 *	The animation name will be get from the CCAnimationCache
		 */
		virtual KDvoid				setDisplayFrameWithAnimationName ( const KDchar* szAnimationName, KDint nFrameIndex );

    /// @}


    /// @{
    /// @name Sprite Properties' setter/getters

		/** 
		 *	Whether or not the Sprite needs to be updated in the Atlas.
		 *
		 *	@return true if the sprite needs to be updated in the Atlas, false otherwise.
		 */
		inline virtual KDbool		isDirty ( KDvoid ) {	return m_bDirty; }		

		/** 
		 *	Makes the Sprite to be updated in the Atlas.
		 */
		inline virtual KDvoid		setDirty ( KDbool bDirty ) { m_bDirty = bDirty; }

		/**
		 *	Returns the quad (tex coords, vertex coords and color) information. 
		 */
		inline ccV3F_C4B_T2F_Quad	getQuad ( KDvoid ) { return m_tQuad; }

		/** 
		 *	Returns whether or not the texture rectangle is rotated.
		 */
		inline KDbool				isTextureRectRotated ( KDvoid ) { return m_bRectRotated;	}
		
		/** 
		 *	Returns the index used on the TextureAtlas. 
		 */
		inline KDuint				getAtlasIndex ( KDvoid ) { return m_uAtlasIndex; }

		/** 
		 *	Sets the index used on the TextureAtlas.
		 *	@warning Don't modify this value unless you know what you are doing
		 */
		inline KDvoid				setAtlasIndex ( KDuint uAtlasIndex ) { m_uAtlasIndex = uAtlasIndex; }

		/** 
		 *	Returns the rect of the CCSprite in points 
		 */
		inline const CCRect&		getTextureRect ( KDvoid ) { return m_tRect; }

		/**
		 *	Gets the weak reference of the CCTextureAtlas when the sprite is rendered using via CCSpriteBatchNode
		 */
		inline CCTextureAtlas*		getTextureAtlas ( KDvoid ) { return m_pTextureAtlas; }

		/**
		 *	Sets the weak reference of the CCTextureAtlas when the sprite is rendered using via CCSpriteBatchNode
		 */
		inline KDvoid				setTextureAtlas ( CCTextureAtlas* pTextureAtlas ) { m_pTextureAtlas = pTextureAtlas; }

		/** 
		 *	Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
		 */
		inline const CCPoint&		getOffsetPosition ( KDvoid ) { return m_tOffsetPosition; }

		/** 
		 *	Returns the flag which indicates whether the sprite is flipped horizontally or not.
		 *
		 *	It only flips the texture of the sprite, and not the texture of the sprite's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	sprite->setScaleX(sprite->getScaleX() * -1);
		 *
		 *	 @return true if the sprite is flipped horizaontally, false otherwise.
		 */
		KDbool						isFlipX ( KDvoid );

		/**
		 *	Sets whether the sprite should be flipped horizontally or not.
		 *
		 *	@param bFlipX true if the sprite should be flipped horizaontally, false otherwise.
		 */
		KDvoid						setFlipX ( KDbool bFlipX );

		/** 
		 *	Return the flag which indicates whether the sprite is flipped vertically or not.
		 * 
		 *	It only flips the texture of the sprite, and not the texture of the sprite's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	sprite->setScaleY(sprite->getScaleY() * -1);
		 * 
		 *	@return true if the sprite is flipped vertically, flase otherwise.
		 */
		KDbool						isFlipY ( KDvoid );

		/**
		 *	Sets whether the sprite should be flipped vertically or not.
		 *
		 *	@param bFlipY true if the sprite should be flipped vertically, flase otherwise.
		 */
		KDvoid						setFlipY ( KDbool bFlipY );

    /// @} End of Sprite properties getter/setters


	protected :

		KDvoid						updateColor ( KDvoid );
		virtual KDvoid				setTextureCoords ( CCRect tRect );
		virtual KDvoid				updateBlendFunc ( KDvoid );
		virtual KDvoid				setReorderChildDirtyRecursively ( KDvoid );
		virtual KDvoid				setDirtyRecursively ( KDbool bValue );

	protected :
	
		//
		// Data used when the sprite is rendered using a CCSpriteSheet
		//
		CCTextureAtlas*             m_pTextureAtlas;				/// CCSpriteBatchNode texture atlas (weak reference)
		KDuint                      m_uAtlasIndex;					/// Absolute (real) Index on the SpriteSheet
		CCSpriteBatchNode*          m_pBatchNode;					/// Used batch node (weak reference)
		
		KDbool                      m_bDirty;						/// Whether the sprite needs to be updated
		KDbool                      m_bRecursiveDirty;				/// Whether all of the sprite's children needs to be updated
		KDbool                      m_bHasChildren;					/// Whether the sprite contains children
		KDbool                      m_bShouldBeHidden;				/// should not be drawn because one of the ancestors is not visible
		CCAffineTransform			m_tTransformToBatch;	

		//
		// Data used when the sprite is self-rendered
		//
		ccBlendFunc                 m_tBlendFunc;					/// It's required for CCTextureProtocol inheritance
		CCTexture2D*                m_pTexture;						/// CCTexture2D object that is used to render the sprite
		
		//
		// Shared data
		//

		// texture
		CCRect                      m_tRect;						/// Retangle of CCTexture2D
		KDbool                      m_bRectRotated;					/// Whether the texture is rotated

		// Offset Position (used by Zwoptex)
		CCPoint                     m_tOffsetPosition;			
		CCPoint                     m_tUnflippedOffsetPositionFromCenter;
		
		// vertex coords, texture coords and color info
		ccV3F_C4B_T2F_Quad          m_tQuad;
		
		// opacity and RGB protocol
		KDbool                      m_bOpacityModifyRGB;
		
		// image is flipped
		KDbool                      m_bFlipX;						/// Whether the sprite is flipped horizaontally or not.
		KDbool                      m_bFlipY;						/// Whether the sprite is flipped vertically or not.
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CCSprite_h__
