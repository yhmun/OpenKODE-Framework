/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionEase.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionEase_h__
#define __CCActionEase_h__

#include "CCActionInterval.h"

NS_CC_BEGIN

class Object;

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief Base class for Easing actions
 *	@ingroup Actions
 */
class CC_DLL ActionEase : public ActionInterval
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ActionEase ( KDvoid );

		/** initializes the action */
		KDbool						initWithAction ( ActionInterval* pAction );

		virtual ActionInterval*		getInnerAction ( KDvoid );

		//
		// Overrides
		//
		virtual ActionEase*			clone ( KDvoid ) const override = 0;
		virtual ActionEase*			reverse ( KDvoid ) const override = 0;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		/** The inner action */
		ActionInterval*				m_pInner;
};

/** 
 *	@brief Base class for Easing actions with rate parameters
 *	@ingroup Actions
 */
class CC_DLL EaseRateAction : public ActionEase
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~EaseRateAction ( KDvoid );

		/** Initializes the action with the inner action and the rate parameter */
		KDbool						initWithAction ( ActionInterval* pAction, KDfloat fRate );

		/** set rate value for the actions */
		inline KDvoid				setRate ( KDfloat fRate ) { m_fRate = fRate; }

		/** get rate value for the actions */
		inline KDfloat				getRate ( KDvoid ) const { return m_fRate; }

		//
		// Overrides
		//
		virtual EaseRateAction*		clone ( KDvoid ) const override = 0;
		virtual EaseRateAction*		reverse ( KDvoid ) const override = 0;

	protected :

		KDfloat						m_fRate;
};

/** 
 *	@brief EaseIn action with a rate
 *	@ingroup Actions
 */
class CC_DLL EaseIn : public EaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static EaseIn*				create ( ActionInterval* pAction, KDfloat fRate );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseIn*				clone ( KDvoid ) const override;
		virtual EaseIn*				reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseOut action with a rate
 *	@ingroup Actions
 */
class CC_DLL EaseOut : public EaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static EaseOut*				create ( ActionInterval* pAction, KDfloat fRate );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseOut* 			clone ( KDvoid ) const override;
		virtual EaseOut* 			reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseInOut action with a rate
 *	@ingroup Actions
 */
class CC_DLL EaseInOut : public EaseRateAction
{
	public :

		/** Creates the action with the inner action and the rate parameter */
		static EaseInOut*			create(ActionInterval* action, float rate);

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseInOut*			clone ( KDvoid ) const override;
		virtual EaseInOut*			reverse() const  override;
};

/** 
 *	@brief Ease Exponential In
 *	@ingroup Actions
 */
class CC_DLL EaseExponentialIn : public ActionEase
{
	public :

		/** creates the action */
		static EaseExponentialIn* create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseExponentialIn*	clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Exponential Out
 *	@ingroup Actions
 */
class CC_DLL EaseExponentialOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseExponentialOut*	create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseExponentialOut* clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Exponential InOut
 *	@ingroup Actions
 */
class CC_DLL EaseExponentialInOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseExponentialInOut*	create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual EaseExponentialInOut*	clone ( KDvoid ) const override;
		virtual EaseExponentialInOut*	reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Sine In
 *	@ingroup Actions
 */
class CC_DLL EaseSineIn : public ActionEase
{
	public :

		/** creates the action */
		static EaseSineIn*			create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseSineIn*			clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Sine Out
 *	@ingroup Actions
 */
class CC_DLL EaseSineOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseSineOut*			create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseSineOut*		clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Sine InOut
 *	@ingroup Actions
 */
class CC_DLL EaseSineInOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseSineInOut*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseSineInOut*		clone ( KDvoid ) const override;
		virtual EaseSineInOut*		reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Elastic abstract class
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseElastic : public ActionEase
{
	public :

		/** Initializes the action with the inner action and the period in radians (default is 0.3) */
		KDbool						initWithAction ( ActionInterval* pAction, KDfloat fPeriod = 0.3f );

		/** get period of the wave in radians. default is 0.3 */
		inline KDfloat				getPeriod() const { return m_fPeriod; }

		/** set period of the wave in radians. */
		inline KDvoid				setPeriod ( KDfloat fPeriod ) { m_fPeriod = fPeriod; }

		//
		// Overrides
		//
		virtual EaseElastic*		clone ( KDvoid ) const override = 0;
		virtual EaseElastic*		reverse ( KDvoid ) const override = 0;

	protected :

		KDfloat						m_fPeriod;
};

/** 
 *	@brief Ease Elastic In action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseElasticIn : public EaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static EaseElasticIn*		create ( ActionInterval* pAction, KDfloat fPeriod );
		static EaseElasticIn*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseElasticIn*		clone ( KDvoid ) const override;
		virtual EaseElastic*		reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Elastic Out action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseElasticOut : public EaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static EaseElasticOut*		create ( ActionInterval* pAction, KDfloat fPeriod );
		static EaseElasticOut*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseElasticOut*		clone ( KDvoid ) const override;
		virtual EaseElastic*		reverse ( KDvoid ) const override;
};

/** 
 *	@brief Ease Elastic InOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseElasticInOut : public EaseElastic
{
	public :

		/** Creates the action with the inner action and the period in radians (default is 0.3) */
		static EaseElasticInOut*	create ( ActionInterval* pAction, KDfloat fPeriod );
		static EaseElasticInOut*	create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseElasticInOut*	clone ( KDvoid ) const override;
		virtual EaseElasticInOut*	reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBounce abstract class.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBounce : public ActionEase
{
	public :

		float bounceTime(float time);

		// Overrides
		virtual EaseBounce*			clone ( KDvoid ) const override = 0;
		virtual EaseBounce*			reverse ( KDvoid ) const override = 0;
};

/** 
 *	@brief EaseBounceIn action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBounceIn : public EaseBounce
{
	public :

		/** creates the action */
		static EaseBounceIn*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBounceIn*		clone ( KDvoid ) const override;
		virtual EaseBounce*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBounceOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBounceOut : public EaseBounce
{
	public :

		/** creates the action */
		static EaseBounceOut*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBounceOut*		clone ( KDvoid ) const override;
		virtual EaseBounce*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBounceInOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBounceInOut : public EaseBounce
{
	public :

		/** creates the action */
		static EaseBounceInOut*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBounceInOut*	clone ( KDvoid ) const override;
		virtual EaseBounceInOut*	reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBackIn action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBackIn : public ActionEase
{
	public :

		/** creates the action */
		static EaseBackIn*			create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBackIn*			clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBackOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBackOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseBackOut*			create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBackOut*		clone ( KDvoid ) const override;
		virtual ActionEase*			reverse ( KDvoid ) const override;
};

/** 
 *	@brief EaseBackInOut action.
 *	@warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 *	@since v0.8.2
 *	@ingroup Actions
 */
class CC_DLL EaseBackInOut : public ActionEase
{
	public :

		/** creates the action */
		static EaseBackInOut*		create ( ActionInterval* pAction );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual EaseBackInOut*		clone ( KDvoid ) const override;
		virtual EaseBackInOut*		reverse ( KDvoid ) const override;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionEase_h__
