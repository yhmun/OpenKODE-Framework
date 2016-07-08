/* -----------------------------------------------------------------------------------
 *
 *      File            PRFilledPolygon.h
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

#ifndef __PRFilledPolygon_h__
#define __PRFilledPolygon_h__

#include "PRRatcliffTriangulator.h"
#include "../../base_nodes/CCNode.h"

NS_CC_BEGIN


class PRFilledPolygon : public CCNode 
{
	public :    
				 PRFilledPolygon ( KDvoid );
		virtual ~PRFilledPolygon ( KDvoid );
	    
		/**
		 *	Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
		 */
		static PRFilledPolygon*		create ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture );
	    
		/**
		 *	Returns an autoreleased filled poly with a supplied triangulator.
		 */
		static PRFilledPolygon*		create ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture, PRRatcliffTriangulator* pPolygonTriangulator );
	    
	public :

		/**
		 *	Initialize the polygon.  polygonPoints will be triangulated.  Default triangulator is used (Ratcliff).
		 */
		virtual KDbool				initWithPoints ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture );
	    
		/**
		 *	Initialize the polygon.  polygonPoints will be triangulated using the supplied triangulator.
		 */
		virtual KDbool				initWithPoints ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture, PRRatcliffTriangulator* pPolygonTriangulator );
	    
		KDvoid						setPoints ( Vector2dVector& aPoints );

		KDvoid						setTexture ( CCTexture2D* pTexture );
		CCTexture2D*				getTexture ( KDvoid );

		KDvoid						updateBlendFunc ( KDvoid );

		KDvoid						setBlendFunc ( ccBlendFunc tBlendFunc );

		virtual KDvoid				draw ( KDvoid );
  
	private :

		KDvoid						calculateTextureCoordinates ( KDvoid );

	protected :

		KDint						m_nAreaTrianglePointCount;

		CCTexture2D*				m_pTexture;
		ccBlendFunc					m_tBlendFunc;

		ccVertex2F*					m_pAreaTrianglePoints;
		ccVertex2F*					m_pTextureCoordinates;

		PRRatcliffTriangulator*		m_pTriangulator;
};


NS_CC_END

#endif	// __PRFilledPolygon_h__

