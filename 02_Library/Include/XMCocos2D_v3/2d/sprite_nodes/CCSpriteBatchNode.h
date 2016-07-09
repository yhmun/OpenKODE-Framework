/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteBatchNode.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2009      Matt Oswald
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

#ifndef __CCSpriteBatchNode_h__
#define __CCSpriteBatchNode_h__

#include <vector>

#include "../base_nodes/CCNode.h"
#include "../textures/CCTextureAtlas.h"

NS_CC_BEGIN

/**
 *	@addtogroup sprite_nodes
 *	@{
 */


class Sprite;

/** 
 *	SpriteBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
 *	(often known as "batch draw").
 *
 *	A SpriteBatchNode can reference one and only one texture (one image file, one texture atlas).
 *	Only the Sprites that are contained in that texture can be added to the SpriteBatchNode.
 *	All Sprites added to a SpriteBatchNode are drawn in one OpenGL ES draw call.
 *	If the Sprites are not added to a SpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
 *
 *
 *	Limitations:
 *		- The only object that is accepted as child (or grandchild, grand-grandchild, etc...) is Sprite or any subclass of Sprite. eg: particles, labels and layer can't be added to a SpriteBatchNode.
 *		- Either all its children are Aliased or Antialiased. It can't be a mix. This is because "alias" is a property of the texture, and all the sprites share the same texture.
 * 
 *	@since v0.7.1
 */
class CC_DLL SpriteBatchNode : public Node, public TextureProtocol
{
    static const KDint		DEFAULT_CAPACITY = 29;

	public :

		/**
		 *	creates a SpriteBatchNode with a texture2d and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 */
		static SpriteBatchNode*		createWithTexture ( Texture2D* pTex, KDint32 nCapacity = DEFAULT_CAPACITY );

		/**
		 *	creates a SpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 *	The file will be loaded using the TextureMgr.
		 */
		static SpriteBatchNode*		create ( const KDchar* pFileImage, KDint32 nCapacity = DEFAULT_CAPACITY);
		
		/**
		 *	@js ctor
		 */
		SpriteBatchNode ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~SpriteBatchNode ( KDvoid );

		/** 
		 *	initializes a SpriteBatchNode with a texture2d and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 */
		KDbool						initWithTexture ( Texture2D* pTex, KDint32 nCapacity );

		/**
		 *	initializes a SpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and a capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 *	The file will be loaded using the TextureMgr.
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithFile ( const KDchar* pFileImage, KDint32 nCapacity );
		KDbool						init ( KDvoid );

		/** returns the TextureAtlas object */
		inline TextureAtlas*		getTextureAtlas ( KDvoid ) { return m_pTextureAtlas; }

		/** sets the TextureAtlas object */
		inline KDvoid				setTextureAtlas ( TextureAtlas* pTextureAtlas )
		{ 
			if ( pTextureAtlas != m_pTextureAtlas )
			{
				CC_SAFE_RETAIN ( pTextureAtlas );
				CC_SAFE_RELEASE ( m_pTextureAtlas );
				m_pTextureAtlas = pTextureAtlas;
			}
		}

		/** 
		 *	returns an array with the descendants (children, gran children, etc.). 
		 *	This is specific to BatchNode. In order to use the children, use getChildren() instead 
		 */
		inline const std::vector<Sprite*>&		getDescendants ( KDvoid ) const { return m_aDescendants; }

		KDvoid						increaseAtlasCapacity ( KDvoid );

		/**
		 *	removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
		 *	@warning Removing a child from a SpriteBatchNode is very slow
		 */
		KDvoid						removeChildAtIndex ( KDint nIndex, KDbool bDoCleanup );

		KDvoid						appendChild ( Sprite* pSprite );
		KDvoid						removeSpriteFromAtlas ( Sprite* pSprite );

		KDint						rebuildIndexInOrder ( Sprite* pParent, KDint nIndex );
		KDint						highestAtlasIndexInChild ( Sprite* pSprite );
		KDint						lowestAtlasIndexInChild ( Sprite* pSprite );
		KDint						atlasIndexForChild ( Sprite* pSprite, KDint z );

		/* Sprites use this to start sortChildren, don't call this manually */
		KDvoid						reorderBatch ( KDbool bReorder );

		//
		// Overrides
		//
		// TextureProtocol
		virtual Texture2D*			getTexture ( KDvoid ) const override;
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;
		
		/**
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var setBlendFunc(var src, var dst)
		 *	@endcode
		 *	@lua NA 
		 */
		virtual KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc ) override;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual const BlendFunc&	getBlendFunc ( KDvoid ) const override;

		virtual KDvoid				visit ( KDvoid ) override;
		virtual KDvoid				addChild ( Node* pChild ) override { Node::addChild ( pChild ); }
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder ) override { Node::addChild ( pChild, zOrder); }
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;
		virtual KDvoid				reorderChild ( Node* pChild, int zOrder) override;
        
		virtual KDvoid				removeChild ( Node* pChild, KDbool bCleanup ) override;
		virtual KDvoid				removeAllChildrenWithCleanup ( KDbool bCleanup ) override;
		virtual KDvoid				sortAllChildren ( KDvoid ) override;
		virtual KDvoid				draw ( KDvoid ) override;

	protected :

		/** 
		 *	Inserts a quad at a certain index into the texture atlas. The Sprite won't be added into the children array.
		 *	This method should be called only when you are dealing with very big AtlasSrite and when most of the Sprite won't be updated.
		 *	For example: a tile map (TMXMap) or a label with lots of characters (LabelBMFont)
		 */
		KDvoid						insertQuadFromSprite ( Sprite* pSprite, KDint nIndex );

		/**
		 *	Updates a quad at a certain index into the texture atlas. The Sprite won't be added into the children array.
		 *	This method should be called only when you are dealing with very big AtlasSrite and when most of the Sprite won't be updated.
		 *	For example: a tile map (TMXMap) or a label with lots of characters (LabelBMFont)
		 */
		KDvoid						updateQuadFromSprite ( Sprite* pSprite, KDint nIndex );

		/**
		 *	This is the opposite of "addQuadFromSprite.
		 *	It add the sprite to the children and descendants array, but it doesn't update add it to the texture atlas
		 */
		SpriteBatchNode*			addSpriteWithoutQuad ( Sprite* pChild, KDint z, KDint nTag );

		KDvoid						updateAtlasIndex ( Sprite* pSprite, KDint* pCurIndex );
		KDvoid						swap ( KDint nOldIndex, KDint nNewIndex );
		KDvoid						updateBlendFunc ( KDvoid );

		TextureAtlas*				m_pTextureAtlas;
		BlendFunc					m_tBlendFunc;

		// all descendants: children, grand children, etc...
		// There is not need to retain/release these objects, since they are already retained by _children
		// So, using std::vector<Sprite*> is slightly faster than using cocos2d::Array for this particular case
		std::vector<Sprite*>		m_aDescendants;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif	// __CCSpriteBatchNode_h__
