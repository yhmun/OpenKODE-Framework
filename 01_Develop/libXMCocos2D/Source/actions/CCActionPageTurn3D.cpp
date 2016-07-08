/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionPageTurn3D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Sindesso Pty Ltd 
 *
 *         http://www.cocos2d-x.org      
 *         http://www.sindesso.com    
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
#include "actions/CCActionPageTurn3D.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
CCPageTurn3D* CCPageTurn3D::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCPageTurn3D*  pRet = new CCPageTurn3D ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

/*
 * Update each tick
 * Time is the percentage of the way through the duration
 */
KDvoid CCPageTurn3D::update ( KDfloat fTime )
{
	KDfloat  fTime2 = KD_MAX ( 0, fTime - 0.25f );
	KDfloat  fDeltaAy = ( fTime2 * fTime2 * 500 );
	KDfloat  fAy = -100 - fDeltaAy;
	
	KDfloat  fDeltaTheta = -KD_PI_2_F * kdSqrtf ( fTime );
	KDfloat  fTheta = /*0.01f */ +KD_PI_2_F + fDeltaTheta;
	
	KDfloat  fSinTheta = kdSinf ( fTheta );
	KDfloat  fCosTheta = kdCosf ( fTheta );
	
	for ( KDint i = 0; i <= (KDint) m_tGridSize.cx; ++i )
	{
		for ( KDint j = 0; j <= (KDint) m_tGridSize.cy; ++j )
		{
			// Get original vertex
			ccVertex3F  tVertex = originalVertex ( ccp ( i , j ) );
			
			KDfloat  fR = kdSqrtf ( ( tVertex.x * tVertex.x ) + ( ( tVertex.y - fAy ) * ( tVertex.y - fAy ) ) );
			KDfloat  fR2 = fR * fSinTheta;
			KDfloat  fAlpha = kdAsinf ( tVertex.x / fR );
			KDfloat  fBeta = fAlpha / fSinTheta;
			KDfloat  fCosBeta = kdCosf ( fBeta );
			
			// If beta > PI then we've wrapped around the cone
			// Reduce the radius to stop these points interfering with others
			if ( fBeta <= KD_PI_F )
			{
				tVertex.x = ( fR2 * kdSinf ( fBeta ) );
			}
			else
			{
				// Force X = 0 to stop wrapped
				// points
				tVertex.x = 0;
			}

            tVertex.y = ( fR + fAy - ( fR2 * ( 1 - fCosBeta ) * fSinTheta ) );

            // We scale z here to avoid the animation being
            // too much bigger than the screen due to perspective transform
            tVertex.z = ( fR2 * ( 1 - fCosBeta ) * fCosTheta ) / 7;// "100" didn't work for

			//	Stop z coord from dropping beneath underlying page in a transition
			// issue #751
			if ( tVertex.z < 0.5f )
			{
				tVertex.z = 0.5f;
			}
			
			// Set new coords
			setVertex ( ccp ( i, j ), tVertex );
		}
	}
}

    
NS_CC_END