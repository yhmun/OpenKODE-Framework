/* --------------------------------------------------------------------------
 *
 *      File            Cube3D.h
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

#ifndef __Cube3D_h__
#define __Cube3D_h__

class Cube3D : public CCSprite
{
	public :

		Cube3D ( KDvoid );

		static Cube3D*				create ( const KDchar* szFilePath );

	public :		

		virtual KDvoid				draw ( KDvoid );

		virtual const Vector3D&		getTranslation3D ( KDvoid );
		virtual KDvoid				setTranslation3D ( const Vector3D& tVector );

		virtual const Vector3D&		getRotation3DAxis ( KDvoid );
		virtual KDvoid				setRotation3DAxis ( const Vector3D& tVector );

		virtual KDfloat				getRotation3DAngle ( KDvoid );
		virtual KDvoid				setRotation3DAngle ( KDfloat fAngle );

		virtual KDbool				 isDrawTextured ( KDvoid );
		virtual KDvoid				setDrawTextured ( KDbool bDraw );

	protected :

		Vector3D					m_tTranslation3D;
		Vector3D					m_tRotation3DAxis;
		KDfloat						m_fRotation3DAngle;
		KDbool						m_bDrawTextured;
};

#endif	// __Cube3D_h__
