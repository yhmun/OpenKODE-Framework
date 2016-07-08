/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelTTF.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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
#include "label_nodes/CCLabelTTF.h"
#include "support/CCDirector.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM	kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM	kCCShader_PositionTextureA8Color
#endif

CCLabelTTF::CCLabelTTF ( KDvoid )
{
    m_eAlignH			= kCCTextAlignmentCenter;
	m_eAlignV			= kCCVerticalTextAlignmentTop;
    m_sFontName			= "";
    m_fFontSize			= 0;
    m_sString			= "";
	m_bDirty			= KD_TRUE;
	m_bShadowEnabled	= KD_FALSE;
	m_bStrokeEnabled	= KD_FALSE;
	m_tTextFillColor	= ccWHITE;
}

CCLabelTTF::~CCLabelTTF ( KDvoid )
{
	m_sFontName.clear ( ); 
	m_sString.clear ( );
}

CCLabelTTF* CCLabelTTF::create ( KDvoid )
{
    CCLabelTTF*  pRet = new CCLabelTTF ( );

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

CCLabelTTF* CCLabelTTF::create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize )
{
    return CCLabelTTF::create ( szString, szFontName, fFontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop );
}

CCLabelTTF* CCLabelTTF::create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH )
{
    return CCLabelTTF::create ( szString, szFontName, fFontSize, tDimensions, eAlignH, kCCVerticalTextAlignmentTop );
}

CCLabelTTF* CCLabelTTF::create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV )
{
	CCLabelTTF*  pRet = new CCLabelTTF ( );

	if ( pRet && pRet->initWithString ( szString, szFontName, fFontSize, tDimensions, eAlignH, eAlignV ) )
	{
		pRet->autorelease ( );		
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCLabelTTF* CCLabelTTF::create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCAlignment eAlign )
{
	CCTextAlignment			eAlignH = ( eAlign & kCCAlignmentLeft ? kCCTextAlignmentLeft : ( eAlign & kCCAlignmentRight ? kCCTextAlignmentRight : kCCTextAlignmentCenter ) );
	CCVerticalTextAlignment eAlignV = ( eAlign & kCCAlignmentBottom ? kCCVerticalTextAlignmentBottom : ( eAlign & kCCAlignmentTop ? kCCVerticalTextAlignmentTop : kCCVerticalTextAlignmentCenter ) );

	return CCLabelTTF::create ( szString, szFontName, fFontSize, tDimensions, eAlignH, eAlignV );	
}

CCLabelTTF* CCLabelTTF::createWithFontDefinition ( const KDchar* szString, ccFontDefinition& tTextDefinition )
{
	CCLabelTTF*		pRet = new CCLabelTTF ( );

	if ( pRet && pRet->initWithStringAndTextDefinition ( szString, tTextDefinition ) )
	{
		pRet->autorelease ( );		
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCLabelTTF::init ( KDvoid )
{
    return this->initWithString ( "", "Helvetica", 12 );
}

KDbool CCLabelTTF::initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize )
{
    return this->initWithString ( szString, szFontName, fFontSize, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop );
}

KDbool CCLabelTTF::initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH )
{
    return this->initWithString ( szString, szFontName, fFontSize, CCSizeZero, eAlignH, kCCVerticalTextAlignmentTop );
}

KDbool CCLabelTTF::initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCAlignment eAlign )
{
	CCTextAlignment			eAlignH = ( eAlign & kCCAlignmentLeft ? kCCTextAlignmentLeft : ( eAlign & kCCAlignmentRight ? kCCTextAlignmentRight : kCCTextAlignmentCenter ) );
	CCVerticalTextAlignment eAlignV = ( eAlign & kCCAlignmentBottom ? kCCVerticalTextAlignmentBottom : ( eAlign & kCCAlignmentTop ? kCCVerticalTextAlignmentTop : kCCVerticalTextAlignmentCenter ) );

	return this->initWithString ( szString, szFontName, fFontSize, CCSizeZero, eAlignH, eAlignV );
}

KDbool CCLabelTTF::initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV )
{
	CCAssert ( szString != KD_NULL, "" );

	if ( CCSprite::init ( ) )
	{
        // shader program
        setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( SHADER_PROGRAM ) );

		m_tDimensions = tDimensions;
		m_eAlignH	  = eAlignH;
		m_eAlignV	  = eAlignV;
		m_sFontName   = szFontName;
		m_fFontSize   = fFontSize;

		this->setString ( szString );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDbool CCLabelTTF::initWithStringAndTextDefinition ( const KDchar* szString, ccFontDefinition& tTextDefinition )
{
	if ( CCSprite::init ( ) )
	{
		// shader program
		this->setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( SHADER_PROGRAM ) );

		// prepare everything needed to render the label
		_updateWithTextDefinition ( tTextDefinition, false );

		// set the string
		this->setString ( szString );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCLabelTTF::setString ( const KDchar* szString )
{
	CCAssert ( szString != KD_NULL, "Invalid string" );
    
    if ( m_sString.compare ( szString ) )
    {
        m_sString = szString;
        
        this->updateTexture ( );
    }
}

const KDchar* CCLabelTTF::getString ( KDvoid )
{
	return m_sString.c_str ( );
}

const KDchar* CCLabelTTF::description ( KDvoid )
{
	return CCString::createWithFormat("<CCLabelTTF | FontName = %s, FontSize = %.1f>", m_sFontName.c_str ( ), m_fFontSize )->getCString ( );
}

CCTextAlignment CCLabelTTF::getHorizontalAlignment ( KDvoid )
{
    return m_eAlignH;
}

KDvoid CCLabelTTF::setHorizontalAlignment ( CCTextAlignment eAlignment )
{
    if ( eAlignment != m_eAlignH )
    {
        m_eAlignH = eAlignment;
        
        // Force update
        if ( m_sString.size ( ) > 0 )
        {
            this->updateTexture ( );
        }
    }
}

CCVerticalTextAlignment CCLabelTTF::getVerticalAlignment ( KDvoid )
{
    return m_eAlignV;
}

KDvoid CCLabelTTF::setVerticalAlignment ( CCVerticalTextAlignment eAlignment )
{
    if ( eAlignment != m_eAlignV)
    {
        m_eAlignV = eAlignment;
        
        // Force update
        if ( m_sString.size ( ) > 0 )
        {
            this->updateTexture ( );
        }
    }
}

CCSize CCLabelTTF::getDimensions ( KDvoid )
{
    return m_tDimensions;
}

KDvoid CCLabelTTF::setDimensions ( const CCSize& tDimensions )
{
    if ( tDimensions.cx != m_tDimensions.cx || tDimensions.cy != m_tDimensions.cy )
    {
        m_tDimensions = tDimensions;
        
        // Force udpate
        if ( m_sString.size ( ) > 0 )
        {
            this->updateTexture ( );
        }
    }
}

KDfloat CCLabelTTF::getFontSize ( KDvoid )
{
    return m_fFontSize;
}

KDvoid CCLabelTTF::setFontSize ( KDfloat fFontSize )
{
    if ( m_fFontSize != fFontSize )
    {
        m_fFontSize = fFontSize;
        
        // Force update
        if ( m_sString.size ( ) > 0 )
        {
            this->updateTexture ( );
        }
    }
}

const KDchar* CCLabelTTF::getFontName ( KDvoid )
{
    return m_sFontName.c_str ( );
}

KDvoid CCLabelTTF::setFontName ( const KDchar* szFontName )
{
    if ( m_sFontName.compare ( szFontName ) )
    {
        m_sFontName = szFontName;
        
        // Force update
        if ( m_sString.size ( ) > 0 )
        {
            this->updateTexture ( );
        }
    }
}

// Helper
KDbool CCLabelTTF::updateTexture ( KDvoid )
{
	// let system compute label's width or height when its value is 0
    // refer to cocos2d-x issue #1430
	CCTexture2D*  pTexture = new CCTexture2D ( );
    if ( !pTexture )
    {
		return KD_FALSE;
    }

	ccFontDefinition	tTexDef = _prepareTextDefinition ( KD_TRUE );
	pTexture->initWithString ( m_sString.c_str ( ), &tTexDef );

	// set the texture
    this->setTexture ( pTexture );	

	// set the size in the sprite
    this->setTextureRect ( CCRect ( ccpz, m_pTexture->getContentSize ( ) ) );
	
	// release it
	pTexture->release ( );

	return KD_TRUE;
}

KDvoid CCLabelTTF::enableShadow ( const CCSize& tShadowOffset, KDfloat fShadowOpacity, KDfloat fShadowBlur, KDbool bUpdateTexture )
{
	KDbool	bValueChanged = KD_FALSE;

	if ( KD_FALSE == m_bShadowEnabled )
	{
		m_bShadowEnabled = KD_TRUE;
		bValueChanged = KD_TRUE;
	}

	if ( ( m_tShadowOffset.width != tShadowOffset.width ) || ( m_tShadowOffset.height != tShadowOffset.height ) )
	{
		m_tShadowOffset.width  = tShadowOffset.width;
		m_tShadowOffset.height = tShadowOffset.height;

		bValueChanged = KD_TRUE;
	}

	if ( m_fShadowOpacity != fShadowOpacity )
	{
		m_fShadowOpacity = fShadowOpacity;
		bValueChanged = KD_TRUE;
	}

	if ( m_fShadowBlur != fShadowBlur )
	{
		m_fShadowBlur = fShadowBlur;
		bValueChanged = KD_TRUE;
	}


	if ( bValueChanged && bUpdateTexture )
	{
		this->updateTexture();
	}
}

KDvoid CCLabelTTF::disableShadow ( KDbool bUpdateTexture )
{
	if ( m_bShadowEnabled )
	{
		m_bShadowEnabled = KD_FALSE;

		if ( bUpdateTexture )
		{
			this->updateTexture ( );
		}
	}
}

KDvoid CCLabelTTF::enableStroke ( const ccColor3B& tStrokeColor, KDfloat fStrokeSize, KDbool bUpdateTexture )
{
	KDbool		bValueChanged = KD_FALSE;

	if ( m_bStrokeEnabled == KD_FALSE )
	{
		m_bStrokeEnabled = KD_TRUE;
		bValueChanged = KD_TRUE;
	}

	if ( ( m_tStrokeColor.r != tStrokeColor.r ) || ( m_tStrokeColor.g != tStrokeColor.g ) || ( m_tStrokeColor.b != tStrokeColor.b ) )
	{
		m_tStrokeColor = tStrokeColor;
		bValueChanged = KD_TRUE;
	}

	if ( m_fStrokeSize != fStrokeSize )
	{
		m_fStrokeSize = fStrokeSize;
		bValueChanged = KD_TRUE;
	}

	if ( bValueChanged && bUpdateTexture )
	{
		this->updateTexture ( );
	}
}

KDvoid CCLabelTTF::disableStroke ( KDbool bUpdateTexture )
{
	if ( m_bStrokeEnabled )
	{
		m_bStrokeEnabled = KD_FALSE;

		if ( bUpdateTexture )
		{
			this->updateTexture ( );
		}
	}
}

KDvoid CCLabelTTF::setFontFillColor ( const ccColor3B& tTintColor, KDbool bUpdateTexture )
{
	if ( m_tTextFillColor.r != tTintColor.r || m_tTextFillColor.g != tTintColor.g || m_tTextFillColor.b != tTintColor.b )
	{
		m_tTextFillColor = tTintColor;

		if ( bUpdateTexture )
		{
			this->updateTexture ( );
		}
	}
}

KDvoid CCLabelTTF::setTextDefinition ( ccFontDefinition* pTheDefinition )
{
	if ( pTheDefinition )
	{
		_updateWithTextDefinition ( *pTheDefinition, true );
	}
}

ccFontDefinition* CCLabelTTF::getTextDefinition ( KDvoid )
{
	ccFontDefinition*	pTempDefinition = new ccFontDefinition;
	*pTempDefinition = _prepareTextDefinition ( KD_FALSE);
	return pTempDefinition;
}

KDvoid CCLabelTTF::_updateWithTextDefinition ( ccFontDefinition& tTextDefinition, KDbool bMustUpdateTexture )
{
	m_tDimensions = CCSizeMake ( tTextDefinition.m_tDimensions.width, tTextDefinition.m_tDimensions.height );
	m_eAlignH = tTextDefinition.m_eAlignment;
	m_eAlignV = tTextDefinition.m_eVertAlignment;

	m_sFontName = tTextDefinition.m_sFontName;
	m_fFontSize = (KDfloat) tTextDefinition.m_nFontSize;

	// shadow
	if ( tTextDefinition.m_tShadow.m_bShadowEnabled )
	{
		enableShadow ( tTextDefinition.m_tShadow.m_tShadowOffset, tTextDefinition.m_tShadow.m_fShadowOpacity, tTextDefinition.m_tShadow.m_fShadowBlur, KD_FALSE );
	}

	// stroke
	if ( tTextDefinition.m_tStroke.m_bStrokeEnabled )
	{
		enableStroke ( tTextDefinition.m_tStroke.m_tStrokeColor, tTextDefinition.m_tStroke.m_fStrokeSize, KD_FALSE );
	}

	// fill color
	setFontFillColor ( tTextDefinition.m_tFontFillColor, KD_FALSE );

	if ( bMustUpdateTexture )
	{
		updateTexture ( );
	}
}

ccFontDefinition CCLabelTTF::_prepareTextDefinition ( KDbool bAdjustForResolution )
{
	ccFontDefinition	tTexDef;

	if ( bAdjustForResolution )
	{
		tTexDef.m_nFontSize = (KDint) ( m_fFontSize * CC_CONTENT_SCALE_FACTOR ( ) );
	}
	else
	{
		tTexDef.m_nFontSize = (KDint) ( m_fFontSize );
	}

	tTexDef.m_sFontName      =  m_sFontName;
	tTexDef.m_eAlignment     =  m_eAlignH;
	tTexDef.m_eVertAlignment =  m_eAlignV;

	if ( bAdjustForResolution )
	{
		tTexDef.m_tDimensions =  CC_SIZE_POINTS_TO_PIXELS ( m_tDimensions );
	}
	else
	{
		tTexDef.m_tDimensions =  m_tDimensions;
	}

	// stroke
	if ( m_bStrokeEnabled )
	{
		tTexDef.m_tStroke.m_bStrokeEnabled = KD_TRUE;
		tTexDef.m_tStroke.m_tStrokeColor   = m_tStrokeColor;

		if ( bAdjustForResolution )
		{
			tTexDef.m_tStroke.m_fStrokeSize = m_fStrokeSize * CC_CONTENT_SCALE_FACTOR ( );
		}
		else
		{
			tTexDef.m_tStroke.m_fStrokeSize = m_fStrokeSize;
		}
	}
	else
	{
		tTexDef.m_tStroke.m_bStrokeEnabled = KD_FALSE;
	}

	// shadow
	if ( m_bShadowEnabled )
	{
		tTexDef.m_tShadow.m_bShadowEnabled	= KD_TRUE;
		tTexDef.m_tShadow.m_fShadowBlur		= m_fShadowBlur;
		tTexDef.m_tShadow.m_fShadowOpacity	= m_fShadowOpacity;

		if ( bAdjustForResolution )
		{
			tTexDef.m_tShadow.m_tShadowOffset = CC_SIZE_POINTS_TO_PIXELS ( m_tShadowOffset );
		}
		else
		{
			tTexDef.m_tShadow.m_tShadowOffset = m_tShadowOffset;
		}
	}
	else
	{
		tTexDef.m_tShadow.m_bShadowEnabled = KD_FALSE;
	}

	// text tint
	tTexDef.m_tFontFillColor = m_tTextFillColor;

	return tTexDef;
}

NS_CC_END