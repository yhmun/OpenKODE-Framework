/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlHuePicker.h
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

#ifndef __CCControlHuePicker_h__
#define __CCControlHuePicker_h__

#include "CCControl.h"
#include "CCInvocation.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

class ControlHuePicker : public Control
{
	public :

		static ControlHuePicker*	create ( Node* pTarget, Point tPos );

		/**
		 *	@js ctor
		 */
		ControlHuePicker ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlHuePicker ( KDvoid );

		virtual KDbool				initWithTargetAndPos ( Node* pTarget, Point tPos );

		virtual KDvoid				setEnabled ( KDbool bEnabled );

		// overrides
		virtual KDbool				onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid				onTouchMoved ( Touch* pTouch, Event* pEvent ) override;

	protected :

		KDvoid						updateSliderPosition ( Point tLocation );
		KDbool						checkSliderPosition ( Point tLocation );

		// maunally put in the setters
		CC_SYNTHESIZE_READONLY	( KDfloat, m_fHue, Hue );

		virtual KDvoid				setHue ( KDfloat fVal );

		CC_SYNTHESIZE_READONLY	( KDfloat, m_fHuePercentage, HuePercentage );

		virtual KDvoid				setHuePercentage ( KDfloat fVal );

		// not sure if these need to be there actually. I suppose someone might want to access the sprite?
		CC_SYNTHESIZE_RETAIN	( Sprite*, m_pBackground, Background );
		CC_SYNTHESIZE_RETAIN	( Sprite*, m_pSlider, Slider );
		CC_SYNTHESIZE_READONLY	( Point, m_tStartPos, StartPos );
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlHuePicker_h__