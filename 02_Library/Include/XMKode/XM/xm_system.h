/* --------------------------------------------------------------------------
 *
 *      File            xm_system.h
 *      Description     System IO function.
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __xm_system_h__
#define __xm_system_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

// xmGetKeyName : Get key input name.
KD_API const KDchar* KD_APIENTRY xmGetKeyName ( KDint index );


// xmEventAttach, xmEventDetach : Attach System Event Callback. ( ACCELEROMETER, MAGNETOMETER )
KD_API KDvoid xmEventAttach ( KDenum mode );
KD_API KDvoid xmEventDetach ( KDenum mode );


// xmSetIMEKeyboardState 
KD_API KDvoid KD_APIENTRY xmSetIMEKeyboardState ( KDbool show );


// xmVibrate : Vibrate Device. ( Android and iOS Supported, iOS can not control time. )
KD_API KDvoid KD_APIENTRY xmVibrate ( KDuint msec );


// xmSetBrightness : Set Device's Brightness.
KD_API KDvoid KD_APIENTRY xmSetBrightness ( KDfloat value );


// xmSetFullScreen : Hide Status Bar.
KD_API KDvoid KD_APIENTRY xmSetFullScreen ( KDbool enable );

// xmOpenURL : Open Browser with url page. ( Currently iOS Supported )
KD_API KDvoid KD_APIENTRY xmOpenURL ( const KDchar* url );

// For Cocos2D Engine
KD_API KDvoid KD_APIENTRY xmSetContentScaleFactor ( KDfloat scaleX, KDfloat scaleY );
    
KD_API KDvoid KD_APIENTRY xmSetViewportRect ( KDfloat x, KDfloat y, KDfloat w, KDfloat h );
    
    
#define kdGetKeyName				xmGetKeyName
#define kdEventAttach				xmEventAttach
#define kdEventDetach				xmEventDetach
#define kdSetIMEKeyboardState		xmSetIMEKeyboardState
#define kdVibrate                   xmVibrate
#define kdSetBrightness				xmSetBrightness
#define kdSetFullScreen				xmSetFullScreen

#ifdef __cplusplus
}
#endif

#endif	// __xm_system_h__



