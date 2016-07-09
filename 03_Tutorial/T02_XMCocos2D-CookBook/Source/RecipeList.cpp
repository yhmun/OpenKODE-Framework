/* --------------------------------------------------------------------------
 *
 *      File            RecipeList.cpp
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "RecipeList.h"
#include "Includes.h"

RecipeList*		g_pRecipeList = KD_NULL;

KDbool RecipeList::init ( KDvoid )
{
	g_pRecipeList = this;
	this->retain ( );

	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	CCLayerGradient*  pBG = CCLayerGradient::create ( ccc4 ( 100, 100, 100, 100 ), ccc4 ( 100, 100, 100, 255 ) );	
	pBG->setContentSize ( tLyrSize );
	this->addChild ( pBG, -10, TAG_BG );

	CCSprite*  pBottomLeft = CCSprite::create ( "jungle-left.png" );
	this->addChild ( pBottomLeft, 10 );
	pBottomLeft->setOpacity ( 210 );
	pBottomLeft->setPositionWithParent ( kCCAlignmentBottomLeft );

	CCSprite*  pBottomRight = CCSprite::create ( "jungle-right.png" );
	this->addChild ( pBottomRight, 10 );
	pBottomRight->setOpacity ( 210 );
	pBottomRight->setPositionWithParent ( kCCAlignmentBottomRight );

	CCTableView*  pTableView = CCTableView::create ( this, ccs ( tLyrSize.cx, tLyrSize.cy - 20.f ) );
	this->addChild ( pTableView );
	pTableView->setDelegate   ( this );	
	pTableView->setDataSource ( this );
	pTableView->setPosition   ( ccp ( 0, 10 ) );

	m_pTableView = pTableView;
	
	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont::setFontName ( "ThonburiBold.ttf" );

	return KD_TRUE;
}

KDuint RecipeList::numberOfCellsInTableView ( CCTableView* pTable )
{
	return sizeof ( l_aRecipeNames ) / sizeof ( l_aRecipeNames [ 0 ] );
}

CCSize RecipeList::cellSizeForTable ( CCTableView* pTable )
{
	return CCSize ( this->getContentSize ( ).cx, 25.f );
}

CCTableViewCell* RecipeList::tableCellAtIndex ( CCTableView* pTable, KDuint uIndex )
{
	CCTableViewCell*  pCell = pTable->dequeueCell ( );
	CCMenuItemFont*   pItem = KD_NULL;

	if ( !pCell )
	{
		pCell = CCTableViewCell::create ( );
		
		pItem = CCMenuItemFont::create ( "Recipe Name" );	
		pItem->setPosition ( ccpMid ( cellSizeForTable ( pTable ) ) );
		pCell->addChild ( pItem, 0, 1000 );		
	}
	else
	{
		pItem = (CCMenuItemFont*) pCell->getChildByTag ( 1000 );
	}

	pItem->setString ( l_aRecipeNames [ uIndex ] );
	pItem->setScale ( 1 );

	return pCell;
}

KDvoid RecipeList::tableCellTouched ( CCTableView* pTable, CCTableViewCell* pCell )
{
	this->showRecipe ( pCell->getIdx ( ) );
}

KDvoid RecipeList::tableCellHighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1000 ) )->selected ( );
}

KDvoid RecipeList::tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1000 ) )->unselected ( );
}

KDvoid RecipeList::scrollViewDidScroll ( CCScrollView* pView )
{

}

KDvoid RecipeList::scrollViewDidZoom ( CCScrollView* pView )
{

}

KDvoid RecipeList::showRecipe ( KDuint uIndex )
{
	CCDirector*		pDirector = CCDirector::sharedDirector ( );
	CCScene*		pScene    = KD_NULL;

	m_nSelected = uIndex;

	switch ( uIndex )
	{
		case  0	:	pScene = Ch1_DrawingSprites				::scene ( );	break;
		case  1 :	pScene = Ch1_ColoringSprites			::scene ( );	break;
		case  2	:	pScene = Ch1_AnimatingSprites			::scene ( );	break;
		case  3	:	pScene = Ch1_OpenGLPrimitives			::scene ( );	break;
		case  4	:	pScene = Ch1_PlayingVideoFiles			::scene ( );	break;
		case  5 :	pScene = Ch1_GridEffects				::scene ( );	break;
		case  6 :	pScene = Ch1_ParticleEffects			::scene ( );	break;
		case  7 :	pScene = Ch1_MotionStreak				::scene ( );	break;			
		case  8 :	pScene = Ch1_RetinaDisplay				::scene ( );	break;
		case  9 :	pScene = Ch1_1DEasingActions			::scene ( );	break;
		case 10 :	pScene = Ch1_2DEasingActions			::scene ( );	break;
		case 11 :	pScene = Ch1_3DCubes					::scene ( );	break;
		case 12 :	pScene = Ch1_RenderTexturedPolygon		::scene ( );	break;
		case 13 :	pScene = Ch1_AnimateTexturedPolygon		::scene ( );	break;
		case 14 :	pScene = Ch1_PaletteSwapping			::scene ( );	break;
		case 15 :	pScene = Ch1_MutablePaletteSwapping		::scene ( );	break;
		case 16 :	pScene = Ch1_UsingAWTextureFilter		::scene ( );	break;
		case 17 :	pScene = Ch1_RetroPixelatedFilter		::scene ( );	break;
		case 18 :	pScene = Ch1_TakingScreenshots			::scene ( );	break;
		case 19 :	pScene = Ch1_UsingCCParallaxNode		::scene ( );	break;
		case 20 :	pScene = Ch1_PixelFormat				::scene ( );	break;
		case 21 :	pScene = Ch1_ColorMaskLighting			::scene ( );	break;
		case 22 :	pScene = Ch2_TapHoldDragInput			::scene ( );	break;
		case 23 :	pScene = Ch2_DepthTestingInput			::scene ( );	break;
		case 24 :	pScene = Ch2_Buttons					::scene ( );	break;
		case 25 :	pScene = Ch2_DPad						::scene ( );	break;
		case 26 :	pScene = Ch2_AnalogStick				::scene ( );	break;
		case 27 :	pScene = Ch2_AccelerometerSteering		::scene ( );	break;
		case 28 :	pScene = Ch2_AccelerometerRotation		::scene ( );	break;
		case 29 :	pScene = Ch2_PinchZooming				::scene ( );	break;
		case 30 :	pScene = Ch2_Gestures					::scene ( );	break;
		case 31 :	pScene = Ch3_ArchivingObjects			::scene ( );	break;
		case 32 :	pScene = Ch3_ReadingPlistFiles			::scene ( );	break;
		case 33 :	pScene = Ch3_ReadingJsonFiles			::scene ( );	break;
		case 34 :	pScene = Ch3_ReadingXmlFiles			::scene ( );	break;
		case 35 :	pScene = Ch3_SavingSimpleData			::scene ( );	break;
		case 36 :	pScene = Ch3_MutatingNestedMetadata		::scene ( );	break;
		case 37 :	pScene = Ch3_SavingDataPlist			::scene ( );	break;
		case 38 :	pScene = Ch3_SavingDataSQLite			::scene ( );	break;
		case 39 :	pScene = Ch3_SavingDataCoreData			::scene ( );	break;
		case 40 :	pScene = Ch4_BasicSetup					::scene ( );	break;
		case 41 :	pScene = Ch4_CollisionResponse			::scene ( );	break;
		case 42 :	pScene = Ch4_DifferentShapes			::scene ( );	break;
		case 43 :	pScene = Ch4_DraggingAndFiltering		::scene ( );	break;
		case 44 :	pScene = Ch4_PhysicalProperties			::scene ( );	break;
		case 45 :	pScene = Ch4_Impulses					::scene ( );	break;
		case 46 :	pScene = Ch4_Forces						::scene ( );	break;
		case 47 :	pScene = Ch4_AsyncBodyDestruction		::scene ( );	break;
		case 48 :	pScene = Ch4_Joints						::scene ( );	break;
		case 49 :	pScene = Ch4_Vehicles					::scene ( );	break;
		case 50 :	pScene = Ch4_CharacterMovement			::scene ( );	break;
		case 51 :	pScene = Ch4_Bullets					::scene ( );	break;
		case 52 :	pScene = Ch4_Rope						::scene ( );	break;
		case 53 :	pScene = Ch4_TopDownIsometric			::scene ( );	break;
		case 54 :	pScene = Ch5_SwitchingScenes			::scene ( );	break;
		case 55 :	pScene = Ch5_SceneTransitions			::scene ( );	break;
		case 56 :	pScene = Ch5_UsingCCLayerMultiplex		::scene ( );	break;
		case 57 :	pScene = Ch5_UsingCCLabel				::scene ( );	break;
		case 58 :	pScene = Ch5_UsingCCMenu				::scene ( );	break;
		case 59 :	pScene = Ch5_ShadowedLabels				::scene ( );	break;
		case 60 :	pScene = Ch5_UIKitAlerts				::scene ( );	break;
		case 61 :	pScene = Ch5_WrappingUIKit				::scene ( );	break;
		case 62 :	pScene = Ch5_MenuWindows				::scene ( );	break;
		case 63 :	pScene = Ch5_HorizScrollMenu			::scene ( );	break;
		case 64 :	pScene = Ch5_VertSlidingMenuGrid		::scene ( );	break;
		case 65 :	pScene = Ch5_LoadingScreen				::scene ( );	break;
		case 66 :	pScene = Ch5_Minimap					::scene ( );	break;
		case 67 :	pScene = Ch6_SoundsAndMusic				::scene ( );	break;
		case 68 :	pScene = Ch6_AudioProperties			::scene ( );	break;
		case 69 :	pScene = Ch6_FadingSoundsAndMusic		::scene ( );	break;
		case 70 :	pScene = Ch6_AudioInGame				::scene ( );	break;
		case 71 :	pScene = Ch6_PositionalAudio			::scene ( );	break;
		case 72 :	pScene = Ch6_MeteringMusic				::scene ( );	break;
		case 73 :	pScene = Ch6_MeteringDialogue			::scene ( );	break;
		case 74 :	pScene = Ch6_StreamingAudio				::scene ( );	break;
		case 75 :	pScene = Ch6_RecordingAudio				::scene ( );	break;
		case 76 :	pScene = Ch6_iPodLibrary				::scene ( );	break;
		case 77 :	pScene = Ch6_MIDISynthesization			::scene ( );	break;
		case 78 :	pScene = Ch7_Waypoints					::scene ( );	break;
		case 79 :	pScene = Ch7_ProjectileAiming			::scene ( );	break;
		case 80 :	pScene = Ch7_LineOfSight				::scene ( );	break;
		case 81 :	pScene = Ch7_AIFlocking					::scene ( );	break;
		case 82 :	pScene = Ch7_GridPathfinding			::scene ( );	break;
		case 83 :	pScene = Ch7_Box2DPathfinding			::scene ( );	break;
		case 84 :	pScene = Ch7_TileMapPathfinding			::scene ( );	break;
		case 85 :	pScene = Ch7_SideScrollingPathfinding	::scene ( );	break;
		case 86 :	pScene = Ch7_LuaScripting				::scene ( );	break;
		case 87 :	pScene = Ch7_DynamicScriptLoading		::scene ( );	break;
		case 88 :	pScene = Ch7_LuaDecisionTree			::scene ( );	break;
		case 89 :	pScene = Ch8_Cocos2dTestbed				::scene ( );	break;
		case 90 :	pScene = Ch8_Tiled						::scene ( );	break;
		case 91 :	pScene = Ch8_JSONWorldBuilder			::scene ( );	break;
		case 92 :	pScene = Ch8_CocosBuilder				::scene ( );	break;
		case 93 :	pScene = Ch8_Cocos2d_X					::scene ( );	break;
		case 94 :	pScene = Ch8_Cocos3d					::scene ( );	break;
	}

	pDirector->replaceScene ( pScene );
}

KDvoid RecipeList::showRecipeList ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	pScene->addChild ( this );
	
	CCPoint		tPos = m_pTableView->getContentOffset ( );
	m_pTableView->reloadData ( );
	m_pTableView->setContentOffset ( tPos );

	CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

const KDchar* RecipeList::getRecipeName ( KDvoid )
{
	return l_aRecipeNames [ m_nSelected ];
}

KDvoid RecipeList::prevCallback ( CCObject* pSender )
{
	m_nSelected--;
	if ( m_nSelected < 0 )
	{
		m_nSelected = this->numberOfCellsInTableView ( KD_NULL ) - 1;
	}

	this->showRecipe ( m_nSelected );
}

KDvoid RecipeList::nextCallback ( CCObject* pSender )
{
	m_nSelected++;
	if ( m_nSelected >= (KDint) this->numberOfCellsInTableView ( KD_NULL ) )
	{
		m_nSelected = 0;
	}

	this->showRecipe ( m_nSelected );
}