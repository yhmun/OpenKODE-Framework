/* --------------------------------------------------------------------------
 *
 *      File            Ch5_SceneTransitions.h
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

#ifndef __Ch5_SceneTransitions_h__
#define __Ch5_SceneTransitions_h__

// Our Base Recipe
class Ch5_SceneTransitions : public Recipe
{
	public :

		SCENE_FUNC ( Ch5_SceneTransitions );

	protected :		

		virtual KDbool		init ( KDvoid );

		KDvoid				viewTransitions ( CCObject* pSender );
};

// TransSceneMenu
// Our scene class
class TransSceneMenu : public CCLayer 
{
	public :	
	
		static	CCScene*	sceneWithString ( const KDchar* szStr, KDint nCurrentTransition );

	public :

		virtual KDbool		initWithString ( const KDchar* szStr, KDint nCurrentTransition );

		KDvoid				prevScene ( CCObject* pSender );

		KDvoid				nextScene ( CCObject* pSender );

		KDvoid				randomScene ( CCObject* pSender );

		KDvoid				quit ( CCObject* pSender );

		KDvoid				loadNewScene ( KDvoid );

	protected :

		KDint				m_nCurrentTransition;
		std::string			m_sName;
};

#endif	// __Ch5_SceneTransitions_h__
