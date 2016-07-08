/* -----------------------------------------------------------------------------------
 *
 *      File            triangulate.cpp
 *      Description     This class fills a polygon as described by an array of 
 *                      NSValue-encapsulated points with a texture.
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Precognitive Research, LLC. All rights reserved. ( Created by Andy Sinesio on 6/25/10 )
 *
 *         http://precognitiveresearch.com      
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

// COTD Entry submitted by John W. Ratcliff [jratcliff@verant.com]

// ** THIS IS A CODE SNIPPET WHICH WILL EFFICIEINTLY TRIANGULATE ANY
// ** POLYGON/CONTOUR (without holes) AS A STATIC CLASS.  THIS SNIPPET
// ** IS COMPRISED OF 3 FILES, TRIANGULATE.H, THE HEADER FILE FOR THE
// ** TRIANGULATE BASE CLASS, TRIANGULATE.CPP, THE IMPLEMENTATION OF
// ** THE TRIANGULATE BASE CLASS, AND TEST.CPP, A SMALL TEST PROGRAM
// ** DEMONSTRATING THE USAGE OF THE TRIANGULATOR.  THE TRIANGULATE
// ** BASE CLASS ALSO PROVIDES TWO USEFUL HELPER METHODS, ONE WHICH
// ** COMPUTES THE AREA OF A POLYGON, AND ANOTHER WHICH DOES AN EFFICENT
// ** POINT IN A TRIANGLE TEST.
// ** SUBMITTED BY JOHN W. RATCLIFF (jratcliff@verant.com) July 22, 2000

#include "Precompiled.h"
#include "extensions/CCPRKit/triangulate.h"

NS_CC_BEGIN

//
// Vector2d
//

Vector2d::Vector2d ( KDfloat x, KDfloat y )
{
	Set ( x, y );
}

KDfloat Vector2d::GetX ( KDvoid ) const
{
	return m_fX; 
}

KDfloat Vector2d::GetY ( KDvoid ) const
{
	return m_fY; 
}

KDvoid Vector2d::Set ( KDfloat x, KDfloat y )
{
	m_fX = x;
	m_fY = y;
}



//
// Triangulate
//

static const KDfloat	EPSILON = 0.0000000001f;

KDfloat Triangulate::Area ( const Vector2dVector& aContour )
{	
	KDint		n = aContour.size ( );	
	KDfloat		A = 0.0f;
	
	for ( KDint p = n - 1, q = 0; q < n; p = q++ )
	{
		A += aContour [ p ].GetX ( ) * aContour [ q ].GetY ( ) - aContour [ q ].GetX ( ) * aContour [ p ].GetY ( );
	}

	return A * 0.5f;
}



//
// InsideTriangle decides if a point P is Inside of the triangle
// defined by A, B, C.
//
KDbool Triangulate::InsideTriangle ( KDfloat Ax, KDfloat Ay, KDfloat Bx, KDfloat By, KDfloat Cx, KDfloat Cy, KDfloat Px, KDfloat Py )

{
	KDfloat		ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	KDfloat		cCROSSap, bCROSScp, aCROSSbp;
	
	ax  = Cx - Bx;  ay  = Cy - By;
	bx  = Ax - Cx;  by  = Ay - Cy;
	cx  = Bx - Ax;  cy  = By - Ay;
	apx = Px - Ax;  apy = Py - Ay;
	bpx = Px - Bx;  bpy = Py - By;
	cpx = Px - Cx;  cpy = Py - Cy;
	
	aCROSSbp = ax * bpy - ay * bpx;
	cCROSSap = cx * apy - cy * apx;
	bCROSScp = bx * cpy - by * cpx;
	
	return ( ( aCROSSbp >= 0.0f ) && ( bCROSScp >= 0.0f ) && ( cCROSSap >= 0.0f ) );
};


KDbool Triangulate::Snip ( const Vector2dVector& aContour, KDint u, KDint v, KDint w, KDint n, KDint* V )
{
	KDint		p;
	KDfloat		Ax, Ay, Bx, By, Cx, Cy, Px, Py;
	
	Ax = aContour [ V [ u ] ].GetX ( );
	Ay = aContour [ V [ u ] ].GetY ( );
	
	Bx = aContour [ V [ v ] ].GetX ( );
	By = aContour [ V [ v ] ].GetY ( );
	
	Cx = aContour [ V [ w ] ].GetX ( );
	Cy = aContour [ V [ w ] ].GetY ( );
	
	if ( EPSILON > ( ( ( Bx - Ax ) * ( Cy - Ay ) ) - ( ( By - Ay ) * ( Cx - Ax ) ) ) ) 
	{
		return KD_FALSE;
	}
	
	for ( p = 0; p < n; p++ )
	{
		if ( ( p == u ) || ( p == v ) || ( p == w ) )
		{
			continue;
		}

		Px = aContour [ V [ p ] ].GetX ( );
		Py = aContour [ V [ p ] ].GetY ( );

		if ( InsideTriangle ( Ax, Ay, Bx, By, Cx, Cy, Px, Py ) )
		{
			return KD_FALSE;
		}
	}
	
	return KD_TRUE;
}

KDbool Triangulate::Process ( const Vector2dVector& aContour, Vector2dVector& aResult )
{
	// allocate and initialize list of Vertices in polygon 	
	KDint	n = aContour.size ( );
	
	if ( n < 3 ) 
	{
		return KD_FALSE;
	}
	
	KDint*	V = new KDint [ n ];
	
	// we want a counter-clockwise polygon in V 	
	if ( 0.0f < Area ( aContour ) )
	{
		for ( KDint v = 0; v < n; v++ )
		{
			V [ v ] = v;
		}
	}
	else
	{
		for ( KDint v = 0; v < n; v++ )
		{
			V [ v ] = ( n - 1 ) - v;
		}
	}
	
	KDint	nv = n;
	
	//  remove nv-2 Vertices, creating 1 triangle every time 
	KDint	count = 2 * nv;		// error detection 
	
	for ( KDint m = 0, v = nv - 1; nv > 2; )
	{
		// if we loop, it is probably a non-simple polygon 
		if ( 0 >= ( count-- ) )
		{
			// Triangulate: ERROR - probable bad polygon!
			return KD_FALSE;
		}
		
		// three consecutive vertices in current polygon, <u,v,w> 
		KDint	u = v; 
		if ( nv <= u )
		{
			u = 0;     // previous 
		}

		v = u + 1;
		if ( nv <= v )
		{
			v = 0;     // new v    
		}

		KDint	w = v + 1;
		if ( nv <= w )
		{
			w = 0;     // next     
		}
		
		if ( Snip ( aContour, u, v, w, nv, V ) )
		{
			KDint	a, b, c, s, t;
			
			// true names of the vertices 
			a = V [ u ]; b = V [ v ]; c = V [ w ];
			
			// output Triangle 
			aResult.push_back ( aContour [ a ] );
			aResult.push_back ( aContour [ b ] );
			aResult.push_back ( aContour [ c ] );
			
			m++;
			
			// remove v from remaining polygon 
			for ( s = v, t = v + 1; t < nv; s++, t++ ) V[s] = V[t]; nv--;
			
			// resest error detection counter 
			count = 2 * nv;
		}
	}
			
	delete V;
	
	return KD_TRUE;
}


NS_CC_END