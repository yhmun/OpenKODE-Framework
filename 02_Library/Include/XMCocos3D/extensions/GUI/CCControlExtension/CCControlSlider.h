/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlSlider.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCControlSlider_h__
#define __CCControlSlider_h__

#include "CCControl.h"
#include "CCInvocation.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */
class ControlSlider: public Control
{
	public :

		/**
		 *	Creates slider with a background filename, a progress filename and a
		 *	thumb image filename.
		 */
		static ControlSlider*	create ( const KDchar* pBgFile, const KDchar* pProgressFile, const KDchar* pThumbFile );

		/**
		 *	Creates a slider with a given background sprite and a progress bar and a
		 *	thumb item.
		 *
		 *	@see initWithSprites
		 */
		static ControlSlider*	create ( Sprite* pBackgroundSprite, Sprite* pProgressSprite, Sprite* pThumbSprite );

		/**
		 *	@js ctor
		 */
		ControlSlider ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlSlider ( KDvoid );

		/** 
		 *	Initializes a slider with a background sprite, a progress bar and a thumb
		 *	item.
		 *
		 *	@param backgroundSprite  Sprite, that is used as a background.
		 *	@param progressSprite    Sprite, that is used as a progress bar.
		 *	@param thumbSprite       Sprite, that is used as a thumb.
		 */
		virtual KDbool			initWithSprites ( Sprite* pBackgroundSprite, Sprite* pProgressSprite, Sprite* pThumbSprite );

		virtual KDvoid			needsLayout ( KDvoid );

		virtual KDvoid			setMaximumValue ( KDfloat fVal );
		virtual KDvoid			setEnabled ( KDbool bEnabled );
		virtual KDbool			isTouchInside ( Touch* pTouch );
		Point					locationFromTouch ( Touch* pTouch );
		virtual KDvoid			setValue ( KDfloat fVal );
		virtual KDvoid			setMinimumValue ( KDfloat fVal );

	protected :

		KDvoid					sliderBegan ( Point tLocation );
		KDvoid					sliderMoved ( Point tLocation );
		KDvoid					sliderEnded ( Point tLocation );

		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent );
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent );
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent );

		/** Returns the value for the given location. */
		KDfloat					valueForLocation ( Point tLocation );

		// maunally put in the setters
		/** Contains the receiver's current value. */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fValue, Value );

		/**
		 *	Contains the minimum value of the receiver.
		 *	The default value of this property is 0.0. 
		 */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fMinimumValue, MinimumValue );

		/** 
		 *	Contains the maximum value of the receiver.
		 *	The default value of this property is 1.0. 
		 */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fMaximumValue, MaximumValue );

		CC_SYNTHESIZE ( KDfloat, m_fMinimumAllowedValue, MinimumAllowedValue );
		CC_SYNTHESIZE ( KDfloat, m_fMaximumAllowedValue, MaximumAllowedValue );

		// maybe this should be read-only
		CC_SYNTHESIZE_RETAIN ( Sprite*, m_pThumbSprite, ThumbSprite );
		CC_SYNTHESIZE_RETAIN ( Sprite*, m_pProgressSprite, ProgressSprite );
		CC_SYNTHESIZE_RETAIN ( Sprite*, m_pBackgroundSprite, BackgroundSprite );
};

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlSlider_h__
