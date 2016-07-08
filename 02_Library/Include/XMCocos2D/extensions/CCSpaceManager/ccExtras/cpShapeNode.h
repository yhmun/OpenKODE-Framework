/* --------------------------------------------------------------------------
 *
 *      File            cpShapeNode.h
 *      Description     Provide Drawing for Shapes
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2009 Robert Blackwood 02/22/2009
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __cpShapeNode_h__
#define __cpShapeNode_h__

#include "cpCCNode.h"

NS_CC_BEGIN

// Utility draw functions
/** GL pre draw state for cpShapeNode */
KDvoid		cpShapeNodePreDrawState ( CCGLProgram* pShader );

/** GL post draw state for cpShapeNode */
KDvoid		cpShapeNodePostDrawState ( KDvoid );

// draw a shape with the correct pre/post states
KDvoid		cpShapeNodeDrawAt ( cpShape* pShape, CCPoint tPosition, CCPoint tRotation );

// draw a shape without the pre/post states
// Use the pre/post calls above to draw many shapes at once
KDvoid		cpShapeNodeEfficientDrawAt ( cpShape* pShape, CCPoint tPosition, CCPoint tRotation );

class cpShapeNode : public cpCCNode
{
	public :
				 cpShapeNode ( KDvoid );
		virtual ~cpShapeNode ( KDvoid );

		static cpShapeNode*		create ( cpShape* pShape );

	public :

		KDbool					initWithShape ( cpShape* pShape );

		virtual KDvoid			draw ( KDvoid );

		virtual cpShape*		getShape ( KDvoid );
		virtual KDvoid			setShape ( cpShape* pShape );

		/** Size of drawn points, default is 3 */
		CC_SYNTHESIZE ( cpFloat, m_fPointSize, PointSize );

		/** Width of the drawn lines, default is 1 */
		CC_SYNTHESIZE ( cpFloat, m_fLineWidth, LineWidth );

		/** If this is set to YES/TRUE then the shape will be drawn with smooth lines/points */
		CC_SYNTHESIZE_BOOL ( m_bSmoothDraw, SmoothDraw );

		/** If this is set to YES/TRUE then the shape will be filled when drawn */
		CC_SYNTHESIZE_BOOL ( m_bFillShape, FillShape );

		/** Currently only circle has a "decoration" it is an extra line to see the rotation */
		CC_SYNTHESIZE_BOOL ( m_bDrawDecoration, DrawDecoration );

		/** 
		 *	Cache the drawing (default is YES), if you are going to be changing the
		 *	shape physically (ex. increase circle radius) then this should be NO 
		 */
		CC_SYNTHESIZE_BOOL ( m_bCacheDraw, CacheDraw );

		CC_SYNTHESIZE_BOOL ( m_bOpacityModifyRGB, OpacityModifyRGB );

	private :

		KDvoid			cacheCircle   ( KDvoid );
		KDvoid			cachePoly     ( KDvoid );
		KDvoid			cacheSegment  ( KDvoid );

		KDvoid			preDrawState  ( KDvoid );
		KDvoid			postDrawState ( KDvoid );

		KDvoid			drawCircleShape  ( KDvoid );
		KDvoid			drawSegmentShape ( KDvoid );
		KDvoid			drawPolyShape    ( KDvoid );

	protected :

		cpShape*		m_pShape;

		// cache
		cpShapeType		m_eLastType;
		GLfloat*		m_pVertices;
		KDint			m_nVerticesCount;

		KDint			m_nColorLocation;
		KDint			m_nPointLocation;
};

class cpShapeTextureBatchNode;

class cpShapeTextureNode : public cpShapeNode
{
	public :

		/** Create a node given a shape and texture filename */
		static cpShapeTextureNode*		create ( cpShape* pShape, const KDchar* szFile );

		/** Create a node given a shape and texture */
		static cpShapeTextureNode*		create ( cpShape* pShape, CCTexture2D* pTexture );

		/** Create a node that will be added to a cpShapeTextureBatchNode */
		static cpShapeTextureNode*		create ( cpShape* pShape, cpShapeTextureBatchNode* pBatchNode );

	public :

		/** Initialize a node given a shape and texture filename */
		KDbool  initWithShape ( cpShape* pShape, const KDchar* szFile );

		/** Initialize a node given a shape and texture */
		KDbool  initWithShape ( cpShape* pShape, CCTexture2D* pTexture );

		/** Initialize a node that will be added to a cpShapeTextureBatchNode */
		KDbool  initWithShape ( cpShape* pShape, cpShapeTextureBatchNode* pBatchNode );

	public :

		CC_PROPERTY ( CCTexture2D*, m_pTexture, Texture );

		CC_PROPERTY ( CCPoint, m_tTextureOffset, TextureOffset );

		CC_PROPERTY ( KDfloat, m_fTextureRotation, TextureRotation );

	protected :

		cpShapeTextureBatchNode*		m_pBatchNode;
		
		// cache
		GLfloat*						m_pCoordinates;
		ccColor4B*						m_pColors;
};

class cpShapeTextureBatchNode : public CCNode
{
	public :	

		/** Create a node given a texture filename */
		static cpShapeTextureBatchNode*		create ( const KDchar* szFile );

		/** Create a node given a texture */
		static cpShapeTextureBatchNode*		create ( CCTexture2D* pTexture );

	public :

		/** Initialize a node given a texture filename */
		KDbool		initWithFile ( const KDchar* szFile );

		/** Initialize a node given a texture */
		KDbool		initWithTexture ( CCTexture2D* pTexture );

	public :

		CC_PROPERTY ( CCTexture2D*, m_pTexture, Texture );
};

NS_CC_END

#endif	// __cpShapeNode_h__