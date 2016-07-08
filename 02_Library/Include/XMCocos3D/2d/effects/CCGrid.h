/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrid.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCGrid_h__
#define __CCGrid_h__

#include "../../ccTypes.h"
#include "../../base/CCObject.h"
#include "../base_nodes/CCNode.h"
#include "../CCCamera.h"
#include "../textures/CCTexture2D.h"
#include "../CCDirector.h"
#include "XMKazmath/mat4.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class Texture2D;
class Grabber;
class GLProgram;

/**
 *	@addtogroup effects
 *	@{
 */

/** 
 *	Base class for other
 */
class CC_DLL GridBase : public Object
{
	public :

		/** create one Grid */
		static GridBase*		create(const Size& tGridSize, Texture2D *texture, bool flipped);
		
		/** create one Grid */
		static GridBase*		create ( const Size& tGridSize );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~GridBase ( KDvoid );

		KDbool					initWithSize ( const Size& tGridSize, Texture2D* pTexture, KDbool bFlipped );
		KDbool					initWithSize ( const Size& tGridSize );

		/** whether or not the grid is active */
		inline KDbool			 isActive ( KDvoid ) const { return m_bActive; }
		KDvoid					setActive ( KDbool bActive );

		/** number of times that the grid will be reused */
		inline KDint			getReuseGrid ( KDvoid ) const { return m_nReuseGrid; }
		inline KDvoid			setReuseGrid ( KDint nReuseGrid) { m_nReuseGrid = nReuseGrid; }

		/** size of the grid */
		inline const Size&		getGridSize ( KDvoid ) const { return m_tGridSize; }
		inline KDvoid			setGridSize ( const Size& tGridSize ) { m_tGridSize = tGridSize; }

		/** pixels between the grids */
		inline const Point&		getStep ( KDvoid ) const { return m_tStep; }
		inline KDvoid			setStep ( const Point& tStep ) { m_tStep = tStep; }

		/** is texture flipped */
		inline KDbool			 isTextureFlipped ( KDvoid ) const { return m_bIsTextureFlipped; }
		KDvoid					setTextureFlipped ( KDbool bFlipped );

		KDvoid					beforeDraw ( KDvoid );
		KDvoid					afterDraw ( Node* pTarget );

		virtual KDvoid			blit ( KDvoid );
		virtual KDvoid			reuse ( KDvoid );
		virtual KDvoid			calculateVertexPoints ( KDvoid );

		KDvoid					set2DProjection ( KDvoid );

	protected :

		KDbool					m_bActive;
		KDint32					m_nReuseGrid;
		Size					m_tGridSize;
		Texture2D*				m_pTexture;
		Point					m_tStep;
		Grabber*				m_pGrabber;
		KDbool					m_bIsTextureFlipped;
		GLProgram*				m_pShaderProgram;
		Director::Projection	m_eDirectorProjection;
};

/**
 *	Grid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
 */
class CC_DLL Grid3D : public GridBase
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/** create one Grid */
		static Grid3D*			create ( const Size& tGridSize, Texture2D* pTexture, KDbool bFlipped );

		/** create one Grid */
		static Grid3D*			create ( const Size& tGridSize );

		/**
		 *	@js ctor
		 */
		Grid3D ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~Grid3D ( KDvoid );

		/** 
		 *	returns the vertex at a given position 
		 *	@js NA
		 *	@lua NA
		 */
		Vertex3F				getVertex ( const Point& tPos ) const;

		/**
		 *	returns the original (non-transformed) vertex at a given position
		 *	@js NA
		 *	@lua NA
		 */
		Vertex3F				getOriginalVertex ( const Point& tPos ) const;

		/**
		 *	sets a new vertex at a given position 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					setVertex ( const Point& tPos, const Vertex3F& tVertex );

		// Overrides
		virtual KDvoid			blit ( KDvoid ) override;
		virtual KDvoid			reuse ( KDvoid ) override;
		virtual KDvoid			calculateVertexPoints ( KDvoid ) override;

	protected :

		GLvoid*					m_pTexCoordinates;
		GLvoid*					m_pVertices;
		GLvoid*					m_pOriginalVertices;
		GLushort*				m_pIndices;
};

/**
 *	TiledGrid3D is a 3D grid implementation. It differs from Grid3D in that
 *	the tiles can be separated from the grid.
 */
class CC_DLL TiledGrid3D : public GridBase
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/** create one Grid */
		static TiledGrid3D*		create ( const Size& tGridSize, Texture2D* pTexture, KDbool bFlipped );

		/** create one Grid */
		static TiledGrid3D*		create ( const Size& tGridSize );
		
		/**
		 *	@js ctor
		 */
		TiledGrid3D ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~TiledGrid3D ( KDvoid );

		/** 
		 *	returns the tile at the given position 
		 *	@js NA
		 *	@lua NA
		 */
		Quad3					getTile ( const Point& tPos ) const;

		/**
		 *	returns the original tile (untransformed) at the given position 
		 *	@js NA
		 *	@lua NA
		 */
		Quad3					getOriginalTile ( const Point& tPos ) const;

		/** 
		 *	sets a new tile 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					setTile ( const Point& tPos, const Quad3& tCoords );

		// Overrides
		virtual KDvoid			blit ( KDvoid ) override;
		virtual KDvoid			reuse ( KDvoid ) override;
		virtual KDvoid			calculateVertexPoints ( KDvoid ) override;

	protected :

		GLvoid*					m_pTexCoordinates;
		GLvoid*					m_pVertices;
		GLvoid*					m_pOriginalVertices;
		GLushort*				m_pIndices;
};

// end of effects group
/// @}

NS_CC_END

#endif	// __CCGrid_h__
