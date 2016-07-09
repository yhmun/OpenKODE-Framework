/* --------------------------------------------------------------------------
 *
 *      File            Board.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/22
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
#include "Board.h"
#include "Ball.h"

enum 
{
    kBoardCertGroupWall,
    kBoardCertGroupHall,
    kBoardCertGroupBall,
};

static const ccColor3B	ccBRAWN = { 110, 0, 0 };

static ccColor3B		l_aBallColors [ 15 ] =
{
    // 1列目
    ccYELLOW	,
    // 2列目
    ccGREEN		,
    ccORANGE	,
    ccGREEN		,
    ccBLACK		,
    ccBLUE		,
    // 4列目
    ccORANGE	,
    ccBRAWN		,
    ccBRAWN		,
    ccRED		,
    // 5列目
    ccORANGE	,
    ccMAGENTA	,
    ccMAGENTA	,
    ccRED		,
    ccBLUE		,
};

Board* Board::create ( b2World* pWorld )
{
	Board*	pRet = new Board ( );

	if ( pRet && pRet->initWithWorld ( pWorld ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Board::initWithWorld ( b2World* pWorld )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pWorld = pWorld;

	CCSprite*	pBoard = CCSprite::create ( "board_side.png" );
	pBoard->setPosition ( Helper::screenCenter ( ) );
	this->addChild ( pBoard );

	this->setup ( );

	return KD_TRUE;
}

KDbool Board::isInHall ( const CCPoint& tPosition )
{
    for ( KDint i = 0; i < HALL_COUNT; i++ )
	{
        CCPoint		tHall = m_aHalls [ i ];
        KDfloat		fDistance = kdSqrtf ( kdPowf ( tPosition.x - tHall.x, 2 ) + kdPowf ( tPosition.y - tHall.y, 2 ) );
        if ( fDistance < 10 )
		{
            return KD_TRUE;
        }
    }

    return KD_FALSE;
}

KDvoid Board::resetBalls ( KDvoid )
{
	CCArray*	pArray = CCArray::create ( );
	CCObject*	pObject;

	CCARRAY_FOREACH ( this->getChildren ( ), pObject )
	{
		Ball*	pBall = dynamic_cast<Ball*> ( pObject );
		if ( pBall )
		{
			pArray->addObject ( pBall );
		}
	}

	CCARRAY_FOREACH ( pArray, pObject )
	{
		CCNode*		pBall = (CCNode*) pObject;
		this->removeChild ( pBall, KD_TRUE );
	}

	this->setupAll ( KD_FALSE );
}

KDvoid Board::setup ( KDvoid )
{
	this->setupAll ( KD_TRUE );
}

KDvoid Board::setupAll ( KDbool bSetupAll )
{
	b2Vec2		aVerts [ b2_maxPolygonVertices ];
	KDint		nHallIndex = 0;

	CCDictionary*	pVertex   = CCDictionary::createWithContentsOfFile ( "vertex.plist" );
	CCArray*		pFixtures = (CCArray*) ( (CCDictionary*) ( (CCDictionary*) pVertex->objectForKey ( "bodies" ) )->objectForKey ( "board_side" ) )->objectForKey ( "fixtures" );
	CCObject*		pObject   = KD_NULL;

	CCARRAY_FOREACH ( pFixtures, pObject )
	{
		CCDictionary*	pFixture = (CCDictionary*) pObject;
		KDint			nIndex   = ( (CCString*) pFixture->objectForKey ( "filter_groupIndex" ) )->intValue ( );

		if ( nIndex == kBoardCertGroupWall && bSetupAll )
		{
			CCArray*	pPolygons = (CCArray*) pFixture->objectForKey ( "polygons" );
			CCObject*	pObject2  = KD_NULL;
			CCARRAY_FOREACH ( pPolygons, pObject2 )
			{
				CCLOG ( "polygon" );

				CCArray*	pPolygon = (CCArray*) pObject2;
				CCObject*	pObject3 = KD_NULL;
				KDint		nNum = 0;
								
				CCARRAY_FOREACH ( pPolygon, pObject3 )
				{
					CCString*	pPolygonStr = (CCString*) pObject3;
					CCPoint		tPoint = CCPointFromString ( pPolygonStr->getCString ( ) );

					aVerts [ nNum ] = b2Vec2 ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
					nNum++;

					CCLOG ( "x = %f, y = %f", tPoint.x, tPoint.y );
				}

				this->createStaticBodyWithVertices ( aVerts, nNum );
			}
		}
		else if ( nIndex == kBoardCertGroupHall && bSetupAll )
		{
			CCString*	pHallPos = (CCString*) ( (CCDictionary*) pFixture->objectForKey ( "circle" ) )->objectForKey ( "position" );			
			CCPoint		tPoint = CCPointFromString ( pHallPos->getCString ( ) );
			
			m_aHalls [ nHallIndex ] = tPoint;
			nHallIndex++;
		}
		else if ( nIndex == kBoardCertGroupBall )
		{
			CCString*	pBallPos = (CCString*) ( (CCDictionary*) pFixture->objectForKey ( "circle" ) )->objectForKey ( "position" );			
			CCPoint		tPoint = CCPointFromString ( pBallPos->getCString ( ) );

			CCString*	pBallId = (CCString*) pFixture->objectForKey ( "id" );

			if ( !pBallId->compare ( "main" ) )
			{
				Ball*	pBall = Ball::create ( m_pWorld, tPoint, KD_FALSE ); 
				pBall->setTag ( kTagMainBall );
				this->addChild ( pBall );
			}
			else
			{
				KDint		nBallNumber = pBallId->intValue ( );
				ccColor3B	tBallColor  = l_aBallColors [ nBallNumber - 1 ];
				this->addChild ( Ball::create ( m_pWorld, tPoint, tBallColor ) );
			}							
		}
	}
}

KDvoid Board::createStaticBodyWithVertices ( b2Vec2 aVertices [ ], KDint nNumVertices )
{
	// Create a body definition 
	b2BodyDef		tBodyDef;
	tBodyDef.position = Helper::toMeters ( ccpz );
	
	b2PolygonShape	tShape;
	tShape.Set ( aVertices, nNumVertices );
	
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tShape;
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.2f;
	tFixtureDef.restitution = 0.1f;
	
	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	pBody->CreateFixture ( &tFixtureDef );
}

