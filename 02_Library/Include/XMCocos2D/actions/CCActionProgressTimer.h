/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionProgressTimer.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Lam Pham
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCActionProgressTimer_h__
#define __CCActionProgressTimer_h__

#include "CCActionInterval.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

/**
 *	@brief	Progress to percentage
 *	@since	v0.99.1
 */
class CCProgressTo : public CCActionInterval
{
	public :

		/** Creates and initializes with a duration and a percent */
		static  CCProgressTo*	create ( KDfloat fDuration, KDfloat fPercent );

	public :

		/** Initializes with a duration and a percent */
		virtual KDbool		initWithDuration ( KDfloat fDuration, KDfloat fPercent );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );		

	protected :

		KDfloat				m_fTo;
		KDfloat				m_fFrom;
};

/**
 *	@brief	Progress from a percentage to another percentage
 *	@since	v0.99.1
 */
class CCProgressFromTo : public CCActionInterval
{
	public :

		/** Creates and initializes the action with a duration, a "from" percentage and a "to" percentage */
		static  CCProgressFromTo*	create ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage );

	public :

		/** Initializes the action with a duration, a "from" percentage and a "to" percentage */
		virtual KDbool		initWithDuration ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat				m_fTo;
		KDfloat				m_fFrom;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionProgressTimer_h__
