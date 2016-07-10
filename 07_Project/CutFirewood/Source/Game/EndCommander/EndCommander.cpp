/* -----------------------------------------------------------------------------------
 *
 *      File            EndCommander.cpp
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
#include "EndCommander.h"
#include "Scene/Main/MainScene.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Button/Button.h"
#include "Object/Label/LabelAtlasCostom.h"

cEndCommander::cEndCommander ( JBaseGameScene* pScene )
{
	// scene
	m_pScene		= pScene;

	//point
	m_tPos			= Point (   0,	 0 );
	m_tPosBack		= Point (  11,  73 );
	m_tPosPerfect	= Point ( 125, 242 );
	m_tPosBerserk	= Point ( 195, 221 );
	m_tPosTimeStop	= Point ( 208, 199 );
	m_tPosMaxPower	= Point ( 170, 178 );
	m_tPosTotal		= Point ( 227, 112 );
	m_tPosEndScore	= Point (  30, 218 );
	m_tPosBestScore	= Point (  30, 158 );
	m_tPosTwitter	= Point (  42,  46 );
	m_tPosFacebook	= Point ( 177,  46 );
	m_tPosRank		= Point ( 312,  46 );
	
	//button
	m_pBtnTwitter	= new cButton ( "result_twitter.png" , "result_twitter_push.png" , CC_CALLBACK_1 ( cEndCommander::onBtnArcadeMode, this ), eLayerGame_Result, 11, m_tPosTwitter    );
	m_pBtnFacebook	= new cButton ( "result_facebook.png", "result_facebook_push.png", CC_CALLBACK_1 ( cEndCommander::onBtnFacebook	 , this ), eLayerGame_Result, 12, m_tPosFacebook   );
	m_pBtnRank		= new cButton ( "result_rank.png"	 , "result_rank_push.png"	 , CC_CALLBACK_1 ( cEndCommander::onBtnRank		 , this ), eLayerGame_Result, 13, m_tPosRank	   );
	m_pBtnRetry		= new cButton ( "result_retry.png"	 , "result_retry_push.png"	 , CC_CALLBACK_1 ( cEndCommander::onBtnRetry	 , this ), eLayerGame_Result, 14, Point ( 320, 6 ) );
	m_pBtnNext		= new cButton ( "result_next.png"	 , "result_next_push.png"	 , CC_CALLBACK_1 ( cEndCommander::onBtnNext		 , this ), eLayerGame_Result, 14, Point ( 420, 6 ) );
	m_pBtnExit		= new cButton ( "result_next.png"	 , "result_next_push.png"	 , CC_CALLBACK_1 ( cEndCommander::onBtnExit		 , this ), eLayerGame_Result, 14, Point ( 420, 6 ) );

	m_pBtnTwitter ->setIsVisible ( false );
	m_pBtnFacebook->setIsVisible ( false );
	m_pBtnRank	  ->setIsVisible ( false );
	m_pBtnRetry	  ->setIsVisible ( false );
	m_pBtnNext	  ->setIsVisible ( false );
	m_pBtnExit	  ->setIsVisible ( false );
	m_pBtnTwitter ->setIsTouchEnable ( false );
	m_pBtnFacebook->setIsTouchEnable ( false );
	m_pBtnRank	  ->setIsTouchEnable ( false );
	m_pBtnRetry	  ->setIsTouchEnable ( false );
	m_pBtnNext	  ->setIsTouchEnable ( false );
	m_pBtnExit	  ->setIsTouchEnable ( false );
	
	m_pScene->addBtnObject ( m_pBtnTwitter	);
	m_pScene->addBtnObject ( m_pBtnFacebook	);
	m_pScene->addBtnObject ( m_pBtnRank		);
	m_pScene->addBtnObject ( m_pBtnRetry	);
	m_pScene->addBtnObject ( m_pBtnNext		);
	m_pScene->addBtnObject ( m_pBtnExit		);
	
	//sprite
	m_pSprBackNext		 = new cSprite ( "result_bg.png"	, eLayerGame_Result, m_tPosBack		);
	m_pSprBackExit		 = new cSprite ( "result_bg2.png"	, eLayerGame_Result, m_tPosBack		);
	m_pSprPerfectFailed	 = new cSprite ( "result_failed.png", eLayerGame_Result, m_tPosPerfect  );
	m_pSprBerserkFailed	 = new cSprite ( "result_failed.png", eLayerGame_Result, m_tPosBerserk  );
	m_pSprTimeStopFailed = new cSprite ( "result_failed.png", eLayerGame_Result, m_tPosTimeStop );
	m_pSprMaxPowerFailed = new cSprite ( "result_failed.png", eLayerGame_Result, m_tPosMaxPower );
	
	m_pSprBackNext	    ->setIsVisible ( false );
	m_pSprBackExit	    ->setIsVisible ( false );
	m_pSprPerfectFailed ->setIsVisible ( false );
	m_pSprBerserkFailed ->setIsVisible ( false );
	m_pSprTimeStopFailed->setIsVisible ( false );
	m_pSprMaxPowerFailed->setIsVisible ( false );

	m_pScene->addDrawObject ( m_pSprBackNext	   );
	m_pScene->addDrawObject ( m_pSprBackExit	   );
	m_pScene->addDrawObject ( m_pSprPerfectFailed  );
	m_pScene->addDrawObject ( m_pSprBerserkFailed  );
	m_pScene->addDrawObject ( m_pSprTimeStopFailed );
	m_pScene->addDrawObject ( m_pSprMaxPowerFailed );
	
	//label
	m_pLblPerfect = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosPerfect );
	m_pLblPerfect->addCharInfo ( '1',   0, 13, 10 );	m_pLblPerfect->addCharInfo ( '2',  13, 18, 15 );	m_pLblPerfect->addCharInfo ( '3',  31, 17, 14 );
	m_pLblPerfect->addCharInfo ( '4',  48, 18, 15 );	m_pLblPerfect->addCharInfo ( '5',  66, 17, 14 );	m_pLblPerfect->addCharInfo ( '6',  83, 17, 15 );
	m_pLblPerfect->addCharInfo ( '7', 100, 17, 15 );	m_pLblPerfect->addCharInfo ( '8', 117, 18, 15 );	m_pLblPerfect->addCharInfo ( '9', 135, 17, 15 );
	m_pLblPerfect->addCharInfo ( '0', 152, 17, 15 );	m_pLblPerfect->addCharInfo ( '+', 169, 15, 15 );	m_pLblPerfect->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblPerfect->addCharInfo ( 'E', 199, 14, 14 );	m_pLblPerfect->addCharInfo ( 'C', 213, 15, 15 );	m_pLblPerfect->addCharInfo ( '/', 228, 15, 15 );
	m_pLblPerfect->addCharInfo ( 'S', 243, 14, 14 );	m_pLblPerfect->addCharInfo ( 'C', 257, 14, 14 );	m_pLblPerfect->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblPerfect->addCharInfo ( 'R', 285, 14, 14 );	m_pLblPerfect->addCharInfo ( 'E', 299, 17, 17 );	m_pLblPerfect->addCharInfo ( ' ', 316,  5,  5 );

	m_pLblBerserk = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosBerserk );
	m_pLblBerserk->addCharInfo ( '1',   0, 13, 10 );	m_pLblBerserk->addCharInfo ( '2',  13, 18, 15 );	m_pLblBerserk->addCharInfo ( '3',  31, 17, 14 );
	m_pLblBerserk->addCharInfo ( '4',  48, 18, 15 );	m_pLblBerserk->addCharInfo ( '5',  66, 17, 14 );	m_pLblBerserk->addCharInfo ( '6',  83, 17, 15 );
	m_pLblBerserk->addCharInfo ( '7', 100, 17, 15 );	m_pLblBerserk->addCharInfo ( '8', 117, 18, 15 );	m_pLblBerserk->addCharInfo ( '9', 135, 17, 15 );
	m_pLblBerserk->addCharInfo ( '0', 152, 17, 15 );	m_pLblBerserk->addCharInfo ( '+', 169, 15, 15 );	m_pLblBerserk->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblBerserk->addCharInfo ( 'E', 199, 14, 14 );	m_pLblBerserk->addCharInfo ( 'C', 213, 15, 15 );	m_pLblBerserk->addCharInfo ( '/', 228, 15, 15 );
	m_pLblBerserk->addCharInfo ( 'S', 243, 14, 14 );	m_pLblBerserk->addCharInfo ( 'C', 257, 14, 14 );	m_pLblBerserk->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblBerserk->addCharInfo ( 'R', 285, 14, 14 );	m_pLblBerserk->addCharInfo ( 'E', 299, 17, 17 );	m_pLblBerserk->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblTimeStop = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosTimeStop );
	m_pLblTimeStop->addCharInfo ( '1',   0, 13, 10 );	m_pLblTimeStop->addCharInfo ( '2',  13, 18, 15 );	m_pLblTimeStop->addCharInfo ( '3',  31, 17, 14 );
	m_pLblTimeStop->addCharInfo ( '4',  48, 18, 15 );	m_pLblTimeStop->addCharInfo ( '5',  66, 17, 14 );	m_pLblTimeStop->addCharInfo ( '6',  83, 17, 15 );
	m_pLblTimeStop->addCharInfo ( '7', 100, 17, 15 );	m_pLblTimeStop->addCharInfo ( '8', 117, 18, 15 );	m_pLblTimeStop->addCharInfo ( '9', 135, 17, 15 );
	m_pLblTimeStop->addCharInfo ( '0', 152, 17, 15 );	m_pLblTimeStop->addCharInfo ( '+', 169, 15, 15 );	m_pLblTimeStop->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblTimeStop->addCharInfo ( 'E', 199, 14, 14 );	m_pLblTimeStop->addCharInfo ( 'C', 213, 15, 15 );	m_pLblTimeStop->addCharInfo ( '/', 228, 15, 15 );
	m_pLblTimeStop->addCharInfo ( 'S', 243, 14, 14 );	m_pLblTimeStop->addCharInfo ( 'C', 257, 14, 14 );	m_pLblTimeStop->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblTimeStop->addCharInfo ( 'R', 285, 14, 14 );	m_pLblTimeStop->addCharInfo ( 'E', 299, 17, 17 );	m_pLblTimeStop->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblMaxPower = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosMaxPower );
	m_pLblMaxPower->addCharInfo ( '1',   0, 13, 10 );	m_pLblMaxPower->addCharInfo ( '2',  13, 18, 15 );	m_pLblMaxPower->addCharInfo ( '3',  31, 17, 14 );
	m_pLblMaxPower->addCharInfo ( '4',  48, 18, 15 );	m_pLblMaxPower->addCharInfo ( '5',  66, 17, 14 );	m_pLblMaxPower->addCharInfo ( '6',  83, 17, 15 );
	m_pLblMaxPower->addCharInfo ( '7', 100, 17, 15 );	m_pLblMaxPower->addCharInfo ( '8', 117, 18, 15 );	m_pLblMaxPower->addCharInfo ( '9', 135, 17, 15 );
	m_pLblMaxPower->addCharInfo ( '0', 152, 17, 15 );	m_pLblMaxPower->addCharInfo ( '+', 169, 15, 15 );	m_pLblMaxPower->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblMaxPower->addCharInfo ( 'E', 199, 14, 14 );	m_pLblMaxPower->addCharInfo ( 'C', 213, 15, 15 );	m_pLblMaxPower->addCharInfo ( '/', 228, 15, 15 );
	m_pLblMaxPower->addCharInfo ( 'S', 243, 14, 14 );	m_pLblMaxPower->addCharInfo ( 'C', 257, 14, 14 );	m_pLblMaxPower->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblMaxPower->addCharInfo ( 'R', 285, 14, 14 );	m_pLblMaxPower->addCharInfo ( 'E', 299, 17, 17 );	m_pLblMaxPower->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblTotal = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosTotal );
	m_pLblTotal->addCharInfo ( '1',   0, 13, 10 );	m_pLblTotal->addCharInfo ( '2',  13, 18, 15 );	m_pLblTotal->addCharInfo ( '3',  31, 17, 14 );
	m_pLblTotal->addCharInfo ( '4',  48, 18, 15 );	m_pLblTotal->addCharInfo ( '5',  66, 17, 14 );	m_pLblTotal->addCharInfo ( '6',  83, 17, 15 );
	m_pLblTotal->addCharInfo ( '7', 100, 17, 15 );	m_pLblTotal->addCharInfo ( '8', 117, 18, 15 );	m_pLblTotal->addCharInfo ( '9', 135, 17, 15 );
	m_pLblTotal->addCharInfo ( '0', 152, 17, 15 );	m_pLblTotal->addCharInfo ( '+', 169, 15, 15 );	m_pLblTotal->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblTotal->addCharInfo ( 'E', 199, 14, 14 );	m_pLblTotal->addCharInfo ( 'C', 213, 15, 15 );	m_pLblTotal->addCharInfo ( '/', 228, 15, 15 );
	m_pLblTotal->addCharInfo ( 'S', 243, 14, 14 );	m_pLblTotal->addCharInfo ( 'C', 257, 14, 14 );	m_pLblTotal->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblTotal->addCharInfo ( 'R', 285, 14, 14 );	m_pLblTotal->addCharInfo ( 'E', 299, 17, 17 );	m_pLblTotal->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblEndScore = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosEndScore );
	m_pLblEndScore->addCharInfo ( '1',   0, 13, 10 );	m_pLblEndScore->addCharInfo ( '2',  13, 18, 15 );	m_pLblEndScore->addCharInfo ( '3',  31, 17, 14 );
	m_pLblEndScore->addCharInfo ( '4',  48, 18, 15 );	m_pLblEndScore->addCharInfo ( '5',  66, 17, 14 );	m_pLblEndScore->addCharInfo ( '6',  83, 17, 15 );
	m_pLblEndScore->addCharInfo ( '7', 100, 17, 15 );	m_pLblEndScore->addCharInfo ( '8', 117, 18, 15 );	m_pLblEndScore->addCharInfo ( '9', 135, 17, 15 );
	m_pLblEndScore->addCharInfo ( '0', 152, 17, 15 );	m_pLblEndScore->addCharInfo ( '+', 169, 15, 15 );	m_pLblEndScore->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblEndScore->addCharInfo ( 'E', 199, 14, 14 );	m_pLblEndScore->addCharInfo ( 'C', 213, 15, 15 );	m_pLblEndScore->addCharInfo ( '/', 228, 15, 15 );
	m_pLblEndScore->addCharInfo ( 'S', 243, 14, 14 );	m_pLblEndScore->addCharInfo ( 'C', 257, 14, 14 );	m_pLblEndScore->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblEndScore->addCharInfo ( 'R', 285, 14, 14 );	m_pLblEndScore->addCharInfo ( 'E', 299, 17, 17 );	m_pLblEndScore->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblBestScore = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, m_tPosBestScore );
	m_pLblBestScore->addCharInfo ( '1',   0, 13, 10 );	m_pLblBestScore->addCharInfo ( '2',  13, 18, 15 );	m_pLblBestScore->addCharInfo ( '3',  31, 17, 14 );
	m_pLblBestScore->addCharInfo ( '4',  48, 18, 15 );	m_pLblBestScore->addCharInfo ( '5',  66, 17, 14 );	m_pLblBestScore->addCharInfo ( '6',  83, 17, 15 );
	m_pLblBestScore->addCharInfo ( '7', 100, 17, 15 );	m_pLblBestScore->addCharInfo ( '8', 117, 18, 15 );	m_pLblBestScore->addCharInfo ( '9', 135, 17, 15 );
	m_pLblBestScore->addCharInfo ( '0', 152, 17, 15 );	m_pLblBestScore->addCharInfo ( '+', 169, 15, 15 );	m_pLblBestScore->addCharInfo ( 'S', 184, 15, 15 );
	m_pLblBestScore->addCharInfo ( 'E', 199, 14, 14 );	m_pLblBestScore->addCharInfo ( 'C', 213, 15, 15 );	m_pLblBestScore->addCharInfo ( '/', 228, 15, 15 );
	m_pLblBestScore->addCharInfo ( 'S', 243, 14, 14 );	m_pLblBestScore->addCharInfo ( 'C', 257, 14, 14 );	m_pLblBestScore->addCharInfo ( 'O', 271, 14, 14 );
	m_pLblBestScore->addCharInfo ( 'R', 285, 14, 14 );	m_pLblBestScore->addCharInfo ( 'E', 299, 17, 17 );	m_pLblBestScore->addCharInfo ( ' ', 316,  5,  5 );
	
	m_pLblPerfect	->setIsVisible ( false );
	m_pLblBerserk	->setIsVisible ( false );
	m_pLblTimeStop	->setIsVisible ( false );
	m_pLblMaxPower	->setIsVisible ( false );
	m_pLblTotal		->setIsVisible ( false );
	m_pLblEndScore	->setIsVisible ( false );
	m_pLblBestScore	->setIsVisible ( false );
	
	m_pScene->addDrawObject ( m_pLblPerfect		);
	m_pScene->addDrawObject ( m_pLblBerserk		);
	m_pScene->addDrawObject ( m_pLblTimeStop	);
	m_pScene->addDrawObject ( m_pLblMaxPower	);
	m_pScene->addDrawObject ( m_pLblTotal		);
	m_pScene->addDrawObject ( m_pLblEndScore	);
	m_pScene->addDrawObject ( m_pLblBestScore	);
	
	// switch
	m_eResultType	= eResultType_Next;

	// step
	m_nStep			= 0;
	m_dStepLeftTime	= 0;

	//time, score
	m_dAddTime		= 0;
	m_dAddScore		= 0;

	//switch
	m_bIsRetry		= false;
	
	// facebook
//	m_pSession		= nullptr;
//	m_pLoginDialog	= nullptr;
//	m_sFacebookName	= nullptr;
}

cEndCommander::~cEndCommander ( KDvoid )
{
	// sprite
	m_pSprBackNext			= nullptr;
	m_pSprBackExit			= nullptr;
	m_pSprPerfectFailed		= nullptr;
	m_pSprBerserkFailed		= nullptr;
	m_pSprTimeStopFailed	= nullptr;
	m_pSprMaxPowerFailed	= nullptr;
	
	// button
	m_pBtnTwitter			= nullptr;
	m_pBtnFacebook			= nullptr;
	m_pBtnRank				= nullptr;
	m_pBtnRetry				= nullptr;
	m_pBtnNext				= nullptr;
	m_pBtnExit				= nullptr;
	
	// label
	m_pLblPerfect			= nullptr;
	m_pLblBerserk			= nullptr;
	m_pLblTimeStop			= nullptr;
	m_pLblMaxPower			= nullptr;
	m_pLblTotal				= nullptr;
	m_pLblEndScore			= nullptr;
	m_pLblBestScore			= nullptr;

	// facebook
//	if ( m_pSession	     != nil)	{	[m_pSession		release);	m_pSession		= nil;	}
//	if ( m_pLoginDialog  != nil)	{	[m_pLoginDialog	release);	m_pLoginDialog	= nil;	}
//	if ( m_sFacebookName != nil)	{	[m_sFacebookName release);	m_sFacebookName	= nil;	}
}

KDvoid cEndCommander::viewNext ( KDbool bIsPerfectSuccess, KDbool bIsBerserkSuccess, KDbool bIsTimeStopSuccess, KDint nMaxPowerCount, KDint nTotalScore )
{
	// switch
	m_eResultType = eResultType_Next;
	
	// step
	m_nStep = 0;
	m_dStepLeftTime	= 0.15f;
	
	// point
	m_tPos = Point ( 0, _SCR_H);

	// sprite
	m_pSprBackNext->setIsVisible ( true );
	if ( bIsPerfectSuccess  == false )	m_pSprPerfectFailed ->setIsVisible ( true  );
	else								m_pSprPerfectFailed ->setIsVisible ( false );
	if ( bIsBerserkSuccess	== false )	m_pSprBerserkFailed ->setIsVisible ( true  );
	else								m_pSprBerserkFailed ->setIsVisible ( false );
	if ( bIsTimeStopSuccess == false )	m_pSprTimeStopFailed->setIsVisible ( true  );
	else								m_pSprTimeStopFailed->setIsVisible ( false );
	if ( nMaxPowerCount     == 0     )	m_pSprMaxPowerFailed->setIsVisible ( true  );
	else								m_pSprMaxPowerFailed->setIsVisible ( false );
	
	m_pSprBackNext->setPoint ( m_tPos + m_tPosBack );
	m_pSprPerfectFailed ->setPoint ( ( m_tPos + m_tPosPerfect  ) + Point ( 5, 0 ) );
	m_pSprBerserkFailed ->setPoint ( ( m_tPos + m_tPosBerserk  ) + Point ( 5, 0 ) );
	m_pSprTimeStopFailed->setPoint ( ( m_tPos + m_tPosTimeStop ) + Point ( 5, 0 ) );
	m_pSprMaxPowerFailed->setPoint ( ( m_tPos + m_tPosMaxPower ) + Point ( 5, 0 ) );
	
	// button
	m_pBtnNext->setIsVisible ( true );
	m_pBtnNext->setOpacity ( 0 );
	
	// time
	m_dAddTime = 0;
	if ( bIsPerfectSuccess  == true )	m_dAddTime += 10;
	if ( bIsBerserkSuccess	== true )	m_dAddTime += 5;
	if ( bIsTimeStopSuccess == true )	m_dAddTime += 5;
	if ( nMaxPowerCount	    >= 10   )	m_dAddTime += 10;
	
	// score
	m_dAddScore = 0;
	if ( bIsPerfectSuccess  == true )	m_dAddScore += 10000;
	if ( bIsBerserkSuccess  == true )	m_dAddScore += 5000;
	if ( bIsTimeStopSuccess == true )	m_dAddScore += 8000;
	m_dAddScore += nMaxPowerCount * 500;
	
	// label
	if ( bIsPerfectSuccess  == false )	m_pLblPerfect ->setIsVisible ( false );
	else								m_pLblPerfect ->setIsVisible ( true  );
	if ( bIsBerserkSuccess	== false )	m_pLblBerserk ->setIsVisible ( false );
	else								m_pLblBerserk ->setIsVisible ( true  );
	if ( bIsTimeStopSuccess == false )	m_pLblTimeStop->setIsVisible ( false );
	else								m_pLblTimeStop->setIsVisible ( true  );
	if ( nMaxPowerCount     == 0     )	m_pLblMaxPower->setIsVisible ( false );
	else								m_pLblMaxPower->setIsVisible ( true  );
	m_pLblTotal->setIsVisible ( true );

	m_pLblPerfect	->setPoint ( m_tPos + m_tPosPerfect  );
	m_pLblBerserk	->setPoint ( m_tPos + m_tPosBerserk  );
	m_pLblTimeStop	->setPoint ( m_tPos + m_tPosTimeStop );
	m_pLblMaxPower	->setPoint ( m_tPos + m_tPosMaxPower );
	m_pLblTotal		->setPoint ( m_tPos + m_tPosTotal	 );
	
	m_pLblPerfect ->setString ( ccszf ( "+%d SEC/+%d SCORE", 10, 10000 ) );
	m_pLblBerserk ->setString ( ccszf ( "+%d SEC/+%d SCORE",  5, 5000  ) );
	m_pLblTimeStop->setString ( ccszf ( "+%d SEC/+%d SCORE",  5, 8000  ) );
	if ( nMaxPowerCount >= 10 )	m_pLblMaxPower->setString ( ccszf ( "+%d SEC/+%d SCORE", 10, nMaxPowerCount * 500 ) );
	else						m_pLblMaxPower->setString ( ccszf ( "+%d SCORE"		   ,     nMaxPowerCount * 500 ) );
	m_pLblTotal->setString ( ccszf ( "%d", (KDint) ( nTotalScore + m_dAddScore ) ) );
	
	// sound
	if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "result_clear.wav" );
	}
}

KDvoid cEndCommander::viewExit ( KDint nTotalScore )
{
	// game center
//	[ [gameKitHelper sharedGameKitHelper] submitScore:totalScore category:"52W8F4SAD6_R1" );
	
	// result type
	m_eResultType = eResultType_Exit;
	
	// best score
	KDint	nBestScore = 0;

	/*
	std::string		path = [ [ NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)  objectAtIndex:0 ]  UTF8String );
	path += "/best_score.dat";
	
	//open
	FILE *fp = fopen(path.c_str(), "rb");
	if(fp != NULL)
	{
		//write
		fread( &bestScore, sizeof(int), 1, fp );
		
		//close
		fclose(fp);
	}
	*/
	if ( nTotalScore > nBestScore )
	{
		nBestScore = nTotalScore;
		
		/*
		FILE *fp = fopen(path.c_str(), "wb");
		if(fp != NULL)
		{
			//write
			fwrite( &bestScore, sizeof(int), 1, fp );
			
			//close
			fclose(fp);
		}
		*/
	}
	
	//step
	m_nStep			= 0;
	m_dStepLeftTime	= 0.15f;
	
	// point
	m_tPos = Point ( 0, _SCR_H );
	
	// sprite
	m_pSprBackExit->setIsVisible ( true );
	m_pSprBackExit->setPoint ( m_tPos + m_tPosBack );
	
	// button
	m_pBtnTwitter ->setIsVisible ( true );
	m_pBtnFacebook->setIsVisible ( true );
	m_pBtnRank	  ->setIsVisible ( true );
	m_pBtnRetry	  ->setIsVisible ( true );
	m_pBtnExit	  ->setIsVisible ( true );
	m_pBtnTwitter ->setPoint ( m_tPosTwitter );
	m_pBtnFacebook->setPoint ( m_tPosFacebook );
	m_pBtnRank	  ->setPoint ( m_tPosRank );
	m_pBtnRetry	  ->setOpacity ( 0 );
	m_pBtnExit	  ->setOpacity ( 0 );
	
	// label
	m_pLblEndScore ->setIsVisible ( true );
	m_pLblBestScore->setIsVisible ( true );
	
	m_pLblEndScore ->setPoint ( m_tPos + m_tPosEndScore );
	m_pLblBestScore->setPoint ( m_tPos + m_tPosBestScore );
	
	m_pLblEndScore ->setString ( ccszf ( "%d", nTotalScore ) );
	m_pLblBestScore->setString ( ccszf ( "%d", nBestScore  ) );
	
	// switch
	m_bIsRetry = false;

	// sound
	if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "result_over.wav" );
	}
}

KDvoid cEndCommander::update ( KDdouble dLeftTime )
{
	switch ( m_eResultType )
	{
		case eResultType_Next :	this->updateNext ( dLeftTime );	break;
		case eResultType_Exit :	this->updateExit ( dLeftTime );	break;
	}
}

KDvoid cEndCommander::updateNext ( KDdouble dLeftTime )
{
	// in
	if ( m_nStep == 0 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// check
		if ( m_dStepLeftTime < 0 )
		{
			// point
			m_tPos.y = 0;
			m_pSprBackNext		->setPoint ( m_tPos + m_tPosBack	   );
			m_pSprPerfectFailed	->setPoint ( ( m_tPos + m_tPosPerfect  ) + Point ( 5, 0 ) );
			m_pSprBerserkFailed	->setPoint ( ( m_tPos + m_tPosBerserk  ) + Point ( 5, 0 ) );
			m_pSprTimeStopFailed->setPoint ( ( m_tPos + m_tPosTimeStop ) + Point ( 5, 0 ) );
			m_pSprMaxPowerFailed->setPoint ( ( m_tPos + m_tPosMaxPower ) + Point ( 5, 0 ) );
			m_pLblPerfect		->setPoint ( m_tPos + m_tPosPerfect    );
			m_pLblBerserk		->setPoint ( m_tPos + m_tPosBerserk    );
			m_pLblTimeStop		->setPoint ( m_tPos + m_tPosTimeStop   );
			m_pLblMaxPower		->setPoint ( m_tPos + m_tPosMaxPower   );
			m_pLblTotal			->setPoint ( m_tPos + m_tPosTotal      );
			
			// button
			m_pBtnNext->setIsTouchEnable ( true );
			m_pBtnNext->setOpacity ( 255 );
			
			// score
			m_pScene->msgAddScore ( m_dAddScore );
			
			// time
			m_pScene->msgAddTime ( m_dAddTime );
			
			// step
			++m_nStep;
		}
		else
		{
			// percent
			KDfloat		fPercent = ( 0.15f - m_dStepLeftTime ) / 0.15f;
			
			// point
			m_tPos.y = _SCR_H - _SCR_H*fPercent;
			m_pSprBackNext		->setPoint ( m_tPos + m_tPosBack	   );
			m_pSprPerfectFailed	->setPoint ( ( m_tPos + m_tPosPerfect  ) + Point ( 5, 0 ) );
			m_pSprBerserkFailed	->setPoint ( ( m_tPos + m_tPosBerserk  ) + Point ( 5, 0 ) );
			m_pSprTimeStopFailed->setPoint ( ( m_tPos + m_tPosTimeStop ) + Point ( 5, 0 ) );
			m_pSprMaxPowerFailed->setPoint ( ( m_tPos + m_tPosMaxPower ) + Point ( 5, 0 ) );
			m_pLblPerfect		->setPoint ( m_tPos + m_tPosPerfect    );
			m_pLblBerserk		->setPoint ( m_tPos + m_tPosBerserk    );
			m_pLblTimeStop		->setPoint ( m_tPos + m_tPosTimeStop   );
			m_pLblMaxPower		->setPoint ( m_tPos + m_tPosMaxPower   );
			m_pLblTotal			->setPoint ( m_tPos + m_tPosTotal      );
			
			// opacity
			KDfloat		fOpacity = fPercent * 255;
			m_pBtnNext->setOpacity ( fOpacity );
		}
	}
	// stay
	else if ( m_nStep == 1 )
	{
		
	}
	// out
	else if ( m_nStep == 2 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// check
		if ( m_dStepLeftTime < 0 )
		{
			// visible
			m_pSprBackNext		->setIsVisible ( false );
			m_pSprPerfectFailed	->setIsVisible ( false );
			m_pSprBerserkFailed	->setIsVisible ( false );
			m_pSprTimeStopFailed->setIsVisible ( false );
			m_pSprMaxPowerFailed->setIsVisible ( false );
			m_pLblPerfect		->setIsVisible ( false );
			m_pLblBerserk		->setIsVisible ( false );
			m_pLblTimeStop		->setIsVisible ( false );
			m_pLblMaxPower		->setIsVisible ( false );
			m_pLblTotal			->setIsVisible ( false );
			m_pBtnTwitter		->setIsVisible ( false );
			m_pBtnFacebook		->setIsVisible ( false );
			m_pBtnRank			->setIsVisible ( false );
			m_pBtnNext			->setIsVisible ( false );
			
			m_pScene->msgPlay ( );
		}
		else
		{
			// percent
			KDfloat		fPercent = ( 0.15f - m_dStepLeftTime ) / 0.15f;
			
			// point
			m_tPos.y = _SCR_H * fPercent;
			m_pSprBackNext		->setPoint ( m_tPos + m_tPosBack	   );
			m_pSprPerfectFailed	->setPoint ( ( m_tPos + m_tPosPerfect  ) + Point ( 5, 0 ) );
			m_pSprBerserkFailed	->setPoint ( ( m_tPos + m_tPosBerserk  ) + Point ( 5, 0 ) );
			m_pSprTimeStopFailed->setPoint ( ( m_tPos + m_tPosTimeStop ) + Point ( 5, 0 ) );
			m_pSprMaxPowerFailed->setPoint ( ( m_tPos + m_tPosMaxPower ) + Point ( 5, 0 ) );
			m_pLblPerfect		->setPoint ( m_tPos + m_tPosPerfect    );
			m_pLblBerserk		->setPoint ( m_tPos + m_tPosBerserk    );
			m_pLblTimeStop		->setPoint ( m_tPos + m_tPosTimeStop   );
			m_pLblMaxPower		->setPoint ( m_tPos + m_tPosMaxPower   );
			m_pLblTotal			->setPoint ( m_tPos + m_tPosTotal      );
			
			// opacity
			KDfloat		fOpacity = 255 - fPercent * 255;
			m_pBtnNext->setOpacity ( fOpacity );
		}
	}
}

KDvoid cEndCommander::updateExit ( KDdouble dLeftTime )
{
	// in
	if ( m_nStep == 0 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		//check
		if ( m_dStepLeftTime < 0 )
		{
			// point
			m_tPos.y = 0;
			m_pSprBackExit	->setPoint ( m_tPos + m_tPosBack		);
			m_pLblEndScore	->setPoint ( m_tPos + m_tPosEndScore	);
			m_pLblBestScore	->setPoint ( m_tPos + m_tPosBestScore	);
			m_pBtnTwitter	->setPoint ( m_tPos + m_tPosTwitter		);
			m_pBtnFacebook	->setPoint ( m_tPos + m_tPosFacebook	);
			m_pBtnRank		->setPoint ( m_tPos + m_tPosRank		);
			
			// button
			m_pBtnTwitter	->setIsTouchEnable ( true );
			m_pBtnFacebook	->setIsTouchEnable ( true );
			m_pBtnRank		->setIsTouchEnable ( true );
			m_pBtnRetry		->setIsTouchEnable ( true );
			m_pBtnExit		->setIsTouchEnable ( true );

			m_pBtnRetry->setOpacity ( 255 );
			m_pBtnExit ->setOpacity ( 255 );
			
			// score
			m_pScene->msgAddScore ( m_dAddScore );
			
			// time
		 	m_pScene->msgAddTime ( m_dAddTime );
			
			// step
			++m_nStep;
		}
		else
		{
			// percent
			KDfloat	fPercent = ( 0.15f - m_dStepLeftTime ) / 0.15f;
			
			//point
			m_tPos.y = _SCR_H - _SCR_H * fPercent;

			m_pSprBackExit	->setPoint ( m_tPos + m_tPosBack		);
			m_pLblEndScore	->setPoint ( m_tPos + m_tPosEndScore	);
			m_pLblBestScore	->setPoint ( m_tPos + m_tPosBestScore	);
			m_pBtnTwitter	->setPoint ( m_tPos + m_tPosTwitter		);
			m_pBtnFacebook	->setPoint ( m_tPos + m_tPosFacebook	);
			m_pBtnRank		->setPoint ( m_tPos + m_tPosRank		);
			
			// opacity
			KDfloat		fOpacity = fPercent * 255;
			m_pBtnRetry->setOpacity ( fOpacity );
			m_pBtnExit ->setOpacity ( fOpacity );
		}
	}
	// stay
	else if ( m_nStep == 1 )
	{
	}
	else if ( m_nStep == 2 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// check
		if ( m_dStepLeftTime < 0 )
		{
			// visible
			m_pSprBackNext		->setIsVisible ( false );
			m_pSprPerfectFailed	->setIsVisible ( false );
			m_pSprBerserkFailed	->setIsVisible ( false );
			m_pSprTimeStopFailed->setIsVisible ( false );
			m_pSprMaxPowerFailed->setIsVisible ( false );
			m_pLblPerfect		->setIsVisible ( false );
			m_pLblBerserk		->setIsVisible ( false );
			m_pLblTimeStop		->setIsVisible ( false );
			m_pLblMaxPower		->setIsVisible ( false );
			m_pLblTotal			->setIsVisible ( false );
			m_pBtnTwitter		->setIsVisible ( false );
			m_pBtnFacebook		->setIsVisible ( false );
			m_pBtnRank			->setIsVisible ( false );
			m_pBtnNext			->setIsVisible ( false );

			if ( m_bIsRetry == true )
			{
				m_pScene->msgRetry ( );
			}
		}
		else
		{
			// percent
			KDfloat		fPercent = ( 0.15f - m_dStepLeftTime ) / 0.15f;
			
			// point
			m_tPos.y = _SCR_H * fPercent;

			m_pSprBackExit	->setPoint ( m_tPos + m_tPosBack		);
			m_pLblEndScore	->setPoint ( m_tPos + m_tPosEndScore	);
			m_pLblBestScore	->setPoint ( m_tPos + m_tPosBestScore	);
			m_pBtnTwitter	->setPoint ( m_tPos + m_tPosTwitter		);
			m_pBtnFacebook	->setPoint ( m_tPos + m_tPosFacebook	);
			m_pBtnRank		->setPoint ( m_tPos + m_tPosRank		);
			
			// opacity
			KDfloat		fOpacity = 255 - fPercent * 255;
			m_pBtnRetry->setOpacity ( fOpacity );
			m_pBtnExit ->setOpacity ( fOpacity );
		}
	}
}

KDvoid cEndCommander::onBtnArcadeMode ( Object* pSender )
{

}

KDvoid cEndCommander::onBtnTwitter ( Object* pSender )
{
	/*
	[SHKTwitter setScore:m_pLblEndScore->getString());
	SHKTwitter* controller = [[SHKTwitter alloc] init);
	SHKItem* item = [ [ [SHKItem alloc] init ] autorelease );
	item.shareType = SHKShareTypeText;
	controller.item = item;
	
	// share and/or show UI
	[controller share);
	*/
}

KDvoid cEndCommander::onBtnFacebook ( Object* pSender )
{
	/*
	if(m_pSession == nil)
	{
		// Set these values from your application page on http://www.facebook.com/developers
		// Keep in mind that this method is not as secure as using the sessionForApplication:getSessionProxy:delegate method!
		// These values are from a dummy facebook app I made called MyGrades - feel free to play around!
		static NSString* kApiKey	= "d7ee1fc72f84654a667c06abc5e78bdb";
		static NSString* kApiSecret = "36f9c878597ce8a5f2efe369fd5222c3";
		m_pSession = [[FBSession sessionForApplication:kApiKey secret:kApiSecret delegate:self] retain);
		
		// Load a previous session from disk if available.  Note this will call session:didLogin if a valid session exists.
		[m_pSession resume);
	}
	
	m_bIsPosting = YES;
	// If we're not logged in, log in first...
	if( ![m_pSession isConnected] )
	{
		m_pLoginDialog = nil;
		m_pLoginDialog = [[FBLoginDialog alloc] init);	
		[m_pLoginDialog show);	
	}
	// If we have a session and a name, post to the wall!
	else if (m_sFacebookName != nil)
	{
		[self postToWall);
	}
	// Otherwise, we don't have a name yet, just wait for that to come through.
	*/
}

KDvoid cEndCommander::onBtnRank	( Object* pSender )
{
//	[ [gameKitHelper sharedGameKitHelper] showLeaderboard );
}

KDvoid cEndCommander::onBtnRetry ( Object* pSender )
{
	// btn
	m_pBtnTwitter ->setIsTouchEnable ( false );
	m_pBtnFacebook->setIsTouchEnable ( false );
	m_pBtnRank	  ->setIsTouchEnable ( false );
	m_pBtnExit	  ->setIsTouchEnable ( false );

	// step
	++m_nStep;
	m_dStepLeftTime = 0.15f;

	// switch
	m_bIsRetry = true;
}

KDvoid cEndCommander::onBtnNext ( Object* pSender )
{
	// btn
	m_pBtnNext->setIsTouchEnable ( false );
	
	// step
	++m_nStep;
	m_dStepLeftTime = 0.15f;
}

KDvoid cEndCommander::onBtnExit	( Object* pSender )
{
	// btn
	m_pBtnTwitter ->setIsTouchEnable ( false );
	m_pBtnFacebook->setIsTouchEnable ( false );
	m_pBtnRank	  ->setIsTouchEnable ( false );
	m_pBtnExit	  ->setIsTouchEnable ( false );
	
	// go race scene
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, MainScene::create ( ) ) );
}

/*
#pragma mark FBSessionDelegate methods

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)session:(FBSession*)session didLogin:(FBUID)uid
{
	[self getFacebookName);
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)session:(FBSession*)session willLogout:(FBUID)uid
{
	m_sFacebookName = nil;
}

#pragma mark Get Facebook Name Helper

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)getFacebookName
{
	NSString* fql = [NSString stringWithFormat:"select uid,name from user where uid == %lld", m_pSession.uid);
	NSDictionary* params = [NSDictionary dictionaryWithObject:fql forKey:"query");
	[[FBRequest requestWithDelegate:self] call:"facebook.fql.query" params:params);
}

#pragma mark FBRequestDelegate methods

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)request:(FBRequest*)request didLoad:(id)result
{
	if ([request.method isEqualToString:"facebook.fql.query"])
	{
		NSArray* users = result;
		NSDictionary* user = [users objectAtIndex:0);
		NSString* name = [user objectForKey:"name");
		m_sFacebookName = name;		
		if (m_bIsPosting)
		{
			[self postToWall);
			m_bIsPosting = NO;
		}
	}
}

#pragma mark Post to Wall Helper

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)postToWall
{
	FBStreamDialog* dialog = [[[FBStreamDialog alloc] init] autorelease);
	dialog.userMessagePrompt = "Enter your message:";
	dialog.attachment = [NSString stringWithFormat:"{\"name\":\"I got up score to %@ in NURUNGEE!!\",\"href\":\"http://itunes.apple.com/us/app/NURUNGEE/id428909174?mt=8/\",\"caption\":\"This game is SO ADDICTIVE!\",\"description\":\"\",\"media\":[{\"type\":\"image\",\"src\":\"http://www.icontenter.com/fowcom/icon_100.png\",\"href\":\"http://itunes.apple.com/us/app/NURUNGEE/id428909174?mt=8/\"}]}",
						 m_pLblEndScore->getString() );
	dialog.actionLinks = "[{\"text\":\"Get NURUNGEE!\",\"href\":\"http://itunes.apple.com/us/app/NURUNGEE/id428909174?mt=8/\"}]";
	[dialog show);
}

#pragma mark Memory Cleanup

//----------------------------------------------------------------------------------------------
// Name : JINE
// Desc : 
//----------------------------------------------------------------------------------------------
- (void)viewDidUnload
{
}


@end
*/