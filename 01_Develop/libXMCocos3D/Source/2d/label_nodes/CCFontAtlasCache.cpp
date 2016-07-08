/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontAtlasCache.cpp
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

#include "2d/label_nodes/CCFontAtlasCache.h"
#include "2d/label_nodes/CCFontAtlasFactory.h"
#include <sstream>

NS_CC_BEGIN

std::unordered_map<std::string, FontAtlas *> FontAtlasCache::m_aAtlasMap;

FontAtlas * FontAtlasCache::getFontAtlasTTF(const char *fontFileName, int size, GlyphCollection glyphs, const char *customGlyphs)
{
    std::string atlasName = generateFontName(fontFileName, size, glyphs);
    FontAtlas  *tempAtlas = m_aAtlasMap[atlasName];
    
    if ( !tempAtlas )
    {
        tempAtlas = FontAtlasFactory::createAtlasFromTTF(fontFileName, size, glyphs, customGlyphs);
        if (tempAtlas)
            m_aAtlasMap[atlasName] = tempAtlas;
    }
    else
    {
        tempAtlas->retain();
    }
    
    return tempAtlas;
}

FontAtlas * FontAtlasCache::getFontAtlasFNT(const char *fontFileName)
{
    std::string atlasName = generateFontName(fontFileName, 0, GlyphCollection::CUSTOM);
    FontAtlas *tempAtlas = m_aAtlasMap[atlasName];
    
    if ( !tempAtlas )
    {
        tempAtlas = FontAtlasFactory::createAtlasFromFNT(fontFileName);
        if (tempAtlas)
            m_aAtlasMap[atlasName] = tempAtlas;
    }
    else
    {
        tempAtlas->retain();
    }
    
    return tempAtlas;
}

std::string FontAtlasCache::generateFontName(const char *fontFileName, int size, GlyphCollection theGlyphs)
{
    std::string tempName(fontFileName);
    
    switch (theGlyphs)
    {
        case GlyphCollection::DYNAMIC:
            tempName.append("_DYNAMIC_");
        break;
            
        case GlyphCollection::NEHE:
            tempName.append("_NEHE_");
            break;
            
        case GlyphCollection::ASCII:
            tempName.append("_ASCII_");
            break;
            
        case GlyphCollection::CUSTOM:
            tempName.append("_CUSTOM_");
            break;
            
        default:
            break;
    }
    
    // std::to_string is not supported on android, using std::stringstream instead.
    std::stringstream ss;
    ss << size;
    return  tempName.append(ss.str());
}

bool FontAtlasCache::releaseFontAtlas(FontAtlas *atlas)
{
    if (atlas)
    {
        for( auto &item: m_aAtlasMap )
        {
            if ( item.second == atlas )
            {
                bool removeFromList = false;
                if(item.second->isSingleReference())
                    removeFromList = true;
                
                item.second->release();
                
                if (removeFromList)
                    m_aAtlasMap.erase(item.first);
                
                return true;
            }
        }
    }
    
    return false;
}

NS_CC_END