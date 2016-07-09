/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingDataCoreData.cpp
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
#include "Ch3_SavingDataCoreData.h"
#include "Libraries/MatchCard.h"

KDbool Ch3_SavingDataCoreData::init ( KDvoid )
{	
	if ( !SimpleTimedGameRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Welcome to Memory!" );
	
	this->initCardDict ( );
	
	CCLabelBMFont*		pStrikesLabel = CCLabelBMFont::create ( "STRIKES:", "eurostile_30.fnt" );
	pStrikesLabel->setPosition ( ccp ( 430, 120 ) );
	pStrikesLabel->setScale ( 0.5f );
	this->addChild ( pStrikesLabel, Z_TIMED_GAME_HUD );

	this->createCards ( );

	return KD_TRUE;
}


KDvoid Ch3_SavingDataCoreData::onExit ( KDvoid )
{
	m_pCardDict->release ( );

	SimpleTimedGameRecipe::onExit ( );
}

KDvoid Ch3_SavingDataCoreData::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{

}

KDvoid Ch3_SavingDataCoreData::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{

}

KDvoid Ch3_SavingDataCoreData::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_nGameState != MID_GAME )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	for ( KDint x = 0; x <= 5; x++ )
	{
		for ( KDint y = 0; y <= 2; y++ )
		{
			MatchCard*		pCard = (MatchCard*) ( (CCDictionary*) m_pCardDict->objectForKey ( x ) )->objectForKey ( y );

			if ( !pCard->isCardUp ( ) && pointIsInRect ( tPoint, pCard->rect ( ) ) )
			{
				this->flipCard ( pCard );
			}
		}
	}	
}

KDvoid Ch3_SavingDataCoreData::addHiScoresToMenu ( KDvoid )
{
	/*
	for(id score in hiScores){
		Hiscore *hiscore = (Hiscore*)score;
		NSString *scoreStr = [NSString stringWithFormat:@"%@: %i", hiscore.name, [hiscore.score intValue]];
		[hiScoresMenu addChild:[CCMenuItemFont itemFromString:scoreStr]];
	}
	*/
}

KDvoid Ch3_SavingDataCoreData::loadHiScores ( KDvoid )
{
	/*
	//Initialization
	managedObjectContext = self.managedObjectContext;

	//Attempt to create SQLite database
	NSEntityDescription *entity;
	@try{
		//Define our table/entity to use  
		entity = [NSEntityDescription entityForName:@"Hiscore" inManagedObjectContext:managedObjectContext];   
	   
	}@catch (NSException *exception){
		NSLog(@"Caught %@: %@", [exception name], [exception reason]);
	
		//Copy SQLite template because creation failed
		NSString *fileName = @"memory.sqlite";
		NSString *templateName = @"memory_template.sqlite";
	
		//File paths
		CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [paths objectAtIndex:0];
		NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

		if(![[NSFileManager defaultManager] fileExistsAtPath:filePath]){	
			//If file doesn't exist in document directory create a new one from the template
			[[NSFileManager defaultManager] copyItemAtPath:getActualPath(templateName) 
				toPath:[NSString stringWithFormat:@"%@/%@", documentsDirectory, fileName] error:nil];
		}
		
		//Finally define our table/entity to use
		entity = [NSEntityDescription entityForName:@"Hiscore" inManagedObjectContext:managedObjectContext];
	}
	
	//Setup the fetch request  
	NSFetchRequest *request = [[NSFetchRequest alloc] init];  
	[request setEntity:entity];   
  
	//Define how we will sort the records with a descriptor 
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"score" ascending:NO];  
	CCArray *sortDescriptors = [CCArray arrayWithObject:sortDescriptor];  
	[request setSortDescriptors:sortDescriptors];  
	[sortDescriptor release]; 

	//Init hiScores
	hiScores = [[managedObjectContext executeFetchRequest:request error:nil] mutableCopy];
	
	//Add an intial score if necessary	
	if(hiScores.count < 1){
		NSLog(@"Putting in initial hi score");
		currentScore = 0;
		currentPlayerName = @"Player1";
		[self addHiScore];
		hiScores = [[managedObjectContext executeFetchRequest:request error:nil] mutableCopy];
	}
	
	//Set the hi score
	Hiscore *highest = [hiScores objectAtIndex:0];
	hiScore = [highest.score intValue];
	*/
}

KDvoid Ch3_SavingDataCoreData::addHiScore ( KDvoid )
{
	/*
	bool hasScore = NO;
	
	//Add score if player's name already exists
	for(id score in hiScores){
		Hiscore *hiscore = (Hiscore*)score;
		if([hiscore.name isEqualToString:currentPlayerName]){
			hasScore = YES;
			if(currentScore > [hiscore.score intValue]){
				hiscore.score = [NSNumber numberWithInt:currentScore];
			}
		}
	}

	//Add new score if player's name doesn't exist
	if(!hasScore){
		Hiscore *hiscoreObj = (Hiscore *)[NSEntityDescription insertNewObjectForEntityForName:@"Hiscore" inManagedObjectContext:managedObjectContext];  
		[hiscoreObj setName:currentPlayerName];
		[hiscoreObj setScore:[NSNumber numberWithInt:currentScore]];
		[hiScores addObject:hiscoreObj];
	}
	
	//Save managedObjectContext
    [managedObjectContext save:nil];
	*/
}

KDvoid Ch3_SavingDataCoreData::deleteHiScores ( KDvoid )
{
	/*
	//Delete all Hi Score objects
	NSFetchRequest * allHiScores = [[NSFetchRequest alloc] init];
	[allHiScores setEntity:[NSEntityDescription entityForName:@"Hiscore" inManagedObjectContext:managedObjectContext]];
	[allHiScores setIncludesPropertyValues:NO]; //only fetch the managedObjectID

	CCArray * hs = [managedObjectContext executeFetchRequest:allHiScores error:nil];
	[allHiScores release];
	for (NSManagedObject *h in hs) {
	  [managedObjectContext deleteObject:h];
	}
	
	//Our file name
	NSString *fileName = @"memory.sqlite";

	//We get our file path
	CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

	//Delete our file
	[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
	
	[message setString:@"Hi scores deleted!"];
	
	hiScore = 0;
	[hiScores removeAllObjects];
	[hiScores release];
	hiScores = nil;
	
	//Finally, load clean hi scores
	[self loadHiScores];
	*/
}

KDvoid Ch3_SavingDataCoreData::startNewGame ( CCObject* pSender )
{
	SimpleTimedGameRecipe::startNewGame ( pSender );

	m_fGameTimer = 60;
	m_nCardsFlipped = 0;
	m_nNumberOfStrikes = 0;
	m_nRound = 1;
	
	this->deleteCardsAndStrikes ( );
	
	this->createCards ( );

	this->startStudyTime ( );
	
	m_pMessage->setString ( "Study the cards..." );
}

KDvoid Ch3_SavingDataCoreData::gameOver ( KDvoid )
{
	SimpleTimedGameRecipe::gameOver ( );
}

KDvoid Ch3_SavingDataCoreData::step ( KDfloat fDelta )
{
	SimpleTimedGameRecipe::step ( fDelta );
	if ( m_nGameState == MID_GAME )
	{
		
	}
}

KDvoid Ch3_SavingDataCoreData::initCardDict ( KDvoid )
{
	m_pCardDict = CCDictionary::create ( );
	m_pCardDict->retain ( );

	for ( KDint x = 0; x <= 5; x++ )
	{
		m_pCardDict->setObject ( CCDictionary::create ( ), x );
	}
}

KDvoid Ch3_SavingDataCoreData::createCardPairWithIndex1 ( const CCPoint& tIndex1, const CCPoint& tIndex2 )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "match_card.plist" );
	
	KDint			nCardTypeNum = kdRand ( ) % 4;
	const KDchar*	szCardFrameName;
	
	switch ( nCardTypeNum )
	{
		case 0 :	szCardFrameName = "match_card_circle.png";		break;
		case 1 :	szCardFrameName = "match_card_square.png";		break;
		case 2 :	szCardFrameName = "match_card_pentagon.png";	break;
		case 3 :	szCardFrameName = "match_card_triangle.png";	break;
	}
	
	MatchCard*		pCard1 = MatchCard::createWithSpriteFrameName ( szCardFrameName );
	pCard1->setAllPositions ( ccp ( tIndex1.x * 50+ 50, tIndex1.y * 80 + 70 ) );
	this->addChild ( pCard1->getCardUp   ( ) );
	this->addChild ( pCard1->getCardDown ( ) );

	( (CCDictionary*) m_pCardDict->objectForKey ( (KDint) tIndex1.x ) )->setObject ( pCard1, (KDint) tIndex1.y );
	
	MatchCard*		pCard2 = MatchCard::createWithSpriteFrameName ( szCardFrameName );
	pCard2->setAllPositions ( ccp ( tIndex2.x * 50+ 50, tIndex2.y * 80 + 70 ) );
	this->addChild ( pCard2->getCardUp   ( ) );
	this->addChild ( pCard2->getCardDown ( ) );

	( (CCDictionary*) m_pCardDict->objectForKey ( (KDint) tIndex2.x ) )->setObject ( pCard2, (KDint) tIndex2.y );
}

KDvoid Ch3_SavingDataCoreData::deleteCardsAndStrikes ( KDvoid )
{
	for ( KDint x = 0; x <= 5; x++ )
	{
		for ( KDint y = 0; y <= 2; y++ )
		{
			MatchCard*		pCard = (MatchCard*) ( (CCDictionary*) m_pCardDict->objectForKey ( x ) )->objectForKey ( y );

			this->removeChild ( pCard->getCardUp   ( ) );
			this->removeChild ( pCard->getCardDown ( ) );

			( (CCDictionary*) m_pCardDict->objectForKey ( x ) )->removeObjectForKey ( y );
		}
	}	
	
	this->removeChildByTag ( 1 ); 
	this->removeChildByTag ( 2 ); 
	this->removeChildByTag ( 3 ); 
}

KDvoid Ch3_SavingDataCoreData::createCards ( KDvoid )
{
	CCArray*	pNumArr = CCArray::create ( );
	for ( KDint x = 0; x <= 5; x++ )
	{
		for ( KDint y = 0; y <= 2; y++ )
		{
			pNumArr->addObject ( CCPointValue::create ( ccp ( x, y ) ) );
		}
	}

	CCArray*	pRandNumArr = CCArray::create ( );
	for ( KDint z = 0; z < 18; z++ )
	{
		KDuint			uRandNum = kdRand ( ) % pNumArr->count ( );
		CCObject*		pPoint = pNumArr->objectAtIndex ( uRandNum );

		pNumArr->removeObject ( pPoint );
		pRandNumArr->addObject ( pPoint );
	}
	
	for ( KDint i = 0; i < 18; i += 2 )
	{
		CCPoint		tIndex1 = ( (CCPointValue*) pRandNumArr->objectAtIndex ( i ) )->getValue ( ); 
		CCPoint		tIndex2 = ( (CCPointValue*) pRandNumArr->objectAtIndex ( i + 1 ) )->getValue ( ); 

		this->createCardPairWithIndex1 ( tIndex1, tIndex2 );
	}
}

KDvoid Ch3_SavingDataCoreData::startStudyTime ( KDvoid )
{
	this->runAction 
	(
		CCSequence::create 
		(
			CCDelayTime::create ( 5.f ),
			CCCallFunc::create ( this, callfunc_selector ( Ch3_SavingDataCoreData::flipNewCards ) ),
			KD_NULL
		)
	);
}

KDvoid Ch3_SavingDataCoreData::flipNewCards ( KDvoid )
{
	for ( KDint x = 0; x <= 5; x++ )
	{
		for ( KDint y = 0; y <= 2; y++ )
		{
			MatchCard*		pCard = (MatchCard*) ( (CCDictionary*) m_pCardDict->objectForKey ( x ) )->objectForKey ( y );
			pCard->flipCardNoAnim ( );
		}
	}
	m_pMessage->setString ( "Go!" );
}

KDvoid Ch3_SavingDataCoreData::flipCard ( MatchCard* pCard )
{
	m_nCardsFlipped++;
	
	if ( m_nCardsFlipped % 2 == 1 ) 
	{
		m_pLastFlipped1 = pCard;
	}
	else
	{
		m_pLastFlipped2 = pCard;
	}
	
	if ( m_nCardsFlipped != 0 && m_nCardsFlipped % 2 == 0 )
	{
		this->compareCards ( );
	}
	else
	{
		pCard->flipCardNoAnim ( );
		m_pMessage->setString ( "" );
	}
	
	if ( m_nCardsFlipped >= 18 && m_nNumberOfStrikes < 3 )
	{
		m_nRound++;

		m_pMessage->setString ( ccszf ( "Great job! Round %d!", m_nRound ) );

		m_fGameTimer += 30;
		m_nCardsFlipped = 0;
		m_nNumberOfStrikes = 0;
		this->deleteCardsAndStrikes ( );
		this->createCards ( );
		this->startStudyTime ( );
	}
}

KDvoid Ch3_SavingDataCoreData::compareCards ( KDvoid )
{
	if ( !kdStrcmp ( m_pLastFlipped1->getCardType ( ), m_pLastFlipped2->getCardType ( ) ) ) 
	{
		m_nCurrentScore++;
		m_pMessage->setString ( "Correct!" );
		this->setCurrentScore ( );
		m_pLastFlipped2->flipCard ( );
	}
	else
	{
		m_pMessage->setString ( "Wrong!" );
		this->addStrike ( );
		m_pLastFlipped2->flipCardNoAnim ( );
	}
}

KDvoid Ch3_SavingDataCoreData::addStrike ( KDvoid )
{
	m_nNumberOfStrikes++;
	
	if ( m_nNumberOfStrikes >= 3 )
	{
		this->gameOver ( );
	}
	
	CCSprite*	pSprite = CCSprite::create ( "red_x.png" );
	pSprite->setScale ( 0.15f );
	pSprite->setPosition ( ccp ( m_nNumberOfStrikes * 16 + 395, 100 ) );
	this->addChild ( pSprite, Z_TIMED_GAME_HUD, m_nNumberOfStrikes );
}



