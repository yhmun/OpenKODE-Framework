/* --------------------------------------------------------------------------
 *
 *      File            StackObject.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "StackObject.h"

StackObject* StackObject::create (	b2World*		pWorld, 
									const CCPoint&	tLocation, 
									const KDchar*	szSpriteFileName,
									KDbool			bBreaksOnGround,
									KDbool			bBreaksFromNinja,
									KDbool			bHasAnimatedBreakFrames,
									KDbool			bDamagesEnemy,
									KDfloat			fDensity,
									CreationMethod	eCreateHow,
									KDint			nAngleChange,
									KDbool			bMakeImmovable,
									KDint			nPoints,
									BreakEffect		eSimpleScoreVisualFXType )
{
	StackObject*	pRet = new StackObject ( );

	if ( pRet && pRet->InitWithWorld (	pWorld,
										tLocation, 
										szSpriteFileName,
										bBreaksOnGround,
										bBreaksFromNinja,
										bHasAnimatedBreakFrames,
										bDamagesEnemy,
										fDensity,
										eCreateHow,
										nAngleChange,
										bMakeImmovable,
										nPoints,
										eSimpleScoreVisualFXType ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool StackObject::InitWithWorld ( b2World*		pWorld, 
									const CCPoint&	tLocation, 
									const KDchar*	szSpriteFileName,
									KDbool			bBreaksOnGround,
									KDbool			bBreaksFromNinja,
									KDbool			bHasAnimatedBreakFrames,
									KDbool			bDamagesEnemy,
									KDfloat			fDensity,
									CreationMethod	eCreateHow,
									KDint			nAngleChange,
									KDbool			bMakeImmovable,
									KDint			nPoints,
									BreakEffect		eSimpleScoreVisualFXType )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pTheWorld					= pWorld;
	m_tInitialLocation			= tLocation;
	m_sBaseImageName			= szSpriteFileName;
	m_sSpriteImageName			= ccszf ( "%s.png", m_sBaseImageName.c_str ( ) );

	m_bIsBreaksOnGroundContact	= bBreaksOnGround; 
	m_bIsBreaksOnNinjaContact	= bBreaksFromNinja; 
	m_bAddedAnimatedBreakFrames = bHasAnimatedBreakFrames;
	m_bIsCanDamageEnemy			= bDamagesEnemy;
	m_fTheDensity				= fDensity;
	m_eShapeCreationMethod		= eCreateHow;
	m_nAngle					= nAngleChange;
	m_bIsStatic					= bMakeImmovable;

	m_nCurrentFrame				= 0;
	m_nFramesToAnimate			= 10;

	m_nPointValue				= nPoints;
	m_eSimpleScoreVisualFX		= eSimpleScoreVisualFXType;

	this->createObject ( );

	return KD_TRUE;
}

KDvoid StackObject::playBreakAnimationFromNinjaContact ( KDvoid )
{
	if ( m_bIsBreaksOnNinjaContact ) 
	{
		this->schedule ( schedule_selector ( StackObject::startBreakAnimation ), 1.0f / 30.0f );
	}
}

KDvoid StackObject::playBreakAnimationFromGroundContact ( KDvoid )
{
	if ( m_bIsBreaksOnGroundContact )
	{		
		this->schedule ( schedule_selector ( StackObject::startBreakAnimation ), 1.0f / 30.0f );
	}
}

KDvoid StackObject::startBreakAnimation ( KDvoid )
{
	if ( m_nCurrentFrame == 0 )
	{		
		this->removeBody ( );				
	}
	
	m_nCurrentFrame++;		// adds 1 every frame

	if ( m_nCurrentFrame <= m_nFramesToAnimate && m_bAddedAnimatedBreakFrames ) 
	{ 
		// if we included frames to show for breaking and the current frame is less than the max number of frames to play		
		const KDchar*	szFile;
		if ( m_nCurrentFrame < 10 )
		{
			szFile = ccszf ( "%s_000%d.png", m_sBaseImageName.c_str ( ), m_nCurrentFrame );
		}
		else if ( m_nCurrentFrame < 100 )
		{ 		
			szFile = ccszf ( "%s_00%d.png", m_sBaseImageName.c_str ( ), m_nCurrentFrame );			
		}		
		m_pSprite->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szFile ) );
	}

	if ( m_nCurrentFrame > m_nFramesToAnimate || !m_bAddedAnimatedBreakFrames )
	{ 		
		// if the currentFrame equals the number of frames to animate, we remove the sprite OR if
		// the stackObject didn't include animated images for breaking		
		this->removeSpriteAndBody ( );
		this->unschedule ( schedule_selector ( StackObject::startBreakAnimation ) );
		
	}
}

KDvoid StackObject::makeUnScoreable ( KDvoid )
{
	m_nPointValue = 0;
	CCLOG ( "Points have been accumulated for this object" );
}

KDbool StackObject::isBreaksOnGroundContact ( KDvoid )
{
	return m_bIsBreaksOnGroundContact;
}

KDvoid StackObject::setBreaksOnGroundContact ( KDbool bValue )
{
	m_bIsBreaksOnGroundContact = bValue;
}

KDbool StackObject::isBreaksOnNinjaContact ( KDvoid )
{
	return m_bIsBreaksOnNinjaContact;
}

KDvoid StackObject::setBreaksOnNinjaContact ( KDbool bValue )
{
	m_bIsBreaksOnNinjaContact = bValue;
}

KDbool StackObject::isCanDamageEnemy ( KDvoid )
{
	return m_bIsCanDamageEnemy;
}

KDvoid StackObject::setCanDamageEnemy ( KDbool bValue )
{
	m_bIsCanDamageEnemy = bValue;
}

KDbool StackObject::isStatic ( KDvoid )
{
	return m_bIsStatic;
}

KDvoid StackObject::setStatic ( KDbool bValue )
{
	m_bIsStatic = bValue;
}

KDint StackObject::getPointValue ( KDvoid )
{
	return m_nPointValue;
}

KDvoid StackObject::setPointValue ( KDint nValue )
{
	m_nPointValue = nValue;
}

BreakEffect StackObject::getSimpleScoreVisualFX ( KDvoid )
{
	return m_eSimpleScoreVisualFX;
}

KDvoid StackObject::setSimpleScoreVisualFX ( BreakEffect eValue )
{
	m_eSimpleScoreVisualFX = eValue;
}

KDvoid StackObject::createObject ( KDvoid )
{
	// Define the dynamic body.
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;				// or you could use b2_staticBody
	tBodyDef.position.Set ( m_tInitialLocation.x / PTM_RATIO, m_tInitialLocation.y / PTM_RATIO );

	b2PolygonShape*	pShape = new b2PolygonShape ( );
	b2CircleShape*	pShapeCircle = new b2CircleShape ( );

	CCSprite*		pTempSprite = CCSprite::create ( m_sSpriteImageName.c_str ( ) );
	CCSize			tTempSize = pTempSprite->getContentSize ( );

	switch ( m_eShapeCreationMethod )
	{
		case DiameterOfImageForCircle :
			pShapeCircle->m_radius = tTempSize.cx / PTM_RATIO * 0.5f; 
			break;

		case ShapeOfSourceImage :
		{
			KDint	nNum = 4;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top    left  corner
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom left  corner
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom right corner
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO )		// top    right corner
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Triangle :
		{
			KDint	nNum = 3;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom left  corner
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom right corner	
				b2Vec2 (								 0, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top center of image				
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case TriangleRightAngle :
		{
			KDint	nNum = 3;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top    right corner
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top    left  corner
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom left  corner			
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Trapezoid :
		{
			KDint	nNum = 4;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx /  4 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top of image, 3/4's across
				b2Vec2 ( ( tTempSize.cx / -4 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top of image, 1/4's across
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom left  corner
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO )		// bottom right corner
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Hexagon :
		{
			KDint	nNum = 6;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -4 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top of image, 1/4 across
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO,									0 ),	// left, center
				b2Vec2 ( ( tTempSize.cx / -4 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom of image, 1/4 across
				b2Vec2 ( ( tTempSize.cx /  4 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom of image, 3/4's across
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO,									0 ),	// right, center
				b2Vec2 ( ( tTempSize.cx /  4 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO )		// top of image, 3/4's across
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Pentagon :
		{
			KDint	nNum = 5;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 (								 0, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top of image, center 
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO,									0 ),	// left, center
				b2Vec2 ( ( tTempSize.cx / -4 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom of image, 1/4 across
				b2Vec2 ( ( tTempSize.cx /  4 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom of image, 3/4's across
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO,									0 )		// right, center
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Octagon :
		{
			KDint	nNum = 8;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -6 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// use the source image octogonShape.png for reference
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy /  6 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -6 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx / -6 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx /  6 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy / -6 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy /  6 ) / PTM_RATIO ),	
				b2Vec2 ( ( tTempSize.cx /  6 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO )		
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case Parallelogram :
		{
			KDint	nNum = 4;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -4 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO ),	// top of image, 1/4 across
				b2Vec2 ( ( tTempSize.cx / -2 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom left corner
				b2Vec2 ( ( tTempSize.cx /  4 ) / PTM_RATIO, ( tTempSize.cy / -2 ) / PTM_RATIO ),	// bottom of image, 3/4's across
				b2Vec2 ( ( tTempSize.cx /  2 ) / PTM_RATIO, ( tTempSize.cy /  2 ) / PTM_RATIO )		// top right corner
			};
			pShape->Set ( aVertices, nNum );
		}	break;

		case CustomCoordinates1 :
		{
			KDint	nNum = 4;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( -64.f / PTM_RATIO,  16.f / PTM_RATIO ),	// use your own custom coordinates from a program like Vertex Helper Pro
				b2Vec2 ( -64.f / PTM_RATIO, -16.f / PTM_RATIO ),	
				b2Vec2 (  64.f / PTM_RATIO, -16.f / PTM_RATIO ),	
				b2Vec2 (  64.f / PTM_RATIO,  16.f / PTM_RATIO )		
			};
			pShape->Set ( aVertices, nNum );
		}	break;
            
        default : ;
	}

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;

	if ( m_eShapeCreationMethod == DiameterOfImageForCircle )
	{		
		tFixtureDef.shape = pShapeCircle;	
		delete pShape;
	}
	else 
	{
		tFixtureDef.shape = pShape;			
		delete pShapeCircle;
	}

	tFixtureDef.density = m_fTheDensity;
	tFixtureDef.friction = 0.3f;
	tFixtureDef.restitution =  0.1f;

	this->createBodyWithSpriteAndFixture ( m_pTheWorld, &tBodyDef, &tFixtureDef, m_sSpriteImageName.c_str ( ) );

	if ( m_nAngle != 0 )
	{		
		KDint		nCurrentAngle = (KDint) m_pBody->GetAngle ( );
		b2Vec2		tLocationInMeters = m_pBody->GetPosition ( );
		
		m_pBody->SetTransform ( tLocationInMeters, CC_DEGREES_TO_RADIANS ( nCurrentAngle + m_nAngle ) );		
	}

	if ( m_bIsStatic )
	{		
		this->makeBodyStatic ( );
	}
}