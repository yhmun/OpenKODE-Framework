/* -----------------------------------------------------------------------------------
 *
 *      File            TestVRope.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Creeng Ltd. ( Ported to cocos2d-x 12/7/2012 )
 *      Copyright (c) 2012      Flightless Ltd. ( Modified 20/4/2012 )
 *      Copyright (c) 2010      Clever Hamster Games. ( Created by patrick 14/10/2010 )
 *
 *         http://www.www.flightless.co.nz 
 *         http://www.creeng.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy 
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights 
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is furnished
 *      to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all 
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *      INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *      A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 *      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "TestVRope.h"

#define PTM_RATIO			100

enum 
{
    kTagParentNode			= 1,
    KTagRopeBatchNode		= 2
};

class VRopeSprite : public CCPhysicsSprite 
{
	public :

		VRopeSprite ( KDvoid )
		{
			m_pRope = KD_NULL; 
		}

		virtual ~VRopeSprite ( KDvoid )
		{
			CC_SAFE_RELEASE ( m_pRope );
		}

		static VRopeSprite*		createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
		{
			VRopeSprite*	pRet = new VRopeSprite ( );

			if ( pRet && pRet->initWithTexture ( pTexture, tRect ) )
			{
				pRet->autorelease ( );
			}

			return pRet;
		}

	public :

		KDvoid		createBody ( b2World* pWorld, const CCPoint& tPosition )
		{
			const CCSize&	tSize = this->getContentSize ( );
			const KDfloat	fPTMRatio = this->getPTMRatio ( );			

			b2BodyDef		tBodyDef;
			tBodyDef.type = b2_dynamicBody;
			tBodyDef.position.Set ( tPosition.x / fPTMRatio, tPosition.y / fPTMRatio );
		    
			b2Body*			pBody = pWorld->CreateBody ( &tBodyDef );
		    
			// Define another box shape for our dynamic body.
			b2PolygonShape	tDynamicBox;
			tDynamicBox.SetAsBox ( tSize.cx / 2 / fPTMRatio, tSize.cy / 2 / fPTMRatio );	// These are mid points for our 1m box
		    
			// Define the dynamic body fixture.
			b2FixtureDef	tFixtureDef;
			tFixtureDef.shape = &tDynamicBox;    
			tFixtureDef.density = 1.0f;
			tFixtureDef.friction = 0.3f;
			pBody->CreateFixture ( &tFixtureDef );    
			pBody->SetUserData ( this );

			this->setB2Body ( pBody );
		}

		KDvoid		createRope ( b2World* pWorld, b2Body* pAnchor, CCSpriteBatchNode* pBatchNode )
		{
			b2RopeJointDef		tJointDef;
		    
			// define bodies
			tJointDef.bodyA			= pAnchor; 
			tJointDef.bodyB			= this->getB2Body ( );
			tJointDef.localAnchorA	= b2Vec2 ( 0, 0 );
			tJointDef.localAnchorB	= b2Vec2 ( 0, 0 );
		    
			// define max length of joint = current distance between bodies
			tJointDef.maxLength		= ( tJointDef.bodyA->GetPosition ( ) - tJointDef.bodyB->GetPosition ( ) ).Length ( );

			b2RopeJoint*		pBodyAbodyBJoint = (b2RopeJoint*) pWorld->CreateJoint ( &tJointDef );	// create joint

			// create batchnode and vrope for joint			
			m_pRope = VRope::create ( (b2Joint*) pBodyAbodyBJoint, pBatchNode, this->getPTMRatio ( ) ); 
			m_pRope->retain ( );
		}

	public :

		VRope*			m_pRope;
};

KDbool TestVRope::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );


	CCSpriteBatchNode*	pParent = CCSpriteBatchNode::create ( "Images/blocks.png", 100 );
	m_pSpriteTexture = pParent->getTexture ( );
	this->addChild ( pParent, 0, kTagParentNode );

    CCSpriteBatchNode*	pRopeBatch = CCSpriteBatchNode::create ( "xm_supports/rope.png", 100 );
    this->addChild ( pRopeBatch, 1, KTagRopeBatchNode );


	this->initPhysics ( );
	
    // Top Anchor
    b2BodyDef	tAnchorBodyDef;
	tAnchorBodyDef.position.Set ( tLyrSize.cx / PTM_RATIO / 2, tLyrSize.cy / PTM_RATIO );	// center body on screen
	m_pTopAnchorBody = m_pWorld->CreateBody ( &tAnchorBodyDef );

	this->addNewSpriteWithCoords ( ccpMid ( tLyrSize ) );

	this->scheduleUpdate ( );
	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDuint TestVRope::count ( KDvoid )
{
	return 1;
}

KDvoid TestVRope::update ( KDfloat fDelta )
{
    // TODO : FIXED TIME STEP IMPLEMENTATION
    // It is recommended that a fixed time step is used with Box2D for stability
    // of the simulation, however, we are using a variable time step here.
    // You need to make an informed choice, the following URL is useful
    // http://gafferongames.com/game-physics/fix-your-timestep/
    
    KDint	nVelocityIterations = 8;
    KDint	nPositionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
    
    // Iterate over the bodies in the physics world
    for ( b2Body* b = m_pWorld->GetBodyList ( ); b; b = b->GetNext ( ) )
    {
        if ( b->GetUserData ( ) != KD_NULL )
		{
            // Synchronize the AtlasSprites position and rotation with the corresponding body
            VRopeSprite*	pActor = (VRopeSprite*) b->GetUserData ( );
			
            if ( pActor->m_pRope )
			{
                pActor->m_pRope->update ( fDelta );
                pActor->m_pRope->updateSprites ( );
            }			
        }    
    }
}

KDvoid TestVRope::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Add a new body/atlas sprite at the touched location
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		CCTouch*		pTouch	  = (CCTouch*) ( *it );
		CCPoint			tLocation = this->convertTouchToNodeSpace ( pTouch );

		this->addNewSpriteWithCoords ( tLocation );
	}
}

KDvoid TestVRope::initPhysics ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	b2Vec2			tGravity;
	tGravity.Set ( 0.0f, -10.0f );

	m_pWorld = new b2World ( tGravity );

	// Do we want to let bodies sleep?
	m_pWorld->SetAllowSleeping ( KD_TRUE );

	m_pWorld->SetContinuousPhysics ( KD_TRUE );

	// Define the ground body.
	b2BodyDef		tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );	// bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape ( also from a pool ).
	// The body is also added to the world.
	b2Body*			pGroundBody = m_pWorld->CreateBody ( &tGroundBodyDef );
	
	// Define the ground box shape.
	b2EdgeShape		tGroundBox;		
	
	// bottom
	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// top
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// left
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( 0,0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// right
	tGroundBox.Set ( b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
}

KDvoid TestVRope::addNewSpriteWithCoords ( const CCPoint& tPoint )
{
	// We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
	// just randomly picking one of the images
	KDint				nOffsetX = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );
	KDint				nOffsetY = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );

	CCNode*				pParent = getChildByTag ( kTagParentNode );
	VRopeSprite*		pSprite = VRopeSprite::createWithTexture ( m_pSpriteTexture, ccr ( 32 * nOffsetX, 32 * nOffsetY, 32, 32 ) );

	pSprite->setPTMRatio ( PTM_RATIO );
	pSprite->createBody  ( m_pWorld, tPoint );
	pSprite->createRope  ( m_pWorld, m_pTopAnchorBody, (CCSpriteBatchNode*) this->getChildByTag ( KTagRopeBatchNode ) );
	pSprite->setPosition ( tPoint );

	pParent->addChild ( pSprite );
}
