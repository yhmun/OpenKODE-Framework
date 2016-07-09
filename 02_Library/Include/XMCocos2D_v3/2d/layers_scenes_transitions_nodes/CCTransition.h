/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransition.h
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

#ifndef __CCTransition_h__
#define __CCTransition_h__

#include "CCScene.h"
#include "ccTypes.h"

NS_CC_BEGIN

/**
 *	@addtogroup transition
 *	@{
 */

// static creation function macro
// c/c++ don't support object creation of using class name
// so, all classes need creation method.

class ActionInterval;
class Node;

/** 
 *	@brief TransitionEaseScene can ease the actions of the scene protocol.
 *	@since v0.8.2
*/
class CC_DLL TransitionEaseScene// : public Object
{
	public :

		virtual ~TransitionEaseScene ( KDvoid ) { }

		/** 
		 *	returns the Ease action that will be performed on a linear action.
		 *	@since v0.8.2
		 */
		virtual ActionInterval*		easeActionWithAction ( ActionInterval* pAction ) = 0;
};

/** 
 *	@brief Base class for Transition scenes
 */
class CC_DLL TransitionScene : public Scene
{
	public :

		/**
		 *	Orientation Type used by some transitions
		 */
		enum class Orientation
		{
			/// An horizontal orientation where the Left is nearer
			LEFT_OVER = 0,
			/// An horizontal orientation where the Right is nearer
			RIGHT_OVER = 1,
			/// A vertical orientation where the Up is nearer
			UP_OVER = 0,
			/// A vertical orientation where the Bottom is nearer
			DOWN_OVER = 1,
		};
    
		/** creates a base transition with duration and incoming scene */
		static TransitionScene*		create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionScene ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionScene ( KDvoid );

		/** initializes a transition with duration and incoming scene */
		KDbool						initWithDuration ( KDfloat t, Scene* pScene );

		/** called after the transition finishes */
		KDvoid						finish ( KDvoid );

		/** used by some transitions to hide the outer scene */
		KDvoid						hideOutShowIn ( KDvoid );

		//
		// Overrides
		//
		virtual KDvoid				draw ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onExit ( KDvoid ) override;
		virtual KDvoid				cleanup ( KDvoid ) override;

	protected :

		virtual KDvoid				sceneOrder ( KDvoid );

	private :

		KDvoid						setNewScene ( KDfloat dt );

	protected :

		Scene*						m_pInScene;
		Scene*						m_pOutScene;
		KDfloat						m_fDuration;
		KDbool						m_bIsInSceneOnTop;
		KDbool						m_bIsSendCleanupToScene;
};

/** 
 *	@brief A Transition that supports orientation like.
 *	Possible orientation: LeftOver, RightOver, UpOver, DownOver
 */
class CC_DLL TransitionSceneOriented : public TransitionScene
{
	public :

		/** creates a base transition with duration and incoming scene */
		static TransitionSceneOriented*		create ( KDfloat t, Scene* pScene, Orientation eOrientation );
		
		/**
		 *	@js ctor
		 */
		TransitionSceneOriented ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSceneOriented ( KDvoid );

		/** initializes a transition with duration and incoming scene */
		KDbool						initWithDuration ( KDfloat t, Scene* pScene, Orientation eOrientation );

	protected :

		Orientation					m_eOrientation;
};

/** 
 *	@brief TransitionRotoZoom:
 *	Rotate and zoom out the outgoing scene, and then rotate and zoom in the incoming 
 */
class CC_DLL TransitionRotoZoom : public TransitionScene
{
	public :
		static TransitionRotoZoom*	create ( KDfloat t, Scene* pScene );

		TransitionRotoZoom ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionRotoZoom ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/** @brief TransitionJumpZoom:
Zoom out and jump the outgoing scene, and then jump and zoom in the incoming 
*/
class CC_DLL TransitionJumpZoom : public TransitionScene
{
	public :

		static TransitionJumpZoom*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionJumpZoom ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionJumpZoom ( KDvoid );

		//
		// Overrides
		//
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/** 
 *	@brief TransitionMoveInL:
 *	Move in from to the left the incoming scene.
 */
class CC_DLL TransitionMoveInL : public TransitionScene, public TransitionEaseScene
{
	public :

		static TransitionMoveInL*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionMoveInL ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionMoveInL ( KDvoid );

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed */
		virtual ActionInterval*		action ( KDvoid );

		virtual ActionInterval*		easeActionWithAction ( ActionInterval* pAction );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/**
 *	@brief TransitionMoveInR:
 *	Move in from to the right the incoming scene.
 */
class CC_DLL TransitionMoveInR : public TransitionMoveInL
{
	public :

		static TransitionMoveInR*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionMoveInR ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionMoveInR ( KDvoid );

		virtual KDvoid				initScenes ( KDvoid );
};

/** 
 *	@brief TransitionMoveInT:
 *	Move in from to the top the incoming scene.
 */
class CC_DLL TransitionMoveInT : public TransitionMoveInL 
{
	public :

		static TransitionMoveInT*	create ( KDfloat t, Scene* pScene );

		/**
		 * @js ctor
		 */
		TransitionMoveInT ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionMoveInT ( KDvoid );

		virtual KDvoid				initScenes ( KDvoid );
};

/** 
 *	@brief TransitionMoveInB:
 *	Move in from to the bottom the incoming scene.
 */
class CC_DLL TransitionMoveInB : public TransitionMoveInL
{
	public :

		static TransitionMoveInB*	create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionMoveInB ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionMoveInB ( KDvoid );

		virtual KDvoid				initScenes ( KDvoid );
};

/** 
 *	@brief TransitionSlideInL:
 *	Slide in the incoming scene from the left border.
 */
class CC_DLL TransitionSlideInL : public TransitionScene, public TransitionEaseScene
{
	public :

		static TransitionSlideInL*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionSlideInL ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSlideInL ( KDvoid );

		virtual ActionInterval*		easeActionWithAction ( ActionInterval* pAction );

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual ActionInterval*		action ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
    
	protected :

		virtual KDvoid				sceneOrder ( KDvoid ) override;
};

/**
 *	@brief TransitionSlideInR:
 *	Slide in the incoming scene from the right border.
 */
class CC_DLL TransitionSlideInR : public TransitionSlideInL 
{
	public :

		static TransitionSlideInR*	create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionSlideInR ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSlideInR ( KDvoid );

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual ActionInterval*		action ( KDvoid );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid ) override;
};

/**
 *	@brief TransitionSlideInB:
 *	Slide in the incoming scene from the bottom border.
 */
class CC_DLL TransitionSlideInB : public TransitionSlideInL
{
	public :

		static TransitionSlideInB*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionSlideInB ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSlideInB ( KDvoid );

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );

		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual ActionInterval*		action ( KDvoid );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid ) override;
};

/** 
 *	@brief TransitionSlideInT:
 *	Slide in the incoming scene from the top border.
 */
class CC_DLL TransitionSlideInT : public TransitionSlideInL
{
	public :

		static TransitionSlideInT*	create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionSlideInT ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSlideInT ( KDvoid );

		/** initializes the scenes */
		virtual KDvoid				initScenes ( KDvoid );
		
		/** returns the action that will be performed by the incoming and outgoing scene */
		virtual ActionInterval*		action ( KDvoid );

	protected :

		virtual KDvoid				sceneOrder ( KDvoid ) override;
};

/**
 *	@brief Shrink the outgoing scene while grow the incoming scene
 */
class CC_DLL TransitionShrinkGrow : public TransitionScene , public TransitionEaseScene
{
	public :
		static TransitionShrinkGrow*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionShrinkGrow ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionShrinkGrow ( KDvoid );

		//
		// Overrides
		//
		virtual KDvoid				onEnter ( KDvoid ) override;
		virtual ActionInterval*		easeActionWithAction ( ActionInterval* pAction ) override;
};

/** 
 *	@brief TransitionFlipX:
 *	Flips the screen horizontally.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionFlipX : public TransitionSceneOriented
{
	public :

		static TransitionFlipX*		create ( KDfloat t, Scene* s, Orientation o );
		static TransitionFlipX*		create ( KDfloat t, Scene* s );

		/**
		 *	@js ctor
		 */
		TransitionFlipX ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFlipX ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/** 
 *	@brief TransitionFlipY:
 *	Flips the screen vertically.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionFlipY : public TransitionSceneOriented
{
	public :

		static TransitionFlipY*		create ( KDfloat t, Scene* s, Orientation o );
		static TransitionFlipY*		create ( KDfloat t, Scene* s );

		/**
		 *	@js ctor
		 */
		TransitionFlipY ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFlipY ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/**
 *	@brief TransitionFlipAngular:
 *	Flips the screen half horizontally and half vertically.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionFlipAngular : public TransitionSceneOriented
{
	public :

		static TransitionFlipAngular*	create ( KDfloat t, Scene* s, Orientation o );
		static TransitionFlipAngular*	create ( KDfloat t, Scene* s );
		
		/**
		 *	@js ctor
		 */
		TransitionFlipAngular ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFlipAngular ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
};

/** 
 *	@brief TransitionZoomFlipX:
 *	Flips the screen horizontally doing a zoom out/in
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionZoomFlipX : public TransitionSceneOriented
{
	public :

		static TransitionZoomFlipX*		create ( KDfloat t, Scene* s, Orientation o );
		static TransitionZoomFlipX*		create ( KDfloat t, Scene* s );
		
		/**
		 *	@js ctor
		 */
		TransitionZoomFlipX ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionZoomFlipX ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
};

/**
 *	@brief TransitionZoomFlipY:
 *	Flips the screen vertically doing a little zooming out/in
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionZoomFlipY : public TransitionSceneOriented
{
	public :

		static TransitionZoomFlipY*		create ( KDfloat t, Scene* s, Orientation o );
		static TransitionZoomFlipY*		create ( KDfloat t, Scene* s );
		
		/**
		 *	@js ctor
		 */
		TransitionZoomFlipY ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionZoomFlipY ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
};

/**
 *	@brief TransitionZoomFlipAngular:
 *	Flips the screen half horizontally and half vertically doing a little zooming out/in.
 *	The front face is the outgoing scene and the back face is the incoming scene.
 */
class CC_DLL TransitionZoomFlipAngular : public TransitionSceneOriented
{
	public :

		static TransitionZoomFlipAngular*	create ( KDfloat t, Scene* s, Orientation o );
		static TransitionZoomFlipAngular*	create ( KDfloat t, Scene* s );
		
		/**
		 *	@js ctor
		 */
		TransitionZoomFlipAngular ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionZoomFlipAngular ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid ) override;
};

/** 
 *	@brief TransitionFade:
 *	Fade out the outgoing scene and then fade in the incoming scene.'''
 */
class CC_DLL TransitionFade : public TransitionScene
{
	public :

		/** 
		 *	creates the transition with a duration and with an RGB color
		 *	Example: FadeTransition::create(2, scene, Color3B(255,0,0); // red color
		 */
		static TransitionFade*		create ( KDfloat fDuration, Scene* pScene, const Color3B& tColor );
		static TransitionFade*		create ( KDfloat fDuration, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionFade ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFade ( KDvoid );

		/** initializes the transition with a duration and with an RGB color */
		KDbool						initWithDuration ( KDfloat t, Scene*scene ,const Color3B& tColor );

		//
		// Overrides
		//
		KDbool						initWithDuration ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onEnter ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				onExit ( KDvoid );

	protected :

		Color4B						m_tColor;
};

class RenderTexture;

/**
 *	@brief TransitionCrossFade:
 *	Cross fades two scenes using the RenderTexture object.
 */
class CC_DLL TransitionCrossFade : public TransitionScene
{
	public :

		static TransitionCrossFade*		create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionCrossFade ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionCrossFade ( KDvoid );

		//
		// Overrides
		//
		virtual KDvoid					draw ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onExit ( KDvoid ) override;
};

/**
 *	@brief TransitionTurnOffTiles:
 *	Turn off the tiles of the outgoing scene in random order
 */
class CC_DLL TransitionTurnOffTiles : public TransitionScene ,public TransitionEaseScene
{
	public :

		static TransitionTurnOffTiles*	create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionTurnOffTiles ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionTurnOffTiles ( KDvoid );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
		virtual ActionInterval*			easeActionWithAction ( ActionInterval* pAction ) override;

	protected :

		virtual KDvoid					sceneOrder ( KDvoid ) override;
};

/** 
 *	@brief TransitionSplitCols:
 *	The odd columns goes upwards while the even columns goes downwards.
 */
class CC_DLL TransitionSplitCols : public TransitionScene , public TransitionEaseScene
{
	public :

		static TransitionSplitCols*		create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionSplitCols ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSplitCols ( KDvoid );

		virtual ActionInterval*			action ( KDvoid );

		//
		// Overrides
		//
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
		virtual ActionInterval*			easeActionWithAction ( ActionInterval* pAction ) override;
};

/** 
 *	@brief TransitionSplitRows:
 *	The odd rows goes to the left while the even rows goes to the right.
 */
class CC_DLL TransitionSplitRows : public TransitionSplitCols
{
	public :

		static TransitionSplitRows*		create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionSplitRows ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionSplitRows ( KDvoid );

		//
		// Overrides
		//
		virtual ActionInterval*			action ( KDvoid ) override;
};

/**
 *	@brief TransitionFadeTR:
 *	Fade the tiles of the outgoing scene from the left-bottom corner the to top-right corner.
 */
class CC_DLL TransitionFadeTR : public TransitionScene , public TransitionEaseScene
{
	public :

		static TransitionFadeTR*		create ( KDfloat t, Scene* pScene );

		/**
		 *	@js ctor
		 */
		TransitionFadeTR ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFadeTR ( KDvoid );

		virtual ActionInterval*			actionWithSize ( const Size& tSize );

		//
		// Overrides
		//

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid ) override;
		virtual ActionInterval*			easeActionWithAction ( ActionInterval* pAction ) override;

	protected :

		virtual KDvoid					sceneOrder ( KDvoid );
};

/** 
 *	@brief TransitionFadeBL:
 *	Fade the tiles of the outgoing scene from the top-right corner to the bottom-left corner.
 */
class CC_DLL TransitionFadeBL : public TransitionFadeTR
{
	public :

		static TransitionFadeBL*		create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionFadeBL ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFadeBL ( KDvoid );

		//
		// Overrides
		//
		virtual ActionInterval*			actionWithSize ( const Size& tSize ) override;
};

/**
 *	@brief TransitionFadeUp:
 *	Fade the tiles of the outgoing scene from the bottom to the top.
 */
class CC_DLL TransitionFadeUp : public TransitionFadeTR
{
	public :

		static TransitionFadeUp*		create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionFadeUp ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFadeUp ( KDvoid );

		//
		// Overrides
		//
		virtual ActionInterval*			actionWithSize ( const Size& tSize ) override;
};

/** 
 *	@brief TransitionFadeDown:
 *	Fade the tiles of the outgoing scene from the top to the bottom.
 */
class CC_DLL TransitionFadeDown : public TransitionFadeTR
{
	public :

		static TransitionFadeDown*		create ( KDfloat t, Scene* pScene );
		
		/**
		 *	@js ctor
		 */
		TransitionFadeDown ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TransitionFadeDown ( KDvoid );

		//
		// Overrides
		//
		virtual ActionInterval*			actionWithSize ( const Size& tSize ) override;
};

//	end of transition group
/// @}

NS_CC_END

#endif	// __CCTransition_h__

