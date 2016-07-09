/* --------------------------------------------------------------------------
 *
 *      File            T02_Quake3Map.cpp
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
#include "T02_Quake3Map.h"

CT02_Quake3Map::CT02_Quake3Map ( KDvoid )
{
	/*
		To display the Quake 3 map, we first need to load it. Quake 3 maps
		are packed into .pk3 files which are nothing else than .zip files.
		So we add the .pk3 file to our irr::io::IFileSystem. After it was added,
		we are able to read from the files in that archive as if they are
		directly stored on the disk.
	*/
	s_pDevice->getFileSystem ( )->addFileArchive ( "/res/media/map-20kdm2.pk3" );

	/*
		Now we can load the mesh by calling
		irr::scene::ISceneManager::getMesh(). We get a pointer returned to an
		irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not
		really animated, they are only a huge chunk of static geometry with
		some materials attached. Hence the IAnimatedMesh consists of only one
		frame, so we get the "first frame" of the "animation", which is our
		quake level and create an Octree scene node with it, using
		irr::scene::ISceneManager::addOctreeSceneNode().
		The Octree optimizes the scene a little bit, trying to draw only geometry
		which is currently visible. An alternative to the Octree would be a
		irr::scene::IMeshSceneNode, which would always draw the complete
		geometry of the mesh, without optimization. Try it: Use
		irr::scene::ISceneManager::addMeshSceneNode() instead of
		addOctreeSceneNode() and compare the primitives drawn by the video
		driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn()
		method in the irr::video::IVideoDriver class). Note that this
		optimization with the Octree is only useful when drawing huge meshes
		consisting of lots of geometry.
	*/
	scene::IAnimatedMesh*  pMesh = m_pScrMgr->getMesh ( "20kdm2.bsp" );
	scene::ISceneNode*     pNode = KD_NULL;

	if ( pMesh )
	{
		pNode = m_pScrMgr->addOctreeSceneNode ( pMesh->getMesh ( 0 ), 0, -1, 1024 );
	}

	/*
		Because the level was not modelled around the origin (0,0,0), we
		translate the whole level a little bit. This is done on
		irr::scene::ISceneNode level using the methods
		irr::scene::ISceneNode::setPosition() (in this case),
		irr::scene::ISceneNode::setRotation(), and
		irr::scene::ISceneNode::setScale().
	*/
	if ( pNode )
	{
		pNode->setPosition ( core::vector3df ( -1300, -144, -1249 ) );
	}

	/*
		Now we only need a camera to look at the Quake 3 map.
		We want to create a user controlled camera. There are some
		cameras available in the Irrlicht engine. For example the
		MayaCamera which can be controlled like the camera in Maya:
		Rotate with left mouse button pressed, Zoom with both buttons pressed,
		translate with right mouse button pressed. This could be created with
		irr::scene::ISceneManager::addCameraSceneNodeMaya(). But for this
		example, we want to create a camera which behaves like the ones in
		first person shooter games (FPS) and hence use
		irr::scene::ISceneManager::addCameraSceneNodeFPS().
	*/
	m_pScrMgr->addCameraSceneNodeFPS ( );
	s_pDevice->getCursorControl ( )->setVisible ( false );
}

CT02_Quake3Map::~CT02_Quake3Map ( KDvoid )
{
	
}

video::SColor CT02_Quake3Map::getClear ( KDvoid )
{
	return video::SColor ( 255, 200, 200, 200 );
}

const wchar_t* CT02_Quake3Map::getTitle ( KDvoid )
{
	return L"02. Quake3Map";
}