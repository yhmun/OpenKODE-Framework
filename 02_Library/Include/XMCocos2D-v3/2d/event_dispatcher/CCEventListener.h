/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventListener.h
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

#ifndef __CCEventListener_h__
#define __CCEventListener_h__

#include "../../platform/CCPlatformMacros.h"
#include "../../base/CCObject.h"

#include <functional>
#include <string>
#include <memory>
#include <set>

NS_CC_BEGIN

class Event;
class Node;

/**
 *  The base class of event listener.
 *  If you need custom listener which with different callback, you need to inherit this class.
 *  For instance, you could refer to EventListenerAcceleration, EventListenerKeyboard, EventListenerTouchOneByOne, EventListenerCustom.
 */
class EventListener : public Object
{
	public :

		enum class Type
		{
			UNKNOWN,
			TOUCH_ONE_BY_ONE,
			TOUCH_ALL_AT_ONCE,
			KEYBOARD,
			MOUSE,
			ACCELERATION,
			CUSTOM
		};
    
		typedef std::size_t		ListenerID;
    
	protected :

		/** Constructor */
		EventListener ( KDvoid );
    
		/** Initializes event with type and callback function */
		KDbool					init ( Type eType, ListenerID eListenerID, std::function<KDvoid(Event*)> pCallback );

	public :

		/** Destructor */
		virtual ~EventListener ( KDvoid );
    
		/** Checks whether the listener is available. */
		virtual KDbool			checkAvailable ( KDvoid ) = 0;
    
		/** Clones the listener, its subclasses have to override this method. */
		virtual EventListener*	clone ( KDvoid ) = 0;
    
	protected :

		inline void				setPaused ( KDbool bPaused ) { m_bPaused = bPaused; }
		inline bool				 isPaused ( KDvoid ) const { return m_bPaused; }
    
		inline void				setRegistered ( KDbool bRegistered ) { m_bIsRegistered = bRegistered; }
		inline bool				 isRegistered ( KDvoid ) const { return m_bIsRegistered; }
    
		inline Type				getType ( KDvoid ) const { return m_eType; }
		inline ListenerID		getListenerID ( KDvoid ) const { return m_eListenerID; }
    
		inline void				setFixedPriority ( KDint nFixedPriority ) { m_nFixedPriority = nFixedPriority; }
		inline int				getFixedPriority ( KDvoid ) const { return m_nFixedPriority; }
    
		inline void				setSceneGraphPriority ( Node* pNode ) { m_pNode = pNode; }
		inline Node*			getSceneGraphPriority ( KDvoid ) const { return m_pNode; }
    
		std::function<KDvoid(Event*)>		onEvent;								/// Event callback function
    
		Type								m_eType;								/// Event listener type
		ListenerID							m_eListenerID;							/// Event listener ID
		KDbool								m_bIsRegistered;						/// Whether the listener has been added to dispatcher.
    
		// The priority of event listener
		KDint								m_nFixedPriority;						// The higher the number, the higher the priority, 0 is for scene graph base priority.
		Node*								m_pNode;								// scene graph based priority
		KDbool								m_bPaused;								// Whether the listener is paused
    
		friend class EventDispatcher;
};

NS_CC_END

#endif	// __CCEventListener_h__
