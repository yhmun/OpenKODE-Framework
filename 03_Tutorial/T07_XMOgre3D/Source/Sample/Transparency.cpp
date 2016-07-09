/* ------------------------------------------------------------------------------------
 *
 *      File            Transparency.cpp
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
#include "Transparency.h"

Sample_Transparency::Sample_Transparency ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Transparency";
    m_aInfo [ "Description" ] = "Demonstrates the use of transparent materials (or scene blending).";
    m_aInfo [ "Thumbnail"   ] = "thumb_trans.png";
    m_aInfo [ "Category"    ] = "Lighting";
}

bool Sample_Transparency::frameRenderingQueued ( const FrameEvent& tEvent )
{
    Real  fTheta = m_pRoot->getTimer ( )->getMilliseconds ( ) / 1000.0f;
    
    // this is the equation for a PQ torus knot
    Ogre::Real r = 28 * ( 2 + Math::Sin ( fTheta * 3 / 2 + 0.2 ) );
    Ogre::Real x =  r * Math::Cos ( fTheta );
    Ogre::Real y =  r * Math::Sin ( fTheta );
    Ogre::Real z = 60 * Math::Cos ( fTheta * 3 / 2 + 0.2 );
    
    Vector3  tLastPos = m_pFishNode->getPosition ( );   // save fishy's last position
    m_pFishNode->setPosition ( x, y, z );               // set fishy's new position
    
    // set fishy's direction based on the change in position
    m_pFishNode->setDirection ( m_pFishNode->getPosition ( ) - tLastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X );
    
    m_pFishSwim->addTime ( tEvent.timeSinceLastFrame * 5 );     // update fishy's swimming animation
    
    return SdkSample::frameRenderingQueued ( tEvent );          // don't forget the parent class updates!
}

KDvoid Sample_Transparency::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/TrippySkyBox" );
    
    m_pCamera->setPosition ( 0, 0, 300 );   // set camera's starting position
    
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );   // add basic point light
    
    // create a torus knot model, give it the translucent texture, and attach it to the origin
    Entity*  pEntity = m_pSceneMgr->createEntity ( "Knot", "knot.mesh" );
    pEntity->setMaterialName ( "Examples/WaterStream" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    
    // create a fishy and enable its swimming animation
    pEntity = m_pSceneMgr->createEntity ( "Fish", "fish.mesh" );
    m_pFishSwim = pEntity->getAnimationState ( "swim" );
    m_pFishSwim->setEnabled ( true );
    
    // create a scene node, attach fishy to it, and scale it by a factor of 2
    m_pFishNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    m_pFishNode->attachObject ( pEntity );
    m_pFishNode->setScale ( 2, 2, 2 );
}

