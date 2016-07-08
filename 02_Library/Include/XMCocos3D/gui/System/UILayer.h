/* -----------------------------------------------------------------------------------
 *
 *      File            UILayer.h
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

#ifndef __UILayer_h__
#define __UILayer_h__

#include "../BaseClasses/UIRootWidget.h"
#include "UIInputManager.h"

namespace gui {

class UILayer : public cocos2d::Layer
{
    
	public:
		/**
		 *	Default constructor
		 */
		UILayer ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UILayer ( KDvoid );
    
		/**
		 *	Allocates and initializes a widget.
		 */
		static UILayer*			create ( KDvoid );
    
		// initializes state of uilayer.
		virtual KDbool			init ( KDvoid ) override;
    
		virtual KDvoid			onEnter ( KDvoid ) override;
		virtual KDvoid			onExit ( KDvoid ) override;
		virtual KDvoid			onEnterTransitionDidFinish ( KDvoid ) override;
    
		virtual KDbool			onTouchBegan ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid			onTouchMoved ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid			onTouchEnded ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid			onTouchCancelled ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
    
		/**
		 *	Add a widget to UILayer, for drawing.
		 *
		 *	@param widget.
		 */
		KDvoid					addWidget ( UIWidget* pWidget );
    
		/**
		 *	Remove a widget from UILayer.
		 *
		 *	@param widget.
		 *
		 *	@param cleanup true if all running actions on all children widgets should be cleanup, false otherwise.
		 */
		KDvoid					removeWidget ( UIWidget* pWidget );
    
		/**
		 *	Sets whether the UILayer is visible
		 *
		 *	The default value is true, a UILayer is default to visible
		 *
		 *	@param visible   true if the UILayer is visible, false if the UILayer is hidden.
		 */
		virtual KDvoid			setVisible ( KDbool bVisible ) override;
    
		/**
		 *	Finds a widget whose tag is equal tag param from widget tree.
		 *
		 *	@param tag.
		 */
		UIWidget*				getWidgetByTag ( KDint nTag );
    
		/**
		 * Seek a widget whose name is equal name param from widget tree.
		 *
		 * @param name.
		 */
		UIWidget*				getWidgetByName ( const KDchar* pName );
    
		/**
		 *	Gets UIInputManager.
		 *
		 *	UIInputManager is the touch manager of UILayer.
		 *
		 *	@return UIInputManager.
		 */
		UIInputManager*			getInputManager ( KDvoid );
    
		/**
		 *	Remove and clean up all of UILayer's widget.
		 */
		virtual KDvoid			clear ( KDvoid );
    
		/**
		 *	Gets root widget of UILayer.
		 *
		 *	@return UIRootWidget, "UIRootWidget" is the root widget of UILayer.
		 */
		UIRootWidget*			getRootWidget ( KDvoid );

	protected :

		UIRootWidget*			m_pRootWidget;
		UIInputManager*			m_pInputManager;
};

}

#endif	// __UILayer_h__
