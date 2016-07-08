/* --------------------------------------------------------------------------
 *
 *      File            Box2DDebugDraw.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2006-2009 Erin Catto
 *
 *         http://www.cocos2d-x.org      
 *         http://www.box2d.org  
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#include "extensions/physics-nodes/CCBox2DDebugDraw.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"

USING_NS_CC;

NS_CC_EXT_BEGIN

Box2DDebugDraw::Box2DDebugDraw ( KDfloat fRatio )
{
	m_fRatio = fRatio;
	m_fPerspectiveRatio = 1.f;
	m_tAreaSize = Size::ZERO;
}

Box2DDebugDraw::Box2DDebugDraw ( KDfloat fRatio, KDfloat fPerspectiveRatio, const Size& tAreaSize )
{
	m_fRatio = fRatio;
	m_fPerspectiveRatio = fPerspectiveRatio;
	m_tAreaSize = tAreaSize;
}

KDfloat Box2DDebugDraw::convertPositionX ( const Size& tAreaSize, KDfloat x )
{
	return x - ( m_tAreaSize.cx / 2 );
}

KDfloat Box2DDebugDraw::convertPositionY ( const Size& tAreaSize, KDfloat y )
{
	return y - ( m_tAreaSize.cy / 2 );
}

KDvoid Box2DDebugDraw::DrawPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor )
{
	Point*	pNewPoints = new Point [ nVertexCount ];

	for ( KDint  i = 0; i < nVertexCount; i++ )
	{
		pNewPoints [ i ].x = convertPositionX ( m_tAreaSize, pVertices [ i ].x * m_fRatio );
		pNewPoints [ i ].y = convertPositionY ( m_tAreaSize, pVertices [ i ].y * m_fRatio * m_fPerspectiveRatio );
	}

	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::drawPoly ( pNewPoints, nVertexCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid Box2DDebugDraw::DrawSolidPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor )
{
	Point*	pNewPoints = new Point [ nVertexCount ];

	for ( KDint  i = 0; i < nVertexCount; i++ )
	{
		pNewPoints [ i ].x = convertPositionX ( m_tAreaSize, pVertices [ i ].x * m_fRatio );
		pNewPoints [ i ].y = convertPositionY ( m_tAreaSize, pVertices [ i ].y * m_fRatio * m_fPerspectiveRatio );
	}

	DrawPrimitives::setDrawColor4F ( tColor.r * 0.5f, tColor.g * 0.5f, tColor.b * 0.5f, 0.5f );
	DrawPrimitives::drawSolidPoly  ( pNewPoints, nVertexCount );

	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::drawPoly	   ( pNewPoints, nVertexCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid Box2DDebugDraw::DrawCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Color& tColor )
{	
	Point		tNewCenter = Point 
	(
		convertPositionX ( m_tAreaSize, tCenter.x * m_fRatio ),
		convertPositionY ( m_tAreaSize, tCenter.y * m_fRatio * m_fPerspectiveRatio )
	); 

	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::drawCircle ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
}

KDvoid Box2DDebugDraw::DrawSolidCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Vec2& tAxis, const b2Color& tColor )
{		
	Point		tNewCenter = Point 
	(
		convertPositionX ( m_tAreaSize, tCenter.x * m_fRatio ),
		convertPositionY ( m_tAreaSize, tCenter.y * m_fRatio * m_fPerspectiveRatio )
	); 

	if ( m_fPerspectiveRatio == 1 )
	{
		DrawPrimitives::setDrawColor4F  ( tColor.r * 0.5f, tColor.g * 0.5f, tColor.b * 0.5f, 0.5f );
		DrawPrimitives::drawSolidCircle ( tNewCenter, fRadius, 0, 16, m_fRatio, m_fRatio );

		DrawPrimitives::setDrawColor4F  ( tColor.r, tColor.g, tColor.b, 1 );
		DrawPrimitives::drawCircle	    ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
	}
	else
	{
		DrawPrimitives::setDrawColor4F  ( 0.15f, 0.15f, 0.15f, 0.1f );
		DrawPrimitives::drawSolidCircle ( tNewCenter, fRadius, 0, 16, m_fRatio, m_fRatio );
	}

	// Draw the axis line
	DrawSegment ( tCenter, tCenter + fRadius * tAxis, tColor );
}

KDvoid Box2DDebugDraw::DrawSegment ( const b2Vec2& tPoint1, const b2Vec2& tPoint2, const b2Color& tColor )
{
	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::drawLine
	(
		Point
		(
			convertPositionX ( m_tAreaSize, tPoint1.x * m_fRatio ),
			convertPositionY ( m_tAreaSize, tPoint1.y * m_fRatio * m_fPerspectiveRatio )
		),
		Point 
		(
			convertPositionX ( m_tAreaSize, tPoint2.x * m_fRatio ), 
			convertPositionY ( m_tAreaSize, tPoint2.y * m_fRatio * m_fPerspectiveRatio )  
		)
	);
}

KDvoid Box2DDebugDraw::DrawTransform ( const b2Transform& xf )
{
	b2Vec2				p1 = xf.p, p2;
	const KDfloat32		k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis ( );
	DrawSegment ( p1, p2, b2Color ( 1, 0, 0 ) );
	
	p2 = p1 + k_axisScale * xf.q.GetYAxis ( );
	DrawSegment ( p1, p2, b2Color ( 0, 1, 0 ) );
}

KDvoid Box2DDebugDraw::DrawPoint ( const b2Vec2& tPoint, KDfloat32 fSize, const b2Color& tColor )
{
	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::setPointSize   ( fSize );

	DrawPrimitives::drawPoint 
	(
		Point 
		(
			convertPositionX ( m_tAreaSize, tPoint.x * m_fRatio ),
			convertPositionY ( m_tAreaSize, tPoint.y * m_fRatio * m_fPerspectiveRatio )
		) 
	);
	DrawPrimitives::setPointSize ( 1.0f );
}

KDvoid Box2DDebugDraw::DrawString ( KDint x, KDint y, const KDchar* szString, ... )
{

}

KDvoid Box2DDebugDraw::DrawAABB ( b2AABB* aabb, const b2Color& tColor )
{
	Point		pVertices [ ] =
	{
		Point ( aabb->lowerBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		Point ( aabb->upperBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		Point ( aabb->upperBound.x * m_fRatio, aabb->upperBound.y * m_fRatio ),
		Point ( aabb->lowerBound.x * m_fRatio, aabb->upperBound.y * m_fRatio )
	};

	DrawPrimitives::setDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	DrawPrimitives::drawPoly ( pVertices, 4, KD_TRUE );
}

NS_CC_EXT_END
