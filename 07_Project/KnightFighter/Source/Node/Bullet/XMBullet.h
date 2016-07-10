/* --------------------------------------------------------------------------
 *
 *      File            XMBullet.h
 *      Description     XMBullet
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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

#ifndef __XMBullet_h__
#define __XMBullet_h__

class XMBullet : public PhysicsSprite
{
	public :

		KDbool				initWithFile ( const std::string& sFilename );

		virtual	KDvoid		shootLinear ( XMShootDirection eDirection, KDfloat fSpeed = 480.0f );
		virtual	KDvoid		shootAiming ( Point tPoint, KDfloat fSpeed = 480.0f );

		virtual KDvoid		shootBulletMoveAndSpread ( Point tPoint, KDfloat nSpeed = 480.0f );

		KDvoid				destroyBullet ( Node* pSender, KDvoid* pData );

		virtual	KDvoid		setDamage ( KDint nDamage );
		virtual KDint		getDamage ( KDvoid );


	protected :

		
		KDint				m_nDamage;
		

		b2World*			m_pParentWorld;
};

#endif  // __XMBullet_h__