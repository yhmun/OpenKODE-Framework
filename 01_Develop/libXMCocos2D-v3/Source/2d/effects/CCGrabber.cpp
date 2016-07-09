/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrabber.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/effects/CCGrabber.h"
#include "2d/textures/CCTexture2D.h"

NS_CC_BEGIN

Grabber::Grabber(void)
    : m_uFBO(0)
    , m_nOldFBO(0)
{
    kdMemset(m_aOldClearColor, 0, sizeof(m_aOldClearColor));

    // generate FBO
    glGenFramebuffers(1, &m_uFBO);
}

void Grabber::grab(Texture2D *texture)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);

    // bind
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

    // associate texture with FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getName(), 0);

    // check if it worked (probably worth doing :) )
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        CCASSERT(0, "Frame Grabber: could not attach texture to framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
}

void Grabber::beforeRender(Texture2D *texture)
{
    CC_UNUSED_PARAM(texture);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    
    // save clear color
    glGetFloatv(GL_COLOR_CLEAR_VALUE, m_aOldClearColor);
    // BUG XXX: doesn't work with RGB565.

    glClearColor(0, 0, 0, 0);

    // BUG #631: To fix #631, uncomment the lines with #631
    // Warning: But it Grabber won't work with 2 effects at the same time
//  glClearColor(0.0f,0.0f,0.0f,1.0f);    // #631

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  glColorMask(true, true, true, false);    // #631
}

void Grabber::afterRender(cocos2d::Texture2D *texture)
{
    CC_UNUSED_PARAM(texture);

    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
//  glColorMask(true, true, true, true);    // #631
    
    // Restore clear color
    glClearColor(m_aOldClearColor[0], m_aOldClearColor[1], m_aOldClearColor[2], m_aOldClearColor[3]);
}

Grabber::~Grabber()
{
    CCLOGINFO("deallocing Grabber: %p", this);
    glDeleteFramebuffers(1, &m_uFBO);
}

NS_CC_END
