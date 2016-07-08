/* -----------------------------------------------------------------------------------
 *
 *      File            CCString.cpp
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
#include "cocoa/CCString.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN

#define kMaxStringLen	( 1024 * 100 )

CCString::CCString ( KDvoid )
{
	m_sString = "";
}

CCString::CCString ( const KDchar* szStr )
{
	m_sString = szStr;
}	

CCString::CCString ( const std::string& sStr )
{
	m_sString = sStr;
}

CCString::CCString ( const CCString& tStr )
{
	m_sString = tStr.getCString ( );
}

CCString::~CCString ( KDvoid )
{ 
    m_sString.clear ( );
}

CCString& CCString::operator= ( const CCString& tOther )
{
    m_sString = tOther.m_sString;

    return *this;
}

KDbool CCString::initWithFormatAndValist ( const KDchar* szFormat, KDVaListKHR pArgs )
{
    KDbool   bRet = KD_FALSE;
    KDchar*  pBuf = (KDchar*) kdMalloc ( kMaxStringLen );
    
	if ( pBuf )
    {
        kdVsnprintf ( pBuf, kMaxStringLen, szFormat, pArgs );
        m_sString = pBuf;
        kdFree ( pBuf );
        bRet = KD_TRUE;
    }

    return bRet;
}

KDbool CCString::initWithFormat ( const KDchar* szFormat, ... )
{
    KDbool  bRet = KD_FALSE;
    m_sString.clear ( );

    KDVaListKHR  pArgs;
    KD_VA_START_KHR ( pArgs, szFormat );

    bRet = initWithFormatAndValist ( szFormat, pArgs );

    KD_VA_END_KHR ( pArgs );

    return bRet;
}

KDint CCString::intValue ( KDvoid ) const
{
    if ( length ( ) == 0 )
    {
        return 0;
    }

    return kdStrtol ( m_sString.c_str ( ), KD_NULL, 0 );
}

KDuint CCString::uintValue ( KDvoid ) const
{
    if ( length ( ) == 0 )
    {
        return 0;
    }

    return kdStrtoul ( m_sString.c_str ( ), KD_NULL, 0 );
}

KDfloat CCString::floatValue ( KDvoid ) const
{
    if ( length ( ) == 0 )
    {
        return 0.0f;
    }

    return (KDfloat) kdStrtod ( m_sString.c_str ( ), KD_NULL );
}

KDdouble CCString::doubleValue ( KDvoid ) const
{
    if ( length ( ) == 0 )
    {
        return 0.0;
    }

    return kdStrtod ( m_sString.c_str ( ), KD_NULL );
}

KDbool CCString::boolValue ( KDvoid ) const
{
    if ( length ( ) == 0 )
    {
        return KD_FALSE;
    }

    if ( 0 == kdStrcmp ( m_sString.c_str ( ), "0" ) || 0 == kdStrcmp ( m_sString.c_str ( ), "false" ) )
    {
        return KD_FALSE;
    }

    return KD_TRUE;
}

const KDchar* CCString::getCString ( KDvoid ) const
{
    return m_sString.c_str ( );
} 

KDuint CCString::length ( KDvoid ) const
{
    return m_sString.length ( );
}

KDint CCString::compare ( const KDchar* szStr ) const
{
    return kdStrcmp ( getCString ( ), szStr );
}

CCObject* CCString::copyWithZone ( CCZone* pZone )
{
    CCAssert ( pZone == KD_NULL, "CCString should not be inherited." );

    return new CCString ( m_sString.c_str ( ) );
}

KDbool CCString::isEqual ( const CCObject* pObject )
{
    KDbool           bRet = KD_FALSE;
    const CCString*  pStr = dynamic_cast<const CCString*> ( pObject );

    if ( pStr != KD_NULL )
    {
        if ( 0 == m_sString.compare ( pStr->m_sString ) )
        {
            bRet = KD_TRUE;
        }
    }

    return bRet;
}

CCString* CCString::create ( const std::string& sStr )
{
    CCString*  pRet = new CCString ( sStr );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

    return pRet;
}

CCString* CCString::createWithData ( const KDubyte* pData, KDsize nLen )
{
    CCString*  pRet = KD_NULL;
    
	if ( pData != KD_NULL )
    {
        KDchar*  szStr = (KDchar*) kdMalloc ( nLen + 1 );
        if ( szStr != KD_NULL )
        {
            szStr [ nLen ] = '\0';
			if ( nLen > 0 )
			{
				kdMemcpy ( szStr, pData, nLen );				
			}

			pRet = CCString::create ( szStr );
            kdFree ( szStr );
        }
    }

    return pRet;
}

CCString* CCString::createWithFormat ( const KDchar* szFormat, ... )
{
    CCString*  pRet = CCString::create ( "" );

    KDVaListKHR  pArgs;
    KD_VA_START_KHR ( pArgs, szFormat );

    pRet->initWithFormatAndValist ( szFormat, pArgs );

    KD_VA_END_KHR ( pArgs );

    return pRet;
}

CCString* CCString::createWithContentsOfFile ( const KDchar* szFileName )
{
    KDsize    nSize = 0;
    KDubyte*  pData = 0;
    CCString*  pRet = KD_NULL;
    
	pData = CCFileUtils::sharedFileUtils ( )->getFileData ( szFileName, "rb", &nSize );
    pRet  = CCString::createWithData ( pData, nSize );
    CC_SAFE_DELETE_ARRAY ( pData );

    return pRet;
}

KDvoid CCString::acceptVisitor ( CCDataVisitor& tVisitor )
{
	tVisitor.visit ( this );
}

NS_CC_END
