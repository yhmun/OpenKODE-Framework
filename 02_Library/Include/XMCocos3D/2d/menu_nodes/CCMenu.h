/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenu.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCMenu_h__
#define __CCMenu_h__

#include "CCMenuItem.h"
#include "../layers_scenes_transitions_nodes/CCLayer.h"
#include "../event_dispatcher/CCEventTouch.h"

NS_CC_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup menu
 *	@{
 */

/** 
 *	@brief A Menu
 * 
 *	Features and Limitation:
 *		- You can add MenuItem objects in runtime using addChild:
 *		- But the only accepted children are MenuItem objects
 */
class CC_DLL Menu : public LayerRGBA
{
	public :

		enum class State
		{
			WAITING,
			TRACKING_TOUCH,
		};
    
		/** creates an empty Menu */
		static Menu*		create ( KDvoid );

		/** creates a Menu with MenuItem objects */
		static Menu*		create ( MenuItem* pItem, ... ) CC_REQUIRES_NULL_TERMINATION;

		/** creates a Menu with a Array of MenuItem objects */
		static Menu*		createWithArray ( Array* pArrayOfItems );

		/** 
		 *	creates a Menu with it's item, then use addChild() to add 
		 *	other items. It is used for script, it can't init with undetermined
		 *	number of variables.
		 */
		static Menu*		createWithItem ( MenuItem* pItem );
    
		/** creates a Menu with MenuItem objects */
		static Menu*		createWithItems ( MenuItem* pFirstItem, KDVaListKHR pArgs );

		/**
		 * @js ctor
		 */
		Menu ( KDvoid ) : m_pSelectedItem ( KD_NULL ) { }

		virtual ~Menu ( KDvoid );

		/** initializes an empty Menu */
		KDbool				init ( KDvoid );

		/** initializes a Menu with a NSArray of MenuItem objects */
		KDbool				initWithArray ( Array* pArrayOfItems );

		/** align items vertically */
		KDvoid				alignItemsVertically ( KDvoid );

		/**
		 *	align items vertically with padding
		 *	@since v0.7.2
		 */
		KDvoid				alignItemsVerticallyWithPadding ( KDfloat fPadding );

		/** align items horizontally */
		KDvoid				alignItemsHorizontally ( KDvoid );

		/**
		 *	align items horizontally with padding
		 *	@since v0.7.2
		 */
		KDvoid				alignItemsHorizontallyWithPadding ( KDfloat fPadding );

		/** align items in rows of columns */
		KDvoid				alignItemsInColumns ( KDint nColumns, ...) CC_REQUIRES_NULL_TERMINATION;
		KDvoid				alignItemsInColumns ( KDint nColumns, KDVaListKHR pArgs );
		KDvoid				alignItemsInColumnsWithArray ( Array* pRows );

		/** align items in columns of rows */
		KDvoid				alignItemsInRows ( KDint nRows, ... ) CC_REQUIRES_NULL_TERMINATION;
		KDvoid				alignItemsInRows ( KDint nRows, KDVaListKHR pArgs );
		KDvoid				alignItemsInRowsWithArray ( Array* pColumns );

		virtual KDbool		 isEnabled ( KDvoid ) const { return m_bEnabled; }
		virtual KDvoid		setEnabled ( KDbool bValue ) { m_bEnabled = bValue; }

		virtual KDbool		onTouchBegan ( Touch* pTouch, Event* pEvent );
		virtual KDvoid		onTouchMoved ( Touch* pTouch, Event* pEvent );
		virtual KDvoid		onTouchEnded ( Touch* pTouch, Event* pEvent );
		virtual KDvoid		onTouchCancelled ( Touch* pTouch, Event* pEvent );

    
		// overrides
		virtual KDvoid		removeChild ( Node* pChild, KDbool bCleanup ) override;
    
		virtual KDvoid		addChild ( Node* pChild ) override;
		virtual KDvoid		addChild ( Node* pChild, KDint zOrder ) override;
		virtual KDvoid		addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;
    
		virtual KDvoid		onEnter ( KDvoid ) override;
		virtual KDvoid		onExit  ( KDvoid ) override;

		virtual KDvoid		setOpacityModifyRGB ( KDbool bValue ) override { CC_UNUSED_PARAM ( bValue ); }
		virtual KDbool		 isOpacityModifyRGB ( KDvoid ) const override { return false;}

		// Added By XMSoft - All Child Applied.
		KDvoid				setSoundActivate   ( const KDchar* szPath );
		KDvoid				setSoundSelected   ( const KDchar* szPath );
		KDvoid				setSoundUnselected ( const KDchar* szPath );

	protected :

		/** whether or not the menu will receive events */
		KDbool				m_bEnabled;

		MenuItem*			itemForTouch ( Touch* pTouch );

		State				m_eState;
		MenuItem*			m_pSelectedItem;
};

//	end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCMenu_h__
