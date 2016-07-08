/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionObject.h
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

#ifndef __CCActionObject_h__
#define __CCActionObject_h__

#include "CCActionNode.h"
#include "../json/CSContentJsonDictionary.h"

namespace cocostudio {

class ActionObject:public cocos2d::Object
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionObject ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionObject ( KDvoid );
    
		/**
		 *	Sets name for object
		 *
		 *	@param name    name of object
		 */
		KDvoid					setName ( const KDchar* pName );
    
		/**
		 *	Sets name for object
		 *
		 *	@return name of object
		 */
		const KDchar*			getName ( KDvoid );
    
		/**
		 *	Sets if the action will loop play.
		 *
		 *	@param bLoop     that if the action will loop play
		 */
		KDvoid					setLoop ( KDbool bLoop );
    
		/**
		 *	Gets if the action will loop play.
		 *
		 *	@return   that if the action will loop play
		 */
		KDbool					getLoop ( KDvoid );
    
		/**
		 *	Sets the time interval of frame.
		 *
		 *	@param fTime   the time interval of frame
		 */
		KDvoid				setUnitTime ( KDfloat fTime );
    
		/**
		 *	Gets the time interval of frame.
		 *
		 *	@return fTime   the time interval of frame
		 */
		KDfloat					getUnitTime ( KDvoid );

		/**
		 *	Sets the current time of frame.
		 *
		 *	@param fTime   the current time of frame
		 */
		KDvoid					setCurrentTime ( KDfloat fTime );
     
		/**
		 *	Gets the current time of frame.
		 *
		 *	@return fTime   the current time of frame
		 */
		KDfloat					getCurrentTime ( KDvoid );
	
		/**
		 *	Return if the action is playing.
		 *
		 *	@return true if the action is playing, false the otherwise
		 */
		KDbool					isPlaying ( KDvoid );

		/**
		 *	Play the action.
		 */
		KDvoid					play ( KDvoid );

		/**
		 *	Pause the action.
		 */
		KDvoid					pause ( KDvoid );
    
		/**
		 *	Stop the action.
		 */
		KDvoid					stop ( KDvoid );

		/**
		 *	Adds a ActionNode to play the action.
		 *
		 *	@node    the ActionNode which will play the action
		 */
		KDvoid					addActionNode ( ActionNode* pNode );
    
		/**
		 *	Removes a ActionNode which play the action.
		 *
		 *	@node    the ActionNode which play the action
		 */
		KDvoid					removeActionNode ( ActionNode* pNode );

		/** update frame method */
		KDvoid					updateToFrameByTime ( KDfloat fTime );

		/** init properties with a json dictionary */
		KDvoid					initWithDictionary ( JsonDictionary* pDic, cocos2d::Object* pRoot );

		/**	scheduler update function */
		KDvoid					simulationActionUpdate ( KDfloat dt );

	protected :

		cocos2d::Array*			m_pActionNodeList;		/* actionnode */
		std::string				m_sName;
		KDbool					m_bLoop;
		KDbool					m_bPause;
		KDbool					m_bPlaying;
		KDfloat					m_fUnitTime;
		KDfloat					m_fCurrentTime;
		cocos2d::Scheduler*		m_pScheduler;
};

}

#endif	// __CCActionObject_h__
