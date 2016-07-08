/* --------------------------------------------------------------------------
 *
 *      File            CCControlSlider.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
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

#ifndef __CCControlSlider_h__
#define __CCControlSlider_h__

#include "CCControl.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

class CCControlSlider: public CCControl
{
	public :
				 CCControlSlider ( KDvoid );
		virtual ~CCControlSlider ( KDvoid );

		/** 
		 *	Creates slider with a background filename, a progress filename and a thumb image filename.
		 */
		static CCControlSlider*		create ( const KDchar* szBackgroundFile, const KDchar* szProgressFile, const KDchar* szThumbFile );

		/** 
		 *	Creates a slider with a given background sprite and a progress bar and a thumb item.
		 *	@see initWithBackgroundSprite:progressSprite:thumbMenuItem:
		 */
		static CCControlSlider*		create ( CCSprite* pBackgroundSprite, CCSprite* pProgressSprite, CCSprite* pThumbSprite );

	public :

		/** 
		 *	Initializes a slider with a background sprite, a progress bar and a thumb item.
		 *
		 *	@param pBackgroundSprite CCSprite, that is used as a background.
		 *	@param pProgressSprite	 CCSprite, that is used as a progress bar.
		 *	@param pThumbSprite		 CCSprite, that is used as a thumb.
		 */
		virtual KDbool		initFromSprites ( CCSprite* pBackgroundSprite, CCSprite* pProgressSprite, CCSprite* pThumbSprite );		

		// maunally put in the setters

		/** Contains the receiver¡¯s current value. */
		CC_SYNTHESIZE_READONLY  ( KDfloat, m_fValue, Value );
		virtual KDvoid		setValue ( KDfloat fValue );

		/** 
		 *	Contains the minimum value of the receiver. 
		 *	The default value of this property is 0.0. 
		 */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fMinimumValue, MinimumValue );
		virtual KDvoid		setMinimumValue ( KDfloat fValue );

		/**
		 *	Contains the maximum value of the receiver. 
		 *	The default value of this property is 1.0. 
		 */
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fMaximumValue, MaximumValue );
		virtual KDvoid		setMaximumValue ( KDfloat fValue );

		virtual KDvoid		setEnabled ( KDbool bEnabled );
		virtual KDbool		isTouchInside ( CCTouch* pTouch );
		CCPoint				locationFromTouch ( CCTouch* pTouch );

		CC_SYNTHESIZE ( KDfloat, m_fMinimumAllowedValue, MinimumAllowedValue );
		CC_SYNTHESIZE ( KDfloat, m_fMaximumAllowedValue, MaximumAllowedValue );

		// maybe this should be read-only
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pThumbSprite     , ThumbSprite );
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pProgressSprite  , ProgressSprite );
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pBackgroundSprite, BackgroundSprite );

		virtual KDvoid		needsLayout ( KDvoid );

	protected :

		KDvoid				sliderBegan ( CCPoint tLocation );
		KDvoid				sliderMoved ( CCPoint tLocation );
		KDvoid				sliderEnded ( CCPoint tLocation );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		/** Returns the value for the given location. */
		KDfloat				valueForLocation ( CCPoint tLocation );
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlSlider_h__