/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrameCache.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCSpriteFrameCache_h__
#define __CCSpriteFrameCache_h__

#include "CCSpriteFrame.h"
#include "../cocoa/CCDictionary.h"
#include "../textures/CCTexture2D.h"

#include <set>

NS_CC_BEGIN
    
/**
 * @addtogroup sprite_nodes
 * @{
 */

/*
 *	To create sprite frames and texture atlas, use this tool:
 *	http://zwoptex.zwopple.com/
 */

/**
 *	@brief	Singleton that handles the loading of the sprite frames.
 *	It saves in a cache the sprite frames.
 *	@since	v0.9
 */
class CCSpriteFrameCache : public CCObject
{
	protected :		
				 CCSpriteFrameCache ( KDvoid );
		virtual ~CCSpriteFrameCache ( KDvoid );

		virtual KDbool			init ( KDvoid );

	public :

		/** Returns the shared instance of the Sprite Frame cache */
		static CCSpriteFrameCache*		sharedSpriteFrameCache ( KDvoid );

		/** Purges the cache. It releases all the Sprite Frames and the retained instance. */
		static KDvoid			purgeSharedSpriteFrameCache ( KDvoid );

	private :

		/*	Adds multiple Sprite Frames with a dictionary. The texture will be associated with the created sprite frames. */
		KDvoid					addSpriteFramesWithDictionary ( CCDictionary* pDictionary, CCTexture2D* pTexture );

	public :

		/** 
		 *	Adds multiple Sprite Frames from a plist file.
		 *	A texture will be loaded automatically. The texture name will composed by replacing the .plist suffix with .png
		 *	If you want to use another texture, you should use the addSpriteFramesWithFile:texture method.
		 */
		KDvoid					addSpriteFramesWithFile ( const KDchar* szPlist );

		/** 
		 *	Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames.
		 *	@since	v0.99.5
		 */
		KDvoid					addSpriteFramesWithFile ( const KDchar* szPlist, const KDchar* szTextureName );

		/** Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames. */
		KDvoid					addSpriteFramesWithFile ( const KDchar* szPlist, CCTexture2D* pTexture );

		/**
		 *	Adds an sprite frame with a given name.
		 *	If the name already exists, then the contents of the old name will be replaced with the new one.
		 */
		KDvoid					addSpriteFrame ( CCSpriteFrame* pFrame, const KDchar* szFrameName );

		/** 
		 *	Purges the dictionary of loaded sprite frames.
		 *	Call this method if you receive the "Memory Warning".
		 *	In the short term: it will free some resources preventing your app from being killed.
		 *	In the medium term: it will allocate more resources.
		 *	In the long term: it will be the same.
		 */
		KDvoid					removeSpriteFrames ( KDvoid );

		/** 
		 *	Removes unused sprite frames.
		 *	Sprite Frames that have a retain count of 1 will be deleted.
		 *	It is convenient to call this method after when starting a new Scene.
		 */
		KDvoid					removeUnusedSpriteFrames ( KDvoid );

		/** Deletes an sprite frame from the sprite frame cache. */
		KDvoid					removeSpriteFrameByName ( const KDchar* szName );

		/** 
		 *	Removes multiple Sprite Frames from a plist file.
		 *	Sprite Frames stored in this file will be removed.
		 *	It is convenient to call this method when a specific texture needs to be removed.
		 *	@since	v0.99.5
		 */
		KDvoid					removeSpriteFramesFromFile ( const KDchar* szPlist );

	private :

		/**
		 *	Removes multiple Sprite Frames from CCDictionary.
		 *	@since	v0.99.5
		 */
		KDvoid					removeSpriteFramesFromDictionary ( CCDictionary* pDictionary );

	public :

		/**
		 *	Removes all Sprite Frames associated with the specified textures.
		 *	It is convenient to call this method when a specific texture needs to be removed.
		 *	@since	v0.995.
		 */
		KDvoid					removeSpriteFramesFromTexture( CCTexture2D* pTexture );

		/** 
		 *	Returns an Sprite Frame that was previously added.
		 *	If the name is not found it will return nil.
		 *	You should retain the returned copy if you are going to use it.
		 */
		CCSpriteFrame*			spriteFrameByName ( const KDchar* szName );
			
	protected :

		CCDictionary*			m_pSpriteFrames;
		CCDictionary*			m_pSpriteFramesAliases;
		std::set<std::string>*  m_pLoadedFileNames;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CCSpriteFrameCache_h__
