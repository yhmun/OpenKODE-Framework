/* --------------------------------------------------------------------------
 *
 *      File            CWorld.cpp
 *      Description     Game World
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
#include "../Manager/EngManager.h"
#include "../Manager/EnvManager.h"
#include "../Manager/ResManager.h"
#include "../Manager/ScrManager.h"

#include "CWorld.h"

struct SConstant
{
    EPath               eMesh;
    KDfloat             fSpawnDist;
    core::vector3df     tPoint;
};

static struct SConstant  l_aConstants [ ] = 
{
    { eMAP_Stage1,  600.f, core::vector3df (   0, -25,  0 ) },
    { eMAP_Stage2,  600.f, core::vector3df (   0, -25,  0 ) },
    { eMAP_Stage1,  600.f, core::vector3df (   0, -15,  0 ) },
    { eMAP_Stage2,  600.f, core::vector3df (   0, -20,  0 ) }, 
    { eMAP_Stage2,  600.f, core::vector3df (   0, -20,  0 ) }, 
};

CWorld::CWorld ( KDvoid )
{ 
    m_uType = 0;
    m_pNode = 0;
}

CWorld::~CWorld ( KDvoid )
{

}

KDbool CWorld::init ( KDvoid )
{
    return load ( g_pEnvMgr->getStage ( ) );
}

KDbool CWorld::load ( KDuint uType )
{
    scene::ISceneManager*           pScene  = g_pEngMgr->getScene ( );
    scene::ICameraSceneNode*        pCamera = pScene->addCameraSceneNodeFPS ( );
    scene::IAnimatedMesh*           pMesh   = pScene->getMesh ( g_pResMgr->getPath ( l_aConstants [ uType ].eMesh ) );
    scene::IAnimatedMeshSceneNode*  pNode   = pScene->addAnimatedMeshSceneNode ( pMesh );   
    
    //pNode->setScale ( l_aConstants [ uType ].tScale );
    pNode->setMaterialType ( video::EMT_PARALLAX_MAP_SOLID );
    //pNode->setMaterialFlag ( video::EMF_FOG_ENABLE, true );
    pNode->setMaterialFlag ( video::EMF_LIGHTING, true );
    pNode->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
    pNode->addShadowVolumeSceneNode ( );

	pNode->getMaterial(0).Shininess = 0.15f; 
	pNode->getMaterial(0).AmbientColor .set ( 255, 224, 224, 224 ); 
	pNode->getMaterial(0).DiffuseColor .set ( 255, 224, 224, 224 ); 
	pNode->getMaterial(0).EmissiveColor.set ( 255,  10,  10,  10 ); 
    pNode->getMaterial(0).SpecularColor.set ( 255, 255, 255, 255 ); 
    pNode->setPosition ( l_aConstants [ uType ].tPoint );
    
	//pCamera->setPosition ( l_aConstants [ uType ].tCamPos );// * l_aConstants [ uType ].tScale );
    //pCamera->setFOV ( KD_PI_F / 2.5f );             
    pCamera->setFOV ( KD_DEG2RAD ( 90 ) ); 
    
    scene::ILightSceneNode*  pLight = pScene->addLightSceneNode 
	(
        0, 
        core::vector3df ( 0, 0, 0 ),//pCamera->getPosition ( ), 
        video::SColorf ( 0.7f, 0.3f, 0.3f, 0.5f ), 
        150.f
    );
    
    scene::ISceneNodeAnimator*  pAnim = pScene->createFlyCircleAnimator (core::vector3df ( 0, 0, 0 ), 20.0f, 0.005f );
    pLight->addAnimator ( pAnim );
    pAnim->drop ( );
    
    //pScene->setAmbientLight ( video::SColorf ( 0.050f, 0.050f, 0.050f, 1.0f ) );
    
    m_uType = uType;
    m_pNode = pNode;
    
    return KD_TRUE;
}

scene::IAnimatedMeshSceneNode* CWorld::getNode ( KDvoid )
{
    return m_pNode;
}

KDfloat CWorld::getSpawnDist ( KDvoid )
{
    return l_aConstants [ g_pEnvMgr->getStage ( ) ].fSpawnDist;
}

const core::vector3df& CWorld::getPosition ( KDvoid )
{
    return l_aConstants [ m_uType ].tPoint;
}

