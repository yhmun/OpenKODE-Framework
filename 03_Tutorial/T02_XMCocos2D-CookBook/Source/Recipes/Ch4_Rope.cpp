/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Rope.cpp
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
#include "Ch4_Rope.h"

KDbool Ch4_Rope::init ( KDvoid )
{	
	if ( !SideScrollerRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Press B to fire a rope." );
	
	// Initialization
	m_bOnRope = KD_FALSE;
	m_fRopeUseTimer = 0;
	
	// Move gunman to left
	m_pGunman->getBody ( )->SetTransform ( b2Vec2 ( 2, 10 ), m_pGunman->getBody ( )->GetAngle ( ) );
	
	// Create buttons
	this->createButtonWithPosition ( ccp ( 340, 75 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( ccp ( 420, 75 ), "a_button_up.png", "a_button_down.png", "A" );
	
	// Create ground
	this->addBoxWithPosition ( ccp ( 20, 40 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 60, 40 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 20, 80 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 60, 80 ), "bricks.png", 5.0f );

	this->addBoxWithPosition ( ccp ( 440, 40 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 480, 40 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 440, 80 ), "bricks.png", 5.0f );
	this->addBoxWithPosition ( ccp ( 480, 80 ), "bricks.png", 5.0f );
	
	// Add invisible rope anchor
	this->addRopeAnchor ( );

	return KD_TRUE;
}

KDvoid Ch4_Rope::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pRopes );

	SideScrollerRecipe::onExit ( );
}

KDvoid Ch4_Rope::step ( KDfloat fDelta )
{
	SideScrollerRecipe::step ( fDelta );

	// Process input for the A button
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "B" ) )
		{
			if ( !m_bOnRope )
			{
				this->useRope ( );
			}
			else
			{
				this->releaseRope ( );
			}	
		}

		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			if ( m_bOnRope )
			{
				this->releaseRope ( );
			}
			else
			{
				this->processJump ( );
			}
		}
		else if ( !pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			m_fJumpCounter = -10.f;
		}
	}

	// Update all ropes
	CCARRAY_FOREACH ( m_pRopes, pObject )
	{
		VRope*	pRope = (VRope*) pObject;

		pRope->update ( fDelta );
		pRope->updateSprites ( );
	}

	// Decrement our use timer
	m_fRopeUseTimer -= fDelta;
}
		
KDvoid Ch4_Rope::addRopeAnchor ( KDvoid )
{
	// Add rope anchor body
	b2BodyDef	tAnchorBodyDef;
	tAnchorBodyDef.position.Set ( 240 / PTM_RATIO, 350 / PTM_RATIO );	// center body on screen
	m_pAnchorBody = m_pWorld->CreateBody ( &tAnchorBodyDef );
	
	// Add rope spritesheet to layer
	m_pRopeSpriteSheet = CCSpriteBatchNode::create ( "rope.png"  );
	this->addChild ( m_pRopeSpriteSheet );
	
	// Init array that will hold references to all our ropes
	m_pRopes = new CCArray ( );
}

KDvoid Ch4_Rope::useRope ( KDvoid )
{
	if ( m_fRopeUseTimer > 0 )
	{
		return;
	}
	else
	{
		m_fRopeUseTimer = 0.2f;
	}
	
	// The rope joint goes from the anchor to the gunman
	b2RopeJointDef		tJointDef;
	tJointDef.bodyA = m_pAnchorBody;
	tJointDef.bodyB = m_pGunman->getBody ( );
	tJointDef.localAnchorA = b2Vec2 ( 0, 0 );
	tJointDef.localAnchorB = b2Vec2 ( 0, 0 );
	tJointDef.maxLength = ( m_pGunman->getBody ( )->GetPosition ( ) - m_pAnchorBody->GetPosition ( ) ).Length ( );

	// Create VRope with two b2bodies and pointer to spritesheet
	VRope*		pNewRope = VRope::create ( m_pAnchorBody, m_pGunman->getBody ( ), m_pRopeSpriteSheet );
	
	// Create joint
	pNewRope->setJoint ( m_pWorld->CreateJoint ( &tJointDef ) );
	m_pRopes->addObject ( pNewRope );	
	
	// Keep track of 'onRope' state
	m_bOnRope = !m_bOnRope;	
}

KDvoid Ch4_Rope::releaseRope ( KDvoid )
{
	if ( m_fRopeUseTimer > 0 )
	{
		return;
	}
	else
	{
		m_fRopeUseTimer = 0.2f;
	}
	
	// Jump off the rope
	this->processJump ( );
	
	// Destroy the rope
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pRopes, pObject )
	{
		VRope*	pRope = (VRope*) pObject;

		m_pWorld->DestroyJoint ( pRope->getJoint ( ) );
		pRope->removeSprites ( );
	}
	m_pRopes->removeAllObjects ( );
	
	// Keep track of 'onRope' state
	m_bOnRope = !m_bOnRope;
}