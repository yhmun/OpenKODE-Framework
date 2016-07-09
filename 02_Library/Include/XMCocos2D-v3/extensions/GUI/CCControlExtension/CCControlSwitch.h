/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlSwitch.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Yannick Loriot.
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

#ifndef __CCControlSwitch_h__
#define __CCControlSwitch_h__

#include "CCControl.h"

namespace cocos2d { class Sprite; }
namespace cocos2d { class LabelTTF; }

NS_CC_EXT_BEGIN

class ControlSwitchSprite;

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

/** @class ControlSwitch Switch control for Cocos2D. */
class ControlSwitch : public Control
{
	public :

		/** Creates a switch with a mask sprite, on/off sprites for on/off states, a thumb sprite and an on/off labels. */
		static ControlSwitch*	create ( Sprite* pMaskSprite, Sprite* pOnSprite, Sprite* pOffSprite, Sprite* pThumbSprite, LabelTTF* pOnLabel, LabelTTF* pOffLabel );

		/** Creates a switch with a mask sprite, on/off sprites for on/off states and a thumb sprite. */
		static ControlSwitch*	create ( Sprite* pMaskSprite, Sprite* pOnSprite, Sprite* pOffSprite, Sprite* pThumbSprite );
		
		/**
		 *	@js ctor
		 */
		ControlSwitch ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlSwitch ( KDvoid );

		/** Initializes a switch with a mask sprite, on/off sprites for on/off states and a thumb sprite. */
		KDbool					initWithMaskSprite ( Sprite* pMaskSprite, Sprite* pOnSprite, Sprite* pOffSprite, Sprite* pThumbSprite );

		/** Initializes a switch with a mask sprite, on/off sprites for on/off states, a thumb sprite and an on/off labels. */
		KDbool					initWithMaskSprite ( Sprite* pMaskSprite, Sprite* pOnSprite, Sprite* pOffSprite, Sprite* pThumbSprite, LabelTTF* pOnLabel, LabelTTF* pOffLabel );

		/**
		 *	Set the state of the switch to On or Off, optionally animating the transition.
		 *
		 *	@param isOn YES if the switch should be turned to the On position; NO if it 
		 *	should be turned to the Off position. If the switch is already in the 
		 *	designated position, nothing happens.
		 *	@param animated YES to animate the "flipping" of the switch; otherwise NO.
		 */
		KDvoid					setOn ( KDbool bIsOn, KDbool bAnimated );
		KDvoid					setOn ( KDbool bIsOn );
		KDbool					 isOn ( KDvoid ) const { return m_bOn; }

		KDbool					hasMoved ( KDvoid ) const { return m_bMoved; }
		virtual KDvoid			setEnabled ( KDbool bEnabled );

		Point					locationFromTouch ( Touch* pTouch );

		// Overrides
		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchCancelled ( Touch* pTouch, Event* pEvent ) override;

	protected :

		/** Sprite which represents the view. */
		ControlSwitchSprite*	m_pSwitchSprite;
		KDfloat					m_fInitialTouchXPosition;
    
		KDbool					m_bMoved;
		/** A Boolean value that determines the off/on state of the switch. */
		KDbool					m_bOn;
};

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlSwitch_h__

