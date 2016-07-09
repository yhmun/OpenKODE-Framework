/* ------------------------------------------------------------------------------------
 *
 *      File            Instancing.cpp
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
#include "Lighting.h"

const uint8  l_cPriorityMain   = 50;
const uint8  l_cPriorityQuery  = 51;
const uint8  l_cPriorityLights = 55;

Sample_Lighting::Sample_Lighting ( KDvoid )
{
    m_pGreenLightAnimState  = 0;
    m_pYellowLightAnimState = 0;
    m_pLight1BBFlare        = 0;
    m_pLight1BBQueryArea    = 0;
    m_pLight1BBQueryVisible = 0;
    m_pLight2BBFlare        = 0;
    m_pLight2BBQueryArea    = 0;
    m_pLight2BBQueryVisible = 0;
    m_pLight1QueryArea      = 0;
    m_pLight1QueryVisible   = 0;
    m_pLight2QueryArea      = 0;
    m_pLight2QueryVisible   = 0;
    m_pActiveQuery          = 0;
    m_bUseOcclusionQuery    = false;
    m_bDoOcclusionQuery     = false;
    
    m_aInfo [ "Title"       ] = "Lighting";
    m_aInfo [ "Description" ] = "Shows OGRE's lighting support. Also demonstrates "
                                "usage of occlusion queries and automatic time-relative behaviour "
                                "using billboards and controllers.";
    m_aInfo [ "Thumbnail"   ] = "thumb_lighting.png";
    m_aInfo [ "Category"    ] = "Lighting";
}

bool Sample_Lighting::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // Move the lights along their paths
    m_pGreenLightAnimState ->addTime ( tEvent.timeSinceLastFrame );
    m_pYellowLightAnimState->addTime ( tEvent.timeSinceLastFrame );
    
    // Modulate the light flare according to performed occlusion queries
    if ( m_bUseOcclusionQuery )
    {
        // Stop occlusion queries until we get their information
        // (may not happen on the same frame they are requested in)
        m_bDoOcclusionQuery = false;
        
        // Check if all query information available
        if ( ( m_pLight1QueryArea   ->isStillOutstanding ( ) == false ) &&
             ( m_pLight1QueryVisible->isStillOutstanding ( ) == false ) &&
             ( m_pLight2QueryArea   ->isStillOutstanding ( ) == false ) &&
             ( m_pLight2QueryVisible->isStillOutstanding ( ) == false ) )
        {
            // Modulate the lights according to the query data
            KDuint   uLightAreaCount;
            KDuint   uLightVisibleCount;
            KDfloat  fRatio;
            
            m_pLight1QueryArea   ->pullOcclusionQuery ( &uLightAreaCount );
            m_pLight1QueryVisible->pullOcclusionQuery ( &uLightVisibleCount );
            fRatio = (KDfloat) uLightVisibleCount / (KDfloat) uLightAreaCount;
            m_pLight1BBFlare->setColour ( m_pTrail->getInitialColour(0) * fRatio );
            
            m_pLight2QueryArea   ->pullOcclusionQuery ( &uLightAreaCount );
            m_pLight2QueryVisible->pullOcclusionQuery ( &uLightVisibleCount );
            fRatio = (KDfloat) uLightVisibleCount / (KDfloat) uLightAreaCount;
            m_pLight2BBFlare->setColour ( m_pTrail->getInitialColour ( 1 ) * fRatio );
            
            // Request new query data
            m_bDoOcclusionQuery = true;
        }
    }
    
    return SdkSample::frameRenderingQueued ( tEvent );   // don't forget the parent class updates!
}

KDvoid Sample_Lighting::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
/*
    if ( Root::getSingletonPtr ( )->getRenderSystem ( )->getName ( ).find ( "OpenGL ES 1" ) != String::npos )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "This sample uses 1D textures which are not supported in OpenGL ES 1.1, "
            "so you cannot run this sample. Sorry!", "Sample_Lighting::testCapabilities"
        );
    }
*/ 
}

KDvoid Sample_Lighting::setupContent ( KDvoid )
{
    // Set our camera to orbit around the origin at a suitable distance
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pCameraMan->setYawPitchDist ( Radian ( 0 ), Radian ( 0 ), 400 );
    
    m_pTrayMgr->showCursor ( );
    
    // Create an ogre head and place it at the origin
    Entity*  pHead = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    pHead->setRenderQueueGroup ( l_cPriorityMain );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pHead );
    
    setupLights ( );
}

KDvoid Sample_Lighting::setupLights ( KDvoid )
{
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.1, 0.1, 0.1 ) );  // Dim ambient lighting
    
    // Create a ribbon trail that our lights will leave behind
    NameValuePairList  aParams;
    aParams [ "numberOfChains" ] = "2";
    aParams [ "maxElements"    ] = "80";
    
    m_pTrail = (RibbonTrail*) m_pSceneMgr->createMovableObject ( "RibbonTrail", &aParams );
    m_pSceneMgr->getRootSceneNode()->attachObject ( m_pTrail );
    m_pTrail->setMaterialName ( "Examples/LightRibbonTrail" );
    m_pTrail->setTrailLength ( 400 );
    m_pTrail->setRenderQueueGroup ( l_cPriorityLights );
    
    // Create the occlusion queries to be used in this sample
    try
    {
        RenderSystem*  pRenderSystem = Root::getSingleton ( ).getRenderSystem ( );
        m_pLight1QueryArea    = pRenderSystem->createHardwareOcclusionQuery ( );
        m_pLight1QueryVisible = pRenderSystem->createHardwareOcclusionQuery ( );
        m_pLight2QueryArea    = pRenderSystem->createHardwareOcclusionQuery ( );
        m_pLight2QueryVisible = pRenderSystem->createHardwareOcclusionQuery ( );
        
        m_bUseOcclusionQuery  = ( m_pLight1QueryArea    != KD_NULL ) &&
                                ( m_pLight1QueryVisible != KD_NULL ) &&
                                ( m_pLight2QueryArea    != KD_NULL ) &&
                                ( m_pLight2QueryVisible != KD_NULL );
    }
    catch ( Exception e )
    {
        m_bUseOcclusionQuery = false;
    }
    
    if ( m_bUseOcclusionQuery == false )
    {
        LogManager::getSingleton ( ).logMessage ( "Sample_Lighting - Error: failed to create hardware occlusion query" );
    }
    
    // Create the materials to be used by the objects used fo the occlusion query
    MaterialPtr  pMatBase = MaterialManager::getSingleton ( ).getByName ( "BaseWhiteNoLighting" );
    
    MaterialPtr  pMatQueryArea = pMatBase->clone ( "QueryArea" );
    pMatQueryArea->setDepthWriteEnabled  ( false );
    pMatQueryArea->setColourWriteEnabled ( false );
    pMatQueryArea->setDepthCheckEnabled  ( false ); // Not occluded by objects
    
    MaterialPtr  pMatQueryVisible = pMatBase->clone ( "QueryVisible" );
    pMatQueryVisible->setDepthWriteEnabled  ( false );
    pMatQueryVisible->setColourWriteEnabled ( false );
    pMatQueryVisible->setDepthCheckEnabled  ( true ); // Occluded by objects
    
    SceneNode*           pNode;
    Animation*           pAnim;
    NodeAnimationTrack*  pTrack;
    Light*               pLight;
    BillboardSet*        pBillSet;
    
    // Create a light node
    pNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3 ( 50, 30, 0 ) );
    
    // Create a 14 second animation with spline interpolation
    pAnim = m_pSceneMgr->createAnimation ( "Path1", 14 );
    pAnim->setInterpolationMode ( Animation::IM_SPLINE );
    
    pTrack = pAnim->createNodeTrack ( 1, pNode );  // Create a node track for our animation
    
    // Enter keyframes for our track to define a path for the light to follow
    pTrack->createNodeKeyFrame (  0 )->setTranslate ( Vector3 (   50,  30,    0 ) );
    pTrack->createNodeKeyFrame (  2 )->setTranslate ( Vector3 (  100, -30,    0 ) );
    pTrack->createNodeKeyFrame (  4 )->setTranslate ( Vector3 (  120, -80,  150 ) );
    pTrack->createNodeKeyFrame (  6 )->setTranslate ( Vector3 (   30, -80,   50 ) );
    pTrack->createNodeKeyFrame (  8 )->setTranslate ( Vector3 (  -50,  30,  -50 ) );
    pTrack->createNodeKeyFrame ( 10 )->setTranslate ( Vector3 ( -150, -20, -100 ) );
    pTrack->createNodeKeyFrame ( 12 )->setTranslate ( Vector3 (  -50, -30,    0 ) );
    pTrack->createNodeKeyFrame ( 14 )->setTranslate ( Vector3 (   50,  30,    0 ) );
    
    // Create an animation state from the animation and enable it
    m_pYellowLightAnimState = m_pSceneMgr->createAnimationState ( "Path1" );
    m_pYellowLightAnimState->setEnabled ( true );
    
    // Set initial settings for the ribbon mTrail and add the light node
    m_pTrail->setInitialColour ( 0, 1.0, 0.8, 0 );
    m_pTrail->setColourChange ( 0, 0.5, 0.5, 0.5, 0.5 );
    m_pTrail->setInitialWidth ( 0, 5 );
    m_pTrail->addNode ( pNode );
    
    // Attach a light with the same colour to the light node
    pLight = m_pSceneMgr->createLight ( );
    pLight->setDiffuseColour ( m_pTrail->getInitialColour ( 0 ) );
    pNode->attachObject ( pLight );
    
    // Attach a flare with the same colour to the light node
    pBillSet = m_pSceneMgr->createBillboardSet ( 1 );
    m_pLight1BBFlare = pBillSet->createBillboard ( Vector3::ZERO, m_pTrail->getInitialColour ( 0 ) );
    pBillSet->setMaterialName ( "Examples/Flare" );
    pBillSet->setRenderQueueGroup ( l_cPriorityLights );
    pNode->attachObject ( pBillSet );
    
    if ( m_bUseOcclusionQuery )
    {
        // Attach a billboard which will be used to get a relative area occupied by the light
        m_pLight1BBQueryArea = m_pSceneMgr->createBillboardSet ( 1 );
        m_pLight1BBQueryArea->setDefaultDimensions ( 10, 10 );
        m_pLight1BBQueryArea->createBillboard ( Vector3::ZERO );
        m_pLight1BBQueryArea->setMaterialName ( "QueryArea" );
        m_pLight1BBQueryArea->setRenderQueueGroup ( l_cPriorityQuery );
        pNode->attachObject ( m_pLight1BBQueryArea );
        
        // Attach a billboard which will be used to get the visible area occupied by the light
        m_pLight1BBQueryVisible = m_pSceneMgr->createBillboardSet ( 1 );
        m_pLight1BBQueryVisible->setDefaultDimensions ( 10, 10 );
        m_pLight1BBQueryVisible->createBillboard ( Vector3::ZERO );
        m_pLight1BBQueryVisible->setMaterialName ( "QueryVisible" );
        m_pLight1BBQueryVisible->setRenderQueueGroup ( l_cPriorityQuery );
        pNode->attachObject ( m_pLight1BBQueryVisible );
    }

    // Create a second light node
    pNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3 ( -50, 100, 0 ) );
    
    // Create a 10 second animation with spline interpolation
    pAnim = m_pSceneMgr->createAnimation ( "Path2", 10 );
    pAnim->setInterpolationMode ( Animation::IM_SPLINE );
        
    pTrack = pAnim->createNodeTrack ( 1, pNode );  // Create a node track for our animation
    
    // Enter keyframes for our track to define a path for the light to follow
    pTrack->createNodeKeyFrame (  0 )->setTranslate ( Vector3 (  -50,  100,   0 ) );
    pTrack->createNodeKeyFrame (  2 )->setTranslate ( Vector3 ( -100,  150, -30 ) );
    pTrack->createNodeKeyFrame (  4 )->setTranslate ( Vector3 ( -200,    0,  40 ) );
    pTrack->createNodeKeyFrame (  6 )->setTranslate ( Vector3 (    0, -150,  70 ) );
    pTrack->createNodeKeyFrame (  8 )->setTranslate ( Vector3 (   50,    0,  30 ) );
    pTrack->createNodeKeyFrame ( 10 )->setTranslate ( Vector3 (  -50,  100,   0 ) );
    
    // Create an animation state from the animation and enable it
    m_pGreenLightAnimState = m_pSceneMgr->createAnimationState ( "Path2" );
    m_pGreenLightAnimState->setEnabled ( true );
    
    // Set initial settings for the ribbon mTrail and add the light node
    m_pTrail->setInitialColour ( 1, 0.0, 1.0, 0.4 );
    m_pTrail->setColourChange ( 1, 0.5, 0.5, 0.5, 0.5 );
    m_pTrail->setInitialWidth ( 1, 5 );
    m_pTrail->addNode ( pNode );
    
    // Attach a light with the same colour to the light node
    pLight = m_pSceneMgr->createLight ( );
    pLight->setDiffuseColour ( m_pTrail->getInitialColour ( 1 ) );
    pNode->attachObject ( pLight );
    
    // Attach a flare with the same colour to the light node
    pBillSet = m_pSceneMgr->createBillboardSet ( 1 );
    m_pLight2BBFlare = pBillSet->createBillboard ( Vector3::ZERO, m_pTrail->getInitialColour ( 1 ) );
    pBillSet->setMaterialName ( "Examples/Flare" );
    pBillSet->setRenderQueueGroup ( l_cPriorityLights );
    pNode->attachObject ( pBillSet );

    if ( m_bUseOcclusionQuery )
    {
        // Attach a billboard which will be used to get a relative area occupied by the light
        m_pLight2BBQueryArea = m_pSceneMgr->createBillboardSet ( 1 );
        m_pLight2BBQueryArea->setDefaultDimensions ( 10, 10 );
        m_pLight2BBQueryArea->createBillboard ( Vector3::ZERO );
        m_pLight2BBQueryArea->setMaterialName ( "QueryArea" );
        m_pLight2BBQueryArea->setRenderQueueGroup ( l_cPriorityQuery );
        pNode->attachObject ( m_pLight2BBQueryArea );
        
        // Attach a billboard which will be used to get the visible area occupied by the light
        m_pLight2BBQueryVisible = m_pSceneMgr->createBillboardSet ( 1 );
        m_pLight2BBQueryVisible->setDefaultDimensions ( 10, 10 );
        m_pLight2BBQueryVisible->createBillboard ( Vector3::ZERO );
        m_pLight2BBQueryVisible->setMaterialName ( "QueryVisible" );
        m_pLight2BBQueryVisible->setRenderQueueGroup ( l_cPriorityQuery );
        pNode->attachObject ( m_pLight2BBQueryVisible );
    }
    
    // Setup the listener for the occlusion query
    if ( m_bUseOcclusionQuery)
    {
        m_pSceneMgr->addRenderObjectListener ( this );
        m_bDoOcclusionQuery = true;
    }
}

KDvoid Sample_Lighting::notifyRenderSingleObject ( Renderable* pRend, const Pass* pPass, const AutoParamDataSource* pSource,
                                                   const LightList* pLightList, bool bSuppressRenderStateChanges )
{
    //
    // The following code activates and deactivates the occlusion queries
    // so that the queries only include the rendering of their intended targets
    //
    
    // Close the last occlusion query
    // Each occlusion query should only last a single rendering
    if ( m_pActiveQuery != KD_NULL )
    {
        m_pActiveQuery->endOcclusionQuery ( );
        m_pActiveQuery = KD_NULL;
    }
    
    // Open a new occlusion query
    if ( m_bDoOcclusionQuery == true )
    {
        // Check if a the object being rendered needs
        // to be occlusion queried, and by which query instance.
        if      ( pRend == m_pLight1BBQueryArea )
        {
            m_pActiveQuery = m_pLight1QueryArea;
        }
        else if ( pRend == m_pLight1BBQueryVisible )
        {
            m_pActiveQuery = m_pLight1QueryVisible;
        }
        else if ( pRend == m_pLight2BBQueryArea )
        {
            m_pActiveQuery = m_pLight2QueryArea;
        }
        else if ( pRend == m_pLight2BBQueryVisible  )
        {
            m_pActiveQuery = m_pLight2QueryVisible;
        }
        
        if ( m_pActiveQuery != KD_NULL )
        {
            m_pActiveQuery->beginOcclusionQuery ( );
        }
    }
}

KDvoid Sample_Lighting::cleanupContent ( KDvoid )
{
    RenderSystem*  pRenderSystem = Root::getSingleton ( ).getRenderSystem ( );
    
    if ( m_pLight1QueryArea != KD_NULL )
    {
        pRenderSystem->destroyHardwareOcclusionQuery ( m_pLight1QueryArea );
    }
    
    if ( m_pLight1QueryVisible != KD_NULL )
    {
        pRenderSystem->destroyHardwareOcclusionQuery ( m_pLight1QueryVisible );
    }
    
    if ( m_pLight2QueryArea != KD_NULL )
    {
        pRenderSystem->destroyHardwareOcclusionQuery ( m_pLight2QueryArea );
    }
    
    if ( m_pLight2QueryVisible != KD_NULL )
    {
        pRenderSystem->destroyHardwareOcclusionQuery ( m_pLight2QueryVisible );
    }
}
