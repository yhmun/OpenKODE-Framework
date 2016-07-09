/* --------------------------------------------------------------------------
 *
 *      File            T10_Shaders.cpp
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
#include "T10_Shaders.h"

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
	public:

		virtual void OnSetConstants ( video::IMaterialRendererServices* pServices, s32 nUserData )
		{
			irr::IrrlichtDevice*  pDevice = CTutorial::s_pDevice;
			video::IVideoDriver*  pDriver = pServices->getVideoDriver ( );

			// set inverted world matrix
			// if we are using highlevel shaders ( the user can select this when
			// starting the program ), we must set the constants by name.
			core::matrix4  tInvWorld = pDriver->getTransform ( video::ETS_WORLD );
			tInvWorld.makeInverse ( );

			pServices->setVertexShaderConstant ( "mInvWorld", tInvWorld.pointer ( ), 16 );

			// set clip matrix
			core::matrix4  tWorldViewProj;
			tWorldViewProj  = pDriver->getTransform ( video::ETS_PROJECTION );
			tWorldViewProj *= pDriver->getTransform ( video::ETS_VIEW );
			tWorldViewProj *= pDriver->getTransform ( video::ETS_WORLD ); 

			pServices->setVertexShaderConstant ( "mWorldViewProj", tWorldViewProj.pointer ( ), 16 );

			// set camera position
			core::vector3df  tPos = pDevice->getSceneManager ( )->getActiveCamera ( )->getAbsolutePosition ( );
			pServices->setVertexShaderConstant ( "mLightPos", reinterpret_cast<f32*> ( &tPos ), 3 );
	
			// set light color
		 	video::SColorf  tCol ( 0.0f,1.0f,1.0f,0.0f );
			pServices->setVertexShaderConstant ( "mLightColor", reinterpret_cast<f32*> ( &tCol ), 4 );

			// set transposed world matrix
			core::matrix4  tWorld = pDriver->getTransform ( video::ETS_WORLD );
			tWorld = tWorld.getTransposed ( );

			pServices->setVertexShaderConstant ( "mTransWorld", tWorld.pointer ( ), 16 );
		}
};

CT10_Shaders::CT10_Shaders ( KDvoid )
{
	/*
		Now lets create the new materials. As you maybe know from previous
		examples, a material type in the Irrlicht engine is set by simply
		changing the MaterialType value in the SMaterial struct. And this value
		is just a simple 32 bit value, like video::EMT_SOLID. So we only need
		the engine to create a new value for us which we can set there. To do
		this, we get a pointer to the IGPUProgrammingServices and call
		addShaderMaterialFromFiles ( ), which returns such a new 32 bit value.
		That's all.

		The parameters to this method are the following: First, the names of
		the files containing the code of the vertex and the pixel shader. If
		you would use addShaderMaterial ( ) instead, you would not need file
		names, then you could write the code of the shader directly as string.
		The following parameter is a pointer to the IShaderConstantSetCallBack
		class we wrote at the beginning of this tutorial. If you don't want to
		set constants, set this to 0. The last paramter tells the engine which
		material it should use as base material.

		To demonstrate this, we create two materials with a different base
		material, one with EMT_SOLID and one with EMT_TRANSPARENT_ADD_COLOR.
	*/

	video::IGPUProgrammingServices*  pGpu = s_pDriver->getGPUProgrammingServices ( );
	s32  nNewMaterialType1 = 0;
	s32  nNewMaterialType2 = 0;

	MyShaderCallBack*  pMc = new MyShaderCallBack ( );

	// create the shaders depending on if the user wanted high level
	// or low level shaders:

	// create material from high level shaders ( hlsl or glsl )
	nNewMaterialType1 = pGpu->addHighLevelShaderMaterialFromFiles
	( 
		"", "vertexMain", video::EVST_VS_1_1,
		"", "pixelMain", video::EPST_PS_1_1,
		pMc, video::EMT_SOLID
	);

	nNewMaterialType2 = pGpu->addHighLevelShaderMaterialFromFiles
	( 
		"", "vertexMain", video::EVST_VS_1_1,
		"", "pixelMain", video::EPST_PS_1_1,
		pMc, video::EMT_TRANSPARENT_ADD_COLOR
	);

	pMc->drop ( );

	/*
		Now it's time for testing the materials. We create a test cube and set
		the material we created. In addition, we add a text scene node to the
		cube and a rotation animator to make it look more interesting and
		important.
	*/

	// create test scene node 1, with the new created material type 1
	scene::ISceneNode*  pNode = m_pScrMgr->addCubeSceneNode ( 50 );
	pNode->setPosition ( core::vector3df ( 0,0,0 ) );
	pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/wall.bmp" ) );
	pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
	pNode->setMaterialType ( (video::E_MATERIAL_TYPE) nNewMaterialType1 );

	m_pScrMgr->addTextSceneNode 
	(
		m_pGuiEnv->getBuiltInFont ( ),
		L"PS & VS & EMT_SOLID",
		video::SColor ( 255, 255, 255, 255 ), 
		pNode
	);

	scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createRotationAnimator ( core::vector3df ( 0, 0.3f, 0 ) );
	pNode->addAnimator ( pAnim );
	pAnim->drop ( );

	/*
		Same for the second cube, but with the second material we created.
	*/

	// create test scene node 2, with the new created material type 2

	pNode = m_pScrMgr->addCubeSceneNode ( 50 );
	pNode->setPosition ( core::vector3df ( 0, -10, 50 ) );
	pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/wall.bmp" ) );
	pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
	pNode->setMaterialFlag ( video::EMF_BLEND_OPERATION, true );
	pNode->setMaterialType ( (video::E_MATERIAL_TYPE) nNewMaterialType2 );

	m_pScrMgr->addTextSceneNode 
	(
		m_pGuiEnv->getBuiltInFont ( ),
		L"PS & VS & EMT_TRANSPARENT",
		video::SColor ( 255, 255, 255, 255 ), 
		pNode
	);

	pAnim = m_pScrMgr->createRotationAnimator ( core::vector3df ( 0, 0.3f, 0 ) );
	pNode->addAnimator ( pAnim );
	pAnim->drop ( );

	/*
		Then we add a third cube without a shader on it, to be able to compare
		the cubes.
	*/

	// add a scene node with no shader
	pNode = m_pScrMgr->addCubeSceneNode ( 50 );
	pNode->setPosition ( core::vector3df ( 0,50,25 ) );
	pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/wall.bmp" ) );
	pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
	m_pScrMgr->addTextSceneNode 
	(
		m_pGuiEnv->getBuiltInFont ( ),
		L"NO SHADER",
		video::SColor ( 255,255,255,255 ),
		pNode 
	);

	/*
		And last, we add a skybox and a user controlled camera to the scene.
		For the skybox textures, we disable mipmap generation, because we don't
		need mipmaps on it.
	*/

	// add a nice skybox

	s_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, false );

	m_pScrMgr->addSkyBoxSceneNode 
	( 
		s_pDriver->getTexture ( "/res/media/irrlicht2_up.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_dn.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_lf.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_rt.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_ft.jpg" ),
		s_pDriver->getTexture ( "/res/media/irrlicht2_bk.jpg" ) 
	);

	s_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

	// add a camera and disable the mouse cursor
	scene::ICameraSceneNode*  pCam = m_pScrMgr->addCameraSceneNodeFPS ( );
	pCam->setPosition ( core::vector3df ( -100, 50, 100 ) );
	pCam->setTarget ( core::vector3df ( 0, 0, 0 ) );

	s_pDevice->getCursorControl ( )->setVisible ( false );
}

CT10_Shaders::~CT10_Shaders ( KDvoid )
{
	
}

const wchar_t* CT10_Shaders::getTitle ( KDvoid )
{
	return L"10. Shaders";
}
