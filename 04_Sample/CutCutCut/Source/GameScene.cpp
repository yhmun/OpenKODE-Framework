/* -----------------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved.
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
 *      all copies or substantial portions of the Software.
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
#include "GameScene.h"
#include "Watermelon.h"
#include "Banana.h"
#include "Bomb.h"
#include "Grapes.h"
#include "Pineapple.h"
#include "Strawberry.h"

#define calculate_determinant_2x2( x1, y1, x2, y2 )				x1 * y2 - y1 * x2
#define calculate_determinant_2x3( x1, y1, x2, y2, x3, y3 )		x1 * y2 + x2 * y3 + x3 * y1 - y1 * x2 - y2 * x3 - y3 * x1

#define frandom													(KDfloat) kdRand ( ) / 0x100000000ULL
#define frandom_range( low, high )								CCRANDOM_BETWEEN ( low, high ) //( ( high - low ) * frandom ) + low
#define random_range( low, high )								( ( kdRand ( ) % ( high - low + 1 ) ) + low )
#define midpoint(a, b )											(KDfloat) ( a + b ) / 2



/**
 *	A function that compares two b2Vec2's x coordinate
 *	Built to follow the requirements of the qsort function
 *	http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/
 */
static KDint comparator ( const KDvoid* a, const KDvoid* b )
{
    const b2Vec2*	va = (const b2Vec2*) a;
    const b2Vec2*	vb = (const b2Vec2*) b;
    
    if ( va->x > vb->x ) 
	{
        return 1;
    } 
	else if ( va->x < vb->x )
	{
        return -1;
    }

    return 0;    
}

GameScene::GameScene ( KDvoid )
{
	m_pWorld			= KD_NULL;
	m_pDebugDraw		= KD_NULL;
	m_pCaches			= KD_NULL;
	m_pRaycastCallback	= KD_NULL;	
	m_pBlade			= KD_NULL;
	m_pBlades			= KD_NULL;
}

GameScene::~GameScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pCaches );
    CC_SAFE_RELEASE ( m_pBlades );

	CC_SAFE_DELETE ( m_pWorld );
	CC_SAFE_DELETE ( m_pDebugDraw );
	CC_SAFE_DELETE ( m_pRaycastCallback );
}

/**
 *	The main init method
 */
KDbool GameScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// enable events
	this->setTouchEnabled ( KD_TRUE );

	this->initPhysics	 ( );
	this->initBackground ( );
	this->initSprites	 ( );
	this->initHUD		 ( );

	// initialize variables for toss mechanic
	m_fNextTossTime  = kdGetMilliseconds ( ) / 1000 + 1;
	m_nQueuedForToss = 0;

	// initialize variables for cutting
	m_pRaycastCallback = new RaycastCallback ( );

	// initialize the blade effect
	m_fDeltaRemainder = 0.0;
	m_pBlades = CCArray::createWithCapacity ( 3 );
	m_pBlades->retain ( );

	CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "streak.png" );

    for ( KDint i = 0; i < 3; i++ )
    {
		CCBlade*	pBlade = CCBlade::create ( 50 );
        pBlade->setAutoDim ( KD_FALSE );
        pBlade->setTexture ( pTexture );        
        this->addChild ( pBlade, 2 );
        m_pBlades->addObject ( pBlade );
    }

	// initialize the blade sparkle particle effect
	m_pBladeSparkle = CCParticleSystemQuad::create ( "blade_sparkle.plist" );
    m_pBladeSparkle->stopSystem ( );
    this->addChild ( m_pBladeSparkle, 3 );

	// initialize all sound effects
	m_uSwoosh = SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "swoosh.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "squash.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "toss_consecutive.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "toss_simultaneous.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "toss_bomb.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "lose_life.wav" );

	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "nature_bgm.mp3" );

	m_fTimeCurrent  = 0;
	m_fTimePrevious = 0;

	this->scheduleUpdate ( );

	return KD_TRUE;
}

/**
 *	Draws additional stuff to the scene
 */
KDvoid GameScene::draw ( KDvoid )
{
	CCLayer::draw ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    kmGLPushMatrix ( );

//	m_pWorld->DrawDebugData ( );

    kmGLPopMatrix  ( );

    CHECK_GL_ERROR_DEBUG ( );
}

/**
 *	Main update loop
 */
KDvoid GameScene::update ( KDfloat fDelta )
{
    // It is recommended that a fixed time step is used with Box2D for stability
    // of the simulation, however, we are using a variable time step here.
    // You need to make an informed choice, the following URL is useful
    // http://gafferongames.com/game-physics/fix-your-timestep/
    
    KDint	nVelocityIterations = 8;
    KDint	nPositionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );

    // tossing
    this->spriteLoop ( );

	// slicing
	this->checkAndSliceObjects ( );
    
    // cleaning
    this->cleanSprites ( );

    // handles the fading trail of the blade effect
    if ( m_pBlade && m_pBlade->getPath ( )->count ( ) > 3 )
	{
        m_fDeltaRemainder += fDelta * 60 * 1.2f;
        KDint		nPop = (KDint) kdRoundf ( m_fDeltaRemainder );
        m_fDeltaRemainder -= nPop;
        m_pBlade->pop ( nPop );
    }
    
    // update the time used by the swoosh sound
    m_fTimeCurrent += fDelta;
}

KDvoid GameScene::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = pTouch->getLocation ( );

	m_tEndPoint = m_tStartPoint	= tLocation;

    // get an unused blade and move it to the touch
    CCBlade*	pBlade;
	CCObject*	pObject;
    CCARRAY_FOREACH ( m_pBlades, pObject )
    {
		pBlade = (CCBlade*) pObject;

        if ( pBlade->getPath ( )->count ( ) == 0 )
        {
            m_pBlade = pBlade;
            m_pBlade->push ( tLocation );
            break;
        }
    }
    
    // move the sparkle to the touch
    m_pBladeSparkle->setPosition ( tLocation );
    m_pBladeSparkle->resetSystem ( );
}

KDvoid GameScene::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = pTouch->getLocation ( );

	m_tEndPoint = tLocation;

	if ( ccpLengthSQ ( ccpSub ( m_tStartPoint, m_tEndPoint ) ) > 25 )
	{
		m_pWorld->RayCast
		(
			m_pRaycastCallback, 
			b2Vec2 ( m_tStartPoint.x / PTM_RATIO, m_tStartPoint.y / PTM_RATIO ),
			b2Vec2 ( m_tEndPoint.x   / PTM_RATIO, m_tEndPoint.y   / PTM_RATIO ) 
		);

		m_pWorld->RayCast 
		(
			m_pRaycastCallback, 
			b2Vec2 ( m_tEndPoint.x	 / PTM_RATIO, m_tEndPoint.y	  / PTM_RATIO ),
			b2Vec2 ( m_tStartPoint.x / PTM_RATIO, m_tStartPoint.y / PTM_RATIO ) 
		);

		m_tStartPoint = m_tEndPoint;

        // add a point to the blade
        m_pBlade->push ( tLocation );
        
        KDfloat		fDeltaTime = m_fTimeCurrent - m_fTimePrevious;
        m_fTimePrevious = m_fTimeCurrent;
        
        // calculate the velocity (distance / time)
        CCPoint		tOldPosition = m_pBladeSparkle->getPosition ( );
        
        // sparkle follows the touch
        m_pBladeSparkle->setPosition ( tLocation );
    
        // play the sound if velocity is past a certain value
        if ( ccpDistance ( m_pBladeSparkle->getPosition ( ), tOldPosition ) / fDeltaTime > 1000 )
        {
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "swoosh.wav" );
        }
	}
}

KDvoid GameScene::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    // remove all entry and exit points from all polygons
    this->clearSlices ( );
    
    // fade the blade
    m_pBlade->setAutoDim ( KD_TRUE );
    
    // sparkle effect stops
    m_pBladeSparkle->stopSystem ( );
}

/**
 *	Initializes the Box2D physics world
 */
KDvoid GameScene::initPhysics ( KDvoid )
{
	CCSize			tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

    b2Vec2			tGravity;
    tGravity.Set ( 0.0f, -4.25f );
    m_pWorld = new b2World ( tGravity );

    // Do we want to let bodies sleep?
	m_pWorld->SetAllowSleeping ( KD_TRUE );

    m_pWorld->SetContinuousPhysics ( KD_TRUE );

	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );
    

    KDuint32		uFlags = 0;
    uFlags += b2Draw::e_shapeBit;
//	uFlags += b2Draw::e_jointBit;
//	uFlags += b2Draw::e_aabbBit;
//	uFlags += b2Draw::e_pairBit;
//	uFlags += b2Draw::e_centerOfMassBit;
    m_pDebugDraw->SetFlags ( uFlags );


    // Define the ground body.
    b2BodyDef		tGroundBodyDef;
    tGroundBodyDef.position.Set ( 0, 0 ); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body*			pGroundBody = m_pWorld->CreateBody ( &tGroundBodyDef );

    // Define the ground box shape.
    b2EdgeShape		tGroundBox;
/*
    // bottom
    tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tWinSize.cx / PTM_RATIO, 0 ) );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );
    
    // top
    tGroundBox.Set ( b2Vec2 ( 0, tWinSize.cy / PTM_RATIO ), b2Vec2 ( tWinSize.cx / PTM_RATIO, tWinSize.cy / PTM_RATIO ) );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );

    // left
    tGroundBox.Set ( b2Vec2 ( 0, tWinSize.cy / PTM_RATIO ), b2Vec2 ( 0, 0 ) );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );

    // right
    tGroundBox.Set ( b2Vec2 ( tWinSize.cx / PTM_RATIO, tWinSize.cy / PTM_RATIO ), b2Vec2 ( tWinSize.cx / PTM_RATIO, 0 ) );
    pGroundBody->CreateFixture ( &tGroundBox, 0 );    
*/
}

/**
 *	Initializes everything in the background
 */
KDvoid GameScene::initBackground ( KDvoid )
{
	const CCSize&	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	// add the background image
	CCSprite*		pBackground = CCSprite::create ( "bg.png" );
	pBackground->setPosition ( ccpMid ( tWinSize ) );
	this->addChild ( pBackground );

	// add the particle effect
	CCParticleSystemQuad*	pSunPollen = CCParticleSystemQuad::create ( "sun_pollen.plist" );
	this->addChild ( pSunPollen );
}

/**
 *	Initializes the sprite cache and populates it with sprites
 */
KDvoid GameScene::initSprites ( KDvoid )
{
	m_pCaches = CCArray::createWithCapacity ( 53 );
	m_pCaches->retain ( );

    // create fruits
    for ( KDint i = 0; i < 10; i++ )
    {
		PolygonSpriteEx*	pSprite = Watermelon::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }

    for ( KDint i = 0; i < 10; i++ )
    {
		PolygonSpriteEx*	pSprite = Strawberry::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }

    for ( KDint i = 0; i < 10; i++ )
    {
		PolygonSpriteEx*	pSprite = Pineapple::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }

    for ( KDint i = 0; i < 10; i++ )
    {
		PolygonSpriteEx*	pSprite = Grapes::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }

    for ( KDint i = 0; i < 10; i++ )
    {
		PolygonSpriteEx*	pSprite = Banana::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }
    
    // create bombs
    for ( KDint i = 0; i < 3; i++ )
    {
		PolygonSpriteEx*	pSprite = Bomb::createWithWorld ( m_pWorld );
		pSprite->setPosition ( ccp ( -64 * ( i + 1 ), -64 ) );
        this->addChild ( pSprite, 1 );
        this->addChild ( pSprite->getSplurt ( ), 3 );
        m_pCaches->addObject ( pSprite );
    }
}

/**
 *	Initializes the score and lives
 */
KDvoid GameScene::initHUD ( KDvoid )
{
    const CCSize&	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CCSize			tSize;
    
    m_nCuts	 = 0;
    m_nLives = 5;
    
    // add unfilled crosses at the upper right corner of the screen
    for ( KDint i = 0; i < 5; i++ )
    {
		CCSprite*	pCross = CCSprite::create ( "x_unfilled.png" );
		tSize = pCross->getContentSize ( );
		pCross->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - i * tSize.cx, tWinSize.cy - tSize.cy / 2 ) );
        this->addChild ( pCross, 4 );
    }
    
    // add an icon to represent the score
	CCSprite*	pCutsIcon = CCSprite::create ( "fruit_cut.png" );
	tSize = pCutsIcon->getContentSize ( );
    pCutsIcon->setPosition ( ccp ( tSize.cx / 2, tWinSize.cy - tSize.cy / 2 ) );
    this->addChild ( pCutsIcon );
    
    // add a label that shows the score
	m_pCutsLabel = CCLabelTTF::create ( "0", "Marker Felt.ttf", 30 );
	tSize = m_pCutsLabel->getContentSize ( );
    m_pCutsLabel->setAnchorPoint ( ccp ( 0, 0.5f ) );
	m_pCutsLabel->setPosition ( ccp ( pCutsIcon->getPosition ( ).x + pCutsIcon->getContentSize ( ).cx / 2 + tSize.cx / 2, pCutsIcon->getPosition ( ).y ) );
    this->addChild ( m_pCutsLabel, 4 );
}

/**
 *	The main loop for tossing sprites. Picks out random fruits to be tossed based on a toss type.
 */
KDvoid GameScene::spriteLoop ( KDvoid )
{
	KDfloat		fCurTime = kdGetMilliseconds ( ) / 1000;
    
    // execute only when it's time to toss sprites again
    if ( fCurTime > m_fNextTossTime )
    {
        PolygonSpriteEx*	pSprite;
		CCObject*			pObject;
        
        KDint	nChance = kdRand ( ) % 8;
        if ( nChance == 0 )
        {
            CCARRAY_FOREACH ( m_pCaches, pObject )
            {
				pSprite = (PolygonSpriteEx*) pObject;

                if ( pSprite->getState ( ) == kStateIdle && pSprite->getType ( ) == kTypeBomb )
                {
                    this->tossSprite ( pSprite );
					SimpleAudioEngine::sharedEngine ( )->playEffect ( "toss_bomb.wav" );
                    break;
                }
            }
        }
	
        KDint	nRandom = random_range ( 0, 4 );
        // if we haven't run out of fruits to toss for consecutive tossing, toss another
        Type	eType = (Type) nRandom;
        if ( m_eCurrentTossType == kTossConsecutive && m_nQueuedForToss > 0 )
        {
            CCARRAY_FOREACH ( m_pCaches, pObject )
            {
				pSprite = (PolygonSpriteEx*) pObject;

                if ( pSprite->getState ( ) == kStateIdle && pSprite->getType ( ) == eType )
                {
                    this->tossSprite ( pSprite );
                    m_nQueuedForToss--;
					SimpleAudioEngine::sharedEngine ( )->playEffect ( "toss_consecutive.wav" );
                    break;
                }
            }
        }
        else
        {
            // determine toss type and number of fruits to toss
            m_nQueuedForToss = random_range ( 3, 8 );
            KDint  nTossType = random_range ( 0, 1 );
            
            m_eCurrentTossType = (TossType) nTossType;
            				
            if ( m_eCurrentTossType == kTossSimultaneous )
            {			
                // toss fruits simultaneously
                CCARRAY_FOREACH ( m_pCaches, pObject )
                {
					pSprite = (PolygonSpriteEx*) pObject;

                    if ( pSprite->getState ( ) == kStateIdle && pSprite->getType ( ) == eType )
                    {
                        this->tossSprite ( pSprite );
                        m_nQueuedForToss--;
                        
                        // get a different fruit type
                        nRandom = random_range ( 0, 4 );
                        eType = (Type) nRandom;
                        
                        if ( m_nQueuedForToss == 0 )
                        {
                            break;
                        }
                    }
                }

				SimpleAudioEngine::sharedEngine ( )->playEffect ( "toss_simultaneous.wav" );			
            }
            else if ( m_eCurrentTossType == kTossConsecutive )
            {				
                // toss fruits consecutively
                CCARRAY_FOREACH ( m_pCaches, pObject )
                {
					pSprite = (PolygonSpriteEx*) pObject;

                    if ( pSprite->getState ( ) == kStateIdle && pSprite->getType ( ) == eType )
                    {
                        // just toss one fruit
                        this->tossSprite ( pSprite );
						SimpleAudioEngine::sharedEngine ( )->playEffect ( "toss_consecutive.wav" );	   
                        m_nQueuedForToss--;
                        break;
                    }
                }				
            }
        }

        // randomize an interval
        if ( m_nQueuedForToss == 0 )
        {
            // if no more fruits to toss, set a longer interval
            m_fTossInterval = frandom_range ( 2, 3 );
            m_fNextTossTime = fCurTime + m_fTossInterval;
        }
        else 
        {
            // if more fruits to toss, set a shorter interval
            m_fTossInterval = frandom_range ( 0.3f, 0.8f );
            m_fNextTossTime = fCurTime + m_fTossInterval;
        }		
    }
}

/**
 *	Handles sprites that have fallen offscreen
 */
KDvoid GameScene::cleanSprites ( KDvoid )
{
    PolygonSpriteEx*	pSprite;
	CCObject*			pObject;

	//we check for all tossed sprites that have dropped offscreen and reset them
	CCARRAY_FOREACH ( m_pCaches, pObject )
	{
		pSprite = (PolygonSpriteEx*) pObject;

		if ( pSprite->getState ( ) == kStateTossed )
		{
			CCPoint		tPosition  = ccp ( pSprite->getBody ( )->GetPosition ( ).x * PTM_RATIO, pSprite->getBody ( )->GetPosition ( ).y * PTM_RATIO );
			KDfloat		fVelocityY = pSprite->getBody ( )->GetLinearVelocity ( ).y;
	        
			// this means the sprite has dropped offscreen
			if ( tPosition.y < -64 && fVelocityY < 0 )
			{
				b2Vec2		tZero ( 0, 0 );

				pSprite->setState ( kStateIdle );
				pSprite->setSliceEntered ( KD_FALSE );
				pSprite->setSliceExited  ( KD_FALSE );
				pSprite->setEntryPoint ( tZero );
				pSprite->setExitPoint  ( tZero );
				pSprite->setPosition ( ccp ( -64, -64 ) );
				pSprite->getBody ( )->SetLinearVelocity  ( b2Vec2 (0.0f, 0.0f ) );
				pSprite->getBody ( )->SetAngularVelocity ( 0.0f );
				pSprite->deactivateCollisions ( );
	            
				if ( pSprite->getType ( ) != kTypeBomb )
				{
					this->subtractLife ( );
				}
			}
		}
	}

	// we check for all sliced pieces that have dropped offscreen and remove them
	const CCSize&	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
	{
		if ( b->GetUserData ( ) != KD_NULL )
		{
			PolygonSpriteEx*	pSprite = (PolygonSpriteEx*) b->GetUserData ( );
			CCPoint				tPosition = ccp ( b->GetPosition ( ).x * PTM_RATIO, b->GetPosition ( ).y * PTM_RATIO );
			if ( tPosition.x < -64 || tPosition.x > tWinSize.cx || tPosition.y < -64 )
			{
				if ( !pSprite->isOriginal ( ) )
				{
					m_pWorld->DestroyBody ( pSprite->getBody ( ) );
					this->removeChild ( pSprite, KD_TRUE ); 
				}
			}
		}
	}
}

/**
 *	Assigns a random position, linear, and angular velocity to a sprite.
 */
KDvoid GameScene::tossSprite ( PolygonSpriteEx* pSprite )
{
	const CCSize&	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

    // set a random position and rotation rate
    CCPoint		tRandomPosition = ccp ( frandom_range ( 100, tWinSize.cx - 164 ), -64 );
    KDfloat		fRandomAngularVelocity = frandom_range ( -1, 1 );
    
    // limit the velocity based on their position so that sprites aren't tossed offscreen
    KDfloat		fModifierX = 50 * ( tRandomPosition.x - 100 ) / ( tWinSize.cx - 264 );
    KDfloat		fMin = -25.0 - fModifierX;
    KDfloat		fMax =  75.0 - fModifierX;
    
    KDfloat		fRandomXVelocity = frandom_range ( fMin, fMax );
    KDfloat		fRandomYVelocity = frandom_range ( 250, 300 );
    
    // activate and toss the sprite
    pSprite->setState ( kStateTossed );
    pSprite->setPosition ( tRandomPosition );
    pSprite->activateCollisions ( );
    pSprite->getBody ( )->SetLinearVelocity ( b2Vec2 ( fRandomXVelocity / PTM_RATIO, fRandomYVelocity / PTM_RATIO ) );
    pSprite->getBody ( )->SetAngularVelocity ( fRandomAngularVelocity );
}

/**
 *	Responsible for checking all sprites that have been cut, and splits them.
 */
KDvoid GameScene::checkAndSliceObjects ( KDvoid )
{
    KDfloat		fCurTime = kdGetMilliseconds ( ) / 1000;

    for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
    {
        if ( b->GetUserData ( ) != NULL )
		{
            PolygonSpriteEx*	pSprite = (PolygonSpriteEx*) b->GetUserData ( );
            
            if ( pSprite->isSliceEntered ( ) && fCurTime > pSprite->getSliceEntryTime ( ) ) 
            {
				// if the sprite entry time has expired, reset the state of the sprite
                pSprite->setSliceEntered ( KD_FALSE );
            }
            else if ( pSprite->isSliceEntered ( ) && pSprite->isSliceExited ( ) )
            {
				// if the sprite has been cut, then split the sprite
                this->splitPolygonSprite ( pSprite );
            }
        }
    }
}

/**
 *	Removes all slice entry and exit points of all PolygonSprites
 */
KDvoid GameScene::clearSlices ( KDvoid )
{
    for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
    {
        if ( b->GetUserData ( ) != KD_NULL )
		{
            PolygonSpriteEx*	pSprite = (PolygonSpriteEx*) b->GetUserData ( );
            pSprite->setSliceEntered ( KD_FALSE );
            pSprite->setSliceExited  ( KD_FALSE );
        }
    }
}

/**
 *	Splits a PolygonSprite into two new PolygonSprites
 */
KDvoid GameScene::splitPolygonSprite ( PolygonSpriteEx* pSprite )
{
	// declare & initialize variables to be used for later
	PolygonSprite*		pNewSprite1;
	PolygonSprite*		pNewSprite2;

	// our original shape's attributes
	b2Fixture*			pOriginalFixture = pSprite->getBody ( )->GetFixtureList ( );
	b2PolygonShape*		pOriginalPolygon = (b2PolygonShape*) pOriginalFixture->GetShape ( );
	KDint				nVertexCount = pOriginalPolygon->GetVertexCount ( );

    for ( KDint i = 0; i < nVertexCount; i++ )
    {
        b2Vec2	tPoint = pOriginalPolygon->GetVertex ( i );
    }

    // our determinant(to be described later) and iterator
    KDfloat		fDeterminant;
    KDint		i;
            	
    // we store the vertices of our two new sprites here
    b2Vec2*		pSprite1Vertices = (b2Vec2*) kdCalloc ( 24, sizeof ( b2Vec2 ) );
    b2Vec2*		pSprite2Vertices = (b2Vec2*) kdCalloc ( 24, sizeof ( b2Vec2 ) );
    b2Vec2*		pSprite1VerticesSorted;
	b2Vec2*		pSprite2VerticesSorted;

    // we store how many vertices there are for each of the two new sprites here
    KDint		nSprite1VertexCount = 0;
    KDint		nSprite2VertexCount = 0;

    // step 1:
    // the entry and exit point of our cut are considered vertices of our two new shapes, so we add these before anything else
    pSprite1Vertices [ nSprite1VertexCount++ ] = pSprite->getEntryPoint ( );
    pSprite1Vertices [ nSprite1VertexCount++ ] = pSprite->getExitPoint  ( );
    pSprite2Vertices [ nSprite2VertexCount++ ] = pSprite->getEntryPoint ( );
    pSprite2Vertices [ nSprite2VertexCount++ ] = pSprite->getExitPoint  ( );
 
    //step 2:
    // iterate through all the vertices clockwise and counter-clockwise from the cutting line, and add them to each sprite's shape
	for ( i = 0; i < nVertexCount; i++ )
    {
        //get our vertex from the polygon
        b2Vec2		tPoint = pOriginalPolygon->GetVertex ( i );
        
        //we check if our point is not the same as our entry or exit point first
        b2Vec2		tDiffFromEntryPoint = tPoint - pSprite->getEntryPoint ( );
        b2Vec2		tDiffFromExitPoint  = tPoint - pSprite->getExitPoint  ( );
        
        if ( ( tDiffFromEntryPoint.x == 0 && tDiffFromEntryPoint.y == 0 ) || ( tDiffFromExitPoint.x == 0 && tDiffFromExitPoint.y == 0 ) )
        {
        }
        else 
        {
			// mathematically determine the direction of a point
            fDeterminant = calculate_determinant_2x3 ( pSprite->getEntryPoint ( ).x, pSprite->getEntryPoint ( ).y, pSprite->getExitPoint ( ).x, pSprite->getExitPoint ( ).y, tPoint.x, tPoint.y );
            
            if ( fDeterminant > 0 )
            {
                // if the determinant is positive, then the three points are in clockwise order
                pSprite1Vertices [ nSprite1VertexCount++ ] = tPoint;
            }
            else
            {
                // if the determinant is 0, the points are on the same line. if the determinant is negative, then they are in counter-clockwise order
                pSprite2Vertices [ nSprite2VertexCount++ ] = tPoint;                
            }
        }
    }

    // step 3:
    // Box2D needs vertices to be arranged in counter-clockwise order so we reorder our points using a custom function
    pSprite1VerticesSorted = this->arrangeVertices ( pSprite1Vertices, nSprite1VertexCount );
    pSprite2VerticesSorted = this->arrangeVertices ( pSprite2Vertices, nSprite2VertexCount );
                   
    // step 4:
    // Box2D has some restrictions with defining shapes, so we have to consider these. We only cut the shape if both shapes pass certain requirements from our function
    KDbool		bSprite1VerticesAcceptable = this->areVerticesAcceptable ( pSprite1VerticesSorted, nSprite1VertexCount );
    KDbool		bSprite2VerticesAcceptable = this->areVerticesAcceptable ( pSprite2VerticesSorted, nSprite2VertexCount );
    
    // step 5:
    // we destroy the old shape and create the new shapes and sprites
    if ( bSprite1VerticesAcceptable && bSprite2VerticesAcceptable )
    {
        b2Vec2		tWorldEntry = pSprite->getBody ( )->GetWorldPoint ( pSprite->getEntryPoint ( ) );
        b2Vec2		tWorldExit	= pSprite->getBody ( )->GetWorldPoint ( pSprite->getExitPoint  ( ) );
        KDfloat		fAngle		= ccpToAngle ( ccpSub ( ccp ( tWorldExit.x, tWorldExit.y ), ccp ( tWorldEntry.x, tWorldEntry.y ) ) );
        CCPoint		tVector1	= ccpForAngle ( fAngle + 1.570796f );
        CCPoint		tVector2	= ccpForAngle ( fAngle - 1.570796f );
        
        // calculate the midpoint based on world coordinates
        KDfloat		fMidX = midpoint ( tWorldEntry.x, tWorldExit.x );
        KDfloat		fMidY = midpoint ( tWorldEntry.y, tWorldExit.y );

        // create the first sprite's body        
        b2Body*		pBody1 = this->createBodyWithPosition ( pSprite->getBody ( )->GetPosition ( ), pSprite->getBody ( )->GetAngle ( ), pSprite1VerticesSorted, nSprite1VertexCount, pOriginalFixture->GetDensity ( ), pOriginalFixture->GetFriction ( ), pOriginalFixture->GetRestitution ( ) );
        
        // create the first sprite
		pNewSprite1 = PolygonSpriteEx::createWithTexture ( pSprite->getTexture ( ),  pBody1, KD_FALSE, PTM_RATIO );
        this->addChild ( pNewSprite1, 1 );
                	
        // push the sprite away from the second sprite
        pNewSprite1->getBody ( )->ApplyLinearImpulse ( b2Vec2 ( 2 * pBody1->GetMass ( ) * tVector1.x, 2 * pBody1->GetMass ( ) * tVector1.y ), b2Vec2 ( fMidX, fMidY ) );
        
        // create the second sprite's body
        b2Body*		pBody2 = this->createBodyWithPosition ( pSprite->getBody ( )->GetPosition ( ), pSprite->getBody ( )->GetAngle ( ), pSprite2VerticesSorted, nSprite2VertexCount, pOriginalFixture->GetDensity ( ), pOriginalFixture->GetFriction ( ), pOriginalFixture->GetRestitution ( ) );
        
        // create the second sprite
		pNewSprite2 = PolygonSpriteEx::createWithTexture ( pSprite->getTexture ( ),  pBody2, KD_FALSE, PTM_RATIO );
        this->addChild ( pNewSprite2, 1 );

        // push the sprite away from the first sprite
        pNewSprite2->getBody ( )->ApplyLinearImpulse ( b2Vec2 ( 2 * pBody2->GetMass ( ) * tVector2.x, 2 * pBody2->GetMass ( ) * tVector2.y ), b2Vec2 ( fMidX, fMidY ) );
       
        // we don't need the old shape & sprite anymore so we either destroy it or squirrel it away
        if ( pSprite->isOriginal ( ) )
        {   
			b2Vec2		tZero ( 0, 0 );

            // particles should appear in the middle of the cut
            b2Vec2		tConvertedWorldEntry = b2Vec2 ( tWorldEntry.x * PTM_RATIO, tWorldEntry.y * PTM_RATIO );
            b2Vec2		tConvertedWorldExit  = b2Vec2 ( tWorldExit .x * PTM_RATIO, tWorldExit .y * PTM_RATIO );
            KDfloat		fMidX = midpoint ( tConvertedWorldEntry.x, tConvertedWorldExit.x );
            KDfloat		fMidY = midpoint ( tConvertedWorldEntry.y, tConvertedWorldExit.y );
            pSprite->getSplurt ( )->setPosition ( ccp ( fMidX, fMidY ) );
            pSprite->getSplurt ( )->resetSystem ( );

            // reset the state of the sprite
            pSprite->setState ( kStateIdle ); 

            pSprite->deactivateCollisions ( );

            pSprite->setPosition ( ccp ( -256, -256 ) );   // cast them faraway
            pSprite->setSliceEntered ( KD_FALSE );
            pSprite->setSliceExited  ( KD_FALSE );
            pSprite->setEntryPoint ( tZero );
            pSprite->setExitPoint  ( tZero );

            if ( pSprite->getType ( ) == kTypeBomb )
            {
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "explosion.wav" );
                this->subtractLife ( );
            }
            else
            {
                SimpleAudioEngine::sharedEngine ( )->playEffect ( "squash.wav" );
            }
        }
        else 
        {
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "smallcut.wav" );
            m_pWorld->DestroyBody ( pSprite->getBody ( ) );
            this->removeChild ( pSprite, KD_TRUE );
        }		
		m_nCuts++;
        m_pCutsLabel->setString ( ccszf ( "%d", m_nCuts ) );
    }
    else
    {
        pSprite->setSliceEntered ( KD_FALSE );
        pSprite->setSliceExited  ( KD_FALSE );
    }

    // free up our allocated vectors
    kdFree ( pSprite1VerticesSorted );
    kdFree ( pSprite2VerticesSorted );	
    kdFree ( pSprite1Vertices );
    kdFree ( pSprite2Vertices );
}

/**
 *	Arranges all given points in a counter clockwise order
 */
b2Vec2* GameScene::arrangeVertices ( b2Vec2* pVertices, KDint nCount )
{
    KDfloat		fDeterminant;
    KDint		nCounterClockWise = 1;
    KDint		nClockWise = nCount - 1;
    KDint		i;
    
    b2Vec2		tReferencePointA;
	b2Vec2		tReferencePointB;
    b2Vec2*		pSortedVertices = (b2Vec2*) kdCalloc ( nCount, sizeof ( b2Vec2 ) );
    
    // sort all vertices in ascending order according to their x-coordinate so we can get two points of a line
    kdQsort ( pVertices, nCount, sizeof ( b2Vec2 ), comparator );
    
    pSortedVertices [ 0 ] = pVertices [ 0 ];
    tReferencePointA = pVertices [ 0 ];					// leftmost point
    tReferencePointB = pVertices [ nCount - 1 ];		// rightmost point
    
    // we arrange the points by filling our vertices in both clockwise and counter-clockwise directions using the determinant function
    for ( i = 1; i < nCount - 1; i++ )
    {
        fDeterminant = calculate_determinant_2x3 ( tReferencePointA.x, tReferencePointA.y, tReferencePointB.x, tReferencePointB.y, pVertices [ i ].x, pVertices [ i ].y );
        if ( fDeterminant < 0 )
        {
            pSortedVertices [ nCounterClockWise++ ] = pVertices [ i ];
        }
        else 
        {
            pSortedVertices [ nClockWise-- ] = pVertices [ i ];
        }
    }
    
    pSortedVertices [ nCounterClockWise ] = pVertices [ nCount - 1 ];
    return pSortedVertices;
}

/**
 *	Determines if a shape's vertices are acceptable by Box2D standards
 */
KDbool GameScene::areVerticesAcceptable ( b2Vec2* pVertices, KDint nCount )
{
    // check 1: polygons need to at least have 3 vertices
    if ( nCount < 3 )
    {
        return KD_FALSE;
    }
    
    // check 2: the number of vertices cannot exceed b2_maxPolygonVertices
    if ( nCount > b2_maxPolygonVertices )
    {
        return KD_FALSE;
    }
    
    // check 3: Box2D needs the distance from each vertex to be greater than b2_epsilon
    KDint	 i;
    for ( i = 0; i < nCount; ++i )
    {
        KDint	i1 = i;
        KDint	i2 = i + 1 < nCount ? i + 1 : 0;
        b2Vec2	tEdge = pVertices [ i2 ] - pVertices [ i1 ];
        if ( tEdge.LengthSquared ( ) <= b2_epsilon * b2_epsilon )
        {
            return KD_FALSE;
        }
    }
    
    // check 4: Box2D needs the area of a polygon to be greater than b2_epsilon
    KDfloat		fArea = 0.0f;
    
    b2Vec2		tRef ( 0.0f,0.0f );
    
    for ( i = 0; i < nCount; ++i )
    {
        b2Vec2	p1 = tRef;
        b2Vec2	p2 = pVertices [ i ];
        b2Vec2	p3 = i + 1 < nCount ? pVertices [ i + 1 ] : pVertices [ 0 ];
        
        b2Vec2	e1 = p2 - p1;
        b2Vec2	e2 = p3 - p1;
        
        KDfloat	 D = b2Cross(e1, e2);
        
        KDfloat	 fTriangleArea = 0.5f * D;
        fArea += fTriangleArea;
    }

	// we assign a value of 0.0001 since anything further is too small to see anyway
    if ( fArea <= 0.0001f )
    {
        return KD_FALSE;
    }
    
    // check 5: Box2D requires that the shape be Convex.
    KDfloat		fDeterminant;
    KDfloat		fReferenceDeterminant;
    b2Vec2		v1 = pVertices [ 0 ] - pVertices [ nCount - 1 ];
    b2Vec2		v2 = pVertices [ 1 ] - pVertices [ 0 ];
    fReferenceDeterminant = calculate_determinant_2x2 ( v1.x, v1.y, v2.x, v2.y );
    
    for ( i = 1; i < nCount - 1; i++ )
    {
        v1 = v2;
        v2 = pVertices [ i + 1 ] - pVertices [ i ];
        fDeterminant = calculate_determinant_2x2 ( v1.x, v1.y, v2.x, v2.y );
        // we use the determinant to check direction from one point to another. A convex shape's points should only go around in one direction. The sign of the determinant determines that direction. If the sign of the determinant changes mid-way, then we have a concave shape.
        if ( fReferenceDeterminant * fDeterminant < 0.0f )
        {
            // if multiplying two determinants result to a negative value, we know that the sign of both numbers differ, hence it is concave
            return KD_FALSE;
        }
    }

	// check the last two vertices
    v1 = v2;
    v2 = pVertices [ 0 ] - pVertices [ nCount - 1 ];
    fDeterminant = calculate_determinant_2x2 ( v1.x, v1.y, v2.x, v2.y );
    if ( fReferenceDeterminant * fDeterminant < 0.0f )
    {
        return KD_FALSE;
    }

	// passed all tests
    return KD_TRUE;
}

/**
 *	Creates a Box2D body for polygons that have been split
 */
b2Body* GameScene::createBodyWithPosition ( b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution )
{
    b2BodyDef		tBodyDef;
    tBodyDef.type		= b2_dynamicBody;
    tBodyDef.position	= tPosition;
    tBodyDef.angle		= fRotation;

    b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
    
    b2FixtureDef	tFixtureDef;
    tFixtureDef.density		= fDensity;
    tFixtureDef.friction	= fFriction;
    tFixtureDef.restitution = fRestitution;
    
    b2PolygonShape	tShape;
    tShape.Set ( pVertices, nCount );
    tFixtureDef.shape = &tShape;
    pBody->CreateFixture ( &tFixtureDef );
    
    return pBody;
}

/**
 *	Subtracts the life of the player and checks if the game should end
 */
KDvoid GameScene::subtractLife ( KDvoid )
{
	const CCSize&	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CCSize			tSize;

    m_nLives--;
    
    // replace the unfilled cross with a filled cross at the correct spot
	CCSprite*		pLostLife = CCSprite::create ( "x_filled.png" );
	tSize = pLostLife->getContentSize ( );
    pLostLife->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - m_nLives * tSize.cx, tWinSize.cy - tSize.cy / 2 ) );
    this->addChild ( pLostLife, 4 );
    
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "lose_life.wav" );
    
    // end the game if there are no more lives
    if ( m_nLives <= 0 )
    {
        this->endGame ( );
    }
}

/**
 *	Unschedules the update loop and shows a menu at the end of the game
 */
KDvoid GameScene::endGame ( KDvoid )
{
	this->unscheduleUpdate ( );

	CCMenuItemLabel*	pLabel = CCMenuItemLabel::create ( CCLabelTTF::create ( "RESTART", "Marker Felt.ttf", 50 ), this, menu_selector ( GameScene::restart ) );
	CCMenu*				pMenu  = CCMenu::create ( pLabel, KD_NULL );
	pMenu->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
	this->addChild ( pMenu, 4 );
}

KDvoid GameScene::restart ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer = GameScene::create ( );
	pScene->addChild ( pLayer );
	CCDirector::sharedDirector ( )->replaceScene ( pScene );	
}
