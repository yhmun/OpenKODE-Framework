/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionEase.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2009 Jason Booth
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

#ifndef __CCActionEase_h__
#define __CCActionEase_h__

#include "CCActionInterval.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

class CCObject;
class CCZone;

/** 
 *	@brief	Base class for Easing actions
 *	@ingroup Actions
 */
class CCActionEase : public CCActionInterval
{
	public :

		virtual ~CCActionEase ( KDvoid );

		/** creates the action */
		static   CCActionEase*		create ( CCActionInterval* pAction );

	public :

		/** initializes the action */
		virtual KDbool		initWithAction ( CCActionInterval* pAction );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );    

		virtual KDvoid		stop ( KDvoid );

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );	

		virtual CCActionInterval*	getInnerAction ( KDvoid );

	protected :

		/** The inner action */
		CCActionInterval*			m_pInner;
};

/** 
 *	@brief	Base class for Easing actions with rate parameters
 *	@ingroup Actions
 */
class CCEaseRateAction : public CCActionEase
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static  CCEaseRateAction*	create ( CCActionInterval* pAction, KDfloat fRate );

	public :

		/** Initializes the action with the inner action and the rate parameter */
		virtual KDbool		initWithAction ( CCActionInterval* pAction, KDfloat fRate );

		/** set rate value for the actions */
		inline  KDvoid		setRate ( KDfloat fRate ) { m_fRate = fRate; }

		/** get rate value for the actions */
		inline  KDfloat		getRate ( KDvoid ) { return m_fRate; }

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat				m_fRate;
};

/** 
 *	@brief	CCEaseIn action with a rate
 *	@ingroup Actions
 */
class CCEaseIn : public CCEaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static  CCEaseIn*	create ( CCActionInterval* pAction, KDfloat fRate );

	public :

		virtual KDvoid		update ( KDfloat fTime );	

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	CCEaseOut action with a rate
 *	@ingroup Actions
 */
class CCEaseOut : public CCEaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static  CCEaseOut*	create ( CCActionInterval* pAction, KDfloat fRate );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	CCEaseInOut action with a rate
 *	@ingroup Actions
 */
class CCEaseInOut : public CCEaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static  CCEaseInOut*	create ( CCActionInterval* pAction, KDfloat fRate );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	CCEase Exponential In
 *	@ingroup Actions
 */
class CCEaseExponentialIn : public CCActionEase
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static  CCEaseExponentialIn*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );    
};

/** 
 *	@brief	Ease Exponential Out
 *	@ingroup Actions
 */
class CCEaseExponentialOut : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseExponentialOut*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	Ease Exponential InOut
 *	@ingroup Actions
 */
class CCEaseExponentialInOut : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseExponentialInOut*		create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	Ease Sine In
 *	@ingroup Actions
 */
class CCEaseSineIn : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseSineIn*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	Ease Sine Out
 *	@ingroup Actions
 */
class CCEaseSineOut : public CCActionEase
{
	public :
		
		/** creates the action */
		static  CCEaseSineOut*  create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	Ease Sine InOut
 *	@ingroup Actions
 */
class CCEaseSineInOut : public CCActionEase
{
	public :

		/** creates the action */
		static CCEaseSineInOut*		create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	Ease Elastic abstract class
 *	@since	v0.8.2
 *	@ingroup Actions
 */
class CCEaseElastic : public CCActionEase
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static  CCEaseElastic*	create ( CCActionInterval* pAction );
		static  CCEaseElastic*	create ( CCActionInterval* pAction, KDfloat fPeriod );

	public:

		/** Initializes the action with the inner action and the period in radians (default is 0.3) */
		virtual KDbool		initWithAction ( CCActionInterval* pAction, KDfloat fPeriod = 0.3f );

		/** get period of the wave in radians. default is 0.3 */
		inline  KDfloat		getPeriod ( KDvoid ) { return m_fPeriod; }

		/** set period of the wave in radians. */
		inline  KDvoid		setPeriod ( KDfloat fPeriod ) { m_fPeriod = fPeriod; }

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

	protected :

		KDfloat				m_fPeriod;
};

/** 
 *	@brief	 Ease Elastic In action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseElasticIn : public CCEaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static  CCEaseElasticIn*	create ( CCActionInterval* pAction );
		static  CCEaseElasticIn*	create ( CCActionInterval* pAction, KDfloat fPeriod );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 Ease Elastic Out action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseElasticOut : public CCEaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static  CCEaseElasticOut*	create ( CCActionInterval* pAction );
		static  CCEaseElasticOut*	create ( CCActionInterval* pAction, KDfloat fPeriod );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 Ease Elastic InOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseElasticInOut : public CCEaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static  CCEaseElasticInOut*		create ( CCActionInterval* pAction );
		static  CCEaseElasticInOut*		create ( CCActionInterval* pAction, KDfloat fPeriod = 0.3f );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	CCEaseBounce abstract class.
 *	@since	v0.8.2
 *	@ingroup Actions
 */
class CCEaseBounce : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseBounce*	create ( CCActionInterval* pAction );

	public :

		KDfloat				bounceTime ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 CCEaseBounceIn action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBounceIn : public CCEaseBounce
{
	public :

		/** creates the action */
		static  CCEaseBounceIn*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 EaseBounceOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBounceOut : public CCEaseBounce
{
	public :

		/** creates the action */
		static  CCEaseBounceOut*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 CCEaseBounceInOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBounceInOut : public CCEaseBounce
{
	public :

		/** creates the action */
		static CCEaseBounceInOut*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 CCEaseBackIn action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBackIn : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseBackIn*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 CCEaseBackOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBackOut : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseBackOut*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief	 CCEaseBackInOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since	 v0.8.2
 *	@ingroup Actions
 */
class CCEaseBackInOut : public CCActionEase
{
	public :

		/** creates the action */
		static  CCEaseBackInOut*	create ( CCActionInterval* pAction );

	public :

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionEase_h__
