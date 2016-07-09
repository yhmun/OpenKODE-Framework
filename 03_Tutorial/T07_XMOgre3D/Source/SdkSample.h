/* ------------------------------------------------------------------------------------
 *
 *      File            SdkSample.h
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

#ifndef __SdkSample_h__
#define __SdkSample_h__

#include "Sample.h"
#include "SdkTrays.h"
#include "SdkCameraMan.h"

/**
 *  Base SDK sample class. Includes default player camera and SDK trays.
 */ 
class SdkSample : public Sample, public SdkTrayListener
{
    public :
    
        SdkSample ( KDvoid );
    
        virtual ~SdkSample ( KDvoid );
    
        virtual bool  frameRenderingQueued ( const FrameEvent& tEvent );
    
        virtual bool  touchMoved    ( const KDEventTouches& tTouches );
        virtual bool  touchPressed  ( const KDEventTouches& tTouches );
        virtual bool  touchReleased ( const KDEventTouches& tTouches );
    
        /**
         *  Extended to setup a default tray interface and camera controller.
         */
        virtual KDvoid  _setup ( RenderWindow* pWindow );
    
        virtual KDvoid  _shutdown ( KDvoid );
    
    protected :
    
        virtual KDvoid  setupView ( KDvoid );
    
        virtual KDvoid  setDragLook ( bool bEnabled );
    
    protected :
    
        Viewport*           m_pViewport;    		// main viewport
        Camera*             m_pCamera;        		// main camera
        SdkTrayManager*     m_pTrayMgr;     		// tray interface manager
        SdkCameraMan*       m_pCameraMan;     		// basic camera controller
        ParamsPanel*        m_pDetailsPanel;   		// sample details panel
        bool                m_bCursorWasVisible;    // was cursor visible before dialog appeared
        bool                m_bDragLook;            // click and drag to free-look
};

#endif // __SdkSample_h__
