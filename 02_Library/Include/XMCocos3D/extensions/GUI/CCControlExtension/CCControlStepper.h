/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlStepper.h
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

#ifndef __CCControlStepper_h__
#define __CCControlStepper_h__

#include "CCControl.h"
#include "../../../2d/label_nodes/CCLabelTTF.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

class ControlStepper : public Control
{
	public :

		enum class Part
		{
			MINUS,
			PLUS,
			NONE
		};
    
		static ControlStepper*	create ( Sprite* pMinusSprite, Sprite* pPlusSprite );

		/**
		 *	@js ctor
		 */
		ControlStepper ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlStepper ( KDvoid );

		KDbool					initWithMinusSpriteAndPlusSprite ( Sprite* pMinusSprite, Sprite* pPlusSprite );

		virtual KDvoid			setWraps ( KDbool bWraps );
		virtual KDvoid			setMinimumValue ( KDdouble dMinimumValue );
		virtual KDvoid			setMaximumValue ( KDdouble dMaximumValue );
		virtual KDvoid			setValue ( KDdouble dValue );

		virtual KDdouble		getValue ( KDvoid ) const;
		virtual KDvoid			setStepValue ( KDdouble dStepValue );
		/** Set the numeric value of the stepper. If send is true, the Control::EventType::VALUE_CHANGED is sent. */
		virtual KDvoid			setValueWithSendingEvent ( KDdouble dValue, KDbool bSend );
		virtual KDbool			isContinuous ( KDvoid ) const;

		// Overrides
		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) override;
		KDvoid					update ( KDfloat dt );

		/** Update the layout of the stepper with the given touch location. */
		KDvoid					updateLayoutUsingTouchLocation ( Point tLocation );

		/** Start the autorepeat increment/decrement. */
		KDvoid					startAutorepeat ( KDvoid );

		/** Stop the autorepeat. */
		KDvoid					stopAutorepeat ( KDvoid );

	protected :

		/** The numeric value of the stepper. */
		KDdouble				m_dValue;
		/** The continuous vs. noncontinuous state of the stepper. */
		KDbool					m_bContinuous;
		/** The automatic vs. nonautomatic repeat state of the stepper. */
		KDbool					m_bAutorepeat;
		/** The wrap vs. no-wrap state of the stepper. */
		KDbool					m_bWraps;
		/** The lowest possible numeric value for the stepper. */
		KDdouble				m_dMinimumValue;
		/** The highest possible numeric value for the stepper. */
		KDdouble				m_dMaximumValue;
		/** The step, or increment, value for the stepper. */
		KDdouble				m_dStepValue;
		KDbool					m_bTouchInsideFlag;
		Part                    m_tTouchedPart;
		KDint					m_nAutorepeatCount;

		// Weak links to children
		CC_SYNTHESIZE_RETAIN ( Sprite*  , m_pMinusSprite, MinusSprite )
		CC_SYNTHESIZE_RETAIN ( Sprite*  , m_pPlusSprite , PlusSprite  )
		CC_SYNTHESIZE_RETAIN ( LabelTTF*, m_pMinusLabel , MinusLabel  )
		CC_SYNTHESIZE_RETAIN ( LabelTTF*, m_pPlusLabel  , PlusLabel   )
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlStepper_h__
