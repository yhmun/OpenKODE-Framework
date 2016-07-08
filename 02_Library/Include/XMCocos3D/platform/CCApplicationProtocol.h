/* -----------------------------------------------------------------------------------
 *
 *      File            CCApplicationProtocol.h
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

#ifndef __CCApplicationProtocol_h__
#define __CCApplicationProtocol_h__

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

class CC_DLL ApplicationProtocol
{
	public:

		// Since WINDOWS and ANDROID are defined as macros, we could not just use these keywords in enumeration(Platform).
		// Therefore, 'OS_' prefix is added to avoid conflicts with the definitions of system macros.
		enum class Platform
		{
			OS_WINDOWS		,
			OS_LINUX		,
			OS_MAC			,
			OS_ANDROID		,
			OS_IPHONE		,
			OS_IPAD			,
			OS_BLACKBERRY	,
			OS_NACL			,
			OS_EMSCRIPTEN	,
			OS_TIZEN		,
		};

		/**
		 *	@brief	Implement Director and Scene init code here.
		 *	@return true    Initialize success, app continue.
		 *	@return false   Initialize failed, app terminate.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			applicationDidFinishLaunching ( KDvoid ) = 0;

		/**
		 *	@brief  The function be called when the application will ternimate
		 *	@param  the pointer of the application
		 */
		virtual KDvoid			applicationWillTerminate ( KDvoid ) = 0;

		/**
		 *	@brief  This function will be called when the application enters background.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			applicationDidEnterBackground ( KDvoid ) = 0;

		/**
		 *	@brief  This function will be called when the application enters foreground.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			applicationWillEnterForeground ( KDvoid ) = 0;
};

//	end of platform group
/// @}

NS_CC_END

#endif	// __CCApplicationProtocol_h__
