/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBattleNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CasinoBattleNode_h__
#define __CasinoBattleNode_h__

#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/Sprite/SprEffects.h"
#include "Object/Particle/Particle.h"
#include "Object/NodeChageImgs/NodeChangeImgs.h"
#include "Status/CasinoBattleStatus.h"
#include "Unit/Commander/UnitCommanderCasino.h"
#include "Status/CasinoBattleStatus.h"

class cBeadWork : public Object
{
	public :
		cBeadWork ( KDvoid )
		: nSeatIndex		( 0 )
		, nHp				( 0 )
		, nAtk				( 0 )
		, nDef				( 0 )
		, nAgl				( 0 )
		, nCrt				( 0 )
		, dKeepTime			( 0 )
		, nExerciseCount	( 0 )
		, nDamage			( 0 )
		{

		}

		KDint						nSeatIndex;

		KDint						nHp;
		KDint						nAtk;
		KDint						nDef;
		KDint						nAgl;
		KDint						nCrt;

		KDdouble					dKeepTime;
		KDint						nExerciseCount;

		KDint						nDamage;
};

class cBead : public Object
{
	public :
				 cBead ( Node* pNode );
		virtual ~cBead ( KDvoid );

		KDvoid						update ( KDdouble dLeftTime );

		KDbool						bIsInit;
		KDbool						bIsFinish;
		KDbool						bIsMoveUp;

		KDint						nParticleType;
		std::vector<cSprite*>		aSprites;
		std::vector<cParticle*>		aParticleSouls;

		std::vector<cBeadWork*>		aTargetBeadWorks;

		Point						tPoint;
};

class CasinoBattleNode : public Scene
{
	public :
				 CasinoBattleNode ( KDvoid );
		virtual ~CasinoBattleNode ( KDvoid );

		static CasinoBattleNode*	createWithFileLoad ( eIntoType eIntro );

		static CasinoBattleNode*	createWithMobs ( const std::vector<cUnit*>& rMobs, eModeType eType );

		static KDvoid				saveData ( KDvoid );

	public :

		virtual KDvoid				releaseObject ( KDvoid );
			
		virtual KDbool				init ( KDvoid );

		virtual KDbool				initWithFileLoad ( eIntoType eIntro );

		virtual KDbool				initWithMobs ( const std::vector<cUnit*>& rMobs, eModeType eType );

		KDvoid						onExit ( KDvoid );

		KDbool						onTouchBegan ( Touch* pTouch, Event* pEvent ); 

		KDvoid						setMobs ( const std::vector<cUnit*>& rMobs, eModeType eType );

		KDvoid						step ( KDfloat fDelta );

		// save 
		KDvoid						save ( KDvoid );

		// update
		KDbool						updateNodeChage ( KDdouble dLeftTime );
		KDvoid						updateGameStart ( KDdouble dLeftTime );
		KDvoid						updateBead	    ( KDdouble dLeftTime );
		KDvoid						updateParticle  ( KDdouble dLeftTime );

		KDvoid						skipCallback ( Object* pSender );

		KDvoid						survivalExitCallback ( Object* pSender );

		// touch
		KDbool						touchBeadProcess ( Point tPoint );

	private :

		// touch
		KDint						m_nTidCount;
		std::list<sTouch>			m_aListTouch;

		// time
		KDdouble					m_dTickCount;
		KDdouble					m_dBeforeTickCount;

		// node change
		KDbool						m_bIsNodeChangeIn;
		KDbool						m_bIsNodeChangeOut;
		cNodeChangeImgs*			m_pNodeChangeImg;

		// back
		Sprite*						m_pSprBack;

		// back decorations
		std::vector<cSprAni*>		m_aBackDecos;

		// effects
		std::vector<cSprEffects*>	m_aListEffects;

		// particle
		std::list<cParticle*>		m_aParticles;

		// mob commander
		cUnitCommanderCasino*		m_pUnitCommander;

		//game status view
		cCasinoBattleStatus*		m_pStatus;

		//bead
		std::vector<cBead*>			m_aBeads;

		// game start mode
		KDint						m_nGameStartStep;
		KDdouble					m_dGameStartLeftTime;
		cSprite*					m_pSprGameStartText;
		cSprite*					m_pSprGameStartBack;

		// game end mode
		KDbool						m_bIsEnd;
		KDdouble					m_dEndLeftTime;

		// skip
		Menu*						m_pBtnSkip;
		KDbool						m_bIsSkip;

		// load wait time
		KDdouble					m_dLoadWaitLeftTime;

		// switch
		KDbool						m_bIsViewStatus;
		KDbool						m_bIsLastBossMusic;

		// survival
		Menu*						m_pBtnSurvival;
	
		// mode type
		eModeType					m_eModeType;
};

#endif	// __CasinoBattleNode_h__