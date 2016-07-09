/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrid.cpp
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

#include "2d/effects/CCGrid.h"
#include "2d/CCDirector.h"
#include "2d/effects/CCGrabber.h"
#include "support/ccUtils.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "support/TransformUtils.h"
#include "XMKazmath/kazmath.h"
#include "XMKazmath/GL/matrix.h"

NS_CC_BEGIN
// implementation of GridBase

GridBase* GridBase::create(const Size& gridSize)
{
    GridBase *pGridBase = new GridBase();

    if (pGridBase)
    {
        if (pGridBase->initWithSize(gridSize))
        {
            pGridBase->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE(pGridBase);
        }
    }

    return pGridBase;
}

GridBase* GridBase::create(const Size& gridSize, Texture2D *texture, bool flipped)
{
    GridBase *pGridBase = new GridBase();

    if (pGridBase)
    {
        if (pGridBase->initWithSize(gridSize, texture, flipped))
        {
            pGridBase->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE(pGridBase);
        }
    }

    return pGridBase;
}

bool GridBase::initWithSize(const Size& gridSize, Texture2D *texture, bool bFlipped)
{
    bool bRet = true;

    m_bActive = false;
    m_nReuseGrid = 0;
    m_tGridSize = gridSize;

    m_pTexture = texture;
    CC_SAFE_RETAIN(m_pTexture);
    m_bIsTextureFlipped = bFlipped;

    Size texSize = m_pTexture->getContentSize();
    m_tStep.x = texSize.width / m_tGridSize.width;
    m_tStep.y = texSize.height / m_tGridSize.height;

    m_pGrabber = new Grabber();
    if (m_pGrabber)
    {
        m_pGrabber->grab(m_pTexture);
    }
    else
    {
        bRet = false;
    }
    
    m_pShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE);
    calculateVertexPoints();

    return bRet;
}

bool GridBase::initWithSize(const Size& gridSize)
{
    Director *pDirector = Director::getInstance();
    Size s = pDirector->getWinSizeInPixels();
    
    unsigned long POTWide = ccNextPOT((unsigned int)s.width);
    unsigned long POTHigh = ccNextPOT((unsigned int)s.height);

    // we only use rgba8888
    Texture2D::PixelFormat format = Texture2D::PixelFormat::RGBA8888;

    long dataLen = POTWide * POTHigh * 4;
    void *data = kdCalloc(dataLen, 1);
    if (! data)
    {
        CCLOG("cocos2d: Grid: not enough memory.");
        this->release();
        return false;
    }

    Texture2D *texture = new Texture2D();
    texture->initWithData(data, dataLen,  format, POTWide, POTHigh, s);

    kdFree(data);

    if (! texture)
    {
        CCLOG("cocos2d: Grid: error creating texture");
        return false;
    }

    initWithSize(gridSize, texture, false);

    texture->release();

    return true;
}

GridBase::~GridBase(void)
{
    CCLOGINFO("deallocing GridBase: %p", this);

//TODO: ? why 2.0 comments this line        setActive(false);
    CC_SAFE_RELEASE(m_pTexture);
    CC_SAFE_RELEASE(m_pGrabber);
}

// properties
void GridBase::setActive(bool bActive)
{
    m_bActive = bActive;
    if (! bActive)
    {
        Director *pDirector = Director::getInstance();
        Director::Projection proj = pDirector->getProjection();
        pDirector->setProjection(proj);
    }
}

void GridBase::setTextureFlipped(bool bFlipped)
{
    if (m_bIsTextureFlipped != bFlipped)
    {
        m_bIsTextureFlipped = bFlipped;
        calculateVertexPoints();
    }
}

void GridBase::set2DProjection()
{
    Director *director = Director::getInstance();

    Size    size = director->getWinSizeInPixels();

    glViewport(0, 0, (GLsizei)(size.width), (GLsizei)(size.height) );
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();

    kmMat4 orthoMatrix;
    kmMat4OrthographicProjection(&orthoMatrix, 0, size.width, 0, size.height, -1, 1);
    kmGLMultMatrix( &orthoMatrix );

    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();

    GL::setProjectionMatrixDirty();
}

void GridBase::beforeDraw(void)
{
    // save projection
    Director *director = Director::getInstance();
    m_eDirectorProjection = director->getProjection();

    // 2d projection
    //    [director setProjection:Director::Projection::_2D];
    set2DProjection();
    m_pGrabber->beforeRender(m_pTexture);
}

void GridBase::afterDraw(cocos2d::Node *target)
{
    m_pGrabber->afterRender(m_pTexture);

    // restore projection
    Director *director = Director::getInstance();
    director->setProjection(m_eDirectorProjection);

    if (target->getCamera()->isDirty())
    {
        Point offset = target->getAnchorPointInPoints();

        //
        // XXX: Camera should be applied in the AnchorPoint
        //
        kmGLTranslatef(offset.x, offset.y, 0);
        target->getCamera()->locate();
        kmGLTranslatef(-offset.x, -offset.y, 0);
    }

    GL::bindTexture2D(m_pTexture->getName());

    // restore projection for default FBO .fixed bug #543 #544
//TODO:         Director::getInstance()->setProjection(Director::getInstance()->getProjection());
//TODO:         Director::getInstance()->applyOrientation();
    blit();
}

void GridBase::blit(void)
{
    CCASSERT(0, "");
}

void GridBase::reuse(void)
{
    CCASSERT(0, "");
}

void GridBase::calculateVertexPoints(void)
{
    CCASSERT(0, "");
}

// implementation of Grid3D

Grid3D* Grid3D::create(const Size& gridSize, Texture2D *texture, bool bFlipped)
{
    Grid3D *pRet= new Grid3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize, texture, bFlipped))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}

Grid3D* Grid3D::create(const Size& gridSize)
{
    Grid3D *pRet= new Grid3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}


Grid3D::Grid3D()
    : m_pTexCoordinates(NULL)
    , m_pVertices(NULL)
    , m_pOriginalVertices(NULL)
    , m_pIndices(NULL)
{

}

Grid3D::~Grid3D(void)
{
    CC_SAFE_FREE(m_pTexCoordinates);
    CC_SAFE_FREE(m_pVertices);
    CC_SAFE_FREE(m_pIndices);
    CC_SAFE_FREE(m_pOriginalVertices);
}

void Grid3D::blit(void)
{
    int n = m_tGridSize.width * m_tGridSize.height;

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();;

    //
    // Attributes
    //
#ifdef EMSCRIPTEN
    // Size calculations from calculateVertexPoints().
    unsigned int numOfPoints = (m_tGridSize.width+1) * (m_tGridSize.height+1);

    // position
    setGLBufferData(m_pVertices, numOfPoints * sizeof(Vertex3F), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // texCoords
    setGLBufferData(m_pTexCoordinates, numOfPoints * sizeof(Vertex2F), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLIndexData(m_pIndices, n * 12, 0);
    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, 0);
#else
    // position
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices);

    // texCoords
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, m_pTexCoordinates);

    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, m_pIndices);
#endif // EMSCRIPTEN
    CC_INCREMENT_GL_DRAWS(1);
}

void Grid3D::calculateVertexPoints(void)
{
    float width = (float)m_pTexture->getPixelsWide();
    float height = (float)m_pTexture->getPixelsHigh();
    float imageH = m_pTexture->getContentSizeInPixels().height;

    int x, y, i;
    CC_SAFE_FREE(m_pVertices);
    CC_SAFE_FREE(m_pOriginalVertices);
    CC_SAFE_FREE(m_pTexCoordinates);
    CC_SAFE_FREE(m_pIndices);

    unsigned int numOfPoints = (m_tGridSize.width+1) * (m_tGridSize.height+1);

    m_pVertices = kdMalloc(numOfPoints * sizeof(Vertex3F));
    m_pOriginalVertices = kdMalloc(numOfPoints * sizeof(Vertex3F));
    m_pTexCoordinates = kdMalloc(numOfPoints * sizeof(Vertex2F));
    m_pIndices = (GLushort*) kdMalloc(m_tGridSize.width * m_tGridSize.height * sizeof(GLushort) * 6);

    GLfloat *vertArray = (GLfloat*)m_pVertices;
    GLfloat *texArray = (GLfloat*)m_pTexCoordinates;
    GLushort *idxArray = m_pIndices;

    for (x = 0; x < m_tGridSize.width; ++x)
    {
        for (y = 0; y < m_tGridSize.height; ++y)
        {
            int idx = (y * m_tGridSize.width) + x;

            GLfloat x1 = x * m_tStep.x;
            GLfloat x2 = x1 + m_tStep.x;
            GLfloat y1 = y * m_tStep.y;
            GLfloat y2= y1 + m_tStep.y;

            GLushort a = (GLushort)(x * (m_tGridSize.height + 1) + y);
            GLushort b = (GLushort)((x + 1) * (m_tGridSize.height + 1) + y);
            GLushort c = (GLushort)((x + 1) * (m_tGridSize.height + 1) + (y + 1));
            GLushort d = (GLushort)(x * (m_tGridSize.height + 1) + (y + 1));

            GLushort tempidx[6] = {a, b, d, b, c, d};

            memcpy(&idxArray[6*idx], tempidx, 6*sizeof(GLushort));

            int l1[4] = {a*3, b*3, c*3, d*3};
            Vertex3F e(x1, y1, 0);
            Vertex3F f(x2, y1, 0);
            Vertex3F g(x2, y2, 0);
            Vertex3F h(x1, y2, 0);

            Vertex3F l2[4] = {e, f, g, h};

            int tex1[4] = {a*2, b*2, c*2, d*2};
            Point Tex2F[4] = {Point(x1, y1), Point(x2, y1), Point(x2, y2), Point(x1, y2)};

            for (i = 0; i < 4; ++i)
            {
                vertArray[l1[i]] = l2[i].x;
                vertArray[l1[i] + 1] = l2[i].y;
                vertArray[l1[i] + 2] = l2[i].z;

                texArray[tex1[i]] = Tex2F[i].x / width;
                if (m_bIsTextureFlipped)
                {
                    texArray[tex1[i] + 1] = (imageH - Tex2F[i].y) / height;
                }
                else
                {
                    texArray[tex1[i] + 1] = Tex2F[i].y / height;
                }
            }
        }
    }

    memcpy(m_pOriginalVertices, m_pVertices, (m_tGridSize.width+1) * (m_tGridSize.height+1) * sizeof(Vertex3F));
}

Vertex3F Grid3D::getVertex(const Point& pos) const
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    
    int index = (pos.x * (m_tGridSize.height+1) + pos.y) * 3;
    float *vertArray = (float*)m_pVertices;

    Vertex3F vert(vertArray[index], vertArray[index+1], vertArray[index+2]);

    return vert;
}

Vertex3F Grid3D::getOriginalVertex(const Point& pos) const
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    
    int index = (pos.x * (m_tGridSize.height+1) + pos.y) * 3;
    float *vertArray = (float*)m_pOriginalVertices;

    Vertex3F vert(vertArray[index], vertArray[index+1], vertArray[index+2]);

    return vert;
}

void Grid3D::setVertex(const Point& pos, const Vertex3F& vertex)
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    int index = (pos.x * (m_tGridSize.height + 1) + pos.y) * 3;
    float *vertArray = (float*)m_pVertices;
    vertArray[index] = vertex.x;
    vertArray[index+1] = vertex.y;
    vertArray[index+2] = vertex.z;
}

void Grid3D::reuse(void)
{
    if (m_nReuseGrid > 0)
    {
        memcpy(m_pOriginalVertices, m_pVertices, (m_tGridSize.width+1) * (m_tGridSize.height+1) * sizeof(Vertex3F));
        --m_nReuseGrid;
    }
}

// implementation of TiledGrid3D

TiledGrid3D::TiledGrid3D()
    : m_pTexCoordinates(NULL)
    , m_pVertices(NULL)
    , m_pOriginalVertices(NULL)
    , m_pIndices(NULL)
{

}

TiledGrid3D::~TiledGrid3D(void)
{
    CC_SAFE_FREE(m_pTexCoordinates);
    CC_SAFE_FREE(m_pVertices);
    CC_SAFE_FREE(m_pOriginalVertices);
    CC_SAFE_FREE(m_pIndices);
}

TiledGrid3D* TiledGrid3D::create(const Size& gridSize, Texture2D *texture, bool bFlipped)
{
    TiledGrid3D *pRet= new TiledGrid3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize, texture, bFlipped))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}

TiledGrid3D* TiledGrid3D::create(const Size& gridSize)
{
    TiledGrid3D *pRet= new TiledGrid3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}

void TiledGrid3D::blit(void)
{
    int n = m_tGridSize.width * m_tGridSize.height;

    
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    //
    // Attributes
    //
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS );
#ifdef EMSCRIPTEN
    int numQuads = m_tGridSize.width * m_tGridSize.height;

    // position
    setGLBufferData(m_pVertices, (numQuads*4*sizeof(Vertex3F)), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // texCoords
    setGLBufferData(m_pTexCoordinates, (numQuads*4*sizeof(Vertex2F)), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLIndexData(m_pIndices, n * 12, 0);
    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, 0);
#else
    // position
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices);

    // texCoords
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, m_pTexCoordinates);

    glDrawElements(GL_TRIANGLES, (GLsizei)n*6, GL_UNSIGNED_SHORT, m_pIndices);
#endif // EMSCRIPTEN


    CC_INCREMENT_GL_DRAWS(1);
}

void TiledGrid3D::calculateVertexPoints(void)
{
    float width = (float)m_pTexture->getPixelsWide();
    float height = (float)m_pTexture->getPixelsHigh();
    float imageH = m_pTexture->getContentSizeInPixels().height;
    
    int numQuads = m_tGridSize.width * m_tGridSize.height;
    CC_SAFE_FREE(m_pVertices);
    CC_SAFE_FREE(m_pOriginalVertices);
    CC_SAFE_FREE(m_pTexCoordinates);
    CC_SAFE_FREE(m_pIndices);

    m_pVertices = kdMalloc(numQuads*4*sizeof(Vertex3F));
    m_pOriginalVertices = kdMalloc(numQuads*4*sizeof(Vertex3F));
    m_pTexCoordinates = kdMalloc(numQuads*4*sizeof(Vertex2F));
    m_pIndices = (GLushort*) kdMalloc(numQuads*6*sizeof(GLushort));

    GLfloat *vertArray = (GLfloat*)m_pVertices;
    GLfloat *texArray = (GLfloat*)m_pTexCoordinates;
    GLushort *idxArray = m_pIndices;
    
    int x, y;
    
    for( x = 0; x < m_tGridSize.width; x++ )
    {
        for( y = 0; y < m_tGridSize.height; y++ )
        {
            float x1 = x * m_tStep.x;
            float x2 = x1 + m_tStep.x;
            float y1 = y * m_tStep.y;
            float y2 = y1 + m_tStep.y;
            
            *vertArray++ = x1;
            *vertArray++ = y1;
            *vertArray++ = 0;
            *vertArray++ = x2;
            *vertArray++ = y1;
            *vertArray++ = 0;
            *vertArray++ = x1;
            *vertArray++ = y2;
            *vertArray++ = 0;
            *vertArray++ = x2;
            *vertArray++ = y2;
            *vertArray++ = 0;
            
            float newY1 = y1;
            float newY2 = y2;
            
            if (m_bIsTextureFlipped) 
            {
                newY1 = imageH - y1;
                newY2 = imageH - y2;
            }

            *texArray++ = x1 / width;
            *texArray++ = newY1 / height;
            *texArray++ = x2 / width;
            *texArray++ = newY1 / height;
            *texArray++ = x1 / width;
            *texArray++ = newY2 / height;
            *texArray++ = x2 / width;
            *texArray++ = newY2 / height;
        }
    }
    
    for (x = 0; x < numQuads; x++)
    {
        idxArray[x*6+0] = (GLushort)(x * 4 + 0);
        idxArray[x*6+1] = (GLushort)(x * 4 + 1);
        idxArray[x*6+2] = (GLushort)(x * 4 + 2);
        
        idxArray[x*6+3] = (GLushort)(x * 4 + 1);
        idxArray[x*6+4] = (GLushort)(x * 4 + 2);
        idxArray[x*6+5] = (GLushort)(x * 4 + 3);
    }
    
    memcpy(m_pOriginalVertices, m_pVertices, numQuads * 12 * sizeof(GLfloat));
}

void TiledGrid3D::setTile(const Point& pos, const Quad3& coords)
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    int idx = (m_tGridSize.height * pos.x + pos.y) * 4 * 3;
    float *vertArray = (float*)m_pVertices;
    memcpy(&vertArray[idx], &coords, sizeof(Quad3));
}

Quad3 TiledGrid3D::getOriginalTile(const Point& pos) const
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    int idx = (m_tGridSize.height * pos.x + pos.y) * 4 * 3;
    float *vertArray = (float*)m_pOriginalVertices;

    Quad3 ret;
    memcpy(&ret, &vertArray[idx], sizeof(Quad3));

    return ret;
}

Quad3 TiledGrid3D::getTile(const Point& pos) const
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    int idx = (m_tGridSize.height * pos.x + pos.y) * 4 * 3;
    float *vertArray = (float*)m_pVertices;

    Quad3 ret;
    memcpy(&ret, &vertArray[idx], sizeof(Quad3));

    return ret;
}

void TiledGrid3D::reuse(void)
{
    if (m_nReuseGrid > 0)
    {
        int numQuads = m_tGridSize.width * m_tGridSize.height;

        memcpy(m_pOriginalVertices, m_pVertices, numQuads * 12 * sizeof(GLfloat));
        --m_nReuseGrid;
    }
}

NS_CC_END
