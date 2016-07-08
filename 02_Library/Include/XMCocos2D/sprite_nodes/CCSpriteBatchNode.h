/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteBatchNode.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCSpriteBatchNode_h__
#define __CCSpriteBatchNode_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"
#include "../textures/CCTextureAtlas.h"
#include "../cocoa/CCArray.h"

NS_CC_BEGIN
    
/**
 * @addtogroup sprite_nodes
 * @{
 */

#define kDefaultSpriteBatchCapacity		29

class CCSprite;

/** 
 *	CCSpriteBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
 *	(often known as "batch draw").
 *
 *	A CCSpriteBatchNode can reference one and only one texture (one image file, one texture atlas).
 *	Only the CCSprites that are contained in that texture can be added to the CCSpriteBatchNode.
 *	All CCSprites added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call.
 *	If the CCSprites are not added to a CCSpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
 *
 *
 *	Limitations:
 *		- The only object that is accepted as child (or grandchild, grand-grandchild, etc...) is CCSprite or any subclass of CCSprite. eg: particles, labels and layer can't be added to a CCSpriteBatchNode.
 *		- Either all its children are Aliased or Antialiased. It can't be a mix. This is because "alias" is a property of the texture, and all the sprites share the same texture.
 *  
 *  @since	v0.7.1
 */
class CCSpriteBatchNode : public CCNode, public CCTextureProtocol
{
	public :
				 CCSpriteBatchNode ( KDvoid );
		virtual ~CCSpriteBatchNode ( KDvoid );

		/** 
		 *	creates a CCSpriteBatchNode with a texture2d and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 */
		static   CCSpriteBatchNode*		createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity = kDefaultSpriteBatchCapacity );

		/**
		 *	creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 *	The file will be loaded using the TextureMgr.
		 */
		static   CCSpriteBatchNode*		create ( const KDchar* szFilePath, KDuint uCapacity = kDefaultSpriteBatchCapacity );

	public :

		/** 
		 *	initializes a CCSpriteBatchNode with a texture2d and capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 */
		virtual KDbool			initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity );

		/** 
		 *	initializes a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and a capacity of children.
		 *	The capacity will be increased in 33% in runtime if it run out of space.
		 *	The file will be loaded using the TextureMgr.
		 */
		virtual KDbool			initWithFile ( const KDchar* szFilePath, KDuint uCapacity );

		virtual KDbool			init ( KDvoid );
	
		// property

		// retain
		inline  CCTextureAtlas*		getTextureAtlas ( KDvoid ) { return m_pTextureAtlas;  }
		inline  KDvoid				setTextureAtlas ( CCTextureAtlas* pTextureAtlas ) 
		{ 
			if ( pTextureAtlas != m_pTextureAtlas )
			{
				CC_SAFE_RETAIN ( pTextureAtlas );
				CC_SAFE_RELEASE ( m_pTextureAtlas );
				m_pTextureAtlas = pTextureAtlas;
			}
		}

		inline CCArray*			getDescendants ( KDvoid ) { return m_pDescendants; }

		KDvoid					increaseAtlasCapacity ( KDvoid );

		/** 
		 *	removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
		 *	@warning Removing a child from a CCSpriteBatchNode is very slow
		 */
		KDvoid					removeChildAtIndex ( KDuint uIndex, KDbool bCleanup );

		KDvoid					insertChild ( CCSprite* pChild, KDuint uIndex );
		KDvoid					appendChild ( CCSprite* pSprite );
		KDvoid					removeSpriteFromAtlas ( CCSprite* pSprite );

		KDuint					rebuildIndexInOrder ( CCSprite* pParent, KDuint uIndex );
		KDuint					highestAtlasIndexInChild ( CCSprite* pSprite );
		KDuint					lowestAtlasIndexInChild ( CCSprite* pSprite );
		KDuint					atlasIndexForChild ( CCSprite* pSprite, KDint nZOrder );
		
		/* Sprites use this to start sortChildren, don't call this manually */
		KDvoid					reorderBatch ( KDbool bReorder );

		// CCTextureProtocol
		virtual CCTexture2D*	getTexture ( KDvoid );
		virtual KDvoid			setTexture ( CCTexture2D* pTexture );

		virtual KDvoid			setBlendFunc ( ccBlendFunc tBlendFunc );
		virtual ccBlendFunc		getBlendFunc ( KDvoid );

		virtual KDvoid			visit ( KDvoid );

		virtual KDvoid			addChild ( CCNode* pChild );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );
		
		virtual KDvoid			reorderChild ( CCNode* pChild, KDint nZOrder );
	        
		virtual KDvoid			removeChild ( CCNode* pChild, KDbool bCleanup );
		virtual KDvoid			removeAllChildrenWithCleanup ( KDbool bCleanup );

		virtual KDvoid			sortAllChildren ( KDvoid );

		virtual KDvoid			draw ( KDvoid );

	protected :

		/** 
		 *	Inserts a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
		 *	This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
		 *	For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
		 */
		KDvoid					insertQuadFromSprite ( CCSprite* pSprite, KDuint uIndex );
		
		/**
		 *	Updates a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
		 *	This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
		 *	For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
		 */
		KDvoid					updateQuadFromSprite ( CCSprite* pSprite, KDuint uIndex );

		/* 
		 *	This is the opposite of "addQuadFromSprite.
         *	It add the sprite to the children and descendants array, but it doesn't update add it to the texture atlas
         */
		CCSpriteBatchNode*		addSpriteWithoutQuad ( CCSprite* pChild, KDuint nZOrder, KDint nTag );

	private :

		KDvoid					updateAtlasIndex ( CCSprite* pSprite, KDint* pCurIndex );
		KDvoid					swap ( KDint nOldIndex, KDint nNewIndex );
		KDvoid					updateBlendFunc ( KDvoid );

	protected :

		CCTextureAtlas*			m_pTextureAtlas;
		ccBlendFunc				m_tBlendFunc;

		// all descendants: children, gran children, etc...
		CCArray*				m_pDescendants;
};

// end of sprite_nodes group
/// @}

NS_CC_END 

#endif // __CCSpriteBatchNode_h__