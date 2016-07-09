/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCActionNode_h__
#define __CCActionNode_h__

#include "CCActionFrame.h"
#include "../json/CSContentJsonDictionary.h"
#include "../../../2d/actions/CCActionInterval.h"

namespace cocostudio {

class ActionNode : public cocos2d::Object
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionNode ( KDvoid );

		/**
		 *	Default destructor
		 */
		virtual ~ActionNode ( KDvoid );

		/**
		 *	Sets the time interval of frame.
		 *
		 *	@param fTime   the time interval of frame
		 */
		KDvoid						setUnitTime ( KDfloat fTime );
    
		/**
		 *	Gets the time interval of frame.
		 *
		 *	@return fTime   the time interval of frame
		 */
		KDfloat						getUnitTime ( KDvoid );

		/**
		 *	Sets tag for ActionNode
		 *
		 *	@param tag    tag of ActionNode
		 */
		KDvoid						setActionTag ( KDint nTag );
    
		/**
		 *	Gets tag for ActionNode
		 *
		 *	@return tag    tag of ActionNode
		 */
		KDint						getActionTag ( KDvoid );

		/**
		 *	Sets node which will run a action.
		 *
		 *	@param  node which will run a action
		 */
		KDvoid						setObject ( cocos2d::Object* pNode );
    
		/**
		 *	Gets node which will run a action.
		 *
		 *	@return  node which will run a action
		 */
		cocos2d::Object*			getObject ( KDvoid );

		/**
		 *	Insets a ActionFrame to ActionNode.
		 *
		 *	@param index  the index of ActionFrame
		 *
		 *	@param frame  the ActionFrame which will be inserted
		 */
		KDvoid						insertFrame ( KDint nIndex, ActionFrame* pFrame );
    
		/**
		 *	Pushs back a ActionFrame to ActionNode.
		 *
		 *	@param frame  the ActionFrame which will be added
		 */
		KDvoid						addFrame ( ActionFrame* pFrame );
    
		/**
		 *	Remove a ActionFrame from ActionNode.
		 *
		 *	@param frame  the ActionFrame which will be removed
		 */
		KDvoid						deleteFrame ( ActionFrame* pFrame );
    
		/**
		 *	Remove all ActionFrames from ActionNode.
		 */
		KDvoid						clearAllFrame ( KDvoid );

		/**
		 *	Gets index of first ActionFrame.
		 *
		 *	@return  index of first ActionFrame
		 */
		KDint						getFirstFrameIndex ( KDvoid );
    
		/**
		 *	Gets index of last ActionFrame.
		 *
		 *	@return  index of last ActionFrame
		 */
		KDint						getLastFrameIndex ( KDvoid );

		/**
		 *	Updates action states to some time.
		 *
		 *	@param fTime   the time when need to update
		 */
		virtual KDbool				updateActionToTimeLine ( KDfloat fTime );

		/**
		 *	Play the action.
		 */
		virtual KDvoid				playAction ( KDvoid );
    
		/**
		 *	Stop the action.
		 */
		virtual KDvoid				stopAction ( KDvoid );
	
		/** init properties with a json dictionary */
		virtual KDvoid				initWithDictionary ( JsonDictionary* pDic, cocos2d::Object* pRoot );

		/**
		 *	Gets if the action is done once time.
		 *
		 *	@return   that if the action is done once time
		 */
		virtual KDbool				isActionDoneOnce ( KDvoid );

	protected :

		KDint						m_nCurrentFrameIndex;
		KDint						m_nDestFrameIndex;
	
		KDfloat						m_fUnitTime;
    
		KDint						m_nActionTag;
		cocos2d::Spawn*				m_pActionSpawn;
		cocos2d::Action*			m_pAction;
		cocos2d::Object*			m_pObject;
    
		cocos2d::Array*				m_pFrameArray;
		KDint						m_nFrameArrayNum;

	protected :

		virtual cocos2d::Node*		getActionNode ( KDvoid );
		virtual cocos2d::Spawn*		refreshActionProperty ( KDvoid );
		virtual KDvoid				runAction ( KDvoid );
		virtual KDvoid				initActionNodeFromRoot ( cocos2d::Object* pRoot );
		virtual KDvoid				easingToFrame ( KDfloat fDuration, KDfloat fDelayTime, ActionFrame* pDestFrame );
};

}

#endif	// __CCActionNode_h__
