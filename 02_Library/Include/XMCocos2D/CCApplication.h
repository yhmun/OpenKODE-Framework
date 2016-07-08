/* -----------------------------------------------------------------------------------
 *
 *      File            CCApplication.h
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

#ifndef __CCApplication_h__
#define __CCApplication_h__

#include "ccMacros.h"
#include <string>

#define COCOS2D_ENGINE_LAUNCH( _MODE_ )								\
XM_APP_MAIN_BEGIN													\
	XM_APP_PROPERTY_SET;											\
	xmRealizeWindow ( XM_OPEN_GL_MODE | _MODE_, &ccEventProc );	    \
XM_APP_MAIN_END														

KD_API KDvoid KD_APIENTRY  ccEventProc ( const KDEvent* pEvent );

NS_CC_BEGIN

/**
 *	@brief Enum the language type supported now
 */
typedef enum LanguageType
{
    kCCLanguageEnglish	= 0	,
    kCCLanguageChinese		,
    kCCLanguageFrench		,
    kCCLanguageItalian		,
    kCCLanguageGerman		,
    kCCLanguageSpanish		,
    kCCLanguageRussian		,
    kCCLanguageKorean		,
} ccLanguageType;

typedef enum TargetPlatform
{
	kCCTargetWindows		,
	kCCTargetLinux			,
	kCCTargetMacOS			,
	kCCTargetAndroid		,
	kCCTargetIphone			,
	kCCTargetIpad			,
	kCCTargetBlackBerry		,
	kCCTargetNaCl			,
	kCCTargetEmscripten		,
	kCCTargetTizen			,
	kCCTargetWinRT			,
	kCCTargetWP8			,
	kCCTargetWince			,
	kCCTargetBada			,
	kCCTargetUnknown		,
} ccTargetPlatform;

class CCApplicationProtocol
{
	public :

		/**
		 *	@brief  Implement CCDirector and CCScene init code here.
		 *	@return true    Initialize success, app continue.
		 *	@return false   Initialize failed, app terminate.
		 */
		virtual KDbool			applicationDidFinishLaunching ( KDvoid ) = 0;

		/**
		 *	@brief  The function be called when the application will ternimate
		 *	@param  the pointer of the application
		 */
		virtual	KDvoid			applicationWillTerminate ( KDvoid ) = 0;

		/**
		 *	@brief  The function be called when the application enter background
		 *	@param  the pointer of the application
		 */
		virtual KDvoid			applicationDidEnterBackground ( KDvoid ) = 0;

		/**
		 *	@brief  The function be called when the application enter foreground
		 *	@param  the pointer of the application
		 */
		virtual KDvoid			applicationWillEnterForeground ( KDvoid ) = 0;
};

class CCApplication : public CCApplicationProtocol
{
	protected :

		CCApplication ( KDvoid );

	public :

		static CCApplication*	sharedApplication ( KDvoid );

	public :

		const KDchar*			getVersion ( KDvoid );

		ccLanguageType			getCurrentLanguage ( KDvoid );
	    
		/** Get target platform */
		ccTargetPlatform		getTargetPlatform  ( KDvoid );

		/* set the Resource root path */
		KDvoid					setResourceRootPath ( const KDchar* szRootResDir );

		/* get the Resource root path */
		const std::string&		getResourceRootPath ( KDvoid );

		KDvoid					setStartupScriptFilename ( const KDchar* szStartupScriptFile );

		const std::string&		getStartupScriptFilename ( KDvoid );

	protected :

		std::string				m_sStartupScriptFilename;
		std::string				m_sResourceRootPath;
};

class AppDelegate : public CCApplication 
{
	public :

		virtual	KDbool			applicationDidFinishLaunching	( KDvoid );

		virtual KDvoid			applicationWillTerminate		( KDvoid );

		virtual KDvoid			applicationWillEnterForeground	( KDvoid );

		virtual KDvoid			applicationDidEnterBackground	( KDvoid );
};

NS_CC_END

#endif // __CCApplication_h__
