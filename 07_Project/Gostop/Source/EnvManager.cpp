/*
 *
 *      File            EnvManager.cpp
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

#include "Precompiled.h"

#define SAVE_FILE_PATH		"/data/XMGostop.dat"

CEnvManager*  g_pEnvManager  = KD_NULL;

static KDvoid*  l_pBuffer    = KD_NULL;
static KDFile*  l_pSaveFile  = KD_NULL;

static KDuint   l_uMissionTable [ ][ 5 ] =
{
	{ 2,  0,  1,  2,  3			},			// 0
	{ 2,  4,  5,  6,  7			},			// 1
	{ 2,  8,  9, 10, 11			},			// 2
	{ 2, 12, 13, 14, 15			},			// 3
	{ 2, 16, 17, 18, 19			},			// 4
	{ 2, 20, 21, 22, 23			},			// 5
	{ 2, 24, 25, 26, 27			},			// 6
	{ 3, 28, 29, 30, 31			},			// 7
	{ 3, 32, 33, 34, 35			},			// 8
	{ 2, 36, 37, 38, 39			},			// 9
	{ 4, 40, 41, 42, 43			},			// 10
	{ 3, 44, 45, 46, 47			},			// 11
	{ 3,  1,  5,  9, KDUINT_MAX	},			// 12
	{ 3, 13, 17, 25, KDUINT_MAX	},			// 13
	{ 3, 21, 33, 37, KDUINT_MAX	},			// 14
	{ 4,  4, 12, 29, KDUINT_MAX	},			// 15
	{ 3, 32, 41, 47, KDUINT_MAX	},			// 16
	{ 4, 32, 47, 48, KDUINT_MAX	},			// 17
	{ 3, 32, 41, 49, KDUINT_MAX	},			// 18
	{ 2, 20, 24, 36, KDUINT_MAX	},			// 19
	{ 5, 48, 49, 50, KDUINT_MAX	},			// 20
};

static KDuint  l_uStarTable [ ][ 5 ][ 2 ] =
{
	{	// 0
		{ 10,  1 },	
		{ 10,  1 },	
		{ 10,  2 },	
		{ 15,  4 },	
		{ 20,  4 },	
	},
	{	// 1
		{ 10,  4 },	
		{ 15,  4 },	
		{ 20,  4 },	
		{ 25,  4 },	
		{ 30,  4 },	
	},
	{	// 2
		{ 15,  4 },	
		{ 20,  4 },	
		{ 25,  4 },	
		{ 30,  4 },	
		{ 35,  4 },	
	},
	{	// 3
		{ 20,  4 },	
		{ 25,  4 },	
		{ 30,  4 },	
		{ 35,  4 },	
		{ 40,  4 },	
	},
	{	// 4
		{ 20,  4 },	
		{ 15,  8 },	
		{ 20,  8 },	
		{ 25,  8 },	
		{ 30,  8 },	
	},
	{	// 5
		{ 20,  8 },	
		{ 25,  8 },	
		{ 30,  8 },	
		{ 35,  8 },	
		{ 40,  8 },	
	},
	{	// 6
		{ 30,  8 },	
		{ 35,  8 },	
		{ 20, 16 },	
		{ 30, 16 },	
		{ 40, 16 },	
	},
	{	// 7
		{ 35,  8 },	
		{ 35, 16 },	
		{ 25, 32 },	
		{ 30, 32 },	
		{ 40, 32 },	
	},
	{	// 8
		{ 20, 32 },	
		{ 30, 32 },	
		{ 20, 64 },	
		{ 30, 64 },	
		{ 40, 64 },	
	},
};

CEnvManager::CEnvManager ( KDvoid )
{    
	CC_ASSERT ( !g_pEnvManager );

	KDuint  uSize = sizeof ( CEnvManager );

	l_pBuffer = kdMalloc ( uSize );
	CC_ASSERT ( l_pBuffer );

	m_bActivate  = KD_FALSE;
	m_uFirstTurn = 0;
	m_uPushCount = 0;

	this->load ( );
}

CEnvManager::~CEnvManager ( KDvoid )
{
	if ( m_bActivate )
	{
		this->save ( );
	        
		if ( l_pSaveFile )
		{
			kdFclose ( l_pSaveFile );
			l_pSaveFile = KD_NULL;
		}
	}
    
	kdFree ( l_pBuffer );
	l_pBuffer = KD_NULL;
}

KDvoid CEnvManager::create ( KDvoid )
{
    g_pEnvManager = new CEnvManager ( );

    CC_ASSERT ( g_pEnvManager );
}

KDvoid CEnvManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pEnvManager );
}

KDvoid CEnvManager::load ( KDvoid )
{
	KDFile*  pFile = kdFopen ( SAVE_FILE_PATH, "rb" );

	if ( pFile )
	{
		KDuint   uSize = sizeof ( CEnvManager );
		
		if ( kdFread ( l_pBuffer, uSize, 1, pFile ) == 0 )
		{
			kdFclose ( pFile );
			kdAssertion ( "Load File Fail!!!" );
		}
		
		kdMemcpy ( this, l_pBuffer, uSize );
		kdFclose ( pFile );
	}
	else
	{
		resetGame   ( );
		resetConfig ( );
	}
}

KDvoid CEnvManager::save ( KDvoid )
{
	if ( !l_pSaveFile )
	{
		l_pSaveFile = kdFopen ( SAVE_FILE_PATH, "wb" );
		CC_ASSERT ( l_pSaveFile );
	}
	else
	{
		kdFseek ( l_pSaveFile, 0, KD_SEEK_SET );
	}

	KDuint   uSize = sizeof ( CEnvManager );

	kdMemcpy ( l_pBuffer, this, uSize );
	if ( kdFwrite ( l_pBuffer, uSize, 1, l_pSaveFile ) == 0 )
	{
		kdAssertion ( "Save File Fail!!" );
	}
}

KDvoid CEnvManager::resetGame ( KDvoid )
{
	KDuint  i;

	kdSprintfKHR 
	(
		m_szName, "%s%s",
		g_pResManager->getText ( eTXT_Title_Name0 ),
		g_pResManager->getText ( eTXT_Name_KBM )
	);

	kdMemset ( m_bRewards, 0, sizeof ( m_bRewards ) );
	kdMemset ( m_uStars  , 0, sizeof ( m_uStars   ) );

	for ( i = 0; i < COUNT_SKILL; i++ )
	{
		m_uSkills [ i ] = 10;
	}

	for ( i = 0; i < COUNT_STAGE; i++ )
	{
		m_uEnemyCashs [ i ] = getEnemyCashMax ( i );
	}

	m_uCoin			 = 3000;
	m_uCash			 = 10000;
	m_uCashMax		 = 10000;
	m_uCashLast		 = 10000;
	m_uLevel		 = 0;
	m_uStage		 = KDUINT_MAX;
	m_uStageMax		 = 0;
	m_uTitle		 = KDUINT_MAX;
	m_uTitleMax		 = 0;

	m_uLose		     = 0;
	m_uWin		     = 0;
	m_uWinCombo      = 0;
	m_uWinComboMax   = 0;
	m_uWinCashMax    = 0;
	m_uWinScoreMax   = 0;

	m_uFirstTurn     = 0;
	m_uPushCount	 = 0;
}

KDvoid CEnvManager::resetConfig ( KDvoid )
{
	m_uSpeed		= 1;
	m_bSound		= KD_TRUE;
	m_bVoice		= KD_TRUE;
	m_bVibrate		= KD_TRUE;
}

KDuint CEnvManager::rand ( KDuint uValue )
{
	KDuint  uRand = kdRand ( ) % uValue;

	return uRand;
}

const KDchar* CEnvManager::convCash	( KDuint uValue )
{
	static std::string  sRet;

	KDchar  szValue [ 32 ];

	KDsize  uLen = 0;
	KDuint  uCnt = 2;

	if ( uValue > 0 )
	{
		sRet = "00";
		kdSprintfKHR ( szValue, "%u", uValue );
		uLen = kdStrlen ( szValue );

		for ( KDint  i = (KDint) ( uLen - 1 ); i >= 0; i-- )
		{
			if ( uCnt == 0 )
			{
				sRet = std::string ( "," ) + sRet;
			}

			sRet = std::string ( &szValue [ i ] ) + sRet;
			szValue [ i ] = 0;

			uCnt = uCnt == 2 ? 0 : uCnt + 1;
		}
	}
	else
	{
		sRet = "0";
	}

	return sRet.c_str ( );
}

const KDchar* CEnvManager::convCoin	( KDuint uValue )
{
	static std::string  sRet;

	KDchar  szValue [ 32 ];

	KDsize  uLen = 0;
	KDuint  uCnt = 1;

	sRet = "";
	kdSprintfKHR ( szValue, "%u", uValue );
	uLen = kdStrlen ( szValue );

	if ( uLen > 3 )
	{
		for ( KDint  i = (KDint) ( uLen - 2 ); i >= 0; i-- )
		{
			if ( uCnt == 0 )
			{
				sRet = std::string ( "," ) + sRet;
			}

			sRet = std::string ( &szValue [ i ] ) + sRet;
			szValue [ i ] = 0;

			uCnt = uCnt == 2 ? 0 : uCnt + 1;
		}
	}
	else
	{
		sRet = szValue;
	}


	return sRet.c_str ( );
}

const KDchar* CEnvManager::convNumber ( KDint  nValue )
{
	static KDchar  szRet [ 64 ];

	kdSprintfKHR ( szRet, "%d", nValue );

	return szRet;
}

KDbool CEnvManager::getActivate ( KDvoid )
{
	return m_bActivate;
}

const KDchar* CEnvManager::getName ( KDvoid )
{
	return m_szName;
}

KDbool CEnvManager::getReward ( KDuint uIndex )
{
	return m_bRewards [ uIndex ];
}

KDuint CEnvManager::getStar ( KDvoid )
{
	KDuint  uStar  = 0;
	KDuint  uCount = m_uStageMax == 0 ? 0 : m_uStageMax - 1;

	for ( KDuint i = 0; i < uCount; i++ )
	{
		uStar += m_uStars [ i ];
	}

	return uStar;
}

KDuint CEnvManager::getStar	( KDuint uIndex )
{
	return m_uStars [ uIndex ];
}

const KDuint* CEnvManager::getMission ( KDuint uIndex )
{
	return l_uMissionTable [ uIndex ];
}

KDuint CEnvManager::getSkill ( KDuint uIndex )
{
	return m_uSkills [ uIndex ];
}

KDuint CEnvManager::getEnemyCash ( KDvoid )
{
	return m_uEnemyCashs [ m_uStage ];
}

KDuint CEnvManager::getEnemyCashMax  ( KDvoid )
{
	return this->getEnemyCashMax ( m_uStage );
}

KDuint CEnvManager::getEnemyCash ( KDuint uIndex )
{
	return m_uEnemyCashs [ uIndex ];
}

KDuint CEnvManager::getEnemyCashMax	( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 2500;
		case 1 : return 10000;
		case 2 : return 50000;
		case 3 : return 150000;
		case 4 : return 500000;
		case 5 : return 2500000;
		case 6 : return 10000000;
		case 7 : return 30000000;
		case 8 : return 100000000;
	}

	return 0;
}

KDuint64 CEnvManager::getPriceCoin ( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 1000UL;
		case 1 : return 3000UL;
		case 2 : return 5000UL;
		case 3 : return 9900UL;
		case 4 : return 29900UL;
	}

	return 0;
}

KDuint CEnvManager::getPriceCash ( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 1200;
		case 1 : return 3000;
		case 2 : return 5000;
		case 3 : return 10000;
		case 4 : return 15000;
	}

	return 0;
}

KDuint CEnvManager::getPriceSkill ( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 150;
		case 1 : return 300;
		case 2 : return 1000;
		case 3 : return 1200;
		case 4 : return 450;
		case 5 : return 150;
		case 6 : return 450;
		case 7 : return 1000;
		case 8 : return 1000;
		case 9 : return 450;
	}

	return 0;
}

KDuint CEnvManager::getExchageCoin ( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 1500;
		case 1 : return 5000;
		case 2 : return 10000;
		case 3 : return 25000;
		case 4 : return 100000;
	}

	return 0;
}

KDuint CEnvManager::getExchageCash	( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 200000;
		case 1 : return 300000;
		case 2 : return 1000000;
		case 3 : return 5000000;
		case 4 : return 10000000;
	}

	return 0;
}

KDuint CEnvManager::getBet ( KDuint uIndex )
{
	switch ( uIndex )
	{
		case 0 : return 5;
		case 1 : return 10;
		case 2 : return 20;
		case 3 : return 30;
		case 4 : return 50;
		case 5 : return 100;
		case 6 : return 200;
		case 7 : return 300;
		case 8 : return 500;
	}

	return 1000;
}

KDuint CEnvManager::getBet ( KDvoid )
{
	return getBet ( m_uStage );
}

KDuint CEnvManager::getCoin	( KDvoid )
{
	return m_uCoin;
}

KDuint CEnvManager::getCash	( KDvoid )
{
	return m_uCash;
}

KDuint CEnvManager::getCashMax ( KDvoid )
{
	return m_uCashMax;
}

KDuint CEnvManager::getCashLast	( KDvoid )
{
	return m_uCashLast;
}

KDuint CEnvManager::getWin ( KDvoid )
{
	return m_uWin;
}

KDuint CEnvManager::getLose	( KDvoid )
{
	return m_uLose;
}

KDuint CEnvManager::getLevel ( KDvoid )
{
	return m_uLevel;
}

KDuint CEnvManager::getStage ( KDvoid )
{
	return m_uStage;
}

KDuint CEnvManager::getStageMax	( KDvoid )
{
	return m_uStageMax;
}

KDuint CEnvManager::getTitle ( KDvoid )
{
	return m_uTitle;
}

KDuint CEnvManager::getTitleMax	( KDvoid )
{
	return m_uTitleMax;
}

KDuint CEnvManager::getWinComboMax ( KDvoid )
{
	return m_uWinComboMax;
}

KDuint CEnvManager::getWinCashMax ( KDvoid )
{
	return m_uWinCashMax;
}

KDuint CEnvManager::getWinScoreMax ( KDvoid )
{
	return m_uWinScoreMax;
}

KDfloat CEnvManager::getAnimate ( KDvoid )
{
	return 0.2f / ( m_uSpeed + 1 );
}

KDuint CEnvManager::getSpeed ( KDvoid )
{
	return m_uSpeed;
}

KDbool CEnvManager::getSound ( KDvoid )
{
	return m_bSound;
}

KDbool CEnvManager::getVoice ( KDvoid )
{
	return m_bVoice;
}

KDbool CEnvManager::getVibrate ( KDvoid )
{
	return m_bVibrate;
}

KDuint CEnvManager::getFirstTurn ( KDvoid )
{
	return m_uFirstTurn;
}

KDuint CEnvManager::getPushCount ( KDvoid )
{
	return m_uPushCount;
}

KDvoid CEnvManager::setActivate ( KDvoid )
{
	m_bActivate = KD_TRUE;
}

KDvoid CEnvManager::setName ( const KDchar* szName )
{
	kdStrcpy ( m_szName, szName );
}

KDvoid CEnvManager::setReward ( KDuint uIndex, KDbool bValue )
{
	m_bRewards [ uIndex ] = bValue;
}

KDvoid CEnvManager::setSkill ( KDuint uIndex, KDuint uValue )
{
	m_uSkills [ uIndex ] = uValue;
}

KDvoid CEnvManager::setEnemyCash ( KDuint uIndex, KDuint uValue )
{
	m_uEnemyCashs [ uIndex ] = uValue;
}

KDvoid CEnvManager::setEnemyCash ( KDuint uValue )
{
	m_uEnemyCashs [ m_uStage ] = uValue;
}

KDvoid CEnvManager::setCoin ( KDuint uValue )
{
	m_uCoin = uValue;
}

KDvoid CEnvManager::setCash	( KDuint uValue )
{
	if ( m_uCashMax < uValue )
	{
		m_uCashMax = uValue;
	}

	m_uCashLast = m_uCash;
	m_uCash = uValue;
}

KDvoid CEnvManager::setLevel ( KDuint uValue )
{
	m_uLevel = uValue;
}

KDvoid CEnvManager::setStage ( KDuint uValue )
{
	m_uStage = uValue;
}

KDvoid CEnvManager::setStageMax	( KDuint uValue )
{
	if ( uValue > COUNT_STAGE )
	{
		uValue = COUNT_STAGE;
	}

	m_uStageMax = uValue;
}

KDvoid CEnvManager::setTitle ( KDuint uValue )
{
	m_uTitle = uValue;
}

KDvoid CEnvManager::setTitleMax	( KDuint uValue )
{
	m_uTitleMax = uValue;
}

KDvoid CEnvManager::setSpeed ( KDuint uValue )
{
	m_uSpeed = uValue;
}

KDvoid CEnvManager::setSound ( KDbool bValue )
{
	m_bSound = bValue;
}

KDvoid CEnvManager::setVoice ( KDbool bValue )
{
	m_bVoice = bValue;
}

KDvoid CEnvManager::setVibrate ( KDbool bValue )
{
	m_bVibrate = bValue;
}

KDvoid CEnvManager::rollback ( KDvoid )
{
	KDuint  uSize = sizeof ( CEnvManager );

	kdMemcpy ( this, l_pBuffer, uSize );
}

KDvoid CEnvManager::setResult ( KDbool bVictory, KDuint uCash, KDuint uScore, KDuint uTimes )
{
	KDuint  uSize = sizeof ( CEnvManager );

	kdMemcpy ( l_pBuffer, this, uSize );

	if ( bVictory )
	{
		KDuint  uTotalScore = uScore * uTimes;

		m_uWin++;
		m_uWinCombo++;
		m_uFirstTurn = 0;

		if ( m_uWinComboMax < m_uWinCombo )
		{
			m_uWinComboMax = m_uWinCombo;
		}

		if ( m_uWinCashMax < uCash )
		{
			m_uWinCashMax = uCash;
		}

		if ( m_uWinScoreMax < uTotalScore )
		{
			m_uWinScoreMax = uTotalScore;
		}

		this->setCash ( this->getCash ( ) + uCash );
		this->setEnemyCash ( this->getEnemyCash ( ) - uCash );
        
		for ( KDuint  i = m_uStars [ m_uStage ]; i < 5; i++ )
		{
			if ( uScore >= l_uStarTable [ m_uStage ][ i ][ 0 ] &&
				 uTimes >= l_uStarTable [ m_uStage ][ i ][ 1 ] )
			{
				m_uStars [ m_uStage ] = i + 1; 
			}
		}
	}
	else
	{
		m_uLose++;
		m_uWinCombo = 0;
		m_uFirstTurn = 1;

		this->setCash ( this->getCash ( ) - uCash );
		this->setEnemyCash ( this->getEnemyCash ( ) + uCash );
	}	
}

KDvoid CEnvManager::setFirstTurn ( KDuint uValue )
{
	m_uFirstTurn = uValue;
}

KDvoid CEnvManager::setPushCount ( KDuint uValue )
{
	m_uPushCount = uValue;
}
