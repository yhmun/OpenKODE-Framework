/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsHelper_chipmunk.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __CCPhysicsHelper_chipmunk_h__
#define __CCPhysicsHelper_chipmunk_h__

#include "../CCPhysicsSetting.h"
#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)

#include "XMChipmunk/chipmunk.h"
#include "../../platform/CCPlatformMacros.h"
#include "../../base/CCGeometry.h"

NS_CC_BEGIN

class PhysicsHelper
{
	public :

		static Point		cpv2point ( const cpVect& tVec ) { return Point ( tVec.x, tVec.y ); }
		static cpVect		point2cpv ( const Point& tPoint ) { return cpv ( tPoint.x, tPoint.y ); }
		static Size			cpv2size ( const cpVect& tVec ) { return Size ( tVec.x, tVec.y ); }
		static cpVect		size2cpv ( const Size& tSize ) { return cpv ( tSize.width, tSize.height ); }
		static KDfloat		cpfloat2float ( cpFloat f ) { return f; }
		static cpFloat		float2cpfloat ( KDfloat f ) { return f; }
		static cpBB			rect2cpbb ( const Rect& tRect ) { return cpBBNew ( tRect.origin.x, tRect.origin.y, tRect.origin.x + tRect.size.width, tRect.origin.y + tRect.size.height ); }
		static Rect			cpbb2rect ( const cpBB& bb ) { return Rect ( bb.l, bb.b, bb.r -  bb.l, bb.t - bb.b ); }
    
		static Point*		cpvs2points ( const cpVect* pCpvs, Point* pOut, KDint nCount )
		{
			for ( KDint i = 0; i < nCount; ++i )
			{
				pOut [ i ] = cpv2point ( pCpvs [ i ] );
			}
        
			return pOut;
		}
    
		static cpVect*		points2cpvs ( const Point* pPoints, cpVect* pOut, KDint nCount )
		{
			for ( KDint i = 0; i < nCount; ++i )
			{
				pOut [ i ] = point2cpv ( pPoints [ i ] );
			}
        
			return pOut;
		}
};

NS_CC_END

#endif	// CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
#endif	// __CCPhysicsHelper_chipmunk_h__
