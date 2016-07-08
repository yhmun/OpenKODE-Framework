/* -----------------------------------------------------------------------------------
 *
 *      File            vrope.cpp
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
#include "extensions/CCVRope/vrope.h"
#include "sprite_nodes/CCSprite.h"
#include "draw_nodes/CCDrawingPrimitives.h"

NS_CC_BEGIN

// Flightless, init rope using a joint between two bodies				 	

VRope* VRope::create ( b2Body* pBodyA, b2Body* pBodyB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	VRope*		pRet = new VRope ( );

	if ( pRet && pRet->initWithBodies ( pBodyA, pBodyB, pRopeBatchNode, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VRope* VRope::create ( b2Joint* pJointAB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	VRope*		pRet = new VRope ( );

	if ( pRet && pRet->initWithJoint ( pJointAB, pRopeBatchNode, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VRope* VRope::create ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	VRope*		pRet = new VRope ( );

	if ( pRet && pRet->initWithRopeJoint ( pJoint, pRopeBatchNode, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VRope* VRope::create ( const CCPoint& tPointA, const CCPoint& tPointB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	VRope*		pRet = new VRope ( );

	if ( pRet && pRet->initWithPoints ( tPointA, tPointB, pRopeBatchNode, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VRope* VRope::create ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, CCArray* pPoints, CCArray* pSticks, CCArray* pSprites, KDfloat fPTMRatio )
{
	VRope*		pRet = new VRope ( );

	if ( pRet && pRet->initWithArrays ( pJoint, pRopeBatchNode, pPoints, pSticks, pSprites, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VRope::VRope ( KDvoid )
{
	m_nNumPoints	= 0;		
	m_pPoints		= KD_NULL;
	m_pSticks		= KD_NULL;
	m_pRopeSprites	= KD_NULL;
	m_pSpriteBatch	= KD_NULL;
	m_fAntiSagHack	= 0.f;
	m_pBodyA		= KD_NULL;
	m_pBodyB		= KD_NULL;
	m_pJointAB		= KD_NULL;
	m_pJoint		= KD_NULL;
	m_fPTMRatio		= 0;		
}

VRope::~VRope ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPoints );
	CC_SAFE_RELEASE ( m_pSticks );
	CC_SAFE_RELEASE ( m_pRopeSprites );
}

KDbool VRope::initWithBodies ( b2Body* pBodyA, b2Body* pBodyB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	m_pSpriteBatch	= pRopeBatchNode;
	m_pBodyA		= pBodyA;
	m_pBodyB		= pBodyB;
	m_pJointAB		= KD_NULL;
	m_pJoint		= KD_NULL;
	m_fPTMRatio		= fPTMRatio;	

    CCPoint			tPointA;
	CCPoint			tPointB;

	this->getPoints  ( tPointA, tPointB );
	this->createRope ( tPointA, tPointB );

	return KD_TRUE;
}

KDbool VRope::initWithJoint ( b2Joint* pJointAB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	m_pSpriteBatch	= pRopeBatchNode;
	m_pBodyA		= KD_NULL;
	m_pBodyB		= KD_NULL;
	m_pJointAB		= pJointAB;
	m_pJoint		= KD_NULL;
	m_fPTMRatio		= fPTMRatio;	

    CCPoint			tPointA;
	CCPoint			tPointB;

	this->getPoints  ( tPointA, tPointB );
	this->createRope ( tPointA, tPointB );

	return KD_TRUE;
}

KDbool VRope::initWithRopeJoint ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	m_pSpriteBatch	= pRopeBatchNode;
	m_pBodyA		= KD_NULL;
	m_pBodyB		= KD_NULL;
	m_pJointAB		= KD_NULL;
	m_pJoint		= pJoint;
	m_fPTMRatio		= fPTMRatio;	

    CCPoint			tPointA;
	CCPoint			tPointB;

	this->getPoints  ( tPointA, tPointB );
    this->createRope ( tPointA, tPointB, m_pJoint->GetMaxLength ( ) * m_fPTMRatio );

	return KD_TRUE;
}

KDbool VRope::initWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio )
{
	m_pSpriteBatch	= pRopeBatchNode;
	m_pBodyA		= KD_NULL;
	m_pBodyB		= KD_NULL;
	m_pJointAB		= KD_NULL;
	m_pJoint		= KD_NULL;
	m_fPTMRatio		= fPTMRatio;	

    this->createRope ( tPointA, tPointB );

	return KD_TRUE;
}

KDbool VRope::initWithArrays ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, CCArray* pPoints, CCArray* pSticks, CCArray* pSprites, KDfloat fPTMRatio )
{
	m_pSpriteBatch	= pRopeBatchNode;
	m_pBodyA		= KD_NULL;
	m_pBodyB		= KD_NULL;
	m_pJointAB		= KD_NULL;
	m_pJoint		= pJoint;
	m_fPTMRatio		= fPTMRatio;	

	CC_SAFE_RELEASE ( m_pPoints );
	CC_SAFE_RELEASE ( m_pSticks );
	CC_SAFE_RELEASE ( m_pRopeSprites );

	m_pPoints		= new CCArray ( );
	m_pSticks		= new CCArray ( );
	m_pRopeSprites	= new CCArray ( );

	m_pPoints->addObjectsFromArray ( pPoints );
	m_pSticks->addObjectsFromArray ( pSticks );
	m_pRopeSprites->addObjectsFromArray ( pSprites );

	m_nNumPoints	= (KDint) m_pPoints->count ( );

	return KD_TRUE;
}
KDvoid VRope::reset ( KDvoid )
{
    CCPoint		tPointA;
	CCPoint		tPointB;

	this->getPoints ( tPointA, tPointB );

	this->resetWithPoints ( tPointA, tPointB );
}

KDvoid VRope::update ( KDfloat fDelta )
{
    CCPoint		tPointA;
	CCPoint		tPointB;

	this->getPoints ( tPointA, tPointB );

    this->updateWithPoints ( tPointA, tPointB, fDelta );
}

// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
KDvoid VRope::updateWithPreIntegratedGravity ( KDfloat fDelta, KDfloat fGravityX, KDfloat fGravityY )
{
    CCPoint		tPointA;
	CCPoint		tPointB;

	this->getPoints ( tPointA, tPointB );
    
    // update points with pre-integrated gravity
    this->updateWithPoints ( tPointA, tPointB, fGravityX * fDelta, fGravityY * fDelta );
}

// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
// nb. uses current global point gravity, should probably be moved to a gravity for each rope
KDvoid VRope::updateWithPreIntegratedGravity ( KDfloat fDelta )
{
    CCPoint		tPointA;
	CCPoint		tPointB;

	this->getPoints ( tPointA, tPointB );
    
    // pre-integrate current gravity
    CCPoint		tGravity = ccpMult ( VPoint::getGravity ( ), fDelta );
        
    // update points with pre-integrated gravity
    this->updateWithPoints ( tPointA, tPointB, tGravity.x, tGravity.y );
}


// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
// nb. this uses a gravity with origin (0,0) and an average of bodyA and bodyB positions to determine which way is 'down' for each rope.
KDvoid VRope::updateWithPreIntegratedOriginGravity ( KDfloat fDelta )
{
    CCPoint		tPointA;
	CCPoint		tPointB;

	this->getPoints ( tPointA, tPointB );
    
    // pre-integrate gravity, based on average position of bodies
    CCPoint		tGravityAtPoint = ccp ( -0.5f * ( tPointA.x + tPointB.x ), -0.5f * ( tPointA.y + tPointB.y ) );
    tGravityAtPoint = ccpMult ( ccpNormalize ( tGravityAtPoint ), -10.0f * fDelta );	// nb. vrope uses negative gravity!
    
    // update points with pre-integrated gravity
    this->updateWithPoints ( tPointA, tPointB, tGravityAtPoint.x, tGravityAtPoint.y );
}

KDvoid VRope::createRope ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat fDistance )
{
	CC_SAFE_RELEASE ( m_pPoints );
	CC_SAFE_RELEASE ( m_pSticks );
	CC_SAFE_RELEASE ( m_pRopeSprites );

	m_pPoints	   = new CCArray ( );
	m_pSticks	   = new CCArray ( );
	m_pRopeSprites = new CCArray ( );

	KDint		nSegmentFactor = fDistance != 0.f ? 6 : 20; //16; //12; // increase value to have less segments per rope, decrease to have more segments
	if ( fDistance == 0.f )
	{
		fDistance = ccpDistance ( tPointA, tPointB );
	}

	m_nNumPoints = (KDint) ( fDistance / nSegmentFactor );

	CCPoint		tDiffVector = ccpSub ( tPointB, tPointA );
	KDfloat		fMultiplier = fDistance / ( m_nNumPoints - 1 );
	m_fAntiSagHack = 0.1f; // HACK: scale down rope points to cheat sag. set to 0 to disable, max suggested value 0.1
	for ( KDint i = 0; i < m_nNumPoints; i++ ) 
	{
		CCPoint		tNewVector = ccpAdd ( tPointA, ccpMult ( ccpNormalize ( tDiffVector ), fMultiplier * i * ( 1 - m_fAntiSagHack ) ) );		
		VPoint*		pNewPoint  = VPoint::create ( tNewVector );

        m_pPoints->addObject ( pNewPoint );
	}

	for ( KDint i = 0; i < m_nNumPoints - 1; i++ ) 
	{
		VPoint*		pPointA = (VPoint*) m_pPoints->objectAtIndex ( i );
		VPoint*		pPointB = (VPoint*) m_pPoints->objectAtIndex ( i + 1 );
		VStick*		pNewStick = VStick::create ( pPointA, pPointB );

        m_pSticks->addObject ( pNewStick );
	}

    if ( m_pSpriteBatch ) 
	{
		for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
		{
			VStick*		pStick  = (VStick*) m_pSticks->objectAtIndex ( i );
            VPoint*		pPoint1 = pStick->getPointA ( );
            VPoint*		pPoint2 = pStick->getPointB ( );
			
			CCPoint		tStickVector = ccpSub ( ccp ( pPoint1->x, pPoint1->y ), ccp ( pPoint2->x, pPoint2->y ) );
			KDfloat		fStickAngle  = ccpToAngle ( tStickVector );

            KDfloat		f = m_pSpriteBatch->getTextureAtlas ( )->getTexture ( )->getPixelsHigh ( ) / CC_CONTENT_SCALE_FACTOR ( );
            CCRect		r = CCRectMake ( 0, 0, fMultiplier, f );
            CCSprite*	pNewSprite = CCSprite::createWithTexture ( m_pSpriteBatch->getTexture ( ), r );

            ccTexParams		tParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
            pNewSprite->getTexture ( )->setTexParameters ( tParams );
            pNewSprite->setPosition ( ccpMidpoint ( ccp ( pPoint1->x, pPoint1->y ), ccp ( pPoint2->x, pPoint2->y ) ) );
            pNewSprite->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( fStickAngle ) );
            m_pSpriteBatch->addChild ( pNewSprite );
            m_pRopeSprites->addObject ( pNewSprite );			
		}
	}
}

KDvoid VRope::resetWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB )
{
	KDfloat		fDistance   = ccpDistance ( tPointA, tPointB );
	CCPoint		tDiffVector = ccpSub ( tPointB, tPointA );
	KDfloat		fMultiplier = fDistance / ( m_nNumPoints - 1 );
	for ( KDint i = 0; i < m_nNumPoints; i++ )
	{
		CCPoint		tTmpVector = ccpAdd ( tPointA, ccpMult ( ccpNormalize ( tDiffVector ), fMultiplier * i * ( 1 - m_fAntiSagHack ) ) );
        VPoint*		pTmpPoint  = (VPoint*) m_pPoints->objectAtIndex ( i );
        pTmpPoint->setPoint ( tTmpVector );
	}
}

KDvoid VRope::removeSprites ( KDvoid )
{
    for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
	{
		CCSprite*	pTmpSprite = (CCSprite*) m_pRopeSprites->objectAtIndex ( i );
        m_pSpriteBatch->removeChild ( pTmpSprite, KD_TRUE );
	}

    m_pRopeSprites->removeAllObjects ( );

	CC_SAFE_RELEASE ( m_pRopeSprites );
}

KDvoid VRope::updateWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat fDelta )
{
    // manually set position for first and last point of rope
    ( (VPoint*) m_pPoints->objectAtIndex ( 0 ) )->setPoint ( tPointA );
    ( (VPoint*) m_pPoints->objectAtIndex ( m_nNumPoints - 1 ) )->setPoint ( tPointB );
	
	// update points, apply gravity
	for ( KDint i = 1; i < m_nNumPoints - 1; i++ )
	{
		VPoint*		pPoint = (VPoint*) m_pPoints->objectAtIndex ( i );

        pPoint->applyGravity ( fDelta );
        pPoint->update ( );
	}
	
	// contract sticks
	KDint	nIterations = 4;
	for ( KDint j = 0; j < nIterations; j++ ) 
	{
		for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
		{
            ( (VStick*) m_pSticks->objectAtIndex ( i ) )->contract ( );
		}
	}
}

KDvoid VRope::updateWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat gxdt, KDfloat gydt )
{
	// manually set position for first and last point of rope
    ( (VPoint*) m_pPoints->objectAtIndex ( 0 ) )->setPoint ( tPointA );
    ( (VPoint*) m_pPoints->objectAtIndex ( m_nNumPoints - 1 ) )->setPoint ( tPointB );
	
	// update points, apply pre-integrated gravity
	for ( KDint i = 1; i < m_nNumPoints - 1; i++ )
	{
		VPoint*		pPoint = (VPoint*) m_pPoints->objectAtIndex ( i );

        pPoint->applyGravityxdt ( gxdt, gydt );
        pPoint->update ( );
	}
	
	// contract sticks
	KDint	nIterations = 4;
	for ( KDint j = 0; j < nIterations; j++ ) 
	{
		for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
		{
            ( (VStick*) m_pSticks->objectAtIndex ( i ) )->contract ( );
		}
	}
}

KDvoid VRope::updateSprites ( KDvoid )
{
	if ( m_pSpriteBatch ) 
	{
		for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
		{
			VStick*		pStick  = (VStick*) m_pSticks->objectAtIndex ( i );
			VPoint*		pPoint1 = pStick->getPointA ( );
			VPoint*		pPoint2 = pStick->getPointB ( );
			CCPoint		tPoint1 = ccp ( pPoint1->x, pPoint1->y );
			CCPoint		tPoint2 = ccp ( pPoint2->x, pPoint2->y );
			KDfloat		fStickAngle = ccpToAngle ( ccpSub ( tPoint1, tPoint2 ) );
			CCSprite*	pTmpSprite = (CCSprite*) m_pRopeSprites->objectAtIndex ( i );
			pTmpSprite->setPosition ( ccpMidpoint ( tPoint1, tPoint2 ) );
			pTmpSprite->setRotation ( -CC_RADIANS_TO_DEGREES ( fStickAngle ) );
		}
	}
}

KDvoid VRope::debugDraw ( KDvoid )
{
    ccDrawColor4F ( 0.0f, 0.0f, 1.0f, 1.0f );
    ccLineWidth ( 5.0f );

	for ( KDint i = 0; i < m_nNumPoints - 1; i++ )
	{
		VStick*		pStick  = (VStick*) m_pSticks->objectAtIndex ( i );
		VPoint*		pPointA = pStick->getPointA ( );
		VPoint*		pPointB = pStick->getPointB ( );

		ccDrawPoint ( ccp ( pPointA->x, pPointA->y ) );
		ccDrawPoint ( ccp ( pPointB->x, pPointB->y ) );
	}

    // restore to white and default thickness
	ccDrawColor4F ( 1.0f, 1.0f, 1.0f, 1.0f );
    ccLineWidth ( 1 );
}

CCArray* VRope::getSticks ( KDvoid )
{
	return m_pSticks;
}

b2RopeJoint* VRope::getRopeJoint ( KDvoid )
{
	return m_pJoint;
}

KDvoid VRope::setRopeJoint ( b2RopeJoint* pJoint )
{
	m_pJoint = pJoint;
}

b2Joint* VRope::getJoint ( KDvoid )
{
	return m_pJointAB;
}

KDvoid VRope::setJoint ( b2Joint* pJoint )
{	
	m_pJointAB = pJoint;
}

VRope* VRope::cutRopeInStick ( VStick* pStick, b2Body* pBodyA, b2Body* pBodyB )
{
	KDuint		i = 0;
	CCObject*	pObject;

    // 1-First, find out where in your array the rope will be cut
	KDuint		uIndex = m_pSticks->indexOfObject ( pStick ) + 1;
	
	// 3-Keep the sticks in a new array
	i = 0;
	CCArray*	pNewRopeSticks = CCArray::create ( );
	CCARRAY_FOREACH ( m_pSticks, pObject )      
	{
		if ( i++ < uIndex )
		{
			continue;
		}
	
		pNewRopeSticks->addObject ( pObject );
	}

	// 4-and remove from this object's array
	m_pSticks->removeObjectsInArray ( pNewRopeSticks );

	// 5-Same for the sprites
	i = 0;
	CCArray*	pNewRopeSprites = CCArray::create ( );
	CCARRAY_FOREACH ( m_pRopeSprites, pObject )      
	{
		if ( i++ < uIndex )
		{
			continue;
		}
	
		pNewRopeSprites->addObject ( pObject );
	}
	m_pRopeSprites->removeObjectsInArray ( pNewRopeSprites );

	// 6-Number of points is always the number of sticks + 1
	i = 0;
	CCArray*	pNewRopePoints = CCArray::create ( );
	CCARRAY_FOREACH ( m_pPoints, pObject )      
	{
		if ( i++ < uIndex )
		{
			continue;
		}
	
		pNewRopePoints->addObject ( pObject );
	}
	m_pPoints->removeObjectsInArray ( pNewRopePoints );


	// 7-The removeObjectsInRange above removed the last point of
	// this rope that now belongs to the new rope. You need to clone
	// that VPoint and add it to this rope, otherwise you'll have a
	// wrong number of points in this rope
	VPoint*		pPointOfBreak = (VPoint*) pNewRopePoints->objectAtIndex ( 0 );    
	VPoint*		pNewPoint	  = VPoint::create ( pPointOfBreak->x, pPointOfBreak->y );
	m_pPoints->addObject ( pNewPoint );


    // 7-And last: fix the last VStick of this rope to point to this new point
	// instead of the old point that now belongs to the new rope
	KDint		n = m_pSticks->count ( ) - 1;
	if ( n < 0 )
	{
		n = 0;
	}
    
	VStick*		pLastStick = (VStick*) m_pSticks->objectAtIndex ( n );
	pLastStick->setPointB ( pNewPoint );


	// 8-This will determine how long the rope is now and how long the new rope will be
	KDfloat		fCutRatio = (KDfloat) uIndex / ( m_nNumPoints - 1 );
    
	// 9-Fix my number of points
	m_nNumPoints = uIndex + 1;

	// Position in Box2d world where the new bodies will initially be
	b2Vec2		tNewBodiesPosition = b2Vec2 ( pPointOfBreak->x / m_fPTMRatio, pPointOfBreak->y / m_fPTMRatio );
    
	// Get a reference to the world to create the new joint
	b2World*	pWorld = pBodyA->GetWorld ( );
    
	// 10-Re-create the joint used in this VRope since bRopeJoint does not allow
	// to re-define the attached bodies
	b2RopeJointDef	tJointDef;
	tJointDef.bodyA = m_pJoint->GetBodyA ( );
	tJointDef.bodyB = pBodyB;
	tJointDef.localAnchorA = m_pJoint->GetLocalAnchorA ( );
	tJointDef.localAnchorB = b2Vec2 ( 0, 0 );
	tJointDef.maxLength = m_pJoint->GetMaxLength ( ) * fCutRatio;
	pBodyB->SetTransform ( tNewBodiesPosition, 0.0 );

	b2RopeJoint*	pNewJoint1 = (b2RopeJoint*) pWorld->CreateJoint ( &tJointDef ); // create joint
    
    // 11-Create the new rope joint
    tJointDef.bodyA = pBodyA;
    tJointDef.bodyB = m_pJoint->GetBodyB ( );
    tJointDef.localAnchorA = b2Vec2 ( 0, 0 );
    tJointDef.localAnchorB = m_pJoint->GetLocalAnchorB ( );
    tJointDef.maxLength = m_pJoint->GetMaxLength ( ) * ( 1 - fCutRatio );
    pBodyA->SetTransform ( tNewBodiesPosition, 0.0 );

    b2RopeJoint*	pNewJoint2 = (b2RopeJoint*) pWorld->CreateJoint ( &tJointDef ); // create joint
    
    // 12-Destroy the old joint and update to the new one
    pWorld->DestroyJoint ( m_pJoint );
    m_pJoint = pNewJoint1;

	return VRope::create ( pNewJoint2, m_pSpriteBatch, pNewRopePoints, pNewRopeSticks, pNewRopeSprites, m_fPTMRatio );
}

KDvoid VRope::getPoints ( CCPoint& tPointA, CCPoint& tPointB )
{
	if ( m_pBodyA && m_pBodyB )
	{
        tPointA = ccp ( m_pBodyA->GetPosition ( ).x * m_fPTMRatio, m_pBodyA->GetPosition ( ).y * m_fPTMRatio );
        tPointB = ccp ( m_pBodyB->GetPosition ( ).x * m_fPTMRatio, m_pBodyB->GetPosition ( ).y * m_fPTMRatio );
	}
	else if ( m_pJointAB )
	{
        tPointA = ccp ( m_pJointAB->GetAnchorA ( ).x * m_fPTMRatio, m_pJointAB->GetAnchorA ( ).y * m_fPTMRatio );
        tPointB = ccp ( m_pJointAB->GetAnchorB ( ).x * m_fPTMRatio, m_pJointAB->GetAnchorB ( ).y * m_fPTMRatio );
	}
    else if ( m_pJoint ) 
	{
        tPointA = ccp ( m_pJoint->GetAnchorA ( ).x * m_fPTMRatio, m_pJoint->GetAnchorA ( ).y * m_fPTMRatio );
        tPointB = ccp ( m_pJoint->GetAnchorB ( ).x * m_fPTMRatio, m_pJoint->GetAnchorB ( ).y * m_fPTMRatio );
    }
}

NS_CC_END