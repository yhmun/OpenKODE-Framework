/* -----------------------------------------------------------------------------------
 *
 *      File            PRRatcliffTriangulator.cpp
 *      Description     This class fills a polygon as described by an array of 
 *                      NSValue-encapsulated points with a texture.
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Damiano Mazzella ( Translated in C++ for Cocos2d-X on 19/03/2012 )
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

#include "Precompiled.h"
#include "extensions/CCPRKit/PRRatcliffTriangulator.h"

NS_CC_BEGIN


Vector2dVector PRRatcliffTriangulator::triangulateVertices ( Vector2dVector aVertices ) 
{
	Vector2dVector*		pInputPointsForTriangulation = new Vector2dVector;

	for ( KDuint uIndex = 0; uIndex < aVertices.size ( ); uIndex++ )
	{
		Vector2d		tValue = (Vector2d) aVertices.at ( uIndex );
		pInputPointsForTriangulation->push_back ( tValue );
	}

	// Triangulate results
	Vector2dVector		aTriangulatedPoints;

	Triangulate::Process ( *pInputPointsForTriangulation, aTriangulatedPoints );

	delete pInputPointsForTriangulation;

	return aTriangulatedPoints;
}


NS_CC_END