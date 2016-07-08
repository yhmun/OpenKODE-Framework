/* --------------------------------------------------------------------------
 *
 *      File            Ch5_LoadingScreen.h
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

#ifndef __Ch5_LoadingScreen_h__
#define __Ch5_LoadingScreen_h__

class Ch5_LoadingScreen : public Recipe
{
	public :

		SCENE_FUNC ( Ch5_LoadingScreen );

	protected :		

		virtual KDbool			init ( KDvoid );

		KDvoid					loadLevel ( CCObject* pObject );
};

class LoadingScene : public CCLayer 
{
	public :

		static CCScene*			scene ( const KDchar* szStr );

	public :

		virtual KDbool			initWithLevel ( const KDchar* szStr );

		KDvoid					preloadLevel ( KDvoid );

		KDvoid					preloadSpriteFile ( CCDictionary* pNode );

		KDvoid					runGame ( KDvoid );

	private :

		KDvoid					nodeLoaded ( CCObject* pSender );

	protected :

		std::string				m_sLevelStr;			// Our level filename
		KDuint					m_uNodesLoaded;			// Number of nodes currently loaded
		KDuint					m_uNodesToLoad;			// Total number of nodes to load
		CCLabelBMFont*			m_pLoadingMessage;		// Message top show the user
		CCSprite*				m_pLoadingBar;			// Bar indicating loading progress
};

class GameScene : public CCLayer 
{
	public :

		static CCScene*			scene ( const KDchar* szStr );

	public :

		virtual KDbool			initWithLevel ( const KDchar* szStr );

		KDvoid					loadLevel ( const KDchar* szStr );

		KDvoid					processSpriteFile ( CCDictionary* pNode );

		KDvoid					quit ( CCObject* pSender );
};

#endif	// __Ch5_LoadingScreen_h__
