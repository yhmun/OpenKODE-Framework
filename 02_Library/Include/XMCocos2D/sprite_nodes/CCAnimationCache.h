/* -----------------------------------------------------------------------------------
 *
 *      File            CCAnimationCache.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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

#ifndef __CCAnimationCache_h__
#define __CCAnimationCache_h__

#include "../cocoa/CCDictionary.h"

NS_CC_BEGIN
    
class CCAnimation;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** 
 *	Singleton that manages the Animations.
 *	It saves in a cache the animations. You should use this class if you want to save your animations in a cache.
 *
 *	Before v0.99.5, the recommend way was to save them on the CCSprite. Since v0.99.5, you should use this class instead.
 *
 *	@since v0.99.5
 */
class CCAnimationCache : public CCObject
{
	protected :		
		         CCAnimationCache ( KDvoid );
		virtual ~CCAnimationCache ( KDvoid );

		virtual  KDbool		init ( KDvoid );

	public :

		/** Returns the shared instance of the Animation cache */
		static  CCAnimationCache*	sharedAnimationCache ( KDvoid );

		/** Purges the cache. It releases all the CCAnimation objects and the shared instance. */
		static  KDvoid		purgeSharedAnimationCache ( KDvoid );

	public :

		/** Adds a CCAnimation with a name. */
		KDvoid				addAnimation ( CCAnimation* pAnimation, const KDchar* szName );

		/** Deletes a CCAnimation from the cache. */
		KDvoid				removeAnimationByName ( const KDchar* szName );

		/**
		 *	Returns a CCAnimation that was previously added.
		 *	If the name is not found it will return nil.
		 *	You should retain the returned copy if you are going to use it.
		 */
		CCAnimation*		animationByName ( const KDchar* szName );

		/** Adds an animation from an NSDictionary
		 *	Make sure that the frames were previously loaded in the CCSpriteFrameCache.
		 *	@since v1.1
		 */
		KDvoid				addAnimationsWithDictionary ( CCDictionary* pDictionary );

		/** Adds an animation from a plist file.
		 *	Make sure that the frames were previously loaded in the CCSpriteFrameCache.
		 *	@since v1.1
		 */
		KDvoid				addAnimationsWithFile ( const KDchar* szPlist );

	private :

		KDvoid				parseVersion1 ( CCDictionary* pAnimations );
		KDvoid				parseVersion2 ( CCDictionary* pAnimations );

	private :

		CCDictionary*		m_pAnimations;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CCAnimationCache_h__