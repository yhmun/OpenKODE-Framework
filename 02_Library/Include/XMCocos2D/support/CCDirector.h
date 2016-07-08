/* -----------------------------------------------------------------------------------
 *
 *      File            CCDirector.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCDirector_h__
#define __CCDirector_h__

#include "../cocoa/CCObject.h"
#include "../cocoa/CCArray.h"
#include "../cocoa/CCGeometry.h"
#include "../ccTypes.h"
#include "../ccTypeInfo.h"
#include "../kazmath/kazmath.h"

#include <string>

NS_CC_BEGIN
   
/**
 * @addtogroup base_nodes
 * @{
 */

/**
 *	@typedef	ccProjection
 *	Possible OpenGL projections used by director
 */
typedef enum
{	
	/// sets a 2D projection (orthogonal projection)
	kCCDirectorProjection2D     ,                            

	/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
	kCCDirectorProjection3D     ,                       

	/// it calls "updateProjection" on the projection delegate.
	kCCDirectorProjectionCustom ,         

	/// Default projection is 3D projection
    kCCDirectorProjectionDefault = kCCDirectorProjection3D,
} ccDirectorProjection;

// Forward declarations. 
class CCLabelAtlas;
class CCScene;
class CCDirectorDelegate;
class CCNode;
class CCScheduler;
class CCActionManager;
class CCTouchDispatcher;
class CCKeypadDispatcher;
class CCAccelerometer;
class CCEventDispatcher;

/**
 *	@brief Class that creates and handle the main Window and manages how and when to execute the Scenes.
 *
 *	The CCDirector is also responsible for:
 *		- initializing the OpenGL context
 *		- setting the projection (default one is 3D)
 *		- setting the orientation (default one is Protrait)
 *
 *	Since the CCDirector is a singleton, the standard way to use it is by calling:
 *		- CCDirector::sharedDirector()->methodName();
 *
 *	The CCDirector also sets the default OpenGL context:
 *		- GL_TEXTURE_2D is enabled
 *		- GL_VERTEX_ARRAY is enabled
 *		- GL_COLOR_ARRAY is enabled
 *		- GL_TEXTURE_COORD_ARRAY is enabled
 */
class CCDirector : public CCObject, public TypeInfo
{
	protected :
				 CCDirector ( KDvoid );
		virtual ~CCDirector ( KDvoid );		

		virtual KDbool			init ( KDvoid );				

    public :
    
		/** returns a shared instance of the director */
		static CCDirector*		sharedDirector ( KDvoid );

		// Don't call it manually.
		KDvoid					purgeDirector  ( KDvoid );

	public :

		virtual KDint32			getClassTypeInfo ( KDvoid );

		// attribute

		/** Get current running Scene. Director can only run one Scene at the time */
		inline CCScene*			getRunningScene ( KDvoid ) { return m_pRunningScene; }

		/** Get the FPS value */
		inline  KDdouble		getAnimationInterval ( KDvoid ) { return m_dAnimationInterval; }

		/** Set the FPS value. */
		virtual KDvoid			setAnimationInterval ( KDdouble dValue );

		/** Whether or not to display the FPS on the bottom-left corner */
		inline  KDbool			isDisplayStats ( KDvoid ) { return m_bDisplayStats; }

		/** Display the FPS on the bottom-left corner */
		inline  KDvoid			setDisplayStats ( KDbool bDisplayStats ) { m_bDisplayStats = bDisplayStats; }
	    
		/** seconds per frame */
		inline  KDfloat			getSecondsPerFrame ( KDvoid ) { return m_fSecondsPerFrame; }

		inline KDbool			 isNextDeltaTimeZero ( KDvoid ) { return m_bNextDeltaTimeZero; }
		KDvoid					setNextDeltaTimeZero ( KDbool bNextDeltaTimeZero );

		/** Whether or not the Director is paused */
		inline KDbool			 isPaused ( KDvoid ) { return m_bPaused; }

		/** How many frames were called since the director started */
		inline KDuint			getTotalFrames ( KDvoid ) { return m_uTotalFrames; }	

		/**
		 *	Sets an OpenGL projection
		 *	@since v0.8.2
		 */
		inline  ccDirectorProjection	getProjection ( KDvoid ) { return m_eProjection; }
		KDvoid							setProjection ( ccDirectorProjection eProjection );

		/** reshape projection matrix when canvas has been change"*/
		KDvoid					reshapeProjection ( const CCSize& tNewWindowSize );

		/** Sets the glViewport*/
		KDvoid					setViewport ( KDvoid );

		/** 
		 *	Whether or not the replaced scene will receive the cleanup message.
		 *	If the new scene is pushed, then the old scene won't receive the "cleanup" message.
		 *	If the new scene replaces the old one, the it will receive the "cleanup" message.
		 *	@since	v0.99.0
		 */
		inline KDbool			isSendCleanupToScene ( KDvoid ) { return m_bSendCleanupToScene; }

		/** 
		 *	This object will be visited after the main scene is visited.
		 *	This object MUST implement the "visit" selector.
		 *	Useful to hook a notification object, like CCNotifications (http://github.com/manucorporat/CCNotifications)
		 *	@since	v0.99.5
		 */
		CCNode*					getNotificationNode ( KDvoid );
		KDvoid					setNotificationNode ( CCNode* pNode );

		CCNode*					getBackgroundNode ( KDvoid );
		KDvoid					setBackgroundNode ( CCNode* pNode );

		/** 
		 *	CCDirector delegate. It shall implemente the CCDirectorDelegate protocol
		 *	@since v0.99.5
		 */
		CCDirectorDelegate*		getDelegate ( KDvoid ) const;
		KDvoid					setDelegate ( CCDirectorDelegate* pDelegate );

		/** returns the size of the OpenGL view in points. */
        CCSize					getWinSize ( KDvoid );

		/** returns the size of the OpenGL view in pixels. */
		CCSize					getWinSizeInPixels ( KDvoid );

		/** 
		 *	returns visible size of the OpenGL view in points.
		 *  the value is equal to getWinSize if don't invoke
		 *  CCEGLView::setDesignResolutionSize()
		 */
		CCSize					getVisibleSize ( KDvoid );
	    
		/** returns visible origin of the OpenGL view in points. */
		CCPoint					getVisibleOrigin ( KDvoid );

		KDvoid					GLToClipTransform ( kmMat4* pTransformOut = KD_NULL );
		KDvoid					ClipToGLTransform ( KDvoid );

		/**
		 *	converts a UIKit coordinate to an OpenGL coordinate
		 *	Useful to convert (multi) touch coordinates to the current layout (portrait or landscape)
		 */
		CCPoint					convertToGL ( const CCPoint& tUIPoint );

		/** 
		 *	converts an OpenGL coordinate to a UIKit coordinate
		 *	Useful to convert node points to window points for calls such as glScissor
		 */
		CCPoint					convertToUI ( const CCPoint& tGLPoint );

		/// XXX: missing description 
		KDfloat					getZEye ( KDvoid );

		// Scene Management

		/**	
		 *	Enters the Director's main loop with the given Scene. 
		 *	Call it to run only your FIRST scene.
		 *	Don't call it if there is already a running scene.
		 */
		KDvoid					runWithScene ( CCScene* pScene );

		/**
		 *	Suspends the execution of the running scene, pushing it on the stack of suspended scenes.
		 *	The new scene will be executed.
		 *	Try to avoid big stacks of pushed scenes to reduce memory allocation. 
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid					pushScene ( CCScene* pScene );

		/**
		 *	Pops out a scene from the queue.
		 *	This scene will replace the running one.
		 *	The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid					popScene ( KDvoid );

		/**
		 *	Pops out all scenes from the queue until the root scene in the queue.
		 *	This scene will replace the running one.
		 *	The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid					popToRootScene ( KDvoid );

		/** 
		 *	Pops out all scenes from the queue until it reaches `level`.
		 *	If level is 0, it will end the director.
		 *	If level is 1, it will pop all scenes until it reaches to root scene.
		 *	If level is <= than the current stack level, it won't do anything.
		 */
 		KDvoid					popToSceneStackLevel ( KDint nLevel );

		/**
		 *	Replaces the running scene with a new one. The running scene is terminated.
		 *	ONLY call it if there is a running scene.
		 */
		KDvoid					replaceScene ( CCScene* pScene );

		/** 
		 *	Ends the execution, releases the running scene.
		 *	It doesn't remove the OpenGL view from its parent. You have to do it manually.
		 */
		KDvoid					end ( KDvoid );

		/** 
		 *	Pauses the running scene.
		 *	The running scene will be _drawed_ but all scheduled timers will be paused
		 *	While paused, the draw rate will be 4 FPS to reduce CPU consumption
		 */
		KDvoid					pause ( KDvoid );

		/** 
		 *	Resumes the paused scene
		 *	The scheduled timers will be activated again.
		 *	The "delta time" will be 0 (as if the game wasn't paused)
		 */
		KDvoid					resume ( KDvoid );

		/**
		 *	Stops the animation. Nothing will be drawn. The main loop won't be triggered anymore.
		 *	If you don't want to pause your animation call [pause] instead.
		 */
		KDvoid					stopAnimation ( KDvoid );

		/**
		 *	The main loop is triggered again.
		 *	Call this function only if [stopAnimation] was called earlier
		 *	@warning Don't call this function to start the main loop. To run the main loop call runWithScene
		 */
		KDvoid					startAnimation ( KDvoid );

		/**
		 *	Draw the scene.
		 *	This method is called every frame. Don't call it manually.
		 */
		KDvoid					drawScene ( KDvoid );

		// Memory Helper
		
		/**
		 *	Removes cached all cocos2d cached data.
		 *	It will purge the CCTextureCache, CCSpriteFrameCache, CCLabelBMFont cache
		 *	@since	v0.99.3
		 */
		KDvoid					purgeCachedData ( KDvoid );

		/** sets the default values based on the CCConfiguration info */
		KDvoid					setDefaultValues ( KDvoid );

		// OpenGL Helper

		/** sets the OpenGL default values */
		KDvoid					setGLDefaultValues ( KDvoid );

		/** enables/disables OpenGL alpha blending */
		KDvoid					setAlphaBlending ( KDbool bOn );

		/** enables/disables OpenGL depth test */
		KDvoid					setDepthTest ( KDbool bOn );

		KDvoid					setClearColor ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha );

		KDvoid					setClearFlags ( KDuint uFlags );

		KDbool					mainLoop ( KDvoid );

		/**
		 *	The size in pixels of the surface. It could be different than the screen size.
		 *	High-res devices might have a higher surface size than the screen size.
		 *	Only available when compiled using SDK >= 4.0.
		 *	@since v0.99.4
		 */
		KDvoid					setContentScaleFactor ( KDfloat fScaleFactor );
		KDfloat					getContentScaleFactor ( KDvoid );

	public :

		/** 
		 *	CCScheduler associated with this director
		 *	@since v2.0
		 */
		CC_PROPERTY ( CCScheduler*, m_pScheduler, Scheduler );

		/** 
		 *	CCActionManager associated with this director
		 *	@since v2.0
		 */
		CC_PROPERTY ( CCActionManager*, m_pActionManager, ActionManager );

		/** 
		 *	CCTouchDispatcher associated with this director
		 *	@since v2.0
		 */
		CC_PROPERTY ( CCTouchDispatcher*, m_pTouchDispatcher, TouchDispatcher );

		/** 
		 *	CCKeypadDispatcher associated with this director
		 *	@since v2.0
		 */
		CC_PROPERTY ( CCKeypadDispatcher*, m_pKeypadDispatcher, KeypadDispatcher );

		/** 
		 *	CCAccelerometer associated with this director
		 *	@since v2.0
		 */
		CC_PROPERTY ( CCAccelerometer*, m_pAccelerometer, Accelerometer );

		CC_PROPERTY ( CCEventDispatcher*, m_pEventDispatcher, EventDispatcher );

		/* delta time since last tick to main loop */
		CC_PROPERTY_READONLY ( KDfloat, m_fDeltaTime, DeltaTime );

	protected :		

		KDvoid					setNextScene	 ( KDvoid );		

		KDvoid					showStats		 ( KDvoid );
		KDvoid					createStatsLabel ( KDvoid );
		KDvoid					calculateMPF	 ( KDvoid );
		KDvoid					getFPSImageData	 ( KDubyte** pDatapointer, KDuint* pLength );

		/** calculates delta time since last time it was called */    
		KDvoid					calculateDeltaTime ( KDvoid );

	protected :

		KDbool					m_bPurgeDirecotorInNextLoop; // this flag will be set to true in end()

		KDdouble				m_dAnimationInterval;
		KDdouble				m_dOldAnimationInterval;

		/* landscape mode? */
		KDbool					m_bLandscape;

		KDbool					m_bDisplayStats;
		KDfloat					m_fAccumDt;
		KDfloat					m_fFrameRate;

		CCLabelAtlas*			m_pFPSLabel;
		CCLabelAtlas*			m_pSPFLabel;
		CCLabelAtlas*			m_pDrawsLabel;

		/* Whether or not the Director is paused */
		KDbool					m_bPaused;

		/* How many frames were called since the director started */
		KDuint					m_uTotalFrames;
		KDuint					m_uFrames;
		KDfloat					m_fSecondsPerFrame;

		/* The running scene */
		CCScene*				m_pRunningScene;
    
		/* will be the next 'runningScene' in the next frame
		 nextScene is a weak reference. */
		CCScene*				m_pNextScene;
	    
		/* If YES, then "old" scene will receive the cleanup message */
		KDbool					m_bSendCleanupToScene;

		/* scheduled scenes */
		CCArray*				m_pScenesStack;
	    
		/* last time the main loop was updated */
		KDuint					m_uLastUpdate;

		/* whether or not the next delta time will be zero */
		KDbool					m_bNextDeltaTimeZero;
	    
		/* projection used */
		ccDirectorProjection	m_eProjection;

		/* window size in points */
        CCSize                  m_tWinSizeInPoints;

		/* content scale factor */
		KDfloat					m_fContentScaleFactor;

		/* store the fps string */
		KDchar					m_szFPS [ 16 ];

		/* This object will be visited after the scene. Useful to hook a notification node */
		CCNode*					m_pNotificationNode;

		/* Projection protocol delegate */
		CCDirectorDelegate*		m_pProjectionDelegate;

		KDbool					m_bInvalid;
		KDbool					m_bNodeDebug;

		ccColor4F				m_tClearColor;
		KDuint					m_uClearFlags;

		kmMat4					m_tProjection;
		kmMat4					m_tModelview;

		CCNode*					m_pBackNode;

		// CCEGLView will recreate stats labels to fit visible rect
		friend class CCEGLView;
};

// end of base_node group
/// @}

NS_CC_END

#endif // __CCDirector_h__
