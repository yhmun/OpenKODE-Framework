/* -----------------------------------------------------------------------------------
 *
 *      File            CCGLBufferedNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2013      Zynga Inc.
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

#include "2d/base_nodes/CCGLBufferedNode.h"

GLBufferedNode::GLBufferedNode()
{
    for(int i = 0; i < BUFFER_SLOTS; i++)
    {
        m_aBufferObject[i] = 0;
        m_aBufferSize[i] = 0;
        m_aIndexBufferObject[i] = 0;
        m_aIndexBufferSize[i] = 0;
    }
}

GLBufferedNode::~GLBufferedNode()
{
    for(int i = 0; i < BUFFER_SLOTS; i++)
    {
        if(m_aBufferSize[i])
        {
            glDeleteBuffers(1, &(m_aBufferObject[i]));
        }
        if(m_aIndexBufferSize[i])
        {
            glDeleteBuffers(1, &(m_aIndexBufferObject[i]));
        }
    }
}

void GLBufferedNode::setGLBufferData(void *buf, GLuint bufSize, int slot)
{
    // WebGL doesn't support client-side arrays, so generate a buffer and load the data first.
    if(m_aBufferSize[slot] < bufSize)
    {
        if(m_aBufferObject[slot])
        {
            glDeleteBuffers(1, &(m_aBufferObject[slot]));
        }
        glGenBuffers(1, &(m_aBufferObject[slot]));
        m_aBufferSize[slot] = bufSize;

        glBindBuffer(GL_ARRAY_BUFFER, m_aBufferObject[slot]);
        glBufferData(GL_ARRAY_BUFFER, bufSize, buf, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_aBufferObject[slot]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufSize, buf);
    }
}

void GLBufferedNode::setGLIndexData(void *buf, GLuint bufSize, int slot)
{
    // WebGL doesn't support client-side arrays, so generate a buffer and load the data first.
    if(m_aIndexBufferSize[slot] < bufSize)
    {
        if(m_aIndexBufferObject[slot])
        {
            glDeleteBuffers(1, &(m_aIndexBufferObject[slot]));
        }
        glGenBuffers(1, &(m_aIndexBufferObject[slot]));
        m_aIndexBufferSize[slot] = bufSize;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aIndexBufferObject[slot]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, buf, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aIndexBufferObject[slot]);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufSize, buf);
    }
}

