/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingSimpleData.cpp
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
#include "Ch3_SavingSimpleData.h"

KDbool Ch3_SavingSimpleData::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Set font size
	CCMenuItemFont::setFontSize ( 30 );

	// Add main label
	CCLabelBMFont*	pChooseDifficultyLabel = CCLabelBMFont::create ( "CHOOSE DIFFICULTY:", "eurostile_30.fnt" );
	pChooseDifficultyLabel->setPosition ( ccp ( 240, 250 ) );
	pChooseDifficultyLabel->setScale ( 0.5f );
	this->addChild ( pChooseDifficultyLabel, 1 );
	
	// Add difficulty choices
	m_pEasyMIF	 = CCMenuItemFont::create ( "Easy"  , this, menu_selector ( Ch3_SavingSimpleData::chooseEasy ) );
	m_pMediumMIF = CCMenuItemFont::create ( "Medium", this, menu_selector ( Ch3_SavingSimpleData::chooseMedium ) );
	m_pHardMIF	 = CCMenuItemFont::create ( "Hard"  , this, menu_selector ( Ch3_SavingSimpleData::chooseHard ) );
	m_pInsaneMIF = CCMenuItemFont::create ( "Insane", this, menu_selector ( Ch3_SavingSimpleData::chooseInsane ) );

	CCMenu*		pMainMenu = CCMenu::create ( m_pEasyMIF, m_pMediumMIF, m_pHardMIF, m_pInsaneMIF, KD_NULL );
	pMainMenu->alignItemsVertically ( );
	pMainMenu->setPosition ( ccp ( 240, 140 ) );
	this->addChild ( pMainMenu, 1 );	
	
	// Load any previously chosen difficulty
	this->loadDifficulty ( );

	return KD_TRUE;
}

KDvoid Ch3_SavingSimpleData::resetMenuColors ( KDvoid )
{
	// Menu color management
	m_pEasyMIF	->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pMediumMIF->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pHardMIF	->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pInsaneMIF->setColor ( ccc3 ( 255, 255, 255 ) );
}

KDvoid Ch3_SavingSimpleData::setDifficultyFromValue ( KDvoid )
{
	// More menu color management
	this->resetMenuColors ( );
	
	if ( !m_sDifficulty.compare ( "Easy" ) )
	{
		m_pEasyMIF->setColor ( ccc3 ( 255, 0, 0 ) );
	}
	else if ( !m_sDifficulty.compare ( "Medium" ) ) 
	{
		m_pMediumMIF->setColor ( ccc3 ( 255, 0, 0 ) );
	}
	else if ( !m_sDifficulty.compare ( "Hard" ) ) 
	{
		m_pHardMIF->setColor ( ccc3 ( 255, 0, 0 ) );
	}
	else if ( !m_sDifficulty.compare ( "Insane" ) ) 
	{
		m_pInsaneMIF->setColor ( ccc3 ( 255, 0, 0 ) );
	}
	
	this->saveDifficulty ( );
}

KDvoid Ch3_SavingSimpleData::loadDifficulty ( KDvoid )
{
	// If a difficulty is set we use that, otherwise we choose Medium
	CCUserDefault*		pDefaults = CCUserDefault::sharedUserDefault ( ); 

	m_sDifficulty = pDefaults->getStringForKey ( "simple_data_difficulty", "Medium" );

	this->setDifficultyFromValue ( );
}

KDvoid Ch3_SavingSimpleData::saveDifficulty ( KDvoid )
{
	// Save our difficulty
	CCUserDefault*		pDefaults = CCUserDefault::sharedUserDefault ( );

	pDefaults->setStringForKey ( "simple_data_difficulty", m_sDifficulty.c_str ( ) );
	pDefaults->flush ( );
}

/// Shortcut callback methods 

KDvoid Ch3_SavingSimpleData::chooseEasy ( CCObject* pSender )
{
	m_sDifficulty = "Easy";

	this->setDifficultyFromValue ( );
}

KDvoid Ch3_SavingSimpleData::chooseMedium ( CCObject* pSender )
{
	m_sDifficulty = "Medium";

	this->setDifficultyFromValue ( );
}

KDvoid Ch3_SavingSimpleData::chooseHard ( CCObject* pSender )
{
	m_sDifficulty = "Hard";

	this->setDifficultyFromValue ( );
}

KDvoid Ch3_SavingSimpleData::chooseInsane ( CCObject* pSender )
{
	m_sDifficulty = "Insane";

	this->setDifficultyFromValue ( );
}