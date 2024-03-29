/* --------------------------------------------------------------------------
 *
 *      File            EngManager.h
 *      Description     Engine Manager
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

#ifndef __EngManager_h__
#define __EngManager_h__

class CEngManager
{
	protected :

		 CEngManager ( KDvoid );
		~CEngManager ( KDvoid );

	public :

		static KDvoid  create  ( KDvoid );
		static KDvoid  release ( KDvoid );

	public :

        KDvoid  dispatch ( const KDEvent* pEvent );
    
		IrrlichtDevice*                 getDevice    ( KDvoid );
        scene::ISceneManager*           getScene     ( KDvoid );
        video::IVideoDriver*            getVideo     ( KDvoid );
        scene::ICameraSceneNode*        getCamera    ( KDvoid );
        const scene::IGeometryCreator*  getGeometry  ( KDvoid );
        scene::ISceneCollisionManager*  getCollision ( KDvoid );
        ITimer*                         getTimer     ( KDvoid );
        KDuint                          getTime      ( KDvoid );
    
        inline KDfloat  getBearing ( KDvoid ) { return m_fBearing; }
        inline KDfloat  getPitch   ( KDvoid ) { return m_fPitch; }
    
    protected :
    
        IrrlichtDevice*     m_pIrrlicht;
        KDfloat             m_fBearing;
        KDfloat             m_fPitch;
};

extern CEngManager*  g_pEngMgr;

#endif