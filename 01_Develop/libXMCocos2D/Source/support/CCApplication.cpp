/* -----------------------------------------------------------------------------------
 *
 *      File            CCApplication.cpp
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
#include "CCApplication.h"
#include "support/CCFileUtils.h"
#include "ccConfig.h"

NS_CC_BEGIN


CCApplication* CCApplication::sharedApplication ( KDvoid )
{
	static AppDelegate	tApplication;

	return &tApplication;
}

CCApplication::CCApplication ( KDvoid )
{
	m_sStartupScriptFilename = "";
}

const KDchar* CCApplication::getVersion ( KDvoid )
{
	static KDchar	szVersion [ 64 ] = "";

	if ( kdStrlen ( szVersion ) == 0 )
	{
		kdSprintf 
		(
			szVersion, "XMCocos2D-%s %s",				
	#if defined ( USE_OPEN_GLES2 )  
			"ES2",
	#else
			"ES",
	#endif
			"v2.2.0"				// Version
		);
	}

	return szVersion;
}

ccLanguageType CCApplication::getCurrentLanguage ( KDvoid )
{
	const KDchar*	szLocale  = kdGetLocale ( );

	if ( szLocale )
	{
		if		( !kdStrncmp ( szLocale, "ko", 2 ) )
		{
			return kCCLanguageKorean;
		}
		else if ( !kdStrncmp ( szLocale, "zh", 2 ) )
		{
			return kCCLanguageChinese;
		}
		else if ( !kdStrncmp ( szLocale, "fr", 2 ) )
		{
			return kCCLanguageFrench;
		}
		else if ( !kdStrncmp ( szLocale, "it", 2 ) )
		{
			return kCCLanguageItalian;
		}
		else if ( !kdStrncmp ( szLocale, "de", 2 ) )
		{
			return kCCLanguageGerman;
		}
		else if ( !kdStrncmp ( szLocale, "es", 2 ) )
		{
			return kCCLanguageSpanish;
		}
		else if ( !kdStrncmp ( szLocale, "ru", 2 ) )
		{
			return kCCLanguageRussian;
		}
	}

	return kCCLanguageEnglish;
}

ccTargetPlatform CCApplication::getTargetPlatform  ( KDvoid )
{
	switch ( CC_TARGET_PLATFORM )
	{
		case CC_PLATFORM_IOS		:	return kCCTargetIphone;
		case CC_PLATFORM_MAC		:	return kCCTargetMacOS;
		case CC_PLATFORM_ANDROID	:	return kCCTargetAndroid;			
		case CC_PLATFORM_WIN32		:	return kCCTargetWindows;	
		case CC_PLATFORM_WINCE		:	return kCCTargetWince;	
		case CC_PLATFORM_LINUX		:	return kCCTargetLinux;	
		case CC_PLATFORM_BADA		:	return kCCTargetBada;	
		case CC_PLATFORM_BLACKBERRY :	return kCCTargetBlackBerry;
		default						:	return kCCTargetUnknown;
	}
}

KDvoid CCApplication::setResourceRootPath ( const KDchar* szRootResDir )
{
    m_sResourceRootPath = szRootResDir;
    if ( m_sResourceRootPath [ m_sResourceRootPath.length ( ) - 1 ] != '/' )
    {
        m_sResourceRootPath += '/';
    }

    CCFileUtils*  pFileUtils = CCFileUtils::sharedFileUtils ( );
    std::vector<std::string>  aSearchPaths = pFileUtils->getSearchPaths ( );
    aSearchPaths.insert ( aSearchPaths.begin ( ), m_sResourceRootPath );
    pFileUtils->setSearchPaths ( aSearchPaths );
}

const std::string& CCApplication::getResourceRootPath ( KDvoid )
{
	return m_sResourceRootPath;
}

KDvoid CCApplication::setStartupScriptFilename ( const KDchar* szStartupScriptFile )
{
	m_sStartupScriptFilename = szStartupScriptFile;
}

const std::string& CCApplication::getStartupScriptFilename ( KDvoid )
{
	return m_sStartupScriptFilename;
}

NS_CC_END
