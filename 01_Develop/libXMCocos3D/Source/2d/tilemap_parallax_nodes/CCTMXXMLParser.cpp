/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXXMLParser.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011		Максим Аксенов
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

#include <map>
#include <sstream>
#include "2d/tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "2d/tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "platform/CCFileUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "support/base64.h"

using namespace std;

NS_CC_BEGIN

static const char* valueForKey(const char *key, std::map<std::string, std::string>* dict)
{
    if (dict)
    {
        std::map<std::string, std::string>::iterator it = dict->find(key);
        return it!=dict->end() ? it->second.c_str() : "";
    }
    return "";
}
// implementation TMXLayerInfo
TMXLayerInfo::TMXLayerInfo()
: m_sName("")
, m_pTiles(NULL)
, m_bOwnTiles(true)
, m_uMinGID(100000)
, m_uMaxGID(0)        
, m_tOffset(Point::ZERO)
{
    m_pProperties = new Dictionary();
    m_pProperties->init();
}

TMXLayerInfo::~TMXLayerInfo()
{
    CCLOGINFO("deallocing TMXLayerInfo: %p", this);
    CC_SAFE_RELEASE(m_pProperties);
    if( m_bOwnTiles && m_pTiles )
    {
        free(m_pTiles);
        m_pTiles = NULL;
    }
}

Dictionary * TMXLayerInfo::getProperties()
{
    return m_pProperties;
}
void TMXLayerInfo::setProperties(Dictionary* var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pProperties);
    m_pProperties = var;
}

// implementation TMXTilesetInfo
TMXTilesetInfo::TMXTilesetInfo()
    :m_uFirstGid(0)
    ,m_tTileSize(Size::ZERO)
    ,m_uSpacing(0)
    ,m_uMargin(0)
    ,m_tImageSize(Size::ZERO)
{
}

TMXTilesetInfo::~TMXTilesetInfo()
{
    CCLOGINFO("deallocing TMXTilesetInfo: %p", this);
}

Rect TMXTilesetInfo::rectForGID(unsigned int gid)
{
    Rect rect;
    rect.size = m_tTileSize;
    gid &= kFlippedMask;
    gid = gid - m_uFirstGid;
    int max_x = (int)((m_tImageSize.width - m_uMargin*2 + m_uSpacing) / (m_tTileSize.width + m_uSpacing));
    //    int max_y = (imageSize.height - margin*2 + spacing) / (tileSize.height + spacing);
    rect.origin.x = (gid % max_x) * (m_tTileSize.width + m_uSpacing) + m_uMargin;
    rect.origin.y = (gid / max_x) * (m_tTileSize.height + m_uSpacing) + m_uMargin;
    return rect;
}

// implementation TMXMapInfo

TMXMapInfo * TMXMapInfo::create(const std::string& tmxFile)
{
    TMXMapInfo *pRet = new TMXMapInfo();
    if(pRet->initWithTMXFile(tmxFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

TMXMapInfo * TMXMapInfo::createWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    TMXMapInfo *pRet = new TMXMapInfo();
    if(pRet->initWithXML(tmxString, resourcePath))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void TMXMapInfo::internalInit(const std::string& tmxFileName, const std::string& resourcePath)
{
    m_pTilesets = Array::create();
    m_pTilesets->retain();

    m_pLayers = Array::create();
    m_pLayers->retain();

    if (tmxFileName.size() > 0)
    {
        m_sTMXFileName = FileUtils::getInstance()->fullPathForFilename(tmxFileName);
    }
    
    if (resourcePath.size() > 0)
    {
        m_sResources = resourcePath;
    }
    
    m_pObjectGroups = Array::createWithCapacity(4);
    m_pObjectGroups->retain();

    m_pProperties = new Dictionary();
    m_pProperties->init();
    m_pTileProperties = new Dictionary();
    m_pTileProperties->init();

    // tmp vars
    m_sCurrentString = "";
    m_bStoringCharacters = false;
    m_nLayerAttribs = TMXLayerAttribNone;
    m_nParentElement = TMXPropertyNone;
    m_uCurrentFirstGID = 0;
}
bool TMXMapInfo::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    internalInit("", resourcePath);
    return parseXMLString(tmxString);
}

bool TMXMapInfo::initWithTMXFile(const std::string& tmxFile)
{
    internalInit(tmxFile, "");
    return parseXMLFile(m_sTMXFileName.c_str());
}

TMXMapInfo::TMXMapInfo()
: m_tMapSize(Size::ZERO)    
, m_tTileSize(Size::ZERO)
, m_pLayers(NULL)
, m_pTilesets(NULL)
, m_pObjectGroups(NULL)
, m_nLayerAttribs(0)
, m_bStoringCharacters(false)
, m_pProperties(NULL)
, m_pTileProperties(NULL)
, m_uCurrentFirstGID(0)
{
}

TMXMapInfo::~TMXMapInfo()
{
    CCLOGINFO("deallocing TMXMapInfo: %p", this);
    CC_SAFE_RELEASE(m_pTilesets);
    CC_SAFE_RELEASE(m_pLayers);
    CC_SAFE_RELEASE(m_pProperties);
    CC_SAFE_RELEASE(m_pTileProperties);
    CC_SAFE_RELEASE(m_pObjectGroups);
}

bool TMXMapInfo::parseXMLString(const std::string& xmlString)
{
    int len = xmlString.size();
    if (len <= 0)
        return false;

    SAXParser parser;

    if (false == parser.init("UTF-8") )
    {
        return false;
    }

    parser.setDelegator(this);

    return parser.parse(xmlString.c_str(), len);
}

bool TMXMapInfo::parseXMLFile(const std::string& xmlFilename)
{
    SAXParser parser;
    
    if (false == parser.init("UTF-8") )
    {
        return false;
    }
    
    parser.setDelegator(this);

    return parser.parse(FileUtils::getInstance()->fullPathForFilename(xmlFilename).c_str());
}


// the XML parser calls here with all the elements
void TMXMapInfo::startElement(void *ctx, const char *name, const char **atts)
{    
    CC_UNUSED_PARAM(ctx);
    TMXMapInfo *pTMXMapInfo = this;
    std::string elementName = (char*)name;
    std::map<std::string, std::string> *attributeDict = new std::map<std::string, std::string>();
    if (atts && atts[0])
    {
        for(int i = 0; atts[i]; i += 2) 
        {
            std::string key = (char*)atts[i];
            std::string value = (char*)atts[i+1];
            attributeDict->insert(pair<std::string, std::string>(key, value));
        }
    }
    if (elementName == "map")
    {
        std::string version = valueForKey("version", attributeDict);
        if ( version != "1.0")
        {
            CCLOG("cocos2d: TMXFormat: Unsupported TMX version: %s", version.c_str());
        }
        std::string orientationStr = valueForKey("orientation", attributeDict);
        if (orientationStr == "orthogonal")
            pTMXMapInfo->setOrientation(TMXOrientationOrtho);
        else if (orientationStr  == "isometric")
            pTMXMapInfo->setOrientation(TMXOrientationIso);
        else if(orientationStr == "hexagonal")
            pTMXMapInfo->setOrientation(TMXOrientationHex);
        else
            CCLOG("cocos2d: TMXFomat: Unsupported orientation: %d", pTMXMapInfo->getOrientation());

        Size s;
        s.width = kdAtof(valueForKey("width", attributeDict));
        s.height = kdAtof(valueForKey("height", attributeDict));
        pTMXMapInfo->setMapSize(s);

        s.width = kdAtof(valueForKey("tilewidth", attributeDict));
        s.height = kdAtof(valueForKey("tileheight", attributeDict));
        pTMXMapInfo->setTileSize(s);

        // The parent element is now "map"
        pTMXMapInfo->setParentElement(TMXPropertyMap);
    } 
    else if (elementName == "tileset") 
    {
        // If this is an external tileset then start parsing that
        std::string externalTilesetFilename = valueForKey("source", attributeDict);
        if (externalTilesetFilename != "")
        {
            // Tileset file will be relative to the map file. So we need to convert it to an absolute path
            if (m_sTMXFileName.find_last_of("/") != string::npos)
            {
                string dir = m_sTMXFileName.substr(0, m_sTMXFileName.find_last_of("/") + 1);
                externalTilesetFilename = dir + externalTilesetFilename;
            }
            else 
            {
                externalTilesetFilename = m_sResources + "/" + externalTilesetFilename;
            }
            externalTilesetFilename = FileUtils::getInstance()->fullPathForFilename(externalTilesetFilename.c_str());
            
            m_uCurrentFirstGID = (unsigned int)kdAtoi(valueForKey("firstgid", attributeDict));
            
            pTMXMapInfo->parseXMLFile(externalTilesetFilename.c_str());
        }
        else
        {
            TMXTilesetInfo *tileset = new TMXTilesetInfo();
            tileset->m_sName = valueForKey("name", attributeDict);
            if (m_uCurrentFirstGID == 0)
            {
                tileset->m_uFirstGid = (unsigned int)kdAtoi(valueForKey("firstgid", attributeDict));
            }
            else
            {
                tileset->m_uFirstGid = m_uCurrentFirstGID;
                m_uCurrentFirstGID = 0;
            }
            tileset->m_uSpacing = (unsigned int)kdAtoi(valueForKey("spacing", attributeDict));
            tileset->m_uMargin = (unsigned int)kdAtoi(valueForKey("margin", attributeDict));
            Size s;
            s.width = kdAtof(valueForKey("tilewidth", attributeDict));
            s.height = kdAtof(valueForKey("tileheight", attributeDict));
            tileset->m_tTileSize = s;

            pTMXMapInfo->getTilesets()->addObject(tileset);
            tileset->release();
        }
    }
    else if (elementName == "tile")
    {
        if (pTMXMapInfo->getParentElement() == TMXPropertyLayer)
        {
            TMXLayerInfo* layer = (TMXLayerInfo*)pTMXMapInfo->getLayers()->getLastObject();
            Size layerSize = layer->m_tLayerSize;
            unsigned int gid = (unsigned int)kdAtoi(valueForKey("gid", attributeDict));
            int tilesAmount = layerSize.width*layerSize.height;
            
            do
            {
                // Check the gid is legal or not
                CC_BREAK_IF(gid == 0);
                
                if (tilesAmount > 1)
                {
                    // Check the value is all set or not
                    CC_BREAK_IF(layer->m_pTiles[tilesAmount - 2] != 0 && layer->m_pTiles[tilesAmount - 1] != 0);
                    
                    int currentTileIndex = tilesAmount - layer->m_pTiles[tilesAmount - 1] - 1;
                    layer->m_pTiles[currentTileIndex] = gid;
                    
                    if (currentTileIndex != tilesAmount - 1)
                    {
                        --layer->m_pTiles[tilesAmount - 1];
                    }
                }
                else if(tilesAmount == 1)
                {
                    if (layer->m_pTiles[0] == 0)
                    {
                        layer->m_pTiles[0] = gid;
                    }
                }
            } while (0);
        }
        else
        {
            TMXTilesetInfo* info = (TMXTilesetInfo*)pTMXMapInfo->getTilesets()->getLastObject();
            Dictionary *dict = new Dictionary();
            dict->init();
            pTMXMapInfo->setParentGID(info->m_uFirstGid + kdAtoi(valueForKey("id", attributeDict)));
            pTMXMapInfo->getTileProperties()->setObject(dict, pTMXMapInfo->getParentGID());
            CC_SAFE_RELEASE(dict);

            pTMXMapInfo->setParentElement(TMXPropertyTile);
        }
    }
    else if (elementName == "layer")
    {
        TMXLayerInfo *layer = new TMXLayerInfo();
        layer->m_sName = valueForKey("name", attributeDict);

        Size s;
        s.width = kdAtof(valueForKey("width", attributeDict));
        s.height = kdAtof(valueForKey("height", attributeDict));
        layer->m_tLayerSize = s;

        std::string visible = valueForKey("visible", attributeDict);
        layer->m_bVisible = !(visible == "0");

        std::string opacity = valueForKey("opacity", attributeDict);
        if( opacity != "" )
        {
            layer->m_cOpacity = (unsigned char)(255 * kdAtof(opacity.c_str()));
        }
        else
        {
            layer->m_cOpacity = 255;
        }

        float x = kdAtof(valueForKey("x", attributeDict));
        float y = kdAtof(valueForKey("y", attributeDict));
        layer->m_tOffset = Point(x,y);

        pTMXMapInfo->getLayers()->addObject(layer);
        layer->release();

        // The parent element is now "layer"
        pTMXMapInfo->setParentElement(TMXPropertyLayer);

    } 
    else if (elementName == "objectgroup")
    {
        TMXObjectGroup *objectGroup = new TMXObjectGroup();
        objectGroup->setGroupName(valueForKey("name", attributeDict));
        Point positionOffset;
        positionOffset.x = kdAtof(valueForKey("x", attributeDict)) * pTMXMapInfo->getTileSize().width;
        positionOffset.y = kdAtof(valueForKey("y", attributeDict)) * pTMXMapInfo->getTileSize().height;
        objectGroup->setPositionOffset(positionOffset);

        pTMXMapInfo->getObjectGroups()->addObject(objectGroup);
        objectGroup->release();

        // The parent element is now "objectgroup"
        pTMXMapInfo->setParentElement(TMXPropertyObjectGroup);

    }
    else if (elementName == "image")
    {
        TMXTilesetInfo* tileset = (TMXTilesetInfo*)pTMXMapInfo->getTilesets()->getLastObject();

        // build full path
        std::string imagename = valueForKey("source", attributeDict);

        if (m_sTMXFileName.find_last_of("/") != string::npos)
        {
            string dir = m_sTMXFileName.substr(0, m_sTMXFileName.find_last_of("/") + 1);
            tileset->m_sSourceImage = dir + imagename;
        }
        else 
        {
            tileset->m_sSourceImage = m_sResources + (m_sResources.size() ? "/" : "") + imagename;
        }
    } 
    else if (elementName == "data")
    {
        std::string encoding = valueForKey("encoding", attributeDict);
        std::string compression = valueForKey("compression", attributeDict);

        if (encoding == "")
        {
            pTMXMapInfo->setLayerAttribs(pTMXMapInfo->getLayerAttribs() | TMXLayerAttribNone);
            
            TMXLayerInfo* layer = (TMXLayerInfo*)pTMXMapInfo->getLayers()->getLastObject();
            Size layerSize = layer->m_tLayerSize;
            int tilesAmount = layerSize.width*layerSize.height;

            int *tiles = (int *) malloc(tilesAmount*sizeof(int));
            for (int i = 0; i < tilesAmount; i++)
            {
                tiles[i] = 0;
            }
            
            /* Save the special index in tiles[tilesAmount - 1];
             * When we load tiles, we can do this:
             * tiles[tilesAmount - tiles[tilesAmount - 1] - 1] = tileNum;
             * --tiles[tilesAmount - 1];
             * We do this because we can easily contorl how much tiles we loaded without add a "curTilesAmount" into class member.
             */
            if (tilesAmount > 1)
            {
                tiles[tilesAmount - 1] = tilesAmount - 1;
            }

            layer->m_pTiles = (unsigned int*) tiles;
        }
        else if (encoding == "base64")
        {
            int layerAttribs = pTMXMapInfo->getLayerAttribs();
            pTMXMapInfo->setLayerAttribs(layerAttribs | TMXLayerAttribBase64);
            pTMXMapInfo->setStoringCharacters(true);

            if( compression == "gzip" )
            {
                layerAttribs = pTMXMapInfo->getLayerAttribs();
                pTMXMapInfo->setLayerAttribs(layerAttribs | TMXLayerAttribGzip);
            } else
            if (compression == "zlib")
            {
                layerAttribs = pTMXMapInfo->getLayerAttribs();
                pTMXMapInfo->setLayerAttribs(layerAttribs | TMXLayerAttribZlib);
            }
            CCASSERT( compression == "" || compression == "gzip" || compression == "zlib", "TMX: unsupported compression method" );
        }

    } 
    else if (elementName == "object")
    {
        char buffer[32] = {0};
        TMXObjectGroup* objectGroup = (TMXObjectGroup*)pTMXMapInfo->getObjectGroups()->getLastObject();

        // The value for "type" was blank or not a valid class name
        // Create an instance of TMXObjectInfo to store the object and its properties
        Dictionary *dict = new Dictionary();
        dict->init();
        // Parse everything automatically
        const char* pArray[] = {"name", "type", "width", "height", "gid"};
        
        for(size_t i = 0; i < sizeof(pArray)/sizeof(pArray[0]); ++i )
        {
            const char* key = pArray[i];
            String* obj = new String(valueForKey(key, attributeDict));
            if( obj )
            {
                obj->autorelease();
                dict->setObject(obj, key);
            }
        }

        // But X and Y since they need special treatment
        // X

        const char* value = valueForKey("x", attributeDict);
        if (value) 
        {
            int x = kdAtoi(value) + (int)objectGroup->getPositionOffset().x;
            sprintf(buffer, "%d", x);
            String* pStr = new String(buffer);
            pStr->autorelease();
            dict->setObject(pStr, "x");
        }

        // Y
        value = valueForKey("y", attributeDict);
        if (value)  {
            int y = kdAtoi(value) + (int)objectGroup->getPositionOffset().y;

            // Correct y position. (Tiled uses Flipped, cocos2d uses Standard)
            y = (int)(m_tMapSize.height * m_tTileSize.height) - y - kdAtoi(valueForKey("height", attributeDict));
            sprintf(buffer, "%d", y);
            String* pStr = new String(buffer);
            pStr->autorelease();
            dict->setObject(pStr, "y");
        }

        // Add the object to the objectGroup
        objectGroup->getObjects()->addObject(dict);
        dict->release();

         // The parent element is now "object"
         pTMXMapInfo->setParentElement(TMXPropertyObject);

    } 
    else if (elementName == "property")
    {
        if ( pTMXMapInfo->getParentElement() == TMXPropertyNone ) 
        {
            CCLOG( "TMX tile map: Parent element is unsupported. Cannot add property named '%s' with value '%s'",
                valueForKey("name", attributeDict), valueForKey("value",attributeDict) );
        } 
        else if ( pTMXMapInfo->getParentElement() == TMXPropertyMap )
        {
            // The parent element is the map
            String *value = new String(valueForKey("value", attributeDict));
            std::string key = valueForKey("name", attributeDict);
            pTMXMapInfo->getProperties()->setObject(value, key.c_str());
            value->release();

        } 
        else if ( pTMXMapInfo->getParentElement() == TMXPropertyLayer )
        {
            // The parent element is the last layer
            TMXLayerInfo* layer = (TMXLayerInfo*)pTMXMapInfo->getLayers()->getLastObject();
            String *value = new String(valueForKey("value", attributeDict));
            std::string key = valueForKey("name", attributeDict);
            // Add the property to the layer
            layer->getProperties()->setObject(value, key.c_str());
            value->release();

        } 
        else if ( pTMXMapInfo->getParentElement() == TMXPropertyObjectGroup ) 
        {
            // The parent element is the last object group
            TMXObjectGroup* objectGroup = (TMXObjectGroup*)pTMXMapInfo->getObjectGroups()->getLastObject();
            String *value = new String(valueForKey("value", attributeDict));
            const char* key = valueForKey("name", attributeDict);
            objectGroup->getProperties()->setObject(value, key);
            value->release();

        } 
        else if ( pTMXMapInfo->getParentElement() == TMXPropertyObject )
        {
            // The parent element is the last object
            TMXObjectGroup* objectGroup = (TMXObjectGroup*)pTMXMapInfo->getObjectGroups()->getLastObject();
            Dictionary* dict = (Dictionary*)objectGroup->getObjects()->getLastObject();

            const char* propertyName = valueForKey("name", attributeDict);
            String *propertyValue = new String(valueForKey("value", attributeDict));
            dict->setObject(propertyValue, propertyName);
            propertyValue->release();
        } 
        else if ( pTMXMapInfo->getParentElement() == TMXPropertyTile ) 
        {
            Dictionary* dict = (Dictionary*)pTMXMapInfo->getTileProperties()->objectForKey(pTMXMapInfo->getParentGID());

            const char* propertyName = valueForKey("name", attributeDict);
            String *propertyValue = new String(valueForKey("value", attributeDict));
            dict->setObject(propertyValue, propertyName);
            propertyValue->release();
        }
    }
    else if (elementName == "polygon") 
    {
        // find parent object's dict and add polygon-points to it
        TMXObjectGroup* objectGroup = (TMXObjectGroup*)m_pObjectGroups->getLastObject();
        Dictionary* dict = (Dictionary*)objectGroup->getObjects()->getLastObject();

        // get points value string
        const char* value = valueForKey("points", attributeDict);
        if(value)
        {
            Array* pointsArray = Array::createWithCapacity(10);

            // parse points string into a space-separated set of points
            stringstream pointsStream(value);
            string pointPair;
            while(std::getline(pointsStream, pointPair, ' '))
            {
                // parse each point combo into a comma-separated x,y point
                stringstream pointStream(pointPair);
                string xStr,yStr;
                char buffer[32] = {0};
                
                Dictionary* pointDict = new Dictionary;
                pointDict->init();

                // set x
                if(std::getline(pointStream, xStr, ','))
                {
                    int x = kdAtoi(xStr.c_str()) + (int)objectGroup->getPositionOffset().x;
                    sprintf(buffer, "%d", x);
                    String* pStr = new String(buffer);
                    pStr->autorelease();
                    pointDict->setObject(pStr, "x");
                }

                // set y
                if(std::getline(pointStream, yStr, ','))
                {
                    int y = kdAtoi(yStr.c_str()) + (int)objectGroup->getPositionOffset().y;
                    sprintf(buffer, "%d", y);
                    String* pStr = new String(buffer);
                    pStr->autorelease();
                    pointDict->setObject(pStr, "y");
                }
                
                // add to points array
                pointsArray->addObject(pointDict);
                pointDict->release();
            }
            
            dict->setObject(pointsArray, "points");
        }
    } 
    else if (elementName == "polyline")
    {
        // find parent object's dict and add polyline-points to it
        TMXObjectGroup* objectGroup = (TMXObjectGroup*)m_pObjectGroups->getLastObject();
        Dictionary* dict = (Dictionary*)objectGroup->getObjects()->getLastObject();
        
        // get points value string
        const char* value = valueForKey("points", attributeDict);
        if(value)
        {
            Array* pointsArray = Array::createWithCapacity(10);
            
            // parse points string into a space-separated set of points
            stringstream pointsStream(value);
            string pointPair;
            while(std::getline(pointsStream, pointPair, ' '))
            {
                // parse each point combo into a comma-separated x,y point
                stringstream pointStream(pointPair);
                string xStr,yStr;
                char buffer[32] = {0};
                
                Dictionary* pointDict = new Dictionary;
                pointDict->init();
                
                // set x
                if(std::getline(pointStream, xStr, ','))
                {
                    int x = kdAtoi(xStr.c_str()) + (int)objectGroup->getPositionOffset().x;
                    sprintf(buffer, "%d", x);
                    String* pStr = new String(buffer);
                    pStr->autorelease();
                    pointDict->setObject(pStr, "x");
                }
                
                // set y
                if(std::getline(pointStream, yStr, ','))
                {
                    int y = kdAtoi(yStr.c_str()) + (int)objectGroup->getPositionOffset().y;
                    sprintf(buffer, "%d", y);
                    String* pStr = new String(buffer);
                    pStr->autorelease();
                    pointDict->setObject(pStr, "y");
                }
                
                // add to points array
                pointsArray->addObject(pointDict);
                pointDict->release();
            }
            
            dict->setObject(pointsArray, "polylinePoints");
        }
    }

    if (attributeDict)
    {
        attributeDict->clear();
        delete attributeDict;
    }
}

void TMXMapInfo::endElement(void *ctx, const char *name)
{
    CC_UNUSED_PARAM(ctx);
    TMXMapInfo *pTMXMapInfo = this;
    std::string elementName = (char*)name;

    int len = 0;

    if(elementName == "data")
    {
        if (pTMXMapInfo->getLayerAttribs() & TMXLayerAttribBase64)
        {
            pTMXMapInfo->setStoringCharacters(false);
            
            TMXLayerInfo* layer = (TMXLayerInfo*)pTMXMapInfo->getLayers()->getLastObject();
            
            std::string currentString = pTMXMapInfo->getCurrentString();
            unsigned char *buffer;
            len = base64Decode((unsigned char*)currentString.c_str(), (unsigned int)currentString.length(), &buffer);
            if( ! buffer )
            {
                CCLOG("cocos2d: TiledMap: decode data error");
                return;
            }
            
            if( pTMXMapInfo->getLayerAttribs() & (TMXLayerAttribGzip | TMXLayerAttribZlib) )
            {
                unsigned char *deflated;
                Size s = layer->m_tLayerSize;
                // int sizeHint = s.width * s.height * sizeof(uint32_t);
                int sizeHint = (int)(s.width * s.height * sizeof(unsigned int));
                
                int inflatedLen = ZipUtils::inflateMemoryWithHint(buffer, len, &deflated, sizeHint);
                CCASSERT(inflatedLen == sizeHint, "");
                
                inflatedLen = (size_t)&inflatedLen; // XXX: to avoid warnings in compiler
                
                free(buffer);
                buffer = NULL;
                
                if( ! deflated )
                {
                    CCLOG("cocos2d: TiledMap: inflate data error");
                    return;
                }
                
                layer->m_pTiles = (unsigned int*) deflated;
            }
            else
            {
                layer->m_pTiles = (unsigned int*) buffer;
            }
            
            pTMXMapInfo->setCurrentString("");
        }
        else if (pTMXMapInfo->getLayerAttribs() & TMXLayerAttribNone)
        {
            TMXLayerInfo* layer = (TMXLayerInfo*)pTMXMapInfo->getLayers()->getLastObject();
            Size layerSize = layer->m_tLayerSize;
            int tilesAmount = layerSize.width * layerSize.height;
            
            //reset the layer->_tiles[tilesAmount - 1]
            if (tilesAmount > 1 && layer->m_pTiles[tilesAmount - 2] == 0)
            {
                layer->m_pTiles[tilesAmount - 1] = 0;
            }
        }

    }
    else if (elementName == "map")
    {
        // The map element has ended
        pTMXMapInfo->setParentElement(TMXPropertyNone);
    }    
    else if (elementName == "layer")
    {
        // The layer element has ended
        pTMXMapInfo->setParentElement(TMXPropertyNone);
    }
    else if (elementName == "objectgroup")
    {
        // The objectgroup element has ended
        pTMXMapInfo->setParentElement(TMXPropertyNone);
    } 
    else if (elementName == "object") 
    {
        // The object element has ended
        pTMXMapInfo->setParentElement(TMXPropertyNone);
    }
}

void TMXMapInfo::textHandler(void *ctx, const char *ch, int len)
{
    CC_UNUSED_PARAM(ctx);
    TMXMapInfo *pTMXMapInfo = this;
    std::string pText((char*)ch,0,len);

    if (pTMXMapInfo->isStoringCharacters())
    {
        std::string currentString = pTMXMapInfo->getCurrentString();
        currentString += pText;
        pTMXMapInfo->setCurrentString(currentString.c_str());
    }
}

NS_CC_END

