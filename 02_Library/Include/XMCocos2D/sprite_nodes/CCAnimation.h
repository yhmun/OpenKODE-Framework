/* -----------------------------------------------------------------------------------
 *
 *      File            CCAnimation.h
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

#ifndef __CCAnimation_h__
#define __CCAnimation_h__

#include "../cocoa/CCDictionary.h"
#include "CCSpriteFrame.h"

NS_CC_BEGIN
   
class CCTexture2D;
class CCSpriteFrame;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** CCAnimationFrame
 *	A frame of the animation. It contains information like:
 *	   - sprite frame name
 *	   - # of delay units.
 *	   - offset
 * 
 *	@since v2.0
 */
class CCAnimationFrame : public CCObject
{
	public :
				 CCAnimationFrame ( KDvoid );
		virtual ~CCAnimationFrame ( KDvoid );

	public :

		/** initializes the animation frame with a spriteframe, number of delay units and a notification user info */
		virtual KDbool			initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, KDfloat fDelayUnits, CCDictionary* pUserInfo );

		virtual CCObject*		copyWithZone ( CCZone* pZone );
	    
		/** CCSpriteFrameName to be used */
		CC_SYNTHESIZE_RETAIN ( CCSpriteFrame*, m_pSpriteFrame, SpriteFrame );

		/**  how many units of time the frame takes */
		CC_SYNTHESIZE ( KDfloat, m_fDelayUnits, DelayUnits );

		/**  A CCAnimationFrameDisplayedNotification notification will be broadcast when the frame is displayed with this dictionary as UserInfo. If UserInfo is nil, then no notification will be broadcasted. */
		CC_SYNTHESIZE_RETAIN ( CCDictionary*, m_pUserInfo, UserInfo );
};

/** 
 *	A CCAnimation object is used to perform animations on the CCSprite objects.
 *
 *	The CCAnimation object contains CCAnimationFrame objects, and a possible delay between the frames.
 *	You can animate a CCAnimation object by using the CCAnimate action. Example:
 *
 *	[sprite runAction:[CCAnimate actionWithAnimation:animation]];
 *
 */
class CCAnimation : public CCObject
{
	public :
		         CCAnimation ( KDvoid );
		virtual ~CCAnimation ( KDvoid );

        /** 
		 *	Creates an animation
		 *	@since v0.99.5
		 */
		static CCAnimation*		create ( KDvoid );

		static CCAnimation*		create ( KDfloat fDelay );

		/** 
		 *	Creates an animation with an array of CCSpriteFrame and a delay between frames in seconds.
		 *	The frames will be added with one "delay unit".
		 *	@since v0.99.5
		 */
		static CCAnimation*		createWithSpriteFrames ( CCArray* pArrayOfSpriteFrameNames, KDfloat fDelay = 0.0f );

		/**
		 *	Creates an animation with an array of CCAnimationFrame, the delay per units in seconds and and how many times it should be executed.
		 *	@since v2.0
		 */
		static CCAnimation*		create ( CCArray* pArrayOfAnimationFrameNames, KDfloat fDelayPerUnit, KDuint uLoops = 1 );

	public :

		virtual KDbool			init ( KDvoid );

		/** Initializes a CCAnimation with frames and a delay between frames
		 *	@since v0.99.5
		 */
		virtual KDbool			initWithSpriteFrames ( CCArray* pFrames, KDfloat fDelay = 0.0f );

		/** Initializes a CCAnimation with CCAnimationFrame
		 *	@since v2.0
		 */
		virtual KDbool			initWithAnimationFrames ( CCArray* pArrayOfSpriteFrameNames, KDfloat fDelayPerUnit, KDuint uLoops );

		/** Adds a CCSpriteFrame to a CCAnimation.
		 *	The frame will be added with one "delay unit".
		 */
		KDvoid					addSpriteFrame ( CCSpriteFrame* pFrame );

		/** Adds a frame with an image filename. Internally it will create a CCSpriteFrame and it will add it.
		 *	The frame will be added with one "delay unit".
		 *	Added to facilitate the migration from v0.8 to v0.9.
		 */  
		KDvoid					addSpriteFrameWithFileName ( const KDchar* szFileName );

		/** Adds a frame with a texture and a rect. Internally it will create a CCSpriteFrame and it will add it.
		 *	The frame will be added with one "delay unit".
		 *	Added to facilitate the migration from v0.8 to v0.9.
		 */
		KDvoid					addSpriteFrameWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		/** total Delay units of the CCAnimation. */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fTotalDelayUnits, TotalDelayUnits );

		/** Delay in seconds of the "delay unit" */
		CC_SYNTHESIZE ( KDfloat, m_fDelayPerUnit, DelayPerUnit );

		/** duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit */
		CC_PROPERTY_READONLY ( KDfloat, m_fDuration, Duration );

		/** array of CCAnimationFrames */
		CC_SYNTHESIZE_RETAIN ( CCArray*, m_pFrames, Frames );

		/** whether or not it shall restore the original frame when the animation finishes */
		CC_SYNTHESIZE ( KDbool, m_bRestoreOriginalFrame, RestoreOriginalFrame );

		/** how many times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... */
		CC_SYNTHESIZE ( KDuint, m_uLoops, Loops );
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CCAnimation_h__
