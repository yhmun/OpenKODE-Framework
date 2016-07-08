/* --------------------------------------------------------------------------
 *
 *      File            TestChipmunk.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2011 cocos2d-x.org
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
#include "TestChipmunk.h"

enum
{
    kTagParentNode = 1,
};

enum 
{
    Z_PHYSICS_DEBUG = 100,
};

KDvoid TestChipmunk::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	// enable events
    this->setTouchEnabled ( KD_TRUE );
    this->setAccelerometerEnabled ( KD_TRUE );

    // init physics
    initPhysics ( );

    // Use batch node. Faster
    CCSpriteBatchNode*  pParent = CCSpriteBatchNode::create ( "Images/grossini_dance_atlas.png", 100 );
    m_pSpriteTexture = pParent->getTexture ( );
	this->addChild ( pParent, 0, kTagParentNode );

	addNewSpriteAtPosition ( ccp ( 200, 200 ) );

	// menu for debug layer
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );

    CCMenuItemFont*  pItem = CCMenuItemFont::create ( "Toggle debug", this, menu_selector ( TestChipmunk::toggleDebugCallback ) );
	pItem->setPosition ( this, kCCAlignmentLeft, ccp ( 50, 0 ) );
    this->addChild( CCMenu::createWithItem ( pItem ) );

	schedule ( schedule_selector ( TestChipmunk::update ), 1 / 30.f );
}

KDvoid TestChipmunk::onExit ( KDvoid )
{
    // manually Free rogue shapes
    for ( KDint i = 0; i < 4; i++ ) 
	{
        cpShapeFree ( m_pWalls [ i ] );
    }

    cpSpaceFree ( m_pSpace );

	TestBasic::onExit ( );
}

const KDchar* TestChipmunk::subtitle ( KDvoid )
{
	return "( tap screen )";
}

KDuint TestChipmunk::count ( KDvoid )
{
	return 1;
}

KDvoid TestChipmunk::toggleDebugCallback ( CCObject* pSender )
{
	m_pDebugLayer->setVisible ( !m_pDebugLayer->isVisible ( ) );
}

KDvoid TestChipmunk::initPhysics ( KDvoid )
{
    const CCSize&  s = this->getContentSize ( );

    // init chipmunk
    //cpInitChipmunk ( );

    m_pSpace = cpSpaceNew ( );

    m_pSpace->gravity = cpv ( 0, -100 );

    //
    // rogue shapes
    // We have to free them manually
    //
    
    m_pWalls[0] = cpSegmentShapeNew ( m_pSpace->staticBody, cpv (    0,    0 ), cpv ( s.cx,    0 ), 0.0f );	// bottom
    m_pWalls[1] = cpSegmentShapeNew ( m_pSpace->staticBody, cpv (    0, s.cy ), cpv ( s.cx, s.cy ), 0.0f );	// top    
    m_pWalls[2] = cpSegmentShapeNew ( m_pSpace->staticBody, cpv (    0,    0 ), cpv (    0, s.cy ), 0.0f );	// left    
    m_pWalls[3] = cpSegmentShapeNew ( m_pSpace->staticBody, cpv ( s.cx,    0 ), cpv ( s.cx, s.cy ), 0.0f );	// right

    for ( KDint i = 0; i < 4; i++ )
	{
        m_pWalls[i]->e = 1.0f;
        m_pWalls[i]->u = 1.0f;

        cpSpaceAddStaticShape ( m_pSpace, m_pWalls[i] );
    }

    // Physics debug layer
    m_pDebugLayer = CCPhysicsDebugNode::create ( m_pSpace );
    this->addChild ( m_pDebugLayer, Z_PHYSICS_DEBUG );
}

KDvoid TestChipmunk::update ( KDfloat fDelta )
{
	cpSpaceStep ( m_pSpace, fDelta );    
}

KDvoid TestChipmunk::addNewSpriteAtPosition ( CCPoint tPosition )
{
    KDint    nOffsetX, nOffsetY;

    CCNode*  pParent = getChildByTag ( kTagParentNode );

    nOffsetX = (KDint) ( CCRANDOM_0_1 ( ) * 200.0f );
    nOffsetY = (KDint) ( CCRANDOM_0_1 ( ) * 200.0f );

    nOffsetX = ( nOffsetX % 4 ) * 85;
    nOffsetY = ( nOffsetY % 3 ) * 121;

    KDint   nNum = 4;
    cpVect  aVerts [ ] =
	{
        cpv ( -24, -54 ),
        cpv ( -24,  54 ),
        cpv (  24,  54 ),
        cpv (  24, -54 ),
    };

    cpBody*  pBody = cpBodyNew ( 1.0f, cpMomentForPoly ( 1.0f, nNum, aVerts, cpvzero ) );

    pBody->p = cpv ( tPosition.x, tPosition.y );
    cpSpaceAddBody ( m_pSpace, pBody );

    cpShape*  pShape = cpPolyShapeNew ( pBody, nNum, aVerts, cpvzero );
    pShape->e = 0.5f; pShape->u = 0.5f;
    cpSpaceAddShape ( m_pSpace, pShape );

    CCPhysicsSprite*    pSprite = CCPhysicsSprite::createWithTexture ( m_pSpriteTexture, ccr ( nOffsetX, nOffsetY, 85, 121 ) );
    pParent->addChild ( pSprite );
    pSprite->setCPBody ( pBody );
    pSprite->setPosition ( tPosition );
}

KDvoid TestChipmunk::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    // Add a new body/atlas sprite at the touched location
    for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
    {
        CCTouch*  pTouch = (CCTouch*) ( *it );

        addNewSpriteAtPosition ( this->convertTouchToNodeSpace ( pTouch ) );
    }
}

KDvoid TestChipmunk::didAccelerate ( CCAcceleration* pAccelerationValue )
{
    static KDfloat  fPrevX = 0, fPrevY = 0;

	#define kFilterFactor 0.05f

    KDfloat  fAccelX = (KDfloat) pAccelerationValue->x * kFilterFactor + ( 1 - kFilterFactor ) * fPrevX;
    KDfloat  fAccelY = (KDfloat) pAccelerationValue->y * kFilterFactor + ( 1 - kFilterFactor ) * fPrevY;

    fPrevX = fAccelX;
    fPrevY = fAccelY;

    CCPoint  tPoint = ccp ( fAccelX, fAccelY );
    tPoint = ccpMult ( tPoint, 200 );
    m_pSpace->gravity = cpv ( tPoint.x, tPoint.y );
}

