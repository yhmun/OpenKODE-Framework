/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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
#include "GameScene.h"
#include "LevelEditor/LevelFileHandler.h"
#include "LevelEditor/LevelEditor.h"
#include "Model/PineappleModel.h"
#include "Model/RopeModel.h"
#include "Utils/CoordinateHelper.h"

#define PTM_RATIO				32

#define cc_to_b2Vec( x, y )		( b2Vec2 ( ( x ) / PTM_RATIO, ( y ) / PTM_RATIO ) )

#define kCuttingSound			"sound/cut.wav"
#define kBiteSound				"sound/bit.wav"
#define kSplashSound			"sound/splash.wav"
#define kBackgroundMusic		"sound/CheeZeeJungle.wav"

CCScene* GameScene::GameSceneWithLevelHandler ( LevelFileHelper* pLevelHelper )
{
    CCScene*		pScene = CCScene::create ( );
    GameScene*		pLayer = new GameScene ( pLevelHelper );
    pScene->addChild ( pLayer );    

    return pScene;
}

GameScene::GameScene ( LevelFileHelper* pLevelHelper )
{
	m_pRopes	= KD_NULL;
	m_pSticks	= KD_NULL;
	m_pWinLabel = KD_NULL;
	m_pWinLabel = KD_NULL;


    this->setTouchEnabled ( KD_TRUE );
  
    CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "CutTheVerlet.plist" );
    
    CCSprite*	pBG = CCSprite::createWithSpriteFrameName ( "bg.png" );
    pBG->setAnchorPoint ( ccpz );  
    this->addChild ( pBG, -1 );       

    // Add the croc
    m_pCroc = CCSprite::createWithSpriteFrameName ( "croc_front_mouthclosed.png" );
    m_pCroc->setAnchorPoint ( ccp ( 1.0, 0.0 ) );  // this is also the achor point strategy
    m_pCroc->setPosition ( ccp ( 320, 30 ) );
    this->addChild ( m_pCroc, 1 );

    m_pRopeSpriteSheet = CCSpriteBatchNode::create ( "rope_texture.png" );
    this->addChild ( m_pRopeSpriteSheet );

    m_pLevelEditor = pLevelHelper;
       
    this->createMenu ( );
 
	this->initPhysics ( );
   
    this->initLevel ( );
    
    SimpleAudioEngine::sharedEngine ( )->preloadEffect ( kCuttingSound );
    SimpleAudioEngine::sharedEngine ( )->preloadEffect ( kBiteSound );
    SimpleAudioEngine::sharedEngine ( )->preloadEffect ( kSplashSound );
    SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( kBackgroundMusic, KD_TRUE );
    SimpleAudioEngine::sharedEngine ( )->setBackgroundMusicVolume ( 0.4f );
    
    this->schedule ( schedule_selector ( GameScene::changeCrocAttitude ), 4.0f );
           
    this->scheduleUpdate ( );
}

GameScene::~GameScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pRopes );
	CC_SAFE_RELEASE ( m_pSticks );
}

KDvoid GameScene::createMenu ( KDvoid )
{
	CCMenuItemFont::setFontName ( "font/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );

    CCSize			tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

    CCMenuItem*		pToEditorItem = CCMenuItemFont::create ( "Editor", this, menu_selector ( GameScene::switchToEditor ) );
    CCMenuItem*		pToMenuItem   = CCMenuItemFont::create ( "Menu"  , this, menu_selector ( GameScene::switchToMenu ) );
    
    CCMenu*			pMenu = CCMenu::create ( pToEditorItem, pToMenuItem, KD_NULL );
    pMenu->setPosition ( ccp ( tWinSize.cx / 2, pToEditorItem->getContentSize ( ).cy / 2 ) );
    this->addChild ( pMenu, 100 );
    pMenu->alignItemsHorizontallyWithPadding ( tWinSize.cx * 0.1f );
}

KDvoid GameScene::initPhysics ( KDvoid )
{
	CCSize		tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

    b2Vec2		tGravity;
    tGravity.Set ( 0.0f, -10.0f );
    m_pWorld = new b2World ( tGravity );

    // Do we want to let bodies sleep?
	m_pWorld->SetAllowSleeping ( KD_TRUE );

    m_pWorld->SetContinuousPhysics ( KD_TRUE );

	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );
    
    m_pContactListener = new MyContactListener ( );
    m_pWorld->SetContactListener ( m_pContactListener );
	
    KDuint32	uFlags = 0;
    uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
//	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;
    m_pDebugDraw->SetFlags ( uFlags );


    // Define the ground body.
    b2BodyDef	tGroundBodyDef;
    tGroundBodyDef.position.Set ( 0, 0 ); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    m_pGroundBody = m_pWorld->CreateBody ( &tGroundBodyDef );

    // Define the ground box shape.
    b2EdgeShape		tGroundBox;

    // bottom

    tGroundBox.Set ( b2Vec2 ( -tWinSize.cx / PTM_RATIO, 0 ), b2Vec2 ( 2 * tWinSize.cx / PTM_RATIO, 0 ) );
    m_pGroundBody->CreateFixture ( &tGroundBox, 0 );
    
    
    b2BodyDef		tCrocBodyDef;
    tCrocBodyDef.position.Set ( ( tWinSize.cx - m_pCroc->getTextureRect ( ).size.cx ) / PTM_RATIO, ( m_pCroc->getPosition ( ).y ) / PTM_RATIO );
    
    m_pCrocMouth = m_pWorld->CreateBody ( &tCrocBodyDef );
    
    // Define the croc's box shape.
    b2EdgeShape		tCrocBox;
    
    // bottom
    tCrocBox.Set ( b2Vec2 ( 5.0f / PTM_RATIO, 15.0f / PTM_RATIO ), b2Vec2 ( 45.0f / PTM_RATIO, 15.0f / PTM_RATIO ) );
    m_pCrocMouthBottom = m_pCrocMouth->CreateFixture ( &tCrocBox, 0 );
    
    m_pCrocMouth->SetActive ( KD_FALSE );
}

KDvoid GameScene::initLevel ( KDvoid )
{
    std::map<KDint, b2Body*>	aMap;
    
	m_pRopes  = new CCArray ( );
	m_pSticks = new CCArray ( );

    CCArray*	pPineapples = m_pLevelEditor->m_pPineapples;
    CCObject*	pObject;
    CCARRAY_FOREACH ( pPineapples, pObject )
    {
        PineappleModel*		pPM = (PineappleModel*) pObject;
        b2Body*		pBody = this->createCandyAt ( CoordinateHelper::levelPositioinToScreenPosition ( pPM->m_tPosition ) );
        pBody->SetLinearDamping ( pPM->m_fDamping );
		aMap.insert ( std::make_pair ( pPM->m_nId, pBody ) );
    }

    CCArray*	pRopes = m_pLevelEditor->m_pRopes;
    CCARRAY_FOREACH ( pRopes, pObject )
    {
        RopeModel*	pRM = (RopeModel*) pObject;
        b2Vec2		tVec1;
        b2Body*		pBody1;
        if ( pRM->m_nBodyAId == -1 ) 
		{
            pBody1 = m_pGroundBody;
            CCPoint	 tAnchorA = CoordinateHelper::levelPositioinToScreenPosition ( pRM->m_tAchorA );
            tVec1 = cc_to_b2Vec ( tAnchorA.x, tAnchorA.y );
        }
		else
		{
            pBody1 = aMap [ pRM->m_nBodyAId ];
            tVec1  = pBody1->GetLocalCenter ( );
        }
        
        b2Vec2		tVec2;
        b2Body*		pBody2;
        if ( pRM->m_nBodyBId == -1 ) 
		{
            pBody2 = m_pGroundBody;
            CCPoint  tAnchorB = CoordinateHelper::levelPositioinToScreenPosition ( pRM->m_tAchorB );
            tVec2 = cc_to_b2Vec ( tAnchorB.x, tAnchorB.y );
        }
		else
		{
            pBody2 = aMap [ pRM->m_nBodyBId ];
            tVec2  = pBody2->GetLocalCenter ( );
        }
        
        this->createRope ( pBody1, pBody2, tVec1, tVec2, pRM->m_fSagity );
    }

    
    KDint		n = 10 * 60;
    KDint32		nVelocityIterations = 8;
    KDint32		nPositionIterations = 1;
    KDfloat32	fDelta = 1.0 / 60.0f;
    while ( n-- )
    {
        // Instruct the world to perform a single step of simulation.
        m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
		
		CCObject*	pObject;
		CCARRAY_FOREACH ( m_pRopes, pObject )        
		{
			VRope*	pRope = (VRope*) pObject;

			pRope->update ( fDelta );
        }
    }
    
    // This last update takes care of the texture repositioning.
    this->update ( fDelta );
    
    m_bCrocMouthOpened = KD_TRUE;
}

KDvoid GameScene::draw ( KDvoid )
{
	CCLayer::draw ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    kmGLPushMatrix ( );

	m_pWorld->DrawDebugData ( );

    kmGLPopMatrix  ( );

    CHECK_GL_ERROR_DEBUG ( );
}

KDvoid GameScene::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch  = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPos0   = pTouch->getPreviousLocation ( );
	CCPoint		tPos1   = pTouch->getLocation ( );
	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( m_pRopes, pObject )        
	{
		VRope*		pRope    = (VRope*) pObject;
		CCArray*	pSticks  = pRope->getSticks ( );
		CCObject*	pObject2 = KD_NULL;
		CCARRAY_FOREACH ( pSticks, pObject2 )
		{
			VStick*		pStick = (VStick*) pObject2;
			CCPoint		tPosA  = pStick->getPointA ( )->getPoint ( );
			CCPoint		tPosB  = pStick->getPointB ( )->getPoint ( );
			
			if ( this->checkLineIntersection ( tPos0, tPos1, tPosA, tPosB ) )
			{
				// cut the rope here
				b2Body*		pNewBodyA = this->createRopeTipBody ( );
				b2Body*		pNewBodyB = this->createRopeTipBody ( );	           
				VRope*		pNewRope  = pRope->cutRopeInStick ( pStick, pNewBodyA, pNewBodyB );

				m_pRopes->addObject ( pNewRope );	            
				SimpleAudioEngine::sharedEngine ( )->playEffect ( kCuttingSound );
				return;
			}
		}
	}
}

KDvoid GameScene::update ( KDfloat fDelta )
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    KDint	nVelocityIterations = 8;
    KDint	nPositionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
    
    //Iterate over the bodies in the physics world
    for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
    {
        if ( b->GetUserData ( ) != KD_NULL )
		{
            // Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite*	pMyActor = (CCSprite*) b->GetUserData ( );
            pMyActor->setPosition ( ccp ( b->GetPosition ( ).x * PTM_RATIO, b->GetPosition ( ).y * PTM_RATIO ) );
            pMyActor->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( b->GetAngle ( ) ) );
        }    
    }
    

    // Update all the ropes    
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pRopes, pObject )        
	{
		VRope*	pRope = (VRope*) pObject;
        pRope->update ( fDelta );
        pRope->updateSprites ( );
    }
    

    // Check for collisions
    KDbool								bShouldCloseCrocMouth = KD_FALSE;
    std::vector<b2Body*>				aToDestroy;
    std::vector<MyContact>::iterator	itPos;

    for ( itPos = m_pContactListener->m_aContacts.begin ( ); itPos != m_pContactListener->m_aContacts.end ( ); ++itPos )
    {
        MyContact	tContact = *itPos;
        
        KDbool		bHitTheFloor = KD_FALSE;
        b2Body*		pPotentialCandy = KD_NULL;

        // The candy can hit the floor or the croc's mouth. Let's check
        // what it's touching.
        if ( tContact.pFixtureA == m_pCrocMouthBottom )
        {
            pPotentialCandy = tContact.pFixtureB->GetBody ( );
        }
        else if ( tContact.pFixtureB == m_pCrocMouthBottom )
        {
            pPotentialCandy = tContact.pFixtureA->GetBody ( );
        }
        else if ( tContact.pFixtureA->GetBody ( ) == m_pGroundBody )
        {
            pPotentialCandy = tContact.pFixtureB->GetBody ( );
            bHitTheFloor = KD_TRUE;
        }
        else if ( tContact.pFixtureB->GetBody ( ) == m_pGroundBody )
        {
            pPotentialCandy = tContact.pFixtureA->GetBody ( );
            bHitTheFloor = KD_TRUE;
        }
  
        // Check if the body was indeed one of the candies
		std::vector<b2Body*>::iterator	itCandy = std::find ( m_aCandies.begin ( ), m_aCandies.end ( ), pPotentialCandy );
        if ( pPotentialCandy && itCandy != m_aCandies.end ( ) )
        {            		
            // Set it to be destroyed
            aToDestroy.push_back ( pPotentialCandy );
            if ( bHitTheFloor )
            { 
                SimpleAudioEngine::sharedEngine ( )->playEffect ( kSplashSound );
                
				CCSize	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
				m_pLoseLabel = CCLabelTTF::create ( "Try Again",  "font/Marker Felt.ttf", 60.0f );
				m_pLoseLabel->setColor ( ccRED );
				m_pLoseLabel->setPosition ( ccpMid ( tWinSize ) );
				this->addChild ( m_pLoseLabel );

                // If it hits the floor you'll remove all the physics of it and just simulate the pineapple sinking
                CCSprite*	pSinkingCandy = (CCSprite*) pPotentialCandy->GetUserData ( );
                
                // Sink the pineapple
                CCFiniteTimeAction*	 pSink = CCMoveBy::create ( 3, ccp ( 0, -pSinkingCandy->getTextureRect ( ).size.cy ) );
                
                
                // Remove the sprite and check if should finish the level.
                CCFiniteTimeAction*	 pFinish = CCCallFuncO::create ( this, callfuncO_selector ( GameScene::finishAction ), pSinkingCandy );
                
                // Run the actions sequentially.
                pSinkingCandy->runAction ( CCSequence::create ( pSink, pFinish, KD_NULL ) );
                
                // All the physics will be destroyed below, but you don't want the
                // sprite do be removed, so you set it to null here.
                pPotentialCandy->SetUserData ( KD_NULL );
            }
            else
            {
                bShouldCloseCrocMouth = KD_TRUE;
            }
			
        }
		
    }

    std::vector<b2Body*>::iterator	itPos2;
    for ( itPos2 = aToDestroy.begin ( ); itPos2 != aToDestroy.end ( ); ++itPos2 )
    {
        b2Body*		pBody = *itPos2;
        if ( pBody->GetUserData ( ) != KD_NULL )
        {
            // Remove the sprite
            CCSprite*	pSprite = (CCSprite*) pBody->GetUserData ( );
            this->removeChild ( pSprite, KD_TRUE );
            pBody->SetUserData ( KD_NULL );
        }
        
        // Iterate though the joins and check if any are a rope
        b2JointEdge*	pJoints = pBody->GetJointList ( );
        while ( pJoints )
        {
            b2Joint*	pJoint = pJoints->joint;
            
            // Look in all the ropes
			CCARRAY_FOREACH ( m_pRopes, pObject )        
			{
				VRope*	pRope = (VRope*) pObject;

                if ( pRope->getRopeJoint ( ) == pJoint )
				{
                    m_pRopes->removeObject ( pRope );
                    break;
                }
            }
            
            pJoints = pJoints->next;
            m_pWorld->DestroyJoint ( pJoint );
        }
        
        // Destroy the physics body
        m_pWorld->DestroyBody ( pBody );
        
        // Removes from the candies array
        m_aCandies.erase ( std::find ( m_aCandies.begin ( ), m_aCandies.end ( ), pBody ) );
    }
    
    if ( bShouldCloseCrocMouth )
    {
        // If the pineapple went into the croc's mouth, immediately closes it.
        this->changeCrocAttitude ( 0 );
        
        // Check if the level should finish
        this->checkLevelFinish ( KD_FALSE );
        SimpleAudioEngine::sharedEngine ( )->playEffect ( kBiteSound );
    }
}

b2Body* GameScene::createCandyAt ( CCPoint tPos )
{
	CCSprite*	pSprite = CCSprite::createWithSpriteFrameName ( "pineapple.png" );
	this->addChild ( pSprite );

	// Defines the body of your candy
	b2BodyDef	tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position = b2Vec2 ( tPos.x / PTM_RATIO, tPos.y / PTM_RATIO );
	tBodyDef.userData = pSprite;
	tBodyDef.linearDamping = 0.3f;

	b2Body*		pBody = m_pWorld->CreateBody ( &tBodyDef );

	// Define the fixture as a polygon
	b2FixtureDef	tFixtureDef;
	b2PolygonShape	tSpriteShape;

	b2Vec2		aVerts [ ] = 
	{
		b2Vec2 (  -7.6f  / PTM_RATIO, -34.4f  / PTM_RATIO ),
		b2Vec2 (   8.3f  / PTM_RATIO, -34.4f  / PTM_RATIO ),
		b2Vec2 (  15.55f / PTM_RATIO, -27.15f / PTM_RATIO ),
		b2Vec2 (  13.8f  / PTM_RATIO,  23.05f / PTM_RATIO ),
		b2Vec2 (  -3.35f / PTM_RATIO,  35.25f / PTM_RATIO ),
		b2Vec2 ( -16.25f / PTM_RATIO,  25.55f / PTM_RATIO ),
		b2Vec2 ( -15.55f / PTM_RATIO, -23.95f / PTM_RATIO )
	};

	tSpriteShape.Set ( aVerts, 7 );
	tFixtureDef.shape = &tSpriteShape;
	tFixtureDef.density = 30.0f;
	tFixtureDef.filter.categoryBits = 0x01;
	tFixtureDef.filter.maskBits = 0x01;
	pBody->CreateFixture ( &tFixtureDef );

	m_aCandies.push_back ( pBody );
    
    return pBody;
}

KDvoid GameScene::createRope ( b2Body* pBodyA, b2Body* pBodyB, b2Vec2 tAnchorA, b2Vec2 tAnchorB, KDfloat fSagity )
{
    b2RopeJointDef		tJointDef;

    tJointDef.bodyA = pBodyA;
    tJointDef.bodyB = pBodyB;

    tJointDef.localAnchorA = tAnchorA;
    tJointDef.localAnchorB = tAnchorB;
    
    // Max length of joint = current distance between bodies * sag
    KDfloat			fRopeLength = ( pBodyA->GetWorldPoint ( tAnchorA ) - pBodyB->GetWorldPoint( tAnchorB ) ).Length ( ) * fSagity;
    tJointDef.maxLength = fRopeLength;
    
    // Create joint
    b2RopeJoint*	pRopeJoint = (b2RopeJoint*) m_pWorld->CreateJoint ( &tJointDef );    
	VRope*			pNewRope   = VRope::create ( pRopeJoint, m_pRopeSpriteSheet, PTM_RATIO );
    m_pRopes->addObject ( pNewRope );
}

b2Body* GameScene::createRopeTipBody ( KDvoid )
{
    b2BodyDef		tBodyDef;
    tBodyDef.type = b2_dynamicBody;
    tBodyDef.linearDamping = 0.5f;
    b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
    
    b2FixtureDef	tCircleDef;
    b2CircleShape	tCircle;
    tCircle.m_radius = 1.0 / PTM_RATIO;
    tCircleDef.shape = &tCircle;
    tCircleDef.density = 10.0f;
    
    // Since these tips don't have to collide with anything
    // set the mask bits to zero
    tCircleDef.filter.maskBits = 0;
    pBody->CreateFixture ( &tCircleDef );

	return pBody;
}

KDvoid GameScene::changeCrocAttitude ( KDfloat fDelta )
{
	m_bCrocMouthOpened = !m_bCrocMouthOpened;

	const KDchar*	szSpriteName = m_bCrocMouthOpened ? "croc_front_mouthopen.png" : "croc_front_mouthclosed.png";

	m_pCroc->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szSpriteName ) );

	m_pCrocMouth->SetActive ( m_bCrocMouthOpened );
}

KDbool GameScene::checkLineIntersection ( CCPoint p1, CCPoint p2, CCPoint p3, CCPoint p4 )
{
    // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
    KDfloat		fDenominator = ( p4.y - p3.y ) * ( p2.x - p1.x ) - ( p4.x - p3.x ) * ( p2.y - p1.y );
    
    // In this case the lines are parallel so you assume they don't intersect
    if ( fDenominator == 0.0f )
	{
        return KD_FALSE;
	}

    KDfloat		ua = ( ( p4.x - p3.x ) * ( p1.y - p3.y ) - ( p4.y - p3.y ) * ( p1.x - p3.x ) ) / fDenominator;
    KDfloat		ub = ( ( p2.x - p1.x ) * ( p1.y - p3.y ) - ( p2.y - p1.y ) * ( p1.x - p3.x ) ) / fDenominator;
    
    if ( ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0 )
    {
        return KD_TRUE;
    }
    
    return KD_FALSE;
}

KDvoid GameScene::switchToEditor ( CCObject* pSender )
{

}

KDvoid GameScene::switchToMenu ( CCObject* pSender )
{
    CCScene*	pEditorScene = LevelEditor::createWithLevel ( m_pLevelEditor );
    CCDirector::sharedDirector ( )->replaceScene ( pEditorScene );
}

KDvoid GameScene::checkLevelFinish ( KDbool bForceFinish )
{
    if ( m_aCandies.size ( ) == 0 || bForceFinish )
	{
        this->finishLevel ( );
        
        this->scheduleOnce ( schedule_selector ( GameScene::restartGame ), 2.0f );
    }
    
    if ( m_aCandies.size ( ) == 0 && !bForceFinish )
    {
        CCSize	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
        m_pWinLabel = CCLabelTTF::create ( "You Win!",  "font/Marker Felt.ttf", 60.0f );
        m_pWinLabel->setColor ( ccRED );
		m_pWinLabel->setPosition ( ccpMid ( tWinSize ) );
        this->addChild ( m_pWinLabel );
    }
}

KDvoid GameScene::finishLevel ( KDvoid )
{
	std::set<b2Body*>	aToDestroy;
    
    // Destroy every rope and add the objects that should be destroyed    
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pRopes, pObject )        
	{
		VRope*	pRope = (VRope*) pObject;
        pRope->removeSprites ( );
        
        if ( pRope->getRopeJoint ( )->GetBodyA ( ) != m_pGroundBody )
		{
            aToDestroy.insert ( pRope->getRopeJoint ( )->GetBodyA ( ) );
        }
        
        if ( pRope->getRopeJoint ( )->GetBodyB ( ) != m_pGroundBody ) 
		{
            aToDestroy.insert ( pRope->getRopeJoint ( )->GetBodyB ( ) );
        }
        
        m_pWorld->DestroyJoint ( pRope->getRopeJoint ( ) );
    }
    CC_SAFE_RELEASE ( m_pRopes );
        
    
    // Destroy all the objects
    std::set<b2Body *>::iterator  it;
    for ( it = aToDestroy.begin ( ); it != aToDestroy.end ( ); ++it )
    {
        b2Body*		pBody = *it;
        if ( pBody->GetUserData ( ) != KD_NULL )
        {
            // Remove the sprite
            CCSprite*	pSprite = (CCSprite*) pBody->GetUserData ( );
         
            this->removeChild  ( pSprite, KD_TRUE );
            pBody->SetUserData ( KD_NULL );
        }
        m_pWorld->DestroyBody ( pBody );
    }    
    m_aCandies.clear ( );
    
	CC_SAFE_RELEASE ( m_pSticks );
}

KDvoid GameScene::finishAction ( CCObject* pSender )
{
    this->removeChild ( (CCNode*) pSender, KD_TRUE );
    this->checkLevelFinish ( KD_TRUE );
    this->removeChild ( m_pLoseLabel, KD_TRUE );
}

KDvoid GameScene::restartGame ( KDfloat fDelta )
{
    this->removeChild ( m_pWinLabel, KD_TRUE );
    this->initLevel ( );
}
