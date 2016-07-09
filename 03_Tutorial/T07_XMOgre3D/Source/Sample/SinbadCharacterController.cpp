/* ------------------------------------------------------------------------------------
 *
 *      File            SinbadCharacterController.cpp
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
#include "SinbadCharacterController.h"

SinbadCharacterController::SinbadCharacterController ( Camera* pCamera )
{
    setupBody       ( pCamera->getSceneManager ( ) );
    setupCamera     ( pCamera );
    setupAnimations ( );
}

KDvoid SinbadCharacterController::addTime ( Real fDeltaTime )
{
    updateBody       ( fDeltaTime );
    updateAnimations ( fDeltaTime );
    updateCamera     ( fDeltaTime );
}

KDvoid SinbadCharacterController::injectTouchMoved ( const KDEventTouches& tTouches )
{
    updateCameraGoal
    (
        -0.5f * ( tTouches.touches[0].diff_x ),
         0.5f * ( tTouches.touches[0].diff_y ),
         0
    );
}

KDvoid SinbadCharacterController::injectTouchPressed ( const KDEventTouches& tTouches )
{
    if ( xmGetPressedTouchesCount ( ) == 2 )
    {
        if (  m_eBaseAnimID == ANIM_DANCE )
        {
            // stop dancing
            setBaseAnimation ( ANIM_IDLE_BASE );
            setTopAnimation ( ANIM_IDLE_TOP );
            // re-enable hand animation
            m_eAnims [ ANIM_HANDS_RELAXED ]->setEnabled ( true );
        }
        else
        {
            // start dancing
            setBaseAnimation ( ANIM_DANCE, true );
            setTopAnimation  ( ANIM_NONE );
            // disable hand animation because the dance controls hands
            m_eAnims [ ANIM_HANDS_RELAXED ]->setEnabled ( false );
        }
    }
    else if ( m_bSwordsDrawn && ( m_eTopAnimID == ANIM_IDLE_TOP || m_eTopAnimID == ANIM_RUN_TOP ) )
    {
        // if swords are out, and character's not doing something weird, then SLICE!
        setTopAnimation ( ANIM_SLICE_VERTICAL, true );
        m_fTimer = 0;
    }
    
    else if ( ( m_eTopAnimID == ANIM_IDLE_TOP || m_eTopAnimID == ANIM_RUN_TOP ) )
    {
        setTopAnimation ( ANIM_DRAW_SWORDS, true );
        m_fTimer = 0;
    }
}

KDvoid SinbadCharacterController::setupBody ( SceneManager* pSceneMgr )
{
    // create main model
    m_pBodyNode = pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( Vector3::UNIT_Y * CHAR_HEIGHT );
    m_pBodyEnt  = pSceneMgr->createEntity ( "SinbadBody", "Sinbad.mesh" );
    m_pBodyNode->attachObject ( m_pBodyEnt );
    
    // create swords and attach to sheath
    LogManager::getSingleton ( ).logMessage ( "Creating swords" );
    m_pSword1 = pSceneMgr->createEntity ( "SinbadSword1", "Sword.mesh" );
    m_pSword2 = pSceneMgr->createEntity ( "SinbadSword2", "Sword.mesh" );
    m_pBodyEnt->attachObjectToBone ( "Sheath.L", m_pSword1 );
    m_pBodyEnt->attachObjectToBone ( "Sheath.R", m_pSword2 );
    
    LogManager::getSingleton ( ).logMessage ( "Creating the chains" );
    // create a couple of ribbon trails for the swords, just for fun
    NameValuePairList  tParams;
    tParams [ "numberOfChains" ] = "2";
    tParams [ "maxElements" ] = "80";
    m_pSwordTrail = (RibbonTrail*) pSceneMgr->createMovableObject ( "RibbonTrail", &tParams );
    m_pSwordTrail->setMaterialName ( "Examples/LightRibbonTrail" );
    m_pSwordTrail->setTrailLength ( 20 );
    m_pSwordTrail->setVisible ( false );
    pSceneMgr->getRootSceneNode ( )->attachObject ( m_pSwordTrail );
    
    for ( KDint i = 0; i < 2; i++ )
    {
        m_pSwordTrail->setInitialColour ( i, 1, 0.8f, 0 );
        m_pSwordTrail->setColourChange  ( i, 0.75, 1.25, 1.25, 1.25 );
        m_pSwordTrail->setWidthChange   ( i, 1 );
        m_pSwordTrail->setInitialWidth  ( i, 0.5 );
    }
    
    m_tKeyDirection = Vector3::ZERO;
    m_fVerticalVelocity = 0;
}

KDvoid SinbadCharacterController::setupAnimations ( KDvoid )
{
    // this is very important due to the nature of the exported animations
    m_pBodyEnt->getSkeleton ( )->setBlendMode ( ANIMBLEND_CUMULATIVE );
    
    String  sAnimNames[] =
    {
        "IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
        "SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"
    };
    
    // populate our animation list
    for ( KDint i = 0; i < NUM_ANIMS; i++ )
    {
        m_eAnims[i] = m_pBodyEnt->getAnimationState ( sAnimNames[i] );
        m_eAnims[i]->setLoop ( true );
        
        m_bFadingIn [i] = false;
        m_bFadingOut[i] = false;
    }
    
    // start off in the idle state (top and bottom together)
    setBaseAnimation ( ANIM_IDLE_BASE );
    setTopAnimation  ( ANIM_IDLE_TOP );
    
    // relax the hands since we're not holding anything
    m_eAnims [ ANIM_HANDS_RELAXED ]->setEnabled ( true );
    
    m_bSwordsDrawn = false;
}

KDvoid SinbadCharacterController::setupCamera ( Camera* pCamera )
{
    // create a pivot at roughly the character's shoulder
    m_pCameraPivot = pCamera->getSceneManager ( )->getRootSceneNode ( )->createChildSceneNode ( );
    // this is where the camera should be soon, and it spins around the pivot
    m_pCameraGoal = m_pCameraPivot->createChildSceneNode ( Vector3 ( 0, 0, 15 ) );
    // this is where the camera actually is
    m_pCameraNode = pCamera->getSceneManager ( )->getRootSceneNode ( )->createChildSceneNode ( );
    m_pCameraNode->setPosition ( m_pCameraPivot->getPosition ( ) + m_pCameraGoal->getPosition ( ) );
    
    m_pCameraPivot->setFixedYawAxis ( true );
    m_pCameraGoal->setFixedYawAxis  ( true );
    m_pCameraNode->setFixedYawAxis  ( true );
    
    // our model is quite small, so reduce the clipping planes
    pCamera->setNearClipDistance ( 0.1f );
    pCamera->setFarClipDistance  ( 100 );
    m_pCameraNode->attachObject  ( pCamera );
    
    m_fPivotPitch = 0;
}

KDvoid SinbadCharacterController::updateBody ( Real fDeltaTime )
{
    m_tGoalDirection = Vector3::ZERO;   // we will calculate this
    
    if ( m_tKeyDirection != Vector3::ZERO && m_eBaseAnimID != ANIM_DANCE )
    {
        // calculate actually goal direction in world based on player's key directions
        m_tGoalDirection += m_tKeyDirection.z * m_pCameraNode->getOrientation ( ).zAxis ( );
        m_tGoalDirection += m_tKeyDirection.x * m_pCameraNode->getOrientation ( ).xAxis ( );
        m_tGoalDirection.y = 0;
        m_tGoalDirection.normalise ( );
        
        Quaternion  tToGoal = m_pBodyNode->getOrientation ( ).zAxis ( ).getRotationTo ( m_tGoalDirection );
        
        // calculate how much the character has to turn to face goal direction
        Real  fYawToGoal = tToGoal.getYaw ( ).valueDegrees ( );
        // this is how much the character CAN turn this frame
        Real  fYawAtSpeed = fYawToGoal / Math::Abs ( fYawToGoal ) * fDeltaTime * TURN_SPEED;
        // reduce "turnability" if we're in midair
        if ( m_eBaseAnimID == ANIM_JUMP_LOOP )
        {
            fYawAtSpeed *= 0.2f;
        }
        
        // turn as much as we can, but not more than we need to
        if ( fYawToGoal < 0 )
        {
            fYawToGoal = std::min<Real> ( 0, std::max<Real> ( fYawToGoal, fYawAtSpeed ) ); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
        }
        else if ( fYawToGoal > 0 )
        {
            fYawToGoal = std::max<Real> ( 0, std::min<Real> ( fYawToGoal, fYawAtSpeed ) ); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
        }
        
        m_pBodyNode->yaw ( Degree ( fYawToGoal ) );
        
        // move in current body direction (not the goal direction)
        m_pBodyNode->translate ( 0, 0, fDeltaTime * RUN_SPEED * m_eAnims [ m_eBaseAnimID ]->getWeight ( ), Node::TS_LOCAL );
    }
    
    if ( m_eBaseAnimID == ANIM_JUMP_LOOP )
    {
        // if we're jumping, add a vertical offset too, and apply gravity
        m_pBodyNode->translate ( 0, m_fVerticalVelocity * fDeltaTime, 0, Node::TS_LOCAL );
        m_fVerticalVelocity -= GRAVITY * fDeltaTime;
        
        Vector3  tPos = m_pBodyNode->getPosition ( );
        if ( tPos.y <= CHAR_HEIGHT )
        {
            // if we've hit the ground, change to landing state
            tPos.y = CHAR_HEIGHT;
            m_pBodyNode->setPosition ( tPos );
            setBaseAnimation ( ANIM_JUMP_END, true );
            m_fTimer = 0;
        }
    }
}

KDvoid SinbadCharacterController::updateAnimations ( Real fDeltaTime )
{
    Real  fBaseAnimSpeed = 1;
    Real  fTopAnimSpeed  = 1;
    
    m_fTimer += fDeltaTime;
    
    if ( m_eTopAnimID == ANIM_DRAW_SWORDS )
    {
        // flip the draw swords animation if we need to put it back
        fTopAnimSpeed = m_bSwordsDrawn ? -1.f : 1.f;
        
        // half-way through the animation is when the hand grasps the handles...
        if ( m_fTimer >= m_eAnims [ m_eTopAnimID ]->getLength ( ) / 2 &&
             m_fTimer - fDeltaTime < m_eAnims [ m_eTopAnimID ]->getLength ( ) / 2 )
        {
            // so transfer the swords from the sheaths to the hands
            m_pBodyEnt->detachAllObjectsFromBone ( );
            m_pBodyEnt->attachObjectToBone ( m_bSwordsDrawn ? "Sheath.L" : "Handle.L", m_pSword1 );
            m_pBodyEnt->attachObjectToBone ( m_bSwordsDrawn ? "Sheath.R" : "Handle.R", m_pSword2 );
            
            // change the hand state to grab or let go
            m_eAnims [ ANIM_HANDS_CLOSED  ]->setEnabled ( !m_bSwordsDrawn );
            m_eAnims [ ANIM_HANDS_RELAXED ]->setEnabled (  m_bSwordsDrawn );
            
            // toggle sword trails
            if ( m_bSwordsDrawn )
            {
                m_pSwordTrail->setVisible ( false );
                m_pSwordTrail->removeNode ( m_pSword1->getParentNode ( ) );
                m_pSwordTrail->removeNode ( m_pSword2->getParentNode ( ) );
            }
            else
            {
                m_pSwordTrail->setVisible ( true );
                m_pSwordTrail->addNode ( m_pSword1->getParentNode ( ) );
                m_pSwordTrail->addNode ( m_pSword2->getParentNode ( ) );
            }
        }
        
        if ( m_fTimer >= m_eAnims [ m_eTopAnimID ]->getLength ( ) )
        {
            // animation is finished, so return to what we were doing before
            if ( m_eBaseAnimID == ANIM_IDLE_BASE )
            {
                setTopAnimation ( ANIM_IDLE_TOP );
            }
            else
            {
                setTopAnimation ( ANIM_RUN_TOP );
                m_eAnims [ ANIM_RUN_TOP ]->setTimePosition ( m_eAnims [ ANIM_RUN_BASE ]->getTimePosition ( ) );
            }
            m_bSwordsDrawn = !m_bSwordsDrawn;
        }
    }
    else if ( m_eTopAnimID == ANIM_SLICE_VERTICAL || m_eTopAnimID == ANIM_SLICE_HORIZONTAL )
    {
        if ( m_fTimer >= m_eAnims [ m_eTopAnimID ]->getLength ( ) )
        {
            // animation is finished, so return to what we were doing before
            if ( m_eBaseAnimID == ANIM_IDLE_BASE )
            {
                setTopAnimation ( ANIM_IDLE_TOP );
            }
            else
            {
                setTopAnimation ( ANIM_RUN_TOP );
                m_eAnims [ ANIM_RUN_TOP ]->setTimePosition ( m_eAnims [ ANIM_RUN_BASE ]->getTimePosition ( ) );
            }
        }
        
        // don't sway hips from side to side when slicing. that's just embarrassing.
        if ( m_eBaseAnimID == ANIM_IDLE_BASE )
        {
            fBaseAnimSpeed = 0;
        }
    }
    else if ( m_eBaseAnimID == ANIM_JUMP_START )
    {
        if ( m_fTimer >= m_eAnims [ m_eBaseAnimID ]->getLength ( ) )
        {
            // takeoff animation finished, so time to leave the ground!
            setBaseAnimation ( ANIM_JUMP_LOOP, true );
            // apply a jump acceleration to the character
            m_fVerticalVelocity = JUMP_ACCEL;
        }
    }
    else if ( m_eBaseAnimID == ANIM_JUMP_END )
    {
        if ( m_fTimer >= m_eAnims [ m_eBaseAnimID ]->getLength ( ) )
        {
            // safely landed, so go back to running or idling
            if ( m_tKeyDirection == Vector3::ZERO )
            {
                setBaseAnimation ( ANIM_IDLE_BASE );
                setTopAnimation  ( ANIM_IDLE_TOP  );
            }
            else
            {
                setBaseAnimation ( ANIM_RUN_BASE, true );
                setTopAnimation  ( ANIM_RUN_TOP , true );
            }
        }
    }
    
    // increment the current base and top animation times
    if ( m_eBaseAnimID != ANIM_NONE )
    {
        m_eAnims [ m_eBaseAnimID ]->addTime ( fDeltaTime * fBaseAnimSpeed );
    }
    
    if ( m_eTopAnimID != ANIM_NONE )
    {
        m_eAnims [ m_eTopAnimID ]->addTime ( fDeltaTime * fTopAnimSpeed );
    }
    
    // apply smooth transitioning between our animations
    fadeAnimations ( fDeltaTime );
}

KDvoid SinbadCharacterController::fadeAnimations ( Real fDeltaTime )
{
    for ( KDint i = 0; i < NUM_ANIMS; i++ )
    {
        if ( m_bFadingIn[i] )
        {
            // slowly fade this animation in until it has full weight
            Real  fNewWeight = m_eAnims[i]->getWeight() + fDeltaTime * ANIM_FADE_SPEED;
            m_eAnims[i]->setWeight(Math::Clamp<Real> ( fNewWeight, 0, 1 ) );
            if ( fNewWeight >= 1 )
            {
                m_bFadingIn[i] = false;
            }
        }
        else if ( m_bFadingOut[i] )
        {
            // slowly fade this animation out until it has no weight, and then disable it
            Real  fNewWeight = m_eAnims[i]->getWeight() - fDeltaTime * ANIM_FADE_SPEED;
            m_eAnims[i]->setWeight(Math::Clamp<Real> ( fNewWeight, 0, 1 ) );
            if ( fNewWeight <= 0 )
            {
                m_eAnims[i]->setEnabled ( false );
                m_bFadingOut[i] = false;
            }
        }
    }
}

KDvoid SinbadCharacterController::updateCamera ( Real fDeltaTime )
{
    // place the camera pivot roughly at the character's shoulder
    m_pCameraPivot->setPosition ( m_pBodyNode->getPosition ( ) + Vector3::UNIT_Y * CAM_HEIGHT );
    // move the camera smoothly to the goal
    Vector3  tGoalOffset = m_pCameraGoal->_getDerivedPosition ( ) - m_pCameraNode->getPosition ( );
    m_pCameraNode->translate ( tGoalOffset * fDeltaTime * 9.0f );
    // always look at the pivot
    m_pCameraNode->lookAt ( m_pCameraPivot->_getDerivedPosition ( ), Node::TS_WORLD );
     
}

KDvoid SinbadCharacterController::updateCameraGoal ( Real fDeltaYaw, Real fDeltaPitch, Real fDeltaZoom )
{
    m_pCameraPivot->yaw ( Degree ( fDeltaYaw ), Node::TS_WORLD );
    
    // bound the pitch
    if ( ! ( m_fPivotPitch + fDeltaPitch >  25 && fDeltaPitch > 0 ) &&
         ! ( m_fPivotPitch + fDeltaPitch < -60 && fDeltaPitch < 0 ) )
    {
        m_pCameraPivot->pitch ( Degree ( fDeltaPitch ), Node::TS_LOCAL );
        m_fPivotPitch += fDeltaPitch;
    }
    
    Real  fDist = m_pCameraGoal->_getDerivedPosition ( ).distance ( m_pCameraPivot->_getDerivedPosition ( ) );
    Real  fDistChange = fDeltaZoom * fDist;
    
    // bound the zoom
    if ( !( fDist + fDistChange <  8 && fDistChange < 0 ) &&
         !( fDist + fDistChange > 25 && fDistChange > 0 ) )
    {
        m_pCameraGoal->translate ( 0, 0, fDistChange, Node::TS_LOCAL );
    }
}

KDvoid SinbadCharacterController::setBaseAnimation ( AnimID eID, bool bReset )
{
    if ( m_eBaseAnimID >= 0 && m_eBaseAnimID < NUM_ANIMS )
    {
        // if we have an old animation, fade it out
        m_bFadingIn  [ m_eBaseAnimID ] = false;
        m_bFadingOut [ m_eBaseAnimID ] = true;
    }
    
    m_eBaseAnimID = eID;
    
    if ( eID != ANIM_NONE )
    {
        // if we have a new animation, enable it and fade it in
        m_eAnims [ eID ]->setEnabled ( true );
        m_eAnims [ eID ]->setWeight  ( 0 );
        
        m_bFadingOut [ eID ] = false;
        m_bFadingIn  [ eID ] = true;
        
        if ( bReset )
        {
            m_eAnims [ eID ]->setTimePosition ( 0 );
        }
    }
}

KDvoid SinbadCharacterController::setTopAnimation ( AnimID eID, bool bReset )
{
    if ( m_eTopAnimID >= 0 && m_eTopAnimID < NUM_ANIMS )
    {
        // if we have an old animation, fade it out
        m_bFadingIn  [ m_eTopAnimID ] = false;
        m_bFadingOut [ m_eTopAnimID ] = true;
    }
    
    m_eTopAnimID = eID;
    
    if ( eID != ANIM_NONE )
    {
        // if we have a new animation, enable it and fade it in
        m_eAnims [ eID ]->setEnabled ( true );
        m_eAnims [ eID ]->setWeight ( 0 );
        
        m_bFadingOut [ eID ] = false;
        m_bFadingIn  [ eID ] = true;
        
        if ( bReset )
        {
            m_eAnims [ eID ]->setTimePosition ( 0 );
        }
    }
}