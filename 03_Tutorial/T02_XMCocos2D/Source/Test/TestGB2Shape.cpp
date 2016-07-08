/* -----------------------------------------------------------------------------------
 *
 *      File            TestGB2Shape.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      Created by Thomas Broquist. All rights reserved.
 *
 *         http://www.PhysicsEditor.de  
 *         http://texturepacker.com
 *         http://www.code-and-web.de
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "TestGB2Shape.h"

#define PTM_RATIO		32

enum 
{
	kTagTileMap			= 1,
	kTagSpriteManager	= 1,
	kTagAnimation1		= 1,
}; 

static const KDchar*	l_aNames [ ] =
{
    "hotdog"	,
    "drink"		,
    "icecream"	,
    "icecream2"	,
    "icecream3"	,
    "hamburger"	,
    "orange"	
};

KDbool TestGB2Shape::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

    // load physics shapes
    GB2ShapeCache::sharedGB2ShapeCache ( )->addShapesWithFile ( "xm_physicseditor/shapedefs.plist" );
    
	// Define the gravity vector.
	b2Vec2		tGravity;
	tGravity.Set ( 0.0f, -10.0f );
	
	// Do we want to let bodies sleep?
	KDbool		bDoSleep = KD_TRUE;
    
	// Construct a world object, which will hold and simulate the rigid bodies.
	m_tWorld = new b2World ( tGravity );
	m_tWorld->SetAllowSleeping ( bDoSleep );    
	m_tWorld->SetContinuousPhysics ( KD_TRUE );
        	
    // m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    // world->SetDebugDraw(m_debugDraw);
     
//    KDuint32	uFlags = 0;
//    uFlags += b2Draw::e_shapeBit;
//    uFlags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);
    
	
	b2BodyDef	tGroundBodyDef;
	tGroundBodyDef.position.Set ( tLyrSize.cx / 2 / PTM_RATIO, tLyrSize.cy / 2 / PTM_RATIO ); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body*		pGroundBody = m_tWorld->CreateBody ( &tGroundBodyDef );
	
	// Define the ground box shape.
	b2PolygonShape	tGroundBox;		
	
    // bottom
    tGroundBox.SetAsBox ( tLyrSize.cx / 2 / PTM_RATIO, 0, b2Vec2 ( 0, -tLyrSize.cy / 2 / PTM_RATIO ), 0 );
 	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
    // top
    tGroundBox.SetAsBox ( tLyrSize.cx / 2 / PTM_RATIO, 0, b2Vec2 ( 0, tLyrSize.cy / 2 / PTM_RATIO ), 0 );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );
    
    // left
    tGroundBox.SetAsBox(0, tLyrSize.cy / 2 / PTM_RATIO, b2Vec2 ( -tLyrSize.cx / 2 / PTM_RATIO, 0 ), 0 );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );
    
    // right
    tGroundBox.SetAsBox(0, tLyrSize.cy / 2 / PTM_RATIO, b2Vec2 (  tLyrSize.cx / 2 / PTM_RATIO, 0 ), 0 );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );
    
	
	//Set up sprite
	CCSpriteBatchNode*  pMgr = CCSpriteBatchNode::create ( "xm_physicseditor/blocks.png", 150 );
	addChild ( pMgr, 0, kTagSpriteManager );
	
	addNewSpriteWithCoords ( ccpMid ( tLyrSize ) );
	
	this->setTouchEnabled ( KD_TRUE );
	this->setAccelerometerEnabled( KD_TRUE );
	this->schedule ( schedule_selector ( TestGB2Shape::tick ) );

	return KD_TRUE;
}

const KDchar* TestGB2Shape::subtitle ( KDvoid )
{
	return "Physics Editor - Tap screen";
}

KDuint TestGB2Shape::count ( KDvoid )
{
	return 1;
}

KDvoid TestGB2Shape::draw ( KDvoid )
{
    CCLayer::draw ( );
    
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix ( );
    
//    m_tWorld->DrawDebugData ( );
    
    kmGLPopMatrix  ( );
}

KDvoid TestGB2Shape::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Add a new body/atlas sprite at the touched location
	CCSetIterator	iter;    
	for ( iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );        
		if ( !pTouch )
		{
			break;
		}
        
		CCPoint		tLocation = pTouch->getLocation ( );		
		tLocation = this->convertToNodeSpace ( tLocation );
        
		addNewSpriteWithCoords ( tLocation );
	}
}

KDvoid TestGB2Shape::addNewSpriteWithCoords ( CCPoint tPos )
{    
	const KDchar*  szName = l_aNames [ kdRand ( ) % 7 ];

    CCSprite*	pSprite = CCSprite::create ( ccszf ( "xm_physicseditor/%s.png", szName ) );    
    pSprite->setPosition ( tPos );    
    this->addChild ( pSprite );
    
	b2BodyDef	tBodyDef;
	tBodyDef.type = b2_dynamicBody;
    
	tBodyDef.position.Set ( tPos.x / PTM_RATIO, tPos.y / PTM_RATIO );
	tBodyDef.userData = pSprite;
	b2Body*		pBody = m_tWorld->CreateBody ( &tBodyDef );
    
    // add the fixture definitions to the body
    
    GB2ShapeCache*  pSC = GB2ShapeCache::sharedGB2ShapeCache ( );
    pSC->addFixturesToBody ( pBody, szName ); 
    pSprite->setAnchorPoint ( pSC->anchorPointForShape ( szName ) );
}

KDvoid TestGB2Shape::tick ( KDfloat fDelta )
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	KDint	nVelocityIterations = 8;
	KDint	nPositionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_tWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
	
	//Iterate over the bodies in the physics world
	for ( b2Body* pBody = m_tWorld->GetBodyList ( ); pBody; pBody = pBody->GetNext ( ) )
	{
		if ( pBody->GetUserData ( ) != KD_NULL )
		{
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite*	pActor = (CCSprite*) pBody->GetUserData ( );
			pActor->setPosition ( ccp ( pBody->GetPosition ( ).x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO ) );
			pActor->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );
		}	
	}
}