/* -----------------------------------------------------------------------------------
 *
 *      File            CCVertex.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      ForzeField Studios S.L.
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
#include "support/CCVertex.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

KDvoid  ccVertexLineToPolygon ( CCPoint* points, KDfloat stroke, ccVertex2F* vertices, KDuint offset, KDuint nuPoints )
{
    nuPoints += offset;
    if ( nuPoints <= 1 )
	{
		return;
	}

    stroke *= 0.5f;

    KDuint  idx;
    KDuint  nuPointsMinus = nuPoints - 1;

    for ( KDuint i = offset; i < nuPoints; i++ )
    {
        idx = i * 2;

        CCPoint p1 = points[i];
        CCPoint perpVector;

        if ( i == 0 )
		{
            perpVector = ccpPerp ( ccpNormalize ( ccpSub ( p1, points [ i + 1 ] ) ) );
		}
        else if ( i == nuPointsMinus )
		{
            perpVector = ccpPerp ( ccpNormalize ( ccpSub ( points [ i - 1 ], p1 ) ) );
		}
        else
        {
            CCPoint  p2 = points[i+1];
            CCPoint  p0 = points[i-1];

            CCPoint  p2p1 = ccpNormalize ( ccpSub ( p2, p1 ) );
            CCPoint  p0p1 = ccpNormalize ( ccpSub ( p0, p1 ) );

            // Calculate angle between vectors
            KDfloat  angle = kdAcosf ( ccpDot ( p2p1, p0p1 ) );

            if ( angle < CC_DEGREES_TO_RADIANS ( 70 ) )
			{
                perpVector = ccpPerp ( ccpNormalize ( ccpMidpoint ( p2p1, p0p1 ) ) );
			}
            else if ( angle < CC_DEGREES_TO_RADIANS ( 170 ) )
			{
                perpVector = ccpNormalize ( ccpMidpoint ( p2p1, p0p1 ) );
			}
            else
			{
                perpVector = ccpPerp ( ccpNormalize ( ccpSub ( p2, p0 ) ) );
			}
        }
        perpVector = ccpMult ( perpVector, stroke );

        vertices [ idx     ] = vertex2 ( p1.x + perpVector.x, p1.y + perpVector.y );
        vertices [ idx + 1 ] = vertex2 ( p1.x - perpVector.x, p1.y - perpVector.y );

    }

    // Validate vertexes
    offset = ( offset == 0 ) ? 0 : offset - 1;
    for ( KDuint i = offset; i<nuPointsMinus; i++ )
    {
        idx = i * 2;
        const KDuint idx1 = idx + 2;

        ccVertex2F p1 = vertices [ idx      ];
        ccVertex2F p2 = vertices [ idx + 1  ];
        ccVertex2F p3 = vertices [ idx1     ];
        ccVertex2F p4 = vertices [ idx1 + 1 ];

        KDfloat  s;
        //BOOL fixVertex = !ccpLineIntersect(ccp(p1.x, p1.y), ccp(p4.x, p4.y), ccp(p2.x, p2.y), ccp(p3.x, p3.y), &s, &t);
        KDbool fixVertex = !ccVertexLineIntersect ( p1.x, p1.y, p4.x, p4.y, p2.x, p2.y, p3.x, p3.y, &s );
        if ( !fixVertex )
		{
            if ( s < 0.0f || s > 1.0f )
			{
                fixVertex = KD_TRUE;
			}
		}

        if ( fixVertex )
        {
            vertices [ idx1     ] = p4;
            vertices [ idx1 + 1 ] = p3;
        }
    }
}

KDbool  ccVertexLineIntersect ( KDfloat Ax, KDfloat Ay,
                                KDfloat Bx, KDfloat By,
                                KDfloat Cx, KDfloat Cy,
                                KDfloat Dx, KDfloat Dy, KDfloat* T )
{
    KDfloat  distAB, theCos, theSin, newX;

    // FAIL: Line undefined
    if ( ( Ax == Bx && Ay == By ) || ( Cx == Dx && Cy == Dy ) )
	{
		return KD_FALSE;
	}

    //  Translate system to make A the origin
    Bx-=Ax; By-=Ay;
    Cx-=Ax; Cy-=Ay;
    Dx-=Ax; Dy-=Ay;

    // Length of segment AB
    distAB = kdSqrtf ( Bx * Bx + By * By );

    // Rotate the system so that point B is on the positive X axis.
    theCos = Bx / distAB;
    theSin = By / distAB;
    newX = Cx * theCos + Cy * theSin;
    Cy   = Cy * theCos - Cx * theSin; Cx = newX;
    newX = Dx * theCos + Dy * theSin;
    Dy   = Dy * theCos - Dx * theSin; Dx = newX;

    // FAIL: Lines are parallel.
    if ( Cy == Dy ) 
	{
		return KD_FALSE;
	}

    // Discover the relative position of the intersection in the line AB
    *T = ( Dx + ( Cx - Dx ) * Dy / ( Dy - Cy ) ) / distAB;

    // Success.
    return KD_TRUE;
}

NS_CC_END
