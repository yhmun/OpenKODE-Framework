/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrid.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCGrid_h__
#define __CCGrid_h__

#include "../base_nodes/CCNode.h"
#include "../support/CCCamera.h"
#include "../support/CCDirector.h"
#include "../textures/CCTexture2D.h"

#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN
   
class CCTexture2D;
class CCGrabber;
class CCGLProgram;

/**
 * @addtogroup effects
 * @{
 */

/** Base class for other */
class CCGridBase : public CCObject
{
	public :
				 CCGridBase ( KDvoid );
		virtual ~CCGridBase ( KDvoid );

		/** create one Grid */
		static  CCGridBase*		create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );

		/** create one Grid */
		static  CCGridBase*		create ( const CCSize& tGridSize );

	public :

		virtual KDbool			initWithSize ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );
		virtual KDbool			initWithSize ( const CCSize& tGridSize );

		/** whether or not the grid is active */
		inline  KDbool			 isActive ( KDvoid ) { return m_bActive; }
		        KDvoid			setActive ( KDbool bActive );

		/** number of times that the grid will be reused */
		inline  KDint			getReuseGrid ( KDvoid ) { return m_nReuseGrid; }
		inline  KDvoid			setReuseGrid ( KDint nReuseGrid ) { m_nReuseGrid = nReuseGrid; }

		/** size of the grid */
		inline  const CCSize&	getGridSize ( KDvoid ) { return m_tGridSize; }
		inline  KDvoid			setGridSize ( const CCSize& tGridSize ) { m_tGridSize = tGridSize; }

		/** pixels between the grids */
		inline  const CCPoint&	getStep ( KDvoid ) { return m_tStep; }
		inline  KDvoid			setStep ( const CCPoint& tStep ) { m_tStep = tStep; }

		/** is texture flipped */
		inline  KDbool			 isTextureFlipped ( KDvoid ) { return m_bIsTextureFlipped; }
		        KDvoid			setTextureFlipped ( KDbool bFlipped );

		KDvoid					beforeDraw ( CCNode* pTarget = KD_NULL );
		KDvoid					 afterDraw ( CCNode* pTarget );

		virtual KDvoid			blit ( KDvoid );
		virtual KDvoid			reuse ( KDvoid );
		virtual KDvoid			calculateVertexPoints ( KDvoid );

		KDvoid					set2DProjection ( KDvoid );
		
		/**
		 *	listen the event that coming to foreground on Android
		 */
		KDvoid					listenBackToForeground ( CCObject* pObject );

	protected :

		KDbool					m_bActive;
		KDint					m_nReuseGrid;
		CCSize					m_tGridSize;
		CCTexture2D*			m_pTexture;
		CCPoint					m_tStep;
		CCGrabber*				m_pGrabber;
		KDbool					m_bIsTextureFlipped;
		CCGLProgram*			m_pShaderProgram;
		ccDirectorProjection	m_eDirectorProjection;
};

/**
 *	CCGrid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
 */
class CCGrid3D : public CCGridBase
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
				 CCGrid3D ( KDvoid );
		virtual ~CCGrid3D ( KDvoid );

		/** create one Grid */
		static  CCGrid3D*		create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );

		/** create one Grid */
		static  CCGrid3D*		create ( const CCSize& tGridSize );

	public :

		/** returns the vertex at a given position */
		ccVertex3F				vertex ( const CCPoint& tPos );

		/** returns the original (non-transformed) vertex at a given position */
		ccVertex3F				originalVertex ( const CCPoint& tPos );

		/** sets a new vertex at a given position */
		KDvoid					setVertex ( const CCPoint& tPos, const ccVertex3F& tVertex );

		virtual KDvoid			blit ( KDvoid );
		virtual KDvoid			reuse ( KDvoid );
		virtual KDvoid			calculateVertexPoints ( KDvoid );

	protected :

		GLvoid*					m_pTexCoordinates;
		GLvoid*					m_pVertices;
		GLvoid*					m_pOriginalVertices;
		GLushort*				m_pIndices;
};

/**
 *	CCTiledGrid3D is a 3D grid implementation. It differs from Grid3D in that
 *	the tiles can be separated from the grid.
 */
class CCTiledGrid3D : public CCGridBase
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
				 CCTiledGrid3D ( KDvoid );
		virtual ~CCTiledGrid3D ( KDvoid );

		/** create one Grid */
		static  CCTiledGrid3D*		create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped );

		/** create one Grid */
		static  CCTiledGrid3D*		create ( const CCSize& tGridSize );

	public :

		/** returns the tile at the given position */
		ccQuad3					tile ( const CCPoint& tPos );

		/** returns the original tile (untransformed) at the given position */
		ccQuad3					originalTile ( const CCPoint& tPos );

		/** sets a new tile */
		KDvoid					setTile ( const CCPoint& tPos, const ccQuad3& tCoords );

		virtual KDvoid			blit ( KDvoid );
		virtual KDvoid			reuse ( KDvoid );
		virtual KDvoid			calculateVertexPoints ( KDvoid );

	protected :

		GLvoid*					m_pTexCoordinates;
		GLvoid*					m_pVertices;
		GLvoid*					m_pOriginalVertices;
		GLushort*				m_pIndices;
};

// end of effects group
/// @}

NS_CC_END

#endif // __CCGrid_h__
