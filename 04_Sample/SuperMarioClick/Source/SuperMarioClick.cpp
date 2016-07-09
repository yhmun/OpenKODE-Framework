/* --------------------------------------------------------------------------
 *
 *      File            SuperMarioClick.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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
#include "SuperMarioClick.h"
#include "ContactListener.h"
#include "WorldInitialize.h"
#include "MashRoom.h"
#include "Ball.h"

static SuperMarioClick*		l_pSuperMarioInstance = KD_NULL;

SuperMarioClick* SuperMarioClick::sharedSuperMario ( KDvoid )
{
	return l_pSuperMarioInstance;
}

SuperMarioClick::~SuperMarioClick ( KDvoid )
{
//	delete m_pWorld;	
//	delete m_pContactListener;
	delete m_pDebugDraw;

//	l_pSuperMarioInstance = KD_NULL;
}

KDbool SuperMarioClick::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	l_pSuperMarioInstance = this;

	m_pMouseJoint = KD_NULL;
	m_nContackCount = 0;

	this->initBox2dWorld ( );
	this->enableBox2dDebugDrawing ( );

	// Preload the sprite frames from the texture atlas
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "manofclick.plist" );

	// batch node for all dynamic elements
	CCSpriteBatchNode*	pBatch = CCSpriteBatchNode::create ( "manofclick.png", 100 );
	this->addChild ( pBatch, -2, kTagBatchNode );

	// Background
	CCSprite*	pBackground = CCSprite::create ( "background.png" );
	pBackground->setPosition ( ccp ( 220, 240 ) );
	this->addChild ( pBackground, -3 ); 

	// Move the background
	pBackground->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 5, ccp ( -220, 0 ) ),
				CCMoveBy::create ( 5, ccp (  220, 0 ) ),
				KD_NULL
			)
		) 
	);

	// Setup static elements
	WorldInitialize*	pSetUp = WorldInitialize::create ( m_pWorld );
	this->addChild ( pSetUp, -1 );

	// Add the scorebord
	m_pLabel = CCLabelTTF::create ( "0", "Arial Bold.ttf", 26 );
	this->addChild ( m_pLabel );
	m_pLabel->setColor ( ccc3 ( 255, 0, 0 ) );
	m_pLabel->setPosition ( ccp ( 70, 450 ) );
	
	// Add the scorebord
	m_pFighting = CCLabelTTF::create ( "Fighting!!", "Arial Bold.ttf", 26 );
	this->addChild ( m_pFighting );
	m_pFighting->setColor ( ccc3 ( 0, 255, 0 ) );
	m_pFighting->setPosition ( ccp ( 160, 500 ) );

	// Add the scorebord
	m_pMan = CCLabelTTF::create ( "????", "Arial Bold.ttf", 30 );
	this->addChild ( m_pMan );
	m_pMan->setColor ( ccc3 ( 0, 255, 0 ) );
	m_pMan->setPosition ( ccp ( 160, 500 ) );

	// Get the body and fixture to move the mushroom
	m_pPaddleFixture = MashRoom::sharedMashRoom ( )->shareMashRoomFixture ( );
	m_pPaddleBody    = MashRoom::sharedMashRoom ( )->sharedMashRoomBody   ( );

	this->setTouchEnabled ( KD_TRUE );

	// Menu
	this->createMenu ( );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid SuperMarioClick::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid SuperMarioClick::update ( KDfloat fDelta )
{
	// The number of iterations influence the accuracy of the physics simulation. With higher values the
	// body's velocity and position are more accurately tracked but at the cost of speed.
	// Usually for games only 1 position iteration is necessary to achieve good results.
	KDfloat			fTimeStep = 0.03f;
	KDint32			nVelocityIterations = 8;
	KDint32			nPositionIterations = 1;

	m_pWorld->Step ( fTimeStep, nVelocityIterations, nPositionIterations );

	// for each body, get its assigned BodyNode and update the sprite's position
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		BodyNode*	pBodyNode = (BodyNode*) pBody->GetUserData ( );
		if ( pBodyNode != KD_NULL )
		{
			CCSprite*	pBodySprite = pBodyNode->getSprite ( );

			if ( pBodySprite != KD_NULL )
			{
				// update the sprite's position to where their physics bodies are
				pBodySprite->setPosition ( Helper::toPixels ( pBody->GetPosition ( ) ) );			
				pBodySprite->setRotation ( -CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );

				// update the scoreboard if the ball fall on the floor
				if ( dynamic_cast<Ball*> ( pBodyNode ) )
				{
					if ( pBodySprite->getPositionX ( ) < 20 )
					{
						m_nContackCount = 0;
					}
				}
			}
		}
	}

	m_pLabel->setString ( ccszf ( "TOTAL: %d", m_nContackCount ) );
}

KDvoid SuperMarioClick::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_pMouseJoint != KD_NULL )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = pTouch->getLocation ( );
	b2Vec2		tLocationWorld = b2Vec2 ( tLocation.x / PTM_RATIO, tLocation.y / PTM_RATIO );

	if ( m_pPaddleFixture->TestPoint ( tLocationWorld ) )
	{
		b2MouseJointDef		tJointDef;
		tJointDef.bodyA = m_pContainerBody;
		tJointDef.bodyB = m_pPaddleBody;
		tJointDef.target = tLocationWorld;
		tJointDef.collideConnected = KD_TRUE;
		tJointDef.maxForce = 1000.0f * m_pPaddleBody->GetMass ( );
	    
		m_pMouseJoint = (b2MouseJoint*) m_pWorld->CreateJoint ( &tJointDef );
		m_pPaddleBody->SetAwake ( KD_TRUE );
	}
}

KDvoid SuperMarioClick::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_pMouseJoint == KD_NULL )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = pTouch->getLocation ( );
	b2Vec2		tLocationWorld = b2Vec2 ( tLocation.x / PTM_RATIO, tLocation.y / PTM_RATIO );

	m_pMouseJoint->SetTarget ( tLocationWorld );
}

KDvoid SuperMarioClick::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_pMouseJoint ) 
	{
		m_pWorld->DestroyJoint ( m_pMouseJoint );
		m_pMouseJoint = KD_NULL;
	}
}

KDvoid SuperMarioClick::initBox2dWorld ( KDvoid )
{
	// Construct a world object, which will hold and simulate the rigid bodies.
	b2Vec2		tGravity;
	tGravity.Set ( 0.0f, -5.0f );
	
	// Initialize world
	KDbool		bDoSleep = KD_TRUE;
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( bDoSleep );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );

	m_pContactListener = new ContactListener ( );
	m_pWorld->SetContactListener ( m_pContactListener );

	// Create edges around the entire screen
	b2BodyDef			tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );

	m_pContainerBody = m_pWorld->CreateBody ( &tGroundBodyDef );

	b2EdgeShape			tGroundBox;		
		
	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( m_tContentSize.cx / PTM_RATIO, 0 ) );
	m_pContainerBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set ( b2Vec2 ( 0, m_tContentSize.cy / PTM_RATIO ), b2Vec2 ( m_tContentSize.cx / PTM_RATIO, m_tContentSize.cy / PTM_RATIO ) );
	m_pContainerBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set ( b2Vec2 ( 0, m_tContentSize.cy / PTM_RATIO ), b2Vec2 ( 0, 0 ) );
	m_pContainerBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set (b2Vec2 ( m_tContentSize.cx / PTM_RATIO, m_tContentSize.cy / PTM_RATIO ), b2Vec2 ( m_tContentSize.cx / PTM_RATIO, 0 ) );
	m_pContainerBody->CreateFixture ( &tGroundBox, 0 );
}

KDvoid SuperMarioClick::enableBox2dDebugDrawing ( KDvoid )
{
	// Initialize debug drawing
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	KDuint32	uFlags = 0;
//	uFlags += b2Draw::e_shapeBit;
	m_pDebugDraw->SetFlags ( uFlags );	
}

KDvoid SuperMarioClick::createMenu ( KDvoid )
{
	CCMenuItemImage*	pReset = CCMenuItemImage::create ( "Reset.png", "Reset.png", this, menu_selector ( SuperMarioClick::resetGame ) );    
	( (CCSprite*) pReset->getSelectedImage ( ) )->setOpacity ( 200 );

	CCMenu*				pMenu  = CCMenu::createWithItem ( pReset ); 	
	pMenu->setPosition ( m_tContentSize.cx - 36 * 3, m_tContentSize.cy - 36 );	
	this->addChild ( pMenu, -1 );
}

KDvoid SuperMarioClick::resetGame ( CCObject* pSender )
{
	Ball::sharedBall ( )->gameReset ( );
}

CCSpriteBatchNode* SuperMarioClick::getSpriteBatch ( KDvoid )
{
	return (CCSpriteBatchNode*) this->getChildByTag ( kTagBatchNode );
}

KDvoid SuperMarioClick::setContactCount ( KDint nCount )
{
    if ( nCount == 0 )
    {
        m_nContackCount = 0;
    }
    else
    {
        m_nContackCount += 1;
        this->splashScreen ( );
    }
}

KDvoid SuperMarioClick::splashScreen ( KDvoid )
{
    if ( m_nContackCount != 0 && m_nContackCount % 10 == 0 && m_nContackCount != 100 )
	{        
        m_pFighting->runAction 
		(
			CCSequence::create 
			( 
				CCMoveTo::create ( 0.1f, ccp ( 160, 360 ) ),
				CCBlink ::create ( 2, 4 ),
				CCMoveTo::create ( 0.1f, ccp ( 160, 500 ) ),
				KD_NULL 
			) 
		);
    }

    if ( m_nContackCount == 100 )
    {
		m_pMan->runAction 
		(
			CCSequence::create 
			( 
				CCMoveTo::create ( 0.1f, ccp ( 160, 360 ) ),
				CCBlink ::create ( 3, 3 ),
				CCMoveTo::create ( 0.1f, ccp ( 160, 500 ) ),
				KD_NULL 
			) 			
		);        
    }
}
