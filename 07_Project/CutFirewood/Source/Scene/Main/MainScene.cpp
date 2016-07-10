/* -----------------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
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
#include "MainScene.h"
#include "Scene/Arcade/ArcadeScene.h"

KDbool MainScene::init ( KDvoid )
{
	if ( !JBaseScene::init ( ) )
	{
		return false;
	}

	// sprite
	m_pSprBack		= new cSprite ( "main_back.png"		, 0, Point (	0,  0 ) );
	m_pSprNurungee	= new cSprite ( "main_nurungee.png"	, 0, Point ( -210,  0 ) );
	m_pSprCredis	= new cSprite ( "credit.png"		, 3, Point (   48, 38 ) );
	
	m_pSprBack->setIsStatic ( true );	
	m_pSprCredis->setIsVisible ( false );

	this->addDrawObject ( m_pSprBack );
	this->addDrawObject ( m_pSprNurungee );
	this->addDrawObject ( m_pSprCredis );

	// layer
	Color4B		tColor;
	tColor.r = 0;
	tColor.g = 0;
	tColor.b = 0;
	tColor.a = 255;
	m_pCredisBackLayer = new cColorLayer ( tColor, 2, Size ( _SCR_W, _SCR_H ), Point ( 0, 0 ) );
	m_pCredisBackLayer->setIsVisible ( false );
	m_pCredisBackLayer->setOpacity ( 150 );
	this->addDrawObject ( m_pCredisBackLayer );

	// button
	m_pBtnArcadeMode		 = new cButton ( "main_btn_arcademode.png"		, "main_btn_arcademode_push.png"	, CC_CALLBACK_1 ( MainScene::onBtnArcadeMode		, this ), 1, 0, Point ( _SCR_W, 157 ) );
	m_pBtnMoreFowcomGames	 = new cButton ( "main_moreFowcomGames.png"		, "main_moreFowcomGames_push.png"	, CC_CALLBACK_1 ( MainScene::onBtnMoreFowcomGames	, this ), 1, 0, Point ( _SCR_W, 103 ) );
	m_pBtnCredis			 = new cButton ( "main_btn_credis.png"			, "main_btn_credis_push.png"		, CC_CALLBACK_1 ( MainScene::onBtnCredis			, this ), 1, 0, Point ( _SCR_W,  63 ) );
	m_pBtnBackgroundMusicOn	 = new cButton ( "main_background_music_on.png"	, "main_background_music_on.png"	, CC_CALLBACK_1 ( MainScene::onBtnBackgroundMusicOn	, this ), 1, 0, Point (	   205,	  4 ) );
	m_pBtnBackgroundMusicOff = new cButton ( "main_background_music_off.png", "main_background_music_off.png"	, CC_CALLBACK_1 ( MainScene::onBtnBackgroundMusicOff, this ), 1, 0, Point (	   205,	  4 ) );	
	m_pBtnEffectMusicOn		 = new cButton ( "main_effect_music_on.png"		, "main_effect_music_on.png"		, CC_CALLBACK_1 ( MainScene::onBtnEffectMusicOn		, this ), 1, 0, Point (	   265,	  6 ) );
	m_pBtnEffectMusicOff	 = new cButton ( "main_effect_music_off.png"	, "main_effect_music_off.png"		, CC_CALLBACK_1 ( MainScene::onBtnEffectMusicOff	, this ), 1, 0, Point (	   265,	  6 ) );

	m_pBtnArcadeMode		->setIsTouchEnable ( false );
	m_pBtnMoreFowcomGames	->setIsTouchEnable ( false );
	m_pBtnCredis			->setIsTouchEnable ( false );
	m_pBtnBackgroundMusicOn	->setIsTouchEnable ( false );
	m_pBtnBackgroundMusicOff->setIsTouchEnable ( false );
	m_pBtnEffectMusicOn		->setIsTouchEnable ( false );
	m_pBtnEffectMusicOff	->setIsTouchEnable ( false );

	if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic )	m_pBtnBackgroundMusicOff->setIsVisible ( false );
	else													m_pBtnBackgroundMusicOn	->setIsVisible ( false );
	if ( cGlobalData::getObj ( )->m_bIsEffectMusic )		m_pBtnEffectMusicOff	->setIsVisible ( false );
	else													m_pBtnEffectMusicOn		->setIsVisible ( false );
	
	this->addBtnObject ( m_pBtnArcadeMode			);
	this->addBtnObject ( m_pBtnMoreFowcomGames		);
	this->addBtnObject ( m_pBtnCredis				);
	this->addBtnObject ( m_pBtnBackgroundMusicOn	);
	this->addBtnObject ( m_pBtnBackgroundMusicOff	);
	this->addBtnObject ( m_pBtnEffectMusicOn		);
	this->addBtnObject ( m_pBtnEffectMusicOff		);

	// switch
	m_bIsDirection	= true;

	// throw step
	m_dThrowCheckTime			= 0;
	m_bIsThrowNurungee			= false;
	m_bIsThrowArcadeMode		= false;
	m_bIsThrowMoreFowcomGames	= false;
	m_bIsThrowCredis			= false;
	
	// background music
	if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic )
	{
		SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "mainpage.mp3" ); 
	}

	return true;
}

KDvoid MainScene::update ( KDfloat fDelta )
{
	// super update
	JBaseScene::update ( fDelta );
	
	// left time
	KDdouble	dLeftTime = this->getLeftTime ( );

	// update
	this->updateThrowDrawObject ( dLeftTime );
	this->updateMoveObject ( dLeftTime );
	
	// btn touch enabled switch change
	if ( m_bIsDirection == true && m_aMoveObjects.size ( ) == 0 )
	{
		m_pBtnArcadeMode		->setIsTouchEnable ( true );
		m_pBtnMoreFowcomGames	->setIsTouchEnable ( true );
		m_pBtnCredis			->setIsTouchEnable ( true );

		if ( cGlobalData::getObj ( )->m_bIsBackgroundMusic )	m_pBtnBackgroundMusicOn	->setIsTouchEnable ( true );
		else													m_pBtnBackgroundMusicOff->setIsTouchEnable ( true );
		if ( cGlobalData::getObj ( )->m_bIsEffectMusic )		m_pBtnEffectMusicOn		->setIsTouchEnable ( true );
		else													m_pBtnEffectMusicOff	->setIsTouchEnable ( true );
		
		m_bIsDirection = false;
	}
}

KDvoid MainScene::updateThrowDrawObject ( KDdouble dLeftTime )
{
	// pass time
	m_dThrowCheckTime += dLeftTime;
	
	// nuringee
	if ( m_bIsThrowNurungee == false && m_dThrowCheckTime > 1.0f )
	{
		SMainSceneMoveObject	tMoveObject;

		tMoveObject.pDrawObject	 = m_pSprNurungee;
		tMoveObject.nStep		 = 0;
		tMoveObject.dPassTime	 = 0.0f;
		tMoveObject.dArrivedTime = 0.15f;
		tMoveObject.tStartPoint	 = Point ( -210, 0 );
		tMoveObject.tEndPoint	 = Point (    0, 0 );
		
		m_aMoveObjects.push_back ( tMoveObject );
		
		m_bIsThrowNurungee = true;
	}
	
	// btn arcade mode
	if ( m_bIsThrowArcadeMode == false && m_dThrowCheckTime > 1.5f )
	{
		SMainSceneMoveObject	tMoveObject;

		tMoveObject.pDrawObject	 = m_pBtnArcadeMode;
		tMoveObject.nStep		 = 0;
		tMoveObject.dPassTime	 = 0.0f;
		tMoveObject.dArrivedTime = 0.1f;
		tMoveObject.tStartPoint	 = Point ( _SCR_W, 157 );
		tMoveObject.tEndPoint	 = Point (	  237, 157 );
		
		m_aMoveObjects.push_back ( tMoveObject );

		m_bIsThrowArcadeMode = true;
	}

	// btn more fowcom games
	if ( m_bIsThrowMoreFowcomGames == false && m_dThrowCheckTime > 1.6f )
	{
		SMainSceneMoveObject	tMoveObject;

		tMoveObject.pDrawObject	 = m_pBtnMoreFowcomGames;
		tMoveObject.nStep		 = 0;
		tMoveObject.dPassTime	 = 0.0f;
		tMoveObject.dArrivedTime = 0.1f;
		tMoveObject.tStartPoint	 = Point ( _SCR_W, 103 );
		tMoveObject.tEndPoint	 = Point (	  224, 103 );
		
		m_aMoveObjects.push_back ( tMoveObject );

		m_bIsThrowMoreFowcomGames = true;
	}
	
	// btn credis
	if ( m_bIsThrowCredis == false && m_dThrowCheckTime > 1.7f )
	{
		SMainSceneMoveObject	tMoveObject;

		tMoveObject.pDrawObject	 = m_pBtnCredis;
		tMoveObject.nStep		 = 0;
		tMoveObject.dPassTime	 = 0.0f;
		tMoveObject.dArrivedTime = 0.1f;
		tMoveObject.tStartPoint	 = Point ( _SCR_W, 63 );
		tMoveObject.tEndPoint	 = Point (	  395, 63 );
		
		m_aMoveObjects.push_back ( tMoveObject );

		m_bIsThrowCredis = true;
	}
}

KDvoid MainScene::updateMoveObject ( KDdouble dLeftTime )
{
	// loop
	std::list<SMainSceneMoveObject>::iterator	pNode = m_aMoveObjects.begin ( );
	while ( pNode != m_aMoveObjects.end ( ) )
	{
		// init
		SMainSceneMoveObject&	tMoveObject = *pNode;
		KDbool					bIsRelease  = false;

		// step
		if ( tMoveObject.nStep == 0 )
		{
			// pass time
			tMoveObject.dPassTime += dLeftTime;
			
			// check time
			if ( tMoveObject.dPassTime > tMoveObject.dArrivedTime )
			{
				// pt
				tMoveObject.pDrawObject->setPoint ( tMoveObject.tEndPoint );

				// release switch
				bIsRelease = true;
			}
			else
			{
				// percent
				KDfloat		fPercent = tMoveObject.dPassTime / tMoveObject.dArrivedTime;
				
				// pt
				KDfloat		x = tMoveObject.tStartPoint.x + ( tMoveObject.tEndPoint.x - tMoveObject.tStartPoint.x ) * fPercent;
				KDfloat		y = tMoveObject.tStartPoint.y + ( tMoveObject.tEndPoint.y - tMoveObject.tStartPoint.y ) * fPercent;

				tMoveObject.pDrawObject->setPoint ( Point ( x, y ) );
			}
		}
		// release or next
		if ( bIsRelease == false )
		{
			++pNode;
		}
		else
		{
			pNode = m_aMoveObjects.erase ( pNode );
		}
	}
}

KDvoid MainScene::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	// super
	JBaseScene::onTouchesBegan ( aTouches, pEvent );

	// credis
	if ( m_pCredisBackLayer->getIsVisible ( ) == true )
	{
		m_pBtnArcadeMode		->setIsTouchEnable ( true );
		m_pBtnMoreFowcomGames	->setIsTouchEnable ( true );
		m_pBtnCredis			->setIsTouchEnable ( true );
		m_pBtnBackgroundMusicOn	->setIsTouchEnable ( true );
		m_pBtnBackgroundMusicOff->setIsTouchEnable ( true );
		m_pBtnEffectMusicOn		->setIsTouchEnable ( true );
		m_pBtnEffectMusicOff	->setIsTouchEnable ( true );	

		m_pSprCredis			->setIsVisible ( false );
		m_pCredisBackLayer		->setIsVisible ( false );
	}
}

KDvoid MainScene::onBtnArcadeMode ( Object* pSender )
{
	// switch
	m_pBtnArcadeMode		->setIsTouchEnable ( false );
	m_pBtnMoreFowcomGames	->setIsTouchEnable ( false );
	m_pBtnCredis			->setIsTouchEnable ( false );

	// background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
		
	// go race scene
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, ArcadeScene::create ( ) ) );
}

KDvoid MainScene::onBtnMoreFowcomGames ( Object* pSender )
{

}

KDvoid MainScene::onBtnCredis ( Object* pSender )
{
	m_pBtnArcadeMode		->setIsTouchEnable ( false );
	m_pBtnMoreFowcomGames	->setIsTouchEnable ( false );
	m_pBtnCredis			->setIsTouchEnable ( false );
	m_pBtnBackgroundMusicOn	->setIsTouchEnable ( false );
	m_pBtnBackgroundMusicOff->setIsTouchEnable ( false );
	m_pBtnEffectMusicOn		->setIsTouchEnable ( false );
	m_pBtnEffectMusicOff	->setIsTouchEnable ( false );
	
	m_pSprCredis		->setIsVisible ( true );
	m_pCredisBackLayer	->setIsVisible ( true );
}

KDvoid MainScene::onBtnBackgroundMusicOn ( Object* pSender )
{
	cGlobalData::getObj ( )->m_bIsBackgroundMusic = false;
	
	m_pBtnBackgroundMusicOn	->setIsVisible	   ( false );
	m_pBtnBackgroundMusicOn	->setIsTouchEnable ( false );
	m_pBtnBackgroundMusicOff->setIsVisible	   ( true );
	m_pBtnBackgroundMusicOff->setIsTouchEnable ( true );

	cGlobalData::getObj ( )->saveMusic ( );

	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MainScene::onBtnBackgroundMusicOff ( Object* pSender )
{
	cGlobalData::getObj ( )->m_bIsBackgroundMusic = true;
	
	m_pBtnBackgroundMusicOn	->setIsVisible	   ( true );
	m_pBtnBackgroundMusicOn	->setIsTouchEnable ( true );
	m_pBtnBackgroundMusicOff->setIsVisible	   ( false );
	m_pBtnBackgroundMusicOff->setIsTouchEnable ( false );

	cGlobalData::getObj ( )->saveMusic ( );

	SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "mainpage.mp3" );
}

KDvoid MainScene::onBtnEffectMusicOn ( Object* pSender )
{
	cGlobalData::getObj ( )->m_bIsEffectMusic = false;
	
	m_pBtnEffectMusicOn	->setIsVisible	   ( false );
	m_pBtnEffectMusicOn	->setIsTouchEnable ( false );
	m_pBtnEffectMusicOff->setIsVisible	   ( true );
	m_pBtnEffectMusicOff->setIsTouchEnable ( true );

	cGlobalData::getObj ( )->saveMusic ( );
}

KDvoid MainScene::onBtnEffectMusicOff ( Object* pSender )
{
	cGlobalData::getObj ( )->m_bIsEffectMusic = true;
	
	m_pBtnEffectMusicOn	->setIsVisible	   ( true );
	m_pBtnEffectMusicOn	->setIsTouchEnable ( true );
	m_pBtnEffectMusicOff->setIsVisible	   ( false );
	m_pBtnEffectMusicOff->setIsTouchEnable ( false );

	cGlobalData::getObj ( )->saveMusic ( );
}
