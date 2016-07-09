/* --------------------------------------------------------------------------
 *
 *      File            TheLevel.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "TheLevel.h"
#include "TheMenu.h"
#include "TheStack.h"
#include "ContactListener.h"
#include "GameData.h"
#include "GameSounds.h"
#include "Ninja.h"
#include "GroundPlane.h"
#include "StackObject.h"
#include "StartPlatform.h"
#include "CustomAnimation.h"

static TheLevel*	l_pLayerInstance = KD_NULL;

TheLevel* TheLevel::sharedLevel ( KDvoid )
{
	return l_pLayerInstance;
}

KDbool TheLevel::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	GameData*		pData = GameData::sharedData ( );

	// enable touches
	this->setTouchEnabled ( KD_TRUE );

	l_pLayerInstance						= this;				

	if ( m_tContentSize.cx == 1024 )
	{
		IS_IPAD								= KD_TRUE;
	}
	else
	{
		IS_IPAD								= KD_FALSE;
	}
	
	IS_IPHONE								= !IS_IPAD;
	IS_RETINA								= KD_FALSE;

	m_bAreWeOnTheIPad						= KD_FALSE;

	m_bSlingShotNinjaInHand					= KD_FALSE;

	m_bSomethingJustScored					= KD_FALSE;
	m_bDottingOn							= KD_FALSE;	
	m_bThrowInProgress						= KD_FALSE;					// is a throw currently in progress, as in, is a ninja in midair (mostly used to prevent tossing two ninjas, one right after another)
	m_bAreWeInTheStartingPosition			= KD_TRUE;					// is the world back at 0 on the X axis (if yes, then we can put a ninja in the sling)
				
	m_nThrowCount							= 0;
	m_nDotTotalOnOddNumberedTurn			= 0;
	m_nDotTotalOnEvenNumberedTurn			= 0;

	m_nCurrentLevel							= pData->getLevel ( );		// use currentLevel =  0 for testing new shapes, will call [self buildLevelWithAllShapes];

	m_nPointTotalThisRound					= 0;
	m_nPointsToPassLevel					= pData->getPointsToPassLevel  ( );
	m_nBonusThisRound						= 0;
	m_nBonusPerExtraNinja					= pData->getBonusPerExtraNinja ( );	
			
	CCLOG ( "The level is %d, you need %d to move up a level", m_nCurrentLevel, m_nPointsToPassLevel );

	// PREFERENCE VARIABLES....			
	m_bOpenWithMenuInsteadOfGame			= KD_FALSE;					// start with the menu opening the game
			
	m_bAutoReverseOn						= KD_FALSE;
	m_bPanningTowardSling					= KD_FALSE;
	m_bContinuePanningScreenOnFingerRelease = KD_TRUE;					// if the screen panning is midway between either the sling or targets, when you release your finger the screen will continue panning the last direction it moved (jumpy on iPhone if set to NO)
	m_bReverseHowFingerPansScreen			= KD_FALSE;					// switch to yes to reverse. 
	m_bTopLeftTouchEnablesDebugMode			= KD_TRUE;					// SET TO NO IN FINAL BUILD
	m_bUseImagesForPointScoreLabels			= KD_TRUE;					// IF NO, means you use Marker Felt text for scores

	// set up background art			
	m_pBackgroundLayerClouds = CCSprite::create ( pData->getBackgroundCloudsFileName ( ) );		// will return the background clouds file for a particular level
	this->addChild ( m_pBackgroundLayerClouds, DepthClouds );

	m_pBackgroundLayerHills = CCSprite::create ( pData->getBackgroundHillsFileName ( ) );		// will return the background hills file for a particular level
	this->addChild ( m_pBackgroundLayerHills, DepthHills );
	m_pBackgroundLayerHills->setScaleX ( 1.05f );

	m_pSlingShotFront = CCSprite::create ( "slingshot_front.png" );		
	this->addChild ( m_pSlingShotFront, DepthSlingShotFront );

	m_pStrapFront = CCSprite::create ( "strap.png" );		
	this->addChild ( m_pStrapFront, DepthStrapFront );

	m_pStrapBack = CCSprite::create ( "strapBack.png" );		
	this->addChild ( m_pStrapBack, DepthStrapBack );

	m_pStrapEmpty = CCSprite::create ( "strapEmpty.png" );		
	this->addChild ( m_pStrapEmpty, DepthStrapBack );

	m_pStrapBack ->setVisible ( KD_FALSE );								// visible only when stretching
	m_pStrapFront->setVisible ( KD_FALSE );								// visible only when stretching
			
	// setup positions and variables for iPad devices or iPhones
	if ( IS_IPAD )
	{
		m_bAreWeOnTheIPad			= KD_TRUE;

		// vars 
		m_nMaxStretchOfSlingShot	= 75;								// best to leave as is, since this value ties in closely to the image size of strap.png. (should be 1/4 the size of the source image)
		m_fMultipyThrowPower		= 1.0f;								// fine tune how powerful the sling shot is. Range is probably best between .5 to 1.5, and a little goes a long way
		
		m_fWorldMaxHorizontalShift	= -m_tContentSize.cx;				// This determines how far the user can slide left or right to see the entire board. Always a negative number
		m_fMaxScaleDownValue		= 1.0f;								// range should probably be between 0.75 and 1.0;
		m_fScaleAmount				= 0;								// increment to change the scale of the entire world when panning
		m_nInitialPanAmount			= 30;								// how fast the screen pan starts
		m_nExtraAmountOnPanBack		= 10;								// I like a faster pan back. Adding a bit more		
		m_fAdjustY					= 0;								// best to leave at 0 for iPad (moves the world down when panning)
							
		// background stuff
		m_pBackgroundLayerClouds->setPosition ( ccp ( m_tContentSize.cx, m_tContentSize.cy / 2 ) );
		m_pBackgroundLayerHills ->setPosition ( ccp ( m_tContentSize.cx, m_tContentSize.cy / 2 ) );

		if ( !IS_RETINA )
		{
			// non retina adjustment
		}
		else
		{
			// retina adjustment
			
			m_pBackgroundLayerClouds->setScale ( 2.0f );			
			m_pBackgroundLayerHills ->setScale ( 2.0f );
		}

		m_tMenuStartPosition			  = ccp ( 130, m_tContentSize.cy - 24 );
		m_tCurrentScoreLabelStartPosition = ccp ( 200, m_tContentSize.cy - 60 );	
		m_tHighScoreLabelStartPosition	  = ccp ( 200, m_tContentSize.cy - 80 );
		m_fFontSizeForScore = 22;
					
		// ground plane and platform				
		m_tGroundPlaneStartPosition = ccp ( m_tContentSize.cx, 50 );
		m_tPlatformStartPosition	= ccp ( 340, 190 );

		// sling shot
		m_tSlingShotCenterPosition	= ccp ( 370, 255 );

		m_pSlingShotFront->setPosition ( ccp ( 374, 240 ) );
		m_pStrapFront	 ->setPosition ( ccp ( m_tSlingShotCenterPosition.x, m_tSlingShotCenterPosition.y ) );
		m_pStrapBack	 ->setPosition ( ccp ( m_tSlingShotCenterPosition.x + 33, m_tSlingShotCenterPosition.y - 10 ) );
		m_pStrapEmpty	 ->setPosition ( ccp ( 378, 235 ) );

		// ninja	
		m_tNinjaStartPosition1 = ccp ( 280, 250 );  
		m_tNinjaStartPosition2 = ccp ( 300, 155 );
		m_tNinjaStartPosition3 = ccp ( 260, 155 );
		m_tNinjaStartPosition4 = ccp ( 200, 120 );
		m_tNinjaStartPosition5 = ccp ( 160, 120 );
	}
	else
	{
//		CCLOG ( "this is an iphone" );
		m_bAreWeOnTheIPad			= KD_FALSE;

		// vars 
		m_nMaxStretchOfSlingShot	= 75;								// best to leave as is, since this value ties in closely to the image size of strap.png. (should be 1/4 the size of the source image)
		m_fMultipyThrowPower		= 1.0f;								// fine tune how powerful the sling shot is. Range is probably best between .5 to 1.5, and a little goes a long way
		
		m_fWorldMaxHorizontalShift	= -m_tContentSize.cx;				// This determines how far the user can slide left or right to see the entire board. Always a negative number
		m_fMaxScaleDownValue		= 0.65f;							// range should probably be between 0.75 and 1.0;
		m_fScaleAmount				= 0.01f;							// increment to change the scale of the entire world when panning
		m_fAdjustY					= -34;
					
		m_nInitialPanAmount			= 20;								// how fast the screen pan starts
		m_nExtraAmountOnPanBack		= 0;								// best to leave at 0 on iPhone

		// background stuff
		if ( !IS_RETINA )
		{
			// non retina adjustment
			m_pBackgroundLayerClouds->setPosition ( ccp ( m_tContentSize.cx, 192 ) );
			m_pBackgroundLayerClouds->setScale ( 0.7f );

			m_pBackgroundLayerHills->setPosition ( ccp ( m_tContentSize.cx, 245 ) );
			m_pBackgroundLayerHills->setScale ( 0.7f );
		}
		else
		{
			// retina adjustment
			m_pBackgroundLayerClouds->setPosition ( ccp ( m_tContentSize.cx, 192 ) );
			m_pBackgroundLayerClouds->setScale ( 1.7f );

			m_pBackgroundLayerHills->setPosition ( ccp ( m_tContentSize.cx, 265 ) );
			m_pBackgroundLayerHills->setScale ( 1.7f );
		}

		m_tMenuStartPosition			  = ccp (  70, m_tContentSize.cy - 17 );
		m_tCurrentScoreLabelStartPosition = ccp ( 140, m_tContentSize.cy - 50 );	// score label
		m_tHighScoreLabelStartPosition	  = ccp ( 140, m_tContentSize.cy - 70 );
		m_fFontSizeForScore = 18;
					
		// ground plane and platform				
		m_tGroundPlaneStartPosition = ccp ( m_tContentSize.cx, -25 );
		m_tPlatformStartPosition	= ccp ( 130, 120 );

		// sling shot
		m_tSlingShotCenterPosition	= ccp ( 160, 185 );

		m_pSlingShotFront->setPosition ( ccp ( 164, 170 ) );
		m_pStrapFront	 ->setPosition ( ccp ( m_tSlingShotCenterPosition.x, m_tSlingShotCenterPosition.y ) );
		m_pStrapBack	 ->setPosition ( ccp ( m_tSlingShotCenterPosition.x + 33, m_tSlingShotCenterPosition.y - 10 ) );
		m_pStrapEmpty	 ->setPosition ( ccp ( 168, 163 ) );

		// ninja	
		m_tNinjaStartPosition1 = ccp ( 170, 175 );  
		m_tNinjaStartPosition2 = ccp ( 110,  82 );
		m_tNinjaStartPosition3 = ccp (  65,  82 );
		m_tNinjaStartPosition4 = ccp (  90,  65 );
		m_tNinjaStartPosition5 = ccp (  43,  65 );
	}
																		
	setUpParticleSystemSun ( );
	
	CCMenuItemImage*	pButton1 = CCMenuItemImage::create ( "gameMenu.png", "gameMenu.png", this, menu_selector ( TheLevel::showMenu ) );

	m_pMenuButton = CCMenu::createWithItem ( pButton1 );
	m_pMenuButton->setPosition ( m_tMenuStartPosition );
	this->addChild ( m_pMenuButton, DepthScore );
				
	// assign CCPoints to keep track of the starting positions of objects that move relative to the entire layer.			
	m_tHillsLayerStartPosition = m_pBackgroundLayerHills ->getPosition ( );
	m_tCloudLayerStartPosition = m_pBackgroundLayerClouds->getPosition ( ); 

	// Define the gravity vector.
    m_fAxisGravityY = -9.81f;
	m_bGravityOn	= KD_TRUE;

	b2Vec2		tGravity;
	tGravity.Set ( 0.0f, m_fAxisGravityY );
	
	// Construct a world object, which will hold and simulate the rigid bodies.
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( KD_FALSE );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );

//	EnableDebugMode ( );
			
	// Define the ground body.
	b2BodyDef		tGroundBodyDef;								// Make sure we call 
	tGroundBodyDef.position.Set ( 0, 0 );						// bottom-left corner
			
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body*			pGroundBody = m_pWorld->CreateBody ( &tGroundBodyDef );

	// Define the ground box shape.
	b2EdgeShape		tGroundBox;

	KDfloat			fWorldMaxWidth  = m_tContentSize.cx * 4;	// If you ever want the BOX2D world width to be more than it is then increase this  (currently, this should be plenty of extra space)
	KDfloat			fWorldMaxHeight = m_tContentSize.cy * 3;	// If you ever want the BOX2D world height to be more  than it is then increase this (currently, this should be plenty of extra space)

	// bottom
	tGroundBox.Set ( b2Vec2 ( -4, 0 ), b2Vec2 ( fWorldMaxWidth / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );

	// top
	tGroundBox.Set ( b2Vec2 ( -4, fWorldMaxHeight / PTM_RATIO ), b2Vec2 ( fWorldMaxWidth / PTM_RATIO, fWorldMaxHeight / PTM_RATIO ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );

	// left
	tGroundBox.Set ( b2Vec2 ( -4, fWorldMaxHeight / PTM_RATIO ), b2Vec2 ( -4, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );

	// right
	tGroundBox.Set ( b2Vec2 ( fWorldMaxWidth / PTM_RATIO, fWorldMaxHeight / PTM_RATIO ), b2Vec2 ( fWorldMaxWidth / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );

	// Contact listener 
	m_pContactListener = new ContactListener ( );
	m_pWorld->SetContactListener ( m_pContactListener );
	
	// Set up the ground plane		
	m_pTheGroundPlane = GroundPlane::create ( m_pWorld, m_tGroundPlaneStartPosition, pData->getGroundPlaneFileName ( ) );
	this->addChild ( m_pTheGroundPlane, DepthFloor );
	
	// Set up the starting platform			
	m_pThePlatform = StartPlatform::create ( m_pWorld, m_tPlatformStartPosition, "platform.png" ); 
	this->addChild ( m_pThePlatform, DepthPlatform );

	// Set up ninjas			
	m_nNinjaBeingThrown = 1;												// always starts at 1 (first ninja, then second ninja, and so on) 
	m_nNinjasToTossThisLevel = pData->getNumberOfNinjasToTossThisLevel ( );	// total number of ninjas to toss for this level			
			
	m_pNinja1 = Ninja::create ( m_pWorld, m_tNinjaStartPosition1, "ninja" );
	this->addChild ( m_pNinja1, DepthNinjas );
			
	m_pCurrentBodyNode = m_pNinja1;

	m_pCurrentBodyNode->spriteInSlingState ( );

	if ( m_nNinjasToTossThisLevel >= 2 )
	{		
		m_pNinja2 = Ninja::create ( m_pWorld, m_tNinjaStartPosition2, "ninjaRed" );
		this->addChild ( m_pNinja2, DepthNinjas );
		m_pNinja2->spriteInStandingState ( );
	}
	if ( m_nNinjasToTossThisLevel >= 3 )
	{		
		m_pNinja3 = Ninja::create ( m_pWorld, m_tNinjaStartPosition3, "ninjaBlue" );
		this->addChild ( m_pNinja3, DepthNinjas );
		m_pNinja3->spriteInStandingState ( );
	}
	if ( m_nNinjasToTossThisLevel >= 4 )
	{		
		m_pNinja4 = Ninja::create ( m_pWorld, m_tNinjaStartPosition4, "ninjaBrown" );
		this->addChild ( m_pNinja4, DepthNinjas );
		m_pNinja4->spriteInStandingState ( );
	}
	if ( m_nNinjasToTossThisLevel >= 5 )
	{		
		m_pNinja5 = Ninja::create ( m_pWorld, m_tNinjaStartPosition5, "ninjaGreen" );
		this->addChild ( m_pNinja5, DepthNinjas );
		m_pNinja5->spriteInStandingState ( );
	}

	// Build the Stack. 			
	m_pStack = TheStack::create ( m_pWorld );
	this->addChild ( m_pStack, DepthStack );
			
	// give the stack a moment to drop, then switches every pieces to static (locks it into position, until the first slingshot)...
	this->scheduleOnce ( schedule_selector ( TheLevel::switchAllStackObjectsToStatic ), 1.0f );

	m_pCurrentScoreLabel = CCLabelTTF::create ( ccszf ( "%d: Needed", m_nPointsToPassLevel ), "Marker Felt.ttf", m_fFontSizeForScore );
	this->addChild ( m_pCurrentScoreLabel, DepthScore );
	m_pCurrentScoreLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pCurrentScoreLabel->setPosition ( m_tCurrentScoreLabelStartPosition );
	m_pCurrentScoreLabel->setAnchorPoint ( ccp ( 1, 0.5f ) );

	// HighScoreForLevel
	m_pHighScoreLabel = CCLabelTTF::create ( ccszf ( "High Score: %d", pData->getHighScoreForLevel ( ) ), "Marker Felt.ttf", m_fFontSizeForScore );
	this->addChild ( m_pHighScoreLabel, DepthScore );
	m_pHighScoreLabel->setColor ( ccc3 ( 255, 255, 255 ) );

    m_pHighScoreLabel->setPosition ( ccpSub ( m_pCurrentScoreLabel->getPosition ( ), ccp ( 0, m_pHighScoreLabel->getContentSize ( ).cy ) ) );// highScoreLabelStartPosition;
	m_pHighScoreLabel->setAnchorPoint ( ccp ( 1, 0.5f ) );
    m_tHighScoreLabelStartPosition = m_pHighScoreLabel->getPosition ( );

	this->showBoardMessage ( ccszf ( "Level: %d", m_nCurrentLevel ) );

	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( "/////////////////////////////////////////////////////" );
	CCLOG ( "/////////////////////////////////////////////////////" );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( "The art and animation in this template is copyright CartoonSmart LLC" );
	CCLOG ( "You must make significant changes to the art before submitting your game to the App Store" );
	CCLOG ( "Please create your own characters, backgrounds, etc and spend the time to make the game look totally unique" );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( "The Video guide for this template is at https://vimeo.com/cartoonsmart/angryninjasguide  " );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( " " );
	CCLOG ( "/////////////////////////////////////////////////////" );
	CCLOG ( "/////////////////////////////////////////////////////" );
	CCLOG ( " " );
	CCLOG ( " " );

	GameSounds::sharedGameSounds ( )->introTag ( );

	if ( pData->getLevel ( ) == 1 ) 
	{		
		GameSounds::sharedGameSounds ( )->playBackgroundMusic ( Frogs );		
	}
	else 
	{	
		GameSounds::sharedGameSounds ( )->playBackgroundMusic ( Insects ); 
	}

	if ( pData->isFirstRunEver ( ) && m_bOpenWithMenuInsteadOfGame )
	{
		CCLOG ( "First run ever" );
		this->schedule ( schedule_selector ( TheLevel::showMenuFromSelector ), 2.f );
		pData->setFirstRunEver ( KD_FALSE );
	}

	// Always do this last.
	this->schedule ( schedule_selector ( TheLevel::tick ) );
		
	return KD_TRUE;
}

KDvoid TheLevel::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid TheLevel::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator pIter = pTouches->begin ( ); pIter != pTouches->end ( ); pIter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *pIter );
		CCPoint		tLocation = pTouch->getLocation ( );

		m_fPreviousTouchLocationX = tLocation.x;

		if ( !m_bThrowInProgress )
		{
			m_pCurrentBodyNode->getBody ( )->SetType ( b2_staticBody );			
		}

		if ( m_bTopLeftTouchEnablesDebugMode && tLocation.x > m_tContentSize.cx * 0.1f && tLocation.y > m_tContentSize.cy * 0.9f ) 
		{
			CCLOG ( "touching upper left" );
			EnableDebugMode ( );
		} 
	}
}

KDvoid TheLevel::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_bAutoPanningInProgress )
	{
		this->cancelAutoPan ( );
	}

	for ( CCSetIterator pIter = pTouches->begin ( ); pIter != pTouches->end ( ); pIter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *pIter );
		CCPoint		tLocation = pTouch->getLocation ( );

		if ( !cockTheSlingshot ( tLocation ) )
		{
			KDint	nDiff = (KDint) ( tLocation.x - m_fPreviousTouchLocationX );	// difference between the starting/previous touch location and current one
			this->panTheScreen ( nDiff );
		}

		m_fPreviousTouchLocationX = tLocation.x;
	}
}

KDvoid TheLevel::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator pIter = pTouches->begin ( ); pIter != pTouches->end ( ); pIter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *pIter );
		CCPoint		tLocation = pTouch->getLocation ( );

		if ( m_bSlingShotNinjaInHand )
		{
			b2Vec2		tDirection = b2Vec2 ( m_tSlingShotCenterPosition.x - tLocation.x, m_tSlingShotCenterPosition.y - tLocation.y );
			tDirection.Normalize ( );
			fireSlignshot ( tDirection );
		}
		else if ( m_bContinuePanningScreenOnFingerRelease )
		{
			if ( m_bPanningTowardSling )
			{
				this->startScreenPanToSling ( );
			}
			else
			{
				this->startScreenPanToTargets ( );
			}
		}
	}
}

KDvoid TheLevel::showMenuFromSelector ( KDfloat fDelta )
{
	this->showMenu ( KD_NULL );
}

KDvoid TheLevel::showMenu ( CCObject* pObejct )
{
	CCLOG ( "Show Menu" );

	CCDirector::sharedDirector ( )->pushScene ( TheMenu::scene ( ) );
}

KDvoid TheLevel::switchAllStackObjectsToStatic ( KDfloat fDelta )
{
	m_bStackIsNowDynamic = KD_FALSE;
	
	// Iterate over the bodies in the physics world
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{		
		StackObject*	pActor = dynamic_cast<StackObject*> ( (CCNode*) pBody->GetUserData ( ) );
		if ( pActor != KD_NULL )
		{
			pActor->getBody ( )->SetType ( b2_staticBody );
		}	
	}
}

KDvoid TheLevel::switchAllStackObjectsToDynamic ( KDfloat fDelta )
{
	if ( !m_bStackIsNowDynamic ) 
	{
		m_bStackIsNowDynamic = KD_TRUE;
		
		// Iterate over the bodies in the physics world
		for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
		{
			StackObject*	pActor = dynamic_cast<StackObject*> ( (CCNode*) pBody->GetUserData ( ) );
			if ( pActor != KD_NULL )
			{
				if ( !pActor->isStatic ( ) )
				{
					pActor->getBody ( )->SetType  ( b2_dynamicBody );
					pActor->getBody ( )->SetAwake ( KD_TRUE );
				}												
			}	
		}
	}
}

KDvoid TheLevel::EnableDebugMode ( KDvoid )
{
	CCBox2DDebugDraw*	pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( pDebugDraw );

	KDuint32	uFlags = 0;
	
	uFlags += b2Draw::e_shapeBit;
//	uFlags += b2Draw::e_jointBit;
//	uFlags += b2Draw::e_aabbBit;
//	uFlags += b2Draw::e_pairBit;
//	uFlags += b2Draw::e_centerOfMassBit;

	pDebugDraw->SetFlags ( uFlags );		
}

KDvoid TheLevel::setUpParticleSystemSun ( KDvoid )
{
	m_pSystem = CCParticleSun::create ( );
	this->addChild ( m_pSystem, DepthParticles );
	m_pSystem->setScale ( 3 );
	m_pSystem->setPosition ( 240, 400 );
	
	m_tParticleSystemStartPosition = m_pSystem->getPosition ( );
}

KDvoid TheLevel::tick ( KDfloat fDelta )
{
	KDint	nVelocityIterations = 8;
	KDint	nPositionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );

	// Iterate over the bodies in the physics world
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		CCNode*		pActor = (CCNode*) pBody->GetUserData ( );
		if ( pActor != KD_NULL )
		{
			b2Vec2		tBodyPos = pBody->GetPosition ( );
			// Synchronize the AtlasSprites position and rotation with the corresponding body		              
			pActor->setPosition ( ccp ( tBodyPos.x * PTM_RATIO, tBodyPos.y * PTM_RATIO ) );
			pActor->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );  
		}	
	}
}

KDvoid TheLevel::proceedToNextTurn ( Ninja* pTheNinja )
{
	// if ninja hit the ground			
	if ( pTheNinja == m_pCurrentBodyNode )
	{		
//		CCLOG ( "the current ninja has hit the ground" );
		
		this->unschedule ( schedule_selector ( TheLevel::timerAfterThrow ) );				// unschedule this since the ninja has hit the ground we won't need it
		this->  schedule ( schedule_selector ( TheLevel::moveNextNinjaIntoSling ), 1.0f );			
	}
	else 
	{		
//		CCLOG ( "disregard: some other ninja not in play has hit the ground somehow" );		
	}
}

KDvoid TheLevel::moveNextNinjaIntoSling ( KDfloat fDelta )
{
	if ( !m_bSomethingJustScored )
	{
		// dont move a ninja unless we are done scoring	
		this->unschedule ( schedule_selector ( TheLevel::moveNextNinjaIntoSling ) );
		
		m_nNinjaBeingThrown++;
		
		if ( m_nNinjaBeingThrown <= m_nNinjasToTossThisLevel && m_nPointTotalThisRound < m_nPointsToPassLevel ) 
		{			
			switch ( m_nNinjaBeingThrown )
			{ 
				case 2 :	m_pCurrentBodyNode = m_pNinja2;		break;	
				case 3 :	m_pCurrentBodyNode = m_pNinja3;		break;	
				case 4 :	m_pCurrentBodyNode = m_pNinja4;		break;	
				case 5 :	m_pCurrentBodyNode = m_pNinja5;		break;				
			}

			b2Vec2		tLocationInMeters;
			tLocationInMeters.Set ( m_tNinjaStartPosition1.x / PTM_RATIO, m_tNinjaStartPosition1.y / PTM_RATIO );
			m_pCurrentBodyNode->getBody ( )->SetTransform ( tLocationInMeters, CC_DEGREES_TO_RADIANS ( 0 ) );			
			m_pCurrentBodyNode->spriteInSlingState ( );
			
			m_bThrowInProgress = KD_FALSE;			
		}  
		else if ( m_nNinjaBeingThrown > m_nNinjasToTossThisLevel || m_nPointTotalThisRound >= m_nPointsToPassLevel )
		{ 					
			this->scheduleOnce ( schedule_selector ( TheLevel::resetOrAdvanceLevel ), 2.0f );
		}		
	}
}

KDvoid TheLevel::stopDotting ( KDvoid )
{
	m_bDottingOn = KD_FALSE;
}

KDvoid TheLevel::showNinjaImpactingStack ( Ninja* pTheNinja )
{
	if ( pTheNinja == m_pCurrentBodyNode )
	{
		// make sure the currentBodyNode is the same ninja that hit the stack		
		m_pCurrentBodyNode->spriteInRollStateWithAnimationFirst ( );			// if you have animated frames you want to include use this
		
		// do anything you want when the ninja hits any stack object
		// maybe you want extra audio		
	}
}

KDvoid TheLevel::showNinjaOnGround ( Ninja* pTheNinja )
{
	if ( pTheNinja == m_pCurrentBodyNode )
	{
		// make sure the currentBodyNode is the same ninja that hit the ground		
		this->schedule ( schedule_selector ( TheLevel::makeNinjaStaticOnGround ), 1.0f / 60.0f );
		
		// do anything else you want when the ninja hits the ground
		m_pCurrentBodyNode->scheduleOnce ( schedule_selector ( BodyNode::fadeThenRemove ), 2.0f );      // I like to fade out the sprite after a couple seconds
	}
}

KDvoid TheLevel::makeNinjaStaticOnGround ( KDfloat fDelta )
{
	// optionally you can make the ninja a static body when it hits the ground (keeps it from rolling)
	m_pCurrentBodyNode->spriteInGroundState ( );

	b2Body*		pBody = m_pCurrentBodyNode->getBody ( );
	pBody->SetType ( b2_staticBody );
	pBody->SetTransform ( pBody->GetPosition ( ), CC_DEGREES_TO_RADIANS ( 0 ) );

	this->unschedule ( schedule_selector ( TheLevel::makeNinjaStaticOnGround ) );
}

KDvoid TheLevel::moveScreen ( KDint nAmountToShiftScreen )
{
	if ( nAmountToShiftScreen == 0 )
	{
		return;
	}

	CCPoint		tPoint = this->getPosition ( );

	this->setPosition ( ccp ( tPoint.x - nAmountToShiftScreen, tPoint.y ) );

	if ( m_bAreWeOnTheIPad )	// the label seems to stay in one place, but really the entire layer is moving (only do this on iPad)
	{		
		m_pMenuButton		->setPositionX ( m_pMenuButton		 ->getPositionX ( ) + nAmountToShiftScreen );		
		m_pCurrentScoreLabel->setPositionX ( m_pCurrentScoreLabel->getPositionX ( ) + nAmountToShiftScreen );
		m_pHighScoreLabel	->setPositionX ( m_pHighScoreLabel	 ->getPositionX ( ) + nAmountToShiftScreen );
	}

	// the clouds / particles will seem like they wont move as much as the entire layer
	m_pBackgroundLayerHills ->setPositionX ( m_pBackgroundLayerHills ->getPositionX ( ) + ( nAmountToShiftScreen * 0.5f  ) );
	m_pBackgroundLayerClouds->setPositionX ( m_pBackgroundLayerClouds->getPositionX ( ) + ( nAmountToShiftScreen * 0.75f ) ); 

	m_pSystem->setPositionX ( m_pSystem->getPositionX ( ) + ( nAmountToShiftScreen * 0.75f ) );

	// deal with scaling and y positions...
	KDfloat		fScale = this->getScale ( );
	if ( nAmountToShiftScreen > 0 )
	{
		// scaling down		
		if ( fScale > m_fMaxScaleDownValue )
		{
			this->setScale ( fScale - m_fScaleAmount );	
		}						
	}
	else 
	{ 
		// or scaling up		
		if ( fScale < 1 )
		{			
			this->setScale ( fScale + m_fScaleAmount );		
		}		
	}
}

KDvoid TheLevel::putEverythingInStartingViewOfSlingShot ( KDvoid )
{
	this->setPosition ( ccpz ); 

	m_pMenuButton			->setPosition ( m_tMenuStartPosition );
	m_pCurrentScoreLabel	->setPosition ( m_tCurrentScoreLabelStartPosition );
	m_pHighScoreLabel		->setPosition ( m_tHighScoreLabelStartPosition );
	m_pSystem				->setPosition ( m_tParticleSystemStartPosition );
	m_pBackgroundLayerClouds->setPosition ( m_tCloudLayerStartPosition );
	m_pBackgroundLayerHills	->setPosition ( m_tHillsLayerStartPosition );

	this->setScale ( 1 ); 

	m_bAreWeInTheStartingPosition = KD_TRUE;
}

KDvoid TheLevel::putEverythingInViewOfTargets ( KDvoid )
{
	this->setPositionX ( m_fWorldMaxHorizontalShift );
	this->setPositionY ( m_fAdjustY );

	if ( m_bAreWeOnTheIPad ) 
	{  
		// I'm only keeping these labels in place on the iPad because it doesn't scale up or down 		
		m_pMenuButton		->setPositionX ( m_tMenuStartPosition.x				 - m_fWorldMaxHorizontalShift );
		m_pCurrentScoreLabel->setPositionX ( m_tCurrentScoreLabelStartPosition.x - m_fWorldMaxHorizontalShift );
		m_pHighScoreLabel	->setPositionX ( m_tHighScoreLabelStartPosition.x	 - m_fWorldMaxHorizontalShift );		
	}

	m_pBackgroundLayerHills ->setPositionX ( m_tHillsLayerStartPosition.x - ( m_fWorldMaxHorizontalShift * .5f  ) );
	m_pBackgroundLayerClouds->setPositionX ( m_tCloudLayerStartPosition.x - ( m_fWorldMaxHorizontalShift * .75f ) );

	m_pSystem->setPositionX ( m_tParticleSystemStartPosition.x - ( m_fWorldMaxHorizontalShift * .75f ) );

	if ( this->getScale ( ) < m_fMaxScaleDownValue )
	{		
		this->setScale ( m_fMaxScaleDownValue );		
	}

	m_bAreWeInTheStartingPosition = KD_FALSE;			
}

KDbool TheLevel::cockTheSlingshot ( const CCPoint& tPoint )
{
    if ( ( this->checkCircleCollision ( tPoint, 2, m_tSlingShotCenterPosition, (KDfloat) m_nMaxStretchOfSlingShot ) || m_bSlingShotNinjaInHand )
        && !m_bThrowInProgress && m_bAreWeInTheStartingPosition )
    {
        if ( !m_bSlingShotNinjaInHand )
        {
            m_tPositionInSling = m_tSlingShotCenterPosition;
            m_bSlingShotNinjaInHand = KD_TRUE;

            m_pStrapBack ->setVisible ( KD_TRUE );
            m_pStrapFront->setVisible ( KD_TRUE );
            m_pStrapEmpty->setVisible ( KD_FALSE );
        }

        KDfloat		fCurrentAngle = m_pCurrentBodyNode->getBody ( )->GetAngle ( );
        KDfloat		fRadius = (KDfloat) m_nMaxStretchOfSlingShot;																				// radius of slingShot
        KDfloat		fAngle  = this->calculateAngle ( tPoint.x, tPoint.y, m_tSlingShotCenterPosition.x, m_tSlingShotCenterPosition.y );	// angle from slingShot center to the location of the touch

        // if the user is moving the ninja within the max stretch of the slingShot  (the radius)
        if ( this->checkCircleCollision ( tPoint, 2, m_tSlingShotCenterPosition, fRadius ) )
        {
            m_tPositionInSling.x = tPoint.x;
            m_tPositionInSling.y = tPoint.y;

            // tie the strap size into the location of the touch in relation to the distance from the slingshot center
            KDfloat		fScaleStrap = kdFabsf ( m_tSlingShotCenterPosition.x - tPoint.x ) / fRadius;

            fScaleStrap = fScaleStrap + 0.3f;			// add a little extra

            if ( fScaleStrap > 1 )
            { 
				// make sure it doesn't go over 100% scale
                fScaleStrap = 1;
            }

            m_pStrapFront->setScaleX ( fScaleStrap );
            m_pStrapBack ->setScaleX ( fScaleStrap );	// strap back is the same size as the strap front (until we rework it a tad below)
        }
        else
        {			
            // if the user is moving the ninja outside the max stretch of the slingShot
            KDfloat		fAngleRadians = CC_DEGREES_TO_RADIANS ( fAngle - 90 );
            m_tPositionInSling.x = m_tSlingShotCenterPosition.x - kdCosf ( fAngleRadians ) * fRadius;
            m_tPositionInSling.y = m_tSlingShotCenterPosition.y + kdSinf ( fAngleRadians ) * fRadius;

            m_pStrapFront->setScaleX ( 1 );
			m_pStrapBack ->setScaleX ( 1 );
        }
	
        m_pStrapFront->setRotation ( fAngle - 90 );
        this->adjustBackStrap ( fAngle );

        // positions the box2D bodyNode of the ninja
        b2Vec2		tLocationInMeters = b2Vec2 ( m_tPositionInSling.x / PTM_RATIO, m_tPositionInSling.y / PTM_RATIO );

        m_pCurrentBodyNode->getBody ( )->SetTransform ( tLocationInMeters, CC_DEGREES_TO_RADIANS ( fCurrentAngle ) );
        m_pCurrentBodyNode->spriteInPulledBackSlingState ( );
		
        return KD_TRUE;		
    }
	
    // Did not consume this point, so let it pass to the next consumer
    return KD_FALSE;
}

KDvoid TheLevel::panTheScreen ( KDint nDiff )
{
	KDint			nAmountToShiftScreen;
	nAmountToShiftScreen = this->returnAmountToShiftScreen ( nDiff );	// just a method to prevent the screen from moving too wildly.

	// pan the screen back and forth if there isn't a ninja in hand
	const CCPoint&	tPoint = this->getPosition ( );

	if ( tPoint.x <= 0 && tPoint.x >= m_fWorldMaxHorizontalShift && !m_bSlingShotNinjaInHand )
	{
		m_bAreWeInTheStartingPosition = KD_FALSE;

		this->moveScreen ( nAmountToShiftScreen );

		if ( tPoint.x > 0 )
		{ 
			// if we try to shift too far left, we reset to the starting position
			this->putEverythingInStartingViewOfSlingShot ( );
		}
		else if ( tPoint.x < m_fWorldMaxHorizontalShift )
		{ 
			// if we try to shift too far the other way, we keep everything at the max position
			this->putEverythingInViewOfTargets ( );
		}
	}
}

KDbool TheLevel::checkCircleCollision ( const CCPoint& tCenter1, KDfloat fRadius1, const CCPoint& tCenter2, KDfloat fRadius2 )
{
	KDfloat		a = tCenter2.x - tCenter1.x;
	KDfloat		b = tCenter2.y - tCenter1.y;
	KDfloat		c = fRadius1 + fRadius2;
	KDfloat		fDistanceSqrd = ( a * a ) + ( b * b );

	if ( fDistanceSqrd < ( c * c ) )
	{		
		return KD_TRUE;
	}
	else 
	{
		return KD_FALSE;
	}
}

KDfloat TheLevel::calculateAngle ( KDfloat x1, KDfloat y1, KDfloat x2, KDfloat y2 )
{
	// DX
	KDfloat		x = x2 - x1;

	// DY
	KDfloat		y = y2 - y1;


	KDfloat		fAngle = 180 + kdAtan2f ( -x, -y ) * ( 180 / KD_PI_F );

	return fAngle;		// degrees
}

KDint TheLevel::returnAmountToShiftScreen ( KDint nDiff )
{
	KDint	nAmountToShiftScreen;

	if ( nDiff > 50 ) 
	{	
		nAmountToShiftScreen = 50;		
	}
	else if ( nDiff < -50 )
	{		
		nAmountToShiftScreen = -50;		
	}
	else 
	{		
		nAmountToShiftScreen = nDiff;
	}

	if ( !m_bReverseHowFingerPansScreen )
	{		
		nAmountToShiftScreen = nAmountToShiftScreen * -1;				
	} 

	if ( nAmountToShiftScreen < 0 )
	{		
		m_bPanningTowardSling = KD_TRUE;		
	}
	else 
	{		
		m_bPanningTowardSling = KD_FALSE;
	}

	return nAmountToShiftScreen;  
}

KDvoid TheLevel::fireSlignshot ( const b2Vec2& tDir )
{
	GameSounds::sharedGameSounds ( )->releaseSlingSounds ( );

	switchAllStackObjectsToDynamic ( 0 );

	m_nThrowCount++;
	m_nDotCount = 0;

	m_bThrowInProgress = KD_TRUE;

	b2Body*		pCurBody = m_pCurrentBodyNode->getBody ( );
	pCurBody->SetType ( b2_dynamicBody );
	// SetAwake is set to true when changed to dynamicBody
//	pCurBody->SetAwake ( KD_FALSE );
//	pCurBody->SetActive ( KD_TRUE );

	m_pStrapBack ->setVisible ( KD_FALSE );
	m_pStrapFront->setVisible ( KD_FALSE );
	m_pStrapEmpty->setVisible ( KD_TRUE );

	// This determines the speed variance
	m_fSpeed = kdFabsf ( m_tSlingShotCenterPosition.x - m_tPositionInSling.x ) + kdFabsf ( m_tSlingShotCenterPosition.y - m_tPositionInSling.y );
	m_fSpeed = m_fSpeed / 5;
	m_fSpeed = m_fSpeed * m_fMultipyThrowPower;

	// targetPosition is actually touch point
//	b2Vec2		tTargetInWorld = b2Vec2 ( location.X, location.Y );

	// This determines the actual direction of the ninja, from slingshot center position to touch point

	// This moves the body, and the key part is multiplication
	// of 'speed' variable with direction. .
	pCurBody->SetLinearVelocity ( m_fSpeed * tDir );

	m_nInitialPanAmount = (KDint) ( pCurBody->GetLinearVelocity ( ).x * 1.25f );

	m_bSlingShotNinjaInHand = KD_FALSE;

	m_pCurrentBodyNode->spriteInAirState ( );

	// add dots following throw...
	this->removePreviousDots ( );

	m_bDottingOn = KD_TRUE;																// dotting in progress

	this->schedule ( schedule_selector ( TheLevel::placeWhiteDots ), 1.0f / 45.0f );	// increase or decrease frequency of dots with the interval

	// ensures throwInProgress is set to NO after 6 seconds
	this->unschedule ( schedule_selector ( TheLevel::timerAfterThrow ) );
	this->  schedule ( schedule_selector ( TheLevel::timerAfterThrow ), 6.0f );

	if ( tDir.x > 0.0f )																// fire direction is forward
	{
		this->startScreenPanToTargetsWithAutoReverseOn ( );
	}
}

KDvoid TheLevel::removePreviousDots ( KDvoid )
{
	KDint		m_nSomeNum = 0;

	if ( m_nThrowCount % 2 == 0 )
	{
		// odd numbered turn..						
		while ( m_nSomeNum <= m_nDotTotalOnOddNumberedTurn ) 
		{			
			this->removeChildByTag ( TagForWhiteDotsOddNumberedTurn + m_nSomeNum, KD_FALSE );
			m_nSomeNum++;						
		}
		
		m_nDotTotalOnOddNumberedTurn = 0;
		
	} 
	else 
	{
		// even numbered turn..					
		while ( m_nSomeNum <= m_nDotTotalOnEvenNumberedTurn ) 
		{			
			this->removeChildByTag ( TagForWhiteDotsEvenNumberedTurn + m_nSomeNum, KD_FALSE );
			m_nSomeNum++;						
		}
		
		m_nDotTotalOnEvenNumberedTurn = 0;
	}					
}

KDvoid TheLevel::removeAllDots ( KDvoid )
{
	// runs when the board is cleaned up...

	KDint		m_nSomeNum = 0;

	while ( m_nSomeNum <= m_nDotTotalOnOddNumberedTurn )
	{		
		this->removeChildByTag ( TagForWhiteDotsOddNumberedTurn + m_nSomeNum, KD_FALSE );
		m_nSomeNum++;				
	}

	m_nDotTotalOnOddNumberedTurn = 0;
	m_nSomeNum = 0;

	while ( m_nSomeNum <= m_nDotTotalOnEvenNumberedTurn ) 
	{			
		this->removeChildByTag ( TagForWhiteDotsEvenNumberedTurn + m_nSomeNum, KD_FALSE );
		m_nSomeNum++;						
	}

	m_nDotTotalOnEvenNumberedTurn = 0;
}

KDvoid TheLevel::placeWhiteDots ( KDfloat fDelta )
{
	if ( m_bDottingOn ) 
	{		
		m_nDotCount++;
		
		CCSprite*	pWhiteDot = CCSprite::create ( "circle.png" );
		
		if ( m_nThrowCount % 2 != 0 )
		{
			// odd number..			
			this->addChild ( pWhiteDot, DepthWhiteDots, TagForWhiteDotsOddNumberedTurn + m_nDotCount );
			m_nDotTotalOnOddNumberedTurn = m_nDotCount;			
		}
		else 
		{			
			this->addChild ( pWhiteDot, DepthWhiteDots, TagForWhiteDotsEvenNumberedTurn + m_nDotCount );
			m_nDotTotalOnOddNumberedTurn = m_nDotCount;
		}
				
		pWhiteDot->setPosition ( m_pCurrentBodyNode->getPosition ( ) ); 
		
		if ( m_nDotCount % 2 != 0 )
		{
			// odd number..			
			pWhiteDot->setScale ( 0.5f );			
		} 		
	}
	else 
	{		
		this->unschedule ( schedule_selector ( TheLevel::placeWhiteDots ) );	
	}
}

KDvoid TheLevel::timerAfterThrow ( KDfloat fDelta )
{
	// this method will get cancelled if the ninja hits the ground. Which in most cases will happen. 
	// BUT if the ninja were to get stuck on an stack object and never hit the ground, this method will get called
	
	this->proceedToNextTurn ( m_pCurrentBodyNode );
	
	this->unschedule ( schedule_selector ( TheLevel::timerAfterThrow ) );
}

KDvoid TheLevel::startScreenPanToTargets ( KDvoid )
{
	m_fPanAmount			 = (KDfloat) m_nInitialPanAmount;

	m_bAutoPanningInProgress = KD_TRUE;
	m_bAutoReverseOn		 = KD_FALSE;
	m_bPanningTowardSling	 = KD_FALSE;

	this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToSling   ) );
	this->  schedule ( schedule_selector ( TheLevel::autoScreenPanToTargets ), 1.0f / 60.0f );

	if ( !m_bAreWeOnTheIPad ) 
	{	
		this->unschedule ( schedule_selector ( TheLevel::moveScreenUp   ) );
		this->  schedule ( schedule_selector ( TheLevel::moveScreenDown ), 1.0f / 60.0f );
	}	
}

KDvoid TheLevel::startScreenPanToTargetsWithAutoReverseOn ( KDvoid )
{
	m_fPanAmount			 = (KDfloat) m_nInitialPanAmount;

	m_bAutoPanningInProgress = KD_TRUE;
	m_bAutoReverseOn		 = KD_TRUE;
	m_bPanningTowardSling	 = KD_FALSE;

	this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToSling   ) );
	this->  schedule ( schedule_selector ( TheLevel::autoScreenPanToTargets ), 1.0f / 60.0f );

	if ( !m_bAreWeOnTheIPad  ) 
	{
		this->unschedule ( schedule_selector ( TheLevel::moveScreenUp   ) );
		this->  schedule ( schedule_selector ( TheLevel::moveScreenDown ), 1.0f / 60.0f );
	}
}

KDvoid TheLevel::startScreenPanToSlingIfScoringIsNotOccuring ( KDfloat fDelta )
{
	if ( !m_bSomethingJustScored )
	{		
		CCLOG ( "scoring done, auto move back" );
		
		this->startScreenPanToSling ( );
		this->unschedule ( schedule_selector ( TheLevel::startScreenPanToSlingIfScoringIsNotOccuring ) );
	} 
	else 
	{	
		CCLOG ( "something just scored, wait a bit longer to move screen back" );		
	}
}

KDvoid TheLevel::startScreenPanToSling ( KDvoid )
{
	m_fPanAmount = (KDfloat) ( m_nInitialPanAmount + m_nExtraAmountOnPanBack );

	m_bAutoPanningInProgress = KD_TRUE;
	m_bPanningTowardSling = KD_TRUE;

	this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToTargets ) ); 
	this->  schedule ( schedule_selector ( TheLevel::autoScreenPanToSling   ), 1.0f / 60.0f );

	if ( !m_bAreWeOnTheIPad )
	{		
		this->unschedule ( schedule_selector ( TheLevel::moveScreenDown ) );
		this->  schedule ( schedule_selector ( TheLevel::moveScreenUp   ), 1.0f / 60.0f );
	}	
}

KDvoid TheLevel::autoScreenPanToTargets ( KDfloat fDelta )
{
	if ( m_fPanAmount > 3 ) 
	{		
		m_fPanAmount = m_fPanAmount - 0.5f;		
	}

	KDfloat		fPosX = this->getPositionX ( );

	if ( fPosX > m_fWorldMaxHorizontalShift ) 
	{				
		if ( fPosX > m_fWorldMaxHorizontalShift && fPosX < m_fWorldMaxHorizontalShift + 50 )
		{
			// slows down panning when close to finishing			
			this->moveScreen ( 3 ); 			
		}
		else 
		{			
			this->moveScreen ( (KDint) m_fPanAmount ); 
		}
	}
	else 
	{	
		this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToTargets ) );
		this->putEverythingInViewOfTargets ( );
		
		if ( m_bAutoReverseOn ) 
		{			
			this->schedule ( schedule_selector ( TheLevel::startScreenPanToSlingIfScoringIsNotOccuring ), 2.0f );
		}
	}
}

KDvoid TheLevel::autoScreenPanToSling ( KDfloat fDelta )
{
	if ( m_fPanAmount > 3  ) 
	{
		m_fPanAmount = m_fPanAmount - 0.5f;
	}

	KDfloat		fPosX = this->getPositionX ( );

	if ( fPosX < 0 )
	{
		if ( fPosX < 0 && fPosX > -50 ) 
		{
			// slows down panning when close to finishing		
			this->moveScreen ( -3 ); 		
		}
		else 
		{		
			this->moveScreen ( (KDint) m_fPanAmount * -1 ); 
		}
	}
	else 
	{
		this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToSling ) );
		this->putEverythingInStartingViewOfSlingShot ( );

		m_bAutoPanningInProgress = KD_FALSE;
		this->setScale ( 1 );
	}
}

KDvoid TheLevel::cancelAutoPan ( KDvoid )
{
	m_bAutoPanningInProgress = KD_FALSE;
	this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToSling ) );
	this->unschedule ( schedule_selector ( TheLevel::autoScreenPanToTargets ) );
	this->unschedule ( schedule_selector ( TheLevel::startScreenPanToSlingIfScoringIsNotOccuring ) );
}

KDvoid TheLevel::moveScreenUp ( KDfloat fDelta )
{
	KDfloat		fPosY = this->getPositionY ( );

	if ( fPosY < 0 ) 
	{
		this->setPositionY ( fPosY + 2 );
	}
	else 
	{
		this->setPositionY ( 0 );
		this->unschedule ( schedule_selector ( TheLevel::moveScreenUp ) );
	}
}

KDvoid TheLevel::moveScreenDown ( KDfloat fDelta )
{
	KDfloat		fPosY = this->getPositionY ( );

	if ( fPosY > m_fAdjustY ) 
	{
		this->setPositionY ( fPosY - 2 );
	}
	else 
	{
		this->setPositionY ( m_fAdjustY );
		this->unschedule ( schedule_selector ( TheLevel::moveScreenDown ) );
	}
}

KDvoid TheLevel::showSimpleVisualFX ( const CCPoint& tPositionToShowScore, BreakEffect eTheSimpleScoreVisualFX )
{
	if ( eTheSimpleScoreVisualFX == SmokePuffs ) 
	{	
		GameSounds::sharedGameSounds ( )->playBreakSound ( );
		
		CCLOG ( "Play Smoke Puffs on Score" );
		
		CustomAnimation*	pSmokeFX = CustomAnimation::create 
		(
			"puffs",
			1,
			7,
			(KDint) tPositionToShowScore.x,
			(KDint) tPositionToShowScore.y,
			KD_FALSE,
			KD_FALSE,
			KD_FALSE,
			KD_FALSE
		);
		this->addChild ( pSmokeFX, DepthVisualFx );	
	} 
	else if ( eTheSimpleScoreVisualFX == Explosion ) 
	{		
		GameSounds::sharedGameSounds ( )->playBreakSound ( );
		
		CCLOG ( "Play explosion on Score" );
		
		CustomAnimation*	pSmokeFX = CustomAnimation::create 
		(
			"explosion",
			1,
			11,
			(KDint) tPositionToShowScore.x,
			(KDint) tPositionToShowScore.y,
			KD_FALSE,
			KD_FALSE,
			KD_FALSE,
			KD_FALSE
		);
		this->addChild ( pSmokeFX, DepthVisualFx );	
	}    
}

KDvoid TheLevel::showPoints ( KDint nPointValue, const CCPoint& tPositionToShowScore, BreakEffect eTheSimpleScoreVisualFX )
{	
	m_nPointTotalThisRound += nPointValue;

	this->updatePointsLabel ( );

//	CCLOG ( "Point Value %d, total points is now %d", nPointValue, m_nPointTotalThisRound );

	this->showSimpleVisualFX ( tPositionToShowScore, eTheSimpleScoreVisualFX );

	this->somethingJustScoredVar ( );

	if ( m_bUseImagesForPointScoreLabels )
	{		
		this->showPointsWithImagesForValue ( nPointValue, tPositionToShowScore );		
	} 
	else 
	{		
		this->showPointsWithFontLabelForValue ( nPointValue, tPositionToShowScore );
	}
}

KDvoid TheLevel::showPointsWithImagesForValue ( KDint nPointValue, const CCPoint& tPositionToShowScore )
{
	const KDchar*	szFile;

	switch ( nPointValue )
	{
		case   100	:	szFile = "100points.png";	break;
		case   500	:	szFile = "500points.png";	break;
		case  1000	:	szFile = "1000points.png";	break;
		case  5000	:	szFile = "5000points.png";	break;
		case 10000	:	szFile = "10000points.png";	break;

		default		:	szFile = "100points.png";	break;
	}

	CCSprite*		pScoreLabel = CCSprite::create ( szFile );
	this->addChild ( pScoreLabel, DepthPointScore );
	pScoreLabel->setPosition ( tPositionToShowScore );

	pScoreLabel->runAction 
	(
		CCMoveTo::create ( 1.0f, ccp ( tPositionToShowScore.x, tPositionToShowScore.y + 25 ) )
	);

	pScoreLabel->runAction 
	(
		CCSequence::create 
		(
			CCFadeTo::create ( 1.5f, 20 ),
			CCRemoveSelf::create ( ),	
			KD_NULL 
		)
	);
}

KDvoid TheLevel::showPointsWithFontLabelForValue ( KDint nPointValue, const CCPoint& tPositionToShowScore )
{
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", nPointValue ), "Marker Felt.ttf", 22 );
	this->addChild ( pScoreLabel, DepthPointScore );
	pScoreLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	pScoreLabel->setPosition ( tPositionToShowScore );

	pScoreLabel->runAction 
	(
		CCMoveTo::create ( 1.0f, ccp ( tPositionToShowScore.x, tPositionToShowScore.y + 25 ) )
	);

	pScoreLabel->runAction 
	(
		CCSequence::create 
		(
			CCFadeTo::create ( 1.5f, 20 ),
			CCRemoveSelf::create ( ),			
			KD_NULL 
		)
	);
}

KDvoid TheLevel::updatePointsLabel ( KDvoid )
{	
	m_pCurrentScoreLabel->setString ( ccszf ( "%d/%d ", m_nPointTotalThisRound, m_nPointsToPassLevel ) );
}

KDvoid TheLevel::somethingJustScoredVar ( KDvoid )
{
	m_bSomethingJustScored = KD_TRUE;

	this->unschedule ( schedule_selector ( TheLevel::resetSomethingJustScoredVar ) );
	this->  schedule ( schedule_selector ( TheLevel::resetSomethingJustScoredVar ), 3.0f );
}

KDvoid TheLevel::resetSomethingJustScoredVar ( KDfloat fDelta )
{
	m_bSomethingJustScored = KD_FALSE;

	this->unschedule ( schedule_selector ( TheLevel::resetSomethingJustScoredVar ) );
}

KDvoid TheLevel::adjustBackStrap ( KDfloat fAngle )
{
//	CCLOG ( " %f", fAngle );
	
	KDfloat		fScale  = m_pStrapBack ->getScaleX   ( );
	KDfloat		fRotate = m_pStrapFront->getRotation ( );

	if ( fAngle < 30 ) 
	{	
//		CCLOG ( " between 6 and 7 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 1.0f );
		m_pStrapBack->setRotation ( fRotate * 0.8f );		
	}
	else if ( fAngle < 60 ) 
	{		
//		CCLOG ( " between 7 and 8 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 1.05f );
		m_pStrapBack->setRotation ( fRotate * 0.80f );		
	}
	else if ( fAngle < 90 ) 
	{		
//		CCLOG ( " between 8 and 9 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 1.1f  );
		m_pStrapBack->setRotation ( fRotate * 0.85f );		
	}
	else if ( fAngle < 120 ) 
	{		
//		CCLOG ( " between 9 and 10 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 1.2f  );
		m_pStrapBack->setRotation ( fRotate * 0.95f );		
	}
	else if ( fAngle < 150 ) 
	{		
//		CCLOG ( " between 10 and 11 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 1.2f );
		m_pStrapBack->setRotation ( fRotate * 0.9f );	
	} 	
	else if ( fAngle < 180 )
	{
//		CCLOG ( " between 11 and 12 oclock" );		

		m_pStrapBack->setScaleX   ( fScale  * 1.10f );
		m_pStrapBack->setRotation ( fRotate * 0.85f );			
	} 
	else if ( fAngle < 210 )
	{
//		CCLOG ( " between 12 and 1 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.95f );
		m_pStrapBack->setRotation ( fRotate * 0.85f );		
	} 
	else if ( fAngle < 240 ) 
	{
//		CCLOG ( " between 1 and 2 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.7f  );
		m_pStrapBack->setRotation ( fRotate * 0.85f );			
	}	
	else if ( fAngle < 270 ) 
	{
//		CCLOG ( " between 2 and 3 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.6f );
		m_pStrapBack->setRotation ( fRotate * 0.9f );		
	} 	
	else if ( fAngle < 300 ) 
	{
//		CCLOG ( " between 3 and 4 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.5f );
		m_pStrapBack->setRotation ( fRotate * 1.0f );			
	}
	else if ( fAngle < 330 ) 
	{
//		CCLOG ( " between 4 and 5 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.6f );
		m_pStrapBack->setRotation ( fRotate * 1.1f );			
	}	
	else if ( fAngle < 360 ) 
	{
//		CCLOG ( " between 5 and 6 oclock" );
		
		m_pStrapBack->setScaleX   ( fScale  * 0.6f );
		m_pStrapBack->setRotation ( fRotate * 1.1f );	
	}
}

KDvoid TheLevel::showBoardMessage ( const KDchar* szTheMessage )
{
	CCLabelTTF*		pBoardMessage = CCLabelTTF::create ( szTheMessage, "Marker Felt.ttf", 22 );
	this->addChild ( pBoardMessage, DepthPointScore );
	pBoardMessage->setColor ( ccc3 ( 255, 255, 255 ) );
	pBoardMessage->setPosition ( ccp ( m_tContentSize.cx / 2, m_tContentSize.cy * 0.7f ) );

	pBoardMessage->runAction 
	(
		CCSequence::create 
		(
			CCScaleTo::create ( 2.0f, 2.0f ),
			CCFadeTo::create ( 1.0f, 0 ),
			CCRemoveSelf::create ( ),			
			KD_NULL 
		)
	);
}

KDvoid TheLevel::doPointBonusForExtraNinjas ( KDvoid )
{
	KDint	nNinjasLeft = ( m_nNinjasToTossThisLevel - m_nNinjaBeingThrown ) + 1;

	m_nBonusThisRound = ( m_nBonusPerExtraNinja * nNinjasLeft );
	m_nPointTotalThisRound = m_nPointTotalThisRound  + m_nBonusThisRound;

	this->updatePointsLabel ( );

	CCLOG ( "Ninjas Left to Throw: %i", nNinjasLeft );
}

KDvoid TheLevel::resetOrAdvanceLevel ( KDfloat fDelta )
{
	if ( m_nPointTotalThisRound >= m_nPointsToPassLevel )
	{	
//		CCLOG ( "board passed" );
		
		this->doPointBonusForExtraNinjas ( );
		
		if ( m_nBonusThisRound > 0 ) 
		{
			// if theres a bonus, show it in the level passed message			
			this->showBoardMessage ( ccszf ( "Level Passed: %d Bonus!", m_nBonusThisRound ) );			
		}
		else 
		{			
			this->showBoardMessage ( "Level Passed" ); 
		}
		
		GameData::sharedData ( )->setHighScoreForLevel ( m_nPointTotalThisRound );		// will check to see if there's a high score set
		
		GameData::sharedData ( )->addToPointTotalForAllLevels ( m_nPointTotalThisRound );
		
		GameData::sharedData ( )->levelUp ( );											// level up	
	} 
	else
	{		
		GameData::sharedData ( )->setHighScoreForLevel ( m_nPointTotalThisRound );		// will check to see if there's a high score set even if you failed the round

		this->showBoardMessage ( "Level Failed" );
//		CCLOG ( "not enough points to go up a level, will reset with the same board" );
	}

	this->scheduleOnce ( schedule_selector ( TheLevel::transitionOut ), 3.0f );			// if you want to transition after a different amount of time, then change 3 to whatever
}

KDvoid TheLevel::transitionAfterMenuPop ( KDvoid )
{
	// transition upon coming back from the menu

	this->cancelAutoPan ( );
	this->stopDotting ( );
	this->unscheduleAllSelectors ( );

	this->scheduleOnce ( schedule_selector ( TheLevel::transitionOut ), 0.1f ); 
}

KDvoid TheLevel::transitionOut ( KDfloat fDelta )
{
	this->cleanupTheLevel ( );

	// Too select a random transition comment the two lines below and uncomment the section below.
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1, TheLevel::scene ( ) ) );	
}

KDvoid TheLevel::cleanupTheLevel ( KDvoid )
{
	this->cancelAutoPan ( );

	this->pauseSchedulerAndActions ( );

	this->removeAllDots ( );

	CCLOG ( "deleting body nodes" );

	// Iterate over the bodies in the physics world
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		BodyNode*	pNode = dynamic_cast<BodyNode*> ( (CCNode*) pBody->GetUserData ( ) );
		if ( pNode != KD_NULL )
		{
//			pNode->removeSpriteAndBody ( );
		}
	}
}