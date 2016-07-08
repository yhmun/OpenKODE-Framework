/* -----------------------------------------------------------------------------------
 *
 *      File            CCLayer.h
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
#ifndef __CCLayer_h__
#define __CCLayer_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"
#include "../touch_dispatcher/CCTouchDelegateProtocol.h"
#include "../keypad_dispatcher/CCKeypadDelegate.h"
#include "../accelerometer_dispatcher/CCAccelerometerDelegate.h"
#include "../event_dispatcher/CCEventDelegate.h"
#include "../cocoa/CCArray.h"

#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN
   
typedef enum
{
	kCCTouchesAllAtOnce	,
	kCCTouchesOneByOne	,
} ccTouchesMode;

/**
 * @addtogroup layer
 * @{
 */

class CCTouchScriptHandlerEntry;

/**
 *	@brief CCLayer is a subclass of CCNode that implements the TouchEventsDelegate protocol.
 *
 *	All features from CCNode are valid, plus the following new features:
 *		- It can receive iPhone Touches
 *		- It can receive Accelerometer input
 */
class CCLayer : public CCNode, public CCTouchDelegate, public CCKeypadDelegate, public CCAccelerometerDelegate, public CCEventDelegate
{
	public :
		         CCLayer ( KDvoid );
		virtual ~CCLayer ( KDvoid );

		/** create one layer */
		static   CCLayer*		create ( KDvoid );

		static   CCLayer*		createWithVisibleViewport ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDbool			initWithVisibleViewport ( KDvoid );

		virtual KDvoid			onEnter ( KDvoid );
		virtual KDvoid			onEnterTransitionDidFinish ( KDvoid );
		virtual KDvoid			onExit ( KDvoid );
		
		// default implements are used to call script callback if exist
		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		// default implements are used to call script callback if exist
		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			didAccelerate  ( CCAcceleration* pAccelerationValue );

		virtual KDvoid			onEvent ( const KDEvent* pEvent );

		KDvoid					 registerScriptAccelerateHandler ( KDint nHandler );
		KDvoid					unregisterScriptAccelerateHandler ( KDvoid );

		/** 
		 *	If isTouchEnabled, this method is called onEnter. Override it to change the
		 *	way CCLayer receives touch events.
		 *	( Default: CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0); )
		 *	Example:
		 *	void CCLayer::registerWithTouchDispatcher()
		 *	{
		 *		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,INT_MIN+1,KD_TRUE);
		 *	}
		 *	@since v0.8.0
		 */
		virtual KDvoid			registerWithTouchDispatcher ( KDvoid );

		/** Register script touch events handler */
		virtual KDvoid			registerScriptTouchHandler ( KDint nHandler, KDbool bIsMultiTouches = KD_FALSE, KDint nPriority = KDINT_MIN, KDbool bSwallowsTouches = KD_FALSE );
		
		/** Unregister script touch events handler */
		virtual KDvoid			unregisterScriptTouchHandler ( KDvoid );

		/**
		 *	whether or not it will receive Touch events.
		 *	You can enable / disable touch events with this property.
		 *	Only the touches of this node will be affected. This "method" is not propagated to it's children.
		 *	@since	v0.8.1
		 */
		virtual KDbool			 isTouchEnabled ( KDvoid );
		virtual KDvoid			setTouchEnabled ( KDbool bEnable );

		virtual KDvoid			setTouchMode ( ccTouchesMode eMode );
		virtual ccTouchesMode	getTouchMode ( KDvoid );

		virtual KDvoid			setTouchPriority ( KDint nPriority );
		virtual KDint			getTouchPriority ( KDvoid );

		/** 
		 *	whether or not it will receive Accelerometer events
		 *	You can enable / disable accelerometer events with this property.
		 *	@since	v0.8.1
		 */
		virtual KDbool			 isAccelerometerEnabled  ( KDvoid );
		virtual KDvoid			setAccelerometerEnabled  ( KDbool bEnable );
		virtual KDvoid			setAccelerometerInterval ( KDdouble dInterval );

		/** 
		 *	whether or not it will receive keypad events
		 *	You can enable / disable accelerometer events with this property.
		 *	it's new in cocos2d-x
		 */
		virtual KDbool			 isKeypadEnabled ( KDvoid );
		virtual KDvoid			setKeypadEnabled ( KDbool bEnable );

		virtual KDbool			 isEventEnabled ( KDvoid );
		virtual KDvoid			setEventEnabled ( KDbool bEnable );

		/** Register keypad events handler */
		KDvoid					registerScriptKeypadHandler ( KDint nHandler );

		/** Unregister keypad events handler */
		KDvoid					unregisterScriptKeypadHandler ( KDvoid );

		virtual KDvoid			keyBackClicked ( KDvoid );
		virtual KDvoid			keyMenuClicked ( KDvoid );

		inline  CCTouchScriptHandlerEntry*		getScriptTouchHandlerEntry		( KDvoid ) { return m_pScriptTouchHandlerEntry; };
		inline  CCScriptHandlerEntry*			getScriptKeypadHandlerEntry		( KDvoid ) { return m_pScriptKeypadHandlerEntry; };
		inline  CCScriptHandlerEntry*			getScriptAccelerateHandlerEntry	( KDvoid ) { return m_pScriptAccelerateHandlerEntry; };

		// Set position and content size with the visible opengl viewport.
		KDvoid					setVisibleViewport ( KDvoid );

	private :

		// Script touch events handler
		KDint					excuteScriptTouchHandler ( KDint nEventType, CCTouch* pTouch );
		KDint					excuteScriptTouchHandler ( KDint nEventType, CCSet* pTouches );
		
	protected :

		KDbool							m_bTouchEnabled;
		KDbool							m_bAccelerometerEnabled;
		KDbool							m_bKeypadEnabled;
		KDbool							m_bEventEnabled;

		CCTouchScriptHandlerEntry*		m_pScriptTouchHandlerEntry;
		CCScriptHandlerEntry*			m_pScriptKeypadHandlerEntry;
		CCScriptHandlerEntry*			m_pScriptAccelerateHandlerEntry;

		KDint							m_nTouchPriority;
		ccTouchesMode					m_eTouchMode;
};

/**
 *	CCLayerRGBA is a subclass of CCLayer that implements the CCRGBAProtocol protocol using a solid color as the background.
 *
 *	All features from CCLayer are valid, plus the following new features that propagate into children that conform to the CCRGBAProtocol:
 *		- opacity
 *		- RGB colors
 *	@since 2.1
 */
class CCLayerRGBA : public CCLayer, public CCRGBAProtocol
{
	public :
				 CCLayerRGBA ( KDvoid );
		virtual ~CCLayerRGBA ( KDvoid );

		CREATE_FUNC ( CCLayerRGBA );
	    	 
	public :

		virtual KDbool				init ( KDvoid );
	    
		virtual GLubyte				getOpacity ( KDvoid );
		virtual GLubyte				getDisplayedOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity );

		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );
	    
		virtual const ccColor3B&	getColor ( KDvoid );
		virtual const ccColor3B&	getDisplayedColor ( KDvoid );
		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tParentColor );

		virtual KDbool				 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );
	    
		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue ) { CC_UNUSED_PARAM ( bValue ); }
		virtual KDbool				 isOpacityModifyRGB ( KDvoid ) { return KD_FALSE; }

	protected :

		GLubyte						m_cDisplayedOpacity;
		GLubyte						m_cRealOpacity;
		ccColor3B					m_tDisplayedColor;
		ccColor3B					m_tRealColor;
		KDbool						m_bCascadeOpacityEnabled;
		KDbool						m_bCascadeColorEnabled;
};

/**
 *	@brief	CCLayerColor is a subclass of CCLayer that implements the CCRGBAProtocol protocol.
 *
 *	All features from CCLayer are valid, plus the following new features:
 *		- opacity
 *		- RGB colors
 */
class CCLayerColor : public CCLayerRGBA, public CCBlendProtocol
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
				 CCLayerColor ( KDvoid );
		virtual ~CCLayerColor ( KDvoid );

		static  CCLayerColor*	create ( KDvoid );

		/** creates a CCLayer with color, width and height in Points */
		static  CCLayerColor*	create ( const ccColor4B& tColor, GLfloat fWidth, GLfloat fHeight );
		static  CCLayerColor*	create ( const ccColor4B& tColor, const CCSize& tSize );	

		/** creates a CCLayer with color. Width and height are the window size. */
		static  CCLayerColor*	create ( const ccColor4B& tColor );

	public :

		virtual KDbool			init ( KDvoid );

		/** initializes a CCLayer with color, width and height in Points */
		virtual KDbool			initWithColor ( const ccColor4B& tColor, GLfloat fWidth, GLfloat fHeight );
		virtual KDbool			initWithColor ( const ccColor4B& tColor, const CCSize& tSize );

		/** initializes a CCLayer with color. Width and height are the window size. */
		virtual KDbool			initWithColor ( const ccColor4B& tColor );

		virtual KDvoid			draw ( KDvoid );

		virtual KDvoid			setContentSize ( const CCSize& tSize );

		/** change width in Points*/
		KDvoid					changeWidth ( GLfloat fWidth );

		/** change height in Points*/
		KDvoid					changeHeight ( GLfloat fHeight );

		/** 
		 *	change width and height in Points
		 *	@since	v0.8
		 */
		KDvoid					changeWidthAndHeight ( GLfloat fWidth, GLfloat fHeight );

		/** BlendFunction. Conforms to CCBlendProtocol protocol */
		CC_PROPERTY ( ccBlendFunc, m_tBlendFunc, BlendFunc );

		virtual KDvoid			setColor ( const ccColor3B& tColor );
		virtual KDvoid			setOpacity ( GLubyte cOpacity );

	protected :

		virtual KDvoid			updateColor ( KDvoid );

	protected :

		ccVertex2F				m_aSquareVertices [ 4 ];
		ccColor4F				m_aSquareColors   [ 4 ];
};

/**
 *	CCLayerGradient is a subclass of CCLayerColor that draws gradients across
 *	the background.
 *
 *	All features from CCLayerColor are valid, plus the following new features:
 *		- direction
 *		- final color
 *		- interpolation mode
 *
 *	Color is interpolated between the startColor and endColor along the given
 *	vector (starting at the origin, ending at the terminus).  If no vector is
 *	supplied, it defaults to (0, -1) -- a fade from top to bottom.
 *
 *	If 'compressedInterpolation' is disabled, you will not see either the start or end color for
 *	non-cardinal vectors; a smooth gradient implying both end points will be still
 *	be drawn, however.
 *
 *	If ' compressedInterpolation' is enabled (default mode) you will see both the start and end colors of the gradient.
 *
 *	@since	v0.99.5
 */
class CCLayerGradient : public CCLayerColor
{
	public :

		static CCLayerGradient*		create ( KDvoid );

		/** Creates a full-screen CCLayer with a gradient between start and end. */
		static CCLayerGradient*		create ( const ccColor4B& tStart, const ccColor4B& tEnd );

		/** Creates a full-screen CCLayer with a gradient between start and end in the direction of v. */
		static CCLayerGradient*		create ( const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& tVector );

	public :

		virtual KDbool				init ( KDvoid );

		/** Initializes the CCLayer with a gradient between start and end. */
		virtual KDbool				initWithColor ( const ccColor4B& tStart, const ccColor4B& tEnd );

		/** Initializes the CCLayer with a gradient between start and end in the direction of v. */
		virtual KDbool				initWithColor ( const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& tVector );

		CC_PROPERTY_PASS_BY_REF ( ccColor3B, m_tStartColor, StartColor );
		CC_PROPERTY_PASS_BY_REF ( ccColor3B, m_tEndColor  , EndColor   );
		
		CC_PROPERTY ( GLubyte, m_cStartOpacity, StartOpacity );
		CC_PROPERTY ( GLubyte, m_cEndOpacity  , EndOpacity   );

		CC_PROPERTY_PASS_BY_REF ( CCPoint, m_tVector, Vector );

		/**
		 *	Whether or not the interpolation will be compressed in order to display all the colors of the gradient both in canonical and non canonical vectors
		 *	Default: YES
		 */
		virtual KDvoid				setCompressedInterpolation ( KDbool bCompressedInterpolation );
		virtual KDbool				 isCompressedInterpolation ( KDvoid );

	protected :

		virtual KDvoid				updateColor ( KDvoid );

	protected :

		KDbool						m_bCompressedInterpolation;
};

/**
 *	@brief	CCMultipleLayer is a CCLayer with the ability to multiplex it's children.
 *	Features:
 *		- It supports one or more children
 *		- Only one children will be active a time
 */
class CCLayerMultiplex : public CCLayer
{
	public :
		         CCLayerMultiplex ( KDvoid );
		virtual ~CCLayerMultiplex ( KDvoid );

	public :

		static  CCLayerMultiplex*	create ( KDvoid );

		/** 
		 *	creates a CCMultiplexLayer with an array of layers.
		 *	@since v2.1
		 */
		static  CCLayerMultiplex*	createWithArray ( CCArray* pArrayOfLayers );

		/** creates a CCLayerMultiplex with one or more layers using a variable argument list. */
		static  CCLayerMultiplex*	create ( CCLayer* pLayer, ... );

		/**
		 *	lua script can not init with undetermined number of variables
		 *	so add these functions to be used with lua.
		 */
		static  CCLayerMultiplex*	createWithLayer  ( CCLayer* pLayer );

	public :

		/** initializes a MultiplexLayer with one or more layers using a variable argument list. */
		virtual KDbool				initWithLayers ( CCLayer* pLayer, KDVaListKHR pArgs );

		/** 
		 *	initializes a CCMultiplexLayer with an array of layers
		 *	@since v2.1
		*/
		virtual KDbool				initWithArray ( CCArray* pArrayOfLayers );

		KDvoid						addLayer ( CCLayer* pLayer );

		/**
		 *	switches to a certain layer indexed by n. 
		 *	The current (old) layer will be removed from it's parent with 'cleanup:YES'.
		 */
		KDvoid						switchTo ( KDuint nEnabled );

		/** 
		 *	release the current layer and switches to another layer indexed by n.
		 *	The current (old) layer will be removed from it's parent with 'cleanup:YES'.
		 */
		KDvoid						switchToAndReleaseMe ( KDuint nEnabled );
		
		inline  KDuint				getEnabled ( KDvoid ) { return m_nEnabled; }

	protected :

		KDuint						m_nEnabled;
		CCArray*					m_pLayers;
};

// end of layer group
/// @}

NS_CC_END

#endif // __CCLayer_h__

