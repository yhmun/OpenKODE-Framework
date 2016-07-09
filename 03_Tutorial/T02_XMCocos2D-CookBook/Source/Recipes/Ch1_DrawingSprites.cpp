/* --------------------------------------------------------------------------
 *
 *      File            Ch1_DrawingSprites.cpp
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
#include "Ch1_DrawingSprites.h"

enum 
{
	TAG_ALICE_SPRITE	= 0,
	TAG_CAT_SPRITE		= 1,
	TAG_TREE_SPRITE_1	= 2,
	TAG_TREE_SPRITE_2	= 3,
	TAG_TREE_SPRITE_3	= 4,
	TAG_CLOUD_BATCH		= 5,
	TAG_GRASS_BATCH_1	= 6,
	TAG_GRASS_BATCH_2	= 7,
};

KDbool Ch1_DrawingSprites::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	//
	// Draw a sprite using CCSprite 
	//

	CCSprite*	pTree1 = CCSprite::create ( "tree.png" );
	pTree1->setPosition ( ccp ( 20, 20 ) );
	pTree1->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	pTree1->setScale ( 1.5f );
	this->addChild ( pTree1, 2, TAG_TREE_SPRITE_1 );

	//
	// Draw a sprite using CCSprite 
	//

	CCSprite*	pCat = CCSprite::create ( "cheshire_cat.png" );
	pCat->setPosition ( ccp ( 250, 180) );
	pCat->setScale ( 0.4f );
	this->addChild ( pCat, 3, TAG_CAT_SPRITE );

	//
	// Draw a sprite using CCTexture2D 
	//

	CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "tree.png" );
	CCSprite*		pTree2 = CCSprite::createWithTexture ( pTexture );
	pTree2->setPosition ( ccp ( 300, 20 ) );
	pTree2->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	pTree2->setScale ( 2.0f );
	this->addChild ( pTree2, 2, TAG_TREE_SPRITE_2 );

	//
	// Draw a sprite using CCSpriteFrameCache and CCTexture2D 
	//

	CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pTexture, pTree2->getTextureRect ( ) );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFrame ( pFrame, "tree.png" );
	CCSprite*		pTree3 = CCSprite::createWithSpriteFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "tree.png" ) );
	pTree3->setPosition ( ccp ( 400, 20 ) );
	pTree3->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	pTree3->setScale ( 1.25f );
	this->addChild ( pTree3, 2, TAG_TREE_SPRITE_3 );
	
	//
	// Load a set of spriteframes from a PLIST file and draw one by name 
	//
	
	// Get the sprite frame cache singleton
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Load our scene sprites from a spritesheet
	pCache->addSpriteFramesWithFile ( "alice_scene_sheet.plist" );

	// Specify the sprite frame and load it into a CCSprite
	CCSprite*	pAlice = CCSprite::create ( "alice.png" );
	
	// Generate Mip Maps for the sprite
//	pAlice->getTexture ( )->generateMipmap ( );
//	ccTexParams		tTexParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
//	pAlice->getTexture ( )->setTexParameters ( tTexParams );
	
	// Set other information. 
	pAlice->setPosition ( ccp ( 120, 20 ) );
	pAlice->setScale ( 0.4f );
	pAlice->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	this->addChild ( pAlice, 2, TAG_ALICE_SPRITE );
	
	// Make Alice grow and shrink.
	pAlice->runAction 
	(
		CCRepeatForever::create 
		( 
			CCSequence::create
			(
				CCScaleTo::create ( 4.0f, 0.7f ),
				CCScaleTo::create ( 4.0f, 0.1f ),
				KD_NULL
			)
		)
	);

	//
	// Draw sprites using CCBatchSpriteNode 
	//
	
	// Clouds
	CCSpriteBatchNode*	pCloudBatch = CCSpriteBatchNode::create ( "cloud_01.png", 10 );
	this->addChild ( pCloudBatch, 1, TAG_CLOUD_BATCH );
	for ( KDint x = 0; x < 10; x++ )
	{
		CCSprite*	pS = CCSprite::createWithTexture ( pCloudBatch->getTexture ( ), CCRect ( 0, 0, 64, 64 ) );
		pS->setOpacity ( 100 );
		pCloudBatch->addChild ( pS );
		pS->setPosition ( ccp ( kdRand ( ) % 500 - 50, kdRand ( ) % 150 + 200 ) );
	}

	// Middleground Grass
	CCSpriteBatchNode*	pGrassBatch1 = CCSpriteBatchNode::create ( "grass_01.png", 30 );
	this->addChild ( pGrassBatch1, 1, TAG_GRASS_BATCH_1 );
	for ( KDint x = 0; x < 30; x++ )
	{
		CCSprite*	pS = CCSprite::createWithTexture ( pGrassBatch1->getTexture ( ), CCRect ( 0, 0, 64, 64 ) );
		pS->setOpacity ( 255 );
		pGrassBatch1->addChild ( pS );
		pS->setPosition ( ccp ( kdRand ( ) % 500 - 50, kdRand ( ) % 20 + 70 ) );
	}

	// Foreground Grass
	CCSpriteBatchNode*	pGrassBatch2 = CCSpriteBatchNode::create ( "grass_01.png", 30 );
	this->addChild ( pGrassBatch2, 3, TAG_GRASS_BATCH_2 );
	for ( KDint x = 0; x < 30; x++ )
	{
		CCSprite*	pS = CCSprite::createWithTexture ( pGrassBatch2->getTexture ( ), CCRect ( 0, 0, 64, 64 ) );
		pS->setOpacity ( 255 );
		pGrassBatch2->addChild ( pS );
		pS->setPosition ( ccp ( kdRand ( ) % 500 - 50, kdRand ( ) % 40 - 10 ) );
	}

	//
	// Draw colored rectangles using a 1px x 1px white texture 
	//

	// Draw the sky using blank.png
	this->drawColoredSpriteAt ( ccp ( 240, 190 ), CCRect ( 0, 0, 480, 260 ), ccc3 ( 150, 200, 200 ), 0 );
	
	// Draw the ground using blank.png
	this->drawColoredSpriteAt ( ccp ( 240, 30 ), CCRect ( 0, 0, 480, 60 ), ccc3 ( 80, 50, 25 ), 0 );

	return KD_TRUE;
}

KDvoid Ch1_DrawingSprites::drawColoredSpriteAt ( const CCPoint& tPosition, const CCRect& tRect, const ccColor3B& tColor, KDint nZOrder )
{
	CCSprite*	pSprite = CCSprite::create ( "blank.png" );
	pSprite->setPosition ( tPosition );
	pSprite->setTextureRect ( tRect );
	pSprite->setColor ( tColor );
	this->addChild ( pSprite );
	
	// Set Z Order
	this->reorderChild ( pSprite, nZOrder );
}