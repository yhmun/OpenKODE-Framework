/* --------------------------------------------------------------------------
 *
 *      File            T07_Collision.cpp
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
#include "T07_Collision.h"

CT07_Collision::CT07_Collision ( KDvoid )
{
	s_pDevice->getFileSystem ( )->addFileArchive ( "/res/media/map-20kdm2.pk3" );

	scene::IAnimatedMesh*   pQ3levelmesh = m_pScrMgr->getMesh ( "20kdm2.bsp" );
	scene::IMeshSceneNode*  pQ3node = KD_NULL;

	// The Quake mesh is pickable, but doesn't get highlighted.
	if ( pQ3levelmesh )
	{
		pQ3node = m_pScrMgr->addOctreeSceneNode ( pQ3levelmesh->getMesh ( 0 ), 0, IDFlag_IsPickable );
	}

	/*
		So far so good, we've loaded the quake 3 level like in tutorial 2. Now,
		here comes something different: We create a triangle selector. A
		triangle selector is a class which can fetch the triangles from scene
		nodes for doing different things with them, for example collision
		detection. There are different triangle selectors, and all can be
		created with the ISceneManager. In this example, we create an
		OctreeTriangleSelector, which optimizes the triangle output a little
		bit by reducing it like an octree. This is very useful for huge meshes
		like quake 3 levels. After we created the triangle selector, we attach
		it to the q3node. This is not necessary, but in this way, we do not
		need to care for the selector, for example dropping it after we do not
		need it anymore.
	*/

	scene::ITriangleSelector*  pSelector = KD_NULL;

	if ( pQ3node )
	{
		pQ3node->setPosition ( core::vector3df ( -1350, -130, -1400 ) );

		pSelector = m_pScrMgr->createOctreeTriangleSelector ( pQ3node->getMesh ( ), pQ3node, 128 );
		pQ3node->setTriangleSelector ( pSelector );
		// We're not done with this selector yet, so don't drop it.
	}

	/*
		We add a first person shooter camera to the scene so that we can see and
		move in the quake 3 level like in tutorial 2. But this, time, we add a
		special animator to the camera: A Collision Response animator. This
		animator modifies the scene node to which it is attached to in order to
		prevent it moving through walls, and to add gravity to it. The
		only thing we have to tell the animator is how the world looks like,
		how big the scene node is, how much gravity to apply and so on. After the
		collision response animator is attached to the camera, we do not have to do
		anything more for collision detection, anything is done automatically.
		The rest of the collision detection code below is for picking. And please
		note another cool feature: The collision response animator can be
		attached also to all other scene nodes, not only to cameras. And it can
		be mixed with other scene node animators. In this way, collision
		detection and response in the Irrlicht engine is really easy.

		Now we'll take a closer look on the parameters of
		createCollisionResponseAnimator(). The first parameter is the
		TriangleSelector, which specifies how the world, against collision
		detection is done looks like. The second parameter is the scene node,
		which is the object, which is affected by collision detection, in our
		case it is the camera. The third defines how big the object is, it is
		the radius of an ellipsoid. Try it out and change the radius to smaller
		values, the camera will be able to move closer to walls after this. The
		next parameter is the direction and speed of gravity.  We'll set it to
		(0, -10, 0), which approximates to realistic gravity, assuming that our
		units are metres. You could set it to (0,0,0) to disable gravity. And the
		last value is just a translation: Without this, the ellipsoid with which
		collision detection is done would be around the camera, and the camera would
		be in the middle of the ellipsoid. But as human beings, we are used to have our
		eyes on top of the body, with which we collide with our world, not in
		the middle of it. So we place the scene node 50 units over the center
		of the ellipsoid with this parameter. And that's it, collision
		detection works now.
	*/

	// Set a jump speed of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	scene::ICameraSceneNode*  pCamera =
		m_pScrMgr->addCameraSceneNodeFPS ( 0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f );

	pCamera->setPosition ( core::vector3df ( 50, 50, -60 ) );
	pCamera->setTarget ( core::vector3df ( -70, 30, -60 ) );

	if ( pSelector )
	{
		scene::ISceneNodeAnimator*  pAnim = m_pScrMgr->createCollisionResponseAnimator 
		(
			pSelector, pCamera, core::vector3df ( 30, 50, 30 ),
			core::vector3df ( 0, -10, 0 ), core::vector3df ( 0, 30, 0 )
		);

		pSelector->drop ( );	// As soon as we're done with the selector, drop it.
		pCamera->addAnimator ( pAnim );
		pAnim->drop ( );		// And likewise, drop the animator when we're done referring to it.
	}

	// Now I create three animated characters which we can pick, a dynamic light for
	// lighting them, and a billboard for drawing where we found an intersection.

	// First, let's get rid of the mouse cursor.  We'll use a billboard to show
	// what we're looking at.
	s_pDevice->getCursorControl ( )->setVisible ( false );

	// Add the billboard.
	scene::IBillboardSceneNode*  pBill = m_pScrMgr->addBillboardSceneNode ( );
	pBill->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
	pBill->setMaterialTexture ( 0, s_pDriver->getTexture ( "/res/media/particle.bmp" ) );
	pBill->setMaterialFlag ( video::EMF_LIGHTING, false );
	pBill->setMaterialFlag ( video::EMF_ZBUFFER, false );
	pBill->setSize ( core::dimension2d<f32> ( 20.0f, 20.0f ) );
	pBill->setID ( ID_IsNotPickable );	// This ensures that we don't accidentally ray-pick it

	/*
		Add 3 animated hominids, which we can pick using a ray-triangle intersection.
		They all animate quite slowly, to make it easier to see that accurate triangle
		selection is being performed.
	*/
	scene::IAnimatedMeshSceneNode*  pNode = KD_NULL;

	// Add an MD2 node, which uses vertex-based animation.
	pNode = m_pScrMgr->addAnimatedMeshSceneNode
	( 
		m_pScrMgr->getMesh ( "/res/media/faerie.md2" ), 0, IDFlag_IsPickable | IDFlag_IsHighlightable 
	);
	pNode->setPosition ( core::vector3df ( -90, -15, -140 ) );	// Put its feet on the floor.
	pNode->setScale ( core::vector3df ( 1.6f ) );				// Make it appear realistically scaled
	pNode->setMD2Animation ( scene::EMAT_POINT );
	pNode->setAnimationSpeed ( 20.f );

	m_tMaterial.setTexture ( 0, s_pDriver->getTexture ( "/res/media/faerie2.bmp" ) );
	m_tMaterial.Lighting = true;
	m_tMaterial.NormalizeNormals = true;

	pNode->getMaterial ( 0 ) = m_tMaterial;

	// Now create a triangle selector for it.  The selector will know that it
	// is associated with an animated node, and will update itself as necessary.
	pSelector = m_pScrMgr->createTriangleSelector ( pNode );
	pNode->setTriangleSelector ( pSelector );
	pSelector->drop ( ); // We're done with this selector, so drop it now.

	// And this B3D file uses skinned skeletal animation.
	pNode = m_pScrMgr->addAnimatedMeshSceneNode
	(
		m_pScrMgr->getMesh ( "/res/media/ninja.b3d" ), 0, IDFlag_IsPickable | IDFlag_IsHighlightable
	);
	pNode->setScale ( core::vector3df ( 10 ) );
	pNode->setPosition ( core::vector3df ( -75,-66,-80 ) );
	pNode->setRotation ( core::vector3df ( 0, 90, 0 ) );
	pNode->setAnimationSpeed ( 8.f );
	pNode->getMaterial ( 0 ).NormalizeNormals = true;
	pNode->getMaterial ( 0 ).Lighting = true;
	// Just do the same as we did above.
	pSelector = m_pScrMgr->createTriangleSelector ( pNode );
	pNode->setTriangleSelector ( pSelector );
	pSelector->drop ( );

	// This X files uses skeletal animation, but without skinning.
	pNode = m_pScrMgr->addAnimatedMeshSceneNode
	(
		m_pScrMgr->getMesh("/res/media/dwarf.x"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable
	);
	pNode->setPosition ( core::vector3df ( -70, -66, -30 ) );	// Put its feet on the floor.
	pNode->setRotation ( core::vector3df ( 0, -90, 0 ) );		// And turn it towards the camera.
	pNode->setAnimationSpeed ( 20.f );
	pNode->getMaterial ( 0 ).Lighting = true;
	pSelector = m_pScrMgr->createTriangleSelector ( pNode );
	pNode->setTriangleSelector ( pSelector );
	pSelector->drop ( );

	// And this mdl file uses skinned skeletal animation.
	pNode = m_pScrMgr->addAnimatedMeshSceneNode
	(
		m_pScrMgr->getMesh ( "/res/media/yodan.mdl" ), 0, IDFlag_IsPickable | IDFlag_IsHighlightable
	);
	pNode->setPosition ( core::vector3df ( -90, -25, 20 ) );
	pNode->setScale ( core::vector3df ( 0.8f ) );
	pNode->getMaterial ( 0 ).Lighting = true;
	pNode->setAnimationSpeed ( 20.f );

	// Just do the same as we did above.
	pSelector = m_pScrMgr->createTriangleSelector ( pNode );
	pNode->setTriangleSelector ( pSelector );
	pSelector->drop ( );

	m_tMaterial.setTexture ( 0, 0 );
	m_tMaterial.Lighting = false;

	// Add a light, so that the unselected nodes aren't completely dark.
	scene::ILightSceneNode*  pLight = m_pScrMgr->addLightSceneNode
	(
		0,
		core::vector3df ( -60, 100, 400 ),
		video::SColorf ( 1.0f, 1.0f, 1.0f, 1.0f ),
		600.0f
	);
	pLight->setID ( ID_IsNotPickable ); // Make it an invalid target for selection.

	// Remember which scene node is highlighted
	m_pHighlightedSceneNode = 0;
	m_pColMgr = m_pScrMgr->getSceneCollisionManager ( );

	// draw the selection triangle only as wireframe
	m_tMaterial.Wireframe = true;

	m_pCamera = pCamera;
	m_pBill = pBill;
}

CT07_Collision::~CT07_Collision ( KDvoid )
{
	
}

KDvoid CT07_Collision::Draw ( KDvoid )
{
	// Unlight any currently highlighted scene node
	if ( m_pHighlightedSceneNode )
	{
		m_pHighlightedSceneNode->setMaterialFlag ( video::EMF_LIGHTING, true );
		m_pHighlightedSceneNode = 0;
	}

	// All intersections in this example are done with a ray cast out from the camera to
	// a distance of 1000.  You can easily modify this to check (e.g.) a bullet
	// trajectory or a sword's position, or create a ray from a mouse click position using
	// ISceneCollisionManager::getRayFromScreenCoordinates()
	core::line3d<f32>  tRay;
	tRay.start = m_pCamera->getPosition ( );
	tRay.end = tRay.start + ( m_pCamera->getTarget ( ) - tRay.start ).normalize ( ) * 1000.0f;

	// Tracks the current intersection point with the level or a mesh
	core::vector3df  tIntersection;
	// Used to show with triangle has been hit
	core::triangle3df  tHitTriangle;

	// This call is all you need to perform ray/triangle collision on every scene node
	// that has a triangle selector, including the Quake level mesh.  It finds the nearest
	// collision point/triangle, and returns the scene node containing that point.
	// Irrlicht provides other types of selection, including ray/triangle selector,
	// ray/box and ellipse/triangle selector, plus associated helpers.
	// See the methods of ISceneCollisionManager
	scene::ISceneNode*  pSelectedSceneNode = m_pColMgr->getSceneNodeAndCollisionPointFromRay
	(
		tRay,
		tIntersection,		// This will be the position of the collision
		tHitTriangle,		// This will be the triangle hit in the collision
		IDFlag_IsPickable,	// This ensures that only nodes that we have
							// set up to be pickable are considered
		0					// Check the entire scene (this is actually the implicit default)
	);

	// If the ray hit anything, move the billboard to the collision position
	// and draw the triangle that was hit.
	if ( pSelectedSceneNode )
	{
		m_pBill->setPosition ( tIntersection );

		// We need to reset the transform before doing our own rendering.
		s_pDriver->setTransform ( video::ETS_WORLD, core::matrix4 ( ) );
		s_pDriver->setMaterial ( m_tMaterial );
		s_pDriver->draw3DTriangle ( tHitTriangle, video::SColor ( 0, 255, 0, 0 ) );

		// We can check the flags for the scene node that was hit to see if it should be
		// highlighted. The animated nodes can be highlighted, but not the Quake level mesh
		if ( ( pSelectedSceneNode->getID ( ) & IDFlag_IsHighlightable ) == IDFlag_IsHighlightable )
		{
			m_pHighlightedSceneNode = pSelectedSceneNode;

			// Highlighting in this case means turning lighting OFF for this node,
			// which means that it will be drawn with full brightness.
			m_pHighlightedSceneNode->setMaterialFlag ( video::EMF_LIGHTING, false );
		}
	}

	CTutorial::Draw ( );
}

const wchar_t* CT07_Collision::getTitle ( KDvoid )
{
	return L"07. Collision";
}

KDbool CT07_Collision::getVirPad ( KDvoid )
{
	return KD_TRUE;
}