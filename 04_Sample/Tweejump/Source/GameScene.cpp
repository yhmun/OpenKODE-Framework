/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org  
 *         https://code.google.com/p/tweejump-cocos2dx
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "GameScene.h"

//  Bonus prize Images
const KDchar*  l_szBonusImage [ ]=
{
	"5.png"	,
	"10.png",
	"50.png",
	"100.png"
};


// Initialize the GameScene
KDbool GameScene::init ( KDvoid )
{
	CCLOG ( "GameScene::init" );

	if ( !MainScene::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&        tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Start off as game suspended
	m_bGameSuspended = KD_TRUE;

	// Get the bird sprite
	CCSprite*  pBird = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( "bird.png" ) );
	this->addChild ( pBird, 4, kBird );

	// Initialize the platforms
	initPlatforms ( );

	// Create the bonus sprite
	// Load in the bonus images, 5, 10, 50, 100
	for( KDint i = 0; i < kNumBonuses; i++ ) 
	{
		CCSprite*  pBonus = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( l_szBonusImage [ i ] ) );
		this->addChild ( pBonus, 4, kBonusStartTag + i );
		pBonus->setVisible( KD_FALSE );
	}

	// Create the Score Label
	CCLabelBMFont*  pScoreLabel = CCLabelBMFont::create ( "0",  "Fonts/bitmapFont.fnt" );
	this->addChild ( pScoreLabel, 5, kScoreLabel );

	// Center the label
	pScoreLabel->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy - 50 ) );

	// Start the GameScene stepping
	schedule ( schedule_selector ( GameScene::step ) );

	// Enable the touch events
	setTouchEnabled ( KD_TRUE );

	// Enable accelerometer events
	setAccelerometerEnabled ( KD_TRUE );

	// Start the game
	startGame ( );

    return KD_TRUE;
}

// Initialize the platforms that the bird jumps on
KDvoid GameScene::initPlatforms ( KDvoid )
{
	CCLOG ( "initPlatforms" );
	
	m_nCurrentPlatformTag = kPlatformsStartTag;
	while ( m_nCurrentPlatformTag < kPlatformsStartTag + kNumPlatforms ) 
	{
		initPlatform ( );
		m_nCurrentPlatformTag++;
	}
	
	resetPlatforms ( );
}

// Supports Two types of platforms although only one exists in the art.
KDvoid GameScene::initPlatform ( KDvoid )
{
	CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCSprite*  pPlatform = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( kdRand ( ) % 2 == 0 ? "platform1.png" : "platform2.png" ) );

	this->addChild ( pPlatform, 3, m_nCurrentPlatformTag );
}

// Initializes everything and then starts the game by setting the gameSuspend
KDvoid GameScene::startGame ( KDvoid )
{
	CCLOG ( "startGame" );

	m_nScore = 0;
	
	resetClouds		( );
	resetPlatforms	( );
	resetBird		( );
	resetBonus		( );
	
	m_bGameSuspended = KD_FALSE;
}

// Reset the platforms as they move off the screen
KDvoid GameScene::resetPlatforms ( KDvoid )
{
	CCLOG ( "resetPlatforms" );
	
	m_fCurrentPlatformY			 = -1;					// Set the current platform y to -1 so it gets initialized
	m_nCurrentPlatformTag		 = kPlatformsStartTag;	// set starting platform
	m_fCurrentMaxPlatformStep	 = 60.0f;				// maximum space between platforms
	m_nCurrentBonusPlatformIndex = 0;
	m_nCurrentBonusType			 = 0;
	m_nPlatformCount			 = 0;

	while ( m_nCurrentPlatformTag < kPlatformsStartTag + kNumPlatforms ) 
	{
		resetPlatform ( );
		m_nCurrentPlatformTag++;
	}
}

KDvoid GameScene::resetPlatform ( KDvoid )
{	
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	// We set this to -1 to initialize the first platform y coordinate
	if ( m_fCurrentPlatformY < 0 ) 
	{
		m_fCurrentPlatformY = (KDfloat) kStartingCurrentPlatformY;
	} 
	else 
	{	
		// If not the first one then randomly determine a y coordinate for the platform
		m_fCurrentPlatformY += kdRand ( ) % (KDint) ( m_fCurrentMaxPlatformStep - kMinPlatformStep ) + kMinPlatformStep;

		// If the current platofmr step is less than the max (top of screen) then increment it
		if ( m_fCurrentMaxPlatformStep < kMaxPlatformStep ) 
		{
			m_fCurrentMaxPlatformStep += 0.5f;
		}
	}
	
	CCSprite*  pPlatform = (CCSprite*) getChildByTag ( m_nCurrentPlatformTag );

	if ( kdRand() % 2 == 1 )
	{
		pPlatform->setScaleX ( -1.0f );
	}

	CCSize   tSize = pPlatform->getContentSize ( );
	KDfloat  x;

	// If the current platform is the first one initialized then just center it
	if ( m_fCurrentPlatformY == (KDfloat) kStartingCurrentPlatformY ) 
	{
		x = tWinSize.cx / 2;
	} 
	else 
	{
		x = kdRand ( ) % (KDint) ( tWinSize.cx - tSize.cx ) + tSize.cx / 2;
	}
	
	pPlatform->setPosition ( ccp ( x, m_fCurrentPlatformY ) );
	m_nPlatformCount++;

	// If the platform is to have to bonus then put it there.
	if ( m_nPlatformCount == m_nCurrentBonusPlatformIndex ) 
	{
		CCLOG ( "platformCount = %d", m_nPlatformCount );
		CCSprite*  pBonus = (CCSprite*) getChildByTag ( kBonusStartTag + m_nCurrentBonusType );
		pBonus->setPosition ( ccp ( x, m_fCurrentPlatformY + 30 ) );
		pBonus->setVisible ( KD_TRUE );
	}
}

// Reset the bird to its home location
KDvoid GameScene::resetBird ( KDvoid )
{
	CCLOG ( "resetBird" );

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pBird = (CCSprite*) getChildByTag ( kBird );

	// Place bird in center
	m_tBirdPos = ccpMid ( tWinSize );
	pBird->setPosition ( m_tBirdPos );
	
	m_tBirdVel.x = 0;
	m_tBirdVel.y = 0;
	
	m_tBirdAcc.x = 0;
	m_tBirdAcc.y = -550.0f;	// -250.0f makes the bird jump higher, -950.0f makes the bird jump lower
	
	m_bBirdLookingRight = KD_TRUE;
	pBird->setScaleX ( 1.0f );
}

// Reset the bonus types based on the current score
KDvoid GameScene::resetBonus ( KDvoid )
{
	CCLOG ( "resetBonus" );
	
	CCSprite*  pBonus = (CCSprite*) getChildByTag ( kBonusStartTag + m_nCurrentBonusType );

	// Set the bonus to not be visible
	pBonus->setVisible ( KD_FALSE );

	// Randomly determine which platform will get the bonus next by adding a random amount
	m_nCurrentBonusPlatformIndex += kdRand ( ) % ( kMaxBonusStep - kMinBonusStep ) + kMinBonusStep;

	if ( m_nScore < 10000 ) 
	{
		m_nCurrentBonusType = 0;
	} 
	else if ( m_nScore < 20000 ) 
	{
		m_nCurrentBonusType = kdRand ( ) % 2;
	} 
	else if ( m_nScore < 10000 ) 
	{
		m_nCurrentBonusType = kdRand ( ) % 3;
	} 
	else 
	{
		m_nCurrentBonusType = kdRand ( ) % 2 + 2;
	}
}

/////////////////////////////////////////////////////////
// Main game loop
KDvoid GameScene::step ( KDfloat fDelta )
{
//	CCLOG ( "Game::step" );

	// Return if game suspended
	if ( m_bGameSuspended )	
	{
		return;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	// Get the bird sprite
	CCSprite*  pBird = (CCSprite*) getChildByTag ( kBird );
	
	// Update the player x position based on velocity and delta time
	m_tBirdPos.x += m_tBirdVel.x * fDelta;
	
	// Flip the player based on it's x velocity and current looking direction
	if ( m_tBirdVel.x < -30.0f && m_bBirdLookingRight ) 
	{
		m_bBirdLookingRight = KD_FALSE;
		pBird->setScaleX ( -1.0f );
	}
	else if ( m_tBirdVel.x > 30.0f && !m_bBirdLookingRight ) 
	{
		m_bBirdLookingRight = KD_TRUE;
		pBird->setScaleX ( 1.0f );
	}

	// Calculate the max and min x values for the bird
	// based on the screen and bird widths
	CCSize  tBirdSize = pBird->getContentSize ( );
	KDfloat fMaxX = tWinSize.cx - tBirdSize.cx / 2;
	KDfloat fMinX = tBirdSize.cx / 2;
	
	// Limit the bird position based on max and min values allowed
	if ( m_tBirdPos.x > fMaxX ) m_tBirdPos.x = fMaxX;
	if ( m_tBirdPos.x < fMinX ) m_tBirdPos.x = fMinX;

	// Update the bird velocity based on acceleration and time
	m_tBirdVel.y += m_tBirdAcc.y * fDelta;

	// Update the bird y positin based on velocity and time
	m_tBirdPos.y += m_tBirdVel.y * fDelta;
	
	////////////////////////////////////////////////////////////////////////////
	// Handle the bonus scoring
	CCSprite*  pBonus = (CCSprite*) getChildByTag ( kBonusStartTag + m_nCurrentBonusType );

	// If bonus is visible then see if the bird is within range to get the bonus
	if ( pBonus->isVisible ( ) ) 
	{
		CCPoint tBonusPos = pBonus->getPosition ( );
		KDfloat fRange = 20.0f;

		// If the player is within range of the bonus value then award the prize
		if ( m_tBirdPos.x > tBonusPos.x - fRange &&
		     m_tBirdPos.x < tBonusPos.x + fRange &&
		     m_tBirdPos.y > tBonusPos.y - fRange &&
		     m_tBirdPos.y < tBonusPos.y + fRange ) 
		{
			// Update score based on bonus
			switch ( m_nCurrentBonusType ) 
			{
				case kBonus5	: m_nScore += 5000;   break;
				case kBonus10	: m_nScore += 10000;  break;
				case kBonus50	: m_nScore += 50000;  break;
				case kBonus100	: m_nScore += 100000; break;
			}

			// Build the score string to display
			KDchar  szScore [10] = {0};
			kdSprintf ( szScore, "%d", m_nScore );
			CCLabelBMFont*  pScoreLabel = (CCLabelBMFont*) getChildByTag ( kScoreLabel );
			pScoreLabel->setString ( szScore );

			// Highlight the score with some actions to celebrate the bonus win
			CCActionInterval*  pA1 = CCScaleTo::create ( 0.2f, 1.5f, 0.8f );
			CCActionInterval*  pA2 = CCScaleTo::create ( 0.2f, 1.0f, 1.0f );
			pScoreLabel->runAction ( CCSequence::create 
			(
				pA1, pA2, pA1->copy ( ), pA2->copy ( ), pA1->copy ( ), pA2->copy ( ), KD_NULL 
			) );

			// Reset the bonus to another platform
			resetBonus ( );
		}
	}

	KDint  nTag;
	// If the bird has stopped moving then make it jump from the platform it is on
	if ( m_tBirdVel.y < 0 ) 
	{
		// Search through all the platforms and compare the birds position with the platfor position
		for ( nTag = kPlatformsStartTag; nTag < kPlatformsStartTag + kNumPlatforms; nTag++ ) 
		{
			CCSprite*  pPlatform = (CCSprite*) getChildByTag ( nTag );

			CCSize   tPlatformSize = pPlatform->getContentSize ( );
			CCPoint  tPlatformPos  = pPlatform->getPosition ( );
			
			fMaxX = tPlatformPos.x - tPlatformSize.cx / 2 - 10;
			fMinX = tPlatformPos.x + tPlatformSize.cx / 2 + 10;

			KDfloat  fMinY = tPlatformPos.y + ( tPlatformSize.cy + tBirdSize.cy ) / 2 - kPlatformTopPadding;
			
			if ( m_tBirdPos.x > fMaxX &&
			     m_tBirdPos.x < fMinX &&
			     m_tBirdPos.y > tPlatformPos.y &&
			     m_tBirdPos.y < fMinY ) 
			{
				jump ( );
				break;	// Can only jump from one platform at a time to break out of the loop
			}
		}
	
		// If the bird has fallen below the screen then game over
		if ( m_tBirdPos.y < - tBirdSize.cy / 2 ) 
		{
			// [self showHighscores];   <== NEED TO IMPLEMENT THE HIGHTSCORE
			resetBird ( );		// Highscore not implmented yet so just keep on going.
		}
	} 
	else if ( m_tBirdPos.y > tWinSize.cy / 2 ) 
	{
		// If bird position is greater than the middle of the screen then move the platforms
		// the difference between the bird y position and middle point of the screen
		KDfloat   fDelta2 = m_tBirdPos.y - tWinSize.cy / 2;

		// Set the bird y position to the middle of the screen
		m_tBirdPos.y = tWinSize.cy / 2;

		// Move the current platform y by the delta amount
		m_fCurrentPlatformY -= fDelta2;

		// Move the clouds vertically and reset if necessary
		for ( nTag = kCloudsStartTag; nTag < kCloudsStartTag + kNumClouds; nTag++ ) 
		{
			CCSprite*  pCloud = (CCSprite*) getChildByTag ( nTag );
			CCPoint    tPos = pCloud->getPosition ( );

			// Calculate new position for cloud
			tPos.y -= fDelta2 * pCloud->getScaleY ( ) * 0.8f;

			// If the cloud is off the screen then need to reset this cloud else set its new position
			if ( tPos.y < -pCloud->getContentSize ( ).cy / 2 ) 
			{
				m_nCurrentCloudTag = nTag;
				resetCloud ( );
			} 
			else 
			{	// Update the new y position for the cloud.
				pCloud->setPosition ( tPos );
			}
		}

		// Move the platforms vertically and reset if necessary
		for ( nTag = kPlatformsStartTag; nTag < kPlatformsStartTag + kNumPlatforms; nTag++ ) 
		{
			CCSprite*  pPlatform = (CCSprite*) getChildByTag ( nTag );			
			CCPoint    tPos = pPlatform->getPosition ( );

			// Calculate new position for platform
			tPos = ccp ( tPos.x, tPos.y - fDelta2 );

			// If the platform is off the screen then reset the platform else set its new position
			if ( tPos.y < - pPlatform->getContentSize ( ).cy / 2 ) 
			{
				m_nCurrentPlatformTag = nTag;
				resetPlatform ( );
			} 
			else 
			{
				pPlatform->setPosition ( tPos );
			}
		}

		// If the bonus is visible then adjust it's y position
		if ( pBonus->isVisible ( ) ) 
		{
			CCPoint  tPos = pBonus->getPosition ( );

			// Calculate new position of bonus
			tPos.y -= fDelta2;
			
			// If the bonus is off the screen then reset the bonus else set its new position
			if ( tPos.y < -pBonus->getContentSize ( ).cy / 2 ) 
			{
				resetBonus ( );
			} 
			else 
			{
				pBonus->setPosition ( tPos );
			}
		}
		
		// Update score based on how much the bird has moved
		m_nScore += (KDint) fDelta2;

		// Display the new score value
		KDchar  szScore [10] = { 0 };
		kdSprintf ( szScore, "%d", m_nScore );
		CCLabelBMFont*  pScoreLabel = (CCLabelBMFont*) getChildByTag ( kScoreLabel );
		pScoreLabel->setString ( szScore );

	}

	// Set the birds position
	pBird->setPosition ( m_tBirdPos );
}

/////////////////////////////////////////////////////////
// Update the player y velocity for jumping
// Base the y velocity on the x velocity
KDvoid GameScene::jump ( KDvoid )
{
	m_tBirdVel.y = 350.0f + kdFabsf ( m_tBirdVel.x );
}

KDvoid GameScene::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority + 1, KD_TRUE );
}

KDbool GameScene::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

/////////////////////////////////////////////////////////
// Touch on left side of screen moves player left
// Touch on right side of screen moves player right
KDvoid GameScene::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCPoint  tLocation = pTouch->getLocation ( );	

	tLocation =  convertToNodeSpace ( tLocation );

	KDfloat  fTouchCenter = tWinSize.cx / 2 - tLocation.x;
	KDfloat  fAccelFilter = 0.1f;
	m_tBirdVel.x = m_tBirdVel.x * fAccelFilter - fTouchCenter;  
}

/////////////////////////////////////////////////////////
// Acceleramoter routine to move the player object
KDvoid GameScene::didAccelerate ( CCAcceleration* pAccelerationValue )
{
	if ( m_bGameSuspended )
	{
		return;
	}

	KDfloat  fAccelFilter = 0.1f;
	m_tBirdVel.x = m_tBirdVel.x * fAccelFilter + pAccelerationValue->x * ( 1.0f - fAccelFilter ) * 500.0f;

	// CCLOG ( "GameScene::didAccelerate %f",m_tBirdVel.x );
}
