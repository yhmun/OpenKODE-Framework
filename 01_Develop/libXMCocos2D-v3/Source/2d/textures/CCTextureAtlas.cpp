/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureAtlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

// cocos2d
#include "2d/textures/CCTextureAtlas.h"
#include "2d/textures/CCTextureCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/CCNotificationCenter.h"
#include "CCEventType.h"
#include "2d/CCDirector.h"
#include "2d/CCConfiguration.h"
// support
#include "2d/textures/CCTexture2D.h"
#include "base/CCString.h"

//According to some tests GL_TRIANGLE_STRIP is slower, MUCH slower. Probably I'm doing something very wrong

// implementation TextureAtlas

NS_CC_BEGIN

TextureAtlas::TextureAtlas()
    :m_pIndices(NULL)
    ,m_bDirty(false)
    ,m_pTexture(NULL)
    ,m_pQuads(NULL)
{}

TextureAtlas::~TextureAtlas()
{
    CCLOGINFO("deallocing TextureAtlas: %p", this);

    CC_SAFE_FREE(m_pQuads);
    CC_SAFE_FREE(m_pIndices);

    glDeleteBuffers(2, m_aBuffersVBO);

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        glDeleteVertexArrays(1, &m_uVAOname);
        GL::bindVAO(0);
    }
    CC_SAFE_RELEASE(m_pTexture);
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    NotificationCenter::getInstance()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
}

KDint32 TextureAtlas::getTotalQuads() const
{
    return m_nTotalQuads;
}

KDint32 TextureAtlas::getCapacity() const
{
    return m_nCapacity;
}

Texture2D* TextureAtlas::getTexture() const
{
    return m_pTexture;
}

void TextureAtlas::setTexture(Texture2D * var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pTexture);
    m_pTexture = var;
}

V3F_C4B_T2F_Quad* TextureAtlas::getQuads()
{
    //if someone accesses the quads directly, presume that changes will be made
    m_bDirty = true;
    return m_pQuads;
}

void TextureAtlas::setQuads(V3F_C4B_T2F_Quad* quads)
{
    m_pQuads = quads;
}

// TextureAtlas - alloc & init

TextureAtlas * TextureAtlas::create(const char* file, KDint32 capacity)
{
    TextureAtlas * textureAtlas = new TextureAtlas();
    if(textureAtlas && textureAtlas->initWithFile(file, capacity))
    {
        textureAtlas->autorelease();
        return textureAtlas;
    }
    CC_SAFE_DELETE(textureAtlas);
    return NULL;
}

TextureAtlas * TextureAtlas::createWithTexture(Texture2D *texture, KDint32 capacity)
{
    TextureAtlas * textureAtlas = new TextureAtlas();
    if (textureAtlas && textureAtlas->initWithTexture(texture, capacity))
    {
        textureAtlas->autorelease();
        return textureAtlas;
    }
    CC_SAFE_DELETE(textureAtlas);
    return NULL;
}

bool TextureAtlas::initWithFile(const char * file, KDint32 capacity)
{
    // retained in property
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(file);

    if (texture)
    {   
        return initWithTexture(texture, capacity);
    }
    else
    {
        CCLOG("cocos2d: Could not open file: %s", file);
        return false;
    }
}

bool TextureAtlas::initWithTexture(Texture2D *texture, KDint32 capacity)
{
    CCASSERT(capacity>=0, "Capacity must be >= 0");
    
//    CCASSERT(texture != NULL, "texture should not be null");
    m_nCapacity = capacity;
    m_nTotalQuads = 0;

    // retained in property
    this->m_pTexture = texture;
    CC_SAFE_RETAIN(m_pTexture);

    // Re-initialization is not allowed
    CCASSERT(m_pQuads == NULL && m_pIndices == NULL, "");

    m_pQuads = (V3F_C4B_T2F_Quad*) kdMalloc( m_nCapacity * sizeof(V3F_C4B_T2F_Quad) );
    m_pIndices = (GLushort *) kdMalloc( m_nCapacity * 6 * sizeof(GLushort) );
    
    if( ! ( m_pQuads && m_pIndices) && m_nCapacity > 0) 
    {
        //CCLOG("cocos2d: TextureAtlas: not enough memory");
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);

        // release texture, should set it to null, because the destruction will
        // release it too. see cocos2d-x issue #484
        CC_SAFE_RELEASE(m_pTexture);
        return false;
    }

    kdMemset( m_pQuads, 0, m_nCapacity * sizeof(V3F_C4B_T2F_Quad) );
    kdMemset( m_pIndices, 0, m_nCapacity * 6 * sizeof(GLushort) );
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // listen the event when app go to background
    NotificationCenter::getInstance()->addObserver(this,
                                                           callfuncO_selector(TextureAtlas::listenBackToForeground),
                                                           EVENT_COME_TO_FOREGROUND,
                                                           NULL);
#endif
    
    this->setupIndices();

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        setupVBOandVAO();
    }
    else
    {
        setupVBO();
    }

    m_bDirty = true;

    return true;
}

void TextureAtlas::listenBackToForeground(Object *obj)
{  
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        setupVBOandVAO();
    }
    else
    {
        setupVBO();
    }
    
    // set _dirty to true to force it rebinding buffer
    m_bDirty = true;
}

const char* TextureAtlas::description() const
{
    return String::createWithFormat("<TextureAtlas | totalQuads = %d>", m_nTotalQuads)->getCString();
}


void TextureAtlas::setupIndices()
{
    if (m_nCapacity == 0)
        return;

    for( int i=0; i < m_nCapacity; i++)
    {
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+0;
        m_pIndices[i*6+2] = i*4+2;        
        m_pIndices[i*6+3] = i*4+1;
        m_pIndices[i*6+4] = i*4+3;
        m_pIndices[i*6+5] = i*4+3;
#else
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+1;
        m_pIndices[i*6+2] = i*4+2;

        // inverted index. issue #179
        m_pIndices[i*6+3] = i*4+3;
        m_pIndices[i*6+4] = i*4+2;
        m_pIndices[i*6+5] = i*4+1;        
#endif    
    }
}

//TextureAtlas - VAO / VBO specific

void TextureAtlas::setupVBOandVAO()
{
    glGenVertexArrays(1, &m_uVAOname);
    GL::bindVAO(m_uVAOname);

#define kQuadSize sizeof(m_pQuads[0].bl)

    glGenBuffers(2, &m_aBuffersVBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_nCapacity, m_pQuads, GL_DYNAMIC_DRAW);

    // vertices
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, vertices));

    // colors
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, colors));

    // tex coords
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_nCapacity * 6, m_pIndices, GL_STATIC_DRAW);

    // Must unbind the VAO before changing the element buffer.
    GL::bindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

void TextureAtlas::setupVBO()
{
    glGenBuffers(2, &m_aBuffersVBO[0]);

    mapBuffers();
}

void TextureAtlas::mapBuffers()
{
    // Avoid changing the element buffer for whatever VAO might be bound.
	GL::bindVAO(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_nCapacity, m_pQuads, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_nCapacity * 6, m_pIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

// TextureAtlas - Update, Insert, Move & Remove

void TextureAtlas::updateQuad(V3F_C4B_T2F_Quad *quad, KDint32 index)
{
    CCASSERT( index >= 0 && index < m_nCapacity, "updateQuadWithTexture: Invalid index");

    m_nTotalQuads = KD_MAX( index+1, m_nTotalQuads);

    m_pQuads[index] = *quad;    


    m_bDirty = true;

}

void TextureAtlas::insertQuad(V3F_C4B_T2F_Quad *quad, KDint32 index)
{
    CCASSERT( index>=0 && index<m_nCapacity, "insertQuadWithTexture: Invalid index");

    m_nTotalQuads++;
    CCASSERT( m_nTotalQuads <= m_nCapacity, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    unsigned int remaining = (m_nTotalQuads-1) - index;

    // last object doesn't need to be moved
    if( remaining > 0) 
    {
        // texture coordinates
        memmove( &m_pQuads[index+1],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );        
    }

    m_pQuads[index] = *quad;


    m_bDirty = true;

}

void TextureAtlas::insertQuads(V3F_C4B_T2F_Quad* quads, KDint32 index, KDint32 amount)
{
    CCASSERT(index>=0 && amount>=0 && index+amount<=m_nCapacity, "insertQuadWithTexture: Invalid index + amount");

    m_nTotalQuads += amount;

    CCASSERT( m_nTotalQuads <= m_nCapacity, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    long remaining = (m_nTotalQuads-1) - index - amount;

    // last object doesn't need to be moved
    if( remaining > 0)
    {
        // tex coordinates
        memmove( &m_pQuads[index+amount],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );
    }


    long max = index + amount;
    int j = 0;
    for (long i = index; i < max ; i++)
    {
        m_pQuads[index] = quads[j];
        index++;
        j++;
    }

    m_bDirty = true;
}

void TextureAtlas::insertQuadFromIndex(KDint32 oldIndex, KDint32 newIndex)
{
    CCASSERT( newIndex >= 0 && newIndex < m_nTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCASSERT( oldIndex >= 0 && oldIndex < m_nTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    // because it is ambiguous in iphone, so we implement abs ourselves
    // unsigned int howMany = abs( oldIndex - newIndex);
    long howMany = (oldIndex - newIndex) > 0 ? (oldIndex - newIndex) :  (newIndex - oldIndex);
    long dst = oldIndex;
    long src = oldIndex + 1;
    if( oldIndex > newIndex)
    {
        dst = newIndex+1;
        src = newIndex;
    }

    // texture coordinates
    V3F_C4B_T2F_Quad quadsBackup = m_pQuads[oldIndex];
    memmove( &m_pQuads[dst],&m_pQuads[src], sizeof(m_pQuads[0]) * howMany );
    m_pQuads[newIndex] = quadsBackup;


    m_bDirty = true;
}

void TextureAtlas::removeQuadAtIndex(KDint32 index)
{
    CCASSERT( index>=0 && index<m_nTotalQuads, "removeQuadAtIndex: Invalid index");

    long remaining = (m_nTotalQuads-1) - index;

    // last object doesn't need to be moved
    if( remaining ) 
    {
        // texture coordinates
        memmove( &m_pQuads[index],&m_pQuads[index+1], sizeof(m_pQuads[0]) * remaining );
    }

    m_nTotalQuads--;


    m_bDirty = true;
}

void TextureAtlas::removeQuadsAtIndex(KDint32 index, KDint32 amount)
{
    CCASSERT(index>=0 && amount>=0 && index+amount<=m_nTotalQuads, "removeQuadAtIndex: index + amount out of bounds");

    long remaining = (m_nTotalQuads) - (index + amount);

    m_nTotalQuads -= amount;

    if ( remaining )
    {
        memmove( &m_pQuads[index], &m_pQuads[index+amount], sizeof(m_pQuads[0]) * remaining );
    }

    m_bDirty = true;
}

void TextureAtlas::removeAllQuads()
{
    m_nTotalQuads = 0;
}

// TextureAtlas - Resize
bool TextureAtlas::resizeCapacity(KDint32 newCapacity)
{
    CCASSERT(newCapacity>=0, "capacity >= 0");
    if( newCapacity == m_nCapacity )
    {
        return true;
    }
    long oldCapactiy = m_nCapacity;
    // update capacity and totolQuads
    m_nTotalQuads = KD_MIN(m_nTotalQuads, newCapacity);
    m_nCapacity = newCapacity;

    V3F_C4B_T2F_Quad* tmpQuads = NULL;
    GLushort* tmpIndices = NULL;
    
    // when calling initWithTexture(fileName, 0) on bada device, calloc(0, 1) will fail and return NULL,
    // so here must judge whether _quads and _indices is NULL.
    if (m_pQuads == NULL)
    {
        tmpQuads = (V3F_C4B_T2F_Quad*) kdMalloc( m_nCapacity * sizeof(m_pQuads[0]) );
        if (tmpQuads != NULL)
        {
            kdMemset(tmpQuads, 0, m_nCapacity * sizeof(m_pQuads[0]) );
        }
    }
    else
    {
        tmpQuads = (V3F_C4B_T2F_Quad*) kdRealloc( m_pQuads, sizeof(m_pQuads[0]) * m_nCapacity );
        if (tmpQuads != NULL && m_nCapacity > oldCapactiy)
        {
            kdMemset(tmpQuads+oldCapactiy, 0, (m_nCapacity - oldCapactiy)*sizeof(m_pQuads[0]) );
        }
    }

    if (m_pIndices == NULL)
    {    
        tmpIndices = (GLushort*) kdMalloc( m_nCapacity * 6 * sizeof(m_pIndices[0]) );
        if (tmpIndices != NULL)
        {
            kdMemset( tmpIndices, 0, m_nCapacity * 6 * sizeof(m_pIndices[0]) );
        }
        
    }
    else
    {
        tmpIndices = (GLushort*) kdRealloc( m_pIndices, sizeof(m_pIndices[0]) * m_nCapacity * 6 );
        if (tmpIndices != NULL && m_nCapacity > oldCapactiy)
        {
            kdMemset( tmpIndices+oldCapactiy, 0, (m_nCapacity-oldCapactiy) * 6 * sizeof(m_pIndices[0]) );
        }
    }

    if( ! ( tmpQuads && tmpIndices) ) {
        CCLOG("cocos2d: TextureAtlas: not enough memory");
        CC_SAFE_FREE(tmpQuads);
        CC_SAFE_FREE(tmpIndices);
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);
        m_nCapacity = m_nTotalQuads = 0;
        return false;
    }

    m_pQuads = tmpQuads;
    m_pIndices = tmpIndices;


    setupIndices();
    mapBuffers();

    m_bDirty = true;

    return true;
}

void TextureAtlas::increaseTotalQuadsWith(KDint32 amount)
{
    CCASSERT(amount>=0, "amount >= 0");
    m_nTotalQuads += amount;
}

void TextureAtlas::moveQuadsFromIndex(KDint32 oldIndex, KDint32 amount, KDint32 newIndex)
{
    CCASSERT(oldIndex>=0 && amount>=0 && newIndex>=0, "values must be >= 0");
    CCASSERT(newIndex + amount <= m_nTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCASSERT(oldIndex < m_nTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    //create buffer
    size_t quadSize = sizeof(V3F_C4B_T2F_Quad);
    V3F_C4B_T2F_Quad* tempQuads = (V3F_C4B_T2F_Quad*) kdMalloc( quadSize * amount);
    memcpy( tempQuads, &m_pQuads[oldIndex], quadSize * amount );

    if (newIndex < oldIndex)
    {
        // move quads from newIndex to newIndex + amount to make room for buffer
        memmove( &m_pQuads[newIndex], &m_pQuads[newIndex+amount], (oldIndex-newIndex)*quadSize);
    }
    else
    {
        // move quads above back
        memmove( &m_pQuads[oldIndex], &m_pQuads[oldIndex+amount], (newIndex-oldIndex)*quadSize);
    }
    memcpy( &m_pQuads[newIndex], tempQuads, amount*quadSize);

    kdFree(tempQuads);

    m_bDirty = true;
}

void TextureAtlas::moveQuadsFromIndex(KDint32 index, KDint32 newIndex)
{
    CCASSERT(index>=0 && newIndex>=0, "values must be >= 0");
    CCASSERT(newIndex + (m_nTotalQuads - index) <= m_nCapacity, "moveQuadsFromIndex move is out of bounds");

    memmove(m_pQuads + newIndex,m_pQuads + index, (m_nTotalQuads - index) * sizeof(m_pQuads[0]));
}

void TextureAtlas::fillWithEmptyQuadsFromIndex(KDint32 index, KDint32 amount)
{
    CCASSERT(index>=0 && amount>=0, "values must be >= 0");
    V3F_C4B_T2F_Quad quad;
    kdMemset(&quad, 0, sizeof(quad));

    long to = index + amount;
    for (long i = index ; i < to ; i++)
    {
        m_pQuads[i] = quad;
    }
}

// TextureAtlas - Drawing

void TextureAtlas::drawQuads()
{
    this->drawNumberOfQuads(m_nTotalQuads, 0);
}

void TextureAtlas::drawNumberOfQuads(KDint32 numberOfQuads)
{
    CCASSERT(numberOfQuads>=0, "numberOfQuads must be >= 0");
    this->drawNumberOfQuads(numberOfQuads, 0);
}

void TextureAtlas::drawNumberOfQuads(KDint32 numberOfQuads, KDint32 start)
{
    CCASSERT(numberOfQuads>=0 && start>=0, "numberOfQuads and start must be >= 0");

    if(!numberOfQuads)
        return;

    GL::bindTexture2D(m_pTexture->getName());

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        //
        // Using VBO and VAO
        //

        // XXX: update is done in draw... perhaps it should be done in a timer
        if (m_bDirty) 
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
            // option 1: subdata
            //glBufferSubData(GL_ARRAY_BUFFER, sizeof(_quads[0])*start, sizeof(_quads[0]) * n , &_quads[start] );
            
            // option 2: data
            //      glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * (n-start), &quads_[start], GL_DYNAMIC_DRAW);
            
            // option 3: orphaning + glMapBuffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * (numberOfQuads-start), NULL, GL_DYNAMIC_DRAW);
            void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            memcpy(buf, m_pQuads, sizeof(m_pQuads[0])* (numberOfQuads-start));
            glUnmapBuffer(GL_ARRAY_BUFFER);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            m_bDirty = false;
        }

        GL::bindVAO(m_uVAOname);

#if CC_REBIND_INDICES_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
#endif

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) numberOfQuads*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#else
        glDrawElements(GL_TRIANGLES, (GLsizei) numberOfQuads*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP

#if CC_REBIND_INDICES_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

//    glBindVertexArray(0);
    }
    else
    {
        //
        // Using VBO without VAO
        //

#define kQuadSize sizeof(m_pQuads[0].bl)
        glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);

        // XXX: update is done in draw... perhaps it should be done in a timer
        if (m_bDirty) 
        {
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*start, sizeof(m_pQuads[0]) * numberOfQuads , &m_pQuads[start] );
            m_bDirty = false;
        }

        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        // vertices
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, vertices));

        // colors
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, colors));

        // tex coords
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, texCoords));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)numberOfQuads*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#else
        glDrawElements(GL_TRIANGLES, (GLsizei)numberOfQuads*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
}


NS_CC_END

