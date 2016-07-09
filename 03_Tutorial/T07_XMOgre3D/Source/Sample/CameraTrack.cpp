/* ------------------------------------------------------------------------------------
 *
 *      File            CameraTrack.cpp
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
#include "CameraTrack.h"

Sample_CameraTrack::Sample_CameraTrack ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Camera Tracking";
    m_aInfo [ "Description" ] = "An example of using AnimationTracks to make a node smoothly follow "
    "a predefined path with spline interpolation. Also uses the auto-tracking feature of the camera.";
    m_aInfo [ "Thumbnail"   ] = "thumb_camtrack.png";
    m_aInfo [ "Category"    ] = "Unsorted";
}

bool Sample_CameraTrack::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pAnimState->addTime ( tEvent.timeSinceLastFrame );   // increment animation time
    return SdkSample::frameRenderingQueued ( tEvent );
}

KDvoid Sample_CameraTrack::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    m_pSceneMgr->setSkyBox ( true, "Examples/MorningSkyBox" );
    
    // create an ogre head entity and attach it to a node
    Entity*    pHead = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    SceneNode* pHeadNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pHeadNode->attachObject ( pHead );
    
    m_pCameraMan->setStyle ( CS_MANUAL );               // we will be controlling the camera ourselves, so disable the camera man
    m_pCamera->setAutoTracking ( true, pHeadNode );     // make the camera face the head
    
    // create a camera node and attach camera to it
    SceneNode*  pCamNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pCamNode->attachObject ( m_pCamera );
    
    // set up a 10 second animation for our camera, using spline interpolation for nice curves
    Animation*  pAnim = m_pSceneMgr->createAnimation ( "CameraTrack", 10 );
    pAnim->setInterpolationMode ( Animation::IM_SPLINE );
    
    // create a track to animate the camera's node
    NodeAnimationTrack*  pTrack = pAnim->createNodeTrack ( 0, pCamNode );
    
    // create keyframes for our track
    pTrack->createNodeKeyFrame ( 0   )->setTranslate ( Vector3 (  200,   0,    0 ) );
    pTrack->createNodeKeyFrame ( 2.5 )->setTranslate ( Vector3 (    0, -50,  100 ) );
    pTrack->createNodeKeyFrame ( 5   )->setTranslate ( Vector3 ( -500, 100,    0 ) );
    pTrack->createNodeKeyFrame ( 7.5 )->setTranslate ( Vector3 (    0, 200, -300 ) );
    pTrack->createNodeKeyFrame ( 10  )->setTranslate ( Vector3 (  200,   0,    0 ) );
    
    // create a new animation state to track this
    m_pAnimState = m_pSceneMgr->createAnimationState ( "CameraTrack" );
    m_pAnimState->setEnabled ( true );
}

