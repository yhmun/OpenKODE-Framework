/* -----------------------------------------------------------------------------------
 *
 *      File            Cocos2D.h
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

#ifndef __Cocos2D_h__
#define __Cocos2D_h__

// 0x00 HI ME LO
// 00   02 01 00
#define COCOS2D_VERSION		0x00020100

//
// all cocos2d include files
//
#include "ccConfig.h"              
#include "ccMacros.h"
#include "ccTypes.h"
#include "CCProtocols.h"

// accelerometer_dispatcher
#include "accelerometer_dispatcher/CCAccelerometer.h"

// actions
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionCamera.h"
#include "actions/CCActionManager.h"
#include "actions/CCActionEase.h"
#include "actions/CCActionPageTurn3D.h"
#include "actions/CCActionGrid.h"
#include "actions/CCActionProgressTimer.h"
#include "actions/CCActionGrid3D.h"
#include "actions/CCActionTiledGrid.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionTween.h"
#include "actions/CCActionCatmullRom.h"

// base_nodes
#include "base_nodes/CCNode.h"
#include "base_nodes/CCAtlasNode.h"

// cocoa
#include "cocoa/CCAffineTransform.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCGeometry.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCAutoreleasePool.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCFloat.h"
#include "cocoa/CCDouble.h"
#include "cocoa/CCBool.h"
#include "cocoa/CCPointer.h"
#include "cocoa/CCString.h"
#include "cocoa/CCData.h"
#include "cocoa/CCValue.h"
#include "cocoa/CCNS.h"
#include "cocoa/CCZone.h"

// denshion
#include "denshion/SimpleAudioEngine.h"
#include "denshion/CCSound.h"

// draw_nodes
#include "draw_nodes/CCDrawingPrimitives.h"
#include "draw_nodes/CCDrawNode.h"

// effects
#include "effects/CCGrabber.h"
#include "effects/CCGrid.h"

// event_dispatcher
#include "event_dispatcher/CCEventDelegate.h"
#include "event_dispatcher/CCEventDispatcher.h"

// kazmath
#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"

// keypad_dispatcher
#include "keypad_dispatcher/CCKeypadDelegate.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"

// label_nodes
#include "label_nodes/CCLabelAtlas.h"
#include "label_nodes/CCLabelTTF.h"
#include "label_nodes/CCLabelBMFont.h"

// layers_scenes_transitions_nodes
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "layers_scenes_transitions_nodes/CCScene.h"
#include "layers_scenes_transitions_nodes/CCTransition.h"
#include "layers_scenes_transitions_nodes/CCTransitionPageTurn.h"
#include "layers_scenes_transitions_nodes/CCTransitionProgress.h"
#include "layers_scenes_transitions_nodes/CCThreadLayer.h"

// menu_nodes
#include "menu_nodes/CCMenu.h"
#include "menu_nodes/CCMenuItem.h"

// misc_nodes
#include "misc_nodes/CCClippingNode.h"
#include "misc_nodes/CCMotionStreak.h"
#include "misc_nodes/CCProgressTimer.h"
#include "misc_nodes/CCProgress.h"
#include "misc_nodes/CCRenderTexture.h"
#include "misc_nodes/CCProgress.h"

// particle_nodes
#include "particle_nodes/CCParticleBatchNode.h"
#include "particle_nodes/CCParticleSystem.h"
#include "particle_nodes/CCParticleExamples.h"
#include "particle_nodes/CCParticleSystemQuad.h"

// script_support
#include "script_support/CCScriptSupport.h"

// shaders
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccShaders.h"

// sprite_nodes
#include "sprite_nodes/CCAnimation.h"
#include "sprite_nodes/CCAnimationCache.h"
#include "sprite_nodes/CCSprite.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSpriteFrame.h"
#include "sprite_nodes/CCSpriteFrameCache.h"

// support
#include "support/CCPointExtension.h"
#include "support/CCProfiling.h"
#include "support/user_default/CCUserDefault.h"
#include "support/CCVertex.h"
#include "support/CCFileUtils.h"
#include "support/CCImage.h"
#include "support/CCSAXParser.h"
#include "support/TransformUtils.h"
#include "support/ccUtils.h"
#include "support/ccUTF8.h"

#include "support/CCCamera.h"
#include "support/CCConfiguration.h"
#include "support/CCDirector.h"
#include "support/CCEGLView.h"
#include "support/CCScheduler.h"
#include "support/CCNotificationCenter.h"

// text_input_node
#include "text_input_node/CCIMEDelegate.h"
#include "text_input_node/CCIMEDispatcher.h"
#include "text_input_node/CCTextFieldTTF.h"

// textures
#include "textures/CCTexture2D.h"
#include "textures/CCTextureAtlas.h"
#include "textures/CCTextureCache.h"

// tilemap_parallax_nodes
#include "tilemap_parallax_nodes/CCParallaxNode.h"
#include "tilemap_parallax_nodes/CCTMXLayer.h"
#include "tilemap_parallax_nodes/CCTMXObjectGroup.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "tilemap_parallax_nodes/CCTileMapAtlas.h"
#include "tilemap_parallax_nodes/CCDiamondTiledLayer.h"

// touch_dispatcher
#include "touch_dispatcher/CCTouch.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouchHandler.h"

// component
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"

// extensions
#include "extensions/CCNetwork/HttpClient.h"
#include "extensions/CCPhysics/CCBox2DDebugDraw.h"
#include "extensions/CCPhysics/CCPhysicsDebugNode.h"
#include "extensions/CCPhysics/CCPhysicsSprite.h"
#include "extensions/CCControl/CCScale9Sprite.h"
#include "extensions/CCControl/CCControl.h"
#include "extensions/CCControl/CCControlButton.h"
#include "extensions/CCControl/CCControlColourPicker.h"
#include "extensions/CCControl/CCControlPotentiometer.h"
#include "extensions/CCControl/CCControlSlider.h"
#include "extensions/CCControl/CCControlStepper.h"
#include "extensions/CCControl/CCControlSwitch.h"
#include "extensions/CCEditBox/CCEditBox.h"
#include "extensions/CCScrollView/CCScrollView.h"
#include "extensions/CCScrollView/CCTableView.h"
#include "extensions/CCScrollLayer/CCScrollLayer.h"
#include "extensions/CCBReader/CCBFileLoader.h"
#include "extensions/CCBReader/CCBMemberVariableAssigner.h"
#include "extensions/CCBReader/CCBRelativePositioning.h"
#include "extensions/CCBReader/CCBSelectorResolver.h"
#include "extensions/CCBReader/CCBAnimationManager.h"
#include "extensions/CCBReader/CCNodeLoader.h"
#include "extensions/CCBReader/CCNodeLoaderListener.h"
#include "extensions/CCBReader/CCNodeLoaderLibrary.h"
#include "extensions/CCBReader/CCLabelBMFontLoader.h"
#include "extensions/CCBReader/CCLabelTTFLoader.h"
#include "extensions/CCBReader/CCLayerColorLoader.h"
#include "extensions/CCBReader/CCLayerGradientLoader.h"
#include "extensions/CCBReader/CCLayerLoader.h"
#include "extensions/CCBReader/CCMenuItemImageLoader.h"
#include "extensions/CCBReader/CCMenuItemLoader.h"
#include "extensions/CCBReader/CCMenuLoader.h"
#include "extensions/CCBReader/CCParticleSystemQuadLoader.h"
#include "extensions/CCBReader/CCScale9SpriteLoader.h"
#include "extensions/CCBReader/CCScrollViewLoader.h"
#include "extensions/CCBReader/CCSpriteLoader.h"
#include "extensions/CCArtPig/CCArtPig.h"
#include "extensions/CCSpaceManager/CCSpaceManager.h"
#include "extensions/CCAssetsManager/AssetsManager.h"
#include "extensions/CCLocalStorage/LocalStorage.h"
#include "extensions/CCSpine/SpineCocos2dx.h"
#include "extensions/CCGB2Shape/GB2ShapeCache.h"
#include "extensions/CCParallaxScroll/CCParallaxScrollNode.h"
#include "extensions/CCVRope/vrope.h"
#include "extensions/CCPolygonSprite/PolygonSprite.h"
#include "extensions/CCBlade/CCBlade.h"
#include "CCApplication.h"

#endif	// __Cocos2D_h__
