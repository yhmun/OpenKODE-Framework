/* --------------------------------------------------------------------------
 *
 *      File            XMGlobalManager.h
 *      Description     XMGlobalManager
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

#ifndef __XMGlobalManager_h__
#define __XMGlobalManager_h__

class XMEnemy;

class XMGlobalManager
{
	public :
		
		static KDvoid		create ( KDvoid );
		static KDvoid		release ( KDvoid );

	public :

		KDvoid				resetData ( KDvoid );

		Array*				getEnemyContainer ( KDvoid );

		std::map <std::string, std::function<XMEnemy*( b2World*, String* )>> getEnemyMap ( KDvoid );

		KDvoid				setStage ( KDint nStage );
		KDint				getStage ( KDvoid );

		KDvoid				setLevel ( KDint nLevel );
		KDint				getLevel ( KDvoid );

		KDvoid				setScore ( KDint nScore );
		KDint				getScore ( KDvoid );
		KDvoid				addScore ( KDint nAddScore );

	protected :

				 XMGlobalManager ( KDvoid );
		virtual ~XMGlobalManager ( KDvoid );

		KDvoid				initEnemyMap ( KDvoid );

	private :

		Array*				m_pEnemyContainer;

		std::map <std::string, std::function<XMEnemy*( b2World*, String* )>> m_AenemyMap;


		KDint				m_nStage;
		KDint				m_nLevel;

		KDint				m_nScore;
};

extern XMGlobalManager*		GLOBAL;

#endif  // __XMGlobalManager_h__