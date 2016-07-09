/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventMouse.h
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

#ifndef __CCEventMouse_h__
#define __CCEventMouse_h__

#include "CCEvent.h"

#define MOUSE_BUTTON_LEFT       0
#define MOUSE_BUTTON_RIGHT      1
#define MOUSE_BUTTON_MIDDLE     2
#define MOUSE_BUTTON_4          3
#define MOUSE_BUTTON_5          4
#define MOUSE_BUTTON_6          5
#define MOUSE_BUTTON_7          6
#define MOUSE_BUTTON_8          7

NS_CC_BEGIN

class EventMouse : public Event
{
	public :

		/**
		 *	Different types of MouseEvent
		 */
		enum class MouseEventType
		{
			MOUSE_NONE,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			MOUSE_SCROLL,
		};

		EventMouse ( MouseEventType eMouseEventCode );

		/** Set mouse scroll data */
		inline KDvoid				setScrollData ( KDfloat fScrollX, KDfloat fScrollY ) { m_fScrollX = fScrollX; m_fScrollY = fScrollY; }
		inline KDfloat				getScrollX ( KDvoid ) { return m_fScrollX; }
		inline KDfloat				getScrollY ( KDvoid ) { return m_fScrollY; }

		inline KDvoid				setCursorPosition ( KDfloat x, KDfloat y ) { m_fX = x; m_fY = y; }
		inline KDvoid				setMouseButton ( KDint nButton ) { m_nMouseButton = nButton; }
		inline KDint				getMouseButton ( KDvoid ) { return m_nMouseButton; }
		inline KDfloat				getCursorX ( KDvoid ) { return m_fX; }
		inline KDfloat				getCursorY ( KDvoid ) { return m_fY; }

	private :

		MouseEventType				m_eMouseEventType;
		KDint						m_nMouseButton;
		KDfloat						m_fX;
		KDfloat						m_fY;
		KDfloat						m_fScrollX;
		KDfloat						m_fScrollY;

		friend class EventListenerMouse;
};

NS_CC_END

#endif	// __CCEventMouse_h__
