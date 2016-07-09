/* ------------------------------------------------------------------------------------
 *
 *      File            DualQuaternion.cpp
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
#include "DualQuaternion.h"

Sample_DualQuaternion::Sample_DualQuaternion ( KDvoid )
{
    m_pEntity    = 0;
    m_pEntityDQ  = 0;
    m_fTotalTime = 0;
    
#ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
    m_pSrsHardwareSkinning = 0;
#endif

    m_aInfo [ "Title"       ] = "Dual Quaternion Skinning";
    m_aInfo [ "Description" ] = "A demo of the dual quaternion skinning feature in conjunction with the linear skinning feature.";
    m_aInfo [ "Thumbnail"   ] = "thumb_dualquaternionskinning.png";
    m_aInfo [ "Category"    ] = "Animation";
}

bool Sample_DualQuaternion::frameRenderingQueued ( const FrameEvent& tEvent )
{
    const Real  fStart = 30;
    const Real  fRange = 145;
    const Real  fSpeed = 1;
    const Vector3  tVec = Vector3 ( 1, 0.3, 0 ).normalisedCopy ( );
    m_fTotalTime += tEvent.timeSinceLastFrame;
    
    Quaternion  tOrient = Quaternion ( Degree ( fStart + Math::Sin ( m_fTotalTime * fSpeed ) * fRange ), tVec );
    m_pEntity  ->getSkeleton ( )->getBone ( "Bone02" )->setOrientation ( tOrient );
    m_pEntityDQ->getSkeleton ( )->getBone ( "Bone02" )->setOrientation ( tOrient );
    
    return SdkSample::frameRenderingQueued ( tEvent );
}

KDvoid Sample_DualQuaternion::setupContent ( KDvoid )
{
    OGRE_EXCEPT
    (
        Exception::ERR_NOT_IMPLEMENTED, "Not Support DDS Format"
        ", so you cannot run this sample. Sorry!", "Sample_DualQuaternion::setupContent"
    );
    
#ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
  /*
    //Add the hardware skinning to the shader generator default render state
    m_pSrsHardwareSkinning = m_pShaderGenerator->createSubRenderState ( RTShader::HardwareSkinning::Type );
    RTShader::RenderState*  pRenderState = m_pShaderGenerator->getRenderState ( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME );
    pRenderState->addTemplateSubRenderState ( m_pSrsHardwareSkinning );
    
    MaterialPtr  pCast1 = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_dq_skinning_1weight_twophase" );
    MaterialPtr  pCast2 = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_dq_skinning_2weight_twophase" );
    MaterialPtr  pCast3 = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_dq_skinning_3weight_twophase" );
    MaterialPtr  pCast4 = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_dq_skinning_4weight_twophase" );
    
    RTShader::HardwareSkinningFactory::getSingleton ( ).setCustomShadowCasterMaterials ( RTShader::ST_DUAL_QUATERNION, pCast1, pCast2, pCast3, pCast4 );
    
    MaterialPtr  pCast1l = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_skinning_1weight" );
    MaterialPtr  pCast2l = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_skinning_2weight" );
    MaterialPtr  pCast3l = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_skinning_3weight" );
    MaterialPtr  pCast4l = MaterialManager::getSingleton ( ).getByName ( "Ogre/RTShader/shadow_caster_skinning_4weight" );
    
    RTShader::HardwareSkinningFactory::getSingleton().setCustomShadowCasterMaterials(RTShader::ST_LINEAR, pCast1l, pCast2l, pCast3l, pCast4l);
   */
#endif
    
    // set shadow properties
    m_pSceneMgr->setShadowTechnique ( SHADOWTYPE_TEXTURE_MODULATIVE );
    m_pSceneMgr->setShadowTextureSize ( 2048 );
    m_pSceneMgr->setShadowColour ( ColourValue ( 0.6, 0.6, 0.6 ) );
    m_pSceneMgr->setShadowTextureCount ( 1 );
    
    // add a little ambient lighting
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.2, 0.2, 0.2 ) );
    
    SceneNode*  pLightsBbsNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    BillboardSet*  pBillSet;
    
    // Create billboard set for lights .
    pBillSet = m_pSceneMgr->createBillboardSet ( );
    pBillSet->setMaterialName ( "Examples/Flare" );
    pLightsBbsNode->attachObject ( pBillSet );
    
    Light*   pLight = m_pSceneMgr->createLight ( );
    Vector3  tDir;
    pLight->setType ( Light::LT_POINT );
    pLight->setPosition ( 30, 70, 40 );
    tDir = -pLight->getPosition ( );
    tDir.normalise ( );
    pLight->setDirection ( tDir );
    pLight->setDiffuseColour ( 1, 1, 1 );
    pBillSet->createBillboard ( pLight->getPosition ( ) )->setColour ( pLight->getDiffuseColour ( ) );
    
    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, -1 ), 250, 250, 25, 25, true, 1, 15, 15, Vector3::UNIT_Z
    );
    
    // add a floor to our scene using the floor mesh we created
    Entity*  pFloor = m_pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/Rockwall" );
    pFloor->setCastShadows ( false );
    m_pSceneMgr->getRootSceneNode()->attachObject ( pFloor );
    
    // set camera initial transform and speed
    m_pCamera->setPosition ( 100, 20, 0 );
    m_pCamera->lookAt ( 0, 10, 0 );
    m_pCameraMan->setTopSpeed ( 50 );
    
    setupModels ( );
}

KDvoid Sample_DualQuaternion::setupModels ( KDvoid )
{
    SceneNode*  pSceneNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pSceneNode->translate ( 0, 0, 20, Node::TS_LOCAL );
    
    //Create and attach a spine entity with standard skinning
    m_pEntity = m_pSceneMgr->createEntity ( "Spine", "spine.mesh" );
    m_pEntity->setMaterialName ( "spine" );
    m_pEntity->getSkeleton ( )->getBone ( "Bone02" )->setManuallyControlled ( true );
    pSceneNode->attachObject ( m_pEntity );
    pSceneNode->scale ( Vector3 ( 0.2, 0.2, 0.2 ) );
    
    pSceneNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pSceneNode->translate ( 0, 0, -20, Node::TS_LOCAL );
    
    //Create and attach a spine entity with dual quaternion skinning
    m_pEntityDQ = m_pSceneMgr->createEntity ( "SpineDQ", "spine.mesh" );
    m_pEntityDQ->setMaterialName ( "spineDualQuat" );
    m_pEntityDQ->getSkeleton ( )->getBone ( "Bone02" )->setManuallyControlled ( true );
    pSceneNode->attachObject ( m_pEntityDQ );
    pSceneNode->scale ( Vector3 ( 0.2, 0.2, 0.2 ) );
    
#ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
    //In case the system uses the RTSS, the following line will ensure
    //that the entity is using hardware animation in RTSS as well.
    RTShader::HardwareSkinningFactory::getSingleton ( ).prepareEntityForSkinning ( m_pEntity );
    RTShader::HardwareSkinningFactory::getSingleton ( ).prepareEntityForSkinning ( m_pEntityDQ, RTShader::ST_DUAL_QUATERNION, false, true );
    
    //The following line is needed only because the spine models' materials have shaders and
    //as such is not automatically reflected in the RTSS system
    RTShader::ShaderGenerator::getSingleton ( ).createShaderBasedTechnique
    (
        m_pEntity->getSubEntity ( 0 )->getMaterialName ( ),
        MaterialManager::DEFAULT_SCHEME_NAME,
        RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
        true
    );
    
    RTShader::ShaderGenerator::getSingleton ( ).createShaderBasedTechnique
    (
        m_pEntityDQ->getSubEntity ( 0 )->getMaterialName ( ),
        MaterialManager::DEFAULT_SCHEME_NAME,
        RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
        true
    );
#endif
    
    // create name and value for skinning mode
    StringVector  aNames;
    aNames.push_back ( "Skinning" );
    String  sValue = "Software";
    
    // change the value if hardware skinning is enabled
    MaterialPtr  pDqMat = m_pEntity->getSubEntity ( 0 )->getMaterial ( );
    if ( !pDqMat.isNull ( ) )
    {
        Technique*  pBestTechnique = pDqMat->getBestTechnique ( );
        if ( pBestTechnique )
        {
            Pass*  pPass = pBestTechnique->getPass ( 0 );
            if ( pPass && pPass->hasVertexProgram ( ) && pPass->getVertexProgram ( )->isSkeletalAnimationIncluded ( ) )
            {
                sValue = "Hardware";
            }
        }
    }
    
    // create a params panel to display the skinning mode
    m_pTrayMgr->createParamsPanel ( TL_TOPLEFT, "Skinning", 170, aNames )->setParamValue ( 0, sValue );
}

KDvoid Sample_DualQuaternion::cleanupContent ( KDvoid )
{
    MeshManager::getSingleton ( ).remove ( "floor" );
    
#ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
//    RTShader::RenderState*  pRenderState = m_pShaderGenerator->getRenderState ( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME );
//    pRenderState->removeTemplateSubRenderState ( m_pSrsHardwareSkinning );
#endif
}

