/* --------------------------------------------------------------------------
 *
 *      File            CCControlStepper.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Yannick Loriot
 *
 *         http://www.cocos2d-x.org   
 *         http://yannickloriot.com
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __CCControlStepper_h__
#define __CCControlStepper_h__

#include "CCControl.h"
#include "../../label_nodes/CCLabelTTF.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

typedef enum
{
    kCCControlStepperPartMinus	,
    kCCControlStepperPartPlus	,
    kCCControlStepperPartNone	,
} CCControlStepperPart;

class CCControlStepper : public CCControl
{
	public :
				 CCControlStepper ( KDvoid );
		virtual ~CCControlStepper ( KDvoid );

		static   CCControlStepper*		create ( CCSprite* pMinusSprite, CCSprite* pPlusSprite );

	public :

		virtual KDbool		initWithMinusSpriteAndPlusSprite ( CCSprite* pMinusSprite, CCSprite* pPlusSprite );

		virtual KDvoid		setWraps ( KDbool bWraps );

		virtual KDvoid		setMinimumValue ( KDdouble fMinimumValue );
		virtual KDvoid		setMaximumValue ( KDdouble fMaximumValue );

		virtual KDvoid		setValue ( KDdouble dValue);
		virtual KDdouble	getValue ( KDvoid );

		virtual KDvoid		setStepValue ( KDdouble dStepValue );
		virtual KDvoid		setValueWithSendingEvent ( KDdouble dValue, KDbool bSend );

		virtual KDbool		isContinuous ( KDvoid );

		KDvoid				update ( KDfloat fDelta );

		// events
		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

	protected :

		// Weak links to children
		CC_SYNTHESIZE_RETAIN ( CCSprite*  , m_pMinusSprite, MinusSprite );
		CC_SYNTHESIZE_RETAIN ( CCSprite*  , m_pPlusSprite , PlusSprite  );
		CC_SYNTHESIZE_RETAIN ( CCLabelTTF*, m_pMinusLabel , MinusLabel  );
		CC_SYNTHESIZE_RETAIN ( CCLabelTTF*, m_pPlusLabel  , PlusLabel   );
	    
		/** Update the layout of the stepper with the given touch location. */
		KDvoid				updateLayoutUsingTouchLocation ( CCPoint tLocation );

		/** Set the numeric value of the stepper. If send is true, the CCControlEventValueChanged is sent. */
		KDvoid				setValue ( KDdouble dValue, KDbool bSend );

		/** Start the autorepeat increment/decrement. */
		KDvoid				startAutorepeat ( KDvoid );

		/** Stop the autorepeat. */
		KDvoid				stopAutorepeat ( KDvoid );

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
		CCControlStepperPart	m_eTouchedPart;
		KDint					m_nAutorepeatCount;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCControlStepper_h__
