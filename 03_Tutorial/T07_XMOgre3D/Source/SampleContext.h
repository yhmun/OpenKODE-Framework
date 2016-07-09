/* ------------------------------------------------------------------------------------
 *
 *      File            SampleContext.h
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

#ifndef __SampleContext_h__
#define __SampleContext_h__

#include "Sample.h"

/**
 *  Base class responsible for setting up a common context for samples.
 *  May be subclassed for specific sample types (not specific samples).
 *  Allows one sample to run at a time, while maintaining a sample queue.
 */
class SampleContext : public FrameListener, public WindowEventListener
{
    public :
    
                 SampleContext ( KDvoid );    
        virtual ~SampleContext ( KDvoid );
    
        /**
         *  This function initializes the render system and resources.
         */
        virtual KDvoid  initApp ( Sample* pInitialSample = 0 );
    
        /**
         *  This function closes down the application - saves the configuration then
         *  shutdowns.
         */
        virtual KDvoid  closeApp ( KDvoid );
    
        virtual KDvoid  render ( KDvoid );    
    
        virtual KDvoid  pauseCurrentSample ( KDvoid );
    
        virtual KDvoid  unpauseCurrentSample ( KDvoid );
    
        /**
         *  Quits the current sample and starts a new one.
         */
        virtual KDvoid  runSample ( Sample* pSample );
    
        /**
         *  Processes frame started events.
         */
        virtual bool    frameStarted ( const FrameEvent& tEvent );
    
        /**
         *  Processes rendering queued events.
         */
        virtual bool    frameRenderingQueued ( const FrameEvent& tEvent );
    
        /**
         *  Processes frame ended events.
         */
        virtual bool    frameEnded ( const FrameEvent& tEvent );
    
        virtual bool    touchMoved    ( const KDEventTouches& tTouches );
        virtual bool    touchPressed  ( const KDEventTouches& tTouches );
        virtual bool    touchReleased ( const KDEventTouches& tTouches );
    
    protected :
    
        /**
         *  Sets up the context after configuration.
         */
        virtual KDvoid          setup ( KDvoid );
    
        /**
         *  Creates the OGRE root.
         */
        virtual KDvoid          createRoot ( KDvoid );
    
        /**
         *  Configures the startup settings for OGRE. I use the config dialog here,
         *  but you can also restore from a config file. Note that this only happens
         *  when you start the context, and not when you reset it.
         */
        virtual bool            oneTimeConfig ( KDvoid );
    
        /**
         *  Creates the render window to be used for this context. I use an auto-created
         *  window here, but you can also create an external window if you wish.
         *  Just don't forget to initialise the root.
         */
        virtual RenderWindow*   createWindow ( KDvoid );
    
        /**
         *  Finds context-wide resource groups. I load paths from a config file here,
         *  but you can choose your resource locations however you want.
         */
        virtual KDvoid      locateResources ( KDvoid );
    
        /**
         *  Loads context-wide resource groups. I chose here to simply initialise all
         *  groups, but you can fully load specific ones if you wish.
         */
        virtual KDvoid      loadResources ( KDvoid );
    
        /**
         *  Reconfigures the context. Attempts to preserve the current sample state.
         */
        virtual KDvoid      reconfigure ( const String& sRenderer, NameValuePairList& aOptions );
    
        /**
         *  Recovers the last sample after a reset. You can override in the case that
         *  the last sample is destroyed in the process of resetting, and you have to
         *  recover it through another means.
         */
        virtual KDvoid      recoverLastSample ( KDvoid );
    
        /**
         *  Cleans up and shuts down the context.
         */
        virtual KDvoid      shutdown ( KDvoid );
    
    protected :
    
        Root*               m_pRoot;                    // OGRE root
    
        Sample*             m_pCurrentSample;           // currently running sample
        bool                m_bSamplePaused;            // whether current sample is paused
        bool                m_bFirstRun;                // whether or not this is the first run
        bool                m_bLastRun;                 // whether or not this is the final run
        String              m_sNextRenderer;            // name of renderer used for next run
        Sample*             m_pLastSample;              // last sample run before reconfiguration
        NameValuePairList   m_aLastSampleState;         // state of last sample    
    
    public :
    
        RenderWindow*       m_pWindow;                  // render window
};

#endif // __SampleContext_h__
