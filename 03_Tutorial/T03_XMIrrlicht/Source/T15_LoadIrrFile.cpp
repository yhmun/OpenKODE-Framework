/* --------------------------------------------------------------------------
 *
 *      File            T15_LoadIrrFile.cpp
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
#include "T15_LoadIrrFile.h"

CT15_LoadIrrFile::CT15_LoadIrrFile ( KDvoid )
{
	/*
		Now load our .irr file.
		.irr files can store the whole scene graph including animators,
		materials and particle systems. And there is also the possibility to
		store arbitrary user data for every scene node in that file. To keep
		this example simple, we are simply loading the scene here. See the
		documentation at ISceneManager::loadScene and ISceneManager::saveScene
		for more information. So to load and display a complicated huge scene,
		we only need a single call to loadScene ( ).
	*/

	// load the scene
	m_pScrMgr->loadScene ( "/res/media/example.irr" );

	/*
		Now we'll create a camera, and give it a collision response animator
		that's built from the mesh nodes in the scene we just loaded.
	*/
	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNodeFPS ( 0, 50.f, 0.1f );

	// Create a meta triangle selector to hold several triangle selectors.
	scene::IMetaTriangleSelector*  pMeta = m_pScrMgr->createMetaTriangleSelector ( );

	/*
		Now we will find all the nodes in the scene and create triangle
		selectors for all suitable nodes.  Typically, you would want to make a
		more informed decision about which nodes to performs collision checks
		on; you could capture that information in the node name or Id.
	*/
	core::array<scene::ISceneNode*>  pNodes;
	m_pScrMgr->getSceneNodesFromType ( scene::ESNT_ANY, pNodes ); // Find all nodes

	for ( u32 i = 0; i < pNodes.size ( ); ++i )
	{
		scene::ISceneNode*  pNode = pNodes [ i ];
		scene::ITriangleSelector* pSelector = KD_NULL;

		switch ( pNode->getType ( ) )
		{
			case scene::ESNT_CUBE:
			case scene::ESNT_ANIMATED_MESH:
				// Because the selector won't animate with the mesh,
				// and is only being used for camera collision, we'll just use an approximate
				// bounding box instead of ( (scene::IAnimatedMeshSceneNode*) pNode )->getMesh ( 0 )
				pSelector = m_pScrMgr->createTriangleSelectorFromBoundingBox ( pNode );
			break;

			case scene::ESNT_MESH:
			case scene::ESNT_SPHERE: // Derived from IMeshSceneNode
				pSelector = m_pScrMgr->createTriangleSelector ( ( (scene::IMeshSceneNode*) pNode )->getMesh ( ), pNode );
				break;

			case scene::ESNT_TERRAIN:
				pSelector = m_pScrMgr->createTerrainTriangleSelector ( (scene::ITerrainSceneNode*) pNode );
				break;

			case scene::ESNT_OCTREE:
				pSelector = m_pScrMgr->createOctreeTriangleSelector ( ( (scene::IMeshSceneNode*) pNode )->getMesh ( ), pNode );
				break;

			default:
				// Don't create a selector for this node type
				break;
		}

		if ( pSelector )
		{
			// Add it to the meta selector, which will take a reference to it
			pMeta->addTriangleSelector ( pSelector );
			// And drop my reference to it, so that the meta selector owns it.
			pSelector->drop ( );
		}
	}

	/*
		Now that the mesh scene nodes have had triangle selectors created and added
		to the meta selector, create a collision response animator from that meta selector.
	*/
	scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createCollisionResponseAnimator 
	( 
		pMeta, pCamera, core::vector3df ( 5, 5, 5 ),
		core::vector3df ( 0, 0, 0 ) 
	);
	pMeta->drop ( ); // I'm done with the meta selector now

	pCamera->addAnimator ( pAnim );
	pAnim->drop ( ); // I'm done with the animator now

	// And set the camera position so that it doesn't start off stuck in the geometry
	pCamera->setPosition ( core::vector3df ( 0.f, 20.f, -50.f ) );

	// Point the camera at the cube node, by finding the first node of type ESNT_CUBE
	scene::ISceneNode*  pCube = m_pScrMgr->getSceneNodeFromType ( scene::ESNT_CUBE );
	if ( pCube )
	{
		pCamera->setTarget ( pCube->getAbsolutePosition ( ) );
	}
}

CT15_LoadIrrFile::~CT15_LoadIrrFile ( KDvoid )
{
	
}

const wchar_t* CT15_LoadIrrFile::getTitle ( KDvoid )
{
	return L"15. LoadIrrFile";
}