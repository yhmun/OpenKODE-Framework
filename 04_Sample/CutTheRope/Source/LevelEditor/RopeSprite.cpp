/* --------------------------------------------------------------------------
 *
 *      File            RopeSprite.cpp
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
#include "RopeSprite.h"
#include "Utils/CoordinateHelper.h"


RopeSprite::RopeSprite ( CCNode* pParent, RopeModel* pRopeModel )
{
    m_pRopeModel  = pRopeModel;    
    m_pRopeSprite = CCSprite::create ( "rope_texture.png" );

    ccTexParams		tParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE };
    m_pRopeSprite->getTexture ( )->setTexParameters ( tParams );

    this->updateRope ( );
    pParent->addChild ( m_pRopeSprite );
}

RopeSprite::~RopeSprite ( KDvoid )
{
    
}

KDint RopeSprite::getID ( KDvoid )
{
    return m_pRopeModel->m_nId;
}

CCPoint RopeSprite::getAnchorA ( KDvoid )
{
    return CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorA );
}

CCPoint RopeSprite::getAnchorB ( KDvoid )
{
    return CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorB );
}

KDint RopeSprite::getBodyAID ( KDvoid )
{
    return m_pRopeModel->m_nBodyAId;
}

KDint RopeSprite::getBodyBID ( KDvoid )
{
    return m_pRopeModel->m_nBodyBId;
}

KDvoid RopeSprite::setAnchorA ( CCPoint tPos )
{
    m_pRopeModel->m_tAchorA = CoordinateHelper::screenPositionToLevelPosition ( tPos );
    this->updateRope ( );
}

KDvoid RopeSprite::setAnchorB ( CCPoint tPos )
{
    m_pRopeModel->m_tAchorB = CoordinateHelper::screenPositionToLevelPosition ( tPos );
    this->updateRope ( );
}

KDvoid RopeSprite::updateRope ( KDvoid )
{
    CCPoint		tAnchorA = CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorA );
    CCPoint		tAnchorB = CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorB );
    
    KDfloat		fDistance = ccpDistance ( tAnchorA, tAnchorB );
    CCPoint		tStickVector = ccpSub ( ccp ( tAnchorA.x, tAnchorA.y ), ccp ( tAnchorB.x, tAnchorB.y ) );
    KDfloat		fAngle = ccpToAngle ( tStickVector );

    m_pRopeSprite->setTextureRect ( CCRectMake ( 0, 0, fDistance, m_pRopeSprite->getTexture ( )->getPixelsHigh ( ) ) );
    m_pRopeSprite->setPosition ( ccpMidpoint ( tAnchorA, tAnchorB ) );
    m_pRopeSprite->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( fAngle ) );
}

KDfloat RopeSprite::distanceToPoint ( CCPoint tPos )
{
    CCPoint		tAnchorA = CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorA );
    CCPoint		tAnchorB = CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorB );
    KDfloat		fDeltaX  = tAnchorA.x - tAnchorB.x;
    KDfloat		fDeltaY  = tAnchorA.y - tAnchorB.y;
    KDfloat		fDistance;

    if ( fDeltaX == 0 ) 
	{
        fDistance = kdFabsf ( tAnchorA.x - tPos.x );
    }
	else if ( fDeltaY == 0 )
	{
        fDistance = kdFabsf ( tAnchorA.y - tPos.y );
    } 
	else 
	{
        // calculate slope by dividing y-coordinate distance with x-coordinate distance
        KDfloat		fSlope = fDeltaY / fDeltaX;
        // calculate y-intercept of rope t = y-m*x
        KDfloat		fYIntercept = tAnchorA.y - tAnchorA.x * fSlope;
        // construct line that is orthographic to rope and goes through point, we want to calculate distance for
        KDfloat		fSlopeOrthogonal = -1 / fSlope;
        // position.y = slopeOrthogonal * position.x + yInterceptOrthogonal => solve for yInterceptOrthogonal
        KDfloat		fYInterceptOrthogonal = tPos.y - fSlopeOrthogonal * tPos.x;
        // calculate interception between rope and orthogonal line
        KDfloat		x = ( fYInterceptOrthogonal - fYIntercept ) / ( fSlope - fSlopeOrthogonal );
        KDfloat		y = fSlope * x + fYIntercept;
        
        fDistance = ccpDistance ( tPos, ccp ( x, y ) );
    }

    return fDistance;
}

CCRect RopeSprite::getBoundingBox ( KDvoid )
{
    return m_pRopeSprite->boundingBox();
}

KDvoid RopeSprite::cleanupSprite ( KDvoid )
{
    m_pRopeSprite->removeFromParentAndCleanup ( KD_TRUE );
}


KDvoid RopeSprite::setSelectedAnchorType ( EAnchorType eType )
{
    m_eSelectedAnchor = eType;
}

EAnchorType RopeSprite::getSelectedAnchorType ( KDvoid )
{
    return m_eSelectedAnchor;
}

CCPoint RopeSprite::getSelectedAnchor ( KDvoid )
{
    if ( m_eSelectedAnchor == kAnchorA )
	{
        return CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorA );
    }

    if ( m_eSelectedAnchor == kAnchorB )
	{
        return CoordinateHelper::levelPositioinToScreenPosition ( m_pRopeModel->m_tAchorB );
    }

    return ccp ( -1, -1 );
}

KDvoid RopeSprite::moveSelectedAnchorTo ( CCPoint tVector )
{
    if ( m_eSelectedAnchor == kAnchorA ) 
	{
        m_pRopeModel->m_tAchorA = CoordinateHelper::screenPositionToLevelPosition ( tVector );
    }
    
	if ( m_eSelectedAnchor == kAnchorB ) 
	{
        m_pRopeModel->m_tAchorB = CoordinateHelper::screenPositionToLevelPosition ( tVector );
    }

    this->updateRope ( );
}

KDbool RopeSprite::isValideNewAnchorID ( KDint nNewAnchorID )
{
    KDint	nUnselectedAnchorID;
    
	if ( m_eSelectedAnchor == kAnchorA )
	{
        nUnselectedAnchorID = m_pRopeModel->m_nBodyBId;
    }
	else 
	{
        nUnselectedAnchorID = m_pRopeModel->m_nBodyAId;
    }

    if ( nNewAnchorID == nUnselectedAnchorID )
	{
        return KD_FALSE;
    }

    return KD_TRUE;
}
