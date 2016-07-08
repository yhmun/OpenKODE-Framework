/* -----------------------------------------------------------------------------------
 *
 *      File            ccMacros.h
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

#ifndef __ccMacros_h__
#define __ccMacros_h__

#include <kode.h>

#define NS_CC_BEGIN												namespace cocos2d {
#define NS_CC_END												}
#define USING_NS_CC												using namespace cocos2d

#define CC_ASSERT( __COND__ )									kdAssert ( __COND__ )

#if defined ( SHP ) || defined ( TIZEN ) || defined ( __APPLE__ )
#define CC_UNUSED_PARAM( __PARAM__ )
#else
#define CC_UNUSED_PARAM( __PARAM__ )							__PARAM__
#endif

#if defined(_MSC_VER)
#define CC_FORMAT_PRINTF_SIZE_T									"%08lX"
#else
#define CC_FORMAT_PRINTF_SIZE_T									"%08zX"
#endif

#define CREATE_FUNC( __TYPE__ )									\
static __TYPE__*  create ( KDvoid )								\
{																\
	__TYPE__*  __RET = new __TYPE__ ( );						\
																\
	if ( __RET && __RET->init ( ) )								\
	{															\
		__RET->autorelease ( );									\
	}															\
	else														\
	{															\
		CC_SAFE_DELETE ( __RET );								\
	}															\
																\
	return __RET;												\
}; 

#define SCENE_FUNC( __TYPE__ )									\
static CCScene*  scene ( KDvoid )								\
{																\
	CCScene*	pRet = KD_NULL;									\
	__TYPE__*   pLyr = new __TYPE__ ( );						\
																\
	if ( pLyr && pLyr->init ( ) )								\
	{															\
		pLyr->autorelease ( );									\
		pRet = CCScene::create ( );								\
		pRet->addChild ( pLyr );								\
	}															\
	else														\
	{															\
		CC_SAFE_DELETE ( pLyr );								\
	}															\
																\
	return pRet;												\
}; 

/**
 *	@def CC_SWAP
 *	simple macro that swaps 2 __VAR__iables
 */
#define CC_SWAP( __X__, __Y__, __TYPE__ )						\
{	__TYPE__  __TEMP = ( __X__ );								\
	__X__ = __Y__; __Y__ = __TEMP;								\
}

/**
 *	@def CCRANDOM_MINUS1_1
 *	returns a random float between -1 and 1
 */
#define CCRANDOM_MINUS1_1( )									( ( 2.0f * ( (KDfloat) kdRand ( ) / KD_RAND_MAX ) ) - 1.0f )

/**
 *	@def CCRANDOM_0_1
 *	returns a random float between 0 and 1
 */
#define CCRANDOM_0_1( )											( (KDfloat) kdRand ( ) / KD_RAND_MAX )

#define CCRANDOM_BETWEEN( _LOW, _HIGH )							( CCRANDOM_0_1 ( ) * ( _HIGH - _LOW )  + _LOW )

/**
 *	@def CC_DEGREES_TO_RADIANS
 *	converts degrees to radians
 */
#define CC_DEGREES_TO_RADIANS( __ANGLE__ )						( ( __ANGLE__ ) * 0.01745329252f ) 

/**
 *	@def CC_RADIANS_TO_DEGREES
 *	converts radians to degrees
 */
#define CC_RADIANS_TO_DEGREES( __ANGLE__ )						( ( __ANGLE__ ) * 57.29577951f )

#define kCCRepeatForever										( KDUINT_MAX - 1 )

/**
 *	@def CC_BLEND_SRC
 *	default gl blend src function. Compatible with premultiplied alpha images.
 */
#define CC_BLEND_SRC											GL_ONE

/**
 *	@def CC_BLEND_DST
 *	default gl blend dst function. Compatible with premultiplied alpha images.
 */
#define CC_BLEND_DST											GL_ONE_MINUS_SRC_ALPHA


#define CC_PROPERTY_READONLY( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )							\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual __VAR_TYPE__ get##__FUNC_NAME__ ( KDvoid );

#define CC_PROPERTY_READONLY_PASS_BY_REF( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )				\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual const __VAR_TYPE__& get##__FUNC_NAME__ ( KDvoid );

#define CC_PROPERTY( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )									\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual __VAR_TYPE__ get##__FUNC_NAME__ ( KDvoid );										\
public : virtual KDvoid set##__FUNC_NAME__ ( __VAR_TYPE__ __VAR__ );

#define CC_PROPERTY_BOOL( __VAR_NAME__, __FUNC_NAME__ )												\
protected : KDbool  __VAR_NAME__;																	\
public : virtual KDbool  is##__FUNC_NAME__ ( KDvoid );												\
public : virtual KDvoid set##__FUNC_NAME__ ( KDbool __VAR__ );

#define CC_PROPERTY_PASS_BY_REF( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )						\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual const __VAR_TYPE__& get##__FUNC_NAME__ ( KDvoid );									\
public : virtual KDvoid set##__FUNC_NAME__ ( const __VAR_TYPE__& __VAR__ );

#define CC_SYNTHESIZE_READONLY( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )							\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual __VAR_TYPE__ get##__FUNC_NAME__ ( KDvoid ) { return __VAR_NAME__; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )				\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual const __VAR_TYPE__& get##__FUNC_NAME__ ( KDvoid ) const { return __VAR_NAME__; }

#define CC_SYNTHESIZE( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )									\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual __VAR_TYPE__ get##__FUNC_NAME__ ( KDvoid ) { return __VAR_NAME__; }				\
public : virtual KDvoid  set##__FUNC_NAME__ ( __VAR_TYPE__ __VAR__ ) { __VAR_NAME__ = __VAR__; }

#define CC_SYNTHESIZE_BOOL( __VAR_NAME__, __FUNC_NAME__ )											\
protected : KDbool __VAR_NAME__;																	\
public : virtual KDbool   is##__FUNC_NAME__ ( KDvoid ) { return __VAR_NAME__; }						\
public : virtual KDvoid  set##__FUNC_NAME__ ( KDbool __VAR__ ) { __VAR_NAME__ = __VAR__; }

#define CC_SYNTHESIZE_PASS_BY_REF( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )						\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual const __VAR_TYPE__& get##__FUNC_NAME__ ( KDvoid ) { return __VAR_NAME__; }			\
public : virtual KDvoid set##__FUNC_NAME__ ( const __VAR_TYPE__& __VAR__ ) { __VAR_NAME__ = __VAR__; }

#define CC_SYNTHESIZE_RETAIN( __VAR_TYPE__, __VAR_NAME__, __FUNC_NAME__ )							\
protected : __VAR_TYPE__ __VAR_NAME__;																\
public : virtual __VAR_TYPE__ get##__FUNC_NAME__ ( KDvoid ) const { return __VAR_NAME__; }			\
public : virtual KDvoid set##__FUNC_NAME__ ( __VAR_TYPE__ __VAR__ )									\
{																									\
    if ( __VAR_NAME__ != __VAR__ )																	\
    {																								\
        CC_SAFE_RETAIN  ( __VAR__ );																\
        CC_SAFE_RELEASE ( __VAR_NAME__ );															\
        __VAR_NAME__ = __VAR__;																		\
    }																								\
} 

#define CC_SAFE_DELETE( __P__ )									if ( __P__ ) { delete   (__P__);	 (__P__) = KD_NULL; }
#define CC_SAFE_DELETE_ARRAY( __P__ )							if ( __P__ ) { delete[] (__P__);	 (__P__) = KD_NULL; }
#define CC_SAFE_FREE( __P__ )									if ( __P__ ) { kdFree ( (__P__) );	 (__P__) = KD_NULL; }
#define CC_SAFE_RELEASE( __P__ )								if ( __P__ ) { (__P__)->release ( ); (__P__) = KD_NULL; }
#define CC_SAFE_RETAIN( __P__ )									if ( __P__ ) { (__P__)->retain  ( );					}
#define CC_BREAK_IF( __COND__ )									if ( __COND__ ) break;

/**
 *	@def CC_DIRECTOR_END
 *	Stops and removes the director from memory.
 *	Removes the CCGLView from its parent
 *
 *	@since v0.99.4
 */
#define CC_DIRECTOR_END( )										CCDirector::sharedDirector ( )->end ( )


/** 
 *	@def CC_CONTENT_SCALE_FACTOR
 *	On Mac it returns 1;
 *	On iPhone it returns 2 if RetinaDisplay is On. Otherwise it returns 1
 */
#define CC_CONTENT_SCALE_FACTOR( )								CCDirector::sharedDirector ( )->getContentScaleFactor ( )

/**
 *	@def CC_RECT_PIXELS_TO_POINTS
 *	Converts a rect in pixels to points
 */
#define CC_RECT_PIXELS_TO_POINTS( __VAL__ )																			\
CCRectMake ( __VAL__.origin.x / CC_CONTENT_SCALE_FACTOR ( ), __VAL__.origin.y / CC_CONTENT_SCALE_FACTOR ( ),		\
			 __VAL__.size.cx  / CC_CONTENT_SCALE_FACTOR ( ), __VAL__.size.cy  / CC_CONTENT_SCALE_FACTOR ( ) )

/**
 *	@def CC_RECT_POINTS_TO_PIXELS
 *	Converts a rect in points to pixels
 */
#define CC_RECT_POINTS_TO_PIXELS( __VAL__ )																			\
CCRectMake ( __VAL__.origin.x * CC_CONTENT_SCALE_FACTOR ( ), __VAL__.origin.y * CC_CONTENT_SCALE_FACTOR ( ),		\
             __VAL__.size.cx  * CC_CONTENT_SCALE_FACTOR ( ), __VAL__.size.cy  * CC_CONTENT_SCALE_FACTOR ( ) )

/** 
 *	@def CC_POINT_PIXELS_TO_POINTS
 *	Converts a rect in pixels to points
 */
#define CC_POINT_PIXELS_TO_POINTS( __VAL__ )																		\
CCPointMake( __VAL__.x / CC_CONTENT_SCALE_FACTOR ( ), __VAL__.y / CC_CONTENT_SCALE_FACTOR ( ) )

/** 
 *	@def CC_POINT_POINTS_TO_PIXELS
 *	Converts a rect in points to pixels
 */
#define CC_POINT_POINTS_TO_PIXELS( __VAL__ )																		\
CCPointMake( __VAL__.x * CC_CONTENT_SCALE_FACTOR ( ), __VAL__.y * CC_CONTENT_SCALE_FACTOR ( ) )

/** 
 *	@def CC_POINT_PIXELS_TO_POINTS
 *	Converts a rect in pixels to points
 */
#define CC_SIZE_PIXELS_TO_POINTS( __VAL__ )																			\
CCSizeMake ( __VAL__.cx / CC_CONTENT_SCALE_FACTOR ( ), __VAL__.cy / CC_CONTENT_SCALE_FACTOR ( ) )

/** 
 *	@def CC_POINT_POINTS_TO_PIXELS
 *	Converts a rect in points to pixels
 */
#define CC_SIZE_POINTS_TO_PIXELS( __VAL__ )																			\
CCSizeMake ( __VAL__.cx * CC_CONTENT_SCALE_FACTOR ( ), __VAL__.cy * CC_CONTENT_SCALE_FACTOR ( ) )

#define DISALLOW_COPY_AND_ASSIGN( __TYPE_NAME__ )				\
    __TYPE_NAME__ ( const __TYPE_NAME__& );                     \
    KDvoid	operator = ( const __TYPE_NAME__& )


/**
 *	Helper macros which converts 4-byte little/big endian 
 *	integral number to the machine native number representation
 * 
 *	It should work same as apples CFSwapInt32LittleToHost(..)
 */

/// when define returns true it means that our architecture uses big endian
#define CC_HOST_IS_BIG_ENDIAN						(KDbool)(*(KDuint16 *)"\0\xff" < 0x100) 
#define CC_SWAP32(i)								((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define CC_SWAP16(i)								((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define CC_SWAP_INT32_LITTLE_TO_HOST(i)				((CC_HOST_IS_BIG_ENDIAN == KD_TRUE)? CC_SWAP32(i) : (i) )
#define CC_SWAP_INT16_LITTLE_TO_HOST(i)				((CC_HOST_IS_BIG_ENDIAN == KD_TRUE)? CC_SWAP16(i) : (i) )
#define CC_SWAP_INT32_BIG_TO_HOST(i)				((CC_HOST_IS_BIG_ENDIAN == KD_TRUE)? (i) : CC_SWAP32(i) )
#define CC_SWAP_INT16_BIG_TO_HOST(i)				((CC_HOST_IS_BIG_ENDIAN == KD_TRUE)? (i):  CC_SWAP16(i) )

#define CC_COLOR_4UB( c4UB )													\
{																				\
	ccColor4F   c4F = ccc4FFromccc4B ( c4UB );									\
    glColor4f ( c4F.r, c4F.g, c4F.b, c4F.a );									\
}

//
// Profiling Macros 
//
#define CC_PROFILER_DISPLAY_TIMERS( )				
#define CC_PROFILER_PURGE_ALL( ) 

#define CC_PROFILER_START( __NAME__ )  
#define CC_PROFILER_STOP(  __NAME__ ) 
#define CC_PROFILER_RESET( __NAME__ ) 

#define CC_PROFILER_START_CATEGORY( __CAT__, __NAME__ ) 
#define CC_PROFILER_STOP_CATEGORY(  __CAT__, __NAME__ ) 
#define CC_PROFILER_RESET_CATEGORY( __CAT__, __NAME__ ) 

#define CC_PROFILER_START_INSTANCE( __ID__, __NAME__ ) 
#define CC_PROFILER_STOP_INSTANCE(  __ID__, __NAME__ ) 
#define CC_PROFILER_RESET_INSTANCE( __ID__, __NAME__ ) 

#ifndef COCOS2D_DEBUG
#define COCOS2D_DEBUG											1
#endif

#if !defined ( COCOS2D_DEBUG ) || COCOS2D_DEBUG == 0
    #define CCLOG( __FORMAT__, ... )                     
    #define CCLOGINFO( __FORMAT__, ... )                 
    #define CCLOGERROR( __FORMAT__, ... ) 
	#define CCLOGWARN( __FORMAT__, ... ) 
#elif COCOS2D_DEBUG == 1
    #define CCLOG( __FORMAT__, ... )							kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
    #define CCLOGINFO( __FORMAT__, ... )						do { } while ( 0 )
    #define CCLOGERROR( __FORMAT__, ... )						kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
	#define CCLOGWARN( __FORMAT__, ... )						kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
#else
    #define CCLOG( __FORMAT__, ... )							kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
    #define CCLOGINFO( __FORMAT__, ... )						kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
    #define CCLOGERROR( __FORMAT__, ... )						kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
	#define CCLOGWARN( __FORMAT__, ... )						kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
#endif

// Lua engine debug
#if !defined ( COCOS2D_DEBUG ) || COCOS2D_DEBUG == 0 || CC_LUA_ENGINE_DEBUG == 0
	#define LUALOG( ... )
#else
	#define LUALOG( __FORMAT__, ... )							kdPrintf ( __FORMAT__, ##__VA_ARGS__ )
#endif // Lua engine debug

#define CCAssert( __CONDITION__, __FORMAT__, ... )													\
if ( !( __CONDITION__ ) )																			\
{																									\
	const KDchar*  __TEXT = "";																		\
	if ( __FORMAT__ )																				\
	{																								\
		__TEXT = CCString::createWithFormat ( __FORMAT__, ##__VA_ARGS__ )->getCString ( );			\
	}																								\
																									\
	if ( kdStrlen ( __TEXT ) > 0 )																	\
	{																								\
		kdAssertion ( __TEXT );																		\
	}																								\
	else																							\
	{																								\
		kdAssert ( __CONDITION__ );																	\
	}																								\
}

/** @def CC_NODE_DRAW_SETUP
 *	Helpful macro that setups the GL server state, the correct GL program and sets the Model View Projection matrix
 *	@since v2.0
 */
#define CC_NODE_DRAW_SETUP( )																		\
do																									\
{																									\
    ccGLEnable ( m_eGLServerState );																\
    CCAssert ( getShaderProgram ( ), "No shader program set for this node" );						\
    getShaderProgram ( )->use ( );																	\
    getShaderProgram ( )->setUniformsForBuiltins ( );												\
} while ( 0 )

#if !defined ( COCOS2D_DEBUG ) || COCOS2D_DEBUG < 2
#define CHECK_GL_ERROR_DEBUG( )
#else
#define CHECK_GL_ERROR_DEBUG( )																		\
do																									\
{																									\
    GLenum  __ERROR = glGetError ( );																\
    if ( __ERROR )																					\
	{																								\
        CCLOG ( "OpenGL error 0x%04X in %s %s %d\n", __ERROR, __FILE__, __FUNCTION__, __LINE__ );	\
    }																								\
} while ( 0)
#endif

/** @def CC_INCREMENT_GL_DRAWS_BY_ONE
 *	Increments the GL Draws counts by one.
 *	The number of calls per frame are displayed on the screen when the CCDirector's stats are enabled.
 */
NS_CC_BEGIN
extern KDuint	g_uNumberOfDraws;
NS_CC_END

#define CC_INCREMENT_GL_DRAWS( __N__ )			g_uNumberOfDraws += __N__

/*******************/
/** Notifications **/
/*******************/
/** 
 *	@def CCAnimationFrameDisplayedNotification
 *	Notification name when a CCSpriteFrame is displayed
 */
#define CCAnimationFrameDisplayedNotification	"CCAnimationFrameDisplayedNotification"

/**
 *	This header is used for defining event types using in CCNotificationCenter
 */

// The application will come to foreground.
// This message is used for reloading resources before come to foreground on Android.
// This message is posted in main.cpp.
#define EVENT_COME_TO_FOREGROUND				"event_come_to_foreground"

// The application will come to background.
// This message is used for doing something before coming to background, such as save CCRenderTexture.
// This message is posted in cocos2dx/platform/android/jni/MessageJni.cpp.
#define EVENT_COME_TO_BACKGROUND				"event_come_to_background"

#endif // __ccMacros_h__
