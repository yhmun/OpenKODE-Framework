/* --------------------------------------------------------------------------
 *
 *      File            APSSharedType.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSSharedType_h__
#define __APSSharedType_h__

#include "APSMacroCPP.h"
#include "APSCommon.h"
#include "../../ccTypes.h"

NS_APS_BEGIN

typedef KDuint  APSIndex;

enum
{
	// The device is in landscape mode, with the device held upright and the home button on the right side.
	kAPSOrientationLandscapeRight		,
	// The device is in landscape mode, with the device held upright and the home button on the left side.
	kAPSOrientationLandscapeLeft		,
	// The device is in portrait mode, with the device held upright and the home button at the bottom. If you do not specify any orientations, this orientation is assumed by default.
	kAPSOrientationPortrait				,
	// The device is in portrait mode but upside down, with the device held upright and the home button at the top.
	kAPSOrientationPortraitUpsideDown	,	
};

typedef KDuint  APSOrientation;

enum _APSEventType 
{
	kAPSEventTypeTouchBegan		,		// The type of event source that occurs when touch begins.
	kAPSEventTypeTouchEnded		,		// The type of event source that occurs when touch ends.
	kAPSEventTypeTouchMoved		,		// The type of event source that occurs when touch moves.
//	kAPSEventTypeTouchStationary,
	kAPSEventTypeShakeBegan		,		// The type of event source that occurs when shake begins.
	kAPSEventTypeShakeEnded		,		// The type of event source that occurs after shake ends.
	kAPSEventTypeGravityChange	,		// The type of event source that occurs when the gravity changes.
};
typedef KDuint  APSEventType;

enum _APSTouchOption
{
    kAPSTouchOptionRetain				= 1		,
    kAPSTouchOptionAcceptMoveEnter		= 1 << 1,	// If set, graphic accepts touch events that has begun external position.
    kAPSTouchOptionAcceptsOnlyVisible	= 1 << 2,	// If set, graphic accepts touch events only when it is visible in the screen.
};
typedef KDuint  APSTouchOption;

enum _ResourceType
{
	kAPSResourceTypeNone		,
	kAPSResourceTypeGraphic		,
	kAPSResourceTypeAction		,
	kAPSResourceTypeMedia		,
	kAPSResourceTypePage		,
};

enum 
{
    kAPSLinkerTypeTrigger		,				// Type of linker that triggers an action.
    kAPSLinkerTypeInterrupt		,				// Type of linker that interrupts an action.
    kAPSLinkerTypeInterruptAll	,				// Type of linker that interrupts all actions.
    kAPSLinkerTypeEnable		,				// Type of linker that enables an action.
    kAPSLinkerTypeDisable		,				// Type of linker that disables an action.
    kAPSLinkerTypeNotifyEvent					// Type of linker that notifies an event. !!! Never use this type to link actions.
};
typedef KDuint  APSLinkerType;

enum _actionType 
{
	kAPSActionTypeNone			,				// Does nothing, an empty action
    kAPSActionTypeGroup			,				// Contains a series of actions.
	kAPSActionTypeMove			,                     
	kAPSActionTypeScale			,
	kAPSActionTypeRotation		,                 
	kAPSActionTypeFade			,
	kAPSActionTypeAnimate		,
	kAPSActionTypeVisibility	,
	kAPSActionTypeLinker		,               
	kAPSActionTypeAudio			,
	kAPSActionTypeSkew			,
};
typedef KDuint  APSActionType;

#define APS_GRAPHIC_ACTION_TYPES_COUNT    5

enum _APSTimingMode
{
	kAPSTimingModeLinear				,
	kAPSTimingModeEaseIn				,
	kAPSTimingModeEaseOut				,
	kAPSTimingModeEaseInEaseOut			,
	kAPSTimingModeEaseExponentialIn		,
	kAPSTimingModeEaseExponentialOut	,
	kAPSTimingModeEaseExponentialInOut	,
	kAPSTimingModeEaseSineIn			,
	kAPSTimingModeEaseSineOut			,
	kAPSTimingModeEaseSineInOut			,
	kAPSTimingModeEaseElasticIn			,
	kAPSTimingModeEaseElasticOut		,
	kAPSTimingModeEaseElasticInOut		,
	kAPSTimingModeEaseBounceIn			,
	kAPSTimingModeEaseBounceOut			,
	kAPSTimingModeEaseBounceInOut		,
	kAPSTimingModeEaseBackIn			,
	kAPSTimingModeEaseBackOut			,
	kAPSTimingModeEaseBackInOut			,
};

enum APSPredefinedResourceIndex
{
    kAPSSymbolIndex						,
    kAPSResetSymbolActionIndex			,
    kAPSTriggerProloguesActionIndex		,
    kAPSInterruptAllActionIndex			,
};
#define APS_PREDEFINED_RESOURCE_INDEXES_LENGTH  4

//
// APS_APP_FILE_VERSION 
// 1.0 -  March 20, 2012
//        ArtPigEditor v1.01
// 1.01 - July 12, 2012 
//        ArtPigEditor v1.5
//
#define APS_APP_FILE_VERSION					1.01

#define APS_LIB_VERSION							1.5

#define APS_MAX_OPACITY							255
#define APS_MIN_OPACITY							0

#define APS_NIL_INDEX							KDUINT32_MAX
#define APS_MAX_PRIORITY						KDINT32_MAX
#define APS_MIN_PRIORITY						KDINT32_MIN

#define APS_MIN_TIMING_FACTOR					2
#define APS_MAX_TIMING_FACTOR					6

inline KDfloat  easeElasticPeriod ( KDfloat fFactor ) 
{
    return 0.3f + ( fFactor - APS_MIN_TIMING_FACTOR ) * 0.0375f;
}

/** x is almost close to zero value or negligible. */
inline KDint  isZeroF ( KDfloat x ) 
{
    return ( x < KD_FLT_EPSILON && -x < KD_FLT_EPSILON );
}

inline KDint  isZeroD ( KDdouble x ) 
{
    return ( x < KD_DBL_EPSILON_KHR && -x < KD_DBL_EPSILON_KHR );
}

inline CCPoint  accpRotate ( const CCPoint& tPoint, KDfloat fTheta )
{
    //
    // x' = Cos ( Theta ) * x - Sin ( Theta ) * y
    // y' = Sin ( Theta ) * x + Cos ( Theta ) * y
    //
    
    return CCPoint ( tPoint.x * kdCosf ( fTheta ) - tPoint.y * kdSinf ( fTheta ), tPoint.x * kdSinf ( fTheta ) + tPoint.y * kdCosf ( fTheta ) );
}

inline CCPoint  accpAdd ( const CCPoint& tP1, const CCPoint& tP2 )
{
    return CCPoint ( tP1.x + tP2.x, tP1.y + tP2.y );
}

inline CCPoint accpSub ( const CCPoint& tP1, const CCPoint& tP2 )
{
    return CCPoint ( tP1.x - tP2.x, tP1.y - tP2.y );
}

inline CCPoint accpCompMult ( const CCPoint& tP1, const CCPoint& tP2 )
{
    return CCPoint ( tP1.x * tP2.x, tP1.y * tP2.y );
}

inline CCPoint accpMult ( const CCPoint& tP, KDfloat fScale ) 
{
    return CCPoint ( tP.x * fScale, tP.y * fScale );
}

inline CCSize accsMult ( const CCSize& tSize, KDfloat fScale )
{
    return CCSize ( tSize.cx * fScale, tSize.cy * fScale );
}

NS_APS_END

#endif	// __APSSharedType_h__





