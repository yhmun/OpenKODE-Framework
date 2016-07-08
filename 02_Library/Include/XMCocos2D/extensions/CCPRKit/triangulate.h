/* -----------------------------------------------------------------------------------
 *
 *      File            triangulate.h
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

#ifndef __triangulate_h__
#define __triangulate_h__

/*****************************************************************/
/** Static class to triangulate any contour/polygon efficiently **/
/** You should replace Vector2d with whatever your own Vector   **/
/** class might be.  Does not support polygons with holes.      **/
/** Uses STL vectors to represent a dynamic array of vertices.  **/
/** This code snippet was submitted to FlipCode.com by          **/
/** John W. Ratcliff (jratcliff@verant.com) on July 22, 2000    **/
/** I did not write the original code/algorithm for this        **/
/** this triangulator, in fact, I can't even remember where I   **/
/** found it in the first place.  However, I did rework it into **/
/** the following black-box static class so you can make easy   **/
/** use of it in your own code.  Simply replace Vector2d with   **/
/** whatever your own Vector implementation might be.           **/
/*****************************************************************/

#include "../../ccMacros.h"
#include <vector> 

NS_CC_BEGIN


class Vector2d
{
	public :

		Vector2d ( KDfloat x, KDfloat y );
		
	public :

		KDfloat		GetX ( KDvoid ) const;
		
		KDfloat		GetY ( KDvoid ) const;
		
		KDvoid		Set ( KDfloat x, KDfloat y );

	private :

		KDfloat		m_fX;
		KDfloat		m_fY;
};

// Typedef an STL vector of vertices which are used to represent
// a polygon/contour and a series of triangles.
typedef std::vector<Vector2d>	Vector2dVector;

class Triangulate
{
	public:
		
		// triangulate a contour/polygon, places results in STL vector
		// as series of triangles.
		static KDbool		Process ( const Vector2dVector& aContour, Vector2dVector& aResult );
		
		// compute area of a contour/polygon
		static KDfloat		Area ( const Vector2dVector& aContour );
		
		// decide if point Px/Py is inside triangle defined by
		// (Ax,Ay) (Bx,By) (Cx,Cy)
		static KDbool		InsideTriangle ( KDfloat Ax, KDfloat Ay, KDfloat Bx, KDfloat By, KDfloat Cx, KDfloat Cy, KDfloat Px, KDfloat Py );
		
		
	private :

		static KDbool		Snip ( const Vector2dVector& aContour, KDint u, KDint v, KDint w, KDint n, KDint* V );
	
};

NS_CC_END

#endif	// __triangulate_h__