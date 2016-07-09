/* ------------------------------------------------------------------------------------
 *
 *      File            SkyDome.cpp
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
#include "SkyDome.h"

Sample_SkyDome::Sample_SkyDome ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Sky Dome";
    m_aInfo [ "Description" ] = "Shows how to use skydomes (fixed-distance domes used for backgrounds).";
    m_aInfo [ "Thumbnail"   ] = "thumb_skydome.png";
    m_aInfo [ "Category"    ] = "Environment";
}

KDvoid Sample_SkyDome::sliderMoved ( Slider* pSlider )
{
    // use the values from the sliders to update the skydome properties
    m_pSceneMgr->setSkyDome ( true, "Examples/CloudySky", m_pCurvatureSlider->getValue ( ), m_pTilingSlider->getValue ( ) );
}

KDvoid Sample_SkyDome::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pCameraMan->setYawPitchDist ( Degree ( 0 ), Degree ( 0 ), 250 );
    m_pTrayMgr->showCursor ( );
    
    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, -30 ), 1000, 1000, 10, 10, true, 1, 8, 8, Vector3::UNIT_Z
    );
    
    // create a floor entity, give it a material, and place it at the origin
    Entity*  pFloor = m_pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/BumpyMetal" );
    m_pSceneMgr->getRootSceneNode()->attachObject ( pFloor );
    
    // create an ogre head entity and place it at the origin
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" ) );
    
    // create slider bars to control the dome curvature and texture tiling
    m_pCurvatureSlider = m_pTrayMgr->createThickSlider ( TL_TOPLEFT, "Curvature", "Dome Curvature", 200, 60, 0, 50, 11  );
    m_pTilingSlider    = m_pTrayMgr->createThickSlider ( TL_TOPLEFT, "Tiling"   , "Dome Tiling"   , 200, 60, 1, 20, 191 );
    
    // Here, we set default values for our sliders. We do not need to setup a skydome here, because when
    // slider values change, the sliderMoved callback is invoked, and we setup the skydome with the appropriate
    // values in there. See its definition above. 
    m_pCurvatureSlider->setValue ( 10 );
    m_pTilingSlider   ->setValue ( 8 );
}

KDvoid Sample_SkyDome::cleanupContent ( KDvoid )
{
    MeshManager::getSingleton ( ).remove ( "floor" );
}
