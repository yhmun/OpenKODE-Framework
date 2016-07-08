/* --------------------------------------------------------------------------
 *
 *      File            Includes.h
 *      Author          Young-Hwan Mun
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

#ifndef __Includes_h__
#define __Includes_h__

#include "Recipes/Ch1_DrawingSprites.h"
#include "Recipes/Ch1_ColoringSprites.h"
#include "Recipes/Ch1_AnimatingSprites.h"
#include "Recipes/Ch1_OpenGLPrimitives.h"
#include "Recipes/Ch1_PlayingVideoFiles.h"
#include "Recipes/Ch1_GridEffects.h"
#include "Recipes/Ch1_ParticleEffects.h"
#include "Recipes/Ch1_MotionStreak.h"
#include "Recipes/Ch1_RetinaDisplay.h"
#include "Recipes/Ch1_1DEasingActions.h"
#include "Recipes/Ch1_2DEasingActions.h"
#include "Recipes/Ch1_3DCubes.h"
#include "Recipes/Ch1_RenderTexturedPolygon.h"
#include "Recipes/Ch1_AnimateTexturedPolygon.h"
#include "Recipes/Ch1_PaletteSwapping.h"
#include "Recipes/Ch1_MutablePaletteSwapping.h"
#include "Recipes/Ch1_UsingAWTextureFilter.h"
#include "Recipes/Ch1_RetroPixelatedFilter.h"
#include "Recipes/Ch1_TakingScreenshots.h"
#include "Recipes/Ch1_UsingCCParallaxNode.h"
#include "Recipes/Ch1_PixelFormat.h"
#include "Recipes/Ch1_ColorMaskLighting.h"
#include "Recipes/Ch2_TapHoldDragInput.h"
#include "Recipes/Ch2_DepthTestingInput.h"
#include "Recipes/Ch2_Buttons.h"
#include "Recipes/Ch2_DPad.h"
#include "Recipes/Ch2_AnalogStick.h"
#include "Recipes/Ch2_AccelerometerSteering.h"
#include "Recipes/Ch2_AccelerometerRotation.h"
#include "Recipes/Ch2_PinchZooming.h"
#include "Recipes/Ch2_Gestures.h"
#include "Recipes/Ch3_ArchivingObjects.h"
#include "Recipes/Ch3_ReadingPlistFiles.h"
#include "Recipes/Ch3_ReadingJsonFiles.h"
#include "Recipes/Ch3_ReadingXmlFiles.h"
#include "Recipes/Ch3_SavingSimpleData.h"
#include "Recipes/Ch3_MutatingNestedMetadata.h"
#include "Recipes/Ch3_SavingDataPlist.h"
#include "Recipes/Ch3_SavingDataSQLite.h"
#include "Recipes/Ch3_SavingDataCoreData.h"
#include "Recipes/Ch4_BasicSetup.h"
#include "Recipes/Ch4_CollisionResponse.h"
#include "Recipes/Ch4_DifferentShapes.h"
#include "Recipes/Ch4_DraggingAndFiltering.h"
#include "Recipes/Ch4_PhysicalProperties.h"
#include "Recipes/Ch4_Impulses.h"
#include "Recipes/Ch4_Forces.h"
#include "Recipes/Ch4_AsyncBodyDestruction.h"
#include "Recipes/Ch4_Joints.h"
#include "Recipes/Ch4_Vehicles.h"
#include "Recipes/Ch4_CharacterMovement.h"
#include "Recipes/Ch4_Bullets.h"
#include "Recipes/Ch4_Rope.h"
#include "Recipes/Ch4_TopDownIsometric.h"
#include "Recipes/Ch5_SwitchingScenes.h"
#include "Recipes/Ch5_SceneTransitions.h"
#include "Recipes/Ch5_UsingCCLayerMultiplex.h"
#include "Recipes/Ch5_UsingCCLabel.h"
#include "Recipes/Ch5_UsingCCMenu.h"
#include "Recipes/Ch5_ShadowedLabels.h"
#include "Recipes/Ch5_UIKitAlerts.h"
#include "Recipes/Ch5_WrappingUIKit.h"
#include "Recipes/Ch5_MenuWindows.h"
#include "Recipes/Ch5_HorizScrollMenu.h"
#include "Recipes/Ch5_VertSlidingMenuGrid.h"
#include "Recipes/Ch5_LoadingScreen.h"
#include "Recipes/Ch5_Minimap.h"
#include "Recipes/Ch6_SoundsAndMusic.h"
#include "Recipes/Ch6_AudioProperties.h"
#include "Recipes/Ch6_FadingSoundsAndMusic.h"
#include "Recipes/Ch6_AudioInGame.h"
#include "Recipes/Ch6_PositionalAudio.h"
#include "Recipes/Ch6_MeteringMusic.h"
#include "Recipes/Ch6_MeteringDialogue.h"
#include "Recipes/Ch6_StreamingAudio.h"
#include "Recipes/Ch6_RecordingAudio.h"
#include "Recipes/Ch6_iPodLibrary.h"
#include "Recipes/Ch6_MIDISynthesization.h"
#include "Recipes/Ch7_Waypoints.h"
#include "Recipes/Ch7_ProjectileAiming.h"
#include "Recipes/Ch7_LineOfSight.h"
#include "Recipes/Ch7_AIFlocking.h"
#include "Recipes/Ch7_GridPathfinding.h"
#include "Recipes/Ch7_Box2DPathfinding.h"
#include "Recipes/Ch7_TileMapPathfinding.h"
#include "Recipes/Ch7_SideScrollingPathfinding.h"
#include "Recipes/Ch7_LuaScripting.h"
#include "Recipes/Ch7_DynamicScriptLoading.h"
#include "Recipes/Ch7_LuaDecisionTree.h"
#include "Recipes/Ch8_Cocos2dTestbed.h"
#include "Recipes/Ch8_Tiled.h"
#include "Recipes/Ch8_JSONWorldBuilder.h"
#include "Recipes/Ch8_CocosBuilder.h"
#include "Recipes/Ch8_Cocos2d_X.h"
#include "Recipes/Ch8_Cocos3d.h"

const KDchar*  l_aRecipeNames [ ] =
{
	"Ch1_DrawingSprites"			,
	"Ch1_ColoringSprites"			,
	"Ch1_AnimatingSprites"			,
	"Ch1_OpenGLPrimitives"			,
	"Ch1_PlayingVideoFiles"			,
	"Ch1_GridEffects"				,
	"Ch1_ParticleEffects"			,
	"Ch1_MotionStreak"				,
	"Ch1_RetinaDisplay"				,
	"Ch1_1DEasingActions"			,
	"Ch1_2DEasingActions"			,
	"Ch1_3DCubes"					,
	"Ch1_RenderTexturedPolygon"		,
	"Ch1_AnimateTexturedPolygon"	,
	"Ch1_PaletteSwapping"			,
	"Ch1_MutablePaletteSwapping"	,
	"Ch1_UsingAWTextureFilter"		,
	"Ch1_RetroPixelatedFilter"		,
	"Ch1_TakingScreenshots"			,
	"Ch1_UsingCCParallaxNode"		,
	"Ch1_PixelFormat"				,
	"Ch1_ColorMaskLighting"			,
	"Ch2_TapHoldDragInput"			,
	"Ch2_DepthTestingInput"			,
	"Ch2_Buttons"					,
	"Ch2_DPad"						,
	"Ch2_AnalogStick"				,
	"Ch2_AccelerometerSteering"		,
	"Ch2_AccelerometerRotation"		,
	"Ch2_PinchZooming"				,
	"Ch2_Gestures"					,
	"Ch3_ArchivingObjects"			,
	"Ch3_ReadingPlistFiles"			,
	"Ch3_ReadingJsonFiles"			,
	"Ch3_ReadingXmlFiles"			,
	"Ch3_SavingSimpleData"			,
	"Ch3_MutatingNestedMetadata"	,
	"Ch3_SavingDataPlist"			,
	"Ch3_SavingDataSQLite"			,
	"Ch3_SavingDataCoreData"		,
	"Ch4_BasicSetup"				,
	"Ch4_CollisionResponse"			,
	"Ch4_DifferentShapes"			,
	"Ch4_DraggingAndFiltering"		,
	"Ch4_PhysicalProperties"		,
	"Ch4_Impulses"					,
	"Ch4_Forces"					,
	"Ch4_AsyncBodyDestruction"		,
	"Ch4_Joints"					,
	"Ch4_Vehicles"					,
	"Ch4_CharacterMovement"			,
	"Ch4_Bullets"					,
	"Ch4_Rope"						,
	"Ch4_TopDownIsometric"			,
    "Ch5_SwitchingScenes"			,
    "Ch5_SceneTransitions"			,
	"Ch5_UsingCCLayerMultiplex"		,
	"Ch5_UsingCCLabel"				,
    "Ch5_UsingCCMenu"				,
    "Ch5_ShadowedLabels"			,
	"Ch5_UIKitAlerts"				,
    "Ch5_WrappingUIKit"				,
    "Ch5_MenuWindows"				,
	"Ch5_HorizScrollMenu"			,
	"Ch5_VertSlidingMenuGrid"		,
    "Ch5_LoadingScreen"				,
    "Ch5_Minimap"					,
	"Ch6_SoundsAndMusic"			,
	"Ch6_AudioProperties"			,
	"Ch6_FadingSoundsAndMusic"		,
	"Ch6_AudioInGame"				,
	"Ch6_PositionalAudio"			,
	"Ch6_MeteringMusic"				,
	"Ch6_MeteringDialogue"			,
	"Ch6_StreamingAudio"			,
	"Ch6_RecordingAudio"			,
	"Ch6_iPodLibrary"				,
	"Ch6_MIDISynthesization"		,
	"Ch7_Waypoints"					,
	"Ch7_ProjectileAiming"			,
	"Ch7_LineOfSight"				,
	"Ch7_AIFlocking"				,
	"Ch7_GridPathfinding"			,
	"Ch7_Box2DPathfinding"			,
	"Ch7_TileMapPathfinding"		,
	"Ch7_SideScrollingPathfinding"	,
	"Ch7_LuaScripting"				,
	"Ch7_DynamicScriptLoading"		,
	"Ch7_LuaDecisionTree"			,
	"Ch8_Cocos2dTestbed"			,
	"Ch8_Tiled"						,
	"Ch8_JSONWorldBuilder"			,
	"Ch8_CocosBuilder"				,
	"Ch8_Cocos2d_X"					,
	"Ch8_Cocos3d"					,
};

#endif // __Includes_h__
