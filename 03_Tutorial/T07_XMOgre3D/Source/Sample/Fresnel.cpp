/* ------------------------------------------------------------------------------------
 *
 *      File            Fresnel.cpp
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
#include "Fresnel.h"

Sample_Fresnel::Sample_Fresnel ( KDvoid )
: NUM_FISH ( 30 )
, NUM_FISH_WAYPOINTS ( 10 )
, FISH_PATH_LENGTH ( 200 )
, FISH_SCALE ( 2 )
{
    m_aInfo [ "Title"       ] = "Fresnel";
    m_aInfo [ "Description" ] = "Shows how to create reflections and refractions using render-to-texture and shaders.";
    m_aInfo [ "Thumbnail"   ] = "thumb_fresnel.png";
    m_aInfo [ "Category"    ] = "Unsorted";
}

StringVector Sample_Fresnel::getRequiredPlugins ( KDvoid )
{
    StringVector  aNames;
    if ( !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "glsles" ) )
    {
        aNames.push_back("Cg Program Manager");
    }
    return aNames;
}

KDvoid Sample_Fresnel::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
    if ( !pCaps->hasCapability ( RSC_VERTEX_PROGRAM ) || !pCaps->hasCapability ( RSC_FRAGMENT_PROGRAM ) )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex and fragment"
            " programs, so you cannot run this sample. Sorry!", "FresnelSample::testCapabilities"
        );
    }
    
    if ( !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "arbfp1" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "ps_4_0" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "ps_2_0" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "ps_1_4" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "glsles" ) )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support advanced fragment"
            " programs, so you cannot run this sample. Sorry!", "FresnelSample::testCapabilities"
        );
    }
}

bool Sample_Fresnel::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // update the fish spline path animations and loop as needed
    m_fFishAnimTime += tEvent.timeSinceLastFrame;
    while ( m_fFishAnimTime >= FISH_PATH_LENGTH )
    {
        m_fFishAnimTime -= FISH_PATH_LENGTH;
    }
    
    for ( KDuint i = 0; i < NUM_FISH; i++ )
    {
        m_aFishAnimStates[i]->addTime ( tEvent.timeSinceLastFrame * 2 );  // update fish swim animation
        
        // set the new position based on the spline path and set the direction based on displacement
        Vector3  tLastPos = m_aFishNodes[i]->getPosition ( );
        m_aFishNodes[i]->setPosition ( m_aFishSplines[i].interpolate ( m_fFishAnimTime / FISH_PATH_LENGTH ) );
        m_aFishNodes[i]->setDirection ( m_aFishNodes[i]->getPosition ( ) - tLastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X );
        m_aFishNodes[i]->setFixedYawAxis(true);
    }
    
    return SdkSample::frameRenderingQueued ( tEvent );
}

KDvoid Sample_Fresnel::preRenderTargetUpdate ( const RenderTargetEvent& tEvent )
{
    m_pWater->setVisible ( false );  // hide the water
    
    if ( tEvent.source == m_pReflectionTarget)  // for reflection, turn on camera reflection and hide submerged entities
    {
        m_pCamera->enableReflection ( m_tWaterPlane );
        for ( std::vector<Entity*>::iterator it = m_aSubmergedEnts.begin ( ); it != m_aSubmergedEnts.end ( ); it++ )
        {
            ( *it )->setVisible ( false );
        }
    }
    else  // for refraction, hide surface entities
    {
        for ( std::vector<Entity*>::iterator it = m_aSurfaceEnts.begin ( ); it != m_aSurfaceEnts.end ( ); it++ )
        {
            ( *it )->setVisible ( false );
        }
    }
}

KDvoid Sample_Fresnel::postRenderTargetUpdate ( const RenderTargetEvent& tEvent )
{
    m_pWater->setVisible ( true );  // unhide the water
    
    if ( tEvent.source == m_pReflectionTarget)  // for reflection, turn off camera reflection and hide submerged entities
    {
        m_pCamera->disableReflection ( );
        for ( std::vector<Entity*>::iterator it = m_aSubmergedEnts.begin ( ); it != m_aSubmergedEnts.end ( ); it++ )
        {
            ( *it )->setVisible ( true );
        }
    }
    else  // for refraction, hide surface entities
    {
        for ( std::vector<Entity*>::iterator it = m_aSurfaceEnts.begin ( ); it != m_aSurfaceEnts.end ( ); it++ )
        {
            ( *it )->setVisible ( true );
        }
    }
}

KDvoid Sample_Fresnel::setupContent ( KDvoid )
{
    m_pCamera->setPosition ( -50, 125, 760 );
    m_pCameraMan->setTopSpeed ( 280 );
    
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.5, 0.5, 0.5 ) );  // set ambient light
    
    m_pSceneMgr->setSkyBox ( true, "Examples/CloudyNoonSkyBox" );  // set a skybox
    
    // make the scene's main light come from above
    Light*  pLight = m_pSceneMgr->createLight ( );
    pLight->setType ( Light::LT_DIRECTIONAL );
    pLight->setDirection ( Vector3::NEGATIVE_UNIT_Y );
    
    setupWater ( );
    setupProps ( );
    setupFish  ( );
}

KDvoid Sample_Fresnel::setupWater ( KDvoid )
{
    // create our reflection & refraction render textures, and setup their render targets
    for ( KDuint i = 0; i < 2; i++ )
    {
        TexturePtr  pTex = TextureManager::getSingleton ( ).createManual
        (
            i == 0 ? "refraction" : "reflection",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 512, 512, 0, PF_R8G8B8, TU_RENDERTARGET
        );
        
        RenderTarget*  pRtt = pTex->getBuffer ( )->getRenderTarget ( );
        pRtt->addViewport ( m_pCamera )->setOverlaysEnabled ( false );
        pRtt->addListener ( this );
        
        if ( i == 0 )
        {
            m_pRefractionTarget = pRtt;
        }
        else
        {
            m_pReflectionTarget = pRtt;
        }
    }
    
    // create our water plane mesh
    m_tWaterPlane = Plane ( Vector3::UNIT_Y, 0 );
    MeshManager::getSingleton ( ).createPlane
    (
        "water", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        m_tWaterPlane, 700, 1300, 10, 10, true, 1, 3, 5, Vector3::UNIT_Z
    );
    
    // create a water entity using our mesh, give it the shader material, and attach it to the origin
    m_pWater = m_pSceneMgr->createEntity ( "Water", "water" );
    m_pWater->setMaterialName ( "Examples/FresnelReflectionRefraction" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( m_pWater );
}

KDvoid Sample_Fresnel::setupProps ( KDvoid )
{
    Entity*  pEntity;
    
    // setting up props might take a while, so create a progress bar for visual feedback
    ProgressBar*  pProgress = m_pTrayMgr->createProgressBar ( TL_CENTER, "FresnelBuildingBar", "Creating Props...", 280, 100 );
    m_pTrayMgr->showBackdrop ( "SdkTrays/Shade" );

    pProgress->setComment ( "Upper Bath" );
    pEntity = m_pSceneMgr->createEntity ( "UpperBath", "RomanBathUpper.mesh" );
    m_pSceneMgr->getRootSceneNode()->attachObject ( pEntity );
    m_aSurfaceEnts.push_back ( pEntity );
    pProgress->setProgress ( 0.4 );
 
    pProgress->setComment("Columns");
    pEntity = m_pSceneMgr->createEntity ( "Columns", "columns.mesh" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    m_aSurfaceEnts.push_back ( pEntity );
    pProgress->setProgress ( 0.5 );

    pProgress->setComment ( "Ogre Head" );
    pEntity = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    pEntity->setMaterialName ( "RomanBath/OgreStone" );
    m_aSurfaceEnts.push_back ( pEntity );
    pProgress->setProgress ( 0.6 );
    
    SceneNode*  pHeadNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pHeadNode->setPosition ( -350, 55, 130 );
    pHeadNode->yaw ( Degree ( 90 ) );
    pHeadNode->attachObject ( pEntity );

    pProgress->setComment ( "Lower Bath" );
    pEntity = m_pSceneMgr->createEntity ( "LowerBath", "RomanBathLower.mesh" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    m_aSubmergedEnts.push_back( pEntity );
    pProgress->setProgress ( 1 );

    m_pTrayMgr->destroyWidget ( pProgress );
    m_pTrayMgr->hideBackdrop ( );
}

KDvoid Sample_Fresnel::setupFish ( KDvoid )
{
    m_aFishNodes     .resize ( NUM_FISH );
    m_aFishAnimStates.resize ( NUM_FISH );
    m_aFishSplines   .resize ( NUM_FISH );
    
    for ( KDuint i = 0; i < NUM_FISH; i++ )
    {
        // create fish entity
        Entity*  pEntity = m_pSceneMgr->createEntity ( "Fish" + StringConverter::toString ( i + 1 ), "fish.mesh" );
        m_aSubmergedEnts.push_back ( pEntity );
        
        // create an appropriately scaled node and attach the entity
        m_aFishNodes[i] = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
        m_aFishNodes[i]->setScale ( Vector3::UNIT_SCALE * FISH_SCALE );
        m_aFishNodes[i]->attachObject ( pEntity );
        
        // enable and save the swim animation state
        m_aFishAnimStates[i] = pEntity->getAnimationState ( "swim" );
        m_aFishAnimStates[i]->setEnabled ( true );
        
        m_aFishSplines[i].setAutoCalculate ( false );  // save the tangent calculation for when we are all done
        
        // generate random waypoints for the fish to swim through
        for ( KDuint j = 0; j < NUM_FISH_WAYPOINTS; j++ )
        {
            Vector3  tPos ( Math::SymmetricRandom ( ) * 270, -10, Math::SymmetricRandom ( ) * 700 );
            
            if ( j > 0 )  // make sure the waypoint isn't too far from the last, or our fish will be turbo-fish
            {
                const Vector3&  tLastPos = m_aFishSplines[i].getPoint ( j - 1 );
                Vector3  tDelta = tPos - tLastPos;
                if ( tDelta.length ( ) > 750)
                {
                    tPos = tLastPos + tDelta.normalisedCopy ( ) * 750;
                }
            }
            
            m_aFishSplines[i].addPoint ( tPos );
        }
        
        // close the spline and calculate all the tangents at once
        m_aFishSplines[i].addPoint ( m_aFishSplines[i].getPoint ( 0 ) );
        m_aFishSplines[i].recalcTangents ( );
    }
    
    m_fFishAnimTime = 0;
}

KDvoid Sample_Fresnel::cleanupContent ( KDvoid )
{
    m_aSurfaceEnts   .clear ( );
    m_aSubmergedEnts .clear ( );
    m_aFishNodes     .clear ( );
    m_aFishAnimStates.clear ( );
    m_aFishSplines   .clear ( );
    
    MeshManager::getSingleton ( ).remove ( "water" );
    
    TextureManager::getSingleton ( ).remove ( "refraction" );
    TextureManager::getSingleton ( ).remove ( "reflection" );
}
