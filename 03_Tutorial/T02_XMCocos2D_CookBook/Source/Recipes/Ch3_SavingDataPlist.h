/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingDataPlist.h
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

#ifndef __Ch3_SavingDataPlist_h__
#define __Ch3_SavingDataPlist_h__

#include "Libraries/SimpleTimedGameRecipe.h"
#include "Libraries/Mole.h"

class Ch3_SavingDataPlist : public SimpleTimedGameRecipe, public MoleDelgate
{
	public :

		SCENE_FUNC ( Ch3_SavingDataPlist );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			onExit ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			addHiScoresToMenu ( KDvoid );
		
		virtual KDvoid			loadHiScores ( KDvoid );
		
		virtual KDvoid			addHiScore ( KDvoid );
		
		virtual KDvoid			deleteHiScores ( KDvoid );
		
		virtual KDvoid			startNewGame ( CCObject* pSender = KD_NULL );
		
		virtual KDvoid			gameOver ( KDvoid );
		
		virtual KDvoid			step ( KDfloat fDelta );

	public :

		virtual KDvoid			processMoleHit ( KDvoid );

	private :

		KDvoid					initBackground ( KDvoid );

		KDvoid					createMoleAtPosition ( const CCPoint& tPoint, KDfloat fZOrder );

	private :

		CCArray*				m_pMoles;
		KDint					m_nTagCount;
		KDint					m_nMoleCount;
		CCSprite*				m_pMallet;
		CCPoint					m_tMalletPosition;
};

#endif	// __Ch3_SavingDataPlist_h__
