/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCamera.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "actions/CCActionCamera.h"
#include "support/CCCamera.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
//
// ActionCamera Base Class
//
CCActionCamera::CCActionCamera ( KDvoid )
{
    m_fCenterXOrig  = 0.0f;
    m_fCenterYOrig  = 0.0f;
    m_fCenterZOrig  = 0.0f;
    m_fEyeXOrig     = 0.0f;
    m_fEyeYOrig     = 0.0f;
    m_fEyeZOrig     = 0.0f;
    m_fUpXOrig      = 0.0f;
    m_fUpYOrig      = 0.0f;
    m_fUpZOrig      = 0.0f;
}

KDvoid CCActionCamera::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
	
	CCCamera*  pCamera = pTarget->getCamera ( );

	pCamera->getCenterXYZ ( &m_fCenterXOrig, &m_fCenterYOrig, &m_fCenterZOrig );
	pCamera->getEyeXYZ ( &m_fEyeXOrig, &m_fEyeYOrig, &m_fEyeZOrig );
	pCamera->getUpXYZ ( &m_fUpXOrig, &m_fUpYOrig, &m_fUpZOrig );
}

CCActionInterval* CCActionCamera::reverse ( KDvoid )
{
	return CCReverseTime::create ( this );
}

//
// OrbitCamera
//
CCOrbitCamera::CCOrbitCamera ( KDvoid )
{
    m_fRadius       = 0.0f;
    m_fDeltaRadius  = 0.0f;
    m_fAngleZ       = 0.0f;
    m_fDeltaAngleZ  = 0.0f;
    m_fAngleX       = 0.0f;
    m_fDeltaAngleX  = 0.0f;
    m_fRadZ         = 0.0f;
    m_fRadDeltaZ    = 0.0f;
    m_fRadX         = 0.0f;
    m_fRadDeltaX    = 0.0f;
}

CCOrbitCamera* CCOrbitCamera::create ( KDfloat fDuration, KDfloat fRadius, KDfloat fDeltaRadius, KDfloat fAngleZ, KDfloat fDeltaAngleZ, KDfloat fAngleX, KDfloat fDeltaAngleX )
{
	CCOrbitCamera*  pRet = new CCOrbitCamera ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fRadius, fDeltaRadius, fAngleZ, fDeltaAngleZ, fAngleX, fDeltaAngleX ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCOrbitCamera::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCOrbitCamera*  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCOrbitCamera *) pZone->m_pCopyObject;
	}
	else
	{
		pRet  = new CCOrbitCamera ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInterval::copyWithZone ( pZone );

	pRet->initWithDuration ( m_fDuration, m_fRadius, m_fDeltaRadius, m_fAngleZ, m_fDeltaAngleZ, m_fAngleX, m_fDeltaAngleX );

	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDbool CCOrbitCamera::initWithDuration ( KDfloat fDuration, KDfloat fRadius, KDfloat fDeltaRadius, KDfloat fAngleZ, KDfloat fDeltaAngleZ, KDfloat fAngleX, KDfloat fDeltaAngleX )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fRadius      = fRadius;
		m_fDeltaRadius = fDeltaRadius;
		m_fAngleZ      = fAngleZ;
		m_fDeltaAngleZ = fDeltaAngleZ;
		m_fAngleX      = fAngleX;
		m_fDeltaAngleX = fDeltaAngleX;
		m_fRadDeltaZ   = CC_DEGREES_TO_RADIANS ( fDeltaAngleZ );
		m_fRadDeltaX   = CC_DEGREES_TO_RADIANS ( fDeltaAngleX );
        
		return KD_TRUE;
	}
    
	return KD_FALSE;
}

KDvoid CCOrbitCamera::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	KDfloat  fRadius, fZenith, fAzimuth;

	this->sphericalRadius ( &fRadius, &fZenith, &fAzimuth );
	
	if ( kdIsNan ( m_fRadius ) )
	{
		m_fRadius = fRadius;
	}

	if ( kdIsNan ( m_fAngleZ ) )
	{
		m_fAngleZ = CC_RADIANS_TO_DEGREES ( fZenith );
	}

	if ( kdIsNan ( m_fAngleX ) )
	{
		m_fAngleX = CC_RADIANS_TO_DEGREES ( fAzimuth );
	}

	m_fRadZ = CC_DEGREES_TO_RADIANS ( m_fAngleZ );
	m_fRadX = CC_DEGREES_TO_RADIANS ( m_fAngleX );
}

KDvoid CCOrbitCamera::update ( KDfloat fDelta )
{
	KDfloat   r  = ( m_fRadius + m_fDeltaRadius * fDelta ) * CCCamera::getZEye ( );
	KDfloat   za = m_fRadZ + m_fRadDeltaZ * fDelta;
	KDfloat   xa = m_fRadX + m_fRadDeltaX * fDelta;

	KDfloat   i  = kdSinf ( za ) * kdCosf ( xa ) * r + m_fCenterXOrig;
	KDfloat   j  = kdSinf ( za ) * kdSinf ( xa ) * r + m_fCenterYOrig;
	KDfloat   k  = kdCosf ( za ) * r + m_fCenterZOrig;

	m_pTarget->getCamera ( )->setEyeXYZ ( i, j, k );
}

KDvoid CCOrbitCamera::sphericalRadius ( KDfloat* fRadius, KDfloat* fZenith, KDfloat* fAzimuth )
{
	KDfloat   ex, ey, ez, cx, cy, cz, x, y, z;
	KDfloat   r; // radius
	KDfloat   s;

	CCCamera*  pCamera = m_pTarget->getCamera ( );
	pCamera->getEyeXYZ ( &ex, &ey, &ez );
	pCamera->getCenterXYZ ( &cx, &cy, &cz );

	x = ex - cx;
	y = ey - cy;
	z = ez - cz;

	r = kdSqrtf ( kdPowf ( x, 2 ) + kdPowf ( y, 2 ) + kdPowf ( z, 2 ) );
	s = kdSqrtf ( kdPowf ( x, 2 ) + kdPowf ( y, 2 ) );
	if ( s == 0.0f )
	{
		s = KD_FLT_EPSILON;
	}
	if ( r == 0.0f )
	{
		r = KD_FLT_EPSILON;
	}

	*fZenith = kdAcosf ( z / r );
	if( x < 0 )
	{
		*fAzimuth= KD_PI_F - kdAsinf ( y / s );
	}
	else
	{
		*fAzimuth = kdAsinf ( y / s );
	}

	*fRadius = r / CCCamera::getZEye ( );				
}


NS_CC_END