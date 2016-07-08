/* --------------------------------------------------------------------------
 *
 *      File            TestRotateWorld.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestRotateWorld.h"

//------------------------------------------------------------------
//
// TestRotateWorld
//
//------------------------------------------------------------------
KDbool TestRotateWorld::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCLayer*  pLayer = CCLayer::create ( );
	pLayer->setContentSize ( tLyrSize );
	pLayer->runAction	( CCRotateBy::create ( 4, -360 ) );
	this->addChild		( pLayer );

	CCNode*  pWhite = CCLayerColor::create ( ccc4 ( 255, 255, 255, 128 ), tLyrSize );
	pWhite->setScale	( 0.5f );
	pWhite->setPosition ( ccp ( -tLyrSize.cx / 4, tLyrSize.cy / 4 ) );
	pWhite->runAction	( CCRotateBy::create ( 12, 720 ) );	
	pLayer->addChild	( pWhite );

	CCNode*  pRed = CCLayerColor::create ( ccc4 ( 255, 0, 0, 128 ), tLyrSize );
	pRed->setScale		( 0.5f );
	pRed->setPosition	( ccp ( -tLyrSize.cx / 4, -tLyrSize.cy / 4 ) );		
	pRed->runAction		( CCRotateBy::create ( 12, 720 ) );
	pLayer->addChild	( pRed );

	CCNode*  pGreen = CCLayerColor::create ( ccc4 ( 0, 255, 0, 128 ), tLyrSize );
	pGreen->setScale    ( 0.5f );
	pGreen->setPosition ( ccp ( tLyrSize.cx / 4, tLyrSize.cy / 4 ) );		
	pGreen->addChild    ( LabelLayer::create ( ) );
	pGreen->runAction   ( CCRotateBy::create ( 12, 720 ) );
	pLayer->addChild    ( pGreen );

	CCNode*  pBlue = CCLayerColor::create ( ccc4 ( 0, 0, 255, 128 ), tLyrSize );

	pBlue->setScale		( 0.5f );
	pBlue->setPosition	( ccp ( tLyrSize.cx / 4, - tLyrSize.cy / 4 ) );
	pBlue->addChild		( SpriteLayer::create ( ) );
	pBlue->runAction	( CCRotateBy::create ( 12, 720 ) );
	pLayer->addChild	( pBlue, -1 );

	return KD_TRUE;
}

KDuint TestRotateWorld::count ( KDvoid )
{
	return 1;
}

//------------------------------------------------------------------
//
// LabelLayer
//
//------------------------------------------------------------------
KDbool LabelLayer::init ( KDvoid )
{
	CCLayer::initWithVisibleViewport ( );
	
	this->setPosition ( ccp ( 0, 0 ) );

	CCLabelTTF*  pLabel = CCLabelTTF::create ( "Cocos2D", "fonts/Marker Felt.ttf", 64 );
	pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pLabel );

	return KD_TRUE;
}

//------------------------------------------------------------------
//
// SpriteLayer
//
//------------------------------------------------------------------
KDbool SpriteLayer::init ( KDvoid )
{
	CCLayer::initWithVisibleViewport ( );

	this->setPosition ( ccp ( 0, 0 ) );

	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCSprite*  pSprite = CCSprite::create ( "Images/grossini.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccpMid ( tLyrSize ) );
	pSprite->runAction ( CCRotateBy::create ( 16, -3600 ) );
	this->addChild ( pSprite );

	CCSprite*  pSpriteSister1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	pSpriteSister1->setScale ( 1.5f );
	pSpriteSister1->setPosition ( ccp ( 40, tLyrSize.cy / 2 ) );
	this->addChild ( pSpriteSister1 );

	CCSprite*  pSpriteSister2 = CCSprite::create ( "Images/grossinis_sister2.png" );
	pSpriteSister2->setScale ( 1.5f );
	pSpriteSister2->setPosition ( ccp ( tLyrSize.cx - 40, tLyrSize.cy / 2 ) );
	this->addChild ( pSpriteSister2 );
	
	CCActionInterval*  jump1 = CCJumpBy::create ( 4, CCPointMake ( -400, 0 ), 100, 4 );
	CCActionInterval*  rot1  = CCRotateBy::create ( 4, 360 * 2 );
	CCActionInterval*  jump2 = jump1->reverse ( );
	CCActionInterval*  rot2  = rot1->reverse ( );

	pSpriteSister1->runAction ( CCRepeat::create ( CCSequence::create ( jump2, jump1, KD_NULL ), 5 ) );
	pSpriteSister1->runAction ( CCRepeat::create ( CCSequence::create ( rot1, rot2, KD_NULL ), 5 ) );

	pSpriteSister2->runAction ( CCRepeat::create ( CCSequence::create ( ( CCFiniteTimeAction * ) ( jump1->copy ( )->autorelease ( ) ), ( CCFiniteTimeAction * ) ( jump2->copy ( )->autorelease ( ) ), KD_NULL ), 5 ) );
	pSpriteSister2->runAction ( CCRepeat::create ( CCSequence::create ( ( CCFiniteTimeAction * ) ( rot2->copy ( )->autorelease ( ) ), ( CCFiniteTimeAction * ) ( rot1->copy ( )->autorelease ( ) ), KD_NULL ), 5 ) );

	return KD_TRUE;
}
