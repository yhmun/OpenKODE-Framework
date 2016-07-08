/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
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

#include "2d/tilemap_parallax_nodes/CCTMXLayer.h"
#include "2d/tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "2d/tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/textures/CCTextureCache.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "support/data_support/ccCArray.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN


// TMXLayer - init & alloc & dealloc

TMXLayer * TMXLayer::create(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
    TMXLayer *pRet = new TMXLayer();
    if (pRet->initWithTilesetInfo(tilesetInfo, layerInfo, mapInfo))
    {
        pRet->autorelease();
        return pRet;
    }
    return NULL;
}
bool TMXLayer::initWithTilesetInfo(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{    
    // XXX: is 35% a good estimate ?
    Size size = layerInfo->m_tLayerSize;
    float totalNumberOfTiles = size.width * size.height;
    float capacity = totalNumberOfTiles * 0.35f + 1; // 35 percent is occupied ?

    Texture2D *texture = NULL;
    if( tilesetInfo )
    {
        texture = Director::getInstance()->getTextureCache()->addImage(tilesetInfo->m_sSourceImage.c_str());
    }

    if (SpriteBatchNode::initWithTexture(texture, (unsigned int)capacity))
    {
        // layerInfo
        m_sLayerName = layerInfo->m_sName;
        m_tLayerSize = size;
        m_pTiles = layerInfo->m_pTiles;
        m_uMinGID = layerInfo->m_uMinGID;
        m_uMaxGID = layerInfo->m_uMaxGID;
        m_cOpacity = layerInfo->m_cOpacity;
        setProperties(Dictionary::createWithDictionary(layerInfo->getProperties()));
        m_fContentScaleFactor = Director::getInstance()->getContentScaleFactor(); 

        // tilesetInfo
        m_pTileSet = tilesetInfo;
        CC_SAFE_RETAIN(m_pTileSet);

        // mapInfo
        m_tMapTileSize = mapInfo->getTileSize();
        m_uLayerOrientation = mapInfo->getOrientation();

        // offset (after layer orientation is set);
        Point offset = this->calculateLayerOffset(layerInfo->m_tOffset);
        this->setPosition(CC_POINT_PIXELS_TO_POINTS(offset));

        m_pAtlasIndexArray = ccCArrayNew((unsigned int)totalNumberOfTiles);

        this->setContentSize(CC_SIZE_PIXELS_TO_POINTS(Size(m_tLayerSize.width * m_tMapTileSize.width, m_tLayerSize.height * m_tMapTileSize.height)));

        m_bUseAutomaticVertexZ = false;
        m_nVertexZvalue = 0;
        
        return true;
    }
    return false;
}

TMXLayer::TMXLayer()
:m_sLayerName("")
,m_cOpacity(0)
,m_uMinGID(0)
,m_uMaxGID(0)
,m_nVertexZvalue(0)
,m_bUseAutomaticVertexZ(false)
,m_pReusedTile(NULL)
,m_pAtlasIndexArray(NULL)
,m_fContentScaleFactor(1.0f)
,m_tLayerSize(Size::ZERO)
,m_tMapTileSize(Size::ZERO)
,m_pTiles(NULL)
,m_pTileSet(NULL)
,m_uLayerOrientation(TMXOrientationOrtho)
,m_pProperties(NULL)
{}

TMXLayer::~TMXLayer()
{
    CC_SAFE_RELEASE(m_pTileSet);
    CC_SAFE_RELEASE(m_pReusedTile);
    CC_SAFE_RELEASE(m_pProperties);

    if (m_pAtlasIndexArray)
    {
        ccCArrayFree(m_pAtlasIndexArray);
        m_pAtlasIndexArray = NULL;
    }

    CC_SAFE_DELETE_ARRAY(m_pTiles);
}

void TMXLayer::releaseMap()
{
    if (m_pTiles)
    {
        delete [] m_pTiles;
        m_pTiles = NULL;
    }

    if (m_pAtlasIndexArray)
    {
        ccCArrayFree(m_pAtlasIndexArray);
        m_pAtlasIndexArray = NULL;
    }
}

// TMXLayer - setup Tiles
void TMXLayer::setupTiles()
{    
    // Optimization: quick hack that sets the image size on the tileset
    m_pTileSet->m_tImageSize = m_pTextureAtlas->getTexture()->getContentSizeInPixels();

    // By default all the tiles are aliased
    // pros:
    //  - easier to render
    // cons:
    //  - difficult to scale / rotate / etc.
    m_pTextureAtlas->getTexture()->setAliasTexParameters();

    //CFByteOrder o = CFByteOrderGetCurrent();

    // Parse cocos2d properties
    this->parseInternalProperties();

    for (unsigned int y=0; y < m_tLayerSize.height; y++) 
    {
        for (unsigned int x=0; x < m_tLayerSize.width; x++) 
        {
            unsigned int pos = (unsigned int)(x + m_tLayerSize.width * y);
            unsigned int gid = m_pTiles[ pos ];

            // gid are stored in little endian.
            // if host is big endian, then swap
            //if( o == CFByteOrderBigEndian )
            //    gid = CFSwapInt32( gid );
            /* We support little endian.*/

            // XXX: gid == 0 --> empty tile
            if (gid != 0) 
            {
                this->appendTileForGID(gid, Point(x, y));

                // Optimization: update min and max GID rendered by the layer
                m_uMinGID = KD_MIN(gid, m_uMinGID);
                m_uMaxGID = KD_MAX(gid, m_uMaxGID);
            }
        }
    }

    CCASSERT( m_uMaxGID >= m_pTileSet->m_uFirstGid &&
        m_uMinGID >= m_pTileSet->m_uFirstGid, "TMX: Only 1 tileset per layer is supported");    
}

// TMXLayer - Properties
String* TMXLayer::getProperty(const char *propertyName) const
{
    return static_cast<String*>(m_pProperties->objectForKey(propertyName));
}

void TMXLayer::parseInternalProperties()
{
    // if cc_vertex=automatic, then tiles will be rendered using vertexz

    String *vertexz = getProperty("cc_vertexz");
    if (vertexz) 
    {
        // If "automatic" is on, then parse the "cc_alpha_func" too
        if (vertexz->m_sString == "automatic")
        {
            m_bUseAutomaticVertexZ = true;
            String *alphaFuncVal = getProperty("cc_alpha_func");
            float alphaFuncValue = 0.0f;
            if (alphaFuncVal != NULL)
            {
                alphaFuncValue = alphaFuncVal->floatValue();
            }
            setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST));

            GLint alphaValueLocation = glGetUniformLocation(getShaderProgram()->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

            // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
            
            // use shader program to set uniform
            getShaderProgram()->use();
            getShaderProgram()->setUniformLocationWith1f(alphaValueLocation, alphaFuncValue);
            CHECK_GL_ERROR_DEBUG();
        }
        else
        {
            m_nVertexZvalue = vertexz->intValue();
        }
    }
}

void TMXLayer::setupTileSprite(Sprite* sprite, Point pos, unsigned int gid)
{
    sprite->setPosition(getPositionAt(pos));
    sprite->setVertexZ((float)getVertexZForPos(pos));
    sprite->setAnchorPoint(Point::ZERO);
    sprite->setOpacity(m_cOpacity);

    //issue 1264, flip can be undone as well
    sprite->setFlippedX(false);
    sprite->setFlippedY(false);
    sprite->setRotation(0.0f);
    sprite->setAnchorPoint(Point(0,0));

    // Rotation in tiled is achieved using 3 flipped states, flipping across the horizontal, vertical, and diagonal axes of the tiles.
    if (gid & kTMXTileDiagonalFlag)
    {
        // put the anchor in the middle for ease of rotation.
        sprite->setAnchorPoint(Point(0.5f,0.5f));
        sprite->setPosition(Point(getPositionAt(pos).x + sprite->getContentSize().height/2,
           getPositionAt(pos).y + sprite->getContentSize().width/2 ) );

        unsigned int flag = gid & (kTMXTileHorizontalFlag | kTMXTileVerticalFlag );

        // handle the 4 diagonally flipped states.
        if (flag == kTMXTileHorizontalFlag)
        {
            sprite->setRotation(90.0f);
        }
        else if (flag == kTMXTileVerticalFlag)
        {
            sprite->setRotation(270.0f);
        }
        else if (flag == (kTMXTileVerticalFlag | kTMXTileHorizontalFlag) )
        {
            sprite->setRotation(90.0f);
            sprite->setFlippedX(true);
        }
        else
        {
            sprite->setRotation(270.0f);
            sprite->setFlippedX(true);
        }
    }
    else
    {
        if (gid & kTMXTileHorizontalFlag)
        {
            sprite->setFlippedX(true);
        }

        if (gid & kTMXTileVerticalFlag)
        {
            sprite->setFlippedY(true);
        }
    }
}

Sprite* TMXLayer::reusedTileWithRect(Rect rect)
{
    if (! m_pReusedTile) 
    {
        m_pReusedTile = new Sprite();
        m_pReusedTile->initWithTexture(m_pTextureAtlas->getTexture(), rect, false);
        m_pReusedTile->setBatchNode(this);
    }
    else
    {
        // XXX HACK: Needed because if "batch node" is nil,
		// then the Sprite'squad will be reset
        m_pReusedTile->setBatchNode(NULL);
        
		// Re-init the sprite
        m_pReusedTile->setTextureRect(rect, false, rect.size);
        
		// restore the batch node
        m_pReusedTile->setBatchNode(this);
    }

    return m_pReusedTile;
}

// TMXLayer - obtaining tiles/gids
Sprite * TMXLayer::getTileAt(const Point& pos)
{
    CCASSERT(pos.x < m_tLayerSize.width && pos.y < m_tLayerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    CCASSERT(m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released");

    Sprite *tile = nullptr;
    unsigned int gid = this->getTileGIDAt(pos);

    // if GID == 0, then no tile is present
    if (gid) 
    {
        int z = (int)(pos.x + pos.y * m_tLayerSize.width);
        tile = static_cast<Sprite*>(this->getChildByTag(z));

        // tile not created yet. create it
        if (! tile) 
        {
            Rect rect = m_pTileSet->rectForGID(gid);
            rect = CC_RECT_PIXELS_TO_POINTS(rect);

            tile = new Sprite();
            tile->initWithTexture(this->getTexture(), rect);
            tile->setBatchNode(this);
            tile->setPosition(getPositionAt(pos));
            tile->setVertexZ((float)getVertexZForPos(pos));
            tile->setAnchorPoint(Point::ZERO);
            tile->setOpacity(m_cOpacity);

            unsigned int indexForZ = atlasIndexForExistantZ(z);
            this->addSpriteWithoutQuad(tile, indexForZ, z);
            tile->release();
        }
    }
    
    return tile;
}

unsigned int TMXLayer::getTileGIDAt(const Point& pos, ccTMXTileFlags* flags/* = nullptr*/)
{
    CCASSERT(pos.x < m_tLayerSize.width && pos.y < m_tLayerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    CCASSERT(m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released");

    int idx = (int)(pos.x + pos.y * m_tLayerSize.width);
    // Bits on the far end of the 32-bit global tile ID are used for tile flags
    unsigned int tile = m_pTiles[idx];

    // issue1264, flipped tiles can be changed dynamically
    if (flags) 
    {
        *flags = (ccTMXTileFlags)(tile & kFlipedAll);
    }
    
    return (tile & kFlippedMask);
}

// TMXLayer - adding helper methods
Sprite * TMXLayer::insertTileForGID(unsigned int gid, const Point& pos)
{
    Rect rect = m_pTileSet->rectForGID(gid);
    rect = CC_RECT_PIXELS_TO_POINTS(rect);

    KDintptr z = (KDintptr)(pos.x + pos.y * m_tLayerSize.width);

    Sprite *tile = reusedTileWithRect(rect);

    setupTileSprite(tile, pos, gid);

    // get atlas index
    unsigned int indexForZ = atlasIndexForNewZ(z);

    // Optimization: add the quad without adding a child
    this->insertQuadFromSprite(tile, indexForZ);

    // insert it into the local atlasindex array
    ccCArrayInsertValueAtIndex(m_pAtlasIndexArray, (void*)z, indexForZ);

    // update possible children
    if (m_pChildren && m_pChildren->count()>0)
    {
        Object* pObject = nullptr;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            Sprite* child = static_cast<Sprite*>(pObject);
            if (child)
            {
                unsigned int ai = child->getAtlasIndex();
                if ( ai >= indexForZ )
                {
                    child->setAtlasIndex(ai+1);
                }
            }
        }
    }
    m_pTiles[z] = gid;
    return tile;
}
Sprite * TMXLayer::updateTileForGID(unsigned int gid, const Point& pos)    
{
    Rect rect = m_pTileSet->rectForGID(gid);
    rect = Rect(rect.origin.x / m_fContentScaleFactor, rect.origin.y / m_fContentScaleFactor, rect.size.width/ m_fContentScaleFactor, rect.size.height/ m_fContentScaleFactor);
    int z = (int)(pos.x + pos.y * m_tLayerSize.width);

    Sprite *tile = reusedTileWithRect(rect);

    setupTileSprite(tile ,pos ,gid);

    // get atlas index
    unsigned int indexForZ = atlasIndexForExistantZ(z);
    tile->setAtlasIndex(indexForZ);
    tile->setDirty(true);
    tile->updateTransform();
    m_pTiles[z] = gid;

    return tile;
}

// used only when parsing the map. useless after the map was parsed
// since lot's of assumptions are no longer true
Sprite * TMXLayer::appendTileForGID(unsigned int gid, const Point& pos)
{
    Rect rect = m_pTileSet->rectForGID(gid);
    rect = CC_RECT_PIXELS_TO_POINTS(rect);

    KDintptr z = (KDintptr)(pos.x + pos.y * m_tLayerSize.width);

    Sprite *tile = reusedTileWithRect(rect);

    setupTileSprite(tile ,pos ,gid);

    // optimization:
    // The difference between appendTileForGID and insertTileforGID is that append is faster, since
    // it appends the tile at the end of the texture atlas
    unsigned int indexForZ = m_pAtlasIndexArray->num;

    // don't add it using the "standard" way.
    insertQuadFromSprite(tile, indexForZ);

    // append should be after addQuadFromSprite since it modifies the quantity values
    ccCArrayInsertValueAtIndex(m_pAtlasIndexArray, (void*)z, indexForZ);

    return tile;
}

// TMXLayer - atlasIndex and Z
static inline int compareInts(const void * a, const void * b)
{
    return ((*(int*)a) - (*(int*)b));
}
unsigned int TMXLayer::atlasIndexForExistantZ(unsigned int z)
{
    int key=z;
    int *item = (int*) kdBsearch((void*)&key, (void*)&m_pAtlasIndexArray->arr[0], m_pAtlasIndexArray->num, sizeof(void*), compareInts);

    CCASSERT(item, "TMX atlas index not found. Shall not happen");

    int index = ((size_t)item - (size_t)m_pAtlasIndexArray->arr) / sizeof(void*);
    return index;
}
unsigned int TMXLayer::atlasIndexForNewZ(int z)
{
    // XXX: This can be improved with a sort of binary search
    int i=0;
    for (i=0; i< m_pAtlasIndexArray->num ; i++) 
    {
        int val = (size_t) m_pAtlasIndexArray->arr[i];
        if (z < val)
        {
            break;
        }
    } 
    
    return i;
}

// TMXLayer - adding / remove tiles
void TMXLayer::setTileGID(unsigned int gid, const Point& pos)
{
    setTileGID(gid, pos, (ccTMXTileFlags)0);
}

void TMXLayer::setTileGID(unsigned int gid, const Point& pos, ccTMXTileFlags flags)
{
    CCASSERT(pos.x < m_tLayerSize.width && pos.y < m_tLayerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    CCASSERT(m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released");
    CCASSERT(gid == 0 || gid >= m_pTileSet->m_uFirstGid, "TMXLayer: invalid gid" );

    ccTMXTileFlags currentFlags;
    unsigned int currentGID = getTileGIDAt(pos, &currentFlags);

    if (currentGID != gid || currentFlags != flags) 
    {
        unsigned gidAndFlags = gid | flags;

        // setting gid=0 is equal to remove the tile
        if (gid == 0)
        {
            removeTileAt(pos);
        }
        // empty tile. create a new one
        else if (currentGID == 0)
        {
            insertTileForGID(gidAndFlags, pos);
        }
        // modifying an existing tile with a non-empty tile
        else 
        {
            unsigned int z = (unsigned int)(pos.x + pos.y * m_tLayerSize.width);
            Sprite *sprite = static_cast<Sprite*>(getChildByTag(z));
            if (sprite)
            {
                Rect rect = m_pTileSet->rectForGID(gid);
                rect = CC_RECT_PIXELS_TO_POINTS(rect);

                sprite->setTextureRect(rect, false, rect.size);
                if (flags) 
                {
                    setupTileSprite(sprite, sprite->getPosition(), gidAndFlags);
                }
                m_pTiles[z] = gidAndFlags;
            } 
            else 
            {
                updateTileForGID(gidAndFlags, pos);
            }
        }
    }
}

void TMXLayer::addChild(Node * child, int zOrder, int tag)
{
    CC_UNUSED_PARAM(child);
    CC_UNUSED_PARAM(zOrder);
    CC_UNUSED_PARAM(tag);
    CCASSERT(0, "addChild: is not supported on TMXLayer. Instead use setTileGID:at:/tileAt:");
}

void TMXLayer::removeChild(Node* node, bool cleanup)
{
    Sprite *sprite = (Sprite*)node;
    // allows removing nil objects
    if (! sprite)
    {
        return;
    }

    CCASSERT(m_pChildren->containsObject(sprite), "Tile does not belong to TMXLayer");

    unsigned int atlasIndex = sprite->getAtlasIndex();
    unsigned int zz = (size_t)m_pAtlasIndexArray->arr[atlasIndex];
    m_pTiles[zz] = 0;
    ccCArrayRemoveValueAtIndex(m_pAtlasIndexArray, atlasIndex);
    SpriteBatchNode::removeChild(sprite, cleanup);
}

void TMXLayer::removeTileAt(const Point& pos)
{
    CCASSERT(pos.x < m_tLayerSize.width && pos.y < m_tLayerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    CCASSERT(m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released");

    unsigned int gid = getTileGIDAt(pos);

    if (gid) 
    {
        unsigned int z = (unsigned int)(pos.x + pos.y * m_tLayerSize.width);
        unsigned int atlasIndex = atlasIndexForExistantZ(z);

        // remove tile from GID map
        m_pTiles[z] = 0;

        // remove tile from atlas position array
        ccCArrayRemoveValueAtIndex(m_pAtlasIndexArray, atlasIndex);

        // remove it from sprites and/or texture atlas
        Sprite *sprite = (Sprite*)getChildByTag(z);
        if (sprite)
        {
            SpriteBatchNode::removeChild(sprite, true);
        }
        else 
        {
            m_pTextureAtlas->removeQuadAtIndex(atlasIndex);

            // update possible children
            if (m_pChildren && m_pChildren->count()>0)
            {
                Object* pObject = nullptr;
                CCARRAY_FOREACH(m_pChildren, pObject)
                {
                    Sprite* child = static_cast<Sprite*>(pObject);
                    if (child)
                    {
                        unsigned int ai = child->getAtlasIndex();
                        if ( ai >= atlasIndex )
                        {
                            child->setAtlasIndex(ai-1);
                        }
                    }
                }
            }
        }
    }
}

//CCTMXLayer - obtaining positions, offset
Point TMXLayer::calculateLayerOffset(const Point& pos)
{
    Point ret = Point::ZERO;
    switch (m_uLayerOrientation) 
    {
    case TMXOrientationOrtho:
        ret = Point( pos.x * m_tMapTileSize.width, -pos.y *m_tMapTileSize.height);
        break;
    case TMXOrientationIso:
        ret = Point((m_tMapTileSize.width /2) * (pos.x - pos.y),
                  (m_tMapTileSize.height /2 ) * (-pos.x - pos.y));
        break;
    case TMXOrientationHex:
        CCASSERT(pos.equals(Point::ZERO), "offset for hexagonal map not implemented yet");
        break;
    }
    return ret;    
}

Point TMXLayer::getPositionAt(const Point& pos)
{
    Point ret = Point::ZERO;
    switch (m_uLayerOrientation)
    {
    case TMXOrientationOrtho:
        ret = getPositionForOrthoAt(pos);
        break;
    case TMXOrientationIso:
        ret = getPositionForIsoAt(pos);
        break;
    case TMXOrientationHex:
        ret = getPositionForHexAt(pos);
        break;
    }
    ret = CC_POINT_PIXELS_TO_POINTS( ret );
    return ret;
}

Point TMXLayer::getPositionForOrthoAt(const Point& pos)
{
    return Point(pos.x * m_tMapTileSize.width,
                            (m_tLayerSize.height - pos.y - 1) * m_tMapTileSize.height);
}

Point TMXLayer::getPositionForIsoAt(const Point& pos)
{
    return Point(m_tMapTileSize.width /2 * (m_tLayerSize.width + pos.x - pos.y - 1),
                             m_tMapTileSize.height /2 * ((m_tLayerSize.height * 2 - pos.x - pos.y) - 2));
}

Point TMXLayer::getPositionForHexAt(const Point& pos)
{
    float diffY = 0;
    if ((int)pos.x % 2 == 1)
    {
        diffY = -m_tMapTileSize.height/2 ;
    }

    Point xy = Point(pos.x * m_tMapTileSize.width*3/4,
                            (m_tLayerSize.height - pos.y - 1) * m_tMapTileSize.height + diffY);
    return xy;
}

int TMXLayer::getVertexZForPos(const Point& pos)
{
    int ret = 0;
    unsigned int maxVal = 0;
    if (m_bUseAutomaticVertexZ)
    {
        switch (m_uLayerOrientation) 
        {
        case TMXOrientationIso:
            maxVal = (unsigned int)(m_tLayerSize.width + m_tLayerSize.height);
            ret = (int)(-(maxVal - (pos.x + pos.y)));
            break;
        case TMXOrientationOrtho:
            ret = (int)(-(m_tLayerSize.height-pos.y));
            break;
        case TMXOrientationHex:
            CCASSERT(0, "TMX Hexa zOrder not supported");
            break;
        default:
            CCASSERT(0, "TMX invalid value");
            break;
        }
    } 
    else
    {
        ret = m_nVertexZvalue;
    }
    
    return ret;
}

NS_CC_END

