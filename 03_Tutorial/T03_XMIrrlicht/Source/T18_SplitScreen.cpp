/* --------------------------------------------------------------------------
 *
 *      File            T18_SplitScreen.cpp
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
#include "T18_SplitScreen.h"

CT18_SplitScreen::CT18_SplitScreen ( KDvoid )
: m_bInit ( false )
{
	// Load model
	scene::IAnimatedMesh* pModel = m_pScrMgr->getMesh ( "/res/media/sydney.md2" );

	scene::IAnimatedMeshSceneNode* pModelNode = m_pScrMgr->addAnimatedMeshSceneNode ( pModel );

	// Load texture
	if ( pModelNode )
	{
		video::ITexture*  pTexture = s_pDriver->getTexture ( "/res/media/sydney.bmp" );
		pModelNode->setMaterialTexture ( 0, pTexture );
		pModelNode->setMD2Animation ( scene::EMAT_RUN );
		//Disable lighting (we've got no light)
		pModelNode->setMaterialFlag ( video::EMF_LIGHTING, false );
	}

	// Load map
	s_pDevice->getFileSystem ( )->addFileArchive ( "/res/media/map-20kdm2.pk3" );
	scene::IAnimatedMesh*  pMap = m_pScrMgr->getMesh ( "20kdm2.bsp" );
	if ( pMap )
	{
		scene::ISceneNode*  pMapNode = m_pScrMgr->addOctreeSceneNode ( pMap->getMesh ( 0 ) );
		// Set position
		pMapNode->setPosition ( core::vector3df ( -850, -220, -850 ) );
	}

	/*
		Now we create our four cameras. One is looking at the model
		from the front, one from the top and one from the side. In
		addition there's a FPS-camera which can be controlled by the
		user.
	*/
	// Create 3 fixed and one user-controlled cameras
	//Front
	m_pCamera [ 0 ] = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df ( 50,  0,  0 ), core::vector3df ( 0, 0, 0 ) );
	//Top
	m_pCamera [ 1 ] = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df (  0, 50,  0 ), core::vector3df ( 0, 0, 0 ) );
	//Left
	m_pCamera [ 2 ] = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df (  0,  0, 50 ), core::vector3df ( 0, 0, 0 ) );
	//User-controlled
	m_pCamera [ 3 ] = m_pScrMgr->addCameraSceneNodeFPS ( );
	// don't start at sydney's position
	if ( m_pCamera [ 3 ])
	{
		m_pCamera [ 3 ]->setPosition ( core::vector3df ( -50, 0, -50 ) );
	}

	/*
		Create a variable for counting the fps and hide the mouse:
	*/
	//Hide mouse
	s_pDevice->getCursorControl ( )->setVisible ( false );

	m_bSplitScreen = true;
	m_bInit = true;
}

CT18_SplitScreen::~CT18_SplitScreen ( KDvoid )
{
	const core::dimension2d<u32> tSize = s_pDriver->getScreenSize ();

	s_pDriver->setViewPort ( core::rect<s32> ( 0, 0, tSize.Width, tSize.Height ) );
}

KDvoid CT18_SplitScreen::Draw ( KDvoid )
{
	const core::dimension2d<u32> tSize = s_pDriver->getScreenSize ();

	s32  w2 = tSize.Width / 2;
	s32  h2 = tSize.Height / 2;

	if ( m_bSplitScreen )
	{
		//Activate camera1
		m_pScrMgr->setActiveCamera ( m_pCamera [ 0 ] );
		//Set viewpoint to the first quarter (left top)
		s_pDriver->setViewPort ( _rect ( 0, 0, w2, h2 ) );
		m_pScrMgr->drawAll();

		//Activate camera2
		m_pScrMgr->setActiveCamera ( m_pCamera [ 1 ] );
		//Set viewpoint to the second quarter (right top)
		s_pDriver->setViewPort ( _rect ( w2, 0, w2, h2 ) );
		m_pScrMgr->drawAll ( );

		//Activate camera3
		m_pScrMgr->setActiveCamera ( m_pCamera [ 2 ] );
		//Set viewpoint to the third quarter (left bottom)
		s_pDriver->setViewPort ( _rect ( 0, h2, w2, h2 ) );
		m_pScrMgr->drawAll ( );

		//Set viewport the last quarter (right bottom)
		m_pScrMgr->setActiveCamera ( m_pCamera [ 3 ] );
		s_pDriver->setViewPort ( _rect ( w2, h2, w2, h2 ) );
		m_pScrMgr->drawAll ( );
	}
	else
	{
		m_pScrMgr->setActiveCamera ( m_pCamera [ 3 ] );
		s_pDriver->setViewPort ( _rect ( 0, 0, tSize.Width, tSize.Height ) );
		m_pScrMgr->drawAll ( );
	}

	s_pDriver->setViewPort ( _rect ( 0, 0, tSize.Width, tSize.Height ) );
	m_pGuiEnv->drawAll ( );
}

video::SColor CT18_SplitScreen::getClear ( KDvoid )
{
	return video::SColor ( 255, 100, 100, 100 );
}

const wchar_t* CT18_SplitScreen::getTitle ( KDvoid )
{
	return L"18. SplitScreen";
}

bool CT18_SplitScreen::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// Key S enables/disables SplitScreen
	if ( event.EventType == irr::EET_KEY_INPUT_EVENT &&
		 event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown )
	{
		m_bSplitScreen = !m_bSplitScreen;
		return true;
	}

	// Send all other events to camera4
	if ( m_bInit && m_pCamera [ 3 ] )
	{
		return m_pCamera [ 3 ]->OnEvent ( event );
	}

	return false;
}