/* --------------------------------------------------------------------------
 *
 *      File            ex_sound.cpp
 *      Description     Simple sound
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright ( C ) 1995-2010 Jean-loup Gailly and Mark Adler
 * 
 *      Jean-loup Gailly        Mark Adler
 *      jloup@gzip.org          madler@alumni.caltech.edu	
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

#include "main.h"

static XMSound*   l_sound  = KD_NULL;
static XMSound*   l_sound2 = KD_NULL;
static XMSound*   l_sound3 = KD_NULL;
static XMSound*   l_sound4 = KD_NULL;
static KDbool     l_pause  = KD_FALSE;
static KDbool     l_play   = KD_TRUE;
static KDint      l_case   = 2;

KDvoid xmExample_Sound ( KDvoid )
{	
	kdLogMessage ( "Example Sound\n\n" );

	xmExample_Sound_Free ( );

	l_sound  = xmSoundOpen ( "/res/Sound/background.mp3", XM_SOUND_PLAY | XM_SOUND_REPEAT );
	l_sound2 = xmSoundOpen ( "/res/Sound/background.wav", XM_SOUND_PLAY );
	
	// midi not supported in iPhone
	l_sound3 = xmSoundOpen ( "/res/Sound/music.mid", XM_SOUND_PLAY );	

	l_sound4 = xmSoundOpen ( "/res/Sound/effect1.wav", XM_SOUND_EFFECT );
}

KDvoid xmExample_Sound_Free ( KDvoid )
{
	if ( l_sound )
	{
		xmSoundClose ( l_sound );
		l_sound = KD_NULL;
	}

	if ( l_sound2 )
	{
		xmSoundClose ( l_sound2 );
		l_sound2 = KD_NULL;
	}

	if ( l_sound3 )
	{
		xmSoundClose ( l_sound3 );
		l_sound3 = KD_NULL;
	}
	
	if ( l_sound4 )
	{
		xmSoundClose ( l_sound4 );
		l_sound2 = KD_NULL;
	}
}

KDvoid xmExample_Sound_Event ( KDvoid )
{
    xmVibrate ( 100 );
    
	if ( l_sound4 )
	{
		xmSoundPlay ( l_sound4 );
	}

	if ( l_sound )
	{
		switch ( l_case )
		{
			case 0 :
				if ( l_pause == KD_FALSE )
				{
					xmSoundPause ( l_sound );
					l_pause = KD_TRUE;
				}
				else
				{
					xmSoundResume ( l_sound );
					l_pause = KD_FALSE;
				}
				break;

			case 1 :
				if ( l_play == KD_FALSE )
				{
					xmSoundPlay ( l_sound );
					l_play = KD_TRUE;
				}
				else
				{
					xmSoundStop ( l_sound );
					l_play = KD_FALSE;
				}
				break;

			case 2 :

				xmSoundRewind ( l_sound );

				break;
		}
	}
}
