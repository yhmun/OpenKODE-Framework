/* ------------------------------------------------------------------------------------
 *
 *      File            Sample.cpp
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
#include "Sample.h"

Sample::Sample ( KDvoid )
{
    m_pRoot             = Root::getSingletonPtr();
    m_pWindow           = 0;
    m_pSceneMgr         = 0;
    m_bDone             = KD_TRUE;
    m_bResourcesLoaded  = KD_FALSE;
    m_bContentSetup     = KD_FALSE;
    m_pShaderGenerator  = KD_NULL;
}

Sample::~Sample ( KDvoid )
{
    
}

NameValuePairList& Sample::getInfo ( KDvoid )
{
    return m_aInfo;
}

KDvoid Sample::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
    
}

String Sample::getRequiredRenderSystem ( KDvoid )
{
    return "";
}

StringVector Sample::getRequiredPlugins ( KDvoid )
{
    return StringVector ( );
}

SceneManager* Sample::getSceneManager ( KDvoid )
{
    return m_pSceneMgr;
}

KDbool Sample::isDone ( KDvoid )
{
    return m_bDone;
}

KDvoid Sample::_setup ( RenderWindow* pWindow )
{
    // assign mRoot here in case Root was initialised after the Sample's constructor ran.
    m_pRoot   = Root::getSingletonPtr ( );
    m_pWindow = pWindow;
    
    locateResources ( );
    
    createSceneManager ( );
    
    setupView ( );
    
    loadResources ( );
    m_bResourcesLoaded = KD_TRUE;
    
    setupContent ( );
    m_bContentSetup = KD_TRUE;
    
    m_bDone = KD_FALSE;
}

KDvoid Sample::_shutdown ( KDvoid )
{
    if ( m_bContentSetup )
    {
        cleanupContent ( );
    }
    
    if ( m_pSceneMgr )
    {
        m_pSceneMgr->clearScene ( );
    }
    
    m_bContentSetup = KD_FALSE;
    
    if ( m_bResourcesLoaded )
    {
        unloadResources ( );
    }
    m_bResourcesLoaded = KD_FALSE;
    
    if ( m_pSceneMgr )
    {
#ifdef USE_RTSHADER_SYSTEM
        m_pShaderGenerator->removeSceneManager ( m_pSceneMgr );
#endif
        m_pRoot->destroySceneManager ( m_pSceneMgr );
    }
    m_pSceneMgr = 0;
    
    m_bDone = KD_TRUE;
}

KDvoid Sample::paused ( KDvoid )
{
    
}

KDvoid Sample::unpaused ( KDvoid )
{
    
}

KDvoid Sample::saveState ( NameValuePairList& aState )
{
    
}

KDvoid Sample::restoreState ( NameValuePairList& aState )
{
    
}

bool Sample::frameStarted ( const FrameEvent& tEvent )
{
    return true;
}

bool Sample::frameRenderingQueued ( const FrameEvent& tEvent )
{
    return true;
}

bool Sample::frameEnded ( const FrameEvent& tEvent )
{
    return true;
}

bool Sample::touchMoved ( const KDEventTouches& tTouches )
{
    return true;
}

bool Sample::touchPressed ( const KDEventTouches& tTouches )
{
    return true;
}

bool Sample::touchReleased ( const KDEventTouches& tTouches )
{
    return true;
}

KDvoid Sample::locateResources ( KDvoid )
{
    
}

KDvoid Sample::loadResources ( KDvoid )
{
    
}

KDvoid Sample::createSceneManager ( KDvoid )
{
    m_pSceneMgr = Root::getSingleton ( ).createSceneManager ( ST_GENERIC );
    
#ifdef USE_RTSHADER_SYSTEM
    m_pShaderGenerator->addSceneManager ( m_pSceneMgr );
#endif
}

KDvoid Sample::setupView ( KDvoid )
{

}

KDvoid Sample::setupContent ( KDvoid )
{

}

KDvoid Sample::cleanupContent ( KDvoid )
{

}

KDvoid Sample::unloadResources ( KDvoid )
{
    ResourceGroupManager::ResourceManagerIterator it = ResourceGroupManager::getSingleton ( ).getResourceManagerIterator ( );
    
    while ( it.hasMoreElements ( ) )
    {
        it.getNext ( )->unloadUnreferencedResources ( );
    }
}

KDvoid Sample::setShaderGenerator ( RTShader::ShaderGenerator* pShaderGenerator )
{
    m_pShaderGenerator = pShaderGenerator;
}
