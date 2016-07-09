#include "cocos2d.h"
#include "controller.h"

#define XM_EMULATOR_CX  640
#define XM_EMULATOR_CY  960

#include "platform.h"
//#include "SimpleAudioEngine.h"
//#include "cocostudio/CocoStudio.h"
//#include "extensions/cocos-ext.h"

USING_NS_CC;
//using namespace CocosDenshion;

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_ACCELEROMETER );

bool AppDelegate::applicationDidFinishLaunching()
{
	// As an example, load config file
	// XXX: This should be loaded before the Director is initialized,
	// XXX: but at this point, the director is already initialized
	Configuration::getInstance()->loadConfigFile("configs/config-example.plist");

    // initialize director
    auto director = Director::getInstance();
    director->setOpenGLView(EGLView::getInstance());

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    auto screenSize = EGLView::getInstance()->getFrameSize();

    auto designSize = Size(480, 320);

    auto pFileUtils = FileUtils::getInstance();
	std::vector<std::string> searchPaths;
    
    if (screenSize.height > 320)
    {
        auto resourceSize = Size(960, 640);
        searchPaths.push_back("hd");
		searchPaths.push_back("hd/scenetest");
        director->setContentScaleFactor(resourceSize.height/designSize.height);
    }
	else
	{
		searchPaths.push_back("scenetest");
	}
	searchPaths.push_back("ccb");
	pFileUtils->setSearchPaths(searchPaths);

    EGLView::getInstance()->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);

    auto scene = Scene::create();
    auto layer = new TestController();
    layer->autorelease();

    scene->addChild(layer);
    director->runWithScene(scene);

    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{
    Director::getInstance()->stopAnimation();
//    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
    Director::getInstance()->startAnimation();
//    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
//    SimpleAudioEngine::getInstance()->resumeAllEffects();
}
