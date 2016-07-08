/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrameCache.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Jason Booth
 *      Copyright (c) 2009      Robert J Payne
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

#ifndef __CCSpriteFrameCache_h__
#define __CCSpriteFrameCache_h__

/**
 *	To create sprite frames and texture atlas, use this tool:
 *	http://zwoptex.zwopple.com/
 */

#include "CCSpriteFrame.h"
#include "../textures/CCTexture2D.h"
#include "../../base/CCObject.h"
#include <set>
#include <string>

NS_CC_BEGIN

class Dictionary;
class Array;
class Sprite;

/**
 *	@addtogroup sprite_nodes
 *	@{
 */

/** 
 *	@brief Singleton that handles the loading of the sprite frames.
 *	It saves in a cache the sprite frames.
 *	@since v0.9
 */
class CC_DLL SpriteFrameCache : public Object
{
	public :

		/** Returns the shared instance of the Sprite Frame cache */
		static SpriteFrameCache*	getInstance ( KDvoid );

		/** Destroys the cache. It releases all the Sprite Frames and the retained instance. */
		static KDvoid				destroyInstance ( KDvoid );

	protected :

		// MARMALADE: Made this protected not private, as deriving from this class is pretty useful
		SpriteFrameCache ( KDvoid ) : m_pSpriteFrames ( KD_NULL ), m_pSpriteFramesAliases ( KD_NULL ) { }

	public :

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~SpriteFrameCache ( KDvoid );

		KDbool						init ( KDvoid );

	public :

		/**
		 *	Adds multiple Sprite Frames from a plist file.
		 *	A texture will be loaded automatically. The texture name will composed by replacing the .plist suffix with .png
		 *	If you want to use another texture, you should use the addSpriteFramesWithFile(const std::string& plist, const std::string& textureFileName) method.
		 *	@js addSpriteFrames
		 *	@lua addSpriteFrames
		 */
		KDvoid						addSpriteFramesWithFile ( const std::string& sPlist );

		/**
		 *	Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames.
		 *	@since v0.99.5
		 *	@js addSpriteFrames
		 *	@lua addSpriteFrames
		 */
		KDvoid						addSpriteFramesWithFile ( const std::string& sPlist, const std::string& sTextureFileName );

		/**
		 *	Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames. 
		 *	@js addSpriteFrames
		 *	@lua addSpriteFrames
		 */
		KDvoid						addSpriteFramesWithFile ( const std::string& sPlist, Texture2D* pTexture );

		/** 
		 *	Adds an sprite frame with a given name.
		 *	If the name already exists, then the contents of the old name will be replaced with the new one.
		 */
		KDvoid						addSpriteFrame ( SpriteFrame* pFrame, const std::string& sFrameName );

		/**
		 *	Purges the dictionary of loaded sprite frames.
		 *	Call this method if you receive the "Memory Warning".
		 *	In the short term: it will free some resources preventing your app from being killed.
		 *	In the medium term: it will allocate more resources.
		 *	In the long term: it will be the same.
		 */
		KDvoid						removeSpriteFrames ( KDvoid );

		/**
		 *	Removes unused sprite frames.
		 *	Sprite Frames that have a retain count of 1 will be deleted.
		 *	It is convenient to call this method after when starting a new Scene.
		 */
		KDvoid						removeUnusedSpriteFrames ( KDvoid );

		/** Deletes an sprite frame from the sprite frame cache. */
		KDvoid						removeSpriteFrameByName ( const std::string& sName );

		/**
		 *	Removes multiple Sprite Frames from a plist file.
		 *	Sprite Frames stored in this file will be removed.
		 *	It is convenient to call this method when a specific texture needs to be removed.
		 *	@since v0.99.5
		 */
		KDvoid						removeSpriteFramesFromFile ( const std::string& sPlist );

		/**
		 *	Removes all Sprite Frames associated with the specified textures.
		 *	It is convenient to call this method when a specific texture needs to be removed.
		 *	@since v0.995.
		 */
		KDvoid						removeSpriteFramesFromTexture ( Texture2D* pTexture );

		/**
		 *	Returns an Sprite Frame that was previously added.
		 *	If the name is not found it will return nil.
		 *	You should retain the returned copy if you are going to use it.
		 *	@js getSpriteFrame
		 *	@lua getSpriteFrame
		 */
		SpriteFrame*				getSpriteFrameByName ( const std::string& sName );

	private :

		/**
		 *	Adds multiple Sprite Frames with a dictionary. The texture will be associated with the created sprite frames.
		 */
		KDvoid						addSpriteFramesWithDictionary ( Dictionary* pDictionary, Texture2D* pTexture );

		/** 
		 *	Removes multiple Sprite Frames from Dictionary.
		 *	@since v0.99.5
		 */
		KDvoid						removeSpriteFramesFromDictionary ( Dictionary* pDictionary );

	protected :

		Dictionary*					m_pSpriteFrames;
		Dictionary*					m_pSpriteFramesAliases;
		std::set<std::string>*		m_pLoadedFileNames;
};

//	end of sprite_nodes group
/// @}

NS_CC_END

#endif	// __CCSpriteFrameCache_h__
