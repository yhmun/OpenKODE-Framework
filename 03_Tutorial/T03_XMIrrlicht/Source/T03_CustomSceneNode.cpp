/* --------------------------------------------------------------------------
 *
 *      File            T03_CustomSceneNode.cpp
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
#include "T03_CustomSceneNode.h"

class CSampleSceneNode : public scene::ISceneNode
{
	/*
		The parameters of the constructor specify the parent of the scene node,
		a pointer to the scene manager, and an id of the scene node.
		In the constructor we call the parent class' constructor,
		set some properties of the material, and
		create the 4 vertices of the tetraeder we will draw later.
	*/

	public :

		CSampleSceneNode ( scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id )
		: scene::ISceneNode ( parent, mgr, id )
		{
			Material.Wireframe = false;
			Material.Lighting  = false;
			Material.Thickness = 0.f;

			Vertices[0] = video::S3DVertex(0,0,10, 5,1,0,
						  video::SColor(255,0,255,255), 0, 1);
			Vertices[1] = video::S3DVertex(10,0,-10, 10,0,0,
						  video::SColor(255,255,0,255), 1, 1);
			Vertices[2] = video::S3DVertex(0,20,0, 20,1,1,
						  video::SColor(255,255,255,0), 1, 0);
			Vertices[3] = video::S3DVertex(-10,0,-10, 40,0,1,
						  video::SColor(255,0,255,0), 0, 0);

			/*
				The Irrlicht Engine needs to know the bounding box of a scene node.
				It will use it for automatic culling and other things. Hence, we
				need to create a bounding box from the 4 vertices we use.
				If you do not want the engine to use the box for automatic culling,
				and/or don't want to create the box, you could also call
				irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
			*/

			Box.reset ( Vertices[0].Pos );
			for ( s32 i = 1; i < 4; ++i )
			{
				Box.addInternalPoint ( Vertices[i].Pos );
			}
		}

		/*
			Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
			method of every scene node in the scene is called by the scene manager.
			If the scene node wishes to draw itself, it may register itself in the
			scene manager to be drawn. This is necessary to tell the scene manager
			when it should call irr::scene::ISceneNode::render(). For
			example, normal scene nodes render their content one after another,
			while stencil buffer shadows would like to be drawn after all other
			scene nodes. And camera or light scene nodes need to be rendered before
			all other scene nodes (if at all). So here we simply register the
			scene node to render normally. If we would like to let it be rendered
			like cameras or light, we would have to call
			SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
			After this, we call the actual
			irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
			which simply lets also all the child scene nodes of this node register
			themselves.
		*/
		virtual void OnRegisterSceneNode ( )
		{
			if ( IsVisible )
			{
				SceneManager->registerNodeForRendering ( this );
			}

			ISceneNode::OnRegisterSceneNode();
		}

		/*
			In the render() method most of the interesting stuff happens: The
			Scene node renders itself. We override this method and draw the
			tetraeder.
		*/
		virtual void render ( )
		{
			u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
			video::IVideoDriver* driver = SceneManager->getVideoDriver();

			driver->setMaterial(Material);
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
			driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
		}

		/*
			And finally we create three small additional methods.
			irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
			this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
			amount of materials in this scene node (our tetraeder only has one
			material), and irr::scene::ISceneNode::getMaterial() returns the
			material at an index. Because we have only one material here, we can
			return the only one material, assuming that no one ever calls
			getMaterial() with an index greater than 0.
		*/
		virtual const core::aabbox3d<f32>& getBoundingBox ( ) const
		{
			return Box;
		}

		virtual u32 getMaterialCount ( ) const
		{
			return 1;
		}

		virtual video::SMaterial& getMaterial ( u32 i )
		{
			return Material;
		}	

	private :

		/*
			First, we declare some member variables:
			The bounding box, 4 vertices, and the material of the tetraeder.
		*/
		core::aabbox3d<f32>		Box;
		video::S3DVertex		Vertices[4];
		video::SMaterial		Material;
};

CT03_CustomSceneNode::CT03_CustomSceneNode ( KDvoid )
{
	m_pScrMgr->addCameraSceneNode ( 0, core::vector3df ( 0, -40, 0 ), core::vector3df ( 0, 0, 0) );

	/*
		Create our scene node. I don't check the result of calling new, as it
		should throw an exception rather than returning 0 on failure. Because
		the new node will create itself with a reference count of 1, and then
		will have another reference added by its parent scene node when it is
		added to the scene, I need to drop my reference to it. Best practice is
		to drop it only *after* I have finished using it, regardless of what
		the reference count of the object is after creation.
	*/
	CSampleSceneNode*  pNode = new CSampleSceneNode ( m_pScrMgr->getRootSceneNode ( ), m_pScrMgr, 666 );

	/*
		To animate something in this boring scene consisting only of one
		tetraeder, and to show that you now can use your scene node like any
		other scene node in the engine, we add an animator to the scene node,
		which rotates the node a little bit.
		irr::scene::ISceneManager::createRotationAnimator() could return 0, so
		should be checked.
	*/
	scene::ISceneNodeAnimator* pAnim = m_pScrMgr->createRotationAnimator ( core::vector3df ( 0.8f, 0, 0.8f ) );

	if ( pAnim )
	{
		pNode->addAnimator ( pAnim );

		/*
			I'm done referring to anim, so must
			irr::IReferenceCounted::drop() this reference now because it
			was produced by a createFoo() function. As I shouldn't refer to
			it again, ensure that I can't by setting to 0.
		*/
		pAnim->drop ( );
	}

	/*
		I'm done with my CSampleSceneNode object, and so must drop my reference.
		This won't delete the object, yet, because it is still attached to the
		scene graph, which prevents the deletion until the graph is deleted or the
		custom scene node is removed from it.
	*/
	pNode->drop ( );
}

CT03_CustomSceneNode::~CT03_CustomSceneNode ( KDvoid )
{
	
}

video::SColor CT03_CustomSceneNode::getClear ( KDvoid )
{
	return video::SColor ( 0, 100, 100, 100 );
}

const wchar_t* CT03_CustomSceneNode::getTitle ( KDvoid )
{
	return L"03. CustomSceneNode";
}