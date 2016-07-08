/* --------------------------------------------------------------------------
 *
 *      File            kd_timer.c
 *      Description     Timer functions
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

struct KDTimer
{
	KDint64     interval;
	KDint       periodic;
	KDvoid*     eventuserptr;
	KDust       fire;
};

static KDint _xmFindTimer ( KDvoid* d, KDvoid* f )
{ 	
	KDTimer*    data = (KDTimer *) d;
	KDTimer*    find = (KDTimer *) f;

	return data == find ? 0 : -1;
}

static KDint _xmFindFireTimer ( KDvoid* d, KDvoid* f )
{	
	KDEvent*    data = (KDEvent *) d;
	KDvoid*     find = (KDvoid *) f;

	return data->type == KD_EVENT_TIMER && data->userptr == find ? 0 : -1;
}

// xmFireTimer : Fire timer.
KDvoid xmFireTimer ( XMContext* context )
{
	KDEvent*    event = 0;
	KDTimer*    timer = 0;	
	
	timer = (KDTimer *) xmQueueFind ( context->timers, 0, 0 );

	while ( ( timer ) )
	{
		if( ( timer->fire != 0 ) && ( timer->fire < kdGetTimeUST ( ) ) )
		{	
			if ( ( event = kdCreateEvent ( ) ) )
			{
				event->type	= KD_EVENT_TIMER;
				event->userptr = timer->eventuserptr;				
				
				kdPostThreadEvent( event, context->thread );
			}

			switch ( timer->periodic )
			{					
				case KD_TIMER_ONESHOT			:	timer->fire = 0;								break;
				case KD_TIMER_PERIODIC_AVERAGE	:	timer->fire = kdGetTimeUST() + timer->interval;	break;
				case KD_TIMER_PERIODIC_MINIMUM	:	timer->fire = kdGetTimeUST() + timer->interval;	break;
			}
		}						

		timer = (KDTimer *) xmQueueFindNext ( context->timers );
	}		
}

// kdSetTimer : Set timer.
KD_API KDTimer* KD_APIENTRY kdSetTimer ( KDint64 interval, KDint periodic, KDvoid* eventuserptr )
{
	KDTimer*        timer	= KD_NULL;
	XMContext*      context = KD_NULL;

	if ( ( timer = (KDTimer *) kdMalloc ( sizeof ( KDTimer ) ) ) )
	{
		if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
		{
			timer->interval	    = interval;
			timer->periodic	    = periodic;
			timer->eventuserptr = eventuserptr;
			timer->fire	        = kdGetTimeUST ( ) + timer->interval;

			if( xmQueuePushRear ( context->timers, timer ) )
			{
				kdFree ( timer );	
				timer = KD_NULL;
			}
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}

	return timer;
}

// kdCancelTimer : Cancel and free a timer.
KD_API KDint KD_APIENTRY kdCancelTimer ( KDTimer* timer )
{
	XMContext*      context = KD_NULL;
	KDEvent*        fire    = 0;
	KDint           ret	    = -1;
	
	if ( ( timer ) )
	{
		if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
		{
			if ( xmQueueFind ( context->timers, _xmFindTimer, timer ) )
			{
				fire = (KDEvent *) xmQueueFind ( context->events, _xmFindFireTimer, timer->eventuserptr );

				while ( fire )
				{
					xmQueueEraseFound ( context->events );
					kdFreeEvent ( fire );

					fire = (KDEvent *) xmQueueFindNext ( context->events );
				}				

				xmQueueEraseFound ( context->timers );
				kdFree ( timer );

				ret = 0;
			}
		}
	}
	else
	{
		kdSetError ( KD_EINVAL );
	}

	return 0;
}

// kdSleep : The function sleep gives a simple way to make the program wait for a short interval.
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSleep ( KDust ust )
{
#ifdef XM_HAS_PLATFORM_SLEEP

	xmPlatformSleep ( ust );

#else

	struct timespec clock;

	kdMemset ( &clock, 0, sizeof ( struct timespec ) );

	clock.tv_sec  = (long) ( ust / 1000000000UL );
	clock.tv_nsec = (long) ( ust - ( clock.tv_sec * 1000000000UL ) );

	nanosleep ( &clock, KD_NULL );

#endif
}
