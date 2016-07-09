/* --------------------------------------------------------------------------
 *
 *      File            EngManager.cpp
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

#include "Precompiled.h"
#include "EngManager.h"
#include "EnvManager.h"
#include "ResManager.h"
#include "ScrManager.h"

#include "../Screen/ScrGame.h"

CEngManager*  g_pEngMgr = KD_NULL;

CEngManager::CEngManager ( KDvoid )
{
	const CCSize&  tEGLSize = CCEGLView::sharedOpenGLView ( )->getFrameSize ( );
    
	m_pIrrlicht = createDevice 
	(
        video::EDT_OGLES1, core::dimension2d<u32> ( (KDuint) tEGLSize.cx , (KDuint) tEGLSize.cy ), 32, false, false, false, 0
    );

	kdSrand ( KD_GET_UST2MSEC );
    
    CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 960, 640, kResolutionExactFit ); 
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );    
}

CEngManager::~CEngManager ( KDvoid )
{
    //m_pIrrlicht->drop ( );
    m_pIrrlicht = KD_NULL;
}

KDvoid CEngManager::create  ( KDvoid )
{
	if ( !g_pEngMgr )
	{
		g_pEngMgr = new CEngManager ( );
	}

	kdAssert ( g_pEngMgr );
}

KDvoid CEngManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pEngMgr );
}

KDvoid CEngManager::dispatch ( const KDEvent* pEvent )
{
	switch ( pEvent->type )
	{
        case KD_EVENT_MAGNETOMETER :
            {                                
                m_fBearing = pEvent->data.magnetometer.azimuth / 360.f;
                
                CScrGame*  pScreen = dynamic_cast<CScrGame*> ( g_pScrMgr->getScreen ( ) );
                if ( pScreen )
                {
                    pScreen->onBearing ( g_pEngMgr->m_fBearing );
                }                            
            }   
            break;
            
        case KD_EVENT_ACCELEROMETER :
            {
                m_fPitch = -pEvent->data.accelerometer.z;
                
                CScrGame*  pScreen = dynamic_cast<CScrGame*> ( g_pScrMgr->getScreen ( ) );
                if ( pScreen )
                {
                    pScreen->onPitch ( g_pEngMgr->m_fPitch );
                }
            }
            break;
	}

	if ( m_pIrrlicht )
	{
		m_pIrrlicht->EventProc ( pEvent );
	}
}

IrrlichtDevice* CEngManager::getDevice ( KDvoid )
{
	return m_pIrrlicht;
}

scene::ISceneManager* CEngManager::getScene ( KDvoid )
{
    return m_pIrrlicht->getSceneManager ( );
}

video::IVideoDriver* CEngManager::getVideo ( KDvoid )
{
    return m_pIrrlicht->getVideoDriver ( );
}

scene::ICameraSceneNode* CEngManager::getCamera ( KDvoid )
{
    return m_pIrrlicht->getSceneManager ( )->getActiveCamera ( );
}

const scene::IGeometryCreator* CEngManager::getGeometry ( KDvoid )
{
    return m_pIrrlicht->getSceneManager ( )->getGeometryCreator ( );
}

scene::ISceneCollisionManager* CEngManager::getCollision ( KDvoid )
{
    return m_pIrrlicht->getSceneManager ( )->getSceneCollisionManager ( );
}

ITimer* CEngManager::getTimer ( KDvoid )
{
    return m_pIrrlicht->getTimer ( );
}

KDuint CEngManager::getTime ( KDvoid )
{
    return m_pIrrlicht->getTimer ( )->getTime ( );
}