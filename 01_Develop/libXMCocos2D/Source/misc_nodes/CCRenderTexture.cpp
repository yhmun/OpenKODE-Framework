/* -----------------------------------------------------------------------------------
 *
 *      File            CCRenderTexture.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Jason Booth
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
#include "misc_nodes/CCRenderTexture.h"
#include "effects/CCGrid.h"
#include "support/ccUtils.h"
#include "support/CCConfiguration.h"
#include "support/CCDirector.h"
#include "support/CCFileUtils.h"
#include "support/CCImage.h"
#include "support/CCNotificationCenter.h"
#include "textures/CCTextureCache.h"
#include "kazmath/GL/matrix.h"
#include "support/CCEGLView.h"

NS_CC_BEGIN
    
//
// implementation CCRenderTexture
//
CCRenderTexture::CCRenderTexture ( KDvoid )
{
    m_pSprite				= KD_NULL;
    m_uFBO					= 0;
	m_uDepthRenderBufffer	= 0;
    m_nOldFBO				= 0;
    m_pTexture				= 0;
	m_pTextureCopy			= 0;
    m_pImage				= KD_NULL;
    m_ePixelFormat			= kCCTexture2DPixelFormat_RGBA8888;
	m_uClearFlags			= 0;
	m_tClearColor			= ccc4F ( 0, 0, 0, 0 );
	m_fClearDepth			= 0.0f;
	m_nClearStencil			= 0;
	m_bAutoDraw				= KD_FALSE;

#if CC_ENABLE_CACHE_TEXTURE_DATA
	// Listen this event to save render texture before come to background.
    // Then it can be restored after coming to foreground on Android.
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver 
	(
		this,
		callfuncO_selector ( CCRenderTexture::listenToBackground ),
		EVENT_COME_TO_BACKGROUND,
		KD_NULL
	);
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver 
	(
		this,
		callfuncO_selector ( CCRenderTexture::listenToBackground ),
		EVENT_COME_TO_FOREGROUND,	// this is misspelt
		KD_NULL
	);
#endif
}

CCRenderTexture::~CCRenderTexture ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite );
	CC_SAFE_RELEASE ( m_pTextureCopy );

    ccGLDeleteFramebuffers ( 1, &m_uFBO );

	if ( m_uDepthRenderBufffer )
    {
        ccGLDeleteRenderbuffers ( 1, &m_uDepthRenderBufffer );
    }

	CC_SAFE_DELETE ( m_pImage );
#if CC_ENABLE_CACHE_TEXTURE_DATA
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_BACKGROUND );
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_FOREGROUND );
#endif
}

KDvoid CCRenderTexture::listenToBackground ( CCObject* pObject )
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    
    CC_SAFE_DELETE ( m_pImage );
    
    // to get the rendered texture data
    m_pImage = newCCImage ( KD_FALSE );

    if ( m_pImage )
    {
        const CCSize&  tSize = m_pTexture->getContentSizeInPixels ( );
        VolatileTexture::addDataTexture ( m_pTexture, m_pImage->getData ( ), kCCTexture2DPixelFormat_RGBA8888, tSize );
    } 
    else
    {
        CCLOG ( "Cache rendertexture failed!" );
    }

#endif // CC_ENABLE_CACHE_TEXTURE_DATA
}

KDvoid CCRenderTexture::listenToForeground(cocos2d::CCObject *obj)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // -- regenerate frame buffer object and attach the texture
    glGetIntegerv ( CC_GL_FRAMEBUFFER_BINDING, &m_nOldFBO );
    
    ccGLGenFramebuffers ( 1, &m_uFBO );
    ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_uFBO );
    
    m_pTexture->setAliasTexParameters ( );
    
    if ( m_pTextureCopy )
    {
        m_pTextureCopy->setAliasTexParameters ( );
    }
    
    ccGLFramebufferTexture2D ( CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName ( ), 0 );
    ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_nOldFBO );
#endif
}

CCSprite* CCRenderTexture::getSprite ( KDvoid )
{
	return m_pSprite;
}

KDvoid CCRenderTexture::setSprite ( CCSprite* pVar )
{
	CC_SAFE_RELEASE ( m_pSprite );
	m_pSprite = pVar;
	CC_SAFE_RETAIN  ( m_pSprite );
}

KDuint CCRenderTexture::getClearFlags ( KDvoid ) const
{
    return m_uClearFlags;
}

KDvoid CCRenderTexture::setClearFlags ( KDuint uClearFlags )
{
    m_uClearFlags = uClearFlags;
}

const ccColor4F& CCRenderTexture::getClearColor ( KDvoid ) const
{
    return m_tClearColor;
}

KDvoid CCRenderTexture::setClearColor ( const ccColor4F& tClearColor )
{
    m_tClearColor = tClearColor;
}

KDfloat CCRenderTexture::getClearDepth ( KDvoid ) const
{
    return m_fClearDepth;
}

KDvoid CCRenderTexture::setClearDepth ( KDfloat fClearDepth )
{
    m_fClearDepth = fClearDepth;
}

KDint CCRenderTexture::getClearStencil ( KDvoid ) const
{
    return m_nClearStencil;
}

KDvoid CCRenderTexture::setClearStencil ( KDfloat fClearStencil )
{
    m_nClearStencil = (GLint) fClearStencil;
}

KDbool CCRenderTexture::isAutoDraw ( KDvoid ) const
{
    return m_bAutoDraw;
}

KDvoid CCRenderTexture::setAutoDraw ( KDbool bAutoDraw )
{
    m_bAutoDraw = bAutoDraw;
}

CCRenderTexture* CCRenderTexture::create ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat )
{
    CCRenderTexture*  pRet = new CCRenderTexture ( );

    if ( pRet && pRet->initWithWidthAndHeight ( uWidth, uHeight, eFormat ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCRenderTexture* CCRenderTexture::create ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat )
{
    CCRenderTexture*  pRet = new CCRenderTexture ( );

    if ( pRet && pRet->initWithWidthAndHeight ( uWidth, uHeight, eFormat, uDepthStencilFormat ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCRenderTexture* CCRenderTexture::create ( KDuint uWidth, KDuint uHeight )
{
	CCRenderTexture*  pRet = new CCRenderTexture ( );

    if ( pRet && pRet->initWithWidthAndHeight ( uWidth, uHeight, kCCTexture2DPixelFormat_RGBA8888, 0 ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRenderTexture::initWithWidthAndHeight ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat )
{
	return initWithWidthAndHeight ( uWidth, uHeight, eFormat, 0 );
}

KDbool CCRenderTexture::initWithWidthAndHeight ( KDuint uWidth, KDuint uHeight, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat )
{
	CCAssert ( eFormat != kCCTexture2DPixelFormat_A8, "only RGB and RGBA formats are valid for a render texture" );

    KDbool  bRet = KD_FALSE;

    do 
    {
		KDfloat  fScale = CC_CONTENT_SCALE_FACTOR ( );
		KDint    nSizeW = (KDint) ( uWidth  * fScale );
		KDint    nSizeH = (KDint) ( uHeight * fScale );
		KDuint   uPowW  = ccNextPOT ( nSizeW );
		KDuint   uPowH  = ccNextPOT ( nSizeH );

        glGetIntegerv ( CC_GL_FRAMEBUFFER_BINDING, &m_nOldFBO );

        m_ePixelFormat = eFormat;

        m_pTexture = new CCTexture2D ( );
        if ( m_pTexture )
		{
			m_pTexture->initWithData ( KD_NULL, (CCTexture2DPixelFormat) m_ePixelFormat, ccs ( nSizeW, nSizeH ) );
		}

		GLint  nOldRBO;
        glGetIntegerv ( CC_GL_RENDERBUFFER_BINDING, &nOldRBO );

        if ( CCConfiguration::sharedConfiguration ( )->checkForGLExtension ( "GL_QCOM" ) )
        {
            m_pTextureCopy = new CCTexture2D ( );
            if ( m_pTextureCopy )
            {
				m_pTextureCopy->initWithData ( KD_NULL, (CCTexture2DPixelFormat) m_ePixelFormat, ccs ( nSizeW, nSizeW ) );
			}
            else
            {
                break;
            }
        }

        // generate FBO
        ccGLGenFramebuffers ( 1, &m_uFBO );
        ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_uFBO );

        // associate texture with FBO
        ccGLFramebufferTexture2D ( CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName ( ), 0 );

		if ( uDepthStencilFormat != 0 ) 
        {
            //create and attach depth buffer
            ccGLGenRenderbuffers ( 1, &m_uDepthRenderBufffer );
            ccGLBindRenderbuffer ( CC_GL_RENDERBUFFER, m_uDepthRenderBufffer );
            ccGLRenderbufferStorage ( CC_GL_RENDERBUFFER, uDepthStencilFormat, uPowW, uPowH );
            ccGLFramebufferRenderbuffer ( CC_GL_FRAMEBUFFER, CC_GL_DEPTH_ATTACHMENT, CC_GL_RENDERBUFFER, m_uDepthRenderBufffer );

            // if depth format is the one with stencil part, bind same render buffer as stencil attachment
            if ( uDepthStencilFormat == CC_GL_DEPTH24_STENCIL8 )
			{
                ccGLFramebufferRenderbuffer ( CC_GL_FRAMEBUFFER, CC_GL_STENCIL_ATTACHMENT, CC_GL_RENDERBUFFER, m_uDepthRenderBufffer );
			}
        }

        // check if it worked (probably worth doing :) )
		GLuint  uStatus = ccGLCheckFramebufferStatus ( CC_GL_FRAMEBUFFER );
		if ( uStatus != CC_GL_FRAMEBUFFER_COMPLETE )
		{
			CCLOG ( "Render Texture : Could not attach texture to framebuffer" );
		}

        m_pTexture->setAliasTexParameters ( );

		// retained
        setSprite ( CCSprite::createWithTexture ( m_pTexture ) );

        m_pTexture->release ( );
        m_pSprite->setScaleY ( -1 );        

        ccBlendFunc  tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        m_pSprite->setBlendFunc ( tBlendFunc );

		ccGLBindRenderbuffer ( CC_GL_RENDERBUFFER, nOldRBO );
        ccGLBindFramebuffer  ( CC_GL_FRAMEBUFFER , m_nOldFBO );

		// Diabled by default.
        m_bAutoDraw = KD_FALSE;
        
        // add sprite for backward compatibility
        addChild ( m_pSprite );

        bRet = KD_TRUE;
    
    } while ( 0 );

    return bRet;
}

KDvoid CCRenderTexture::begin ( KDvoid )
{
	kmGLMatrixMode ( KM_GL_PROJECTION );
    kmGLPushMatrix ( );
	kmGLMatrixMode ( KM_GL_MODELVIEW );
	kmGLPushMatrix ( );

	CCDirector*  pDirector = CCDirector::sharedDirector ( );
    pDirector->setProjection ( pDirector->getProjection ( ) );

#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	kmMat4		tModifiedProjection;
	kmGLGetMatrix ( KM_GL_PROJECTION, &tModifiedProjection );
	kmMat4Multiply ( &tModifiedProjection, CCEGLView::sharedOpenGLView ( )->getReverseOrientationMatrix ( ), &tModifiedProjection );
	kmGLMatrixMode ( KM_GL_PROJECTION );
	kmGLLoadMatrix ( &tModifiedProjection );
	kmGLMatrixMode ( KM_GL_MODELVIEW );
#endif

	const CCSize&  tTexSize = m_pTexture->getContentSizeInPixels ( );

	// Calculate the adjustment ratios based on the old and new projections
	const CCSize&  tSize  = pDirector->getWinSizeInPixels ( );
          CCSize   tRatio = ccs ( tSize.cx / tTexSize.cx, tSize.cy / tTexSize.cy );

	// Adjust the orthographic propjection and viewport
	glViewport ( 0, 0, (GLsizei) tTexSize.cx, (GLsizei) tTexSize.cy );

    kmMat4  tOrthoMatrix;
    kmMat4OrthographicProjection ( &tOrthoMatrix, -1.0f / tRatio.cx, 1.0f / tRatio.cx, -1.0f / tRatio.cy, 1.0f / tRatio.cy, -1, 1 );
    kmGLMultMatrix ( &tOrthoMatrix );

	glGetIntegerv ( CC_GL_FRAMEBUFFER_BINDING, &m_nOldFBO );
	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_uFBO );

    //  Certain Qualcomm Andreno gpu's will retain data in memory after a frame buffer switch which corrupts the render to the texture. The solution is to clear the frame buffer before rendering to the texture. However, calling glClear has the unintended result of clearing the current texture. Create a temporary texture to overcome this. At the end of CCRenderTexture::begin(), switch the attached texture to the second one, call glClear, and then switch back to the original texture. This solution is unnecessary for other devices as they don't have the same issue with switching frame buffers.
    if ( CCConfiguration::sharedConfiguration ( )->checkForGLExtension ( "GL_QCOM" ) )
    {
        // -- bind a temporary texture so we can clear the render buffer without losing our texture
        ccGLFramebufferTexture2D ( CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureCopy->getName ( ), 0 );
        CHECK_GL_ERROR_DEBUG ( );
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        ccGLFramebufferTexture2D ( CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName ( ), 0 );
    }
}

KDvoid CCRenderTexture::beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha )
{
    beginWithClear ( fRed, fGreen, fBlue, fAlpha, 0, 0, GL_COLOR_BUFFER_BIT );
}

KDvoid CCRenderTexture::beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha, KDfloat fDepthValue )
{
    beginWithClear ( fRed, fGreen, fBlue, fAlpha, fDepthValue, 0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

KDvoid CCRenderTexture::beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha, KDfloat fDepthValue, KDint nStencilValue )
{
    beginWithClear ( fRed, fGreen, fBlue, fAlpha, fDepthValue, nStencilValue, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

KDvoid CCRenderTexture::beginWithClear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha, KDfloat fDepthValue, KDint nStencilValue, GLbitfield uFlags )
{
    this->begin ( );

    // save clear color
	GLfloat	 aClearColor [ 4 ]  = { 0.0f };
    GLfloat  fDepthClearValue   = 0.0f;
	KDint    nStencilClearValue = 0;

	if ( uFlags & GL_COLOR_BUFFER_BIT )
    {
        glGetFloatv  ( GL_COLOR_CLEAR_VALUE, aClearColor ); 
        glClearColor ( fRed, fGreen, fBlue, fAlpha );
    }

	if ( uFlags & GL_DEPTH_BUFFER_BIT )
	{
		glGetFloatv ( GL_DEPTH_CLEAR_VALUE, &fDepthClearValue );
		glClearDepth ( fDepthValue );
	}

	if ( uFlags & GL_STENCIL_BUFFER_BIT )
    {
        glGetIntegerv ( GL_STENCIL_CLEAR_VALUE, &nStencilClearValue );
        glClearStencil ( nStencilClearValue );
    }

    glClear ( uFlags );

    // restore
    if ( uFlags & GL_COLOR_BUFFER_BIT )
    {
        glClearColor ( aClearColor[0], aClearColor[1], aClearColor[2], aClearColor[3] );
    }
    
	if ( uFlags & GL_DEPTH_BUFFER_BIT )
    {
        glClearDepth ( fDepthClearValue );
    }
    
	if ( uFlags & GL_STENCIL_BUFFER_BIT )
    {
        glClearStencil ( nStencilClearValue );
    }
}

KDvoid CCRenderTexture::end ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );

	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_nOldFBO );

	// restore viewport
	pDirector->setViewport ( );

    kmGLMatrixMode ( KM_GL_PROJECTION );
	kmGLPopMatrix  ( );
	kmGLMatrixMode ( KM_GL_MODELVIEW );
	kmGLPopMatrix  ( );
}

KDvoid CCRenderTexture::clear ( KDfloat fRed, KDfloat fGreen, KDfloat fBlue, KDfloat fAlpha )
{
	this->beginWithClear ( fRed, fGreen, fBlue, fAlpha );
	this->end ( );
}

KDvoid CCRenderTexture::clearDepth ( KDfloat fDepthValue )
{
    this->begin ( );

    //! save old depth value
    GLfloat  fDepthClearValue;
    glGetFloatv ( GL_DEPTH_CLEAR_VALUE, &fDepthClearValue );

    glClearDepth ( fDepthValue );
    glClear ( GL_DEPTH_BUFFER_BIT );

    // restore clear color
    glClearDepth ( fDepthClearValue );

    this->end ( );
}

KDvoid CCRenderTexture::clearStencil ( KDint nStencilValue )
{
    // save old stencil value
    KDint  nStencilClearValue;
    glGetIntegerv ( GL_STENCIL_CLEAR_VALUE, &nStencilClearValue );

    glClearStencil ( nStencilValue );
    glClear ( GL_STENCIL_BUFFER_BIT );

    // restore clear color
    glClearStencil ( nStencilClearValue );
}

KDvoid CCRenderTexture::visit ( KDvoid )
{
    // override visit.
	// Don't call visit on its children
    if (!m_bVisible)
    {
        return;
    }
	
	kmGLPushMatrix ( );
	
    if ( m_pGrid && m_pGrid->isActive ( ) )
    {
        m_pGrid->beforeDraw ( );
        transformAncestors ( );
    }
    
    transform ( );
    m_pSprite->visit ( );

    draw ( );
	
    if ( m_pGrid && m_pGrid->isActive ( ) )
    {
        m_pGrid->afterDraw ( this );
    }
	
	kmGLPopMatrix ( );

    m_uOrderOfArrival = 0;
}

KDvoid CCRenderTexture::draw ( KDvoid )
{
    if ( m_bAutoDraw )
    {
        begin ( );
		
        if ( m_uClearFlags )
        {
            GLfloat  aOldClearColor[4] = { 0.0f };
			GLfloat  fOldDepthClearValue = 0.0f;
			GLint    nOldStencilClearValue = 0;
			
			// backup and set
			if ( m_uClearFlags & GL_COLOR_BUFFER_BIT )
            {
				glGetFloatv ( GL_COLOR_CLEAR_VALUE, aOldClearColor );
				glClearColor ( m_tClearColor.r, m_tClearColor.g, m_tClearColor.b, m_tClearColor.a );
			}
			
			if ( m_uClearFlags & GL_DEPTH_BUFFER_BIT )
            {
				glGetFloatv ( GL_DEPTH_CLEAR_VALUE, &fOldDepthClearValue );
				glClearDepth ( m_fClearDepth );
			}
			
			if ( m_uClearFlags & GL_STENCIL_BUFFER_BIT )
            {
				glGetIntegerv ( GL_STENCIL_CLEAR_VALUE, &nOldStencilClearValue );
				glClearStencil ( m_nClearStencil );
			}
			
			// clear
			glClear ( m_uClearFlags );
			
			// restore
			if ( m_uClearFlags & GL_COLOR_BUFFER_BIT )
            {
				glClearColor ( aOldClearColor[0], aOldClearColor[1], aOldClearColor[2], aOldClearColor[3] );
            }

			if ( m_uClearFlags & GL_DEPTH_BUFFER_BIT )
            {
				glClearDepth ( fOldDepthClearValue );
            }

			if ( m_uClearFlags & GL_STENCIL_BUFFER_BIT )
            {
				glClearStencil ( nOldStencilClearValue );
            }
		}
		
		//! make sure all children are drawn
        sortAllChildren ( );
		
		CCObject*  pElement;
		CCARRAY_FOREACH ( m_pChildren, pElement )
        {
            CCNode*  pChild = (CCNode*) pElement;

            if ( pChild != m_pSprite )
            {
                pChild->visit ( );
            }
		}
        
        end ( );
	}
}

KDbool CCRenderTexture::saveToFile ( const KDchar* szFilePath )
{
	KDbool    bRet   = KD_FALSE;
    CCImage*  pImage = newCCImage ( KD_TRUE );

    if ( pImage )
    {
		bRet = pImage->saveToFile ( CCFileUtils::sharedFileUtils ( )->fullPathFromWritablePath ( szFilePath ), KD_TRUE );
    }

    CC_SAFE_DELETE ( pImage );

    return bRet;
}

KDbool CCRenderTexture::saveToFile ( const KDchar* szFilePath, tCCImageFormat eFormat )
{
    CCAssert ( eFormat == kCCImageFormatJPEG || eFormat == kCCImageFormatPNG, "the image can only be saved as JPG or PNG format" );
	
	KDbool    bRet   = KD_FALSE;
    CCImage*  pImage = newCCImage ( KD_TRUE );

    if ( pImage )
    {
        bRet = pImage->saveToFile ( CCFileUtils::sharedFileUtils ( )->fullPathFromWritablePath ( szFilePath ), KD_TRUE );
    }

    CC_SAFE_DELETE ( pImage );

    return bRet;
}

/* get buffer as CCImage */
CCImage* CCRenderTexture::newCCImage ( KDbool bFlipImage )
{
    CCAssert ( m_ePixelFormat == kCCTexture2DPixelFormat_RGBA8888, "only RGBA8888 can be saved as image" );

    if ( KD_NULL == m_pTexture )
    {
        return KD_NULL;
    }

    const CCSize&  tSize = m_pTexture->getContentSizeInPixels ( );

    // to get the image size to save
    //        if the saving image domain exceeds the buffer texture domain,
    //        it should be cut
    KDint  nSavedBufferWidth  = (KDint) tSize.cx;
    KDint  nSavedBufferHeight = (KDint) tSize.cy; 

    GLubyte*  pBuffer   = KD_NULL;
    GLubyte*  pTempData = KD_NULL;
    CCImage*  pImage    = new CCImage ( );

    do
    {
		pBuffer = new GLubyte [ nSavedBufferWidth * nSavedBufferHeight * 4 ];

        CC_BREAK_IF ( !pBuffer );

		this->begin ( );

		glPixelStorei ( GL_PACK_ALIGNMENT  , 1 );
		glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		
        glReadPixels  ( 0, 0, nSavedBufferWidth, nSavedBufferHeight, GL_RGBA,GL_UNSIGNED_BYTE, pBuffer );

		this->end ( );

		if ( bFlipImage )
		{
			// to get the actual texture data
            // #640 the image read from rendertexture is dirty
			pTempData = new GLubyte [ nSavedBufferWidth * 4 ];

			for ( KDint i = 0; i < nSavedBufferHeight; i++ )
			{
				kdMemcpy ( pTempData, &pBuffer [ i * nSavedBufferWidth * 4 ], nSavedBufferWidth * 4 );
				kdMemcpy ( &pBuffer [ i * nSavedBufferWidth * 4 ], &pBuffer [ ( nSavedBufferHeight - i - 1 ) * nSavedBufferWidth * 4 ], nSavedBufferWidth * 4 );
				kdMemcpy ( &pBuffer [ ( nSavedBufferHeight - i - 1 ) * nSavedBufferWidth * 4 ], pTempData, nSavedBufferWidth * 4 );
			}
		}

		pImage->initWithRawData ( pBuffer, nSavedBufferWidth * nSavedBufferHeight * 4, nSavedBufferWidth, nSavedBufferHeight, 32 );

    } while ( 0 );

    CC_SAFE_DELETE_ARRAY ( pBuffer );
    CC_SAFE_DELETE_ARRAY ( pTempData );

    return pImage;
}

NS_CC_END
