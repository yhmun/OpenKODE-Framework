/* -----------------------------------------------------------------------------------
 *
 *      File            PhysicalWorld.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "PhysicalWorld.h"

PhysicalWorld* PhysicalWorld::create ( EnemyListBuilder* pBuilder, Player* pPlayer )
{
	PhysicalWorld*		pRet = new PhysicalWorld ( );

    if ( pRet && pRet->init ( pBuilder, pPlayer ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool PhysicalWorld::init ( EnemyListBuilder* pBuilder, Player* pPlayer )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	initPhysics ( );

	initPlayer ( pPlayer );

	initEnemy ( pBuilder );

	scheduleUpdate ( );

	return true;
}

KDvoid PhysicalWorld::initPhysics ( KDvoid )
{
	// create world without gravity
	b2Vec2		tGravity ( 0, 0 );
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( true );
	
	// owner contact listener
	m_pContactListener = new MyContactListener ( );
	m_pWorld->SetContactListener ( m_pContactListener );

	MyContactFilter*	pContactFilter = new MyContactFilter ( );
	m_pWorld->SetContactFilter ( pContactFilter );

	m_pDebugDraw = new Box2DDebugDraw ( PT_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );
    
	KDuint32	uFlags = 0;

	uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;
    
	m_pDebugDraw->SetFlags ( uFlags );
}

KDvoid PhysicalWorld::initPlayer ( Player* pPlayer )
{
	m_pPlayer = pPlayer;
	if ( m_pPlayer == KD_NULL )
	{
		m_pPlayer = Player::create ( );
	}

	this->addChild ( m_pPlayer );

	
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	m_pPlayer->getArmature ( )->setB2Body ( pBody );

	setPlayerFilterInfo ( pBody );
	
	// bind frame event to player
	m_pPlayer->getArmature ( )->getAnimation ( )->setFrameEventCallFunc ( this, frameEvent_selector ( PhysicalWorld::onFrameEvent ) );	
}

KDvoid PhysicalWorld::setPlayerFilterInfo ( b2Body* pBody )
{
	b2Fixture*	pFixture = pBody->GetFixtureList ( );
	if ( pFixture != KD_NULL )  
	{
		pFixture->SetUserData ( m_pPlayer );

		b2Filter	tFilter  = pFixture->GetFilterData ( );
		tFilter.categoryBits = PLAYER;
		tFilter.groupIndex   = PLAYER_GROUPINDEX;
		pFixture->SetFilterData ( tFilter );
	}
}

KDvoid PhysicalWorld::initEnemy ( EnemyListBuilder* pBuilder )
{
	if ( pBuilder == KD_NULL )
	{
		return;
	}

	m_pEnemyListBuilder = pBuilder;

	while ( !m_pEnemyListBuilder->isTraverseListFinished ( ) )
	{
		addEnemyIntoWorld ( m_pEnemyListBuilder->getEnemy ( ) );
	}
}

KDvoid PhysicalWorld::addEnemyIntoWorld ( Enemy* pEnemy )
{
	addChild ( pEnemy );

	b2BodyDef	tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	b2Body*		pBody = m_pWorld->CreateBody ( &tBodyDef );
	pEnemy->getArmature ( )->setB2Body ( pBody );

	setEnemyFilterInfo ( pBody, pEnemy );
}

KDvoid PhysicalWorld::setEnemyFilterInfo ( b2Body* pBody, Enemy* pEnemy )
{
	b2Fixture*		pFixture = pBody->GetFixtureList ( );
	if ( pFixture != KD_NULL ) 
	{
		pFixture->SetUserData ( pEnemy );

		b2Filter	tFilter  = pFixture->GetFilterData ( );
		tFilter.categoryBits = ENEMY;
		tFilter.groupIndex   = ENEMY_GROUPINDEX;
		pFixture->SetFilterData ( tFilter );
	}
}

KDvoid PhysicalWorld::onFrameEvent ( Bone* pBone, const KDchar* pEvt, KDint nOriginFrameIndex, KDint nCurrentFrameIndex )
{
	// define bullet action
	ActionInterval*		pBulletAction;
	Point				tStartPosition = m_pPlayer->getArmature ( )->getPosition ( );
	if ( m_pPlayer->getArmature ( )->getScaleX ( ) > 0 )
	{
		pBulletAction = MoveBy::create ( 1.5f, Point ( -450, 0 ) );
		tStartPosition.x -= 50;	// hands length
	}
	else 
	{
		tStartPosition.x += 50;
		pBulletAction = MoveBy::create ( 1.5f, Point ( 450, 0 ) );
	}

	createBullet ( tStartPosition, pBulletAction );
}

KDvoid PhysicalWorld::createBullet ( Point tStartPosition, ActionInterval* pBulletAction )
{
	PhysicsSprite*	pBullet = PhysicsSprite::create ( "bullet.png" );

	// Define the dynamic body.
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
    
	// Define another box shape for our dynamic body.
	b2PolygonShape	tDynamicBox;
	//the collision box size of bullet
	tDynamicBox.SetAsBox ( 0.4f, 0.4f );
	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape    = &tDynamicBox;
	tFixtureDef.isSensor = true;
    tFixtureDef.userData = pBullet;
    
	pBody->CreateFixture ( &tFixtureDef );
    
	pBullet->setB2Body ( pBody );
	pBullet->setPTMRatio ( PT_RATIO );
    pBullet->setPosition ( tStartPosition );

	setBulletFilterInfo ( pBody, pBullet );

	this->addChild ( pBullet );
	
	FiniteTimeAction*	pAction = Sequence::create 
	(
		pBulletAction,
		CallFunc::create ( CC_CALLBACK_0 ( PhysicalWorld::destroyBullet, this, pBullet, (KDvoid*) pBody ) ),
		nullptr
	);

	pBullet->runAction ( pAction );
}

KDvoid PhysicalWorld::setBulletFilterInfo ( b2Body* pBody, PhysicsSprite* pBullet )
{
	b2Fixture*	pFixture = pBody->GetFixtureList ( );
	if ( pFixture != KD_NULL ) 
	{
		pFixture->SetUserData ( pBullet );

		b2Filter	tFilter  = pFixture->GetFilterData ( );
		tFilter.categoryBits = BULLET;
		tFilter.groupIndex   = PLAYER_GROUPINDEX;
		pFixture->SetFilterData ( tFilter );
	}
}

KDvoid PhysicalWorld::destroyBullet ( Node* pSender, KDvoid* pData )
{
	m_pWorld->DestroyBody ( (b2Body*) pData );
	pSender->removeFromParentAndCleanup ( true );
}

KDvoid PhysicalWorld::update ( KDfloat fDelta )
{
	m_pWorld->Step ( fDelta, 0, 0 );

	processContactList ( );

	processDiedEnemy ( );
}

KDvoid PhysicalWorld::processContactList ( KDvoid )
{
	if ( m_pContactListener->isContactListEmpty ( ) )
	{
		return;
	}

	Contact		tContact = m_pContactListener->getListFront ( );
	
	b2Filter	aFilters [ 2 ];
	for ( KDint i = 0; i < 2; i++ )
	{
		aFilters [ i ] = tContact.fixture [ i ]->GetFilterData ( );
		if ( aFilters [ i ].categoryBits == ENEMY ) 
		{
			Enemy*		pEnemy = (Enemy*) tContact.fixture [ i ]->GetUserData ( );
			pEnemy->beHitted ( );
		}
		else if ( aFilters [ i ].categoryBits == PLAYER )
		{
			Player*		pPlayer = (Player*) tContact.fixture [ i ]->GetUserData ( );
			pPlayer->beHitted ( );
		}
		else if ( aFilters [ i ].categoryBits == BULLET )
		{
			PhysicsSprite*	pBullet = (PhysicsSprite*) tContact.fixture [ i ]->GetUserData ( );
			pBullet->setVisible ( false );
			tContact.fixture [ i ]->GetBody ( )->SetActive ( false );
		}
	}

	m_pContactListener->clearContactList ( );	
}

KDvoid PhysicalWorld::processDiedEnemy ( KDvoid )
{
	if ( m_pEnemyListBuilder->isEnemyAllDied ( ) )
	{
		return;
	}

	while ( !m_pEnemyListBuilder->isTraverseListFinished ( ) )
	{
		Enemy*	pEnemy = m_pEnemyListBuilder->getEnemy ( );
		if ( pEnemy->isEnemyDied ( ) )
		{
			destroyEnemy ( pEnemy );
		}
	}
}

KDvoid PhysicalWorld::destroyEnemy ( Enemy* pEnemy )
{
	m_pWorld->DestroyBody ( pEnemy->getArmature ( )->getB2Body ( ) );
	m_pEnemyListBuilder->deleteEnemy ( pEnemy );
	pEnemy->removeFromParentAndCleanup ( true );
}

KDvoid PhysicalWorld::draw ( KDvoid )
{
//	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
//	kmGLPushMatrix();
	m_pWorld->DrawDebugData ( );
//	kmGLPopMatrix();
}

PhysicalWorld::~PhysicalWorld ( KDvoid )
{
	unscheduleUpdate ( );

	CC_SAFE_DELETE ( m_pEnemyListBuilder );
	CC_SAFE_DELETE ( m_pWorld );
	CC_SAFE_DELETE ( m_pContactListener );
	CC_SAFE_DELETE ( m_pDebugDraw );
}


