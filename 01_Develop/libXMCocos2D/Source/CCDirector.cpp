/* -----------------------------------------------------------------------------------
 *
 *      File            CCDirector.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCDirector.h"
#include "support/CCScheduler.h"
#include "support/CCConfiguration.h"
#include "support/user_default/CCUserDefault.h"
#include "support/CCPointExtension.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "cocoa/CCAutoreleasePool.h"
#include "cocoa/CCSet.h"
#include "actions/CCActionManager.h"
#include "sprite_nodes/CCAnimationCache.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "textures/CCTextureCache.h"
#include "shaders/CCShaderCache.h"
#include "accelerometer_dispatcher/CCAccelerometer.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "text_input_node/CCIMEDispatcher.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "event_dispatcher/CCEventDispatcher.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "layers_scenes_transitions_nodes/CCTransition.h"
#include "label_nodes/CCLabelAtlas.h"
#include "label_nodes/CCLabelBMFont.h"
#include "support/CCEGLView.h"
#include "support/CCNotificationCenter.h"
#include "extensions/CCBReader/CCNodeLoaderLibrary.h"
#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"
#include "CCApplication.h"
#include "ccFPSImages.h"

NS_CC_BEGIN


// Position of the FPS 
// Default: 0,0 (bottom-left corner)

#ifndef CC_DIRECTOR_STATS_POSITION
#define CC_DIRECTOR_STATS_POSITION		CCDirector::sharedDirector ( )->getVisibleOrigin ( )
#endif

	   KDuint			g_uNumberOfDraws  = 0;
static CCDirector*		l_pSharedDirector = KD_NULL;

#define kDefaultFPS		60  // 60 frames per second

CCDirector* CCDirector::sharedDirector ( KDvoid )
{
	if ( !l_pSharedDirector )
	{
		l_pSharedDirector = new CCDirector ( );	
		
		CCAssert ( l_pSharedDirector, "" ); 

		l_pSharedDirector->init ( );
	}

	return l_pSharedDirector;
}

CCDirector::CCDirector ( KDvoid )
{
	m_tClearColor.r = 0.0f;
	m_tClearColor.g = 0.0f;
	m_tClearColor.b = 0.0f;
	m_tClearColor.a = 1.0f;

	m_uClearFlags   = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	m_pBackNode		= KD_NULL;
}

KDbool CCDirector::init ( KDvoid )
{
	this->setDefaultValues ( );

    // scenes
    m_pRunningScene				= KD_NULL;
    m_pNextScene				= KD_NULL;
    m_pNotificationNode			= KD_NULL;

    m_pScenesStack				= new CCArray ( );
    m_pScenesStack->init ( );

    // projection delegate if "Custom" projection is used
    m_pProjectionDelegate		= KD_NULL;

    // FPS
    m_fAccumDt					= 0.0f;
    m_fFrameRate				= 0.0f;
    m_pFPSLabel					= KD_NULL;
    m_pSPFLabel					= KD_NULL;
    m_pDrawsLabel				= KD_NULL;
    m_uTotalFrames				= m_uFrames = 0;
	m_uLastUpdate				= 0;
	m_bInvalid					= KD_FALSE;

    // paused ?
    m_bPaused					= KD_FALSE;
    
    // purge ?
    m_bPurgeDirecotorInNextLoop = KD_FALSE;

	m_tWinSizeInPoints			= CCSizeZero;
	m_fContentScaleFactor		= 1.0f;

    // scheduler
    m_pScheduler				= new CCScheduler ( );

    // action manager
    m_pActionManager			= new CCActionManager ( );
    m_pScheduler->scheduleUpdateForTarget ( m_pActionManager, kCCPrioritySystem, KD_FALSE );

    // touchDispatcher
    m_pTouchDispatcher			= new CCTouchDispatcher ( );
    m_pTouchDispatcher->init ( );
	m_pTouchDispatcher->setDispatchEvents ( KD_TRUE );

    // KeypadDispatcher
    m_pKeypadDispatcher			= new CCKeypadDispatcher ( );

    // Accelerometer
    m_pAccelerometer			= new CCAccelerometer ( );

	m_pEventDispatcher			= new CCEventDispatcher ( );

    // create autorelease pool
    CCPoolManager::sharedPoolManager ( )->push ( );

    return KD_TRUE;
}

KDint32 CCDirector::getClassTypeInfo ( KDvoid )
{
	static const KDint32  nID = getHashCodeByString ( typeid ( CCDirector ).name ( ) );
	return nID;
}

CCDirector::~CCDirector( KDvoid )
{
    CCLOGINFO ( "XMCocos2D : deallocing CCDirector = %p", this );
    
    CC_SAFE_RELEASE ( m_pRunningScene );
    CC_SAFE_RELEASE ( m_pNotificationNode );
    CC_SAFE_RELEASE ( m_pScenesStack );
    CC_SAFE_RELEASE ( m_pScheduler );
    CC_SAFE_RELEASE ( m_pActionManager );
    CC_SAFE_RELEASE ( m_pTouchDispatcher );
    CC_SAFE_RELEASE ( m_pKeypadDispatcher );
    CC_SAFE_DELETE  ( m_pAccelerometer );
	CC_SAFE_DELETE  ( m_pEventDispatcher );

    // pop the autorelease pool
    CCPoolManager::sharedPoolManager ( )->pop ( );
    CCPoolManager::purgePoolManager  ( );

	l_pSharedDirector = KD_NULL;

	kdExit ( 0 );
}

KDvoid CCDirector::setDefaultValues ( KDvoid )
{
	CCConfiguration*	pConf = CCConfiguration::sharedConfiguration ( );

	// default FPS
	KDdouble			dFps = pConf->getNumber ( "cocos2d.x.fps", kDefaultFPS );
	m_dOldAnimationInterval = m_dAnimationInterval = 1.0 / dFps;

	// Display FPS
	m_bDisplayStats = pConf->getBool ( "cocos2d.x.display_fps", KD_FALSE );

	// GL projection
	const KDchar*		szProjection = pConf->getCString ( "cocos2d.x.gl.projection", "3d" );
	if ( kdStrcmp ( szProjection, "3d" ) == 0 )
	{
		m_eProjection = kCCDirectorProjection3D;
	}
	else if ( kdStrcmp ( szProjection, "2d" ) == 0 )
	{
		m_eProjection = kCCDirectorProjection2D;
	}
	else if ( kdStrcmp ( szProjection, "custom" ) == 0 )
	{
		m_eProjection = kCCDirectorProjectionCustom;
	}
	else
	{
		CCAssert ( KD_FALSE, "Invalid projection value" );
	}

	// Default pixel format for PNG images with alpha
	const KDchar*		szPixelFormat = pConf->getCString ( "cocos2d.x.texture.pixel_format_for_png", "rgba8888" );
	if ( kdStrcmp ( szPixelFormat, "rgba8888" ) == 0 )
	{
		CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA8888 );
	}
	else if ( kdStrcmp ( szPixelFormat, "rgba4444" ) == 0 )
	{
		CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA4444 );
	}
	else if ( kdStrcmp ( szPixelFormat, "rgba5551" ) == 0 )
	{
		CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB5A1 );
	}

	// PVR v2 has alpha premultiplied ?
//	KDbool	bPvrAlphaPremultipled = pConf->getBool ( "cocos2d.x.texture.pvrv2_has_alpha_premultiplied", KD_FALSE );
//	CCTexture2D::PVRImagesHavePremultipliedAlpha ( bPvrAlphaPremultipled );
}

KDvoid CCDirector::setGLDefaultValues ( KDvoid )
{
	ccGLInvalidateStateCache ( );

    setAlphaBlending ( KD_TRUE );
    setDepthTest     ( KD_FALSE );

	setProjection ( m_eProjection );

    // set other opengl default values
    glClearColor ( m_tClearColor.r, m_tClearColor.g, m_tClearColor.b, m_tClearColor.a );

	glPixelStorei ( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );	

	glEnable    ( GL_CULL_FACE );
	glFrontFace ( GL_CCW );

	glDisable ( GL_CULL_FACE );

	glActiveTexture ( GL_TEXTURE1 );
	glDisable ( GL_TEXTURE_2D );

	glActiveTexture ( GL_TEXTURE0 );
	glDisable ( GL_TEXTURE_2D );
	
	glBindTexture ( GL_TEXTURE_2D, 0 );		
	glBindBuffer  ( GL_ARRAY_BUFFER, 0 );
	glBindBuffer  ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glLineWidth ( 1 );
    
#if !defined ( USE_OPEN_GLES2 )      
    glPointSize ( 1 ); 
	glDisable ( GL_LINE_SMOOTH );
	glDisable ( GL_ALPHA_TEST );
    glDisable ( GL_FOG );
	glDisable ( GL_LIGHTING );
	glDisable ( GL_NORMALIZE );

   	glClientActiveTexture ( GL_TEXTURE1 );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );	
    
	glClientActiveTexture ( GL_TEXTURE0 );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
    
	glDisableClientState ( GL_COLOR_ARRAY );
	glDisableClientState ( GL_VERTEX_ARRAY );
	glDisableClientState ( GL_NORMAL_ARRAY );
    
	glTexEnvi ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
#endif

}

// Draw the Scene
KDvoid CCDirector::drawScene ( KDvoid )
{
	CCEGLView*			pEGLView = CCEGLView::sharedOpenGLView ( );
	ResolutionPolicy	ePolicy  = pEGLView->getDesignResolutionPolicy ( );
	CCSize				tSize    = pEGLView->getDesignResolutionSize ( );

    // calculate "global" dt
    calculateDeltaTime ( );

    // tick before glClear: issue #533
    if ( !m_bPaused )
    {
        m_pScheduler->update ( m_fDeltaTime );
    }

	glClear ( m_uClearFlags );
  
	if ( m_pBackNode )
	{
		pEGLView->setDesignResolutionSize ( m_pBackNode->getContentSize ( ), kResolutionExactFit );

		m_pBackNode->visit ( );

		pEGLView->setDesignResolutionSize ( tSize, ePolicy );
	}

    // to avoid flickr, nextScene MUST be here: after tick and before draw.
    // XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 
    if ( m_pNextScene )
    {
        setNextScene ( );
    }

    kmGLPushMatrix ( );

    // draw the scene
    if ( m_pRunningScene )
    {
        m_pRunningScene->visit ( );
    }

    // draw the notifications node
    if ( m_pNotificationNode )
    {
        m_pNotificationNode->visit ( );
    }  

    kmGLPopMatrix ( );

    if ( m_bDisplayStats )
    {
        showStats ( );	
    }

    m_uTotalFrames++;
    
    if ( m_bDisplayStats )
    {
        calculateMPF ( );
    }
}

KDvoid CCDirector::calculateDeltaTime ( KDvoid )
{
	KDuint  uNow = KD_GET_UST2MSEC;

    // new delta time. Re-fixed issue #1277
    if ( m_bNextDeltaTimeZero )
    {
        m_fDeltaTime = 0;
        m_bNextDeltaTimeZero = KD_FALSE;
    }
    else
    {
        m_fDeltaTime = ( uNow - m_uLastUpdate ) / 1000.f;
        m_fDeltaTime = KD_MAX ( 0 , m_fDeltaTime );
    }

    m_uLastUpdate = uNow;
}

KDfloat CCDirector::getDeltaTime ( KDvoid )
{
	return m_fDeltaTime;
}

KDvoid CCDirector::setViewport ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setViewPortInPoints ( 0, 0, m_tWinSizeInPoints.cx, m_tWinSizeInPoints.cy );
}

KDvoid CCDirector::setNextDeltaTimeZero ( KDbool bNextDeltaTimeZero )
{
    m_bNextDeltaTimeZero = bNextDeltaTimeZero;
}

KDvoid CCDirector::setProjection ( ccDirectorProjection eProjection )
{    
	CCSize  tSize = m_tWinSizeInPoints;
	
	setViewport ( );

    switch ( eProjection )
    {
		case kCCDirectorProjection2D :
			{              
				kmGLMatrixMode ( KM_GL_PROJECTION );
				kmGLLoadIdentity ( );

			#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
				kmGLMultMatrix ( CCEGLView::sharedOpenGLView ( )->getOrientationMatrix ( ) );
			#endif

				kmMat4  tOrthoMatrix;
				kmMat4OrthographicProjection ( &tOrthoMatrix, 0, tSize.cx, 0, tSize.cy, -1024, 1024 );
				kmGLMultMatrix ( &tOrthoMatrix );

				kmGLMatrixMode ( KM_GL_MODELVIEW );
				kmGLLoadIdentity ( );
			}
			break;

		case kCCDirectorProjection3D :
			{
				KDfloat  fZEye = this->getZEye ( );

				kmGLMatrixMode ( KM_GL_PROJECTION );
				kmGLLoadIdentity ( );

			#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
				//if needed, we need to add a rotation for Landscape orientations on Windows Phone 8 since it is always in Portrait Mode
				kmGLMultMatrix ( CCEGLView::sharedOpenGLView ( )->getOrientationMatrix ( ) );
			#endif

				kmMat4  tMatrixPerspective;
				kmMat4PerspectiveProjection ( &tMatrixPerspective, 60, tSize.cx / tSize.cy, 0.1f, fZEye * 2 );
				kmGLMultMatrix ( &tMatrixPerspective );

				kmGLMatrixMode ( KM_GL_MODELVIEW );
				kmGLLoadIdentity ( );

				kmMat4  tMatrixLookup;
				kmVec3  tEye, tCenter, tUp;				
				kmVec3Fill ( &tEye   , tSize.cx / 2, tSize.cy / 2, fZEye );
				kmVec3Fill ( &tCenter, tSize.cx / 2, tSize.cy / 2, 0.0f );
				kmVec3Fill ( &tUp    , 0.0f, 1.0f, 0.0f );
				kmMat4LookAt ( &tMatrixLookup, &tEye, &tCenter, &tUp );
				kmGLMultMatrix ( &tMatrixLookup );
			}
			break;
	            
		case kCCDirectorProjectionCustom :

			if ( m_pProjectionDelegate )
			{
				m_pProjectionDelegate->updateProjection ( );
			}

			break;
	            
		default :

			CCLOG("XMCocos2D : Director: unrecognized projection");

			break;
    }

    m_eProjection = eProjection;

    ccSetProjectionMatrixDirty ( );
}

KDvoid CCDirector::purgeCachedData ( KDvoid )
{
    CCLabelBMFont::purgeCachedData ( );
    CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );
	CCFileUtils::sharedFileUtils ( )->purgeCachedEntries ( );
}

KDfloat CCDirector::getZEye ( KDvoid )
{
    return ( m_tWinSizeInPoints.cy / 1.1566f );    
}

KDvoid CCDirector::setAlphaBlending ( KDbool bOn )
{
    if ( bOn )
    {        
        ccGLBlendFunc ( CC_BLEND_SRC, CC_BLEND_DST );
    }
	else
	{
		ccGLBlendFunc ( GL_ONE, GL_ZERO );
	}

    CHECK_GL_ERROR_DEBUG ( );
}

KDvoid CCDirector::reshapeProjection ( const CCSize& tNewWindowSize )
{
	CC_UNUSED_PARAM ( tNewWindowSize );
//	if ( m_pobOpenGLView )
	{
		m_tWinSizeInPoints = CCSizeMake ( tNewWindowSize.width * m_fContentScaleFactor, tNewWindowSize.height * m_fContentScaleFactor );
		setProjection ( m_eProjection );       
	}
}

KDvoid CCDirector::setDepthTest ( KDbool bOn )
{
    if ( bOn )
    {
		glClearDepth ( 1.0f );
        glEnable	 ( GL_DEPTH_TEST );
        glDepthFunc  ( GL_LEQUAL );
    }
    else
    {
        glDisable	  ( GL_DEPTH_TEST );
    }

    CHECK_GL_ERROR_DEBUG ( );
}

KDvoid CCDirector::setClearColor ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha )
{
    m_tClearColor.r = fRed;
	m_tClearColor.g = fGreen;
	m_tClearColor.b = fBlue;
	m_tClearColor.a = fAlpha;
}

KDvoid CCDirector::setClearFlags ( KDuint uFlags )
{
	m_uClearFlags = uFlags;
}

KDbool CCDirector::mainLoop ( KDvoid )
{
	if ( m_bPurgeDirecotorInNextLoop )
    {
        m_bPurgeDirecotorInNextLoop = KD_FALSE;
        return KD_FALSE;
    }
    else if ( !m_bInvalid )
	{
		drawScene ( );

		// release the objects
		CCPoolManager::sharedPoolManager ( )->pop ( );        		
	}

	return KD_TRUE;
}

KDvoid CCDirector::GLToClipTransform ( kmMat4* pTransformOut )
{
	kmGLMatrixMode ( KM_GL_PROJECTION );
	kmGLGetMatrix  ( KM_GL_PROJECTION, &m_tProjection );

#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	//if needed, we need to undo the rotation for Landscape orientation in order to get the correct positions
	kmMat4Multiply ( &m_tProjection, CCEGLView::sharedOpenGLView ( )->getReverseOrientationMatrix ( ), &m_tProjection );
#endif

	kmGLMatrixMode ( KM_GL_MODELVIEW );
	kmGLGetMatrix  ( KM_GL_MODELVIEW, &m_tModelview );

	if ( pTransformOut )
	{
		kmMat4Multiply ( pTransformOut, &m_tProjection, &m_tModelview );
	}
}

KDvoid CCDirector::ClipToGLTransform ( KDvoid )
{
	kmGLMatrixMode ( KM_GL_PROJECTION );
	kmGLLoadMatrix ( &m_tProjection );

	kmGLMatrixMode ( KM_GL_MODELVIEW );
	kmGLLoadMatrix ( &m_tModelview );
}

CCPoint CCDirector::convertToGL ( const CCPoint& tUIPoint )
{
	kmMat4		tTransform;
	GLToClipTransform ( &tTransform );
	
	kmMat4		tTransformInv;
	kmMat4Inverse ( &tTransformInv, &tTransform );
	
	// Calculate z=0 using -> transform*[0, 0, 0, 1]/w
	kmScalar	fZClip = tTransform.mat [ 14 ] / tTransform.mat [ 15 ];
	
	const CCSize&  tGLSize = CCEGLView::sharedOpenGLView ( )->getDesignResolutionSize ( );
	kmVec3		tClipCoord = { 2.0f * tUIPoint.x / tGLSize.cx - 1.0f, 1.0f - 2.0f * tUIPoint.y / tGLSize.cy, fZClip };
	
	kmVec3		tGLCoord;
	kmVec3TransformCoord ( &tGLCoord, &tClipCoord, &tTransformInv );
	
	return ccp ( tGLCoord.x, tGLCoord.y );
}

CCPoint CCDirector::convertToUI ( const CCPoint& tGLPoint )
{
    kmMat4		tTransform;
	GLToClipTransform ( &tTransform );
    
	kmVec3		tClipCoord;
	// Need to calculate the zero depth from the transform.
	kmVec3		tGLCoord = { tGLPoint.x, tGLPoint.y, 0.0f };
	kmVec3TransformCoord ( &tClipCoord, &tGLCoord, &tTransform );
	
	const CCSize&	tGLSize = CCEGLView::sharedOpenGLView ( )->getDesignResolutionSize ( );
	return ccp ( tGLSize.cx * ( tClipCoord.x * 0.5f + 0.5f ), tGLSize.cy * ( -tClipCoord.y * 0.5f + 0.5f ) );
}

CCSize CCDirector::getWinSize ( KDvoid )
{
    return m_tWinSizeInPoints;
}

CCSize CCDirector::getWinSizeInPixels ( KDvoid )
{
    return CCSizeMake ( m_tWinSizeInPoints.cx * m_fContentScaleFactor, m_tWinSizeInPoints.cy * m_fContentScaleFactor );
}

CCSize CCDirector::getVisibleSize ( KDvoid )
{  
	return CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );
}

CCPoint CCDirector::getVisibleOrigin ( KDvoid )
{
	return CCEGLView::sharedOpenGLView ( )->getVisibleOrigin ( );
}


//
// scene management
//

KDvoid CCDirector::runWithScene ( CCScene* pScene )
{
    CCAssert ( pScene != KD_NULL, "This command can only be used to start the CCDirector. There is already a scene present." ); 
    CCAssert ( m_pRunningScene == KD_NULL, "m_pRunningScene should be null" );

    pushScene ( pScene );

    startAnimation ( );
}

KDvoid CCDirector::replaceScene ( CCScene* pScene )
{
    CCAssert ( m_pRunningScene, "Use runWithScene: instead to start the director" );
    CCAssert ( pScene != KD_NULL, "the scene should not be null" );

    KDuint  uIndex = m_pScenesStack->count ( );

    m_bSendCleanupToScene = KD_TRUE;
    m_pScenesStack->replaceObjectAtIndex ( uIndex - 1, pScene );

    m_pNextScene = pScene;
}

KDvoid CCDirector::pushScene ( CCScene* pScene )
{
    CCAssert ( pScene, "the scene should not null" );

    m_bSendCleanupToScene = KD_FALSE;

    m_pScenesStack->addObject ( pScene );
    m_pNextScene = pScene;
}

KDvoid CCDirector::popScene ( KDvoid )
{
    CCAssert ( m_pRunningScene != KD_NULL, "running scene should not null" );

    m_pScenesStack->removeLastObject  ( );
    KDuint  c = m_pScenesStack->count ( );

    if ( c == 0 )
    {
        end ( );
    }
    else
    {
        m_bSendCleanupToScene = KD_TRUE;
        m_pNextScene = (CCScene*) m_pScenesStack->objectAtIndex ( c - 1 );
    }
}

KDvoid CCDirector::popToRootScene ( KDvoid )
{
    popToSceneStackLevel ( 1 );
}

KDvoid CCDirector::popToSceneStackLevel ( KDint nLevel )
{
	CCAssert ( m_pRunningScene != KD_NULL, "A running Scene is needed" );
	KDint  c = m_pScenesStack->count ( );

	// level 0? -> end
	if ( nLevel == 0 )
	{
		end ( );
		return;
	}

	// current level or lower -> nothing
	if ( nLevel >= c )
	{
		return;
	}

	// pop stack until reaching desired level
	while ( c > nLevel )
	{
		CCScene*	pCurrent = (CCScene*) m_pScenesStack->lastObject ( );

		if ( pCurrent->isRunning ( ) )
		{
			pCurrent->onExitTransitionDidStart ( );
			pCurrent->onExit ( );
		}

		pCurrent->cleanup ( );
		m_pScenesStack->removeLastObject ( );
		c--;
	}

	m_pNextScene = (CCScene*) m_pScenesStack->lastObject ( );
	m_bSendCleanupToScene = KD_FALSE;
}

KDvoid CCDirector::end ( KDvoid )
{
    m_bPurgeDirecotorInNextLoop = KD_TRUE;
}

KDvoid CCDirector::purgeDirector ( KDvoid )
{
	// cleanup scheduler
    getScheduler ( )->unscheduleAll ( );

    // don't release the event handlers
    // They are needed in case the director is run again
    m_pTouchDispatcher->removeAllDelegates ( );

    if ( m_pRunningScene )
    {
		m_pRunningScene->onExitTransitionDidStart ( );
        m_pRunningScene->onExit  ( );
        m_pRunningScene->cleanup ( );
        m_pRunningScene->release ( );
    }
    
    m_pRunningScene = KD_NULL;
    m_pNextScene	= KD_NULL;

    // remove all objects, but don't release it.
    // runWithScene might be executed after 'end'.
    m_pScenesStack->removeAllObjects ( );

    stopAnimation ( );
    
    CC_SAFE_RELEASE ( m_pFPSLabel );
    CC_SAFE_RELEASE ( m_pSPFLabel );
    CC_SAFE_RELEASE ( m_pDrawsLabel );
	CC_SAFE_RELEASE ( m_pBackNode );

    // purge bitmap cache
    CCLabelBMFont::purgeCachedData ( );
    
    // purge all managers £¯ caches
	ccDrawFree ( );

    CCAnimationCache	::purgeSharedAnimationCache ( );   
    CCSpriteFrameCache	::purgeSharedSpriteFrameCache ( );
    CCTextureCache		::purgeSharedTextureCache ( );
    CCShaderCache		::purgeSharedShaderCache ( );
	CCFileUtils			::purgeFileUtils ( );
    CCConfiguration		::purgeConfiguration ( );

    // cocos2d-x specific data structures
    CCUserDefault		::purgeSharedUserDefault ( );
    CCNotificationCenter::purgeNotificationCenter ( );
    CCNodeLoaderLibrary	::purgeSharedCCNodeLoaderLibrary();

	kmGLFreeAll ( );    
    
    CHECK_GL_ERROR_DEBUG ( );

	kdExit ( 0 );

    release ( );
}

KDvoid CCDirector::setNextScene ( KDvoid )
{
    KDbool  bRunningIsTransition = dynamic_cast<CCTransitionScene*> ( m_pRunningScene ) != KD_NULL;
    KDbool  bNewIsTransition     = dynamic_cast<CCTransitionScene*> ( m_pNextScene	  ) != KD_NULL;

    // If it is not a transition, call onExit/cleanup
     if ( !bNewIsTransition )
     {
         if ( m_pRunningScene )
         {
			 m_pRunningScene->onExitTransitionDidStart ( );
             m_pRunningScene->onExit ( );
         }
 
         // issue #709. the root node (scene) should receive the cleanup message too
         // otherwise it might be leaked.
         if ( m_bSendCleanupToScene && m_pRunningScene )
         {
             m_pRunningScene->cleanup ( );
         }
     }

    if ( m_pRunningScene )
    {
        m_pRunningScene->release ( );
    }
    m_pRunningScene = m_pNextScene;
    m_pNextScene->retain ( );
    m_pNextScene = KD_NULL;

    if ( ( !bRunningIsTransition ) && m_pRunningScene )
    {
        m_pRunningScene->onEnter ( );
        m_pRunningScene->onEnterTransitionDidFinish ( );
    }
}

KDvoid CCDirector::pause ( KDvoid )
{
    if ( m_bPaused )
    {
        return;
    }

    m_dOldAnimationInterval = m_dAnimationInterval;

    // when paused, don't consume CPU
    setAnimationInterval ( 1 / 4.0 );
    m_bPaused = KD_TRUE;
}

KDvoid CCDirector::resume ( KDvoid )
{
    if ( !m_bPaused )
    {
        return;
    }

    setAnimationInterval ( m_dOldAnimationInterval );

    m_bPaused    = KD_FALSE;
    m_fDeltaTime = 0;
}

// display the FPS using a LabelAtlas
// updates the FPS every frame
KDvoid CCDirector::showStats ( KDvoid )
{
    m_uFrames++;
    m_fAccumDt += m_fDeltaTime;
    
    if ( m_bDisplayStats )
    {
        if ( m_pFPSLabel && m_pSPFLabel && m_pDrawsLabel )
        {
            if ( m_fAccumDt > CC_DIRECTOR_STATS_INTERVAL )
            {
                kdSprintf ( m_szFPS, "%.3f", m_fSecondsPerFrame );
                m_pSPFLabel->setString ( m_szFPS );
                
                m_fFrameRate = m_uFrames / m_fAccumDt;
                m_uFrames    = 0;
                m_fAccumDt   = 0;
                
                kdSprintf ( m_szFPS, "%.1f", m_fFrameRate );
                m_pFPSLabel->setString ( m_szFPS );
                
                kdSprintf ( m_szFPS, "%4lu", g_uNumberOfDraws );
                m_pDrawsLabel->setString ( m_szFPS );
            }
            
            m_pDrawsLabel->visit ( );
            m_pFPSLabel  ->visit ( );
            m_pSPFLabel  ->visit ( );
        }
    }    
    
    g_uNumberOfDraws = 0;
}

KDvoid CCDirector::calculateMPF ( KDvoid )
{
    KDuint  uNow = KD_GET_UST2MSEC;
    
    m_fSecondsPerFrame = ( uNow - m_uLastUpdate ) / 1000.f;
}

// returns the FPS image data pointer and len
KDvoid CCDirector::getFPSImageData ( KDubyte** datapointer, KDuint* pLength )
{
    // XXX fixed me if it should be used 
    *datapointer = cc_fps_images_png;
	*pLength = cc_fps_images_len ( );
}

KDvoid CCDirector::createStatsLabel ( KDvoid )
{
	CCTexture2D*		pTexture = KD_NULL;
    CCTextureCache*		pTextureCache = CCTextureCache::sharedTextureCache ( );

    if ( m_pFPSLabel && m_pSPFLabel )
    {
        CC_SAFE_RELEASE ( m_pFPSLabel );
        CC_SAFE_RELEASE ( m_pSPFLabel );
        CC_SAFE_RELEASE ( m_pDrawsLabel );
        pTextureCache->removeTextureForKey ( "cc_fps_images" );
        CCFileUtils::sharedFileUtils ( )->purgeCachedEntries ( );
    }

    CCTexture2DPixelFormat	eCurrentFormat = CCTexture2D::defaultAlphaPixelFormat ( );
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA4444 );

    KDubyte*		pData = NULL;
    KDuint			uDataLen = 0;
    getFPSImageData ( &pData, &uDataLen );

	CCImage*		pImage = CCImage::create ( );
    KDbool			bIsOK = pImage->initWithData ( pData, uDataLen );
    if ( !bIsOK )
	{
        CCLOGERROR ( "%s", "Fails: init fps_images" );
        return;
    }

    pTexture = pTextureCache->addCCImage ( pImage, "cc_fps_images" );

    //
    //	We want to use an image which is stored in the file named ccFPSImage.c 
    //	for any design resolutions and all resource resolutions. 
    // 
    //	To achieve this,
    // 
    //	Firstly, we need to ignore 'contentScaleFactor' in 'CCAtlasNode' and 'CCLabelAtlas'.
    //	So I added a new method called 'setIgnoreContentScaleFactor' for 'CCAtlasNode',
    //	this is not exposed to game developers, it's only used for displaying FPS now.
    // 
    //	Secondly, the size of this image is 480*320, to display the FPS label with correct size, 
    //	a factor of design resolution ratio of 480x320 is also needed.
    //
    KDfloat		fFactor = CCEGLView::sharedOpenGLView ( )->getDesignResolutionSize ( ).height / 320.0f;

    m_pFPSLabel = new CCLabelAtlas ( );
    m_pFPSLabel->setIgnoreContentScaleFactor ( KD_TRUE );
    m_pFPSLabel->initWithString ( "00.0", pTexture, 12, 32 , '.' );
    m_pFPSLabel->setScale ( fFactor );

    m_pSPFLabel = new CCLabelAtlas ( );
    m_pSPFLabel->setIgnoreContentScaleFactor ( KD_TRUE );
    m_pSPFLabel->initWithString ( "0.000", pTexture, 12, 32, '.' );
    m_pSPFLabel->setScale ( fFactor );

    m_pDrawsLabel = new CCLabelAtlas ( );
    m_pDrawsLabel->setIgnoreContentScaleFactor ( KD_TRUE );
    m_pDrawsLabel->initWithString("000", pTexture, 12, 32, '.' );
    m_pDrawsLabel->setScale ( fFactor );

    CCTexture2D::setDefaultAlphaPixelFormat ( eCurrentFormat );

    m_pDrawsLabel->setPosition ( ccpAdd ( ccp ( 0, 34 * fFactor ), CC_DIRECTOR_STATS_POSITION ) );
    m_pSPFLabel->setPosition ( ccpAdd ( ccp ( 0, 17 * fFactor ), CC_DIRECTOR_STATS_POSITION ) );
    m_pFPSLabel->setPosition ( CC_DIRECTOR_STATS_POSITION );
}

KDfloat CCDirector::getContentScaleFactor ( KDvoid )
{
	return m_fContentScaleFactor;
}

KDvoid CCDirector::setContentScaleFactor ( KDfloat fScaleFactor )
{
	if ( fScaleFactor != m_fContentScaleFactor )
	{
		m_fContentScaleFactor = fScaleFactor;
	}
}

CCNode* CCDirector::getNotificationNode ( KDvoid ) 
{ 
    return m_pNotificationNode; 
}

KDvoid CCDirector::setNotificationNode ( CCNode* pNode )
{
    CC_SAFE_RELEASE ( m_pNotificationNode );
    m_pNotificationNode = pNode;
    CC_SAFE_RETAIN ( m_pNotificationNode );
}

CCNode* CCDirector::getBackgroundNode ( KDvoid )
{
	return m_pBackNode;
}

KDvoid CCDirector::setBackgroundNode ( CCNode* pNode )
{
	if ( m_pBackNode )
	{
		m_pBackNode->onExitTransitionDidStart ( );
		m_pBackNode->onExit ( );
	}

    CC_SAFE_RELEASE ( m_pBackNode );
    m_pBackNode = pNode;
    CC_SAFE_RETAIN ( m_pBackNode );

	if ( m_pBackNode )
	{
		m_pBackNode->onEnter ( );
		m_pBackNode->onEnterTransitionDidFinish ( );
	}
}

CCDirectorDelegate* CCDirector::getDelegate ( KDvoid ) const
{
    return m_pProjectionDelegate;
}

KDvoid CCDirector::setDelegate ( CCDirectorDelegate* pDelegate )
{
    m_pProjectionDelegate = pDelegate;
}

KDvoid CCDirector::setScheduler ( CCScheduler* pScheduler )
{
    if ( m_pScheduler != pScheduler )
    {
        CC_SAFE_RETAIN  ( pScheduler );
        CC_SAFE_RELEASE ( m_pScheduler );
        m_pScheduler = pScheduler;
    }
}

CCScheduler* CCDirector::getScheduler ( KDvoid )
{
    return m_pScheduler;
}

KDvoid CCDirector::setActionManager ( CCActionManager* pActionManager )
{
    if ( m_pActionManager != pActionManager )
    {
        CC_SAFE_RETAIN  ( pActionManager );
        CC_SAFE_RELEASE ( m_pActionManager );
        m_pActionManager = pActionManager;
    }    
}

CCActionManager* CCDirector::getActionManager ( KDvoid )
{
    return m_pActionManager;
}

KDvoid CCDirector::setTouchDispatcher ( CCTouchDispatcher* pTouchDispatcher )
{
    if ( m_pTouchDispatcher != pTouchDispatcher )
    {
        CC_SAFE_RETAIN ( pTouchDispatcher );
        CC_SAFE_RELEASE ( m_pTouchDispatcher );
        m_pTouchDispatcher = pTouchDispatcher;
    }    
}

CCTouchDispatcher* CCDirector::getTouchDispatcher ( KDvoid )
{
    return m_pTouchDispatcher;
}

KDvoid CCDirector::setKeypadDispatcher ( CCKeypadDispatcher* pKeypadDispatcher )
{
    CC_SAFE_RETAIN ( pKeypadDispatcher );
    CC_SAFE_RELEASE ( m_pKeypadDispatcher );
    m_pKeypadDispatcher = pKeypadDispatcher;
}

CCKeypadDispatcher* CCDirector::getKeypadDispatcher ( KDvoid )
{
    return m_pKeypadDispatcher;
}

KDvoid CCDirector::setAccelerometer ( CCAccelerometer* pAccelerometer )
{
    if ( m_pAccelerometer != pAccelerometer )
    {
        CC_SAFE_DELETE ( m_pAccelerometer );
        m_pAccelerometer = pAccelerometer;
    }
}

CCAccelerometer* CCDirector::getAccelerometer ( KDvoid )
{
    return m_pAccelerometer;
}

KDvoid CCDirector::setEventDispatcher ( CCEventDispatcher* pEventDispatcher )
{
    if ( m_pEventDispatcher != pEventDispatcher )
    {       
		CC_SAFE_DELETE ( m_pEventDispatcher );
        m_pEventDispatcher = pEventDispatcher;
    }
}

CCEventDispatcher* CCDirector::getEventDispatcher ( KDvoid )
{
    return m_pEventDispatcher;
}

KDvoid CCDirector::startAnimation ( KDvoid )
{
	m_bInvalid = KD_FALSE;

	xmSetFrameInterval ( m_dAnimationInterval );
}

KDvoid CCDirector::stopAnimation ( KDvoid )
{
    m_bInvalid = KD_TRUE;
}

KDvoid CCDirector::setAnimationInterval ( double dValue )
{
    m_dAnimationInterval = dValue;
    if ( !m_bInvalid )
    {
        stopAnimation  ( );
        startAnimation ( );
    }    
}

NS_CC_END

