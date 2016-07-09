/* ------------------------------------------------------------------------------------
 *
 *      File            SdkCameraMan.cpp
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
#include "SdkCameraMan.h"

SdkCameraMan::SdkCameraMan ( Camera* pCamera )
{
    m_pCamera       = 0;
    m_pTarget       = 0;
    m_bOrbiting     = false;
    m_bZooming      = false;
    m_fTopSpeed     = 150;
    m_tVelocity     = Vector3::ZERO;
    m_bGoingForward = false;
    m_bGoingBack    = false;
    m_bGoingLeft    = false;
    m_bGoingRight   = false;
    m_bGoingUp      = false;
    m_bGoingDown    = false;
    m_bFastMove     = false;
        
    setCamera ( pCamera );
    setStyle  ( CS_FREELOOK );
}

SdkCameraMan::~SdkCameraMan ( KDvoid )
{
    
}

KDvoid SdkCameraMan::setCamera ( Camera* pCamera )
{
    m_pCamera = pCamera;
}

Camera* SdkCameraMan::getCamera ( KDvoid )
{
    return m_pCamera;
}

KDvoid SdkCameraMan::setTarget ( SceneNode* pTarget )
{
    if ( pTarget != m_pTarget )
    {
        m_pTarget = pTarget;
        if( pTarget )
        {
            setYawPitchDist ( Degree ( 0 ), Degree ( 15 ), 150 );
            m_pCamera->setAutoTracking ( true, m_pTarget );
        }
        else
        {
            m_pCamera->setAutoTracking ( false );
        }        
    }
}

SceneNode* SdkCameraMan::getTarget ( KDvoid )
{
    return m_pTarget;
}

KDvoid SdkCameraMan::setYawPitchDist ( Radian tYaw, Radian tPitch, Real tDist )
{
    m_pCamera->setPosition ( m_pTarget->_getDerivedPosition());
    m_pCamera->setOrientation ( m_pTarget->_getDerivedOrientation());
    m_pCamera->yaw ( tYaw );
    m_pCamera->pitch ( -tPitch );
    m_pCamera->moveRelative ( Vector3 ( 0, 0, tDist ) );
}

KDvoid SdkCameraMan::setTopSpeed ( Real fTopSpeed )
{
    m_fTopSpeed = fTopSpeed;
}

Real SdkCameraMan::getTopSpeed ( KDvoid )
{
    return m_fTopSpeed;
}

KDvoid SdkCameraMan::setStyle ( CameraStyle eStyle )
{
    if ( m_eStyle != CS_ORBIT && eStyle == CS_ORBIT )
    {
        setTarget ( m_pTarget ? m_pTarget : m_pCamera->getSceneManager ( )->getRootSceneNode ( ) );
        m_pCamera->setFixedYawAxis ( true );
        manualStop ( );
        setYawPitchDist ( Degree ( 0 ), Degree ( 15 ), 150 );        
    }
    else if ( m_eStyle != CS_FREELOOK && eStyle == CS_FREELOOK )
    {
        m_pCamera->setAutoTracking(false);
        m_pCamera->setFixedYawAxis(true);
    }
    else if ( m_eStyle != CS_MANUAL && eStyle == CS_MANUAL )
    {
        m_pCamera->setAutoTracking(false);
        manualStop ( );
    }
    
    m_eStyle = eStyle;
}

CameraStyle SdkCameraMan::getStyle ( KDvoid )
{
    return m_eStyle;
}

KDvoid SdkCameraMan::manualStop ( KDvoid )
{
    if ( m_eStyle == CS_FREELOOK )
    {
        m_bGoingForward = false;
        m_bGoingBack    = false;
        m_bGoingLeft    = false;
        m_bGoingRight   = false;
        m_bGoingUp      = false;
        m_bGoingDown    = false;
        m_tVelocity     = Vector3::ZERO;
    }
}

bool SdkCameraMan::frameRenderingQueued ( const FrameEvent& tEvent )
{
    if ( m_eStyle == CS_FREELOOK )
    {
        // build our acceleration vector based on keyboard input composite
        Vector3  tAccel = Vector3::ZERO;
        if ( m_bGoingForward ) tAccel += m_pCamera->getDirection ( );
        if ( m_bGoingBack    ) tAccel -= m_pCamera->getDirection ( );
        if ( m_bGoingRight   ) tAccel += m_pCamera->getRight ( );
        if ( m_bGoingLeft    ) tAccel -= m_pCamera->getRight ( );
        if ( m_bGoingUp      ) tAccel += m_pCamera->getUp ( );
        if ( m_bGoingDown    ) tAccel -= m_pCamera->getUp ( );
        
        // if accelerating, try to reach top speed in a certain time
        Real  fTopSpeed = m_bFastMove ? m_fTopSpeed * 20 : m_fTopSpeed;
        if ( tAccel.squaredLength ( ) != 0 )
        {
            tAccel.normalise ( );
            m_tVelocity += tAccel * fTopSpeed * tEvent.timeSinceLastFrame * 10;
        }
        // if not accelerating, try to stop in a certain time
        else
        {
            m_tVelocity -= m_tVelocity * tEvent.timeSinceLastFrame * 10;
        }
        
        Real  fTooSmall = std::numeric_limits<Real>::epsilon ( );
        
        // keep camera velocity below top speed and above epsilon
        if ( m_tVelocity.squaredLength ( ) > fTopSpeed * fTopSpeed )
        {
            m_tVelocity.normalise ( );
            m_tVelocity *= fTooSmall;
        }
        else if ( m_tVelocity.squaredLength ( ) < fTooSmall * fTooSmall )
        {
            m_tVelocity = Vector3::ZERO;
        }
        
        if ( m_tVelocity != Vector3::ZERO )
        {
            m_pCamera->move ( m_tVelocity * tEvent.timeSinceLastFrame );
        }
    }
    
    return true;
}

KDvoid SdkCameraMan::injectTouchMoved ( const KDEventTouches& tTouches )
{
    if ( m_eStyle == CS_ORBIT )
    {
        Real  fDist = ( m_pCamera->getPosition ( ) - m_pTarget->_getDerivedPosition ( ) ).length ( );
        
        if ( m_bOrbiting )   // yaw around the target, and pitch locally
        {
            m_pCamera->setPosition ( m_pTarget->_getDerivedPosition ( ) );
            
            m_pCamera->yaw   ( Degree ( -tTouches.touches [ 0 ].diff_x * 0.25f ) );
            m_pCamera->pitch ( Degree (  tTouches.touches [ 0 ].diff_y * 0.25f ) );
            
            m_pCamera->moveRelative ( Vector3 ( 0, 0, fDist ) );
            
            // don't let the camera go over the top or around the bottom of the target
        }
        else if ( m_bZooming )  // move the camera toward or away from the target
        {
            // the further the camera is, the faster it moves
            m_pCamera->moveRelative ( Vector3 ( 0, 0, -tTouches.touches [ 0 ].diff_y * 0.004f * fDist ) );
        }
//        else if (evt.state.Z.rel != 0)  // move the camera toward or away from the target
        {
            // the further the camera is, the faster it moves
//            mCamera->moveRelative(Ogre::Vector3(0, 0, -evt.state.Z.rel * 0.0008f * dist));
        }
    }
    else if ( m_eStyle == CS_FREELOOK )
    {
        m_pCamera->yaw   ( Degree ( -tTouches.touches [ 0 ].diff_x * 0.15f ) );
        m_pCamera->pitch ( Degree (  tTouches.touches [ 0 ].diff_y * 0.15f ) );
    }
}

KDvoid SdkCameraMan::injectTouchPressed  ( const KDEventTouches& tTouches )
{
    if ( m_eStyle == CS_ORBIT)
    {
        m_bOrbiting = true;
    }
}

KDvoid SdkCameraMan::injectTouchReleased ( const KDEventTouches& tTouches )
{
    if ( m_eStyle == CS_ORBIT )
    {
        m_bOrbiting = false;
    }
}
