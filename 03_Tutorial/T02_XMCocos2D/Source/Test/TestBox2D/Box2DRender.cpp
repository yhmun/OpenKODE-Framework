/* --------------------------------------------------------------------------
 *
 *      File            Box2DRender.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2006-2007 Erin Catto
 *
 *         http://www.cocos2d-x.org      
 *         http://www.gphysics.com   
 *
 *      iPhone port by Simon Oliver
 *
 *         http://www.simonoliver.com - http://www.handcircus.com 
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
#include "Box2DRender.h"

GLESDebugDraw::GLESDebugDraw ( KDfloat fRatio )
{
	m_fRatio = fRatio;
}

KDvoid GLESDebugDraw::DrawPolygon ( const b2Vec2* old_vertices, KDint vertexCount, const b2Color& color )
{
	KDint     nCount     = vertexCount;
	CCPoint*  pNewPoints = new CCPoint [ nCount ];

	for ( KDint  i = 0; i < nCount; i++ )
	{
		pNewPoints [ i ].x = old_vertices [ i ].x * m_fRatio;
		pNewPoints [ i ].y = old_vertices [ i ].y * m_fRatio;
	}

	ccDrawColor4F ( color.r, color.g, color.b,1 );
	ccDrawPoly ( pNewPoints, nCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid GLESDebugDraw::DrawSolidPolygon ( const b2Vec2* old_vertices, KDint vertexCount, const b2Color& color )
{
	KDint     nCount     = vertexCount;
	CCPoint*  pNewPoints = new CCPoint [ nCount ];

	for ( KDint  i = 0; i < nCount; i++ )
	{
		pNewPoints [ i ].x = old_vertices [ i ].x * m_fRatio;
		pNewPoints [ i ].y = old_vertices [ i ].y * m_fRatio;
	}

	ccDrawColor4F	( color.r, color.g, color.b, 0.5f );
	ccDrawSolidPoly ( pNewPoints, nCount );

	ccDrawColor4F	( color.r, color.g, color.b, 1 );
	ccDrawPoly		( pNewPoints, nCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

KDvoid GLESDebugDraw::DrawCircle ( const b2Vec2& center, float32 radius, const b2Color& color )
{	
	ccDrawColor4F ( color.r, color.g, color.b, 1 );
	ccDrawCircle  ( ccp ( center.x, center.y - 1 ), radius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );
}

KDvoid GLESDebugDraw::DrawSolidCircle ( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
{		
	ccDrawColor4F	  ( color.r, color.g, color.b, 0.5f );
	ccDrawSolidCircle ( ccp ( center.x, center.y - 1 ), radius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );

	ccDrawColor4F	  ( color.r, color.g, color.b, 1 );
	ccDrawCircle	  ( ccp ( center.x, center.y - 1 ), radius, 0, 16, KD_FALSE, m_fRatio, m_fRatio );

	// Draw the axis line
	DrawSegment ( center, center + radius * axis, color );
}

KDvoid GLESDebugDraw::DrawSegment ( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
{
	ccDrawColor4F ( color.r, color.g, color.b, 1 );
	ccDrawLine ( ccp ( p1.x * m_fRatio, p1.y * m_fRatio ), ccp ( p2.x * m_fRatio, p2.y * m_fRatio ) );
}

KDvoid GLESDebugDraw::DrawTransform ( const b2Transform& xf )
{
	b2Vec2  p1 = xf.p, p2;
	const float32  k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis ( );
	DrawSegment ( p1, p2, b2Color ( 1, 0, 0 ) );
	
	p2 = p1 + k_axisScale * xf.q.GetYAxis ( );
	DrawSegment ( p1, p2, b2Color ( 0, 1, 0 ) );
}

KDvoid GLESDebugDraw::DrawPoint ( const b2Vec2& p, float32 size, const b2Color& color )
{
	ccDrawColor4F ( color.r, color.g, color.b,1 );
	ccPointSize   ( size );

	ccDrawPoint ( ccp ( p.x * m_fRatio, p.y * m_fRatio ) );
	ccPointSize ( 1.0f );
}

KDvoid GLESDebugDraw::DrawString ( KDint x, KDint y, const KDchar* string, ... )
{

}

KDvoid GLESDebugDraw::DrawAABB ( b2AABB* aabb, const b2Color& c )
{
	CCPoint	 vertices[] =
	{
		ccp ( aabb->lowerBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		ccp ( aabb->upperBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio ),
		ccp ( aabb->upperBound.x * m_fRatio, aabb->upperBound.y * m_fRatio ),
		ccp ( aabb->lowerBound.x * m_fRatio, aabb->upperBound.y * m_fRatio )
	};

	ccDrawColor4F ( c.r, c.g, c.b, 1 );
	ccDrawPoly ( vertices, 4, KD_TRUE );
}
