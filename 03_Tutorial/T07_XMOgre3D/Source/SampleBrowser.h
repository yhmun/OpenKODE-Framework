/* ------------------------------------------------------------------------------------
 *
 *      File            SampleBrowser.h
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

#ifndef __SampleBrowser_h__
#define __SampleBrowser_h__

#include "SampleContext.h"
#include "SamplePlugin.h"
#include "SdkTrays.h"

#include "Sample/BezierPatch.h"
#include "Sample/BSP.h"
#include "Sample/CameraTrack.h"
#include "Sample/CelShading.h"
#include "Sample/CharacterSample.h"
#include "Sample/Compositor.h"
#include "Sample/CubeMapping.h"
#include "Sample/DeferredShadingDemo.h"
#include "Sample/Dot3Bump.h"
#include "Sample/DualQuaternion.h"
#include "Sample/DynTex.h"
#include "Sample/FacialAnimation.h"
#include "Sample/Fresnel.h"
#include "Sample/Grass.h"
#include "Sample/Instancing.h"
#include "Sample/Lighting.h"
#include "Sample/NewInstancing.h"
#include "Sample/OceanDemo.h"
#include "Sample/ParticleGS.h"
#include "Sample/ParticleFX.h"
#include "Sample/ShaderSystem.h"
#include "Sample/Shadows.h"
#include "Sample/SkeletalAnimation.h"
#include "Sample/SkyBox.h"
#include "Sample/SkyDome.h"
#include "Sample/SkyPlane.h"
#include "Sample/Smoke.h"
#include "Sample/SphereMapping.h"
#include "Sample/SSAO.h"
#include "Sample/Terrain.h"
#include "Sample/TextureArray.h"
#include "Sample/TextureFX.h"
#include "Sample/Transparency.h"
#include "Sample/VolumeTex.h"
#include "Sample/Water.h"

typedef std::map<std::string, SdkSample*>  PluginMap;

/** 
 *  This class demonstrates basic usage of the RTShader system.
 *  It sub class the material manager listener class and when a target scheme callback
 *  is invoked with the shader generator scheme it tries to create an equivalent shader
 *  based technique based on the default technique of the given material.
 */
class ShaderGeneratorTechniqueResolverListener : public MaterialManager::Listener
{
    public :
    
        ShaderGeneratorTechniqueResolverListener ( RTShader::ShaderGenerator* pShaderGenerator );
    
        /** 
         *  This is the hook point where shader based technique will be created.
         *  It will be called whenever the material manager won't find appropriate technique
         *  that satisfy the target scheme name. If the scheme name is out target RT Shader System
         *  scheme name we will try to create shader generated technique for it.
         */
        virtual Technique*      handleSchemeNotFound ( KDushort uSchemeIndex, const String& tSchemeName, Material* pOriginalMaterial, KDushort uLodIndex, const Renderable* pRend );
    
    protected :
    
        RTShader::ShaderGenerator*      m_pShaderGenerator;			// The shader generator instance.
};

/**
 *  The OGRE Sample Browser. Features a menu accessible from all samples,
 *  dynamic configuration, resource reloading, node labelling, and more.
 */

class SampleBrowser : public SampleContext, public SdkTrayListener
{
    public :
    
        SampleBrowser ( KDvoid );
    
        /**
         *  Extends runSample to handle creation and destruction of dummy scene.
         */
        virtual KDvoid      runSample ( Sample* pSample );
   
        /**
         *  Extends frameRenderingQueued to update tray manager and carousel.
         */
        virtual bool        frameRenderingQueued ( const FrameEvent& tEvent );
    
        /**
         *  Handles confirmation dialog responses.
         */
        virtual KDvoid      yesNoDialogClosed ( const DisplayString& tQuestion, KDbool bYesHit );
    
        /**
         *  Handles button widget events.
         */
        virtual KDvoid      buttonHit ( Button* pButton );
    
        /**
         *  Handles menu item selection changes.
         */
        virtual KDvoid      itemSelected ( SelectMenu* pMenu );
    
        /**
         *  Handles sample slider changes.
         */
        virtual KDvoid      sliderMoved ( Slider* pSlider );
    
        virtual bool        touchMoved    ( const KDEventTouches& tTouches );
        virtual bool        touchPressed  ( const KDEventTouches& tTouches );
        virtual bool        touchReleased ( const KDEventTouches& tTouches );
    
        /**
         *  Extends setup to create dummy scene and tray interface.
         */
        virtual KDvoid      setup ( KDvoid );
    
    protected :
    
        /**
         *  Initialises only the browser's resources and those most commonly used
         *  by samples. This way, additional special content can be initialised by
         *  the samples that use them, so startup time is unaffected.
         */
        virtual KDvoid      loadResources ( KDvoid );
    
        /**
         *  Creates dummy scene to allow rendering GUI in viewport.
         */
        virtual KDvoid      createDummyScene ( KDvoid );
    
        /**
         *  Loads sample plugins from a configuration file.
         */
        virtual Sample*     loadSamples ( KDvoid );
    
        /**
         *  Unloads any loaded sample plugins.
         */
        virtual KDvoid      unloadSamples ( KDvoid );
    
        /**
         *  Sets up main page for browsing samples.
         */
        virtual KDvoid      setupWidgets ( KDvoid );
    
        /**
         *  Populates home menus with loaded samples.
         */
        virtual KDvoid      populateSampleMenus ( KDvoid );
    
        /**
         *  Overrides to recover by last sample's index instead.
         */
        virtual KDvoid      recoverLastSample ( KDvoid );
    
        /**
         *  Extends reconfigure to save the view and the index of last sample run.
         */
        virtual KDvoid      reconfigure ( const String& sRenderer, NameValuePairList& aOptions );

        /**
         *  Extends shutdown to destroy dummy scene and tray interface.
         */
        virtual KDvoid      shutdown ( KDvoid );
    
    protected :
    
        /**
         *  Destroys dummy scene.
         */
        virtual KDvoid      destroyDummyScene ( KDvoid );
    
        /**
         *  Extend to temporarily hide a sample's overlays while in the pause menu.
         */
        virtual KDvoid      pauseCurrentSample ( KDvoid );
    
        /**
         *  Extend to unnhide all of sample's temporarily hidden overlays.
         */
        virtual KDvoid      unpauseCurrentSample ( KDvoid );
    
        /**
         *  Initialize the RT Shader system.
         */
        virtual KDbool      initializeRTShaderSystem ( SceneManager* pSceneMgr );
    
        /**
         *  Finalize the RT Shader system.
         */
        virtual KDvoid      finalizeRTShaderSystem ( KDvoid );    
    
    protected :
    
        SdkTrayManager*                             m_pTrayMgr;                     // SDK tray interface
        PluginMap                                   m_aPluginNameMap;               // A structure to map plugin names to class types
        StringVector                                m_aLoadedSamplePlugins;         // loaded sample plugins
        std::set<String>                            m_aSampleCategories;            // sample categories
        SampleSet                                   m_aLoadedSamples;               // loaded samples
    
        SelectMenu*                                 m_pCategoryMenu;                // sample category select menu
        SelectMenu*                                 m_pSampleMenu;                  // sample select menu
        Slider*                                     m_pSampleSlider;                // sample slider bar
        Label*                                      m_pTitleLabel;                  // sample title label
        TextBox*                                    m_pDescBox;                     // sample description box
        SelectMenu*                                 m_pRendererMenu;                // render system selection menu
        std::vector<Overlay*>                       m_aHiddenOverlays;              // sample overlays hidden for pausing
        std::vector<OverlayContainer*>              m_aThumbs;                      // sample thumbnails
        Real                                        m_fCarouselPlace;               // current state of carousel
        KDint                                       m_nLastViewTitle;               // last sample title viewed
        KDint                                       m_nLastViewCategory;            // last sample category viewed
        KDint                                       m_nLastSampleIndex;             // index of last sample running
    
        RTShader::ShaderGenerator*                  m_pShaderGenerator;             // The Shader generator instance.
        ShaderGeneratorTechniqueResolverListener*	m_pMaterialMgrListener;         // Shader generator material manager listener.
};

#endif // __SampleBrowser_h__
