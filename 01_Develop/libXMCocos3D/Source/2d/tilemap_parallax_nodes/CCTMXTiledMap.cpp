/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXTiledMap.cpp
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

#include "2d/tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "2d/tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "2d/tilemap_parallax_nodes/CCTMXLayer.h"
#include "2d/sprite_nodes/CCSprite.h"
#include <algorithm>

NS_CC_BEGIN

// implementation TMXTiledMap

TMXTiledMap * TMXTiledMap::create(const std::string& tmxFile)
{
    TMXTiledMap *ret = new TMXTiledMap();
    if (ret->initWithTMXFile(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

TMXTiledMap* TMXTiledMap::createWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    TMXTiledMap *ret = new TMXTiledMap();
    if (ret->initWithXML(tmxString, resourcePath))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool TMXTiledMap::initWithTMXFile(const std::string& tmxFile)
{
    CCASSERT(tmxFile.size()>0, "TMXTiledMap: tmx file should not be empty");
    
    setContentSize(Size::ZERO);

    TMXMapInfo *mapInfo = TMXMapInfo::create(tmxFile);

    if (! mapInfo)
    {
        return false;
    }
    CCASSERT( mapInfo->getTilesets()->count() != 0, "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);

    return true;
}

bool TMXTiledMap::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    setContentSize(Size::ZERO);

    TMXMapInfo *mapInfo = TMXMapInfo::createWithXML(tmxString, resourcePath);

    CCASSERT( mapInfo->getTilesets()->count() != 0, "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);

    return true;
}

TMXTiledMap::TMXTiledMap()
    :m_tMapSize(Size::ZERO)
    ,m_tTileSize(Size::ZERO)        
    ,m_pObjectGroups(NULL)
    ,m_pProperties(NULL)
    ,m_pTileProperties(NULL)
{
}
TMXTiledMap::~TMXTiledMap()
{
    CC_SAFE_RELEASE(m_pProperties);
    CC_SAFE_RELEASE(m_pObjectGroups);
    CC_SAFE_RELEASE(m_pTileProperties);
}

// private
TMXLayer * TMXTiledMap::parseLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
    TMXTilesetInfo *tileset = tilesetForLayer(layerInfo, mapInfo);
    TMXLayer *layer = TMXLayer::create(tileset, layerInfo, mapInfo);

    // tell the layerinfo to release the ownership of the tiles map.
    layerInfo->m_bOwnTiles = false;
    layer->setupTiles();

    return layer;
}

TMXTilesetInfo * TMXTiledMap::tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
    Size size = layerInfo->m_tLayerSize;
    Array* tilesets = mapInfo->getTilesets();
    if (tilesets && tilesets->count()>0)
    {
        TMXTilesetInfo* tileset = NULL;
        Object* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(tilesets, pObj)
        {
            tileset = static_cast<TMXTilesetInfo*>(pObj);
            if (tileset)
            {
                for( unsigned int y=0; y < size.height; y++ )
                {
                    for( unsigned int x=0; x < size.width; x++ ) 
                    {
                        unsigned int pos = (unsigned int)(x + size.width * y);
                        unsigned int gid = layerInfo->m_pTiles[ pos ];

                        // gid are stored in little endian.
                        // if host is big endian, then swap
                        //if( o == CFByteOrderBigEndian )
                        //    gid = CFSwapInt32( gid );
                        /* We support little endian.*/

                        // XXX: gid == 0 --> empty tile
                        if( gid != 0 ) 
                        {
                            // Optimization: quick return
                            // if the layer is invalid (more than 1 tileset per layer) an CCAssert will be thrown later
                            if( (gid & kFlippedMask) >= tileset->m_uFirstGid )
                                return tileset;
                        }
                    }
                }        
            }
        }
    }

    // If all the tiles are 0, return empty tileset
    CCLOG("cocos2d: Warning: TMX Layer '%s' has no tiles", layerInfo->m_sName.c_str());
    return NULL;
}

void TMXTiledMap::buildWithMapInfo(TMXMapInfo* mapInfo)
{
    m_tMapSize = mapInfo->getMapSize();
    m_tTileSize = mapInfo->getTileSize();
    m_nMapOrientation = mapInfo->getOrientation();

    CC_SAFE_RELEASE(m_pObjectGroups);
    m_pObjectGroups = mapInfo->getObjectGroups();
    CC_SAFE_RETAIN(m_pObjectGroups);

    CC_SAFE_RELEASE(m_pProperties);
    m_pProperties = mapInfo->getProperties();
    CC_SAFE_RETAIN(m_pProperties);

    CC_SAFE_RELEASE(m_pTileProperties);
    m_pTileProperties = mapInfo->getTileProperties();
    CC_SAFE_RETAIN(m_pTileProperties);

    int idx=0;

    Array* layers = mapInfo->getLayers();
    if (layers && layers->count()>0)
    {
        TMXLayerInfo* layerInfo = NULL;
        Object* pObj = NULL;
        CCARRAY_FOREACH(layers, pObj)
        {
            layerInfo = static_cast<TMXLayerInfo*>(pObj);
            if (layerInfo && layerInfo->m_bVisible)
            {
                TMXLayer *child = parseLayer(layerInfo, mapInfo);
                addChild((Node*)child, idx, idx);

                // update content size with the max size
                const Size& childSize = child->getContentSize();
                Size currentSize = this->getContentSize();
                currentSize.width = std::max( currentSize.width, childSize.width );
                currentSize.height = std::max( currentSize.height, childSize.height );
                this->setContentSize(currentSize);

                idx++;
            }
        }
    }
}

// public
TMXLayer * TMXTiledMap::getLayer(const std::string& layerName) const
{
    CCASSERT(layerName.size() > 0, "Invalid layer name!");
    Object* pObj = NULL;
    CCARRAY_FOREACH(m_pChildren, pObj) 
    {
        TMXLayer* layer = dynamic_cast<TMXLayer*>(pObj);
        if(layer)
        {
            if(layerName.compare( layer->getLayerName()) == 0)
            {
                return layer;
            }
        }
    }

    // layer not found
    return NULL;
}

TMXObjectGroup * TMXTiledMap::getObjectGroup(const std::string& groupName) const
{
    CCASSERT(groupName.size() > 0, "Invalid group name!");

    if (m_pObjectGroups && m_pObjectGroups->count()>0)
    {
        TMXObjectGroup* objectGroup = NULL;
        Object* pObj = NULL;
        CCARRAY_FOREACH(m_pObjectGroups, pObj)
        {
            objectGroup = static_cast<TMXObjectGroup*>(pObj);
            if (objectGroup && objectGroup->getGroupName() == groupName)
            {
                return objectGroup;
            }
        }
    }

    // objectGroup not found
    return NULL;
}

String* TMXTiledMap::getProperty(const std::string& propertyName) const
{
    return static_cast<String*>(m_pProperties->objectForKey(propertyName));
}

Dictionary* TMXTiledMap::getPropertiesForGID(int GID) const
{
    return static_cast<Dictionary*>(m_pTileProperties->objectForKey(GID));
}
        

NS_CC_END

