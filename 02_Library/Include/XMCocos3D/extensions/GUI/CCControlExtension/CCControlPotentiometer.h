/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlPotentiometer.h
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

#ifndef __CCControlPotentiometer_h__
#define __CCControlPotentiometer_h__

#include "CCControl.h"
#include "../../../2d/misc_nodes/CCProgressTimer.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

/** @class ControlPotentiometer Potentiometer control for Cocos2D. */
class ControlPotentiometer : public Control
{
	public :

		/**
		 *	Creates potentiometer with a track filename and a progress filename.
		 */
		static ControlPotentiometer*	create(const char* backgroundFile, const char* progressFile, const char* thumbFile);
    
		/**
		 *	@js ctor
		 */
		ControlPotentiometer ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlPotentiometer ( KDvoid );

		/** 
		 *	Initializes a potentiometer with a track sprite and a progress bar.
		 *
		 *	@param trackSprite   Sprite, that is used as a background.
		 *	@param progressTimer ProgressTimer, that is used as a progress bar.
		 */
		KDbool					initWithTrackSprite_ProgressTimer_ThumbSprite ( Sprite* pTrackSprite, ProgressTimer* pProgressTimer, Sprite* pThumbSprite );

		KDvoid					setValue ( KDfloat fValue );
		KDfloat					getValue ( KDvoid );
     
		KDvoid					setMinimumValue ( KDfloat fMinimumValue );
		KDfloat					getMinimumValue ( KDvoid );

		KDvoid					setMaximumValue ( KDfloat fMaximumValue );
		KDfloat					getMaximumValue ( KDvoid );

		// Overrides
		virtual KDbool			isTouchInside ( Touch* pTouch ) override;
		KDvoid					setEnabled ( KDbool bEnabled ) override;

		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) override;

		/** Factorize the event dispath into these methods. */
		KDvoid					potentiometerBegan ( Point tLocation );
		KDvoid					potentiometerMoved ( Point tLocation );
		KDvoid					potentiometerEnded ( Point tLocation );

		/** Returns the distance between the point1 and point2. */
		KDfloat					distanceBetweenPointAndPoint ( Point tPoint1, Point tPoint2 );

		/** Returns the angle in degree between line1 and line2. */
		KDfloat					angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint ( Point tBeginLineA, Point tEndLineA, Point tBeginLineB, Point tEndLineB );

	protected :

		/** Contains the receiver’s current value. */
		KDfloat					m_fValue;
		/** Contains the minimum value of the receiver.
		 * The default value of this property is 0.0. */
		KDfloat					m_fMinimumValue;
		/** Contains the maximum value of the receiver.
		 * The default value of this property is 1.0. */
		KDfloat					m_fMaximumValue;

		CC_SYNTHESIZE_RETAIN ( Sprite*, m_pThumbSprite, ThumbSprite )
		CC_SYNTHESIZE_RETAIN ( ProgressTimer*, m_pProgressTimer, ProgressTimer )
		CC_SYNTHESIZE ( Point, m_tPreviousLocation, PreviousLocation )
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlPotentiometer_h__
