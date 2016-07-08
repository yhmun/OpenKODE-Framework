/* --------------------------------------------------------------------------
 *
 *      File            Recipe.cpp
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
#include "Recipe.h"
#include "RecipeList.h"

KDbool Recipe::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCLayerGradient*  pBG = CCLayerGradient::create ( ccc4 ( 100, 100, 100, 100 ), ccc4 ( 100, 100, 100, 255 ) );	
	pBG->setContentSize ( tLyrSize );
	this->addChild ( pBG, Z_BG, TAG_BG );

	CCSprite*  pBottomLeft = CCSprite::create ( "jungle-left.png" );
	this->addChild ( pBottomLeft, Z_HUD, TAG_GRASS_LEFT );
	pBottomLeft->setOpacity ( 210 );
	pBottomLeft->setScale ( 0.7f );
	pBottomLeft->setPositionWithParent ( kCCAlignmentBottomLeft );

	CCSprite*  pBottomRight = CCSprite::create ( "jungle-right.png" );
	this->addChild ( pBottomRight, Z_HUD, TAG_GRASS_RIGHT );
	pBottomRight->setOpacity ( 210 );
	pBottomRight->setScale ( 0.7f );
	pBottomRight->setPositionWithParent ( kCCAlignmentBottomRight );

	CCLayerColor*  pTitleBG = CCLayerColor::create ( ccc4 ( 100, 100, 100, 100 ), ccs ( tLyrSize.cx, 30 ) );
	this->addChild ( pTitleBG, Z_HUD, TAG_RECIPE_BG );
	pTitleBG->setPositionWithParent ( kCCAlignmentTop );
	m_pTitleBG = pTitleBG;

	CCLabelTTF*  pTitle = CCLabelTTF::create ( g_pRecipeList->getRecipeName ( ), "ThonburiBold.ttf", 18 );
	pTitleBG->addChild ( pTitle, Z_HUD, TAG_RECIPE_NAME );
	pTitle->setPositionWithParent ( kCCAlignmentLeft, ccp ( 60, 0 ) );
	pTitle->setColor ( ccGREEN );

	CCMenuItemImage*  pBack = CCMenuItemImage::create 
	(
		"btn-back-0.png", "btn-back-1.png", g_pRecipeList, menu_selector ( RecipeList::showRecipeList ) 
	);
	pBack->setPosition ( pTitleBG, kCCAlignmentLeft, ccp ( 10, 0 ) );
	pBack->setScale ( 0.8f );
	pTitleBG->addChild ( CCMenu::createWithItem ( pBack ), Z_HUD, TAG_RECIPE_BACK );

	m_pMessage = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pMessage->setPosition ( ccp ( 30, 283 ) );
	m_pMessage->setScale ( 0.5f );
	m_pMessage->setAnchorPoint ( ccp ( 0, 1 ) );
	this->addChild ( m_pMessage, Z_HUD, TAG_RECIPE_NAME );

	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 30 );

	CCMenuItemFont*  pPrev = CCMenuItemFont::create 
	(
		"Prev", g_pRecipeList, menu_selector ( RecipeList::prevCallback ) 
	);
	pPrev->setPosition ( this, kCCAlignmentBottomLeft, ccp ( 30, 10 ) );
	this->addChild ( CCMenu::createWithItem ( pPrev ), Z_HUD, TAG_PREV_BUTTON );

	CCMenuItemFont*  pNext = CCMenuItemFont::create 
	(
		"Next", g_pRecipeList, menu_selector ( RecipeList::nextCallback ) 
	);
	pNext->setPosition ( this, kCCAlignmentBottomRight, ccp ( 30, 10 ) );
	this->addChild ( CCMenu::createWithItem ( pNext ), Z_HUD, TAG_NEXT_BUTTON );

	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont::setFontName ( "ThonburiBold.ttf" );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid Recipe::drawLayer ( KDvoid )
{

}

KDvoid Recipe::initSubMenus ( KDint nSubCount )
{
	CCMenuItemFont::setFontSize ( 16 );

	CCMenuItemFont*		pPrev   = CCMenuItemFont::create ( "<PREV"   , this, menu_selector ( Recipe::prevSubCallback ) );
	CCMenuItemFont*		pReload = CCMenuItemFont::create ( "<RELOAD>", this, menu_selector ( Recipe::reloadSubCallback ) );
	CCMenuItemFont*		pNext   = CCMenuItemFont::create ( "NEXT>   ", this, menu_selector ( Recipe::nextSubCallback ) );

	CCMenu*				pMenu = CCMenu::create ( pPrev, pReload, pNext, KD_NULL );
	pMenu->alignItemsHorizontally ( );
    pMenu->setPosition ( ccp ( 240, 20 ) );
    this->addChild ( pMenu, 10, TAG_RECIPE_SUB );	

	m_pSubName = CCLabelTTF::create ( "", "ThonburiBold.ttf", 16 );
	m_pTitleBG->addChild ( m_pSubName, 0 );	

	m_nSubIndex = 0;
	m_nSubCount = nSubCount;

	this->reloadSubCallback ( KD_NULL );
}

const KDchar* Recipe::runSubAction ( KDint nIndex )
{
	return "";
}

KDvoid Recipe::prevSubCallback ( CCObject* pSender )
{
	m_nSubIndex--;
	if ( m_nSubIndex < 0 )
	{
		m_nSubIndex = m_nSubCount - 1;
	}

	m_pSubName->setString ( this->runSubAction ( m_nSubIndex ) );
	m_pSubName->setPositionWithParent ( kCCAlignmentRight, ccp ( 15, 0 ) );
}

KDvoid Recipe::reloadSubCallback ( CCObject* pSender )
{
	m_pSubName->setString ( this->runSubAction ( m_nSubIndex ) );
	m_pSubName->setPositionWithParent ( kCCAlignmentRight, ccp ( 15, 0 ) );
}

KDvoid Recipe::nextSubCallback ( CCObject* pSender )
{
	m_nSubIndex++;
	if ( m_nSubIndex >= m_nSubCount )
	{
		m_nSubIndex = 0;
	}

	m_pSubName->setString ( this->runSubAction ( m_nSubIndex ) );
	m_pSubName->setPositionWithParent ( kCCAlignmentRight, ccp ( 15, 0 ) );
}

KDvoid Recipe::resetMessage ( KDvoid )
{
	m_pMessage->setString ( "" );
}

KDvoid Recipe::showMessage ( const KDchar* szMsg )
{
	m_pMessage->setString ( szMsg );

	this->runAction 
	(
		CCSequence::create 
		(
			CCDelayTime::create ( 5.0f ), 
			CCCallFunc ::create ( this, callfunc_selector ( Recipe::resetMessage ) ),
			KD_NULL 
		)
	);
}

KDvoid Recipe::appendMessage ( const KDchar* szMsg )
{
	this->stopAllActions ( );

	m_pMessage->setString ( ccszf ( "%s\n%s", m_pMessage->getString ( ), szMsg ) );
}
