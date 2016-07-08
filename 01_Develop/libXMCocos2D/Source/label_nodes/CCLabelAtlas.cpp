/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelAtlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "label_nodes/CCLabelAtlas.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCDictionary.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "textures/CCTextureAtlas.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN
    
//
// CCLabelAtlas - Creation & Init
//
CCLabelAtlas::CCLabelAtlas ( KDvoid )
{
	m_sString = "";
}

CCLabelAtlas::~CCLabelAtlas ( KDvoid )
{
	m_sString.clear ( ); 
}

CCLabelAtlas* CCLabelAtlas::create ( const KDchar* szString, const KDchar* szCharMapFile, KDuint uItemWidth, KDuint uItemHeight, KDuint uStartCharMap )
{
	CCLabelAtlas*  pRet = new CCLabelAtlas ( );

	if ( pRet && pRet->initWithString ( szString, szCharMapFile, uItemWidth, uItemHeight, uStartCharMap ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCLabelAtlas::initWithString ( const KDchar* szString, const KDchar* szCharMapFile, KDuint uItemWidth, KDuint uItemHeight, KDuint uStartCharMap )
{
    CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szCharMapFile );
	return initWithString ( szString, pTexture, uItemWidth, uItemHeight, uStartCharMap );
}

KDbool CCLabelAtlas::initWithString ( const KDchar* szString, CCTexture2D* pTexture, KDuint uItemWidth, KDuint uItemHeight, KDuint uStartCharMap )
{
	CCAssert ( szString != KD_NULL, "" );

	if ( CCAtlasNode::initWithTexture ( pTexture, uItemWidth, uItemHeight, kdStrlen ( szString ) ) )
	{
		m_uMapStartChar = uStartCharMap;
		this->setString ( szString );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCLabelAtlas* CCLabelAtlas::create ( const KDchar* szString, const KDchar* szFNTFile )
{
	CCLabelAtlas*  pRet = new CCLabelAtlas ( );

	if ( pRet && pRet->initWithString ( szString, szFNTFile ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCLabelAtlas::initWithString ( const KDchar* szString, const KDchar* szFNTFile )
{
	std::string		sPathStr = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFNTFile );
	std::string		sRelPathStr = sPathStr.substr ( 0, sPathStr.find_last_of ( "/" ) ) + "/";
	CCDictionary*	pDict = CCDictionary::createWithContentsOfFile ( sPathStr.c_str ( ) );

	CCAssert ( ( (CCString*) pDict->objectForKey ( "version" ) )->intValue ( ) == 1, "Unsupported version. Upgrade XMCocos2D version" );

	std::string		sTexturePathStr = sRelPathStr + ( (CCString*) pDict->objectForKey ( "textureFilename" ) )->getCString ( );
	CCString*		pTextureFilename = CCString::create ( sTexturePathStr );

    KDuint  uWidth		= (KDuint) ( ( (CCString*) pDict->objectForKey ( "itemWidth"  ) )->intValue ( ) / CC_CONTENT_SCALE_FACTOR ( ) );
    KDuint  uHeight		= (KDuint) ( ( (CCString*) pDict->objectForKey ( "itemHeight" ) )->intValue ( ) / CC_CONTENT_SCALE_FACTOR ( ) );
    KDuint  uStartChar	= ( (CCString*) pDict->objectForKey ( "firstChar"  ) )->intValue ( );

	this->initWithString ( szString, pTextureFilename->getCString ( ), uWidth, uHeight, uStartChar );
   
    return KD_TRUE;
}

//
// CCLabelAtlas - Atlas generation
//
KDvoid CCLabelAtlas::updateAtlasValues ( KDvoid )
{
	const KDuchar*		szString = (const KDuchar*) m_sString.c_str ( );
	KDuint				uLen = m_sString.length ( );

    CCTexture2D*		pTexture = m_pTextureAtlas->getTexture ( );
    KDfloat				fPixelsWide = (KDfloat) pTexture->getPixelsWide ( );
    KDfloat				fPixelsHigh = (KDfloat) pTexture->getPixelsHigh ( );
    KDfloat				fItemWidthInPixels  = m_uItemWidth  * CC_CONTENT_SCALE_FACTOR ( );
    KDfloat				fItemHeightInPixels = m_uItemHeight * CC_CONTENT_SCALE_FACTOR ( );

	if ( m_bIgnoreContentScaleFactor )
	{
		fItemWidthInPixels  = (KDfloat) m_uItemWidth;
		fItemHeightInPixels = (KDfloat) m_uItemHeight;
	}

	CCAssert ( uLen <= m_pTextureAtlas->getCapacity ( ), "updateAtlasValues: Invalid String length" );
	ccV3F_C4B_T2F_Quad*		pQuads = m_pTextureAtlas->getQuads ( );

	for ( KDuint i = 0; i < uLen; i++ ) 
	{
		KDuchar  cCharMap = szString [ i ] - m_uMapStartChar;

		KDfloat  fRow = (KDfloat) ( cCharMap % m_uItemsPerRow );
		KDfloat  fCol = (KDfloat) ( cCharMap / m_uItemsPerRow );

        KDfloat  fLeft   = fRow  * fItemWidthInPixels / fPixelsWide;
        KDfloat  fRight  = fLeft + fItemWidthInPixels / fPixelsWide;
        KDfloat  fTop    = fCol  * fItemHeightInPixels / fPixelsHigh;
        KDfloat  fBottom = fTop  + fItemHeightInPixels / fPixelsHigh;

        pQuads [ i ].tl.texCoords.u = fLeft;
        pQuads [ i ].tl.texCoords.v = fTop;

        pQuads [ i ].tr.texCoords.u = fRight;
        pQuads [ i ].tr.texCoords.v = fTop;

        pQuads [ i ].bl.texCoords.u = fLeft;
        pQuads [ i ].bl.texCoords.v = fBottom;

        pQuads [ i ].br.texCoords.u = fRight;
        pQuads [ i ].br.texCoords.v = fBottom;

		pQuads [ i ].bl.vertices.x = (KDfloat) ( i * m_uItemWidth );
		pQuads [ i ].bl.vertices.y = 0.0f;
		pQuads [ i ].bl.vertices.z = 0.0f;

		pQuads [ i ].br.vertices.x = (KDfloat) ( i * m_uItemWidth + m_uItemWidth );
		pQuads [ i ].br.vertices.y = 0.0f;
		pQuads [ i ].br.vertices.z = 0.0f;

		pQuads [ i ].tl.vertices.x = (KDfloat) ( i * m_uItemWidth );
		pQuads [ i ].tl.vertices.y = (KDfloat) ( m_uItemHeight );
		pQuads [ i ].tl.vertices.z = 0.0f;

		pQuads [ i ].tr.vertices.x = (KDfloat) ( i * m_uItemWidth + m_uItemWidth );
		pQuads [ i ].tr.vertices.y = (KDfloat) ( m_uItemHeight);
		pQuads [ i ].tr.vertices.z = 0.0f;

		ccColor4B  tColor = { m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity };

        pQuads [ i ].tl.colors = tColor;
        pQuads [ i ].tr.colors = tColor;
        pQuads [ i ].bl.colors = tColor;
        pQuads [ i ].br.colors = tColor;
	}

	if ( uLen > 0 ) 
	{
		m_pTextureAtlas->setDirty ( KD_TRUE );
		KDuint		uTotalQuads = m_pTextureAtlas->getTotalQuads ( );
		if ( uLen > uTotalQuads ) 
		{
			m_pTextureAtlas->increaseTotalQuadsWith ( uLen - uTotalQuads );
		}
	}
}

//
// CCLabelAtlas - CCLabelProtocol
//
KDvoid CCLabelAtlas::setString ( const KDchar* szText )
{
	KDuint  uLen = kdStrlen ( szText );

	if ( uLen > m_pTextureAtlas->getTotalQuads ( ) )
	{
		m_pTextureAtlas->resizeCapacity ( uLen );
	}

	m_sString.clear ( );
	m_sString = szText;

	this->updateAtlasValues ( );
	this->setContentSize ( ccs ( uLen * m_uItemWidth, m_uItemHeight ) );

	m_uQuadsToDraw = uLen;
}

const KDchar* CCLabelAtlas::getString ( KDvoid )
{
	return m_sString.c_str ( );
}

// CCLabelAtlas - draw
KDvoid CCLabelAtlas::draw ( KDvoid )
{
    CCAtlasNode::draw ( );

	if ( m_bIsDebugDraw )
	{
		ccDrawRect ( ccpz, CCPoint ( this->getContentSize ( ) ) );
	}
}

NS_CC_END