/* --------------------------------------------------------------------------
 *
 *      File            CCControlSwitch.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      starfair       ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
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

#ifndef __CCControlSwitch_h__
#define __CCControlSwitch_h__

#include "CCControl.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

class CCControlSwitchSprite;

/** class CCControlSwitch Switch control for Cocos2D. */
class CCControlSwitch : public CCControl
{
	public :
				 CCControlSwitch ( KDvoid );
		virtual ~CCControlSwitch ( KDvoid );

		/** Creates a switch with a mask sprite, on/off sprites for on/off states and a thumb sprite. */
		static CCControlSwitch*		create ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite );

		/** Creates a switch with a mask sprite, on/off sprites for on/off states, a thumb sprite and an on/off labels. */
		static CCControlSwitch*		create ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel );

	public :

		/** Initializes a switch with a mask sprite, on/off sprites for on/off states and a thumb sprite. */
		virtual KDbool		initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite );

		/** Initializes a switch with a mask sprite, on/off sprites for on/off states, a thumb sprite and an on/off labels. */
		virtual KDbool		initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel );

		/**
		 * Set the state of the switch to On or Off, optionally animating the transition.
		 *
		 * @param isOn YES if the switch should be turned to the On position; NO if it 
		 * should be turned to the Off position. If the switch is already in the 
		 * designated position, nothing happens.
		 * @param animated YES to animate the "flipping" of the switch; otherwise NO.
		 */
		KDvoid				setOn ( KDbool bIsOn, KDbool bAnimated );
		KDvoid				setOn ( KDbool bIsOn );
		
		inline  KDbool		isOn ( KDvoid ) { return m_bOn; }

		inline  KDbool		hasMoved ( KDvoid ) { return m_bMoved; }
		
		virtual KDvoid		setEnabled ( KDbool bEnabled );

		CCPoint				locationFromTouch ( CCTouch* pTouch );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );		

	protected :

		/** Sprite which represents the view. */
		CCControlSwitchSprite*		m_pSwitchSprite;
		KDfloat						m_fInitialTouchXPosition;

		KDbool						m_bMoved;

		/** A Boolean value that determines the off/on state of the switch. */
		KDbool						m_bOn;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlSwitch_h__
