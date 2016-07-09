/* ------------------------------------------------------------------------------------
 *
 *      File            Grass.cpp
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
#include "Grass.h"

Sample_Grass::Sample_Grass ( KDvoid )
: GRASS_WIDTH  ( 40 )
, GRASS_HEIGHT ( 40 )
{
    m_aInfo [ "Title"       ] = "Grass";
    m_aInfo [ "Description" ] = "Demonstrates how to use the StaticGeometry class to create 'baked' "
                                "instances of many meshes, to create effects like grass efficiently.";
    m_aInfo [ "Thumbnail"   ] = "thumb_grass.png";
    m_aInfo [ "Category"    ] = "Environment";
    m_aInfo [ "Help"        ] = "Press B to toggle bounding boxes.";
}

bool Sample_Grass::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pLightAnimState->addTime ( tEvent.timeSinceLastFrame );   // move the light around
    waveGrass ( tEvent.timeSinceLastFrame );                    // wave the grass around slowly to simulate wind
    return SdkSample::frameRenderingQueued ( tEvent );          // don't forget the parent class updates!
}

KDvoid Sample_Grass::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/SpaceSkyBox" );
    
    // create a mesh for our ground
    MeshManager::getSingleton ( ).createPlane
    (
        "ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, 0 ), 1000, 1000, 20, 20, true, 1, 6, 6, Vector3::UNIT_Z
    );
        
    // create a ground entity from our mesh and attach it to the origin
    Entity*  pGround = m_pSceneMgr->createEntity ( "Ground", "ground" );
    pGround->setMaterialName ( "Examples/GrassFloor" );
    pGround->setCastShadows ( false );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pGround );

    // create our grass mesh, and create a grass entity from it
    createGrassMesh ( );

    Entity*  pGrass = m_pSceneMgr->createEntity ( "Grass", "grass" );
    
    // create a static geometry field, which we will populate with grass
    m_pField = m_pSceneMgr->createStaticGeometry ( "Field" );
    m_pField->setRegionDimensions ( Vector3 ( 140, 140, 140 ) );
    m_pField->setOrigin ( Vector3 ( 70, 70, 70 ) );
    
    // add grass uniformly throughout the field, with some random variations
    for ( KDint x = -280; x < 280; x += 20 )
    {
        for ( KDint z = -280; z < 280; z += 20 )
        {
            Vector3     tPos ( x + Math::RangeRandom ( -7, 7 ), 0, z + Math::RangeRandom ( -7, 7 ) );
            Quaternion  tOri ( Degree(Math::RangeRandom ( 0, 359 ) ), Vector3::UNIT_Y );
            Vector3     tScale ( 1, Math::RangeRandom ( 0.85, 1.15 ), 1 );
            
            m_pField->addEntity ( pGrass, tPos, tOri, tScale );
        }
    }
    
    m_pField->build ( );  // build our static geometry (bake the grass into it)

    // build tangent vectors for the ogre head mesh
    MeshPtr   pHeadMesh = MeshManager::getSingleton ( ).load ( "ogrehead.mesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    KDushort  uSrc, uDest;
    if ( !pHeadMesh->suggestTangentVectorBuildParams(VES_TANGENT, uSrc, uDest ) )
    {
        pHeadMesh->buildTangentVectors ( VES_TANGENT, uSrc, uDest );
    }
    
    // put an ogre head in the middle of the field
    Entity*  pHead = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    pHead->setMaterialName ( "Examples/OffsetMapping/Specular" );
    m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3 ( 0, 30, 0 ) )->attachObject ( pHead );
    
    setupLighting ( );
    
    // set initial camera position and speed
    m_pCamera->setPosition ( 0, 50, 100 );
}

KDvoid Sample_Grass::createGrassMesh ( KDvoid )
{
    MeshPtr  pMesh = MeshManager::getSingleton ( ).createManual ( "grass", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    
    // create a submesh with the grass material
    SubMesh*  pSubMesh = pMesh->createSubMesh ( );
    pSubMesh->setMaterialName ( "Examples/GrassBlades" );
    pSubMesh->useSharedVertices = false;
    pSubMesh->vertexData = OGRE_NEW VertexData ( );
    pSubMesh->vertexData->vertexStart = 0;
    pSubMesh->vertexData->vertexCount = 12;
    pSubMesh->indexData->indexCount   = 18;
    
    // specify a vertex format declaration for our mesh: 3 floats for position, 3 floats for normal, 2 floats for UV
    VertexDeclaration*  pDecl = pSubMesh->vertexData->vertexDeclaration;
    pDecl->addElement ( 0, 0, VET_FLOAT3, VES_POSITION );
    pDecl->addElement ( 0, sizeof ( KDfloat ) * 3, VET_FLOAT3, VES_NORMAL );
    pDecl->addElement ( 0, sizeof ( KDfloat ) * 6, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );
    
    // create a vertex buffer
    HardwareVertexBufferSharedPtr  pVB = HardwareBufferManager::getSingleton ( ).createVertexBuffer
    (
        pDecl->getVertexSize ( 0 ), pSubMesh->vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
    );
    
    GrassVertex*  aVerts = (GrassVertex*) pVB->lock ( HardwareBuffer::HBL_DISCARD );  // start filling in vertex data
    
    for ( KDuint i = 0; i < 3; i++ )  // each grass mesh consists of 3 planes
    {
        // planes intersect along the Y axis with 60 degrees between them
        Real  x = Math::Cos ( Degree ( i * 60 ) ) * GRASS_WIDTH / 2;
        Real  z = Math::Sin ( Degree ( i * 60 ) ) * GRASS_WIDTH / 2;
        
        for ( KDuint j = 0; j < 4; j++ )  // each plane has 4 vertices
        {
            GrassVertex&  tVert = aVerts [ i * 4 + j ];
            
            tVert.x = j < 2 ? -x : x;
            tVert.y = j % 2 ? 0 : GRASS_HEIGHT;
            tVert.z = j < 2 ? -z : z;
            
            // all normals point straight up
            tVert.nx = 0;
            tVert.ny = 1;
            tVert.nz = 0;
            
            tVert.u = j < 2 ? 0 : 1;
            tVert.v = j % 2;
        }
    }
    
    pVB->unlock ( );  // commit vertex changes
    
    pSubMesh->vertexData->vertexBufferBinding->setBinding ( 0, pVB );  // bind vertex buffer to our submesh
    
    // create an index buffer
    pSubMesh->indexData->indexBuffer = HardwareBufferManager::getSingleton ( ).createIndexBuffer
    (
        HardwareIndexBuffer::IT_16BIT, pSubMesh->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
    );
    
    // start filling in index data
    uint16*  aIndices = (uint16*) pSubMesh->indexData->indexBuffer->lock ( HardwareBuffer::HBL_DISCARD );
    
    for ( KDuint i = 0; i < 3; i++ )  // each grass mesh consists of 3 planes
    {
        KDuint  uOff = i * 4;  // each plane consists of 2 triangles
        
        *aIndices++ = 0 + uOff;
        *aIndices++ = 3 + uOff;
        *aIndices++ = 1 + uOff;
        
        *aIndices++ = 0 + uOff;
        *aIndices++ = 2 + uOff;
        *aIndices++ = 3 + uOff;
    }
    
    pSubMesh->indexData->indexBuffer->unlock ( );  // commit index changes
     
}

KDvoid Sample_Grass::setupLighting ( KDvoid )
{
    m_pSceneMgr->setAmbientLight ( ColourValue::Black );  // turn off ambient light
    
    ColourValue  tLightColour ( 1, 1, 0.3 );
    
    // create a light
    Light*  pLight = m_pSceneMgr->createLight ( );
    pLight->setDiffuseColour ( tLightColour );
    pLight->setSpecularColour ( 1, 1, 0.3 );
    pLight->setAttenuation ( 1500, 1, 0.0005, 0 );
    
    // create a flare
    BillboardSet*  pBillSet = m_pSceneMgr->createBillboardSet ( 1 );
    pBillSet->setMaterialName ( "Examples/Flare" );
    Billboard*  pBill = pBillSet->createBillboard ( 0, 0, 0, tLightColour );
    
    // create a controller for the light intensity, using our LightPulsator class
    ControllerFunctionRealPtr pFunc ( OGRE_NEW WaveformControllerFunction ( WFT_SINE, 0.5, 0.5, 0, 0.5 ) );
    ControllerValueRealPtr  pDest ( OGRE_NEW LightPulse ( pLight, pBill, tLightColour, 15 ) );
    ControllerManager&   tCtrMgr = ControllerManager::getSingleton ( );
    m_pLightController = tCtrMgr.createController ( tCtrMgr.getFrameTimeSource ( ), pDest, pFunc );
    
    // create a light node and attach the light and flare to it
    SceneNode*  pLightNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pLightNode->attachObject ( pLight );
    pLightNode->attachObject ( pBillSet );
    
    // set up a 20 second animation for our light, using spline interpolation for nice curves
    Animation*  pAnim = m_pSceneMgr->createAnimation ( "LightTrack", 20 );
    pAnim->setInterpolationMode ( Animation::IM_SPLINE );
    
    // create a track to animate the camera's node
    NodeAnimationTrack*  pTrack = pAnim->createNodeTrack ( 0, pLightNode );
    
    // create keyframes for our track
    pTrack->createNodeKeyFrame (  0 )->setTranslate ( Vector3 (  42, 77, -42 ) );
    pTrack->createNodeKeyFrame (  2 )->setTranslate ( Vector3 (  21, 84, -35 ) );
    pTrack->createNodeKeyFrame (  4 )->setTranslate ( Vector3 ( -21, 91, -14 ) );
    pTrack->createNodeKeyFrame (  6 )->setTranslate ( Vector3 ( -56, 70, -28 ) );
    pTrack->createNodeKeyFrame (  8 )->setTranslate ( Vector3 ( -28, 70, -56 ) );
    pTrack->createNodeKeyFrame ( 10 )->setTranslate ( Vector3 ( -14, 63, -28 ) );
    pTrack->createNodeKeyFrame ( 12 )->setTranslate ( Vector3 ( -14, 56,  28 ) );
    pTrack->createNodeKeyFrame ( 14 )->setTranslate ( Vector3 (   0, 35,  84 ) );
    pTrack->createNodeKeyFrame ( 16 )->setTranslate ( Vector3 (  14, 35,  14 ) );
    pTrack->createNodeKeyFrame ( 18 )->setTranslate ( Vector3 (  35, 84,   0 ) );
    pTrack->createNodeKeyFrame ( 20 )->setTranslate ( Vector3 (  42, 77, -42 ) );
    
    pLightNode->setPosition ( pTrack->getNodeKeyFrame ( 0 )->getTranslate ( ) );
    
    // create a new animation state to track this
    m_pLightAnimState = m_pSceneMgr->createAnimationState ( "LightTrack" );
    m_pLightAnimState->setEnabled ( true );
}

KDvoid Sample_Grass::waveGrass ( Real fTimeElapsed )
{
    static Real  fIncX = Math::PI * 0.3;
    static Real  fIncZ = Math::PI * 0.44;
    static Real  fPosX = Math::RangeRandom ( -Math::PI, Math::PI );
    static Real  fPosZ = Math::RangeRandom ( -Math::PI, Math::PI );
    static Vector4  tOffset ( 0, 0, 0, 0 );
    
    fPosX += fIncX * fTimeElapsed;
    fPosZ += fIncZ * fTimeElapsed;
    
    // update vertex program parameters by binding a value to each renderable
    StaticGeometry::RegionIterator  itRegs =  m_pField->getRegionIterator ( );
    while ( itRegs.hasMoreElements ( ) )
    {
        StaticGeometry::Region*  pReg = itRegs.getNext ( );
        
        // a little randomness
        fPosX += pReg->getCentre ( ).x * 0.001;
        fPosZ += pReg->getCentre ( ).z * 0.001;
        tOffset.x = Math::Sin ( fPosX ) * 4;
        tOffset.z = Math::Sin ( fPosZ ) * 4;
        
        StaticGeometry::Region::LODIterator itLods = pReg->getLODIterator ( );
        while ( itLods.hasMoreElements ( ) )
        {
            StaticGeometry::LODBucket::MaterialIterator  itMats = itLods.getNext ( )->getMaterialIterator ( );
            while ( itMats.hasMoreElements ( ) )
            {
                StaticGeometry::MaterialBucket::GeometryIterator itGeoms = itMats.getNext ( )->getGeometryIterator ( );
                while ( itGeoms.hasMoreElements ( ) )
                {
                    itGeoms.getNext ( )->setCustomParameter ( 999, tOffset );
                }
            }
        }
    }
}

KDvoid Sample_Grass::cleanupContent ( KDvoid )
{
    ControllerManager::getSingleton ( ).destroyController ( m_pLightController );
    MeshManager::getSingleton ( ).remove ( "ground" );
    MeshManager::getSingleton ( ).remove ( "grass" );
}

//--------------------------------------------------------------------------
//
//  LightPulse
//
//--------------------------------------------------------------------------

LightPulse::LightPulse ( Light* pLight, Billboard* pBillboard, const ColourValue& tMaxColour, Real fMaxSize )
{
    m_pLight     = pLight;
    m_pBillboard = pBillboard;
    m_tMaxColour = tMaxColour;
    m_fMaxSize   = fMaxSize;
}

Real LightPulse::getValue ( KDvoid ) const
{
    return m_fIntensity;
}

KDvoid LightPulse::setValue ( Real fValue )
{
    m_fIntensity = fValue;
    
    // calculate new colour and apply it to the light and billboard
    ColourValue  tNewColour = m_tMaxColour * m_fIntensity;
    m_pLight->setDiffuseColour ( tNewColour );
    m_pBillboard->setColour ( tNewColour );
    
    // calculate new billboard size and apply it
    Real  fNewSize = m_fMaxSize * m_fIntensity;
    m_pBillboard->setDimensions ( fNewSize, fNewSize );
}
