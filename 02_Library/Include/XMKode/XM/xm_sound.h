/* --------------------------------------------------------------------------
 *
 *      File            xm_sound.h
 *      Description     Simple sound function.
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

#ifndef __xm_sound_h__
#define __xm_sound_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XM_SOUND_NORMAL				0x0000		
#define XM_SOUND_EFFECT				0x0001		// For short sound
#define XM_SOUND_REPEAT				0x0010		
#define XM_SOUND_PLAY				0x0020		
#define XM_SOUND_AUTO_PAUSE_OFF		0x0040		// When Background/Foreground, not auto pause

typedef struct XMSound				XMSound;

KD_API XMSound* KD_APIENTRY xmSoundOpen      ( const KDchar* path, KDint mode );
KD_API KDint    KD_APIENTRY xmSoundClose     ( XMSound* sound ); 

KD_API KDint    KD_APIENTRY xmSoundPlay      ( XMSound* sound );
KD_API KDint    KD_APIENTRY xmSoundStop      ( XMSound* sound );

KD_API KDint    KD_APIENTRY xmSoundPause     ( XMSound* sound );
KD_API KDint    KD_APIENTRY xmSoundResume    ( XMSound* sound );
KD_API KDint    KD_APIENTRY xmSoundRewind    ( XMSound* sound );

KD_API KDint    KD_APIENTRY xmSoundSetRepeat ( XMSound* sound, KDint on ); 

KD_API KDint    KD_APIENTRY xmSoundSetVolume ( XMSound* sound, KDfloat volume );
KD_API KDfloat  KD_APIENTRY xmSoundGetVolume ( XMSound* sound ); 

KD_API KDint    KD_APIENTRY xmSoundSetPitch  ( XMSound* sound, KDfloat pitch );
KD_API KDfloat  KD_APIENTRY xmSoundGetPitch  ( XMSound* sound );

KD_API KDint    KD_APIENTRY xmSoundSetPan    ( XMSound* sound, KDfloat pan );
KD_API KDfloat  KD_APIENTRY xmSoundGetPan    ( XMSound* sound );
    
// range 0.0 ~ 1.0
KD_API KDint    KD_APIENTRY xmSoundSetVolumes ( KDint nMode, KDfloat volume );          // XM_SOUND_NORMAL or XM_SOUND_EFFECT
KD_API KDfloat  KD_APIENTRY xmSoundGetVolumes ( KDint nMode );                          // XM_SOUND_NORMAL or XM_SOUND_EFFECT
    
// range 0.5 ~ 2.0
KD_API KDint    KD_APIENTRY xmSoundSetPitches ( KDint nMode, KDfloat pitch );           // XM_SOUND_NORMAL or XM_SOUND_EFFECT
KD_API KDfloat  KD_APIENTRY xmSoundGetPitches ( KDint nMode );                          // XM_SOUND_NORMAL or XM_SOUND_EFFECT

// range -1.0 ~ 1.0
KD_API KDint    KD_APIENTRY xmSoundSetPans    ( KDint nMode, KDfloat pan );             // XM_SOUND_NORMAL or XM_SOUND_EFFECT
KD_API KDfloat  KD_APIENTRY xmSoundGetPans    ( KDint nMode );                          // XM_SOUND_NORMAL or XM_SOUND_EFFECT

/////////////////////////////////////////////////////////////////////////////////////
// Effect Mode : Not worked.

KD_API KDint    KD_APIENTRY xmSoundSetCallback ( XMSound* sound, KDCallbackFunc* func );

KD_API KDint    KD_APIENTRY xmSoundGetDuration ( XMSound* sound ); 
KD_API KDint    KD_APIENTRY xmSoundGetPosition ( XMSound* sound ); 
KD_API KDint	KD_APIENTRY xmSoundSetPosition ( XMSound* sound, KDint pos ); 
    
#define KD_SOUND_NORMAL				XM_SOUND_NORMAL
#define KD_SOUND_EFFECT				XM_SOUND_EFFECT
#define KD_SOUND_REPEAT				XM_SOUND_REPEAT
#define KD_SOUND_PLAY				XM_SOUND_PLAY
#define KD_SOUND_AUTO_PAUSE_OFF		XM_SOUND_AUTO_PAUSE_OFF

#define KDSound						XMSound

#define kdSoundOpen					xmSoundOpen
#define kdSoundClose				xmSoundClose

#define kdSoundPlay					xmSoundPlay
#define kdSoundStop					xmSoundStop

#define kdSoundPause				xmSoundPause
#define kdSoundResume				xmSoundResume
#define kdSoundRewind				xmSoundRewind

#define kdSoundSetRepeat			xmSoundSetRepeat

#define kdSoundSetVolume			xmSoundSetVolume
#define kdSoundGetVolume			xmSoundGetVolume

#define kdSoundSetCallback			xmSoundSetCallback

#define kdSoundGetDuration			xmSoundGetDuration
#define kdSoundGetPosition			xmSoundGetPosition
#define kdSoundSetPosition			xmSoundSetPosition   

#ifdef __cplusplus
}
#endif

#endif 



