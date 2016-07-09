/* -----------------------------------------------------------------------------------
 *
 *      File            RaycastCallback.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved. 
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "RaycastCallback.h"

#define collinear( x1, y1, x2, y2, x3, y3 )		kdFabsf ( ( y1 - y2 ) * ( x1 - x3 ) - ( y1 - y3 ) * ( x1 - x2 ) )

KDfloat RaycastCallback::ReportFixture ( b2Fixture* pFixture, const b2Vec2& tPoint,const b2Vec2& tNormal, KDfloat fFraction )
{
    PolygonSpriteEx*	pPS = (PolygonSpriteEx*) pFixture->GetBody ( )->GetUserData ( );
		
    if ( !pPS->isSliceEntered ( ) )
    {
        pPS->setSliceEntered ( KD_TRUE );
        
        // we need to get the point coordinates within the shape
        pPS->setEntryPoint ( pPS->getBody ( )->GetLocalPoint ( tPoint ) );
        
        // we also need to store the slice entry time so that there's a time limit for each slice to complete
        pPS->setSliceEntryTime ( kdGetMilliseconds ( ) / 1000 + 1 );
    }

    else if ( !pPS->isSliceExited ( ) )
    {			
        pPS->setExitPoint ( pPS->getBody ( )->GetLocalPoint ( tPoint ) );

        b2Vec2		tEntrySide = pPS->getEntryPoint ( ) - pPS->getCentroid ( );
        b2Vec2		tExitSide  = pPS->getExitPoint  ( ) - pPS->getCentroid ( );
        
		
        if ( tEntrySide.x * tExitSide.x < 0 || tEntrySide.y * tExitSide.y < 0 )
        {
            pPS->setSliceExited ( KD_TRUE );
        }
        else 
		{
            // if the cut didn't cross the centroid, we check if the entry and exit point lie on the same line
            b2Fixture*			pFixture = pPS->getBody ( )->GetFixtureList ( );
            b2PolygonShape*		pPolygon = (b2PolygonShape*) pFixture->GetShape ( );
            KDint				nCount   = pPolygon->GetVertexCount ( );            
            KDbool				bOnSameLine = KD_FALSE;
	
            for ( KDint i = 0 ; i < nCount; i++ )
            {			
                b2Vec2		tPointA = pPolygon->GetVertex ( i );
                b2Vec2		tPointB;
                
                if ( i == nCount - 1 )
                {
                    tPointB = pPolygon->GetVertex ( 0 );
                }
                else
				{
                    tPointB = pPolygon->GetVertex ( i + 1 );
                }
                
                KDfloat		fCollinear = collinear ( tPointA.x, tPointA.y, pPS->getEntryPoint ( ).x, pPS->getEntryPoint ( ).y, tPointB.x, tPointB.y );
                
                if ( fCollinear <= 0.00001f )
                {
                    KDfloat		fCollinear2 = collinear ( tPointA.x, tPointA.y, pPS->getExitPoint ( ).x, pPS->getExitPoint ( ).y, tPointB.x, tPointB.y );
                    if ( fCollinear2 <= 0.00001f )
                    {
                        bOnSameLine = KD_TRUE;
                    }
                    break;
                }			
            }
            
            if ( bOnSameLine )
            {
                pPS->setEntryPoint ( pPS->getExitPoint ( ) );
                pPS->setSliceEntryTime ( kdGetMilliseconds ( ) / 1000 + 1 );
                pPS->setSliceExited ( KD_FALSE );
            }
            else 
			{
                pPS->setSliceExited ( KD_TRUE );
            }
        }		
    }
	
    return 1;
}
