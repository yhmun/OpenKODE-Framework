/* --------------------------------------------------------------------------
 *
 *      File            TestDef.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __TestDef_h__
#define __TestDef_h__

#include "Test/TestActions.h"
#include "Test/TestTransitions.h"
#include "Test/TestProgressActions.h"
#include "Test/TestEffects.h"
#include "Test/TestClickAndMove.h"
#include "Test/TestRotateWorld.h"
#include "Test/TestParticle.h"
#include "Test/TestEaseActions.h"
#include "Test/TestMotionStreak.h"
#include "Test/TestDrawPrimitives.h"
#include "Test/TestNode.h"
#include "Test/TestTouches.h"
#include "Test/TestMultiTouch.h"
#include "Test/TestMenu.h"
#include "Test/TestActionManager.h"
#include "Test/TestLayer.h"
#include "Test/TestScene.h"
#include "Test/TestParallax.h"
#include "Test/TestParallaxScrollNode.h"
#include "Test/TestTileMap.h"
#include "Test/TestDiamondTile.h"
#include "Test/TestInterval.h"
#include "Test/TestLabel.h"
#include "Test/TestFont.h"
#include "Test/TestFileUtils.h"
#include "Test/TestTextInput.h"
#include "Test/TestSprite.h"
#include "Test/TestScheduler.h"
#include "Test/TestRenderTexture.h"
#include "Test/TestTexture2D.h"
#include "Test/TestTextureCache.h"
#include "Test/TestBox2D.h"
#include "Test/TestBox2DBed.h"
#include "Test/TestChipmunk.h"
#include "Test/TestChipmunk2.h"
#include "Test/TestEffectsAdvance.h"
#include "Test/TestAccelerometer.h"
#include "Test/TestKeypad.h"
#include "Test/TestZwoptex.h"
#include "Test/TestUserDefault.h"
#include "Test/TestNotificationCenter.h"
#include "Test/TestControl.h"
#include "Test/TestEditBox.h"
#include "Test/TestTableView.h"
#include "Test/TestClippingNode.h"
#include "Test/TestCocosBuilder.h"
#include "Test/TestArtPig.h"
#include "Test/TestDenshion.h"
#include "Test/TestTTS.h"
#include "Test/TestCurl.h"
#include "Test/TestNetwork.h"
#include "Test/TestSqlite.h"
#include "Test/TestJson.h"
#include "Test/TestLocation.h"
#include "Test/TestCurrentLanguage.h"
#include "Test/TestAssetsManager.h"
#include "Test/TestSpine.h"
#include "Test/TestGB2Shape.h"
#include "Test/TestVRope.h"
#include "Test/TestBlade.h"
#include "Test/TestPerformance.h"
#include "Test/TestBugAndSupport.h"
#include "Test/TestShader.h"
#include "Test/TestGraphics.h"

const KDchar*  l_pTestNames [] =
{
	"Actions Test"				,
    "Transitions Test"			,
    "ProgressActions Test"		,
    "Effects Test"				,
    "Click & Move Test"			,
    "RotateWorld Test"			,
    "Particle Test"				,
    "EaseActions Test"			,
    "MotionStreak Test"			,
    "DrawPrimitives Test"		,
    "Node Test"					,
    "Touches Test"				,
	"Multi touch Test"			,
    "Menu Test"					,
    "ActionManager Test"		,
    "Layer Test"				,
    "Scene Test"				,
    "Parallax Test"				,
    "ParallaxScrollNode Test"	,
    "TileMap Test"				,
	"DiamondTile Test"			,
    "Interval Test"				,
    "Label Test"				,
	"Font Test"					,
    "TextInput Test"			,
    "Sprite Test"				,
    "Schduler Test"				,
    "RenderTexture Test"		,
    "Texture2D Test"			,
    "TextureCache Test"			,
    "Box2D Test"				,
    "Box2D Bed Test"			,
    "Chipmunk Test"				,
	"Chipmunk2 Test"			,
    "EffectAdvanced Test"		,
    "Accelerometer Test"		,
    "Keypad Test"				,
    "Zwoptex Test"				,
	"UserDefault Test"			,
	"NotificationCenter Test"	,
	"Control Test"				,
	"EditBox Test"				,
	"TableView Test"			,
	"ClippingNode Test"			,
	"CocosBuilder Test"			,
	"ArtPig Test"				,
	"Spine Test"				,
	"GB2Shape Test"				,
	"VRope Test"				,
	"Blade Test"				,
	"Denshion Test"				,
	"TTS Test"					,
	"Curl Test"					,
	"Network Test"				,
	"Sqlite Test"				,
	"Json Test"					,
	"Location Test"				,
	"Current Language Test"		,
	"FileUtils Test"			,
	"Assets Manager Test"		,
    "Performance Test"			,
    "Bug And Support Test"		,
#if defined ( USE_OPEN_GLES2 )	
	"Shader Test",
#else
	"Graphics Test",
#endif
};

#endif // __TestDef_h__
