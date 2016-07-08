/* -----------------------------------------------------------------------------------
 *
 *      File            CCSprite.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCSprite_h__
#define __CCSprite_h__

#include "../base_nodes/CCNode.h"
#include "../textures/CCTextureAtlas.h"
#include "../../ccTypes.h"
#include "../../base/CCDictionary.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN
#include "../../physics/CCPhysicsBody.h"

NS_CC_BEGIN

class SpriteBatchNode;
class SpriteFrame;
class Animation;
class Rect;
class Point;
class Size;
class Texture2D;
struct transformValues_;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/**
 * Sprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) )
 *
 * Sprite can be created with an image, or with a sub-rectangle of an image.
 *
 * If the parent or any of its ancestors is a SpriteBatchNode then the following features/limitations are valid
 *    - Features when the parent is a BatchNode:
 *        - MUCH faster rendering, specially if the SpriteBatchNode has many children. All the children will be drawn in a single batch.
 *
 *    - Limitations
 *        - Camera is not supported yet (eg: OrbitCamera action doesn't work)
 *        - GridBase actions are not supported (eg: Lens, Ripple, Twirl)
 *        - The Alias/Antialias property belongs to SpriteBatchNode, so you can't individually set the aliased property.
 *        - The Blending function property belongs to SpriteBatchNode, so you can't individually set the blending function property.
 *        - Parallax scroller is not supported, but can be simulated with a "proxy" sprite.
 *
 *  If the parent is an standard Node, then Sprite behaves like any other Node:
 *    - It supports blending functions
 *    - It supports aliasing / antialiasing
 *    - But the rendering will be slower: 1 draw per children.
 *
 * The default anchorPoint in Sprite is (0.5, 0.5).
 */
class CC_DLL Sprite : public NodeRGBA, public TextureProtocol
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		static const KDint			INDEX_NOT_INITIALIZED = -1;		/// Sprite invalid index on the SpriteBatchNode

		/// @{
		/// @name Creators

		/**
		 *	Creates an empty sprite without texture. You can call setTexture method subsequently.
		 *
		 *	@return An empty sprite object that is marked as autoreleased.
		 */
		static Sprite*				create ( KDvoid );

		/**
		 *	Creates a sprite with an image filename.
		 *
		 *	After creation, the rect of sprite will be the size of the image,
		 *	and the offset will be (0,0).
		 *
		 *	@param   filename The string which indicates a path to image file, e.g., "scene1/monster.png".
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				create ( const std::string& sFilename );

		/**
		 *	Creates a sprite with an image filename and a rect.
		 *
		 *	@param   filename The string wich indicates a path to image file, e.g., "scene1/monster.png"
		 *	@param   rect        Only the contents inside rect of filename's texture will be applied for this sprite.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				create ( const std::string& sFilename, const Rect& tRect );

		/**
		 *	Creates a sprite with an exsiting texture contained in a Texture2D object
		 *	After creation, the rect will be the size of the texture, and the offset will be (0,0).
		 *
		 *	@param   texture    A pointer to a Texture2D object.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				createWithTexture ( Texture2D* pTexture );

		/**
		 *	Creates a sprite with a texture and a rect.
		 *
		 *	After creation, the offset will be (0,0).
		 *
		 *	@param   texture    A pointer to an existing Texture2D object.
		 *                      You can use a Texture2D object for many sprites.
		 *	@param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				createWithTexture ( Texture2D* pTexture, const Rect& tRect );

		/**
		 *	Creates a sprite with an sprite frame.
		 *
		 *	@param   pSpriteFrame    A sprite frame which involves a texture and a rect
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				createWithSpriteFrame ( SpriteFrame* pSpriteFrame );

		/**
		 *	Creates a sprite with an sprite frame name.
		 *
		 *	A SpriteFrame will be fetched from the SpriteFrameCache by spriteFrameName param.
		 *	If the SpriteFrame doesn't exist it will raise an exception.
		 *
		 *	@param   spriteFrameName A null terminated string which indicates the sprite frame name.
		 *	@return  A valid sprite object that is marked as autoreleased.
		 */
		static Sprite*				createWithSpriteFrameName ( const std::string& sSpriteFrameName );

		/// @}  end of creators group


		/// @{
		/// @name Initializers

		/**
		 *	Default constructor
		 *	@js ctor
		 */
		Sprite ( KDvoid );

		/**
		 *	Default destructor
		 *	@js  NA
		 *	@lua NA
		 */
		virtual ~Sprite ( KDvoid );

		/**
		 *	Initializes an empty sprite with nothing init.
		 */
		virtual KDbool				init ( KDvoid );

		/**
		 *	Initializes a sprite with a texture.
		 *
		 *	After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
		 *
		 *	@param   texture    A pointer to an existing Texture2D object.
		 *                      You can use a Texture2D object for many sprites.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( Texture2D* pTexture );

		/**
		 *	Initializes a sprite with a texture and a rect.
		 *
		 *	After initialization, the offset will be (0,0).
		 *
		 *	@param   texture     A pointer to an exisiting Texture2D object.
		 *                       You can use a Texture2D object for many sprites.
		 *	@param   rect        Only the contents inside rect of this texture will be applied for this sprite.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( Texture2D* pTexture, const Rect& tRect );

		/**
		 *	Initializes a sprite with a texture and a rect in points, optionally rotated.
		 *
		 *	After initialization, the offset will be (0,0).
		 *	@note    This is the designated initializer.
		 *
		 *	@param   texture     A Texture2D object whose texture will be applied to this sprite.
		 *	@param   rect        A rectangle assigned the contents of texture.
		 *	@param   rotated     Whether or not the texture rectangle is rotated.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithTexture ( Texture2D* pTexture, const Rect& tRect, KDbool bRotated );

		/**
		 *	Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
		 *
		 *	@param   pSpriteFrame  A SpriteFrame object. It should includes a valid texture and a rect
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithSpriteFrame ( SpriteFrame* pSpriteFrame );

		/**
		 *	Initializes a sprite with an sprite frame name.
		 *
		 *	A SpriteFrame will be fetched from the SpriteFrameCache by name.
		 *	If the SpriteFrame doesn't exist it will raise an exception.
		 *
		 *	@param   spriteFrameName  A key string that can fected a volid SpriteFrame from SpriteFrameCache
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 */
		virtual KDbool				initWithSpriteFrameName ( const std::string& sSpriteFrameName );

		/**
		 *	Initializes a sprite with an image filename.
		 *
		 *	This method will find filename from local file system, load its content to Texture2D,
		 *	then use Texture2D to create a sprite.
		 *	After initialization, the rect used will be the size of the image. The offset will be (0,0).
		 *
		 *	@param   filename The path to an image file in local file system
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 *	@js      init
		 *	@lua     init
		 */
		virtual KDbool				initWithFile ( const std::string& sFilename );

		/**
		 *	Initializes a sprite with an image filename, and a rect.
		 *
		 *	This method will find filename from local file system, load its content to Texture2D,
		 *	then use Texture2D to create a sprite.
		 *	After initialization, the offset will be (0,0).
		 *
		 *	@param   filename The path to an image file in local file system.
		 *	@param   rect        The rectangle assigned the content area from texture.
		 *	@return  true if the sprite is initialized properly, false otherwise.
		 *	@js      init
		 *	@lua     init
		 */
		virtual KDbool				initWithFile ( const std::string& sFilename, const Rect& tRect );

		/// @} end of initializers

		/// @{
		/// @name BatchNode methods

		/**
		 *	Updates the quad according the rotation, position, scale values.
		 */
		virtual KDvoid				updateTransform ( KDvoid );

		/**
		 *	Returns the batch node object if this sprite is rendered by SpriteBatchNode
		 *
		 *	@return The SpriteBatchNode object if this sprite is rendered by SpriteBatchNode,
		 *          NULL if the sprite isn't used batch node.
		 */
		virtual SpriteBatchNode*	getBatchNode ( KDvoid );

		/**
		 *	Sets the batch node to sprite
		 *	@warning This method is not recommended for game developers. Sample code for using batch node
		 *	@code
		 *	SpriteBatchNode *batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
		 *	Sprite *sprite = Sprite::createWithTexture(batch->getTexture(), Rect(0, 0, 57, 57));
		 *	batch->addChild(sprite);
		 *	layer->addChild(batch);
		 *	@endcode
		 */
		virtual KDvoid				setBatchNode ( SpriteBatchNode* pSpriteBatchNode );

		/// @} end of BatchNode methods



		/// @{
		/// @name Texture methods

		/**
		 *	Updates the texture rect of the Sprite in points.
		 *	It will call setTextureRect(const Rect& tRect, bool rotated, const Size& untrimmedSize) with \p rotated = false, and \p utrimmedSize = rect.size.
		 */
		virtual KDvoid				setTextureRect ( const Rect& tRect );

		/**
		 *	Sets the texture rect, rectRotated and untrimmed size of the Sprite in points.
		 *	It will update the texture coordinates and the vertex rectangle.
		 */
		virtual KDvoid				setTextureRect ( const Rect& tRect, KDbool bRotated, const Size& tUntrimmedSize );

		/**
		 *	Sets the vertex rect.
		 *	It will be called internally by setTextureRect.
		 *	Useful if you want to create 2x images from SD images in Retina Display.
		 *	Do not call it manually. Use setTextureRect instead.
		 */
		virtual KDvoid				setVertexRect ( const Rect& tRect );

		/// @} end of texture methods



		/// @{
		/// @name Frames methods

		/**
		 *	Sets a new display frame to the Sprite.
		 */
		virtual KDvoid				setDisplayFrame ( SpriteFrame* pNewFrame );

		// Added By XMSoft
		virtual KDvoid				setDisplayFrameName ( const KDchar* szNewFrameName );

		/**
		 *	Returns whether or not a SpriteFrame is being displayed
		 */
		virtual KDbool				isFrameDisplayed ( SpriteFrame* pFrame ) const;

		/**
		 *	Returns the current displayed frame.
		 */
		virtual SpriteFrame*		getDisplayFrame ( KDvoid );

		/// @} End of frames methods


		/// @{
		/// @name Animation methods
		/**
		 *	Changes the display frame with animation name and index.
		 *	The animation name will be get from the AnimationCache
		 */
		virtual KDvoid				setDisplayFrameWithAnimationName ( const std::string& sAnimationName, KDint nFrameIndex );
		/// @}


		/// @{
		/// @name Sprite Properties' setter/getters

		/**
		 *	Whether or not the Sprite needs to be updated in the Atlas.
		 *
		 *	@return true if the sprite needs to be updated in the Atlas, false otherwise.
		 */
		virtual KDbool				isDirty ( KDvoid ) const { return m_bDirty; }

		/**
		 *	Makes the Sprite to be updated in the Atlas.
		 */
		virtual KDvoid				setDirty ( KDbool bDirty ) { m_bDirty = bDirty; }

		/**
		 *	Returns the quad (tex coords, vertex coords and color) information.
		 *	@js  NA
		 *	@lua NA
		 */
		inline V3F_C4B_T2F_Quad		getQuad ( KDvoid ) const { return m_tQuad; }

		/**
		 *	Returns whether or not the texture rectangle is rotated.
		 */
		inline KDbool				isTextureRectRotated ( KDvoid ) const { return m_bRectRotated; }

		/**
		 *	Returns the index used on the TextureAtlas.
		 */
		inline KDint				getAtlasIndex ( KDvoid ) const { return m_nAtlasIndex; }

		/**
		 *	Sets the index used on the TextureAtlas.
		 *	@warning Don't modify this value unless you know what you are doing
		 */
		inline KDvoid				setAtlasIndex ( KDint nAtlasIndex ) { m_nAtlasIndex = nAtlasIndex; }

		/**
		 *	Returns the rect of the Sprite in points
		 */
		inline const Rect&			getTextureRect ( KDvoid ) { return m_tRect; }

		/**
		 *	Gets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode
		 */
		inline TextureAtlas*		getTextureAtlas ( KDvoid ) { return m_pTextureAtlas; }

		/**
		 *	Sets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode
		 */
		inline KDvoid				setTextureAtlas ( TextureAtlas* pTextureAtlas ) { m_pTextureAtlas = pTextureAtlas; }

		/**
		 *	Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
		 */
		inline const Point&			getOffsetPosition ( KDvoid ) const { return m_tOffsetPosition; }


		/**
		 *	Returns the flag which indicates whether the sprite is flipped horizontally or not.
		 *
		 *	It only flips the texture of the sprite, and not the texture of the sprite's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	sprite->setScaleX(sprite->getScaleX() * -1);
		 *
		 *	@return true if the sprite is flipped horizontally, false otherwise.
		 */
		KDbool						isFlippedX ( KDvoid ) const;

		/**
		 *	Sets whether the sprite should be flipped horizontally or not.
		 *
		 *	@param flippedX true if the sprite should be flipped horizontally, false otherwise.
		 */
		KDvoid						setFlippedX ( KDbool bFlippedX );

		/**
		 *	Return the flag which indicates whether the sprite is flipped vertically or not.
		 *
		 *	It only flips the texture of the sprite, and not the texture of the sprite's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	sprite->setScaleY(sprite->getScaleY() * -1);
		 *
		 *	@return true if the sprite is flipped vertically, false otherwise.
		 */
		KDbool						isFlippedY ( KDvoid ) const;

		/**
		 *	Sets whether the sprite should be flipped vertically or not.
		 *
		 *	@param flippedY true if the sprite should be flipped vertically, false otherwise.
		 */
		KDvoid						setFlippedY ( KDbool bFlippedY );

		/// @} End of Sprite properties getter/setters

		//
		// Overrides
		//
		/// @{
		/// @name Functions inherited from TextureProtocol
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;
		virtual Texture2D*			getTexture ( KDvoid ) const override;

		/**
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var setBlendFunc(var src, var dst)
		 *	In lua: local setBlendFunc(local src, local dst)
		 *	@endcode
		 */
		inline KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc ) override { m_tBlendFunc = tBlendFunc; }

		/**
		 *	@js  NA
		 *	@lua NA
		 */
		inline const BlendFunc&		getBlendFunc ( KDvoid ) const override { return m_tBlendFunc; }
		/// @}

		/// @{
		/// @name Functions inherited from Node
		virtual KDvoid				setScaleX ( KDfloat fScaleX ) override;
		virtual KDvoid				setScaleY ( KDfloat fScaleY ) override;
		virtual KDvoid				setScale  ( KDfloat fScaleX, KDfloat fScaleY ) override;

		/**
		 *	@js  NA
		 *	@lua NA
		 */
		virtual KDvoid				setPosition  ( const Point& tPos ) override;
		
		virtual KDvoid				setRotation  ( KDfloat fRotation ) override;
		virtual KDvoid				setRotationX ( KDfloat fRotationX ) override;
		virtual KDvoid				setRotationY ( KDfloat fRotationY ) override;
		
		virtual KDvoid				setSkewX ( KDfloat sx ) override;
		virtual KDvoid				setSkewY ( KDfloat sy ) override;

		virtual KDvoid				removeChild ( Node* pChild, KDbool bCleanup ) override;
		virtual KDvoid				removeAllChildrenWithCleanup ( KDbool bCleanup ) override;

		virtual KDvoid				reorderChild ( Node* pChild, KDint zOrder ) override;

		virtual KDvoid				addChild ( Node* pChild ) override;
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder ) override;
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;

		virtual KDvoid				sortAllChildren ( KDvoid ) override;

		virtual KDvoid				setScale ( KDfloat fScale ) override;
		virtual KDvoid				setVertexZ ( KDfloat fVertexZ ) override;

		virtual KDvoid				setAnchorPoint ( const Point& tAnchor ) override;
		virtual KDvoid				ignoreAnchorPointForPosition ( KDbool bValue ) override;
		
		virtual KDvoid				setVisible ( KDbool bVisible ) override;

		virtual KDvoid				draw ( KDvoid ) override;
		/// @}

		/// @{
		/// @name Functions inherited from NodeRGBA
		virtual KDvoid				setColor ( const Color3B& tColor3 ) override;
		virtual KDvoid				updateDisplayedColor ( const Color3B& tParentColor ) override;

		virtual KDvoid				setOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid				setOpacityModifyRGB ( KDbool bModify ) override;
		virtual KDbool				isOpacityModifyRGB ( KDvoid ) const override;
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity ) override;
		/// @}

	protected :

		KDvoid						updateColor ( KDvoid );
		virtual KDvoid				setTextureCoords ( Rect tRect );
		virtual KDvoid				updateBlendFunc ( KDvoid );
		virtual KDvoid				setReorderChildDirtyRecursively ( KDvoid );
		virtual KDvoid				setDirtyRecursively ( KDbool bValue );

		//
		// Data used when the sprite is rendered using a SpriteSheet
		//
		TextureAtlas*				m_pTextureAtlas;		/// SpriteBatchNode texture atlas (weak reference)
		KDint						m_nAtlasIndex;			/// Absolute (real) Index on the SpriteSheet
		SpriteBatchNode*			m_pBatchNode;			/// Used batch node (weak reference)

		KDbool						m_bDirty;				/// Whether the sprite needs to be updated
		KDbool						m_bRecursiveDirty;		/// Whether all of the sprite's children needs to be updated
		KDbool						m_bHasChildren;			/// Whether the sprite contains children
		KDbool						m_bShouldBeHidden;		/// should not be drawn because one of the ancestors is not visible
		AffineTransform				m_tTransformToBatch;

		//
		// Data used when the sprite is self-rendered
		//
		BlendFunc					m_tBlendFunc;			/// It's required for TextureProtocol inheritance
		Texture2D*					m_pTexture;				/// Texture2D object that is used to render the sprite

		//
		// Shared data
		//

		// texture
		Rect						m_tRect;				/// Retangle of Texture2D
		KDbool						m_bRectRotated;			/// Whether the texture is rotated

		// Offset Position (used by Zwoptex)
		Point						m_tOffsetPosition;
		Point						m_tUnflippedOffsetPositionFromCenter;

		// vertex coords, texture coords and color info
		V3F_C4B_T2F_Quad			m_tQuad;

		// opacity and RGB protocol
		KDbool						m_bOpacityModifyRGB;

		// image is flipped
		KDbool						m_bFlippedX;			/// Whether the sprite is flipped horizontally or not
		KDbool						m_bFlippedY;			/// Whether the sprite is flipped vertically or not
};

//	end of sprite_nodes group
/// @}

NS_CC_END

#endif	// __CCSprite_h__
