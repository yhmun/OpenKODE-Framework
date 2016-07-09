/* ------------------------------------------------------------------------------------
 *
 *      File            Dot3Bump.cpp
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
#include "Dot3Bump.h"

Sample_Dot3Bump::Sample_Dot3Bump ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Bump Mapping";
    m_aInfo [ "Description" ] = "Shows how to use the dot product blending operation and normalization cube map "
                                "to achieve a bump mapping effect. Tangent space computations made through the guide of the tutorial "
                                "on bump mapping from http://users.ox.ac.uk/~univ1234 by paul.baker@univ.ox.ac.uk.";
    m_aInfo [ "Thumbnail"   ] = "thumb_bump.png";
    m_aInfo [ "Category"    ] = "Lighting";
    m_aInfo [ "Help"        ] = "Left click and drag anywhere in the scene to look around. Let go again to show "
                                "cursor and access widgets. Use WASD keys to move.";
}

StringVector Sample_Dot3Bump::getRequiredPlugins ( KDvoid )
{
    StringVector  aNames;
    if ( !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "glsles" ) )
    {
        aNames.push_back ( "Cg Program Manager" );
    }
    return aNames;
}

KDvoid Sample_Dot3Bump::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
    if ( !pCaps->hasCapability ( RSC_VERTEX_PROGRAM ) || !( pCaps->hasCapability ( RSC_FRAGMENT_PROGRAM ) ) )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex and fragment programs, "
            "so you cannot run this sample. Sorry!", "Dot3BumpSample::testCapabilities"
        );
    }
    
    if ( !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "arbfp1" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "ps_2_0" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "ps_4_0" ) &&
         !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "glsles" ) )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "Your card does not support the shader model needed for this sample, "
            "so you cannot run this sample. Sorry!", "Dot3BumpSample::testCapabilities"
        );
    }
}

bool Sample_Dot3Bump::frameRenderingQueued ( const FrameEvent& tEvent )
{
    if ( m_bMoveLights )
    {
        // rotate the light pivots
        m_pLightPivot1->roll ( Degree ( tEvent.timeSinceLastFrame * 30 ) );
        m_pLightPivot2->roll ( Degree ( tEvent.timeSinceLastFrame * 10 ) );
    }
    
    return SdkSample::frameRenderingQueued ( tEvent );  // don't forget the parent class updates!
}

KDvoid Sample_Dot3Bump::itemSelected ( SelectMenu* pMenu )
{
    if ( pMenu == m_pMeshMenu )
    {
        // change to the selected entity
        m_pObjectNode->detachAllObjects ( );
        m_pObjectNode->attachObject ( m_pSceneMgr->getEntity ( m_pMeshMenu->getSelectedItem ( ) ) );
        
        // remember which material is currently selected
        KDint  nIndex = std::max<KDint> ( 0, m_pMaterialMenu->getSelectionIndex ( ) );
        
        // update the material menu's options
        m_pMaterialMenu->setItems ( m_aPossibilities [ m_pMeshMenu->getSelectedItem ( ) ]);
        
        m_pMaterialMenu->selectItem ( nIndex );   // select the material with the saved index
    }
    else
    {
        // set the selected material for the active mesh
        ( (Entity*) m_pObjectNode->getAttachedObject ( 0 ) )->setMaterialName ( pMenu->getSelectedItem ( ) );
    }
}

KDvoid Sample_Dot3Bump::checkBoxToggled ( CheckBox* pBox )
{
    if ( StringUtil::startsWith ( pBox->getName ( ), "Light", false ) )
    {
        // get the light pivot that corresponds to this checkbox
        SceneNode*  pPivot = pBox->getName ( ) == "Light1" ? m_pLightPivot1 : m_pLightPivot2;
        SceneNode::ObjectIterator it = pPivot->getAttachedObjectIterator ( );
        
        while ( it.hasMoreElements ( ) )  // toggle visibility of light and billboard set
        {
            MovableObject*  pObject = it.getNext ( );
            pObject->setVisible ( pBox->isChecked ( ) );
        }        
    }
    else if ( pBox->getName ( ) == "MoveLights" )
    {
        m_bMoveLights = !m_bMoveLights;
    }
}

KDvoid Sample_Dot3Bump::setupContent ( KDvoid )
{
    // create our main node to attach our entities to
    m_pObjectNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    
    setupModels   ( );
    setupLights   ( );
    setupControls ( );
    
    m_pCamera->setPosition ( 0, 0, 500 );
}

KDvoid Sample_Dot3Bump::loadResources ( KDvoid )
{
#ifdef USE_RTSHADER_SYSTEM
    Ogre::StringVector  groupVector = Ogre::ResourceGroupManager::getSingleton ( ).getResourceGroups ( );
    Ogre::StringVector::iterator itGroup = groupVector.begin ( );
    Ogre::StringVector::iterator itGroupEnd = groupVector.end ( );
    Ogre::String  shaderCoreLibsPath;
    
    for ( ; itGroup != itGroupEnd; ++itGroup )
    {
        Ogre::ResourceGroupManager::LocationList  resLocationsList = Ogre::ResourceGroupManager::getSingleton ( ).getResourceLocationList ( *itGroup );
        Ogre::ResourceGroupManager::LocationList::iterator  it = resLocationsList.begin ( );
        Ogre::ResourceGroupManager::LocationList::iterator  itEnd = resLocationsList.end ( );
        bool  coreLibsFound = false;
        
        // Find the location of the core shader libs
        for ( ; it != itEnd; ++it )
        {
            if ( ( *it )->archive->getName ( ).find ( "RTShaderLib") != String::npos )
            {
                shaderCoreLibsPath = ( *it )->archive->getName ( ) + "/";
                coreLibsFound = true;
                break;
            }
        }
        
        // Core libs path found in the current group.
        if ( coreLibsFound )
        {
            break;
        }
    }
#endif
}

KDvoid Sample_Dot3Bump::setupModels ( KDvoid )
{
    StringVector  aMatNames;
    
    aMatNames.push_back ( "Examples/BumpMapping/MultiLight" );
    aMatNames.push_back ( "Examples/BumpMapping/MultiLightSpecular" );
    aMatNames.push_back ( "Examples/OffsetMapping/Specular" );
    aMatNames.push_back ( "Examples/ShowUV" );
    aMatNames.push_back ( "Examples/ShowNormals" );
    aMatNames.push_back ( "Examples/ShowTangents" );
    
#ifdef USE_RTSHADER_SYSTEM
    aMatNames.push_back("RTSS/NormalMapping_SinglePass");
    aMatNames.push_back("RTSS/NormalMapping_MultiPass");
#endif
    	
    m_aPossibilities [ "ogrehead.mesh" ] = aMatNames;
    m_aPossibilities [ "knot.mesh"     ] = aMatNames;
    
    aMatNames.clear();
    aMatNames.push_back("Examples/Athene/NormalMapped");
    aMatNames.push_back("Examples/Athene/NormalMappedSpecular");
    aMatNames.push_back("Examples/Athene/NormalMappedSpecular");
    aMatNames.push_back("Examples/ShowUV");
    aMatNames.push_back("Examples/ShowNormals");
    aMatNames.push_back("Examples/ShowTangents");
    
#ifdef USE_RTSHADER_SYSTEM
    aMatNames.push_back ( "RTSS/Athene/NormalMapping_SinglePass" );
    aMatNames.push_back ( "RTSS/Athene/NormalMapping_MultiPass" );
#endif
    
    m_aPossibilities["athene.mesh"] = aMatNames;
    
    for ( std::map<String, StringVector>::iterator it = m_aPossibilities.begin ( ); it != m_aPossibilities.end ( ); it++ )
    {
        // load each mesh with non-default hardware buffer usage options
        MeshPtr  pMesh = MeshManager::getSingleton ( ).load
        (
            it->first, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
        );
        
        // build tangent vectors for our mesh
        KDushort  uSrc, uDest;
        if ( !pMesh->suggestTangentVectorBuildParams ( VES_TANGENT, uSrc, uDest ) )
        {
            pMesh->buildTangentVectors ( VES_TANGENT, uSrc, uDest );
            // this version cleans mirrored and rotated UVs but requires quality models
            // mesh->buildTangentVectors(VES_TANGENT, src, dest, true, true);
        }
        
        // create an entity from the mesh and set the first available material
        Entity*  pEntity = m_pSceneMgr->createEntity ( pMesh->getName ( ), pMesh->getName ( ) );
        pEntity->setMaterialName ( it->second.front ( ) );
    }
}

KDvoid Sample_Dot3Bump::setupLights ( KDvoid )
{
    m_pSceneMgr->setAmbientLight ( ColourValue::Black );   // disable ambient lighting
        
    // create pivot nodes
    m_pLightPivot1 = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    m_pLightPivot2 = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
        
    Light*  pLight;
    BillboardSet*  pBillSet;
        
    // create white light
    pLight = m_pSceneMgr->createLight ( );
    pLight->setPosition ( 200, 0, 0 );
    pLight->setDiffuseColour ( 1, 1, 1 );
    pLight->setSpecularColour ( 1, 1, 1 );
    
    // create white flare
    pBillSet = m_pSceneMgr->createBillboardSet ( );
    pBillSet->setMaterialName ( "Examples/Flare" );
    pBillSet->createBillboard ( 200, 0, 0)->setColour ( ColourValue::White );
        
    m_pLightPivot1->attachObject ( pLight );
    m_pLightPivot1->attachObject ( pBillSet );
        
    // create red light
    pLight = m_pSceneMgr->createLight ( );
    pLight->setPosition ( 40, 200, 50 );
    pLight->setDiffuseColour ( 1, 0, 0 );
    pLight->setSpecularColour ( 1, 0.8, 0.8 );
	
	// create white flare
    pBillSet = m_pSceneMgr->createBillboardSet ( );
    pBillSet->setMaterialName ( "Examples/Flare" );
    pBillSet->createBillboard ( 50, 200, 50 )->setColour ( ColourValue::Red );
        
    m_pLightPivot2->attachObject ( pLight );
    m_pLightPivot2->attachObject ( pBillSet );
}

KDvoid Sample_Dot3Bump::setupControls ( KDvoid )
{
    m_pTrayMgr->showCursor ( );
    
    // make room for the controls
    m_pTrayMgr->showLogo(TL_TOPRIGHT);
    m_pTrayMgr->showFrameStats(TL_TOPRIGHT);
    m_pTrayMgr->toggleAdvancedFrameStats();
    
    // create a menu to choose the model displayed
    m_pMeshMenu = m_pTrayMgr->createLongSelectMenu ( TL_BOTTOM, "Mesh", "Mesh", 370, 290, 10 );
    for ( std::map<String, StringVector>::iterator it = m_aPossibilities.begin ( ); it != m_aPossibilities.end ( ); it++ )
    {
        m_pMeshMenu->addItem ( it->first );
    }
    
    // create a menu to choose the material used by the model
    m_pMaterialMenu = m_pTrayMgr->createLongSelectMenu ( TL_BOTTOM, "Material", "Material", 370, 290, 10 );
    
    // create checkboxes to toggle lights
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Light1"    , "Light A"    )->setChecked ( true, false );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Light2"    , "Light B"    )->setChecked ( true, false );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "MoveLights", "Move Lights")->setChecked ( true, false );
    
    // a friendly reminder
    StringVector  aNames;
    aNames.push_back ( "Help" );
    m_pTrayMgr->createParamsPanel ( TL_TOPLEFT, "Help", 100, aNames )->setParamValue ( 0, "H/F1" );
    
    m_pMeshMenu->selectItem ( 0 );  // select first mesh
}

KDvoid Sample_Dot3Bump::cleanupContent ( KDvoid )
{
    // clean up properly to avoid interfering with subsequent samples
    for ( std::map<String, StringVector>::iterator it = m_aPossibilities.begin( ); it != m_aPossibilities.end( ); it++ )
    {
        MeshManager::getSingleton ( ).unload ( it->first );
    }
    
    m_aPossibilities.clear ( );
}
