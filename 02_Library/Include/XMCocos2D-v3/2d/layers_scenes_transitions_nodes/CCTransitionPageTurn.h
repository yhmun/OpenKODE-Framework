/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionPageTurn.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Sindesso Pty Ltd
 *
 *         http://www.cocos2d-x.org      
 *         http://www.sindesso.com 
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

#ifndef __CCTransitionPageTurn_h__
#define __CCTransitionPageTurn_h__

#include "CCTransition.h"

NS_CC_BEGIN

/**
 *	@addtogroup transition
 *	@{
 */

/**
 *	@brief A transition which peels back the bottom right hand corner of a scene
 *	to transition to the scene beneath it simulating a page turn.
 *
 *	This uses a 3DAction so it's strongly recommended that depth buffering
 *	is turned on in Director using:
 *
 *	Director::getInstance()->setDepthBufferFormat(kDepthBuffer16);
 *
 *	@since v0.8.2
 */
class CC_DLL TransitionPageTurn : public TransitionScene
{
	public :

		/**
		 *	Creates a base transition with duration and incoming scene.
		 *	If back is true then the effect is reversed to appear as if the incoming
		 *	scene is being turned from left over the outgoing scene.
		 */
		static TransitionPageTurn*	create ( KDfloat t, Scene* pScene, KDbool bBackwards );

		/**
		 *	@js ctor
		 */
		TransitionPageTurn ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionPageTurn ( KDvoid );
    
		//
		// Overrides
		//
		virtual KDvoid				draw ( KDvoid ) override;

		/**
		 *	Creates a base transition with duration and incoming scene.
		 *	If back is true then the effect is reversed to appear as if the incoming 
		 *	scene is being turned from left over the outgoing scene.
		 */
		KDbool						initWithDuration ( KDfloat t, Scene* pScene, KDbool bBackwards );

		ActionInterval*				actionWithSize ( const Size& tVector );

		//
		// Overrides
		//
		virtual KDvoid				onEnter ( KDvoid ) override;

	protected :

		virtual KDvoid				sceneOrder ( KDvoid ) override;

	protected :

		KDbool						m_bBack;    
		static KDfloat				POLYGON_OFFSET_FACTOR;
		static KDfloat				POLYGON_OFFSET_UNITS;
};

//	end of transition group
/// @}

NS_CC_END

#endif	// __CCTransitionPageTurn_h__

