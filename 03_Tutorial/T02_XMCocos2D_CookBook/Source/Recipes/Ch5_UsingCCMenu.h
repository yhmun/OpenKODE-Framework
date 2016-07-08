/* --------------------------------------------------------------------------
 *
 *      File            Ch5_UsingCCMenu.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Ch5_UsingCCMenu_h__
#define __Ch5_UsingCCMenu_h__

class Ch5_UsingCCMenu : public Recipe
{
	public :

		SCENE_FUNC ( Ch5_UsingCCMenu );

	protected :		

		virtual KDbool		init ( KDvoid );

		KDvoid				buttonTouched ( CCObject* pSender );

		KDvoid				enableOptions ( CCObject* pSender );

		KDvoid				options ( CCObject* pSender );

		KDvoid				reAlign ( CCObject* pSender );

	private :

		CCMenu*				m_pMenu;
		CCMenuItemFont*		m_pOptionsItem;
};

// Options Menu
class OptionsMenu : public CCLayer 
{
	public :

		SCENE_FUNC ( OptionsMenu );

	protected :		

		virtual KDbool		init ( KDvoid );

		KDvoid				back ( CCObject* pSender );

		KDvoid				soundToggle ( CCObject* pSender );

		KDvoid				difficultyToggle ( CCObject* pSender );

	private :

		CCLabelBMFont*		m_pMessage;
};

#endif	// __Ch5_UsingCCMenu_h__
