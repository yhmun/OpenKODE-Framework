/* --------------------------------------------------------------------------
 *
 *      File            TestMenu.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#ifndef __TestMenu_h__
#define __TestMenu_h__

#include "../TestBasic.h"

class TestMenu : public TestBasic
{
	public :

		CREATE_FUNC ( TestMenu );

	protected :		

		virtual KDbool		init ( KDvoid );
};

class MenuLayer1 : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayer1 );

	protected :

		virtual KDbool		init ( KDvoid );
	
		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid				allowTouches ( KDfloat dt );

		KDvoid				menuCallback			 ( CCObject* pSender );
		KDvoid				menuCallbackConfig		 ( CCObject* pSender );
		KDvoid				menuCallbackDisabled	 ( CCObject* pSender );
		KDvoid				menuCallbackEnable		 ( CCObject* pSender );
		KDvoid				menuCallback2			 ( CCObject* pSender );
		KDvoid				menuCallbackPriorityTest ( CCObject* pSender );
		KDvoid				menuCallbackRadio		 ( CCObject* pSender );
		KDvoid				menuMovingCallback		 ( CCObject* pSender );

	protected :

		CCMenuItem*			m_pDisabledItem;
};

class MenuLayer2 : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayer2 );

	protected :

		virtual KDbool		init ( KDvoid );

		KDvoid				menuCallback        ( CCObject* pSender );
		KDvoid				menuCallbackOpacity ( CCObject* pSender );
		KDvoid				menuCallbackAlign   ( CCObject* pSender );

		KDvoid				alignMenusH ( KDvoid );
		KDvoid				alignMenusV ( KDvoid );

	protected :

		CCPoint				m_tCenteredMenu;
		KDbool				m_bAlignedH; 
};

class MenuLayer3 : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayer3 );

	protected :

		virtual KDbool		init ( KDvoid );
		
		KDvoid				menuCallback  ( CCObject* pSender );
		KDvoid				menuCallback2 ( CCObject* pSender );
		KDvoid				menuCallback3 ( CCObject* pSender );

	protected :

		CCMenuItem*			m_pDisabledItem;
};

class MenuLayer4 : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayer4 );

	protected :

		virtual KDbool		init ( KDvoid );

		KDvoid				menuCallback ( CCObject* pSender );
		KDvoid				backCallback ( CCObject* pSender );
};

class MenuLayer5 : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayer5 );

	protected :

		virtual KDbool		init ( KDvoid );

		KDvoid				menuCallback ( CCObject* pSender );
		KDvoid				backCallback ( CCObject* pSender );

	protected :

		CCLabelTTF*			m_pLabel;
};

class MenuLayerPriorityTest : public CCLayer
{
	public :

		CREATE_FUNC ( MenuLayerPriorityTest );

	protected :

		virtual  KDbool		init ( KDvoid );

		KDvoid				menuCallback ( CCObject* pSender );

		KDvoid				disableMenuCallback ( CCObject* pSender );

		KDvoid				enableMenuCallback ( KDvoid );

		KDvoid				togglePriorityCallback ( CCObject* pSender );

	private :

		CCMenu*				m_pMenu1;
		CCMenu*				m_pMenu2;
		KDbool				m_bPriority;
};

class RemoveMenuItemWhenMove : public CCLayer
{
	public :

		virtual ~RemoveMenuItemWhenMove ( KDvoid );
	    
		CREATE_FUNC ( RemoveMenuItemWhenMove );

	public :

		virtual KDbool		init ( KDvoid );
		virtual KDvoid		registerWithTouchDispatcher ( KDvoid );
		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
	    
		KDvoid				goBack ( CCObject* pSender );
	    
	private :

		CCMenuItemFont*		m_pItem;
};

#endif	// __TestMenu_h__
