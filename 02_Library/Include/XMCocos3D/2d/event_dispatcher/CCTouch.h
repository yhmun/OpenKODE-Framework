/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouch.h
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

#ifndef __CCTouch_h__
#define __CCTouch_h__

#include "../../base/CCObject.h"
#include "../../base/CCGeometry.h"

NS_CC_BEGIN

/**
 *	@addtogroup input
 *	@{
 */

class CC_DLL Touch : public Object
{
	public :

		/** how the touches are dispathced */
		enum class DispatchMode 
		{
			/** All at once */
			ALL_AT_ONCE,
			/** one by one */
			ONE_BY_ONE,
		};

		Touch ( KDvoid ) 
		{
			m_nId = -1;
			m_bStartPointCaptured = false;
		}

		/** returns the current touch location in OpenGL coordinates */
		Point			getLocation ( KDvoid ) const;
		
		/** returns the previous touch location in OpenGL coordinates */
		Point			getPreviousLocation ( KDvoid ) const;
		
		/** returns the start touch location in OpenGL coordinates */
		Point			getStartLocation ( KDvoid ) const;
		
		/** returns the delta of 2 current touches locations in screen coordinates */
		Point			getDelta ( KDvoid ) const;
		
		/** returns the current touch location in screen coordinates */
		Point			getLocationInView ( KDvoid ) const;
		
		/** returns the previous touch location in screen coordinates */
		Point			getPreviousLocationInView ( KDvoid ) const;

		/** returns the start touch location in screen coordinates */
		Point			getStartLocationInView ( KDvoid ) const;
    
		KDvoid			setTouchInfo ( KDint id, KDfloat x, KDfloat y )
		{
			m_nId		 = id;
			m_tPrevPoint = m_tPoint;
			m_tPoint.x   = x;
			m_tPoint.y   = y;
			if ( !m_bStartPointCaptured )
			{
				m_tStartPoint = m_tPoint;
				m_bStartPointCaptured = true;
			}
		}

		/**
		 *	@js getId
		 *	@lua getId
		 */
		KDint			getID ( KDvoid ) const
		{
			return m_nId;
		}

	private :

		KDint			m_nId;
		KDbool			m_bStartPointCaptured;
		Point			m_tStartPoint;
		Point			m_tPoint;
		Point			m_tPrevPoint;
};

//	end of input group
/// @}

NS_CC_END

#endif  // __CCTouch_h__
