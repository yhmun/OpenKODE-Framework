/* --------------------------------------------------------------------------
 *
 *      File            Enemy.h
 *      Description     
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Kirill Muzykov on 8/22/13.
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

#ifndef __Enemy_h__
#define __Enemy_h__

class GameLayer;
class Waypoint;
class Tower;

class Enemy : public CCSprite
{
	public :

		static Enemy*		create ( const KDchar* szFileName );

	protected :

		virtual KDbool		initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		KDvoid				getRemoved ( KDvoid );

	public :

		KDvoid				setTheGame ( GameLayer* pGame );

		KDvoid				setDestinationWaypoint ( Waypoint* pWaypoint );

		KDvoid				doActivate ( KDfloat fDelta = KD_NULL );

		KDvoid				getAttacked ( Tower* pAttacker );

		KDvoid				getDamaged ( KDint nDamage );    

		KDvoid				gotLostSight ( Tower* pAttacker );

	private :

		KDint				m_nMaxHP;
		KDint				m_nCurrentHP;
		KDbool				m_bActive;
		KDfloat				m_fWalkingSpeed;
	    
		GameLayer*			m_pTheGame;
		Waypoint*			m_pDestinationWaypoint;
	  
		CCArray				m_aAttackedBy;
};

#endif	// __Enemy_h__
