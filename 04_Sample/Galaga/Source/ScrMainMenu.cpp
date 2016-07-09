/* --------------------------------------------------------------------------
 *
 *      File            ScrMainMenu.cpp
 *      Description     Main Menu Screen
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "ScrMainMenu.h"
#include "ScrMainGame.h"
#include "GameState.h"

KDbool CScrMainMenu::init ( KDvoid )
{
	// 레이어를 초기화 한다.
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	// 현재 레이어 사이즈를 가져온다.
	CCSize	tLyrSize = this->getContentSize ( );
	CCSize	tSize = ccsz;

	// 디버그용 배경
//	this->addChild ( CCLayerColor::create ( ccc4 ( 255, 0, 0, 128 ), tLyrSize ) );

    // 우상단 종료 버튼
    CCMenuItemImage*  pClose = CCMenuItemImage::create 
	(
		"Images/CloseNormal.png",
		"Images/CloseSelected.png", 
		this, menu_selector ( CScrMainMenu::onExit ) 
	);
	this->addChild ( CCMenu::createWithItem ( pClose ), 1 );
	tSize = pClose->getContentSize ( );
	pClose->setPosition ( this, kCCAlignmentTopRight, ccp ( 20, 20 ) );
//	pClose->setPosition ( ccp ( tLyrSize.cx - tSize.cx / 2 - 20, tLyrSize.cy - tSize.cx / 2 - 20 ) );    

    // 타이틀 레이블 생성
    CCLabelTTF*	 pTitle = CCLabelTTF::create ( GSTAT->getText ( eTxtTitle ), "Font/NanumGothicBold.ttf", 40 );
	this->addChild ( pTitle );
    pTitle->setColor ( ccYELLOW );
	pTitle->setPositionWithParent ( kCCAlignmentTop, ccp ( 0, 50 ) );
    
    // 설명 Label 생성
    CCLabelTTF*  pDesc = CCLabelTTF::create( "Press the airplane !!!!", "Font/NanumGothicBold.ttf", 25 );
	this->addChild( pDesc );
	pDesc->setPosition ( ccpMid ( tLyrSize ) );
  
    // 설명 Label이 계속해서 깜빡거리도록 설정
    pDesc->runAction ( CCRepeatForever::create ( CCBlink::create ( 1.0f, 1 ) ) );

    // 비행기 묶음 텍스쳐 생성
	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/galagasheet.png" );

	// 하단에 비행기 생성
    CCSprite*	  pAirPlane = CCSprite::createWithTexture ( pTexture, ccr ( 184, 55, 15, 17 ) );
	this->addChild ( pAirPlane );
    pAirPlane->setScale ( 2 );
	pAirPlane->setPositionWithParent ( kCCAlignmentBottom, ccp ( 0, 50 ) );
	m_pAirPlane = pAirPlane;

	// 레이어 터치 모드 켜기
	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CScrMainMenu::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = m_pAirPlane->convertTouchToNodeSpace ( pTouch );
	CCRect		tAirRect = m_pAirPlane->boundingBox ( );

	tAirRect.origin.x = 0; //-tAirRect.size.cx * 2;
	tAirRect.origin.y = 0; //-tAirRect.size.cy * 2;

//	tAirRect.size.cx *= 2;
//	tAirRect.size.cy *= 2;

	if ( tAirRect.containsPoint ( tPoint ) )
	{
		CCLOG ( "Airplane OK" );

		// 게임 화면으로 전환
		GSTAT->setScene ( eScrMainGame, eTranSlideIn );
	}

	// 하단은 터치 대응 코드 예제
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
	{
		pTouch = (CCTouch*) ( *it );
	}
}

KDvoid CScrMainMenu::onExit ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->end ( );
}
