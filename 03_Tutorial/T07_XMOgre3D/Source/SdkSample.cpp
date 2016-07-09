/* ------------------------------------------------------------------------------------
 *
 *      File            SdkSample.cpp
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
#include "SdkSample.h"

SdkSample::SdkSample ( KDvoid )
{
    // so we don't have to worry about checking if these keys exist later
    m_aInfo [ "Title"       ] = "Untitled";
    m_aInfo [ "Description" ] = "";
    m_aInfo [ "Category"    ] = "Unsorted";
    m_aInfo [ "Thumbnail"   ] = "";
    m_aInfo [ "Help"        ] = "";
    
    m_pTrayMgr          = 0;
    m_pCameraMan        = 0;
    m_pCamera           = 0;
    m_pViewport         = 0;
    m_pDetailsPanel     = 0;
    m_bCursorWasVisible = false;
    m_bDragLook         = false;
}

SdkSample::~SdkSample ( KDvoid )
{
    
}

bool SdkSample::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pTrayMgr->frameRenderingQueued ( tEvent );
    
    if ( !m_pTrayMgr->isDialogVisible ( ) )
    {
        m_pCameraMan->frameRenderingQueued ( tEvent );   // if dialog isn't up, then update the camera
        
        if ( m_pDetailsPanel->isVisible ( ) )   // if details panel is visible, then update its contents
        {
            m_pDetailsPanel->setParamValue ( 0, StringConverter::toString ( m_pCamera->getDerivedPosition    ( ).x ) );
            m_pDetailsPanel->setParamValue ( 1, StringConverter::toString ( m_pCamera->getDerivedPosition    ( ).y ) );
            m_pDetailsPanel->setParamValue ( 2, StringConverter::toString ( m_pCamera->getDerivedPosition    ( ).z ) );
            m_pDetailsPanel->setParamValue ( 4, StringConverter::toString ( m_pCamera->getDerivedOrientation ( ).w ) );
            m_pDetailsPanel->setParamValue ( 5, StringConverter::toString ( m_pCamera->getDerivedOrientation ( ).x ) );
            m_pDetailsPanel->setParamValue ( 6, StringConverter::toString ( m_pCamera->getDerivedOrientation ( ).y ) );
            m_pDetailsPanel->setParamValue ( 7, StringConverter::toString ( m_pCamera->getDerivedOrientation ( ).z ) );
        }	
    }
    
    return true;
}

bool SdkSample::touchMoved ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchMoved ( tTouches ) )
    {
        return true;
    }
    
    m_pCameraMan->injectTouchMoved ( tTouches );
    
    return true;
}

bool SdkSample::touchPressed ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchPressed ( tTouches ) )
    {
        return true;
    }
    
    if ( m_bDragLook )
    {
        m_pCameraMan->setStyle ( CS_FREELOOK );
        m_pTrayMgr->hideCursor ( );
    }
    
    m_pCameraMan->injectTouchPressed ( tTouches );
    
    return true;
}

bool SdkSample::touchReleased ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchReleased ( tTouches ) )
    {
        return true;
    }
    
    if ( m_bDragLook )
    {
        m_pCameraMan->setStyle ( CS_MANUAL );
        m_pTrayMgr->showCursor ( );
    }
    
    m_pCameraMan->injectTouchReleased ( tTouches );
    
    return true;
}

KDvoid SdkSample::_setup ( RenderWindow* pWindow )
{
    // assign mRoot here in case Root was initialised after the Sample's constructor ran.
    m_pRoot = Root::getSingletonPtr ( );
    m_pWindow = pWindow;
    
    locateResources ( );
    createSceneManager ( );
    setupView ( );
    
    m_pTrayMgr = new SdkTrayManager ( "SampleControls", pWindow, this );  // create a tray interface
    
    loadResources ( );
    m_bResourcesLoaded = true;
    
    // show stats and logo and hide the cursor
    m_pTrayMgr->showFrameStats ( TL_BOTTOMLEFT );
    m_pTrayMgr->showLogo ( TL_BOTTOMRIGHT );
    m_pTrayMgr->hideCursor ( );
    
    // create a params panel for displaying sample details
    StringVector  aItems;
    aItems.push_back ( "cam.pX" );
    aItems.push_back ( "cam.pY" );
    aItems.push_back ( "cam.pZ" );
    aItems.push_back ( ""       );
    aItems.push_back ( "cam.oW" );
    aItems.push_back ( "cam.oX" );
    aItems.push_back ( "cam.oY" );
    aItems.push_back ( "cam.oZ" );
    aItems.push_back ( ""       );
    aItems.push_back ( "Filtering" );
    aItems.push_back ( "Poly Mode" );
    
    m_pDetailsPanel = m_pTrayMgr->createParamsPanel ( TL_NONE, "DetailsPanel", 200, aItems );
    m_pDetailsPanel->hide ( );
    
    m_pDetailsPanel->setParamValue (  9, "Bilinear" );
    m_pDetailsPanel->setParamValue ( 10, "Solid" );
    
    setupContent ( );
    m_bContentSetup = true;
    
    m_bDone = false;
}

KDvoid SdkSample::_shutdown ( KDvoid )
{
    Sample::_shutdown ( );
    
    if ( m_pTrayMgr )
    {
        delete m_pTrayMgr;
    }
    
    if ( m_pCameraMan )
    {
        delete m_pCameraMan;
    }
    
    // restore settings we may have changed, so as not to affect other samples
    MaterialManager::getSingleton ( ).setDefaultTextureFiltering ( TFO_BILINEAR );
    MaterialManager::getSingleton ( ).setDefaultAnisotropy ( 1 );
}

KDvoid SdkSample::setupView ( KDvoid )
{
    // setup default viewport layout and camera
    m_pCamera   = m_pSceneMgr->createCamera ( "MainCamera" );
    m_pViewport = m_pWindow->addViewport ( m_pCamera );
    m_pCamera->setAspectRatio ( (Real) m_pViewport->getActualWidth ( ) / (Real) m_pViewport->getActualHeight ( ) );
    m_pCamera->setNearClipDistance ( 5 );
    
    m_pCameraMan = new SdkCameraMan ( m_pCamera );   // create a default camera controller
}

KDvoid SdkSample::setDragLook ( bool bEnabled )
{
    if ( bEnabled )
    {
        m_pCameraMan->setStyle ( CS_MANUAL );
        m_pTrayMgr->showCursor ( );
        m_bDragLook = true;
    }
    else
    {
        m_pCameraMan->setStyle ( CS_FREELOOK );
        m_pTrayMgr->hideCursor ( );
        m_bDragLook = false;
    }
}
