/*
 *
 *      File            EnvManager.h
 *      Description     Environment Manager
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#ifndef __EnvManager_h__
#define __EnvManager_h__

#define COUNT_STAGE		9
#define COUNT_SKILL		10
#define COUNT_TITLE		4

#pragma pack(1)

class CEnvManager
{
	protected :

		 CEnvManager ( KDvoid );
		~CEnvManager ( KDvoid );

	public :

		static KDvoid	create  ( KDvoid );
		static KDvoid	release ( KDvoid );

	public :

		KDvoid			load			 ( KDvoid );
		KDvoid			save			 ( KDvoid );

		KDvoid			resetGame		 ( KDvoid );
		KDvoid			resetConfig		 ( KDvoid );
    
		KDuint			rand			 ( KDuint uValue = 100 );

		const KDchar*	convCash		 ( KDuint uValue );
		const KDchar*   convCoin         ( KDuint uValue );
		const KDchar*	convNumber		 ( KDint  nValue );

		KDbool			getActivate		 ( KDvoid );
		const KDchar*	getName			 ( KDvoid );
		KDbool			getReward		 ( KDuint uIndex );
		KDuint			getStar			 ( KDvoid );									
		KDuint			getStar			 ( KDuint uIndex );	
		KDuint			getSkill		 ( KDuint uIndex );
		const KDuint*   getMission       ( KDuint uIndex );
		KDuint			getEnemyCash     ( KDvoid );
		KDuint			getEnemyCashMax  ( KDvoid );
		KDuint			getEnemyCash	 ( KDuint uIndex );
		KDuint			getEnemyCashMax	 ( KDuint uIndex );
		KDuint64		getPriceCoin	 ( KDuint uIndex );
		KDuint			getPriceCash	 ( KDuint uIndex );
		KDuint			getPriceSkill	 ( KDuint uIndex );
		KDuint			getExchageCoin	 ( KDuint uIndex );
		KDuint			getExchageCash	 ( KDuint uIndex );
		KDuint			getBet			 ( KDuint uIndex );
		KDuint			getBet			 ( KDvoid );
		KDuint			getCoin			 ( KDvoid );
		KDuint			getCash			 ( KDvoid );
		KDuint			getCashMax		 ( KDvoid );
		KDuint			getCashLast		 ( KDvoid );
		KDuint			getWin			 ( KDvoid );
		KDuint			getLose			 ( KDvoid );
		KDuint			getLevel		 ( KDvoid );
		KDuint			getStage		 ( KDvoid );
		KDuint			getStageMax		 ( KDvoid );
		KDuint			getTitle		 ( KDvoid );
		KDuint			getTitleMax		 ( KDvoid );

		KDuint			getWinComboMax	 ( KDvoid );
		KDuint			getWinCashMax    ( KDvoid );
		KDuint			getWinScoreMax 	 ( KDvoid );
		
		KDuint			getFirstTurn     ( KDvoid );
		KDuint			getPushCount	 ( KDvoid ); 

		KDfloat			getAnimate       ( KDvoid );
		KDuint			getSpeed		 ( KDvoid );
		KDbool			getSound		 ( KDvoid );
		KDbool			getVoice		 ( KDvoid );
		KDbool			getVibrate		 ( KDvoid );

		KDvoid			setActivate		 ( KDvoid );
		KDvoid			setName			 ( const KDchar* szName );
		KDvoid			setReward		 ( KDuint uIndex, KDbool bValue );
		KDvoid			setSkill		 ( KDuint uIndex, KDuint uValue );
		KDvoid			setEnemyCash	 ( KDuint uIndex, KDuint uValue );
		KDvoid			setEnemyCash     ( KDuint uValue );
		KDvoid			setCoin			 ( KDuint uValue );
		KDvoid			setCash			 ( KDuint uValue );
		KDvoid			setLevel		 ( KDuint uValue );
		KDvoid			setStage		 ( KDuint uValue );
		KDvoid			setStageMax		 ( KDuint uValue );
		KDvoid			setTitle		 ( KDuint uValue );
		KDvoid			setTitleMax		 ( KDuint uValue );
		KDvoid			setRankWin		 ( KDuint uValue );
		KDvoid			setRankScore	 ( KDuint uValue );

		KDvoid			setSpeed		 ( KDuint uValue );
		KDvoid			setSound		 ( KDbool uValue );
		KDvoid			setVoice		 ( KDbool bValue );
		KDvoid			setVibrate		 ( KDbool bValue );

		KDvoid			rollback	     ( KDvoid );
		KDvoid			setResult		 ( KDbool bVictory, KDuint uCash, KDuint uScore, KDuint uTimes );
		KDvoid			setFirstTurn	 ( KDuint uValue );
		KDvoid			setPushCount	 ( KDuint uValue );

	protected :

		KDbool			m_bActivate;
		KDchar			m_szName      [ 64 ];
		KDbool			m_bRewards	  [ COUNT_STAGE ];
		KDuint			m_uStars	  [ COUNT_STAGE ];
		KDuint			m_uSkills	  [ COUNT_SKILL ];
		KDuint			m_uEnemyCashs [ COUNT_STAGE ];

		KDuint			m_uCoin;
		KDuint			m_uCash;
		KDuint			m_uCashMax;
		KDuint			m_uCashLast;
		KDuint			m_uLevel;
		KDuint			m_uStage;
		KDuint			m_uStageMax;
		KDuint			m_uTitle;
		KDuint			m_uTitleMax;

		KDuint			m_uLose;
		KDuint			m_uWin;
		KDuint			m_uWinCombo;
		KDuint			m_uWinComboMax;
		KDuint			m_uWinCashMax;
		KDuint			m_uWinScoreMax;
		
		KDuint			m_uSpeed;
		KDbool			m_bSound;
		KDbool			m_bVoice;
		KDbool			m_bVibrate;

		KDuint			m_uFirstTurn;
		KDuint			m_uPushCount;
};

extern CEnvManager*  g_pEnvManager;

#pragma pack()

#endif
