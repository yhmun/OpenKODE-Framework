/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelBMFont.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 *      Use any of these editors to generate BMFonts:
 *
 *         http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 *         http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 *         http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 *         http://www.angelcode.com/products/bmfont/ (Free, Windows only)
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
#include "label_nodes/CCLabelBMFont.h"
#include "cocoa/CCDictionary.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"
#include "support/CCFileUtils.h"
#include "support/CCConfiguration.h"
#include "support/ccUTF8.h"
#include "textures/CCTextureCache.h"
#include <vector>

NS_CC_BEGIN
    
// The return value needs to be deleted by CC_SAFE_DELETE_ARRAY.
static KDushort*  copyUTF16StringN ( KDushort* str )
{
    KDint   length = str ? cc_wcslen ( str ) : 0;
    KDushort*  ret = new KDushort [ length + 1 ];
    
	for ( KDint i = 0; i < length; ++i )
	{
        ret[i] = str[i];
	}

    ret [ length ] = 0;
    return ret;
}

static CCDictionary*  l_pConfigurations = KD_NULL;

//
// FNTConfig Cache - free functions
//
CCBMFontConfiguration* FNTConfigLoadFile ( const KDchar* szFntFile )
{
	CCBMFontConfiguration*  pRet = KD_NULL;

	if ( l_pConfigurations == KD_NULL )
	{
		l_pConfigurations = new CCDictionary ( );
	}

	pRet = (CCBMFontConfiguration*) l_pConfigurations->objectForKey ( szFntFile );
	if ( pRet == KD_NULL )
	{
		pRet = CCBMFontConfiguration::create ( szFntFile );
		if ( pRet )
		{
			l_pConfigurations->setObject ( pRet, szFntFile );
		}
	}

	return pRet;
}

KDvoid FNTConfigRemoveCache ( KDvoid )
{
	if ( l_pConfigurations )
	{
		l_pConfigurations->removeAllObjects ( );
        CC_SAFE_RELEASE ( l_pConfigurations );
	}
}

//
// BitmapFontConfiguration
//

CCBMFontConfiguration* CCBMFontConfiguration::create ( const KDchar* szFntFile )
{
	CCBMFontConfiguration*  pRet = new CCBMFontConfiguration ( );

	if ( pRet && pRet->initWithFNTfile ( szFntFile ) )
	{
		pRet->autorelease ( );		
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCBMFontConfiguration::initWithFNTfile ( const KDchar* szFntFile )
{
	m_pKerningDictionary = KD_NULL;
	m_pFontDefDictionary = KD_NULL;
	m_pCharacterSet = this->parseConfigFile ( szFntFile );
    
    if ( !m_pCharacterSet )
	{
		return KD_FALSE;
	}

	return KD_TRUE;
}

std::set<KDuint>* CCBMFontConfiguration::getCharacterSet ( KDvoid ) const
{
    return m_pCharacterSet;
}

CCBMFontConfiguration::CCBMFontConfiguration ( KDvoid )
{
	m_pFontDefDictionary = KD_NULL;
    m_nCommonHeight		 = 0;
    m_pKerningDictionary = KD_NULL;
	m_pCharacterSet		 = KD_NULL;
}

CCBMFontConfiguration::~CCBMFontConfiguration ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCBMFontConfiguration = %p", this );

	this->purgeFontDefDictionary ( );
	this->purgeKerningDictionary ( );

	m_sAtlasName.clear ( );

	CC_SAFE_DELETE ( m_pCharacterSet );
}

const KDchar* CCBMFontConfiguration::description ( KDvoid )
{
    return CCString::createWithFormat 
	(
		"<CCBMFontConfiguration = " CC_FORMAT_PRINTF_SIZE_T " | Glphys:%d Kernings:%d | Image = %s>",
		(KDsize) this,
        HASH_COUNT ( m_pFontDefDictionary ),
        HASH_COUNT ( m_pKerningDictionary ),
        m_sAtlasName.c_str ( )
    )->getCString ( );
}

KDvoid CCBMFontConfiguration::purgeKerningDictionary ( KDvoid )
{
	tCCKerningHashElement*  pCurrent;
	while ( m_pKerningDictionary ) 
	{
		pCurrent = m_pKerningDictionary; 
		HASH_DEL ( m_pKerningDictionary, pCurrent );
		kdFree ( pCurrent );
	}
}

KDvoid CCBMFontConfiguration::purgeFontDefDictionary ( KDvoid )
{    
    tCCFontDefHashElement*  pCurrent;
	tCCFontDefHashElement*  pTemp;

    HASH_ITER ( hh, m_pFontDefDictionary, pCurrent, pTemp ) 
	{
        HASH_DEL ( m_pFontDefDictionary, pCurrent );
        kdFree ( pCurrent );
    }
}

std::set<KDuint>* CCBMFontConfiguration::parseConfigFile ( const KDchar* szControlFile )
{	
	std::string  sFullPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szControlFile );
    CCString*    pContents = CCString::createWithContentsOfFile ( sFullPath.c_str ( ) );

	std::set<KDuint>*  pValidCharsString = new std::set<KDuint> ( );

    if ( !pContents )
    {
		CCLOG ( "XMCocos2D : Error parsing FNTfile %s", szControlFile );
        return KD_NULL;
    }

    // parse spacing / padding
    std::string  sLine;
    std::string  sLeft = pContents->getCString ( );
    while ( sLeft.length ( ) > 0)
    {
        KDuint  nIndex = sLeft.find ( '\n' );

        if ( nIndex != std::string::npos )
        {
            // the data is more than a line.get one line
            sLine = sLeft.substr ( 0, nIndex );
            sLeft = sLeft.substr ( nIndex + 1 );
        }
        else
        {
            // get the left data
            sLine = sLeft;
            sLeft.erase ( );
        }

        if ( sLine.substr ( 0, kdStrlen ( "info face" ) ) == "info face" ) 
        {
            // XXX: info parsing is incomplete
            // Not needed for the Hiero editors, but needed for the AngelCode editor
            this->parseInfoArguments ( sLine );
        }
        // Check to see if the start of the line is something we are interested in
        else if ( sLine.substr ( 0, kdStrlen ( "common lineHeight" ) ) == "common lineHeight" )
        {
            this->parseCommonArguments ( sLine );
        }
        else if ( sLine.substr ( 0, kdStrlen ( "page id" ) ) == "page id" )
        {
            this->parseImageFileName ( sLine, szControlFile );
        }
        else if ( sLine.substr ( 0, kdStrlen ( "chars c" ) ) == "chars c" )
        {
            // Ignore this line
        }
        else if ( sLine.substr ( 0, kdStrlen ( "char" ) ) == "char" )
        {
            // Parse the current line and create a new CharDef
            tCCFontDefHashElement*  pElement = (tCCFontDefHashElement*) kdMalloc ( sizeof ( *pElement ) );
            this->parseCharacterDefinition ( sLine, &pElement->fontDef );

            pElement->key = pElement->fontDef.charID;
            HASH_ADD_INT ( m_pFontDefDictionary, key, pElement );

			pValidCharsString->insert ( pElement->fontDef.charID );
        }
//		else if ( sLine.substr ( 0, kdStrlen ( "kernings count" ) ) == "kernings count" )
//		{
//			this->parseKerningCapacity ( sLine );
//		}
        else if ( sLine.substr ( 0, kdStrlen ( "kerning first" ) ) == "kerning first" )
        {
            this->parseKerningEntry ( sLine );
        }
    }

	return pValidCharsString;
}

KDvoid CCBMFontConfiguration::parseImageFileName ( std::string sLine, const KDchar* szFntFile )
{
	//////////////////////////////////////////////////////////////////////////
	// line to parse:
	// page id=0 file="/res/font/bitmapFontTest.png"
	//////////////////////////////////////////////////////////////////////////

	// page ID. Sanity check
	KDint  nIndex  = sLine.find ( '=' ) + 1;
	KDint  nIndex2 = sLine.find ( ' ' , nIndex );
	std::string  sValue = sLine.substr ( nIndex, nIndex2 - nIndex );
	CCAssert ( kdStrtol ( sValue.c_str ( ), KD_NULL, 0 ) == 0, "LabelBMFont file could not be found" );

	// file 
	nIndex  = sLine.find ( '"' ) + 1;
	nIndex2 = sLine.find ( '"', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );

	m_sAtlasName = CCFileUtils::sharedFileUtils ( )->fullPathFromRelativeFile ( sValue.c_str ( ), szFntFile );
}

KDvoid CCBMFontConfiguration::parseInfoArguments ( std::string sLine )
{
	//////////////////////////////////////////////////////////////////////////
	// possible lines to parse:
	// info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
	// info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
	//////////////////////////////////////////////////////////////////////////

	// padding
	KDint  nIndex  = sLine.find ( "padding=" );
	KDint  nIndex2 = sLine.find ( ' ', nIndex );
	std::string  sValue = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "padding=%d,%d,%d,%d", &m_tPadding.top, &m_tPadding.right, &m_tPadding.bottom, &m_tPadding.left );
	CCLOGINFO ( "XMCocos2D : padding - %d,%d,%d,%d", m_tPadding.left, m_tPadding.top, m_tPadding.right, m_tPadding.bottom );
}


KDvoid CCBMFontConfiguration::parseCommonArguments ( std::string sLine )
{
	//////////////////////////////////////////////////////////////////////////
	// line to parse:
	// common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
	//////////////////////////////////////////////////////////////////////////

	// Height
	KDint  nIndex  = sLine.find ( "lineHeight=" );
	KDint  nIndex2 = sLine.find ( ' ', nIndex );
	std::string  sValue = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "lineHeight=%d", &m_nCommonHeight );
    
	// scaleW. sanity check
	nIndex  = sLine.find ( "scaleW=" ) + kdStrlen ( "scaleW=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	CCAssert ( kdStrtol ( sValue.c_str ( ), KD_NULL, 0 ) <= CCConfiguration::sharedConfiguration ( )->getMaxTextureSize ( ), "CCLabelBMFont: page can't be larger than supported" );

	// scaleH. sanity check
	nIndex  = sLine.find ( "scaleH=" ) + kdStrlen ( "scaleH=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	CCAssert ( kdStrtol ( sValue.c_str ( ), KD_NULL, 0 ) <= CCConfiguration::sharedConfiguration ( )->getMaxTextureSize ( ), "CCLabelBMFont: page can't be larger than supported" );

	// pages. sanity check
	nIndex  = sLine.find ( "pages=") + kdStrlen ( "pages=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	CCAssert ( kdStrtol ( sValue.c_str ( ), KD_NULL, 0 ) == 1, "CCBitfontAtlas: only supports 1 page" );

	// packed (ignore) What does this mean ??
}

KDvoid CCBMFontConfiguration::parseCharacterDefinition ( std::string sLine, ccBMFontDef* characterDefinition )
{	
	//////////////////////////////////////////////////////////////////////////
	// line to parse:
	// char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=44    xadvance=14     page=0  chnl=0 
	//////////////////////////////////////////////////////////////////////////

	// Character ID
	KDint  nIndex  = sLine.find ( "id=" );
	KDint  nIndex2 = sLine.find ( ' ', nIndex );
	std::string  sValue = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "id=%u", &characterDefinition->charID );

	// Character x
	nIndex  = sLine.find ( "x=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "x=%f", &characterDefinition->rect.origin.x );

	// Character y
	nIndex  = sLine.find ( "y=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "y=%f", &characterDefinition->rect.origin.y );

	// Character width
	nIndex  = sLine.find ( "width=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "width=%f", &characterDefinition->rect.size.width );

	// Character height
	nIndex  = sLine.find ( "height=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "height=%f", &characterDefinition->rect.size.height );

	// Character xoffset
	nIndex  = sLine.find ( "xoffset=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str(), "xoffset=%hd", &characterDefinition->xOffset );

	// Character yoffset
	nIndex  = sLine.find ( "yoffset=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "yoffset=%hd", &characterDefinition->yOffset );

	// Character xadvance
	nIndex  = sLine.find ( "xadvance=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "xadvance=%hd", &characterDefinition->xAdvance );
}

KDvoid CCBMFontConfiguration::parseKerningEntry ( std::string sLine )
{		
	//////////////////////////////////////////////////////////////////////////
	// line to parse:
	// kerning first=121  second=44  amount=-7
	//////////////////////////////////////////////////////////////////////////

	// first
	KDint  nFirst;
	KDint  nIndex  = sLine.find ( "first=" );
	KDint  nIndex2 = sLine.find ( ' ', nIndex );
	std::string  sValue = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "first=%d", &nFirst );

	// second
	KDint  nSecond;
	nIndex  = sLine.find ( "second=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "second=%d", &nSecond );

	// amount
	KDint  nAmount;
	nIndex  = sLine.find ( "amount=" );
	nIndex2 = sLine.find ( ' ', nIndex );
	sValue  = sLine.substr ( nIndex, nIndex2 - nIndex );
	kdSscanf ( sValue.c_str ( ), "amount=%d", &nAmount );

	tCCKerningHashElement*  pElement = (tCCKerningHashElement *) kdCalloc ( 1, sizeof ( *pElement ) );
	pElement->amount = nAmount;
	pElement->key = ( nFirst << 16 ) | ( nSecond & 0xffff );
	HASH_ADD_INT ( m_pKerningDictionary,key, pElement );

}

//
// CCLabelBMFont
//

//
// LabelBMFont - Purge Cache
//
KDvoid CCLabelBMFont::purgeCachedData ( KDvoid )
{
	FNTConfigRemoveCache ( );
}

CCLabelBMFont* CCLabelBMFont::create ( KDvoid )
{
    CCLabelBMFont*  pRet = new CCLabelBMFont ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );
	}
	else
	{
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

//
// LabelBMFont - Creation & Init
//
CCLabelBMFont* CCLabelBMFont::create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth, CCTextAlignment eAlignment )
{
	return CCLabelBMFont::create ( szText, szFntFile, fWidth, eAlignment, CCPointZero );
}

CCLabelBMFont* CCLabelBMFont::create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth )
{
	return CCLabelBMFont::create ( szText, szFntFile, fWidth, kCCTextAlignmentLeft, CCPointZero );
}

CCLabelBMFont* CCLabelBMFont::create ( const KDchar* szText, const KDchar* szFntFile )
{
	return CCLabelBMFont::create ( szText, szFntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero );
}

CCLabelBMFont* CCLabelBMFont::create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth, CCTextAlignment eAlignment, CCPoint tImageOffset )
{
	CCLabelBMFont*  pRet = new CCLabelBMFont ( );

    if ( pRet && pRet->initWithString ( szText, szFntFile, fWidth, eAlignment, tImageOffset ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool CCLabelBMFont::init ( KDvoid )
{
    return initWithString ( KD_NULL, KD_NULL, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero );
}

KDbool CCLabelBMFont::initWithString ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth, CCTextAlignment eAlignment, CCPoint tImageOffset )
{	
    CCAssert ( !m_pConfiguration, "re-init is no longer supported" );
    CCAssert ( ( szText && szFntFile ) || ( szText == NULL && szFntFile == NULL ), "Invalid params for CCLabelBMFont" );
    
    CCTexture2D*  pTexture = KD_NULL;
    
    if ( szFntFile )
    {
        CCBMFontConfiguration*  pNewConf = FNTConfigLoadFile ( szFntFile );
		if ( !pNewConf )
        {
            CCLOG ( "XMCocos2D : WARNING. CCLabelBMFont: Impossible to create font. Please check file: '%s'", szFntFile );
            release ( );
            return KD_FALSE;
        }

        pNewConf->retain ( );
        CC_SAFE_RELEASE ( m_pConfiguration );
        m_pConfiguration = pNewConf;
        
        m_sFntFile = szFntFile;
        
        pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( m_pConfiguration->getAtlasName ( ) );
    }
    else 
    {
        pTexture = new CCTexture2D ( );
        pTexture->autorelease ( );
    }

    if ( szText == KD_NULL )
    {
        szText = "";
    }

    if ( CCSpriteBatchNode::initWithTexture ( pTexture, kdStrlen ( szText ) ) )
    {
		m_fWidth		= fWidth;
        m_eAlignment	= eAlignment;

		m_cDisplayedOpacity = m_cRealOpacity = 255;
		m_tDisplayedColor = m_tRealColor = ccWHITE;
        m_bCascadeOpacityEnabled = KD_TRUE;
        m_bCascadeColorEnabled   = KD_TRUE;

		m_tContentSize	= CCSizeZero;

		m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture ( )->hasPremultipliedAlpha ( );
		m_tAnchorPoint  = ccp ( 0.5f, 0.5f );

        m_tImageOffset	= tImageOffset;
                            
        m_pReusedChar = new CCSprite ( );
        m_pReusedChar->initWithTexture ( m_pTextureAtlas->getTexture ( ), CCRectMake ( 0, 0, 0, 0 ), KD_FALSE );
        m_pReusedChar->setBatchNode ( this );

        this->setString ( szText, KD_TRUE );

        return KD_TRUE;
    }

    return KD_FALSE;
}

CCLabelBMFont::CCLabelBMFont ( KDvoid )
{
	m_pString					= KD_NULL;
	m_pInitialString			= KD_NULL;
	m_eAlignment				= kCCTextAlignmentCenter;
    m_fWidth					= -1.f;
    m_pConfiguration			= KD_NULL;
    m_bLineBreakWithoutSpaces	= KD_FALSE;
	m_tImageOffset				= ccpz;
	m_pReusedChar				= KD_NULL;
	m_cDisplayedOpacity			= 255;
	m_cRealOpacity				= 255;
	m_tDisplayedColor			= ccWHITE;
	m_tRealColor				= ccWHITE;
	m_bCascadeColorEnabled		= KD_TRUE;
	m_bCascadeOpacityEnabled	= KD_TRUE;
	m_bIsOpacityModifyRGB		= KD_FALSE;
}

CCLabelBMFont::~CCLabelBMFont ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pReusedChar );  
	CC_SAFE_DELETE_ARRAY ( m_pString );
	CC_SAFE_DELETE_ARRAY ( m_pInitialString );
	CC_SAFE_RELEASE ( m_pConfiguration );
}

//
// LabelBMFont - Atlas generation
//
KDint CCLabelBMFont::kerningAmountForFirst ( KDuint16 uFirst, KDuint16 uSecond )
{
	KDint   nRet = 0;
	KDuint  uKey = ( uFirst << 16 ) | ( uSecond & 0xffff );

	if ( m_pConfiguration->m_pKerningDictionary )
	{
		tCCKerningHashElement*  pElement = KD_NULL;
		HASH_FIND_INT ( m_pConfiguration->m_pKerningDictionary, &uKey, pElement );		
		if ( pElement )
		{
			nRet = pElement->amount;
		}
	}
	
	return nRet;
}

KDvoid CCLabelBMFont::createFontChars ( KDvoid )
{
	KDint     nNextFontPositionX = 0;
    KDint     nNextFontPositionY = 0;
	KDushort  uPrev				 = -1;
	KDint     nKerningAmount	 = 0;

	CCSize    tTempSize = CCSizeZero;

    KDint     nLongestLine = 0;
    KDuint    uTotalHeight = 0;

    KDuint    uQuantityOfLines = 1;

	KDuint    uLen = m_pString ? cc_wcslen ( m_pString ) : 0;
    if ( 0 == uLen )
    {
        return;
    }

	std::set<KDuint>*  pCharSet = m_pConfiguration->getCharacterSet ( );
    for ( KDuint i = 0; i < uLen - 1; ++i )
    {
        KDushort  uChar = m_pString [ i ];
        if ( uChar == '\n' )
        {
            uQuantityOfLines++;
        }
    }

    uTotalHeight = m_pConfiguration->m_nCommonHeight * uQuantityOfLines;
    nNextFontPositionY = 0 - ( m_pConfiguration->m_nCommonHeight - m_pConfiguration->m_nCommonHeight * uQuantityOfLines );

	CCRect       tRect;
    ccBMFontDef  tFontDef;

	for ( KDuint i = 0; i < uLen; i++ )
	{
		KDushort   uChar = m_pString [ i ];

        if ( uChar == '\n' )
        {
            nNextFontPositionX = 0;
            nNextFontPositionY -= m_pConfiguration->m_nCommonHeight;
            continue;
        }

		if ( pCharSet->find ( uChar ) == pCharSet->end ( ) )
        {
            CCLOGWARN ( "CCLabelBMFont: Attempted to use character not defined in this bitmap: %d", uChar );
            continue;      
        }

		nKerningAmount = this->kerningAmountForFirst ( uPrev, uChar );

		tCCFontDefHashElement*  pElement = KD_NULL;
        
		// unichar is a short, and an int is needed on HASH_FIND_INT
        KDuint  uKey = uChar;
        HASH_FIND_INT ( m_pConfiguration->m_pFontDefDictionary, &uKey, pElement );
        if ( !pElement )
        {
            CCLOGWARN ( "XMCocos2D : LabelBMFont: characer not found %d", uChar );
            continue;
        }

		tFontDef = pElement->fontDef;        
		tRect = tFontDef.rect;
		tRect = CC_RECT_PIXELS_TO_POINTS ( tRect );

		tRect.origin.x += m_tImageOffset.x;
        tRect.origin.y += m_tImageOffset.y;

		CCSprite*  pFontChar;
		KDbool     bHasSprite = KD_TRUE;

		pFontChar = (CCSprite*) this->getChildByTag ( i );
		if ( pFontChar )
		{
			// Reusing previous Sprite
			pFontChar->setVisible ( KD_TRUE );
		}
		else
		{
			// New Sprite ? Set correct color, opacity, etc...
			if ( 0 )
            {
				// WIP: Doesn't support many features yet.
				// But this code is super fast. It doesn't create any sprite.
				// Ideal for big labels.
				//
				pFontChar = m_pReusedChar;
				pFontChar->setBatchNode ( KD_NULL );
				bHasSprite = KD_FALSE;
			}
            else
            {
                pFontChar = new CCSprite ( );
                pFontChar->initWithTexture ( m_pTextureAtlas->getTexture ( ), tRect );
                addChild ( pFontChar, i, i );
                pFontChar->release ( );
			}

			// Apply label properties
			pFontChar->setOpacityModifyRGB ( m_bIsOpacityModifyRGB );
            
			// Color MUST be set before opacity, since opacity might change color if OpacityModifyRGB is on
			pFontChar->updateDisplayedColor ( m_tDisplayedColor );
			pFontChar->updateDisplayedOpacity ( m_cDisplayedOpacity );
		}

		// updating previous sprite
        pFontChar->setTextureRect ( tRect, KD_FALSE, tRect.size );

		// See issue 1343. cast( signed short + unsigned integer ) == unsigned integer (sign is lost!)
        KDint    nOffsetY = m_pConfiguration->m_nCommonHeight - tFontDef.yOffset;
		CCPoint  tFontPos = ccp 
		(
			(KDfloat) nNextFontPositionX + tFontDef.xOffset + tFontDef.rect.size.cx * 0.5f + nKerningAmount,
			(KDfloat) nNextFontPositionY + nOffsetY - tRect.size.cy * 0.5f * CC_CONTENT_SCALE_FACTOR ( )
		);	
		pFontChar->setPosition ( CC_POINT_PIXELS_TO_POINTS ( tFontPos ) );

		// update kerning
		nNextFontPositionX += tFontDef.xAdvance + nKerningAmount;
		uPrev = uChar;

        if ( nLongestLine < nNextFontPositionX )
        {
            nLongestLine = nNextFontPositionX;
        }

		if ( !bHasSprite )
        {
            updateQuadFromSprite ( pFontChar, i );
        }
	}

    // If the last character processed has an xAdvance which is less that the width of the characters image, then we need
    // to adjust the width of the string to take this into account, or the character will overlap the end of the bounding
    // box
    if ( tFontDef.xAdvance < tFontDef.rect.size.cx )
    {
        tTempSize.cx = nLongestLine + tFontDef.rect.size.cx - tFontDef.xAdvance;
    }
    else
    {
        tTempSize.cx = (KDfloat) nLongestLine;
    }
    tTempSize.cy = (KDfloat) uTotalHeight;

	this->setContentSize ( CC_SIZE_PIXELS_TO_POINTS ( tTempSize ) );
}

//
// LabelBMFont - CCLabelProtocol protocol
//
KDvoid CCLabelBMFont::setString ( const KDchar* szNewString )
{
	this->setString ( szNewString, KD_TRUE );
}

KDvoid CCLabelBMFont::setString ( const KDchar* szNewString, KDbool bNeedUpdateLabel )
{	
    if ( szNewString == KD_NULL )
	{
        szNewString = "";
    }
    if ( bNeedUpdateLabel ) 
	{
        m_sInitialStringUTF8 = szNewString;
    }

    KDushort*  pUtf16String = cc_utf8_to_utf16 ( szNewString );
    setString ( pUtf16String, bNeedUpdateLabel );
    CC_SAFE_DELETE_ARRAY ( pUtf16String );
}

KDvoid CCLabelBMFont::setString ( KDushort* pNewString, KDbool bNeedUpdateLabel )
{
    if ( !bNeedUpdateLabel )
    {
        KDushort*  pTemp = m_pString;
        m_pString = copyUTF16StringN ( pNewString );
        CC_SAFE_DELETE_ARRAY ( pTemp );
    }
    else
    {
        KDushort*  pTemp = m_pInitialString;
        m_pInitialString = copyUTF16StringN ( pNewString );
        CC_SAFE_DELETE_ARRAY ( pTemp );
    }

	if ( m_pChildren && m_pChildren->count ( ) != 0 )
	{
		CCObject*  pObject;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pNode = (CCNode *) pObject;
			if ( pNode )
			{
				pNode->setVisible ( KD_FALSE );
			}
		}
	}

	this->createFontChars ( );

	if ( bNeedUpdateLabel )
	{
		updateLabel ( );
	}
}

const KDchar* CCLabelBMFont::getString ( KDvoid )
{
	return m_sInitialStringUTF8.c_str ( );
}

KDvoid CCLabelBMFont::setCString ( const KDchar* szText )
{
    setString ( szText );
}

//
// LabelBMFont - CCRGBAProtocol protocol
//
const ccColor3B& CCLabelBMFont::getColor ( KDvoid )
{
	return m_tRealColor;
}

const ccColor3B& CCLabelBMFont::getDisplayedColor ( KDvoid )
{
    return m_tDisplayedColor;
}

KDvoid CCLabelBMFont::setColor ( const ccColor3B& tColor )
{
	m_tDisplayedColor = m_tRealColor = tColor;
	
	if ( m_bCascadeColorEnabled )
	{
		ccColor3B  tParentColor = ccWHITE;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
        if ( pParent && pParent->isCascadeColorEnabled ( ) )
        {
            tParentColor = pParent->getDisplayedColor ( );
        }
        this->updateDisplayedColor ( tParentColor );
	}
}

GLubyte CCLabelBMFont::getOpacity ( KDvoid )
{
	return m_cRealOpacity;
}

GLubyte CCLabelBMFont::getDisplayedOpacity ( KDvoid )
{
    return m_cDisplayedOpacity;
}

// Override synthesized setOpacity to recurse items 
KDvoid CCLabelBMFont::setOpacity ( GLubyte cOpacity )
{
	m_cDisplayedOpacity = m_cRealOpacity = cOpacity;
    
	if ( m_bCascadeOpacityEnabled ) 
	{
		GLubyte  cParentOpacity = 255;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
        if ( pParent && pParent->isCascadeOpacityEnabled ( ) )
        {
            cParentOpacity = pParent->getDisplayedOpacity ( );
        }
        this->updateDisplayedOpacity ( cParentOpacity );
	}
}

KDvoid CCLabelBMFont::setOpacityModifyRGB ( KDbool bValue )
{
	m_bIsOpacityModifyRGB = bValue;

	if ( m_pChildren && m_pChildren->count ( ) != 0 )
	{
		CCObject*  pObject;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pNode = (CCNode *) pObject;
			if ( pNode )
			{
				CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( pNode );
				if ( pRGBAProtocol )
				{
					pRGBAProtocol->setOpacityModifyRGB ( m_bIsOpacityModifyRGB );
				}
			}
		}
	}
}

KDbool CCLabelBMFont::isOpacityModifyRGB ( KDvoid )
{
	return m_bIsOpacityModifyRGB;
}

KDvoid CCLabelBMFont::updateDisplayedOpacity ( GLubyte cParentOpacity )
{
	m_cDisplayedOpacity = (GLubyte) ( m_cRealOpacity * cParentOpacity / 255.0 );
    
	CCObject*  pObj;
	CCARRAY_FOREACH ( m_pChildren, pObj )
    {
        CCSprite*  pItem = (CCSprite*) pObj;
		pItem->updateDisplayedOpacity ( m_cDisplayedOpacity );
	}
}

KDvoid CCLabelBMFont::updateDisplayedColor ( const ccColor3B& tParentColor )
{
	m_tDisplayedColor.r = (GLubyte) ( m_tRealColor.r * tParentColor.r / 255.0 );
	m_tDisplayedColor.g = (GLubyte) ( m_tRealColor.g * tParentColor.g / 255.0 );
	m_tDisplayedColor.b = (GLubyte) ( m_tRealColor.b * tParentColor.b / 255.0 );
    
    CCObject*  pObj;
	CCARRAY_FOREACH ( m_pChildren, pObj )
    {
        CCSprite*  pItem = (CCSprite*) pObj;
		pItem->updateDisplayedColor ( m_tDisplayedColor );
	}
}

KDbool CCLabelBMFont::isCascadeColorEnabled ( KDvoid )
{
    return KD_FALSE;
}

KDvoid CCLabelBMFont::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{
    m_bCascadeColorEnabled = bCascadeColorEnabled;
}

KDbool CCLabelBMFont::isCascadeOpacityEnabled ( KDvoid )
{
    return KD_FALSE;
}

KDvoid CCLabelBMFont::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{
    m_bCascadeOpacityEnabled = bCascadeOpacityEnabled;
}

//
// LabelBMFont - AnchorPoint
//
KDvoid CCLabelBMFont::setAnchorPoint ( const CCPoint& tPoint )
{
	if ( !tPoint.equals ( m_tAnchorPoint ) )
	{
		CCSpriteBatchNode::setAnchorPoint ( tPoint );
		updateLabel ( );
	}
}

// LabelBMFont - Alignment
KDvoid CCLabelBMFont::updateLabel ( KDvoid )
{
	this->setString ( m_pInitialString, KD_FALSE );

	if ( m_fWidth > 0 )
	{
		// Step 1: Make multiline
		std::vector<KDushort>	str_whole = cc_utf16_vec_from_utf16_str ( m_pString );
		KDuint					stringLength = str_whole.size ( );
		std::vector<KDushort>	multiline_string;
		multiline_string.reserve ( stringLength );
		std::vector<KDushort>	last_word;
		last_word.reserve ( stringLength );

		KDuint	line = 1, i = 0;
		KDbool	start_line = KD_FALSE, start_word = KD_FALSE;
		KDfloat startOfLine = -1, startOfWord = -1;
		KDint	skip = 0;

		CCArray*  children = getChildren ( );
		for ( KDuint j = 0; j < children->count ( ); j++ )
		{
			CCSprite*  characterSprite;
			KDuint	   justSkipped = 0;
            
            while ( !( characterSprite = (CCSprite*) this->getChildByTag ( j + skip + justSkipped ) ) )
            {
                justSkipped++;
            }
            
            skip += justSkipped;

			if ( !characterSprite->isVisible ( ) )
			{
				continue;
			}

			if ( i >= stringLength )
			{
				break;
			}

			KDushort  character = str_whole[i];

			if ( !start_word )
			{
				startOfWord = getLetterPosXLeft ( characterSprite );
				start_word  = KD_TRUE;
			}

			if ( !start_line )
			{
				startOfLine = startOfWord;
				start_line  = KD_TRUE;
			}

			// Newline.
			if ( character == '\n' )
			{
				cc_utf8_trim_ws ( &last_word );

				last_word.push_back ( '\n' );
				multiline_string.insert ( multiline_string.end ( ), last_word.begin ( ), last_word.end ( ) );
				last_word.clear ( );
				start_word = KD_FALSE;
				start_line = KD_FALSE;
				startOfWord = -1;
				startOfLine = -1;
				i += justSkipped;
				line++;

				if ( i >= stringLength )
				{
					break;
				}

				character = str_whole[i];

				if ( !startOfWord )
				{
					startOfWord = getLetterPosXLeft ( characterSprite );
					start_word = KD_TRUE;
				}

				if ( !startOfLine )
				{
					startOfLine  = startOfWord;
					start_line = KD_TRUE;
				}
			}

			// Whitespace.
			if ( isspace_unicode ( character ) )
			{
				last_word.push_back ( character );
				multiline_string.insert ( multiline_string.end ( ), last_word.begin ( ), last_word.end ( ) );
				last_word.clear ( );
				start_word = KD_FALSE;
				startOfWord = -1;
				i++;
				continue;
			}

			// Out of bounds.
			if ( getLetterPosXRight ( characterSprite ) - startOfLine > m_fWidth )
			{
				if ( !m_bLineBreakWithoutSpaces )
				{
					last_word.push_back ( character );

					KDint  found = cc_utf8_find_last_not_char ( multiline_string, ' ' );
					if ( found != -1 )
					{
						cc_utf8_trim_ws ( &multiline_string );
					}
					else
					{
						multiline_string.clear ( );
					}

					if ( multiline_string.size ( ) > 0 )
					{
						multiline_string.push_back ( '\n' );
					}

					line++;
					start_line = KD_FALSE;
					startOfLine = -1;
					i++;
				}
				else
				{
					cc_utf8_trim_ws ( &last_word );

					last_word.push_back ( '\n' );
					multiline_string.insert ( multiline_string.end ( ), last_word.begin ( ), last_word.end ( ) );
					last_word.clear ( );
					start_word = KD_FALSE;
					start_line = KD_FALSE;
					startOfWord = -1;
					startOfLine = -1;
					line++;

					if ( i >= stringLength )
					{
						break;
					}

					if ( !startOfWord )
					{
						startOfWord = getLetterPosXLeft ( characterSprite );
						start_word = KD_TRUE;
					}

					if ( !startOfLine )
					{
						startOfLine = startOfWord;
						start_line = KD_TRUE;
					}

					j--;
				}

				continue;
			}
			else
			{
				// Character is normal.
				last_word.push_back ( character );
				i++;
				continue;
			}
		}

		multiline_string.insert ( multiline_string.end ( ), last_word.begin ( ), last_word.end ( ) );

		KDint  size = multiline_string.size ( );
		KDushort*  str_new = new KDushort [size + 1];

		for ( KDint i = 0; i < size; ++i )
		{
			str_new[i] = multiline_string[i];
		}

		str_new[size] = '\0';

		this->setString ( str_new, KD_FALSE );

		CC_SAFE_DELETE_ARRAY ( str_new );
	}

	// Step 2: Make alignment
	if ( !( m_eAlignment & kCCTextAlignmentLeft ) )
	{
		KDint  i = 0;

		KDint  lineNumber = 0;
		KDint  str_len = cc_wcslen ( m_pString );
		std::vector<KDushort>  last_line;
		for ( KDint ctr = 0; ctr <= str_len; ++ctr )
		{
			if ( m_pString[ctr] == '\n' || m_pString[ctr] == 0 )
			{
				KDfloat  lineWidth = 0.0f;
				KDint	 line_length = last_line.size ( );
				
				// if last line is empty we must just increase lineNumber and work with next line
                if ( line_length == 0 )
                {
                    lineNumber++;
                    continue;
                }

				KDint  index = i + line_length - 1 + lineNumber;
				if ( index < 0 )
				{
					continue;
				}

				CCSprite* lastChar = (CCSprite*) getChildByTag ( index );
				if ( lastChar == KD_NULL )
				{
                    continue;
				}

				lineWidth = lastChar->getPosition ( ).x + lastChar->getContentSize ( ).width / 2.0f;

				KDfloat shift = 0;
				switch ( m_eAlignment )
                {
					case kCCTextAlignmentCenter :
						shift = getContentSize ( ).width / 2.0f - lineWidth / 2.0f;
						break;

					case kCCTextAlignmentRight :
						shift = getContentSize ( ).width - lineWidth;
						break;

					default :
						break;
                }

				if ( shift != 0 )
				{
					for ( KDint  j = 0; j < line_length; j++ )
					{
						index = i + j + lineNumber;
						if ( index < 0 )
						{
							continue;
						}

						CCSprite*  characterSprite = (CCSprite*) getChildByTag ( index );
						characterSprite->setPosition ( ccpAdd ( characterSprite->getPosition ( ), ccp ( shift, 0.0f ) ) );
					}
				}

				i += line_length;
				lineNumber++;

				last_line.clear ( );
				continue;
			}

			last_line.push_back ( m_pString[ctr] );
		}
	}
}

// LabelBMFont - Alignment
KDvoid CCLabelBMFont::setAlignment ( CCTextAlignment eAlignment )
{
	this->m_eAlignment = eAlignment;
	updateLabel ( );
}

KDvoid CCLabelBMFont::setWidth ( KDfloat fWidth )
{
	this->m_fWidth = fWidth;
	updateLabel ( );
}

KDvoid CCLabelBMFont::setLineBreakWithoutSpace ( KDbool bBreakWithoutSpace )
{
	m_bLineBreakWithoutSpaces = bBreakWithoutSpace;
	updateLabel ( );
}

KDvoid CCLabelBMFont::setScale ( KDfloat fScale )
{
    CCSpriteBatchNode::setScale ( fScale );
    updateLabel ( );
}

KDvoid CCLabelBMFont::setScaleX ( KDfloat fScaleX )
{
    CCSpriteBatchNode::setScaleX ( fScaleX );
    updateLabel ( );
}

KDvoid CCLabelBMFont::setScaleY ( KDfloat fScaleY )
{
    CCSpriteBatchNode::setScaleY ( fScaleY );
    updateLabel ( );
}

KDfloat CCLabelBMFont::getLetterPosXLeft ( CCSprite* pSprite )
{
    return pSprite->getPosition ( ).x * m_tScale.x - 
		 ( pSprite->getContentSize ( ).cx * m_tScale.x * pSprite->getAnchorPoint ( ).x );
}

KDfloat CCLabelBMFont::getLetterPosXRight ( CCSprite* pSprite )
{
    return pSprite->getPosition ( ).x * m_tScale.x + 
		 ( pSprite->getContentSize ( ).cx * m_tScale.x * pSprite->getAnchorPoint ( ).x );
}

// LabelBMFont - FntFile
KDvoid CCLabelBMFont::setFntFile ( const KDchar* szFntFile )
{
    if ( szFntFile != KD_NULL && kdStrcmp ( szFntFile, m_sFntFile.c_str ( ) ) != 0 )
    {
        CCBMFontConfiguration*  pNewConf = FNTConfigLoadFile ( szFntFile );

        CCAssert ( pNewConf, "CCLabelBMFont: Impossible to create font. Please check file" );

        m_sFntFile = szFntFile;

        CC_SAFE_RETAIN  ( pNewConf );
        CC_SAFE_RELEASE ( m_pConfiguration );
        m_pConfiguration = pNewConf;

        this->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( m_pConfiguration->getAtlasName ( ) ) );
        this->createFontChars ( );
    }
}

const KDchar* CCLabelBMFont::getFntFile ( KDvoid )
{
    return m_sFntFile.c_str ( );
}

CCBMFontConfiguration* CCLabelBMFont::getConfiguration ( KDvoid ) const
{
	return m_pConfiguration;
}

//LabelBMFont - Debug draw
KDvoid CCLabelBMFont::draw ( KDvoid )
{
	CCSpriteBatchNode::draw ( );

	if ( m_bIsDebugDraw )
	{
		ccDrawRect ( CCPointZero, CCPoint ( this->getContentSize ( ) ) );
	}
}

NS_CC_END
