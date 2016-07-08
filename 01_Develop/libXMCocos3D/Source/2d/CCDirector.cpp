/* -----------------------------------------------------------------------------------
 *
 *      File            CCDirector.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

// cocos2d includes
#include "2d/CCDirector.h"

// standard includes
#include "ccFPSImages.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "base/CCNS.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
#include "base/CCArray.h"
#include "2d/CCScheduler.h"
#include "support/CCNotificationCenter.h"
#include "2d/layers_scenes_transitions_nodes/CCTransition.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"
#include "base/CCAutoreleasePool.h"
#include "platform/CCFileUtils.h"
#include "platform/CCApplication.h"
#include "2d/label_nodes/CCLabelBMFont.h"
#include "2d/label_nodes/CCLabelAtlas.h"
#include "2d/actions/CCActionManager.h"
#include "2d/sprite_nodes/CCAnimationCache.h"
#include "2d/event_dispatcher/CCTouch.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "support/user_default/CCUserDefault.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "XMKazmath/kazmath.h"
#include "XMKazmath/GL/matrix.h"
#include "support/CCProfiling.h"
#include "platform/CCImage.h"
#include "platform/CCEGLView.h"
#include "2d/CCConfiguration.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/label_nodes/CCFontFreeType.h"

/**
 Position of the FPS
 
 Default: 0,0 (bottom-left corner)
 */
#ifndef CC_DIRECTOR_STATS_POSITION
#define CC_DIRECTOR_STATS_POSITION Director::getInstance()->getVisibleOrigin()
#endif

using namespace std;

unsigned int g_uNumberOfDraws = 0;

NS_CC_BEGIN
// XXX it should be a Director ivar. Move it there once support for multiple directors is added

// singleton stuff
static DisplayLinkDirector *s_SharedDirector = nullptr;

#define kDefaultFPS        60  // 60 frames per second
extern const char* cocos2dVersion(void);

Director* Director::getInstance()
{
    if (!s_SharedDirector)
    {
        s_SharedDirector = new DisplayLinkDirector();
        s_SharedDirector->init();
    }

    return s_SharedDirector;
}

Director::Director(void)
{

}

KDbool Director::init ( KDvoid )
{
    setDefaultValues ( );

    // scenes
    m_pRunningScene = nullptr;
    m_pNextScene = nullptr;

    m_pNotificationNode = nullptr;

    m_pScenesStack = new Array();
    m_pScenesStack->initWithCapacity(15);

    // projection delegate if "Custom" projection is used
    m_pProjectionDelegate = nullptr;

    // FPS
    m_fAccumDt = 0.0f;
    m_fFrameRate = 0.0f;
    m_pFPSLabel = nullptr;
    m_pSPFLabel = nullptr;
    m_pDrawsLabel = nullptr;
    m_uTotalFrames = m_uFrames = 0;
    m_szFPS = new char[10];
	m_dLastUpdate = 0;

    // paused ?
    m_bPaused = false;
   
    // purge ?
    m_bPurgeDirecotorInNextLoop = false;

    m_tWinSizeInPoints = Size::ZERO;    

    m_pOpenGLView = nullptr;

    m_fContentScaleFactor = 1.0f;

    // scheduler
    m_pScheduler = new Scheduler();
    // action manager
    m_pActionManager = new ActionManager();
    m_pScheduler->scheduleUpdateForTarget(m_pActionManager, Scheduler::PRIORITY_SYSTEM, false);

    m_pEventDispatcher = new EventDispatcher();
    //init TextureCache
    initTextureCache();
    
    // create autorelease pool
    PoolManager::sharedPoolManager()->push();

    return true;
}
    
Director::~Director(void)
{
    CCLOGINFO("deallocing Director: %p", this);

    CC_SAFE_RELEASE(m_pFPSLabel);
    CC_SAFE_RELEASE(m_pSPFLabel);
    CC_SAFE_RELEASE(m_pDrawsLabel);
    
    CC_SAFE_RELEASE(m_pRunningScene);
    CC_SAFE_RELEASE(m_pNotificationNode);
    CC_SAFE_RELEASE(m_pScenesStack);
    CC_SAFE_RELEASE(m_pScheduler);
    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pEventDispatcher);
    
    // pop the autorelease pool
    PoolManager::sharedPoolManager()->pop();
    PoolManager::purgePoolManager();

    // delete fps string
    delete []m_szFPS;

    s_SharedDirector = nullptr;
}

KDvoid Director::setDefaultValues ( KDvoid )
{
	Configuration*	pConf = Configuration::getInstance ( );

	// default FPS
	KDdouble		dFps = pConf->getNumber ( "cocos2d.x.fps", kDefaultFPS );
	m_dOldAnimationInterval = m_dAnimationInterval = 1.0 / dFps;

	// Display FPS
	m_bDisplayStats = pConf->getBool ( "cocos2d.x.display_fps", false );

	// GL projection
	const KDchar*	pProjection = pConf->getCString ( "cocos2d.x.gl.projection", "3d" );
	if ( kdStrcmp ( pProjection, "3d" ) == 0 )
	{
		m_eProjection = Projection::_3D;
	}
	else if ( kdStrcmp ( pProjection, "2d" ) == 0 )
	{
		m_eProjection = Projection::_2D;
	}
	else if ( kdStrcmp ( pProjection, "custom" ) == 0 )
	{
		m_eProjection = Projection::CUSTOM;
	}
	else
	{
		CCASSERT ( false, "Invalid projection value" );
	}

	// Default pixel format for PNG images with alpha
	const KDchar*	pPixelFormat = pConf->getCString ( "cocos2d.x.texture.pixel_format_for_png", "auto" );
	if ( kdStrcmp ( pPixelFormat, "rgba8888" ) == 0 )
	{
		Texture2D::setDefaultAlphaPixelFormat ( Texture2D::PixelFormat::RGBA8888 );
	}
	else if ( kdStrcmp(pPixelFormat, "rgba4444" ) == 0 )
	{
		Texture2D::setDefaultAlphaPixelFormat ( Texture2D::PixelFormat::RGBA4444 );
	}
	else if ( kdStrcmp ( pPixelFormat, "rgba5551" ) == 0 )
	{
		Texture2D::setDefaultAlphaPixelFormat ( Texture2D::PixelFormat::RGB5A1 );
	}
	else
	{
		Texture2D::setDefaultAlphaPixelFormat ( Texture2D::PixelFormat::AUTO );
	}

	// PVR v2 has alpha premultiplied ?
	KDbool		bPvrAlphaPremultipled = pConf->getBool ( "cocos2d.x.texture.pvrv2_has_alpha_premultiplied", false );
	Texture2D::PVRImagesHavePremultipliedAlpha ( bPvrAlphaPremultipled );
}

KDvoid Director::setGLDefaultValues ( KDvoid )
{
    // This method SHOULD be called only after openGLView_ was initialized
    CCASSERT ( m_pOpenGLView, "opengl view should not be null" );

    setAlphaBlending ( true );
    // XXX: Fix me, should enable/disable depth test according the depth format as cocos2d-iphone did
    // [self setDepthTest: view_.depthFormat];
    setDepthTest ( false );
    setProjection ( m_eProjection );

    // set other opengl default values
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
}

// Draw the Scene
KDvoid Director::drawScene ( KDvoid )
{
    // calculate "global" dt
    calculateDeltaTime();

    if (m_pOpenGLView)
    {
//        m_pOpenGLView->pollInputEvents();
    }

    //tick before glClear: issue #533
    if (! m_bPaused)
    {
        m_pScheduler->update(m_fDeltaTime);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* to avoid flickr, nextScene MUST be here: after tick and before draw.
     XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
    if (m_pNextScene)
    {
        setNextScene();
    }

    kmGLPushMatrix();

    // draw the scene
    if (m_pRunningScene)
    {
        m_pRunningScene->visit();
    }

    // draw the notifications node
    if (m_pNotificationNode)
    {
        m_pNotificationNode->visit();
    }
    
    if (m_bDisplayStats)
    {
        showStats();
    }

    kmGLPopMatrix();

    m_uTotalFrames++;

    // swap buffers
    if (m_pOpenGLView)
    {
  //      m_pOpenGLView->swapBuffers();
    }
    
    if (m_bDisplayStats)
    {
        calculateMPF();
    }
}

KDvoid Director::calculateDeltaTime ( KDvoid )
{
	KDdouble now = kdGetMilliseconds();

    // new delta time. Re-fixed issue #1277
    if (m_bNextDeltaTimeZero)
    {
        m_fDeltaTime = 0;
        m_bNextDeltaTimeZero = false;
    }
    else
    {
		m_fDeltaTime = (now - m_dLastUpdate) / 1000.0; 
        m_fDeltaTime = KD_MAX(0, m_fDeltaTime);
    }

	m_dLastUpdate = now;
}

KDfloat Director::getDeltaTime ( KDvoid ) const
{
	return m_fDeltaTime;
}

KDvoid Director::setOpenGLView ( EGLView* pOpenGLView )
{
    CCASSERT ( pOpenGLView, "opengl view should not be null");

    if ( m_pOpenGLView != pOpenGLView )
    {
		// Configuration. Gather GPU info
		Configuration*	pConf = Configuration::getInstance ( );
		pConf->gatherGPUInfo ( );
		pConf->dumpInfo ( );

        // EAGLView is not a Object
        delete m_pOpenGLView; // [openGLView_ release]
        m_pOpenGLView = pOpenGLView;

        // set size
        m_tWinSizeInPoints = m_pOpenGLView->getDesignResolutionSize ( );
        
        createStatsLabel ( );
        
        if ( m_pOpenGLView )
        {
            setGLDefaultValues ( );
        }  
        
        CHECK_GL_ERROR_DEBUG ( );

//		m_pTouchDispatcher->setDispatchEvents ( true );
    }
}

TextureCache* Director::getTextureCache ( KDvoid ) const
{
    return m_pTextureCache;
}

KDvoid Director::initTextureCache ( KDvoid )
{
#ifdef EMSCRIPTEN
    m_pTextureCache = new TextureCacheEmscripten();
#else
    m_pTextureCache = new TextureCache();
#endif // EMSCRIPTEN
}

KDvoid Director::destroyTextureCache ( KDvoid )
{
    if ( m_pTextureCache )
    {
        m_pTextureCache->waitForQuit ( );
        CC_SAFE_RELEASE ( m_pTextureCache );
    }
}

KDvoid Director::setViewport ( KDvoid )
{
    if ( m_pOpenGLView )
    {
        m_pOpenGLView->setViewPortInPoints ( 0, 0, m_tWinSizeInPoints.width, m_tWinSizeInPoints.height );
    }
}

KDvoid Director::setNextDeltaTimeZero ( KDbool bNextDeltaTimeZero )
{
    m_bNextDeltaTimeZero = bNextDeltaTimeZero;
}

KDvoid Director::setProjection ( Projection eProjection )
{
    Size size = m_tWinSizeInPoints;

    setViewport();

    switch (eProjection)
    {
        case Projection::_2D:
            kmGLMatrixMode(KM_GL_PROJECTION);
            kmGLLoadIdentity();
            kmMat4 orthoMatrix;
            kmMat4OrthographicProjection(&orthoMatrix, 0, size.width, 0, size.height, -1024, 1024);
            kmGLMultMatrix(&orthoMatrix);
            kmGLMatrixMode(KM_GL_MODELVIEW);
            kmGLLoadIdentity();
            break;

        case Projection::_3D:
        {
            float zeye = this->getZEye();

            kmMat4 matrixPerspective, matrixLookup;

            kmGLMatrixMode(KM_GL_PROJECTION);
            kmGLLoadIdentity();

            // issue #1334
            kmMat4PerspectiveProjection(&matrixPerspective, 60, (GLfloat)size.width/size.height, 0.1f, zeye*2);
            // kmMat4PerspectiveProjection( &matrixPerspective, 60, (GLfloat)size.width/size.height, 0.1f, 1500);

            kmGLMultMatrix(&matrixPerspective);

            kmGLMatrixMode(KM_GL_MODELVIEW);
            kmGLLoadIdentity();
            kmVec3 eye, center, up;
            kmVec3Fill(&eye, size.width/2, size.height/2, zeye);
            kmVec3Fill(&center, size.width/2, size.height/2, 0.0f);
            kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
            kmMat4LookAt(&matrixLookup, &eye, &center, &up);
            kmGLMultMatrix(&matrixLookup);
            break;
        }
            
        case Projection::CUSTOM:
            if (m_pProjectionDelegate)
                m_pProjectionDelegate->updateProjection();
            
            break;
            
        default:
            CCLOG("cocos2d: Director: unrecognized projection");
            break;
    }

    m_eProjection = eProjection;
    GL::setProjectionMatrixDirty();
}

KDvoid Director::purgeCachedData ( KDvoid )
{
    LabelBMFont::purgeCachedData();
    if (s_SharedDirector->getOpenGLView())
    {
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        m_pTextureCache->removeUnusedTextures();
    }
    FileUtils::getInstance()->purgeCachedEntries();
}

KDfloat Director::getZEye ( KDvoid ) const
{
    return (m_tWinSizeInPoints.height / 1.1566f);
}

void Director::setAlphaBlending(bool on)
{
    if (on)
    {
        GL::blendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    }
    else
    {
        GL::blendFunc(GL_ONE, GL_ZERO);
    }

    CHECK_GL_ERROR_DEBUG();
}

void Director::setDepthTest(bool on)
{
    if (on)
    {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
//        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    CHECK_GL_ERROR_DEBUG();
}

static void GLToClipTransform(kmMat4 *transformOut)
{
	kmMat4 projection;
	kmGLGetMatrix(KM_GL_PROJECTION, &projection);
	
	kmMat4 modelview;
	kmGLGetMatrix(KM_GL_MODELVIEW, &modelview);
	
	kmMat4Multiply(transformOut, &projection, &modelview);
}

Point Director::convertToGL(const Point& uiPoint)
{
    kmMat4 transform;
	GLToClipTransform(&transform);
	
	kmMat4 transformInv;
	kmMat4Inverse(&transformInv, &transform);
	
	// Calculate z=0 using -> transform*[0, 0, 0, 1]/w
	kmScalar zClip = transform.mat[14]/transform.mat[15];
	
    Size glSize = m_pOpenGLView->getDesignResolutionSize();
	kmVec3 clipCoord = {2.0f*uiPoint.x/glSize.width - 1.0f, 1.0f - 2.0f*uiPoint.y/glSize.height, zClip};
	
	kmVec3 glCoord;
	kmVec3TransformCoord(&glCoord, &clipCoord, &transformInv);
	
	return Point(glCoord.x, glCoord.y);
}

Point Director::convertToUI(const Point& glPoint)
{
    kmMat4 transform;
	GLToClipTransform(&transform);
    
	kmVec3 clipCoord;
	// Need to calculate the zero depth from the transform.
	kmVec3 glCoord = {glPoint.x, glPoint.y, 0.0};
	kmVec3TransformCoord(&clipCoord, &glCoord, &transform);
	
	Size glSize = m_pOpenGLView->getDesignResolutionSize();
	return Point(glSize.width*(clipCoord.x*0.5 + 0.5), glSize.height*(-clipCoord.y*0.5 + 0.5));
}

const Size& Director::getWinSize(void) const
{
    return m_tWinSizeInPoints;
}

Size Director::getWinSizeInPixels() const
{
    return Size(m_tWinSizeInPoints.width * m_fContentScaleFactor, m_tWinSizeInPoints.height * m_fContentScaleFactor);
}

Size Director::getVisibleSize() const
{
    if (m_pOpenGLView)
    {
        return m_pOpenGLView->getVisibleSize();
    }
    else 
    {
        return Size::ZERO;
    }
}

Point Director::getVisibleOrigin() const
{
    if (m_pOpenGLView)
    {
        return m_pOpenGLView->getVisibleOrigin();
    }
    else 
    {
        return Point::ZERO;
    }
}

// scene management

void Director::runWithScene(Scene *scene)
{
    CCASSERT(scene != nullptr, "This command can only be used to start the Director. There is already a scene present.");
    CCASSERT(m_pRunningScene == nullptr, "_runningScene should be null");

    pushScene(scene);
    startAnimation();
}

void Director::replaceScene(Scene *scene)
{
    CCASSERT(m_pRunningScene, "Use runWithScene: instead to start the director");
    CCASSERT(scene != nullptr, "the scene should not be null");

    unsigned int index = m_pScenesStack->count();

    m_bSendCleanupToScene = true;
    m_pScenesStack->replaceObjectAtIndex(index - 1, scene);

    m_pNextScene = scene;
}

void Director::pushScene(Scene *scene)
{
    CCASSERT(scene, "the scene should not null");

    m_bSendCleanupToScene = false;

    m_pScenesStack->addObject(scene);
    m_pNextScene = scene;
}

void Director::popScene(void)
{
    CCASSERT(m_pRunningScene != nullptr, "running scene should not null");

    m_pScenesStack->removeLastObject();
    unsigned int c = m_pScenesStack->count();

    if (c == 0)
    {
        end();
    }
    else
    {
        m_bSendCleanupToScene = true;
        m_pNextScene = (Scene*)m_pScenesStack->getObjectAtIndex(c - 1);
    }
}

void Director::popToRootScene(void)
{
    popToSceneStackLevel(1);
}

void Director::popToSceneStackLevel(int level)
{
    CCASSERT(m_pRunningScene != nullptr, "A running Scene is needed");
    int c = static_cast<int>(m_pScenesStack->count());

    // level 0? -> end
    if (level == 0)
    {
        end();
        return;
    }

    // current level or lower -> nothing
    if (level >= c)
        return;

	// pop stack until reaching desired level
	while (c > level)
    {
        Scene *current = (Scene*)m_pScenesStack->getLastObject();

		if (current->isRunning())
        {
            current->onExitTransitionDidStart();
            current->onExit();
		}

        current->cleanup();
        m_pScenesStack->removeLastObject();
		--c;
	}

    m_pNextScene = (Scene*)m_pScenesStack->getLastObject();
	m_bSendCleanupToScene = false;
}

void Director::end()
{
    m_bPurgeDirecotorInNextLoop = true;
}

void Director::purgeDirector()
{
    // cleanup scheduler
    getScheduler()->unscheduleAll();
    
    // don't release the event handlers
    // They are needed in case the director is run again
//    _touchDispatcher->removeAllDelegates();

    if (m_pRunningScene)
    {
        m_pRunningScene->onExitTransitionDidStart();
        m_pRunningScene->onExit();
        m_pRunningScene->cleanup();
        m_pRunningScene->release();
    }
    
    m_pRunningScene = nullptr;
    m_pNextScene = nullptr;

    // remove all objects, but don't release it.
    // runWithScene might be executed after 'end'.
    m_pScenesStack->removeAllObjects();

    stopAnimation();

    CC_SAFE_RELEASE(m_pFPSLabel);
    CC_SAFE_RELEASE(m_pSPFLabel);
    CC_SAFE_RELEASE(m_pDrawsLabel);

    // purge bitmap cache
    LabelBMFont::purgeCachedData();

    FontFreeType::shutdownFreeType();

    // purge all managed caches
    DrawPrimitives::free();
    AnimationCache::destroyInstance();
    SpriteFrameCache::destroyInstance();
    ShaderCache::destroyInstance();
    FileUtils::destroyInstance();
    Configuration::destroyInstance();

    // cocos2d-x specific data structures
    UserDefault::destroyInstance();
    NotificationCenter::destroyInstance();
    
    GL::invalidateStateCache();
    
    destroyTextureCache();

    CHECK_GL_ERROR_DEBUG();
    
    // OpenGL view
    if (m_pOpenGLView)
    {
//        m_pOpenGLView->end();
//        m_pOpenGLView = nullptr;
    }

    // delete Director
    release();

	kdExit ( 0 );
}

void Director::setNextScene()
{
    bool runningIsTransition = dynamic_cast<TransitionScene*>(m_pRunningScene) != nullptr;
    bool newIsTransition = dynamic_cast<TransitionScene*>(m_pNextScene) != nullptr;

    // If it is not a transition, call onExit/cleanup
     if (! newIsTransition)
     {
         if (m_pRunningScene)
         {
             m_pRunningScene->onExitTransitionDidStart();
             m_pRunningScene->onExit();
         }
 
         // issue #709. the root node (scene) should receive the cleanup message too
         // otherwise it might be leaked.
         if (m_bSendCleanupToScene && m_pRunningScene)
         {
             m_pRunningScene->cleanup();
         }
     }

    if (m_pRunningScene)
    {
        m_pRunningScene->release();
    }
    m_pRunningScene = m_pNextScene;
    m_pNextScene->retain();
    m_pNextScene = nullptr;

    if ((! runningIsTransition) && m_pRunningScene)
    {
        m_pRunningScene->onEnter();
        m_pRunningScene->onEnterTransitionDidFinish();
    }
}

void Director::pause()
{
    if (m_bPaused)
    {
        return;
    }

    m_dOldAnimationInterval = m_dAnimationInterval;

    // when paused, don't consume CPU
    setAnimationInterval(1 / 4.0);
    m_bPaused = true;
}

void Director::resume()
{
    if (! m_bPaused)
    {
        return;
    }

    setAnimationInterval(m_dOldAnimationInterval);

	m_dLastUpdate = kdGetMilliseconds();

    m_bPaused = false;
    m_fDeltaTime = 0;
}

// display the FPS using a LabelAtlas
// updates the FPS every frame
void Director::showStats()
{
    ++m_uFrames;
    m_fAccumDt += m_fDeltaTime;
    
    if (m_bDisplayStats)
    {
        if (m_pFPSLabel && m_pSPFLabel && m_pDrawsLabel)
        {
            if (m_fAccumDt > CC_DIRECTOR_STATS_INTERVAL)
            {
                sprintf(m_szFPS, "%.3f", m_fSecondsPerFrame);
                m_pSPFLabel->setString(m_szFPS);
                
                m_fFrameRate = m_uFrames / m_fAccumDt;
                m_uFrames = 0;
                m_fAccumDt = 0;
                
                sprintf(m_szFPS, "%.1f", m_fFrameRate);
                m_pFPSLabel->setString(m_szFPS);
                
                sprintf(m_szFPS, "%4lu", (unsigned long)g_uNumberOfDraws);
                m_pDrawsLabel->setString(m_szFPS);
            }
            
            m_pDrawsLabel->visit();
            m_pFPSLabel->visit();
            m_pSPFLabel->visit();
        }
    }    
    
    g_uNumberOfDraws = 0;
}

KDvoid Director::calculateMPF()
{
	KDdouble now = kdGetMilliseconds();
    
	m_fSecondsPerFrame = (now - m_dLastUpdate) / 1000.0; 
}

// returns the FPS image data pointer and len
KDvoid Director::getFPSImageData ( KDubyte** ppDatapointer, KDsize* pLength )
{
    // XXX fixed me if it should be used 
    *ppDatapointer = cc_fps_images_png;
	*pLength = cc_fps_images_len ( );
}

void Director::createStatsLabel()
{
    Texture2D *texture = nullptr;

    if (m_pFPSLabel && m_pSPFLabel)
    {
        CC_SAFE_RELEASE(m_pFPSLabel);
        CC_SAFE_RELEASE(m_pSPFLabel);
        CC_SAFE_RELEASE(m_pDrawsLabel);
        m_pTextureCache->removeTextureForKey("/cc_fps_images");
        FileUtils::getInstance()->purgeCachedEntries();
    }

    Texture2D::PixelFormat currentFormat = Texture2D::getDefaultAlphaPixelFormat();
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);

    KDubyte*	data = nullptr;
    KDsize		dataLength = 0;
    getFPSImageData ( &data, &dataLength );

    Image* image = new Image();
    bool isOK = image->initWithImageData(data, dataLength);
    if (! isOK) {
        CCLOGERROR("%s", "Fails: init fps_images");
        return;
    }

    texture = m_pTextureCache->addImage(image, "/cc_fps_images");
    CC_SAFE_RELEASE(image);

    /*
     We want to use an image which is stored in the file named ccFPSImage.c 
     for any design resolutions and all resource resolutions. 
     
     To achieve this,
     
     Firstly, we need to ignore 'contentScaleFactor' in 'AtlasNode' and 'LabelAtlas'.
     So I added a new method called 'setIgnoreContentScaleFactor' for 'AtlasNode',
     this is not exposed to game developers, it's only used for displaying FPS now.
     
     Secondly, the size of this image is 480*320, to display the FPS label with correct size, 
     a factor of design resolution ratio of 480x320 is also needed.
     */
    float factor = EGLView::getInstance()->getDesignResolutionSize().height / 320.0f;

    m_pFPSLabel = new LabelAtlas();
    m_pFPSLabel->setIgnoreContentScaleFactor(true);
    m_pFPSLabel->initWithString("00.0", texture, 12, 32 , '.');
    m_pFPSLabel->setScale(factor);

    m_pSPFLabel = new LabelAtlas();
    m_pSPFLabel->setIgnoreContentScaleFactor(true);
    m_pSPFLabel->initWithString("0.000", texture, 12, 32, '.');
    m_pSPFLabel->setScale(factor);

    m_pDrawsLabel = new LabelAtlas();
    m_pDrawsLabel->setIgnoreContentScaleFactor(true);
    m_pDrawsLabel->initWithString("000", texture, 12, 32, '.');
    m_pDrawsLabel->setScale(factor);

    Texture2D::setDefaultAlphaPixelFormat(currentFormat);

    m_pDrawsLabel->setPosition(Point(0, 34*factor) + CC_DIRECTOR_STATS_POSITION);
    m_pSPFLabel->setPosition(Point(0, 17*factor) + CC_DIRECTOR_STATS_POSITION);
    m_pFPSLabel->setPosition(CC_DIRECTOR_STATS_POSITION);
}

float Director::getContentScaleFactor() const
{
    return m_fContentScaleFactor;
}

void Director::setContentScaleFactor(float scaleFactor)
{
    if (scaleFactor != m_fContentScaleFactor)
    {
        m_fContentScaleFactor = scaleFactor;
        createStatsLabel();
    }
}

Node* Director::getNotificationNode() 
{ 
    return m_pNotificationNode; 
}

void Director::setNotificationNode(Node *node)
{
    CC_SAFE_RELEASE(m_pNotificationNode);
    m_pNotificationNode = node;
    CC_SAFE_RETAIN(m_pNotificationNode);
}

DirectorDelegate* Director::getDelegate() const
{
    return m_pProjectionDelegate;
}

void Director::setDelegate(DirectorDelegate* delegate)
{
    m_pProjectionDelegate = delegate;
}

void Director::setScheduler(Scheduler* scheduler)
{
    if (m_pScheduler != scheduler)
    {
        CC_SAFE_RETAIN(scheduler);
        CC_SAFE_RELEASE(m_pScheduler);
        m_pScheduler = scheduler;
    }
}

Scheduler* Director::getScheduler() const
{
    return m_pScheduler;
}

void Director::setActionManager(ActionManager* actionManager)
{
    if (m_pActionManager != actionManager)
    {
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(m_pActionManager);
        m_pActionManager = actionManager;
    }    
}

ActionManager* Director::getActionManager() const
{
    return m_pActionManager;
}

EventDispatcher* Director::getEventDispatcher() const
{
    return m_pEventDispatcher;
}

void Director::setEventDispatcher(EventDispatcher* dispatcher)
{
    if (m_pEventDispatcher != dispatcher)
    {
        CC_SAFE_RETAIN(dispatcher);
        CC_SAFE_RELEASE(m_pEventDispatcher);
        m_pEventDispatcher = dispatcher;
    }
}

/***************************************************
* implementation of DisplayLinkDirector
**************************************************/

DisplayLinkDirector::DisplayLinkDirector ( KDvoid )
{
	m_bInvalid = KD_FALSE;
}

// should we implement 4 types of director ??
// I think DisplayLinkDirector is enough
// so we now only support DisplayLinkDirector
KDvoid DisplayLinkDirector::startAnimation ( KDvoid )
{
	m_dLastUpdate = kdGetMilliseconds ( );

    m_bInvalid = false;
#ifndef EMSCRIPTEN
	Application::getInstance ( )->setAnimationInterval ( m_dAnimationInterval );
#endif // EMSCRIPTEN
}

KDbool DisplayLinkDirector::mainLoop ( KDvoid )
{
    if ( m_bPurgeDirecotorInNextLoop )
    {
        m_bPurgeDirecotorInNextLoop = false;
        return KD_FALSE;
    }
    else if ( !m_bInvalid )
    {
        drawScene ( );
     
        // release the objects
        PoolManager::sharedPoolManager ( )->pop ( );        
    }

	return KD_TRUE;
}

KDvoid DisplayLinkDirector::stopAnimation ( KDvoid )
{
    m_bInvalid = true;
}

KDvoid DisplayLinkDirector::setAnimationInterval ( KDdouble interval )
{
    m_dAnimationInterval = interval;
    if ( !m_bInvalid )
    {
        stopAnimation  ( );
        startAnimation ( );
    }    
}

NS_CC_END

