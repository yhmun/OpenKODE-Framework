/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextFieldTTF.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "text_input_node/CCTextFieldTTF.h"
#include "support/CCDirector.h"
#include "support/ccUtils.h"

NS_CC_BEGIN
    
//////////////////////////////////////////////////////////////////////////
// constructor and destructor
//////////////////////////////////////////////////////////////////////////

CCTextFieldTTF::CCTextFieldTTF ( KDvoid )
{
    m_pDelegate		= 0;
    m_nCharCount	= 0;
    m_sInputText	= "";
    m_sPlaceHolder	= "";   // prevent CCLabelTTF initWithString assertion
    m_sReplaceChar	= "";
    m_tColorSpaceHolder.r = m_tColorSpaceHolder.g = m_tColorSpaceHolder.b = 127;
	m_bSecureTextEntry = KD_FALSE;
}

CCTextFieldTTF::~CCTextFieldTTF ( KDvoid )
{
	m_sInputText.clear ( );
	m_sPlaceHolder.clear ( );
	m_sReplaceChar.clear ( );
}

//////////////////////////////////////////////////////////////////////////
// static constructor
//////////////////////////////////////////////////////////////////////////

CCTextFieldTTF * CCTextFieldTTF::create ( const KDchar* szPlaceHolder, const CCSize& tDimension, CCTextAlignment eAlignment, const KDchar* szFontPath, KDfloat fFontSize )
{		
    CCTextFieldTTF*  pRet = new CCTextFieldTTF ( );

    if ( pRet && pRet->initWithPlaceHolder ( szPlaceHolder, tDimension, eAlignment, szFontPath, fFontSize ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCTextFieldTTF* CCTextFieldTTF::create ( const KDchar* szPlaceHolder, const KDchar* szFontPath, KDfloat fFontSize )
{
    CCTextFieldTTF* pRet = new CCTextFieldTTF ( );

    if ( pRet && pRet->initWithPlaceHolder ( szPlaceHolder, szFontPath, fFontSize ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

//////////////////////////////////////////////////////////////////////////
// initialize
//////////////////////////////////////////////////////////////////////////

KDbool CCTextFieldTTF::initWithPlaceHolder ( const KDchar* szPlaceHolder, const CCSize& tDimension, CCTextAlignment eAlignment, const KDchar* szFontPath, KDfloat fFontSize )
{
	KDbool  bRet = CCLabelTTF::initWithString ( "", szFontPath, fFontSize, tDimension, eAlignment );

    this->setPlaceHolder ( szPlaceHolder );
	
    return bRet;
}

KDbool CCTextFieldTTF::initWithPlaceHolder ( const KDchar* szPlaceHolder, const KDchar* szFontPath, KDfloat fFontSize )
{
	KDbool  bRet = CCLabelTTF::initWithString ( "", szFontPath, fFontSize );

    this->setPlaceHolder ( szPlaceHolder );
	
    return bRet; 
}

//////////////////////////////////////////////////////////////////////////
// CCIMEDelegate
//////////////////////////////////////////////////////////////////////////

KDbool CCTextFieldTTF::attachWithIME ( KDvoid )
{
    KDbool  bRet = CCIMEDelegate::attachWithIME ( );

    if ( bRet )
    {
		// open keyboard
		xmSetIMEKeyboardState ( KD_TRUE );
    }

    return bRet;
}

KDbool CCTextFieldTTF::detachWithIME ( KDvoid )
{
    KDbool  bRet = CCIMEDelegate::detachWithIME ( );

    if ( bRet )
    {
		// close keyboard
        xmSetIMEKeyboardState ( KD_FALSE );
    }

    return bRet;
}

KDbool CCTextFieldTTF::canAttachWithIME ( KDvoid )
{
    return ( m_pDelegate ) ? ( !m_pDelegate->onTextFieldAttachWithIME ( this ) ) : KD_TRUE;
}

KDbool CCTextFieldTTF::canDetachWithIME ( KDvoid )
{
    return ( m_pDelegate ) ? ( !m_pDelegate->onTextFieldDetachWithIME ( this ) ) : KD_TRUE;
}

KDvoid CCTextFieldTTF::insertText ( const KDchar* szText, KDsize uLen )
{
    std::string  sInsert ( szText, uLen );

    // insert \n means input end
    KDuint  uPos = sInsert.find ( '\n' );
    if ( sInsert.npos != uPos )
    {
        uLen = uPos;
        sInsert.erase ( uPos );
    }
    
    if ( uLen > 0 )
    {
        if ( m_pDelegate && m_pDelegate->onTextFieldInsertText ( this, sInsert.c_str ( ), uLen ) )
        {
            // delegate doesn't want to insert text
            return;
        }
        
        m_nCharCount += ccLengthUTF8 ( sInsert.c_str ( ) );
        std::string sText ( m_sInputText );
        sText.append ( sInsert );
        setString ( sText.c_str ( ) );
    }

    if ( sInsert.npos == uPos )
	{
        return;
    }
    
    // '\n' inserted, let delegate process first
    if ( m_pDelegate && m_pDelegate->onTextFieldInsertText ( this, "\n", 1 ) )
    {
        return;
    }
    
    // if delegate hasn't processed, detach from IME by default
    detachWithIME ( );
}

KDvoid CCTextFieldTTF::deleteBackward ( KDvoid )
{
    KDuint  uLen = m_sInputText.length ( );

    if ( !uLen )
    {
        return;
    }

    // get the delete byte number
    KDuint uDeleteLen = 1;    // default, erase 1 byte

    while ( 0x80 == ( 0xC0 & m_sInputText.at ( uLen - uDeleteLen ) ) )
    {
        ++uDeleteLen;
    }

    if ( m_pDelegate && m_pDelegate->onTextFieldDeleteBackward ( this, m_sInputText.c_str ( ) + uLen - uDeleteLen, uDeleteLen ) )
    {
		// delegate doesn't wan't to delete backwards
        return;
    }

    // if all text deleted, show placeholder string
    if ( uLen <= uDeleteLen )
    {
		m_sInputText.clear ( );
        m_nCharCount = 0;
        CCLabelTTF::setString ( m_sPlaceHolder.c_str ( ) );
        return;
    }

    // set new input text
    std::string  sText ( m_sInputText.c_str ( ), uLen - uDeleteLen );
    setString ( sText.c_str ( ) );
}

const KDchar* CCTextFieldTTF::getContentText ( KDvoid )
{
    return m_sInputText.c_str ( );
}

KDvoid CCTextFieldTTF::draw ( KDvoid )
{
    if ( m_pDelegate && m_pDelegate->onDraw ( this ) )
    {
        return;
    }

    if ( m_sInputText.length ( ) )
    {
        CCLabelTTF::draw ( );
    }
	else
	{
		// draw placeholder

		ccColor3B  tColor = getColor ( );
		setColor ( m_tColorSpaceHolder );

		CCLabelTTF::draw ( );

		setColor ( tColor );
	}
}

//////////////////////////////////////////////////////////////////////////
// properties
//////////////////////////////////////////////////////////////////////////

const ccColor3B& CCTextFieldTTF::getColorSpaceHolder ( KDvoid )
{
    return m_tColorSpaceHolder;
}

KDvoid CCTextFieldTTF::setColorSpaceHolder ( const ccColor3B& tColor )
{
    m_tColorSpaceHolder = tColor;
}

// input text property
KDvoid CCTextFieldTTF::setString ( const KDchar* szText )
{
	static KDchar	szBulletString[] = "%xe2%x80%xa2%x00";
    std::string		sDisplayText;
    KDint			nLength;

	m_sInputText = szText ? szText : "";
	sDisplayText = m_sInputText;

	if ( m_bSecureTextEntry )
    {
        sDisplayText = "";
        nLength = m_sInputText.length ( );
        while ( nLength )
        {
            sDisplayText.append ( szBulletString );
            --nLength;
        }
    }

	m_nCharCount = ccLengthUTF8 ( sDisplayText.c_str ( ) );

    // if there is no input text, display placeholder instead
    if ( !m_sInputText.length ( ) )
    {
        CCLabelTTF::setString ( m_sPlaceHolder.c_str ( ) );
    }
    else
    {
		if ( m_sReplaceChar.length ( ) )
		{
			std::string  sReplace = "";

			for ( KDuint i = 0; i < m_nCharCount; i++ )
			{
				sReplace.append ( m_sReplaceChar );
			}

			CCLabelTTF::setString ( sReplace.c_str ( ) );
		}
		else
		{
			CCLabelTTF::setString ( sDisplayText.c_str ( ) );
		}
    }
}

const KDchar* CCTextFieldTTF::getString ( KDvoid )
{
    return m_sInputText.c_str ( );
}

// place holder text property
KDvoid CCTextFieldTTF::setPlaceHolder ( const KDchar* szText )
{
    m_sPlaceHolder = szText ? szText : "";

    if ( !m_sInputText.length ( ) )
    {
        CCLabelTTF::setString ( m_sPlaceHolder.c_str ( ) );
    }
}

const KDchar* CCTextFieldTTF::getPlaceHolder ( KDvoid )
{
    return m_sPlaceHolder.c_str ( );
}

KDvoid CCTextFieldTTF::setReplaceCharacter ( const KDchar* szChar )
{
	m_sReplaceChar = szChar ? szChar : "";

	if ( m_sReplaceChar.length ( ) && m_nCharCount > 0 )
	{
		std::string  sReplace = "";

		for ( KDuint i = 0; i < m_nCharCount; i++ )
		{
			sReplace.append ( m_sReplaceChar );
		}

		CCLabelTTF::setString ( sReplace.c_str ( ) );
	}
}

// secureTextEntry
KDvoid CCTextFieldTTF::setSecureTextEntry ( KDbool bValue )
{
    if ( m_bSecureTextEntry != bValue )
    {
        m_bSecureTextEntry = bValue;
        setString ( getString ( ) );
    }
}

KDbool CCTextFieldTTF::isSecureTextEntry ( KDvoid )
{
    return m_bSecureTextEntry;
}

NS_CC_END