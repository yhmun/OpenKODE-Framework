/* --------------------------------------------------------------------------
 *
 *      File            T13_RenderToTexture.cpp
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
#include "T13_RenderToTexture.h"

CT13_RenderToTexture::CT13_RenderToTexture ( KDvoid )
{
	/*
		Now, we load an animated mesh to be displayed. As in most examples,
		we'll take the fairy md2 model. The difference here: We set the
		shininess of the model to a value other than 0 which is the default
		value. This enables specular highlights on the model if dynamic
		lighting is on. The value influences the size of the highlights.
	*/

	// load and display animated fairy mesh

	scene::IAnimatedMeshSceneNode*  pFairy = m_pScrMgr->addAnimatedMeshSceneNode
	( 
		m_pScrMgr->getMesh ( "/res/media/faerie.md2" ) 
	);

	if ( pFairy )
	{
		pFairy->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/faerie2.bmp" ) ); // set diffuse texture
		pFairy->setMaterialFlag ( video::EMF_LIGHTING, true ); // enable dynamic lighting
		pFairy->getMaterial ( 0 ).Shininess = 20.0f; // set size of specular highlights
		pFairy->setPosition ( core::vector3df ( -10, 0, -100 ) );
		pFairy->setMD2Animation ( scene::EMAT_STAND );
	}
	
	/*
		To make specular highlights appear on the model, we need a dynamic
		light in the scene. We add one directly in vicinity of the model. In
		addition, to make the model not that dark, we set the ambient light to
		gray.
	*/

	// add white light
	m_pScrMgr->addLightSceneNode ( 0, core::vector3df ( -15, 5, -105 ), video::SColorf ( 1.0f, 1.0f, 1.0f ) );

	// set ambient light
	m_pScrMgr->setAmbientLight ( video::SColor ( 0, 60, 60, 60 ) );
	
	/*
		The next is just some standard stuff: Add a test cube and let it rotate
		to make the scene more interesting. The user defined camera and cursor
		setup is made later on, right before the render loop.
	*/

	// create test cube
	scene::ISceneNode*  pTest = m_pScrMgr->addCubeSceneNode ( 60 );

	// let the cube rotate and set some light settings
	scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createRotationAnimator ( core::vector3df ( 0.3f, 0.3f,0 ) );

	pTest->setPosition ( core::vector3df ( -100, 0, -100 ) );
	pTest->setMaterialFlag ( video::EMF_LIGHTING, false ); // disable dynamic lighting
	pTest->addAnimator ( pAnim );
	pAnim->drop ( );

	// set window caption
	s_pDevice->setWindowCaption ( L"Irrlicht Engine - Render to Texture and Specular Highlights example" );
	
	/*
		To test out the render to texture feature, we need a render target
		texture. These are not like standard textures, but need to be created
		first. To create one, we call IVideoDriver::addRenderTargetTexture ( )
		and specify the size of the texture. Please don't use sizes bigger than
		the frame buffer for this, because the render target shares the zbuffer
		with the frame buffer.
		Because we want to render the scene not from the user camera into the
		texture, we add another fixed camera to the scene. But before we do all
		this, we check if the current running driver is able to render to
		textures. If it is not, we simply display a warning text.
	*/

	// create render target
	video::ITexture*  pRt = 0;
	scene::ICameraSceneNode*  pFixedCam = 0;
	
	if ( s_pDriver->queryFeature ( video::EVDF_RENDER_TO_TARGET ) )
	{
		pRt = s_pDriver->addRenderTargetTexture ( core::dimension2d<u32> ( 256, 256 ), "RTT1" );
		pTest->setMaterialTexture ( 0, pRt ); // set material of cube to render target

		// add fixed camera
		pFixedCam = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df ( 10, 10, -80 ), core::vector3df ( -10,10,-100 ) );
	}
	else
	{
		// create problem text
		gui::IGUISkin*  pSkin = m_pGuiEnv->getSkin ( );
		gui::IGUIFont*  pFont = m_pGuiEnv->getFont ( "/res/media/fonthaettenschweiler.bmp" );
		if ( pFont )
		{
			pSkin->setFont ( pFont );
		}

		gui::IGUIStaticText*  pText = m_pGuiEnv->addStaticText 
		( 
			L"Your hardware or this renderer is not able to use the "\
			L"render to texture feature. RTT Disabled.",
			core::rect<s32> ( 150, 20, 470, 60 ) 
		);

		pText->setOverrideColor ( video::SColor ( 100, 255, 255, 255 ) );
	}
	
	// add fps camera
	scene::ICameraSceneNode*  pFpsCam = m_pScrMgr->addCameraSceneNodeFPS ( );
	pFpsCam->setPosition ( core::vector3df ( -100, 0, -200 ) );

	// disable mouse cursor
	s_pDevice->getCursorControl ( )->setVisible ( false );

	m_pRt	  = pRt;
	m_pTest   = pTest;
	m_pFixCam = pFixedCam;
	m_pFpsCam = pFpsCam;
}

CT13_RenderToTexture::~CT13_RenderToTexture ( KDvoid )
{
	
}

KDvoid CT13_RenderToTexture::Draw ( KDvoid )
{
	if ( m_pRt )
	{
		// draw scene into render target
		
		// set render target texture
		s_pDriver->setRenderTarget ( m_pRt, true, true, video::SColor ( 0, 0, 0, 255 ) );

		// make cube invisible and set fixed camera as active camera
		m_pTest->setVisible ( false );
		m_pScrMgr->setActiveCamera ( m_pFixCam );

		// draw whole scene into render buffer
		m_pScrMgr->drawAll ( );

		// set back old render target
		// The buffer might have been distorted, so clear it
		s_pDriver->setRenderTarget ( 0, true, true, 0 );

		// make the cube visible and set the user controlled camera as active one
		m_pTest->setVisible ( true );
		m_pScrMgr->setActiveCamera ( m_pFpsCam );
	}

	CTutorial::Draw ( );
}

const wchar_t* CT13_RenderToTexture::getTitle ( KDvoid )
{
	return L"13. RenderToTexture";
}