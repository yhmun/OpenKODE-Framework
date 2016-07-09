/* --------------------------------------------------------------------------
 *
 *      File            TexturedPolygon.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __TexturedPolygon_h__
#define __TexturedPolygon_h__

// TexturedPolygon Class
typedef struct _ccV2F_T2F
{
	ccVertex2F		vertices;
	ccTex2F			texCoords;
} ccV2F_T2F;

class Vector3D;

class TexturedPolygon : public CCSprite
{
	public :
				 TexturedPolygon ( KDvoid );
		virtual ~TexturedPolygon ( KDvoid );

		static TexturedPolygon*		create ( const KDchar* szFilePath, CCArray* pVerts );
		static TexturedPolygon*		create ( const KDchar* szFilePath, CCArray* pVerts, CCArray* pTris );

	public :

		virtual KDbool		initWithFile ( const KDchar* szFilePath, CCArray* pVerts );
		virtual KDbool		initWithFile ( const KDchar* szFilePath, CCArray* pVerts, CCArray* pTris );

		virtual KDvoid		draw ( KDvoid );

		virtual Vector3D*	getTriangleIndicesFromPoint1 ( const CCPoint& tP1, const CCPoint& tP2, const CCPoint& tP3 );
		
		virtual KDvoid		addAnimFrameWithFile ( const KDchar* szFilePath, CCArray* pArr );

		virtual KDvoid		setCoordInfo ( KDvoid );

	private :

		CCArray*			m_pVertices;
		CCArray*			m_pTriangles;
		ccV2F_T2F*			m_pCoords;
};

#endif	// __TexturedPolygon_h__
