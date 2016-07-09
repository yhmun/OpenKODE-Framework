/* --------------------------------------------------------------------------
 *
 *      File            CDemo.h
 *      Description     This is a Demo of the Irrlicht Engine ( c ) 2006 by N.Gebhardt.
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
#include "CDemo.h"

CDemo::CDemo ( KDint nWidth, KDint nHeight )
{
	m_pDevice = createDevice 
	( 
		video::EDT_OGLES1, 
		core::dimension2d<u32> ( nWidth, nHeight ),
		32
	);

	m_pDriver = m_pDevice->getVideoDriver    ( );
	m_pScrMgr = m_pDevice->getSceneManager   ( );
	m_pGuiEnv = m_pDevice->getGUIEnvironment ( );

	m_pDevice->setEventReceiver ( this );
	m_pDevice->getFileSystem ( )->addFileArchive ( "/res/Media/irrlicht.dat" );
	m_pDevice->getFileSystem ( )->addFileArchive ( "/res/Media/map-20kdm2.pk3" );

	m_pScrMgr->setAmbientLight ( video::SColorf ( 0x00c0c0c0 ) );

	m_nCurrentScene     = -2;
	m_tBackColor        = 0;
	m_nSceneStartTime   = m_pDevice->getTimer ( )->getTime ( );
	m_nTimeForThisScene = 0;
	m_pStatusText		= 0;
	m_pInOutFader		= 0;
	m_pModel1			= 0;
	m_pModel2			= 0;
	m_pCampFire			= 0;
	m_pQuakeLevelMesh	= 0;
	m_pQuakeLevelNode	= 0;
	m_pSkyboxNode		= 0;
	m_pMetaSelector		= 0;
	m_pMapSelector		= 0;
	m_bAdditive         = false;
	m_nKeyCode          = 0;

	// No Delay
	xmSetFrameInterval ( 0 );
}

CDemo::~CDemo ( KDvoid )
{
	if ( m_pMapSelector )
	{
		m_pMapSelector->drop ( );
	}

	if ( m_pMetaSelector )
	{
		m_pMetaSelector->drop ( );
	}

	m_pDevice->drop ( );
}

KDvoid CDemo::EventProc ( const KDEvent* event )
{
	m_pDevice->EventProc ( event );
}

KDbool CDemo::Draw ( KDvoid )
{
	if ( m_pDevice->run ( ) )
	{
		// load next scene if necespAnimatorry
		s32  nNow = m_pDevice->getTimer ( )->getTime ( );

		if ( nNow - m_nSceneStartTime > m_nTimeForThisScene && m_nTimeForThisScene != -1 )
		{
			SwitchToNextScene ( );
		}

		if ( m_nKeyCode != 0 )
		{
			scene::ICameraSceneNode*  pCamera = m_pScrMgr->getActiveCamera ( );
			core::vector3df  tRotate = pCamera->getRotation ( );

			tRotate.Y += m_nKeyCode == KEY_KEY_Q ? -3 : 3;
			pCamera->setRotation ( tRotate );
		}

		CreateParticleImpacts ( );

		m_pDriver->beginScene ( m_nTimeForThisScene != -1, true, m_tBackColor );

		m_pScrMgr->drawAll ( );
		m_pGuiEnv->drawAll ( );

		m_pDriver->endScene ( );
	}
	else
	{
		return KD_FALSE;
	}

#if !defined ( SHP ) && !defined ( WINCE )
	wchar_t  sStr [ 255 ];
	swprintf ( sStr, 255, L"%ls fps:%3d triangles:%0.3f mio/s",
				m_pDriver->getName ( ), m_pDriver->getFPS ( ),
				m_pDriver->getPrimitiveCountDrawn ( 1 ) * ( 1.f / 1000000.f ) );
	m_pStatusText->setText ( sStr );
#endif

	return KD_TRUE;
}

bool CDemo::OnEvent ( const SEvent& event )
{
	if ( !m_pDevice )
	{
		return false;
	}

	if ( event.EventType == EET_KEY_INPUT_EVENT &&
		 event.KeyInput.Key == KEY_ESCAPE &&
	  	 event.KeyInput.PressedDown == false )
	{
		// user wants to quit.
		if ( m_nCurrentScene < 3 )
		{
			m_nTimeForThisScene = 0;
		}
		else
		{
			m_nTimeForThisScene = 0;
			//m_pDevice->closeDevice ( );
		}
	}
	else if ( ( 
		   ( event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_SPACE && event.KeyInput.PressedDown == false ) 
		   //( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP ) ) &&
		   && m_nCurrentScene == 3 ) )
	{
		// shoot
		Shoot ( );
	}
	else if ( event.EventType == EET_KEY_INPUT_EVENT &&
			  event.KeyInput.Key == KEY_F9 &&
			  event.KeyInput.PressedDown == false )
	{
		video::IImage*  pImage = m_pDevice->getVideoDriver ( )->createScreenShot ( );
		if ( pImage )
		{
			m_pDriver->writeImageToFile ( pImage, "screenshot.bmp" );
			m_pDriver->writeImageToFile ( pImage, "screenshot.png" );
			m_pDriver->writeImageToFile ( pImage, "screenshot.tga" );
			m_pDriver->writeImageToFile ( pImage, "screenshot.ppm" );
			m_pDriver->writeImageToFile ( pImage, "screenshot.jpg" );
			m_pDriver->writeImageToFile ( pImage, "screenshot.pcx" );
			pImage->drop ( );
		}
	}
	else if ( event.EventType == EET_KEY_INPUT_EVENT &&
            ( event.KeyInput.Key == KEY_KEY_Q || event.KeyInput.Key == KEY_KEY_E ) )
	{
		m_nKeyCode = event.KeyInput.PressedDown == true ? event.KeyInput.Key : 0;
	}
	else if ( m_pScrMgr->getActiveCamera ( ) )
	{
		m_pScrMgr->getActiveCamera ( )->OnEvent ( event );
		return true;
	}

	return false;
}


KDvoid CDemo::SwitchToNextScene ( KDvoid )
{
	m_nCurrentScene++;
	if ( m_nCurrentScene > 3 )
	{
		m_nCurrentScene = 1;
	}

	scene::ISceneNodeAnimator* pAnimator = 0;
	scene::ICameraSceneNode*   pCamera   = 0;

	pCamera = m_pScrMgr->getActiveCamera ( );
	if ( pCamera )
	{
		m_pScrMgr->setActiveCamera ( 0 );
		pCamera->remove ( );
		pCamera = 0;
	}

	switch ( m_nCurrentScene )
	{
		case -1 : // loading screen
			m_nTimeForThisScene = 0;
			CreateLoadingScreen ( );
			break;

		case 0 : // load scene
			m_nTimeForThisScene = 0;
			LoadSceneData ( );
			break;

		case 1 : // panorama pCamera
		{
			m_nCurrentScene += 1;
			//pCamera = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df ( 0,0,0 ), core::vector3df ( -586,708,52 ) );
			//pCamera->setTarget ( core::vector3df ( 0,400,0 ) );

			core::array<core::vector3df> aPoints;

			aPoints.push_back ( core::vector3df ( -931.473755f, 138.300003f, 987.279114f ) ); // -49873
			aPoints.push_back ( core::vector3df ( -847.902222f, 136.757553f, 915.792725f ) ); // -50559
			aPoints.push_back ( core::vector3df ( -748.680420f, 152.254501f, 826.418945f ) ); // -51964
			aPoints.push_back ( core::vector3df ( -708.428406f, 213.569580f, 784.466675f ) ); // -53251
			aPoints.push_back ( core::vector3df ( -686.217651f, 288.141174f, 762.965576f ) ); // -54015
			aPoints.push_back ( core::vector3df ( -679.685059f, 365.095612f, 756.551453f ) ); // -54733
			aPoints.push_back ( core::vector3df ( -671.317871f, 447.360107f, 749.394592f ) ); // -55588
			aPoints.push_back ( core::vector3df ( -669.468445f, 583.335632f, 747.711853f ) ); // -56178
			aPoints.push_back ( core::vector3df ( -667.611267f, 727.313232f, 746.018250f ) ); // -56757
			aPoints.push_back ( core::vector3df ( -665.853210f, 862.791931f, 744.436096f ) ); // -57859
			aPoints.push_back ( core::vector3df ( -642.649597f, 1026.047607f, 724.259827f ) ); // -59705
			aPoints.push_back ( core::vector3df ( -517.793884f, 838.396790f, 490.326050f ) ); // -60983
			aPoints.push_back ( core::vector3df ( -474.387299f, 715.691467f, 344.639984f ) ); // -61629
			aPoints.push_back ( core::vector3df ( -444.600250f, 601.155701f, 180.938095f ) ); // -62319
			aPoints.push_back ( core::vector3df ( -414.808899f, 479.691406f, 4.866660f ) ); // -63048
			aPoints.push_back ( core::vector3df ( -410.418945f, 429.642242f, -134.332687f ) ); // -63757
			aPoints.push_back ( core::vector3df ( -399.837585f, 411.498383f, -349.350983f ) ); // -64418
			aPoints.push_back ( core::vector3df ( -390.756653f, 403.970093f, -524.454407f ) ); // -65005
			aPoints.push_back ( core::vector3df ( -334.864227f, 350.065491f, -732.397400f ) ); // -65701
			aPoints.push_back ( core::vector3df ( -195.253387f, 349.577209f, -812.475891f ) ); // -66335
			aPoints.push_back ( core::vector3df ( 16.255573f, 363.743134f, -833.800415f ) ); // -67170
			aPoints.push_back ( core::vector3df ( 234.940964f, 352.957825f, -820.150696f ) ); // -67939
			aPoints.push_back ( core::vector3df ( 436.797668f, 349.236450f, -816.914185f ) ); // -68596
			aPoints.push_back ( core::vector3df ( 575.236206f, 356.244812f, -719.788513f ) ); // -69166
			aPoints.push_back ( core::vector3df ( 594.131042f, 387.173828f, -609.675598f ) ); // -69744
			aPoints.push_back ( core::vector3df ( 617.615234f, 412.002899f, -326.174072f ) ); // -70640
			aPoints.push_back ( core::vector3df ( 606.456848f, 403.221954f, -104.179291f ) ); // -71390
			aPoints.push_back ( core::vector3df ( 610.958252f, 407.037750f, 117.209778f ) ); // -72085
			aPoints.push_back ( core::vector3df ( 597.956909f, 395.167877f, 345.942200f ) ); // -72817
			aPoints.push_back ( core::vector3df ( 587.383118f, 391.444519f, 566.098633f ) ); // -73477
			aPoints.push_back ( core::vector3df ( 559.572449f, 371.991333f, 777.689453f ) ); // -74124
			aPoints.push_back ( core::vector3df ( 423.753204f, 329.990051f, 925.859741f ) ); // -74941
			aPoints.push_back ( core::vector3df ( 247.520050f, 252.818954f, 935.311829f ) ); // -75651
			aPoints.push_back ( core::vector3df ( 114.756012f, 199.799759f, 805.014160f ) );
			aPoints.push_back ( core::vector3df ( 96.783348f, 181.639481f, 648.188110f ) );
			aPoints.push_back ( core::vector3df ( 97.865623f, 138.905975f, 484.812561f ) );
			aPoints.push_back ( core::vector3df ( 99.612457f, 102.463669f, 347.603210f ) );
			aPoints.push_back ( core::vector3df ( 99.612457f, 102.463669f, 347.603210f ) );
			aPoints.push_back ( core::vector3df ( 99.612457f, 102.463669f, 347.603210f ) );

			m_nTimeForThisScene = ( aPoints.size ( ) - 3 ) * 1000;

			pCamera = m_pScrMgr->addCameraSceneNode ( 0, aPoints[0], core::vector3df ( 0, 400, 0 ) );
			//pCamera->setTarget ( core::vector3df ( 0, 400, 0 ) );

			pAnimator = m_pScrMgr->createFollowSplineAnimator ( m_pDevice->getTimer ( )->getTime ( ), aPoints );
			pCamera->addAnimator ( pAnimator );
			pAnimator->drop ( );

			m_pModel1->setVisible ( false );
			m_pModel2->setVisible ( false );
			m_pCampFire->setVisible ( false );
			m_pInOutFader->fadeIn ( 7000 );
		}
		break;

		case 2 :	// down fly anim pCamera
			pCamera = m_pScrMgr->addCameraSceneNode ( 0, core::vector3df ( 100,40,-80 ), core::vector3df ( 844,670,-885 ) );
			pAnimator = m_pScrMgr->createFlyStraightAnimator ( core::vector3df ( 94, 1002, 127 ), core::vector3df ( 108, 15, -60 ), 10000, true );
			pCamera->addAnimator ( pAnimator );
			m_nTimeForThisScene = 9900;
			m_pModel1->setVisible ( true );
			m_pModel2->setVisible ( false );
			m_pCampFire->setVisible ( false );
			pAnimator->drop ( );
			break;

		case 3: // interactive, go around
			{
				m_pModel1->setVisible ( true );
				m_pModel2->setVisible ( true );
				m_pCampFire->setVisible ( true );
				m_nTimeForThisScene = -1;

				SKeyMap  tKeyMap [ 9 ];

				tKeyMap[0].Action  = EKA_MOVE_FORWARD;
				tKeyMap[0].KeyCode = KEY_UP;
				tKeyMap[1].Action  = EKA_MOVE_FORWARD;
				tKeyMap[1].KeyCode = KEY_KEY_W;

				tKeyMap[2].Action  = EKA_MOVE_BACKWARD;
				tKeyMap[2].KeyCode = KEY_DOWN;
				tKeyMap[3].Action  = EKA_MOVE_BACKWARD;
				tKeyMap[3].KeyCode = KEY_KEY_S;

				tKeyMap[4].Action  = EKA_STRAFE_LEFT;
				tKeyMap[4].KeyCode = KEY_LEFT;
				tKeyMap[5].Action  = EKA_STRAFE_LEFT;
				tKeyMap[5].KeyCode = KEY_KEY_A;

				tKeyMap[6].Action  = EKA_STRAFE_RIGHT;
				tKeyMap[6].KeyCode = KEY_RIGHT;
				tKeyMap[7].Action  = EKA_STRAFE_RIGHT;
				tKeyMap[7].KeyCode = KEY_KEY_D;

				tKeyMap[8].Action  = EKA_JUMP_UP;
				tKeyMap[8].KeyCode = KEY_KEY_J;

				pCamera = m_pScrMgr->addCameraSceneNodeFPS ( 0, 100.0f, .4f, -1, tKeyMap, 9, false, 3.f );
				pCamera->setPosition ( core::vector3df ( 108,140,-140 ) );

				scene::ISceneNodeAnimatorCollisionResponse*  pCollider =
					m_pScrMgr->createCollisionResponseAnimator 
					( 
						m_pMetaSelector, pCamera, core::vector3df ( 25,50,25 ),
						core::vector3df ( 0, m_pQuakeLevelMesh ? -10.f : 0.0f,0 ),
						core::vector3df ( 0,45,0 ), 0.005f 
					);

				pCamera->addAnimator ( pCollider );
				pCollider->drop ( );
			}
			break;
	}

	m_nSceneStartTime = m_pDevice->getTimer ( )->getTime ( );
}


KDvoid CDemo::LoadSceneData ( KDvoid )
{
	// load m_pQuake level

	// Quake3 Shader controls Z-Writing
	m_pScrMgr->getParameters ( )->setAttribute ( scene::ALLOW_ZWRITE_ON_TRANSPARENT, true );

	m_pQuakeLevelMesh = (scene::IQ3LevelMesh*) m_pScrMgr->getMesh ( "maps/20kdm2.bsp" );

	if ( m_pQuakeLevelMesh )
	{
		u32 i;

		//move all m_pQuake level meshes ( non-realtime )
		core::matrix4 m;
		m.setTranslation ( core::vector3df ( -1300,-70,-1249 ) );

		for ( i = 0; i != scene::quake3::E_Q3_MESH_SIZE; ++i )
		{
			m_pScrMgr->getMeshManipulator ( )->transform ( m_pQuakeLevelMesh->getMesh ( i ), m );
		}

		m_pQuakeLevelNode = m_pScrMgr->addOctreeSceneNode ( m_pQuakeLevelMesh->getMesh ( scene::quake3::E_Q3_MESH_GEOMETRY ) );
		if ( m_pQuakeLevelNode )
		{
			//m_pQuakeLevelNode->setPosition ( core::vector3df ( -1300,-70,-1249 ) );
			m_pQuakeLevelNode->setVisible ( true );

			// create map triangle selector
			m_pMapSelector = m_pScrMgr->createOctreeTriangleSelector ( m_pQuakeLevelMesh->getMesh ( 0 ), m_pQuakeLevelNode, 128 );

			// if not using shader and no gamma it's better to use more lighting, because
			// m_pQuake3 level are usually dark
			m_pQuakeLevelNode->setMaterialType ( video::EMT_LIGHTMAP_M4 );

			// set additive blending if wanted
			if ( m_bAdditive )
			{
				m_pQuakeLevelNode->setMaterialType ( video::EMT_LIGHTMAP_ADD );
			}
		}

		// the additional mesh can be quite huge and is unoptimized
		scene::IMesh* additional_mesh = m_pQuakeLevelMesh->getMesh ( scene::quake3::E_Q3_MESH_ITEMS );

		for ( i = 0; i!= additional_mesh->getMeshBufferCount ( ); ++i )
		{
			scene::IMeshBuffer *meshBuffer = additional_mesh->getMeshBuffer ( i );
			const video::SMaterial &material = meshBuffer->getMaterial ( );

			//! The ShaderIndex is stored in the material parameter
			s32 shaderIndex = ( s32 ) material.MaterialTypeParam2;

			// the meshbuffer can be rendered without additional support, or it has no shader
			const scene::quake3::IShader *shader = m_pQuakeLevelMesh->getShader ( shaderIndex );
			if ( 0 == shader )
			{
				continue;
			}
			// Now add the MeshBuffer ( s ) with the current Shader to the Manager
			m_pScrMgr->addQuake3SceneNode ( meshBuffer, shader );
		}
	}

	// load sydney m_pModel and create 2 instances

	scene::IAnimatedMesh* mesh = 0;
	mesh = m_pScrMgr->getMesh ( "/res/Media/sydney.md2" );
	if ( mesh )
	{
		m_pModel1 = m_pScrMgr->addAnimatedMeshSceneNode ( mesh );
		if ( m_pModel1 )
		{
			m_pModel1->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/spheremap.jpg" ) );
			m_pModel1->setPosition ( core::vector3df ( 100,40,-80 ) );
			m_pModel1->setScale ( core::vector3df ( 2,2,2 ) );
			m_pModel1->setMD2Animation ( scene::EMAT_STAND );
			m_pModel1->setMaterialFlag ( video::EMF_LIGHTING, false );
			m_pModel1->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
			m_pModel1->setMaterialType ( video::EMT_SPHERE_MAP );
			m_pModel1->addShadowVolumeSceneNode ( );
		}

		m_pModel2 = m_pScrMgr->addAnimatedMeshSceneNode ( mesh );
		if ( m_pModel2 )
		{
			m_pModel2->setPosition ( core::vector3df ( 180,15,-60 ) );
			m_pModel2->setScale ( core::vector3df ( 2,2,2 ) );
			m_pModel2->setMD2Animation ( scene::EMAT_RUN );
			m_pModel2->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/sydney.bmp" ) );
			m_pModel2->setMaterialFlag ( video::EMF_LIGHTING, true );
			m_pModel1->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
			m_pModel2->addShadowVolumeSceneNode ( );
		}
	}

	scene::ISceneNodeAnimator* anim = 0;

	// create sky box
	m_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, false );
	m_pSkyboxNode = m_pScrMgr->addSkyBoxSceneNode 
	( 
		m_pDriver->getTexture ( "/res/Media/irrlicht2_up.jpg" ),
		m_pDriver->getTexture ( "/res/Media/irrlicht2_dn.jpg" ),
		m_pDriver->getTexture ( "/res/Media/irrlicht2_lf.jpg" ),
		m_pDriver->getTexture ( "/res/Media/irrlicht2_rt.jpg" ),
		m_pDriver->getTexture ( "/res/Media/irrlicht2_ft.jpg" ),
		m_pDriver->getTexture ( "/res/Media/irrlicht2_bk.jpg" ) 
	);
	m_pDriver->setTextureCreationFlag ( video::ETCF_CREATE_MIP_MAPS, true );

	// create walk-between-portals animation

	core::vector3df waypoint[2];
	waypoint[0].set ( -150,40,100 );
	waypoint[1].set ( 350,40,100 );

	if ( m_pModel2 )
	{
		anim = m_pDevice->getSceneManager ( )->createFlyStraightAnimator ( 
			waypoint[0], waypoint[1], 2000, true );
		m_pModel2->addAnimator ( anim );
		anim->drop ( );
	}

	// create animation for portals;

	core::array<video::ITexture*> textures;
	for ( s32 g=1; g<8; ++g )
	{
		core::stringc tmp ( "/res/Media/portal" );
		tmp += g;
		tmp += ".bmp";
		video::ITexture* t = m_pDriver->getTexture ( tmp );
		textures.push_back ( t );
	}

	anim = m_pScrMgr->createTextureAnimator ( textures, 100 );

	// create portals

	scene::IBillboardSceneNode* bill = 0;

	for ( int r=0; r<2; ++r )
	{
		bill = m_pScrMgr->addBillboardSceneNode ( 0, core::dimension2d<f32> ( 100,100 ),
			waypoint[r]+ core::vector3df ( 0,20,0 ) );
		bill->setMaterialFlag ( video::EMF_LIGHTING, false );
		bill->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/portal1.bmp" ) );
		bill->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
		bill->addAnimator ( anim );
	}

	anim->drop ( );

	// create cirlce flying dynamic light with transparent billboard attached

	scene::ILightSceneNode* light = 0;

	light = m_pScrMgr->addLightSceneNode ( 0, core::vector3df ( 0,0,0 ),	video::SColorf ( 1.0f, 1.0f, 1.f, 1.0f ), 500.f );

	anim = m_pScrMgr->createFlyCircleAnimator ( core::vector3df ( 100,150,80 ), 80.0f, 0.0005f );

	light->addAnimator ( anim );
	anim->drop ( );

	bill = m_pDevice->getSceneManager ( )->addBillboardSceneNode ( light, core::dimension2d<f32> ( 40,40 ) );
	bill->setMaterialFlag ( video::EMF_LIGHTING, false );
	bill->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/particlewhite.bmp" ) );
	bill->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );

	// create meta triangle selector with all triangles selectors in it.
	m_pMetaSelector = m_pScrMgr->createMetaTriangleSelector ( );
	m_pMetaSelector->addTriangleSelector ( m_pMapSelector );

	// create camp fire

	m_pCampFire = m_pScrMgr->addParticleSystemSceneNode ( false );
	m_pCampFire->setPosition ( core::vector3df ( 100,120,600 ) );
	m_pCampFire->setScale ( core::vector3df ( 2,2,2 ) );

	scene::IParticleEmitter* em = m_pCampFire->createBoxEmitter
	( 
		core::aabbox3d<f32> ( -7,0,-7,7,1,7 ),
		core::vector3df ( 0.0f,0.06f,0.0f ),
		80,100, video::SColor ( 1,255,255,255 ),video::SColor ( 1,255,255,255 ), 800,2000 
	);

	em->setMinStartSize ( core::dimension2d<f32> ( 20.0f, 10.0f ) );
	em->setMaxStartSize ( core::dimension2d<f32> ( 20.0f, 10.0f ) );
	m_pCampFire->setEmitter ( em );
	em->drop ( );

	scene::IParticleAffector* paf = m_pCampFire->createFadeOutParticleAffector ( );
	m_pCampFire->addAffector ( paf );
	paf->drop ( );

	m_pCampFire->setMaterialFlag ( video::EMF_LIGHTING, false );
	m_pCampFire->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
	m_pCampFire->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/fireball.bmp" ) );
	m_pCampFire->setMaterialType ( video::EMT_ONETEXTURE_BLEND );
	m_pCampFire->getMaterial ( 0 ).MaterialTypeParam=video::pack_textureBlendFunc ( video::EBF_ONE, video::EBF_ONE_MINUS_SRC_ALPHA, video::EMFN_MODULATE_1X, video::EAS_VERTEX_COLOR );
}


KDvoid CDemo::CreateLoadingScreen ( KDvoid )
{
	core::dimension2d<u32>  size = m_pDriver->getScreenSize ( );

	m_pDevice->getCursorControl ( )->setVisible ( false );

	// setup loading screen

	m_tBackColor.set ( 255, 90, 90, 156 );

	// create in fader

	m_pInOutFader = m_pDevice->getGUIEnvironment ( )->addInOutFader ( );
	m_pInOutFader->setColor ( m_tBackColor,	video::SColor ( 0, 230, 230, 230 ) );

	// irrlicht logo
	m_pDevice->getGUIEnvironment ( )->addImage ( m_pDriver->getTexture ( "/res/Media/irrlichtlogo2.png" ), core::position2d<s32> ( 5,5 ) );

	// loading text

	const int lwidth = size.Width - 20;
	const int lheight = 16;

	core::rect<int> pos ( 10, size.Height - lheight - 40, 10 + lwidth, size.Height - 40 );

	m_pDevice->getGUIEnvironment ( )->addImage ( pos );
	m_pStatusText = m_pDevice->getGUIEnvironment ( )->addStaticText ( L"Loading...",	pos, true );
	m_pStatusText->setOverrideColor ( video::SColor ( 255, 205, 200, 200 ) );

	// load bigger font
	m_pDevice->getGUIEnvironment ( )->getSkin ( )->setFont ( m_pDevice->getGUIEnvironment ( )->getFont ( "/res/Media/fonthaettenschweiler.bmp" ) );

	// set new font color
	m_pDevice->getGUIEnvironment ( )->getSkin ( )->setColor ( gui::EGDC_BUTTON_TEXT, video::SColor ( 255,100,100,100 ) );
}


KDvoid CDemo::Shoot ( KDvoid )
{
	scene::ICameraSceneNode* pCamera = m_pScrMgr->getActiveCamera ( );

	if ( !pCamera || !m_pMapSelector )
	{
		return;
	}

	SParticleImpact imp;
	imp.when = 0;

	// get line of pCamera

	core::vector3df start = pCamera->getPosition ( );
	core::vector3df end = ( pCamera->getTarget ( ) - start );
	end.normalize ( );
	start += end*8.0f;
	end = start + ( end * pCamera->getFarValue ( ) );

	core::triangle3df triangle;

	core::line3d<f32> line ( start, end );

	// get intersection point with map
	scene::ISceneNode* hitNode;
	if ( m_pScrMgr->getSceneCollisionManager ( )->getCollisionPoint ( line, m_pMapSelector, end, triangle, hitNode ) )
	{
		// collides with wall
		core::vector3df out = triangle.getNormal ( );
		out.setLength ( 0.03f );

		imp.when = 1;
		imp.outVector = out;
		imp.pos = end;
	}
	else
	{
		// doesnt collide with wall
		core::vector3df start = pCamera->getPosition ( );
		core::vector3df end = ( pCamera->getTarget ( ) - start );
		end.normalize ( );
		start += end*8.0f;
		end = start + ( end * pCamera->getFarValue ( ) );
	}

	// create fire ball
	scene::ISceneNode* node = 0;
	node = m_pScrMgr->addBillboardSceneNode ( 0, core::dimension2d<f32> ( 25,25 ), start );

	node->setMaterialFlag ( video::EMF_LIGHTING, false );
	node->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/fireball.bmp" ) );
	node->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );

	f32 length = ( f32 ) ( end - start ).getLength ( );
	const f32 speed = 0.6f;
	u32 time = ( u32 ) ( length / speed );

	scene::ISceneNodeAnimator* anim = 0;

	// set flight line

	anim = m_pScrMgr->createFlyStraightAnimator ( start, end, time );
	node->addAnimator ( anim );
	anim->drop ( );

	anim = m_pScrMgr->createDeleteAnimator ( time );
	node->addAnimator ( anim );
	anim->drop ( );

	if ( imp.when )
	{
		// create impact note
		imp.when = m_pDevice->getTimer ( )->getTime ( ) + ( time - 100 );
		m_aImpacts.push_back ( imp );
	}
}

KDvoid CDemo::CreateParticleImpacts ( KDvoid )
{
	u32 now = m_pDevice->getTimer ( )->getTime ( );

	for ( u32 i = 0; i < m_aImpacts.size ( ); ++i )
	{
		if ( now > m_aImpacts[i].when )
		{
			// create m_pScrMgroke particle system
			scene::IParticleSystemSceneNode* pas = 0;

			pas = m_pScrMgr->addParticleSystemSceneNode ( false, 0, -1, m_aImpacts[i].pos );

			pas->setParticleSize ( core::dimension2d<f32> ( 10.0f, 10.0f ) );

			scene::IParticleEmitter* em = pas->createBoxEmitter 
			( 
				core::aabbox3d<f32> ( -5,-5,-5,5,5,5 ),
				m_aImpacts[i].outVector, 20,40, video::SColor ( 0,255,255,255 ),video::SColor ( 0,255,255,255 ),
				1200,1600, 20 
			);

			pas->setEmitter ( em );
			em->drop ( );

			scene::IParticleAffector* paf = m_pCampFire->createFadeOutParticleAffector ( );
			pas->addAffector ( paf );
			paf->drop ( );

			pas->setMaterialFlag ( video::EMF_LIGHTING, false );
			pas->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
			pas->setMaterialTexture ( 0, m_pDriver->getTexture ( "/res/Media/smoke.bmp" ) );
			pas->setMaterialType ( video::EMT_TRANSPARENT_VERTEX_ALPHA );

			scene::ISceneNodeAnimator* anim = m_pScrMgr->createDeleteAnimator ( 2000 );
			pas->addAnimator ( anim );
			anim->drop ( );

			// delete entry
			m_aImpacts.erase ( i );
			i--;
		}
	}
}
