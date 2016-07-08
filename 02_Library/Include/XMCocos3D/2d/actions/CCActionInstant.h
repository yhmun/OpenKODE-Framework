/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInstant.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCActionInstant_h__
#define __CCActionInstant_h__

#include <string>
#include <functional>

#include "CCAction.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief Instant actions are immediate actions. They don't have a duration like
 *	the IntervalAction actions.
 */ 
class CC_DLL ActionInstant : public FiniteTimeAction //<NSCopying>
{
	public :

		//
		// Overrides
		//
		virtual ActionInstant*		clone ( KDvoid ) const override = 0;
		virtual ActionInstant*		reverse ( KDvoid ) const override = 0;
		virtual KDbool				isDone ( KDvoid ) const override;
		virtual KDvoid				step ( KDfloat dt ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;
};

/**
 *	@brief Show the node
 */
class CC_DLL Show : public ActionInstant
{
	public :

		/** Allocates and initializes the action */
		static Show*				create ( KDvoid );

		Show ( KDvoid ) { }

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual ActionInstant*		reverse ( KDvoid ) const override;
		virtual Show*				clone ( KDvoid ) const override;
};

/** 
 *	@brief Hide the node
 */
class CC_DLL Hide : public ActionInstant
{
	public :

		/** Allocates and initializes the action */
		static Hide*				create ( KDvoid );

		Hide ( KDvoid ) { }

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual ActionInstant*		reverse ( KDvoid ) const override;
		virtual Hide*				clone ( KDvoid ) const override;
};

/**
 *	@brief Toggles the visibility of a node
 */
class CC_DLL ToggleVisibility : public ActionInstant
{
	public :

		/** Allocates and initializes the action */
		static ToggleVisibility*	create ( KDvoid );

		ToggleVisibility ( KDvoid ) { }

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual ToggleVisibility*	reverse ( KDvoid ) const override;
		virtual ToggleVisibility*	clone ( KDvoid ) const override;
};

/** 
 *	@brief Remove the node
 */
class CC_DLL RemoveSelf : public ActionInstant
{
	public :

		/** create the action */
		static RemoveSelf*			create ( KDbool bIsNeedCleanUp = true );

		RemoveSelf ( KDvoid ) 
		{
			m_bIsNeedCleanUp = true;
		}

		/** init the action */
		KDbool						init ( KDbool bIsNeedCleanUp );

		//
		// Override
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual RemoveSelf*			clone ( KDvoid ) const override;
		virtual RemoveSelf*			reverse ( KDvoid ) const override;

	protected :

		KDbool						m_bIsNeedCleanUp;
};

/** 
 *	@brief Flips the sprite horizontally
 *	@since v0.99.0
 */
class CC_DLL FlipX : public ActionInstant
{
	public :

		/** create the action */
		static FlipX*				create ( KDbool x );

		FlipX ( KDvoid )
		{
			m_bFlipX = false;
		}

		/** init the action */
		KDbool						initWithFlipX ( KDbool x );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual FlipX*				reverse ( KDvoid ) const override;
		virtual FlipX*				clone ( KDvoid ) const override;

	protected :

		KDbool						m_bFlipX;
};

/** 
 *	@brief Flips the sprite vertically
 *	@since v0.99.0
 */
class CC_DLL FlipY : public ActionInstant
{
	public :

		/** create the action */
		static FlipY*				create ( KDbool y );

		FlipY ( KDvoid )
		{
			m_bFlipY = false;
		}

		/** init the action */
		KDbool						initWithFlipY ( KDbool y );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual FlipY*				reverse ( KDvoid ) const override;
		virtual FlipY*				clone ( KDvoid ) const override;

	protected :

		KDbool						m_bFlipY;
};

/** 
 *	@brief Places the node in a certain position
 */
class CC_DLL Place : public ActionInstant //<NSCopying>
{
	public :

		Place ( KDvoid ) { }

		/** creates a Place action with a position */
		static Place*				create ( const Point& tPos );

		/** Initializes a Place action with a position */
		KDbool						initWithPosition ( const Point& tPos );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual Place*				reverse ( KDvoid ) const override;
		virtual Place*				clone ( KDvoid ) const override;

	protected :

		Point						m_tPosition;
};


/**
 *	@brief Calls a 'callback'
 */
class CC_DLL CallFunc : public ActionInstant //<NSCopying>
{
	public :

		/** 
		 *	creates the action with the callback of type std::function<void()>.
		 *	This is the preferred way to create the callback.
		 *	When this funtion bound in js or lua ,the input param will be changed
		 *	In js: var create(var func, var this, var [data]) or var create(var func)
		 *	In lua:local create(local funcID)
		 */
		static CallFunc*			create ( const std::function<KDvoid()>& pFunc );

	public :

		/**
		 *	@js ctor
		 */
		CallFunc ( KDvoid )
		{
			m_pFunction = nullptr;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~CallFunc ( KDvoid );

		/** 
		 *	initializes the action with the std::function<void()>
		 *	@js NK
		 *	@lua NK
		 */
		KDbool						initWithFunction ( const std::function<KDvoid()>& pFunc );

		/** executes the callback */
		virtual KDvoid				execute ( KDvoid );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual CallFunc*			reverse ( KDvoid ) const override;
		virtual CallFunc*			clone ( KDvoid ) const override;

		inline Object*				getTargetCallback ( KDvoid )
		{
			return m_pSelectorTarget;
		}

		inline KDvoid				setTargetCallback ( Object* pSelector )
		{
			if ( pSelector != m_pSelectorTarget )
			{
				CC_SAFE_RETAIN ( pSelector );
				CC_SAFE_RELEASE ( m_pSelectorTarget );
				m_pSelectorTarget = pSelector;
			}
		}

	protected :
    
		/** Target that will be called */
		Object*						m_pSelectorTarget;

		union
		{
			SEL_CallFunc			m_pCallFunc;
			SEL_CallFuncN			m_pCallFuncN;
		};

		/** function that will be called */
		std::function<KDvoid()>		m_pFunction;
};

/** 
 *	@brief Calls a 'callback' with the node as the first argument
 *	N means Node
 */
class CC_DLL CallFuncN : public CallFunc
{
	public :

		/**
		 *	creates the action with the callback of type std::function<void()>.
		 *	This is the preferred way to create the callback.
		 */
		static CallFuncN*			create ( const std::function<KDvoid(Node*)>& pFunc );

	public :

		CallFuncN ( KDvoid )
		{
			m_pFunctionN = nullptr;
		}

		/** 
		 *	initializes the action with the std::function<void(Node*)>
		 */
		KDbool						initWithFunction ( const std::function<KDvoid(Node*)>& pFunc );

		//
		// Overrides
		//
		virtual CallFuncN*			clone ( KDvoid ) const override;
		virtual KDvoid				execute ( KDvoid ) override;

	protected :

		/** function that will be called with the "sender" as the 1st argument */
		std::function<KDvoid(Node*)>	m_pFunctionN;
};

// end of actions group
/// @}

NS_CC_END

#endif	// __CCActionInstant_h__
