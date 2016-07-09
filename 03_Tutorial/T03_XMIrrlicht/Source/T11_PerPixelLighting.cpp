/* --------------------------------------------------------------------------
 *
 *      File            T11_PerPixelLighting.cpp
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
#include "T11_PerPixelLighting.h"

CT11_PerPixelLighting::CT11_PerPixelLighting ( KDvoid )
{
	s_pDriver->setTextureCreationFlag ( video::ETCF_ALWAYS_32_BIT, true );

	// add camera
	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNodeFPS ( );
	pCamera->setPosition ( core::vector3df ( -200, 200, -200 ) );

	// disable mouse cursor
	s_pDevice->getCursorControl ( )->setVisible ( false );

	/*
		Because we want the whole scene to look a little bit scarier, we add
		some fog to it. This is done by a call to IVideoDriver::setFog(). There
		you can set various fog settings. In this example, we use pixel fog,
		because it will work well with the materials we'll use in this example.
		Please note that you will have to set the material flag EMF_FOG_ENABLE
		to 'true' in every scene node which should be affected by this fog.
	*/
	s_pDriver->setFog
	(
		video::SColor ( 0, 138, 125, 81 ),
		video::EFT_FOG_LINEAR, 
		250, 1000, .003f, true, false
	);

	/*
		To be able to display something interesting, we load a mesh from a .3ds
		file which is a room I modeled with anim8or. It is the same room as
		from the specialFX example. Maybe you remember from that tutorial, I am
		no good modeler at all and so I totally messed up the texture mapping
		in this model, but we can simply repair it with the
		IMeshManipulator::makePlanarTextureMapping() method.
	*/

	scene::IAnimatedMesh*  pRoomMesh = m_pScrMgr->getMesh ( "/res/media/room.3ds" );
	scene::ISceneNode*  pRoom  = KD_NULL;
	scene::ISceneNode*  pEarth = KD_NULL;

	if ( pRoomMesh )
	{
		// The Room mesh doesn't have proper Texture Mapping on the
		// floor, so we can recreate them on runtime
		m_pScrMgr->getMeshManipulator ( )->makePlanarTextureMapping ( pRoomMesh->getMesh ( 0 ), 0.003f );

		/*
			Now for the first exciting thing: If we successfully loaded the
			mesh we need to apply textures to it. Because we want this room
			to be displayed with a very cool material, we have to do a
			little bit more than just set the textures. Instead of only
			loading a color map as usual, we also load a height map which
			is simply a grayscale texture. From this height map, we create
			a normal map which we will set as second texture of the room.
			If you already have a normal map, you could directly set it,
			but I simply didn't find a nice normal map for this texture.
			The normal map texture is being generated by the
			makeNormalMapTexture method of the VideoDriver. The second
			parameter specifies the height of the heightmap. If you set it
			to a bigger value, the map will look more rocky.
		*/

		video::ITexture*  pNormalMap = s_pDriver->getTexture ( "/res/media/rockwall_height.bmp" );

		if ( pNormalMap )
		{
			s_pDriver->makeNormalMapTexture ( pNormalMap, 9.0f );
		}
/*
		// The Normal Map and the displacement map/height map in the alpha channel
		video::ITexture*  pNormalMap = s_pDriver->getTexture ( "/res/media/rockwall_NRM.tga" );
*/
		/*
			But just setting color and normal map is not everything. The
			material we want to use needs some additional informations per
			vertex like tangents and binormals. Because we are too lazy to
			calculate that information now, we let Irrlicht do this for us.
			That's why we call IMeshManipulator::createMeshWithTangents().
			It creates a mesh copy with tangents and binormals from another
			mesh. After we've done that, we simply create a standard
			mesh scene node with this mesh copy, set color and normal map
			and adjust some other material settings. Note that we set
			EMF_FOG_ENABLE to true to enable fog in the room.
		*/

		scene::IMesh*  pTangentMesh = m_pScrMgr->getMeshManipulator ( )->createMeshWithTangents ( pRoomMesh->getMesh ( 0 ) );

		pRoom = m_pScrMgr->addMeshSceneNode ( pTangentMesh );
		pRoom->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/rockwall.jpg" ) );
		pRoom->setMaterialTexture ( 1, pNormalMap );

		// Stones don't glitter..
		pRoom->getMaterial ( 0 ).SpecularColor.set ( 0,0,0,0 );
		pRoom->getMaterial ( 0 ).Shininess = 0.f;

		pRoom->setMaterialFlag ( video::EMF_FOG_ENABLE, true );
		pRoom->setMaterialType ( video::EMT_PARALLAX_MAP_SOLID );
		// adjust height for parallax effect
		pRoom->getMaterial ( 0 ).MaterialTypeParam = 1.f / 64.f;

		// drop mesh because we created it with a create.. call.
		pTangentMesh->drop ( );
	}

	/*
	After we've created a room shaded by per pixel lighting, we add a
	sphere into it with the same material, but we'll make it transparent.
	In addition, because the sphere looks somehow like a familiar planet,
	we make it rotate. The procedure is similar as before. The difference
	is that we are loading the mesh from an .x file which already contains
	a color map so we do not need to load it manually. But the sphere is a
	little bit too small for our needs, so we scale it by the factor 50.
	*/

	// add earth sphere

	scene::IAnimatedMesh*  pEarthMesh = m_pScrMgr->getMesh ( "/res/media/earth.x" );
	if ( pEarthMesh )
	{
		//perform various task with the mesh manipulator
		scene::IMeshManipulator*  pManipulator = m_pScrMgr->getMeshManipulator ( );

		// create mesh copy with tangent informations from original earth.x mesh
		scene::IMesh*  pTangentSphereMesh = pManipulator->createMeshWithTangents ( pEarthMesh->getMesh ( 0 ) );

		// set the alpha value of all vertices to 200
		pManipulator->setVertexColorAlpha ( pTangentSphereMesh, 200 );

		// scale the mesh by factor 50
		core::matrix4  tMatrix;
		tMatrix.setScale ( core::vector3df ( 50, 50, 50 ) );
		pManipulator->transform ( pTangentSphereMesh, tMatrix );

		pEarth = m_pScrMgr->addMeshSceneNode ( pTangentSphereMesh );
		pEarth->setPosition ( core::vector3df ( -70, 130, 45 ) );

		// load heightmap, create normal map from it and set it
		video::ITexture*  pEarthNormalMap = s_pDriver->getTexture ( "/res/media/earthbump.jpg" );
		if ( pEarthNormalMap )
		{
			s_pDriver->makeNormalMapTexture ( pEarthNormalMap, 20.0f );
			pEarth->setMaterialTexture ( 1, pEarthNormalMap );
			pEarth->setMaterialType ( video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA );
		}

		// adjust material settings
		pEarth->setMaterialFlag ( video::EMF_FOG_ENABLE, true );

		// add rotation animator
		scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createRotationAnimator ( core::vector3df ( 0, 0.1f, 0 ) );
		pEarth->addAnimator ( pAnim );
		pAnim->drop ( );

		// drop mesh because we created it with a create.. call.
		pTangentSphereMesh->drop ( );
	}

	/*
		Per pixel lighted materials only look cool when there are moving
		lights. So we add some. And because moving lights alone are so boring,
		we add billboards to them, and a whole particle system to one of them.
		We start with the first light which is red and has only the billboard
		attached.
	*/

	// add light 1 (more green)
	scene::ILightSceneNode*  pLight1 = m_pScrMgr->addLightSceneNode
	(
		0, 
		core::vector3df ( 0, 0, 0 ),
		video::SColorf ( 0.5f, 1.0f, 0.5f, 0.0f ),
		800.0f
	);

	pLight1->setDebugDataVisible ( scene::EDS_BBOX );


	// add fly circle animator to light 1
	scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createFlyCircleAnimator 
	(
		core::vector3df ( 50, 300, 0 ), 190.0f, -0.003f
	);
	pLight1->addAnimator ( pAnim );
	pAnim->drop ( );

	// attach billboard to the light
	scene::ISceneNode*  pBill = m_pScrMgr->addBillboardSceneNode ( pLight1, core::dimension2d<f32> ( 60, 60 ) );

	pBill->setMaterialFlag ( video::EMF_LIGHTING, false );
	pBill->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
	pBill->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
	pBill->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/particlegreen.jpg" ) );

	/*
		Now the same again, with the second light. The difference is that we
		add a particle system to it too. And because the light moves, the
		particles of the particlesystem will follow. If you want to know more
		about how particle systems are created in Irrlicht, take a look at the
		specialFx example. Maybe you will have noticed that we only add 2
		lights, this has a simple reason: The low end version of this material
		was written in ps1.1 and vs1.1, which doesn't allow more lights. You
		could add a third light to the scene, but it won't be used to shade the
		walls. But of course, this will change in future versions of Irrlicht
		where higher versions of pixel/vertex shaders will be implemented too.
	*/

	// add light 2 (red)
	scene::ISceneNode*  pLight2 = m_pScrMgr->addLightSceneNode
	(
		0,
		core::vector3df ( 0, 0, 0 ),
		video::SColorf (1.0f, 0.2f, 0.2f, 0.0f ),
		800.0f
	);

	// add fly circle animator to light 2
	pAnim = m_pScrMgr->createFlyCircleAnimator
	(
		core::vector3df(0,150,0),
		200.0f,
		0.001f, 
		core::vector3df ( 0.2f, 0.9f, 0.f ) 
	);
	pLight2->addAnimator ( pAnim );
	pAnim->drop ( );

	// attach billboard to light
	pBill = m_pScrMgr->addBillboardSceneNode ( pLight2, core::dimension2d<f32> ( 120, 120 ) );
	pBill->setMaterialFlag ( video::EMF_LIGHTING, false );
	pBill->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
	pBill->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
	pBill->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/particlered.bmp" ) );

	// add particle system
	scene::IParticleSystemSceneNode*  pParticle = m_pScrMgr->addParticleSystemSceneNode ( false, pLight2 );

	// create and set emitter
	scene::IParticleEmitter*  pEmitter = pParticle->createBoxEmitter
	(
		core::aabbox3d<f32> ( -3, 0, -3, 3, 1, 3),
		core::vector3df ( 0.0f, 0.03f, 0.0f ),
		80, 100,
		video::SColor ( 10, 255, 255, 255 ),
		video::SColor ( 10, 255, 255, 255 ),
		400, 1100
	);
	pEmitter->setMinStartSize ( core::dimension2d<f32> ( 30.0f, 40.0f ) );
	pEmitter->setMaxStartSize ( core::dimension2d<f32> ( 30.0f, 40.0f ) );

	pParticle->setEmitter ( pEmitter );
	pEmitter->drop ( );

	// create and set affector
	scene::IParticleAffector*  pAffector = pParticle->createFadeOutParticleAffector ( );
	pParticle->addAffector ( pAffector );
	pAffector->drop ( );

	// adjust some material settings
	pParticle->setMaterialFlag ( video::EMF_LIGHTING, false );
	pParticle->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
	pParticle->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/fireball.bmp" ) );
	pParticle->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );

	m_pRoom  = pRoom;
	m_pEarth = pEarth;

	// set a nicer font
	gui::IGUISkin*  pSkin = m_pGuiEnv->getSkin ( );
	gui::IGUIFont*  pFont = m_pGuiEnv->getFont ( "/res/media/fonthaettenschweiler.bmp" );
	if ( pFont )
	{
		pSkin->setFont ( pFont );
	}

	// add window and listbox
	gui::IGUIWindow*  pWindow = m_pGuiEnv->addWindow
	(
		core::rect<s32> ( 500, 305, 670, 400 ),
		false, L"Use 'E' + 'R' to change"
	);

	m_pListbox = m_pGuiEnv->addListBox
	(
		core::rect<s32> ( 2, 22, 165, 88 ), pWindow
	);

	m_pListbox->addItem ( L"Diffuse" );
	m_pListbox->addItem ( L"Bump mapping" );
	m_pListbox->addItem ( L"Parallax mapping" );
	m_pListbox->setSelected ( 1 );

	// create problem text
	m_pProblem = m_pGuiEnv->addStaticText
	(
		L"Your hardware or this renderer is not able to use the "\
		L"needed shaders for this material. Using fall back materials.",
		core::rect<s32> ( 150, 20, 470, 80 )
	);
	m_pProblem->setOverrideColor ( video::SColor ( 100, 255, 255, 255 ) );

	// set start material (prefer parallax mapping if available)
	video::IMaterialRenderer*  pRenderer = s_pDriver->getMaterialRenderer ( video::EMT_PARALLAX_MAP_SOLID );
	if ( pRenderer && pRenderer->getRenderCapability ( ) == 0 )
	{
		m_pListbox->setSelected ( 2 );
	}

	// set the material which is selected in the listbox
	setMaterial ( );
}

CT11_PerPixelLighting::~CT11_PerPixelLighting ( KDvoid )
{
	
}

const wchar_t* CT11_PerPixelLighting::getTitle ( KDvoid )
{
	return L"11. PerPixelLighting";
}

bool CT11_PerPixelLighting::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// check if user presses the key 'E' or 'R'
	if ( event.EventType == irr::EET_KEY_INPUT_EVENT &&
		!event.KeyInput.PressedDown && m_pRoom && m_pListbox )
	{
		// change selected item in listbox
		KDint  nSel = m_pListbox->getSelected ( );

		if ( event.KeyInput.Key == irr::KEY_KEY_R )
		{
			++nSel;
		}
		else if ( event.KeyInput.Key == irr::KEY_KEY_E )
		{
			--nSel;
		}
		else
		{
			return false;
		}

		if ( nSel > 2 ) nSel = 0;
		if ( nSel < 0 ) nSel = 2;
		m_pListbox->setSelected ( nSel );

		// set the material which is selected in the listbox
		setMaterial ( );
	}

	return false;
}

// sets the material of the room mesh the the one set in the
// list box.
void CT11_PerPixelLighting::setMaterial ( )
{
	video::E_MATERIAL_TYPE  eType = video::EMT_SOLID;

	// change material setting
	switch ( m_pListbox->getSelected ( ) )
	{
		case 0 : eType = video::EMT_SOLID;				break;
		case 1 : eType = video::EMT_NORMAL_MAP_SOLID;	break;
		case 2 : eType = video::EMT_PARALLAX_MAP_SOLID;	break;
	}

	m_pRoom->setMaterialType ( eType );

	// change material setting
	switch ( m_pListbox->getSelected ( ) )
	{
		case 0 : eType = video::EMT_TRANSPARENT_VERTEX_ALPHA;				break;
		case 1 : eType = video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA;	break;
		case 2 : eType = video::EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA;	break;
	}

	m_pEarth->setMaterialType ( eType );

	/*
		We need to add a warning if the materials will not be able to
		be displayed 100% correctly. This is no problem, they will be
		renderered using fall back materials, but at least the user
		should know that it would look better on better hardware. We
		simply check if the material renderer is able to draw at full
		quality on the current hardware. The
		IMaterialRenderer::getRenderCapability() returns 0 if this is
		the case.
	*/
	video::IMaterialRenderer*  pRenderer = s_pDriver->getMaterialRenderer ( eType );

	// display some problem text when problem
	if ( !pRenderer || pRenderer->getRenderCapability ( ) != 0 )
	{
		m_pProblem->setVisible ( true );
	}
	else
	{
		m_pProblem->setVisible ( false );
	}
}