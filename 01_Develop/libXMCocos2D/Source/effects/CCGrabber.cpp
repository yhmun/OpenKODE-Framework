/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrabber.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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
#include "effects/CCGrabber.h"
#include "textures/CCTexture2D.h"
#include "shaders/ccGLStateCache.h"

NS_CC_BEGIN
    
CCGrabber::CCGrabber ( KDvoid )
{
    m_uFBO      = 0;
    m_nOldFBO   = 0;
    
	kdMemset ( m_aOldClearColor, 0, sizeof ( m_aOldClearColor ) );

	// generate FBO
	ccGLGenFramebuffers ( 1, &m_uFBO );
}

KDvoid CCGrabber::grab ( CCTexture2D* pTexture )
{
	glGetIntegerv ( CC_GL_FRAMEBUFFER_BINDING, &m_nOldFBO );

	// bind
	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_uFBO );

	// associate texture with FBO
	ccGLFramebufferTexture2D ( CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->getName ( ), 0 );

	// check if it worked (probably worth doing :) )
	GLuint  uStatus = ccGLCheckFramebufferStatus ( CC_GL_FRAMEBUFFER );

	if ( uStatus != CC_GL_FRAMEBUFFER_COMPLETE )
	{
		CCLOG ( "Frame Grabber: could not attach texture to framebuffer" );
	}

	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_nOldFBO );
}

KDvoid CCGrabber::beforeRender ( CCTexture2D* pTexture )
{
	CC_UNUSED_PARAM ( pTexture );

	glGetIntegerv ( CC_GL_FRAMEBUFFER_BINDING, &m_nOldFBO );
	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_uFBO );

	// save clear color
    glGetFloatv ( GL_COLOR_CLEAR_VALUE, m_aOldClearColor );
	// BUG XXX: doesn't work with RGB565.

	glClearColor ( 0, 0, 0, 0 );

	// BUG #631: To fix #631, uncomment the lines with #631
	// Warning: But it CCGrabber won't work with 2 effects at the same time
//	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );	// #631'
    
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//	glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE );	// #631
}

KDvoid CCGrabber::afterRender ( CCTexture2D* pTexture )
{
	CC_UNUSED_PARAM ( pTexture );

	ccGLBindFramebuffer ( CC_GL_FRAMEBUFFER, m_nOldFBO );
//	glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );		// #631

	// Restore clear color
    glClearColor ( m_aOldClearColor [0], m_aOldClearColor [1], m_aOldClearColor [2], m_aOldClearColor [3] );
}

CCGrabber::~CCGrabber ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCGrabber = %p", this );

	ccGLDeleteFramebuffers ( 1, &m_uFBO );
}

NS_CC_END