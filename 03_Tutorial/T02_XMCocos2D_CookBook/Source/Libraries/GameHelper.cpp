/* --------------------------------------------------------------------------
 *
 *      File            GameHelper.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "GameHelper.h"

enum
{
	// how many characters are supported
	kCCBMFontMaxChars = 2048, //256,
};

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns true for clockwise turn or the points are collinear, false another.
KDbool GameHelper::clockwiseO ( Vector3D* O, Vector3D* A, Vector3D* B )
{
	return ( A->x - O->x ) * ( B->y - O->y ) - ( A->y - O->y ) * ( B->x - O->x ) <= 0;
}

// Given a set of points it returns a polygon surrounding those points (a convex hull)
// From here: http://www.algorithmist.com/index.php/Monotone_Chain_Convex_Hull.cpp

struct Point_
{
	KDfloat		x;
	KDfloat		y;

	bool operator < (const Point_& p ) const
	{
		return ( x < p.x || ( x == p.x && y < p.y ) );
	}
};

static long long cross ( Point_ A, Point_ B, Point_ C )
{ 
    return (long long) ( ( B.x - A.x ) * ( C.y - A.y ) - ( B.y - A.y ) * ( C.x - A.x ) );
}

static std::vector<Point_> ConvexHull ( std::vector<Point_>& P ) //Andrew's monotone chain
{
	std::vector<Point_>		H;		// hull
	H.resize ( 2 * P.size ( ) );

	KDint				k = 0;

	if ( P.size ( ) < 3 ) 
	{
		return H;
	}

	std::sort ( P.begin ( ), P.end ( ) );

	// lower
	for ( KDint i = 0; i < (KDint) P.size ( );i++ )
	{
		while ( k >= 2 && cross ( H [ k - 2 ], H [ k - 1 ], P [ i ] ) <= 0 )
		{
			k--;
		}
		H [ k ] = P [ i ];
		k++;        
	}

	KDint				t = k + 1; 

	// upper
	for ( KDint i = P.size ( ) - 2; i >= 0 ; i-- )
	{
		while ( k >= t && cross ( H [ k - 2 ], H [ k - 1 ], P [ i ] ) <= 0 )
		{
			k--;
		}

		H [ k ] = P [ i ];
		k++;
	}

	H.resize ( k ); 
	return H;
};

CCArray* GameHelper::convexHull ( CCArray* pArray )
{
	std::vector<Point_>		aHull;
	CCObject*				pObject;

	CCARRAY_FOREACH ( pArray, pObject )
	{
		Vector3D*		pVector = (Vector3D*) pObject;
		Point_			tPoint  = { pVector->x, pVector->y };

		aHull.push_back ( tPoint );
	}

	aHull = ConvexHull ( aHull );

	CCArray*	pRet = CCArray::create ( );

	std::vector<Point_>::iterator	iter;
	for ( iter = aHull.begin ( ); iter != aHull.end ( ); iter++ )
	{
		Point_			tPoint = *iter;
		pRet->addObject ( Vector3D::create ( tPoint.x, tPoint.y, 0 ) );
	}

	pRet->removeLastObject ( );
	
	return pRet;


	/*
	KDint		n = P->count ( );
	KDint		k = 0;
	CCArray*	H = CCArray::createWithCapacity ( n * 2 );
	
	//Sort points lexicographically (by X, then secondarily by Y)
	NSSortDescriptor *xDescriptor = [[NSSortDescriptor alloc] initWithKey:@"self.x" ascending:YES];
	NSSortDescriptor *yDescriptor = [[NSSortDescriptor alloc] initWithKey:@"self.y" ascending:YES];
	
	CCArray *descriptors = [CCArray arrayWithObjects:xDescriptor, yDescriptor, nil];
	CCArray *sortedP = [P sortedArrayUsingDescriptors:descriptors];
	
	//Build lower hull
	for (int i = 0; i < n; i ++) {
		while (k >= 2 && [self clockwiseO:[H objectAtIndex:k-2] A:[H objectAtIndex:k-1] B:[sortedP objectAtIndex:i]]){
			k--;
		}
		[H insertObject:[sortedP objectAtIndex:i] atIndex:k++];
	};
 	
	//Build upper hull
	for (int i = n-2, t = k + 1; i >= 0; i --) {		
		while (k >= t && [self clockwiseO:[H objectAtIndex:k-2] A:[H objectAtIndex:k-1] B:[sortedP objectAtIndex:i]]){
			k--;
		}
		[H insertObject:[sortedP objectAtIndex:i] atIndex:k++];
	};	
	
	[H removeObjectsInRange:NSMakeRange(k, H.count-k)];

	//Remove all duplicate objects
	NSMutableArray *noDupeArray = [[NSMutableArray alloc] init];
	for(int i = 0; i<H.count; i++){
		if(![noDupeArray containsObject:[H objectAtIndex:i]]){
			[noDupeArray addObject:[H objectAtIndex:i]];
		}
	}

	return noDupeArray;
	*/

	return KD_NULL;
}

KDfloat GameHelper::distanceP1 ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	return kdSqrtf ( kdPowf ( ( tPoint1.x - tPoint2.x ), 2 ) + kdPowf ( ( tPoint1.y - tPoint2.y ), 2 ) );
}

KDfloat GameHelper::degreesToRadians ( KDfloat fDegrees )
{
	return fDegrees * ( PI_CONSTANT / 180 );
}

KDfloat GameHelper::radiansToDegrees ( KDfloat fRadians )
{
	return fRadians * ( 180 / PI_CONSTANT );
}

KDfloat GameHelper::vectorToRadians ( CCPoint tVector )
{
	if ( tVector.y == 0 )
	{
		tVector.y = 0.000001f; 
	}

	KDfloat		fBaseRadians = kdAtanf ( tVector.x / tVector.y );
	if ( tVector.y < 0 )
	{
		fBaseRadians += PI_CONSTANT; 	// Adjust for -Y
	}
	return fBaseRadians;
}

CCPoint GameHelper::radiansToVector ( KDfloat fRadians )
{
	return ccp ( kdSinf ( fRadians ), kdCosf ( fRadians ) );
}

Vector3D* GameHelper::quadraticA ( KDfloat A, KDfloat B, KDfloat C )
{
	Vector3D*	pSolution = KD_NULL;
	
	if ( GameHelper::absoluteValue ( A ) < 1e-6 )
	{
		if ( GameHelper::absoluteValue ( B ) < 1e-6 )
		{
			pSolution = GameHelper::absoluteValue ( C ) < 1e-6 ? Vector3D::create ( 0, 0, 0 ) : KD_NULL;
		}
		else 
		{
			pSolution = Vector3D::create ( -C / B, -C / B, 0 );
		}
	} 
	else 
	{
		KDfloat		fDisc = B * B - 4 * A * C;
		if ( fDisc >= 0 ) 
		{
			fDisc = kdSqrtf ( fDisc );
			A = 2 * A;
			pSolution = Vector3D::create ( ( -B - fDisc ) / A, ( -B + fDisc ) / A, 0 );
		}
	}

	return pSolution;
}

KDfloat GameHelper::absoluteValue ( KDfloat fAbs )
{
	if ( fAbs < 0 )
	{
		fAbs *= -1; 
	}

	return fAbs;
}

CCPoint GameHelper::midPointP1 ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	return ccp ( ( tPoint1.x + tPoint2.x ) / 2 , ( tPoint1.y + tPoint2.y ) / 2 );
}

KDbool GameHelper::point ( const CCPoint& tPoint, const CCRect& tRect )
{
	KDbool		bIsInRect = KD_FALSE;

	if ( tPoint.x < tRect.origin.x + tRect.size.cx && 
	     tPoint.x > tRect.origin.x &&
	     tPoint.y < tRect.origin.y + tRect.size.cy &&
	     tPoint.y > tRect.origin.y )
	{
		bIsInRect = KD_TRUE;
	}

	return bIsInRect;

}

KDbool GameHelper::point ( const CCPoint& tPoint, const CCPoint& tOrigin, KDfloat fRadius )
{
	KDbool		bIsInCircle = KD_FALSE;
	if ( GameHelper::distanceP1 ( tPoint, tOrigin ) <= fRadius )
	{
		bIsInCircle = KD_TRUE;
	}
	return bIsInCircle;
}

b2Vec2 GameHelper::b2ClampA ( const b2Vec2& A, const b2Vec2& tLow, const b2Vec2& tHigh )
{
	return b2Max ( tLow, b2Min ( A, tHigh ) );
}

KDchar* GameHelper::sanitizeString ( KDchar* szStr )
{	
	KDsize	uLen = kdStrlen ( szStr );

	for ( KDuint i = 0; i < uLen; i++ ) 
	{
		KDchar&		c = szStr [ i ];
		
		if ( c >= kCCBMFontMaxChars )
		{ 
			c = '\'';		
		}
	}

	return szStr;
}