/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventListenerTouch.h
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

#ifndef __CCEventListenerTouch_h__
#define __CCEventListenerTouch_h__

#include "CCEventListener.h"
#include "CCTouch.h"

#include <vector>

NS_CC_BEGIN

class EventListenerTouchOneByOne : public EventListener
{
	public :

		static EventListenerTouchOneByOne*		create ( KDvoid );
    
		virtual ~EventListenerTouchOneByOne ( KDvoid );
    
		KDvoid									setSwallowTouches ( KDbool bNeedSwallow );
    
		/// Overrides
		virtual EventListenerTouchOneByOne*		clone ( KDvoid ) override;
		virtual KDbool							checkAvailable ( KDvoid ) override;
		//

	public :

		std::function<bool(Touch*, Event*)>		onTouchBegan;
		std::function<void(Touch*, Event*)>		onTouchMoved;
		std::function<void(Touch*, Event*)>		onTouchEnded;
		std::function<void(Touch*, Event*)>		onTouchCancelled;
    
	private :

		EventListenerTouchOneByOne ( KDvoid );

		KDbool		init ( KDvoid );
    
		std::vector<Touch*>						m_aClaimedTouches;
		KDbool									m_bNeedSwallow;
    
		friend class EventDispatcher;
};


class EventListenerTouchAllAtOnce : public EventListener
{
	public :

		static const ListenerID					ID = static_cast<ListenerID> ( Type::TOUCH_ALL_AT_ONCE );

		static EventListenerTouchAllAtOnce*		create ( KDvoid );

		virtual ~EventListenerTouchAllAtOnce ( KDvoid );
    
		/// Overrides
		virtual EventListenerTouchAllAtOnce*	clone ( KDvoid ) override;
		virtual KDbool							checkAvailable ( KDvoid ) override;
		//
	public:
		std::function<KDvoid(const std::vector<Touch*>&, Event*)>	onTouchesBegan;
		std::function<KDvoid(const std::vector<Touch*>&, Event*)>	onTouchesMoved;
		std::function<KDvoid(const std::vector<Touch*>&, Event*)>	onTouchesEnded;
		std::function<KDvoid(const std::vector<Touch*>&, Event*)>	onTouchesCancelled;
    
	private :

		EventListenerTouchAllAtOnce ( KDvoid );

		KDbool		init ( KDvoid );

	private :
    
		friend class EventDispatcher;
};

NS_CC_END

#endif	// __CCEventListenerTouch_h__
