/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionProgressTimer.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionProgressTimer_h__
#define __CCActionProgressTimer_h__

#include "CCActionInterval.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/**
 *	@brief Progress to percentage
 *	@since v0.99.1
*/
class CC_DLL ProgressTo : public ActionInterval
{
	public :

		/** Creates and initializes with a duration and a percent */
		static ProgressTo*			create ( KDfloat fDuration, KDfloat fPercent );

		/** Initializes with a duration and a percent */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fPercent );

		//
		// Overrides
		//
		virtual ProgressTo*			clone ( KDvoid ) const override;
		virtual ProgressTo*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDfloat						m_fTo;
		KDfloat						m_fFrom;
};

/**
 *	@brief Progress from a percentage to another percentage
 *	@since v0.99.1
 */
class CC_DLL ProgressFromTo : public ActionInterval
{
	public :

		/** Creates and initializes the action with a duration, a "from" percentage and a "to" percentage */
		static ProgressFromTo*		create ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage );

		/** Initializes the action with a duration, a "from" percentage and a "to" percentage */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage );

		//
		// Overrides
		//
		virtual ProgressFromTo*		clone ( KDvoid ) const override;
		virtual ProgressFromTo*		reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDfloat						m_fTo;
		KDfloat						m_fFrom;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionProgressTimer_h__
