/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingDataSQLite.h
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

#ifndef __Ch3_SavingDataSQLite_h__
#define __Ch3_SavingDataSQLite_h__

#include "Libraries/SimpleTimedGameRecipe.h"

class SimpleAnimObject;

class Ch3_SavingDataSQLite : public SimpleTimedGameRecipe
{
	public :

		SCENE_FUNC ( Ch3_SavingDataSQLite );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			onExit ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			addHiScoresToMenu ( KDvoid );
		
		virtual KDvoid			loadHiScores ( KDvoid );
		
		virtual KDvoid			addHiScore ( KDvoid );
		
		virtual KDvoid			deleteHiScores ( KDvoid );
		
		virtual KDvoid			startNewGame ( CCObject* pSender = KD_NULL );

		virtual KDvoid			gameOver ( KDvoid );

		virtual KDvoid			step ( KDfloat fDelta );

		KDvoid					fireDiscAtPosition ( const CCPoint& tPosition, const CCPoint& tVelocity );

		KDvoid					hitDisc ( SimpleAnimObject* pDisc );

		KDvoid					processDiscHit ( KDvoid );

//		CCArray*				createDictionariesArrayFromFMResultSet ( FMResultSet* rs, const KDchar* szFields );

		KDvoid					writeNewScore ( KDint nScore, const KDchar* szName );

		KDvoid					initBackground ( KDvoid );

	private :

		CCSprite*				m_pGun;
		SimpleAnimObject*		m_pBullet;
		CCArray*				m_pDiscs;
		KDint					m_nFireBulletCountdown;
		KDint					m_nDiscRotationNum;
		KDbool					m_bReuseDiscs;		
//		FMDatabase*				m_pDB;
};

#endif	// __Ch3_SavingDataSQLite_h__
