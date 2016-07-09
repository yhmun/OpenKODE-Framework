/* ------------------------------------------------------------------------------------
 *
 *      File            SampleBrowser.cpp
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
#include "SampleBrowser.h"

//------------------------------------------------------------------------------------
//
//  ShaderGeneratorTechniqueResolverListener
//
//------------------------------------------------------------------------------------

ShaderGeneratorTechniqueResolverListener::ShaderGeneratorTechniqueResolverListener ( RTShader::ShaderGenerator* pShaderGenerator )
{
    m_pShaderGenerator = pShaderGenerator;
}

Technique* ShaderGeneratorTechniqueResolverListener::handleSchemeNotFound ( KDushort uSchemeIndex, const String& tSchemeName, Material* pOriginalMaterial, KDushort uLodIndex, const Renderable* pRend )
{
    Technique*  pGeneratedTech = KD_NULL;
    
    // Case this is the default shader generator scheme.
    if ( tSchemeName == RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME )
    {
        KDbool  bTechniqueCreated;
        
        // Create shader generated technique for this material.
        bTechniqueCreated = m_pShaderGenerator->createShaderBasedTechnique
        (
            pOriginalMaterial->getName ( ), MaterialManager::DEFAULT_SCHEME_NAME, tSchemeName
        );
        
        // Case technique registration succeeded.
        if ( bTechniqueCreated )
        {
            // Force creating the shaders for the generated technique.
            m_pShaderGenerator->validateMaterial ( tSchemeName, pOriginalMaterial->getName ( ) );
            
            // Grab the generated technique.
            Ogre::Material::TechniqueIterator  itTech = pOriginalMaterial->getTechniqueIterator ( );
            
            while ( itTech.hasMoreElements ( ) )
            {
                Ogre::Technique*  pCurTech = itTech.getNext();
                
                if ( pCurTech->getSchemeName() == tSchemeName )
                {
                    pGeneratedTech = pCurTech;
                    break;
                }
            }				
        }
    }
    
    return pGeneratedTech;
}

//------------------------------------------------------------------------------------
//
//  SampleBrowser
//
//------------------------------------------------------------------------------------

SampleBrowser::SampleBrowser ( KDvoid )
{
    //mIsShuttingDown       = false;
    //mNoGrabInput          = nograb;
    m_pTrayMgr              = 0;
    m_nLastViewCategory     = 0;
    m_nLastViewTitle        = 0;
    m_nLastSampleIndex      = -1;
    m_pCategoryMenu         = 0;
    m_pSampleMenu           = 0;
    m_pSampleSlider         = 0;
    m_pTitleLabel           = 0;
    m_pDescBox              = 0;
    m_pRendererMenu         = 0;
    m_fCarouselPlace        = 0.0f;
    m_pShaderGenerator      = KD_NULL;
    m_pMaterialMgrListener  = KD_NULL;
}

KDvoid SampleBrowser::runSample ( Sample* pSample )
{
    if ( m_pCurrentSample )  // sample quitting
    {
        m_pCurrentSample->_shutdown ( );
        m_pCurrentSample = 0;
        m_bSamplePaused  = false;     // don't pause next sample
        
        // create dummy scene and modify controls
        createDummyScene ( );
        m_pTrayMgr->showBackdrop ( "SdkTrays/Bands" );
        m_pTrayMgr->showAll ( );
        ( (Button*) m_pTrayMgr->getWidget ( "StartStop" ) )->setCaption ( "Start Sample" );
    }
    
    if ( pSample )  // sample starting
    {
        // destroy dummy scene and modify controls
        ( (Button*) m_pTrayMgr->getWidget ( "StartStop" ) )->setCaption ( "Stop Sample" );
        m_pTrayMgr->showBackdrop ( "SdkTrays/Shade" );
        m_pTrayMgr->hideAll ( );
        destroyDummyScene ( );
        
        try
        {
            SampleContext::runSample ( pSample );
        }
        catch ( Exception e )   // if failed to start, show error and fall back to menu
        {
#ifdef USE_RTSHADER_SYSTEM
            pSample->setShaderGenerator ( m_pShaderGenerator );
#endif
            pSample->_shutdown ( );
            
            createDummyScene ( );
            m_pTrayMgr->showBackdrop ( "SdkTrays/Bands" );
            m_pTrayMgr->showAll();
            ( (Button*) m_pTrayMgr->getWidget ( "StartStop" ) )->setCaption ( "Start Sample" );
            
            m_pTrayMgr->showOkDialog ( "Error!", e.getDescription ( ) + "\nSource: " + e.getSource ( ) );
        }
    }
}

bool SampleBrowser::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // don't do all these calculations when sample's running or when in configuration screen or when no samples loaded
    if ( !m_aLoadedSamples.empty ( ) && m_pTitleLabel->getTrayLocation ( ) != TL_NONE && ( !m_pCurrentSample || m_bSamplePaused ) )
    {
        // makes the carousel spin smoothly toward its right position
        Real  fCarouselOffset = m_pSampleMenu->getSelectionIndex ( ) - m_fCarouselPlace;
        if ( ( fCarouselOffset <= 0.001 ) && ( fCarouselOffset >= -0.001 ) )
        {
            m_fCarouselPlace = m_pSampleMenu->getSelectionIndex ( );
        }
        else
        {
            m_fCarouselPlace += fCarouselOffset * Math::Clamp<Real> ( tEvent.timeSinceLastFrame * 15.0, -1.0, 1.0 );
        }
        
        // update the thumbnail positions based on carousel state
        for ( KDuint i = 0; i < m_aThumbs.size ( ); i++ )
        {
            Real  fThumbOffset = m_fCarouselPlace - i;
            Real  fPhase = ( fThumbOffset / 2.0 ) - 2.8;
            
            if ( fThumbOffset < -5 || fThumbOffset > 4 )    // prevent thumbnails from wrapping around in a circle
            {
                m_aThumbs[i]->hide ( );
                continue;
            }
            else
            {
                m_aThumbs[i]->show ( );
            }
            
            Real  fLeft = Math::Cos ( fPhase ) * 200.0;
            Real  fTop =  Math::Sin ( fPhase ) * 200.0;
            Real  fScale = 1.0 / Math::Pow ( ( Math::Abs ( fThumbOffset ) + 1.0 ), 0.75 );
            
            BorderPanelOverlayElement*  pFrame = (BorderPanelOverlayElement*) m_aThumbs[i]->getChildIterator ( ).getNext ( );
            
            m_aThumbs[i]->setDimensions ( 128.0 * fScale, 96.0 * fScale );
            pFrame->setDimensions ( m_aThumbs[i]->getWidth() + 16.0, m_aThumbs[i]->getHeight() + 16.0 );
            m_aThumbs[i]->setPosition ( (KDint) ( fLeft - 80.0 - ( m_aThumbs[i]->getWidth  ( ) / 2.0 ) ),
                                        (KDint) ( fTop  - 5.0  - ( m_aThumbs[i]->getHeight ( ) / 2.0 ) ) );
            
            if ( i == m_pSampleMenu->getSelectionIndex ( ) )
            {
                pFrame->setBorderMaterialName ( "SdkTrays/Frame/Over" );
            }
            else
            {
                pFrame->setBorderMaterialName ( "SdkTrays/Frame" );
            }
        }
    }
    
    m_pTrayMgr->frameRenderingQueued ( tEvent );
    
    try
    {
        return SampleContext::frameRenderingQueued ( tEvent );
    }
    catch ( Exception e )   // show error and fall back to menu
    {
        runSample ( 0 );
        m_pTrayMgr->showOkDialog ( "Error!", e.getDescription ( ) + "\nSource: " + e.getSource ( ) );
    }
    
    return true;
}

KDvoid SampleBrowser::yesNoDialogClosed ( const DisplayString& tQuestion, KDbool bYesHit )
{
    if ( tQuestion.substr ( 0, 14 ) == "This will stop" && bYesHit )   // confirm unloading of samples
    {
        runSample ( 0 );
        buttonHit ( (Button*) m_pTrayMgr->getWidget ( "UnloadReload" ) );
    }
}

KDvoid SampleBrowser::buttonHit ( Button* pButton )
{
    if ( pButton->getName ( ) == "StartStop" )   // start or stop sample
    {
        if ( pButton->getCaption() == "Start Sample" )
        {
            if ( m_aLoadedSamples.empty ( ) )
            {
                m_pTrayMgr->showOkDialog ( "Error!", "No sample selected!" );
            }
            // use the sample pointer we stored inside the thumbnail
            else
            {
                runSample ( any_cast<Sample*> ( m_aThumbs [ m_pSampleMenu->getSelectionIndex ( ) ]->getUserAny ( ) ) );
            }
        }
        else
        {
            runSample ( 0 );
        }
    }
    else if ( pButton->getName ( ) == "UnloadReload" )   // unload or reload sample plugins and update controls
    {
        if ( pButton->getCaption ( ) == "Unload Samples" )
        {
            if ( m_pCurrentSample )
            {
                m_pTrayMgr->showYesNoDialog ( "Warning!", "This will stop the current sample. Unload anyway?" );
            }
            else
            {
                // save off current view and try to restore it on the next reload
                m_nLastViewTitle    = m_pSampleMenu  ->getSelectionIndex ( );
                m_nLastViewCategory = m_pCategoryMenu->getSelectionIndex ( );
                
                unloadSamples ( );
                populateSampleMenus ( );
                pButton->setCaption ( "Reload Samples" );
            }
        }
        else
        {
            loadSamples ( );
            populateSampleMenus ( );
            if ( !m_aLoadedSamples.empty ( ) )
            {
                pButton->setCaption ( "Unload Samples" );
            }
            
            try  // attempt to restore the last view before unloading samples
            {
                m_pCategoryMenu->selectItem ( m_nLastViewCategory );
                m_pSampleMenu  ->selectItem ( m_nLastViewTitle );
            }
            catch ( Exception e )
            {
                
            }
        }
    }
    else if ( pButton->getName ( ) == "Configure" )   // enter configuration screen
    {        
        m_pTrayMgr->removeWidgetFromTray ( "StartStop" );
        m_pTrayMgr->removeWidgetFromTray ( "UnloadReload" );
        m_pTrayMgr->removeWidgetFromTray ( "Configure" );
        m_pTrayMgr->removeWidgetFromTray ( "Quit" );
        m_pTrayMgr->moveWidgetToTray ( "Apply", TL_RIGHT );
        m_pTrayMgr->moveWidgetToTray ( "Back" , TL_RIGHT );
        
        for ( KDuint i = 0; i < m_aThumbs.size ( ); i++ )
        {
            m_aThumbs[i]->hide ( );
        }
        
        while ( m_pTrayMgr->getTrayContainer ( TL_CENTER )->isVisible ( ) )
        {
            m_pTrayMgr->removeWidgetFromTray ( TL_CENTER, 0 );
        }
        
        while ( m_pTrayMgr->getTrayContainer ( TL_LEFT )->isVisible ( ) )
        {
            m_pTrayMgr->removeWidgetFromTray ( TL_LEFT, 0 );
        }
        
        m_pTrayMgr->moveWidgetToTray ( "ConfigLabel"    , TL_LEFT );
        m_pTrayMgr->moveWidgetToTray ( m_pRendererMenu  , TL_LEFT );
        m_pTrayMgr->moveWidgetToTray ( "ConfigSeparator", TL_LEFT );
        
        m_pRendererMenu->selectItem ( m_pRoot->getRenderSystem ( )->getName ( ) );
        
        windowResized ( m_pWindow );
    }
    else if ( pButton->getName ( ) == "Back" )   // leave configuration screen
    {
        while ( m_pTrayMgr->getNumWidgets ( m_pRendererMenu->getTrayLocation ( ) ) > 3 )
        {
            m_pTrayMgr->destroyWidget ( m_pRendererMenu->getTrayLocation ( ), 3 );
        }
        
        while ( m_pTrayMgr->getNumWidgets ( TL_NONE ) != 0 )
        {
            m_pTrayMgr->moveWidgetToTray ( TL_NONE, 0, TL_LEFT );
        }
        
        m_pTrayMgr->removeWidgetFromTray ( "Apply" );
        m_pTrayMgr->removeWidgetFromTray ( "Back" );
        m_pTrayMgr->removeWidgetFromTray ( "ConfigLabel" );
        m_pTrayMgr->removeWidgetFromTray ( m_pRendererMenu );
        m_pTrayMgr->removeWidgetFromTray ( "ConfigSeparator" );
        
        m_pTrayMgr->moveWidgetToTray ( "StartStop"   , TL_RIGHT );
        m_pTrayMgr->moveWidgetToTray ( "UnloadReload", TL_RIGHT );
        m_pTrayMgr->moveWidgetToTray ( "Configure"   , TL_RIGHT );
        m_pTrayMgr->moveWidgetToTray ( "Quit"        , TL_RIGHT );
        
        windowResized ( m_pWindow );
    }
    else if ( pButton->getName ( ) == "Apply" )   // apply any changes made in the configuration screen
    {
        bool bReset = false;
        
        ConfigOptionMap&  aOptions = m_pRoot->getRenderSystemByName ( m_pRendererMenu->getSelectedItem ( ) )->getConfigOptions ( );
        NameValuePairList aNewOptions;
        
        // collect new settings and decide if a reset is needed
        
        if ( m_pRendererMenu->getSelectedItem ( ) != m_pRoot->getRenderSystem ( )->getName ( ) )
        {
            bReset = true;
        }
        
        for ( KDuint i = 3; i < m_pTrayMgr->getNumWidgets ( m_pRendererMenu->getTrayLocation ( ) ); i++ )
        {
            SelectMenu*  pMenu = (SelectMenu*) m_pTrayMgr->getWidget ( m_pRendererMenu->getTrayLocation ( ), i );
            if ( pMenu->getSelectedItem ( ) != aOptions [ pMenu->getCaption ( ) ].currentValue )
            {
                bReset = true;
            }
            aNewOptions [ pMenu->getCaption ( ) ] = pMenu->getSelectedItem ( );
        }
        
        // reset with new settings if necessary
        if ( bReset )
        {
            reconfigure ( m_pRendererMenu->getSelectedItem ( ), aNewOptions );
        }
    }
    else
    {
        m_pRoot->queueEndRendering ( );   // exit browser
    }
}

KDvoid SampleBrowser::itemSelected ( SelectMenu* pMenu )
{
    if ( pMenu == m_pCategoryMenu )      // category changed, so update the sample menu, carousel, and slider
    {        
        for ( KDuint i = 0; i < m_aThumbs.size ( ); i++ )    // destroy all thumbnails in carousel
        {
            Ogre::MaterialManager::getSingleton ( ).remove ( m_aThumbs[i]->getName ( ) );
            Widget::nukeOverlayElement ( m_aThumbs[i] );
        }
        m_aThumbs.clear ( );
                
        OverlayManager&  tOverMgr = OverlayManager::getSingleton ( );
        String  sSelectedCategory;
        
        if ( pMenu->getSelectionIndex ( ) != -1 )
        {
            sSelectedCategory = pMenu->getSelectedItem ( );
        }
        else
        {
            m_pTitleLabel->setCaption ( "" );
            m_pDescBox->setText ( "" );
        }
        
        bool  bAll = sSelectedCategory == "All";
        StringVector aSampleTitles;
        MaterialPtr  pTemplateMat = MaterialManager::getSingleton ( ).getByName ( "SdkTrays/SampleThumbnail" );

        // populate the sample menu and carousel with filtered samples
        for ( SampleSet::iterator it = m_aLoadedSamples.begin ( ); it != m_aLoadedSamples.end ( ); it++ )
        {
            NameValuePairList&  aInfo = ( *it )->getInfo ( );
            
            if ( bAll || aInfo [ "Category" ] == sSelectedCategory )
            {

                String  sName = "SdkTrays/SampleThumb" + StringConverter::toString ( aSampleTitles.size ( ) + 1 );
                
                // clone a new material for sample thumbnail
                MaterialPtr  pNewMat = pTemplateMat->clone ( sName );
                
                TextureUnitState* pTus = pNewMat->getTechnique ( 0 )->getPass ( 0 )->getTextureUnitState ( 0 );
                if ( ResourceGroupManager::getSingleton ( ).resourceExists ( "Essential", aInfo [ "Thumbnail" ] ) )
                {
                    pTus->setTextureName ( aInfo [ "Thumbnail" ] );
                }
                else
                {
                    pTus->setTextureName ( "thumb_error.png" );
                }
                
                // create sample thumbnail overlay
                BorderPanelOverlayElement*  pBP = (BorderPanelOverlayElement*)
                tOverMgr.createOverlayElementFromTemplate ( "SdkTrays/Picture", "BorderPanel", sName );
                
                pBP->setHorizontalAlignment ( GHA_RIGHT );
                pBP->setVerticalAlignment   ( GVA_CENTER );
                pBP->setMaterialName ( sName );
                pBP->setUserAny ( Any ( *it ) );
                m_pTrayMgr->getTraysLayer ( )->add2D ( pBP );
                
                // add sample thumbnail and title
                m_aThumbs.push_back ( pBP );
                aSampleTitles.push_back ( ( *it )->getInfo ( ) [ "Title" ] );
            }
        }
            
        m_fCarouselPlace = 0;  // reset carousel
        
        m_pSampleMenu->setItems ( aSampleTitles );
        if ( m_pSampleMenu->getNumItems ( ) != 0 )
        {
            itemSelected ( m_pSampleMenu );
        }
                
        m_pSampleSlider->setRange ( 1, aSampleTitles.size ( ), aSampleTitles.size ( ) );        
    }
    else if ( pMenu == m_pSampleMenu )    // sample changed, so update slider, label and description
    {
        if ( m_pSampleSlider->getValue ( ) != pMenu->getSelectionIndex ( ) + 1 )
        {
            m_pSampleSlider->setValue ( pMenu->getSelectionIndex ( ) + 1 );
        }
        
        Sample*  pSample = any_cast<Sample*> ( m_aThumbs [ pMenu->getSelectionIndex ( ) ]->getUserAny ( ) );
        m_pTitleLabel->setCaption ( pMenu->getSelectedItem ( ) );
        m_pDescBox->setText ( "Category: " + pSample->getInfo ( ) [ "Category" ] + "\nDescription: " + pSample->getInfo ( ) [ "Description" ] );
        
        if ( m_pCurrentSample != pSample )
        {
            ((Button*) m_pTrayMgr->getWidget ( "StartStop" ) )->setCaption ( "Start Sample" );
        }
        else
        {
            ((Button*) m_pTrayMgr->getWidget ( "StartStop" ) )->setCaption ( "Stop Sample" );
        }
    }
    else if ( pMenu == m_pRendererMenu )    // renderer selected, so update all settings
    {
        while ( m_pTrayMgr->getNumWidgets ( m_pRendererMenu->getTrayLocation ( ) ) > 3 )
        {
            m_pTrayMgr->destroyWidget ( m_pRendererMenu->getTrayLocation ( ), 3 );
        }
        
        ConfigOptionMap&  aOptions = m_pRoot->getRenderSystemByName ( pMenu->getSelectedItem ( ) )->getConfigOptions ( );
        
        KDuint i = 0;
        
        // create all the config option select menus
        for ( ConfigOptionMap::iterator it = aOptions.begin ( ); it != aOptions.end ( ); it++ )
        {
            i++;
            SelectMenu*  pOptionMenu = m_pTrayMgr->createLongSelectMenu
            (
                TL_LEFT, "ConfigOption" + Ogre::StringConverter::toString ( i ), it->first, 450, 240, 10
            );
            pOptionMenu->setItems ( it->second.possibleValues );
            
            // if the current config value is not in the menu, add it
            try
            {
                pOptionMenu->selectItem(it->second.currentValue);
            }
            catch ( Exception e )
            {
                pOptionMenu->addItem    ( it->second.currentValue );
                pOptionMenu->selectItem ( it->second.currentValue );
            }
        }
        
        windowResized ( m_pWindow );
    }
}

KDvoid SampleBrowser::sliderMoved ( Slider* pSlider )
{
    // format the caption to be fraction style
    String  sDenom = "/" + StringConverter::toString ( m_pSampleMenu->getNumItems ( ) );
    pSlider->setValueCaption ( pSlider->getValueCaption ( ) + sDenom );
    
    // tell the sample menu to change if it hasn't already
    if ( m_pSampleMenu->getSelectionIndex ( ) != -1 && m_pSampleMenu->getSelectionIndex ( ) != pSlider->getValue ( ) - 1 )
    {
         m_pSampleMenu->selectItem ( pSlider->getValue ( ) - 1 );
    }
}

bool SampleBrowser::touchMoved ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchMoved ( tTouches ) )
    {
        return true;
    }
    /*
    if ( !( m_pCurrentSample && !m_bSamplePaused ) && m_pTitleLabel->getTrayLocation ( ) != TL_NONE &&
        orientedEvt.state.Z.rel != 0 && m_pSampleMenu->getNumItems ( ) != 0 )
    {
        int newIndex = m_pSampleMenu->getSelectionIndex ( ) - orientedEvt.state.Z.rel / Math::Abs ( orientedEvt.state.Z.rel );
        m_pSampleMenu->selectItem ( Math::Clamp<KDint> ( newIndex, 0, m_pSampleMenu->getNumItems ( ) - 1 ) );
    }
    */
    
    try
    {
        return SampleContext::touchMoved ( tTouches );
    }
    catch ( Exception e )   // show error and fall back to menu
    {
        runSample ( 0 );
        m_pTrayMgr->showOkDialog ( "Error!", e.getDescription ( ) + "\nSource: " + e.getSource ( ) );
    }
    
    return true;

}

bool SampleBrowser::touchPressed ( const KDEventTouches& tTouches )
{
    if ( xmGetPressedTouchesCount ( ) == 4 || ( tTouches.touches [ 0 ].x < 64 && tTouches.touches [ 0 ].y < 64 ) )
    {
        if ( m_pTitleLabel->getTrayLocation() != TL_NONE )
        {
            // if we're in the main screen and a sample's running, toggle sample pause state
            if ( m_pCurrentSample )
            {
                if ( m_bSamplePaused )
                {
                    m_pTrayMgr->hideAll ( );
                    unpauseCurrentSample ( );
                }
                else
                {
                    pauseCurrentSample ( );
                    m_pTrayMgr->showAll ( );
                }
            }
        }
        else
        {
            buttonHit ( (Button*) m_pTrayMgr->getWidget ( "Back" ) );  // if we're in config, just go back
        }
    }
    
    if ( m_pTrayMgr->injectTouchPressed ( tTouches ) )
    {
        return true;
    }
    
    Vector2  tCursorPos ( tTouches.touches [ 0 ].x, m_pWindow->getHeight ( ) - tTouches.touches [ 0 ].y );
    
    if ( m_pTitleLabel->getTrayLocation() != TL_NONE )
    {
        for ( KDuint i = 0; i < m_aThumbs.size ( ); i++ )
        {
            if ( m_aThumbs[i]->isVisible ( ) && Widget::isCursorOver ( m_aThumbs[i], tCursorPos, 0 ) )
            {
                m_pSampleMenu->selectItem ( i );
                break;
            }
        }
    }
    
    try
    {
        return SampleContext::touchPressed ( tTouches );
    }
    catch ( Exception e )   // show error and fall back to menu
    {
        runSample ( 0 );
        m_pTrayMgr->showOkDialog ( "Error!", e.getDescription ( ) + "\nSource: " + e.getSource ( ) );
    }
    
    return true;
}

bool SampleBrowser::touchReleased ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchReleased ( tTouches ) )
    {
        return true;
    }
    
    try
    {
        return SampleContext::touchReleased ( tTouches );
    }
    catch ( Exception e )   // show error and fall back to menu
    {
        runSample ( 0 );
        m_pTrayMgr->showOkDialog ( "Error!", e.getDescription ( ) + "\nSource: " + e.getSource ( ) );
    }
    
    return true;
}

KDvoid SampleBrowser::setup ( KDvoid )
{
    m_pWindow = createWindow ( );
    
//    setupInput ( m_bNoGrabInput );
    
    locateResources ( );
    
    // Check if the render system supports any shader profiles.
    // Don't load samples that require shaders if we don't have any shader support, GL ES 1.x for example.
    const RenderSystemCapabilities*  pCaps = m_pRoot->getRenderSystem ( )->getCapabilities ( );
    RenderSystemCapabilities::ShaderProfiles  aProfiles = pCaps->getSupportedShaderProfiles ( );
    
    m_aPluginNameMap [ "Sample_BezierPatch"       ] = (SdkSample *) OGRE_NEW Sample_BezierPatch       ( );
    m_aPluginNameMap [ "Sample_CameraTrack"       ] = (SdkSample *) OGRE_NEW Sample_CameraTrack       ( );
    m_aPluginNameMap [ "Sample_Character"         ] = (SdkSample *) OGRE_NEW Sample_Character         ( );
    m_aPluginNameMap [ "Sample_DynTex"            ] = (SdkSample *) OGRE_NEW Sample_DynTex            ( );
    m_aPluginNameMap [ "Sample_FacialAnimation"   ] = (SdkSample *) OGRE_NEW Sample_FacialAnimation   ( );
    m_aPluginNameMap [ "Sample_Grass"             ] = (SdkSample *) OGRE_NEW Sample_Grass             ( );
    m_aPluginNameMap [ "Sample_Instancing"        ] = (SdkSample *) OGRE_NEW Sample_Instancing        ( );
    m_aPluginNameMap [ "Sample_Lighting"          ] = (SdkSample *) OGRE_NEW Sample_Lighting          ( );
    m_aPluginNameMap [ "Sample_NewInstancing"     ] = (SdkSample *) OGRE_NEW Sample_NewInstancing     ( );
    m_aPluginNameMap [ "Sample_ParticleFX"        ] = (SdkSample *) OGRE_NEW Sample_ParticleFX        ( );
    m_aPluginNameMap [ "Sample_Shadows"           ] = (SdkSample *) OGRE_NEW Sample_Shadows           ( );
    m_aPluginNameMap [ "Sample_SkeletalAnimation" ] = (SdkSample *) OGRE_NEW Sample_SkeletalAnimation ( );
    m_aPluginNameMap [ "Sample_SkyBox"            ] = (SdkSample *) OGRE_NEW Sample_SkyBox            ( );
    m_aPluginNameMap [ "Sample_SkyDome"           ] = (SdkSample *) OGRE_NEW Sample_SkyDome           ( );
    m_aPluginNameMap [ "Sample_SkyPlane"          ] = (SdkSample *) OGRE_NEW Sample_SkyPlane          ( );
    m_aPluginNameMap [ "Sample_Smoke"             ] = (SdkSample *) OGRE_NEW Sample_Smoke             ( );
    m_aPluginNameMap [ "Sample_SphereMapping"     ] = (SdkSample *) OGRE_NEW Sample_SphereMapping     ( );
    m_aPluginNameMap [ "Sample_TextureFX"         ] = (SdkSample *) OGRE_NEW Sample_TextureFX         ( );
    m_aPluginNameMap [ "Sample_TextureArray"      ] = (SdkSample *) OGRE_NEW Sample_TextureArray      ( );
    m_aPluginNameMap [ "Sample_Transparency"      ] = (SdkSample *) OGRE_NEW Sample_Transparency      ( );

    // Needed RTSHADER_SYSTEM
    m_aPluginNameMap [ "Sample_BSP"               ] = (SdkSample *) OGRE_NEW Sample_BSP               ( );
    m_aPluginNameMap [ "Sample_CelShading"        ] = (SdkSample *) OGRE_NEW Sample_CelShading        ( );
    m_aPluginNameMap [ "Sample_Compositor"        ] = (SdkSample *) OGRE_NEW Sample_Compositor        ( );
    m_aPluginNameMap [ "Sample_CubeMapping"       ] = (SdkSample *) OGRE_NEW Sample_CubeMapping       ( );
    m_aPluginNameMap [ "Sample_DeferredShading"   ] = (SdkSample *) OGRE_NEW Sample_DeferredShading   ( );
    m_aPluginNameMap [ "Sample_Dot3Bump"          ] = (SdkSample *) OGRE_NEW Sample_Dot3Bump          ( );
    m_aPluginNameMap [ "Sample_DualQuaternion"    ] = (SdkSample *) OGRE_NEW Sample_DualQuaternion    ( );
    m_aPluginNameMap [ "Sample_Fresnel"           ] = (SdkSample *) OGRE_NEW Sample_Fresnel           ( );    
    m_aPluginNameMap [ "Sample_Ocean"             ] = (SdkSample *) OGRE_NEW Sample_Ocean             ( );
    m_aPluginNameMap [ "Sample_Water"             ] = (SdkSample *) OGRE_NEW Sample_Water             ( );
    m_aPluginNameMap [ "Sample_ShaderSystem"      ] = (SdkSample *) OGRE_NEW Sample_ShaderSystem      ( );
    m_aPluginNameMap [ "Sample_SSAO"              ] = (SdkSample *) OGRE_NEW Sample_SSAO              ( );
    m_aPluginNameMap [ "Sample_Terrain"           ] = (SdkSample *) OGRE_NEW Sample_Terrain           ( );
    m_aPluginNameMap [ "Sample_ParticleGS"        ] = (SdkSample *) OGRE_NEW Sample_ParticleGS        ( );
    
    ResourceGroupManager::getSingleton ( ).initialiseResourceGroup ( "Essential" );
    
    m_pTrayMgr = new SdkTrayManager ( "BrowserControls", m_pWindow, this );
    m_pTrayMgr->showBackdrop ( "SdkTrays/Bands" );
    m_pTrayMgr->getTrayContainer ( TL_NONE )->hide ( );
    
    createDummyScene ( );
    
#ifdef USE_RTSHADER_SYSTEM
    if ( m_pRoot->getRenderSystem ( )->getCapabilities ( )->hasCapability ( RSC_FIXED_FUNCTION ) == false )
    {
        RTShader::ShaderGenerator::getSingletonPtr ( )->addSceneManager ( m_pRoot->getSceneManager ( "DummyScene" ) );
    }
#endif // USE_RTSHADER_SYSTEM
    
    loadResources ( );
  
    Sample*  pStartupSample = loadSamples ( );
    
    TextureManager::getSingleton ( ).setDefaultNumMipmaps ( 5 );
    
    // adds context as listener to process context-level (above the sample level) events
    m_pRoot->addFrameListener ( this );
    WindowEventUtilities::addWindowEventListener ( m_pWindow, this );
    
    // create template material for sample thumbnails
    MaterialPtr  pThumbMat = MaterialManager::getSingleton ( ).create ( "SdkTrays/SampleThumbnail", "Essential" );
    pThumbMat->getTechnique ( 0 )->getPass ( 0 )->createTextureUnitState ( );
    
    setupWidgets ( );
    windowResized ( m_pWindow );   // adjust menus for resolution
    
    // if this is our first time running, and there's a startup sample, run it
    if ( pStartupSample && m_bFirstRun )
    {
        runSample ( pStartupSample );
    }
}

KDvoid SampleBrowser::loadResources ( KDvoid )
{
    m_pTrayMgr->showLoadingBar ( 1, 0 );

    ResourceGroupManager::getSingleton ( ).initialiseResourceGroup ( "Popular" );

    m_pTrayMgr->hideLoadingBar ( );
}

KDvoid SampleBrowser::createDummyScene ( KDvoid )
{
    m_pWindow->removeAllViewports ( );
    
    SceneManager*  pSceneMgr = m_pRoot->createSceneManager ( ST_GENERIC, "DummyScene" );
    Camera*  pCamera = pSceneMgr->createCamera ( "DummyCamera" );
    
    m_pWindow->addViewport ( pCamera );
    
#ifdef USE_RTSHADER_SYSTEM
    // Initialize shader generator.
    // Must be before resource loading in order to allow parsing extended material attributes.
    KDbool  bSuccess = initializeRTShaderSystem ( pSceneMgr );
    if ( !bSuccess )
    {
        OGRE_EXCEPT ( Exception::ERR_FILE_NOT_FOUND,
                      "Shader Generator Initialization failed - Core shader libs path not found",
                      "Sample::_setup" );
    }
    if ( m_pRoot->getRenderSystem ( )->getCapabilities ( )->hasCapability ( RSC_FIXED_FUNCTION ) == false )
    {
        //newViewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
        
        // creates shaders for base material BaseWhite using the RTSS
        MaterialPtr  pBaseWhite = MaterialManager::getSingleton ( ).getByName ( "BaseWhite", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
        pBaseWhite->setLightingEnabled ( false );
        m_pShaderGenerator->createShaderBasedTechnique
        (
            "BaseWhite", MaterialManager::DEFAULT_SCHEME_NAME, RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME
        );
        m_pShaderGenerator->validateMaterial ( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, "BaseWhite" );
        pBaseWhite->getTechnique ( 0 )->getPass ( 0 )->setVertexProgram
        (
            pBaseWhite->getTechnique ( 1 )->getPass ( 0 )->getVertexProgram ( )->getName ( )
        );
        pBaseWhite->getTechnique ( 0 )->getPass ( 0 )->setFragmentProgram
        (
            pBaseWhite->getTechnique ( 1 )->getPass ( 0 )->getFragmentProgram ( )->getName ( )
        );
        
        // creates shaders for base material BaseWhiteNoLighting using the RTSS
        m_pShaderGenerator->createShaderBasedTechnique
        (
            "BaseWhiteNoLighting", MaterialManager::DEFAULT_SCHEME_NAME, RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME
        );
        m_pShaderGenerator->validateMaterial ( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, "BaseWhiteNoLighting" );
        MaterialPtr  pBaseWhiteNoLighting = MaterialManager::getSingleton ( ).getByName
        (
            "BaseWhiteNoLighting", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
        );
        pBaseWhiteNoLighting->getTechnique ( 0 )->getPass ( 0 )->setVertexProgram
        (
            pBaseWhiteNoLighting->getTechnique ( 1 )->getPass ( 0 )->getVertexProgram ( )->getName ( )
        );
        pBaseWhiteNoLighting->getTechnique ( 0 )->getPass ( 0 )->setFragmentProgram
        (
            pBaseWhiteNoLighting->getTechnique ( 1 )->getPass ( 0 )->getFragmentProgram ( )->getName ( )
        );
    }
#endif // USE_RTSHADER_SYSTEM
    
}

Sample* SampleBrowser::loadSamples ( KDvoid )
{
    Sample*  pStartupSample = 0;
    
    StringVector  aUnloadedSamplePlugins;

    Ogre::ConfigFile  tConfig;
    tConfig.load ( "/res/config/samples.cfg" );
    
    String          sSampleDir  = tConfig.getSetting ( "SampleFolder" );
    StringVector    aSampleList = tConfig.getMultiSetting ( "SamplePlugin" );
    String  sStartupSampleTitle = tConfig.getSetting ( "StartupSample" );
    
    // loop through all sample plugins...
    for ( StringVector::iterator i = aSampleList.begin ( ); i != aSampleList.end ( ); i++ )
    {
        try   // try to load the plugin
        {
            String  sSampleName = *i;
            
            SdkSample*  pPluginInstance = (SdkSample *) m_aPluginNameMap [ sSampleName ];
            if ( pPluginInstance )
            {
                SamplePlugin*  pSamplePlugin = OGRE_NEW SamplePlugin ( pPluginInstance->getInfo ( ) [ "Title" ] + " Sample" );
                
                pSamplePlugin->addSample ( pPluginInstance );
                m_pRoot->installPlugin ( pSamplePlugin );
            }
        }
        catch ( Exception e )   // plugin couldn't be loaded
        {
            aUnloadedSamplePlugins.push_back ( sSampleDir + *i );
            continue;
        }
        
        Plugin*  pPlugin = m_pRoot->getInstalledPlugins ( ).back ( );   // acquire plugin instance
        SamplePlugin*  pSamplePlugin = dynamic_cast<SamplePlugin*> ( pPlugin );
        
        if ( !pSamplePlugin )  // this is not a SamplePlugin, so unload it
        {
            continue;
        }
        
        m_aLoadedSamplePlugins.push_back ( sSampleDir + *i );   // add to records
        
        // go through every sample in the plugin...
        SampleSet  aNewSamples = pSamplePlugin->getSamples ( );
        for ( SampleSet::iterator j = aNewSamples.begin ( ); j != aNewSamples.end ( ); j++ )
        {
            NameValuePairList&  tInfo = ( *j )->getInfo ( );   // acquire custom sample info
            NameValuePairList::iterator  k;
            
            // give sample default title and category if none found
            k = tInfo.find ( "Title" );
            if ( k == tInfo.end ( ) || k->second.empty ( ) )
            {
                tInfo [ "Title" ] = "Untitled";
            }
            
            k = tInfo.find ( "Category" );
            if ( k == tInfo.end ( ) || k->second.empty ( ) )
            {
                tInfo [ "Category" ] = "Unsorted";
            }
            
            k = tInfo.find ( "Thumbnail" );
            if ( k == tInfo.end ( ) || k->second.empty ( ) )
            {
                tInfo [ "Thumbnail" ] = "thumb_error.png";
            }
            
            m_aLoadedSamples.insert ( *j );                         // add sample only after ensuring title for sorting
            m_aSampleCategories.insert ( tInfo [ "Category" ] );    // add sample category
            
            if ( tInfo [ "Title" ] == sStartupSampleTitle )
            {
                pStartupSample = *j;                                 // we found the startup sample
            }
        }
    }
    
    if ( !m_aLoadedSamples.empty ( ) )
    {
        m_aSampleCategories.insert ( "All" );                       // insert a category for all samples
    }
    
    if ( !aUnloadedSamplePlugins.empty ( ) )                        // show error message summarising missing or invalid plugins
    {
        String  sMessage = "These requested sample plugins were either missing, corrupt or invalid:";
        
        for ( KDuint i = 0; i < aUnloadedSamplePlugins.size ( ); i++ )
        {
            sMessage += "\n- " + aUnloadedSamplePlugins [ i ];
        }
        
        m_pTrayMgr->showOkDialog ( "Error!", sMessage );
    }
    
    return pStartupSample;
}

KDvoid SampleBrowser::unloadSamples ( KDvoid )
{
     const Root::PluginInstanceList  aPluginList = m_pRoot->getInstalledPlugins ( );
     for( KDuint i = 0; i < aPluginList.size ( ); i++ )
     {
         SamplePlugin*  pSamplePlugin = dynamic_cast<SamplePlugin*> ( aPluginList[i] );
     
         // This is a sample plugin so we can unload it
         if ( pSamplePlugin )
         {
             m_pRoot->uninstallPlugin ( aPluginList[i] );
         }
     }
     
     m_aLoadedSamples      .clear ( );
     m_aLoadedSamplePlugins.clear ( );
     m_aSampleCategories   .clear ( );
}

KDvoid SampleBrowser::setupWidgets ( KDvoid )
{
    m_pTrayMgr->destroyAllWidgets ( );
    
    // create main navigation tray
    m_pTrayMgr->showLogo ( TL_RIGHT );
    m_pTrayMgr->createSeparator ( TL_RIGHT, "LogoSep" );
    
    m_pTrayMgr->createButton ( TL_RIGHT, "StartStop"   , "Start Sample", 120 );
    m_pTrayMgr->createButton ( TL_RIGHT, "UnloadReload", m_aLoadedSamples.empty ( ) ? "Reload Samples" : "Unload Samples" );
    m_pTrayMgr->createButton ( TL_RIGHT, "Configure"   , "Configure" );
    m_pTrayMgr->createButton ( TL_RIGHT, "Quit"        , "Quit" );

    // create sample viewing controls
    m_pTitleLabel   = m_pTrayMgr->createLabel           ( TL_LEFT, "SampleTitle" , "" );
    m_pDescBox      = m_pTrayMgr->createTextBox         ( TL_LEFT, "SampleInfo"  , "Sample Info"    , 120, 100 );
    m_pCategoryMenu = m_pTrayMgr->createThickSelectMenu ( TL_LEFT, "CategoryMenu", "Select Category", 120, 10 );
    m_pSampleMenu   = m_pTrayMgr->createThickSelectMenu ( TL_LEFT, "SampleMenu"  , "Select Sample"  , 120, 10 );
    m_pSampleSlider = m_pTrayMgr->createThickSlider     ( TL_LEFT, "SampleSlider", "Slide Samples"  , 120, 42, 0, 0, 0 );
    

    // Sliders do not notify their listeners on creation, so we manually call the callback here
    // to format the slider value correctly. 
    sliderMoved ( m_pSampleSlider );
    
    // create configuration screen button tray
    m_pTrayMgr->createButton ( TL_NONE, "Apply", "Apply Changes" );
    m_pTrayMgr->createButton ( TL_NONE, "Back" , "Go Back" );
    
    // create configuration screen label and renderer menu
    m_pTrayMgr->createLabel ( TL_NONE, "ConfigLabel", "Configuration" );
    
    m_pRendererMenu = m_pTrayMgr->createLongSelectMenu ( TL_NONE, "RendererMenu", "Render System", 216, 115, 10 );

    m_pTrayMgr->createSeparator ( TL_NONE, "ConfigSeparator" );
    
    // populate render system names
    Ogre::StringVector     rsNames;
    Ogre::RenderSystemList rsList = m_pRoot->getAvailableRenderers ( );
    for ( KDuint i = 0; i < rsList.size ( ); i++ )
    {
        rsNames.push_back ( rsList[i]->getName ( ) );
    }
    m_pRendererMenu->setItems ( rsNames );
    
    populateSampleMenus ( );
}

KDvoid SampleBrowser::populateSampleMenus ( KDvoid )
{
    Ogre::StringVector  aCategories;
    for ( std::set<String>::iterator it = m_aSampleCategories.begin ( ); it != m_aSampleCategories.end ( ); it++ )
    {
        aCategories.push_back ( *it );
    }
    
    m_pCategoryMenu->setItems ( aCategories );
    if ( m_pCategoryMenu->getNumItems ( ) != 0 )
    {
        m_pCategoryMenu->selectItem ( 0 );
    }
    else
    {
        itemSelected ( m_pCategoryMenu );   // if there are no items, we can't select one, so manually invoke callback
    }
}

KDvoid SampleBrowser::recoverLastSample ( KDvoid )
{
    // restore the view while we're at it too
    m_pCategoryMenu->selectItem ( m_nLastViewCategory );
    m_pSampleMenu  ->selectItem ( m_nLastViewTitle );
    
    if ( m_nLastSampleIndex != -1)
    {
        KDint  nIndex = -1;
        for ( SampleSet::iterator it = m_aLoadedSamples.begin ( ); it != m_aLoadedSamples.end ( ); it++ )
        {
            nIndex++;
            if ( nIndex == m_nLastSampleIndex )
            {
                runSample ( *it );
                ( *it )->restoreState ( m_aLastSampleState );
                m_pLastSample = 0;
                m_nLastSampleIndex = -1;
                m_aLastSampleState.clear ( );
            }
        }
        
        pauseCurrentSample ( );
        m_pTrayMgr->showAll ( );
    }
    
    buttonHit ( (Button*) m_pTrayMgr->getWidget ( "Configure" ) );
}

KDvoid SampleBrowser::reconfigure ( const String& sRenderer, NameValuePairList& aOptions )
{
    m_nLastViewCategory = m_pCategoryMenu->getSelectionIndex ( );
    m_nLastViewTitle    = m_pSampleMenu  ->getSelectionIndex ( );
    
    m_nLastSampleIndex = -1;
    KDuint  nIndex = -1;
    for ( SampleSet::iterator it = m_aLoadedSamples.begin ( ); it != m_aLoadedSamples.end ( ); it++ )
    {
        nIndex++;
        if ( *it == m_pCurrentSample )
        {
            m_nLastSampleIndex = nIndex;
            break;
        }
    }
    
    SampleContext::reconfigure ( sRenderer, aOptions );
}

KDvoid SampleBrowser::shutdown ( KDvoid )
{
    if ( m_pTrayMgr)
    {
        delete m_pTrayMgr;
        m_pTrayMgr = 0;
    }
    
    if ( !m_pCurrentSample && m_pRoot->getRenderSystem ( ) != KD_NULL )
    {
        destroyDummyScene ( );
    }
    
    m_pCategoryMenu  = 0;
    m_pSampleMenu    = 0;
    m_pSampleSlider  = 0;
    m_pTitleLabel    = 0;
    m_pDescBox       = 0;
    m_pRendererMenu  = 0;
    m_fCarouselPlace = 0;
    
    m_aHiddenOverlays.clear();
    m_aThumbs.clear();
    
    SampleContext::shutdown ( );
    
    unloadSamples ( );
    
#ifdef USE_RTSHADER_SYSTEM
    // Finalize the RT Shader System.
    finalizeRTShaderSystem ( );
#endif // USE_RTSHADER_SYSTEM
}

KDvoid SampleBrowser::destroyDummyScene ( KDvoid )
{
    SceneManager*  pDummyScene = m_pRoot->getSceneManager ( "DummyScene" );
    
#ifdef USE_RTSHADER_SYSTEM
    m_pShaderGenerator->removeSceneManager ( pDummyScene );
#endif
    
    m_pWindow->removeAllViewports ( );
    m_pRoot->destroySceneManager ( pDummyScene );
}

KDvoid SampleBrowser::pauseCurrentSample ( KDvoid )
{
    SampleContext::pauseCurrentSample ( );
    
    OverlayManager::OverlayMapIterator  it = OverlayManager::getSingleton ( ).getOverlayIterator ( );
    m_aHiddenOverlays.clear ( );
    
    while ( it.hasMoreElements ( ) )
    {
        Overlay*  pOver = it.getNext ( );
        if ( pOver->isVisible ( ) )                     // later, we don't want to unhide the initially hidden overlays
        {
            m_aHiddenOverlays.push_back ( pOver );      // save off hidden overlays so we can unhide them later
            pOver->hide ( );
        }
    }
}

KDvoid SampleBrowser::unpauseCurrentSample ( KDvoid )
{
    SampleContext::unpauseCurrentSample ( );
    
    for ( std::vector<Overlay*>::iterator  it = m_aHiddenOverlays.begin ( ); it != m_aHiddenOverlays.end ( ); it++ )
    {
        ( *it )->show ( );
    }
    
    m_aHiddenOverlays.clear ( );
}

KDbool SampleBrowser::initializeRTShaderSystem ( SceneManager* pSceneMgr )
{
#ifdef USE_RTSHADER_SYSTEM
    
    if ( RTShader::ShaderGenerator::initialize ( ) )
    {       
        m_pShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr ( );
        m_pShaderGenerator->addSceneManager ( pSceneMgr );
        
        // Setup core libraries and shader cache path.
        Ogre::StringVector              aGroupVector = ResourceGroupManager::getSingleton ( ).getResourceGroups ( );
        Ogre::StringVector::iterator    itGroup      = aGroupVector.begin ( );
        Ogre::StringVector::iterator    itGroupEnd   = aGroupVector.end ( );
        Ogre::String                    sShaderCoreLibsPath;
        Ogre::String                    sShaderCachePath;
        
        for ( ; itGroup != itGroupEnd; ++itGroup )
        {
            ResourceGroupManager::LocationList  aResLocationsList = ResourceGroupManager::getSingleton ( ).getResourceLocationList ( *itGroup );
            Ogre::ResourceGroupManager::LocationList::iterator  it = aResLocationsList.begin ( );
            Ogre::ResourceGroupManager::LocationList::iterator  itEnd = aResLocationsList.end ( );
            KDbool  bCoreLibsFound = KD_FALSE;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for ( ; it != itEnd; ++it )
            {
                if ( ( *it )->archive->getName ( ).find ( "RTShaderLib" ) != Ogre::String::npos )
                {
                    sShaderCoreLibsPath = (*it)->archive->getName() + "/cache/";
                    sShaderCachePath = sShaderCoreLibsPath;
                    bCoreLibsFound = KD_TRUE;
                    break;
                }
            }
            // Core libs path found in the current group.
            if ( bCoreLibsFound )
            {
                break;
            }
        }
        
        // Core shader libs not found -> shader generating will fail.
        if ( sShaderCoreLibsPath.empty ( ) )
        {
            return KD_FALSE;
        }
        
#ifdef _RTSS_WRITE_SHADERS_TO_DISK
        // Set shader cache path.
        m_pShaderGenerator->setShaderCachePath ( sShaderCachePath );
#endif
        // Create and register the material manager listener if it doesn't exist yet.
        if ( m_pMaterialMgrListener == KD_NULL )
        {
            m_pMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener ( m_pShaderGenerator );
            MaterialManager::getSingleton ( ).addListener ( m_pMaterialMgrListener );
        }        
    }
    
    return KD_TRUE;
#else
    return KD_FALSE;
#endif    
}

KDvoid SampleBrowser::finalizeRTShaderSystem ( KDvoid )
{
#ifdef USE_RTSHADER_SYSTEM
    // Restore default scheme.
    Ogre::MaterialManager::getSingleton ( ).setActiveScheme ( MaterialManager::DEFAULT_SCHEME_NAME );
    
    // Unregister the material manager listener.
    if ( m_pMaterialMgrListener != KD_NULL )
    {
        MaterialManager::getSingleton ( ).removeListener ( m_pMaterialMgrListener );
        delete m_pMaterialMgrListener;
        m_pMaterialMgrListener = KD_NULL;
    }
    
    // Finalize RTShader system.
    if ( m_pShaderGenerator != KD_NULL )
    {
        RTShader::ShaderGenerator::finalize ( );
        m_pShaderGenerator = KD_NULL;
    }
#endif // USE_RTSHADER_SYSTEM    
}

