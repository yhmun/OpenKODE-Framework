/* --------------------------------------------------------------------------
 *
 *      File            GameState.h
 *      Description     GameState
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __GameState_h__
#define __GameState_h__

#define GSTAT	GameState::sharedGameState ( )

enum EText 
{
	eTxtTitle		= 0 ,
};

enum EScene
{
	eScrMainMenu		,
	eScrMainGame		,
};

enum ETransition
{
	eTranNull			,
	eTranFade			,
	eTranSlideIn		,
};

class GameState
{
	private :
				 GameState ( KDvoid );
		virtual ~GameState ( KDvoid );

	public :

		static GameState*   sharedGameState ( KDvoid );
		static KDvoid		 purgeGameState ( KDvoid );
    
	public :

		// 게임 속성중에서 텍스트 가져오기 함수
		const KDchar*		getText  ( EText eText );

		KDint				getState ( KDvoid );

		KDbool				setScene ( EScene eScrID, ETransition eTranID = eTranNull );
	
	private :
    
		KDint				m_nState;
};


#endif	// __GameState_h__
