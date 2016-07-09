/* ------------------------------------------------------------------------------------
 *
 *      File            SampleContext.cpp
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
#include "SampleContext.h"

namespace Ogre
{
    void RenderStartPlugin ( void ) throw ( );
    void RenderStopPlugin  ( void );
}

SampleContext::SampleContext ( KDvoid )
{    
//    mFSLayer = OGRE_NEW_T(FileSystemLayerImpl, Ogre::MEMCATEGORY_GENERAL)(OGRE_VERSION_NAME);
    m_pRoot             = 0;
    m_pWindow           = 0;
    m_pCurrentSample    = 0;
    m_bSamplePaused     = false;
    m_bFirstRun         = true;
    m_bLastRun          = false;
    m_pLastSample       = 0;
//    mInputMgr = 0;
}

SampleContext::~SampleContext ( KDvoid )
{
    
}

KDvoid SampleContext::initApp ( Sample* pInitialSample )
{
    createRoot ( );

    RenderStartPlugin ( );
    
    if ( !oneTimeConfig ( ) )
    {
        return;
    }
    
    setup ( );
    
    if ( !m_bFirstRun )
    {
        recoverLastSample ( );
    }
    else if ( pInitialSample )
    {
        runSample ( pInitialSample );
    }
    
    Root::getSingleton ( ).getRenderSystem ( )->_initRenderTargets ( );
    
    // Clear event times
    Root::getSingleton ( ).clearEventTimes ( );
}

KDvoid SampleContext::closeApp ( KDvoid )
{
    m_pRoot->saveConfig ( );
    
    shutdown ( );
    
    if ( m_pRoot )
    {
        OGRE_DELETE m_pRoot;
    }
    
    RenderStopPlugin ( );
}

KDvoid SampleContext::render ( KDvoid )
{
    if ( m_pRoot && m_pRoot->getRenderSystem ( ) != NULL )
    {
        m_pRoot->renderOneFrame ( );
    }
}

KDvoid SampleContext::pauseCurrentSample ( KDvoid )
{
    if ( m_pCurrentSample && !m_bSamplePaused )
    {
        m_bSamplePaused = true;
        m_pCurrentSample->paused ( );
    }
}

KDvoid SampleContext::unpauseCurrentSample ( KDvoid )
{
    if ( m_pCurrentSample && m_bSamplePaused )
    {
        m_bSamplePaused = false;
        m_pCurrentSample->unpaused ( );
    }
}

KDvoid SampleContext::runSample ( Sample* pSample )
{
    if ( m_pCurrentSample )
    {
        m_pCurrentSample->_shutdown ( );            // quit current sample
        m_bSamplePaused = false;                    // don't pause the next sample
    }
    
    m_pWindow->removeAllViewports ( );              // wipe viewports
    
    if ( pSample )
    {
        // retrieve sample's required plugins and currently installed plugins
        Root::PluginInstanceList  aIp = m_pRoot->getInstalledPlugins ( );
        StringVector aRp = pSample->getRequiredPlugins ( );
        
        for ( StringVector::iterator j = aRp.begin ( ); j != aRp.end ( ); j++ )
        {
            bool  bFound = false;
            // try to find the required plugin in the current installed plugins
            for ( Root::PluginInstanceList::iterator k = aIp.begin ( ); k != aIp.end ( ); k++ )
            {
                if ( ( *k )->getName ( ) == *j )
                {
                    bFound = true;
                    break;
                }
            }
            if ( !bFound )  // throw an exception if a plugin is not found
            {
                String  sDesc = "Sample requires plugin: " + *j;
                String  sSrc  = "SampleContext::runSample";
                OGRE_EXCEPT ( Exception::ERR_NOT_IMPLEMENTED, sDesc, sSrc );
            }
        }
        
        // throw an exception if samples requires the use of another renderer
        String  sRrs = pSample->getRequiredRenderSystem ( );
        if ( !sRrs.empty ( ) && sRrs != m_pRoot->getRenderSystem ( )->getName ( ) )
        {
            String  sDesc = "Sample only runs with renderer: " + sRrs;
            String  sSrc  = "SampleContext::runSample";
            OGRE_EXCEPT ( Exception::ERR_INVALID_STATE, sDesc, sSrc );
        }
        
        // test system capabilities against sample requirements
        pSample->testCapabilities ( m_pRoot->getRenderSystem ( )->getCapabilities ( ) );
        
        pSample->_setup ( m_pWindow );  // start new sample        
    }
    
    m_pCurrentSample = pSample;
}

bool SampleContext::frameStarted ( const FrameEvent& tEvent )
{
//    captureInputDevices();      // capture input
    
    // manually call sample callback to ensure correct order
    return ( m_pCurrentSample && !m_bSamplePaused ) ? m_pCurrentSample->frameStarted ( tEvent ) : true;
}

bool SampleContext::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // manually call sample callback to ensure correct order
    return ( m_pCurrentSample && !m_bSamplePaused ) ? m_pCurrentSample->frameRenderingQueued ( tEvent ) : true;
}

bool SampleContext::frameEnded ( const FrameEvent& tEvent )
{
    // manually call sample callback to ensure correct order
    if ( m_pCurrentSample && !m_bSamplePaused && !m_pCurrentSample->frameEnded ( tEvent ) )
    {
        return false;
    }
    
    // quit if window was closed
    if ( m_pWindow->isClosed ( ) )
    {
        return false;
    }
    
    // go into idle mode if current sample has ended
    if ( m_pCurrentSample && m_pCurrentSample->isDone ( ) )
    {
        runSample ( 0 );
    }
        
    return true;
}

bool SampleContext::touchMoved ( const KDEventTouches& tTouches )
{
    if ( m_pCurrentSample && !m_bSamplePaused )
    {
        return m_pCurrentSample->touchMoved ( tTouches );
    }
    return true;
}

bool SampleContext::touchPressed ( const KDEventTouches& tTouches )
{
    if ( m_pCurrentSample && !m_bSamplePaused )
    {
        return m_pCurrentSample->touchPressed ( tTouches );
    }
    return true;
}

bool SampleContext::touchReleased ( const KDEventTouches& tTouches )
{
    if ( m_pCurrentSample && !m_bSamplePaused )
    {
        return m_pCurrentSample->touchReleased ( tTouches );
    }
    return true;
}

KDvoid SampleContext::setup ( KDvoid )
{
    m_pWindow = createWindow ( );
    
//    setupInput ( );
    
    locateResources ( );
      loadResources ( );
    
    TextureManager::getSingleton ( ).setDefaultNumMipmaps ( 5 );
    
    // adds context as listener to process context-level (above the sample level) events
    m_pRoot->addFrameListener ( this );
}

KDvoid SampleContext::createRoot ( KDvoid )
{
    if ( !m_pRoot )
    {
        m_pRoot = OGRE_NEW Ogre::Root ( );
    }
    
    
}

bool SampleContext::oneTimeConfig ( KDvoid )
{
    return m_pRoot->restoreConfig ( );
}

RenderWindow* SampleContext::createWindow ( KDvoid )
{
    return m_pRoot->initialise ( true );
}

KDvoid SampleContext::locateResources ( KDvoid )
{
    ConfigFile  tConfig;
    tConfig.load ( "/res/config/resources.cfg" );
    
    ConfigFile::SectionIterator  itSec = tConfig.getSectionIterator ( );
    
    String  sSecName, sTypeName, sArchName;
    while ( itSec.hasMoreElements ( ) )
    {
        sSecName = itSec.peekNextKey ( );
		ConfigFile::SettingsMultiMap*  pSettings = itSec.getNext ( );
        ConfigFile::SettingsMultiMap::iterator  it;
        for ( it = pSettings->begin ( ); it != pSettings->end ( ); ++it )
        {
            sTypeName = it->first;
            sArchName = it->second;
            
            ResourceGroupManager::getSingleton ( ).addResourceLocation ( sArchName, sTypeName, sSecName );
        }
    }
}

KDvoid SampleContext::loadResources ( KDvoid )
{
    ResourceGroupManager::getSingleton ( ).initialiseAllResourceGroups ( );
}

KDvoid SampleContext::reconfigure ( const String& sRenderer, NameValuePairList& aOptions )
{
    // save current sample state
    m_pLastSample = m_pCurrentSample;
    if ( m_pCurrentSample )
    {
        m_pCurrentSample->saveState ( m_aLastSampleState );
    }
    
    m_sNextRenderer = sRenderer;
    RenderSystem*  pRs = m_pRoot->getRenderSystemByName ( sRenderer );
    
    // set all given render system options
    for ( NameValuePairList::iterator it = aOptions.begin ( ); it != aOptions.end ( ); it++ )
    {
        pRs->setConfigOption ( it->first, it->second );
    }
    
    m_bLastRun = false;               // we want to go again with the new settings
//    m_pRoot->queueEndRendering ( );   // break from render loop
}

KDvoid SampleContext::recoverLastSample ( KDvoid )
{
    runSample ( m_pLastSample );
    m_pLastSample->restoreState ( m_aLastSampleState );
    m_pLastSample = 0;
    m_aLastSampleState.clear ( );
}

KDvoid SampleContext::shutdown ( KDvoid )
{
    if ( m_pCurrentSample )
    {
        m_pCurrentSample->_shutdown ( );
        m_pCurrentSample = 0;
    }
    
    // remove window event listener before shutting down OIS
    WindowEventUtilities::removeWindowEventListener ( m_pWindow, this );
    
//    shutdownInput ( );
}