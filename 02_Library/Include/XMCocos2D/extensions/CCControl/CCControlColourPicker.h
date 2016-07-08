/* --------------------------------------------------------------------------
 *
 *      File            CCControlColourPicker.h
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

#ifndef __CCControlColourPicker_h__
#define __CCControlColourPicker_h__

#include "CCControl.h"
#include "CCControlHuePicker.h"
#include "CCControlSaturationBrightnessPicker.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

class CCControlColourPicker: public CCControl
{		
	public :
				 CCControlColourPicker ( KDvoid );
		virtual ~CCControlColourPicker ( KDvoid );

		static   CCControlColourPicker*		create ( const KDchar* szSpriteSheet, const KDchar* szSpriteSheetImage );

	public :

		virtual KDbool		init ( const KDchar* szSpriteSheet, const KDchar* szSpriteSheetImage );

		KDvoid				hueSliderValueChanged    ( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid				colourSliderValueChanged ( CCObject* pSender, CCControlEvent uControlEvent );

		virtual KDvoid		setColor ( const ccColor3B& tColorValue );
		virtual KDvoid		setEnabled ( KDbool bEnabled );

	protected :

		KDvoid				updateControlPicker ( KDvoid );
		KDvoid				updateHueAndControlPicker ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
	
	protected :

		CC_SYNTHESIZE_RETAIN ( CCControlSaturationBrightnessPicker*, m_pColourPicker, ColourPicker );
		CC_SYNTHESIZE_RETAIN ( CCControlHuePicker*, m_pHuePicker, HuePicker );
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pBackground, Background );   

	protected :

		CCHsv				m_tHsv;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlColourPicker_h__