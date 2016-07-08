/* --------------------------------------------------------------------------
 *
 *      File            Ch7_LuaDecisionTree.h
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

#ifndef __Ch7_LuaDecisionTree_h__
#define __Ch7_LuaDecisionTree_h__

#include "Libraries/mcLua.h"

class Ch7_LuaDecisionTree : public Recipe
{
	public :

		SCENE_FUNC ( Ch7_LuaDecisionTree );

	public :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				addSpriteFrame ( const KDchar* szName, KDint nZOrder, KDint nTag, KDbool bVisible );

		KDvoid				addSpriteFile ( const KDchar* szName, KDint nZOrder, KDint nTag, KDbool bVisible );

		KDvoid				step ( KDfloat fDelta );

		KDvoid				showText ( const KDchar* szStr );

		KDvoid				desc ( const KDchar* szStr );

		KDvoid				anim ( const KDchar* szStr );

		KDvoid				dialog ( const KDchar* szStr );

		KDint				logic ( const KDchar* szStr );

		KDvoid				dialogOption ( const KDchar* szStr );

		KDvoid				actionOption( const KDchar* szStr );

		KDvoid				presentOptions ( KDvoid );

		KDvoid				selectOption ( CCObject* pSender );

	public :

		mcLuaManager*		m_pLua;
		mcLuaScript*		m_pScript;
		KDint				m_nResponse;
		CCLabelBMFont*		m_pTextLabel;
		std::string			m_sText;
		std::string			m_sBufferString;
		KDfloat				m_fTimeElapsed;
		KDbool				m_bDialogForward;
		KDbool				m_bCanMoveForward;
		KDbool				m_bWrapNext;
			
		KDint				m_nOptionsOnScreen;
		CCNode*				m_pOptionsNode;
	
		KDbool				m_bGunsDown;
};

#endif	// __Ch7_LuaDecisionTree_h__
