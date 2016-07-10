/* -----------------------------------------------------------------------------------
 *
 *      File            ArcadeScene.cpp
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
#include "ArcadeScene.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Label/LabelAtlasCostom.h"
#include "Object/ColorLayer/ColorLayer.h"
#include "Game/BackImg/BackImg.h"
#include "Game/StartDirector/StartDirector.h"
#include "Game/Commander/Commander.h"
#include "Game/Score/Score.h"
#include "Game/GameTime/GameTime.h"
#include "Game/House/House.h"
#include "Game/Cloud/Cloud.h"
#include "Game/EndCommander/EndCommander.h"

// const
static const KDint		l_nStartTime		= 60;
static const KDint		l_nStartClearPoint	= 50;
static KDbool			l_bIsPose			= false;

ArcadeScene::ArcadeScene ( KDvoid )
: m_pBackImg		( nullptr )
, m_pStartDirector	( nullptr )
, m_pCommander		( nullptr )
, m_pScore			( nullptr )
, m_pHouse			( nullptr )
, m_pCloud			( nullptr )
, m_pEndCommander	( nullptr )
, m_pPoseColorLayer ( nullptr )
, m_pPoseSprite		( nullptr )
, m_pLabelTemp		( nullptr )
{

}

ArcadeScene::~ArcadeScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBackImg		);
	CC_SAFE_RELEASE ( m_pStartDirector	);
	CC_SAFE_RELEASE ( m_pCommander		);
	CC_SAFE_RELEASE ( m_pScore			);
	CC_SAFE_RELEASE ( m_pCloud			);
	CC_SAFE_RELEASE ( m_pEndCommander	);

	m_pPoseColorLayer	= nullptr;
	m_pPoseSprite		= nullptr;
	m_pLabelTemp		= nullptr;
}

KDbool ArcadeScene::init ( KDvoid )
{
	if ( !JBaseGameScene::init ( ) )
	{
		return false;
	}

	// back image
	m_pBackImg = new cBackImg ( this, "game_bg_1.png", "game_bg_1_gray.png" );
	m_pBackImg->addSprDeco ( "game_bg_1_deco.png", "game_bg_1_deco_gray.png", eLayerGame_Deco );

	// start director
	m_pStartDirector = new cStartDirector ( this );
	m_pStartDirector->start ( );

	// commander
	m_pCommander = new cCommander ( this, 0, 0 );

	// player
	this->addPlayer ( );

	// house lv
	m_nHouseLv = 0;

	//clear point
	m_nClearLeftPoint = l_nStartClearPoint;

	// score
	m_pScore = new cScore ( this );

	// game time
	m_pGameTime = new cGameTime ( this, l_nStartTime );
					
	// house
	m_pHouse = new cHouse ( this, 0, 5, Point ( _SCR_W_HARP, 120 ) );
	m_pHouse->view ( m_nHouseLv, false );

	// cloud
	m_pCloud = new cCloud ( this );

	// end commander
	m_pEndCommander = new cEndCommander ( this );

	// step
	m_eState = eArcadeTypeState_Start;

	// pose
	Color4B		tColor;
	tColor.r = 0;
	tColor.g = 0;
	tColor.b = 0;
	tColor.a = 0;
	
	m_pPoseColorLayer = new cColorLayer ( tColor, eLayerGame_Pose, Size ( _SCR_W, _SCR_H ), Point::ZERO );
	m_pPoseSprite	  = new cSprite ( "pose.png", eLayerGame_Pose, Point ( 105, 140 ) );
	
	m_pPoseColorLayer->setOpacity ( 150 );
	m_pPoseColorLayer->setIsVisible ( false );
	m_pPoseSprite	->setIsVisible ( false );

	this->addDrawObject ( m_pPoseColorLayer );
	this->addDrawObject ( m_pPoseSprite	);
	
	l_bIsPose = false;

	m_pLabelTemp = new cLabelAtlasCostom ( "result_num.png", 17, eLayerGame_Result, Point ( _SCR_W_HARP - 20, 295 ) );
	m_pLabelTemp->addCharInfo ( '1',   0, 13, 10 );	m_pLabelTemp->addCharInfo ( '2',  13, 18, 15 );	m_pLabelTemp->addCharInfo ( '3',  31, 17, 14 );
	m_pLabelTemp->addCharInfo ( '4',  48, 18, 15 );	m_pLabelTemp->addCharInfo ( '5',  66, 17, 14 );	m_pLabelTemp->addCharInfo ( '6',  83, 17, 15 );
	m_pLabelTemp->addCharInfo ( '7', 100, 17, 15 );	m_pLabelTemp->addCharInfo ( '8', 117, 18, 15 );	m_pLabelTemp->addCharInfo ( '9', 135, 17, 15 );
	m_pLabelTemp->addCharInfo ( '0', 152, 17, 15 );	m_pLabelTemp->addCharInfo ( '+', 169, 15, 15 );	m_pLabelTemp->addCharInfo ( 'S', 184, 15, 15 );
	m_pLabelTemp->addCharInfo ( 'E', 199, 14, 14 );	m_pLabelTemp->addCharInfo ( 'C', 213, 15, 15 );	m_pLabelTemp->addCharInfo ( '/', 228, 15, 15 );
	m_pLabelTemp->addCharInfo ( 'S', 243, 14, 14 );	m_pLabelTemp->addCharInfo ( 'C', 257, 14, 14 );	m_pLabelTemp->addCharInfo ( 'O', 271, 14, 14 );
	m_pLabelTemp->addCharInfo ( 'R', 285, 14, 14 );	m_pLabelTemp->addCharInfo ( 'E', 299, 17, 17 );	m_pLabelTemp->addCharInfo ( ' ', 316,  5,  5 );
	m_pLabelTemp->setString ( String::createWithFormat ( "%d", m_nClearLeftPoint )->getCString ( ) );
	this->addDrawObject ( m_pLabelTemp );
	
	// background music
	m_bIsBackgoundMusicFast = false;
	
	return true;
}

KDvoid ArcadeScene::update ( KDfloat fDelta )
{
	// super update
	JBaseGameScene::update ( fDelta );
	   
	// time
	KDdouble	dLeftTime = this->getLeftTime ( );
	
	if ( this->stepPose ( dLeftTime ) == true )
	{
		dLeftTime = 0;
	}
	
	// step
	switch ( m_eState )
	{
		case eArcadeTypeState_Start	:	this->stepStart ( dLeftTime );	break;
		case eArcadeTypeState_Game	:	this->stepGame  ( dLeftTime );	break;
		case eArcadeTypeState_House	:	this->stepHouse ( dLeftTime );	break;
		case eArcadeTypeState_Next	:	this->stepNext	( dLeftTime );	break;
		case eArcadeTypeState_End	:	this->stepEnd	( dLeftTime );	break;
	}

	// house
	m_pHouse->update ( dLeftTime );	

	m_pLabelTemp->setString ( ccszf ( "%d", m_nClearLeftPoint ) );
}

KDbool ArcadeScene::stepPose ( KDdouble dLeftTime )
{
	if ( m_eState == eArcadeTypeState_Next || m_eState == eArcadeTypeState_End )
	{
		l_bIsPose = false;
		return false;
	}
			
	// pose
	if ( l_bIsPose == true )
	{
		m_pPoseColorLayer->setIsVisible ( true );
		m_pPoseSprite	 ->setIsVisible ( true );
		return true;
	}

	return false;
}

KDvoid ArcadeScene::stepStart ( KDdouble dLeftTime )
{
	//init
	KDbool	bIsEnd = false;
	
	// start director update
	m_pStartDirector->update ( dLeftTime, bIsEnd );

	// commander
	m_pCommander->update ( dLeftTime );

	// end
	if ( bIsEnd == true )
	{
		m_pCommander->playStart ( );
		m_eState = eArcadeTypeState_Game;
		
		// background music
		if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic == true )
		{
			m_bIsBackgoundMusicFast = false;
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay.mp3" );
		}
	}
}

KDvoid ArcadeScene::stepGame ( KDdouble dLeftTime )
{
	// commander
	m_pCommander->update ( dLeftTime );

	// game time
	m_pGameTime->update ( dLeftTime );
	
	// score
	m_pScore->update ( dLeftTime );

	// cloud
	m_pCloud->update ( dLeftTime );

	// step next check
	if ( m_nClearLeftPoint <= 0 && m_pCommander->getIsTimeStopMode ( ) == false )
	{
		// house level
		++m_nHouseLv;
		
		// clearPoint set
		m_nClearLeftPoint = 50 + ( m_nHouseLv * 5 );
		
		// game time
		if ( 60 - ( m_nHouseLv * 5 ) > 0 )
		{
			m_pGameTime->setAddTime ( 60 - ( m_nHouseLv * 5 ) );
		}
		
		// house
		m_pHouse->view ( m_nHouseLv, true );
		m_pHouse->setGray ( false );
	
		// play stop
		m_pCommander->playStop ( );
		
		// setp
		m_eState = eArcadeTypeState_House;

		// game time
		m_pGameTime->setIsTimeStop ( true );
	}
	
	// game end check
	if ( m_pGameTime->getTime ( ) == 0 )
	{
		// play stop
		m_pCommander->playEnd ( );

		// end commander
		m_pEndCommander->viewExit ( m_pScore->getScore ( ) );

		// state
		m_eState = eArcadeTypeState_End;
	}
	
	// background music
	if ( cGlobalData::getObj()->m_bIsBackgroundMusic == true )
	{
		if ( m_bIsBackgoundMusicFast == false && m_pGameTime->getTime ( ) < 10 )
		{
			KDfloat		fVolume = SimpleAudioEngine::getInstance ( )->getBackgroundMusicVolume ( );
			fVolume -= 1.5f * dLeftTime;
			
			if ( fVolume < 0 )
			{
				m_bIsBackgoundMusicFast = true;
				SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( 1 );
				SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay_x2.mp3" );
			}
			else
			{
				SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( fVolume );
			}
		}
	}
}

KDvoid ArcadeScene::stepHouse ( KDdouble dLeftTime )
{
	// commander
	m_pCommander->update ( dLeftTime / 5 );

	// game time
	m_pGameTime->update ( dLeftTime );
	
	// score
	m_pScore->update ( dLeftTime );

	// cloud
	m_pCloud->update ( dLeftTime );
	
	// house
	m_pHouse->update ( dLeftTime );

	// end check
	if ( m_pHouse->getThrobbingEffect ( ) == false && m_pCloud->getCloudCount ( ) == 0 )
	{
		// end commander
		KDbool		bIsPerfectSuccess	= m_pCommander->getIsPerfectSuccess  ( );
		KDbool		bIsBerserkSuccess	= m_pCommander->getIsBerserkSuccess  ( );
		KDbool		bIsTimeStopSuccess	= m_pCommander->getIsTimeStopSuccess ( );
		KDint		nMaxPowerCount		= m_pCommander->getMaxPowerCount	 ( );
		
		m_pEndCommander->viewNext ( bIsPerfectSuccess, bIsBerserkSuccess, bIsTimeStopSuccess, nMaxPowerCount, m_pScore->getLookScore ( ) );
		
		// next
		m_eState = eArcadeTypeState_Next;
	}
}

KDvoid ArcadeScene::stepNext ( KDdouble dLeftTime )
{
	// commander
	m_pCommander->update ( dLeftTime );

	// game time
	m_pGameTime->update ( dLeftTime );
	
	// score
	m_pScore->update ( dLeftTime );
	
	// commander
	m_pEndCommander->update ( dLeftTime );
}

KDvoid ArcadeScene::stepEnd ( KDdouble dLeftTime )
{
	// commander
	m_pCommander->update ( dLeftTime );
	
	// game time
	m_pGameTime->update ( dLeftTime );
	
	// score
	m_pScore->update ( dLeftTime );
	
	// commander
	m_pEndCommander->update ( dLeftTime );
}

KDvoid ArcadeScene::addPlayer ( KDvoid )
{
	// init
	cCommander::sPlayerInitInfo		tPlayerInfo;
	
	// set
	tPlayerInfo.eCharType	= cChar::eType_nurungee;
	tPlayerInfo.eLookAt		= eLookAtType_left;
	tPlayerInfo.tCharPoint	= Point ( 347, 42 );
	tPlayerInfo.tCharHelper	= Point (  85, 50 );
	
	//firewood
	tPlayerInfo.tFirewoodIntro	 = Point ( -40, 200 );
	tPlayerInfo.tFirewoodPreview = Point (  90,  89 );
	tPlayerInfo.tFirewoodStandBy = Point ( 228,  63 );
	tPlayerInfo.tFirewoodProduct = Point ( 200, 120 );
	
	// add
	m_pCommander->addPlayer ( tPlayerInfo );
}

KDvoid ArcadeScene::msgSprBackGray ( KDbool bIsGray )
{
	if ( bIsGray == true )
	{
		m_pBackImg  ->setGray		( true );
		m_pHouse	->setGray		( true );
		m_pGameTime	->setIceVersion ( true );
	}
	else
	{
		m_pBackImg	->setGray		( false );
		m_pHouse	->setGray		( false );
		m_pGameTime	->setIceVersion ( false );
	}
}

KDvoid ArcadeScene::msgCloudAdd ( KDint z, KDdouble dStartDelay, Point tPoint, Point tAdd, KDdouble dLeftTime, KDfloat fScale, KDfloat fScaleAdd, KDfloat fRotation, KDfloat fRotationAdd, KDbool bIsFlipX )
{
	m_pCloud->add ( z, dStartDelay, tPoint, tAdd, dLeftTime, fScale, fScaleAdd, fRotation, fRotationAdd, bIsFlipX );
}

KDvoid ArcadeScene::msgClearPointAdd ( KDint nPoint )
{
	m_nClearLeftPoint -= nPoint;
}

KDvoid ArcadeScene::msgAddTime ( KDdouble dTime )
{
	m_pGameTime->setAddTime ( dTime );
}

KDvoid ArcadeScene::msgAddScore ( KDint nScore )
{
	m_pScore->addScore ( nScore );
}

KDvoid ArcadeScene::msgPlay ( KDvoid )
{
	// game time
	m_pGameTime->setIsTimeStop ( false );

	// commander
	m_pCommander->playStart ( );

	// state
	m_eState = eArcadeTypeState_Game;

	// background music
	if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic == true )
	{
		if ( m_pGameTime->getTime ( ) < 10 )
		{
			m_bIsBackgoundMusicFast = true;
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay_x2.mp3" );
		}
		else
		{
			m_bIsBackgoundMusicFast = false;
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay.mp3" );
		}

		SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( 1 );
	}
}

KDvoid ArcadeScene::msgRetry ( KDvoid )
{
	// game time
	m_pGameTime->setTime ( l_nStartTime );
	
	// house
	m_nHouseLv = 0;
	m_pHouse->view ( m_nHouseLv, false );
	
	// score
	m_pScore->setScore ( 0 );

	// clear point
	m_nClearLeftPoint = l_nStartClearPoint;

	// commander
	m_pCommander->playInit ( );

	// state
	m_eState = eArcadeTypeState_Start;
	
	// start director
	m_pStartDirector->start ( );

	// background music
	if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic == true )
	{
		if ( m_pGameTime->getTime ( ) < 10 )
		{
			m_bIsBackgoundMusicFast = true;
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay_x2.mp3" );
		}
		else
		{
			m_bIsBackgoundMusicFast = false;
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "gameplay.mp3" );
		}
		
		SimpleAudioEngine::getInstance ( )->setBackgroundMusicVolume ( 1 );
	}
}

KDvoid ArcadeScene::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	// pose
	if ( l_bIsPose == true )
	{
		return;
	}

	JBaseScene::onTouchesBegan ( aTouches, pEvent );

	m_pCommander->touchBegan ( );
}

KDvoid ArcadeScene::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	// pose
	if ( l_bIsPose == true )
	{
		return;
	}

	JBaseScene::onTouchesMoved ( aTouches, pEvent );
}

KDvoid ArcadeScene::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	// pose
	if ( l_bIsPose == true )
	{
		m_pPoseColorLayer->setIsVisible ( false );
		m_pPoseSprite	 ->setIsVisible ( false );

		l_bIsPose = false;
		return;
	}

	JBaseScene::onTouchesEnded ( aTouches, pEvent );
}