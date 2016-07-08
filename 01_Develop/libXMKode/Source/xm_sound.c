/* --------------------------------------------------------------------------
 *
 *      File            xm_sound.c
 *      Description     Simple sound function.
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

static XMQueue*     l_sounds = KD_NULL;
static KDfloat      l_volumes [ 2 ] = { 1.0f, 1.0f };
static KDfloat      l_pitches [ 2 ] = { 1.0f, 1.0f };
static KDfloat      l_pans    [ 2 ] = { 1.0f, 1.0f };

static KDvoid _xmFreeSound ( KDvoid* _sound )
{
	XMSound*  sound = (XMSound*) _sound;
	
    xmSoundStop ( sound );
    xmPlatformSoundClose ( sound );
	
	if ( sound->fpath )
	{
		kdFree ( sound->fpath );
	}

	kdFree ( sound );
}

static KDint _xmFindSound ( KDvoid* _sound, KDvoid* _device )
{ 	
	XMSound*  sound = (XMSound *) _sound;	

	return sound->device == _device ? 0 : -1;
}

KDvoid xmCreateSounds ( KDvoid )
{
	l_sounds = xmQueueCreate ( );
	kdAssert ( l_sounds );
}

KDvoid xmFreeSounds ( KDvoid )
{
    if ( l_sounds )
    {
        xmQueueFree ( l_sounds, _xmFreeSound );
        l_sounds = KD_NULL;
    }
}

KDvoid xmSoundAutoPause ( KDbool state )
{
	XMSound*  sound = (XMSound *) xmQueueFind ( l_sounds, 0, 0 );

	while ( sound )
	{
		if ( !( sound->mode & XM_SOUND_AUTO_PAUSE_OFF ) && sound->play == KD_TRUE && sound->pause == KD_FALSE )
		{
			if ( state == 1 )
			{
				sound->pause = KD_TRUE;
				xmSoundResume ( sound );
			}
			else
			{
				xmSoundPause ( sound );
				sound->pause = KD_FALSE;
			}
		}

		sound = (XMSound *) xmQueueFindNext ( l_sounds );
	}
}

KDvoid xmSoundCompleted ( KDvoid* device )
{
	XMSound*  sound = (XMSound*) xmQueueFind ( l_sounds, _xmFindSound, device );

	if ( sound )
	{
		if ( sound->func )
		{
			KDEvent*  event = kdCreateEvent ( );

			event->type = KD_SOUND_COMPLETED;
			event->userptr = sound;

			sound->func ( event );

			kdFreeEvent ( event );
		}

#ifndef XM_HAS_PLATFORM_SOUND_REPEAT
		if ( sound->repeat == KD_TRUE )
		{
			xmSoundRewind ( sound );
		}
		else
		{
			xmSoundStop ( sound );
		}
#endif
	}
}

KD_API XMSound* KD_APIENTRY xmSoundOpen ( const KDchar* path, KDint mode )
{
	XMSound*  sound = KD_NULL;

	if ( ( sound = (XMSound*) kdCalloc ( 1, sizeof ( XMSound ) ) ) )
	{
		if ( ( sound->fpath = xmGetNativePath ( path, &sound->vpath ) ) )
		{
			sound->path   = path;
			sound->mode   = mode;
			sound->volume = 1.f;

			if ( !xmPlatformSoundOpen ( sound ) )
			{
				xmQueuePushRear ( l_sounds, sound );

				xmSoundSetVolume ( sound, sound->volume );

				if ( mode & XM_SOUND_PLAY )
				{
					xmSoundPlay ( sound );
				}

				if ( mode & XM_SOUND_REPEAT )
				{
					xmSoundSetRepeat ( sound, 1 ); 
				}

				return sound;
			}
		}

		_xmFreeSound ( (KDvoid*) sound );
	}

	return 0;
}

KD_API KDint KD_APIENTRY xmSoundClose ( XMSound* sound )
{
	KDint  nRet = 0;

	sound = (XMSound*) xmQueueFind ( l_sounds, _xmFindSound, sound->device );
	if ( sound )
	{
		xmQueueEraseFound ( l_sounds );
		_xmFreeSound ( sound );
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundPlay ( XMSound* sound )
{
	KDint  nRet = 0;

	if ( sound->play == KD_FALSE )
	{
		nRet = xmPlatformSoundPlay ( sound );
		if ( nRet == 0 )
		{
			sound->play  = KD_TRUE;
			sound->pause = KD_FALSE;
		}
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundStop ( XMSound* sound )
{
	KDint  nRet = 0;

	if ( sound->play == KD_TRUE )
	{
		nRet = xmPlatformSoundStop ( sound );
		if ( nRet == 0 )
		{
			sound->play = KD_FALSE;
		}		
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundPause ( XMSound* sound )
{
	KDint  nRet = 0;

	if ( sound->play == KD_TRUE && sound->pause == KD_FALSE )
	{
		nRet = xmPlatformSoundPause ( sound );
		if ( nRet == 0 )
		{
			sound->pause = KD_TRUE;
		}
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundResume ( XMSound* sound )
{
	KDint  nRet = 0;
	if ( sound->play == KD_TRUE && sound->pause == KD_TRUE )
	{
        nRet = xmPlatformSoundResume ( sound );
		if ( nRet == 0 )
		{
			sound->pause = KD_FALSE;
		}		
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundRewind ( XMSound* sound )
{
	KDint  nRet = 0;

	xmSoundStop ( sound );
	nRet = xmSoundPlay ( sound );

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundSetRepeat ( XMSound* sound, KDint on )
{
	sound->repeat = on ? KD_TRUE : KD_FALSE;

#ifdef XM_HAS_PLATFORM_SOUND_REPEAT
	return xmPlatformSoundSetRepeat ( sound, on );
#else
	return 0;
#endif
}

KD_API KDint KD_APIENTRY xmSoundSetVolume ( XMSound* sound, KDfloat volume )
{    
    if ( volume < 0 )
    {
        volume = 0;
    }
    
    if ( volume > 1 )
    {
        volume = 1;
    }
    
    sound->volume = volume;
    
	volume *= l_volumes [ ( XM_SOUND_EFFECT & sound->mode ) ? 1 : 0 ];
 
	return xmPlatformSoundSetVolume ( sound, volume );
}

KD_API KDfloat KD_APIENTRY xmSoundGetVolume ( XMSound* sound )
{
	return sound->volume;
	//return xmPlatformSoundGetVolume ( sound );
}

KD_API KDint KD_APIENTRY xmSoundSetPitch ( XMSound* sound, KDfloat pitch )
{
    if ( pitch < 0.5f )
    {
        pitch = 0.5f;
    }
    
    if ( pitch > 2.0f )
    {
        pitch = 2.0f;
    }
    
    sound->pitch = pitch;
    
	pitch *= l_pitches [ ( XM_SOUND_EFFECT & sound->mode ) ? 1 : 0 ];
 
	return xmPlatformSoundSetPitch ( sound, pitch );
}

KD_API KDfloat  KD_APIENTRY xmSoundGetPitch ( XMSound* sound )
{
	return sound->pitch;
}

KD_API KDint KD_APIENTRY xmSoundSetPan ( XMSound* sound, KDfloat pan )
{
    if ( pan < -1.f )
    {
        pan = -1.f;
    }
    
    if ( pan > 1.f )
    {
        pan = 1.f;
    }
    
    sound->pan = pan;
    
	pan *= l_pans [ ( XM_SOUND_EFFECT & sound->mode ) ? 1 : 0 ];
    
	return xmPlatformSoundSetPan ( sound, pan );
}

KD_API KDfloat  KD_APIENTRY xmSoundGetPan ( XMSound* sound )
{
    return sound->pan;
}

KD_API KDint KD_APIENTRY xmSoundSetVolumes ( KDint nMode, KDfloat volume )
{
    KDint       nRet  = 0;
    XMSound*    sound = (XMSound *) xmQueueFind ( l_sounds, 0, 0 );
    
    if ( volume < 0 )
    {
        volume = 0;
    }
    
    if ( volume > 1 )
    {
        volume = 1;
    }
    
	l_volumes [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ] = volume;
    
	while ( sound )
	{
        xmSoundSetVolume ( sound, sound->volume );    
        
		sound = (XMSound *) xmQueueFindNext ( l_sounds );
	}
    
    return nRet;
}

KD_API KDfloat KD_APIENTRY xmSoundGetVolumes ( KDint nMode )
{    
	return l_volumes [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ];
}

KD_API KDint KD_APIENTRY xmSoundSetPitches ( KDint nMode, KDfloat pitch )
{
    KDint       nRet  = 0;
    XMSound*    sound = (XMSound *) xmQueueFind ( l_sounds, 0, 0 );
    
    if ( pitch < 0.5f )
    {
        pitch = 0.5f;
    }
    
    if ( pitch > 2.0f )
    {
        pitch = 2.0f;
    }
    
	l_pitches [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ] = pitch;
    
	while ( sound )
	{
        xmSoundSetPitch ( sound, sound->pitch );    
        
		sound = (XMSound *) xmQueueFindNext ( l_sounds );
	}
    
    return nRet;
}

KD_API KDfloat KD_APIENTRY xmSoundGetPitches ( KDint nMode )
{    
	return l_pitches [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ];
}

KD_API KDint KD_APIENTRY xmSoundSetPans ( KDint nMode, KDfloat pan )
{
    KDint       nRet  = 0;
    XMSound*    sound = (XMSound *) xmQueueFind ( l_sounds, 0, 0 );
    
    if ( pan < -1.f )
    {
        pan = -1.f;
    }
    
    if ( pan > 1.f )
    {
        pan = 1.f;
    }
    
	l_pans [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ] = pan;
    
	while ( sound )
	{
        xmSoundSetPan ( sound, sound->pan );
        
		sound = (XMSound *) xmQueueFindNext ( l_sounds );
	}
    
    return nRet;
}

KD_API KDfloat KD_APIENTRY xmSoundGetPans ( KDint nMode )
{
    return l_pans [ ( XM_SOUND_EFFECT & nMode ) ? 1 : 0 ];
}

KD_API KDint KD_APIENTRY xmSoundSetCallback ( XMSound* sound, KDCallbackFunc* func )
{
	KDint  nRet = 0;

	if ( sound->mode & XM_SOUND_EFFECT )
	{
		nRet = -1;
	}
	else
	{
		sound->func = func;
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundGetDuration ( XMSound* sound )
{
	KDint  nRet = 0;

	if ( sound->mode & XM_SOUND_EFFECT )
	{
		nRet = -1;
	}
	else
	{
		nRet = xmPlatformSoundGetDuration ( sound );
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundGetPosition ( XMSound* sound )
{
	KDint  nRet = 0;

	if ( sound->mode & XM_SOUND_EFFECT )
	{
		nRet = -1;
	}
	else
	{
		nRet = xmPlatformSoundGetPosition ( sound );
	}

	return nRet;
}

KD_API KDint KD_APIENTRY xmSoundSetPosition ( XMSound* sound, KDint pos )
{
	KDint  nRet = 0;

	if ( sound->mode & XM_SOUND_EFFECT )
	{
		nRet = -1;
	}
	else
	{
		KDint  duration = xmSoundGetDuration ( sound );

		if ( pos < 0 )
		{
			pos = 0;
		}

		if ( pos > duration )
		{
			pos = duration;
		}

		nRet = xmPlatformSoundSetPosition ( sound, pos );
	}

	return nRet;
}

