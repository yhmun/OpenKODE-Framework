/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBattleNode.cpp
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

#include "Precompiled.h"
#include "CasinoBattleNode.h"
#include "MyData/MyData.h"
#include "CasinoNode/CasinoResultNode/CasinoResultNode.h"

#define DEBUG_GAME			1

// static
static CasinoBattleNode*	l_pCasinoBattleNode = nullptr;


//******************************************* cBead *******************************************//


cBead::cBead ( Node* pNode )
: bIsInit			( false			)
, bIsFinish			( false			)
, bIsMoveUp			( true			)
, nParticleType		( 0				)
, tPoint			( Point::ZERO	)
{
	// sprite
	aSprites.push_back ( new cSprite ( pNode, "Game/soulBead_blue.png" , LAYER_GAMEVIEW_EFFECT_SOUL, Point ( 0.5f, 0.5f ), Point::ZERO ) );
	aSprites.push_back ( new cSprite ( pNode, "Game/soulBead_red.png"  , LAYER_GAMEVIEW_EFFECT_SOUL, Point ( 0.5f, 0.5f ), Point::ZERO ) );
	aSprites.push_back ( new cSprite ( pNode, "Game/soulBead_green.png", LAYER_GAMEVIEW_EFFECT_SOUL, Point ( 0.5f, 0.5f ), Point::ZERO ) );
	aSprites.push_back ( new cSprite ( pNode, "Game/soulBead_white.png", LAYER_GAMEVIEW_EFFECT_SOUL, Point ( 0.5f, 0.5f ), Point::ZERO ) );

	for ( auto pNode : aSprites )	
	{
		pNode->get ( )->setVisible ( false );
	}

	// particle
	aParticleSouls.push_back ( new cParticle ( pNode, "bead_para.plist"  , LAYER_GAMEVIEW_EFFECT_SOUL, Point ( -100, -100 ), false, 0 ) );
	aParticleSouls.push_back ( new cParticle ( pNode, "bead_attack.plist", LAYER_GAMEVIEW_EFFECT_SOUL, Point ( -100, -100 ), false, 0 ) );
	aParticleSouls.push_back ( new cParticle ( pNode, "bead_poison.plist", LAYER_GAMEVIEW_EFFECT_SOUL, Point ( -100, -100 ), false, 0 ) );
	aParticleSouls.push_back ( new cParticle ( pNode, "bead_heal.plist"  , LAYER_GAMEVIEW_EFFECT_SOUL, Point ( -100, -100 ), false, 0 ) );

	for ( auto pNode : aParticleSouls )
	{
		pNode->get ( )->setVisible ( false );
	}
}

cBead::~cBead ( KDvoid )
{
	SAFE_RELEASE_ARRAYS ( aSprites );
	SAFE_RELEASE_ARRAYS ( aParticleSouls );
	SAFE_RELEASE_ARRAYS ( aTargetBeadWorks );
}

KDvoid cBead::update ( KDdouble dLeftTime )
{
	for ( KDuint i = 0; i < aSprites.size ( ); i++ )
	{
		if ( aSprites [ i ]->get ( )->isVisible ( ) == false )
		{
			continue;
		}

		if ( bIsInit == true )
		{
			if ( bIsMoveUp == true )
			{
				Point	tPoint = aSprites [ i ]->get ( )->getPosition ( );
				tPoint.y += dLeftTime * 4;

				if ( tPoint.y > tPoint.y + 3 )
				{
					tPoint.y  = tPoint.y + 3;
					bIsMoveUp = false;
				}

				aSprites       [ i ]->get ( )->setPosition ( tPoint );
				aParticleSouls [ i ]->get ( )->setPosition ( tPoint );
			}
			else
			{
				Point	tPoint = aSprites [ i ]->get ( )->getPosition ( );
				tPoint.y -= dLeftTime * 4;

				if ( tPoint.y < tPoint.y )
				{
					tPoint.y  = tPoint.y;
					bIsMoveUp = true;
				}

				aSprites       [ i ]->get ( )->setPosition ( tPoint );
				aParticleSouls [ i ]->get ( )->setPosition ( tPoint );
			}
		}
	}
}

//******************************************* CasinoBattleNode *******************************************//

CasinoBattleNode* CasinoBattleNode::createWithFileLoad ( eIntoType eIntro )
{
	CasinoBattleNode*	pRet = new CasinoBattleNode ( );

	if ( pRet && pRet->initWithFileLoad ( eIntro ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CasinoBattleNode* CasinoBattleNode::createWithMobs ( const std::vector<cUnit*>& rMobs, eModeType eType )
{
	CasinoBattleNode*	pRet = new CasinoBattleNode ( );

	if ( pRet && pRet->initWithMobs ( rMobs, eType ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CasinoBattleNode::CasinoBattleNode ( KDvoid )
{
	l_pCasinoBattleNode = this;

	m_pNodeChangeImg	= nullptr;
	m_pSprBack			= nullptr;
	m_pUnitCommander	= nullptr;
	m_pStatus			= nullptr;
	m_pSprGameStartBack = nullptr;
	m_pSprGameStartText = nullptr;	
	m_pBtnSkip			= nullptr;
	m_pBtnSurvival		= nullptr;	
}

CasinoBattleNode::~CasinoBattleNode ( KDvoid )
{
	this->releaseObject ( );

	// static node
	l_pCasinoBattleNode = nullptr;
}

KDvoid CasinoBattleNode::releaseObject ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNodeChangeImg );

	// back img
	SAFE_REMOVE_CHILD ( m_pSprBack );

	// back decorations
	SAFE_RELEASE_ARRAYS ( m_aBackDecos );

	// btns
	SAFE_REMOVE_CHILD ( m_pBtnSkip );
	SAFE_REMOVE_CHILD ( m_pBtnSurvival );

	// effects
	SAFE_RELEASE_ARRAYS ( m_aListEffects );

	// particles
	SAFE_RELEASE_ARRAYS ( m_aParticles );

	// bead
	SAFE_RELEASE_ARRAYS ( m_aBeads );

	// object
	CC_SAFE_RELEASE ( m_pStatus );
	CC_SAFE_RELEASE ( m_pSprGameStartBack );
	CC_SAFE_RELEASE ( m_pSprGameStartText );

	CC_SAFE_RELEASE ( m_pUnitCommander );
}

KDbool CasinoBattleNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// static node
	l_pCasinoBattleNode = this;

	// switch
	auto	pListener = EventListenerTouchOneByOne::create ( );
	pListener->onTouchBegan = CC_CALLBACK_2 ( CasinoBattleNode::onTouchBegan, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListener, this );

	//touch
	m_nTidCount = 1;

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount = 0;

	// back
	m_pSprBack = nullptr;

	// effects
	m_aListEffects.push_back ( new cSprEffects ( this, LAYER_GAMEVIEW_EFFECT_1, LAYER_GAMEVIEW_EFFECT_2 ) );

	// mob commander
	m_pUnitCommander = nullptr;

	//game start mode
	m_nGameStartStep = 0;
	m_dGameStartLeftTime = 0.7f;

	m_pSprGameStartBack	= new cSprite ( this, "Game/game_start_back.png", LAYER_GAMEVIEW_START_B, Point::ZERO, Point ( 480, 129 ) );
	m_pSprGameStartText	= new cSprite ( this, "Game/game_start_text.png", LAYER_GAMEVIEW_START_T, Point::ZERO, Point ( 120, 144 ) );

	m_pSprGameStartBack->get ( )->setVisible ( true );
	m_pSprGameStartText->get ( )->setVisible ( false );

	m_pSprGameStartBack->setPointTime ( Point ( 0, 129 ), 0.2f );

	// skip
	m_pBtnSkip = Menu::createWithItem ( MenuItemImage::create ( "Game/game_btn_skip.png", "Game/game_btn_skip_push.png", CC_CALLBACK_1 ( CasinoBattleNode::skipCallback, this ) ) );
	
	m_pBtnSkip->setVisible ( false );
	m_pBtnSkip->setEnabled ( false );
	m_pBtnSkip->setAnchorPoint ( Point::ZERO );
	m_pBtnSkip->setPosition ( Point ( 421, 30 ) );

	this->addChild ( m_pBtnSkip, LAYER_GAMEVIEW_INFO );

	m_bIsSkip = false;

	// game end
	m_bIsEnd = false;
	m_dEndLeftTime = 0;

	// node change
	m_bIsNodeChangeIn  = true;
	m_bIsNodeChangeOut = false;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, false );

	// background music
	if ( cMyData::getObj ( )->m_bIsOptionSound != false )
	{
		SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "bm_battle_3.mp3", true );
	}

	// load wait time
	m_dLoadWaitLeftTime = 0;

	// switch
	m_bIsViewStatus	   = true;
	m_bIsLastBossMusic = false;

	// survival
	m_pBtnSurvival = Menu::createWithItem ( MenuItemImage::create ( "Game/survival_kill_exit.png", "Game/survival_kill_exit_push.png", CC_CALLBACK_1 ( CasinoBattleNode::survivalExitCallback, this ) ) );
	
	m_pBtnSurvival->setVisible ( false );
	m_pBtnSurvival->setEnabled ( false );
	m_pBtnSurvival->setAnchorPoint ( Point::ZERO );
	m_pBtnSurvival->setPosition ( Point ( 3 + 24, 299 + 8.5f ) );

	this->addChild ( m_pBtnSurvival, LAYER_GAMEVIEW_INFO );
	
	// gaming exp & kill count
	cMyData::getObj ( )->m_fGamingExp = 0;
	cMyData::getObj ( )->m_nGamingKillCount = 0;

	// schedule
	this->schedule ( schedule_selector ( CasinoBattleNode::step ) );

	return true;
}

KDbool CasinoBattleNode::initWithFileLoad ( eIntoType eIntro )
{
	if ( !this->init ( ) )
	{
		return false;
	}

	// path
	const KDchar*	pPath = nullptr;
	if ( eIntro == eIntoType_Casino )	pPath = _FILE_NAME_CASINO_BATTLE_SAVE;
	else								pPath = _FILE_NAME_MYROOM_BATTLE_SAVE;

	// open
	KDFile*			pFile = kdFopen ( pPath, "rb" );
	if ( pFile != nullptr )
	{
		// version
		KDint		nVersionStrCount = 0;
		kdFread ( &nVersionStrCount, sizeof ( KDint ), 1, pFile );

		KDchar*		pVersionStr = new KDchar [ nVersionStrCount + 1 ];
		kdFread ( pVersionStr, sizeof ( KDchar ), nVersionStrCount + 1, pFile );
		delete pVersionStr;

		// mode type
		KDint		nModeType = 0;
		KDint		nUnitCreateCount = 0;
		KDdouble	dUnitCreateLeftTime = 0;
		kdFread ( &nModeType								, sizeof ( KDint    ), 1, pFile );
		kdFread ( &nUnitCreateCount							, sizeof ( KDint    ), 1, pFile );
		kdFread ( &dUnitCreateLeftTime						, sizeof ( KDdouble ), 1, pFile );
		kdFread ( &cMyData::getObj ( )->m_fGamingExp		, sizeof ( KDfloat  ), 1, pFile );
		kdFread ( &cMyData::getObj ( )->m_nGamingKillCount	, sizeof ( KDint    ), 1, pFile );
		m_eModeType = (eModeType)nModeType;

		// mob count
		KDint		nMobCount = 0;
		kdFread ( &nMobCount, sizeof ( KDint ), 1, pFile );

		//i nit rMobs
		std::vector<cUnit*>		aMobs;

		for ( KDint i = 0; i < 5; i++ )
		{
			aMobs.push_back ( nullptr );
		}

		// mob loop
		for ( KDint i = 0; i < nMobCount; i++ )
		{
			// index & para
			KDint		nCreateIndex		= 0;
			KDint		nSeatIndex			= 0;
			KDint		nCreateInfoFile		= 0;
			KDint		nUniqueType			= 0;
			KDint		nSlotIndex			= 0;
			KDint		nUnitID				= 0;
			KDfloat		fExp				= 0;
			KDfloat		fClassExp			= 0;
			KDfloat		fFriendShipExp		= 0;
			KDint		nKillCount			= 0;
			KDint		nIsArcadeEasyClear	= 0;
			KDint		nIsArcadeNorClear	= 0;
			KDint		nIsArcadeHardClear	= 0;
			
			KDint		nHp		= 0;
			KDint		nUseHp	= 0;
			KDint		nAtk	= 0;
			KDint		nDef	= 0;
			KDint		nAgl	= 0;
			KDint		nCrt	= 0;
			KDint		nHpMax	= 0;
			KDint		nAtkMax = 0;
			KDint		nDefMax = 0;
			KDint		nAglMax	= 0;
			KDint		nCrtMax = 0;

			kdFread ( &nCreateIndex		  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nSeatIndex		  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nCreateInfoFile	  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nUniqueType		  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nSlotIndex		  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nUnitID			  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &fExp				  , sizeof ( KDfloat ), 1, pFile );
			kdFread ( &fClassExp		  , sizeof ( KDfloat ), 1, pFile );
			kdFread ( &fFriendShipExp	  , sizeof ( KDfloat ), 1, pFile );
			kdFread ( &nKillCount		  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nIsArcadeEasyClear , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nIsArcadeNorClear  , sizeof ( KDint	 ), 1, pFile );
			kdFread ( &nIsArcadeHardClear , sizeof ( KDint	 ), 1, pFile );

			kdFread ( &nHp	   , sizeof ( KDint ), 1, pFile );
			kdFread ( &nUseHp  , sizeof ( KDint ), 1, pFile );
			kdFread ( &nAtk    , sizeof ( KDint ), 1, pFile );
			kdFread ( &nDef    , sizeof ( KDint ), 1, pFile );
			kdFread ( &nAgl    , sizeof ( KDint ), 1, pFile );
			kdFread ( &nCrt	   , sizeof ( KDint ), 1, pFile );
			kdFread ( &nHpMax  , sizeof ( KDint ), 1, pFile );
			kdFread ( &nAtkMax , sizeof ( KDint ), 1, pFile );
			kdFread ( &nDefMax , sizeof ( KDint ), 1, pFile );
			kdFread ( &nAglMax , sizeof ( KDint ), 1, pFile );
			kdFread ( &nCrtMax , sizeof ( KDint ), 1, pFile );

			// init papa adds
			std::vector<cParaAdd>	aUseAdds;

			// para add count
			KDint		nParaAddCount = 0;
			kdFread ( &nParaAddCount, sizeof ( KDint ), 1, pFile );

			// para add loop
			for ( KDint a = 0; a < nParaAddCount; a++ )
			{
				cParaAdd	tParaAdd;
				kdFread ( &tParaAdd, sizeof ( cParaAdd ), 1, pFile );

				aUseAdds.push_back ( tParaAdd );
			}

			// die
			KDint		nIsDie = false;
			kdFread ( &nIsDie, sizeof ( KDint ), 1, pFile );

			// bet init
			KDint64		nBetMoney		  = 0;
			KDfloat		fBetMagnification = 0;

			kdFread ( &nBetMoney		, sizeof ( KDint64 ), 1, pFile );
			kdFread ( &fBetMagnification, sizeof ( KDfloat ), 1, pFile );

			// now animation
			KDint		nAniType	 = 0;
			KDint		nAniNowFrame = 0;

			kdFread ( &nAniType	   , sizeof ( KDint ), 1, pFile );
			kdFread ( &nAniNowFrame, sizeof ( KDint ), 1, pFile );

			// create info
			cUnitCreateInfos*	pMobCreate = nullptr;
			switch(nCreateInfoFile)
			{
				case eCreateInfoFileType_Coin_1					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_1.par"	 );		break;
				case eCreateInfoFileType_Coin_2					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_2.par"	 );		break;
				case eCreateInfoFileType_Coin_3					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_3.par"	 );		break;
				case eCreateInfoFileType_Coin_4					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_4.par"	 );		break;
				case eCreateInfoFileType_Coin_5					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_5.par"	 );		break;
				case eCreateInfoFileType_Coin_6					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_6.par"	 );		break;
				case eCreateInfoFileType_Coin_7					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_7.par"	 );		break;
				case eCreateInfoFileType_Coin_8					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_8.par"	 );		break;
				case eCreateInfoFileType_Coin_9					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_9.par"	 );		break;
				case eCreateInfoFileType_Coin_10				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_10.par"	 );		break;
				case eCreateInfoFileType_Coin_11				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_11.par"	 );		break;
				case eCreateInfoFileType_Coin_12				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_12.par"	 );		break;
				case eCreateInfoFileType_Coin_13				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_13.par"	 );		break;
				case eCreateInfoFileType_Premium_1				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_1.par" );		break;
				case eCreateInfoFileType_Premium_2				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_2.par" );		break;
				case eCreateInfoFileType_Premium_3				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_3.par" );		break;
				case eCreateInfoFileType_Premium_4				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_4.par" );		break;
				case eCreateInfoFileType_Casino_1				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos.par"			 );		break;
				case eCreateInfoFileType_Casino_2				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos2.par"			 );		break;
				case eCreateInfoFileType_Casino_3				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos3.par"			 );		break;
				case eCreateInfoFileType_Casino_4				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos4.par"			 );		break;
				case eCreateInfoFileType_ArcadeEasy				:	pMobCreate = new cUnitCreateInfos ( "arcade_easy.par"					 );		break;
				case eCreateInfoFileType_ArcadeEasyMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_easy_middle_boss.par"		 );		break;
				case eCreateInfoFileType_ArcadeEasyLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_easy_last_boss.par"			 );		break;
				case eCreateInfoFileType_ArcadeNor				:	pMobCreate = new cUnitCreateInfos ( "arcade_normal.par"					 );		break;
				case eCreateInfoFileType_ArcadeNorMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_normal_middle_boss.par"		 );		break;
				case eCreateInfoFileType_ArcadeNorLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_normal_last_boss.par"		 );		break;
				case eCreateInfoFileType_ArcadeHard				:	pMobCreate = new cUnitCreateInfos ( "arcade_hard.par"					 );		break;
				case eCreateInfoFileType_ArcadeHardMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_hard_middle_boss.par"		 );		break;
				case eCreateInfoFileType_ArcadeHardLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_hard_last_boss.par"			 );		break;
				case eCreateInfoFileType_Survival				:	pMobCreate = new cUnitCreateInfos ( "survival.par"						 );		break;
			}

			// init mob
			cUnit*		pMob = new cUnit ( this, pMobCreate->getUnitInfo ( nCreateIndex ), (eCreateInfoFileType) nCreateInfoFile, LAYER_GAMEVIEW_MOB, nUnitID, nCreateIndex, nSeatIndex );

			// set unit info
			pMob->setUniqueType			( (eUniqueType) nUniqueType	);
			pMob->setSlotIndex			( nSlotIndex				);
			pMob->setExp				( fExp						);
			pMob->setClassExp			( fClassExp					);
			pMob->setFriendShipExp		( fFriendShipExp			);
			pMob->setKillCount			( nKillCount				);
			pMob->setIsArcadeEasyClear	( nIsArcadeEasyClear 		);
			pMob->setIsArcadeNorClear	( nIsArcadeNorClear 		);
			pMob->setIsArcadeHardClear	( nIsArcadeHardClear		);

			// set para
			pMob->setHP		( nHp	  );
			pMob->setUseHp	( nUseHp  );
			pMob->setATK	( nAtk	  );
			pMob->setDEF	( nDef	  );
			pMob->setAGL	( nAgl	  );
			pMob->setCRT	( nCrt	  );
			pMob->setHPMax	( nHpMax  );
			pMob->setATKMax	( nAtkMax );
			pMob->setDEFMax	( nDefMax );
			pMob->setAGLMax	( nAglMax );
			pMob->setCRTMax	( nCrtMax );

			// set para adds
			pMob->setParaAdds ( aUseAdds );

			// set bet info
			pMob->setBetMoney			   ( nBetMoney		   );
			pMob->setDividendMagnification ( fBetMagnification );

			// load texture
			if ( nBetMoney > 0 )	
			{
				pMob->texLoad ( eUnitAniType_Portrait );
			}

			pMob->texLoad ( eUnitAniType_Normal  );
			pMob->texLoad ( eUnitAniType_Attack  );
			pMob->texLoad ( eUnitAniType_Special );
			pMob->texLoad ( eUnitAniType_Damage  );

			// set now animation
			pMob->changeAni ( (eUnitAniType) nAniType );
			pMob->aniStart ( );
			pMob->setAniNowFrame ( nAniNowFrame );

			// set die
			if ( nIsDie == 1 )
			{
				pMob->dieNoEffect ( );
			}

			// exp
			if ( pMob->getUnitID ( ) > 0 )
			{
				// my data get exp
				for ( auto& rSaveData : cMyData::getObj ( )->m_aMyRoomUnitSaveDatas )
				{
					if (  rSaveData.nUnitID == pMob->getUnitID ( ) )
					{
						pMob->setExp ( rSaveData.fExp );
					}
				}

				// level up
				while ( pMob->getExp ( ) >= 100 )
				{
					// para
					KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX -_MYROOM_LEVELUP_POINT_HP_MIN  );
					KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX-_MYROOM_LEVELUP_POINT_ATK_MIN );
					KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX-_MYROOM_LEVELUP_POINT_DEF_MIN );
					KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX-_MYROOM_LEVELUP_POINT_AGL_MIN );
					KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX-_MYROOM_LEVELUP_POINT_CRT_MIN );
					pMob->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

					//exp
					pMob->setExp ( pMob->getExp ( ) - 100 );
				}
			}				

			// set
			aMobs [ pMob->getSeatIndex ( ) ] = pMob;
		}

		// init unit commander
		m_pUnitCommander = new cUnitCommanderCasino ( this, m_aListEffects, aMobs, m_eModeType );

		// unit create count
		m_pUnitCommander->setUnitCreateCount ( nUnitCreateCount );

		// unit create first left time
		m_pUnitCommander->setUnitCreateLeftTime ( dUnitCreateLeftTime );

		// init active gages
		std::vector<KDfloat>		aActiveGages;

		// active gage count
		KDint		nActiveGageCount = 0;
		kdFread ( &nActiveGageCount, sizeof ( KDint ), 1, pFile );

		// active gage loop
		for ( KDint i = 0; i < nActiveGageCount; i++ )
		{
			KDfloat		fGage = 0;
			kdFread ( &fGage, sizeof ( KDfloat ), 1, pFile );

			aActiveGages.push_back ( fGage );
		}

		// set active gage
		m_pUnitCommander->setMobActiveGages ( aActiveGages );

		// init active gage
		std::vector<sActive>		aActives;

		// active count
		KDint		nActiveCount = 0;
		kdFread ( &nActiveCount, sizeof ( KDint ), 1, pFile );

		// loop active
		for ( KDint i = 0; i < nActiveCount; i++ )
		{
			sActive		tActive;

			// base
			kdFread ( &tActive.eUnit				, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nUnitID				, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nSeatIndex			, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nActionIndex			, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nScriptIndex			, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nSelectHeroID		, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.nSelectMobSeatIndex	, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.eRange				, sizeof ( KDint	), 1, pFile );
			kdFread ( &tActive.dLeftTime			, sizeof ( KDdouble ), 1, pFile );
			kdFread ( &tActive.bIsActionStart		, sizeof ( KDbool	), 1, pFile );

			// target hero id & mob seatIndex count
			KDint		nTargetHeroIDCount	 = 0;
			KDint		nTargetMobIndexCount = 0;
			kdFread ( &nTargetHeroIDCount  , sizeof ( KDint ), 1, pFile );
			kdFread ( &nTargetMobIndexCount, sizeof ( KDint ), 1, pFile );

			// loop target hero id & mob seatIndex count
			for ( KDint a = 0; i < nTargetHeroIDCount; a++ )
			{
				KDint	nHeroID = 0;
				kdFread ( &nHeroID, sizeof ( KDint ), 1, pFile );

				tActive.aTargetHeroIDs.push_back ( nHeroID );
			}
			for ( KDint a = 0; a < nTargetMobIndexCount; a++ )
			{
				KDint	nMobSeatIndex = 0;
				kdFread ( &nMobSeatIndex, sizeof ( KDint ), 1, pFile );

				tActive.aTargetMobIndexs.push_back ( nMobSeatIndex );
			}

			// push active
			aActives.push_back ( tActive );
		}

		// unit commander set active
		m_pUnitCommander->setActives ( aActives );

		// init time effects
		std::vector<sTimeEffect>	aTimeEffects;

		// time effect count
		KDint		nTimeEffectCount = 0;
		kdFread ( &nTimeEffectCount, sizeof ( KDint ), 1, pFile );

		// loop time effect
		for ( KDint i = 0; i < nTimeEffectCount; i++)
		{
			sTimeEffect		tTimeEffect;

			kdFread ( &tTimeEffect.eUnit		, sizeof ( KDint    ), 1, pFile );
			kdFread ( &tTimeEffect.nUnitID		, sizeof ( KDint	), 1, pFile );
			kdFread ( &tTimeEffect.nSeatIndex	, sizeof ( KDint	), 1, pFile );
			kdFread ( &tTimeEffect.nEffectIndex	, sizeof ( KDint	), 1, pFile );
			kdFread ( &tTimeEffect.tPoint		, sizeof ( Point	), 1, pFile );
			kdFread ( &tTimeEffect.dTime		, sizeof ( KDdouble ), 1, pFile );
			kdFread ( &tTimeEffect.dLeftTime	, sizeof ( KDdouble ), 1, pFile );
			kdFread ( &tTimeEffect.nLeftCount	, sizeof ( KDint	), 1, pFile );

			KDint		nEffectNameCount = 0;
			kdFread ( &nEffectNameCount, sizeof ( KDint ), 1, pFile );

			KDchar*		pEffectName = new KDchar [ nEffectNameCount + 1 ];
			kdFread ( pEffectName, sizeof ( KDchar ), nEffectNameCount + 1, pFile );

			tTimeEffect.sEffectName = pEffectName;

			aTimeEffects.push_back ( tTimeEffect );
		}

		// unit commander set time effect
		m_pUnitCommander->setTimeEffects ( aTimeEffects );

		// 연승.
		kdFread ( &cMyData::getObj ( )->m_nSuccessive, sizeof ( KDint ), 1, pFile );

		// soul Bead count
		KDint		nSoulBeadCount = 0;
		kdFread ( &nSoulBeadCount, sizeof ( KDint ), 1, pFile );

		// loop Bead count
		for ( KDint i = 0; i < nSoulBeadCount; i++ )
		{
			cBead*		pBead = new cBead ( this );

			kdFread ( &pBead->bIsInit	   , sizeof ( KDbool ), 1, pFile );
			kdFread ( &pBead->bIsFinish	   , sizeof ( KDbool ), 1, pFile );
			kdFread ( &pBead->nParticleType, sizeof ( KDint  ), 1, pFile );
			kdFread ( &pBead->tPoint	   , sizeof ( Point	 ), 1, pFile );

			KDint		nSoulBeadWorkCount = 0;
			kdFread ( &nSoulBeadWorkCount, sizeof ( KDint ), 1, pFile );
			
			for ( KDint a = 0; a < nSoulBeadWorkCount; a++ )
			{
				cBeadWork*		pBeadWork = new cBeadWork ( );

				kdFread ( &pBeadWork->nSeatIndex	, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nHp			, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nAtk			, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nDef			, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nAgl			, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nCrt			, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->dKeepTime		, sizeof ( KDdouble ), 1, pFile );
				kdFread ( &pBeadWork->nExerciseCount, sizeof ( KDint    ), 1, pFile );
				kdFread ( &pBeadWork->nDamage		, sizeof ( KDint    ), 1, pFile );

				pBead->aTargetBeadWorks.push_back ( pBeadWork );
			}

			// sprite & particle
			if ( pBead->bIsInit == true && pBead->bIsFinish == false )
			{
				pBead->aSprites		  [ pBead->nParticleType ]->get ( )->setVisible  ( true );
				pBead->aSprites		  [ pBead->nParticleType ]->get ( )->setPosition ( pBead->tPoint );
				pBead->aParticleSouls [ pBead->nParticleType ]->get ( )->setVisible  ( true );
				pBead->aParticleSouls [ pBead->nParticleType ]->get ( )->setPosition ( pBead->tPoint );
			}

			m_aBeads.push_back ( pBead );
		}

		// close
		kdFclose ( pFile );
	}

	// 파일 삭제.
	kdUnlink ( _FILE_NAME_CASINO_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_CASINO_RESULT_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_RESULT_SAVE );

	// back
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		// back
		switch ( m_eModeType )
		{
			case eModeType_Casino				:	
			case eModeType_MyRoom_Casino100		:	
			case eModeType_MyRoom_Casino1000	:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
			case eModeType_MyRoom_Arcade_Easy	:	m_pSprBack = Sprite::create ( "Game/Back/bg_easy.png"		);	break;
			case eModeType_MyRoom_Arcade_Nor	:	m_pSprBack = Sprite::create ( "Game/Back/bg_normal.png"		);	break;
			case eModeType_MyRoom_Arcade_Hard	:	m_pSprBack = Sprite::create ( "Game/Back/bg_hard.png"		);	break;
			case eModeType_MyRoom_Survival		:	m_pSprBack = Sprite::create ( "Game/Back/bg_survival.png"	);	break;
			case eModeType_End					:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
		}
		
		m_pSprBack->setPosition	( Point::ZERO );
		m_pSprBack->setAnchorPoint	( Point::ZERO );
		this->addChild ( m_pSprBack , LAYER_BACK );

		// back decorations
		cSprAni*	pAniDeco1 = new cSprAni ( this, "Game/Back/game_back_1_fire_a.png", LAYER_BACK_DECO, Point (  22, 232 ) );
		cSprAni*	pAniDeco2 = new cSprAni ( this, "Game/Back/game_back_1_fire_b.png", LAYER_BACK_DECO, Point ( 318, 166 ) );

		pAniDeco1->addFrame ( Rect ( 0,   0, 138, 66 ), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0,  66, 138, 66 ), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0, 132, 138, 66 ), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0, 198, 138, 66 ), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

		pAniDeco2->addFrame ( Rect ( 0,   0, 142, 66 ), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0,  66, 142, 65 ), 120, false, false, Point ( 0, 65 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0, 131, 142, 66 ), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0, 197, 142, 66 ), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );

		pAniDeco1->setFrameInit ( 0 );
		pAniDeco2->setFrameInit ( 0 );
		pAniDeco1->setLoop ( true );
		pAniDeco2->setLoop ( true );
		pAniDeco1->start ( );
		pAniDeco2->start ( );

		m_aBackDecos.push_back ( pAniDeco1 );
		m_aBackDecos.push_back ( pAniDeco2 );
	}
	else
	{
		// back
		switch ( m_eModeType )
		{
			case eModeType_Casino				:	
			case eModeType_MyRoom_Casino100		:	
			case eModeType_MyRoom_Casino1000	:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
			case eModeType_MyRoom_Arcade_Easy	:	m_pSprBack = Sprite::create ( "Game/Back/bg_easy.png"		);	break;
			case eModeType_MyRoom_Arcade_Nor	:	m_pSprBack = Sprite::create ( "Game/Back/bg_normal.png"		);	break;
			case eModeType_MyRoom_Arcade_Hard	:	m_pSprBack = Sprite::create ( "Game/Back/bg_hard.png"		);	break;
			case eModeType_MyRoom_Survival		:	m_pSprBack = Sprite::create ( "Game/Back/bg_survival.png"	);	break;
			case eModeType_End					:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
		}

		m_pSprBack->setPosition	( Point::ZERO );
		m_pSprBack->setAnchorPoint ( Point::ZERO );
		this->addChild ( m_pSprBack , LAYER_BACK );
	}

	// game status view
	m_pStatus = new cCasinoBattleStatus ( this, m_pUnitCommander->getMyMob ( ), m_eModeType );

	//load wait time
	m_dLoadWaitLeftTime = 1;

	//start effect enable false
	m_nGameStartStep = -1;

	//unit commander update
	m_pUnitCommander->update ( 0 );
	
	// kill exit btn
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pBtnSurvival->setVisible ( true );
		m_pBtnSurvival->setEnabled ( true );
	}

	return true;
}

KDbool CasinoBattleNode::initWithMobs ( const std::vector<cUnit*>& rMobs, eModeType eType )
{
	if ( !this->init ( ) )
	{
		return false;
	}

	// set rMobs
	this->setMobs ( rMobs, eType );

	return true;
}

KDvoid CasinoBattleNode::setMobs ( const std::vector<cUnit*>& rMobs, eModeType eType )
{
	// mode type
	m_eModeType = eType;

	// back
	switch ( m_eModeType )
	{
		case eModeType_Casino				:	
		case eModeType_MyRoom_Casino100		:	
		case eModeType_MyRoom_Casino1000	:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
		case eModeType_MyRoom_Arcade_Easy	:	m_pSprBack = Sprite::create ( "Game/Back/bg_easy.png"		);	break;
		case eModeType_MyRoom_Arcade_Nor	:	m_pSprBack = Sprite::create ( "Game/Back/bg_normal.png"		);	break;
		case eModeType_MyRoom_Arcade_Hard	:	m_pSprBack = Sprite::create ( "Game/Back/bg_hard.png"		);	break;
		case eModeType_MyRoom_Survival		:	m_pSprBack = Sprite::create ( "Game/Back/bg_survival.png"	);	break;
		case eModeType_End					:	m_pSprBack = Sprite::create ( "Game/Back/game_back_1.png"	);	break;
	}
	
	m_pSprBack->setPosition	( Point::ZERO );
	m_pSprBack->setAnchorPoint ( Point::ZERO );
	this->addChild ( m_pSprBack, LAYER_BACK );
	
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		// back decorations
		cSprAni*	pAniDeco1 = new cSprAni ( this, "Game/Back/game_back_1_fire_a.png", LAYER_BACK_DECO, Point (  22, 232 ) );
		cSprAni*	pAniDeco2 = new cSprAni ( this, "Game/Back/game_back_1_fire_b.png", LAYER_BACK_DECO, Point ( 318, 166 ) );

		pAniDeco1->addFrame ( Rect ( 0,   0, 138, 66), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0,  66, 138, 66), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0, 132, 138, 66), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		pAniDeco1->addFrame ( Rect ( 0, 198, 138, 66), 120, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

		pAniDeco2->addFrame ( Rect ( 0,   0, 142, 66), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0,  66, 142, 65), 120, false, false, Point ( 0, 65 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0, 131, 142, 66), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );
		pAniDeco2->addFrame ( Rect ( 0, 197, 142, 66), 120, false, false, Point ( 0, 66 ), Rect::ZERO, Rect::ZERO );

		pAniDeco1->setFrameInit ( 0 );
		pAniDeco2->setFrameInit ( 0 );
		pAniDeco1->setLoop ( true );
		pAniDeco2->setLoop ( true );
		pAniDeco1->start ( );
		pAniDeco2->start ( );

		m_aBackDecos.push_back ( pAniDeco1 );
		m_aBackDecos.push_back ( pAniDeco2 );
	}

	// mob들의 상위 node를 바꿔준다.
	for ( auto pNode : rMobs )
	{
		pNode->changeNode ( this, LAYER_GAMEVIEW_MOB );
	}

	// bead
	for ( KDint i = 0; i < 5; i++ )
	{
		m_aBeads.push_back ( new cBead ( this ) );
	}

	// mob commander
	m_pUnitCommander = new cUnitCommanderCasino ( this, m_aListEffects, rMobs, m_eModeType );

	// game status view
	m_pStatus = new cCasinoBattleStatus ( this, m_pUnitCommander->getMyMob ( ), m_eModeType );

	// kill exit btn
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pBtnSurvival->setVisible ( true );
		m_pBtnSurvival->setEnabled ( true );
	}
}

KDvoid CasinoBattleNode::saveData ( KDvoid )
{
	if ( l_pCasinoBattleNode )
	{
		l_pCasinoBattleNode->save ( );
	}
}

KDvoid CasinoBattleNode::onExit ( KDvoid )
{
	// stop back music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );	

	// super exit
	Scene::onExit ( );
}

KDbool CasinoBattleNode::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	Point	tLocation = this->convertTouchToNodeSpace ( pTouch );

	// soul bead
	KDbool	bIsExercise = this->touchBeadProcess ( tLocation );

	// unit command
	if ( bIsExercise == false )
	{
		if ( m_bIsViewStatus == true )
		{
			m_pUnitCommander->setModeMagnification ( );
			m_bIsViewStatus = false;
		}
		else
		{
			m_pUnitCommander->setModeStatus ( );
			m_bIsViewStatus = true;
		}
	}
		
	return bIsExercise;
}

KDvoid CasinoBattleNode::save ( KDvoid )
{
	// 카지노에서 자신의 유저가 죽었다면 리턴.
	if ( ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 ) && m_pUnitCommander->getMyMob ( )->getDie ( ) == true )
	{
		return;
	}

	// path
	const KDchar*	pPath = nullptr;
	if ( m_eModeType == eModeType_Casino)	pPath = _FILE_NAME_CASINO_BATTLE_SAVE;
	else									pPath = _FILE_NAME_MYROOM_BATTLE_SAVE;

	// open
	KDFile*			pFile = kdFopen ( pPath, "wb" );
	if ( pFile != nullptr )
	{
		// version
		std::string versionStr		= _GAME_SAVE_FILE_SIZE_LATEST_VERSION_STR;
		int			versionStrCount = versionStr.length() + 1;
		char		stringNull		= 0;
		kdFwrite ( &versionStrCount  , sizeof(int ), 1				 , pFile );
		kdFwrite ( versionStr.c_str(), sizeof(char), versionStrCount, pFile );
		kdFwrite ( &stringNull		  , sizeof(char), 1				 , pFile );

		// mode type
		KDint		nModeType			= m_eModeType;
		KDint		nUnitCreateCount	= m_pUnitCommander->getUnitCreateCount ( );
		KDdouble	dUnitCreateLeftTime = m_pUnitCommander->getUnitCreateLeftTime ( );
		kdFwrite ( &nModeType								, sizeof ( KDint    ), 1, pFile );
		kdFwrite ( &nUnitCreateCount						, sizeof ( KDint    ), 1, pFile );
		kdFwrite ( &dUnitCreateLeftTime						, sizeof ( KDdouble ), 1, pFile );
		kdFwrite ( &cMyData::getObj ( )->m_fGamingExp		, sizeof ( KDfloat  ), 1, pFile );
		kdFwrite ( &cMyData::getObj ( )->m_nGamingKillCount	, sizeof ( KDint    ), 1, pFile );

		// get rMobs
		std::vector<cUnit*>&	aMobs = m_pUnitCommander->getMobs ( );

		// mob count
		KDint		nMobCount = 0;
		for ( KDint i = 0; i < (KDint) aMobs.size ( ); i++ )
		{
			if ( aMobs [ i ] != nullptr )
			{
				++nMobCount;	
			}
		}
		kdFwrite ( &nMobCount, sizeof ( KDint ), 1, pFile );

		// mob loop
		for ( KDint i = 0; i < (KDint) aMobs.size ( ); i++ )
		{
			// check
			if ( aMobs [ i ] == nullptr )
			{
				continue;
			}

			// index & para
			KDint		nCreateIndex		= aMobs [ i ]->getCreateIndex		 ( );
			KDint		nSeatIndex			= aMobs [ i ]->getSeatIndex			 ( );
			KDint		nCreateInfoFileType = aMobs [ i ]->getCreateInfoFileType ( );
			KDint		nUniqueType			= aMobs [ i ]->getUniqueType		 ( );
			KDint		nSlotIndex			= aMobs [ i ]->getSlotIndex			 ( );
			KDint		nUnitID				= aMobs [ i ]->getUnitID			 ( );
			KDfloat		fExp				= aMobs [ i ]->getExp				 ( );
			KDfloat		fClassExp			= aMobs [ i ]->getClassExp			 ( );
			KDfloat		fFriendShipExp		= aMobs [ i ]->getFriendShipExp		 ( );
			KDint		nKillCount			= aMobs [ i ]->getKillCount			 ( );
			KDint		nIsArcadeEasyClear	= aMobs [ i ]->getIsArcadeEasyClear	 ( );
			KDint		nIsArcadeNorClear	= aMobs [ i ]->getIsArcadeNorClear	 ( );
			KDint		nIsArcadeHardClear	= aMobs [ i ]->getIsArcadeHardClear	 ( );

			KDint		nHp		= aMobs [ i ]->getHP	 ( );
			KDint		nUseHp	= aMobs [ i ]->getUseHp  ( );
			KDint		nAtk	= aMobs [ i ]->getATK	 ( );
			KDint		nDef	= aMobs [ i ]->getDEF	 ( );
			KDint		nAgl	= aMobs [ i ]->getAGL	 ( );
			KDint		nCrt	= aMobs [ i ]->getCRT	 ( );
			KDint		nHpMax	= aMobs [ i ]->getHPMax  ( );
			KDint		nAtkMax = aMobs [ i ]->getATKMax ( );
			KDint		nDefMax = aMobs [ i ]->getDEFMax ( );
			KDint		nAglMax	= aMobs [ i ]->getAGLMax ( );
			KDint		nCrtMax = aMobs [ i ]->getCRTMax ( );

			kdFwrite ( &nCreateIndex	   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nSeatIndex		   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nCreateInfoFileType, sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nUniqueType		   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nSlotIndex		   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nUnitID			   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &fExp			   , sizeof ( KDfloat ), 1, pFile );
			kdFwrite ( &fClassExp		   , sizeof ( KDfloat ), 1, pFile );
			kdFwrite ( &fFriendShipExp	   , sizeof ( KDfloat ), 1, pFile );
			kdFwrite ( &nKillCount		   , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nIsArcadeEasyClear , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nIsArcadeNorClear  , sizeof ( KDint	  ), 1, pFile );
			kdFwrite ( &nIsArcadeHardClear , sizeof ( KDint	  ), 1, pFile );

			kdFwrite ( &nHp		, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nUseHp	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nAtk	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nDef	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nAgl	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nCrt	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nHpMax  , sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nAtkMax , sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nDefMax , sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nAglMax , sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nCrtMax , sizeof ( KDint ), 1, pFile );

			// get para adds
			const std::vector<cParaAdd>&	rUseAdds = aMobs [ i ]->getUseParaAdd ( );

			// para add count
			KDint		nParaAddCount = (KDint) rUseAdds.size ( );
			kdFwrite ( &nParaAddCount, sizeof ( KDint ), 1, pFile );

			// para add loop
			for ( KDint a = 0; a < nParaAddCount; a++ )
			{
				kdFwrite ( &rUseAdds [ a ], sizeof ( cParaAdd ), 1, pFile );
			}

			//die
			KDint		nIsDie = aMobs [ i ]->getDie ( );
			kdFwrite ( &nIsDie, sizeof ( KDint ), 1, pFile );

			// bet init
			KDint64		nBetMoney		  = aMobs [ i ]->getBetMoney			     ( );
			KDfloat		fBetMagnification = aMobs [ i ]->getDividendMagnification ( );

			kdFwrite ( &nBetMoney		 , sizeof ( KDint64 ), 1, pFile );
			kdFwrite ( &fBetMagnification, sizeof ( KDfloat ), 1, pFile );

			// 현재 애니메이션 동작을 저장.
			KDint		nAniType	 = aMobs [ i ]->getAniType	   ( );
			KDint		nAniNowFrame = aMobs [ i ]->getAniNowFrame ( );

			kdFwrite ( &nAniType	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nAniNowFrame, sizeof ( KDint ), 1, pFile );
		}

		// get active gages
		const std::vector<KDfloat>&		aActiveGages = m_pUnitCommander->getMobActiveGages ( );

		// active gage count
		KDint		nActiveGageCount = (KDint) aActiveGages.size ( );
		kdFwrite ( &nActiveGageCount, sizeof ( KDint ), 1, pFile );

		// active gage loop
		for ( KDint i = 0; i < nActiveGageCount; i++ )
		{
			kdFwrite ( &aActiveGages [ i ], sizeof ( KDfloat ), 1, pFile );
		}

		// active count
		KDint		nActiveCount = m_pUnitCommander->getActiveMax ( );
		kdFwrite ( &nActiveCount, sizeof ( KDint ), 1, pFile );

		// loop active
		for ( KDint i = 0; i < nActiveCount; i++ ) 
		{
			sActive*	pActive = m_pUnitCommander->getActive ( i );

			//base
			kdFwrite ( &pActive->eUnit				, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nUnitID			, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nSeatIndex			, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nActionIndex		, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nScriptIndex		, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nSelectHeroID		, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->nSelectMobSeatIndex, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->eRange				, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &pActive->dLeftTime			, sizeof ( KDdouble	), 1, pFile );
			kdFwrite ( &pActive->bIsActionStart		, sizeof ( KDbool	), 1, pFile );

			// target hero id count
			KDint		nTargetHeroIDCount	 = (KDint) pActive->aTargetHeroIDs	.size ( );
			KDint		nTargetMobIndexCount = (KDint) pActive->aTargetMobIndexs.size ( );
			kdFwrite ( &nTargetHeroIDCount	, sizeof ( KDint ), 1, pFile );
			kdFwrite ( &nTargetMobIndexCount, sizeof ( KDint ), 1, pFile );

			for ( KDint a = 0; a < nTargetHeroIDCount  ; a++ )	kdFwrite ( &pActive->aTargetHeroIDs	  [ a ], sizeof ( KDint ), 1, pFile );
			for ( KDint a = 0; a < nTargetMobIndexCount; a++ )	kdFwrite ( &pActive->aTargetMobIndexs [ a ], sizeof ( KDint ), 1, pFile );
		}

		// get time effects
		const std::vector<sTimeEffect>&		aTimeEffects = m_pUnitCommander->getTimeEffects ( );

		// time effect count
		KDint		nTimeEffectCount = (KDint) aTimeEffects.size ( );
		kdFwrite ( &nTimeEffectCount, sizeof ( KDint ), 1, pFile );

		//loop time effect
		for ( KDint i = 0; i < nTimeEffectCount; i++ )
		{
			kdFwrite ( &aTimeEffects [ i ].eUnit		, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].nUnitID		, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].nSeatIndex	, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].nEffectIndex	, sizeof ( KDint	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].tPoint		, sizeof ( Point	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].dTime		, sizeof ( KDdouble	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].dLeftTime	, sizeof ( KDdouble	), 1, pFile );
			kdFwrite ( &aTimeEffects [ i ].nLeftCount	, sizeof ( KDint	), 1, pFile );

			KDint		nEffectNameCount = aTimeEffects [ i ].sEffectName.length ( ) + 1;
			KDchar		cStringNull		 = 0;
			kdFwrite ( &nEffectNameCount				       , sizeof ( KDint  ), 1				, pFile );
			kdFwrite ( aTimeEffects [ i ].sEffectName.c_str ( ), sizeof ( KDchar ), nEffectNameCount, pFile );
			kdFwrite ( &cStringNull						       , sizeof ( KDchar ), 1				, pFile );
		} 

		// 연승.
		kdFwrite ( &cMyData::getObj ( )->m_nSuccessive, sizeof ( KDint ), 1, pFile );

		// soul Bead count
		KDint	nSoulBeadCount = (KDint) m_aBeads.size ( );
		kdFwrite ( &nSoulBeadCount, sizeof ( KDint ), 1, pFile );

		//loop Bead count
		for ( auto pBead : m_aBeads )
		{
			kdFwrite ( &pBead->bIsInit		, sizeof ( KDbool ), 1, pFile );
			kdFwrite ( &pBead->bIsFinish	, sizeof ( KDbool ), 1, pFile );
			kdFwrite ( &pBead->nParticleType, sizeof ( KDint  ), 1, pFile );
			kdFwrite ( &pBead->tPoint		, sizeof ( Point  ), 1, pFile );

			KDint	nSoulBeadWorkCount = (KDint) pBead->aTargetBeadWorks.size ( );
			kdFwrite ( &nSoulBeadWorkCount, sizeof ( KDint ), 1, pFile );

			for ( auto pBeadWork : pBead->aTargetBeadWorks )
			{
				kdFwrite ( &pBeadWork->nSeatIndex		, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nHp				, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nAtk				, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nDef				, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nAgl				, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nCrt				, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->dKeepTime		, sizeof ( KDdouble  ), 1, pFile );
				kdFwrite ( &pBeadWork->nExerciseCount	, sizeof ( KDint	 ), 1, pFile );
				kdFwrite ( &pBeadWork->nDamage			, sizeof ( KDint	 ), 1, pFile );
			}
		}

		// close
		kdFclose ( pFile );
	}
}

KDbool CasinoBattleNode::touchBeadProcess ( Point tTouch )
{
	for ( auto pBead : m_aBeads )
	{
		if ( pBead->bIsInit == false || pBead->bIsFinish == true )
		{
			continue;
		}

		Point	tPoint = pBead->tPoint;
		Rect	tRect  = Rect ( tPoint.x - 30, tPoint.y - 30, 60, 60 );

		if ( RectInPoint ( tRect, tTouch ) == true )
		{
			// init
			std::vector<cUnit*>&	rMobs = m_pUnitCommander->getMobs ( );

			// switch
			pBead->bIsFinish = true;

			// sound
			if ( cMyData::getObj ( )->m_bIsOptionSound != false )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "soulBead_break.mp3" );
			}

			// particle
			m_aParticles.push_back ( new cParticle ( this, "bead_bomb.plist", LAYER_GAMEVIEW_EFFECT_SOUL, tPoint, true, 5 ) );

			// sprite
			for ( auto pNode : pBead->aSprites		 )	pNode->get ( )->setVisible ( false );
			for ( auto pNode : pBead->aParticleSouls )	pNode->get ( )->setVisible ( false );

			// 효과 초기화.
			pBead->aTargetBeadWorks.clear ( );

			// 타입에 따른 효과 세팅.
			if ( pBead->nParticleType == 0 )
			{
				// init
				KDint	nType2 = kdRand ( ) % 2;

				// 베팅한 몬스터.
				if ( nType2 == 0 )
				{
					// new
					cBeadWork*		pBeadWork = new cBeadWork ( );

					// set
					pBeadWork->nSeatIndex = m_pUnitCommander->getMyMob ( )->getSeatIndex ( );
					pBeadWork->dKeepTime  = 10 + ( kdRand ( ) % 30 );
					pBeadWork->nAtk		  =  1 + ( kdRand ( ) % 3 );
					pBeadWork->nDef		  =  1 + ( kdRand ( ) % 3 );
					pBeadWork->nAgl		  =  1 + ( kdRand ( ) % 3 );
					pBeadWork->nCrt		  =  1 + ( kdRand ( ) % 3 );

					// push
					pBead->aTargetBeadWorks.push_back ( pBeadWork );
				}

				// 베팅한 몬스터를 제외한 전체.
				else if ( nType2 == 1 )
				{					
					for ( KDuint a = 0; a < rMobs.size ( ); a++ )
					{
						auto	pMob = rMobs [ a ];

						// check
						if ( pMob == nullptr || ( pMob->getSeatIndex ( ) == m_pUnitCommander->getMyMob ( )->getSeatIndex ( ) ) )	
						{
							continue;
						}

						// new
						cBeadWork*	pBeadWork = new cBeadWork ( );

						//set
						pBeadWork->nSeatIndex = a;
						pBeadWork->dKeepTime  = 10 + ( kdRand ( ) % 30 );
						pBeadWork->nAtk		  = -( kdRand ( ) % 4 );
						pBeadWork->nDef		  = -( kdRand ( ) % 4 );
						pBeadWork->nAgl		  = -( kdRand ( ) % 4 );
						pBeadWork->nCrt		  = -( kdRand ( ) % 4 );

						// push
						pBead->aTargetBeadWorks.push_back ( pBeadWork );
					}
				}
			}
			// 데미지 타입.
			else if ( pBead->nParticleType == 1 )
			{
				for ( KDuint a = 0; a < rMobs.size ( ); a++ )
				{
					auto			pMob = rMobs [ a ];

					// check
					if ( pMob == nullptr || ( ( kdRand ( ) % 100 < 70 ) && pMob->getSeatIndex ( ) == m_pUnitCommander->getMyMob ( )->getSeatIndex ( ) ) )
					{
						continue;
					}

					// new
					cBeadWork*		pBeadWork = new cBeadWork ( );

					// set
					pBeadWork->nSeatIndex = a;
					pBeadWork->nDamage	  = 2 + ( kdRand ( ) % 7 );

					// push
					pBead->aTargetBeadWorks.push_back ( pBeadWork );
				}
			}
			// 독 타입.
			else if ( pBead->nParticleType == 2 )
			{
				for ( KDuint a = 0; a < rMobs.size ( ); a++ )
				{
					auto			pMob = rMobs [ a ];

					// check
					if ( pMob == nullptr || ( ( kdRand ( ) % 100 < 60 ) && pMob->getSeatIndex() == m_pUnitCommander->getMyMob ( )->getSeatIndex ( ) ) )	
					{
						continue;
					}

					// new
					cBeadWork*		pBeadWork = new cBeadWork ( );

					// set
					pBeadWork->nSeatIndex	  = a;
					pBeadWork->nHp			  = -2 - ( kdRand ( ) % 5 );
					pBeadWork->dKeepTime	  = 2 + ( kdRand ( ) % 3 );
					pBeadWork->nExerciseCount = 2 + ( kdRand ( ) % 2 );

					// push
					pBead->aTargetBeadWorks.push_back ( pBeadWork );
				}
			}
			// 힐 타입.
			else if ( pBead->nParticleType == 3 )
			{
				// 베팅한 몬스터.
				if ( kdRand ( ) % 100 < 80 )
				{
					// new
					cBeadWork*		pBeadWork = new cBeadWork ( );

					// set
					pBeadWork->nSeatIndex	  = m_pUnitCommander->getMyMob ( )->getSeatIndex ( );
					pBeadWork->nHp			  = 5 + ( kdRand ( ) % 6 );
					pBeadWork->dKeepTime	  = 0;
					pBeadWork->nExerciseCount = 1;

					// push
					pBead->aTargetBeadWorks.push_back ( pBeadWork );
				}
				// 전체.
				else
				{
					for ( KDuint a = 0; a < rMobs.size ( ); a++ )
					{
						auto			pMob = rMobs [ a ];

						// check
						if ( pMob == nullptr || ( ( kdRand ( ) % 100 < 60 ) && pMob->getSeatIndex ( ) == m_pUnitCommander->getMyMob ( )->getSeatIndex ( ) ) )	
						{
							continue;
						}

						//new
						cBeadWork*		pBeadWork = new cBeadWork ( );

						//set
						pBeadWork->nSeatIndex	  = a;
						pBeadWork->nHp			  = 4 + ( kdRand ( ) % 5 );
						pBeadWork->dKeepTime	  = 0;
						pBeadWork->nExerciseCount = 1;

						// push
						pBead->aTargetBeadWorks.push_back ( pBeadWork );
					}
				}
			}

			// set status & nDamage
			for ( auto pBeadWork : pBead->aTargetBeadWorks )
			{				
				m_pUnitCommander->setTimeStatus ( pBeadWork->nSeatIndex, pBeadWork->dKeepTime, pBeadWork->nExerciseCount, pBeadWork->nHp, pBeadWork->nAtk, pBeadWork->nDef, pBeadWork->nAgl, pBeadWork->nCrt );
				m_pUnitCommander->setDamage ( pBeadWork->nSeatIndex, pBeadWork->nDamage );
			}

			// set effect
			KDbool		bIsEffect  = false;
			KDint		nUnitCount = 0;
			Point		tEffect    = Point ( 0, 0 );
			for ( auto pBeadWork : pBead->aTargetBeadWorks )
			{			
				if ( pBeadWork->nDamage > 0)
				{
					std::vector<cUnit*>&	rMobs = m_pUnitCommander->getMobs ( );
					bIsEffect = true;
					++nUnitCount;

					switch ( pBeadWork->nSeatIndex )
					{
						case 0	:	tEffect = Point (  50, 70 + rMobs [ pBeadWork->nSeatIndex ]->getNorFirstSize ( ).height / 2 );	break;
						case 1	:	tEffect = Point ( 145, 70 + rMobs [ pBeadWork->nSeatIndex ]->getNorFirstSize ( ).height / 2 );	break;
						case 2	:	tEffect = Point ( 240, 70 + rMobs [ pBeadWork->nSeatIndex ]->getNorFirstSize ( ).height / 2 );	break;
						case 3	:	tEffect = Point ( 334, 70 + rMobs [ pBeadWork->nSeatIndex ]->getNorFirstSize ( ).height / 2 );	break;
						case 4	:	tEffect = Point ( 429, 70 + rMobs [ pBeadWork->nSeatIndex ]->getNorFirstSize ( ).height / 2 );	break;
					}
				}
			}

			if ( bIsEffect == true )
			{
				std::string		sEffectName;

				if ( nUnitCount < 2 )
				{
					switch ( kdRand ( ) % 4 )
					{
						case 0	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_NAIL, tEffect );	break;
						case 1	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_CUT , tEffect );	break;
						case 2	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_BITE, tEffect );	break;
						case 3	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_HIT , tEffect );	break;
					}
				}
				else
				{
					switch ( kdRand ( ) % 3 )
					{
						case 0	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_NAIL_ALL, tEffect );	break;
						case 1	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_CUT_ALL , tEffect );	break;
						case 2	:	m_aListEffects [ 0 ]->addEffect ( _EFFECT_STR_BITE_ALL, tEffect );	break;
					}
				}
			}

			return true;
		}
	}

	return false;
}


KDvoid CasinoBattleNode::step ( KDfloat fDelta )
{
	// tickCount
	m_dTickCount += fDelta;

	if ( m_dBeforeTickCount == 0 )	
	{
		m_dBeforeTickCount = m_dTickCount;
	}
	KDdouble	dLeftTickCount = m_dTickCount - m_dBeforeTickCount;
	m_dBeforeTickCount = m_dTickCount;

	// my data update
	cMyData::getObj ( )->update ( dLeftTickCount );

	if ( dLeftTickCount > 0.1f )
	{
		dLeftTickCount = 0.1f;
	}

	KDdouble	dStaticLeftTickCount = dLeftTickCount;

#if DEBUG_GAME
	m_bIsSkip = true;
	dLeftTickCount *= 2;
#endif

	// skip 적용.	
	if ( m_bIsSkip == true )
	{
		dLeftTickCount *= 7;
	}

	// node chage
	if ( this->updateNodeChage ( dLeftTickCount ) == true )	
	{
		return;
	}

	// load wait time
	m_dLoadWaitLeftTime -= dLeftTickCount;
	if ( m_dLoadWaitLeftTime > 0 )
	{
		return;
	}

	// back decorations
	for ( auto pNode : m_aBackDecos )
	{
		pNode->update ( dLeftTickCount );
	}

	// game start
	this->updateGameStart ( dLeftTickCount );

	// end
	if ( m_bIsEnd == true && m_bIsNodeChangeOut == false )
	{
		m_dEndLeftTime -= dLeftTickCount;

		if ( m_dEndLeftTime < 0 )
		{
			m_bIsNodeChangeOut = true;
			CC_SAFE_RELEASE ( m_pNodeChangeImg );
			m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

			// stop background music
			SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
		}
	}

	if ( m_bIsEnd == false && m_pUnitCommander->getIsGameEnd ( ) == true )
	{
		m_bIsSkip = false;
		m_bIsEnd = true;
		m_dEndLeftTime = 3;

		m_pBtnSkip->setVisible ( false );
		m_pBtnSkip->setEnabled ( false );
	}

	// mob commander
	m_pUnitCommander->update ( dLeftTickCount );

	// bead
	this->updateBead ( dLeftTickCount );

	// game status view
	m_pStatus->update ( m_pUnitCommander->getMyMob ( ), dStaticLeftTickCount );

	// effects
	for ( auto pNode : m_aListEffects )
	{
		pNode->update ( dLeftTickCount );
	}

	// particle
	this->updateParticle ( dLeftTickCount );

	// 보스전 음악.
	if ( cMyData::getObj ( )->m_bIsOptionSound != false && m_bIsEnd == false )
	{

	}

	// skip
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		if ( m_bIsEnd == false && m_bIsSkip == false && m_pBtnSkip->isVisible ( ) == false && m_pUnitCommander->getMyMob ( )->getDie ( ) == true && m_pUnitCommander->getIsGameEnd ( ) == false )
		{
			m_pBtnSkip->setVisible ( true );
			m_pBtnSkip->setEnabled ( true );
		}
	}
}

KDbool CasinoBattleNode::updateNodeChage ( KDdouble dLeftTIme )
{
	if ( m_pNodeChangeImg != nullptr )
	{
		if ( m_pNodeChangeImg->update ( dLeftTIme ) == true )
		{
			if ( m_bIsNodeChangeIn == true )
			{
				m_bIsNodeChangeIn = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );			
			}
			else if ( m_bIsNodeChangeOut == true )
			{
				m_bIsNodeChangeOut = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );		

				// unschedule
				this->unschedule ( schedule_selector ( CasinoBattleNode::step ) );

				// cenece change
				Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoResultNode::createWithBetMob ( m_pUnitCommander->getMyMob ( ), m_eModeType ) ) );

				return true;
			}
		}
	}

	return false;
}

KDvoid CasinoBattleNode::updateGameStart ( KDdouble dLeftTime )
{
	if ( m_nGameStartStep == -1 )	
	{
		return;
	}

	if ( m_nGameStartStep == 0 )
	{
		m_dGameStartLeftTime -= dLeftTime;

		if ( m_dGameStartLeftTime < 0 )
		{
			m_pSprGameStartBack->setPointTime ( Point ( 0, 129 ), 0.2f );

			m_nGameStartStep = 1;
			m_dGameStartLeftTime = 0.2f;
		}
	}
	else if ( m_nGameStartStep == 1 )
	{
		m_pSprGameStartBack->update ( dLeftTime );

		m_dGameStartLeftTime -= dLeftTime;

		if ( m_dGameStartLeftTime < 0 )
		{
			if ( cMyData::getObj ( )->m_bIsOptionSound != 0)
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "battlestart.mp3" );
			}

			m_pSprGameStartText->get ( )->setVisible ( true );
			m_pSprGameStartText->get ( )->setOpacity ( 0 );

			m_nGameStartStep = 2;
			m_dGameStartLeftTime = 0.2f;
		}
	}
	else if ( m_nGameStartStep == 2 )
	{
		m_dGameStartLeftTime -= dLeftTime;

		m_pSprGameStartText->get ( )->setOpacity ( 255 * ( 1 - ( m_dGameStartLeftTime * 5 ) ) );

		if ( m_dGameStartLeftTime < 0 )
		{
			m_pSprGameStartText->get ( )->setOpacity ( 255 );

			m_nGameStartStep = 3;
			m_dGameStartLeftTime = 0.7;
		}
	}
	else if ( m_nGameStartStep == 3 )
	{
		m_dGameStartLeftTime -= dLeftTime;
		
		if ( m_dGameStartLeftTime < 0 )
		{
			m_nGameStartStep = 4;
			m_dGameStartLeftTime = 0.2f;
		}
	}
	else if ( m_nGameStartStep == 4 )
	{
		m_dGameStartLeftTime -= dLeftTime;

		m_pSprGameStartText->get ( )->setOpacity ( 255 * ( m_dGameStartLeftTime * 5 ) );

		if ( m_dGameStartLeftTime < 0 )
		{
			m_pSprGameStartText->get ( )->setVisible ( false );
			m_pSprGameStartBack->setPointTime ( Point ( -480, 129 ), 0.2f );

			m_nGameStartStep = 5;
			m_dGameStartLeftTime = 0.2f;
		}
	}
	else if ( m_nGameStartStep == 5 )
	{
		m_dGameStartLeftTime -= dLeftTime;

		m_pSprGameStartBack->update ( dLeftTime );

		if ( m_dGameStartLeftTime < 0 )
		{
			m_pSprGameStartText = nullptr;
			m_pSprGameStartBack = nullptr;

			m_nGameStartStep = -1;
		}
	}
}

KDvoid CasinoBattleNode::updateBead ( KDdouble dLeftTime )
{
	// init
	std::vector<cUnit*>&	rMobs = m_pUnitCommander->getMobs ( );

	// mob loop
	for ( KDuint i = 0; i < rMobs.size ( ) && i < m_aBeads.size ( ); i++ )
	{
		// update
		m_aBeads [ i ]->update ( dLeftTime );

		// check
		if ( rMobs [ i ] == nullptr )
		{
			continue;
		}

		// 해당 자리에 몬스터가 있다면 구슬을 파괴 해주고 다시 초기화 한다.
		if ( rMobs [ i ]->getDie ( ) == false && m_aBeads [ i ]->bIsInit == true )
		{
			m_aBeads [ i ]->bIsInit	  = false;
			m_aBeads [ i ]->bIsFinish = false;
			m_aBeads [ i ]->bIsMoveUp = true;

			// sprite
			for ( auto pNode : m_aBeads [ i ]->aSprites		  )		pNode->get ( )->setVisible ( false );
			for ( auto pNode : m_aBeads [ i ]->aParticleSouls )		pNode->get ( )->setVisible ( false );

			// particle
			if ( m_aBeads [ i ]->bIsFinish == false )
			{
				m_aParticles.push_back ( new cParticle ( this, "bead_bomb.plist", LAYER_GAMEVIEW_EFFECT_SOUL, m_aBeads [ i ]->tPoint, true, 5 ) );
			}
		}

		// 죽었는데 구슬이 초기화가 안되었다면 생성해준다.
		if ( rMobs [ i ]->getDie ( ) == true && m_aBeads [ i ]->bIsInit == false )
		{
			// particle type
			m_aBeads [ i ]->nParticleType = kdRand ( ) % 4;		// 0 == 능력치 타입, 1 == 데미지 타입, 2 == 독 타입, 3 == 회복 타입.

			// point
			m_aBeads [ i ]->tPoint = rMobs [ i ]->getPoint ( ) + Point ( 0, ( rMobs [ i ]->getNorFirstSize ( ).height / 2 ) - 3 );

			// sprite & particle
			m_aBeads [ i ]->aSprites	   [ m_aBeads [ i ]->nParticleType ]->get ( )->setVisible  ( true );
			m_aBeads [ i ]->aSprites	   [ m_aBeads [ i ]->nParticleType ]->get ( )->setPosition ( m_aBeads [ i ]->tPoint );
			m_aBeads [ i ]->aParticleSouls [ m_aBeads [ i ]->nParticleType ]->get ( )->setVisible  ( true );
			m_aBeads [ i ]->aParticleSouls [ m_aBeads [ i ]->nParticleType ]->get ( )->setPosition ( m_aBeads [ i ]->tPoint );

			// switch
			m_aBeads [ i ]->bIsInit	= true;
			m_aBeads [ i ]->bIsFinish = false;
		}
	}

	// 게임이 끝났다면 구슬을 다 터트린다.
	if ( m_bIsEnd == true )
	{
		for ( KDuint i = 0; i < m_aBeads.size(); i++ )
		{
			// 터지는 이펙트 추가.
			if ( m_aBeads [ i ]->bIsInit == true && m_aBeads [ i ]->bIsFinish == false )
			{
				for ( auto pNode : m_aBeads [ i ]->aSprites		  )		pNode->get ( )->setVisible ( false );
				for ( auto pNode : m_aBeads [ i ]->aParticleSouls )		pNode->get ( )->setVisible ( false );

				m_aParticles.push_back ( new cParticle ( this, "bead_bomb.plist", LAYER_GAMEVIEW_EFFECT_SOUL, m_aBeads [  i]->tPoint, true, 5 ) );

				m_aBeads [ i ]->bIsFinish = true;
			}
		}
	}
}

KDvoid CasinoBattleNode::updateParticle ( KDdouble dLeftTime )
{
	std::list<cParticle*>::iterator		pNode = m_aParticles.begin ( );
	while ( pNode != m_aParticles.end ( ) )
	{
		//init
		cParticle*		pParticle = *pNode;
		KDbool			bIsErase  = false;

		// update
		pParticle->update ( dLeftTime );

		// check erase
		if ( pParticle->getIsCheckTime ( ) == true && pParticle->getCheckTime ( ) < 0 )
		{
			bIsErase = true;
		}

		// next node or erase node
		if ( bIsErase == true )	
		{
			pNode = m_aParticles.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}
}

KDvoid CasinoBattleNode::skipCallback ( Object* pSender )
{
	m_pBtnSkip->setVisible ( false );
	m_pBtnSkip->setEnabled ( false );

	m_bIsSkip = true;
}

KDvoid CasinoBattleNode::survivalExitCallback ( Object* pSender )
{
	m_pBtnSurvival->setEnabled ( false );

	m_bIsEnd = true;
	m_dEndLeftTime = 1;
	
	m_pUnitCommander->setGameEndByForce ( );
}

