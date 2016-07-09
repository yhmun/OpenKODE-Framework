/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTween.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      lhunath (Maarten Billemont)
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionTween_h__
#define __CCActionTween_h__

#include "CCActionInterval.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

class CC_DLL ActionTweenDelegate
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ActionTweenDelegate ( KDvoid ) { }

		virtual KDvoid			updateTweenAction ( KDfloat fValue, const KDchar* pKey ) = 0;
};

/** 
 *	ActionTween
 *
 *	ActionTween is an action that lets you update any property of an object.
 *	For example, if you want to modify the "width" property of a target from 200 to 300 in 2 seconds, then:
 *
 *	@code
 *	    auto modifyWidth = ActionTween::create(2, "width", 200, 300);
 *	    target->runAction(modifyWidth);
 *	@endcode
 *
 *	Another example: ScaleTo action could be rewritten using PropertyAction:
 *
 *	@code
 *	    // scaleA and scaleB are equivalents
 *	    auto scaleA = ScaleTo::create(2, 3);                 // (duration, to)
 *	    auto scaleB = ActionTween::create(2, "scale", 1, 3); // (duration, key, from, to)
 *	@endcode
 *	
 *	@since v0.99.2
 */
class CC_DLL ActionTween : public ActionInterval
{
	public :

		/** creates an initializes the action with the property name (key), and the from and to parameters. */
		static ActionTween*		create ( KDfloat fDuration, const KDchar* pKey, KDfloat fFrom, KDfloat fTo );
		
		/** initializes the action with the property name (key), and the from and to parameters. */
		KDbool					initWithDuration ( KDfloat fDuration, const KDchar* pKey, KDfloat fFrom, KDfloat fTo );

		// Overrides
		KDvoid					startWithTarget ( Node* pTarget ) override;
		KDvoid					update ( KDfloat dt ) override;
		ActionTween*			reverse ( KDvoid ) const override;
		ActionTween*			clone ( KDvoid ) const override;

	protected :

		std::string				m_sKey;
		KDfloat					m_fFrom, m_fTo;
		KDfloat					m_fDelta;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionTween_h__


