/* --------------------------------------------------------------------------
 *
 *      File            CCActionGrid3D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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
#include "actions/CCActionGrid3D.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
//
// implementation of CCWaves3D
//

CCWaves3D* CCWaves3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	CCWaves3D*  pRet = new CCWaves3D ( );

	if ( pRet )
	{
		if ( pRet->initWithDuration ( fDuration, tGridSize, uWaves, fAmplitude ) )
		{
			pRet->autorelease ( );
		}
		else
		{
			CC_SAFE_DELETE ( pRet );
		}
	}

	return pRet;	
}

KDbool CCWaves3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uWaves = uWaves;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCWaves3D::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCWaves3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCWaves3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCWaves3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uWaves, m_fAmplitude );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCWaves3D::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
	{
		for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i , j ) );

			tVertex.z += kdSinf ( KD_PI_F * fTime * m_uWaves * 2 + ( tVertex.y + tVertex.x ) * 0.01f ) * m_fAmplitude * m_fAmplitudeRate;

			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}


//
// implementation of CCFlipX3D
//

CCFlipX3D* CCFlipX3D::create ( KDfloat fDuration )
{
	CCFlipX3D*  pRet = new CCFlipX3D ( );

	if ( pRet )
	{
		if ( pRet->initWithDuration ( fDuration ) )
		{
			pRet->autorelease ( );
		}
		else
		{
			CC_SAFE_DELETE ( pRet );
		}
	}

	return pRet;
}

KDbool CCFlipX3D::initWithDuration ( KDfloat fDuration )
{
	return CCGrid3DAction::initWithDuration ( fDuration, ccs ( 1, 1 ) );
}

KDbool CCFlipX3D::initWithSize ( const CCSize& tGridSize, KDfloat fDuration )
{
	if ( tGridSize.cx != 1 || tGridSize.cy != 1 )
	{
		// Grid size must be (1,1)
		CCAssert ( 0, "Grid size must be (1,1)" );

		return KD_FALSE;
	}

	return CCGrid3DAction::initWithDuration ( fDuration, tGridSize );
}

CCObject* CCFlipX3D::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFlipX3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCFlipX3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCFlipX3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithSize ( m_tGridSize, m_fDuration );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCFlipX3D::update ( KDfloat fTime )
{
	KDfloat  fAngle = KD_PI_F * fTime;     // 180 degrees
	KDfloat  fMz    = kdSinf ( fAngle );
             fAngle = fAngle / 2.0f;               // x calculates degrees from 0 to 90
	KDfloat  fMx    = kdCosf ( fAngle );

	ccVertex3F  tVertex, tDiff;

	ccVertex3F tV0 = originalVertex ( ccp ( 1, 1 ) );
	ccVertex3F tV1 = originalVertex ( ccp ( 0, 0 ) );

	KDfloat	 fX0 = tV0.x;
	KDfloat	 fX1 = tV1.x;
	KDfloat  fX;

	CCPoint	 tA, tB, tC, tD;

	if ( fX0 > fX1 )
	{
		// Normal Grid
		tA = ccp ( 0, 0 );
		tB = ccp ( 0, 1 );
		tC = ccp ( 1, 0 );
		tD = ccp ( 1, 1 );
		fX = fX0;
	}
	else
	{
		// Reversed Grid
		tC = ccp ( 0, 0 );
		tD = ccp ( 0, 1 );
		tA = ccp ( 1, 0 );
		tB = ccp ( 1, 1 );
		fX = fX1;
	}
	
	tDiff.x = ( fX - fX * fMx );
	tDiff.z = kdFabsf ( kdFloorf ( ( fX * fMz ) / 4.0f ) );

	// bottom-left
	tVertex    = originalVertex ( tA );
	tVertex.x  = tDiff.x;
	tVertex.z += tDiff.z;
	setVertex ( tA, tVertex );
	
	// upper-left
	tVertex    = originalVertex ( tB );
	tVertex.x  = tDiff.x;
	tVertex.z += tDiff.z;
	setVertex ( tB, tVertex );
	
	// bottom-right
	tVertex    = originalVertex ( tC );
	tVertex.x -= tDiff.x;
	tVertex.z -= tDiff.z;
	setVertex ( tC, tVertex );
	
	// upper-right
	tVertex    = originalVertex ( tD );
	tVertex.x -= tDiff.x;
	tVertex.z -= tDiff.z;
	setVertex ( tD, tVertex );
}


//
// implementation of FlipY3D
//

CCFlipY3D* CCFlipY3D::create ( KDfloat fDuration )
{
	CCFlipY3D*  pRet = new CCFlipY3D ( );

	if ( pRet )
	{
		if ( pRet->initWithDuration ( fDuration ) )
		{
			pRet->autorelease ( );
		}
		else
		{
			CC_SAFE_DELETE ( pRet );
		}
	}

	return pRet;
}

CCObject* CCFlipY3D::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFlipY3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCFlipY3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCFlipY3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCFlipX3D::copyWithZone ( pZone );

	pCopy->initWithSize ( m_tGridSize, m_fDuration );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCFlipY3D::update ( KDfloat fTime )
{
	KDfloat  fAngle = KD_PI_F * fTime;				// 180 degrees
	KDfloat  fMz    = kdSinf ( fAngle );
             fAngle = fAngle / 2.0f;				// x calculates degrees from 0 to 90
	KDfloat  fMy    = kdCosf ( fAngle );
	
	ccVertex3F	tVertex, tDiff;
	
	ccVertex3F  tV0 = originalVertex ( ccp ( 1, 1 ) );
	ccVertex3F  tV1 = originalVertex ( ccp ( 0, 0 ) );
	
	KDfloat	 fY0 = tV0.y;
	KDfloat  fY1 = tV1.y;
	KDfloat  fY;

	CCPoint  tA, tB, tC, tD;
	
	if ( fY0 > fY1 )
	{
		// Normal Grid
		tA = ccp ( 0, 0 );
		tB = ccp ( 0, 1 );
		tC = ccp ( 1, 0 );
		tD = ccp ( 1, 1 );
		fY = fY0;
	}
	else
	{
		// Reversed Grid
		tB = ccp ( 0, 0 );
		tA = ccp ( 0, 1 );
		tD = ccp ( 1, 0 );
		tC = ccp ( 1, 1 );
		fY = fY1;
	}
	
	tDiff.y = fY - fY * fMy;
	tDiff.z = kdFabsf ( kdFloorf ( ( fY * fMz ) / 4.0f ) );
	
	// bottom-left
	tVertex    = originalVertex ( tA );
	tVertex.y  = tDiff.y;
	tVertex.z += tDiff.z;
	setVertex ( tA, tVertex );
	
	// upper-left
	tVertex    = originalVertex ( tB );
	tVertex.y -= tDiff.y;
	tVertex.z -= tDiff.z;
	setVertex ( tB, tVertex );
	
	// bottom-right
	tVertex    = originalVertex ( tC );
	tVertex.y  = tDiff.y;
	tVertex.z += tDiff.z;
	setVertex ( tC, tVertex );
	
	// upper-right
	tVertex    = originalVertex ( tD );
	tVertex.y -= tDiff.y;
	tVertex.z -= tDiff.z;
	setVertex ( tD, tVertex );
}


//
// implementation of Lens3D
//

CCLens3D* CCLens3D::create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius )
{
	CCLens3D*  pRet = new CCLens3D ( );

	if ( pRet )
	{
		if ( pRet->initWithDuration ( fDuration, tGridSize, tPosition, fRadius ) )
		{
			pRet->autorelease ( );
		}
		else
		{
			CC_SAFE_DELETE ( pRet );
		}
	}

	return pRet;
}

KDbool CCLens3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_tPosition   = ccp ( -1, -1 );
        setPosition ( tPosition );

		m_fRadius     = fRadius;
		m_fLensEffect = 0.7f;
		m_bConcave	  = KD_FALSE;
        m_bDirty	  = KD_TRUE;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCLens3D::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCLens3D*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCLens3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCLens3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_tPosition, m_fRadius );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCLens3D::setPosition ( const CCPoint& tPosition )
{
    if ( !tPosition.equals ( m_tPosition ) )
	{
        m_tPosition = tPosition;
        m_bDirty = KD_TRUE;
    }
}

KDvoid CCLens3D::update ( KDfloat fTime )
{
	if ( m_bDirty )
	{
		for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
		{
			for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
			{
				ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );
				CCPoint		tVector = ccpSub ( m_tPosition, ccp ( tVertex.x, tVertex.y ) );
				KDfloat		fRadius = ccpLength ( tVector );
				
				if ( fRadius < m_fRadius )
				{
					fRadius = m_fRadius - fRadius;
					KDfloat fPreLog = fRadius / m_fRadius;
					if ( fPreLog == 0 ) 
					{
						fPreLog = 0.001f;
					}

					KDfloat fLog = kdLogf ( fPreLog ) * m_fLensEffect;
					KDfloat fNewRadius = kdExpf ( fLog ) * m_fRadius;
					
					if ( ccpLength ( tVector ) > 0 )
					{
						tVector = ccpNormalize ( tVector );

						CCPoint  tNewVect = ccpMult ( tVector, fNewRadius );
						tVertex.z += ( m_bConcave ? -1.0f : 1.0f ) * ccpLength ( tNewVect ) * m_fLensEffect;
					}
				}
				
				setVertex ( ccp ( i, j ), tVertex );
			}
		}
		
		m_bDirty = KD_FALSE;
	}
}


//
// implementation of Ripple3D
//

CCRipple3D* CCRipple3D::create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude )
{
	CCRipple3D*  pAction = new CCRipple3D ( );

	if ( pAction )
	{
		if ( pAction->initWithDuration ( fDuration, tGridSize, tPosition, fRadius, uWaves, fAmplitude ) )
		{
			pAction->autorelease ( );
		}
		else
		{
			CC_SAFE_RELEASE ( pAction );
		}
	}

	return pAction;
}

KDbool CCRipple3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		setPosition ( tPosition );

		m_fRadius        = fRadius;
		m_uWaves         = uWaves;
		m_fAmplitude     = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCRipple3D::setPosition ( const CCPoint& tPosition )
{
    m_tPosition = tPosition;
}

CCObject* CCRipple3D::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCRipple3D*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCRipple3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCRipple3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_tPosition, m_fRadius, m_uWaves, m_fAmplitude );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCRipple3D::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
	{
		for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );
			CCPoint		tVector = ccpSub ( m_tPosition, ccp ( tVertex.x, tVertex.y ) );
			KDfloat		fRadius = ccpLength ( tVector );
			
			if ( fRadius < m_fRadius )
			{
				fRadius = m_fRadius - fRadius;
				KDfloat  fRate = kdPowf ( fRadius / m_fRadius, 2 );
				tVertex.z += ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + fRadius * 0.1f ) * m_fAmplitude * m_fAmplitudeRate * fRate );
			}
			
			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}


//
// implementation of Shaky3D
//

CCShaky3D* CCShaky3D::create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ )
{
	CCShaky3D*  pRet = new CCShaky3D ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, nRange, bShakeZ ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
	

	return pRet;
}

KDbool CCShaky3D::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_nRange  = nRange;
		m_bShakeZ = bShakeZ;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCShaky3D::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCShaky3D*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCShaky3D *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCShaky3D ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_nRange, m_bShakeZ );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCShaky3D::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
	{
		for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );

			tVertex.x += ( kdRand ( ) % ( m_nRange * 2 ) ) - m_nRange;
			tVertex.y += ( kdRand ( ) % ( m_nRange * 2 ) ) - m_nRange;

			if ( m_bShakeZ )
			{
				tVertex.z += ( kdRand ( ) % ( m_nRange * 2 ) ) - m_nRange;
			}
			
			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}


//
// implementation of Liquid
//

CCLiquid* CCLiquid::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	CCLiquid*  pRet = new CCLiquid ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uWaves, fAmplitude ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCLiquid::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uWaves = uWaves;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCLiquid::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCLiquid*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCLiquid *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCLiquid ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uWaves, m_fAmplitude );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCLiquid::update ( KDfloat fTime )
{
	for ( KDint i = 1; i < (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 1; j < (KDint) m_tGridSize.cy; ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );

			tVertex.x = ( tVertex.x + ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + tVertex.x * 0.01f ) * m_fAmplitude * m_fAmplitudeRate ) );
			tVertex.y = ( tVertex.y + ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + tVertex.y * 0.01f ) * m_fAmplitude * m_fAmplitudeRate ) );
			
			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}


//
// implementation of Waves
//

CCWaves* CCWaves::create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical )
{
	CCWaves*  pRet = new CCWaves ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, uWaves, fAmplitude, bHorizontal, bVertical ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCWaves::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		m_uWaves         = uWaves;
		m_fAmplitude     = fAmplitude;
		m_fAmplitudeRate = 1.0f;
		m_bHorizontal    = bHorizontal;
		m_bVertical      = bVertical;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCWaves::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCWaves*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCWaves*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCWaves ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_uWaves, m_fAmplitude, m_bHorizontal, m_bVertical );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCWaves::update ( KDfloat fTime )
{
	for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
	{
		for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );

			if ( m_bVertical )
			{
				tVertex.x = ( tVertex.x + ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + tVertex.y * 0.01f ) * m_fAmplitude * m_fAmplitudeRate ) );
			}

			if ( m_bHorizontal )
			{
                tVertex.y = ( tVertex.y + ( kdSinf ( fTime * KD_PI_F * m_uWaves * 2 + tVertex.x * 0.01f ) * m_fAmplitude * m_fAmplitudeRate ) );
			}

			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}


//
// implementation of Twirl
//

CCTwirl* CCTwirl::create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDuint uTwirls, KDfloat fAmplitude )
{
	CCTwirl*  pRet = new CCTwirl ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize, tPosition, uTwirls, fAmplitude ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCTwirl::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDuint uTwirls, KDfloat fAmplitude )
{
	if ( CCGrid3DAction::initWithDuration ( fDuration, tGridSize ) )
	{
		setPosition ( tPosition );
		m_uTwirls = uTwirls;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCTwirl::setPosition ( const CCPoint& tPosition )
{
    m_tPosition = tPosition;
}

CCObject* CCTwirl::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCTwirl*    pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCTwirl *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCTwirl ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCGrid3DAction::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize, m_tPosition, m_uTwirls, m_fAmplitude );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCTwirl::update ( KDfloat fTime )
{	
	for ( KDint i = 0; i < (KDint) ( m_tGridSize.cx + 1 ); ++i )
	{
		for ( KDint j = 0; j < (KDint) ( m_tGridSize.cy + 1 ); ++j )
		{
			ccVertex3F  tVertex = originalVertex ( ccp ( i, j ) );
			
			CCPoint	 tAvg = ccp ( i - ( m_tGridSize.cx / 2.0f ), j - ( m_tGridSize.cy / 2.0f ) );
			KDfloat  fRadius = ccpLength ( tAvg );
			
			KDfloat  fAmp = 0.1f * m_fAmplitude * m_fAmplitudeRate;
			KDfloat  fA = fRadius * kdCosf ( KD_PI_F / 2.0f + fTime * KD_PI_F * m_uTwirls * 2 ) * fAmp;
			CCPoint  tD = ccp ( kdSinf ( fA ) * ( tVertex.y - m_tPosition.y ) + kdCosf ( fA ) * ( tVertex.x - m_tPosition.x ),
								kdCosf ( fA ) * ( tVertex.y - m_tPosition.y ) - kdSinf ( fA ) * ( tVertex.x - m_tPosition.x ) );

			tVertex.x = m_tPosition.x + tD.x;
			tVertex.y = m_tPosition.y + tD.y;

			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}

NS_CC_END