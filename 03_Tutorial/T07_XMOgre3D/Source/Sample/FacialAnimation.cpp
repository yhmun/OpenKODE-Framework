/* ------------------------------------------------------------------------------------
 *
 *      File            FacialAnimation.cpp
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
#include "FacialAnimation.h"

Sample_FacialAnimation::Sample_FacialAnimation ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Facial Animation";
    m_aInfo [ "Description" ] = "A demonstration of the facial animation feature, using pose animation.";
    m_aInfo [ "Thumbnail"   ] = "thumb_facial.png";
    m_aInfo [ "Category"    ] = "Animation";
    m_aInfo [ "Help"        ] = "Use the checkbox to enable/disable manual animation. "
    "When manual animation is enabled, use the sliders to adjust each pose's influence.";
}

bool Sample_FacialAnimation::frameRenderingQueued ( const FrameEvent& tEvent )
{
    if ( m_bPlayAnimation )
    {
        m_pSpeakAnimState->addTime ( tEvent.timeSinceLastFrame );
    }
    return SdkSample::frameRenderingQueued ( tEvent );  // don't forget the parent class updates!
}

KDvoid Sample_FacialAnimation::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.5, 0.5, 0.5 ) );
    m_pSceneMgr->createLight ( )->setPosition (   40,  60,  50 );
    m_pSceneMgr->createLight ( )->setPosition ( -120, -80, -50 );
    
    // pre-load the mesh so that we can tweak it with a manual animation
    m_pHeadMesh = MeshManager::getSingleton ( ).load ( "facial.mesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    
    // create a manual animation, create a pose track for it, and create a keyframe in that track
    m_pManualKeyFrame = m_pHeadMesh->createAnimation ( "Manual", 0 )->createVertexTrack ( 4, VAT_POSE )->createVertexPoseKeyFrame ( 0 );
    
    // create pose references for the first 15 poses
    for ( KDuint i = 0; i < 15; i++ )
    {
        m_pManualKeyFrame->addPoseReference ( i, 0 );
    }
    
    // create a head entity from the mesh and attach it to a node with a vertical offset to center it
    Entity*  pHead = m_pSceneMgr->createEntity ( "Head", "facial.mesh" );
    m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( Vector3 ( 0, -30, 0 ) )->attachObject ( pHead );
    
    // get the animation states
    m_pSpeakAnimState  = pHead->getAnimationState ( "Speak" );
    m_pManualAnimState = pHead->getAnimationState ( "Manual" );
    
    // make the camera orbit around the head, and show the cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pCameraMan->setYawPitchDist ( Radian ( 0 ), Radian ( 0 ), 130 );
    m_pTrayMgr->showCursor ( );
    
    m_bPlayAnimation = true;   // by default, the speaking animation is enabled
    
    setupControls ( );
}

KDvoid Sample_FacialAnimation::setupControls ( KDvoid )
{
    // make logo and frame stats a little more compact to make room for controls
    m_pTrayMgr->showLogo ( TL_BOTTOMLEFT );
    m_pTrayMgr->toggleAdvancedFrameStats ( );
    
    // create group labels for the different sliders
    m_aExpressions.push_back ( m_pTrayMgr->createLabel ( TL_NONE, "ExpressionLabel", "Expressions"  ) );
    m_aMouthShapes.push_back ( m_pTrayMgr->createLabel ( TL_NONE, "MouthShapeLabel", "Mouth Shapes" ) );
    
    // create sliders to adjust pose influence
    for ( KDuint i = 0; i < m_pManualKeyFrame->getPoseReferences ( ).size ( ); i++ )
    {
        String  sSliderName = "Pose" + StringConverter::toString ( i );
        String  sPoseName = m_pHeadMesh->getPose ( i )->getName ( );
        
        if ( sPoseName.find("Expression") != std::string::npos )
        {
            m_aExpressions.push_back ( m_pTrayMgr->createLongSlider ( TL_NONE, sSliderName, sPoseName.substr ( 11 ), 200, 80, 44, 0, 1, 11 ) );
        }
        else
        {
            m_aMouthShapes.push_back ( m_pTrayMgr->createLongSlider ( TL_NONE, sSliderName, sPoseName.substr ( 0, 1 ), 160, 80, 44, 0, 1, 11 ) );
        }
    }
    
    // checkbox to switch between automatic animation and manual animation.
    m_pTrayMgr->createCheckBox ( TL_TOP, "Manual", "Manual Animation" )->setChecked ( !m_bPlayAnimation );
}

KDvoid Sample_FacialAnimation::cleanupContent ( KDvoid )
{
    m_aExpressions.clear ( );
    m_aMouthShapes.clear ( );
    
    m_pSpeakAnimState  = 0;
    m_pManualAnimState = 0;
    m_pManualKeyFrame  = 0;
    m_bPlayAnimation   = false;
    
    MeshManager::getSingleton ( ).unload ( m_pHeadMesh->getHandle ( ) );
}

KDvoid Sample_FacialAnimation::checkBoxToggled ( CheckBox* pBox )
{
    m_bPlayAnimation = !pBox->isChecked ( );
    
    // toggle animation states
    m_pSpeakAnimState ->setEnabled (  m_bPlayAnimation );
    m_pManualAnimState->setEnabled ( !m_bPlayAnimation );
    
    // toggle expression controls
    for ( KDuint i = 0; i < m_aExpressions.size ( ); i++ )
    {
        m_pTrayMgr->moveWidgetToTray ( m_aExpressions [ i ], m_bPlayAnimation ? TL_NONE : TL_TOPLEFT );
        
        if ( m_bPlayAnimation )
        {
            m_aExpressions [ i ]->hide ( );
        }
        else
        {
            m_aExpressions [ i ]->show ( );
        }
    }
    
    // toggle mouth shape controls
    for ( KDuint i = 0; i < m_aMouthShapes.size ( ); i++ )
    {
        m_pTrayMgr->moveWidgetToTray ( m_aMouthShapes [ i ], m_bPlayAnimation ? TL_NONE : TL_TOPRIGHT );
        
        if ( m_bPlayAnimation )
        {
            m_aMouthShapes [ i ]->hide ( );
        }
        else
        {
            m_aMouthShapes [ i ]->show ( );
        }
    }
}

KDvoid Sample_FacialAnimation::sliderMoved ( Slider* pSlider )
{
    // update the pose reference controlled by this slider
    m_pManualKeyFrame->updatePoseReference ( StringConverter::parseInt ( pSlider->getName ( ).substr ( 4 ) ), pSlider->getValue ( ) );
    
    // dirty animation state since we're fudging this manually
    m_pManualAnimState->getParent ( )->_notifyDirty ( );
}
