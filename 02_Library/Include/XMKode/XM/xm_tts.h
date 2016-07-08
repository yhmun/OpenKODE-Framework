/* --------------------------------------------------------------------------
 *
 *      File            xm_tts.h
 *      Description     Simple tts function.
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

#ifndef __xm_tts_h__
#define __xm_tts_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// XMTTS funtions support iOS and Android platforms.
// iOS needs "XMOpenEars" library and "Slt" voice data file (at iOS Root Resource)
//

#define XM_TTS_LOCALE_DEFAULT    0x0000
#define XM_TTS_LOCALE_US		 0x0001

#define XM_TTS_QUEUE_FLUSH       0x0000
#define XM_TTS_QUEUE_ADD	     0x0001

KD_API KDint    KD_APIENTRY xmTTSCreate ( KDvoid );
KD_API KDint    KD_APIENTRY xmTTSFree   ( KDvoid );

// iOS not support "mode" argument.
KD_API KDint    KD_APIENTRY xmTTSSpeak ( const KDchar* text, KDint mode );
KD_API KDint    KD_APIENTRY xmTTSStop  ( KDvoid );

// iOS not support "Locale" funtions.
KD_API KDint    KD_APIENTRY xmTTSSetLocale ( KDint locale );
KD_API KDint    KD_APIENTRY xmTTSGetLocale ( KDvoid );

KD_API KDint    KD_APIENTRY xmTTSSetPitch ( KDfloat pitch );
KD_API KDfloat  KD_APIENTRY xmTTSGetPitch ( KDvoid );

KD_API KDint    KD_APIENTRY xmTTSSetRate  ( KDfloat rate );
KD_API KDfloat  KD_APIENTRY xmTTSGetRate  ( KDvoid );

#define kdTTSCreate         xmTTSCreate
#define kdTTSFree           xmTTSFree

#define kdTTSSpeak          xmTTSSpeak
#define kdTTSStop           xmTTSStop

#define kdTTSSetLocale      xmTTSSetLocale
#define kdTTSGetLocale      xmTTSGetLocale

#define kdTTSSetPitch       xmTTSSetPitch
#define kdTTSGetPitch       xmTTSGetPitch

#define kdTTSSetRate        xmTTSSetRate
#define kdTTSGetRate        xmTTSGetRate

#ifdef __cplusplus
}
#endif

#endif	// __xm_tts_h__



