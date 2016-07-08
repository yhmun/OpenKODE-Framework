/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransition.h
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

#ifndef __CCTransition_h__
#define __CCTransition_h__

#include "CCScene.h"

NS_CC_BEGIN

/**
 * @addtogroup transition
 * @{
 */

class CCActionInterval;
class CCNode;

/** 
 *	@brief	CCTransitionEaseScene can ease the actions of the scene protocol.
 *	@since	v0.8.2
 */
class CCTransitionEaseScene
{
	public :

		/** 
		 *	returns the Ease action that will be performed on a linear action.
		 *	@since	v0.8.2
		 */
		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction ) = 0;
};

/** 
 *	Orientation Type used by some transitions
 */
typedef enum 
{
	/// An horizontal orientation where the Left is nearer
	kCCTransitionOrientationLeftOver  = 0,

	/// An horizontal orientation where the Right is nearer
	kCCTransitionOrientationRightOver = 1,

	/// A vertical orientation where the Up is nearer
	kCCTransitionOrientationUpOver    = 0,

	/// A vertical orientation where the Bottom is nearer
	kCCTransitionOrientationDownOver  = 1,
} tOrientation;

/**
 *	@brief	Base class for CCTransition scenes
 */
class CCTransitionScene : public CCScene
{
	public :
		         CCTransitionScene ( KDvoid );
		virtual ~CCTransitionScene ( KDvoid );

		/** creates a base transition with duration and incoming scene */
		static   CCTransitionScene*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes a transition with duration and incoming scene */
		virtual KDbool		initWithDuration ( KDfloat fDuration, CCScene* pScene );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		onEnter ( KDvoid );
		virtual KDvoid		onExit  ( KDvoid );

		virtual KDvoid		cleanup ( KDvoid );

		/** called after the transition finishes */
		KDvoid				finish ( KDvoid );

		/** used by some transitions to hide the outer scene */
		KDvoid				hideOutShowIn ( KDvoid );

	protected :

		virtual KDvoid		sceneOrder ( KDvoid );

	private :

		KDvoid				setNewScene ( KDfloat fDelta );

	protected :

		CCScene*			m_pInScene;
		CCScene*			m_pOutScene;
		KDfloat				m_fDuration;
		KDbool				m_bIsInSceneOnTop;
		KDbool				m_bIsSendCleanupToScene;
};

/** 
 *	@brief	A CCTransition that supports orientation like.
 *	Possible orientation: LeftOver, RightOver, UpOver, DownOver
 */
class CCTransitionSceneOriented : public CCTransitionScene
{
	public :

		/** creates a base transition with duration and incoming scene */
		static   CCTransitionSceneOriented*		create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );

	public :

		/** initializes a transition with duration and incoming scene */
		virtual KDbool				initWithDuration ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );

	protected :

		tOrientation				m_eOrientation;
};

/**
 *	@brief	CCTransitionRotoZoom:
 *	Rotate and zoom out the outgoing scene, and then rotate and zoom in the incoming 
 */
class CCTransitionRotoZoom : public CCTransitionScene
{
	public :

		static CCTransitionRotoZoom*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );
};

/**
 *	@brief	CCTransitionJumpZoom:
 *	Zoom out and jump the outgoing scene, and then jump and zoom in the incoming 
 */
class CCTransitionJumpZoom : public CCTransitionScene
{
	public :
		
		static CCTransitionJumpZoom*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );
};

/** 
 *	@brief	CCTransitionMoveInL:
 *	Move in from to the left the incoming scene.
 */
class CCTransitionMoveInL : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionMoveInL*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed */
		virtual CCActionInterval*	action ( KDvoid );

		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );

		virtual KDvoid				onEnter ( KDvoid );

};

/**
 *	@brief	CCTransitionMoveInR:
 *	Move in from to the right the incoming scene.
 */
class CCTransitionMoveInR : public CCTransitionMoveInL
{
	public :

		static CCTransitionMoveInR*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				initScenes ( KDvoid );
};

/** 
 *	@brief	CCTransitionMoveInT:
 *	Move in from to the top the incoming scene.
 */
class CCTransitionMoveInT : public CCTransitionMoveInL 
{
	public :

		static CCTransitionMoveInT*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				initScenes ( KDvoid );
};

/**
 *	@brief	CCTransitionMoveInB:
 *	Move in from to the bottom the incoming scene.
 */
class CCTransitionMoveInB : public CCTransitionMoveInL
{
	public :

		static CCTransitionMoveInB*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				initScenes ( KDvoid );
};

/** 
 *	@brief	CCTransitionSlideInL:
 *	Slide in the incoming scene from the left border.
 */
class CCTransitionSlideInL : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionSlideInL*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual CCActionInterval*	action ( KDvoid );

		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );

		virtual KDvoid				onEnter ( KDvoid );		

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );
};

/**
 *	@brief	CCTransitionSlideInR:
 *	Slide in the incoming scene from the right border.
 */
class CCTransitionSlideInR : public CCTransitionSlideInL 
{
	public :

		static CCTransitionSlideInR*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual CCActionInterval*	action ( KDvoid );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );
};

/** 
 *	@brief	CCTransitionSlideInB:
 *	Slide in the incoming scene from the bottom border.
 */
class CCTransitionSlideInB : public CCTransitionSlideInL
{
	public :

		static CCTransitionSlideInB*  create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual CCActionInterval*	action ( KDvoid );

	protected : 

		virtual KDvoid				sceneOrder ( KDvoid );
};

/** 
 *	@brief	CCTransitionSlideInT:
 *	Slide in the incoming scene from the top border.
 */
class CCTransitionSlideInT : public CCTransitionSlideInL
{
	public :

		static CCTransitionSlideInT*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual CCActionInterval*	action ( KDvoid );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );
};

/**
 *	@brief	Shrink the outgoing scene while grow the incoming scene
 */
class CCTransitionShrinkGrow : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionShrinkGrow*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );

		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );
};

/** 
 *	@brief	CCTransitionFlipX:
 *	Flips the screen horizontally.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionFlipX : public CCTransitionSceneOriented
{
	public :
		
		static  CCTransitionFlipX*		create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionFlipX*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );		
};

/**
 *	@brief	CCTransitionFlipY:
 *	Flips the screen vertically.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionFlipY : public CCTransitionSceneOriented
{
	public :

		static  CCTransitionFlipY*		create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionFlipY*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );		
};

/** 
 *	@brief	CCTransitionFlipAngular:
 *	Flips the screen half horizontally and half vertically.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionFlipAngular : public CCTransitionSceneOriented
{
	public :

		static  CCTransitionFlipAngular*	create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionFlipAngular*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );
};

/** 
 *	@brief	CCTransitionZoomFlipX:
 *	Flips the screen horizontally doing a zoom out/in
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionZoomFlipX : public CCTransitionSceneOriented
{
	public :

		static  CCTransitionZoomFlipX*		create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionZoomFlipX*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );
};

/** 
 *	@brief	CCTransitionZoomFlipY:
 *	Flips the screen vertically doing a little zooming out/in
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionZoomFlipY : public CCTransitionSceneOriented
{
	public :

		static  CCTransitionZoomFlipY*		create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionZoomFlipY*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );		
};

/** 
 *	@brief	CCTransitionZoomFlipAngular:
 *	Flips the screen half horizontally and half vertically doing a little zooming out/in.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CCTransitionZoomFlipAngular : public CCTransitionSceneOriented
{
	public :

		static  CCTransitionZoomFlipAngular*	create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation );
		static  CCTransitionZoomFlipAngular*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );
};

/** 
 *	@brief	CCTransitionFade:
 *	Fade out the outgoing scene and then fade in the incoming scene.'''
 */
class CCTransitionFade : public CCTransitionScene
{
	public:

		/**
		 *	creates the transition with a duration and with an RGB color
		 *	Example: FadeTransition::transitionWithDuration(2, scene, ccc3(255,0,0); // red color
		 */
		static  CCTransitionFade*		create ( KDfloat fDuration, CCScene* pScene, const ccColor3B& tColor );
		static  CCTransitionFade*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		/** initializes the transition with a duration and with an RGB color */
		virtual KDbool				initWithDuration ( KDfloat fDuration, CCScene* pScene,const ccColor3B& tColor );
		virtual KDbool				initWithDuration ( KDfloat fDuration, CCScene* pScene );

		virtual KDvoid				onEnter ( KDvoid );
		virtual KDvoid				onExit ( KDvoid );

	protected :

		ccColor4B					m_tColor;
};

class CCRenderTexture;

/**
 *	@brief	CCTransitionCrossFade:
 *	Cross fades two scenes using the CCRenderTexture object.
 */
class CCTransitionCrossFade : public CCTransitionScene
{
	public :

		static CCTransitionCrossFade*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				onEnter ( KDvoid );
		virtual KDvoid				onExit  ( KDvoid );
};

/** 
 *	@brief	CCTransitionTurnOffTiles:
 *	Turn off the tiles of the outgoing scene in random order
 */
class CCTransitionTurnOffTiles : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionTurnOffTiles*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );

		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );
};

/** 
 *	@brief	CCTransitionSplitCols:
 *	The odd columns goes upwards while the even columns goes downwards.
 */
class CCTransitionSplitCols : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionSplitCols*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );

		virtual CCActionInterval*	action ( KDvoid );
		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );
};

/** 
 *	@brief	CCTransitionSplitRows:
 *	The odd rows goes to the left while the even rows goes to the right.
 */
class CCTransitionSplitRows : public CCTransitionSplitCols
{
	public :

		static CCTransitionSplitRows*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual CCActionInterval*	action ( KDvoid );
};

/**
 *	@brief	CCTransitionFadeTR:
 *	Fade the tiles of the outgoing scene from the left-bottom corner the to top-right corner.
 */
class CCTransitionFadeTR : public CCTransitionScene, public CCTransitionEaseScene
{
	public :

		static CCTransitionFadeTR*		create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual KDvoid				onEnter ( KDvoid );

		virtual CCActionInterval*	actionWithSize ( const CCSize& tSize );
		virtual CCActionInterval*	easeActionWithAction ( CCActionInterval* pAction );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );
};

/**
 *	@brief	CCTransitionFadeBL:
 *	Fade the tiles of the outgoing scene from the top-right corner to the bottom-left corner.
 */
class CCTransitionFadeBL : public CCTransitionFadeTR
{
	public :

		static CCTransitionFadeBL*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual CCActionInterval*	actionWithSize ( const CCSize& tSize );
};

/** 
 *	@brief	CCTransitionFadeUp:
 *	Fade the tiles of the outgoing scene from the bottom to the top.
 */
class CCTransitionFadeUp : public CCTransitionFadeTR
{
	public :

		static CCTransitionFadeUp*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual CCActionInterval*	actionWithSize ( const CCSize& tSize );
};

/**
 *	@brief	CCTransitionFadeDown:
 *	Fade the tiles of the outgoing scene from the top to the bottom.
 */
class CCTransitionFadeDown : public CCTransitionFadeTR
{
	public :

		static CCTransitionFadeDown*	create ( KDfloat fDuration, CCScene* pScene );

	public :

		virtual CCActionInterval*	actionWithSize ( const CCSize& tSize );
};

// end of transition group
/// @}

NS_CC_END

#endif // __CCTransition_h__

