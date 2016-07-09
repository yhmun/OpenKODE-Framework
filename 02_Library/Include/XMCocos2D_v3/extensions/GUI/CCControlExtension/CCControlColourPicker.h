/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlColourPicker.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://creativewax.co.uk
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

#ifndef __CCControlColourPicker_h__
#define __CCControlColourPicker_h__

#include "CCControl.h"
#include "CCControlUtils.h"
#include "CCControlHuePicker.h"
#include "CCControlSaturationBrightnessPicker.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

class ControlColourPicker: public Control
{
	public :

		static ControlColourPicker*		create ( KDvoid );

		/**
		 *	@js ctor
		 */
		ControlColourPicker ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlColourPicker ( KDvoid );

		virtual KDbool					init ( KDvoid );

		virtual KDvoid					setColor ( const Color3B& tColorValue );
		virtual KDvoid					setEnabled ( KDbool bEnabled );

		KDvoid							hueSliderValueChanged ( Object* pSender, Control::EventType eControlEvent );
		KDvoid							colourSliderValueChanged ( Object* pSender, Control::EventType eControlEvent );

	protected :

		KDvoid							updateControlPicker ( KDvoid );
		KDvoid							updateHueAndControlPicker ( KDvoid );
		virtual KDbool					onTouchBegan ( Touch* pTouch, Event* pEvent );

		HSV								m_tHsv;

		CC_SYNTHESIZE_RETAIN ( ControlSaturationBrightnessPicker*, m_pColourPicker, colourPicker )
		CC_SYNTHESIZE_RETAIN ( ControlHuePicker*, m_pHuePicker, HuePicker )
		CC_SYNTHESIZE_RETAIN ( Sprite*, m_pBackground, Background )
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlColourPicker_h__