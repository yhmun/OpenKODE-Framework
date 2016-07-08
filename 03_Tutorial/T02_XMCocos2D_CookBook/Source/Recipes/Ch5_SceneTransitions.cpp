/* --------------------------------------------------------------------------
 *
 *      File            Ch5_SceneTransitions.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch5_SceneTransitions.h"

static const KDchar*	l_szTransitionTypes [ ] =
{
	"CCTransitionJumpZoom"			,
	"CCTransitionCrossFade"			,
	"CCTransitionProgressRadialCCW"	,
	"CCTransitionProgressRadialCW"	,
	"TransitionPageForward"			,
	"TransitionPageBackward"		,
	"CCTransitionFadeTR"			,
	"CCTransitionFadeBL"			,
	"CCTransitionFadeUp"			,
	"CCTransitionFadeDown"			,
	"CCTransitionTurnOffTiles"		,
	"CCTransitionSplitRows"			,
	"CCTransitionSplitCols"			,
	"CCTransitionFade"				,
	"FadeWhiteTransition"			,
	"FlipXLeftOver"					,
	"FlipXRightOver"				,
	"FlipYUpOver"					,
	"FlipYDownOver"					,
	"FlipAngularLeftOver"			,
	"FlipAngularRightOver"			,
	"ZoomFlipXLeftOver"				,
	"ZoomFlipXRightOver"			,
	"ZoomFlipYUpOver"				,
	"ZoomFlipYDownOver"				,
	"ZoomFlipAngularLeftOver"		,
	"ZoomFlipAngularRightOver"		,
	"CCTransitionShrinkGrow"		,
	"CCTransitionRotoZoom"			,
	"CCTransitionMoveInL"			,
	"CCTransitionMoveInR"			,
	"CCTransitionMoveInT"			,
	"CCTransitionMoveInB"			,
	"CCTransitionSlideInL"			,
	"CCTransitionSlideInR"			,
	"CCTransitionSlideInT"			,
	"CCTransitionSlideInB"			,
};

static KDint	l_nNumTransitionTypes = 37;

KDbool Ch5_SceneTransitions::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Font size/name
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	// Go to our initial scene
	CCMenuItemFont*		pViewTransitions = CCMenuItemFont::create ( "View Transitions", this, menu_selector ( Ch5_SceneTransitions::viewTransitions ) );
	CCMenu*				pMenu = CCMenu::create ( pViewTransitions, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	pMenu->alignItemsVertically ( );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid Ch5_SceneTransitions::viewTransitions ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( TransSceneMenu::sceneWithString ( "", 0 ) );
}


CCScene* TransSceneMenu::sceneWithString ( const KDchar* szStr, KDint nCurrentTransition )
{
	TransSceneMenu*		pLayer = new TransSceneMenu ( );

	if ( pLayer && pLayer->initWithString ( szStr, nCurrentTransition ) )
	{
		CCScene*	pScene = CCScene::create ( );
		pScene->addChild ( pLayer );
		pLayer->autorelease ( );

		return pScene;
	}

	CC_SAFE_DELETE ( pLayer );

	return KD_NULL;
}

KDbool TransSceneMenu::initWithString ( const KDchar* szStr, KDint nCurrentTransition )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Set scene name
	m_sName = szStr;
	m_nCurrentTransition = nCurrentTransition;

	// Random background color
	CCSprite*			pBG = CCSprite::create ( "blank.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBG->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBG );

	CCLabelBMFont*		pMessage = CCLabelBMFont::create ( szStr, "eurostile_30.fnt" );
	pMessage->setPosition ( ccp ( 160, 270 ) );
	pMessage->setScale ( 0.5f );
	pMessage->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( pMessage, 10 );

	CCMenuItemFont*		pPrevItem   = CCMenuItemFont::create ( "Prev"	 , this, menu_selector ( TransSceneMenu::prevScene	 ) );
	CCMenuItemFont*		pQuitItem   = CCMenuItemFont::create ( "Quit"	 , this, menu_selector ( TransSceneMenu::quit		 ) );
	CCMenuItemFont*		pRandomItem = CCMenuItemFont::create ( "Random"	 , this, menu_selector ( TransSceneMenu::randomScene ) );
	CCMenuItemFont*		pNextItem   = CCMenuItemFont::create ( "Next"	 , this, menu_selector ( TransSceneMenu::nextScene	 ) );

	// Add menu items
	CCMenu*				pMenu = CCMenu::create ( pPrevItem, pRandomItem, pQuitItem, pNextItem, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	pMenu->alignItemsHorizontally ( 20 );
	this->addChild ( pMenu );
		
	return KD_TRUE;
}

KDvoid TransSceneMenu::prevScene ( CCObject* pSender )
{
	m_nCurrentTransition--;
	if ( m_nCurrentTransition < 0 )
	{
		m_nCurrentTransition = l_nNumTransitionTypes - 1;
	}
	this->loadNewScene ( );
}

KDvoid TransSceneMenu::nextScene ( CCObject* pSender )
{
	m_nCurrentTransition++;
	if ( m_nCurrentTransition >= l_nNumTransitionTypes )
	{
		m_nCurrentTransition = 0;
	}
	this->loadNewScene ( );
}

KDvoid TransSceneMenu::randomScene ( CCObject* pSender )
{
	m_nCurrentTransition = kdRand ( ) % l_nNumTransitionTypes;
	this->loadNewScene ( );
}

KDvoid TransSceneMenu::quit ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popScene (  );
}

KDvoid TransSceneMenu::loadNewScene ( KDvoid )
{
	CCDirector::sharedDirector ( )->popScene (  );

	CCScene*	pScene = TransSceneMenu::sceneWithString ( l_szTransitionTypes [ m_nCurrentTransition ], m_nCurrentTransition );

	switch ( m_nCurrentTransition )
	{
		case  0 :	pScene = CCTransitionJumpZoom			::create ( 1.2f, pScene );										break;		// CCTransitionJumpZoom		
		case  1 :	pScene = CCTransitionCrossFade			::create ( 1.2f, pScene );										break;		// CCTransitionCrossFade		
		case  2 :	pScene = CCTransitionProgressRadialCCW	::create ( 1.2f, pScene );										break;		// CCTransitionProgressRadialCCW		
		case  3 :	pScene = CCTransitionProgressRadialCW	::create ( 1.2f, pScene );										break;		// CCTransitionProgressRadialCW		
		case  4 :	pScene = CCTransitionPageTurn			::create ( 1.2f, pScene, KD_FALSE );							break;		// TransitionPageForward		
		case  5 :	pScene = CCTransitionPageTurn			::create ( 1.2f, pScene, KD_TRUE  );							break;		// TransitionPageBackward		
		case  6 :	pScene = CCTransitionFadeTR				::create ( 1.2f, pScene );										break;		// CCTransitionFadeTR			
		case  7 :	pScene = CCTransitionFadeBL				::create ( 1.2f, pScene );										break;		// CCTransitionFadeBL			
		case  8 :	pScene = CCTransitionFadeUp				::create ( 1.2f, pScene );										break;		// CCTransitionFadeUp			
		case  9 :	pScene = CCTransitionFadeDown			::create ( 1.2f, pScene );										break;		// CCTransitionFadeDown		
		case 10 :	pScene = CCTransitionTurnOffTiles		::create ( 1.2f, pScene );										break;		// CCTransitionTurnOffTiles	
		case 11 :	pScene = CCTransitionSplitRows			::create ( 1.2f, pScene );										break;		// CCTransitionSplitRows		
		case 12 :	pScene = CCTransitionSplitCols			::create ( 1.2f, pScene );										break;		// CCTransitionSplitCols		
		case 13 :	pScene = CCTransitionFade				::create ( 1.2f, pScene );										break;		// CCTransitionFade			
		case 14 :	pScene = CCTransitionFade				::create ( 1.2f, pScene, ccWHITE );								break;		// FadeWhiteTransition		
		case 15 :	pScene = CCTransitionFlipX				::create ( 1.2f, pScene, kCCTransitionOrientationLeftOver  );	break;		// FlipXLeftOver				
		case 16 :	pScene = CCTransitionFlipX				::create ( 1.2f, pScene, kCCTransitionOrientationRightOver );	break;		// FlipXRightOver				
		case 17 :	pScene = CCTransitionFlipY				::create ( 1.2f, pScene, kCCTransitionOrientationUpOver    );	break;		// FlipYUpOver				
		case 18 :	pScene = CCTransitionFlipY				::create ( 1.2f, pScene, kCCTransitionOrientationDownOver  );	break;		// FlipYDownOver				
		case 19 :	pScene = CCTransitionFlipAngular		::create ( 1.2f, pScene, kCCTransitionOrientationLeftOver  );	break;		// FlipAngularLeftOver		
		case 20 :	pScene = CCTransitionFlipAngular		::create ( 1.2f, pScene, kCCTransitionOrientationRightOver );	break;		// FlipAngularRightOver		
		case 21 :	pScene = CCTransitionZoomFlipX			::create ( 1.2f, pScene, kCCTransitionOrientationLeftOver  );	break;		// ZoomFlipXLeftOver			
		case 22 :	pScene = CCTransitionZoomFlipX			::create ( 1.2f, pScene, kCCTransitionOrientationRightOver );	break;		// ZoomFlipXRightOver			
		case 23 :	pScene = CCTransitionZoomFlipY			::create ( 1.2f, pScene, kCCTransitionOrientationUpOver    );	break;		// ZoomFlipYUpOver			
		case 24 :	pScene = CCTransitionZoomFlipY			::create ( 1.2f, pScene, kCCTransitionOrientationDownOver  );	break;		// ZoomFlipYDownOver			
		case 25 :	pScene = CCTransitionZoomFlipAngular	::create ( 1.2f, pScene, kCCTransitionOrientationLeftOver  );	break;		// ZoomFlipAngularLeftOver	
		case 26 :	pScene = CCTransitionZoomFlipAngular	::create ( 1.2f, pScene, kCCTransitionOrientationRightOver );	break;		// ZoomFlipAngularRightOver	
		case 27 :	pScene = CCTransitionShrinkGrow			::create ( 1.2f, pScene );										break;		// CCTransitionShrinkGrow		
		case 28 :	pScene = CCTransitionRotoZoom			::create ( 1.2f, pScene );										break;		// CCTransitionRotoZoom		
		case 29 :	pScene = CCTransitionMoveInL			::create ( 1.2f, pScene );										break;		// CCTransitionMoveInL		
		case 30 :	pScene = CCTransitionMoveInR			::create ( 1.2f, pScene );										break;		// CCTransitionMoveInR		
		case 31 :	pScene = CCTransitionMoveInT			::create ( 1.2f, pScene );										break;		// CCTransitionMoveInT		
		case 32 :	pScene = CCTransitionMoveInB			::create ( 1.2f, pScene );										break;		// CCTransitionMoveInB		
		case 33 :	pScene = CCTransitionSlideInL			::create ( 1.2f, pScene );										break;		// CCTransitionSlideInL		
		case 34 :	pScene = CCTransitionSlideInR			::create ( 1.2f, pScene );										break;		// CCTransitionSlideInR		
		case 35 :	pScene = CCTransitionSlideInT			::create ( 1.2f, pScene );										break;		// CCTransitionSlideInT		
		case 36 :	pScene = CCTransitionSlideInB			::create ( 1.2f, pScene );										break;		// CCTransitionSlideInB		
	}

	CCDirector::sharedDirector ( )->pushScene ( pScene );
}