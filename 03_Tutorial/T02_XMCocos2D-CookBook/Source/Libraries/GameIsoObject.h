/* --------------------------------------------------------------------------
 *
 *      File            GameIsoObject.h
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

#ifndef __GameIsoObject_h__
#define __GameIsoObject_h__

#include "GameObject.h"

class GameIsoObject : public GameObject
{
	public :
				 GameIsoObject ( KDvoid );
		virtual ~GameIsoObject ( KDvoid );

		static   GameIsoObject*		create ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDint		getType ( KDvoid );

		virtual KDfloat		getModifierY ( KDvoid );
		virtual KDvoid		setModifierY ( KDfloat fModifierY );
		
		virtual KDfloat		getActualImageSize ( KDvoid );
		virtual KDvoid		setActualImageSize ( KDfloat fActualImageSize );
		
		virtual KDfloat		getInGameSize ( KDvoid );
		virtual KDvoid		setInGameSize ( KDfloat fInGameSize );
		
		virtual KDfloat		getModifierZ ( KDvoid );
		virtual KDvoid		setModifierZ ( KDfloat fModifierZ );

		virtual Vector3D*	getBounceCoefficient ( KDvoid );
		virtual KDvoid		setBounceCoefficient ( Vector3D* pBounceCoefficient );
		
		virtual Vector3D*	getRollCoefficient ( KDvoid );
		virtual KDvoid		setRollCoefficient ( Vector3D* pRollCoefficient );

		virtual CCSprite*	getSpriteShadow ( KDvoid );
		virtual KDvoid		setSpriteShadow ( CCSprite* pSpriteShadow );

	protected :

		KDfloat				m_fModifierY;			// This is typically half the height of the object. It allows us to change the sprite y.
		KDfloat				m_fActualImageSize;		// This is the actual size of the image (48x48, 96x96, etc)
		KDfloat				m_fInGameSize;			// This is how large the object in the game is.
		KDfloat				m_fModifierZ;			// Changes the depth testing for this object.
		CCSprite*			m_pSpriteShadow;
		Vector3D*			m_pBounceCoefficient;	// x, y, z, lower is bouncier for Z
		Vector3D*			m_pRollCoefficient;
};

#endif	// __GameIsoObject_h__
