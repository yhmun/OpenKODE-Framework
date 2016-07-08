/* -----------------------------------------------------------------------------------
 *
 *      File            ccConfig.h
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

#ifndef __ccConfig_h__
#define __ccConfig_h__

// define supported target platform macro which CC uses.
#define CC_PLATFORM_UNKNOWN				0
#define CC_PLATFORM_IOS					1
#define CC_PLATFORM_ANDROID				2
#define CC_PLATFORM_WIN32				3
#define CC_PLATFORM_MARMALADE			4
#define CC_PLATFORM_LINUX				5
#define CC_PLATFORM_BADA				6
#define CC_PLATFORM_BLACKBERRY			7	
#define CC_PLATFORM_MAC					8
#define CC_PLATFORM_NACL				9
#define CC_PLATFORM_EMSCRIPTEN			10
#define CC_PLATFORM_TIZEN				11
#define CC_PLATFORM_WINRT				12
#define CC_PLATFORM_WP8					13
#define CC_PLATFORM_WINCE				20

#if   defined ( SHP )

#define CC_TARGET_PLATFORM				CC_PLATFORM_BADA

#elif defined ( _WIN32_WCE )

#define CC_TARGET_PLATFORM				CC_PLATFORM_WINCE

#elif defined ( _WIN32 )

#define CC_TARGET_PLATFORM				CC_PLATFORM_WIN32

#elif defined ( __APPLE__ )

#define CC_TARGET_PLATFORM				CC_PLATFORM_IOS

#elif defined ( ANDROID )

#define CC_TARGET_PLATFORM				CC_PLATFORM_ANDROID

#elif defined ( Linux )

#define CC_TARGET_PLATFORM				CC_PLATFORM_LINUX

#else

// Determine target platform by compile environment macro.
#define CC_TARGET_PLATFORM				CC_PLATFORM_UNKNOWN

#endif

/**
 *	@file
 *	cocos2d (cc) configuration file
 */

/** 
 *	@def CC_ENABLE_STACKABLE_ACTIONS
 *	If enabled, actions that alter the position property (eg: CCMoveBy, CCJumpBy, CCBezierBy, etc..) will be stacked.
 *	If you run 2 or more 'position' actions at the same time on a node, then end position will be the sum of all the positions.
 *	If disabled, only the last run action will take effect.
 *
 *	Enabled by default. Disable to be compatible with v2.0 and older versions.
 *
 *	@since v2.1
 */
#ifndef CC_ENABLE_STACKABLE_ACTIONS
#define CC_ENABLE_STACKABLE_ACTIONS						1
#endif

/** 
 *	@def CC_ENABLE_GL_STATE_CACHE
 *	If enabled, cocos2d will maintain an OpenGL state cache internally to avoid unnecessary switches.
 *	In order to use them, you have to use the following functions, instead of the the GL ones:
 *	   - ccGLUseProgram() instead of glUseProgram()
 *	   - ccGLDeleteProgram() instead of glDeleteProgram()
 *	   - ccGLBlendFunc() instead of glBlendFunc()
 *
 *	If this functionality is disabled, then ccGLUseProgram(), ccGLDeleteProgram(), ccGLBlendFunc() will call the GL ones, without using the cache.
 *
 *	It is recommended to enable whenever possible to improve speed.
 *	If you are migrating your code from GL ES 1.1, then keep it disabled. Once all your code works as expected, turn it on.
 *
 *	Default value: Enabled by default
 *
 *	@since v2.0.0
 */
#ifndef CC_ENABLE_GL_STATE_CACHE
#define CC_ENABLE_GL_STATE_CACHE						1
#endif

/** 
 *	@def CC_DIRECTOR_FPS_INTERVAL
 *	Seconds between FPS updates.
 *	0.5 seconds, means that the FPS number will be updated every 0.5 seconds.
 *	Having a bigger number means a more reliable FPS
 *
 *	Default value: 0.1f
 */
#ifndef CC_DIRECTOR_STATS_INTERVAL
#define CC_DIRECTOR_STATS_INTERVAL                      (0.5f)
#endif

/** 
 *	@def CC_NODE_RENDER_SUBPIXEL
 *	If enabled, the CCNode objects (CCSprite, CCLabel,etc) will be able to render in subpixels.
 *	If disabled, integer pixels will be used.
 * 
 *	To enable set it to 1. Enabled by default.
 */
#ifndef CC_NODE_RENDER_SUBPIXEL
#define CC_NODE_RENDER_SUBPIXEL							1
#endif

/** 
 *	@def CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
 *	Use GL_TRIANGLE_STRIP instead of GL_TRIANGLES when rendering the texture atlas.
 *	It seems it is the recommend way, but it is much slower, so, enable it at your own risk
 * 
 *	To enable set it to a value different than 0. Disabled by default.
 *
 */
#ifndef CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
#define CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP				0	
#endif

/** 
 *	@def CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
 *	If enabled, the CCSprite objects rendered with CCSpriteBatchNode will be able to render in subpixels.
 *	If disabled, integer pixels will be used.
 * 
 *	To enable set it to 1. Enabled by default.
 */
#ifndef CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define CC_SPRITEBATCHNODE_RENDER_SUBPIXEL				1
#endif

/** 
 *	@def CC_TEXTURE_ATLAS_USE_VAO
 *	By default, CCTextureAtlas (used by many cocos2d classes) will use VAO (Vertex Array Objects).
 *	Apple recommends its usage but they might consume a lot of memory, specially if you use many of them.
 *	So for certain cases, where you might need hundreds of VAO objects, it might be a good idea to disable it.
 *
 *	To disable it set it to 0. Enabled by default.
 */
#ifndef CC_TEXTURE_ATLAS_USE_VAO
//#if !defined ( ANDROID )
//    #define CC_TEXTURE_ATLAS_USE_VAO					1
//#else
    // Some android devices cannot support VAO very well, so we disable it by default for android platform. 
    #define CC_TEXTURE_ATLAS_USE_VAO					0
//#endif
#endif

#ifndef CC_ENABLE_CACHE_TEXTURE_DATA
#if defined ( ANDROID )
	#define CC_ENABLE_CACHE_TEXTURE_DATA				1
	#define CC_USES_VBO									0
#else
	#define CC_ENABLE_CACHE_TEXTURE_DATA				0
	#define CC_USES_VBO									1
#endif
#endif

#endif	// __ccConfig_h__
