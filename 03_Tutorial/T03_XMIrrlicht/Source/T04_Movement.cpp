/* --------------------------------------------------------------------------
 *
 *      File            T04_Movement.cpp
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
#include "T04_Movement.h"

CT04_Movement::CT04_Movement ( KDvoid )
{
	for ( u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i )
	{
		m_bKeyIsDown [ i ] = false;
	}

	/*
		Create the node which will be moved with the WSAD keys. We create a
		sphere node, which is a built-in geometry primitive. We place the node
		at (0,0,30) and assign a texture to it to let it look a little bit more
		interesting. Because we have no dynamic lights in this scene we disable
		lighting for each model (otherwise the models would be black).
	*/
	scene::ISceneNode*  pNode = m_pScrMgr->addSphereSceneNode ( );
	if ( pNode )
	{
		pNode->setPosition ( core::vector3df ( 0, 0, 30 ) );
		pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/wall.bmp" ) );
		pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
	}

	/*
		Now we create another node, movable using a scene node animator. Scene
		node animators modify scene nodes and can be attached to any scene node
		like mesh scene nodes, billboards, lights and even camera scene nodes.
		Scene node animators are not only able to modify the position of a
		scene node, they can also animate the textures of an object for
		example. We create a cube scene node and attach a 'fly circle' scene
		node animator to it, letting this node fly around our sphere scene node.
	*/
	scene::ISceneNode* pNode2 = m_pScrMgr->addCubeSceneNode ( );
	if ( pNode2 )
	{
		pNode2->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/t351sml.jpg" ) );
		pNode2->setMaterialFlag ( video::EMF_LIGHTING, false );
		scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createFlyCircleAnimator ( core::vector3df ( 0, 0, 30 ), 20.0f );
		if ( pAnim )
		{
			pNode2->addAnimator ( pAnim );
			pAnim->drop ( );
		}
	}

	/*
		The last scene node we add to show possibilities of scene node animators is
		a b3d model, which uses a 'fly straight' animator to run between to points.
	*/
	scene::IAnimatedMeshSceneNode*  pAnms = m_pScrMgr->addAnimatedMeshSceneNode ( m_pScrMgr->getMesh ( "/res/media/ninja.b3d" ) );
	if ( pAnms )
	{
		scene::ISceneNodeAnimator*  pAnim = 
			m_pScrMgr->createFlyStraightAnimator ( core::vector3df ( 100, 0, 60 ), core::vector3df ( -100, 0, 60 ), 3500, true );
		if ( pAnim )
		{
			pAnms->addAnimator ( pAnim );
			pAnim->drop ( );
		}

		/*
			To make the model look right we disable lighting, set the
			frames between which the animation should loop, rotate the
			model around 180 degrees, and adjust the animation speed and
			the texture. To set the right animation (frames and speed), we
			would also be able to just call
			"anms->setMD2Animation(scene::EMAT_RUN)" for the 'run'
			animation instead of "setFrameLoop" and "setAnimationSpeed",
			but this only works with MD2 animations, and so you know how to
			start other animations. But a good advice is to not use
			hardcoded frame-numbers...
		*/
		pAnms->setMaterialFlag ( video::EMF_LIGHTING, false );

		pAnms->setFrameLoop ( 0, 13 );
		pAnms->setAnimationSpeed ( 15 );
//		pAnms->setMD2Animation ( EMAT_RUN );

		pAnms->setScale ( core::vector3df ( 2.f, 2.f, 2.f ) );
		pAnms->setRotation ( core::vector3df ( 0, -90, 0 ) );
//		pAnms->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/sydney.bmp" ) );
	}

	/*
		To be able to look at and move around in this scene, we create a first
		person shooter style camera and make the mouse cursor invisible.
	*/
	m_pScrMgr->addCameraSceneNodeFPS ( );
	s_pDevice->getCursorControl ( )->setVisible ( false );

	gui::IGUIStaticText*  pDiagnostics = m_pGuiEnv->addStaticText ( L"", core::rect<s32> ( 10, 10, 400, 20 ) );
	pDiagnostics->setOverrideColor ( video::SColor ( 255, 255, 255, 0 ) );

	m_pNode = pNode;
	m_uThen = s_pDevice->getTimer ( )->getTime ( );
}

CT04_Movement::~CT04_Movement ( KDvoid )
{
	
}

bool CT04_Movement::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// Remember whether each key is down or up
	if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
	{
		m_bKeyIsDown [ event.KeyInput.Key ] = event.KeyInput.PressedDown;
	}

	return false;
}

video::SColor CT04_Movement::getClear ( KDvoid )
{
	return video::SColor ( 255, 113, 113, 133 );
}

const wchar_t* CT04_Movement::getTitle ( KDvoid )
{
	return L"04. Movement";
}

KDvoid CT04_Movement::Draw ( KDvoid )
{
	const f32 MOVEMENT_SPEED = 5.f;

	// Work out a frame delta time.
	
	const u32  uNow = s_pDevice->getTimer ( )->getTime ( );
	const f32  fDelta = (f32) ( uNow - m_uThen ) / 1000.f; // Time in seconds
	m_uThen = uNow;

	/* 
		Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. 
	*/
	core::vector3df  tPos = m_pNode->getPosition ( );

	if ( m_bKeyIsDown [ irr::KEY_KEY_W ] )
	{
		tPos.Y += MOVEMENT_SPEED * fDelta;
	}
	else if ( m_bKeyIsDown [ irr::KEY_KEY_S ] )
	{
		tPos.Y -= MOVEMENT_SPEED * fDelta;
	}
	
	if ( m_bKeyIsDown [ irr::KEY_KEY_A ] )
	{
		tPos.X -= MOVEMENT_SPEED * fDelta;
	}
	else if ( m_bKeyIsDown [ irr::KEY_KEY_D ] )
	{
		tPos.X += MOVEMENT_SPEED * fDelta;
	}

	m_pNode->setPosition ( tPos );

	CTutorial::Draw ( );
}