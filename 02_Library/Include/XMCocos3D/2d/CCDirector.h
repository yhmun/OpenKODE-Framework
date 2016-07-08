/* -----------------------------------------------------------------------------------
 *
 *      File            CCDirector.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __CCDirector_h__
#define __CCDirector_h__

#include "../platform/CCPlatformMacros.h"
#include "../ccTypes.h"
#include "../base/CCObject.h"
#include "../base/CCGeometry.h"
#include "../base/CCArray.h"
#include "../platform/CCGL.h"
#include "../2d/label_nodes/CCLabelAtlas.h"
#include "XMKazmath/mat4.h"


NS_CC_BEGIN

/**
 *	@addtogroup base_nodes
 *	@{
 */

/* Forward declarations. */
class LabelAtlas;
class Scene;
class EGLView;
class DirectorDelegate;
class Node;
class Scheduler;
class ActionManager;
class EventDispatcher;
class TextureCache;

/**
 *	@brief Class that creates and handles the main Window and manages how
 *	and when to execute the Scenes.
 *
 *	The Director is also responsible for:
 *		- initializing the OpenGL context
 *		- setting the OpenGL pixel format (default on is RGB565)
 *		- setting the OpenGL buffer depth (default one is 0-bit)
 *		- setting the projection (default one is 3D)
 *		- setting the orientation (default one is Portrait)
 *
 *	Since the Director is a singleton, the standard way to use it is by calling:
 *		_ Director::getInstance()->methodName();
 *
 *	The Director also sets the default OpenGL context:
 *		- GL_TEXTURE_2D is enabled
 *		- GL_VERTEX_ARRAY is enabled
 *		- GL_COLOR_ARRAY is enabled
 *		- GL_TEXTURE_COORD_ARRAY is enabled
 */
class CC_DLL Director : public Object
{
	public :

		/** 
		 *	@typedef ccDirectorProjection
		 *	Possible OpenGL projections used by director
		 */
		enum class Projection
		{
			/// sets a 2D projection (orthogonal projection)
			_2D,
        
			/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
			_3D,
        
			/// it calls "updateProjection" on the projection delegate.
			CUSTOM,
        
			/// Default projection is 3D projection
			DEFAULT = _3D,
		};
    
		/** returns a shared instance of the director */
		static Director*	getInstance ( KDvoid );

		// Don't call it manually.
		KDvoid				purgeDirector ( KDvoid );

	protected :

		/**
		 * @js ctor
		 */
		Director ( KDvoid );

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~Director ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		// attribute

		/** Get current running Scene. Director can only run one Scene at a time */
		inline  Scene*		getRunningScene ( KDvoid ) { return m_pRunningScene; }

		/** Get the FPS value */
		inline  KDdouble	getAnimationInterval ( KDvoid ) { return m_dAnimationInterval; }

		/** Set the FPS value. */
		virtual KDvoid		setAnimationInterval ( KDdouble dInterval ) = 0;

		/** Whether or not to display the FPS on the bottom-left corner */
		inline  KDbool		isDisplayStats ( KDvoid ) { return m_bDisplayStats; }

		/** Display the FPS on the bottom-left corner */
		inline  KDvoid		setDisplayStats ( KDbool bDisplayStats ) { m_bDisplayStats = bDisplayStats; }
    
		/** seconds per frame */
		inline  KDfloat		getSecondsPerFrame ( KDvoid ) { return m_fSecondsPerFrame; }

		/** 
		 *	Get the EGLView, where everything is rendered
		 *	@js NA
		 *	@lua NA
		 */
		inline EGLView*		getOpenGLView ( KDvoid ) { return m_pOpenGLView; }
		KDvoid				setOpenGLView ( EGLView* pOpenGLView );

		TextureCache*		getTextureCache ( KDvoid ) const;

		inline KDbool		 isNextDeltaTimeZero ( KDvoid ) { return m_bNextDeltaTimeZero; }
		KDvoid				setNextDeltaTimeZero ( KDbool bNextDeltaTimeZero );

		/** Whether or not the Director is paused */
		inline KDbool		isPaused ( KDvoid ) { return m_bPaused; }

		/** How many frames were called since the director started */
		inline KDuint		getTotalFrames ( KDvoid ) { return m_uTotalFrames; }
    
		/**
		 *	Sets an OpenGL projection
		 *	@since v0.8.2
		 *	@js NA
		 *	@lua NA
		 */
		inline Projection	getProjection ( KDvoid ) { return m_eProjection; }
		KDvoid				setProjection ( Projection eProjection );
    
		/** Sets the glViewport*/
		KDvoid				setViewport ( KDvoid );

		/** How many frames were called since the director started */
    
		/** 
		 *	Whether or not the replaced scene will receive the cleanup message.
		 *	If the new scene is pushed, then the old scene won't receive the "cleanup" message.
		 *	If the new scene replaces the old one, the it will receive the "cleanup" message.
		 *	@since v0.99.0
		 */
		inline KDbool		isSendCleanupToScene ( KDvoid ) { return m_bSendCleanupToScene; }

		/**
		 *	This object will be visited after the main scene is visited.
		 *	This object MUST implement the "visit" selector.
		 *	Useful to hook a notification object, like Notifications (http://github.com/manucorporat/CCNotifications)
		 *	@since v0.99.5
		 */
		Node*				getNotificationNode ( KDvoid );
		KDvoid				setNotificationNode ( Node* pNode );
    
		/** 
		 *	Director delegate. It shall implement the DirectorDelegate protocol
		 *	@since v0.99.5
		 *	@js NA
		 *	@lua NA
		 */
		DirectorDelegate*	getDelegate ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				setDelegate ( DirectorDelegate* pDelegate );

		// window size

		/** 
		 *	returns the size of the OpenGL view in points.
		 */
		const Size&			getWinSize ( KDvoid ) const;

		/**
		 *	returns the size of the OpenGL view in pixels.
		 */
		Size				getWinSizeInPixels ( KDvoid ) const;
    
		/** 
		 *	returns visible size of the OpenGL view in points.
		 *  the value is equal to getWinSize if don't invoke
		 *  EGLView::setDesignResolutionSize()
		 */
		Size				getVisibleSize ( KDvoid ) const;
    
		/**
		 *	returns visible origin of the OpenGL view in points.
		 */
		Point				getVisibleOrigin ( KDvoid ) const;

		/** 
		 *	converts a UIKit coordinate to an OpenGL coordinate
		 *	Useful to convert (multi) touch coordinates to the current layout (portrait or landscape)
		 */
		Point				convertToGL ( const Point& tPoint );

		/**
		 *	converts an OpenGL coordinate to a UIKit coordinate
		 *	Useful to convert node points to window points for calls such as glScissor
		 */
		Point				convertToUI ( const Point& tPoint );

		/// XXX: missing description 
		KDfloat				getZEye ( KDvoid ) const;

		// Scene Management

		/** 
		 *	Enters the Director's main loop with the given Scene.
		 *	Call it to run only your FIRST scene.
		 *	Don't call it if there is already a running scene.
		 *
		 *	It will call pushScene: and then it will call startAnimation
		 */
		KDvoid				runWithScene ( Scene* pScene );

		/** Suspends the execution of the running scene, pushing it on the stack of suspended scenes.
		 * The new scene will be executed.
		 * Try to avoid big stacks of pushed scenes to reduce memory allocation. 
		 * ONLY call it if there is a running scene.
		 */
		KDvoid				pushScene ( Scene* pScene );

		/**
		 *	Pops out a scene from the stack.
		 *	This scene will replace the running one.
		 *	The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid				popScene ( KDvoid );

		/** 
		 *	Pops out all scenes from the stack until the root scene in the queue.
		 *	This scene will replace the running one.
		 *	Internally it will call `popToSceneStackLevel(1)`
		 */
		KDvoid				popToRootScene ( KDvoid );

		/** 
		 *	Pops out all scenes from the stack until it reaches `level`.
		 *	If level is 0, it will end the director.
		 *	If level is 1, it will pop all scenes until it reaches to root scene.
		 *	If level is <= than the current stack level, it won't do anything.
		 */
 		KDvoid				popToSceneStackLevel ( KDint nLevel );

		/** 
		 *	Replaces the running scene with a new one. The running scene is terminated.
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid				replaceScene ( Scene* pScene );

		/**
		 *	Ends the execution, releases the running scene.
		 *	It doesn't remove the OpenGL view from its parent. You have to do it manually.
		 *	@lua endToLua
		 */
		KDvoid				end ( KDvoid );

		/** 
		 *	Pauses the running scene.
		 *	The running scene will be _drawed_ but all scheduled timers will be paused
		 *	While paused, the draw rate will be 4 FPS to reduce CPU consumption
		 */
		KDvoid				pause ( KDvoid );

		/**
		 *	Resumes the paused scene
		 *	The scheduled timers will be activated again.
		 *	The "delta time" will be 0 (as if the game wasn't paused)
		 */
		KDvoid				resume ( KDvoid );

		/**
		 *	Stops the animation. Nothing will be drawn. The main loop won't be triggered anymore.
		 *	If you don't want to pause your animation call [pause] instead.
		 */
		virtual KDvoid		stopAnimation ( KDvoid ) = 0;

		/** 
		 *	The main loop is triggered again.
		 *	Call this function only if [stopAnimation] was called earlier
		 *	@warning Don't call this function to start the main loop. To run the main loop call runWithScene
		 */
		virtual KDvoid		startAnimation ( KDvoid ) = 0;

		/** 
		 *	Draw the scene.
		 *	This method is called every frame. Don't call it manually.
		 */
		KDvoid				drawScene ( KDvoid );

		// Memory Helper

		/** 
		 *	Removes all cocos2d cached data.
		 *	It will purge the TextureCache, SpriteFrameCache, LabelBMFont cache
		 *	@since v0.99.3
		 */
		KDvoid				purgeCachedData ( KDvoid );

		/** sets the default values based on the Configuration info */
		KDvoid				setDefaultValues ( KDvoid );

		// OpenGL Helper

		/** sets the OpenGL default values */
		KDvoid				setGLDefaultValues ( KDvoid );

		/** enables/disables OpenGL alpha blending */
		KDvoid				setAlphaBlending ( KDbool bOn );

		/** enables/disables OpenGL depth test */
		KDvoid				setDepthTest ( KDbool bOn );

		virtual KDbool		mainLoop ( KDvoid ) = 0;

		/**
		 *	The size in pixels of the surface. It could be different than the screen size.
		 *	High-res devices might have a higher surface size than the screen size.
		 *	Only available when compiled using SDK >= 4.0.
		 *	@since v0.99.4
		 */
		KDvoid				setContentScaleFactor ( KDfloat fScaleFactor );
		KDfloat				getContentScaleFactor ( KDvoid ) const;

	public :

		/**
		 *	Gets the Scheduler associated with this director
		 *	@since v2.0
		 */
		Scheduler*			getScheduler ( KDvoid ) const;
    
		/**
		 *	Sets the Scheduler associated with this director
		 *	@since v2.0
		 */
		KDvoid				setScheduler ( Scheduler* pScheduler );

		/**
		 *	Gets the ActionManager associated with this director
		 *	@since v2.0
		 */
		ActionManager*		getActionManager ( KDvoid ) const;
    
		/** 
		 *	Sets the ActionManager associated with this director
		 *	@since v2.0
		 */
		KDvoid				setActionManager ( ActionManager* pActionManager );
    
		/** 
		 *	Gets the EventDispatcher associated with this director 
		 *	@since v3.0
		 */
		EventDispatcher*	getEventDispatcher ( KDvoid ) const;
    
		/** 
		 *	Sets the EventDispatcher associated with this director 
		 *	@since v3.0
		 */
		KDvoid				setEventDispatcher ( EventDispatcher* pDispatcher );
    
		/* Gets delta time since last tick to main loop */
		KDfloat				getDeltaTime ( KDvoid ) const;
    
		/**
		 *  get Frame Rate
		 */
		KDfloat				getFrameRate ( KDvoid ) const { return m_fFrameRate; }

	protected :

		KDbool				m_bPurgeDirecotorInNextLoop; // this flag will be set to true in end()
    
		KDvoid				setNextScene ( KDvoid );
    
		KDvoid				showStats ( KDvoid );
		
		KDvoid				createStatsLabel ( KDvoid );
		
		KDvoid				calculateMPF ( KDvoid );

		KDvoid				getFPSImageData ( KDubyte** ppDatapointer, KDsize* pLength );
    
		/** calculates delta time since last time it was called */    
		KDvoid				calculateDeltaTime ( KDvoid );

		// textureCache creation or release
		KDvoid				initTextureCache ( KDvoid );

		KDvoid				destroyTextureCache ( KDvoid );

	protected :

		/**
		 *	Scheduler associated with this director
		 *	@since v2.0
		 */
		Scheduler*			m_pScheduler;
    
		/**
		 *	ActionManager associated with this director
		 *	@since v2.0
		 */
		ActionManager*		m_pActionManager;
    
		/**
		 *	EventDispatcher associated with this director
		 *	@since v3.0
		 */
		EventDispatcher*	m_pEventDispatcher;
        
		/* delta time since last tick to main loop */
		KDfloat				m_fDeltaTime;
    
		/* The EGLView, where everything is rendered */
		EGLView*			m_pOpenGLView;

		//texture cache belongs to this director
		TextureCache*		m_pTextureCache;

		KDdouble			m_dAnimationInterval;
		KDdouble			m_dOldAnimationInterval;

		/* landscape mode ? */
		KDbool				m_bLandscape;
    
		KDbool				m_bDisplayStats;
		KDfloat				m_fAccumDt;
		KDfloat				m_fFrameRate;
    
		LabelAtlas*			m_pFPSLabel;
		LabelAtlas*			m_pSPFLabel;
		LabelAtlas*			m_pDrawsLabel;
    
		/** Whether or not the Director is paused */
		KDbool				m_bPaused;

		/* How many frames were called since the director started */
		KDuint				m_uTotalFrames;
		KDuint				m_uFrames;
		KDfloat				m_fSecondsPerFrame;
     
		/* The running scene */
		Scene*				m_pRunningScene;
    
		/*
		 *	will be the next 'runningScene' in the next frame
		 *	nextScene is a weak reference. 
		 */
		Scene*				m_pNextScene;
    
		/* If true, then "old" scene will receive the cleanup message */
		KDbool				m_bSendCleanupToScene;

		/* scheduled scenes */
		Array*				m_pScenesStack;
    
		/* last time the main loop was updated */
		KDdouble			m_dLastUpdate;

		/* whether or not the next delta time will be zero */
		KDbool				m_bNextDeltaTimeZero;
    
		/* projection used */
		Projection			m_eProjection;

		/* window size in points */
		Size				m_tWinSizeInPoints;
    
		/* content scale factor */
		KDfloat				m_fContentScaleFactor;

		/* store the fps string */
		KDchar*				m_szFPS;

		/* This object will be visited after the scene. Useful to hook a notification node */
		Node*				m_pNotificationNode;

		/* Projection protocol delegate */
		DirectorDelegate*	m_pProjectionDelegate;
    
		// EGLView will recreate stats labels to fit visible rect
		friend class		EGLView;
};

/** 
 *	@brief DisplayLinkDirector is a Director that synchronizes timers with the refresh rate of the display.
 *
 *	Features and Limitations:
 *		- Scheduled timers & drawing are synchronizes with the refresh rate of the display
 *		- Only supports animation intervals of 1/60 1/30 & 1/15
 *
 *	@since v0.8.2
 */
class DisplayLinkDirector : public Director
{
	public :

		DisplayLinkDirector ( KDvoid );

		//
		// Overrides
		//
		virtual KDbool		mainLoop ( KDvoid ) override;

		virtual KDvoid		setAnimationInterval ( KDdouble value ) override;

		virtual KDvoid		startAnimation ( KDvoid ) override;

		virtual KDvoid		stopAnimation ( KDvoid ) override;

	protected :

		KDbool				m_bInvalid;
};

// end of base_node group
/// @}

NS_CC_END

#endif // __CCDirector_h__
