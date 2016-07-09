/* --------------------------------------------------------------------------
 *
 *      File            T01_HelloWorld.cpp
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
#include "T01_HelloWorld.h"

CT01_HelloWorld::CT01_HelloWorld ( KDvoid )
{

	/*
		To show something interesting, we load a Quake 2 model and display it.
		We only have to get the Mesh from the Scene Manager with getMesh() and add
		a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
		check the return value of getMesh() to become aware of loading problems
		and other errors.

		Instead of writing the filename sydney.md2, it would also be possible
		to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
		other supported file format. By the way, that cool Quake 2 model
		called sydney was modelled by Brian Collins.
	*/
	scene::IAnimatedMesh*  pMesh = m_pScrMgr->getMesh ( "/res/media/sydney.md2" );
	kdAssert ( pMesh );

	/*
		To let the mesh look a little bit nicer, we change its material. We
		disable lighting because we do not have a dynamic light in here, and
		the mesh would be totally black otherwise. Then we set the frame loop,
		such that the predefined STAND animation is used. And last, we apply a
		texture to the mesh. Without it the mesh would be drawn using only a
		color.
	*/
	scene::IAnimatedMeshSceneNode*  pNode = m_pScrMgr->addAnimatedMeshSceneNode ( pMesh );
	if ( pNode )
	{
		pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
		pNode->setMD2Animation ( scene::EMAT_STAND );
		pNode->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/sydney.bmp" ) );
	}

	/*
		To look at the mesh, we place a camera into 3d space at the position
		(0, 30, -40). The camera looks from there to (0,10,0), which is
		approximately the place where our md2 model is.
	*/
	m_pScrMgr->addCameraSceneNode 
	(
		0,
		core::vector3df ( 0, 30, -40 ),
		core::vector3df ( 0, 10, 0 )
	);
}

video::SColor CT01_HelloWorld::getClear ( KDvoid )
{
	return video::SColor ( 255, 100, 101, 140 );
}

const wchar_t* CT01_HelloWorld::getTitle ( KDvoid )
{
	return L"01. HelloWorld";
}
