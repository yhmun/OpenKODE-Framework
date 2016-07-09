/* -----------------------------------------------------------------------------------
 *
 *      File            CCEvent.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCEvent_h__
#define __CCEvent_h__

#include <string>
#include "../../platform/CCPlatformMacros.h"

NS_CC_BEGIN

class Node;

/**
 *   Base class of all kinds of events.
 */
class Event
{
	public :

		enum class Type
		{
			TOUCH,
			KEYBOARD,
			ACCELERATION,
			MOUSE,
			CUSTOM
		};
    
	protected :

		/** Constructor */
		Event ( Type eType );

	public :

		/** Destructor */
		virtual ~Event ( KDvoid );

		/** Gets the event type */
		inline Type			getType ( KDvoid ) const { return m_eType; }
    
		/** Stops propagation for current event */
		inline KDvoid		stopPropagation ( KDvoid ) { m_bIsStopped = true; }
    
		/** Checks whether the event has been stopped */
		inline KDbool		isStopped ( KDvoid ) const { return m_bIsStopped; }
    
		/** 
		 *	@brief Gets current target of the event
		 *  @return The target with which the event associates.
		 *  @note It onlys be available when the event listener is associated with node. 
		 *        It returns 0 when the listener is associated with fixed priority.
		 */
		inline Node*		getCurrentTarget ( KDvoid ) { return m_pCurrentTarget; }
    
	protected :

		/** Sets current target */
		inline KDvoid		setCurrentTarget ( Node* pTarget ) { m_pCurrentTarget = pTarget; }
    
		Type				m_eType;				///< Event type
    
		KDbool				m_bIsStopped;			///< whether the event has been stopped.
		Node*				m_pCurrentTarget;		///< Current target
    
		friend class EventDispatcher;
};

NS_CC_END

#endif	// __CCEvent_h__
