/* --------------------------------------------------------------------------
 *
 *      File            cross_window.c
 *      Description     Window fucntions.
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

#include "../kd_library.h"
#include "platform.h"

static KDint        l_wnd_x  = 0;
static KDint        l_wnd_y  = 0;
static KDuint       l_wnd_cx = 0;
static KDuint       l_wnd_cy = 0;

// xmWinSetProperty : Preset window's position and size.
KD_API KDvoid KD_APIENTRY xmWinSetProperty ( KDint x, KDint y, KDuint cx, KDuint cy )
{
	l_wnd_x  = x;
	l_wnd_y  = y;
	l_wnd_cx = cx;
	l_wnd_cy = cy;
}

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode )
{

}

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid )
{

}

KDvoid xmPlatformEventAttach ( KDenum mode )
{
  
}

KDvoid xmPlatformEventDetach ( KDenum mode )
{
    
}

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume )
{
	return -1;
}

KDfloat xmPlatformSoundGetVolume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint pos )
{
	return -1;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	return -1;
}