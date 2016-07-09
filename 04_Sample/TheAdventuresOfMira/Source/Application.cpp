/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
 *      Description     Application Entrance.
 *
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "platform.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	// Initalize Frame rate and debug display
	CCDirector::sharedDirector ( )->setAnimationInterval ( 1 / 60.0 );
//	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );

    std::vector<std::string>	aPaths;
	aPaths.push_back ( "sfx" );
	aPaths.push_back ( "spritesheets" );
	aPaths.push_back ( "etc" );
	
	CCFileUtils::sharedFileUtils ( )->setSearchPaths ( aPaths );

    TargetPlatform		eTarget = getTargetPlatform ( );

    DeviceSize			eDeviceSize = kDeviceSizeNormal;
    DeviceType			eDeviceType = kDeviceTypeAndroid800x480;

	CCSize				tDesignSize = ccs ( 800, 480 );
	ResolutionPolicy	eResPolicy  = kResolutionExactFit;
	const KDchar*		szResPlist  = "android_800x480.plist";
	const KDchar*		szResFile	= "android_800x480.png";	

	// ANDROID
    if ( eTarget == kCCTargetAndroid )
    {
        if ( SCREEN_WIDTH > 1280 )
        {
			eDeviceType = kDeviceTypeAndroid1280x800;
			eDeviceSize = kDeviceSizeLarge;
			eResPolicy	= kResolutionNoBorder;
			szResPlist	= "android_1280x800.plist";
			szResFile	= "android_1280x800.png";	
			tDesignSize = ccp ( 1280, 800 );
        }
        else if ( SCREEN_WIDTH == 1280 && ( SCREEN_HEIGHT <= 800 && SCREEN_HEIGHT >= 730 ) )
        {
			eDeviceType = kDeviceTypeAndroid1280x800;
			eDeviceSize = kDeviceSizeLarge;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_1280x800.plist";
			szResFile	= "android_1280x800.png";
			tDesignSize = ccp ( 1280, 800 );
        }
        else if ( SCREEN_WIDTH == 1280 && ( SCREEN_HEIGHT <= 720 && SCREEN_HEIGHT >= 650 ) )
        {
			eDeviceType = kDeviceTypeAndroid1280x720;
			eDeviceSize = kDeviceSizeLarge;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_1280x720.plist";
			szResFile	= "android_1280x720.png";
			tDesignSize = ccp ( 1280, 720 );
        }
        else if ( SCREEN_WIDTH == 1024 && ( SCREEN_HEIGHT <= 600 && SCREEN_HEIGHT >= 530 ) )
        {
            eDeviceType = kDeviceTypeAndroid1024x600;
            eDeviceSize = kDeviceSizeLarge;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_1024x600.plist";
			szResFile	= "android_1024x600.png";
			tDesignSize = ccp ( 1024, 600 );
        }
        else if ( SCREEN_WIDTH == 854 && ( SCREEN_HEIGHT <= 480 && SCREEN_HEIGHT >= 410 ) )
        {
            eDeviceType = kDeviceTypeAndroid854x480;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_854x480.plist";
			szResFile	= "android_854x480.png";
			tDesignSize = ccp ( 854, 480 );
        }
        else if ( SCREEN_WIDTH == 800 && ( SCREEN_HEIGHT <= 480 && SCREEN_HEIGHT >= 410 ) )
        {
            eDeviceType = kDeviceTypeAndroid800x480;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_800x480.plist";
			szResFile	= "android_800x480.png";
			tDesignSize = ccp ( 800, 480 );
        }
        else if ( SCREEN_WIDTH == 800 && ( SCREEN_HEIGHT <= 400 && SCREEN_HEIGHT >= 330 ) )
        {
            eDeviceType = kDeviceTypeAndroid800x480;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_800x480.plist";
			szResFile	= "android_800x480.png";
			tDesignSize = ccp ( 800, 480 );
        }
        else if ( SCREEN_WIDTH == 720 && ( SCREEN_HEIGHT <= 480 && SCREEN_HEIGHT >= 410 ) )
        {
            eDeviceType = kDeviceTypeAndroid720x480;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "android_720x480.plist";
			szResFile	= "android_720x480.png";
			tDesignSize = ccp ( 720, 480 );
        }
        else
        {
            eDeviceType = kDeviceTypeAndroid800x400;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionNoBorder;
			szResPlist	= "android_800x480.plist";
			szResFile	= "android_800x480.png";
			tDesignSize = ccp ( 800, 480 );
        }
    }
    // iOS
    else if ( eTarget == kCCTargetIphone )
    {
        // iPad 3
        if ( SCREEN_WIDTH == 2048 )
        {
            eDeviceType = kDeviceTypeiPad3;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "spritesheet-ipad3.plist";
			szResFile	= "spritesheet-ipad3.png";
			tDesignSize = ccp ( 2048, 1536 );
        }
        // iPad 1, 2
        else if ( SCREEN_WIDTH == 1024 )
        {
            eDeviceType = kDeviceTypeiPad;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "spritesheet-ipad.plist";
			szResFile	= "spritesheet-ipad.png";
			tDesignSize = ccp ( 1024, 768 );
        }
        // iPhone 5 Retina Display
        else if ( SCREEN_WIDTH == 1136 || SCREEN_WIDTH == 568 )
        {
            eDeviceType = kDeviceTypeiPhone5;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "spritesheet-iphone5-hd.plist";
			szResFile	= "spritesheet-iphone5-hd.png";
			tDesignSize = ccp ( 1136, 640 );
        }
        // iPhone 4 Retina Display
        else if ( SCREEN_WIDTH == 960 )
        {
            eDeviceType = kDeviceTypeiPhone4;
            eDeviceSize = kDeviceSizeNormal;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "spritesheet-iphone-hd.plist";
			szResFile	= "spritesheet-iphone-hd.png";
			tDesignSize = ccp ( 960, 640 );
        }
        // iPhone3G, iPod Touch
        else
        {
            eDeviceType = kDeviceTypeiPhone;
            eDeviceSize = kDeviceSizeSmall;
			eResPolicy	= kResolutionExactFit;
			szResPlist	= "spritesheet-iphone.plist";
			szResFile	= "spritesheet-iphone.png";
			tDesignSize = ccp ( 480, 320 );
        }
    }

	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( tDesignSize, eResPolicy );

	// setup the texture cache using our spritesheet image
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( szResPlist );
	CCTextureCache::sharedTextureCache ( )->addImage ( szResFile );	

    // initialize the game manager first before the scene
    GameManager::sharedGameManager ( )->setDeviceSize ( eDeviceSize );
    GameManager::sharedGameManager ( )->setDeviceType ( eDeviceType );

    // create a scene. it's an autorelease object
    CCScene*	pScene = GameSceneLayer::scene ( );
    GameManager::sharedGameManager ( )->showMainMenu ( );

    // run
    CCDirector::sharedDirector ( )->runWithScene ( pScene );	

	return KD_TRUE;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{
	GameManager::purgedGameManager ( );
}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{

}

KDvoid AppDelegate::applicationDidEnterBackground  ( KDvoid )
{

}
