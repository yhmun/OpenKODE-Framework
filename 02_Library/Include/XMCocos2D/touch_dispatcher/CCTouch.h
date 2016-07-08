/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouch.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCTouch_h__
#define __CCTouch_h__

#include "../support/CCDirector.h"
#include "../support/CCPointExtension.h"

NS_CC_BEGIN
    
/**
 * @addtogroup input
 * @{
 */

class CCTouch : public CCObject
{
	public :

		CCTouch ( KDvoid );

	public :

		KDvoid				clear ( KDvoid );

		/** returns the current touch location in screen coordinates */
		const CCPoint&		getLocation ( KDvoid ) const;

		/** returns the previous touch location in screen coordinates */
		const CCPoint&		getPreviousLocation ( KDvoid ) const;

		/** returns the start touch location in screen coordinates */
		const CCPoint&		getStartLocation ( KDvoid ) const;

		/** returns the delta of 2 current touches locations in screen coordinates */
		CCPoint				getDelta ( KDvoid ) const;

		KDvoid				setTouchInfo ( KDint nID, KDfloat fX, KDfloat fY );

		KDvoid				setTouchInfo ( KDint nID, const CCPoint& tPoint );

		KDvoid				setTouchInfo ( const CCPoint& tPoint );

		KDvoid				setTouchInfo ( KDfloat fX, KDfloat fY );

		KDint				getID ( KDvoid ) const;

	private :

		KDint				m_nID;
		CCPoint				m_tPoint;
		CCPoint				m_tPrevPoint;
		CCPoint				m_tStartPoint;
		KDbool				m_bStartPointCaptured;
};

class CCEvent : public CCObject
{

};

// end of input group
/// @}

NS_CC_END

#endif // __CCTouch_h__
