/* --------------------------------------------------------------------------
 *
 *      File            T19_Mouse.cpp
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
#include "T19_Mouse.h"

CT19_Mouse::CT19_Mouse ( KDvoid )
{
	/*
		We'll create an arrow mesh and move it around either with the joystick axis/hat,
		or make it follow the mouse pointer. 
	*/
	scene::ISceneNode*  pNode = m_pScrMgr->addMeshSceneNode
	(
		m_pScrMgr->addArrowMesh
		(
			"Arrow",
			video::SColor ( 255, 255, 0, 0 ),
			video::SColor ( 255, 0, 255, 0 ),
			16, 16,
			2.f, 1.3f,
			0.1f, 0.6f
		)
	);
	pNode->setMaterialFlag ( video::EMF_LIGHTING, false );

	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNode ( );
	pCamera->setPosition ( core::vector3df ( 0, 0, -10 ) );

	m_uThen = s_pDevice->getTimer ( )->getTime ( );
	m_pNode = pNode;
	m_pCamera = pCamera;
}

CT19_Mouse::~CT19_Mouse ( KDvoid )
{
	
}

bool CT19_Mouse::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// Remember the mouse state
	if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		switch ( event.MouseInput.Event )
		{
			case EMIE_LMOUSE_PRESSED_DOWN :
				m_tMouseState.LeftButtonDown = true;
				break;

			case EMIE_LMOUSE_LEFT_UP :
				m_tMouseState.LeftButtonDown = false;
				break;

			case EMIE_MOUSE_MOVED :
				m_tMouseState.Position.X = event.MouseInput.X;
				m_tMouseState.Position.Y = event.MouseInput.Y;
				break;

			default :
				// We won't use the wheel
				break;
		}
	}

	return false;
}

KDvoid CT19_Mouse::Draw ( KDvoid )
{
	// Work out a frame delta time.
	const u32  uNow = s_pDevice->getTimer ( )->getTime ( );
	const f32  fFrameDeltaTime = (f32) ( uNow - m_uThen ) / 1000.f; // Time in seconds
	m_uThen = uNow;

	const f32 MOVEMENT_SPEED = 5.f;
	core::vector3df  tNodePosition = m_pNode->getPosition ( );

	if ( m_tMouseState.LeftButtonDown )
	{
		// Create a ray through the mouse cursor.
		core::line3df  tRay = m_pScrMgr->getSceneCollisionManager ( )->getRayFromScreenCoordinates
		(
			m_tMouseState.Position, m_pCamera
		);

		// And intersect the ray with a plane around the node facing towards the camera.
		core::plane3df  tPlane ( tNodePosition, core::vector3df ( 0, 0, -1 ) );
		core::vector3df tMousePosition;
		if ( tPlane.getIntersectionWithLine ( tRay.start, tRay.getVector ( ), tMousePosition ) )
		{
			// We now have a mouse position in 3d space; move towards it.
			core::vector3df tToMousePosition ( tMousePosition - tNodePosition );
			const f32  fAvailableMovement = MOVEMENT_SPEED * fFrameDeltaTime;

			if ( tToMousePosition.getLength ( ) <= fAvailableMovement )
			{
				tNodePosition = tMousePosition; // Jump to the final position
			}
			else
			{
				tNodePosition += tToMousePosition.normalize ( ) * fAvailableMovement; // Move towards it
			}
		}
		
		m_pNode->setPosition ( tNodePosition );
	}

	// Turn lighting on and off depending on whether the left mouse button is down.
	m_pNode->setMaterialFlag ( video::EMF_LIGHTING, m_tMouseState.LeftButtonDown );

	CTutorial::Draw ( );
}

video::SColor CT19_Mouse::getClear ( KDvoid )
{
	return video::SColor ( 255, 113, 113, 133 );
}

const wchar_t* CT19_Mouse::getTitle ( KDvoid )
{
	return L"19. Mouse";
}