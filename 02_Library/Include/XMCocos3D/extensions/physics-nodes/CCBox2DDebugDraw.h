/* --------------------------------------------------------------------------
 *
 *      File            CCBox2DDebugDraw.h
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

#ifndef __CCBox2DDebugDraw_h__
#define __CCBox2DDebugDraw_h__

#include "XMBox2D/Box2D.h"
#include "../ExtensionMacros.h"
#include "../../base/CCGeometry.h"

struct b2AABB;

NS_CC_EXT_BEGIN

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class Box2DDebugDraw : public b2Draw
{
	public :

		Box2DDebugDraw ( KDfloat fRatio = 1.0f );

		Box2DDebugDraw ( KDfloat fRatio, KDfloat fPerspectiveRatio, const cocos2d::Size& tAreaSize );
		
	public :

		virtual KDvoid			DrawPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor );

		virtual KDvoid			DrawSolidPolygon ( const b2Vec2* pVertices, KDint nVertexCount, const b2Color& tColor );

		virtual KDvoid			DrawCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Color& tColor );

		virtual KDvoid			DrawSolidCircle ( const b2Vec2& tCenter, KDfloat32 fRadius, const b2Vec2& tAxis, const b2Color& tColor );

		virtual KDvoid			DrawSegment ( const b2Vec2& tPoint1, const b2Vec2& tPoint2, const b2Color& tColor );

		virtual KDvoid			DrawTransform ( const b2Transform& xf );

		virtual KDvoid			DrawPoint ( const b2Vec2& tPoint, KDfloat32 fSize, const b2Color& tColor );

		virtual KDvoid			DrawString ( KDint x, KDint y, const KDchar* szString, ... ); 

		virtual KDvoid			DrawAABB ( b2AABB* aabb, const b2Color& tColor );

	protected :

		virtual KDfloat			convertPositionX ( const cocos2d::Size& tAreaSize, KDfloat x );
		virtual KDfloat			convertPositionY ( const cocos2d::Size& tAreaSize, KDfloat y );

	protected :

		KDfloat					m_fRatio;
		KDfloat					m_fPerspectiveRatio;
		cocos2d::Size			m_tAreaSize;
};

NS_CC_EXT_END

#endif	// __CCBox2DDebugDraw_h__
