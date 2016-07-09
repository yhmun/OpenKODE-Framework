/* --------------------------------------------------------------------------
 *
 *      File            T12_TerrainRendering.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright ( C ) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "T12_TerrainRendering.h"

CT12_TerrainRendering::CT12_TerrainRendering ( KDvoid )
{
	s_pDriver->setTextureCreationFlag ( video::ETCF_ALWAYS_32_BIT, true );

	//set other font
	m_pGuiEnv->getSkin ( )->setFont ( m_pGuiEnv->getFont ( "/res/media/fontlucida.png" ) );

	// add some help text
	m_pGuiEnv->addStaticText 
	( 
		L"Press 'W' to change wireframe mode\nPress 'D' to toggle detail map\nPress 'S' to toggle skybox/skydome",
		core::rect<s32> ( 10, 10, 250, 64 ), true, true, 0, -1, true 
	);

	// add camera
	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNodeFPS ( 0,100.0f,1.2f );

	pCamera->setPosition ( core::vector3df ( 2700 * 2, 255 * 2, 2600 * 2 ) );
	pCamera->setTarget   ( core::vector3df ( 2397 * 2, 343 * 2, 2700 * 2 ) );
	pCamera->setFarValue ( 42000.0f );

	// disable mouse cursor
	s_pDevice->getCursorControl ( )->setVisible ( false );

	/*
		Here comes the terrain renderer scene node: We add it just like any
		other scene node to the scene using
		ISceneManager::addTerrainSceneNode ( ). The only parameter we use is a
		file name to the heightmap we use. A heightmap is simply a gray scale
		texture. The terrain renderer loads it and creates the 3D terrain from
		it.

		To make the terrain look more big, we change the scale factor of
		it to ( 40, 4.4, 40 ). Because we don't have any dynamic lights in the
		scene, we switch off the lighting, and we set the file
		terrain-texture.jpg as texture for the terrain and detailmap3.jpg as
		second texture, called detail map. At last, we set the scale values for
		the texture: The first texture will be repeated only one time over the
		whole terrain, and the second one ( detail map ) 20 times.
	*/

	// add terrain scene node
	scene::ITerrainSceneNode*  pTerrain = m_pScrMgr->addTerrainSceneNode 
	( 
		"/res/media/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df ( 0.f, 0.f, 0.f ),		// position
		core::vector3df ( 0.f, 0.f, 0.f ),		// rotation
		core::vector3df ( 40.f, 4.4f, 40.f ),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
	);

	pTerrain->setMaterialFlag ( video::EMF_LIGHTING, false );
	pTerrain->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/terrain-texture.jpg" ) );
	pTerrain->setMaterialTexture ( 1, s_pDriver->getTexture ( "/res/media/detailmap3.jpg" ) );
	pTerrain->setMaterialType ( video::EMT_DETAIL_MAP );
	pTerrain->scaleTexture ( 1.0f, 20.0f );

	/*
		To be able to do collision with the terrain, we create a triangle selector.
		If you want to know what triangle selectors do, just take a look into the
		collision tutorial. The terrain triangle selector works together with the
		terrain. To demonstrate this, we create a collision response animator
		and attach it to the camera, so that the camera will not be able to fly
		through the terrain.
	*/

	// create triangle selector for the terrain	
	scene::ITriangleSelector*  pSelector = m_pScrMgr->createTerrainTriangleSelector ( pTerrain, 0 );
	pTerrain->setTriangleSelector ( pSelector );

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createCollisionResponseAnimator
	( 
		pSelector, pCamera, core::vector3df ( 60,100,60 ),
		core::vector3df ( 0,0,0 ),
		core::vector3df ( 0,50,0 ) 
	);
	pSelector->drop ( );
	pCamera->addAnimator ( pAnim );
	pAnim->drop ( );

	/* If you need access to the terrain data you can also do this directly via the following code fragment.
	*/
	scene::CDynamicMeshBuffer*  pBuffer = new scene::CDynamicMeshBuffer ( video::EVT_2TCOORDS, video::EIT_16BIT );
	pTerrain->getMeshBufferForLOD ( *pBuffer, 0 );
	//video::S3DVertex2TCoords*  pData = (video::S3DVertex2TCoords*)
        pBuffer->getVertexBuffer ( ).getData ( );
	// Work on data or get the IndexBuffer with a similar call.
	pBuffer->drop ( ); // When done drop the buffer again.

	/*
		To make the user be able to switch between normal and wireframe mode,
		we create an instance of the event reciever from above and let Irrlicht
		know about it. In addition, we add the skybox which we already used in
		lots of Irrlicht examples and a skydome, which is shown mutually
		exclusive with the skybox by pressing 'S'.
	*/

	// create skybox and skydome
	s_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, false );

	scene::ISceneNode*  pSkybox = m_pScrMgr->addSkyBoxSceneNode 
	( 
		s_pDriver->getTexture ( "/res/media/irrlicht2_up.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_dn.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_lf.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_rt.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_ft.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_bk.jpg" ) 
	);
	scene::ISceneNode*  pSkydome = m_pScrMgr->addSkyDomeSceneNode
	(
		s_pDriver->getTexture ( "/res/media/skydome.jpg" ), 16, 8, 0.95f, 2.0f 
	);

	s_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

	m_pTerrain	 = pTerrain;
	m_pSkybox	 = pSkybox;
	m_pSkydome	 = pSkydome;
	m_bShowBox	 = true;
	m_bShowDebug = false;
}

CT12_TerrainRendering::~CT12_TerrainRendering ( KDvoid )
{
	
}

const wchar_t* CT12_TerrainRendering::getTitle ( KDvoid )
{
	return L"12. TerrainRendering";
}

bool CT12_TerrainRendering::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// check if user presses the key 'W' or 'D'
	if ( event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown )
	{
		switch ( event.KeyInput.Key )
		{
			case irr::KEY_KEY_W :		// switch wire frame mode
				m_pTerrain->setMaterialFlag ( video::EMF_WIREFRAME, !m_pTerrain->getMaterial ( 0 ).Wireframe );
				m_pTerrain->setMaterialFlag ( video::EMF_POINTCLOUD, false );
				return true;
			case irr::KEY_KEY_P :		// switch wire frame mode
				m_pTerrain->setMaterialFlag ( video::EMF_POINTCLOUD, !m_pTerrain->getMaterial ( 0 ).PointCloud );
				m_pTerrain->setMaterialFlag ( video::EMF_WIREFRAME, false );
				return true;
			case irr::KEY_KEY_D :		// toggle detail map
				m_pTerrain->setMaterialType ( m_pTerrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
												video::EMT_DETAIL_MAP : video::EMT_SOLID );
				return true;

			case irr::KEY_KEY_S :		// toggle skies
				m_bShowBox = !m_bShowBox;
				m_pSkybox->setVisible ( m_bShowBox );
				m_pSkydome->setVisible ( !m_bShowBox );
				return true;

			case irr::KEY_KEY_X :		// toggle debug information
				m_bShowDebug = !m_bShowDebug;
				m_pTerrain->setDebugDataVisible ( m_bShowDebug ? scene::EDS_BBOX_ALL:scene::EDS_OFF );
				return true;
			
			default :
				break;
		}
	}

	return false;
}