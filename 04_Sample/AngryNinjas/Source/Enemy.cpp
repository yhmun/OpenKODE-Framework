/* --------------------------------------------------------------------------
 *
 *      File            Enemy.cpp    
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
#include "Enemy.h"
#include "GameSounds.h"

Enemy* Enemy::create		(	b2World*		pWorld, 
								const CCPoint&	tLocation, 
								const KDchar*	szSpriteFileName,
								KDbool			bIsTheRotationFixed,
								KDbool			bGetsDamageFromGround,
								KDbool			bDoesGetDamageFromDamageEnabledStackObjects,
								KDint			nBreaksFromHowMuchContact,
								KDbool			bHasDifferentSpritesForDamage,
								KDint			nNumberOfFramesToAnimateOnBreak,
								KDfloat			fDensity,
								CreationMethod	eCreateHow,
								KDint			nPoints,
								BreakEffect		eSimpleScoreVisualFXType )
{
	Enemy*		pRet = new Enemy ( );

	if ( pRet && pRet->InitWithWorld (	pWorld, 
										tLocation, 
										szSpriteFileName,
										bIsTheRotationFixed,
										bGetsDamageFromGround,
										bDoesGetDamageFromDamageEnabledStackObjects,
										nBreaksFromHowMuchContact,
										bHasDifferentSpritesForDamage,
										nNumberOfFramesToAnimateOnBreak,
										fDensity,
										eCreateHow,
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

KDbool Enemy::InitWithWorld (	b2World*		pWorld, 
								const CCPoint&	tLocation, 
								const KDchar*	szSpriteFileName,
								KDbool			bIsTheRotationFixed,
								KDbool			bGetsDamageFromGround,
								KDbool			bDoesGetDamageFromDamageEnabledStackObjects,
								KDint			nBreaksFromHowMuchContact,
								KDbool			bHasDifferentSpritesForDamage,
								KDint			nNumberOfFramesToAnimateOnBreak,
								KDfloat			fDensity,
								CreationMethod	eCreateHow,
								KDint			nPoints,
								BreakEffect		eSimpleScoreVisualFXType )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pTheWorld					 			= pWorld;
	m_tInitialLocation			 			= tLocation;
	m_sBaseImageName			 			= szSpriteFileName;
	m_sSpriteImageName			 			= ccszf ( "%s.png", m_sBaseImageName.c_str ( ) );

	m_bDamagesFromGroundContact	 			= bGetsDamageFromGround;			// does the ground break / damage the enemy
	
	m_nDamageLevel				 			= 0;								// starts at 0, if breaksAfterHowMuchContact also equals 0 then the enemy will break on first/next contact
	m_nBreaksAfterHowMuchContact 			= nBreaksFromHowMuchContact;		// contact must be made this many times before breaking, or if set to 0, the enemy will break on first/next contact 
	m_bDifferentSpritesForDamage 			= bHasDifferentSpritesForDamage;

	m_nCurrentFrame				 			= 0;
	m_nFramesToAnimateOnBreak	 			= nNumberOfFramesToAnimateOnBreak;	// will animate through breaks frames if this is more than 0, for example,  enemy_break0001.png, enemy_break0002.png
	
	m_fTheDensity				 			= fDensity;
	m_eShapeCreationMethod		 			= eCreateHow;

	m_bIsRotationFixed						= bIsTheRotationFixed;

	m_nPointValue							= nPoints;
	m_eSimpleScoreVisualFX					= eSimpleScoreVisualFXType;

	m_bDamagesFromDamageEnabledStackObjects	= bDoesGetDamageFromDamageEnabledStackObjects;	

	m_bEnemyCantBeDamagedForShortInterval	= KD_FALSE;
			
	if ( m_nDamageLevel == nBreaksFromHowMuchContact )
	{
		m_bBreaksOnNextDamage = KD_TRUE;
	}
	else
	{
		m_bBreaksOnNextDamage = KD_FALSE;
	}

	this->createEnemy ( );

	return KD_TRUE;
}

KDbool Enemy::isBreaksOnNextDamage ( KDvoid )
{
	return m_bBreaksOnNextDamage;
}

KDvoid Enemy::setBreaksOnNextDamage ( KDbool bValue )
{
	m_bBreaksOnNextDamage = bValue;
}

KDbool Enemy::isDamagesFromGroundContact ( KDvoid )
{
	return m_bDamagesFromGroundContact;
}

KDvoid Enemy::setDamagesFromGroundContact ( KDbool bValue )
{
	m_bDamagesFromGroundContact = bValue;
}

KDbool Enemy::isDamagesFromDamageEnabledStackObjects ( KDvoid )
{
	return m_bDamagesFromDamageEnabledStackObjects;
}

KDvoid Enemy::setDamagesFromDamageEnabledStackObjects ( KDbool bValue )
{
	m_bDamagesFromDamageEnabledStackObjects = bValue;
}

KDint Enemy::getPointValue ( KDvoid )
{
	return m_nPointValue;
}

KDvoid Enemy::setPointValue ( KDint nValue )
{
	m_nPointValue = nValue;
}

BreakEffect Enemy::getSimpleScoreVisualFX ( KDvoid )
{
	return m_eSimpleScoreVisualFX;
}

KDvoid Enemy::setSimpleScoreVisualFX ( BreakEffect eValue )
{
	m_eSimpleScoreVisualFX = eValue;
}

KDvoid Enemy::damageEnemy ( KDvoid )
{
	this->unschedule ( schedule_selector ( Enemy::blink ) );
	this->unschedule ( schedule_selector ( Enemy::openEyes ) );
			
	if ( !m_bEnemyCantBeDamagedForShortInterval )
	{		
		m_nDamageLevel++;
		m_bEnemyCantBeDamagedForShortInterval = KD_TRUE;

		this->scheduleOnce ( schedule_selector ( Enemy::enemyCanBeDamagedAgain ), 1.0f );

		if ( m_bDifferentSpritesForDamage )
		{
//			GameSounds::sharedGameSounds ( )->playVoiceSoundFX ( "enemyGrunt.mp3" );  // that sound file doesn't exist

			m_pSprite->setTexture 
			(
				CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_damage%d.png", m_sBaseImageName.c_str ( ), m_nDamageLevel ) ) 
			);
		}
				
		if ( m_nDamageLevel == m_nBreaksAfterHowMuchContact )
		{			
			m_bBreaksOnNextDamage = KD_TRUE;
		}		
	}
}

KDvoid Enemy::enemyCanBeDamagedAgain ( KDfloat fDelta )
{
	m_bEnemyCantBeDamagedForShortInterval = KD_FALSE;
}

KDvoid Enemy::breakEnemy ( KDvoid )
{
	this->unschedule ( schedule_selector ( Enemy::blink ) );
	this->unschedule ( schedule_selector ( Enemy::openEyes ) );

	this->schedule ( schedule_selector ( Enemy::startBreakAnimation ), 1.0f / 30.0f );		
}

KDvoid Enemy::startBreakAnimation ( KDfloat fDelta )
{
	if ( m_nCurrentFrame == 0 )
	{		
		this->removeBody ( );				
	}
	
	m_nCurrentFrame++;		// adds 1 every frame

	if ( m_nCurrentFrame <= m_nFramesToAnimateOnBreak ) 
	{ 
		// if we included frames to show for breaking and the current frame is less than the max number of frames to play	
		const KDchar*	szFile;
		if ( m_nCurrentFrame < 10 )
		{
			szFile = ccszf ( "%s_break000%d.png", m_sBaseImageName.c_str ( ), m_nCurrentFrame );
		}
		else if ( m_nCurrentFrame < 100 )
		{ 		
			szFile = ccszf ( "%s_break00%d.png", m_sBaseImageName.c_str ( ), m_nCurrentFrame );
		}		
		m_pSprite->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szFile ) );
	}

	if ( m_nCurrentFrame > m_nFramesToAnimateOnBreak )
	{ 		
		// if the currentFrame equals the number of frames to animate, we remove the sprite OR if
		// the stackObject didn't include animated images for breaking		
		this->removeSprite ( );
		this->unschedule ( schedule_selector ( Enemy::startBreakAnimation ) );		
	}
}

KDvoid Enemy::blink ( KDvoid )
{
	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_blink.png", m_sBaseImageName.c_str ( ) ) ) 
	);

	this->unschedule ( schedule_selector ( Enemy::blink ) );
	this->schedule   ( schedule_selector ( Enemy::openEyes ), 0.5f );
}

KDvoid Enemy::openEyes ( KDvoid )
{
	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s.png", m_sBaseImageName.c_str ( ) ) ) 
	);

	this->unschedule ( schedule_selector ( Enemy::openEyes ) );
	this->schedule   ( schedule_selector ( Enemy::blink ), CCRANDOM_BETWEEN ( 3.f, 8.f ) );
}

KDvoid Enemy::makeUnScoreable ( KDvoid )
{
	m_nPointValue = 0;
	CCLOG ( "points have been accumulated for this object" );
}

KDvoid Enemy::createEnemy ( KDvoid )
{
	// Define the dynamic body.
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;					// or you could use b2_staticBody
	tBodyDef.fixedRotation = m_bIsRotationFixed;
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

		case ShapeOfSourceImageButSlightlySmaller :
		{
			KDint	nNum = 4;
			b2Vec2	aVertices [ ] = 
			{
				b2Vec2 ( ( tTempSize.cx / -2 ) * .8f / PTM_RATIO, ( tTempSize.cy /  2 ) * .8f / PTM_RATIO ),	// top    left  corner
				b2Vec2 ( ( tTempSize.cx / -2 ) * .8f / PTM_RATIO, ( tTempSize.cy / -2 ) * .8f / PTM_RATIO ),	// bottom left  corner
				b2Vec2 ( ( tTempSize.cx /  2 ) * .8f / PTM_RATIO, ( tTempSize.cy / -2 ) * .8f / PTM_RATIO ),	// bottom right corner
				b2Vec2 ( ( tTempSize.cx /  2 ) * .8f / PTM_RATIO, ( tTempSize.cy /  2 ) * .8f / PTM_RATIO )		// top    right corner
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
	tFixtureDef.restitution =  0.1f;		// how bouncy basically

	this->createBodyWithSpriteAndFixture ( m_pTheWorld, &tBodyDef, &tFixtureDef, m_sSpriteImageName.c_str ( ) );

	this->schedule ( schedule_selector ( Enemy::blink ), CCRANDOM_BETWEEN ( 3.f, 8.f ) );	// comment this out if you never want to show the blink
}