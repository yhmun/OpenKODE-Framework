/* --------------------------------------------------------------------------
 *
 *      File            CCBox2DDebugDraw.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
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

#include "Precompiled.h"
#include "extensions/CCPhysics/CCBox2DDebugDraw.h"
#include "cocoa/CCGeometry.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCBox2DDebugDraw::CCBox2DDebugDraw ( KDfloat fRatio )
{
	m_fRatio = fRatio;
	m_fPerspectiveRatio = 1.f;
	m_tAreaSize = ccsz;
}

CCBox2DDebugDraw::CCBox2DDebugDraw ( KDfloat fRatio, KDfloat fPerspectiveRatio, const CCSize& tAreaSize )
{
	m_fRatio = fRatio;
	m_fPerspectiveRatio = fPerspectiveRatio;
	m_tAreaSize = tAreaSize;
}

KDfloat CCBox2DDebugDraw::convertPositionX ( const CCSize& tAreaSize, KDfloat x )
{
	return x - ( m_tAreaSize.cx / 2 );
}

KDfloat CCBox2DDebugDraw::convertPositionY ( const CCSize& tAreaSize, KDfloat y )
{
	return y - ( m_tAreaSize.cy / 2 );
}

KDvoid CCBox2DDebugDraw::DrawPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor )
{
	CCPoint*	pNewPoints = new CCPoint [ nVertexCount ];

	for ( KDint  i = 0; i < nVertexCount; i++ )
	{
		pNewPoints [ i ].x = convertPositionX ( m_tAreaSize, pVertices [ i ].x * m_fRatio );
		pNewPoints [ i ].y = convertPositionY ( m_tAreaSize, pVertices [ i ].y * m_fRatio * m_fPerspectiveRatio );
	}

	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	ccDrawPoly ( pNewPoints, nVertexCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid CCBox2DDebugDraw::DrawSolidPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor )
{
	CCPoint*	pNewPoints = new CCPoint [ nVertexCount ];

	for ( KDint  i = 0; i < nVertexCount; i++ )
	{
		pNewPoints [ i ].x = convertPositionX ( m_tAreaSize, pVertices [ i ].x * m_fRatio );
		pNewPoints [ i ].y = convertPositionY ( m_tAreaSize, pVertices [ i ].y * m_fRatio * m_fPerspectiveRatio );
	}

	ccDrawColor4F	( tColor.r * 0.5f, tColor.g * 0.5f, tColor.b * 0.5f, 0.5f );
	ccDrawSolidPoly ( pNewPoints, nVertexCount );

	ccDrawColor4F	( tColor.r, tColor.g, tColor.b, 1 );
	ccDrawPoly		( pNewPoints, nVertexCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid CCBox2DDebugDraw::DrawCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Color& tColor )
{	
	CCPoint		tNewCenter = ccp 
	(
		convertPositionX ( m_tAreaSize, tCenter.x * m_fRatio ),
		convertPositionY ( m_tAreaSize, tCenter.y * m_fRatio * m_fPerspectiveRatio )
	); 

	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	ccDrawCircle  ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
}

KDvoid CCBox2DDebugDraw::DrawSolidCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Vec2& tAxis, const b2Color& tColor )
{		
	CCPoint		tNewCenter = ccp 
	(
		convertPositionX ( m_tAreaSize, tCenter.x * m_fRatio ),
		convertPositionY ( m_tAreaSize, tCenter.y * m_fRatio * m_fPerspectiveRatio )
	); 

	if ( m_fPerspectiveRatio == 1 )
	{
		ccDrawColor4F	  ( tColor.r * 0.5f, tColor.g * 0.5f, tColor.b * 0.5f, 0.5f );
		ccDrawSolidCircle ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );

		ccDrawColor4F	  ( tColor.r, tColor.g, tColor.b, 1 );
		ccDrawCircle	  ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
	}
	else
	{
		ccDrawColor4F	  ( 0.15f, 0.15f, 0.15f, 0.1f );
		ccDrawSolidCircle ( tNewCenter, fRadius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
	}

	// Draw the axis line
	DrawSegment ( tCenter, tCenter + fRadius * tAxis, tColor );
}

KDvoid CCBox2DDebugDraw::DrawSegment ( const b2Vec2& tPoint1, const b2Vec2& tPoint2, const b2Color& tColor )
{
	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	ccDrawLine
	(
		ccp
		(
			convertPositionX ( m_tAreaSize, tPoint1.x * m_fRatio ),
			convertPositionY ( m_tAreaSize, tPoint1.y * m_fRatio * m_fPerspectiveRatio )
		),
		ccp 
		(
			convertPositionX ( m_tAreaSize, tPoint2.x * m_fRatio ), 
			convertPositionY ( m_tAreaSize, tPoint2.y * m_fRatio * m_fPerspectiveRatio )  
		)
	);
}

KDvoid CCBox2DDebugDraw::DrawTransform ( const b2Transform& xf )
{
	b2Vec2				p1 = xf.p, p2;
	const KDfloat32		k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis ( );
	DrawSegment ( p1, p2, b2Color ( 1, 0, 0 ) );
	
	p2 = p1 + k_axisScale * xf.q.GetYAxis ( );
	DrawSegment ( p1, p2, b2Color ( 0, 1, 0 ) );
}

KDvoid CCBox2DDebugDraw::DrawPoint ( const b2Vec2& tPoint, KDfloat32 fSize, const b2Color& tColor )
{
	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	ccPointSize   ( fSize );

	ccDrawPoint 
	(
		ccp 
		(
			convertPositionX ( m_tAreaSize, tPoint.x * m_fRatio ),
			convertPositionY ( m_tAreaSize, tPoint.y * m_fRatio * m_fPerspectiveRatio )
		) 
	);
	ccPointSize ( 1.0f );
}

KDvoid CCBox2DDebugDraw::DrawString ( KDint x, KDint y, const KDchar* szString, ... )
{

}

KDvoid CCBox2DDebugDraw::DrawAABB ( b2AABB* aabb, const b2Color& tColor )
{
	CCPoint		pVertices [ ] =
	{
		ccp ( aabb->lowerBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		ccp ( aabb->upperBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		ccp ( aabb->upperBound.x * m_fRatio, aabb->upperBound.y * m_fRatio ),
		ccp ( aabb->lowerBound.x * m_fRatio, aabb->upperBound.y * m_fRatio )
	};

	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, 1 );
	ccDrawPoly ( pVertices, 4, KD_TRUE );
}

NS_CC_END