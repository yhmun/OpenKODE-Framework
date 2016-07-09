/* -----------------------------------------------------------------------------------
 *
 *      File            UIInputManager.h
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

#ifndef __UIInputManager_h__
#define __UIInputManager_h__

#include "../Layouts/UILayout.h"

namespace gui {

class UIInputManager
{
	public :

		/**
		 *	Default constructor
		 */
		UIInputManager ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UIInputManager ( KDvoid );
    
		/**
		 *	Regist a widget to input manager.
		 *
		 *	@param widget    registed widget can be touched.
		 */
		KDvoid					registWidget ( UIWidget* pWidget );
        
		/**
		 *	Remove a registed widget from input manager.
		 *
		 *	@param widget    widget which will be removed.
		 */
		KDvoid					removeManageredWidget ( UIWidget* pWidget );
    
		/**
		 *	Finds a widget which is selected and call it's "onTouchBegan" method.
		 *
		 *	@param touch point.
		 *
		 *	@return true that find a widget selected, false otherwise.
		 */
		KDbool					checkEventWidget ( const cocos2d::Point& tTouchPoint );
    
    
		KDvoid					update ( KDfloat dt );
		KDbool					onTouchBegan ( cocos2d::Touch* pTouch );
		KDvoid					onTouchMoved ( cocos2d::Touch* pTouch );
		KDvoid					onTouchEnd ( cocos2d::Touch* pTouch );
		KDvoid					onTouchCancelled ( cocos2d::Touch* pTouch );
    
		KDvoid					setRootWidget ( UIWidget* pRoot );
		UIWidget*				getRootWidget ( KDvoid );
		KDvoid					addCheckedDoubleClickWidget ( UIWidget* pWidget );

	protected :

		KDbool					checkTouchEvent ( UIWidget* pRoot, const cocos2d::Point& tTouchPoint );

	protected :

		cocos2d::Array*			m_pManageredWidget;
		cocos2d::Array*			m_pSelectedWidgets;
		cocos2d::Point			m_tTouchBeganedPoint;
		cocos2d::Point			m_tTouchMovedPoint;
		cocos2d::Point			m_tTouchEndedPoint;
		cocos2d::Point			m_tTouchCanceledPoint;
		KDbool					m_bTouchDown;
		KDfloat					m_fLongClickTime;
		KDfloat					m_fLongClickRecordTime;
		cocos2d::Array*			m_pCheckedDoubleClickWidget;
		UIWidget*				m_pRootWidget;
};

}

#endif	// __UIInputManager_h__
