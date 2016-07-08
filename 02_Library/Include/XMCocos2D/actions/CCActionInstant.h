/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInstant.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCActionInstant_h__
#define __CCActionInstant_h__

#include "CCAction.h"
#include "../ccTypeInfo.h"

#include <string>

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

/** 
 *	@brief	Instant actions are immediate actions. They don't have a duration like
 *			the CCIntervalAction actions.
 */ 
class CCActionInstant : public CCFiniteTimeAction
{
	public :

		// CCAction methods
		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDbool		isDone ( KDvoid );

		virtual KDvoid		step ( KDfloat fElapsed );

		virtual KDvoid		update ( KDfloat fTime );

		// CCFiniteTimeAction method
		virtual CCFiniteTimeAction*		reverse ( KDvoid );
};

/** 
 *	@brief	Show the node
 */ 
class CCShow : public CCActionInstant
{
	public :

		/** Allocates and initializes the action */
		static  CCShow*		create ( KDvoid );

	public :		

		// super methods

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual CCFiniteTimeAction*		reverse ( KDvoid );
};

/** 
 *	@brief	Hide the node
 */ 
class CCHide : public CCActionInstant
{
	public :

		/** Allocates and initializes the action */
		static  CCHide*		create ( KDvoid );

	public :		

		// super methods

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual CCFiniteTimeAction*		reverse ( KDvoid );
};

/** 
 *	@brief	Toggles the visibility of a node
 */ 
class CCToggleVisibility : public CCActionInstant
{
	public :

		/** Allocates and initializes the action */
		static  CCToggleVisibility*		create ( KDvoid );

	public :

		// super method

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

/** 
 *	@brief Remove the node
 */
class CCRemoveSelf : public CCActionInstant
{
	public :
				 CCRemoveSelf ( KDvoid );
		virtual ~CCRemoveSelf ( KDvoid );

		/** create the action */
		static CCRemoveSelf*	create ( KDbool bIsNeedCleanUp = KD_TRUE );

	public :

		/** init the action */
		virtual KDbool		init ( KDbool bIsNeedCleanUp );

		// super methods
		virtual KDvoid		update ( KDfloat fTime );

		virtual CCFiniteTimeAction*		reverse ( KDvoid );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

	protected :

		KDbool				m_bIsNeedCleanUp;
};

/** 
 *	@brief	Flips the sprite horizontally
 *	@since	v0.99.0
 */ 
class CCFlipX : public CCActionInstant
{
	public :

		CCFlipX ( KDvoid );

		/** create the action */
		static  CCFlipX*	create ( KDbool bFlipX );

	public :

		/** init the action */
		virtual KDbool		initWithFlipX ( KDbool bFlipX );

		// super methods

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual CCFiniteTimeAction*		reverse ( KDvoid );

	protected :

		KDbool				m_bFlipX;
};

/** 
 *	@brief	Flips the sprite vertically
 *	@since	v0.99.0
 */ 
class CCFlipY : public CCActionInstant
{
	public :

		CCFlipY ( KDvoid );
		
		/** create the action */
		static   CCFlipY*	create ( KDbool bFlipY );

	public :

		/** init the action */
		virtual KDbool		initWithFlipY ( KDbool bFlipY );

		// super methods
		
		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual CCFiniteTimeAction*		reverse ( KDvoid );

	protected :

		KDbool				m_bFlipY;
};

/** 
 *	@brief	Places the node in a certain position
 */ 
class CCPlace : public CCActionInstant
{
	public :

		/** creates a Place action with a position */
		static   CCPlace*	create ( const CCPoint& tPosition );

	public :

		/** Initializes a Place action with a position */
		virtual KDbool		initWithPosition ( const CCPoint& tPosition );

		// super methods
		
		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

	protected :

		CCPoint				m_tPosition;
};

/** 
 *	@brief	Calls a 'callback'
 */ 
class CCCallFunc : public CCActionInstant 
{
	public :

		         CCCallFunc ( KDvoid );
		virtual ~CCCallFunc ( KDvoid );

		/** 
		 *	creates the action with the callback 
		 *	typedef void (CCObject::*SEL_CallFunc)(); 
		 */
		static CCCallFunc*	create ( CCObject* pSelectorTarget, SEL_CallFunc pSelector );

		/** creates the action with the handler script function */
		static CCCallFunc*  create ( KDint nHandler );

	public :

		/** 
		 *	initializes the action with the callback 
		 *	typedef void (CCObject::*SEL_CallFunc)();
		 */
		virtual KDbool		initWithTarget ( CCObject* pSelectorTarget );

		// super methods

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*  copyWithZone ( CCZone* pZone );
		
		/** executes the callback */
		virtual KDvoid		execute ( KDvoid );

		inline  CCObject*	getTargetCallback ( KDvoid )
		{
			return m_pSelectorTarget;
		}

		inline KDvoid		setTargetCallback ( CCObject* pSelectorTarget )
		{
			if ( pSelectorTarget != m_pSelectorTarget )
			{
				CC_SAFE_RETAIN  ( pSelectorTarget );
				CC_SAFE_RELEASE ( m_pSelectorTarget );
				m_pSelectorTarget = pSelectorTarget; 		
			}
		}

		inline KDint		getScriptHandler ( KDvoid ) 
		{
			return m_nScriptHandler; 
		};

	protected :

		/** Target that will be called */
		CCObject*			m_pSelectorTarget;

		KDint				m_nScriptHandler;
		union
		{
			SEL_CallFunc	m_pCallFunc;
			SEL_CallFuncN	m_pCallFuncN;
			SEL_CallFuncND	m_pCallFuncND;
            SEL_CallFuncO   m_pCallFuncO;
		};
};

/** 
 *	@brief	Calls a 'callback' with the node as the first argument N means Node
 */ 
class CCCallFuncN : public CCCallFunc, public TypeInfo
{
	public :

		/**
		 *	creates the action with the callback 
		 *	typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
		 */
		static   CCCallFuncN*  create ( CCObject* pSelectorTarget, SEL_CallFuncN pSelector );

		/** creates the action with the handler script function */
		static   CCCallFuncN*  create ( KDint nHandler );

	public :

		/**
		 *	initializes the action with the callback 
		 *	typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
		 */
		virtual KDbool		initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncN pSelector );

		virtual KDint32		getClassTypeInfo ( KDvoid );

		// super methods

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		execute ( KDvoid );
};

/** 
 *	@brief	Calls a 'callback' with the node as the first argument and the 2nd argument is data
 *			ND means: Node and Data. Data is void *, so it could be anything.
 */ 
class CCCallFuncND : public CCCallFuncN
{
	public :

		/** creates the action with the callback and the data to pass as an argument */
		static  CCCallFuncND*  create ( CCObject* pSelectorTarget, SEL_CallFuncND pSelector, KDvoid* pData );

	public :

		/** initializes the action with the callback and the data to pass as an argument */
		virtual KDbool		initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncND pSelector, KDvoid* pData );

		virtual KDint32		getClassTypeInfo ( KDvoid );

		// super methods
		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		execute ( KDvoid );

	protected :

		KDvoid*				m_pData;
};

/** 
 *	@brief	Calls a 'callback' with an object as the first argument. O means Object.
 *	@since	v0.99.5
 */ 
class CCCallFuncO : public CCCallFunc, public TypeInfo
{
    public :

                 CCCallFuncO ( KDvoid );
        virtual ~CCCallFuncO ( KDvoid );

        /**
		 *	creates the action with the callback 
         *	typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
         */
        static   CCCallFuncO*  create ( CCObject* pSelectorTarget, SEL_CallFuncO pSelector, CCObject* pObject );

    public :

		/** 
		 *	initializes the action with the callback 
         *	typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
         */
        virtual KDbool		initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncO pSelector, CCObject* pObject );

		virtual KDint32		getClassTypeInfo ( KDvoid );

		// super methods
		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		execute ( KDvoid );

		inline  CCObject*	getObject ( KDvoid )
		{
			return m_pObject;
		}

		inline  KDvoid		setObject ( CCObject* pObject )
		{
			if ( pObject != m_pObject )
			{
				CC_SAFE_RELEASE ( m_pObject );
				m_pObject = pObject;
				CC_SAFE_RETAIN ( m_pObject );
			}
		}

    protected :

		/** object to be passed as argument */
        CCObject*			m_pObject;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionInstant_h__
