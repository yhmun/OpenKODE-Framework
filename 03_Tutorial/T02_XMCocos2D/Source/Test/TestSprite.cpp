/* --------------------------------------------------------------------------
 *
 *      File            TestSprite.cpp
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
#include "TestSprite.h"
#include "../TestList.h"
enum 
{
    kTagTileMap         = 1,
    kTagSpriteBatchNode = 1,
    kTagNode            = 2,
    kTagAnimation1      = 1,
    kTagSpriteLeft         ,
    kTagSpriteRight        ,
};

enum 
{
	kTagSprite1, 
	kTagSprite2, 
	kTagSprite3, 
	kTagSprite4, 
	kTagSprite5, 
	kTagSprite6, 
	kTagSprite7, 
	kTagSprite8, 
};

TestSprite* TestSprite::create ( KDvoid )
{
	TestSprite*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{	
		case  0 :	pLayer = new Sprite1											( );	break;
        case  1 :	pLayer = new SpriteBatchNode1									( );	break;
        case  2 :	pLayer = new SpriteFrameTest									( );	break;
        case  3 :	pLayer = new SpriteFrameAliasNameTest							( );	break;
        case  4 :	pLayer = new SpriteAnchorPoint									( );	break;
        case  5 :	pLayer = new SpriteBatchNodeAnchorPoint							( );	break;
        case  6 :	pLayer = new SpriteOffsetAnchorRotation							( );	break;
        case  7 :	pLayer = new SpriteBatchNodeOffsetAnchorRotation				( );	break;
        case  8 :	pLayer = new SpriteOffsetAnchorScale							( );	break;
        case  9 :	pLayer = new SpriteBatchNodeOffsetAnchorScale					( );	break;			
        case 10 :	pLayer = new SpriteOffsetAnchorSkew								( );	break;
        case 11 :	pLayer = new SpriteOffsetAnchorRotationalSkew					( );	break;
        case 12 :	pLayer = new SpriteBatchNodeOffsetAnchorSkew					( );	break;
        case 13 :	pLayer = new SpriteBatchNodeOffsetAnchorRotationalSkew			( );	break;
        case 14 :	pLayer = new SpriteOffsetAnchorSkewScale						( );	break;
        case 15 :	pLayer = new SpriteOffsetAnchorRotationalSkewScale				( );	break;
        case 16 :	pLayer = new SpriteBatchNodeOffsetAnchorSkewScale				( );	break;
        case 17 :	pLayer = new SpriteOffsetAnchorRotationalSkewScale				( );	break;
        case 18 :	pLayer = new SpriteOffsetAnchorFlip								( );	break;
        case 19 :	pLayer = new SpriteBatchNodeOffsetAnchorFlip					( );	break;
        case 20 :	pLayer = new SpriteAnimationSplit								( );	break;
        case 21 :	pLayer = new SpriteColorOpacity									( );	break;
        case 22 :	pLayer = new SpriteBatchNodeColorOpacity						( );	break;
        case 23 :	pLayer = new SpriteZOrder										( );	break;
        case 24 :	pLayer = new SpriteBatchNodeZOrder								( );	break;
		case 25 :	pLayer = new SpriteBatchNodeReorder								( );	break;
        case 26 :	pLayer = new SpriteBatchNodeReorderSameIndex					( );	break;
        case 27 :	pLayer = new SpriteBatchNodeReorderOneChild						( );	break;
        case 28 :	pLayer = new NodeSort											( );	break;
        case 29 :	pLayer = new SpriteZVertex										( );	break;
        case 30 :	pLayer = new SpriteBatchNodeZVertex								( );	break;
        case 31 :	pLayer = new Sprite6											( );	break;
        case 32 :	pLayer = new SpriteFlip											( );	break;
        case 33 :	pLayer = new SpriteBatchNodeFlip								( );	break;
        case 34 :	pLayer = new SpriteAliased										( );	break;
        case 35 :	pLayer = new SpriteBatchNodeAliased								( );	break;
        case 36 :	pLayer = new SpriteNewTexture									( );	break;
        case 37 :	pLayer = new SpriteBatchNodeNewTexture							( );	break;
        case 38 :	pLayer = new SpriteHybrid										( );	break;
        case 39 :	pLayer = new SpriteBatchNodeChildren							( );	break;
        case 40 :	pLayer = new SpriteBatchNodeChildrenZ							( );	break;
        case 41 :	pLayer = new SpriteChildrenVisibility							( );	break;
        case 42 :	pLayer = new SpriteChildrenAnchorPoint							( );	break;
        case 43 :	pLayer = new SpriteBatchNodeChildrenAnchorPoint					( );	break;
        case 44 :	pLayer = new SpriteBatchNodeChildrenScale						( );	break;
        case 45 :	pLayer = new SpriteChildrenChildren								( );	break;
        case 46 :	pLayer = new SpriteBatchNodeChildrenChildren					( );	break;
        case 47 :	pLayer = new SpriteBatchNodeChildrenChildren					( );	break;
        case 48 :	pLayer = new SpriteSkewNegativeScaleChildren					( );	break;
        case 49 :	pLayer = new SpriteRotationalSkewNegativeScaleChildren			( );	break;
        case 50 :	pLayer = new SpriteBatchNodeSkewNegativeScaleChildren			( );	break;
        case 51 :	pLayer = new SpriteBatchNodeRotationalSkewNegativeScaleChildren	( );	break;
        case 52 :	pLayer = new SpriteNilTexture									( );	break;
        case 53 :	pLayer = new SpriteSubclass										( );	break;
        case 54 :	pLayer = new SpriteDoubleResolution								( );	break;
        case 55 :	pLayer = new AnimationCache										( );	break;
        case 56 :	pLayer = new AnimationCacheFile									( );	break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestSprite::count ( KDvoid )
{
	return 57;
}

//------------------------------------------------------------------
//
// Sprite1
//
//------------------------------------------------------------------
KDvoid Sprite1::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	this->addNewSpriteWithCoords ( ccpMid ( s ) );	
	this->setTouchEnabled ( KD_TRUE );
}

KDvoid Sprite1::addNewSpriteWithCoords ( CCPoint p )
{
	KDint idx = (KDint) ( CCRANDOM_0_1 ( ) * 1400.0f / 100.0f );
	KDint x = ( idx % 5 ) * 85;
	KDint y = ( idx / 5 ) * 121;	
	
	CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( x, y, 85, 121 ) );
	addChild ( sprite );
	
	sprite->setPosition ( ccp ( p.x, p.y ) );
	
	CCActionInterval* action;
	KDfloat random = CCRANDOM_0_1 ( );
	
	if      ( random < 0.20 )	action = CCScaleBy::create ( 3, 2 );
	else if ( random < 0.40 )	action = CCRotateBy::create ( 3, 360 );
	else if ( random < 0.60 )	action = CCBlink::create ( 1, 3 );
	else if ( random < 0.80 )	action = CCTintBy::create ( 2, 0, -255, -255 );
	else                        action = CCFadeOut::create ( 2 );

	CCActionInterval* action_back = action->reverse ( );
	CCActionInterval* seq = CCSequence::create ( action, action_back, KD_NULL );
	
	sprite->runAction ( CCRepeatForever::create ( seq ) );
}

KDvoid Sprite1::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
	for ( CCSetIterator it = touches->begin ( ); it != touches->end ( ); it++ ) 
	{
		CCTouch*  touch = ( CCTouch* ) ( *it );

		CC_BREAK_IF ( !touch );	

		addNewSpriteWithCoords ( touch->getLocation ( ) );
	}
}

const KDchar* Sprite1::title ( KDvoid )
{
	return "Sprite";
}

const KDchar* Sprite1::subtitle ( KDvoid )
{
	return "( tap screen )";
}

//------------------------------------------------------------------
//
// SpriteBatchNode1
//
//------------------------------------------------------------------
KDvoid SpriteBatchNode1::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	setTouchEnabled ( KD_TRUE );

	CCSpriteBatchNode*  batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 50 );
	if ( batch )
	{
		this->addChild ( batch, 0, kTagSpriteBatchNode );
	}
	
	const CCSize&  s = this->getContentSize ( );
	addNewSpriteWithCoords ( ccpMid ( s ) );
}

KDvoid SpriteBatchNode1::addNewSpriteWithCoords ( CCPoint p )
{
	CCSpriteBatchNode*  batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );
	
	KDint idx = (KDint) ( CCRANDOM_0_1 ( ) * 1400 / 100 );
	KDint x = ( idx % 5 ) * 85;
	KDint y = ( idx / 5 ) * 121;

	CCSprite*  sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( x, y, 85, 121 ) );
	if ( sprite )
	{
		batch->addChild ( sprite );

		sprite->setPosition ( ccp ( p.x, p.y ) );

		CCActionInterval*  action = KD_NULL;
		KDfloat  random = CCRANDOM_0_1 ( );
		
		if      ( random < 0.20 ) action = CCScaleBy::create ( 3, 2 );
		else if ( random < 0.40 ) action = CCRotateBy::create ( 3, 360 );
		else if ( random < 0.60 ) action = CCBlink::create ( 1, 3 );
		else if ( random < 0.80 ) action = CCTintBy::create ( 2, 0, -255, -255 );
		else                      action = CCFadeOut::create ( 2 );

		CCActionInterval*  action_back = action->reverse ( );
		CCActionInterval*  seq = CCSequence::create ( action, action_back, KD_NULL );

		sprite->runAction ( CCRepeatForever::create ( seq ) );
	}
}

KDvoid SpriteBatchNode1::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
	CCSetIterator it;
	CCTouch* touch;

	for ( it = touches->begin ( ); it != touches->end ( ); it++ ) 
	{
		touch = ( CCTouch* ) ( *it );

		if ( !touch )
			break;

		CCPoint location = touch->getLocation ( );
		addNewSpriteWithCoords ( location );
	}

}

const KDchar* SpriteBatchNode1::title ( KDvoid )
{
	return "SpriteBatchNode ( tap screen )";
}


//------------------------------------------------------------------
//
// SpriteColorOpacity
//
//------------------------------------------------------------------
KDvoid SpriteColorOpacity::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSprite* sprite1 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
	CCSprite* sprite2 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	CCSprite* sprite3 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 2, 121 * 1, 85, 121 ) );
	CCSprite* sprite4 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 3, 121 * 1, 85, 121 ) );
	
	CCSprite* sprite5 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
	CCSprite* sprite6 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	CCSprite* sprite7 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 2, 121 * 1, 85, 121 ) );
	CCSprite* sprite8 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 3, 121 * 1, 85, 121 ) );
	
	CCSize s = this->getContentSize ( );
	sprite1->setPosition ( ccp ( (  s.cx / 5 ) * 1, ( s.cy / 3 ) * 1 ) );
	sprite2->setPosition ( ccp ( (  s.cx / 5 ) * 2, ( s.cy / 3 ) * 1 ) );
	sprite3->setPosition ( ccp ( (  s.cx / 5 ) * 3, ( s.cy / 3 ) * 1 ) );
	sprite4->setPosition ( ccp ( (  s.cx / 5 ) * 4, ( s.cy / 3 ) * 1 ) );
	sprite5->setPosition ( ccp ( (  s.cx / 5 ) * 1, ( s.cy / 3 ) * 2 ) );
	sprite6->setPosition ( ccp ( (  s.cx / 5 ) * 2, ( s.cy / 3 ) * 2 ) );
	sprite7->setPosition ( ccp ( (  s.cx / 5 ) * 3, ( s.cy / 3 ) * 2 ) );
	sprite8->setPosition ( ccp ( (  s.cx / 5 ) * 4, ( s.cy / 3 ) * 2 ) );
	
	CCActionInterval* action = CCFadeIn::create ( 2 );
	CCActionInterval* action_back = action->reverse ( );
	CCAction* fade = CCRepeatForever::create ( CCSequence::create ( action, action_back, KD_NULL ) );
	
	CCActionInterval* tintred = CCTintBy::create ( 2, 0, -255, -255 );
	CCActionInterval* tintred_back = tintred->reverse ( );
	CCAction* red = CCRepeatForever::create ( CCSequence::create ( tintred, tintred_back, KD_NULL ) );
	
	CCActionInterval* tintgreen = CCTintBy::create ( 2, -255, 0, -255 );
	CCActionInterval* tintgreen_back = tintgreen->reverse ( );
	CCAction* green = CCRepeatForever::create ( CCSequence::create ( tintgreen, tintgreen_back, KD_NULL ) );
	
	CCActionInterval* tintblue = CCTintBy::create ( 2, -255, -255, 0 );
	CCActionInterval* tintblue_back = tintblue->reverse ( );
	CCAction* blue = CCRepeatForever::create ( CCSequence::create ( tintblue, tintblue_back, KD_NULL ) );
	
	sprite5->runAction ( red );
	sprite6->runAction ( green );
	sprite7->runAction ( blue );
	sprite8->runAction ( fade );
	
	// late add: test dirtyColor and dirtyPosition
	addChild ( sprite1, 0, kTagSprite1 );
	addChild ( sprite2, 0, kTagSprite2 );
	addChild ( sprite3, 0, kTagSprite3 );
	addChild ( sprite4, 0, kTagSprite4 );
	addChild ( sprite5, 0, kTagSprite5 );
	addChild ( sprite6, 0, kTagSprite6 );
	addChild ( sprite7, 0, kTagSprite7 );
	addChild ( sprite8, 0, kTagSprite8 );
	
	schedule ( schedule_selector ( SpriteColorOpacity::removeAndAddSprite ), 2 );
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
KDvoid SpriteColorOpacity::removeAndAddSprite ( KDfloat dt )
{
	CCSprite* sprite = (CCSprite *) ( getChildByTag ( kTagSprite5 ) );	
	sprite->retain ( );
	
	removeChild ( sprite, KD_FALSE );
	addChild ( sprite, 0, kTagSprite5 );
	
	sprite->release ( );
}

const KDchar* SpriteColorOpacity::title ( KDvoid )
{
	return "Sprite: Color & Opacity";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeColorOpacity
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeColorOpacity::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	// small capacity. Testing resizing.
	// Don't use capacity=1 in your real game. It is expensive to resize the capacity
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 1 );
	addChild ( batch, 0, kTagSpriteBatchNode );		
	
	CCSprite* sprite1 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
	CCSprite* sprite2 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	CCSprite* sprite3 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 2, 121 * 1, 85, 121 ) );
	CCSprite* sprite4 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 3, 121 * 1, 85, 121 ) );
	
	CCSprite* sprite5 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
	CCSprite* sprite6 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	CCSprite* sprite7 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 2, 121 * 1, 85, 121 ) );
	CCSprite* sprite8 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 3, 121 * 1, 85, 121 ) );
		
	CCSize    s = this->getContentSize ( );

	sprite1->setPosition ( ccp ( ( s.cx / 5 ) * 1, ( s.cy / 3 ) * 1 ) );
	sprite2->setPosition ( ccp ( ( s.cx / 5 ) * 2, ( s.cy / 3 ) * 1 ) );
	sprite3->setPosition ( ccp ( ( s.cx / 5 ) * 3, ( s.cy / 3 ) * 1 ) );
	sprite4->setPosition ( ccp ( ( s.cx / 5 ) * 4, ( s.cy / 3 ) * 1 ) );
	sprite5->setPosition ( ccp ( ( s.cx / 5 ) * 1, ( s.cy / 3 ) * 2 ) );
	sprite6->setPosition ( ccp ( ( s.cx / 5 ) * 2, ( s.cy / 3 ) * 2 ) );
	sprite7->setPosition ( ccp ( ( s.cx / 5 ) * 3, ( s.cy / 3 ) * 2 ) );
	sprite8->setPosition ( ccp ( ( s.cx / 5 ) * 4, ( s.cy / 3 ) * 2 ) );

	CCActionInterval* action = CCFadeIn::create ( 2 );
	CCActionInterval* action_back = action->reverse ( );
	CCAction* fade = CCRepeatForever::create ( CCSequence::create ( action, action_back, KD_NULL ) );

	CCActionInterval* tintred = CCTintBy::create ( 2, 0, -255, -255 );
	CCActionInterval* tintred_back = tintred->reverse ( );
	CCAction* red = CCRepeatForever::create ( CCSequence::create ( tintred, tintred_back, KD_NULL ) );

	CCActionInterval* tintgreen = CCTintBy::create ( 2, -255, 0, -255 );
	CCActionInterval* tintgreen_back = tintgreen->reverse ( );
	CCAction* green = CCRepeatForever::create ( CCSequence::create ( tintgreen, tintgreen_back, KD_NULL ) );

	CCActionInterval* tintblue = CCTintBy::create ( 2, -255, -255, 0 );
	CCActionInterval* tintblue_back = tintblue->reverse ( );
	CCAction* blue = CCRepeatForever::create ( CCSequence::create ( tintblue, tintblue_back, KD_NULL ) );
		
	sprite5->runAction ( red );
	sprite6->runAction ( green );
	sprite7->runAction ( blue );
	sprite8->runAction ( fade );
	
	// late add: test dirtyColor and dirtyPosition
	batch->addChild ( sprite1, 0, kTagSprite1 );
	batch->addChild ( sprite2, 0, kTagSprite2 );
	batch->addChild ( sprite3, 0, kTagSprite3 );
	batch->addChild ( sprite4, 0, kTagSprite4 );
	batch->addChild ( sprite5, 0, kTagSprite5 );
	batch->addChild ( sprite6, 0, kTagSprite6 );
	batch->addChild ( sprite7, 0, kTagSprite7 );
	batch->addChild ( sprite8, 0, kTagSprite8 );
		
	schedule ( schedule_selector ( SpriteBatchNodeColorOpacity::removeAndAddSprite ), 2 );
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
KDvoid SpriteBatchNodeColorOpacity::removeAndAddSprite ( KDfloat dt )
{
	CCSpriteBatchNode* batch= (CCSpriteBatchNode *) ( getChildByTag ( kTagSpriteBatchNode ) );
	CCSprite* sprite = (CCSprite *) ( batch->getChildByTag ( kTagSprite5 ) );
	
	sprite->retain ( );

	batch->removeChild ( sprite, KD_FALSE );
	batch->addChild ( sprite, 0, kTagSprite5 );
	
	sprite->release ( );
}

const KDchar* SpriteBatchNodeColorOpacity::title ( KDvoid )
{
	return "SpriteBatchNode: Color & Opacity";
}

//------------------------------------------------------------------
//
// SpriteZOrder
//
//------------------------------------------------------------------
KDvoid SpriteZOrder::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	m_nDir = 1;
			
	CCSize s = this->getContentSize ( );
	
	KDfloat step = s.cx / 11;
	for ( KDint i = 0; i < 5; i++ ) 
	{
		CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*0, 121*1, 85, 121 ) );
		sprite->setPosition ( ccp ( (  i+1 )*step, s.cy / 2 ) );
		addChild ( sprite, i );
	}
	
	for ( KDint i = 5; i < 10; i++ ) 
	{
		CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*1, 121*0, 85, 121 ) );
		sprite->setPosition ( ccp ( (  i+1 )*step, s.cy / 2 ) );
		addChild ( sprite, 14-i );
	}
	
	CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*3, 121*0, 85, 121 ) );
	addChild ( sprite, -1, kTagSprite1 );
	sprite->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 20 ) );
	sprite->setScaleX ( 6 );
	sprite->setColor ( ccRED );
	
	schedule ( schedule_selector ( SpriteZOrder::reorderSprite ), 1 );		
}

KDvoid SpriteZOrder::reorderSprite ( KDfloat dt )
{
	CCSprite* sprite = (CCSprite *) ( getChildByTag ( kTagSprite1 ) );
	
	int z = sprite->getZOrder ( );
	
	if ( z < -1 ) m_nDir =  1;
	if ( z > 10 ) m_nDir = -1;
	
	z += m_nDir * 3;
	
	reorderChild ( sprite, z );
	
}

const KDchar* SpriteZOrder::title ( KDvoid )
{
	return "Sprite: Z order";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZOrder
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeZOrder::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	m_nDir = 1;
	
	// small capacity. Testing resizing.
	// Don't use capacity=1 in your real game. It is expensive to resize the capacity
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 1 );
	addChild ( batch, 0, kTagSpriteBatchNode );		
	
	CCSize s = this->getContentSize ( );

	KDfloat step = s.cx / 11;
	for ( KDint i = 0; i < 5; i++ ) 
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*0, 121*1, 85, 121 ) );
		sprite->setPosition ( ccp ( ( i + 1 ) * step, s.cy / 2 ) );
		batch->addChild ( sprite, i );
	}
	
	for ( KDint i = 5; i < 10; i++ ) 
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*1, 121*0, 85, 121 ) );
		sprite->setPosition ( ccp ( ( i + 1 ) * step, s.cy / 2 ) );
		batch->addChild ( sprite, 14-i );
	}
	
	CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*3, 121*0, 85, 121 ) );
	batch->addChild ( sprite, -1, kTagSprite1 );
	sprite->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 20 ) );
	sprite->setScaleX ( 6 );
	sprite->setColor ( ccRED );
	
	schedule ( schedule_selector ( SpriteBatchNodeZOrder::reorderSprite ), 1 );		
}

KDvoid SpriteBatchNodeZOrder::reorderSprite ( KDfloat dt )
{
	CCSpriteBatchNode* batch= (CCSpriteBatchNode *) ( getChildByTag ( kTagSpriteBatchNode ) );
	CCSprite* sprite = (CCSprite *) ( batch->getChildByTag ( kTagSprite1 ) );
	
	KDint z = sprite->getZOrder ( );
	
	if ( z < -1 ) m_nDir = 1;
	if ( z > 10 ) m_nDir = -1;
	
	z += m_nDir * 3;

	batch->reorderChild ( sprite, z );
}

const KDchar* SpriteBatchNodeZOrder::title ( KDvoid )
{
	return "SpriteBatchNode: Z order";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorder
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeReorder::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCArray* a = CCArray::createWithCapacity ( 10 );
	CCSpriteBatchNode* asmtest = CCSpriteBatchNode::create ( "animations/ghosts.png" );
	
	for ( KDint i = 0;  i < 10; i++ )
	{
		CCSprite* s1 = CCSprite::createWithTexture ( asmtest->getTexture ( ), CCRectMake ( 0, 0, 50, 50 ) );
		a->addObject ( s1 );
		asmtest->addChild ( s1, 10 );
	}
	
	for ( KDint i = 0;  i < 10; i++ )
	{
		if ( i != 5 )
		{
			asmtest->reorderChild ( (CCNode*) ( a->objectAtIndex ( i ) ), 9 );
		}
	}
	
	KDint prev = -1;
    CCArray* children = asmtest->getChildren ( );
    CCSprite* child;
    CCObject* pObject = KD_NULL;
    CCARRAY_FOREACH ( children, pObject )
    {
        child = (CCSprite*) pObject;
		if ( !child )
		{
			break;
		}

		KDint currentIndex = child->getAtlasIndex ( );
		CCAssert ( prev == currentIndex-1, "Child order failed" );
		////----UXLOG ( "children %x - atlasIndex:%d", child, currentIndex );
		prev = currentIndex;
	}
	
	prev = -1;
    CCArray* sChildren = asmtest->getDescendants ( );
    CCARRAY_FOREACH ( sChildren, pObject )
    {
        child = (CCSprite*) pObject;
		if ( !child )
		{
			break;
		}

		KDint currentIndex = child->getAtlasIndex ( );
		CCAssert ( prev == currentIndex - 1, "Child order failed" );
		////----UXLOG ( "descendant %x - atlasIndex:%d", child, currentIndex );
		prev = currentIndex;
	}

	a->release ( );	   //memory leak : 2010-0415
}

const KDchar* SpriteBatchNodeReorder::title ( KDvoid )
{
	return "SpriteBatchNode: reorder #1";
}

const KDchar* SpriteBatchNodeReorder::subtitle ( KDvoid )
{
	return "Should not crash";
}

//------------------------------------------------------------------
//
// SpriteZVertex
//
//------------------------------------------------------------------
KDvoid SpriteZVertex::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );
	
	//
	// This test tests z-order
	// If you are going to use it is better to use a 3D projection
	//
	// WARNING:
	// The developer is resposible for ordering it's sprites according to it's Z if the sprite has
	// transparent parts.
	//
	
	m_nDir = 1;
	m_fTime = 0;

	CCSize s = this->getContentSize ( );
	KDfloat step = s.cx / 12;
	
	CCNode* node = CCNode::create ( );
	// camera uses the center of the image as the pivoting point
	node->setContentSize ( CCSizeMake ( s.cx, s.cy ) );
	node->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	node->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );

	addChild ( node, 0 );

	for ( KDint i = 0; i < 5; i++ ) 
	{
		CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
		sprite->setPosition ( ccp ( ( i+1 )*step, s.cy / 2 ) );
		sprite->setVertexZ ( ( KDfloat ) ( 10 + i * 40 ) );
		node->addChild ( sprite, 0 );		
	}
	
	for ( KDint i = 5; i < 11; i++ ) 
	{
		CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 1, 121 * 0, 85, 121 ) );
		sprite->setPosition ( ccp ( (  i + 1 ) * step, s.cy / 2 ) );
		sprite->setVertexZ ( ( KDfloat ) ( 10 + ( 10 - i ) * 40 ) );
		node->addChild ( sprite, 0 );
	}

	node->runAction ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) );
}

KDvoid SpriteZVertex::onExit ( )
{
//	glDisable ( GL_ALPHA_TEST );
	
	TestSprite::onExit ( );
}

const KDchar* SpriteZVertex::title ( KDvoid )
{
	return "Sprite: openGL Z vertex";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZVertex
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeZVertex::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

	//
	// This test tests z-order
	// If you are going to use it is better to use a 3D projection
	//
	// WARNING:
	// The developer is resposible for ordering it's sprites according to it's Z if the sprite has
	// transparent parts.
	//
	
	
	CCSize s = this->getContentSize ( );
	KDfloat step = s.cx / 12;
	
	// small capacity. Testing resizing.
	// Don't use capacity=1 in your real game. It is expensive to resize the capacity
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 1 );
	// camera uses the center of the image as the pivoting point
	batch->setContentSize ( CCSizeMake ( s.cx, s.cy ) );
	batch->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	batch->setPosition ( ccp ( s.cx / 2, s.cy / 2 ) );
	

	addChild ( batch, 0, kTagSpriteBatchNode );		
	
	for ( KDint i = 0; i < 5; i++ ) 
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 0, 121 * 1, 85, 121 ) );
		sprite->setPosition ( ccp ( ( i + 1 ) * step, s.cy / 2 ) );
		sprite->setVertexZ ( ( KDfloat ) ( 10 + i * 40 ) );
		batch->addChild ( sprite, 0 );
		
	}
	
	for ( KDint i = 5; i < 11; i++ )
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 1, 121 * 0, 85, 121 ) );
		sprite->setPosition ( ccp ( ( i + 1 ) * step, s.cy / 2 ) );
		sprite->setVertexZ ( ( KDfloat ) ( 10 + ( 10 - i ) * 40 ) );
		batch->addChild ( sprite, 0 );
	}
	
	batch->runAction ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) );
}

KDvoid SpriteBatchNodeZVertex::onExit ( KDvoid )
{
//	glDisable ( GL_ALPHA_TEST );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeZVertex::title ( KDvoid )
{
	return "SpriteBatchNode: openGL Z vertex";
}


//------------------------------------------------------------------
//
// SpriteAnchorPoint
//
//------------------------------------------------------------------
KDvoid SpriteAnchorPoint::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSize  s = this->getContentSize ( );
		
	CCActionInterval* rotate = CCRotateBy::create ( 10, 360 );
	CCAction* action = CCRepeatForever::create ( rotate );
	
	for ( KDint i = 0; i < 3; i++ ) 
	{
		CCSprite* sprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*i, 121*1, 85, 121 ) );
		sprite->setPosition ( ccp ( s.cx/4* ( i+1 ), s.cy / 2 ) );
		
		CCSprite* point = CCSprite::create ( "Images/r1.png" );
		point->setScale ( 0.25f );
		point->setPosition ( sprite->getPosition ( ) );
		addChild ( point, 10 );
		
		switch ( i ) 
		{
			case 0 : sprite->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );	break;
			case 1 : sprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );	break;
			case 2 : sprite->setAnchorPoint ( ccp ( 1.0f, 1.0f ) ); break;
		}
		
		point->setPosition ( sprite->getPosition ( ) );
		
		CCAction* copy = (CCAction*) ( action->copy ( )->autorelease ( ) );
		sprite->runAction ( copy );
		addChild ( sprite, i );
	}		
}

const KDchar* SpriteAnchorPoint::title ( KDvoid )
{
	return "Sprite: anchor point";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeAnchorPoint
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeAnchorPoint::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	// small capacity. Testing resizing.
	// Don't use capacity=1 in your real game. It is expensive to resize the capacity
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 1 );
	addChild ( batch, 0, kTagSpriteBatchNode );		
	
	CCSize s = this->getContentSize ( );
		
	CCActionInterval* rotate = CCRotateBy::create ( 10, 360 );
	CCAction* action = CCRepeatForever::create ( rotate );
	for ( KDint i = 0; i < 3; i++ ) 
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * i, 121 * 1, 85, 121 ) );
		sprite->setPosition ( ccp ( s.cx / 4 * ( i + 1 ), s.cy / 2 ) );
		
		CCSprite* point = CCSprite::create ( "Images/r1.png" );
		point->setScale ( 0.25f );
		point->setPosition ( sprite->getPosition ( ) );
		addChild ( point, 1 );

		switch ( i ) 
		{
			case 0 : sprite->setAnchorPoint ( ccp ( 0.0f, 0.0f ) ); break;
			case 1 : sprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );	break;
			case 2 : sprite->setAnchorPoint ( ccp ( 1.0f, 1.0f ) ); break;
		}

		point->setPosition ( sprite->getPosition ( ) );
		
		CCAction* copy = (CCAction*) ( action->copy ( )->autorelease ( ) );
		sprite->runAction ( copy );
		batch->addChild ( sprite, i );
	}
}

const KDchar* SpriteBatchNodeAnchorPoint::title ( KDvoid )
{
	return "SpriteBatchNode: anchor point";
}

//------------------------------------------------------------------
//
// Sprite6
//
//------------------------------------------------------------------
KDvoid Sprite6::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	// small capacity. Testing resizing
	// Don't use capacity=1 in your real game. It is expensive to resize the capacity
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 1 );
	addChild ( batch, 0, kTagSpriteBatchNode );
	batch->ignoreAnchorPointForPosition ( KD_TRUE );

	CCSize s = this->getContentSize ( );

	batch->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	batch->setContentSize ( CCSizeMake ( s.cx, s.cy ) );
	
	
	// SpriteBatchNode actions
	CCActionInterval* rotate = CCRotateBy::create ( 5, 360 );
	CCAction* action = CCRepeatForever::create ( rotate );

	// SpriteBatchNode actions
	CCActionInterval* rotate_back = rotate->reverse ( );
	CCActionInterval* rotate_seq = CCSequence::create ( rotate, rotate_back, KD_NULL );
	CCAction* rotate_forever = CCRepeatForever::create ( rotate_seq );
	
	CCActionInterval* scale = CCScaleBy::create ( 5, 1.5f );
	CCActionInterval* scale_back = scale->reverse ( );
	CCActionInterval* scale_seq = CCSequence::create ( scale, scale_back, KD_NULL );
	CCAction* scale_forever = CCRepeatForever::create ( scale_seq );

	KDfloat step = s.cx / 4;

	for ( KDint i = 0; i < 3; i++ ) 
	{
		CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*i, 121*1, 85, 121 ) );
		sprite->setPosition ( ccp ( (  i+1 )*step, s.cy / 2 ) );

		sprite->runAction ( (  CCAction* ) ( action->copy ( )->autorelease ( ) ) );
		batch->addChild ( sprite, i );
	}
	
	batch->runAction ( scale_forever );
	batch->runAction ( rotate_forever );
}

const KDchar* Sprite6::title ( KDvoid )
{
	return "SpriteBatchNode transformation";
}

//------------------------------------------------------------------
//
// SpriteFlip
//
//------------------------------------------------------------------
KDvoid SpriteFlip::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
	
	CCSprite*  sprite1 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	sprite1->setPosition ( ccp ( s.cx / 2 - 100, s.cy / 2 ) );
	addChild ( sprite1, 0, kTagSprite1 );
	
	CCSprite*  sprite2 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	sprite2->setPosition ( ccp ( s.cx / 2 + 100, s.cy / 2 ) );
	addChild ( sprite2, 0, kTagSprite2 );
	
	schedule ( schedule_selector ( SpriteFlip::flipSprites ), 1 );
}

KDvoid SpriteFlip::flipSprites ( KDfloat dt )
{
	CCSprite*  sprite1 = (CCSprite *) getChildByTag ( kTagSprite1 );
	CCSprite*  sprite2 = (CCSprite *) getChildByTag ( kTagSprite2 );
	
	KDbool  x = sprite1->isFlipX ( );
	KDbool  y = sprite2->isFlipY ( );

	sprite1->setFlipX ( !x );
	sprite2->setFlipY ( !y );
}

const KDchar* SpriteFlip::title ( KDvoid )
{
	return "Sprite Flip X & Y";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeFlip
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeFlip::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 10 );
	addChild ( batch, 0, kTagSpriteBatchNode );
	
	CCSize s = this->getContentSize ( );
	
	CCSprite* sprite1 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	sprite1->setPosition ( ccp ( s.cx / 2 - 100, s.cy / 2 ) );
	batch->addChild ( sprite1, 0, kTagSprite1 );
	
	CCSprite* sprite2 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85 * 1, 121 * 1, 85, 121 ) );
	sprite2->setPosition ( ccp ( s.cx / 2 + 100, s.cy / 2 ) );
	batch->addChild ( sprite2, 0, kTagSprite2 );
	
	schedule ( schedule_selector ( SpriteBatchNodeFlip::flipSprites ), 1 );
}

KDvoid SpriteBatchNodeFlip::flipSprites ( KDfloat dt )
{
	CCSpriteBatchNode*  batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );

	CCSprite*  sprite1 = (CCSprite *) batch->getChildByTag ( kTagSprite1 );
	CCSprite*  sprite2 = (CCSprite *) batch->getChildByTag ( kTagSprite2 );
	
	KDbool  x = sprite1->isFlipX ( );
	KDbool  y = sprite2->isFlipY ( );

	sprite1->setFlipX ( !x );
	sprite2->setFlipY ( !y );
}

const KDchar* SpriteBatchNodeFlip::title ( KDvoid )
{
	return "SpriteBatchNode Flip X & Y";
}

//------------------------------------------------------------------
//
// SpriteAliased
//
//------------------------------------------------------------------
KDvoid SpriteAliased::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSize s = this->getContentSize ( );
	
	CCSprite* sprite1 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*1, 121*1, 85, 121 ) );
	sprite1->setPosition ( ccp ( s.cx / 2 - 100, s.cy / 2 ) );
	addChild ( sprite1, 0, kTagSprite1 );
	
	CCSprite* sprite2 = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( 85*1, 121*1, 85, 121 ) );
	sprite2->setPosition ( ccp ( s.cx / 2 + 100, s.cy / 2 ) );
	addChild ( sprite2, 0, kTagSprite2 );
	
	CCActionInterval* scale = CCScaleBy::create ( 2, 5 );
	CCActionInterval* scale_back = scale->reverse ( );
	CCActionInterval* seq = CCSequence::create ( scale, scale_back, KD_NULL );
	CCAction* repeat = CCRepeatForever::create ( seq );
	
	CCAction* repeat2 = (CCAction*) ( repeat->copy ( )->autorelease ( ) );
	
	sprite1->runAction ( repeat );
	sprite2->runAction ( repeat2 );

	//
	// IMPORTANT:
	// This change will affect every sprite that uses the same texture
	// So sprite1 and sprite2 will be affected by this change
	//
	CCSprite* sprite = (CCSprite*) getChildByTag ( kTagSprite1 );
	sprite->getTexture ( )->setAliasTexParameters ( );
}

KDvoid SpriteAliased::onExit ( KDvoid )
{
	// restore the tex parameter to AntiAliased.
	CCSprite* sprite = (CCSprite *)getChildByTag ( kTagSprite1 );
	sprite->getTexture ( )->setAntiAliasTexParameters ( );
	TestSprite::onExit ( );
}

const KDchar* SpriteAliased::title ( KDvoid )
{
	return "Sprite Aliased";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeAliased
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeAliased::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 10 );
	addChild ( batch, 0, kTagSpriteBatchNode );
	
	CCSize s = this->getContentSize ( );

	CCSprite* sprite1 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*1, 121*1, 85, 121 ) );
	sprite1->setPosition ( ccp ( s.cx / 2 - 100, s.cy / 2 ) );
	batch->addChild ( sprite1, 0, kTagSprite1 );
	
	CCSprite* sprite2 = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( 85*1, 121*1, 85, 121 ) );
	sprite2->setPosition ( ccp ( s.cx / 2 + 100, s.cy / 2 ) );
	batch->addChild ( sprite2, 0, kTagSprite2 );
	
	CCActionInterval* scale = CCScaleBy::create ( 2, 5 );
	CCActionInterval* scale_back = scale->reverse ( );
	CCActionInterval* seq = CCSequence::create ( scale, scale_back, KD_NULL );
	CCAction* repeat = CCRepeatForever::create ( seq );
	
	CCAction* repeat2 = (CCAction*) ( repeat->copy ( )->autorelease ( ) );
	
	sprite1->runAction ( repeat );
	sprite2->runAction ( repeat2 );
		
	batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );
	batch->getTexture ( )->setAliasTexParameters ( );
}

KDvoid SpriteBatchNodeAliased::onExit ( )
{
	// restore the tex parameter to AntiAliased.
	CCSpriteBatchNode* batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );
	batch->getTexture ( )->setAntiAliasTexParameters ( );
	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeAliased::title ( KDvoid )
{
	return "SpriteBatchNode Aliased";
}

//------------------------------------------------------------------
//
// SpriteNewTexture
//
//------------------------------------------------------------------
KDvoid SpriteNewTexture::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	setTouchEnabled ( KD_TRUE );
	
	CCNode*  node = CCNode::create ( );
	addChild ( node, 0, kTagSpriteBatchNode );

    m_pTexture1 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossini_dance_atlas.png" );
    m_pTexture1->retain ( );
	m_pTexture2 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossini_dance_atlas-mono.png" );
    m_pTexture2->retain ( );
	
	m_bUsingTexture1 = KD_TRUE;

	for ( KDint i = 0; i < 30; i++ )
	{
		addNewSprite ( );
	}
}

KDvoid SpriteNewTexture::onExit ( KDvoid )
{
	m_pTexture1->release ( );
	m_pTexture2->release ( );

	TestSprite::onExit ( );
}

KDvoid SpriteNewTexture::addNewSprite ( KDvoid )
{
	CCSize s = this->getContentSize ( );

	CCPoint p = ccp ( CCRANDOM_0_1 ( ) * s.cx, CCRANDOM_0_1 ( ) * s.cy );

	KDint idx = (KDint) ( CCRANDOM_0_1 ( ) * 1400 / 100 );
	KDint x = ( idx % 5 ) * 85;
	KDint y = ( idx / 5 ) * 121;
	
	CCNode* node = getChildByTag ( kTagSpriteBatchNode );
	CCSprite* sprite = CCSprite::createWithTexture ( m_pTexture1, CCRectMake ( x, y, 85, 121 ) );
	node->addChild ( sprite );
	
	sprite->setPosition ( ccp ( p.x, p.y ) );
	
	CCActionInterval* action;
	KDfloat random = CCRANDOM_0_1 ( );
	
	if      ( random < 0.20 ) action = CCScaleBy::create ( 3, 2 );
	else if ( random < 0.40 ) action = CCRotateBy::create ( 3, 360 );
	else if ( random < 0.60 ) action = CCBlink::create ( 1, 3 );
	else if ( random < 0.80 ) action = CCTintBy::create ( 2, 0, -255, -255 );
	else                      action = CCFadeOut::create ( 2 );

	CCActionInterval* action_back = action->reverse ( );
	CCActionInterval* seq = CCSequence::create ( action, action_back, KD_NULL );
	
	sprite->runAction ( CCRepeatForever::create ( seq ) );
}

KDvoid SpriteNewTexture::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
	CCNode *node = getChildByTag ( kTagSpriteBatchNode );
	
    CCArray* children = node->getChildren ( );
    CCSprite* sprite;
    CCObject* pObject;

	if ( m_bUsingTexture1 )						  //--> win32 : Let's it make just simple sentence
	{
        CCARRAY_FOREACH ( children, pObject )
        {
            sprite = (CCSprite*) pObject;
			if ( !sprite )
			{
				break;
			}

			sprite->setTexture ( m_pTexture2 );
		}

		m_bUsingTexture1 = KD_FALSE;
	} 
	else 
	{
        CCARRAY_FOREACH ( children, pObject )
        {
            sprite = (CCSprite*) pObject;
			if ( !sprite )
			{
				break;
			}

			sprite->setTexture ( m_pTexture1 );
		}

		m_bUsingTexture1 = KD_TRUE;
	}
}

const KDchar* SpriteNewTexture::title ( KDvoid )
{
	return "Sprite New texture ( tap )";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeNewTexture
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeNewTexture::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	setTouchEnabled ( KD_TRUE );
	
	CCSpriteBatchNode* batch = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 50 );
	addChild ( batch, 0, kTagSpriteBatchNode );
	
	m_pTexture1 = batch->getTexture ( ); m_pTexture1->retain ( );
	m_pTexture2 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossini_dance_atlas-mono.png" );
	m_pTexture2->retain ( );
	
	for ( KDint i = 0; i < 30; i++ )
	{
		addNewSprite ( );
	}
}

KDvoid SpriteBatchNodeNewTexture::onExit ( KDvoid )
{
	m_pTexture1->release ( );
	m_pTexture2->release ( );

	TestSprite::onExit ( );
}

KDvoid SpriteBatchNodeNewTexture::addNewSprite ( KDvoid )
{
	CCSize s = this->getContentSize ( );
	
	CCPoint p = ccp ( CCRANDOM_0_1 ( ) * s.cx, CCRANDOM_0_1 ( ) * s.cy );
	
	CCSpriteBatchNode* batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );
	
	KDint idx = (KDint) ( CCRANDOM_0_1 ( ) * 1400 / 100 );
	KDint x = ( idx % 5 ) * 85;
	KDint y = ( idx / 5 ) * 121;
	
	
	CCSprite* sprite = CCSprite::createWithTexture ( batch->getTexture ( ), CCRectMake ( x, y, 85, 121 ) );
	batch->addChild ( sprite );
	
	sprite->setPosition ( ccp ( p.x, p.y ) );
	
	CCActionInterval* action;
	KDfloat random = CCRANDOM_0_1 ( );
	
	if      ( random < 0.20 ) action = CCScaleBy::create ( 3, 2 );
	else if ( random < 0.40 ) action = CCRotateBy::create ( 3, 360 );
	else if ( random < 0.60 ) action = CCBlink::create ( 1, 3 );
	else if ( random < 0.80 ) action = CCTintBy::create ( 2, 0, -255, -255 );
	else                      action = CCFadeOut::create ( 2 );

	CCActionInterval* action_back = action->reverse ( );
	CCActionInterval* seq = (CCActionInterval*) ( CCSequence::create ( action, action_back, KD_NULL ) );
	
	sprite->runAction ( CCRepeatForever::create ( seq ) );
}

KDvoid SpriteBatchNodeNewTexture::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
	CCSpriteBatchNode* batch = (CCSpriteBatchNode *) getChildByTag ( kTagSpriteBatchNode );
	
	if ( batch->getTexture ( ) == m_pTexture1 )
	{
		batch->setTexture ( m_pTexture2 );
	}
	else
	{
		batch->setTexture ( m_pTexture1 );	
	}
}

const KDchar* SpriteBatchNodeNewTexture::title ( KDvoid )
{
	return "SpriteBatchNode new texture ( tap )";
}


//------------------------------------------------------------------
//
// SpriteFrameTest
//
//------------------------------------------------------------------
KDvoid SpriteFrameTest::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSize s = this->getContentSize ( );

	// IMPORTANT:
	// The sprite frames will be cached AND RETAINED, and they won't be released unless you call
	//     CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames );
	CCSpriteFrameCache*      cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    cache->addSpriteFramesWithFile ( "animations/grossini.plist" );
	cache->addSpriteFramesWithFile ( "animations/grossini_gray.plist", "animations/grossini_gray.png" );
	cache->addSpriteFramesWithFile ( "animations/grossini_blue.plist", "animations/grossini_blue.png" );

	//
	// Animation using Sprite Sheet
	//
	m_pSprite1 = CCSprite::createWithSpriteFrameName ( "grossini_dance_01.png" );
	m_pSprite1->setPosition ( ccp ( s.cx / 2 - 80, s.cy / 2 ) );
	
	CCSpriteBatchNode*  batch = CCSpriteBatchNode::create ( "animations/grossini.png" );
	batch->addChild ( m_pSprite1 );
	addChild ( batch );

	CCArray* animFrames = CCArray::createWithCapacity ( 15 );

    KDchar str[100] = { 0 };
	for ( KDint i = 1; i < 15; i++ ) 
	{
        kdSprintfKHR ( str, "grossini_dance_%02d.png", i );
		CCSpriteFrame* frame = cache->spriteFrameByName ( str );
		animFrames->addObject ( frame );
	}

    CCAnimation* animation = CCAnimation::createWithSpriteFrames ( animFrames, 0.3f );
    m_pSprite1->runAction( CCRepeatForever::create ( CCAnimate::create(animation) ) );

    // to test issue #732, uncomment the following line
    m_pSprite1->setFlipX ( KD_FALSE );
    m_pSprite1->setFlipY ( KD_FALSE );

    //
    // Animation using standard Sprite
    //
    m_pSprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    m_pSprite2->setPosition( ccp( s.cx/2 + 80, s.cy/2) );
    addChild(m_pSprite2);


    CCArray* moreFrames = CCArray::createWithCapacity(20);
    for(int i = 1; i < 15; i++) 
    {
        kdSprintf(str, "grossini_dance_gray_%02d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }


    for( int i = 1; i < 5; i++) {
        kdSprintf(str, "grossini_blue_%02d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }

    // append frames from another batch
    moreFrames->addObjectsFromArray(animFrames);
    CCAnimation *animMixed = CCAnimation::createWithSpriteFrames(moreFrames, 0.3f);


    m_pSprite2->runAction(CCRepeatForever::create( CCAnimate::create(animMixed) ) );


    // to test issue #732, uncomment the following line
    m_pSprite2->setFlipX ( KD_FALSE );
    m_pSprite2->setFlipY ( KD_FALSE );

    schedule(schedule_selector(SpriteFrameTest::startIn05Secs), 0.5f);
    m_nCounter = 0;
}

KDvoid SpriteFrameTest::onExit ( KDvoid )
{
    CCSpriteFrameCache*   cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_blue.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteFrameTest::title ( KDvoid )
{
	return "Sprite vs. SpriteBatchNode animation";
}

const KDchar* SpriteFrameTest::subtitle ( KDvoid )
{
    return "Testing issue #792";
}

KDvoid SpriteFrameTest::startIn05Secs ( KDfloat dt )
{
    unschedule ( schedule_selector ( SpriteFrameTest::startIn05Secs ) );
    schedule ( schedule_selector ( SpriteFrameTest::flipSprites ), 1.0f );
}

KDvoid SpriteFrameTest::flipSprites ( KDfloat dt )
{
    m_nCounter++;

    KDbool fx = KD_FALSE;
    KDbool fy = KD_FALSE;
    KDint  i  = m_nCounter % 4;

    switch (  i ) 
	{
        case 0:
            fx = KD_FALSE;
            fy = KD_FALSE;
            break;
        case 1:
            fx = KD_TRUE;
            fy = KD_FALSE;
            break;
        case 2:
            fx = KD_FALSE;
            fy = KD_TRUE;
            break;
        case 3:
            fx = KD_TRUE;
            fy = KD_TRUE;
            break;
    }

    m_pSprite1->setFlipX ( fx );
    m_pSprite1->setFlipY ( fy );
    m_pSprite2->setFlipX ( fx );
    m_pSprite2->setFlipY ( fy );
    //NSLog ( @"flipX:%d, flipY:%d", fx, fy );
}


//------------------------------------------------------------------
//
// SpriteFrameAliasNameTest
//
//------------------------------------------------------------------
KDvoid SpriteFrameAliasNameTest::onEnter ( KDvoid )
{
    TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     [[CCSpriteFrameCache sharedSpriteFrameCache] removeUnusedSpriteFrames];
    //
    // CCSpriteFrameCache is a cache of CCSpriteFrames
    // CCSpriteFrames each contain a texture id and a rect (frame).

    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    pCache->addSpriteFramesWithFile ( "animations/grossini-aliases.plist", "animations/grossini-aliases.png" );

    //
    // Animation using Sprite batch
    //
    // A CCSpriteBatchNode can reference one and only one texture (one .png file)
    // Sprites that are contained in that texture can be instantiatied as CCSprites and then added to the CCSpriteBatchNode
    // All CCSprites added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call
    // If the CCSprites are not added to a CCSpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient
    //
    // When you animate a sprite, CCAnimation changes the frame of the sprite using setDisplayFrame: (this is why the animation must be in the same texture)
    // When setDisplayFrame: is used in the CCAnimation it changes the frame to one specified by the CCSpriteFrames that were added to the animation,
    // but texture id is still the same and so the sprite is still a child of the CCSpriteBatchNode, 
    // and therefore all the animation sprites are also drawn as part of the CCSpriteBatchNode
    //

    CCSprite*  pSprite = CCSprite::createWithSpriteFrameName ( "grossini_dance_01.png" );
    pSprite->setPosition ( ccp ( s.cx * 0.5f, s.cy * 0.5f ) );

    CCSpriteBatchNode*  pSpriteBatch = CCSpriteBatchNode::create ( "animations/grossini-aliases.png" );
    pSpriteBatch->addChild ( pSprite );
    addChild ( pSpriteBatch );

    CCArray*   pAnimFrames = CCArray::createWithCapacity ( 15 );
    for ( KDint i = 1; i < 15; i++ )
    {
        // Obtain frames by alias name
        CCSpriteFrame*  pFrame = pCache->spriteFrameByName ( ccszf ( "dance_%02d", i ) );
        pAnimFrames->addObject ( pFrame );
    }

    CCAnimation*  pAnimation = CCAnimation::createWithSpriteFrames ( pAnimFrames, 0.3f );
    // 14 frames * 1sec = 14 seconds
    pSprite->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
}

KDvoid SpriteFrameAliasNameTest::onExit ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeSpriteFramesFromFile ( "animations/grossini.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteFrameAliasNameTest::title ( KDvoid )
{
    return "SpriteFrame Alias Name";
}

const KDchar* SpriteFrameAliasNameTest::subtitle ( KDvoid )
{
    return "SpriteFrames are obtained using the alias name";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorRotation
//
//------------------------------------------------------------------
KDvoid SpriteOffsetAnchorRotation::onEnter ( KDvoid )
{
    TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );  

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for(KDint i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( ccp( s.cx/4*(i+1), s.cy/2) );

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);

        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( CCPointZero );
                break;
            case 1:
                sprite->setAnchorPoint( ccp(0.5f, 0.5f) );
                break;
            case 2:
                sprite->setAnchorPoint( ccp(1,1) );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        CCArray* animFrames = CCArray::createWithCapacity(14);
        char str[100] = {0};
        for(int i = 0; i < 14; i++) 
        {
            kdSprintf(str, "grossini_dance_%02d.png",(i+1));
            CCSpriteFrame *frame = cache->spriteFrameByName(str);
            animFrames->addObject(frame);
        }

        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create( CCAnimate::create(animation) ) );            
        sprite->runAction(CCRepeatForever::create(CCRotateBy::create(10, 360) ) );

        addChild(sprite, 0);

    }        
}

KDvoid SpriteOffsetAnchorRotation::onExit ( KDvoid )
{
	TestSprite::onExit ( );

    CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );
}

const KDchar* SpriteOffsetAnchorRotation::title ( KDvoid )
{
	return "Sprite offset + anchor + rot";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorRotation
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeOffsetAnchorRotation::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    for(KDint i=0;i<3;i++) 
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
        
        //
        // Animation using Sprite BatchNode
        //
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( ccp( s.cx/4*(i+1), s.cy/2));
        
        CCSprite* point = CCSprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( CCPointZero );
                break;
            case 1:
                sprite->setAnchorPoint( ccp(0.5f, 0.5f) );
                break;
            case 2:
                sprite->setAnchorPoint( ccp(1,1) );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
        addChild(spritebatch);
        
        CCArray* animFrames = CCArray::createWithCapacity(14);
        char str[100] = {0};
        for(int k = 0; k < 14; k++) 
        {
            kdSprintf(str, "grossini_dance_%02d.png",(k+1));
            CCSpriteFrame *frame = cache->spriteFrameByName(str);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create( CCAnimate::create(animation) ));
        sprite->runAction(CCRepeatForever::create(CCRotateBy::create(10, 360) ));
        
        spritebatch->addChild(sprite, i);
    }        
}


KDvoid SpriteBatchNodeOffsetAnchorRotation::onExit ( KDvoid )
{
	TestSprite::onExit ( );

    CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );
}

const KDchar* SpriteBatchNodeOffsetAnchorRotation::title ( KDvoid )
{
	return "SpriteBatchNode offset + anchor + rot";
}


//------------------------------------------------------------------
//
// SpriteOffsetAnchorScale
//
//------------------------------------------------------------------
KDvoid SpriteOffsetAnchorScale::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    for(KDint i=0;i<3;i++) 
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
        
        //
        // Animation using Sprite BatchNode
        //
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( ccp( s.cx/4*(i+1), s.cy/2) );
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( CCPointZero );
                break;
            case 1:
                sprite->setAnchorPoint( ccp(0.5f, 0.5f) );
                break;
            case 2:
                sprite->setAnchorPoint( ccp(1,1) );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        CCArray* animFrames = CCArray::createWithCapacity(14);
        KDchar str[100] = {0};
        for(KDint i = 0; i < 14; i++) 
        {
            kdSprintf(str, "grossini_dance_%02d.png",(i+1));
            CCSpriteFrame *frame = cache->spriteFrameByName(str);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create( CCAnimate::create(animation) ));            
        
        CCActionInterval* scale = CCScaleBy::create(2, 2);
        CCActionInterval* scale_back = scale->reverse();
        CCActionInterval* seq_scale = CCSequence::create(scale, scale_back, KD_NULL );
        sprite->runAction(CCRepeatForever::create(seq_scale));
        
        addChild(sprite, 0);
    }        
}

KDvoid SpriteOffsetAnchorScale::onExit ( KDvoid )
{
	TestSprite::onExit ( );

    CCSpriteFrameCache*   cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );
}

const KDchar* SpriteOffsetAnchorScale::title ( KDvoid )
{
	return "Sprite offset + anchor + scale";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorScale
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeOffsetAnchorScale::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    for(int i=0;i<3;i++) 
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
        
        //
        // Animation using Sprite BatchNode
        //
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( ccp( s.cx/4*(i+1), s.cy/2) );
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) {
            case 0:
                sprite->setAnchorPoint( CCPointZero );
                break;
            case 1:
                sprite->setAnchorPoint( ccp(0.5f, 0.5f) );
                break;
            case 2:
                sprite->setAnchorPoint( ccp(1,1) );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        CCSpriteBatchNode *spritesheet = CCSpriteBatchNode::create("animations/grossini.png");
        addChild(spritesheet);
        
        CCArray* animFrames = CCArray::createWithCapacity(14);
        char str[100] = {0};
        for(int k = 0; k < 14; k++) 
        {
            kdSprintf(str, "grossini_dance_%02d.png",(k+1));
            CCSpriteFrame *frame = cache->spriteFrameByName(str);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create( CCAnimate::create(animation) ) );

        CCActionInterval* scale = CCScaleBy::create(2, 2);
        CCActionInterval* scale_back = scale->reverse();
        CCActionInterval* seq_scale = CCSequence::create(scale, scale_back, KD_NULL );
        sprite->runAction(CCRepeatForever::create(seq_scale) );
        
        spritesheet->addChild(sprite, i);
    }        
}

KDvoid SpriteBatchNodeOffsetAnchorScale::onExit ( KDvoid )
{
	TestSprite::onExit ( );

    CCSpriteFrameCache*   cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );
}

const KDchar* SpriteBatchNodeOffsetAnchorScale::title ( KDvoid )
{
	return "SpriteBatchNode offset + anchor + scale";
}

//------------------------------------------------------------------
//
// SpriteAnimationSplit
//
//------------------------------------------------------------------
KDvoid SpriteAnimationSplit::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );
    
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("animations/dragon_animation.png");
    
    // manually add frames to the frame cache
    CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*0, 132*0, 132, 132));
    CCSpriteFrame *frame1 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*1, 132*0, 132, 132));
    CCSpriteFrame *frame2 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*2, 132*0, 132, 132));
    CCSpriteFrame *frame3 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*3, 132*0, 132, 132));
    CCSpriteFrame *frame4 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*0, 132*1, 132, 132));
    CCSpriteFrame *frame5 = CCSpriteFrame::createWithTexture(texture, CCRectMake(132*1, 132*1, 132, 132));
    
    
    //
    // Animation using Sprite BatchNode
    //
    CCSprite* sprite = CCSprite::createWithSpriteFrame(frame0);
    sprite->setPosition( ccp( s.cx/2-80, s.cy/2) );
    addChild(sprite);
            
    CCArray* animFrames = CCArray::createWithCapacity(6);
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    animFrames->addObject(frame2);
    animFrames->addObject(frame3);
    animFrames->addObject(frame4);
    animFrames->addObject(frame5);
            
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
    CCAnimate *animate = CCAnimate::create(animation);
    CCActionInterval* seq = CCSequence::create( animate,
                       CCFlipX::create ( KD_TRUE ),
                       animate->copy()->autorelease(),
                       CCFlipX::create ( KD_FALSE ),
                       KD_NULL ) ;
    
    sprite->runAction(CCRepeatForever::create( seq ) );
}

KDvoid SpriteAnimationSplit::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteAnimationSplit::title ( KDvoid )
{
	return "Sprite: Animation + flip";
}

//------------------------------------------------------------------
//
// SpriteHybrid
//
//------------------------------------------------------------------
KDvoid SpriteHybrid::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    // parents
    CCNode *parent1 = CCNode::create();
    CCSpriteBatchNode *parent2 = CCSpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(parent1, 0, kTagNode);
    addChild(parent2, 0, kTagSpriteBatchNode);
    
    
    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");
    
    
    // create 250 sprites
    // only show 80% of them
    for(KDint i = 0; i < 250; i++) 
    {
        KDint spriteIdx = (KDint) (CCRANDOM_0_1() * 14);
        KDchar str[25] = {0};
        kdSprintf(str, "grossini_dance_%02d.png", (spriteIdx+1));
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        CCSprite* sprite = CCSprite::createWithSpriteFrame(frame);
        parent1->addChild(sprite, i, i);
        
        KDfloat x=-1000;
        KDfloat y=-1000;
        if( CCRANDOM_0_1() < 0.2f ) 
        {
            x = CCRANDOM_0_1() * s.cx;
            y = CCRANDOM_0_1() * s.cy;
        }
        sprite->setPosition( ccp(x,y) );
            
        CCActionInterval* action = CCRotateBy::create(4, 360);
        sprite->runAction( CCRepeatForever::create(action) );
    }
    
    m_bUsingSpriteBatchNode = false;
    
    schedule( schedule_selector(SpriteHybrid::reparentSprite), 2);
}

KDvoid SpriteHybrid::reparentSprite ( KDfloat dt )
{
	CCNode* p1 = getChildByTag ( kTagNode );
	CCNode* p2 = getChildByTag ( kTagSpriteBatchNode );
	
	CCArray* retArray = CCArray::createWithCapacity ( 250 );

	if ( m_bUsingSpriteBatchNode )
	{
		CC_SWAP ( p1, p2, CCNode* );
	}
	
	CCNode* node;
	CCObject* pObject;
    CCArray* children = p1->getChildren ( );
    CCARRAY_FOREACH ( children, pObject )
    {
        node = (CCNode*) pObject;
		if ( !node )
		{
			break;
		}

		retArray->addObject ( node );
	}

	KDint i = 0;
	p1->removeAllChildrenWithCleanup ( KD_FALSE );

    CCARRAY_FOREACH ( retArray, pObject )
    {
        node = (CCNode*) pObject;
		if ( !node )
		{
			break;
		}

		p2->addChild ( node, i, i );
		i++;
	}

	m_bUsingSpriteBatchNode = !m_bUsingSpriteBatchNode;
}

KDvoid SpriteHybrid::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteHybrid::title ( KDvoid )
{
	return "HybrCCSprite* sprite Test";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildren
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeChildren::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );
    
    // parents
    CCSpriteBatchNode* batch = CCSpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(batch, 0, kTagSpriteBatchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");
    
    CCSprite* sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp( s.cx/3, s.cy/2));
    
    CCSprite* sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(50,50));
    
    CCSprite* sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-50,-50));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2);
    sprite1->addChild(sprite3);
    
    // BEGIN NEW CODE
    CCArray *animFrames = CCArray::create ( );
    char str[100] = {0};
    for(int i = 1; i < 15; i++) 
    {
        kdSprintf(str, "grossini_dance_%02d.png",i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        animFrames->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
    sprite1->runAction(CCRepeatForever::create( CCAnimate::create(animation) ) );
    // END NEW CODE
    
    CCActionInterval* action = CCMoveBy::create(2, ccp(200,0));
    CCActionInterval* action_back = action->reverse();
    CCActionInterval* action_rot = CCRotateBy::create(2, 360);
    CCActionInterval* action_s = CCScaleBy::create(2, 2);
    CCActionInterval* action_s_back = action_s->reverse();
    
    CCActionInterval* seq2 = action_rot->reverse();
    sprite2->runAction( CCRepeatForever::create(seq2) );
    
    sprite1->runAction ( CCRepeatForever::create ( action_rot ) );
    sprite1->runAction ( CCRepeatForever::create ( CCSequence::create ( action, action_back, KD_NULL ) ) );
    sprite1->runAction ( CCRepeatForever::create ( CCSequence::create ( action_s, action_s_back, KD_NULL ) ) );

		
}

KDvoid SpriteBatchNodeChildren::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteBatchNodeChildren::title ( KDvoid )
{
	return "SpriteBatchNode Grand Children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenZ
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeChildrenZ::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );
    
    // parents
    CCSpriteBatchNode* batch;
    CCSprite* sprite1, *sprite2, *sprite3;

    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");
    
    // test 1
    batch = CCSpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp( s.cx/3, s.cy/2));
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, 2);
    sprite1->addChild(sprite3, -2);
    
    // test 2
    batch = CCSpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp( 2*s.cx/3, s.cy/2));
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    // test 3
    batch = CCSpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp( s.cx/2 - 90, s.cy/4));
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp( s.cx/2 - 60,s.cy/4));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp( s.cx/2 - 30, s.cy/4));
    
    batch->addChild(sprite1, 10);
    batch->addChild(sprite2, -10);
    batch->addChild(sprite3, -5);

    // test 4
    batch = CCSpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp( s.cx/2 +30, s.cy/4));
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp( s.cx/2 +60,s.cy/4));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp( s.cx/2 +90, s.cy/4));
    
    batch->addChild(sprite1, -10);
    batch->addChild(sprite2, -5);
    batch->addChild(sprite3, -2);
}

KDvoid SpriteBatchNodeChildrenZ::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteBatchNodeChildrenZ::title ( KDvoid )
{
	return "SpriteBatchNode Children Z";
}

//------------------------------------------------------------------
//
// SpriteChildrenVisibility
//
//------------------------------------------------------------------
KDvoid SpriteChildrenVisibility::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");

    CCNode *aParent;
    CCSprite* sprite1, *sprite2, *sprite3;
    //
    // SpriteBatchNode
    //
    // parents
    aParent = CCSpriteBatchNode::create("animations/grossini.png", 50);
    aParent->setPosition( ccp(s.cx/3, s.cy/2) );
    addChild(aParent, 0);
    
    
    
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp(0,0));
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(CCBlink::create(5, 10));
    
    //
    // Sprite
    //
    aParent = CCNode::create();
    aParent->setPosition( ccp(2*s.cx/3, s.cy/2) );
    addChild(aParent, 0);

    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(ccp(0,0));
            
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(CCBlink::create(5, 10));
}

KDvoid SpriteChildrenVisibility::onExit ( )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteChildrenVisibility::title ( KDvoid )
{
	return "Sprite & SpriteBatchNode Visibility";
}

//------------------------------------------------------------------
//
// SpriteChildrenAnchorPoint
//
//------------------------------------------------------------------
KDvoid SpriteChildrenAnchorPoint::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");
    
    CCNode *aParent;
    CCSprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = CCNode::create();
    addChild(aParent, 0);
    
    // anchor (0,0)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/4,s.cy/2));
    sprite1->setAnchorPoint( ccp(0,0) );

    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );

    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    
    // anchor (0.5, 0.5)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/2,s.cy/2));
    sprite1->setAnchorPoint( ccp(0.5f, 0.5f) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));

    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));

    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );        

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/2+s.cx/4,s.cy/2));
    sprite1->setAnchorPoint( ccp(1,1) );

    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

KDvoid SpriteChildrenAnchorPoint::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteChildrenAnchorPoint::title ( KDvoid )
{
	return "Sprite: children + anchor";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenAnchorPoint
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeChildrenAnchorPoint::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini.plist");
    
    CCNode *aParent;
    CCSprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = CCSpriteBatchNode::create("animations/grossini.png", 50);
    addChild(aParent, 0);
    
    // anchor (0,0)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/4,s.cy/2));
    sprite1->setAnchorPoint( ccp(0,0) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    
    // anchor (0.5, 0.5)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/2,s.cy/2));
    sprite1->setAnchorPoint( ccp(0.5f, 0.5f) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(ccp(s.cx/2+s.cx/4,s.cy/2));
    sprite1->setAnchorPoint( ccp(1,1) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(ccp(20,30));
    
    sprite3 = CCSprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(ccp(-20,30));
    
    sprite4 = CCSprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(ccp(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = CCSprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

KDvoid SpriteBatchNodeChildrenAnchorPoint::onExit ( KDvoid )
{
	TestSprite::onExit ( );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeUnusedSpriteFrames ( );
}

const KDchar* SpriteBatchNodeChildrenAnchorPoint::title ( KDvoid )
{
	return "SpriteBatchNode: children + anchor";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenScale
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeChildrenScale::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/grossini_family.plist");

    CCNode *aParent;
    CCSprite* sprite1, *sprite2;
    CCActionInterval* rot = CCRotateBy::create(10, 360);
    CCAction* seq = CCRepeatForever::create(rot);
    
    //
    // Children + Scale using Sprite
    // Test 1
    //
    aParent = CCNode::create();
    sprite1 = CCSprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( ccp( s.cx/4, s.cy/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction(seq);
    
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( ccp( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using SpriteBatchNode
    // Test 2
    //
    
    aParent = CCSpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = CCSprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( ccp( 3*s.cx/4, s.cy/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction( (CCAction*)(seq->copy()->autorelease()) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( ccp( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using Sprite
    // Test 3
    //
    
    aParent = CCNode::create();
    sprite1 = CCSprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( ccp( s.cx/4, 2*s.cy/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f );
    sprite1->runAction( (CCAction*)(seq->copy()->autorelease()) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( ccp( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
    
    //
    // Children + Scale using Sprite
    // Test 4
    //
    
    aParent = CCSpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = CCSprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( ccp( 3*s.cx/4, 2*s.cy/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f);
    sprite1->runAction( (CCAction*)(seq->copy()->autorelease()) );
    
    sprite2 = CCSprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( ccp( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
	
}

const KDchar* SpriteBatchNodeChildrenScale::title ( KDvoid )
{
	return "Sprite/Sheet + child + scale + rot";
}

//------------------------------------------------------------------
//
// SpriteChildrenChildren
//
//------------------------------------------------------------------
KDvoid SpriteChildrenChildren::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/ghosts.plist");
    
    CCNode *aParent;
    CCSprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    CCActionInterval* rot = CCRotateBy::create(10, 360);
    CCAction* seq = CCRepeatForever::create(rot);
    
    CCActionInterval* rot_back = rot->reverse();
    CCAction* rot_back_fe = CCRepeatForever::create(rot_back);
    
    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = CCNode::create();
    addChild(aParent);
    
    // parent
    l1 = CCSprite::createWithSpriteFrameName("father.gif");
    l1->setPosition( ccp( s.cx/2, s.cy/2) );
    l1->runAction( (CCAction*)(seq->copy()->autorelease()) );
    aParent->addChild(l1);
    CCSize l1Size = l1->getContentSize();
    
    // child left
    l2a = CCSprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition( ccp( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( (CCAction*)(rot_back_fe->copy()->autorelease()) );
    l1->addChild(l2a);
    CCSize l2aSize = l2a->getContentSize();        
    
    
    // child right
    l2b = CCSprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition( ccp( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( (CCAction*)(rot_back_fe->copy()->autorelease()) );
    l1->addChild(l2b);
    CCSize l2bSize = l2a->getContentSize();        
    
    
    // child left bottom
    l3a1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale( 0.45f );
    l3a1->setPosition( ccp(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale( 0.45f );
    l3a1->setPosition( ccp(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale( 0.45f);
    l3b1->setFlipY( true );
    l3b1->setPosition( ccp(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);
    
    // child right top
    l3b2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale( 0.45f );
    l3b2->setFlipY( true );
    l3b1->setPosition( ccp(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
}

const KDchar* SpriteChildrenChildren::title ( KDvoid )
{
	return "Sprite multiple levels of children";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenChildren
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeChildrenChildren::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );        
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/ghosts.plist");
    
    CCSpriteBatchNode *aParent;
    CCSprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    CCActionInterval* rot = CCRotateBy::create(10, 360);
    CCAction* seq = CCRepeatForever::create(rot);
    
    CCActionInterval* rot_back = rot->reverse();
    CCAction* rot_back_fe = CCRepeatForever::create(rot_back);
    
    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = CCSpriteBatchNode::create("animations/ghosts.png");
    aParent->getTexture()->generateMipmap();
    addChild(aParent);
    
    // parent
    l1 = CCSprite::createWithSpriteFrameName("father.gif");
    l1->setPosition( ccp( s.cx/2, s.cy/2) );
    l1->runAction( (CCAction*)(seq->copy()->autorelease()) );
    aParent->addChild(l1);
    CCSize l1Size = l1->getContentSize();

    // child left
    l2a = CCSprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition( ccp( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( (CCAction*)(rot_back_fe->copy()->autorelease()) );
    l1->addChild(l2a);
    CCSize l2aSize = l2a->getContentSize();        


    // child right
    l2b = CCSprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition( ccp( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( (CCAction*)(rot_back_fe->copy()->autorelease()) );
    l1->addChild(l2b);
    CCSize l2bSize = l2a->getContentSize();        

    
    // child left bottom
    l3a1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale( 0.45f );
    l3a1->setPosition( ccp(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale( 0.45f );
    l3a1->setPosition( ccp(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale( 0.45f );
    l3b1->setFlipY( true );
    l3b1->setPosition( ccp(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);

    // child right top
    l3b2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale( 0.45f );
    l3b2->setFlipY( true );
    l3b1->setPosition( ccp(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
}

const KDchar* SpriteBatchNodeChildrenChildren::title ( KDvoid )
{
	return "SpriteBatchNode multiple levels of children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeSkewNegativeScaleChildren
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeSkewNegativeScaleChildren::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for ( KDint i = 0; i < 2; i++ )
    {
        CCSprite*  sprite = CCSprite::createWithSpriteFrameName ( "grossini_dance_01.png" );
        sprite->setPosition ( ccp ( s.cx / 3 * ( i == 0 ? 1 : 2 ), s.cy / 2 ) );

        // Skew
        CCSkewBy* skewX = CCSkewBy::create(2, 45, 0);
        CCSkewBy* skewX_back = (CCSkewBy*)skewX->reverse();
        CCSkewBy* skewY = CCSkewBy::create(2, 0, 45);
        CCSkewBy* skewY_back = (CCSkewBy*)skewY->reverse();

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        CCSequence* seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction ( CCRepeatForever::create ( seq_skew ) );

        CCSprite *child1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        child1->setPosition(ccp(sprite->getContentSize().width / 2.0f, sprite->getContentSize().height / 2.0f));

        child1->setScale(0.8f);

        sprite->addChild(child1);

        spritebatch->addChild(sprite, i);
    }
}

KDvoid SpriteBatchNodeSkewNegativeScaleChildren::onExit ( KDvoid )
{
    CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeSkewNegativeScaleChildren::title ( KDvoid )
{
    return "SpriteBatchNode + children + skew";
}

const KDchar* SpriteBatchNodeSkewNegativeScaleChildren::subtitle ( KDvoid )
{
    return "SpriteBatchNode skew + negative scale with children";
}

//------------------------------------------------------------------
//
// SpriteSkewNegativeScaleChildren
//
//------------------------------------------------------------------
KDvoid SpriteSkewNegativeScaleChildren::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    CCNode *parent = CCNode::create();
    addChild(parent);

    for ( KDint i = 0; i < 2; i++ )
    {
        CCSprite*  sprite = CCSprite::createWithSpriteFrameName ( "grossini_dance_01.png" );
        sprite->setPosition ( ccp ( s.cx / 3 * ( i == 0 ? 1 : 2 ), s.cy / 2 ) );

        // Skew
        CCSkewBy* skewX = CCSkewBy::create(2, 45, 0);
        CCSkewBy* skewX_back = (CCSkewBy*)skewX->reverse();
        CCSkewBy* skewY = CCSkewBy::create(2, 0, 45);
        CCSkewBy* skewY_back = (CCSkewBy*)skewY->reverse();

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        CCSequence* seq_skew = CCSequence::create ( skewX, skewX_back, skewY, skewY_back, KD_NULL );
        sprite->runAction(CCRepeatForever::create ( seq_skew ) );

        CCSprite *child1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        child1->setPosition(ccp(sprite->getContentSize().cx / 2.0f, sprite->getContentSize().cy / 2.0f));

        sprite->addChild(child1);

        child1->setScale(0.8f);

        parent->addChild(sprite, i);
    }
}

KDvoid SpriteSkewNegativeScaleChildren::onExit ( KDvoid )
{
    CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteSkewNegativeScaleChildren::title ( KDvoid )
{
    return "Sprite + children + skew";
}

const KDchar* SpriteSkewNegativeScaleChildren::subtitle ( KDvoid )
{
    return "Sprite skew + negative scale with children";
}


//------------------------------------------------------------------
//
// SpriteNilTexture
//
//------------------------------------------------------------------
KDvoid SpriteNilTexture::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	CCSize s = this->getContentSize ( );

	CCSprite*  sprite = KD_NULL;
	
	// TEST: If no texture is given, then Opacity + Color should work.

	sprite = new CCSprite ( );
	sprite->init ( );
	sprite->setTextureRect ( CCRect ( 0, 0, 300, 300 ) );
	sprite->setColor ( ccRED );
	sprite->setOpacity ( 128 );
	sprite->setPosition ( ccp ( 3 * s.cx / 4, s.cy / 2 ) );
	addChild ( sprite, 100 );
    sprite->release ( );

	sprite = new CCSprite ( );
	sprite->init ( );
	sprite->setTextureRect ( CCRect ( 0, 0, 300, 300 ) );
	sprite->setColor ( ccBLUE );
	sprite->setOpacity ( 128 );
	sprite->setPosition ( ccp ( 1 * s.cx / 4, s.cy / 2 ) );
	addChild ( sprite, 100 );
    sprite->release ( );
}

const KDchar* SpriteNilTexture::title ( KDvoid )
{
	return "Sprite without texture";
}

const KDchar* SpriteNilTexture::subtitle ( KDvoid )
{
	return "opacity and color should work";
}

class MySprite1 : public CCSprite
{
	public :

		MySprite1 ( KDvoid ) : ivar ( 10 ) { }

		static MySprite1*  createWithSpriteFrameName ( const KDchar* pszSpriteFrameName )
		{
			CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( pszSpriteFrameName );
			MySprite1*  pobSprite = new MySprite1 ( );
			pobSprite->initWithSpriteFrame ( pFrame );
			pobSprite->autorelease ( );

			return pobSprite;
		}

	private :

		KDint    ivar;
};

class MySprite2 : public CCSprite
{
	public :

		MySprite2 ( KDvoid ) : ivar ( 10 ) { }
		
		static MySprite2* spriteWithFile ( const KDchar* pszName )
		{
			MySprite2* pobSprite = new MySprite2 ( );
			pobSprite->initWithFile ( pszName );
			pobSprite->autorelease ( );
			return pobSprite;
		}

	private :

		KDint    ivar;
};

//------------------------------------------------------------------
//
// SpriteSubclass
//
//------------------------------------------------------------------
KDvoid SpriteSubclass::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

   const CCSize  s = this->getContentSize ( );

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/ghosts.plist");
    CCSpriteBatchNode *aParent = CCSpriteBatchNode::create("animations/ghosts.png");

    // MySprite1
    MySprite1 *sprite = MySprite1::createWithSpriteFrameName("father.gif");
    sprite->setPosition(ccp( s.cx/4*1, s.cy/2));
    aParent->addChild(sprite);
    addChild(aParent);

    // MySprite2
    MySprite2 *sprite2 = MySprite2::spriteWithFile("Images/grossini.png");
    addChild(sprite2);
    sprite2->setPosition(ccp(s.cx/4*3, s.cy/2));
}

const KDchar* SpriteSubclass::title ( KDvoid )
{
    return "Sprite subclass";
}

const KDchar* SpriteSubclass::subtitle ( KDvoid )
{
    return "Testing initWithTexture:rect method";
}

class DoubleSprite : public CCSprite
{
	public :
		DoubleSprite() {m_bHD = false;}

		virtual KDbool initWithTexture(CCTexture2D* texture, CCRect rect);
		// Called everytime the vertex needs to be updated.
		virtual KDvoid setContentSize(const CCSize& size);
		// rect used only for the vertex. Called everytime the vertex needs to be updated.
		virtual KDvoid setVertexRect(CCRect rect);

		static DoubleSprite* spriteWithFile(const KDchar* pszFileName);
		KDbool m_bHD;
};

DoubleSprite* DoubleSprite::spriteWithFile(const char* pszFileName)
{
    DoubleSprite* pSp = new DoubleSprite();
    pSp->initWithFile(pszFileName);
    pSp->autorelease();
    return pSp;
}

KDbool DoubleSprite::initWithTexture(CCTexture2D* texture, CCRect rect)
{
    if( CCSprite::initWithTexture(texture, rect)) 
    {
        return true;
    }

    return false;
}

// Called everytime the vertex needs to be updated.
KDvoid DoubleSprite::setContentSize(const CCSize& size)
{
    CCSize s = size;

    CCSprite::setContentSize(size);
}

// rect used only for the vertex. Called everytime the vertex needs to be updated.
KDvoid DoubleSprite::setVertexRect(CCRect rect)
{

    CCSprite::setVertexRect(rect);
}

//------------------------------------------------------------------
//
// SpriteDoubleResolution
//
//------------------------------------------------------------------
KDvoid SpriteDoubleResolution::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    //
    // LEFT: SD sprite
    //
    // there is no HD resolution file of grossini_dance_08.
    DoubleSprite *spriteSD = DoubleSprite::spriteWithFile("Images/grossini_dance_08.png");
    addChild(spriteSD);
    spriteSD->setPosition(ccp(s.cx/4*1,s.cy/2));

    CCSprite *child1_left = DoubleSprite::spriteWithFile("Images/grossini_dance_08.png");
    spriteSD->addChild(child1_left);
    child1_left->setPosition(ccp(-30,0));

    CCSprite *child1_right = CCSprite::create("Images/grossini.png");
    spriteSD->addChild(child1_right);
    child1_left->setPosition(ccp( spriteSD->getContentSize().height, 0));



    //
    // RIGHT: HD sprite
    //
    // there is an HD version of grossini.png
    CCSprite *spriteHD = CCSprite::create("Images/grossini.png");
    addChild(spriteHD);
    spriteHD->setPosition(ccp(s.cx/4*3,s.cy/2));

    CCSprite *child2_left = DoubleSprite::spriteWithFile("Images/grossini_dance_08.png");
    spriteHD->addChild(child2_left);
    child2_left->setPosition(ccp(-30,0));

    CCSprite *child2_right = CCSprite::create("Images/grossini.png");
    spriteHD->addChild(child2_right);
    child2_left->setPosition(ccp( spriteHD->getContentSize().height, 0));



    // Actions
    CCScaleBy* scale = CCScaleBy::create(2, 0.5);
    CCScaleBy* scale_back = (CCScaleBy*)scale->reverse();
    CCSequence* seq = CCSequence::create(scale, scale_back, NULL);

    CCSequence* seq_copy = (CCSequence*) seq->copy()->autorelease();

    spriteSD->runAction(seq);
    spriteHD->runAction(seq_copy);

}

const KDchar* SpriteDoubleResolution::title ( KDvoid )
{
    return "Sprite Double resolution";
}

const KDchar* SpriteDoubleResolution::subtitle ( KDvoid )
{
    return "Retina Display. SD (left) should be equal to HD (right)";
}


//------------------------------------------------------------------
//
// AnimationCache
//
//------------------------------------------------------------------
KDvoid AnimationCache::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("animations/grossini.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_blue.plist");

    //
    // create animation "dance"
    //
    CCArray* animFrames = CCArray::createWithCapacity(15);
    char str[100] = {0};
    for(int i = 1; i < 15; i++)
    {
        kdSprintf(str, "grossini_dance_%02d.png",i);
        CCSpriteFrame *frame = frameCache->spriteFrameByName(str);
        animFrames->addObject(frame);
    }

    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "dance");

    //
    // create animation "dance gray"
    //
    animFrames->removeAllObjects();

    for(int i = 1; i < 15; i++)
    {
        kdSprintf(str, "grossini_dance_gray_%02d.png",i);
        CCSpriteFrame *frame = frameCache->spriteFrameByName(str);
        animFrames->addObject(frame);
    }

    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "dance_gray");

    //
    // create animation "dance blue"
    //
    animFrames->removeAllObjects();

    for(int i = 1; i < 4; i++)
    {
        kdSprintf(str, "grossini_blue_%02d.png",i);
        CCSpriteFrame *frame = frameCache->spriteFrameByName(str);
        animFrames->addObject(frame);
    }

    animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "dance_blue");


    CCAnimationCache *animCache = CCAnimationCache::sharedAnimationCache();

    CCAnimation *normal = animCache->animationByName("dance");
    normal->setRestoreOriginalFrame(KD_TRUE);
    CCAnimation *dance_grey = animCache->animationByName("dance_gray");
    dance_grey->setRestoreOriginalFrame(KD_TRUE);
    CCAnimation *dance_blue = animCache->animationByName("dance_blue");
    dance_blue->setRestoreOriginalFrame(KD_TRUE);

    CCAnimate *animN = CCAnimate::create(normal);
    CCAnimate *animG = CCAnimate::create(dance_grey);
    CCAnimate *animB = CCAnimate::create(dance_blue);

    CCSequence* seq = CCSequence::create ( animN, animG, animB, KD_NULL );

    // create an sprite without texture
    CCSprite *grossini = CCSprite::create();
    CCSpriteFrame *frame = frameCache->spriteFrameByName("grossini_dance_01.png");
    grossini->setDisplayFrame(frame);

    grossini->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    addChild ( grossini );

    // run the animation
    grossini->runAction(seq);
}

const KDchar* AnimationCache::title ( KDvoid )
{
    return "AnimationCache";
}

const KDchar* AnimationCache::subtitle ( KDvoid )
{
    return "Sprite should be animated";
}

//------------------------------------------------------------------
//
// AnimationCacheFile
//
//------------------------------------------------------------------
KDvoid AnimationCacheFile::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("animations/grossini.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_blue.plist");


    // Purge previously loaded animation
    CCAnimationCache::purgeSharedAnimationCache();

    CCAnimationCache *animCache = CCAnimationCache::sharedAnimationCache();

    // Add an animation to the Cache
    animCache->addAnimationsWithFile("animations/animations.plist");


    CCAnimation *normal = animCache->animationByName("dance_1");
    normal->setRestoreOriginalFrame(KD_TRUE);
    CCAnimation *dance_grey = animCache->animationByName("dance_2");
    dance_grey->setRestoreOriginalFrame(KD_TRUE);
    CCAnimation *dance_blue = animCache->animationByName("dance_3");
    dance_blue->setRestoreOriginalFrame(KD_TRUE);

    CCAnimate *animN = CCAnimate::create(normal);
    CCAnimate *animG = CCAnimate::create(dance_grey);
    CCAnimate *animB = CCAnimate::create(dance_blue);

    CCSequence *seq = CCSequence::create(animN, animG, animB, KD_NULL);

    // create an sprite without texture
    CCSprite *grossini = CCSprite::create();

    CCSpriteFrame *frame = frameCache->spriteFrameByName("grossini_dance_01.png");
    grossini->setDisplayFrame(frame);
    grossini->setPosition(ccpMid ( this->getContentSize ( ) ) );
    addChild(grossini);


    // run the animation
    grossini->runAction(seq);
}

const KDchar* AnimationCacheFile::title ( KDvoid )
{
    return "AnimationCache - Load file";
}

const KDchar* AnimationCacheFile::subtitle ( KDvoid )
{
    return "Sprite should be animated";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorSkew
//
//------------------------------------------------------------------
KDvoid SpriteOffsetAnchorSkew::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    for (int i = 0; i < 3; i++)
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());

        CCArray *animFrames = CCArray::create ( );
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            kdSprintf(tmp, "grossini_dance_%02d.png", j + 1);
            CCSpriteFrame *frame = cache->spriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        CCSkewBy *skewX = CCSkewBy::create(2, 45, 0);
        CCActionInterval *skewX_back = skewX->reverse();
        CCSkewBy *skewY = CCSkewBy::create(2, 0, 45);
        CCActionInterval *skewY_back = skewY->reverse();

        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));

        addChild(sprite, 0);
    }
}

KDvoid SpriteOffsetAnchorSkew::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteOffsetAnchorSkew::title ( KDvoid )
{
	return "Sprite offset + anchor + skew";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorSkew
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeOffsetAnchorSkew::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    for (int i = 0; i < 3; i++)
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());

        CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
        addChild(spritebatch);

        CCArray* animFrames = CCArray::create ( );
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            kdSprintf(tmp, "grossini_dance_%02d.png", j + 1);
            CCSpriteFrame *frame = cache->spriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        CCSkewBy *skewX = CCSkewBy::create(2, 45, 0);
        CCActionInterval *skewX_back = skewX->reverse();
        CCSkewBy *skewY = CCSkewBy::create(2, 0, 45);
        CCActionInterval *skewY_back = skewY->reverse();

        CCSequence* seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));

        spritebatch->addChild(sprite, i);
    }
}

KDvoid SpriteBatchNodeOffsetAnchorSkew::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
	cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeOffsetAnchorSkew::title ( KDvoid )
{
	return "SpriteBatchNode offset + anchor + skew";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorSkewScale
//
//------------------------------------------------------------------
KDvoid SpriteOffsetAnchorSkewScale::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for ( KDint i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());

        CCArray* animFrames = CCArray::create ( );
        for ( KDint j = 0; j < 14; j++ )
        {
            CCSpriteFrame *frame = cache->spriteFrameByName( ccszf ( "grossini_dance_%02d.png", j + 1 ) );
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        // Skew
        CCSkewBy *skewX = CCSkewBy::create(2, 45, 0);
        CCActionInterval *skewX_back = skewX->reverse();
        CCSkewBy *skewY = CCSkewBy::create(2, 0, 45);
        CCActionInterval *skewY_back = skewY->reverse();

        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));

        // Scale
        CCScaleBy *scale = CCScaleBy::create(2, 2);
        CCActionInterval *scale_back = scale->reverse();
        CCSequence *seq_scale = CCSequence::create(scale, scale_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_scale));

        addChild(sprite, 0);
    }
}

KDvoid SpriteOffsetAnchorSkewScale::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
	cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteOffsetAnchorSkewScale::title ( KDvoid )
{
	return "Sprite anchor + skew + scale";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorSkewScale
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeOffsetAnchorSkewScale::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

    CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for ( KDint i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());        

        CCArray*  animFrames = CCArray::create ( );
        for ( KDint j = 0; j < 14; j++)
        {            
            CCSpriteFrame *frame = cache->spriteFrameByName( ccszf ( "grossini_dance_%02d.png", j + 1 ) );
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        // skew
        CCSkewBy *skewX = CCSkewBy::create(2, 45, 0);
        CCActionInterval *skewX_back = skewX->reverse();
        CCSkewBy *skewY = CCSkewBy::create(2, 0, 45);
        CCActionInterval *skewY_back = skewY->reverse();

        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));

        // scale 
        CCScaleBy *scale = CCScaleBy::create(2, 2);
        CCActionInterval *scale_back = scale->reverse();
        CCSequence *seq_scale = CCSequence::create(scale, scale_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_scale));

        spritebatch->addChild(sprite, i);
    }
}

KDvoid SpriteBatchNodeOffsetAnchorSkewScale::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
	cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeOffsetAnchorSkewScale::title ( KDvoid )
{
	return "SpriteBatchNode anchor + skew + scale";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorFlip
//
//------------------------------------------------------------------
KDvoid SpriteOffsetAnchorFlip::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    for (int i = 0; i < 3; i++)
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());

        CCArray* animFrames = CCArray::create ( );
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            kdSprintf(tmp, "grossini_dance_%02d.png", i + 1);
            CCSpriteFrame *frame = cache->spriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        CCFlipY *flip = CCFlipY::create ( KD_TRUE );
        CCFlipY *flip_back = CCFlipY::create ( KD_FALSE );
        CCDelayTime *delay = CCDelayTime::create(1);
        CCSequence *seq = CCSequence::create(delay, flip, delay->copyWithZone(NULL)->autorelease(), flip_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq));

        addChild(sprite, 0);
    }
}

KDvoid SpriteOffsetAnchorFlip::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
	cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteOffsetAnchorFlip::title ( KDvoid )
{
	return "Sprite offset + anchor + flip";
}

const KDchar* SpriteOffsetAnchorFlip::subtitle ( KDvoid )
{
	return "issue #1078";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorFlip
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeOffsetAnchorFlip::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    for (int i = 0; i < 3; i++)
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("animations/grossini.plist");
        cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx / 4 * (i + 1), s.cy / 2));

        CCSprite *point = CCSprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(CCPointZero);
            break;
        case 1:
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case 2:
            sprite->setAnchorPoint(ccp(1, 1));
            break;
        }

        point->setPosition(sprite->getPosition());

        CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
        addChild(spritebatch);

        CCArray *animFrames = CCArray::create ( );
        KDchar tmp[50];
        for ( KDint j = 0; j < 14; j++)
        {            
            kdSprintf(tmp, "grossini_dance_%02d.png", i + 1);
            CCSpriteFrame *frame = cache->spriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

        CCFlipY *flip = CCFlipY::create ( KD_TRUE );
		CCFlipY *flip_back = CCFlipY::create ( KD_FALSE );
        CCDelayTime *delay = CCDelayTime::create(1);
        CCSequence *seq = CCSequence::create(delay, flip, delay->copyWithZone(NULL)->autorelease(), flip_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq));

        spritebatch->addChild(sprite, i);
    }
}

KDvoid SpriteBatchNodeOffsetAnchorFlip::onExit ( KDvoid )
{
	CCSpriteFrameCache*  cache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	cache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
	cache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeOffsetAnchorFlip::title ( KDvoid )
{
	return "SpriteBatchNode offset + anchor + flip";
}

const KDchar* SpriteBatchNodeOffsetAnchorFlip::subtitle ( KDvoid )
{
	return "issue #1078";
}

//------------------------------------------------------------------
//
// NodeSort
//
//------------------------------------------------------------------
KDvoid NodeSort::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    m_pNode = CCNode::create();
    addChild(m_pNode, 0, 0);

    m_pSprite1 = CCSprite::create("Images/piece.png", CCRectMake(128, 0, 64, 64));
    m_pSprite1->setPosition(CCPointMake(100, 160));
    m_pNode->addChild(m_pSprite1, -6, 1);

    m_pSprite2 = CCSprite::create("Images/piece.png", CCRectMake(128, 0, 64, 64));
    m_pSprite2->setPosition(CCPointMake(164, 160));
    m_pNode->addChild(m_pSprite2, -6, 2);

    m_pSprite4 = CCSprite::create("Images/piece.png", CCRectMake(128, 0, 64, 64));
    m_pSprite4->setPosition(CCPointMake(292, 160));
    m_pNode->addChild(m_pSprite4, -3, 4);

    m_pSprite3 = CCSprite::create("Images/piece.png", CCRectMake(128, 0, 64, 64));
    m_pSprite3->setPosition(CCPointMake(228, 160));
    m_pNode->addChild(m_pSprite3, -4, 3);

    m_pSprite5 = CCSprite::create("Images/piece.png", CCRectMake(128, 0, 64, 64));
    m_pSprite5->setPosition(CCPointMake(356, 160));
    m_pNode->addChild(m_pSprite5, -3, 5);

    schedule(schedule_selector(NodeSort::reorderSprite));
}

const KDchar* NodeSort::title ( KDvoid )
{
    return "node sort same index";
}

const KDchar* NodeSort::subtitle ( KDvoid )
{
    return "tag order in console should be 2,1,3,4,5";
}

KDvoid NodeSort::reorderSprite ( KDfloat dt )
{
    unschedule(schedule_selector(NodeSort::reorderSprite));

    CCLOG("Before reorder--");
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_pNode->getChildren(), pObj)
    {
        CCSprite *child = (CCSprite*)pObj;
        CCLOG("tag %i z %i",(int)child->getTag(),(int)child->getZOrder());
    }
    //z-4
    m_pNode->reorderChild((CCNode *)m_pNode->getChildren()->objectAtIndex(0), -6);

    m_pNode->sortAllChildren();
    CCLOG("After reorder--");
    CCARRAY_FOREACH(m_pNode->getChildren(), pObj)
    {
        CCSprite *child = (CCSprite*)pObj;
        CCLOG("tag %i z %i",(int)child->getTag(),(int)child->getZOrder());
    }
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderSameIndex
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeReorderSameIndex::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    m_pBatchNode = CCSpriteBatchNode::create("Images/piece.png", 15);
    addChild(m_pBatchNode, 1, 0);

    m_pSprite1 = CCSprite::createWithTexture(m_pBatchNode->getTexture(), CCRectMake(128,0,64,64));
    m_pSprite1->setPosition(CCPointMake(100,160));
    m_pBatchNode->addChild(m_pSprite1, 3, 1);

    m_pSprite2= CCSprite::createWithTexture(m_pBatchNode->getTexture(), CCRectMake(128,0,64,64));
    m_pSprite2->setPosition(CCPointMake(164,160));
    m_pBatchNode->addChild(m_pSprite2, 4, 2);

    m_pSprite3 = CCSprite::createWithTexture(m_pBatchNode->getTexture(), CCRectMake(128,0,64,64));
    m_pSprite3->setPosition(CCPointMake(228,160));
    m_pBatchNode->addChild(m_pSprite3, 4, 3);

    m_pSprite4 = CCSprite::createWithTexture(m_pBatchNode->getTexture(), CCRectMake(128,0,64,64));
    m_pSprite4->setPosition(CCPointMake(292,160));
    m_pBatchNode->addChild(m_pSprite4, 5, 4);

    m_pSprite5 = CCSprite::createWithTexture(m_pBatchNode->getTexture(), CCRectMake(128,0,64,64));
    m_pSprite5->setPosition(CCPointMake(356,160));
    m_pBatchNode->addChild(m_pSprite5, 6, 5);


    scheduleOnce(schedule_selector(SpriteBatchNodeReorderSameIndex::reorderSprite), 2);
}

const KDchar* SpriteBatchNodeReorderSameIndex::title ( KDvoid )
{
    return "SpriteBatchNodeReorder same index";
}

const KDchar* SpriteBatchNodeReorderSameIndex::subtitle ( KDvoid )
{
    return "tag order in console should be 2,3,4,5,1";
}

KDvoid SpriteBatchNodeReorderSameIndex::reorderSprite ( KDfloat dt )
{
    m_pBatchNode->reorderChild(m_pSprite4, 4);
    m_pBatchNode->reorderChild(m_pSprite5, 4);
    m_pBatchNode->reorderChild(m_pSprite1, 4);

    m_pBatchNode->sortAllChildren();
    CCObject *child;
    CCARRAY_FOREACH(m_pBatchNode->getDescendants(), child)
    {
        CCLOG("tag %i", (int)((CCSprite *)child)->getTag());
    }    
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderOneChild
//
//------------------------------------------------------------------
KDvoid SpriteBatchNodeReorderOneChild::onEnter ( KDvoid )
{	
	TestSprite::onEnter ( );

    const CCSize  s = this->getContentSize ( );

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animations/ghosts.plist");

    CCSpriteBatchNode *aParent;
    CCSprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;

    //
    // SpriteBatchNode: 3 levels of children
    //

    aParent = CCSpriteBatchNode::create("animations/ghosts.png");

    m_pBatchNode = aParent;
    //[[aParent texture] generateMipmap];
    addChild(aParent);

    // parent
    l1 = CCSprite::createWithSpriteFrameName("father.gif");
    l1->setPosition(ccp( s.cx/2, s.cy/2));

    aParent->addChild(l1);
    CCSize l1Size = l1->getContentSize();

    // child left
    l2a = CCSprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition(ccp( -10 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2a, 1);
    CCSize l2aSize = l2a->getContentSize();


    // child right
    l2b = CCSprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition(ccp( +50 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2b, 2);
    CCSize l2bSize = l2a->getContentSize();


    // child left bottom
    l3a1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale(0.45f);
    l3a1->setPosition(ccp(0+l2aSize.width/2,-50+l2aSize.height/2));
    l2a->addChild(l3a1, 1);

    // child left top
    l3a2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale(0.45f);
    l3a2->setPosition(ccp(0+l2aSize.width/2,+50+l2aSize.height/2));
    l2a->addChild(l3a2, 2);

    m_pReorderSprite = l2a;

    // child right bottom
    l3b1 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale(0.45f);
    l3b1->setFlipY(KD_TRUE);
    l3b1->setPosition(ccp(0+l2bSize.width/2,-50+l2bSize.height/2));
    l2b->addChild(l3b1);

    // child right top
    l3b2 = CCSprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale(0.45f);
    l3b2->setFlipY(KD_TRUE);
    l3b2->setPosition(ccp(0+l2bSize.width/2,+50+l2bSize.height/2));
    l2b->addChild(l3b2);

    scheduleOnce(schedule_selector(SpriteBatchNodeReorderOneChild::reorderSprite), 2.0f);
}

KDvoid SpriteBatchNodeReorderOneChild::reorderSprite ( KDfloat dt )
{
    m_pReorderSprite->getParent()->reorderChild(m_pReorderSprite, -1);

    m_pBatchNode->sortAllChildren();
    //CCSprite* child;
    //CCARRAY_FOREACH(batchNode.descendants,child) NSLog(@"tag %i",child.tag);
}

const KDchar* SpriteBatchNodeReorderOneChild::title ( KDvoid )
{
    return "SpriteBatchNode reorder 1 child";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorRotationalSkew
//
//------------------------------------------------------------------

KDvoid SpriteOffsetAnchorRotationalSkew::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for ( KDint i = 0; i < 3; i++ )
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
                            
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(CCPointZero);
                break;
            case 1:
                sprite->setAnchorPoint(ccp(0.5f, 0.5f));
                break;
            case 2:
                sprite->setAnchorPoint(ccp(1,1));
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        CCArray *animFrames = CCArray::create();
        for ( KDint i = 0; i < 14; i++ )
        {
            CCSpriteFrame*  pFrame = cache->spriteFrameByName ( ccszf ( "grossini_dance_%02d.png", i + 1 ) );
            animFrames->addObject ( pFrame );
        }
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        addChild(sprite, 0);
    }
}

KDvoid SpriteOffsetAnchorRotationalSkew::onExit ( )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    pCache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteOffsetAnchorRotationalSkew::title ( KDvoid )
{
    return "Sprite offset + anchor + rotational skew";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorRotationalSkew
//
//------------------------------------------------------------------

KDvoid SpriteBatchNodeOffsetAnchorRotationalSkew::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for ( KDint i = 0; i < 3; i++ )
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(CCPointZero);
                break;
            case 1:
                sprite->setAnchorPoint(ccp(0.5f, 0.5f));
                break;
            case 2:
                sprite->setAnchorPoint(ccp(1,1));
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        CCArray *animFrames = CCArray::create();
        for ( KDint j = 0; j < 14; j++ )
        {
            CCSpriteFrame*  pFrame = cache->spriteFrameByName( ccszf ( "grossini_dance_%02d.png", j + 1 ) );
            animFrames->addObject ( pFrame );
        }
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, KD_NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        spritebatch->addChild ( sprite, i );
    }
}

KDvoid SpriteBatchNodeOffsetAnchorRotationalSkew::onExit ( KDvoid )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    pCache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeOffsetAnchorRotationalSkew::title ( KDvoid )
{
    return "SpriteBatchNode offset + anchor + rot skew";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorRotationalSkewScale
//
//------------------------------------------------------------------

KDvoid SpriteOffsetAnchorRotationalSkewScale::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for ( KDint i = 0; i < 3; i++ )
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(CCPointZero);
                break;
            case 1:
                sprite->setAnchorPoint(ccp(0.5f, 0.5f));
                break;
            case 2:
                sprite->setAnchorPoint(ccp(1,1));
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        CCArray *animFrames = CCArray::create();
        for ( KDint j = 0; j < 14; j++ )
        {
            CCSpriteFrame *frame = cache->spriteFrameByName( ccszf ( "grossini_dance_%02d.png", j + 1 ) );
            animFrames->addObject(frame);
        }
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        
        // Skew
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        // Scale
        CCScaleBy *scale = CCScaleBy::create(2, 2);
        CCScaleBy *scale_back = (CCScaleBy *)scale->reverse();
        CCSequence *seq_scale = CCSequence::create(scale, scale_back, KD_NULL);
        sprite->runAction(CCRepeatForever::create(seq_scale));
        
        addChild ( sprite, i );
    }
}

KDvoid SpriteOffsetAnchorRotationalSkewScale::onExit ( KDvoid )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    pCache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteOffsetAnchorRotationalSkewScale::title ( KDvoid )
{
    return "Sprite anchor + rot skew + scale";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorRotationalSkewScale
//
//------------------------------------------------------------------

KDvoid SpriteBatchNodeOffsetAnchorRotationalSkewScale::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for ( KDint i = 0; i < 3; i++ )
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(CCPointZero);
                break;
            case 1:
                sprite->setAnchorPoint(ccp(0.5f, 0.5f));
                break;
            case 2:
                sprite->setAnchorPoint(ccp(1,1));
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        CCArray *animFrames = CCArray::create();
        for ( KDint j = 0; j < 14; j++ )
        {
            CCSpriteFrame*  pFrame = cache->spriteFrameByName( ccszf ( "grossini_dance_%02d.png", j + 1 ) );
            animFrames->addObject ( pFrame );
        }
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        
        // Skew
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        // Scale
        CCScaleBy *scale = CCScaleBy::create(2, 2);
        CCScaleBy *scale_back = (CCScaleBy *)scale->reverse();
        CCSequence *seq_scale = CCSequence::create(scale, scale_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_scale));
        
        spritebatch->addChild ( sprite, i );
    }
}

KDvoid SpriteBatchNodeOffsetAnchorRotationalSkewScale::onExit ( KDvoid )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    pCache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteBatchNodeOffsetAnchorRotationalSkewScale::title ( KDvoid )
{
    return "SpriteBatchNode anchor + rot skew + scale";
}

//------------------------------------------------------------------
//
// SpriteRotationalSkewNegativeScaleChildren
//
//------------------------------------------------------------------

KDvoid SpriteRotationalSkewNegativeScaleChildren::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    CCNode *parent = CCNode::create();
    addChild(parent);
    
    for(int i=0;i<2;i++)
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        // Skew
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        CCSprite *child1 = CCSprite::create("Images/grossini_dance_01.png");
        child1->setPosition(ccp(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));
        
        sprite->addChild(child1);
        
        child1->setScale(0.8f);
        
        parent->addChild(sprite, i);
    }
}

KDvoid SpriteRotationalSkewNegativeScaleChildren::onExit ( KDvoid )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    pCache->removeSpriteFramesFromFile ( "animations/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "animations/grossini_gray.plist" );

	TestSprite::onExit ( );
}

const KDchar* SpriteRotationalSkewNegativeScaleChildren::title ( KDvoid )
{
    return "Sprite rot skew + negative scale with children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeRotationalSkewNegativeScaleChildren
//
//------------------------------------------------------------------

KDvoid SpriteBatchNodeRotationalSkewNegativeScaleChildren::onEnter ( KDvoid )
{
	TestSprite::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for ( KDint i = 0; i < 2; i++ )
    {
        //
        // Animation using Sprite batch
        //
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(ccp(s.cx/4*(i+1), s.cy/2));
        
        CCSprite *point = CCSprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        // Skew
        CCRotateBy *skewX = CCRotateBy::create(2, 45, 0);
        CCRotateBy *skewX_back = (CCRotateBy*)skewX->reverse();
        CCRotateBy *skewY = CCRotateBy::create(2, 0, 45);
        CCRotateBy *skewY_back = (CCRotateBy*)skewY->reverse();
        
        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }
        
        CCSequence *seq_skew = CCSequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(CCRepeatForever::create(seq_skew));
        
        CCSprite *child1 = CCSprite::create("Images/grossini_dance_01.png");
        child1->setPosition(ccp(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));
        
        sprite->addChild(child1);
        
        child1->setScale(0.8f);
        
        spritebatch->addChild(sprite, i);
    }
}

const KDchar* SpriteBatchNodeRotationalSkewNegativeScaleChildren::title ( KDvoid )
{
    return "SpriteBatchNode + children + rot skew";
}
