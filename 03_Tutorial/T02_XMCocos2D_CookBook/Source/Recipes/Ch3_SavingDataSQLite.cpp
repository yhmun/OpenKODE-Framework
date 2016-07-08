/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingDataSQLite.cpp
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
#include "Ch3_SavingDataSQLite.h"
#include "Libraries/SimpleAnimObject.h"

enum				// Disc animation types
{	
	DISC_INTACT	,
	DISC_BROKEN	,
};

enum
{
	TAG_MOUNTAIN_BG		= 3,
	TAG_GROUND_GRADIENT = 7,
};

KDbool Ch3_SavingDataSQLite::init ( KDvoid )
{	
	if ( !SimpleTimedGameRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Welcome to Skeet Shooter!" );

	this->initBackground ( );

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "skeet.plist" );
	
	m_pDiscs = CCArray::create ( );
	m_pDiscs->retain ( );
	
	m_nFireBulletCountdown = 0;
	m_nDiscRotationNum = 0;
	m_bReuseDiscs = KD_FALSE;
	m_pHiScores = KD_NULL;
	
	m_pGun = CCSprite::createWithSpriteFrameName ( "skeet_gun.png" ); 
	m_pGun->setPosition ( ccp ( 240, 0 ) );
	m_pGun->setAnchorPoint ( ccp ( 0.5f, 0.35f ) );
	this->addChild ( m_pGun, 3 );
	
	m_pBullet = SimpleAnimObject::createWithSpriteFrameName ( "skeet_bullet.png" ); 
	m_pBullet->setPosition ( m_pGun->getPosition ( ) );
	this->addChild ( m_pBullet, 2 ); 

	return KD_TRUE;
}

KDvoid Ch3_SavingDataSQLite::onExit ( KDvoid )
{
	m_pDiscs->release ( );

	CC_SAFE_RELEASE ( m_pHiScores );

	SimpleTimedGameRecipe::onExit ( );
}

KDvoid Ch3_SavingDataSQLite::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid Ch3_SavingDataSQLite::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_nGameState != MID_GAME )
	{
		return; 
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	KDfloat		fRadians = vectorToRadians ( ccpSub ( tPoint, m_pGun->getPosition ( ) ) );
	KDfloat		fDegrees = radiansToDegrees ( fRadians );
	
	m_pGun->setRotation ( fDegrees );
}

KDvoid Ch3_SavingDataSQLite::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_nGameState != MID_GAME )
	{
		return; 
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	if ( m_nFireBulletCountdown > 0 )
	{
		return;			// Can't fire bullet yet
	}
	else
	{
		m_nFireBulletCountdown = 20;
	}
	
	KDfloat		fRadians = vectorToRadians ( ccpSub ( tPoint, m_pGun->getPosition ( ) ) );
	CCPoint		tNormalVector = radiansToVector ( fRadians + KD_PI_F / 2 );
	KDfloat		fDegrees = radiansToDegrees ( fRadians );
	
	m_pBullet->setPosition ( m_pGun->getPosition ( ) );
	m_pBullet->setVelocity ( ccp ( tNormalVector.x * 20, tNormalVector.y * 20 ) );
	m_pBullet->setRotation ( fDegrees );
}

KDvoid Ch3_SavingDataSQLite::addHiScoresToMenu ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pHiScores, pObject )
	{
		CCDictionary*	pScore = (CCDictionary*) pObject;
		const KDchar*	szScore = ccszf ( "%s: %s", ( (CCString*) pScore->objectForKey ( "name" ) )->getCString ( ), 
													( (CCString*) pScore->objectForKey ( "score" ) )->getCString ( ) );

		m_pHiScoresMenu->addChild ( CCMenuItemFont::create ( szScore ) );
	}
}

KDvoid Ch3_SavingDataSQLite::loadHiScores ( KDvoid )
{
	/*
	//Our file and template names
	NSString *fileName = @"skeetshooter.sqlite";
	NSString *templateName = @"skeetshooter_template.sqlite";
	
	//We get our file path
	CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *filePath = [documentsDirectory stringByAppendingPathComponent:fileName];

	//If file doesn't exist in document directory create a new one from the template
	if(![[NSFileManager defaultManager] fileExistsAtPath:filePath]){	
		[[NSFileManager defaultManager] copyItemAtPath:getActualPath(templateName) 
			toPath:[NSString stringWithFormat:@"%@/%@", documentsDirectory, fileName] error:nil];
	}

	//Initialize the database
	if(!db){
		db = [FMDatabase databaseWithPath:filePath];
		[db setLogsErrors:YES];
		[db setTraceExecution:YES];
		[db retain];
	
		if(![db open]){
			NSLog(@"Could not open db.");
		}else{
			NSLog(@"DB opened successfully.");
		}
	}
	
	//Select all hi scores
	FMResultSet *rs = [db executeQuery:@"select * from hiscores"];
	
	//Load them into an array of dictionaries
	hiScores = [[NSMutableArray alloc] init];
	hiScores = [self createDictionariesArrayFromFMResultSet:rs fields:@"name,score"]; 

	//Set hi score
	for(id score in hiScores){
		int scoreNum = [[score objectForKey:@"score"] intValue];
		if(hiScore < scoreNum){
			hiScore = scoreNum;
		}
	}
	*/
}

KDvoid Ch3_SavingDataSQLite::addHiScore ( KDvoid )
{
	// Add hi score to db
	this->writeNewScore ( m_nCurrentScore, m_sCurrentPlayerName.c_str ( ) );
	
	// Reset dictionary
//	FMResultSet*	rs = [db executeQuery:@"SELECT * FROM hiscores"];
//	m_pHiScores = [self createDictionariesArrayFromFMResultSet:rs fields:@"name,score"]; 
}

KDvoid Ch3_SavingDataSQLite::deleteHiScores ( KDvoid )
{	
	m_pMessage->setString ( "Hi scores deleted!" );
	
	m_nHiScore = 0;
	
	//Close and release our db pointer
//	[db close];
//	[db release];
//	m_pDB = KD_NULL;

	kdRemove ( "/data/skeetshooter.sqlite" );
	
	// Load new blank hi scores
	this->loadHiScores ( );
}

KDvoid Ch3_SavingDataSQLite::startNewGame ( CCObject* pSender )
{
	SimpleTimedGameRecipe::startNewGame ( );
	
	m_pMessage->setString ( "SKEET SHOOTER!" );
}

KDvoid Ch3_SavingDataSQLite::gameOver ( KDvoid )
{
	SimpleTimedGameRecipe::gameOver ( );
}

KDvoid Ch3_SavingDataSQLite::step ( KDfloat fDelta )
{
	SimpleTimedGameRecipe::step ( fDelta );

	KDfloat		fGravity = 0.1f;

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pDiscs, pObject )
	{
		SimpleAnimObject*	pDisc = (SimpleAnimObject*) pObject;

		// Apply gravity
		pDisc->setVelocity ( ccpAdd ( pDisc->getVelocity ( ), ccp ( 0, -fGravity ) ) );

		// Update movement
		pDisc->update ( fDelta );
					
		if ( m_nGameState == MID_GAME )
		{	
			if ( pointIsInCircle ( m_pBullet->getPosition ( ), pDisc->getPosition ( ), pDisc->rect ( ).size.cx * 1.75f ) )
			{
				this->hitDisc ( pDisc );
			}
		}
	}
	m_pBullet->update ( fDelta );

	m_nFireBulletCountdown--;
	if ( m_nFireBulletCountdown < 0 )
	{
		m_pGun->setColor ( ccc3 ( 255, 200, 200 ) );
	}
	else
	{
		m_pGun->setColor ( ccc3 ( 255, 255, 255 ) );
	}

	if ( m_nGameState == MID_GAME )
	{		
		KDint	nNum = kdRand ( ) % 50;
		if ( nNum < 2 )
		{
			KDfloat		x;
			if ( nNum == 0 )
			{
				x = 480; 
			}
			else
			{
				x = 0; 
			}

			KDfloat		y = (KDfloat) ( kdRand ( ) % 60 );
			CCPoint		tVelocity = ccp ( kdRand ( ) % 4 + 3, 7.5f );

			if ( nNum == 0 )
			{
				tVelocity.x *= -1; 
			}
				
			this->fireDiscAtPosition ( ccp ( x, y ), tVelocity );
		}
	}
}

KDvoid Ch3_SavingDataSQLite::fireDiscAtPosition ( const CCPoint& tPosition, const CCPoint& tVelocity )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	
	if ( !m_bReuseDiscs && m_pDiscs->count ( ) >= 10 )
	{
		m_bReuseDiscs = KD_TRUE; 
	}
	
	SimpleAnimObject*		pDisc;
	if ( !m_bReuseDiscs )
	{
		pDisc = SimpleAnimObject::createWithSpriteFrameName ( "skeet_disc_white_00.png" ); 
	}
	else
	{
		pDisc = (SimpleAnimObject*) m_pDiscs->objectAtIndex ( m_nDiscRotationNum );

		CCAnimation*	pAnimation = CCAnimation::create ( );
		pAnimation->setDelayPerUnit ( 100.f );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( "skeet_disc_white_00.png" ) );
	
		pDisc->setOpacity ( 255 );
		
		pDisc->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
		m_nDiscRotationNum++;
		if ( m_nDiscRotationNum > 9 )
		{
			m_nDiscRotationNum = 0;
		}
	}
		
	pDisc->setAnimationType ( DISC_INTACT );
	pDisc->setPosition ( tPosition );
	pDisc->setVelocity ( tVelocity );
	pDisc->setScale ( 0.25f );
	
	if ( !m_bReuseDiscs ) 
	{
		m_pDiscs->addObject ( pDisc );
		this->addChild ( pDisc, 3 );
	}
}

KDvoid Ch3_SavingDataSQLite::hitDisc ( SimpleAnimObject* pDisc )
{
	if ( pDisc->getAnimationType ( ) == DISC_BROKEN )
	{
		return;		// We already hit this disc
	}
	
	this->processDiscHit ( );	

	pDisc->setAnimationType ( DISC_BROKEN );

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
		
	CCAnimation*			pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 0.05f );

	for ( KDint i = 1; i <= 6; i++ )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "skeet_disc_white_break_0%i.png", i ) ) );
	};
	
	pDisc->runAction
	(
		CCSequence::create
		(
			CCAnimate::create ( pAnimation ), 
			CCFadeOut::create ( 0.0f ), 
			KD_NULL
		)
	);
	
	const CCPoint	tVelocity = m_pBullet->getVelocity ( );
	pDisc->setVelocity ( ccp ( tVelocity.x / 10, tVelocity.y / 10 ) );

}

KDvoid Ch3_SavingDataSQLite::processDiscHit ( KDvoid )
{
	m_nCurrentScore++;
	this->setCurrentScore ( );
}

/*
CCArray* Ch3_SavingDataSQLite::createDictionariesArrayFromFMResultSet ( FMResultSet* rs, const KDchar* szFields )
{
	//Parse field string into an array
	CCArray * listFields = [fields componentsSeparatedByString:@","];

	//Create an array of dictionaries from each field
	NSMutableArray * items = [NSMutableArray arrayWithCapacity:1];
	while ([rs next]) {
		NSMutableDictionary * item = [NSMutableDictionary dictionaryWithCapacity:1];
		for (int i = 0; i < [listFields count]; i++) {
			NSString * key = [listFields objectAtIndex:i];
			NSString * value = [rs stringForColumn: key];
			if (value == NULL) value = @"";
			[item setObject:value forKey:key];
		}
		[items addObject:item];
	}
	[rs close];

	return items;
}
*/

KDvoid Ch3_SavingDataSQLite::writeNewScore ( KDint nScore, const KDchar* szName )
{
	/*
	//Find the hi score with this name
	NSString *selectQuery = [NSString stringWithFormat:@"SELECT * FROM hiscores WHERE name = '%@'", name];
	FMResultSet *rs = [db executeQuery:selectQuery];

	//What is the score? Is there a score at all?
	int storedScore = -1;
	while([rs next]){ 
		storedScore = [[rs stringForColumn:@"score"] intValue];
	}
	[rs close];

	if(storedScore == -1){
		//Name doesn't exist, add it
		NSString *insertQuery = [NSString stringWithFormat:@"INSERT INTO hiscores (name, score) VALUES ('%@','%i')", name, score];		
		rs = [db executeQuery:insertQuery];
		while([rs next]){};
		[rs close];
	}else if(score > storedScore){
		//Write new score for existing name
		NSString *updateQuery = [NSString stringWithFormat:@"UPDATE hiscores SET score='%i' WHERE name='%@'", score, name];
		rs = [db executeQuery:updateQuery];
		while([rs next]){};
		[rs close];
	}
	*/
}

KDvoid Ch3_SavingDataSQLite::initBackground ( KDvoid )
{
	// Draw the sky using blank.png
	CCSprite*	pSky = CCSprite::create ( "blank.png" );
	pSky->setPosition ( ccp ( 240, 190 ) );
	pSky->setTextureRect ( CCRect ( 0, 0, 480, 260 ) );
	pSky->setColor ( ccc3 ( 50, 50, 100 ) );
	this->addChild ( pSky, 0 );
			
	// Draw some mountains in the background
	CCSprite*	pMountains = CCSprite::create ( "mountains.png" );
	pMountains->setPosition ( ccp ( 250, 200 ) );
	pMountains->setScale ( 0.6f );
	this->addChild ( pMountains, 0, TAG_MOUNTAIN_BG );
	
	// Add a gradient below the mountains
	CCLayerGradient*	pGroundGradient = CCLayerGradient::create ( ccc4 ( 61, 33, 62, 255 ), ccc4 ( 65, 89, 54, 255 ), ccp ( 0, -1 ) ); 
	pGroundGradient->setContentSize ( CCSize ( 480, 200 ) );
	pGroundGradient->setPosition ( ccp ( 0, -50 ) );
	this->addChild ( pGroundGradient, 0, TAG_GROUND_GRADIENT );
}
