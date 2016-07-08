/* -----------------------------------------------------------------------------------
 *
 *      File            CCLayer.h
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

#ifndef __CCLayer_h__
#define __CCLayer_h__

#include "../base_nodes/CCNode.h"
#include "../../base/CCArray.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN
#include "../../physics/CCPhysicsSetting.h"

#include "../event_dispatcher/CCEventKeyboard.h"

NS_CC_BEGIN

/**
 *	@addtogroup layer
 *	@{
 */

class TouchScriptHandlerEntry;

class EventListenerTouch;
class EventListenerKeyboard;
class EventListenerAcceleration;

//
// Layer
//

/**
 *	@brief Layer is a subclass of Node that implements the TouchEventsDelegate protocol.
 *
 *	All features from Node are valid, plus the following new features:
 *		- It can receive iPhone Touches
 *		- It can receive Accelerometer input
 */
class CC_DLL Layer : public Node
{
	public :

		/** creates a fullscreen black layer */
		static Layer*		create(void);
		
		/**
		 *	@js ctor
		 */
		Layer();

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Layer ( KDvoid );

		virtual KDbool				init ( KDvoid );
    
		/**
		 *	Callback function should not be deprecated, it will generate lots of warnings.
		 *	Since 'setTouchEnabled' was deprecated, it will make warnings if developer overrides onTouchXXX and invokes setTouchEnabled(true) instead of using EventDispatcher::addEventListenerWithXXX.
		 */
		virtual KDbool				onTouchBegan		( Touch* pTouch, Event* pEvent ); 
		virtual KDvoid				onTouchMoved		( Touch* pTouch, Event* pEvent ); 
		virtual KDvoid				onTouchEnded		( Touch* pTouch, Event* pEvent ); 
		virtual KDvoid				onTouchCancelled	( Touch* pTouch, Event* pEvent );

		virtual KDvoid				onTouchesBegan		( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved		( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded		( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesCancelled	( const std::vector<Touch*>& aTouches, Event* pEvent );

		/** 
		 *	Callback function should not be deprecated, it will generate lots of warnings.
		 *	Since 'setAccelerometerEnabled' was deprecated, it will make warnings if developer overrides onAcceleration and invokes setAccelerometerEnabled(true) instead of using EventDispatcher::addEventListenerWithXXX.
		 */
		virtual KDvoid				onAcceleration ( Acceleration* pAcc, Event* pEvent );

		/** 
		 *	Callback function should not be deprecated, it will generate lots of warnings.
		 *	Since 'setKeyboardEnabled' was deprecated, it will make warnings if developer overrides onKeyXXX and invokes setKeyboardEnabled(true) instead of using EventDispatcher::addEventListenerWithXXX.
		 */
		virtual KDvoid				onKeyPressed  ( EventKeyboard::KeyCode eKeyCode, Event* pEvent );
		virtual KDvoid				onKeyReleased ( EventKeyboard::KeyCode eKeyCode, Event* pEvent );

		// Added By XMSoft : Move Visible Position & Set Visible Size
		KDvoid						setVisibleViewport ( KDvoid );

	protected :

		KDbool						m_bTouchEnabled;
		KDbool						m_bAccelerometerEnabled;
		KDbool						m_bKeyboardEnabled;
		EventListener*				m_pTouchListener;
		EventListenerKeyboard*		m_pKeyboardListener;
		EventListenerAcceleration*	m_pAccelerationListener;
};

/**
 *	LayerRGBA is a subclass of Layer that implements the RGBAProtocol protocol using a solid color as the background.
 *
 *	All features from Layer are valid, plus the following new features that propagate into children that conform to the RGBAProtocol:
 *		- opacity
 *		- RGB colors
 *	@since 2.1
 */
class CC_DLL LayerRGBA : public Layer, public RGBAProtocol
{
	public :

		CREATE_FUNC ( LayerRGBA );

		/**
		 *	@js ctor
		 */
		LayerRGBA ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LayerRGBA ( KDvoid );
    
		virtual KDbool				init ( KDvoid );

		//
		// Overrides
		//
		virtual GLubyte				getOpacity ( KDvoid ) const override;
		virtual GLubyte				getDisplayedOpacity ( KDvoid ) const override;

		virtual KDvoid				setOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity ) override;

		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid ) const override;
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled ) override;
    
		virtual const Color3B&		getColor ( KDvoid ) const override;
		virtual const Color3B&		getDisplayedColor ( KDvoid ) const override;

		virtual KDvoid				setColor ( const Color3B& tColor ) override;
		virtual KDvoid				updateDisplayedColor ( const Color3B& tParentColor ) override;

		virtual KDbool				 isCascadeColorEnabled ( KDvoid ) const override;
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled ) override;
    
		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue ) override { CC_UNUSED_PARAM ( bValue ); }
		virtual KDbool				 isOpacityModifyRGB ( KDvoid ) const override { return false; }

	protected :

		GLubyte						m_cDisplayedOpacity;
		GLubyte						m_cRealOpacity;
		Color3B						m_tDisplayedColor;
		Color3B						m_tRealColor;
		KDbool						m_bCascadeOpacityEnabled;
		KDbool						m_bCascadeColorEnabled;
};

//
// LayerColor
//
/** 
 *	@brief LayerColor is a subclass of Layer that implements the RGBAProtocol protocol.
 *
 *	All features from Layer are valid, plus the following new features:
 *		- opacity
 *		- RGB colors
 */
class CC_DLL LayerColor : public LayerRGBA, public BlendProtocol
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/** creates a fullscreen black layer */
		static LayerColor*			create ( KDvoid );
		
		/** creates a Layer with color, width and height in Points */
		static LayerColor*			create ( const Color4B& tColor, GLfloat fWidth, GLfloat fHeight );
		
		// Added By XMSoft
		static LayerColor*			create ( const Color4B& tColor, const Size& tSize );

		/** creates a Layer with color. Width and height are the window size. */
		static LayerColor*			create ( const Color4B& tColor);
		
		/**
		 *	@js ctor
		 */
		LayerColor ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LayerColor ( KDvoid );

		virtual KDbool				init ( KDvoid );
		
		/** 
		 *	initializes a Layer with color, width and height in Points 
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithColor ( const Color4B& tColor, GLfloat fWidth, GLfloat fHeight );

		// Added By XMSoft
		KDbool						initWithColor ( const Color4B& tColor, const Size& tSize );

		/**
		 *	initializes a Layer with color. Width and height are the window size. 
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithColor ( const Color4B& tColor );

		/** change width in Points*/
		KDvoid						changeWidth ( GLfloat w );

		/** change height in Points*/
		KDvoid						changeHeight ( GLfloat h );

		/**
		 *	change width and height in Points
		 *	@since v0.8
		 */
		KDvoid						changeWidthAndHeight ( GLfloat w ,GLfloat h );

		//
		// Overrides
		//
		virtual KDvoid				draw ( KDvoid ) override;
		virtual KDvoid				setColor ( const Color3B& tColor ) override;
		virtual KDvoid				setOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid				setContentSize ( const Size& tVar ) override;
		
		/**
		 *	BlendFunction. Conforms to BlendProtocol protocol 
		 *
		 *	@js NA
		 *	@lua NA
		 */
		virtual const BlendFunc&	getBlendFunc ( KDvoid ) const override;
		
		/**
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var setBlendFunc(var src, var dst)
		 *	In lua: local setBlendFunc(local src, local dst)
		 *	@endcode
		 */
		virtual KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc ) override;

	protected :

		virtual KDvoid				updateColor ( KDvoid );

		BlendFunc					m_tBlendFunc;
		Vertex2F					m_aSquareVertices [ 4 ];
		Color4F						m_aSquareColors   [ 4 ];
};

//
// LayerGradient
//

/**
 *	@brief LayerGradient is a subclass of LayerColor that draws gradients across the background.
 *
 *	All features from LayerColor are valid, plus the following new features:
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
 *	@since v0.99.5
 */
class CC_DLL LayerGradient : public LayerColor
{
	public :

		/** Creates a fullscreen black layer */
		static LayerGradient*		create ( KDvoid );

		/** Creates a full-screen Layer with a gradient between start and end. */
		static LayerGradient*		create ( const Color4B& tStart, const Color4B& tEnd );

		/** Creates a full-screen Layer with a gradient between start and end in the direction of v. */
		static LayerGradient*		create ( const Color4B& tStart, const Color4B& tEnd, const Point& v );

		virtual KDbool				init ( KDvoid );

		/**
		 *	Initializes the Layer with a gradient between start and end. 
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithColor ( const Color4B& tStart, const Color4B& tEnd );

		/** 
		 *	Initializes the Layer with a gradient between start and end in the direction of v. 
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithColor ( const Color4B& tStart, const Color4B& tEnd, const Point& v );
    
		/** 
		 *	Whether or not the interpolation will be compressed in order to display all the colors of the gradient both in canonical and non canonical vectors
		 *	Default: true
		 */
		KDvoid						setCompressedInterpolation ( KDbool bCompressedInterpolation );
		KDbool						 isCompressedInterpolation ( KDvoid ) const;

		/** Sets the start color of the gradient */
		KDvoid						setStartColor ( const Color3B& tStartColor );

		/** Returns the start color of the gradient */
		const Color3B&				getStartColor ( KDvoid ) const;

		/** Sets the end color of the gradient */
		KDvoid						setEndColor ( const Color3B& tEndColor );

		/** Returns the end color of the gradient */
		const Color3B&				getEndColor ( KDvoid ) const;

		/** Returns the start opacity of the gradient */
		KDvoid						setStartOpacity ( GLubyte cStartOpacity );

		/** Returns the start opacity of the gradient */
		GLubyte						getStartOpacity ( KDvoid ) const;

		/** Returns the end opacity of the gradient */
		KDvoid						setEndOpacity ( GLubyte cEndOpacity );

		/** Returns the end opacity of the gradient */
		GLubyte						getEndOpacity ( KDvoid ) const;

		/** 
		 *	Sets the directional vector that will be used for the gradient.
		 *	The default value is vertical direction (0,-1). 
		 */
		KDvoid						setVector ( const Point& tAlongVector );

		/** Returns the directional vector used for the gradient */
		const Point&				getVector ( KDvoid ) const;

	protected :

		virtual KDvoid				updateColor ( KDvoid ) override;

		Color3B						m_tStartColor;
		Color3B						m_tEndColor;
		GLubyte						m_cStartOpacity;
		GLubyte						m_cEndOpacity;
		Point						m_tAlongVector;
		KDbool						m_bCompressedInterpolation;
};


/** 
 *	@brief MultipleLayer is a Layer with the ability to multiplex it's children.
 *	Features:
 *		- It supports one or more children
 *		- Only one children will be active a time
 */
class CC_DLL LayerMultiplex : public Layer
{
	public :

		/** 
		 *	creates and initializes a LayerMultiplex object 
		 *	@js NA
		 *	@lua NA
		 */
		static LayerMultiplex*		create ( KDvoid );

		/** 
		 *	creates a LayerMultiplex with an array of layers.
		 *	@since v2.1
		 *	@js NA
		 */
		static LayerMultiplex*		createWithArray ( Array* pArrayOfLayers );

		/** 
		 *	creates a LayerMultiplex with one or more layers using a variable argument list. 
		 *	@code
		 *	When this function bound to lua or js,the input params are changed.
		 *	In js:var create(...)
		 *	In lua:local create(...)
		 *	@endcode
		 */
		static LayerMultiplex*		create ( Layer* pLayer, ... );

		/**
		 *	lua script can not init with undetermined number of variables
		 *	so add these functions to be used with lua.
		 *	@js NA
		 *	@lua NA
		 */
		static LayerMultiplex*		createWithLayer ( Layer* pLayer );

		/**
		 *	@js ctor
		 */
		LayerMultiplex ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LayerMultiplex ( KDvoid );

		virtual KDbool				init ( KDvoid );

		/**
		 *	initializes a MultiplexLayer with one or more layers using a variable argument list. 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool						initWithLayers ( Layer* pLayer, KDVaListKHR pArgs );

		/**
		 *	initializes a MultiplexLayer with an array of layers
		 *	@since v2.1
		 */
		KDbool						initWithArray ( Array* pArrayOfLayers );

		KDvoid						addLayer ( Layer* pLayer );

		/** 
		 *	switches to a certain layer indexed by n.
		 *	The current (old) layer will be removed from it's parent with 'cleanup=true'.
		 */
		KDvoid						switchTo ( KDint n );

		/** 
		 *	release the current layer and switches to another layer indexed by n.
		 *	The current (old) layer will be removed from it's parent with 'cleanup=true'.
		 */
		KDvoid						switchToAndReleaseMe ( KDint n );

	protected :

		KDuint						m_uEnabledLayer;
		Array*						m_pLayers;
};


//	end of layer group
/// @}

NS_CC_END

#endif	// __CCLayer_h__

