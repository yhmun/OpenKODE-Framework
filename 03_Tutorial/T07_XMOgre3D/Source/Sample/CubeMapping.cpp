/* ------------------------------------------------------------------------------------
 *
 *      File            CubeMapping.cpp
 *      Description     This source file is part of OGRE
 *                      (Object-oriented Graphics Rendering Engine)
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd.
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#include "Precompiled.h"
#include "CubeMapping.h"

Sample_CubeMapping::Sample_CubeMapping ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Cube Mapping";
    m_aInfo [ "Description" ] = "Demonstrates the cube mapping feature where a wrap-around environment is reflected "
                                "off of an object. Uses render-to-texture to create dynamic cubemaps.";
    m_aInfo [ "Thumbnail"   ] = "thumb_cubemap.png";
    m_aInfo [ "Category"    ] = "Unsorted";
}

KDvoid Sample_CubeMapping::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
    /*
    if ( !pCaps->hasCapability ( RSC_CUBEMAPPING ) )
    {
        OGRE_EXCEPT ( Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support cube mapping, "
                            "so you cannot run this sample. Sorry!", "CubeMappingSample::testCapabilities" );
    }
    */ 
}

bool Sample_CubeMapping::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pPivot->yaw ( Radian ( tEvent.timeSinceLastFrame ) );        // spin the fishy around the cube mapped one
    m_pFishSwim->addTime ( tEvent.timeSinceLastFrame * 3 );        // make the fishy swim
    return SdkSample::frameRenderingQueued ( tEvent );             // don't forget the parent updates!
}

KDvoid Sample_CubeMapping::preRenderTargetUpdate ( const RenderTargetEvent& tEvent )
{
    m_pHead->setVisible ( false );  // hide the head
    
    // point the camera in the right direction based on which face of the cubemap this is
    m_pCubeCamera->setOrientation ( Quaternion::IDENTITY );
    
    if      ( tEvent.source == m_aTargets[0]) m_pCubeCamera->yaw   ( Degree ( -90 ) );
    else if ( tEvent.source == m_aTargets[1]) m_pCubeCamera->yaw   ( Degree (  90 ) );
    else if ( tEvent.source == m_aTargets[2]) m_pCubeCamera->pitch ( Degree (  90 ) );
    else if ( tEvent.source == m_aTargets[3]) m_pCubeCamera->pitch ( Degree ( -90 ) );
    else if ( tEvent.source == m_aTargets[5]) m_pCubeCamera->yaw   ( Degree ( 180 ) );
}

KDvoid Sample_CubeMapping::postRenderTargetUpdate ( const RenderTargetEvent& tEvent )
{
    m_pHead->setVisible ( true );  // unhide the head
}

KDvoid Sample_CubeMapping::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyDome ( true, "Examples/CloudySky" );
    
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    createCubeMap ( );
    
    // create an ogre head, give it the dynamic cube map material, and place it at the origin
    m_pHead = m_pSceneMgr->createEntity ( "CubeMappedHead", "ogrehead.mesh" );
    m_pHead->setMaterialName ( "Examples/DynamicCubeMap" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( m_pHead );
    
    m_pPivot = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );  // create a pivot node
    
    Entity*  pFish = m_pSceneMgr->createEntity ( "Fish", "fish.mesh" );
    m_pFishSwim = pFish->getAnimationState ( "swim" );
    m_pFishSwim->setEnabled ( true );
    
    // create a child node at an offset and attach a regular ogre head and a nimbus to it
    SceneNode*  pNode = m_pPivot->createChildSceneNode ( Vector3 ( -60, 10, 0 ) );
    pNode->setScale ( 7, 7, 7 );
    pNode->yaw ( Degree ( 90 ) );
    pNode->attachObject ( pFish );
    
    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane(Vector3::UNIT_Y, -30), 1000, 1000, 10, 10, true, 1, 8, 8, Vector3::UNIT_Z
    );
    
    // create a floor entity, give it a material, and place it at the origin
    Entity*  pFloor = m_pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/BumpyMetal" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pFloor );
    
    // set our camera to orbit around the head and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pTrayMgr->showCursor ( );
}

KDvoid Sample_CubeMapping::createCubeMap ( KDvoid )
{
    // create the camera used to render to our cubemap
    m_pCubeCamera = m_pSceneMgr->createCamera ( "CubeMapCamera" );
    m_pCubeCamera->setFOVy ( Degree ( 90 ) );
    m_pCubeCamera->setAspectRatio ( 1 );
    m_pCubeCamera->setFixedYawAxis ( false );
    m_pCubeCamera->setNearClipDistance ( 5 );
    
    // create our dynamic cube map texture
    TexturePtr  pTex = TextureManager::getSingleton ( ).createManual
    (
        "dyncubemap", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_CUBE_MAP, 128, 128, 0, PF_R8G8B8, TU_RENDERTARGET
    );
    
    // assign our camera to all 6 render targets of the texture (1 for each direction)
    for ( KDuint i = 0; i < 6; i++ )
    {
        m_aTargets[i] = pTex->getBuffer ( i )->getRenderTarget ( );
        m_aTargets[i]->addViewport ( m_pCubeCamera )->setOverlaysEnabled ( false );
        m_aTargets[i]->addListener ( this );
    }
}

KDvoid Sample_CubeMapping::cleanupContent ( KDvoid )
{
    m_pSceneMgr->destroyCamera ( m_pCubeCamera );
    MeshManager::getSingleton ( ).remove ( "floor" );
    TextureManager::getSingleton ( ).remove ( "dyncubemap" );
}