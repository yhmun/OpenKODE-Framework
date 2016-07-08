/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionManager.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Valentin Milea
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

#ifndef __CCActionManager_h__
#define __CCActionManager_h__

#include "CCAction.h"
#include "../cocoa/CCArray.h"
#include "../cocoa/CCObject.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

class CCSet;

struct _hashElement;

/** 
 *	@brief	CCActionManager is a singleton that manages all the actions.
 *	Normally you won't need to use this singleton directly. 99% of the cases you will use the CCNode interface,
 *	which uses this singleton.
 *	But there are some cases where you might need to use this singleton.
 *	Examples:
 *		- When you want to run an action where the target is different from a CCNode. 
 *		- When you want to pause / resume the actions
 *
 *	@since v0.8
 */
class CCActionManager : public CCObject
{
	public :
				 CCActionManager ( KDvoid );
		virtual ~CCActionManager ( KDvoid );

	public :

		/** 
		 *	Adds an action with a target. 
		 *	If the target is already present, then the action will be added to the existing target.
		 *	If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
		 *	When the target is paused, the queued actions won't be 'ticked'.
		 */
		KDvoid				addAction ( CCAction* pAction, CCNode* pTarget, KDbool bPaused );

		/**
		 *	Removes all actions from all the targets.
		 */
		KDvoid				removeAllActions ( KDvoid );

		/** 
		 *	Removes all actions from a certain target.
		 *	All the actions that belongs to the target will be removed.
		 */
		KDvoid				removeAllActionsFromTarget ( CCObject* pTarget );

		/**
		 *	Removes an action given an action reference.
		 */
		KDvoid				removeAction ( CCAction* pAction );

		/** Removes an action given its tag and the target */
		KDvoid				removeActionByTag ( KDuint uTag, CCObject* pTarget );

		/** 
		 *	Gets an action given its tag an a target
		 *	@return the Action the with the given tag
		 */
		CCAction*			getActionByTag ( KDuint uTag, CCObject* pTarget );

		/**
		 *	Returns the numbers of actions that are running in a certain target. 
		 *	Composable actions are counted as 1 action. Example:
		 *		- If you are running 1 Sequence of 7 actions, it will return 1.
		 *		- If you are running 7 Sequences of 2 actions, it will return 7.
		 */
		KDuint				numberOfRunningActionsInTarget ( CCObject* pTarget );

		/** Pauses the target: all running actions and newly added actions will be paused. */
		KDvoid				pauseTarget ( CCObject* pTarget );

		/** Resumes the target. All queued actions will be resumed. */
		KDvoid				resumeTarget ( CCObject* pTarget );

		/** Pauses all running actions, returning a list of targets whose actions were paused. */
		CCSet*				pauseAllRunningActions ( KDvoid );
	    
		/** Resume a set of targets (convenience function to reverse a pauseAllRunningActions call) */
		KDvoid				resumeTargets ( CCSet* pTargetsToResume );

	protected :

		// declared in CCActionManager
		KDvoid				removeActionAtIndex ( KDuint uIndex, struct _hashElement* pElement );

		KDvoid				deleteHashElement ( struct _hashElement* pElement );

		KDvoid				actionAllocWithHashElement ( struct _hashElement* pElement );

		KDvoid				update ( KDfloat fDelta );

	protected :

		struct _hashElement*	m_pTargets;
		struct _hashElement*	m_pCurrentTarget;
		KDbool					m_bCurrentTargetSalvaged;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionManager_h__
