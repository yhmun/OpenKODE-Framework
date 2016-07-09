/* ------------------------------------------------------------------------------------
 *
 *      File            SdkCameraMan.h
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

#ifndef __SdkCameraMan_h__
#define __SdkCameraMan_h__

enum CameraStyle   // enumerator values for different styles of camera movement
{
    CS_FREELOOK ,
    CS_ORBIT    ,
    CS_MANUAL
};

/**
 *  Utility class for controlling the camera in samples.
 */ 
class SdkCameraMan
{
    public :
    
        SdkCameraMan ( Camera* pCamera );
    
        virtual ~SdkCameraMan ( KDvoid );
    
        /**
         *  Swaps the camera on our camera man for another camera.
         */
        virtual KDvoid   setCamera ( Camera* pCamera );
        virtual Camera*  getCamera ( KDvoid );
    
        /**
         *  Sets the target we will revolve around. Only applies for orbit style.
         */
        virtual KDvoid      setTarget ( SceneNode* pTarget );
        virtual SceneNode*  getTarget ( KDvoid );
    
        /**
         *  Sets the spatial offset from the target. Only applies for orbit style.
         */
        virtual KDvoid  setYawPitchDist ( Radian tYaw, Radian tPitch, Real tDist );
    
        /**
         *  Sets the camera's top speed. Only applies for free-look style.
         */
        virtual KDvoid  setTopSpeed ( Real fTopSpeed );    
        virtual Real    getTopSpeed ( KDvoid );
    
        /**
         *  Sets the camera's top speed. Only applies for free-look style.
         */
        virtual KDvoid       setStyle ( CameraStyle eStyle );    
        virtual CameraStyle  getStyle ( KDvoid );
    
        /**
         *  Manually stops the camera when in free-look mode.
         */
        virtual KDvoid  manualStop ( KDvoid );
    
        virtual bool  frameRenderingQueued ( const FrameEvent& tEvent );
    
        /**
         *  Processes mouse movement differently for each style.
         */
        virtual KDvoid  injectTouchMoved    ( const KDEventTouches& tTouches );
    
        /**
         *  Processes mouse presses. Only applies for orbit style.
         *  Left button is for orbiting, and right button is for zooming.
         */
        virtual KDvoid  injectTouchPressed  ( const KDEventTouches& tTouches );
    
        /**
         *  Processes mouse releases. Only applies for orbit style.
         *  Left button is for orbiting, and right button is for zooming.
         */
        virtual KDvoid  injectTouchReleased ( const KDEventTouches& tTouches );
    
    protected :
    
        Camera*         m_pCamera;
        CameraStyle     m_eStyle;
        SceneNode*      m_pTarget;
        bool            m_bOrbiting;
        bool            m_bZooming;
        Real            m_fTopSpeed;
        Vector3         m_tVelocity;
        bool            m_bGoingForward;
        bool            m_bGoingBack;
        bool            m_bGoingLeft;
        bool            m_bGoingRight;
        bool            m_bGoingUp;
        bool            m_bGoingDown;
        bool            m_bFastMove;
};

#endif // __SdkCameraMan_h__
