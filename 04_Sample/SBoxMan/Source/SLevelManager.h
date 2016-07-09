/* --------------------------------------------------------------------------
 *
 *      File            SLevelManager.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-07
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

#ifndef __SLevelManager_h__
#define __SLevelManager_h__

class SLevel;

class SLevelManager
{
	public :

		static SLevelManager*	sharedLevelManager ( KDvoid );

	protected :

				 SLevelManager ( KDvoid );
		virtual ~SLevelManager ( KDvoid );

		virtual KDbool		init ( KDvoid );

	public :

		CCArray*			getLevelArray ( KDvoid );

		KDbool				hasPrevLevel ( KDvoid );

		KDbool				hasNextLevel ( KDvoid );

		SLevel*				prevLevel ( KDvoid );

		SLevel*				nextLevel ( KDvoid );

		SLevel*				getLevel ( KDvoid );

		KDint				getCurrentLevel ( KDvoid );
		KDvoid				setCurrentLevel ( KDint nCurrentLevel );		

	private :

		CCArray*			m_pLevelArray;
		SLevel*				m_pLevel;
		KDint				m_nCurrentLevel;
};

#endif	// __SLevelManager_h__