/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystemQuad.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada 
 *      Copyright (c) 2009      Leonardo Kasperavičius
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

#include "2d/particle_nodes/CCParticleSystemQuad.h"
#include "2d/sprite_nodes/CCSpriteFrame.h"
#include "2d/CCDirector.h"
#include "2d/particle_nodes/CCParticleBatchNode.h"
#include "2d/textures/CCTextureAtlas.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "support/TransformUtils.h"
#include "support/CCNotificationCenter.h"
#include "CCEventType.h"
#include "2d/CCConfiguration.h"

// extern
#include "XMKazmath/GL/matrix.h"

NS_CC_BEGIN

//implementation ParticleSystemQuad
// overriding the init method
bool ParticleSystemQuad::initWithTotalParticles(unsigned int numberOfParticles)
{
    // base initialization
    if( ParticleSystem::initWithTotalParticles(numberOfParticles) ) 
    {
        // allocating data space
        if( ! this->allocMemory() ) {
            this->release();
            return false;
        }

        initIndices();
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            setupVBOandVAO();
        }
        else
        {
            setupVBO();
        }

        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        
#if CC_ENABLE_CACHE_TEXTURE_DATA
        // Need to listen the event only when not use batchnode, because it will use VBO
        NotificationCenter::getInstance()->addObserver(this,
                                                                      callfuncO_selector(ParticleSystemQuad::listenBackToForeground),
                                                                      EVENT_COME_TO_FOREGROUND,
                                                                      NULL);
#endif

        return true;
    }
    return false;
}

ParticleSystemQuad::ParticleSystemQuad()
:m_pQuads(NULL)
,m_pIndices(NULL)
,m_uVAOname(0)
{
    kdMemset(m_aBuffersVBO, 0, sizeof(m_aBuffersVBO));
}

ParticleSystemQuad::~ParticleSystemQuad()
{
    if (NULL == m_pBatchNode)
    {
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);
        glDeleteBuffers(2, &m_aBuffersVBO[0]);
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            glDeleteVertexArrays(1, &m_uVAOname);
            GL::bindVAO(0);
        }
    }
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    NotificationCenter::getInstance()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
}

// implementation ParticleSystemQuad

ParticleSystemQuad * ParticleSystemQuad::create(const char *plistFile)
{
    ParticleSystemQuad *pRet = new ParticleSystemQuad();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

ParticleSystemQuad * ParticleSystemQuad::createWithTotalParticles(unsigned int numberOfParticles) {
    ParticleSystemQuad *pRet = new ParticleSystemQuad();
    if (pRet && pRet->initWithTotalParticles(numberOfParticles))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}


// pointRect should be in Texture coordinates, not pixel coordinates
void ParticleSystemQuad::initTexCoordsWithRect(const Rect& pointRect)
{
    // convert to Tex coords

    Rect rect = Rect(
        pointRect.origin.x * CC_CONTENT_SCALE_FACTOR(),
        pointRect.origin.y * CC_CONTENT_SCALE_FACTOR(),
        pointRect.size.width * CC_CONTENT_SCALE_FACTOR(),
        pointRect.size.height * CC_CONTENT_SCALE_FACTOR());

    GLfloat wide = (GLfloat) pointRect.size.width;
    GLfloat high = (GLfloat) pointRect.size.height;

    if (m_pTexture)
    {
        wide = (GLfloat)m_pTexture->getPixelsWide();
        high = (GLfloat)m_pTexture->getPixelsHigh();
    }

#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
    GLfloat left = (rect.origin.x*2+1) / (wide*2);
    GLfloat bottom = (rect.origin.y*2+1) / (high*2);
    GLfloat right = left + (rect.size.width*2-2) / (wide*2);
    GLfloat top = bottom + (rect.size.height*2-2) / (high*2);
#else
    GLfloat left = rect.origin.x / wide;
    GLfloat bottom = rect.origin.y / high;
    GLfloat right = left + rect.size.width / wide;
    GLfloat top = bottom + rect.size.height / high;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

    // Important. Texture in cocos2d are inverted, so the Y component should be inverted
    CC_SWAP( top, bottom, float);

    V3F_C4B_T2F_Quad *quads = NULL;
    unsigned int start = 0, end = 0;
    if (m_pBatchNode)
    {
        quads = m_pBatchNode->getTextureAtlas()->getQuads();
        start = m_nAtlasIndex;
        end = m_nAtlasIndex + m_nTotalParticles;
    }
    else
    {
        quads = m_pQuads;
        start = 0;
        end = m_nTotalParticles;
    }

    for(unsigned int i=start; i<end; i++) 
    {
        // bottom-left vertex:
        quads[i].bl.texCoords.u = left;
        quads[i].bl.texCoords.v = bottom;
        // bottom-right vertex:
        quads[i].br.texCoords.u = right;
        quads[i].br.texCoords.v = bottom;
        // top-left vertex:
        quads[i].tl.texCoords.u = left;
        quads[i].tl.texCoords.v = top;
        // top-right vertex:
        quads[i].tr.texCoords.u = right;
        quads[i].tr.texCoords.v = top;
    }
}
void ParticleSystemQuad::setTextureWithRect(Texture2D *texture, const Rect& rect)
{
    // Only update the texture if is different from the current one
    if( !m_pTexture || texture->getName() != m_pTexture->getName() )
    {
        ParticleSystem::setTexture(texture);
    }

    this->initTexCoordsWithRect(rect);
}
void ParticleSystemQuad::setTexture(Texture2D* texture)
{
    const Size& s = texture->getContentSize();
    this->setTextureWithRect(texture, Rect(0, 0, s.width, s.height));
}
void ParticleSystemQuad::setDisplayFrame(SpriteFrame *spriteFrame)
{
    CCASSERT(spriteFrame->getOffsetInPixels().equals(Point::ZERO), 
             "QuadParticle only supports SpriteFrames with no offsets");

    // update texture before updating texture rect
    if ( !m_pTexture || spriteFrame->getTexture()->getName() != m_pTexture->getName())
    {
        this->setTexture(spriteFrame->getTexture());
    }
}

void ParticleSystemQuad::initIndices()
{
    for(int i = 0; i < m_nTotalParticles; ++i)
    {
        const unsigned int i6 = i*6;
        const unsigned int i4 = i*4;
        m_pIndices[i6+0] = (GLushort) i4+0;
        m_pIndices[i6+1] = (GLushort) i4+1;
        m_pIndices[i6+2] = (GLushort) i4+2;

        m_pIndices[i6+5] = (GLushort) i4+1;
        m_pIndices[i6+4] = (GLushort) i4+2;
        m_pIndices[i6+3] = (GLushort) i4+3;
    }
}

void ParticleSystemQuad::updateQuadWithParticle(tParticle* particle, const Point& newPosition)
{
    V3F_C4B_T2F_Quad *quad;

    if (m_pBatchNode)
    {
        V3F_C4B_T2F_Quad *batchQuads = m_pBatchNode->getTextureAtlas()->getQuads();
        quad = &(batchQuads[m_nAtlasIndex+particle->atlasIndex]);
    }
    else
    {
        quad = &(m_pQuads[m_nParticleIdx]);
    }
    Color4B color = (m_bOpacityModifyRGB)
        ? Color4B( particle->color.r*particle->color.a*255, particle->color.g*particle->color.a*255, particle->color.b*particle->color.a*255, particle->color.a*255)
        : Color4B( particle->color.r*255, particle->color.g*255, particle->color.b*255, particle->color.a*255);

    quad->bl.colors = color;
    quad->br.colors = color;
    quad->tl.colors = color;
    quad->tr.colors = color;

    // vertices
    GLfloat size_2 = particle->size/2;
    if (particle->rotation) 
    {
        GLfloat x1 = -size_2;
        GLfloat y1 = -size_2;

        GLfloat x2 = size_2;
        GLfloat y2 = size_2;
        GLfloat x = newPosition.x;
        GLfloat y = newPosition.y;

        GLfloat r = (GLfloat)-CC_DEGREES_TO_RADIANS(particle->rotation);
        GLfloat cr = kdCosf(r);
        GLfloat sr = kdSinf(r);
        GLfloat ax = x1 * cr - y1 * sr + x;
        GLfloat ay = x1 * sr + y1 * cr + y;
        GLfloat bx = x2 * cr - y1 * sr + x;
        GLfloat by = x2 * sr + y1 * cr + y;
        GLfloat cx = x2 * cr - y2 * sr + x;
        GLfloat cy = x2 * sr + y2 * cr + y;
        GLfloat dx = x1 * cr - y2 * sr + x;
        GLfloat dy = x1 * sr + y2 * cr + y;

        // bottom-left
        quad->bl.vertices.x = ax;
        quad->bl.vertices.y = ay;

        // bottom-right vertex:
        quad->br.vertices.x = bx;
        quad->br.vertices.y = by;

        // top-left vertex:
        quad->tl.vertices.x = dx;
        quad->tl.vertices.y = dy;

        // top-right vertex:
        quad->tr.vertices.x = cx;
        quad->tr.vertices.y = cy;
    } 
    else 
    {
        // bottom-left vertex:
        quad->bl.vertices.x = newPosition.x - size_2;
        quad->bl.vertices.y = newPosition.y - size_2;

        // bottom-right vertex:
        quad->br.vertices.x = newPosition.x + size_2;
        quad->br.vertices.y = newPosition.y - size_2;

        // top-left vertex:
        quad->tl.vertices.x = newPosition.x - size_2;
        quad->tl.vertices.y = newPosition.y + size_2;

        // top-right vertex:
        quad->tr.vertices.x = newPosition.x + size_2;
        quad->tr.vertices.y = newPosition.y + size_2;                
    }
}
void ParticleSystemQuad::postStep()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
    
    // Option 1: Sub Data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pQuads[0])*m_nTotalParticles, m_pQuads);
    
    // Option 2: Data
    //  glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * particleCount, quads_, GL_DYNAMIC_DRAW);
    
    // Option 3: Orphaning + glMapBuffer
    // glBufferData(GL_ARRAY_BUFFER, sizeof(_quads[0])*_totalParticles, NULL, GL_STREAM_DRAW);
    // void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    // memcpy(buf, _quads, sizeof(_quads[0])*_totalParticles);
    // glUnmapBuffer(GL_ARRAY_BUFFER);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
}

// overriding draw method
void ParticleSystemQuad::draw()
{    
    CCASSERT(!m_pBatchNode,"draw should not be called when added to a particleBatchNode");

    CC_NODE_DRAW_SETUP();

    GL::bindTexture2D( m_pTexture->getName() );
    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

	KD_CLAMP ( 0, m_nParticleCount - 1, m_nParticleIdx );
//	CCAssert ( m_nParticleIdx == m_nParticleCount, "Abnormal error in particle quad");

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        //
        // Using VBO and VAO
        //
        GL::bindVAO(m_uVAOname);

#if CC_REBIND_INDICES_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
#endif

        glDrawElements(GL_TRIANGLES, (GLsizei) m_nParticleIdx*6, GL_UNSIGNED_SHORT, 0);

#if CC_REBIND_INDICES_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    }
    else
    {
        //
        // Using VBO without VAO
        //

        #define kQuadSize sizeof(m_pQuads[0].bl)

        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );

        glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
        // vertices
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, vertices));
        // colors
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, colors));
        // tex coords
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, texCoords));
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);

        glDrawElements(GL_TRIANGLES, (GLsizei) m_nParticleIdx*6, GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
}

void ParticleSystemQuad::setTotalParticles(int tp)
{
    // If we are setting the total number of particles to a number higher
    // than what is allocated, we need to allocate new arrays
    if( tp > m_nAllocatedParticles )
    {
        // Allocate new memory
        size_t particlesSize = tp * sizeof(tParticle);
        size_t quadsSize = sizeof(m_pQuads[0]) * tp * 1;
        size_t indicesSize = sizeof(m_pIndices[0]) * tp * 6 * 1;

        tParticle* particlesNew = (tParticle*) kdRealloc(m_pParticles, particlesSize);
        V3F_C4B_T2F_Quad* quadsNew = (V3F_C4B_T2F_Quad*) kdRealloc(m_pQuads, quadsSize);
        GLushort* indicesNew = (GLushort*) kdRealloc(m_pIndices, indicesSize);

        if (particlesNew && quadsNew && indicesNew)
        {
            // Assign pointers
            m_pParticles = particlesNew;
            m_pQuads = quadsNew;
            m_pIndices = indicesNew;

            // Clear the memory
            // XXX: Bug? If the quads are cleared, then drawing doesn't work... WHY??? XXX
            kdMemset(m_pParticles, 0, particlesSize);
            kdMemset(m_pQuads, 0, quadsSize);
            kdMemset(m_pIndices, 0, indicesSize);

            m_nAllocatedParticles = tp;
        }
        else
        {
            // Out of memory, failed to resize some array
            if (particlesNew) m_pParticles = particlesNew;
            if (quadsNew) m_pQuads = quadsNew;
            if (indicesNew) m_pIndices = indicesNew;

            CCLOG("Particle system: out of memory");
            return;
        }

        m_nTotalParticles = tp;

        // Init particles
        if (m_pBatchNode)
        {
            for (int i = 0; i < m_nTotalParticles; i++)
            {
                m_pParticles[i].atlasIndex=i;
            }
        }

        initIndices();
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            setupVBOandVAO();
        }
        else
        {
            setupVBO();
        }
    }
    else
    {
        m_nTotalParticles = tp;
    }
    
    resetSystem();
}

void ParticleSystemQuad::setupVBOandVAO()
{
    // clean VAO
    glDeleteBuffers(2, &m_aBuffersVBO[0]);
    glDeleteVertexArrays(1, &m_uVAOname);
    GL::bindVAO(0);
    
    glGenVertexArrays(1, &m_uVAOname);
    GL::bindVAO(m_uVAOname);

	#define kQuadSize sizeof(m_pQuads[0].bl)

    glGenBuffers(2, &m_aBuffersVBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_nTotalParticles, m_pQuads, GL_DYNAMIC_DRAW);

    // vertices
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, vertices));

    // colors
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, colors));

    // tex coords
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( V3F_C4B_T2F, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_nTotalParticles * 6, m_pIndices, GL_STATIC_DRAW);

    // Must unbind the VAO before changing the element buffer.
    GL::bindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

void ParticleSystemQuad::setupVBO()
{
    glDeleteBuffers(2, &m_aBuffersVBO[0]);
    
    glGenBuffers(2, &m_aBuffersVBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_aBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_nTotalParticles, m_pQuads, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_aBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_nTotalParticles * 6, m_pIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

void ParticleSystemQuad::listenBackToForeground(Object *obj)
{
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        setupVBOandVAO();
    }
    else
    {
        setupVBO();
    }
}

bool ParticleSystemQuad::allocMemory()
{
    CCASSERT( ( !m_pQuads && !m_pIndices), "Memory already alloced");
    CCASSERT( !m_pBatchNode, "Memory should not be alloced when not using batchNode");

    CC_SAFE_FREE(m_pQuads);
    CC_SAFE_FREE(m_pIndices);

    m_pQuads = (V3F_C4B_T2F_Quad*) kdMalloc(m_nTotalParticles * sizeof(V3F_C4B_T2F_Quad));
    m_pIndices = (GLushort*) kdMalloc(m_nTotalParticles * 6 * sizeof(GLushort));
    
    if( !m_pQuads || !m_pIndices) 
    {
        CCLOG("cocos2d: Particle system: not enough memory");
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);

        return false;
    }

    kdMemset(m_pQuads, 0, m_nTotalParticles * sizeof(V3F_C4B_T2F_Quad));
    kdMemset(m_pIndices, 0, m_nTotalParticles * 6 * sizeof(GLushort));

    return true;
}

void ParticleSystemQuad::setBatchNode(ParticleBatchNode * batchNode)
{
    if( m_pBatchNode != batchNode ) 
    {
        ParticleBatchNode* oldBatch = m_pBatchNode;

        ParticleSystem::setBatchNode(batchNode);

        // NEW: is self render ?
        if( ! batchNode ) 
        {
            allocMemory();
            initIndices();
            setTexture(oldBatch->getTexture());
            if (Configuration::getInstance()->supportsShareableVAO())
            {
                setupVBOandVAO();
            }
            else
            {
                setupVBO();
            }
        }
        // OLD: was it self render ? cleanup
        else if( !oldBatch )
        {
            // copy current state to batch
            V3F_C4B_T2F_Quad *batchQuads = m_pBatchNode->getTextureAtlas()->getQuads();
            V3F_C4B_T2F_Quad *quad = &(batchQuads[m_nAtlasIndex] );
            memcpy( quad, m_pQuads, m_nTotalParticles * sizeof(m_pQuads[0]) );

            CC_SAFE_FREE(m_pQuads);
            CC_SAFE_FREE(m_pIndices);

            glDeleteBuffers(2, &m_aBuffersVBO[0]);
            kdMemset(m_aBuffersVBO, 0, sizeof(m_aBuffersVBO));
            if (Configuration::getInstance()->supportsShareableVAO())
            {
                glDeleteVertexArrays(1, &m_uVAOname);
                GL::bindVAO(0);
                m_uVAOname = 0;
            }
        }
    }
}

ParticleSystemQuad * ParticleSystemQuad::create() {
    ParticleSystemQuad *pParticleSystemQuad = new ParticleSystemQuad();
    if (pParticleSystemQuad && pParticleSystemQuad->init())
    {
        pParticleSystemQuad->autorelease();
        return pParticleSystemQuad;
    }
    CC_SAFE_DELETE(pParticleSystemQuad);
    return NULL;
}

NS_CC_END
