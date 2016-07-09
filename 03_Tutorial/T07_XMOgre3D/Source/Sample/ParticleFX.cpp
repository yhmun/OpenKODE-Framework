/* ------------------------------------------------------------------------------------
 *
 *      File            ParticleFX.cpp
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
#include "ParticleFX.h"

Sample_ParticleFX::Sample_ParticleFX ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Particle Effects";
    m_aInfo [ "Description" ] = "Demonstrates the creation and usage of particle effects.";
    m_aInfo [ "Thumbnail"   ] = "thumb_particles.png";
    m_aInfo [ "Category"    ] = "Effects";
    m_aInfo [ "Help"        ] = "Use the checkboxes to toggle visibility of the individual particle systems.";
}

bool Sample_ParticleFX::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pFountainPivot->yaw ( Degree ( tEvent.timeSinceLastFrame * 30 ) );   // spin the fountains around
    
    return SdkSample::frameRenderingQueued ( tEvent );   // don't forget the parent class updates!
}

KDvoid Sample_ParticleFX::checkBoxToggled ( CheckBox* pBox )
{
    // show or hide the particle system with the same name as the check box
    m_pSceneMgr->getParticleSystem ( pBox->getName ( ) )->setVisible ( pBox->isChecked ( ) );
}

KDvoid Sample_ParticleFX::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pCameraMan->setYawPitchDist ( Degree ( 0 ), Degree ( 15 ), 250 );
    m_pTrayMgr->showCursor ( );
    
    // create an ogre head entity and place it at the origin
    Entity*  pEntity = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    
    setupParticles ( );   // setup particles
    setupTogglers  ( );   // setup particle togglers
}

KDvoid Sample_ParticleFX::setupParticles ( KDvoid )
{
    ParticleSystem::setDefaultNonVisibleUpdateTimeout ( 5 );  // set nonvisible timeout
    
    ParticleSystem*  pParticle;
    
    // create some nice fireworks and place it at the origin
    pParticle = m_pSceneMgr->createParticleSystem ( "Fireworks", "Examples/Fireworks" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pParticle );
    
    // create a green nimbus around the ogre head
    pParticle = m_pSceneMgr->createParticleSystem ( "Nimbus", "Examples/GreenyNimbus" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pParticle );
    
    pParticle = m_pSceneMgr->createParticleSystem ( "Rain", "Examples/Rain" );  // create a rainstorm
    pParticle->fastForward ( 5 );   // fast-forward the rain so it looks more natural
    m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3 ( 0, 1000, 0 ) )->attachObject ( pParticle );
    
    // create aureola around ogre head perpendicular to the ground
    pParticle = m_pSceneMgr->createParticleSystem("Aureola", "Examples/Aureola");
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pParticle );
    
    // create shared pivot node for spinning the fountains
    m_pFountainPivot = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    
    pParticle = m_pSceneMgr->createParticleSystem ( "Fountain1", "Examples/PurpleFountain" );  // create fountain 1
    // attach the fountain to a child node of the pivot at a distance and angle
    m_pFountainPivot->createChildSceneNode ( Vector3 ( 200, -100, 0 ), Quaternion ( Degree ( 20 ), Vector3::UNIT_Z ) )->attachObject ( pParticle );
    
    pParticle = m_pSceneMgr->createParticleSystem("Fountain2", "Examples/PurpleFountain");  // create fountain 2
    // attach the fountain to a child node of the pivot at a distance and angle
    m_pFountainPivot->createChildSceneNode ( Vector3 ( -200, -100, 0 ), Quaternion ( Degree ( -20 ), Vector3::UNIT_Z ) )->attachObject ( pParticle );
}

KDvoid Sample_ParticleFX::setupTogglers ( KDvoid )
{
    // create check boxes to toggle the visibility of our particle systems
    m_pTrayMgr->createLabel    ( TL_TOPLEFT, "VisLabel" , "Particles" );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Fireworks", "Fireworks" , 130 )->setChecked ( true );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Fountain1", "Fountain A", 130 )->setChecked ( true );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Fountain2", "Fountain B", 130 )->setChecked ( true );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Aureola"  , "Aureola"   , 130 )->setChecked ( true );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Nimbus"   , "Nimbus"    , 130 )->setChecked ( true );
    m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "Rain"     , "Rain"      , 130 )->setChecked ( true );
}
