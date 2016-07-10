/* -----------------------------------------------------------------------------------
 *
 *      File            MainNode.cpp
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
#include "MainNode.h"
#include "TitleNode/Manual/ManualNode.h"
#include "CasinoNode/CasinoBattleNode/CasinoBattleNode.h"
#include "CasinoNode/CasinoBetNode/CasinoBetNode.h"
#include "CasinoNode/CasinoResultNode/CasinoResultNode.h"
#include "MyRoomNode/MyRoomNode.h"
#include "Object/TimeGetMoney/TimeGetMoney.h"
#include "MyData/MyData.h"

MainNode* MainNode::create ( KDbool bIsNodeChangeIn )
{
	MainNode*	pRet = new MainNode ( );

	if ( pRet && pRet->init ( bIsNodeChangeIn ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

MainNode::MainNode ( KDvoid )
{
	m_pSprSoundOn		= nullptr;
	m_pSprSoundOff		= nullptr;
	m_pSprCredit		= nullptr;
	m_pSprSaveDataAsk	= nullptr;
	m_pNodeChangeImg	= nullptr;
}

MainNode::~MainNode ( KDvoid )
{
	this->removeObject ( );
}

KDvoid MainNode::removeObject ( KDvoid )
{
	// option sound
	CC_SAFE_RELEASE ( m_pSprSoundOn  );
	CC_SAFE_RELEASE ( m_pSprSoundOff );

	// credit
	CC_SAFE_RELEASE ( m_pSprCredit );

	// save data ask
	CC_SAFE_RELEASE ( m_pSprSaveDataAsk );
	CC_SAFE_RELEASE ( m_pNodeChangeImg	);

	SAFE_REMOVE_CHILD ( m_pMenuSaveDataAskYes );
	SAFE_REMOVE_CHILD ( m_pMenuSaveDataAskNo  );

	// btn
	SAFE_REMOVE_CHILD ( m_pMenuCasino );
	SAFE_REMOVE_CHILD ( m_pMenuMyRoom );
	SAFE_REMOVE_CHILD ( m_pMenuRank   );
	SAFE_REMOVE_CHILD ( m_pMenuManual );
	SAFE_REMOVE_CHILD ( m_pMenuCredit );

	// unschedule
	this->unschedule ( schedule_selector ( MainNode::step ) );
}

KDbool MainNode::init ( KDbool bIsNodeChangeIn )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount = 0;

	// back
	auto	pBg = Sprite::create ( "Main/main_back.png" );
	pBg->setPosition ( Point ( m_tContentSize ) / 2 );
	this->addChild ( pBg, LAYER_BACK );

	// option sound
	m_pSprSoundOn  = new cSprite ( this, "Main/main_sound_on.png" , LAYER_MENE, Point::ZERO, Point ( 398, 283 ) );
	m_pSprSoundOff = new cSprite ( this, "Main/main_sound_off.png", LAYER_MENE, Point::ZERO, Point ( 398, 283 ) );

	if ( cMyData::getObj ( )->m_bIsOptionSound == 0 )
	{
		m_pSprSoundOn ->get ( )->setVisible ( false );
		m_pSprSoundOff->get ( )->setVisible ( true  );
	}
	else
	{
		m_pSprSoundOn ->get ( )->setVisible ( true  );
		m_pSprSoundOff->get ( )->setVisible ( false );
	}

	// credit
	m_pSprCredit = new cSprite ( this, "Main/credit.png", LAYER_POPUP, Point::ZERO, Point ( 0, -450 ) );
	m_pSprCredit->get ( )->setOpacity ( 0 );

	// main menu
	m_pMenuCasino = Menu::createWithItem ( MenuItemImage::create ( "Main/main_menu_casino.png"		, "Main/main_menu_casino_push.png"		, CC_CALLBACK_1 ( MainNode::gameCasinoCallback	, this ) ) );
	m_pMenuMyRoom = Menu::createWithItem ( MenuItemImage::create ( "Main/main_menu_myroom.png"		, "Main/main_menu_myroom_push.png"		, CC_CALLBACK_1 ( MainNode::myRoomCallback		, this ) ) );
	m_pMenuRank	  = Menu::createWithItem ( MenuItemImage::create ( "Main/main_menu_leaderBoard.png"	, "Main/main_menu_leaderBoard_push.png" , CC_CALLBACK_1 ( MainNode::rankCallback		, this ) ) );
	m_pMenuManual = Menu::createWithItem ( MenuItemImage::create ( "Main/main_menu_manual.png"		, "Main/main_menu_manual_push.png"		, CC_CALLBACK_1 ( MainNode::manualCallback		, this ) ) );
	m_pMenuCredit = Menu::createWithItem ( MenuItemImage::create ( "Main/main_btn_credit.png"		, "Main/main_btn_credit_push.png"		, CC_CALLBACK_1 ( MainNode::creditCallback		, this ) ) );

	m_pMenuCasino->setPosition ( Point ( 111, 123 ) );
	m_pMenuMyRoom->setPosition ( Point ( 130,  63 ) );
	m_pMenuRank	 ->setPosition ( Point ( 331, 123 ) );
	m_pMenuManual->setPosition ( Point ( 370,  63 ) );
	m_pMenuCredit->setPosition ( Point (  42,  17 ) );

	this->addChild ( m_pMenuCasino, LAYER_MENE );
	this->addChild ( m_pMenuMyRoom, LAYER_MENE );
	this->addChild ( m_pMenuRank  , LAYER_MENE );
	this->addChild ( m_pMenuManual, LAYER_MENE );
	this->addChild ( m_pMenuCredit, LAYER_MENE );

	// save data ask
	m_pSprSaveDataAsk = new cSprite ( this, "Main/saveDataAsk_Back.png", LAYER_POPUP, Point::ZERO, Point ( _SCR_W_HARP - 159, _SCR_H_HARP - 77 ) );
	m_pSprSaveDataAsk->get ( )->setVisible ( false );

	m_pMenuSaveDataAskYes = Menu::createWithItem ( MenuItemImage::create ( "Main/saveDataAsk_btn_yes.png", "Main/saveDataAsk_btn_yes_push.png", CC_CALLBACK_1 ( MainNode::saveDataAskYesCallback, this ) ) );
	m_pMenuSaveDataAskNo  = Menu::createWithItem ( MenuItemImage::create ( "Main/saveDataAsk_btn_no.png" , "Main/saveDataAsk_btn_no_push.png" , CC_CALLBACK_1 ( MainNode::saveDataAskNoCallback , this ) ) );

	m_pMenuSaveDataAskYes->setPosition ( m_pSprSaveDataAsk->get ( )->getPosition ( ) + Point (  80.5f, 37 ) );
	m_pMenuSaveDataAskNo ->setPosition ( m_pSprSaveDataAsk->get ( )->getPosition ( ) + Point ( 229.5f, 37 ) );
	m_pMenuSaveDataAskYes->setVisible ( false );
	m_pMenuSaveDataAskNo ->setVisible ( false );
	m_pMenuSaveDataAskYes->setEnabled ( false );
	m_pMenuSaveDataAskNo ->setEnabled ( false );

	this->addChild ( m_pMenuSaveDataAskYes, LAYER_POPUP );
	this->addChild ( m_pMenuSaveDataAskNo , LAYER_POPUP );

	// credit
	m_bIsCredit			= false;
	m_dCreditStep		= 0;
	m_dCreditStepTime	= 0;

	// switch
	m_bIsGoCasinoBattle	= false;
	m_bIsGoCasinoResult	= false;
	m_bIsGoMyRoomBattle	= false;
	m_bIsGoMyRoomResult	= false;

	// time get money
	cTimeGetMoney::getObj ( )->clear ( );

	// schedule
	this->schedule ( schedule_selector ( MainNode::step ) );

	if ( bIsNodeChangeIn == true )
	{
		this->setNodeChangeIn ( );
	}
	
    auto	pTouchListener = EventListenerTouchAllAtOnce::create ( );
	pTouchListener->onTouchesBegan = CC_CALLBACK_2 ( MainNode::onTouchesBagan, this );
	pTouchListener->onTouchesEnded = CC_CALLBACK_2 ( MainNode::onTouchesEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pTouchListener, this );

	return true;
}

KDvoid MainNode::step ( KDfloat fDelta )
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

	// node chage
	if ( this->updateNodeChage ( dLeftTickCount ) == true )	
	{
		return;
	}

	// credit
	if ( m_bIsCredit != false )
	{
		if ( m_dCreditStep == 0 )
		{
			KDint	nOpacity = m_pSprCredit->get ( )->getOpacity ( );
			nOpacity += 10;
			if ( nOpacity > 255 )
			{
				nOpacity = 255;
			}
			m_pSprCredit->get ( )->setOpacity ( nOpacity );

			m_dCreditStepTime -= dLeftTickCount;

			if ( m_dCreditStepTime <= 0 )
			{
				m_pSprCredit->get ( )->setOpacity ( 255 );

				m_dCreditStepTime = 0;

				++m_dCreditStep;
			}
		}
		else if ( m_dCreditStep == 1 )
		{
			Point	tCreditPoint = m_pSprCredit->get ( )->getPosition ( );

			tCreditPoint.y += dLeftTickCount * 30;
			
			if ( tCreditPoint.y >= -130 )
			{
				tCreditPoint.y = -130;
				m_dCreditStepTime = 3.5f;

				++m_dCreditStep;
			}

			m_pSprCredit->get ( )->setPosition ( tCreditPoint );
		}
		else
		{
			m_dCreditStepTime -= dLeftTickCount;

			if ( m_dCreditStepTime <= 0 )
			{
				m_bIsCredit = false;

				m_pMenuCasino->setEnabled ( true ); 
				m_pMenuMyRoom->setEnabled ( true ); 
				m_pMenuRank  ->setEnabled ( true ); 
				m_pMenuCredit->setEnabled ( true ); 
			}
		}
	}
	else
	{
		KDint	nOpacity = m_pSprCredit->get ( )->getOpacity ( );
		nOpacity -= 10;
		if ( nOpacity < 0 )
		{
			nOpacity = 0;
		}
		m_pSprCredit->get ( )->setOpacity ( nOpacity );
	}	
}

KDbool MainNode::updateNodeChage ( KDdouble dLeftTIme )
{
	if ( m_pNodeChangeImg != nullptr )
	{
		if ( m_pNodeChangeImg->update ( dLeftTIme ) == true )
		{
			if ( m_bIsNodeChangeIn == true )
			{
				m_pMenuCasino->setEnabled ( true ); 
				m_pMenuMyRoom->setEnabled ( true ); 
				m_pMenuRank  ->setEnabled ( true ); 
				m_pMenuCredit->setEnabled ( true ); 

				m_bIsNodeChangeIn = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );
			}
			else if ( m_bIsNodeChangeOut == true )
			{
				m_bIsNodeChangeOut = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );

				// cenece change
					 if ( m_bIsGoCasinoBattle == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithFileLoad ( eIntoType_Casino ) ) );	
				else if ( m_bIsGoCasinoResult == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoResultNode::createWithFileLoad ( eIntoType_Casino ) ) );
				else if ( m_bIsGoMyRoomBattle == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithFileLoad ( eIntoType_MyRoom ) ) );
				else if ( m_bIsGoMyRoomResult == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoResultNode::createWithFileLoad ( eIntoType_MyRoom ) ) );
				else if ( m_bIsGoMyRoom	      == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, MyRoomNode		::create ( -1 ) ) );
				else									Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBetNode	::create ( ) ) );	
				
				return true;
			}
		}
	}

	return false;
}

KDvoid MainNode::setNodeChangeIn ( KDvoid )
{
	m_pMenuCasino->setEnabled ( false );
	m_pMenuMyRoom->setEnabled ( false );
	m_pMenuRank	 ->setEnabled ( false );
	m_pMenuCredit->setEnabled ( false );

	m_bIsNodeChangeIn = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg = new cNodeChangeImgs ( this, false );
}

KDvoid MainNode::onTouchesBagan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( auto pTouch : aTouches )
	{
		Point		tTouch = pTouch->getLocation ( );
		Rect		tOptionCheck = Rect ( 396, 280, 80, 32 );

		if ( RectInPoint ( tOptionCheck, tTouch ) == true )
		{
			if ( cMyData::getObj ( )->m_bIsOptionSound == 0 )
			{
				cMyData::getObj ( )->m_bIsOptionSound = true;
			}
			else
			{
				cMyData::getObj ( )->m_bIsOptionSound = false;
			}

			if ( cMyData::getObj ( )->m_bIsOptionSound == 1 )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "Sound/button_select.mp3" );
				SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "Sound/bm_mainpage.mp3", true );

				m_pSprSoundOn ->get ( )->setVisible ( true );
				m_pSprSoundOff->get ( )->setVisible ( false );
			}
			else
			{
				SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );				

				m_pSprSoundOn ->get ( )->setVisible ( false );
				m_pSprSoundOff->get ( )->setVisible ( true );
			}

			cMyData::getObj ( )->save ( );
		}
	}
}

KDvoid MainNode::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	if ( m_bIsCredit != false )
	{
		m_bIsCredit = false;

		m_pMenuCasino->setEnabled ( true );
		m_pMenuMyRoom->setEnabled ( true );
		m_pMenuRank	 ->setEnabled ( true );
		m_pMenuCredit->setEnabled ( true );
	}
}

KDvoid MainNode::gameCasinoCallback	( Object* pSender )
{
	m_pMenuCasino->setEnabled ( false );
	m_pMenuMyRoom->setEnabled ( false );
	m_pMenuRank	 ->setEnabled ( false );
	m_pMenuCredit->setEnabled ( false );

	// 저장된 데이타가 있는지 체크한다.
	for ( KDint i = 0; i < 4; i++ )
	{
		// init
		const KDchar*	pPath = nullptr;

		switch ( i )
		{
			case 0	:	pPath = _FILE_NAME_CASINO_BATTLE_SAVE;	break;
			case 1	:	pPath = _FILE_NAME_CASINO_RESULT_SAVE;	break;
			case 2	:	pPath = _FILE_NAME_MYROOM_BATTLE_SAVE;	break;
			case 3	:	pPath = _FILE_NAME_MYROOM_RESULT_SAVE;	break;
		}

		KDFile*		pFile = kdFopen ( pPath, "rb" );
		if ( pFile != nullptr )
		{
			// version
			KDint			nVersionStrCount = 0;
			kdFread ( &nVersionStrCount, sizeof ( KDint ), 1, pFile );

			KDchar*			pVersionStr = new KDchar [ nVersionStrCount + 1 ];
			kdFread ( pVersionStr, sizeof ( KDchar ), nVersionStrCount + 1, pFile );
			std::string		sVersionStr = pVersionStr;
			delete pVersionStr;

			kdFclose ( pFile );

			// 최신 버젼이 아니라면 삭제해준다.
			if ( sVersionStr.compare ( _GAME_SAVE_FILE_SIZE_LATEST_VERSION_STR ) == 0 )
			{
				switch ( i )
				{
					case 0	:	m_bIsGoCasinoBattle = true;		break;
					case 1	:	m_bIsGoCasinoResult = true;		break;
					case 2	:	m_bIsGoMyRoomBattle = true;		break;
					case 3	:	m_bIsGoMyRoomResult = true;		break;
				}

				m_pSprSaveDataAsk->get ( )	->setVisible ( true );
				m_pMenuSaveDataAskYes		->setVisible ( true );
				m_pMenuSaveDataAskNo		->setVisible ( true );
				m_pMenuSaveDataAskYes		->setEnabled ( true );
				m_pMenuSaveDataAskNo		->setEnabled ( true );

				break;
			}
			else
			{
				kdUnlink ( pPath );
			}
		}
	}

	if ( m_bIsGoCasinoBattle == false && m_bIsGoCasinoResult == false && m_bIsGoMyRoomBattle == false && m_bIsGoMyRoomResult == false )
	{
		m_bIsNodeChangeOut = true;
		CC_SAFE_RELEASE ( m_pNodeChangeImg );
		m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

		// stop background music
		SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
	}
}

KDvoid MainNode::myRoomCallback ( Object* pSender )
{
	m_pMenuCasino->setEnabled ( false );
	m_pMenuMyRoom->setEnabled ( false );
	m_pMenuRank	 ->setEnabled ( false );
	m_pMenuCredit->setEnabled ( false );

	// 저장된 데이타가 있는지 체크한다.
	for ( KDint i = 0; i < 4; i++ )
	{
		// init
		const KDchar*	pPath = nullptr;

		switch(i)
		{
			case 0	:	pPath = _FILE_NAME_CASINO_BATTLE_SAVE;	break;
			case 1	:	pPath = _FILE_NAME_CASINO_RESULT_SAVE;	break;
			case 2	:	pPath = _FILE_NAME_MYROOM_BATTLE_SAVE;	break;
			case 3	:	pPath = _FILE_NAME_MYROOM_RESULT_SAVE;	break;
		}

		KDFile*		pFile = kdFopen ( pPath, "rb" );
		if ( pFile != nullptr )
		{
			// version
			KDint			nVersionStrCount = 0;
			kdFread ( &nVersionStrCount, sizeof ( KDint ), 1, pFile );

			KDchar*			pVersionStr = new KDchar [ nVersionStrCount + 1 ];
			kdFread ( pVersionStr, sizeof ( KDchar ), nVersionStrCount + 1, pFile );
			std::string		sVersionStr = pVersionStr;
			delete pVersionStr;

			kdFclose ( pFile );

			// 최신 버젼이 아니라면 삭제해준다.
			if ( sVersionStr.compare ( _GAME_SAVE_FILE_SIZE_LATEST_VERSION_STR ) == 0 )
			{
				switch ( i )
				{
					case 0	:	m_bIsGoCasinoBattle = true;		break;
					case 1	:	m_bIsGoCasinoResult = true;		break;
					case 2	:	m_bIsGoMyRoomBattle = true;		break;
					case 3	:	m_bIsGoMyRoomResult = true;		break;
				}

				m_pSprSaveDataAsk->get ( )	->setVisible ( true );
				m_pMenuSaveDataAskYes		->setVisible ( true );
				m_pMenuSaveDataAskNo		->setVisible ( true );
				m_pMenuSaveDataAskYes		->setEnabled ( true );
				m_pMenuSaveDataAskNo		->setEnabled ( true );

				break;
			}
			else
			{
				kdUnlink ( pPath );
			}
		}
	}

	if ( m_bIsGoCasinoBattle == false && m_bIsGoCasinoResult == false && m_bIsGoMyRoomBattle == false && m_bIsGoMyRoomResult == false )
	{
		m_bIsNodeChangeOut = true;
		CC_SAFE_RELEASE ( m_pNodeChangeImg );
		m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

		// stop background music
		SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
	}

	m_bIsGoMyRoom = true;
}

KDvoid MainNode::rankCallback ( Object* pSender )
{
#if 0
	[ OpenFeint launchDashboard ];
#endif	// 0
}

KDvoid MainNode::manualCallback ( Object* pSender )
{
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, ManualNode::create ( ) ) );
}

KDvoid MainNode::creditCallback ( Object* pSender )
{
	m_bIsCredit = true;
	m_dCreditStep = 0;
	m_dCreditStepTime = 2;

	m_pSprCredit->get ( )->setVisible ( true );
	m_pSprCredit->get ( )->setPosition ( Point ( 0, -450 ) );
	m_pSprCredit->get ( )->setOpacity ( 0 );

	m_pMenuCasino->setEnabled ( false );
	m_pMenuMyRoom->setEnabled ( false );
	m_pMenuRank	 ->setEnabled ( false );
	m_pMenuCredit->setEnabled ( false );
}

KDvoid MainNode::saveDataAskYesCallback ( Object* pSender )
{
	m_pMenuSaveDataAskYes->setEnabled ( false );
	m_pMenuSaveDataAskNo ->setEnabled ( false );

	m_bIsNodeChangeOut = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );
}

KDvoid MainNode::saveDataAskNoCallback ( Object* pSender )
{
	// 파일 삭제.

	kdUnlink ( _FILE_NAME_CASINO_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_CASINO_RESULT_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_RESULT_SAVE );

	m_bIsGoCasinoBattle	= false;
	m_bIsGoCasinoResult	= false;
	m_bIsGoMyRoomBattle	= false;
	m_bIsGoMyRoomResult	= false;

	m_pMenuSaveDataAskYes->setEnabled ( false );
	m_pMenuSaveDataAskNo ->setEnabled ( false );

	m_bIsNodeChangeOut = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );
}
