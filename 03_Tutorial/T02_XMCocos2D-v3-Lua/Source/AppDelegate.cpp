#include "cocos2d.h"
#include "scripting/cocos-script.h"
#include "lua_assetsmanager_test_sample.h"

#define XM_EMULATOR_CX  640
#define XM_EMULATOR_CY  960

#include "platform.h"

USING_NS_CC;
USING_NS_CD;

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_ACCELEROMETER );

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	// initialize director
	auto	pDirector = Director::getInstance ( );
	pDirector->setOpenGLView ( EGLView::getInstance ( ) );

	// turn on display FPS
	pDirector->setDisplayStats ( true );

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval ( 1.0 / 60 );

	auto	tScreenSize = EGLView::getInstance ( )->getFrameSize ( );

	auto	tDesignSize = Size ( 480, 320 );

	auto	pFileUtils = FileUtils::getInstance ( );

	if ( tScreenSize.height > 320 )
	{
		auto	tResourceSize = Size ( 960, 640 );
		std::vector<std::string>	aSearchPaths;
		aSearchPaths.push_back ( "hd" );
		pFileUtils->setSearchPaths ( aSearchPaths );
		pDirector->setContentScaleFactor ( tResourceSize.height / tDesignSize.height );
	}

	EGLView::getInstance ( )->setDesignResolutionSize ( tDesignSize.width, tDesignSize.height, ResolutionPolicy::FIXED_HEIGHT );

	// register lua engine
	LuaEngine*	pEngine = LuaEngine::getInstance ( );
	ScriptEngineManager::getInstance ( )->setScriptEngine ( pEngine );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	LuaStack*	pStack = pEngine->getLuaStack ( );
	register_assetsmanager_test_sample ( pStack->getLuaState ( ) );
#endif

	std::vector<std::string>	aSearchPaths = pFileUtils->getSearchPaths ( );
	aSearchPaths.insert ( aSearchPaths.begin(), "Images" );
	aSearchPaths.insert ( aSearchPaths.begin(), "cocosbuilderRes" );
	if ( tScreenSize.height > 320 )
	{
		aSearchPaths.insert ( aSearchPaths.begin ( ), "hd/scenetest" );
	}
	else
	{
		aSearchPaths.insert ( aSearchPaths.begin ( ), "scenetest" );
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY
	aSearchPaths.push_back ( "TestCppResources" );
	aSearchPaths.push_back ( "script" );
#endif

	FileUtils::getInstance ( )->setSearchPaths ( aSearchPaths );

	pEngine->executeScriptFile ( "luaScript/controller.lua" );

    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{
	SimpleAudioEngine::end ( );
}

KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{
	Director::getInstance ( )->stopAnimation ( );

	SimpleAudioEngine::getInstance ( )->pauseBackgroundMusic ( );
}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
	Director::getInstance ( )->startAnimation ( );

	SimpleAudioEngine::getInstance ( )->resumeBackgroundMusic ( );
}
