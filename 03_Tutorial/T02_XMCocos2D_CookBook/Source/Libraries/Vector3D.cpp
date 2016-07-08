/* --------------------------------------------------------------------------
 *
 *      File            Vector3D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#include "Precompiled.h"
#include "Vector3D.h"

Vector3D* Vector3D::create ( KDfloat x, KDfloat y, KDfloat z )
{
	Vector3D*		pRet = new Vector3D ( x, y, z );

	if ( pRet )
	{
		pRet->autorelease ( );		
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Vector3D::Vector3D ( KDvoid )
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3D::Vector3D ( KDfloat x, KDfloat y, KDfloat z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

KDbool Vector3D::equals ( const Vector3D& v ) const
{
	return ( this->x == v.x && this->y == v.y && this->z == v.z );
}