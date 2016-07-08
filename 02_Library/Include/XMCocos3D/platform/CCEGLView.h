/* -----------------------------------------------------------------------------------
 *
 *      File            CCEGLView.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013		cocos2d-x.org
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

#ifndef __CCEGLView_h__
#define __CCEGLView_h__

#include "../ccTypes.h"
#include "../platform/CCCommon.h"
#include "../base/CCGeometry.h"
#include "../2d/event_dispatcher/CCEventTouch.h"

NS_CC_BEGIN

enum class ResolutionPolicy
{
    // The entire application is visible in the specified area without trying to preserve the original aspect ratio.
    // Distortion can occur, and the application may appear stretched or compressed.
    EXACT_FIT,
    // The entire application fills the specified area, without distortion but possibly with some cropping,
    // while maintaining the original aspect ratio of the application.
    NO_BORDER,
    // The entire application is visible in the specified area without distortion while maintaining the original
    // aspect ratio of the application. Borders can appear on two sides of the application.
    SHOW_ALL,
    // The application takes the height of the design resolution size and modifies the width of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    FIXED_HEIGHT,
    // The application takes the width of the design resolution size and modifies the height of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    FIXED_WIDTH,

    UNKNOWN,
};

/**
 * @addtogroup platform
 * @{
 */

class CC_DLL EGLView 
{
	protected :

		/**
		 * @js ctor
		 */
		EGLView ( KDvoid );

	public :

		/**
		 *	@brief    get the shared main open gl window
		 */
		static EGLView*		getInstance ( KDvoid );

		KDbool				isOpenGLReady ( KDvoid );

		/** Open or close IME keyboard , subclass must implement this method. */
		KDvoid				setIMEKeyboardState ( KDbool bOpen );

		/**
		 *	Get the frame size of EGL view.
		 *	In general, it returns the screen size since the EGL view is a fullscreen view.
		 */
		const Size&			getFrameSize ( KDvoid ) const;

		/**
		 *	Set the frame size of EGL view.
		 */
		KDvoid				setFrameSize ( KDfloat fWidth, KDfloat fHeight );

		/**
		 *	Get the visible area size of opengl viewport.
		 */
		Size				getVisibleSize ( KDvoid ) const;

		/**
		 *	Get the visible origin point of opengl viewport.
		 */
		Point				getVisibleOrigin ( KDvoid ) const;

		/**
		 *	Set the design resolution size.
		 *	@param width Design resolution width.
		 *	@param height Design resolution height.
		 *	@param resolutionPolicy The resolution policy desired, you may choose:
		 *					[1] EXACT_FIT Fill screen by stretch-to-fit: if the design resolution ratio of width to height is different from the screen resolution ratio, your game view will be stretched.
		 *					[2] NO_BORDER Full screen without black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two areas of your game view will be cut.
		 *					[3] SHOW_ALL  Full screen with black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two black borders will be shown.
		 */
		KDvoid				setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight );

		KDvoid				setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight, ResolutionPolicy eResolutionPolicy );

		/** 
		 *	Get design resolution size.
		 *  Default resolution size is the same as 'getFrameSize'.
		 */
		const Size&			getDesignResolutionSize ( KDvoid ) const;

		/**
		 *	Set opengl view port rectangle with points.
		 */
		KDvoid				setViewPortInPoints ( KDfloat x, KDfloat y, KDfloat w, KDfloat h );

		/**
		 *	Set Scissor rectangle with points.
		 */
		KDvoid				setScissorInPoints ( KDfloat x, KDfloat y, KDfloat w, KDfloat h );

		/**
		 *	Get whether GL_SCISSOR_TEST is enable
		 */
		KDbool				isScissorEnabled ( KDvoid );

		/**
		 *	Get the current scissor rectangle
		 */
		Rect				getScissorRect ( KDvoid );

		KDvoid				setViewName ( const KDchar* szViewName );

		const KDchar*		getViewName ( KDvoid );

		/** Touch events are handled by default; if you want to customize your handlers, please override these functions: */
		KDvoid				handleTouchesBegin ( const KDEventTouches& tTouches );
		KDvoid				handleTouchesMoved ( const KDEventTouches& tTouches );
		KDvoid				handleTouchesEnded ( const KDEventTouches& tTouches );
		KDvoid				handleTouchesCancelled ( const KDEventTouches& tTouches );

		/**
		 *	Get the opengl view port rectangle.
		 */
		const Rect&			getViewPortRect ( KDvoid ) const;

		/**
		 *	Get scale factor of the horizontal direction.
		 */
		KDfloat				getScaleX ( KDvoid ) const;

		/**
		 *	Get scale factor of the vertical direction.
		 */
		KDfloat				getScaleY ( KDvoid ) const;


		KDfloat				getFrameZoomFactor ( KDvoid );

		/** 
		 *	Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
		 */
		KDvoid				setFrameZoomFactor ( KDfloat fZoomFactor );

	protected :

		KDvoid				handleTouchesOfEndOrCancel ( EventTouch::EventCode eEventCode, const KDEventTouches& tTouches );

	private :

		// real screen size
		Size				m_tScreenSize;
		// resolution size, it is the size appropriate for the app resources.
		Size				m_tDesignResolutionSize;
		// the view port size
		Rect				m_tViewPortRect;
		// the view name
		KDchar				m_szViewName[50];

		KDfloat				m_fScaleX;
		KDfloat				m_fScaleY;
		ResolutionPolicy	m_eResolutionPolicy;

		int _frameBufferSize[2];
		KDfloat				m_fFrameZoomFactor;
};

NS_CC_END

//	end of platform group
/// @}

#endif    // __CCEGLView_h__
