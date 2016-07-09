/* --------------------------------------------------------------------------
 *
 *      File            SampleLayer.cpp
 *      Description     SampleLayer
 *      Author          J.S. Park
 *      Contact         pparkppark84@gmail.com
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
#include "SampleLayer.h"
#include "ContectListener.h"
#include "audio/SimpleAudioEngine.h"

using namespace CocosDenshion;

#define PTM_RATIO 256

KDbool SampleLayer::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return KD_FALSE;
	}

	m_nMaxJump = 0;

	EventListenerTouchOneByOne* pListner =  EventListenerTouchOneByOne::create ( );
	pListner->setSwallowTouches ( KD_TRUE );

	pListner->onTouchBegan = CC_CALLBACK_2 ( SampleLayer::onTouchBegan, this );

	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListner, this );

	SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "bgm_main.mp3", KD_TRUE );
	
	this->initBox2DWorld ( );
	this->initItem ( );

	this->addLevelBoundaries ( );
	this->addBackground ( );
	this->addFloor ( );
	this->addCharacter ( );
	
	this->schedule ( schedule_selector ( SampleLayer::step ) );
	//this->schedule ( schedule_selector ( SampleLayer::collisionCheck ), 0.2f );
	
	this->setItem ( );

	return KD_TRUE;
}

KDvoid SampleLayer::initBox2DWorld ( KDvoid )
{
	b2Vec2		tGravity;
	tGravity.Set ( 0.0f, -9.8f );

	KDbool		bDoSleep = KD_TRUE;
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( bDoSleep );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );

	// Set up debug drawing routine
	m_pDebugDraw = new Box2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	KDuint32	uFlags = 0;

	uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;

	m_pDebugDraw->SetFlags ( uFlags );
}

KDvoid SampleLayer::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid SampleLayer::addLevelBoundaries ( KDvoid )
{
	const Size&		tLyrSize = this->getContentSize ( );

	b2BodyDef		tGroundBodyDef;
	tGroundBodyDef.position.SetZero ( );

	b2Body*			pBody = m_pWorld->CreateBody ( &tGroundBodyDef );
	
	b2EdgeShape		tGroundBox;

	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pBody->CreateFixture ( &tGroundBox, 0 );

	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ) );
	pBody->CreateFixture ( &tGroundBox, 0 );

	tGroundBox.Set ( b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pBody->CreateFixture ( &tGroundBox, 0 );

	tGroundBox.Set ( b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ) );
	pBody->CreateFixture ( &tGroundBox, 0 );

	m_pWorld->SetContactListener ( new ContectListener ( this ) );
}

KDvoid SampleLayer::addCharacter ( KDvoid )
{
	// plist 에서 달리기 애니매이션 로딩
	SpriteFrameCache*	pFrameCache = SpriteFrameCache::getInstance ( );
	pFrameCache->addSpriteFramesWithFile ( "ch01.plist" );

	Dictionary*		pDictionary = Dictionary::createWithContentsOfFile ( "ch01_1_aniinfo.plist" );
	Array*			pAnimationList = (Array*) pDictionary->objectForKey ( "animationlist" );
	Array*			pFrameList = (Array*) pDictionary->objectForKey ( "framelist" );
	String*			pName = (String*) pDictionary->objectForKey ( "name" );
	String*			pTexture = (String*) pDictionary->objectForKey ( "texture" );
	String*			pType = (String*) pDictionary->objectForKey ( "type" );

	Dictionary*		pAnimationItem = (Dictionary*) pAnimationList->getObjectAtIndex ( 12 );

	Array*			pTemp = (Array*) pAnimationItem->objectForKey ( "FrameList" );
	KDfloat			fTemp = ( (String*) pAnimationItem->objectForKey ( "FPS" ) )->floatValue ( );

	Array*			pArraySpriteFrame = Array::createWithCapacity ( pTemp->count ( ) );

	Animation*		pAnimation = Animation::create ( );

	for ( KDint32 i = 0; i < pTemp->count ( ); i++ )
	{
		KDint		ii = ( (String*) pTemp->getObjectAtIndex ( i ) )->intValue ( );
		String*		pString = (String*) pFrameList->getObjectAtIndex ( ii );
		pAnimation->addSpriteFrame ( pFrameCache->getSpriteFrameByName ( pString->getCString ( ) ) );
	}
	pAnimation->setDelayPerUnit ( fTemp );

	Sprite*			pSprite = Sprite::createWithSpriteFrameName ( ( (String*) pFrameList->getObjectAtIndex ( 21 ) )->getCString ( ) );
	Point			tPoint = Point ( Point ( 100, 200 ) );
	Point tOffset = pSprite->getDisplayFrame ( )->getOffset ( );
	pSprite->setPosition ( tPoint - tOffset );
	
	pSprite->runAction ( RepeatForever::create ( Animate::create ( pAnimation ) ) );
	pSprite->setTag ( 10 );
	this->addChild ( pSprite );

	// Box2D body setting
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	tBodyDef.userData = pSprite;

	b2PolygonShape	tDynamicBox;
	Size tSize = pSprite->getDisplayFrame ( )->getRect ( ).size;
	tDynamicBox.SetAsBox ( tSize.width / 2 / PTM_RATIO, tSize.height / 2 / PTM_RATIO );

	m_pBody = m_pWorld->CreateBody ( &tBodyDef );

	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tDynamicBox;
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.0f;

	m_pBody->CreateFixture ( &tFixtureDef );
}

KDvoid SampleLayer::addFloor ( KDvoid )
{
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_staticBody;
	tBodyDef.position.Set ( 100.0f / PTM_RATIO, 36.0f / PTM_RATIO );

	b2PolygonShape	tStaticBox;
	tStaticBox.SetAsBox ( 100.0f / 2 / PTM_RATIO, 36.0f / 2 /PTM_RATIO );

	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );

	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tStaticBox;
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.0f;

	pBody->CreateFixture ( &tFixtureDef );
}

KDvoid SampleLayer::addBackground ( KDvoid )
{
	SpriteFrameCache*	pFrameCache = SpriteFrameCache::getInstance ( );
	pFrameCache->addSpriteFramesWithFile ( "tm01_bg.plist" );

	Sprite*				pSprite1 = Sprite::createWithSpriteFrameName ( "tm01_bg1.png" );
	Sprite*				pSprite2 = Sprite::createWithSpriteFrameName ( "tm01_bg2.png" );
	Sprite*				pSprite3 = Sprite::createWithSpriteFrameName ( "tm01_black.png" );
	Sprite*				pSprite4 = Sprite::create ( "floor.png" );

	pSprite1->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite2->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite3->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite4->setAnchorPoint ( Point ( 0.0f, 0.0f ) );

	pSprite1->getTexture ( )->setAliasTexParameters ( );
	pSprite2->getTexture ( )->setAliasTexParameters ( );
	pSprite3->getTexture ( )->setAliasTexParameters ( );
	pSprite4->getTexture ( )->setAliasTexParameters ( );


	Sprite*				pSprite1_1 = Sprite::createWithSpriteFrameName ( "tm01_bg1.png" );
	Sprite*				pSprite2_1 = Sprite::createWithSpriteFrameName ( "tm01_bg2.png" );
	Sprite*				pSprite3_1 = Sprite::createWithSpriteFrameName ( "tm01_black.png" );
	Sprite*				pSprite4_1 = Sprite::create ( "floor.png" );

	pSprite1_1->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite2_1->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite3_1->setAnchorPoint ( Point ( 0.0f, 0.0f ) );
	pSprite4_1->setAnchorPoint ( Point ( 0.0f, 0.0f ) );

	pSprite1_1->getTexture ( )->setAliasTexParameters ( );
	pSprite2_1->getTexture ( )->setAliasTexParameters ( );
	pSprite3_1->getTexture ( )->setAliasTexParameters ( );
	pSprite4_1->getTexture ( )->setAliasTexParameters ( );

	ParallaxScrollNode*		pParallaxNode = ParallaxScrollNode::create ( );
	
	pParallaxNode->addInfiniteScrollXWithZ ( 0, Point ( 0.5f, 0.0f ), Point ( 0, 0 ), pSprite1, pSprite1_1, KD_NULL );
	pParallaxNode->addInfiniteScrollXWithZ ( 1, Point ( 0.7f, 0.0f ), Point ( 0, 0 ), pSprite2, pSprite2_1, KD_NULL );
	pParallaxNode->addInfiniteScrollXWithZ ( 2, Point ( 1.0f, 0.0f ), Point ( 0, 0 ), pSprite4, pSprite4_1, KD_NULL );
	//pParallaxNode->addInfiniteScrollXWithZ ( 2, Point ( 1.0f, 0.0f ), Point ( 0, 0 ), pSprite3, pSprite3_1, KD_NULL );

	this->addChild ( pParallaxNode , 0, 100 );

	this->schedule ( schedule_selector ( SampleLayer::moveBackground ) );
}

KDvoid SampleLayer::initItem ( KDvoid )
{
	SpriteFrameCache*	pFrameCache = SpriteFrameCache::getInstance ( );
	pFrameCache->addSpriteFramesWithFile ( "playing_jelly.plist" );

	m_pJellyDictionary = (Dictionary*) Dictionary::createWithContentsOfFile ( "playing_jelly.plist" )->objectForKey ( "frames" );
	m_pJellyDictionary->retain ( );

	m_pItemArray = Array::create ( );
	m_pItemArray->retain ( );

	m_pTempArray = Array::create ( );
	m_pTempArray->retain ( );

	m_nItemCount = 0;
}

KDvoid SampleLayer::step ( KDfloat fDelta )
{
	KDint32			nVelocityIterations = 8;
	KDint32			nPositionIterations = 3;

	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );

	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		if ( pBody->GetUserData ( ) != KD_NULL )
		{
			Node*	pNode = (Node*) pBody->GetUserData ( );

			Point	tOffset = ( (Sprite*) pNode )->getDisplayFrame ( )->getOffset ( );
			
			pNode->setPosition ( Point ( pBody->GetPosition ( ).x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO ) - tOffset );
			pNode->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );
		}
	}

	Sprite*		pSprite = ( Sprite* ) this->getChildByTag ( 10 );

	Rect		tRect = pSprite->getDisplayFrame ( )->getRect ( );
	Point		tOffset = pSprite->getDisplayFrame ( )->getOffset ( );

	tRect.origin = pSprite->getPosition ( ) + tOffset;
	tRect.origin.x -= pSprite->getDisplayFrame ( )->getRect ( ).size.width / 2;
	tRect.origin.y -= pSprite->getDisplayFrame ( )->getRect ( ).size.height / 2;

	Object*		pObject;
	CCARRAY_FOREACH ( m_pItemArray, pObject )
	{
		Sprite*		pItem = (Sprite*) pObject;
		if ( tRect.containsPoint ( pItem->getPosition ( ) ) )
		{
			eatItem ( pItem );
		}
	}

	CCARRAY_FOREACH ( m_pTempArray, pObject )
	{
		Sprite*		pItem = (Sprite*) pObject;
		this->removeChild ( pItem );
	}

	m_pItemArray->removeObjectsInArray ( m_pTempArray );
	m_pTempArray->removeAllObjects ( );
	
}

KDbool SampleLayer::onTouchBegan ( Touch* pTouch, Event* pUnusedEvent )
{
	if ( m_nMaxJump == 2 )
	{
		return KD_TRUE;
	}

	b2Vec2 result = b2Vec2 ( 0, 10 );
	b2Vec2 vec = m_pBody->GetLinearVelocity ( );

	m_pBody->SetLinearVelocity ( b2Vec2 ( 0, 0 ) );
	m_pBody->ApplyForceToCenter ( result );

	m_nMaxJump++;

	return KD_TRUE;
}

KDvoid SampleLayer::moveBackground ( KDfloat fDelta )
{
	ParallaxScrollNode* pNode = (ParallaxScrollNode*) this->getChildByTag ( 100 );

	pNode->updateWithVelocity ( Point ( -9.5f, 0 ), fDelta );
}

KDvoid SampleLayer::setItem ( KDvoid )
{
	m_nItemCount = 0;
	m_nItemY = 100;

	this->schedule ( schedule_selector ( SampleLayer::makeItem ), 0.2f );
}

KDvoid SampleLayer::makeItem ( KDfloat fDelta )
{
	String*			pString = (String*) m_pJellyDictionary->allKeys ( )->getObjectAtIndex ( m_nItemCount );
	Sprite*			pSprite = Sprite::createWithSpriteFrameName ( pString->getCString ( ) );

	pSprite->setPosition ( Point ( 550, m_nItemY ) );
	this->addChild ( pSprite );

	pSprite->runAction 
	( 
		Sequence::create
		(
			MoveTo::create ( 2.0f, Point ( -50, m_nItemY ) ),
			CallFuncN::create ( CC_CALLBACK_1 ( SampleLayer::removeItem, this ) ),
			KD_NULL
		)
	);

	m_pItemArray->addObject ( pSprite );
	
	if ( m_nItemCount % 2 == 0 )
	{
		m_nItemY = 200; 
	}
	else
	{
		m_nItemY = 100;
	}

	m_nItemCount ++;

	if ( m_nItemCount >= (KDint) m_pJellyDictionary->count ( ) )
	{
		m_nItemCount = 0;
	}
}

KDvoid	SampleLayer::removeItem ( Node* pSender )
{
	Sprite*			pSprite = (Sprite*) pSender;

	pSprite->setOpacity ( 0 );
	m_pTempArray->addObject ( pSprite );
}

KDvoid SampleLayer::eatItem ( Node* pSender )
{
	Sprite*			pSprite = (Sprite*) pSender;

	pSprite->setOpacity ( 0 );
	m_pTempArray->addObject ( pSprite );

	ParticleSystem*	pSuccess = ParticleFlower::create ( );
	pSuccess->setAutoRemoveOnFinish ( KD_TRUE );
	pSuccess->setTexture ( Director::getInstance ( )->getTextureCache ( )->addImage ( "stars-grayscale.png" ) );
	pSuccess->setLife ( 0.2f );
	pSuccess->setDuration ( 0.05f );
	pSuccess->setPosition ( Point ( pSprite->getBoundingBox ( ).size ) / 2 + Point ( 62, 0 ) );
	pSuccess->setSpeed ( 250 );

	Sprite*			pPlayer = ( Sprite* ) this->getChildByTag ( 10 );
	pPlayer->addChild ( pSuccess );
	SimpleAudioEngine::getInstance ( )->playEffect ( "g_jelly.mp3", KD_TRUE );
}

KDvoid SampleLayer::collisionCheck ( KDfloat fDelta )
{
	Sprite*			pSprite = (Sprite*) this->getChildByTag ( 10 );

	Rect			tRect = pSprite->getDisplayFrame ( )->getRect ( );
	Point			tOffset = pSprite->getDisplayFrame ( )->getOffset ( );

	tRect.origin = pSprite->getPosition ( ) + tOffset;
	tRect.origin.x -= pSprite->getDisplayFrame ( )->getRect ( ).size.width  / 2;
	tRect.origin.y -= pSprite->getDisplayFrame ( )->getRect ( ).size.height / 2;

	Object*		pObject;
	CCARRAY_FOREACH ( m_pItemArray, pObject )
	{
		Sprite*		pItem = (Sprite*) pObject;
		if ( tRect.containsPoint ( pItem->getPosition ( ) ) )
		{
			removeItem ( pItem );
		}
	}

	CCARRAY_FOREACH ( m_pTempArray, pObject )
	{
		Sprite*		pItem = (Sprite*) pObject;
		this->removeChild ( pItem );
	}

	m_pItemArray->removeObjectsInArray ( m_pTempArray );
	m_pTempArray->removeAllObjects ( );
}