/* -----------------------------------------------------------------------------------
 *
 *      File            Cocos2D.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __Cocos2D_h__
#define __Cocos2D_h__


// 0x00 HI ME LO
// 00   03 00 00
#define COCOS2D_VERSION		0x00030000

//
// all cocos2d include files
//
#include "ccConfig.h"

// actions
#include "2d/actions/CCAction.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/actions/CCActionCamera.h"
#include "2d/actions/CCActionManager.h"
#include "2d/actions/CCActionEase.h"
#include "2d/actions/CCActionPageTurn3D.h"
#include "2d/actions/CCActionGrid.h"
#include "2d/actions/CCActionProgressTimer.h"
#include "2d/actions/CCActionGrid3D.h"
#include "2d/actions/CCActionTiledGrid.h"
#include "2d/actions/CCActionInstant.h"
#include "2d/actions/CCActionTween.h"
#include "2d/actions/CCActionCatmullRom.h"

// base_nodes
#include "2d/base_nodes/CCNode.h"
#include "2d/base_nodes/CCAtlasNode.h"

// cocoa
#include "base/CCAffineTransform.h"
#include "base/CCDictionary.h"
#include "base/CCObject.h"
#include "base/CCArray.h"
#include "base/CCGeometry.h"
#include "base/CCSet.h"
#include "base/CCAutoreleasePool.h"
#include "base/CCInteger.h"
#include "base/CCFloat.h"
#include "base/CCDouble.h"
#include "base/CCBool.h"
#include "base/CCString.h"
#include "base/CCNS.h"
#include "base/CCData.h"

// draw nodes
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "2d/draw_nodes/CCDrawNode.h"

// effects
#include "2d/effects/CCGrabber.h"
#include "2d/effects/CCGrid.h"

// include
#include "CCEventType.h"
#include "CCProtocols.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "ccTypes.h"

// label_nodes
#include "2d/label_nodes/CCLabelAtlas.h"
#include "2d/label_nodes/CCLabelTTF.h"
#include "2d/label_nodes/CCLabelBMFont.h"
#include "2d/label_nodes/CCLabel.h"

// layers_scenes_transitions_nodes
#include "2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
#include "2d/layers_scenes_transitions_nodes/CCTransition.h"
#include "2d/layers_scenes_transitions_nodes/CCTransitionPageTurn.h"
#include "2d/layers_scenes_transitions_nodes/CCTransitionProgress.h"

// menu_nodes
#include "2d/menu_nodes/CCMenu.h"
#include "2d/menu_nodes/CCMenuItem.h"

// misc_nodes
#include "2d/misc_nodes/CCClippingNode.h"
#include "2d/misc_nodes/CCMotionStreak.h"
#include "2d/misc_nodes/CCProgressTimer.h"
#include "2d/misc_nodes/CCRenderTexture.h"

// particle_nodes
#include "2d/particle_nodes/CCParticleBatchNode.h"
#include "2d/particle_nodes/CCParticleSystem.h"
#include "2d/particle_nodes/CCParticleExamples.h"
#include "2d/particle_nodes/CCParticleSystemQuad.h"

// physics
#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsShape.h"
#include "physics/CCPhysicsJoint.h"
#include "physics/CCPhysicsWorld.h"

// kazmath
#include "XMKazmath/kazmath.h"
#include "XMKazmath/GL/matrix.h"

// platform
#include "platform/CCDevice.h"
#include "platform/CCCommon.h"
#include "platform/CCFileUtils.h"
#include "platform/CCImage.h"
#include "platform/CCSAXParser.h"
#include "platform/CCThread.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCPlatformMacros.h"

#include "platform/CCApplication.h"
#include "platform/CCEGLView.h"
#include "platform/CCGL.h"
#include "platform/CCStdC.h"
/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "platform/ios/CCApplication.h"
    #include "platform/ios/CCEGLView.h"
    #include "platform/ios/CCGL.h"
    #include "platform/ios/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/CCApplication.h"
    #include "platform/android/CCEGLView.h"
    #include "platform/android/CCGL.h"
    #include "platform/android/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    #include "platform/blackberry/CCApplication.h"
    #include "platform/blackberry/CCEGLView.h"
    #include "platform/blackberry/CCGL.h"
    #include "platform/blackberry/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#include "platform/mac/CCApplication.h"
	#include "platform/mac/CCEGLView.h"
	#include "platform/mac/CCGL.h"
	#include "platform/mac/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include "platform/linux/CCApplication.h"
	#include "platform/linux/CCEGLView.h"
	#include "platform/linux/CCGL.h"
	#include "platform/linux/CCStdC.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
*/

// script_support
#include "2d/script_support/CCScriptSupport.h"

// shaders
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccShaders.h"

// sprite_nodes
#include "2d/sprite_nodes/CCAnimation.h"
#include "2d/sprite_nodes/CCAnimationCache.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/sprite_nodes/CCSpriteBatchNode.h"
#include "2d/sprite_nodes/CCSpriteFrame.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"

// support
#include "support/ccUTF8.h"
#include "support/CCNotificationCenter.h"
#include "support/CCProfiling.h"
#include "support/user_default/CCUserDefault.h"
#include "support/CCVertex.h"
#include "support/zip_support/ZipUtils.h"

// text_input_node
#include "2d/text_input_node/CCIMEDelegate.h"
#include "2d/text_input_node/CCIMEDispatcher.h"
#include "2d/text_input_node/CCTextFieldTTF.h"

// textures
#include "2d/textures/CCTexture2D.h"
#include "2d/textures/CCTextureAtlas.h"
#include "2d/textures/CCTextureCache.h"

// tilemap_parallax_nodes
#include "2d/tilemap_parallax_nodes/CCParallaxNode.h"
#include "2d/tilemap_parallax_nodes/CCTMXLayer.h"
#include "2d/tilemap_parallax_nodes/CCTMXObjectGroup.h"
#include "2d/tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "2d/tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "2d/tilemap_parallax_nodes/CCTileMapAtlas.h"

// EventDispatcher
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"
#include "2d/event_dispatcher/CCEventTouch.h"
#include "2d/event_dispatcher/CCEventListenerKeyboard.h"
#include "2d/event_dispatcher/CCEventKeyboard.h"
#include "2d/event_dispatcher/CCEventListenerMouse.h"
#include "2d/event_dispatcher/CCEventMouse.h"
#include "2d/event_dispatcher/CCEventAcceleration.h"
#include "2d/event_dispatcher/CCEventListenerAcceleration.h"
#include "2d/event_dispatcher/CCEventCustom.h"
#include "2d/event_dispatcher/CCEventListenerCustom.h"

// root
#include "2d/CCCamera.h"
#include "2d/CCConfiguration.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"

// component
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"

#include "audio/SimpleAudioEngine.h"

#include "platform/CCApplication.h"

NS_CC_BEGIN

CC_DLL const KDchar*	cocos2dVersion ( KDvoid );

NS_CC_END

#endif // __Cocos2D_h__
