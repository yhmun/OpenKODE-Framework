/* --------------------------------------------------------------------------
 *
 *      File            CookBookTest.h
 *      Ported By       Young-Hwan Mun, Jin-Su Park
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

#ifndef __CookBookTest_h__
#define __CookBookTest_h__

#include "testBasic.h"
#include "BaseTest.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

using namespace extension;

class CookBookTestScene : public TestScene
{
	public :

		virtual KDvoid				runThisTest ( KDvoid );
};

class CookBookTestDemo : public BaseTest
{
	public :

		virtual KDvoid				onEnter ( KDvoid );
    
		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		virtual KDvoid				restartCallback ( Object* pSender );
		virtual KDvoid				nextCallback	( Object* pSender );
		virtual KDvoid				backCallback	( Object* pSender );
};
    
// GameDPadTest

class GameDPadTest : public CookBookTestDemo
{
	public :

		virtual KDvoid				onEnter  ( KDvoid );
    
		virtual std::string			title	 ( KDvoid );
		virtual std::string			subtitle ( KDvoid );

		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

	private :

		GameDPad*					m_pGameDPad;
};

// GameAPadTest

class GameAPadTest : public CookBookTestDemo
{
	public :

		virtual KDvoid				onEnter  ( KDvoid );
    
		virtual std::string			title	 ( KDvoid );
		virtual std::string			subtitle ( KDvoid );

		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

	private :

		GameAPad*					m_pGameAPad;
};


class GameButtonTest : public CookBookTestDemo
{
	public :

		virtual KDvoid				onEnter  ( KDvoid );
    
		virtual std::string			title	 ( KDvoid );
		virtual std::string			subtitle ( KDvoid );

		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		KDvoid						createButtonWithPosition ( const Point& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName );

	private :

		std::vector<GameButton*>	m_aGameButtons;
};

#endif	// __CookBookTest_h__
