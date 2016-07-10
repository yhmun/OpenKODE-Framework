/* --------------------------------------------------------------------------
 *
 *      File            MyRanking.cpp
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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
#include "MyRanking.h"
#include "MenuLayer.h"
#include "XMJson/json.h"

KDbool MyRanking::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	// 배경로고
	CCSprite*	pBackground = CCSprite::create ( "Images/score_bg.jpg" );
	CCSprite*	pLogo		= CCSprite::create ( "Images/score_logo.png" );

	// 폭죽 파티클
    CCParticleSystemQuad*	pEmitter = new CCParticleSystemQuad ( );
	pEmitter->initWithFile ( "Particles/LavaFlow.plist" );
	pEmitter->autorelease ( );

	this->addChild ( pBackground );
	this->addChild ( pLogo );
	this->addChild ( pEmitter );

	pBackground->setPositionWithParent ( kCCAlignmentCenter );
	pLogo->setPositionWithParent ( kCCAlignmentTop );
	pEmitter->setPositionWithParent ( kCCAlignmentTop, ccp ( 0, 100 ) );

	// 메뉴
	CCMenu*				pBack = CCMenu::create ( );
	CCMenuItemImage*	pBackItem = CCMenuItemImage::create 
	( 
		"Images/back.png", 
		"Images/back.png",
		this,
		menu_selector ( MyRanking::onButtonClick )	
	);
	pBack->addChild ( pBackItem );
	pBack->setPosition ( ccp ( 30, tLyrSize.cy - 30 ) );
	this->addChild ( pBack );

	// 로딩이미지
	m_pLoading = CCProgressTimer::create ( "Images/loding.png" );
	m_pLoading->setType ( kCCProgressTimerTypeRadial );
	m_pLoading->setPosition ( ccpMid ( tLyrSize ) );
	m_pLoading->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );
	this->addChild ( m_pLoading, 1000 );

	this->scheduleOnce ( schedule_selector ( MyRanking::selectData ) , 3 );

	return KD_TRUE;	
}

// 뒤로가기 버튼
KDvoid MyRanking::onButtonClick ( CCObject*	pSender )
{
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer = MenuLayer::create ( );

	pScene->addChild ( pLayer );
	CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

// 점수 수신완료
KDvoid MyRanking::onHttpRequestCompleted ( CCNode* pSender, KDvoid* pData )
{
	m_pLoading->setVisible ( KD_FALSE );
	
	CCHttpResponse*		pResponse = (CCHttpResponse*) pData;
	if ( !pResponse )
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if ( 0 != kdStrlen ( pResponse->getHttpRequest ( )->getTag ( ) ) ) 
    {
        CCLOG ( "%s completed", pResponse->getHttpRequest ( )->getTag ( ) );
    }

	std::vector<KDchar>*	 pBuffer = pResponse->getResponseData ( );
	std::string				sOutput  = "";
    for ( KDuint i = 0; i < pBuffer->size ( ); i++ )
    {
		sOutput.push_back ( (*pBuffer) [ i ] );
    }
	
	if ( strcmp ( pResponse->getHttpRequest ( )->getTag ( ), "select" ) != -1 )
	{
		decoding ( sOutput.c_str ( ) );	
	}
}

// 점수 요청
KDvoid MyRanking::selectData ( KDfloat fDelta )
{
	m_pLoading->setVisible ( KD_TRUE );

	CCHttpClient*	pHttpClient = CCHttpClient::getInstance ( );
	CCHttpRequest*  pRequest = KD_NULL;
	
	pRequest = new CCHttpRequest ( );
    pRequest->setUrl ( "http://server1.ausoft.co.kr/uptris/list.php" );
    pRequest->setRequestType ( CCHttpRequest::kCCHttpGet );
    pRequest->setResponseCallback ( this, callfuncND_selector ( MyRanking::onHttpRequestCompleted ) );
    pRequest->setTag ( "select" );
    pHttpClient->send ( pRequest );
    pRequest->release ( );

}

// 점수 형태변환
KDvoid	MyRanking::decoding ( std::string sData )
{
	CCLOG ( sData.c_str ( ) );

	const CCSize&	tLyrSize = this->getContentSize ( );

	KDbool			bRet;
	Json::Value		tRoot;
	Json::Reader	tReader;
	std::string		sOutput;
	
	bRet = tReader.parse ( sData, tRoot );
	if ( bRet )
	{
		const Json::Value  tPlugins = tRoot [ "list" ];

		for ( KDuint  i = 0; i < tPlugins.size ( ); ++i )
		{
			sOutput += ccszf ( "%dst %-12s %5s", i + 1, tPlugins [ i ].get ( "name", 0 ).asString ( ).c_str ( ), tPlugins [ i ].get ( "score", 0 ).asString ( ).c_str ( ) );
			sOutput += "\n";
		}
	}

	CCLabelBMFont*  pBmFont = CCLabelBMFont::create ( sOutput.c_str ( ), "Fonts/helvetica-32.fnt" );
	pBmFont->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 60 ) );
	this->addChild ( pBmFont );
}
