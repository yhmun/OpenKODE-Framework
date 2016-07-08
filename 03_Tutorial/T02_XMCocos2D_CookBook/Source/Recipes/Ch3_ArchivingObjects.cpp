/* --------------------------------------------------------------------------
 *
 *      File            Ch3_ArchivingObjects.cpp
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
#include "Ch3_ArchivingObjects.h"
#include "Libraries/SimpleCharacter.h"

KDbool Ch3_ArchivingObjects::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pHud		 = KD_NULL;
	m_pCharacter = KD_NULL;

	// Add our message
	m_pMessage->setString ( "" );

	// Set up menu buttons
	CCMenuItemFont*		pRandomCharacterMIF = CCMenuItemFont::create ( "<RANDOM>", this, menu_selector ( Ch3_ArchivingObjects::randomCharacter ) );
	CCMenuItemFont*		pDeleteDataMIF = CCMenuItemFont::create ( "<DELETE DATA>", this, menu_selector ( Ch3_ArchivingObjects::deleteData ) );

	CCMenu*				pMenu = CCMenu::create ( pRandomCharacterMIF, pDeleteDataMIF, KD_NULL );
	pMenu->alignItemsHorizontally ( );
	pMenu->setPosition ( ccp ( 240, 50 ) );
	this->addChild ( pMenu, 1 );

	// Finally, load our character
	this->loadCharacter ( );

	return KD_TRUE;
}

KDvoid Ch3_ArchivingObjects::loadCharacter ( KDvoid )
{
	/*
	//Our archive file name
	NSString *fileName = @"dnd_character.archive";

	//We get our file path
	CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];
*/
	CC_SAFE_RELEASE ( m_pCharacter );

	if ( 1 )// ![[NSFileManager defaultManager] fileExistsAtPath:filePath] )
	{	
		//If file doesn't exist in document directory create a new default character and save it
		
		m_pCharacter = SimpleCharacter::create ( );
		
//		[NSKeyedArchiver archiveRootObject:character toFile:filePath];
	}
	else
	{
		// If it does we load it
		//m_pCharacter = [[NSKeyedUnarchiver unarchiveObjectWithFile:filePath] retain];
	}

	m_pCharacter->retain ( );
	
	// Add character and reload HUD
	m_pCharacter->addCharacterToNode ( this, ccp ( 300, 180 ) );	
	this->loadHUD ( );
}

KDvoid Ch3_ArchivingObjects::saveCharacter ( KDvoid )
{
	/*
	//Our archive file name
	NSString *fileName = @"dnd_character.archive";

	//We get our file path
	CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];
	
	//Save character
	[NSKeyedArchiver archiveRootObject:character toFile:filePath];
	*/
}

KDvoid Ch3_ArchivingObjects::loadHUD ( KDvoid )
{
	// Remove old HUD
	if ( m_pHud )
	{
		this->removeChild ( m_pHud );
	}

	// Init new HUD
	m_pHud = CCNode::create ( );
	m_pHud->setPosition ( ccp ( 0, 0 ) );
	
	// Add text to new HUD
	this->addTextToHUD ( "STR:", ccp ( 100, 220 ), ccWHITE );
	this->addTextToHUD ( ccszf ( "%d", m_pCharacter->getStrength ( ) ), ccp ( 130, 220 ), ccWHITE );

	this->addTextToHUD ( "DEX:", ccp ( 100, 200 ), ccWHITE );
	this->addTextToHUD (  ccszf ( "%d", m_pCharacter->getDexterity ( ) ), ccp ( 130, 200 ), ccWHITE );

	this->addTextToHUD ( "CON:", ccp ( 100, 180 ), ccWHITE );
	this->addTextToHUD (  ccszf ( "%d", m_pCharacter->getConstitution ( ) ), ccp ( 130, 180 ), ccWHITE );

	this->addTextToHUD ( "INT:", ccp ( 100, 160 ), ccWHITE );
	this->addTextToHUD (  ccszf ( "%d", m_pCharacter->getIntelligence ( ) ), ccp ( 130, 160 ), ccWHITE );

	this->addTextToHUD ( "WIS:", ccp ( 100, 140 ), ccWHITE );
	this->addTextToHUD (  ccszf ( "%d", m_pCharacter->getWisdom ( ) ), ccp ( 130, 140 ), ccWHITE );

	this->addTextToHUD ( "CHA:", ccp ( 100, 120 ), ccWHITE );
	this->addTextToHUD (  ccszf ( "%d", m_pCharacter->getCharisma ( ) ), ccp ( 130, 120 ), ccWHITE );

	this->addTextToHUD ( ccszf ( "%s %s", m_pCharacter->getCharColor ( ), m_pCharacter->getCharClass ( ) ), ccp ( 130, 250 ), ccWHITE );

	this->addChild ( m_pHud, 1 ); 
}

KDvoid Ch3_ArchivingObjects::addTextToHUD ( const KDchar* szText, const CCPoint& tPos, const ccColor3B& tColor )
{
	CCLabelBMFont*		pLabel = CCLabelBMFont::create ( szText, "eurostile_30.fnt" );
	pLabel->setPosition ( tPos );
	pLabel->setScale ( 0.5f );
	pLabel->setColor ( tColor );
	m_pHud->addChild ( pLabel, 1 );
}

KDvoid Ch3_ArchivingObjects::randomCharacter ( CCObject* pSender )
{
	m_pMessage->setString ( "" );

	this->removeCharacter ( );
	
	// Choose a random color and a random class	
	switch ( ( kdRand ( ) % 6 ) )
	{
		case 0 :	m_pCharacter->setCharColor ( "Red"	  );	break;
		case 1 :	m_pCharacter->setCharColor ( "Blue"	  );	break;
		case 2 :	m_pCharacter->setCharColor ( "Green"  );	break;
		case 3 :	m_pCharacter->setCharColor ( "Yellow" );	break;
		case 4 :	m_pCharacter->setCharColor ( "Orange" );	break;
		case 5 :	m_pCharacter->setCharColor ( "Purple" );	break;
	}

	// Choose random attributes
	m_pCharacter->setStrength		( kdRand ( ) % 10 + 5 );
	m_pCharacter->setDexterity		( kdRand ( ) % 10 + 5 );
	m_pCharacter->setConstitution	( kdRand ( ) % 10 + 5 );
	m_pCharacter->setIntelligence	( kdRand ( ) % 10 + 5 );
	m_pCharacter->setWisdom			( kdRand ( ) % 10 + 5 );
	m_pCharacter->setCharisma		( kdRand ( ) % 10 + 5 );
	
	// Make specific attributes higher for specific classes
	switch ( ( kdRand ( ) % 4 ) )
	{
		case 0 :
			m_pCharacter->setCharClass ( "Wizard" );
			m_pCharacter->setIntelligence ( m_pCharacter->getIntelligence ( ) + 10 );
			break;

		case 1 :
			m_pCharacter->setCharClass ( "Warrior" );
			m_pCharacter->setStrength ( m_pCharacter->getStrength ( ) + 5 );
			m_pCharacter->setConstitution ( m_pCharacter->getConstitution ( ) + 5 );
			break;

		case 2 :
			m_pCharacter->setCharClass ( "Ranger" );
			m_pCharacter->setDexterity ( m_pCharacter->getDexterity	( ) + 5 );
			m_pCharacter->setConstitution ( m_pCharacter->getConstitution ( ) + 5 );
			break;

		case 3 :
			m_pCharacter->setCharClass ( "Shaman" );
			m_pCharacter->setWisdom ( m_pCharacter->getWisdom ( ) + 10 );
			break;
	}
	
	// Finally, add the character
	m_pCharacter->addCharacterToNode ( this, ccp ( 300, 180 ) );
	
	// Update the HUD
	this->loadHUD ( );
	
	// Save the character on the disk
	this->saveCharacter ( );
}

KDvoid Ch3_ArchivingObjects::deleteData ( CCObject* pSender )
{
	/*
	//Our archive file name
	NSString *fileName = @"dnd_character.archive";

	//We get our file path
	CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

	//Delete our file
	[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
	*/

	// Set removal message
	m_pMessage->setString ( "Data deleted!" );

	// Remove character node and load a new default character
	this->removeCharacter ( );
	this->loadCharacter ( );
}

KDvoid Ch3_ArchivingObjects::removeCharacter ( KDvoid )
{
	this->removeChildByTag ( 0 ); 
	this->removeChildByTag ( 1 );

	CC_SAFE_RELEASE ( m_pCharacter );
	m_pCharacter = SimpleCharacter::create ( );
	m_pCharacter->retain ( );
}
