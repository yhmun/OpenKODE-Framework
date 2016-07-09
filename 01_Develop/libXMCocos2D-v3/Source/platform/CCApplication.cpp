/* -----------------------------------------------------------------------------------
 *
 *      File            CCApplication.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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

#include "platform/CCApplication.h"
#include <algorithm>

NS_CC_BEGIN

Application::Application ( KDvoid )
{
	m_sStartupScriptFilename = "";
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance ( KDvoid )
{
	static AppDelegate	tApplication;

	return &tApplication;
}

KDvoid Application::setAnimationInterval ( KDdouble dInterval )
{
	xmSetFrameInterval ( dInterval );
}

LanguageType Application::getCurrentLanguage ( KDvoid )
{
	const KDchar*	szLocale = kdGetLocale ( );

	if ( szLocale )
	{
		if		( !kdStrncmp ( szLocale, "ko", 2 ) )
		{
			return LanguageType::KOREAN;
		}
		else if ( !kdStrncmp ( szLocale, "zh", 2 ) )
		{
			return LanguageType::CHINESE;
		}
		else if ( !kdStrncmp ( szLocale, "en", 2 ) )
		{
			return LanguageType::ENGLISH;
		}
		else if ( !kdStrncmp ( szLocale, "fr", 2 ) )
		{
			return LanguageType::FRENCH;
		}
		else if ( !kdStrncmp ( szLocale, "it", 2 ) )
		{
			return LanguageType::ITALIAN;
		}
		else if ( !kdStrncmp ( szLocale, "de", 2 ) )
		{
			return LanguageType::GERMAN;
		}
		else if ( !kdStrncmp ( szLocale, "es", 2 ) )
		{
			return LanguageType::SPANISH;
		}
		else if ( !kdStrncmp ( szLocale, "ru", 2 ) )
		{
			return LanguageType::RUSSIAN;
		}
		else if ( !kdStrncmp ( szLocale, "jp", 2 ) )
		{
			return LanguageType::JAPANESE;
		}
		else if ( !kdStrncmp ( szLocale, "hu", 2 ) )
		{
			return LanguageType::HUNGARIAN;
		}
		else if ( !kdStrncmp ( szLocale, "ar", 2 ) )
		{
			return LanguageType::ARABIC;
		}
		else if ( !kdStrncmp ( szLocale, "no", 2 ) )
		{
			return LanguageType::NORWEGIAN;
		}
		else if ( !kdStrncmp ( szLocale, "pl", 2 ) )
		{
			return LanguageType::POLISH;
		}
	}

	return LanguageType::ENGLISH;
}

Application::Platform Application::getTargetPlatform ( KDvoid )
{
	switch ( CC_TARGET_PLATFORM )
	{
		case CC_PLATFORM_IOS			:	return Platform::OS_IPHONE;		// OS_IPAD
		case CC_PLATFORM_ANDROID		:	return Platform::OS_ANDROID;
		case CC_PLATFORM_WIN32			:	return Platform::OS_WINDOWS;
//		case CC_PLATFORM_MARMALADE		:	return Platform::OS_WINDOWS;
		case CC_PLATFORM_LINUX			:	return Platform::OS_LINUX;
		case CC_PLATFORM_BADA			:	return Platform::OS_WINDOWS;
		case CC_PLATFORM_BLACKBERRY		:	return Platform::OS_BLACKBERRY;
		case CC_PLATFORM_MAC			:	return Platform::OS_MAC;
		case CC_PLATFORM_NACL			:	return Platform::OS_NACL;
		case CC_PLATFORM_EMSCRIPTEN		:	return Platform::OS_EMSCRIPTEN;
		case CC_PLATFORM_TIZEN			:	return Platform::OS_TIZEN;
//		case CC_PLATFORM_QT5			:	return Platform::OS_WINDOWS;
	}

	return Platform::OS_WINDOWS;
}

KDvoid Application::setStartupScriptFilename ( const std::string& sStartupScriptFile )
{
    m_sStartupScriptFilename = sStartupScriptFile;
    std::replace ( m_sStartupScriptFilename.begin ( ), m_sStartupScriptFilename.end ( ), '\\', '/' );
}

const std::string& Application::getStartupScriptFilename ( KDvoid )
{
	return m_sStartupScriptFilename;
}

NS_CC_END
