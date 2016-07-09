/* ------------------------------------------------------------------------------------
 *
 *      File            SkyBox.cpp
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
#include "SkyBox.h"

Sample_SkyBox::Sample_SkyBox ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Sky Box";
    m_aInfo [ "Description" ] = "Shows how to use skyboxes (fixed-distance cubes used for backgrounds).";
    m_aInfo [ "Thumbnail"   ] = "thumb_skybox.png";
    m_aInfo [ "Category"    ] = "Environment";
}

KDvoid Sample_SkyBox::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    m_pSceneMgr->setSkyBox ( true, "Examples/SpaceSkyBox", 5000 );  // set our skybox
    
    // create a spaceship model, and place it at the origin
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( m_pSceneMgr->createEntity ( "Razor", "razor.mesh" ) );

    // create a particle system with 200 quota, then set its material and dimensions
    ParticleSystem*  pThrusters = m_pSceneMgr->createParticleSystem ( 25 );
    pThrusters->setMaterialName ( "Examples/Flare" );
    pThrusters->setDefaultDimensions ( 25, 25 );
    
    // create two emitters for our thruster particle system
    for ( KDuint i = 0; i < 2; i++ )
    {
        ParticleEmitter*  pEmitter = pThrusters->addEmitter ( "Point" );  // add a point emitter
        
        // set the emitter properties
        pEmitter->setAngle ( Degree ( 3 ) );
        pEmitter->setTimeToLive ( 0.5 );
        pEmitter->setEmissionRate ( 25 );
        pEmitter->setParticleVelocity ( 25 );
        pEmitter->setDirection ( Vector3::NEGATIVE_UNIT_Z);
        pEmitter->setColour ( ColourValue::White, ColourValue::Red );
        pEmitter->setPosition ( Vector3 ( i == 0 ? 5.7 : -18, 0, 0 ) );
    }
    
    // attach our thruster particles to the rear of the ship
    m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3 ( 0, 6.5, -67 ) )->attachObject ( pThrusters );
    
    // set the camera's initial position and orientation
    m_pCamera->setPosition ( 0, 0, 150 );
    m_pCamera->yaw ( Degree ( 5 ) );
}

