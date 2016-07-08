/* --------------------------------------------------------------------------
 *
 *      File            Box2DRender.h
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

#ifndef __Box2DRender_h__
#define __Box2DRender_h__

struct b2AABB;

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class GLESDebugDraw : public b2Draw
{
	public :

		GLESDebugDraw ( KDfloat fRatio = 1.0f );

	public :

		virtual KDvoid  DrawPolygon ( const b2Vec2* vertices, KDint vertexCount, const b2Color& color );

		virtual KDvoid  DrawSolidPolygon ( const b2Vec2* vertices, KDint vertexCount, const b2Color& color );

		virtual KDvoid  DrawCircle ( const b2Vec2& center, KDfloat radius, const b2Color& color );

		virtual KDvoid  DrawSolidCircle ( const b2Vec2& center, KDfloat radius, const b2Vec2& axis, const b2Color& color );

		virtual KDvoid  DrawSegment ( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color );

		virtual KDvoid  DrawTransform ( const b2Transform& xf );

		virtual KDvoid  DrawPoint ( const b2Vec2& p, KDfloat size, const b2Color& color );

		virtual KDvoid  DrawString ( KDint x, KDint y, const KDchar* string, ... ); 

		virtual KDvoid  DrawAABB ( b2AABB* aabb, const b2Color& color );

	protected :

		KDfloat			m_fRatio;
};


#endif
