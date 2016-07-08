/* --------------------------------------------------------------------------
 *
 *      File            GameActor.h
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

#ifndef __GameActor_h__
#define __GameActor_h__

#include "GameObject.h"
#include "GameWaypoint.h"

class GameActor : public GameObject
{
	public :
				 GameActor ( KDvoid );
		virtual ~GameActor ( KDvoid );

		static GameActor*		create ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			addWaypoint ( GameWaypoint* pWaypoint );

		virtual KDvoid			processWaypoints ( KDvoid );

		virtual KDvoid			runWithVector ( const CCPoint& tVect, KDfloat fSpeedMod, KDbool bConstrain );
		
		virtual KDvoid			stopRunning ( KDvoid );

		static  CCPoint			getNormalVectorFromDirection ( KDint nDirection );
		
		virtual KDvoid			animateActor ( KDvoid );

		virtual KDvoid			timeShift ( KDfloat fSeconds, const CCPoint& tVect, KDfloat fMagnitude );

		virtual CCArray*		getWaypoints ( KDvoid );
		virtual KDvoid			setWaypoints ( CCArray* pWaypoints );

		virtual KDfloat			getRunSpeed ( KDvoid );
		virtual KDvoid			setRunSpeed ( KDfloat fRunSpeed );

		virtual KDint			getDirection ( KDvoid );
		virtual KDvoid			setDirection ( KDint nDirection );

		virtual const CCPoint&	getLastVelocity ( KDvoid );
		virtual KDvoid			setLastVelocity ( const CCPoint& tLastVelocity );

		virtual const KDchar*	getColor ( KDvoid );
		virtual KDvoid			setColor ( const KDchar* szColor );

		virtual KDfloat			getAnimCount ( KDvoid );
		virtual KDvoid			setAnimCount ( KDfloat fAnimCount );

	protected :

		CCArray*				m_pWaypoints;
		KDint					m_nTimesBlocked;
		KDfloat					m_fLastAngularVelocity;
		CCPoint					m_tLastVelocity;
		std::string				m_sColor;		
		KDfloat					m_fRunSpeed;
		KDint					m_nDirection;
		KDfloat					m_fAnimCount;
};

#endif	// __GameActor_h__
