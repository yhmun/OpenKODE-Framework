/* --------------------------------------------------------------------------
 *
 *      File            T08_SpecialFX.cpp
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
#include "T08_SpecialFX.h"

CT08_SpecialFX::CT08_SpecialFX ( KDvoid )
{
	scene::ISceneNode*  pNode = KD_NULL;

	/*
		For our environment, we load a .3ds file. It is a small room I modelled
		with Anim8or and exported into the 3ds format because the Irrlicht
		Engine does not support the .an8 format. I am a very bad 3d graphic
		artist, and so the texture mapping is not very nice in this model.
		Luckily I am a better programmer than artist, and so the Irrlicht
		Engine is able to create a cool texture mapping for me: Just use the
		mesh manipulator and create a planar texture mapping for the mesh. If
		you want to see the mapping I made with Anim8or, uncomment this line. I
		also did not figure out how to set the material right in Anim8or, it
		has a specular light color which I don't really like. I'll switch it
		off too with this code.
	*/

	scene::IAnimatedMesh*  pMesh = m_pScrMgr->getMesh ( "/res/media/room.3ds" );
	if ( pMesh )
	{
		m_pScrMgr->getMeshManipulator ( )->makePlanarTextureMapping ( pMesh->getMesh ( 0 ), 0.004f );

		pNode = m_pScrMgr->addAnimatedMeshSceneNode ( pMesh );
		if ( pNode )
		{
			( (scene::IAnimatedMeshSceneNode*) pNode )->addShadowVolumeSceneNode ( );
			pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/wall.jpg" ) );
			pNode->getMaterial ( 0 ).SpecularColor.set ( 0, 0, 0, 0 );
		}
	}

	/*
		Now, for the first special effect: Animated water. It works like this:
		The WaterSurfaceSceneNode takes a mesh as input and makes it wave like
		a water surface. And if we let this scene node use a nice material like
		the EMT_REFLECTION_2_LAYER, it looks really cool. We are doing this
		with the next few lines of code. As input mesh, we create a hill plane
		mesh, without hills. But any other mesh could be used for this, you
		could even use the room.3ds (which would look really strange) if you
		want to.
	*/

	pMesh = KD_NULL;//m_pScrMgr->addHillPlaneMesh( "myHill",
//		core::dimension2d<f32>(20,20),
//		core::dimension2d<u32>(40,40), 0, 0,
//		core::dimension2d<f32>(0,0),
//		core::dimension2d<f32>(10,10));

	if ( pMesh )
	{
		pNode = m_pScrMgr->addWaterSurfaceSceneNode ( pMesh->getMesh ( 0 ), 3.0f, 300.0f, 30.0f );
		if ( pNode )
		{
			pNode->setPosition ( core::vector3df ( 0, 7, 0 ) );

			pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/stones.jpg" ) );
			pNode->setMaterialTexture ( 1, s_pDriver->getTexture ( "/res/media/water.jpg" ) );

			pNode->setMaterialType ( video::EMT_REFLECTION_2_LAYER );
		}
	}

	/*
		The second special effect is very basic, I bet you saw it already in
		some Irrlicht Engine demos: A transparent billboard combined with a
		dynamic light. We simply create a light scene node, let it fly around,
		and to make it look more cool, we attach a billboard scene node to it.
	*/

	// create light
	pNode = m_pScrMgr->addLightSceneNode 
	(
		0, core::vector3df ( 0, 0, 0 ), video::SColorf ( 1.0f, 0.6f, 0.7f, 1.0f ), 800.0f 
	);
	if ( pNode )
	{
		scene::ISceneNodeAnimator*  pAnim = KD_NULL;
		pAnim = m_pScrMgr->createFlyCircleAnimator ( core::vector3df ( 0, 150, 0 ), 250.0f );
		pNode->addAnimator ( pAnim );
		pAnim->drop ( );
	}

	// attach billboard to light

	pNode = m_pScrMgr->addBillboardSceneNode ( pNode, core::dimension2d<f32> ( 50, 50 ) );
	if ( pNode )
	{
		pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
		pNode->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
		pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/particlewhite.bmp" ) );
	}

	/*
		Next we add a volumetric light node, which adds a glowing fake area light to
		the scene. Like with the billboards and particle systems we also assign a
		texture for the desired effect, though this time we'll use a texture animator
		to create the illusion of a magical glowing area effect.
	*/
	scene::IVolumeLightSceneNode* pNode2 = m_pScrMgr->addVolumeLightSceneNode 
	(
		0,
		-1,
		32,										// Subdivisions on U axis
		32,										// Subdivisions on V axis
		video::SColor ( 0, 255, 255, 255 ),		// foot color
		video::SColor ( 0, 0, 0, 0 )			// tail color
	);

	if ( pNode2 )
	{
		pNode2->setScale ( core::vector3df ( 56.0f, 56.0f, 56.0f ) );
		pNode2->setPosition ( core::vector3df ( -120, 50, 40 ) );

		// load textures for animation
		core::array<video::ITexture*> aTextures;
		for (s32 g=7; g > 0; --g)
		{
			core::stringc  tStr;
			tStr =  "/res/media/portal";
			tStr += g;
			tStr += ".bmp";
			video::ITexture*  pTexture = s_pDriver->getTexture ( tStr.c_str ( ) );
			aTextures.push_back ( pTexture );
		}

		// create texture animator
		scene::ISceneNodeAnimator* pGlow = m_pScrMgr->createTextureAnimator ( aTextures, 150 );

		// add the animator
		pNode2->addAnimator ( pGlow );

		// drop the animator because it was created with a create() function
		pGlow->drop ( );
	}

	/*
		The next special effect is a lot more interesting: A particle system.
		The particle system in the Irrlicht Engine is quite modular and
		extensible, but yet easy to use. There is a particle system scene node
		into which you can put a particle emitter, which makes particles come out
		of nothing. These emitters are quite flexible and usually have lots of
		parameters like direction, amount, and color of the particles they
		create.

		There are different emitters, for example a point emitter which lets
		particles pop out at a fixed point. If the particle emitters available
		in the engine are not enough for you, you can easily create your own
		ones, you'll simply have to create a class derived from the
		IParticleEmitter interface and attach it to the particle system using
		setEmitter(). In this example we create a box particle emitter, which
		creates particles randomly inside a box. The parameters define the box,
		direction of the particles, minimal and maximal new particles per
		second, color, and minimal and maximal lifetime of the particles.

		Because only with emitters particle system would be a little bit
		boring, there are particle affectors which modify particles while
		they fly around. Affectors can be added to a particle system for
		simulating additional effects like gravity or wind.
		The particle affector we use in this example is an affector which
		modifies the color of the particles: It lets them fade out. Like the
		particle emitters, additional particle affectors can also be
		implemented by you, simply derive a class from IParticleAffector and
		add it with addAffector().

		After we set a nice material to the particle system, we have a cool
		looking camp fire. By adjusting material, texture, particle emitter,
		and affector parameters, it is also easily possible to create smoke,
		rain, explosions, snow, and so on.
	*/

	// create a particle system
	scene::IParticleSystemSceneNode*  pParticle = m_pScrMgr->addParticleSystemSceneNode ( false );
	if ( pParticle )
	{
		scene::IParticleEmitter*  pEmitter = pParticle->createBoxEmitter
		(
			core::aabbox3d<f32>(-7,0,-7,7,1,7),	 // emitter size
			core::vector3df(0.0f,0.06f,0.0f),	 // initial direction
			80,100,								 // emit rate
			video::SColor(0,255,255,255),		 // darkest color
			video::SColor(0,255,255,255),		 // brightest color
			800,2000,0,							 // min and max age, angle
			core::dimension2df(10.f,10.f),       // min size
			core::dimension2df(20.f,20.f)        // max size
		);

		if ( pEmitter )
		{
			pParticle->setEmitter ( pEmitter ); // this grabs the emitter
			pEmitter->drop ( );					// so we can drop it here without deleting it
		}

		scene::IParticleAffector* pAffector = pParticle->createFadeOutParticleAffector ( );
		if ( pAffector )
		{
			pParticle->addAffector ( pAffector ); // same goes for the affector
			pAffector->drop ( );
		}

		pParticle->setPosition ( core::vector3df(-70,60,40) );
		pParticle->setScale ( core::vector3df(2,2,2) );
		pParticle->setMaterialFlag ( video::EMF_LIGHTING, false );
//		pParticle->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
		pParticle->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/fire.bmp" ) );
		pParticle->setMaterialType ( video::EMT_TRANSPARENT_VERTEX_ALPHA );
	}

	/*
		As our last special effect, we want a dynamic shadow be casted from an
		animated character. For this we load a DirectX .x model and place it
		into our world. For creating the shadow, we simply need to call
		addShadowVolumeSceneNode(). The color of shadows is only adjustable
		globally for all shadows, by calling ISceneManager::setShadowColor().
		Voila, here is our dynamic shadow.

		Because the character is a little bit too small for this scene, we make
		it bigger using setScale(). And because the character is lighted by a
		dynamic light, we need to normalize the normals to make the lighting on
		it correct. This is always necessary if the scale of a dynamic lighted
		model is not (1,1,1). Otherwise it would get too dark or too bright
		because the normals will be scaled too.
	*/

	// add animated character
	pMesh = m_pScrMgr->getMesh ( "/res/media/dwarf.x" );
	scene::IAnimatedMeshSceneNode* pAnode = KD_NULL;

	pAnode = m_pScrMgr->addAnimatedMeshSceneNode ( pMesh );
	pAnode->setPosition ( core::vector3df ( -50, 20, -60 ) );
	pAnode->setAnimationSpeed ( 15 );

	// add shadow
	pAnode->addShadowVolumeSceneNode ( );
	m_pScrMgr->setShadowColor ( video::SColor ( 150, 0, 0, 0 ) );

	// make the model a little bit bigger and normalize its normals
	// because of the scaling, for correct lighting
	pAnode->setScale ( core::vector3df ( 2, 2, 2 ) );
	pAnode->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );

	/*
		Finally we simply have to draw everything, that's all.
	*/

	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNodeFPS ( );
	pCamera->setPosition ( core::vector3df ( -100, 150, -250 ) );

	// disable mouse cursor
	s_pDevice->getCursorControl ( )->setVisible ( false );
}

CT08_SpecialFX::~CT08_SpecialFX ( KDvoid )
{
	
}


const wchar_t* CT08_SpecialFX::getTitle ( KDvoid )
{
	return L"08. SpecialFX";
}