/* --------------------------------------------------------------------------
 *
 *      File            CCControlHuePicker.h
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

#ifndef __CCControlHuePicker_h__
#define __CCControlHuePicker_h__

#include "CCControl.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

class CCControlHuePicker : public CCControl
{
	public :	
				 CCControlHuePicker ( KDvoid );
		virtual ~CCControlHuePicker ( KDvoid );

		static CCControlHuePicker*		create ( CCNode* pTarget, CCPoint tPos );

	public :

		virtual KDbool		initWithTargetAndPos ( CCNode* pTarget, CCPoint tPos );

		virtual KDvoid		setEnabled ( KDbool bEnabled );

		// maunally put in the setters
		CC_SYNTHESIZE_READONLY ( KDfloat, m_fHue, Hue );
		virtual KDvoid		setHue ( KDfloat fValue );

		CC_SYNTHESIZE_READONLY ( KDfloat, m_fHuePercentage, HuePercentage );
		virtual KDvoid		setHuePercentage ( KDfloat fValue );

		// not sure if these need to be there actually. I suppose someone might want to access the sprite?
		CC_SYNTHESIZE_RETAIN   ( CCSprite*, m_pBackground, Background );
		CC_SYNTHESIZE_RETAIN   ( CCSprite*, m_pSlider    , Slider );
		CC_SYNTHESIZE_READONLY ( CCPoint  , m_tStartPos  , StartPos );

	protected :

		KDvoid				updateSliderPosition ( CCPoint tLocation );
		KDbool				checkSliderPosition  ( CCPoint tLocation );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlHuePicker_h__