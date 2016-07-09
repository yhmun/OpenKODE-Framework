/* ------------------------------------------------------------------------------------
 *
 *      File            Sample.h
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

#ifndef __Sample_h__
#define __Sample_h__

/**
 *  Base class responsible for everything specific to one sample.
 *  Designed to be subclassed for each sample.
 */ 
class Sample
{
    public :
    
        /**
         *  Utility comparison structure for sorting samples using SampleSet.
         */
        struct Comparer
        {
            bool  operator ( ) ( Sample* a, Sample* b )
            {
                NameValuePairList::iterator  aTitle = a->getInfo ( ).find ( "Title" );
                NameValuePairList::iterator  bTitle = b->getInfo ( ).find ( "Title" );
            
                if ( aTitle != a->getInfo ( ).end ( ) && bTitle != b->getInfo ( ).end ( ) )
                {
                    return aTitle->second.compare ( bTitle->second ) < 0;
                }
                else
                {
                    return false;
                }
            }
        };
    
    public :    
                 Sample ( KDvoid );
        virtual ~Sample ( KDvoid );
    
        /**
         *  Retrieves custom sample info.
         */
        NameValuePairList&      getInfo ( KDvoid ); 

        /**
         *  Tests to see if target machine meets any special requirements of
         *  this sample. Signal a failure by throwing an exception.
         */
        virtual KDvoid          testCapabilities ( const RenderSystemCapabilities* pCaps );
        
        /**
         *  If this sample requires a specific render system to run, this method
         *  will be used to return its name.
         */
        virtual String          getRequiredRenderSystem ( KDvoid );
    
        /**
         *  If this sample requires specific plugins to run, this method will be
         *  used to return their names.
         */

        virtual StringVector    getRequiredPlugins ( KDvoid );

        SceneManager*           getSceneManager ( KDvoid );
    
        KDbool                  isDone ( KDvoid );
    
        /**
         *  Sets up a sample. Used by the SampleContext class. Do not call directly.
         */
        virtual KDvoid      _setup ( RenderWindow* pWindow );
    
        /**
         *  Shuts down a sample. Used by the SampleContext class. Do not call directly.
         */
        virtual KDvoid      _shutdown ( KDvoid );
    
        /**
         *  Actions to perform when the context stops sending frame listener events
         *  and input device events to this sample.
         */
        virtual KDvoid      paused ( KDvoid );
    
        /**
         *  Actions to perform when the context continues sending frame listener
         *  events and input device events to this sample.
         */
        virtual KDvoid      unpaused ( KDvoid );
    
        /**
         *  Saves the sample state. Optional. Used during reconfiguration.
         */
        virtual KDvoid      saveState ( NameValuePairList& aState );
    
        /**
         *  Restores the sample state. Optional. Used during reconfiguration.
         */
        virtual KDvoid      restoreState ( NameValuePairList& aState );
    
        // callback interface copied from various listeners to be used by SampleContext

        virtual bool        frameStarted         ( const FrameEvent& tEvent );
        virtual bool        frameRenderingQueued ( const FrameEvent& tEvent );
        virtual bool        frameEnded           ( const FrameEvent& tEvent );
    
        virtual bool        touchMoved    ( const KDEventTouches& tTouches );
        virtual bool        touchPressed  ( const KDEventTouches& tTouches );
        virtual bool        touchReleased ( const KDEventTouches& tTouches );
    
        KDvoid              setShaderGenerator ( RTShader::ShaderGenerator* pShaderGenerator );
        
    protected :
    
        /**
         *  Finds sample-specific resources. No such effort is made for most samples,
         *  but this is useful for special samples with large, exclusive resources.
         */
        virtual KDvoid      locateResources ( KDvoid );
    
        /**
         *  Loads sample-specific resources. No such effort is made for most samples,
         *  but this is useful for special samples with large, exclusive resources.
         */
        virtual KDvoid      loadResources ( KDvoid );
    
        /**
         *  Creates a scene manager for the sample. A generic one is the default,
         *  but many samples require a special kind of scene manager.
         */
        virtual KDvoid      createSceneManager ( KDvoid );
    
        /**
         *  Sets up viewport layout and camera.
         */
        virtual KDvoid      setupView ( KDvoid );
    
        /**
         *  Sets up the scene (and anything else you want for the sample).
         */
        virtual KDvoid      setupContent ( KDvoid );
    
        /**
         *  Cleans up the scene (and anything else you used).
         */
        virtual KDvoid      cleanupContent ( KDvoid );
    
        /**
         *  Unloads sample-specific resources. My method here is simple and good
         *  enough for most small samples, but your needs may vary.
         */
        virtual KDvoid      unloadResources ( KDvoid );
    
    protected :
    
        Root*                           m_pRoot;                    // OGRE root object
        RenderWindow*                   m_pWindow;                  // context render window

        SceneManager*                   m_pSceneMgr;                // scene manager for this sample
        NameValuePairList               m_aInfo;                    // custom sample info
        KDbool                          m_bDone;                    // flag to mark the end of the sample
        KDbool                          m_bResourcesLoaded;         // whether or not resources have been loaded
        KDbool                          m_bContentSetup;            // whether or not scene was created
    
        RTShader::ShaderGenerator*      m_pShaderGenerator;         // The Shader generator instance.

};

typedef std::set<Sample*, Sample::Comparer>  SampleSet;

#endif // __Sample_h__
