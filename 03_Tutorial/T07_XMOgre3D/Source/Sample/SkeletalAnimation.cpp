/* ------------------------------------------------------------------------------------
 *
 *      File            SkeletalAnimation.cpp
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
#include "SkeletalAnimation.h"

Sample_SkeletalAnimation::Sample_SkeletalAnimation ( KDvoid )
: NUM_MODELS ( 6 )
, ANIM_CHOP  ( 8 )
{    
    m_aInfo [ "Title"       ] = "Skeletal Animation";
    m_aInfo [ "Description" ] = "A demo of the skeletal animation feature, including spline animation.";
    m_aInfo [ "Thumbnail"   ] = "thumb_skelanim.png";
    m_aInfo [ "Category"    ] = "Animation";
}

bool Sample_SkeletalAnimation::frameRenderingQueued ( const FrameEvent& tEvent )
{
    for ( KDuint i = 0; i < NUM_MODELS; i++ )
    {    
        // update sneaking animation based on speed
        m_aAnimStates[i]->addTime ( m_aAnimSpeeds[i] * tEvent.timeSinceLastFrame );
        
        if ( m_aAnimStates[i]->getTimePosition ( ) >= ANIM_CHOP )   // when it's time to loop...
        {
            // We need reposition the scene node origin, since the animation includes translation.
            // Position is calculated from an offset to the end position, and rotation is calculated
            // from how much the animation turns the character. 
            
            Quaternion tRot ( Degree ( -60 ), Vector3::UNIT_Y );   // how much the animation turns the character
            
            // find current end position and the offset
            Vector3 tCurrEnd = m_aModelNodes[i]->getOrientation ( ) * m_tSneakEndPos + m_aModelNodes[i]->getPosition ( );
            Vector3 tOffset  = tRot * m_aModelNodes[i]->getOrientation ( ) * -m_tSneakStartPos;
            
            m_aModelNodes[i]->setPosition ( tCurrEnd + tOffset );
            m_aModelNodes[i]->rotate ( tRot );
            
            m_aAnimStates[i]->setTimePosition ( 0 );   // reset animation time
         
        }        
    }
    
    return SdkSample::frameRenderingQueued ( tEvent );
}

KDvoid Sample_SkeletalAnimation::setupContent ( KDvoid )
{
    // set shadow properties
    m_pSceneMgr->setShadowTechnique ( SHADOWTYPE_TEXTURE_MODULATIVE );
    m_pSceneMgr->setShadowTextureSize ( 512 );
    m_pSceneMgr->setShadowColour ( ColourValue ( 0.6, 0.6, 0.6 ) );
    m_pSceneMgr->setShadowTextureCount ( 2 );
    
    // add a little ambient lighting
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.5, 0.5, 0.5 ) );
    
    SceneNode*  pLightsBbsNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    BillboardSet*  pBbs;
    
    // Create billboard set for lights .
    pBbs = m_pSceneMgr->createBillboardSet ( );
    pBbs->setMaterialName ( "Examples/Flare" );
    pLightsBbsNode->attachObject ( pBbs );
        
    // add a blue spotlight
    Light*   pLight = m_pSceneMgr->createLight ( );
    Vector3  tDir;
    pLight->setType ( Light::LT_SPOTLIGHT );
    pLight->setPosition ( -40, 180, -10 );
    tDir = -pLight->getPosition ( );
    tDir.normalise ( );
    pLight->setDirection ( tDir );
    pLight->setDiffuseColour ( 0.0, 0.0, 0.5 );
    pBbs->createBillboard ( pLight->getPosition ( ) )->setColour ( pLight->getDiffuseColour ( ) );
        
    // add a green spotlight.
    pLight = m_pSceneMgr->createLight ( );
    pLight->setType ( Light::LT_SPOTLIGHT );
    pLight->setPosition ( 0, 150, -100 );
    tDir = -pLight->getPosition ( );
    tDir.normalise ( );
    pLight->setDirection ( tDir );
    pLight->setDiffuseColour ( 0.0, 0.5, 0.0 );
    pBbs->createBillboard ( pLight->getPosition ( ) )->setColour ( pLight->getDiffuseColour ( ) );
    
    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, -1 ), 250, 250, 25, 25, true, 1, 15, 15, Vector3::UNIT_Z );
    
    // add a floor to our scene using the floor mesh we created
    Entity*  pFloor = m_pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/Rockwall" );
    pFloor->setCastShadows ( false );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pFloor );
    
    // set camera initial transform and speed
    m_pCamera->setPosition ( 100, 20, 0 );
    m_pCamera->lookAt      (   0, 10, 0 );
    m_pCameraMan->setTopSpeed ( 50 );

    setupModels ( );
}

KDvoid Sample_SkeletalAnimation::setupModels  ( KDvoid )
{
    tweakSneakAnim ( );
    
    SceneNode*      pSn  = 0;
    Entity*         pEnt = 0;
    AnimationState* pAs  = 0;
    
    for ( KDuint i = 0; i < NUM_MODELS; i++ )
    {
        // create scene nodes for the models at regular angular intervals
        pSn = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
        pSn->yaw ( Radian ( Math::TWO_PI * (KDfloat) i / (KDfloat) NUM_MODELS ) );
        pSn->translate ( 0, 0, -20, Node::TS_LOCAL );
        m_aModelNodes.push_back ( pSn );
        
        // create and attach a jaiqua entity
        pEnt = m_pSceneMgr->createEntity ( "Jaiqua" + StringConverter::toString ( i + 1 ), "jaiqua.mesh" );
        
//        if ( m_pShaderGenerator->getTargetLanguage() == "glsles")
//        {
//            MaterialPtr mat = MaterialManager::getSingleton().getByName("jaiqua");
//            mat->getTechnique(0)->getPass(0)->setShadowCasterFragmentProgram("Ogre/BasicFragmentPrograms/PassthroughFpGLSLES");
//        }
        pEnt->setMaterialName ( "jaiqua" );

        pSn->attachObject ( pEnt );
		
        // enable the entity's sneaking animation at a random speed and loop it manually since translation is involved
        pAs = pEnt->getAnimationState ( "Sneak" );
        pAs->setEnabled ( true );
        pAs->setLoop ( false );
        m_aAnimSpeeds.push_back ( Math::RangeRandom ( 0.5, 1.5 ) );
        m_aAnimStates.push_back ( pAs );
    }
    
    // create name and value for skinning mode
    StringVector  aNames;
    aNames.push_back ( "Skinning" );
    String  sValue = "Software";
    
    // change the value if hardware skinning is enabled
    MaterialPtr  pEntityMaterial = pEnt->getSubEntity ( 0 )->getMaterial ( );
    if ( !pEntityMaterial.isNull ( ) )
    {
        Technique*  pBestTechnique = pEntityMaterial->getBestTechnique ( );
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

KDvoid Sample_SkeletalAnimation::tweakSneakAnim ( KDvoid )
{
    // get the skeleton, animation, and the node track iterator
    SkeletonPtr  pSkel = SkeletonManager::getSingleton ( ).load ( "jaiqua.skeleton", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    Animation*   pAnim = pSkel->getAnimation ( "Sneak" );
    Animation::NodeTrackIterator  tTracks = pAnim->getNodeTrackIterator ( );
    
    while ( tTracks.hasMoreElements ( ) )   // for every node track...
    {
        NodeAnimationTrack*  pTrack = tTracks.getNext ( );
        
        // get the keyframe at the chopping point
        TransformKeyFrame  tOldKf ( 0, 0 );
        pTrack->getInterpolatedKeyFrame ( ANIM_CHOP, &tOldKf );
        
        // drop all keyframes after the chopping point
        while ( pTrack->getKeyFrame ( pTrack->getNumKeyFrames ( ) - 1 )->getTime ( ) >= ANIM_CHOP - 0.3f )
        {
            pTrack->removeKeyFrame ( pTrack->getNumKeyFrames ( ) - 1 );
        }
        
        // create a new keyframe at chopping point, and get the first keyframe
        TransformKeyFrame*  pNewKf   = pTrack->createNodeKeyFrame ( ANIM_CHOP );
        TransformKeyFrame*  pStartKf = pTrack->getNodeKeyFrame ( 0 );
        
        Bone*  pBone = pSkel->getBone ( pTrack->getHandle ( ) );
        
        if ( pBone->getName ( ) == "Spineroot" )   // adjust spine root relative to new location
        {
            m_tSneakStartPos   = pStartKf->getTranslate ( ) + pBone->getInitialPosition ( );
            m_tSneakEndPos     = tOldKf.getTranslate    ( ) + pBone->getInitialPosition ( );
            m_tSneakStartPos.y = m_tSneakEndPos.y;
            
            pNewKf->setTranslate ( tOldKf.getTranslate ( ) );
            pNewKf->setRotation  ( tOldKf.getRotation  ( ) );
            pNewKf->setScale     ( tOldKf.getScale     ( ) );
        }
        else   // make all other bones loop back
        {
            pNewKf->setTranslate ( pStartKf->getTranslate ( ) );
            pNewKf->setRotation  ( pStartKf->getRotation  ( ) );
            pNewKf->setScale     ( pStartKf->getScale     ( ) );
        }
    }
}

KDvoid Sample_SkeletalAnimation::cleanupContent ( KDvoid )
{
    m_aModelNodes.clear ( );
    m_aAnimStates.clear ( );
    m_aAnimSpeeds.clear ( );
    
    MeshManager::getSingleton ( ).remove ( "floor" );
}