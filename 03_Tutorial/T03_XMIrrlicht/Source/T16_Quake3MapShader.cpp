/* --------------------------------------------------------------------------
 *
 *      File            T16_Quake3MapShader.cpp
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
#include "T16_Quake3MapShader.h"

CT16_Quake3MapShader::CT16_Quake3MapShader ( KDvoid )
{
	/*
		To display the Quake 3 map, we first need to load it. Quake 3 maps
		are packed into .pk3 files, which are nothing other than .zip files.
		So we add the .pk3 file to our FileSystem. After it was added,
		we are able to read from the files in that archive as they would
		directly be stored on disk.
	*/
	s_pDevice->getFileSystem ( )->addFileArchive ( "/res/media/map-20kdm2.pk3" );

	// Quake3 Shader controls Z-Writing
	m_pScrMgr->getParameters ( )->setAttribute ( scene::ALLOW_ZWRITE_ON_TRANSPARENT, true );

	/*
		Now we can load the mesh by calling getMesh ( ). We get a pointer returned
		to a IAnimatedMesh. As you know, Quake 3 maps are not really animated,
		they are only a huge chunk of static geometry with some materials
		attached. Hence the IAnimated mesh consists of only one frame,
		so we get the "first frame" of the "animation", which is our quake level
		and create an Octree scene node with it, using addOctreeSceneNode ( ).
		The Octree optimizes the scene a little bit, trying to draw only geometry
		which is currently visible. An alternative to the Octree would be a
		AnimatedMeshSceneNode, which would draw always the complete geometry of
		the mesh, without optimization. Try it out: Write addAnimatedMeshSceneNode
		instead of addOctreeSceneNode and compare the primitives drawed by the
		video driver. ( There is a getPrimitiveCountDrawed ( ) method in the
		IVideoDriver class ). Note that this optimization with the Octree is only
		useful when drawing huge meshes consisting of lots of geometry.
	*/
	scene::IQ3LevelMesh*  const pMesh = (scene::IQ3LevelMesh*) m_pScrMgr->getMesh ( "20kdm2.bsp" );

	/*
		add the geometry mesh to the Scene ( polygon & patches )
		The Geometry mesh is optimised for faster drawing
	*/
	scene::ISceneNode*  pNode = 0;
	if ( pMesh )
	{
		scene::IMesh* const  pGeometry = pMesh->getMesh ( scene::quake3::E_Q3_MESH_GEOMETRY );
		pNode = m_pScrMgr->addOctreeSceneNode ( pGeometry, 0, -1, 4096 );
	}

	// create an event receiver for making screenshots
	m_pNode = pNode;
	m_sFilenameTemplate = "20kdm2.bsp";
	m_sFilenameTemplate.replace ( '/', '_' );
	m_sFilenameTemplate.replace ( '\\', '_' );

	/*
		now construct SceneNodes for each Shader
		The Objects are stored in the quake mesh scene::E_Q3_MESH_ITEMS
		and the Shader ID is stored in the MaterialParameters
		mostly dark looking skulls and moving lava.. or green flashing tubes?
	*/

	/*
		now construct SceneNodes for each Shader
		The Objects are stored in the quake mesh scene::E_Q3_MESH_ITEMS
		and the Shader ID is stored in the MaterialParameters
		mostly dark looking skulls and moving lava.. or green flashing tubes?
	*/
	if ( pMesh )
	{
		// the additional mesh can be quite huge and is unoptimized
		const scene::IMesh* const  pAdditionalMesh = pMesh->getMesh ( scene::quake3::E_Q3_MESH_ITEMS );

		for ( u32 i = 0; i != pAdditionalMesh->getMeshBufferCount ( ); ++i )
		{
			const scene::IMeshBuffer*  pMeshBuffer = pAdditionalMesh->getMeshBuffer ( i );
			const video::SMaterial&  tMaterial = pMeshBuffer->getMaterial ( );

			// The ShaderIndex is stored in the material parameter
			const s32  nShaderIndex = (s32) tMaterial.MaterialTypeParam2;

			// the meshbuffer can be rendered without additional support, or it has no shader
			const scene::quake3::IShader*  pShader = pMesh->getShader ( nShaderIndex );
			if ( 0 == pShader )
			{
				continue;
			}

			// we can dump the shader to the console in its
			// original but already parsed layout in a pretty
			// printers way.. commented out, because the console
			// would be full...
			// quake3::dumpShader ( Shader );

			pNode = m_pScrMgr->addQuake3SceneNode ( pMeshBuffer, pShader );
		}
	}

	/*
		Now we only need a Camera to look at the Quake 3 map. And we want to
		create a user controlled camera. There are some different cameras
		available in the Irrlicht engine. For example the Maya Camera which can
		be controlled compareable to the camera in Maya: Rotate with left mouse
		button pressed, Zoom with both buttons pressed, translate with right
		mouse button pressed. This could be created with
		addCameraSceneNodeMaya ( ). But for this example, we want to create a
		camera which behaves like the ones in first person shooter games ( FPS ).
	*/

	scene::ICameraSceneNode*  pCamera = m_pScrMgr->addCameraSceneNodeFPS ( );

	/*
		so we need a good starting Position in the level.
		we can ask the Quake3 Loader for all entities with class_name
		"info_player_deathmatch"
		we choose a random launch
	*/
	if ( pMesh )
	{
		scene::quake3::tQ3EntityList &tEntityList = pMesh->getEntityList ( );

		scene::quake3::IEntity  tSearch;
		tSearch.name = "info_player_deathmatch";

		s32  nIndex = tEntityList.binary_search ( tSearch );
		if ( nIndex >= 0 )
		{
			s32  nNotEndList;
			do
			{
				const scene::quake3::SVarGroup*  pGroup = tEntityList [ nIndex ].getGroup ( 1 );

				u32   uParsepos = 0;
				const core::vector3df  tPos = scene::quake3::getAsVector3df ( pGroup->get ( "origin" ), uParsepos );

				uParsepos = 0;
				const f32  fAngle = scene::quake3::getAsFloat ( pGroup->get ( "angle" ), uParsepos );

				core::vector3df  tTarget ( 0.f, 0.f, 1.f );
				tTarget.rotateXZBy ( fAngle );

				pCamera->setPosition ( tPos );
				pCamera->setTarget ( tPos + tTarget );

				++nIndex;

				nNotEndList = nIndex == 2;

			} while ( nNotEndList );
		}
	}

	/*
		The mouse cursor needs not to be visible, so we make it invisible.
	*/
	s_pDevice->getCursorControl ( )->setVisible ( false );
}

CT16_Quake3MapShader::~CT16_Quake3MapShader ( KDvoid )
{
	
}

video::SColor CT16_Quake3MapShader::getClear ( KDvoid )
{
	return video::SColor ( 255, 20, 20, 40 );
}

bool CT16_Quake3MapShader::OnEvent ( const SEvent& event )
{
	if ( CTutorial::OnEvent ( event ) == true )
	{
		return true;
	}

	// check if user presses the key F9
	if ( ( event.EventType == EET_KEY_INPUT_EVENT ) && event.KeyInput.PressedDown )
	{
		if ( event.KeyInput.Key == KEY_F9 )
		{
			video::IImage*  pImage = s_pDevice->getVideoDriver ( )->createScreenShot ( );
			if ( pImage )
			{
				c8  szStr [256];
				kdSnprintfKHR ( szStr, 256, "%s_shot%04d.jpg", m_sFilenameTemplate.c_str ( ), ++m_uNumber );
				s_pDevice->getVideoDriver ( )->writeImageToFile ( pImage, szStr, 85 );
				pImage->drop ( );
			}
		}
		else if ( event.KeyInput.Key == KEY_F8 )
		{
			if ( m_pNode->isDebugDataVisible ( ) )
			{
				m_pNode->setDebugDataVisible(scene::EDS_OFF);
			}
			else
			{
				m_pNode->setDebugDataVisible(scene::EDS_BBOX_ALL);
			}
		}
	}

	return false;
}

const wchar_t* CT16_Quake3MapShader::getTitle ( KDvoid )
{
	return L"16. Quake3MapShader";
}