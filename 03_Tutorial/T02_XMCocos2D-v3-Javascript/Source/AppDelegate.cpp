#include "cocos2d.h"
#include "scripting/cocos-script.h"

#define XM_EMULATOR_CX  640
#define XM_EMULATOR_CY  960

#include "platform.h"

USING_NS_CC;

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_ACCELEROMETER );

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto		pDirector = Director::getInstance ( );
    pDirector->setOpenGLView ( EGLView::getInstance ( ) );

	// JS-Test in Html5 uses 800x450 as design resolution
	EGLView::getInstance ( )->setDesignResolutionSize ( 800, 450, ResolutionPolicy::FIXED_HEIGHT );

    // turn on display FPS
    pDirector->setDisplayStats ( true );

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval ( 1.0 / 60 );

	FileUtils::getInstance ( )->addSearchPath ( "script" );
	FileUtils::getInstance ( )->addSearchPath ( "test" );
	FileUtils::getInstance ( )->addSearchPath ( "res" );
	FileUtils::getInstance ( )->addSearchPath ( "res/scenetest" );

    ScriptingCore*	sc = ScriptingCore::getInstance ( );

    sc->addRegisterCallback ( register_all_cocos2dx					 );	
    sc->addRegisterCallback ( register_all_cocos2dx_extension		 );
    sc->addRegisterCallback ( register_cocos2dx_js_extensions		 );
    sc->addRegisterCallback ( register_all_cocos2dx_extension_manual );

    sc->addRegisterCallback ( jsb_register_chipmunk					 );
    sc->addRegisterCallback ( JSB_register_opengl					 );
    sc->addRegisterCallback ( jsb_register_system					 );
    sc->addRegisterCallback ( MinXmlHttpRequest::_js_register		 );
    sc->addRegisterCallback ( register_jsb_websocket				 );

    sc->addRegisterCallback ( register_all_cocos2dx_builder			 );
    sc->addRegisterCallback ( register_CCBuilderReader				 );

    sc->addRegisterCallback ( register_all_cocos2dx_gui				 );
    sc->addRegisterCallback ( register_all_cocos2dx_gui_manual		 );
    sc->addRegisterCallback ( register_all_cocos2dx_studio			 );
    sc->addRegisterCallback ( register_all_cocos2dx_studio_manual	 );
	
	sc->start ( );

#if defined ( COCOS2D_DEBUG ) && ( COCOS2D_DEBUG > 0 )
    sc->enableDebugger ( );
#endif

    auto	pEngine = ScriptingCore::getInstance ( );
    ScriptEngineManager::getInstance ( )->setScriptEngine ( pEngine );
#ifdef JS_OBFUSCATED
    ScriptingCore::getInstance ( )->runScript ( "game.js" );
#else
    ScriptingCore::getInstance ( )->runScript ( "tests-boot-jsb.js" );
#endif

    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{

}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{

}
