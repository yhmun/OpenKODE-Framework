/* --------------------------------------------------------------------------
 *
 *      File            xm_tts.c
 *      Description     Simple tts function.
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#include "kd_library.h"

static KDint    l_locale = 0;
static KDfloat  l_pitch  = 1.0f;
static KDfloat  l_rate   = 1.0f;

KD_API KDint KD_APIENTRY xmTTSCreate ( KDvoid )
{
#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSCreate ( );
#endif

	return -1;
}

KD_API KDint KD_APIENTRY xmTTSFree ( KDvoid )
{
#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSFree ( );
#endif

	return -1;
}

KD_API KDint KD_APIENTRY xmTTSSpeak ( const KDchar* text, KDint mode )
{
#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSSpeak ( text, mode );
#endif

	return -1;
}

KD_API KDint KD_APIENTRY xmTTSStop  ( KDvoid )
{
#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSStop ( );
#endif

	return -1;
}

KD_API KDint KD_APIENTRY xmTTSSetLocale ( KDint locale )
{
	l_locale = locale;

#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSSetLocale ( locale );
#endif

	return -1;
}

KD_API KDint KD_APIENTRY xmTTSGetLocale ( KDvoid )
{
	return l_locale;
}

KD_API KDint KD_APIENTRY xmTTSSetPitch ( KDfloat pitch )
{
	l_pitch = pitch;

#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSSetPitch ( pitch );
#endif

	return -1;
}

KD_API KDfloat KD_APIENTRY xmTTSGetPitch ( KDvoid )
{	
	return l_pitch;
}

KD_API KDint KD_APIENTRY xmTTSSetRate  ( KDfloat rate )
{
	l_rate = rate;

#ifdef XM_HAS_PLATFORM_TTS
	return xmPlatformTTSSetRate ( rate );
#endif

	return -1;
}

KD_API KDfloat KD_APIENTRY xmTTSGetRate  ( KDvoid )
{
	return l_rate;
}
