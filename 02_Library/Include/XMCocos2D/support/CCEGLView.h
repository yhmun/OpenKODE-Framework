/* -----------------------------------------------------------------------------------
 *
 *      File            CCEGLView.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCEGLView_h__
#define __CCEGLView_h__

#include "../ccTypes.h"

NS_CC_BEGIN

enum ResolutionPolicy
{
    // The entire application is visible in the specified area without trying to preserve the original aspect ratio.
    // Distortion can occur, and the application may appear stretched or compressed.
    kResolutionExactFit,
    // The entire application fills the specified area, without distortion but possibly with some cropping,
    // while maintaining the original aspect ratio of the application.
    kResolutionNoBorder,
    // The entire application is visible in the specified area without distortion while maintaining the original
    // aspect ratio of the application. Borders can appear on two sides of the application.
    kResolutionShowAll,
    // The application takes the height of the design resolution size and modifies the width of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedHeight,
    // The application takes the width of the design resolution size and modifies the height of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedWidth,

    kResolutionUnKnown,
};

#define CC_MAX_TOUCHES		KD_TOUCH_MAX

/**
 * @addtogroup support
 * @{
 */

class CCEGLView
{
	protected :
		
		CCEGLView ( KDvoid );

	public :

		static  CCEGLView*		sharedOpenGLView ( KDvoid );

	public :

		/** Open or close IME keyboard , subclass must implement this method. */
		KDvoid					setIMEKeyboardState ( KDbool bOpen );

		const CCSize&			getDeviceSize ( KDvoid ) const;

		/**
		 *	Get the frame size of EGL view.
		 *	In general, it returns the screen size since the EGL view is a fullscreen view.
		 */
		const CCSize&			getFrameSize ( KDvoid ) const;

		/**
		 *	Set the frame size of EGL view.
		 */
		KDvoid					setFrameSize ( KDfloat fWidth, KDfloat fHeight );

		/** 
		 *	Get the visible area size of opengl viewport. 
		 */
		CCSize					getVisibleSize ( KDvoid ) const;

		/** 
		 *	Get the visible origin point of opengl viewport. 
		 */
		CCPoint					getVisibleOrigin ( KDvoid ) const;

		CCRect					getVisibleRect ( KDvoid ) const;

		/**
		 *	Set the design resolution size.
		 *	@param width Design resolution width.
		 *	@param height Design resolution height.
		 *	@param resolutionPolicy The resolution policy desired, you may choose:
		 *                         [1] kResolutionExactFit Fill screen by stretch-to-fit: if the design resolution ratio of width to height is different from the screen resolution ratio, your game view will be stretched.
		 *                         [2] kResolutionNoBorder Full screen without black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two areas of your game view will be cut.
		 *                         [3] kResolutionShowAll  Full screen with black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two black borders will be shown.
		 */
		KDvoid					setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight, ResolutionPolicy eResolutionPolicy );
		
		KDvoid					setDesignResolutionSize ( const CCSize& tSize, ResolutionPolicy eResolutionPolicy );

		KDvoid					setDesignResolutionSize ( const CCSize& tSize );

		/** 
		 *	Get design resolution size.
		 *  Default resolution size is the same as 'getFrameSize'.
		 */
		const CCSize&			getDesignResolutionSize   ( KDvoid ) const;
		ResolutionPolicy		getDesignResolutionPolicy ( KDvoid );

		/**	Set opengl view port rectangle with points. */
		CCRect					setViewPortInPoints ( KDfloat x , KDfloat y , KDfloat w , KDfloat h );

		/**	Set Scissor rectangle with points. */
		CCRect					setScissorInPoints ( KDfloat x , KDfloat y , KDfloat w , KDfloat h );

		/**
		 * Get whether GL_SCISSOR_TEST is enable
		 */
		KDbool					isScissorEnabled ( KDvoid );

		/**
		 * Get the current scissor rectangle
		 */
		CCRect					getScissorRect ( KDvoid );


		/** Get the opengl view port rectangle. */
		const CCRect&			getViewPortRect ( KDvoid ) const;

		/** Get scale factor of the horizontal direction. */
		KDfloat					getScaleX ( KDvoid ) const;

		/** Get scale factor of the vertical direction. */
		KDfloat					getScaleY ( KDvoid ) const;


		/** Touch events are handled by default */
		KDvoid					handleTouchesBegin ( const KDEventTouches& tTouches );
		KDvoid					handleTouchesMoved ( const KDEventTouches& tTouches );
		KDvoid					handleTouchesEnded ( const KDEventTouches& tTouches );

	protected :

		CCSize					m_tScreenSize;
		CCSize					m_tDesignResolutionSize;	// resolution size, it is the size appropriate for the app resources.		
		CCRect					m_tViewPortRect;			// the view port size

		KDfloat					m_fScaleX;
		KDfloat					m_fScaleY;
		ResolutionPolicy		m_eResolutionPolicy;
};

// end of support group
/// @}

NS_CC_END

#endif	// __CCEGLView_h__
