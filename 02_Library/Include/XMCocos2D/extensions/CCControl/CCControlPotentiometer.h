/* --------------------------------------------------------------------------
 *
 *      File            CCControlPotentiometer.h
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

#ifndef __CCControlPotentiometer_h__
#define __CCControlPotentiometer_h__

#include "CCControl.h"
#include "../../misc_nodes/CCProgressTimer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

/** @class CCControlPotentiometer Potentiometer control for XMCocos2D. */
class CCControlPotentiometer : public CCControl
{
	public :
				 CCControlPotentiometer ( KDvoid );
		virtual ~CCControlPotentiometer ( KDvoid );

		/** 
		 *	Creates potentiometer with a track filename and a progress filename.
		 */
		static CCControlPotentiometer*		create ( const KDchar* szBackgroundFile, const KDchar* szProgressFile, const KDchar* szThumbFile );

		static CCControlPotentiometer*		create ( CCSprite* pBackgroundSprite, CCProgressTimer* pProgressTimer, CCSprite* pThumbSprite );
	public :

		/** 
		 *	Initializes a potentiometer with a track sprite and a progress bar.
		 *
		 *	@param trackSprite CCSprite, that is used as a background.
		 *	@param progressSprite CCProgressTimer, that is used as a progress bar.
		 */
		KDbool				initWithTrackSprite_ProgressTimer_ThumbSprite ( CCSprite* pTrackSprite, CCProgressTimer* pProgressTimer, CCSprite* pThumbSprite );
		
		KDvoid				setValue ( KDfloat value );
		KDfloat				getValue ( KDvoid );
	     
		KDvoid				setMinimumValue ( KDfloat fMinimumValue );
		KDfloat				getMinimumValue ( KDvoid );

		KDvoid				setMaximumValue ( KDfloat fMaximumValue );
		KDfloat				getMaximumValue ( KDvoid );

		KDvoid				setEnabled ( KDbool bEnabled );

		virtual KDbool		isTouchInside ( CCTouch* pTouch );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

	protected :

		CC_SYNTHESIZE_RETAIN ( CCSprite*		, m_pThumbSprite	 , ThumbSprite		);
		CC_SYNTHESIZE_RETAIN ( CCProgressTimer*	, m_pProgressTimer	 , ProgressTimer	);
		CC_SYNTHESIZE        ( CCPoint			, m_tPreviousLocation, PreviousLocation );
		
	protected :

		/** Factorize the event dispath into these methods. */
		KDvoid				potentiometerBegan ( CCPoint tLocation );
		KDvoid				potentiometerMoved ( CCPoint tLocation );
		KDvoid				potentiometerEnded ( CCPoint tLocation );

		/** Returns the distance between the point1 and point2. */
		KDfloat				distanceBetweenPointAndPoint ( CCPoint tPoint1, CCPoint tPoint2 );

		/** Returns the angle in degree between line1 and line2. */
		KDfloat				angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint ( CCPoint tBeginLineA, CCPoint tEndLineA, CCPoint tBeginLineB, CCPoint tEndLineB );

	protected :

		/** Contains the receiver¡¯s current value. */
		KDfloat				m_fValue; 

		/**
		 *	Contains the minimum value of the receiver. 
		 *	The default value of this property is 0.0. 
		 */
		KDfloat				m_fMinimumValue;
		
		/** 
		 *	Contains the maximum value of the receiver. 
		 *	The default value of this property is 1.0. 
		 */
		KDfloat				m_fMaximumValue;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCControlPotentiometer_h__
