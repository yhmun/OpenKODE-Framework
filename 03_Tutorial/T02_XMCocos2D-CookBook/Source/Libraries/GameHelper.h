/* --------------------------------------------------------------------------
 *
 *      File            GameHelper.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __GameHelper_h__
#define __GameHelper_h__

#include "Vector3D.h"

#define PI_CONSTANT		3.14159f

typedef enum 
{
	NO_DIRECTION		= 0,
	LEFT				= 1,
	UP_LEFT				= 2,
	UP					= 3,
	UP_RIGHT			= 4,
	RIGHT				= 5,
	DOWN_RIGHT			= 6,
	DOWN				= 7,
	DOWN_LEFT			= 8,
} DirectionType;

class GameHelper 
{
	public :

		static KDbool			clockwiseO ( Vector3D* O, Vector3D* A, Vector3D* B );

		static CCArray*			convexHull ( CCArray* P );

		static KDfloat			distanceP1 ( const CCPoint& tPoint1, const CCPoint& tPoint2 );

		static KDfloat			degreesToRadians ( KDfloat fDegrees );

		static KDfloat			radiansToDegrees ( KDfloat fRadians );

		static KDfloat			vectorToRadians ( CCPoint tVector );

		static CCPoint			radiansToVector ( KDfloat fRadians );

		static Vector3D*		quadraticA ( KDfloat A, KDfloat B, KDfloat C );

		static KDfloat			absoluteValue ( KDfloat fAbs );

		static CCPoint			midPointP1 ( const CCPoint& tPoint1, const CCPoint& tPoint2 );

		static KDbool			point ( const CCPoint& tPoint, const CCRect& tRect );

		static KDbool			point ( const CCPoint& tPoint, const CCPoint& tOrigin, KDfloat fRadius );

		static b2Vec2			b2ClampA ( const b2Vec2& A, const b2Vec2& tLow, const b2Vec2& tHigh );

		static KDchar*			sanitizeString ( KDchar* szStr );
};

#endif	// __GameHelper_h__
