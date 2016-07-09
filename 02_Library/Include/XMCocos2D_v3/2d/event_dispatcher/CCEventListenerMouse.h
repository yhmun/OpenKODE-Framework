/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventListenerMouse.h
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

#ifndef __CCEventListenerMouse_h__
#define __CCEventListenerMouse_h__

#include "CCEventListener.h"
#include "CCEventMouse.h"

NS_CC_BEGIN

class Event;

class EventListenerMouse : public EventListener
{
	public:
		static EventListenerMouse*				create ( KDvoid );

		/// Overrides
		virtual EventListenerMouse*				clone ( KDvoid ) override;
		virtual KDbool							checkAvailable ( KDvoid ) override;

		std::function<KDvoid(Event* pEvent)>	onMouseDown;
		std::function<KDvoid(Event* pEvent)>	onMouseUp;
		std::function<KDvoid(Event* pEvent)>	onMouseMove;
		std::function<KDvoid(Event* pEvent)>	onMouseScroll;

	private :

		EventListenerMouse ( KDvoid );

		KDbool		init ( KDvoid );
};

NS_CC_END

#endif	// __CCEventListenerMouse_h__
