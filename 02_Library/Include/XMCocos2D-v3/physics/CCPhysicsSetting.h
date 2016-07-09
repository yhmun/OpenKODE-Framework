/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsSetting.h
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

#ifndef __CCPhysicsSetting_h__
#define __CCPhysicsSetting_h__

#include <kode.h>

#define CC_PHYSICS_UNKNOWN			0
#define CC_PHYSICS_BOX2D			1
#define CC_PHYSICS_CHIPMUNK			2

#define CC_USE_CHIPMUNK

#ifdef CC_USE_BOX2D
#define CC_PHYSICS_ENGINE CC_PHYSICS_BOX2D
#elif defined ( CC_USE_CHIPMUNK )
#define CC_PHYSICS_ENGINE CC_PHYSICS_CHIPMUNK
#else
#define CC_PHYSICS_ENGINE CC_PHYSICS_UNKNOWN
#endif

#if ( CC_PHYSICS_ENGINE != CC_PHYSICS_UNKNOWN )
#define CC_USE_PHYSICS
#endif

namespace cocos2d
{
    extern const KDfloat	PHYSICS_INFINITY;
    
    class Point;
    typedef Point Vect;
    
#if ( CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK )
    static const KDint		PHYSICS_CONTACT_POINT_MAX = 4;
#else
#endif
}

#endif	// __CCPhysicsSetting_h__
