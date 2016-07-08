/* --------------------------------------------------------------------------
 *
 *      File            CCGrid.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
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

#ifndef __CCGrid_h__
#define __CCGrid_h__

#include "cocoa/CCObject.h"
#include "base_nodes/CCNode.h"
#include "support/CCCamera.h"
#include "textures/CCTexture2D.h"

namespace cocos2d 
{
    
class CCTexture2D;
class CCGrabber;

/** Base class for other */
class CCGridBase : public CCObject
{
	public :

		virtual ~CCGridBase ( KDvoid );

		static   CCGridBase*  gridWithSize ( const ccGridSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );
		static   CCGridBase*  gridWithSize ( const ccGridSize& tGridSize );

	public :

		KDbool  initWithSize ( const ccGridSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );
		KDbool  initWithSize ( const ccGridSize& tGridSize );

		/** wheter or not the grid is active */
		inline  KDbool  isActive ( KDvoid )	{ return m_bActive;	}
		        KDvoid  setActive ( KDbool bActive );

		/** number of times that the grid will be reused */
		inline  KDint   getReuseGrid ( KDvoid ) { return m_nReuseGrid; }
		inline  KDvoid  setReuseGrid ( KDint nReuseGrid ) { m_nReuseGrid = nReuseGrid; }

		/** size of the grid */
		inline  const ccGridSize&  getGridSize ( KDvoid ) { return m_tGridSize; }
		inline  KDvoid			   setGridSize ( const ccGridSize& tGridSize ) { m_tGridSize = tGridSize; }

		/** pixels between the grids */
		inline  const CCPoint&  getStep ( KDvoid ) { return m_tStep; }
		inline  KDvoid			setStep ( const CCPoint& tStep ) { m_tStep = tStep; }

		/** is texture flipped */
		inline  KDbool  isTextureFlipped ( KDvoid ) { return m_bIsTextureFlipped; }
		        KDvoid  setIsTextureFlipped ( KDbool bFlipped );

		KDvoid  beforeDraw ( KDvoid );
		KDvoid  afterDraw ( CCNode* pTarget );

		virtual KDvoid  blit ( KDvoid );
		virtual KDvoid  reuse ( KDvoid );
		virtual KDvoid  calculateVertexPoints ( KDvoid );

		KDvoid  set2DProjection ( KDvoid );
		
	protected :

		KDbool              m_bActive;
		KDint               m_nReuseGrid;
		ccGridSize          m_tGridSize;
		CCTexture2D*        m_pTexture;
		CCPoint             m_tStep;
		CCGrabber*          m_pGrabber;
		KDbool              m_bIsTextureFlipped;
		KDbool              m_bIsDepthTest;
};

/**
 *	CCGrid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
 */
class CCGrid3D : public CCGridBase
{
	public :

               ~CCGrid3D ( KDvoid );

		static  CCGrid3D*  gridWithSize ( const ccGridSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );
		static  CCGrid3D*  gridWithSize ( const ccGridSize& tGridSize );

	public :

		/** returns the vertex at a given position */
		ccVertex3F  vertex ( const ccGridSize& tGID );

		/** returns the original (non-transformed) vertex at a given position */
		ccVertex3F  originalVertex ( const ccGridSize& tGID );

		/** sets a new vertex at a given position */
		KDvoid   setVertex ( const ccGridSize& tGID, const ccVertex3F& tVertex );

		virtual KDvoid  blit ( KDvoid );
		virtual KDvoid  reuse ( KDvoid );
		virtual KDvoid  calculateVertexPoints ( KDvoid );

	protected :

		GLvoid*         m_pTexCoordinates;
		GLvoid*         m_pVertices;
		GLvoid*         m_pOriginalVertices;
		GLushort*       m_pIndices;
};

/**
 *	CCTiledGrid3D is a 3D grid implementation. It differs from Grid3D in that
 *	the tiles can be separated from the grid.
 */
class CCTiledGrid3D : public CCGridBase
{
	public :

		~CCTiledGrid3D ( KDvoid );

		static  CCTiledGrid3D*  gridWithSize ( const ccGridSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );
		static  CCTiledGrid3D*  gridWithSize ( const ccGridSize& tGridSize );

	public :

		/** returns the tile at the given position */
		ccQuad3  tile ( const ccGridSize& tGID );

		/** returns the original tile (untransformed) at the given position */
		ccQuad3  originalTile ( const ccGridSize& tGID );

		/** sets a new tile */
		KDvoid   setTile ( const ccGridSize& tGID, const ccQuad3& tCoords );

		virtual KDvoid  blit ( KDvoid );
		virtual KDvoid  reuse ( KDvoid );
		virtual KDvoid  calculateVertexPoints ( KDvoid );

	protected :

		GLvoid*         m_pTexCoordinates;
		GLvoid*         m_pVertices;
		GLvoid*         m_pOriginalVertices;
		GLushort*       m_pIndices;
};

}
#endif 
