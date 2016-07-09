/* ------------------------------------------------------------------------------------
 *
 *      File            SkyPlane.cpp
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
#include "Smoke.h"

Sample_Smoke::Sample_Smoke ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Smoke";
    m_aInfo [ "Description" ] = "Demonstrates depth-sorting of particles in particle systems.";
    m_aInfo [ "Thumbnail"   ] = "thumb_smoke.png";
    m_aInfo [ "Category"    ] = "Effects";
}

bool Sample_Smoke::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // spin the head around and make it float up and down
    m_pPivot->setPosition ( 0, Math::Sin ( m_pRoot->getTimer ( )->getMilliseconds ( ) / 150.0 ) * 10, 0 );
    m_pPivot->yaw ( Radian ( -tEvent.timeSinceLastFrame * 1.5 ) );
    return SdkSample::frameRenderingQueued ( tEvent );
}

KDvoid Sample_Smoke::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/EveningSkyBox" );
    
    // dim orange ambient and two bright orange lights to match the skybox
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.2, 0 ) );
    
    Light*  pLight = m_pSceneMgr->createLight ( );
    pLight->setPosition ( 2000, 1000, -1000 );
    pLight->setDiffuseColour ( 1, 0.5, 0 );
    pLight = m_pSceneMgr->createLight ( );
    pLight->setPosition ( -2000, 1000, 1000 );
    pLight->setDiffuseColour ( 1, 0.5, 0 );
    
    m_pPivot = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );  // create a pivot node
    
    // create a child node and attach an ogre head and some smoke to it
    SceneNode*  pHeadNode =  m_pPivot->createChildSceneNode ( Vector3 ( 100, 0, 0 ) );
    pHeadNode->attachObject ( m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" ) );
    pHeadNode->attachObject ( m_pSceneMgr->createParticleSystem ( "Smoke", "Examples/Smoke" ) );
    
    m_pCamera->setPosition ( 0, 30, 350 );
}

