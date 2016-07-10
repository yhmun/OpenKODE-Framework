/* --------------------------------------------------------------------------
 *
 *      File            Level1Scene.h
 *      Ported By       Hyeon-Ho Jeong
 *      Contact         skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      
 *
 *      http://www.usmanacademy.com/2012/07/rage-of-war-game-using-cocos2d-x-and.html
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
#include "Level1Scene.h"

#define PTM_RATIO 32

KDbool Level1Scene::init ( KDvoid )
{	
	m_nScoreCount	 = 0;
	m_bTargetThrough = KD_FALSE;
	m_bSecondTab	 = KD_FALSE;
	m_bHit			 = KD_FALSE;
	m_pMouseJoint	 = KD_NULL;
	m_nTargetIndex	 = 0;
	m_pTargetBody	 = KD_NULL;
	m_pTargetJoint   = KD_NULL;

	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// ��ġ Ȱ��ȭ
	this->setTouchEnabled ( KD_TRUE );
 
	const CCSize&		tLyrSize = this->getContentSize ( );
	

	this->initPhysics ( );
    
	// ����̹����� �ѷ��ش�.
	CCSprite*	pSprite = CCSprite::create ( "BackGround/bgempty.png" );
	pSprite->setAnchorPoint ( ccpz );
	pSprite->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pSprite, 0, 0 );
	
	CCSprite*	spLevel = CCSprite::create ( "SpriteImage/level1.png" );
	spLevel->setAnchorPoint ( ccpz );
	spLevel->setPosition ( ccp ( 440, 290 ) );
	this->addChild ( spLevel, 10 );


	m_pBackgroundNode = CCParallaxNode::create ( );
	this->addChild ( m_pBackgroundNode, 3 );

	m_pCloud1 = CCSprite::create ( "SpriteImage/clowdsmall.png" );
	m_pCloud2 = CCSprite::create ( "SpriteImage/clowdlarge.png" );
	m_pCloud3 = CCSprite::create ( "SpriteImage/clowdlarge.png" );
    m_pCloud4 = CCSprite::create ( "SpriteImage/clowdsmall.png" );

	m_pBird1  = CCSprite::create ( "SpriteImage/bird1.png" );
	m_pBird2  = CCSprite::create ( "SpriteImage/bird1.png" );
	m_pBird3  = CCSprite::create ( "SpriteImage/bird1.png" );
	m_pBird4  = CCSprite::create ( "SpriteImage/bird1.png" );
	
	m_nBirdindex = 1;

	CCPoint dustSpeed = ccp ( 0.1f, 0.1f );
	CCPoint bgSpeed   = ccp ( 0.05f, 0.05f);

	m_pBackgroundNode->addChild ( m_pCloud1, 6,   dustSpeed ,  ccp ( 300,250 ) );
	m_pBackgroundNode->addChild ( m_pCloud3, 6 ,  dustSpeed ,  ccp ( 700,280 ) ); 
	m_pBackgroundNode->addChild ( m_pCloud2, 6,   dustSpeed ,  ccp ( 100,220 ) ); 
	m_pBackgroundNode->addChild ( m_pCloud4, 6 ,  dustSpeed ,  ccp ( 400,200 ) );

	m_pBackgroundNode->addChild ( m_pBird1,  6 ,  dustSpeed ,  ccp ( 200,160 ) );
	m_pBackgroundNode->addChild ( m_pBird2,  6 ,  dustSpeed ,  ccp ( 700,180 ) );
	m_pBackgroundNode->addChild ( m_pBird3,  6 ,  dustSpeed ,  ccp ( 500,260 ) );
	m_pBackgroundNode->addChild ( m_pBird4,  6 ,  dustSpeed ,  ccp ( 800,200 ) );
	
	// ���ھ� ǥ�� 
	this->m_pScoreLabel = CCLabelTTF::create ( "Score:0", "Marker Felt.ttf", 20 );
	m_pScoreLabel ->retain      ( );
	m_pScoreLabel ->setColor    ( ccc3 ( 128, 128, 250 ) );
	m_pScoreLabel ->setPosition ( ccp ( 70, 300 ) );
	this->addChild ( m_pScoreLabel ,0 );

	CCMenuItemImage*	pCloseItem = CCMenuItemImage::create
	(
		"SpriteImage/backbutton.png",
		"SpriteImage/backbutton.png",
		this,
		menu_selector ( Level1Scene::onMenu ) 
	);

	pCloseItem->setPosition ( ccp ( tLyrSize.cx - 30, tLyrSize.cy - 30 ) );
	this->addChild ( CCMenu::createWithItem ( pCloseItem ), 20, 1 );

	// ���� �̹���.
	CCSprite*	pSlingshot = CCSprite::create ( "SpriteImage/shooter.png" );
	this->addChild ( pSlingshot, 1 );
	
	// 
	b2BodyDef tSlingshotBodyDef;
	tSlingshotBodyDef.type = b2_dynamicBody;
	tSlingshotBodyDef.position.Set ( 130.0f / PTM_RATIO, 38.0f / PTM_RATIO );
	tSlingshotBodyDef.userData = pSlingshot;
	m_pSlingBody = m_pWorld->CreateBody ( &tSlingshotBodyDef );

	b2PolygonShape tBox;
	b2FixtureDef tSlingshotBoxDef;
	tSlingshotBoxDef.shape = &tBox;
	tSlingshotBoxDef.density = 0.3f;
	tBox.SetAsBox ( 11.0/PTM_RATIO, 31.0f/PTM_RATIO );
	m_pSlingBody->CreateFixture ( &tSlingshotBoxDef );

	b2RevoluteJointDef tSlingshotJointDef;
	tSlingshotJointDef.Initialize ( m_pBody, m_pSlingBody, b2Vec2 ( 140.0f / PTM_RATIO, 0.0f/ PTM_RATIO ) );
	tSlingshotJointDef.maxMotorTorque = 800.0f; // ������ġ, 0�̸� �������.
	tSlingshotJointDef.motorSpeed     = -10.0f;
	tSlingshotJointDef.lowerAngle     = CC_DEGREES_TO_RADIANS ( 5 );
	tSlingshotJointDef.upperAngle     = CC_DEGREES_TO_RADIANS ( 175 );
	tSlingshotJointDef.enableLimit    = KD_TRUE;
	tSlingshotJointDef.enableMotor    = KD_TRUE;
	
	m_pSlingJoint = ( b2RevoluteJoint* ) m_pWorld->CreateJoint ( &tSlingshotJointDef );
	
	m_pTargets    = new std::vector<b2Body*>;
	m_pStructures = new std::vector<b2Body*>;
	m_pClowns     = new std::vector<b2Body*>;
	m_pClownheads = new std::vector<b2Body*>;
	m_pRocks      = new std::vector<b2Body*>;
	m_pWoods      = new std::vector<b2Body*>;
	m_pAngle      = new std::vector <CCSprite*>;

	GameSetup ( );

	m_pContact    = new Level1ContactListener ( );
	m_pWorld->SetContactListener ( m_pContact );

	schedule ( schedule_selector ( Level1Scene::tick ) );
	schedule ( schedule_selector ( Level1Scene::update ) );

	return KD_TRUE;
}

Level1Scene::~Level1Scene ( )
{
	if ( m_pTargets )
    {
       	delete m_pTargets;
	    m_pTargets = KD_NULL;
    }
	
	if ( m_pStructures )
    {
		delete m_pStructures;
		m_pStructures = KD_NULL;
	}
	if ( m_pClowns )
    {
		delete m_pClowns;
		m_pClowns = KD_NULL;
	}
	if ( m_pRocks )
    {
		delete m_pRocks;
		m_pRocks = KD_NULL;
	}
	if ( m_pWoods )
    {
		delete m_pWoods;
		m_pWoods = KD_NULL;
	}
	if ( m_pWorld )
    {
		delete m_pWorld;
		m_pWorld = KD_NULL;
	}
	
	//delete m_pDebugDraw;
}

KDvoid Level1Scene::initPhysics ( KDvoid )
{
	const CCSize&  tLyrSize = this->getContentSize ( );

	// ��� ��ġ
	b2Vec2		tGravity = b2Vec2 ( 0.0f, -6.0f ); 
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( KD_TRUE );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );

	// ����� ��� ( �ȵȴ�. )
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	KDuint32	uFlags = 0;

	uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;

	m_pDebugDraw->SetFlags ( uFlags );


	// ��� �ܺ� 
	b2BodyDef	tBodyDef;
	tBodyDef.position.Set ( 0, 0 ); 

	m_pBody = m_pWorld->CreateBody ( &tBodyDef );
	
	b2EdgeShape		tGroundEdge; 
	b2FixtureDef tBoxShapeDef; 
	tBoxShapeDef.shape = &tGroundEdge;
    
	tGroundEdge.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) ); 
	m_pBody->CreateFixture ( &tBoxShapeDef );
    
	tGroundEdge.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ) ); 
	m_pBody->CreateFixture ( &tBoxShapeDef );
    
	tGroundEdge.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO), b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ) ); 
	m_pBody->CreateFixture ( &tBoxShapeDef );
    
	tGroundEdge.Set ( b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) ); 
	m_pBody->CreateFixture( &tBoxShapeDef );
}

KDvoid Level1Scene::GameSetup ( KDvoid )
{
	m_nClownCount	= 2;
	m_nTargetCount	= 6;

	// �ʿ� ������Ʈ ��
	generateMap ( );
	// ���� ���� �� ���� m_nTargetCount�� ���� ���Ѽ� ���� �Ǵ�
	AddTargets  ( );

}

KDvoid Level1Scene::generateMap ( KDvoid )
{
	AddBalls	  ( ccp ( 20.0f, 10.0f ) );
	AddBalls	  ( ccp ( 20.0f, 30.0f ) );
	AddBalls	  ( ccp ( 20.0f, 50.0f ) );
	AddBalls	  ( ccp ( 20.0f, 70.0f ) );
	AddTomatos	  ( ccp ( 20.0f, 90.0f ) );
	AddDarts	  ( ccp ( 20.0f, 110.0f) );

	AddRocks      ( ccp ( 630.0f, 0.0f ) );
	AddRocks      ( ccp ( 550.0f, 0.0f ) );
	AddLongRocks  ( ccp ( 550.0f, 42.0f) );
	AddBallons    ( ccp ( 590.0f, 0.0f ) );

	AddWoods	  ( ccp ( 675.0f, 0.0f ) );
	AddWoods	  ( ccp ( 675.0f, 42.0f) );
	AddClownHeads ( ccp ( 720.0f, 0.0f ) );
	AddWoods	  ( ccp ( 750.0f, 0.0f ) );
	AddWoods	  ( ccp ( 750.0f, 42.0f) );
	AddLongWoods  ( ccp ( 675.0f, 84.0f) );

	AddRocks	  ( ccp ( 800.0f, 0.0f ) );

	AddClowns	  ( ccp ( 870.0f, 0.0f ) );
}


// ���� �� �߰�
KDvoid Level1Scene::AddRocks ( CCPoint tPos )
{
	CCSprite*		pSprite = CCSprite::create ( "SpriteImage/rock.png" );
	addChild ( pSprite, 1 );
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*			pBody     = m_pWorld->CreateBody ( &tBodyDef );
	
	b2FixtureDef	tBoxDef;
	tBoxDef.userData  = ( KDvoid* ) 1;
	m_pRocks->push_back ( pBody );

	b2PolygonShape tBox;
	tBox.SetAsBox ( pSprite->getContentSize().cx/2.0f/PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );
	tBoxDef.shape       = &tBox;	
	tBoxDef.density     = 1.5f;
	tBoxDef.friction    = 1.0f;	
	tBoxDef.restitution = 0.0f;
	pBody->CreateFixture ( &tBoxDef );
	
    

}

// ��ٶ� �� �߰�
KDvoid Level1Scene::AddLongRocks ( CCPoint tPos )
{
	CCSprite*		pSprite = CCSprite::create ( "SpriteImage/LongRock.png" );
	addChild ( pSprite, 1 );
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	b2FixtureDef tBoxDef;
	tBoxDef.userData = ( KDvoid* ) 1;
	m_pRocks->push_back ( pBody );

	b2PolygonShape  tBox;
	tBox.SetAsBox ( pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );
	tBoxDef.shape   = &tBox;
	tBoxDef.density = 0.5f;	
	pBody->CreateFixture ( &tBoxDef );
	
    

}

// ª�� ����� �߰�
KDvoid Level1Scene::AddWoods ( CCPoint tPos )
{
	CCSprite*		pSprite = CCSprite::create ( "SpriteImage/Wood.png" );
	addChild ( pSprite, 1 );
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	
	b2FixtureDef	tBoxDef;
	tBoxDef.userData = ( KDvoid* ) 1;
	m_pWoods->push_back ( pBody );

	b2PolygonShape	tBox;
	tBox.SetAsBox(pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );
	tBoxDef.shape       = &tBox;	
	tBoxDef.density     = 0.5f;
	tBoxDef.friction    = 0.5f;
	tBoxDef.restitution = 0.5f;
	pBody->CreateFixture ( &tBoxDef );
	
    

}

// ��ٶ� ����� �߰�
KDvoid Level1Scene::AddLongWoods ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/LongWood.png" );
	addChild ( pSprite, 1 );
	
	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO);
	tBodyDef.userData = pSprite;
	b2Body *pBody = m_pWorld->CreateBody ( &tBodyDef );

	b2FixtureDef		tBoxDef;
	tBoxDef.userData = ( KDvoid* ) 1;
	m_pWoods->push_back ( pBody );

	b2PolygonShape		tBox;
	tBox.SetAsBox ( pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );
	tBoxDef.shape   = &tBox;	
	tBoxDef.density = 0.6f;	
	pBody->CreateFixture ( &tBoxDef );
	
    

}

// ���� �� �߰�
KDvoid Level1Scene::AddClowns ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/clown.png" );
	addChild ( pSprite, 1 );

	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*				pBody = m_pWorld->CreateBody ( &tBodyDef );

	b2FixtureDef		tBoxDef;
	tBoxDef.userData = ( KDvoid* ) 1;
	m_pClowns->push_back ( pBody );

	b2PolygonShape		tBox;
	tBox.SetAsBox ( pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );
	tBoxDef.shape = &tBox;	
	tBoxDef.density = 0.5f;	
	pBody->CreateFixture ( &tBoxDef );
	
    

}

// ���� �밡�� �߰�
KDvoid Level1Scene::AddClownHeads ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/Clownhead.png" );
	addChild ( pSprite, 1 );

	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set(( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
	( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;

	b2Body *pBody = m_pWorld->CreateBody ( &tBodyDef );
	b2FixtureDef		tBoxDef;
	tBoxDef.userData = ( KDvoid* ) 1;
	m_pClownheads->push_back ( pBody );


	b2CircleShape		tCircle;
    tCircle.m_radius = pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO;
    tBoxDef.shape   = &tCircle;
	tBoxDef.density = 0.5f;
	pBody->CreateFixture ( &tBoxDef );
}

KDvoid Level1Scene::AddBallons ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/Balloon.png" );
	addChild ( pSprite, 1 );

	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*				pBody = m_pWorld->CreateBody ( &tBodyDef );

	b2FixtureDef		tBoxDef;
	tBoxDef.filter.groupIndex = 5;

	b2CircleShape		tCircle;
    tCircle.m_radius = pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO;
    tBoxDef.shape    = &tCircle;
	tBoxDef.density  = 0.5f;

	pBody->CreateFixture ( &tBoxDef );
}

// �� �߰�
KDvoid Level1Scene::AddBalls ( CCPoint tPos )
{
	CCSprite*		pSprite = CCSprite::create ( "SpriteImage/Projecctile.png" );
	addChild ( pSprite, 1 );
            
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.bullet = KD_TRUE;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;

	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	pBody->SetActive ( KD_FALSE );

	b2CircleShape	tCircle;
	tCircle.m_radius = pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO;

	b2FixtureDef	tBallDef;
	tBallDef.shape = &tCircle;
	tBallDef.filter.groupIndex = 2;
	tBallDef.filter.categoryBits = 0x0002;
	tBallDef.density = 0.5f;
	tBallDef.restitution = 0.5f;
	tBallDef.friction = 0.5f;
	pBody->CreateFixture ( &tBallDef );
            
	m_pTargets->push_back ( pBody );

}

// �丶�� �߰�
KDvoid Level1Scene::AddTomatos ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/tomato.png" );
	addChild ( pSprite, 1 );
            
	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.bullet = KD_TRUE;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body *pBody = m_pWorld->CreateBody ( &tBodyDef );
	pBody->SetActive ( KD_FALSE );    
	b2CircleShape tCircle;
	tCircle.m_radius = pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO;
	
	b2FixtureDef tBallDef;
	tBallDef.shape = &tCircle;
	tBallDef.filter.groupIndex   = 3;
	tBallDef.filter.categoryBits = 0x0004;
	tBallDef.density			 = 0.4f;
	tBallDef.restitution		 = 0.1f;
	tBallDef.friction			 = 0.1f;
	pBody->CreateFixture ( &tBallDef );
            
	m_pTargets->push_back ( pBody );

}

// ��Ʈ �߰�
KDvoid Level1Scene::AddDarts ( CCPoint tPos )
{
	CCSprite*			pSprite = CCSprite::create ( "SpriteImage/dart.png" );
	addChild(pSprite , 1);
            
	b2BodyDef			tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.bullet = KD_TRUE;
			
	tBodyDef.angle = 90;
	tBodyDef.position.Set ( ( tPos.x + pSprite->getContentSize ( ).cx / 2.0f ) / PTM_RATIO,
							( tPos.y + pSprite->getContentSize ( ).cy / 2.0f ) / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*				pBody = m_pWorld->CreateBody ( &tBodyDef );
	pBody->SetActive ( KD_FALSE );    
	b2PolygonShape		tBox;
	tBox.SetAsBox( pSprite->getContentSize ( ).cx / 2.0f / PTM_RATIO, pSprite->getContentSize ( ).cy / 2.0f / PTM_RATIO );	
	
	b2FixtureDef		tBallDef;
	tBallDef.shape				 = &tBox;
	tBallDef.filter.groupIndex   = 10;
	tBallDef.filter.categoryBits = 0x0008;
	tBallDef.density			 = 0.3f;
	tBallDef.restitution		 = 0.1f;
	tBallDef.friction			 = 0.1f;
	pBody->CreateFixture ( &tBallDef );
            
	m_pTargets->push_back ( pBody );

}

// ȭ�鿡 ������ ���� ��� ��ũ�� �ȴ�.
KDvoid Level1Scene::update ( KDfloat fDelta )
{
	CCPoint tBackgroundScrollVert = ccp ( -500,0 );

 	m_pBackgroundNode->setPosition ( ccpAdd ( m_pBackgroundNode->getPosition ( ), ccpMult ( tBackgroundScrollVert, fDelta ) ) ); 


	CCArray* pClouds = CCArray::createWithCapacity ( 8 );

	pClouds->addObject ( m_pCloud1 );
	pClouds->addObject ( m_pCloud2 );
	pClouds->addObject ( m_pCloud3 );
	pClouds->addObject ( m_pCloud4 );
	pClouds->addObject ( m_pBird1 );
	pClouds->addObject ( m_pBird2 );
	pClouds->addObject ( m_pBird3 );
	pClouds->addObject ( m_pBird4 );
	
	
	for ( KDuint ii = 0  ; ii < pClouds->count ( ) ; ii++ ) 
	{
		CCSprite * spClouds = ( CCSprite* ) ( pClouds->objectAtIndex ( ii ) );
		KDfloat xPosition = m_pBackgroundNode->convertToWorldSpace ( spClouds->getPosition ( ) ).x  ;
		KDfloat tSize = spClouds->getContentSize ( ).cx ;
		
		if ( xPosition < -tSize ) 
		{
			m_pBackgroundNode->incrementOffset ( ccp ( 980,0 ), spClouds ); 
		} 
	}
}

KDvoid Level1Scene::tick ( KDfloat fDelta )
{
	KDint nVelocityIterations = 10;
	KDint positionIterations = 9;
    
	m_pWorld->Step ( fDelta, nVelocityIterations, positionIterations );
	

	// �𸣰ڴ�.
	for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
	{
		if ( b->GetUserData ( ) != KD_NULL )
		{
			b2Fixture* tFdef = b->GetFixtureList ( );
			b2Filter tBfilter = tFdef->GetFilterData ( );

			if ( tBfilter.groupIndex == 5 )
			{ 
				b->ApplyForce ( b2Vec2 ( 0.0, 6.0 * b->GetMass ( ) * 2 ), b->GetWorldCenter ( ) );
			
				if( b->GetPosition ( ).y * PTM_RATIO >= 100 && b->GetPosition ( ).y * PTM_RATIO <= 105 )
				{
					m_nScoreCount+=1000;
					updateScore(m_nScoreCount);
					ScoreAnimation ( ccp ( b->GetPosition ( ).x * PTM_RATIO, b->GetPosition ( ).y * PTM_RATIO ), "SpriteImage/1000.png","SpriteImage/1000g.png" );	
				}
			}
			
			CCSprite *pSprite = ( CCSprite* ) b->GetUserData ( );
			pSprite->setPosition ( ccp ( b->GetPosition ( ).x * PTM_RATIO, b->GetPosition ( ).y * PTM_RATIO ) );
		
			if( tBfilter.groupIndex != 10 )
			{
				pSprite->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( b->GetAngle ( ) ) );
			}
		}
	}
	
	// Ÿ�� ( �� ) �� ������ �� �Ǻ�
    if ( m_bTargetThrough && m_pTargetJoint )
    {
		if ( m_pSlingJoint->GetJointAngle ( ) <= CC_DEGREES_TO_RADIANS ( 10 ) )
        {
            m_bTargetThrough = KD_FALSE;

            m_pWorld->DestroyJoint ( m_pTargetJoint );
            m_pTargetJoint = KD_NULL;
           
			runAction 
			( 
				CCSequence::create
				( 
					CCDelayTime::create ( 1.0f ),
					CCCallFunc ::create ( this, callfunc_selector ( Level1Scene::updateTargets ) ),
					KD_NULL
				) 
			);
	
			std::vector<CCSprite*>::iterator	iter;
			
			for ( iter = m_pAngle->begin ( ); iter != m_pAngle->end ( ); iter++ )
			{
				if ( *iter != KD_NULL )
				{
					this->removeChildByTag ( 20, KD_TRUE );
				}
				
			}
			m_bHit = KD_FALSE;
		}
	}

	std::set<b2Body*>::iterator pos;
	
    for ( pos = m_pContact->m_pContacts.begin ( );  pos != m_pContact->m_pContacts.end ( ); pos++ )
    {
        b2Body*		pBody = *pos;
        
        CCNode*		pContactNode = ( CCNode* ) pBody->GetUserData ( );
        CCPoint tPos = pContactNode->getPosition ( );
       
		removeChild ( pContactNode, KD_TRUE );
		m_pWorld->DestroyBody ( pBody );
        
	
		std::vector<b2Body*>::iterator it,iter,itrWoods,itrRocks,itheads;
		
		// �浹 �Ǻ�
		for ( it = m_pClowns->begin ( ); it != m_pClowns->end ( ); it++ )
		{
			if( *it == pBody )
			{
				m_pClowns->erase ( it );
				m_nClownCount--;
				m_nScoreCount += 5000;
				ExplosionAnimation ( tPos, "SpriteImage/Clownhead.png" );
				ScoreAnimation ( tPos, "SpriteImage/5000.png", "SpriteImage/5000g.png" );	
				updateScore ( m_nScoreCount );
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/clown.mp3" );
				m_bHit = KD_TRUE;
				break;
			}
		}

		// �浹 �Ǻ�
		for ( itheads = m_pClownheads->begin ( ) ; itheads!=m_pClownheads->end ( ); itheads++)
		{
			if(*itheads == pBody )
			{
				m_pClownheads->erase ( itheads );
				m_nClownCount--;
				m_nScoreCount += 2000;
				ExplosionAnimation ( tPos, "SpriteImage/Clownhead.png" );
				ScoreAnimation ( tPos, "SpriteImage/2000.png", "SpriteImage/2000g.png" );	
				updateScore ( m_nScoreCount );
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/clown.mp3" );
				m_bHit = KD_TRUE;
				break;
			}
		}

		for ( iter = m_pStructures->begin ( ); iter != m_pStructures->end ( ); iter++ )
		{
			if ( *iter == pBody )
			{
				m_pStructures->erase ( iter );
				m_nScoreCount += 100;
				ExplosionAnimation ( tPos, "SpriteImage/woodpiece.png" );
				updateScore ( m_nScoreCount );
				m_bHit = KD_TRUE;
				break;
			}
		}
	
		for ( itrWoods = m_pWoods->begin ( ); itrWoods != m_pWoods->end ( ); itrWoods++ )
		{
			if ( *itrWoods == pBody )
			{
				m_pWoods->erase ( itrWoods );
				m_nScoreCount += 100;
				ExplosionAnimation ( tPos, "SpriteImage/woodpiece.png" );
				
				ScoreAnimation ( tPos, "SpriteImage/100.png", "SpriteImage/100g.png" );
				updateScore ( m_nScoreCount );
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/wood.mp3" );
				m_bHit=KD_TRUE;
				break;
			}
		}
	
		for ( itrRocks = m_pRocks->begin ( ) ; itrRocks != m_pRocks->end ( ) ; itrRocks++ )
		{
			if ( *itrRocks == pBody  )
			{
				m_pRocks->erase ( itrRocks );
				m_nScoreCount += 500;
				ExplosionAnimation ( tPos, "SpriteImage/rock.png" );
				ScoreAnimation ( tPos,"SpriteImage/500.png","SpriteImage/500g.png" );
				updateScore ( m_nScoreCount );
				m_bHit = KD_TRUE;
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/rock.mp3" );
				break;
			}
		}
    }
    //?
	if ( m_pTargetBody && m_bTargetThrough == KD_FALSE )
	{
		m_bSecondTab = KD_TRUE;
	}
    
	// �� ���󰡴� ���� ǥ��
	if ( m_pTargetBody && m_bTargetThrough == KD_FALSE && m_bHit==KD_FALSE )
	{
		b2Vec2 tPos = m_pTargetBody->GetPosition ( );
    
		if ( tPos.x * PTM_RATIO > 100 )
		{
			CCSprite* pSprite = CCSprite::create ( "SpriteImage/dot.png" );
	
			pSprite->setPosition ( ccp ( tPos.x * PTM_RATIO, tPos.y * PTM_RATIO ) );
			addChild ( pSprite );
			pSprite->setTag ( 20 );
			m_pAngle->push_back ( pSprite );
		}
	}

	// ���� �ʱ�ȭ
    m_pContact->m_pContacts.clear ( );
	
}

KDvoid Level1Scene::spMoveFinished ( CCNode* pSender )
{
	  CCSprite *pSprite = ( CCSprite* ) pSender;
	  this->removeChild ( pSprite, KD_TRUE );
}

KDvoid Level1Scene::ccTouchesMoved ( CCSet* touches, CCEvent* events )
{
	if ( m_pMouseJoint == KD_NULL )
	{
		return;
	}


	CCTouch*	touch = ( CCTouch* ) ( touches->anyObject( ) );
	
	CCPoint		location = this->convertTouchToNodeSpace ( touch );
	
	b2Vec2 tLocationWorld = b2Vec2 ( location.x / PTM_RATIO, location.y / PTM_RATIO );
	
	m_pMouseJoint->SetTarget ( tLocationWorld );
	
}

KDvoid Level1Scene::ccTouchesBegan(CCSet* touches, CCEvent* events)
{

	if ( m_pMouseJoint != KD_NULL )
	{
		return;
	}
	
	CCTouch*	touch = (CCTouch*)( touches->anyObject() );
	
	CCPoint		location = this->convertTouchToNodeSpace ( touch );

	
	
	b2Vec2 tLocationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);
	
	if(tLocationWorld.x<m_pSlingBody->GetWorldCenter().x + 50.0f/PTM_RATIO)
	{
		b2MouseJointDef mjDef;
		mjDef.bodyA = m_pBody;
		mjDef.bodyB = m_pSlingBody;
		mjDef.target = tLocationWorld;
		mjDef.maxForce = 5000;
		
		m_pMouseJoint = (b2MouseJoint*)m_pWorld->CreateJoint(&mjDef);
	}
}



KDvoid Level1Scene::ccTouchesEnded ( CCSet* touches, CCEvent* events )
{
	CCTouch*	touch = ( CCTouch* ) ( touches->anyObject ( ) );
	
	CCPoint		location = this->convertTouchToNodeSpace ( touch );
	
	if ( m_pMouseJoint != KD_NULL )
	{
		if ( m_pSlingJoint->GetJointAngle ( ) >= CC_DEGREES_TO_RADIANS ( 10 ) )
			m_bTargetThrough = KD_TRUE;
		m_pWorld->DestroyJoint ( m_pMouseJoint );
		m_pMouseJoint = KD_NULL;
	}	
}

KDvoid Level1Scene::draw ( KDvoid )
{
	CCLayer::draw ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    kmGLPushMatrix ( );

	m_pWorld->DrawDebugData ( );

    kmGLPopMatrix  ( );

    CHECK_GL_ERROR_DEBUG ( );
}

// ���� ���� �� ���� m_nTargetCount�� ���� ���Ѽ� ���� �Ǵ�
KDvoid Level1Scene::AddTargets ( KDvoid )
{
	m_nTargetCount--;
	m_pTargetBody = m_pTargets->at ( m_nTargetIndex++ );
	m_pTargetBody->SetTransform ( b2Vec2 ( 130.0f / PTM_RATIO, 45.0f / PTM_RATIO ), 0.0f );
    m_pTargetBody->SetActive ( KD_TRUE );
		
	b2WeldJointDef		tWeldJointDef;
    tWeldJointDef.Initialize ( m_pTargetBody, m_pSlingBody, b2Vec2 ( 140.0f / PTM_RATIO, 45.0f / PTM_RATIO ) );
    tWeldJointDef.collideConnected = KD_FALSE;

    m_pTargetJoint = ( b2WeldJoint* ) m_pWorld->CreateJoint ( &tWeldJointDef );

	
}

// ���� ���� �� ���� ��� ������Ʈ ���� �Ǻ��ؼ� ���и� �Ǻ�.
KDvoid Level1Scene::updateTargets ( KDvoid )
{
	if ( m_nClownCount == 0 )
	{
		SimpleAudioEngine::sharedEngine()->playEffect("Winner.wav") ;
		/*
		CCScene* overScene = CCScene::create ( );
		objModeScene->getLayer()->getLabel()->setString("You Win!!");
		CCDirector::sharedDirector()->replaceScene(objModeScene);
		*/
	
	}
	else if ( m_nTargetCount == 0 )
	{
		SimpleAudioEngine::sharedEngine()->playEffect ("alarm.wav") ;
		/*
		GameOverScene *objModeScene = GameOverScene::create();
		objModeScene->getLayer()->getLabel()->setString("Game Over!!");
		CCDirector::sharedDirector()->replaceScene(objModeScene);
		*/
	}
	else if ( m_nTargetCount != 0 )
	{
		AddTargets ( );
	}
}


KDvoid Level1Scene::onMenu(CCObject* pSender)
{
	CCScene* 	pScene = CCScene::create ( );
	pScene->addChild ( MainScene::create ( ) );
    CCDirector::sharedDirector ( )->replaceScene ( pScene );

}

KDvoid Level1Scene::updateScore ( KDint nScore )
{
	const KDint nLabelLength = 100;
	KDchar scoreLabelText [ nLabelLength ];
	kdSprintf ( scoreLabelText, "Score: %d", nScore );
	m_pScoreLabel ->setString ( scoreLabelText );
}


KDvoid Level1Scene::ScoreAnimation ( CCPoint tPos, const KDchar* szImgName, const KDchar* szImgName2 )
{
	m_pExplosion = CCSprite::create ( szImgName );
	m_pExplosion->setPosition ( tPos );
	this->addChild ( m_pExplosion,0,22 );
	
	CCAnimation*	pAnim = CCAnimation::create ( );		
	pAnim->addSpriteFrameWithFileName ( szImgName );
	pAnim->addSpriteFrameWithFileName ( szImgName2 );

	m_pExplosion->runAction
	(
		CCSequence::create
		(
			CCAnimate	::create ( pAnim ),
			CCCallFuncN	::create ( this, callfuncN_selector ( Level1Scene::spMoveFinished ) ), 
			KD_NULL
		) 
	);
}

KDvoid Level1Scene::ExplosionAnimation ( CCPoint tPos,const KDchar* szImgName )
{

	CCParticleExplosion* pEmitter = new CCParticleExplosion ( );
	pEmitter->setTexture(CCTextureCache::sharedTextureCache ( )->addImage ( szImgName ) );
	pEmitter->initWithTotalParticles ( 10 );      
	pEmitter->setStartSize ( 20.0f );
	pEmitter->setSpeed ( 50.0f );
	pEmitter->setAnchorPoint ( ccp ( 0.5f,0.5f ) );
	pEmitter->setPosition ( tPos );
	pEmitter->setDuration ( 0.5f );
	ccColor4F startColor = { 219.5f, 147.5f, 29.5f, 1.0f };
	pEmitter->setStartColor ( startColor );
	
	ccColor4F startColorVar = { 219.5f, 147.5f, 29.5f, 1.0f };
	pEmitter->setStartColorVar ( startColorVar );
	
	ccColor4F endColor = { 167.1f, 91.1f, 39.1f, 1.0f };
	pEmitter->setEndColor ( endColor );
	
	ccColor4F endColorVar = { 167.1f, 91.1f, 39.1f, 1.0f };    
	pEmitter->setEndColorVar ( endColorVar );
	pEmitter->setAutoRemoveOnFinish ( KD_TRUE );
	pEmitter->retain ( );
	addChild ( pEmitter, 1 );
	pEmitter->release ( );
}
