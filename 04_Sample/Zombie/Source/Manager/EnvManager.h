/* --------------------------------------------------------------------------
 *
 *      File            EnvManager.h
 *      Description     Environment Manager
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __EnvManager_h__
#define __EnvManager_h__

class CEnvManager
{
	protected :

		 CEnvManager ( KDvoid );
		~CEnvManager ( KDvoid );

	public :

		static KDvoid  create  ( KDvoid );
		static KDvoid  release ( KDvoid );

	public :
    
        inline KDvoid  plusScore ( KDuint uValue )
        {
            m_uScore += uValue;
        }

		CC_SYNTHESIZE_PASS_BY_REF ( KDuint, m_uStage  , Stage  );
        CC_SYNTHESIZE_PASS_BY_REF ( KDuint, m_uEnergy , Energy );
        CC_SYNTHESIZE_PASS_BY_REF ( KDuint, m_uScore  , Score  );
};

extern CEnvManager*  g_pEnvMgr;

#endif