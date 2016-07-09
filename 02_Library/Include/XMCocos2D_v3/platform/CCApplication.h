/* -----------------------------------------------------------------------------------
 *
 *      File            CCApplication.h
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

#ifndef __CCApplication_h__
#define __CCApplication_h__

#include "CCStdC.h"
#include "platform/CCCommon.h"
#include "platform/CCApplicationProtocol.h"
#include <string>

#define COCOS2D_ENGINE_LAUNCH( _MODE_ )								\
XM_APP_MAIN_BEGIN													\
	XM_APP_PROPERTY_SET;											\
	xmRealizeWindow ( XM_OPEN_GL_MODE | _MODE_, &ccEventProc );	    \
XM_APP_MAIN_END														

KD_API KDvoid KD_APIENTRY  ccEventProc ( const KDEvent* pEvent );

NS_CC_BEGIN

class Rect;

class CC_DLL Application : public ApplicationProtocol
{
	protected :

		/**
		 * @js ctor
		 */
		Application ( KDvoid );

	public :

		/**
		 *	@brief    Get current applicaiton instance.
		 *	@return Current application instance pointer.
		 */
		static Application*			getInstance ( KDvoid );

	public :

		/**
		 *	@brief	Callback by Director for limit FPS.
		 *	@param	interval The time, expressed in seconds, between current frame and next.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setAnimationInterval ( KDdouble dInterval );

		/**
		 *	@brief Get current language config
		 *	@return Current language config
		 *	@js NA
		 *	@lua NA
		 */
		virtual LanguageType		getCurrentLanguage ( KDvoid );
    
		/**
		 *	@brief Get target platform
		 *	@js NA
		 *	@lua NA
		 */
		virtual Platform			getTargetPlatform ( KDvoid );

		KDvoid						setStartupScriptFilename ( const std::string& sStartupScriptFile );

		const std::string&			getStartupScriptFilename ( KDvoid );

	protected :

		std::string					m_sStartupScriptFilename;
};

class AppDelegate : public Application 
{
	public :

		/**
		 *	@brief    Implement Director and Scene init code here.
		 *	@return true    Initialize success, app continue.
		 *	@return false   Initialize failed, app terminate.
		 */
		virtual	KDbool				applicationDidFinishLaunching	( KDvoid );

		/**
		 *	@brief  The function be called when the application will ternimate
		 *	@param  the pointer of the application
		 */
		virtual KDvoid				applicationWillTerminate		( KDvoid );

		/**
		 *	@brief  The function be called when the application enter background
		 *	@param  the pointer of the application
		 */
		virtual KDvoid				applicationWillEnterForeground	( KDvoid );

		/**
		 *	@brief  The function be called when the application enter foreground
		 *	@param  the pointer of the application
		 */
		virtual KDvoid				applicationDidEnterBackground	( KDvoid );
};

NS_CC_END

#endif    // __CCApplication_h__
