/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenu.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __CCMenu_h__
#define __CCMenu_h__

#include "CCMenuItem.h"
#include "../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

typedef enum  
{
	kCCMenuStateWaiting			,
	kCCMenuStateTrackingTouch	,
	kCCMenuStateAutoSwallow		,
} tCCMenuState;

enum 
{
	//* priority used by the menu for the event handler
    kCCMenuHandlerPriority  = -128,
};

enum 
{
	kDefaultPadding			=  5,
};

/** 
 *	@brief A CCMenu
 * 
 *	Features and Limitation:
 *		- You can add MenuItem objects in runtime using addChild:
 *		- But the only accepted children are MenuItem objects
 */
class CCMenu : public CCLayerRGBA
{
	public :
				 CCMenu ( KDvoid );
		virtual ~CCMenu ( KDvoid );

		/** creates an empty CCMenu */
		static   CCMenu*		create ( KDvoid );

		/** creates a CCMenu with CCMenuItem objects */
		static   CCMenu*		create ( CCMenuItem* pItem, ... );

		/** creates a CCMenu with CCMenuItem objects */
		static	 CCMenu*		createWithItems ( CCMenuItem* pItem, KDVaListKHR pArgs );

		/** creates a CCMenu with a CCArray of CCMenuItem objects */
		static   CCMenu*		createWithArray ( CCArray* pArrayOfItems );

		/** 
		 *	creates a CCMenu with it's item, then use addChild() to add 
		 *	other items. It is used for script, it can't init with undetermined
		 *	number of variables.
		 */
		static   CCMenu*		createWithItem ( CCMenuItem* pItem );

	public :

		/** initializes an empty CCMenu */
		virtual KDbool			init ( KDvoid );

		/** initializes a CCMenu with a CCArray of CCMenuItem objects */
		virtual KDbool			initWithArray ( CCArray* pArrayOfItems );
	
		/** align items vertically */
		KDvoid					alignItemsVertically ( KDfloat fPadding = kDefaultPadding );

		/**
		 *	align items vertically with padding
		 *	@since	v0.7.2
		 */
		KDvoid					alignItemsVerticallyWithPadding ( KDfloat fPadding );
	
		/** align items horizontally */
		KDvoid					alignItemsHorizontally ( KDfloat fPadding = kDefaultPadding );

		/**
		 *	align items horizontally with padding
		 *	@since	v0.7.2
		 */
		KDvoid					alignItemsHorizontallyWithPadding ( KDfloat fPadding );

		/** align items in rows of columns */
		KDvoid					alignItemsInColumns ( KDuint uColumns, ... );
		KDvoid					alignItemsInColumns ( KDuint uColumns, KDVaListKHR pArgs );
		KDvoid					alignItemsInColumnsWithArray ( CCArray* pRows );

		/** align items in columns of rows */
		KDvoid					alignItemsInRows ( KDuint uRows, ... );
		KDvoid					alignItemsInRows ( KDuint uRows, KDVaListKHR pArgs );
		KDvoid					alignItemsInRowsWithArray ( CCArray* pColumns );

		/** set event handler priority. By default it is: kCCMenuHandlerPriority */
		KDvoid					setHandlerPriority ( KDint nPriority );

		KDvoid					setHandlerSwallow ( KDbool bOn );
		KDvoid					setHandlerAutoSwallow ( KDbool bOn, KDfloat fGap = 10.f );

		// super methods
		virtual KDvoid			addChild ( CCNode* pChild );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );

		virtual KDvoid			registerWithTouchDispatcher ( KDvoid );
		virtual KDvoid			removeChild ( CCNode* pChild, KDbool bCleanup );

		/**
         *	@brief	For phone event handle functions
         */
		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );		

        /**
         *	@since	v0.99.5
         *	override onExit
         */
        virtual KDvoid			onExit ( KDvoid );

		virtual KDvoid			setOpacityModifyRGB ( KDbool bValue ) { CC_UNUSED_PARAM ( bValue ); }
	    virtual KDbool			 isOpacityModifyRGB ( KDvoid ) { return KD_FALSE;}

		/** whether or not the menu will receive events */
		virtual KDbool			 isEnabled ( KDvoid ) { return m_bEnabled; }
		virtual KDvoid			setEnabled ( KDbool bEnable ) { m_bEnabled = bEnable; };

	protected :

		CCMenuItem*				itemForTouch ( CCTouch* pTouch );

	protected :

		KDbool				m_bEnabled;
		KDbool				m_bSwallow;
		KDbool				m_bAutoSwallow;
        KDfloat             m_fAutoSwallowGap;
		CCPoint				m_tPrevLocation;
		tCCMenuState		m_eState;
		CCMenuItem*			m_pSelectedItem;
};

class CCMenuRadio : public CCMenu
{
	public :
				 CCMenuRadio ( KDvoid );
		virtual ~CCMenuRadio ( KDvoid );

        static  CCMenuRadio*	create ( KDvoid );
    
        static  CCMenuRadio*	create ( CCObject* pTarget, SEL_MenuHandler pSelector );
    
		/** creates a CCMenuRadio with it's items */
		static  CCMenuRadio*	create ( CCObject* pTarget, SEL_MenuHandler pSelector, CCMenuItem* pItem, ... );

	public :

        virtual KDbool			init ( KDvoid );
    
        virtual KDbool			initWithTarget ( CCObject* pTarget, SEL_MenuHandler pSelector );
    
		virtual KDbool			initWithItems  ( CCObject* pTarget, SEL_MenuHandler pSelector, CCMenuItem* pItem, KDVaListKHR pArgs );

        virtual KDvoid			addChild ( CCNode* pChild );
        virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder );
        virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );
    
		virtual KDvoid			setSelectedIndex ( KDuint uIndex );
		virtual KDuint			getSelectedIndex ( KDvoid );

	private :

		KDvoid					onSelected ( CCObject* pSender );

		CCObject*				m_pListener;
		SEL_MenuHandler			m_pSelector;
		KDuint					m_uSelectedIndex;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCMenu_h__