/* ------------------------------------------------------------------------------------
 *
 *      File            BSP.cpp
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
#include "BSP.h"

Sample_BSP::Sample_BSP ( KDvoid )
{
    m_aInfo [ "Title"       ] = "BSP";
    m_aInfo [ "Description" ] = "A demo of the indoor, or BSP (Binary Space Partition) scene manager. "
                                "Also demonstrates how to load BSP maps from Quake 3.";
    m_aInfo [ "Thumbnail"   ] = "thumb_bsp.png";
    m_aInfo [ "Category"    ] = "Geometry";
}

StringVector Sample_BSP::getRequiredPlugins ( KDvoid )
{
    StringVector  aNames;
    aNames.push_back ( "BSP Scene Manager" );
    return aNames;
}

KDvoid Sample_BSP::createSceneManager ( KDvoid )
{
    m_pSceneMgr = m_pRoot->createSceneManager ( "BspSceneManager" );   // the BSP scene manager is required for this sample
}

KDvoid Sample_BSP::locateResources ( KDvoid )
{
    // load the Quake archive location and map name from a config file
    ConfigFile  tConfig;
    tConfig.load ( "/res/config/quakemap.cfg" );
    
    m_sArchive = tConfig.getSetting ( "Archive" );
    m_sMap     = tConfig.getSetting ( "Map" );
    
    // add the Quake archive to the world resource group
    ResourceGroupManager::getSingleton ( ).addResourceLocation
    (
        m_sArchive, "Zip", ResourceGroupManager::getSingleton ( ).getWorldResourceGroupName ( ), true
    );
}

KDvoid Sample_BSP::loadResources ( KDvoid )
{
    // NOTE: The browser initialises everything at the beginning already, so we use a 0 init proportion.
    // If you're not compiling this sample for use with the browser, then leave the init proportion at 0.7. 
    m_pTrayMgr->showLoadingBar ( 1, 1, 0 );
    
    // associate the world geometry with the world resource group, and then load the group
    ResourceGroupManager&  tRGMgr = ResourceGroupManager::getSingleton();
    tRGMgr.linkWorldGeometryToResourceGroup ( tRGMgr.getWorldResourceGroupName ( ), m_sMap, m_pSceneMgr );
    tRGMgr.initialiseResourceGroup ( tRGMgr.getWorldResourceGroupName ( ) );
    tRGMgr.loadResourceGroup ( tRGMgr.getWorldResourceGroupName ( ), false );
    
    m_pTrayMgr->hideLoadingBar ( );
}

KDvoid Sample_BSP::unloadResources ( KDvoid )
{
    // unload the map so we don't interfere with subsequent samples
    ResourceGroupManager&  tRGMgr = ResourceGroupManager::getSingleton ( );
    tRGMgr.unloadResourceGroup ( tRGMgr.getWorldResourceGroupName ( ) );
    tRGMgr.removeResourceLocation ( m_sArchive, ResourceGroupManager::getSingleton ( ).getWorldResourceGroupName ( ) );
}

KDvoid Sample_BSP::setupView ( KDvoid )
{
    SdkSample::setupView ( );
    
    // modify camera for close work
    m_pCamera->setNearClipDistance ( 4 );
    m_pCamera->setFarClipDistance  ( 4000 );
    
    // set a random player starting point
    ViewPoint tViewPoint = m_pSceneMgr->getSuggestedViewpoint ( true );
    
    // Quake uses the Z axis as the up axis, so make necessary adjustments
    m_pCamera->setFixedYawAxis ( true, Vector3::UNIT_Z );
    m_pCamera->pitch ( Degree ( 90 ) );
    
    m_pCamera->setPosition ( tViewPoint.position );
    m_pCamera->rotate ( tViewPoint.orientation );
    
    m_pCameraMan->setTopSpeed ( 350 );   // make the camera move a bit faster
}

