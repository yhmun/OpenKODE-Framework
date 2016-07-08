/* --------------------------------------------------------------------------
 *
 *      File            kd_event.c
 *      Description     Events
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

typedef struct XMCallback    XMCallback;

struct XMCallback
{
	KDCallbackFunc*	     func;
	KDint                eventtype;
	KDvoid*              eventuserptr;
};

static KDint _xmFindCallbackSubset ( KDvoid* d, KDvoid* f )
{
	KDint		    ret  = -1;
	XMCallback*	    data = (XMCallback *) d;
	XMCallback*	    find = (XMCallback *) f;

	if ( find->eventtype == 0 || find->eventtype == data->eventtype )
	{
		if ( find->eventuserptr == 0 || find->eventuserptr == data->eventuserptr )
		{
			ret = 0;
		}
	}

	if ( ret && ( find->eventuserptr == 0 || find->eventuserptr == data->eventuserptr ) )
	{
		if ( find->eventtype == 0 || find->eventtype == data->eventtype )
		{
			ret = 0;
		}
	}
	
	return ret;
} 

static KDint _xmFindCallback ( KDvoid* d, KDvoid* f )
{
	KDint		    ret   = -1;
	XMCallback*	    data  = (XMCallback *) d;
	KDEvent*	    find  = (KDEvent  *) f;

	if ( data->eventtype == 0 || data->eventtype == find->type )
	{
		if ( data->eventuserptr == 0 || data->eventuserptr == find->userptr )
		{
			ret = 0;
		}
	}
	
	if ( ret && ( data->eventuserptr == 0 || data->eventuserptr == find->userptr ) )
	{
		if ( data->eventtype == 0 || data->eventtype == find->type )
		{
			ret = 0;
		}
	}

	return ret;
}

// kdWaitEvent : Get next event from thread's event queue.
KD_API const KDEvent* KD_APIENTRY kdWaitEvent ( KDust timeout )
{
	KDEvent*        event	= KD_NULL;
	KDust           time	= kdGetTimeUST ( );	
	XMContext*      context = 0;

	do
	{	
		if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
		{					
			kdPumpEvents ( );

			event = (KDEvent *) xmQueuePopFront ( context->events );
			
			if ( event )
			{
				return event;
			}
		}
	} while ( ( timeout == ( (KDust) -1 ) ) || ( ( kdGetTimeUST ( ) - time ) < timeout ) );

	if ( timeout != 0 )
	{
		kdSetError ( KD_EAGAIN );
	}

	return KD_NULL;
}

// kdSetEventUserptr : Set the userptr for global events.
KD_API KDvoid KD_APIENTRY kdSetEventUserptr ( KDvoid* userptr )
{
	XMContext*      context = 0;	

	if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
	{
		context->userptr = userptr;
	}
}

// kdDefaultEvent : Perform default processing on an unrecognized event. 
KD_API KDvoid KD_APIENTRY kdDefaultEvent ( const KDEvent* event )
{		
	XMContext*      context  = 0;
	XMCallback*	    callback = 0;
	KDint32         type     = 0;
	
	if ( event )
	{	
		type = event->type;
		
		if ( !( ( context = xmFindContext ( kdThreadSelf ( ) ) ) &&
			    ( callback = (XMCallback *) xmQueueReverseFind ( context->callbacks, _xmFindCallback, (KDvoid *) event ) ) &&
			    ( callback->func ) ) )
		{	
			kdFreeEvent ( (KDEvent *) event );			
		}
		
		if ( type == KD_EVENT_QUIT )
		{			
			kdExit ( 0 );
		}		
	}	
}

// kdPumpEvents : Pump the thread's event queue, performing callbacks.
KD_API KDint KD_APIENTRY kdPumpEvents ( KDvoid )
{		
	static KDint    pumping	 = 0;
	KDEvent*        event    = 0;		
	XMContext*      context  = 0;
	XMCallback*     callback = 0;

	if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
	{	
		xmFireTimer ( context );

		if ( pumping )
		{
			event = (KDEvent *) xmQueueFindNext ( context->events );
		}
		else
		{
			pumping = 1;
			event = (KDEvent *) xmQueueFind ( context->events, 0, 0 );
		}

		while ( event )
		{
			if ( ( callback = (XMCallback *) xmQueueReverseFind ( context->callbacks, _xmFindCallback, event ) ) && ( callback->func ) )
			{													
				xmQueueEraseFound ( context->events );

				callback->func ( event );
				kdFreeEvent ( event );			
			}
			
			event = (KDEvent *) xmQueueFindNext ( context->events );
		}
	}

	pumping = 0;

	return 0;
}

// kdInstallCallback : Install or remove a callback function for event processing. 
KD_API KDint KD_APIENTRY kdInstallCallback ( KDCallbackFunc* func, KDint eventtype, KDvoid* eventuserptr )
{	
	XMContext*      context  = 0;
	XMCallback*	    callback = 0;
	XMCallback*	    subset	 = 0;
	KDint           ret	     = -1;

	if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
	{
		if( func == 0 && eventtype == 0 && eventuserptr == 0 )
		{
			while( ( callback = (XMCallback *) xmQueuePopRear ( context->callbacks ) ) )
			{
				kdFree ( callback );				
			}

			ret = 0;
		}
		else
		{
			if ( ( callback = (XMCallback *) kdMalloc ( sizeof ( XMCallback ) ) ) )
			{
				callback->func = func;
				callback->eventtype = eventtype;
				callback->eventuserptr = eventuserptr;
								
				subset = (XMCallback *) xmQueueReverseFind ( context->callbacks, _xmFindCallbackSubset, callback );
				while ( subset )
				{
					kdFree ( subset );		
					xmQueueEraseFound ( context->callbacks );		
					subset = (XMCallback *) xmQueueFindNext ( context->callbacks );
				}				
				
				if( xmQueuePushRear ( context->callbacks, callback ) )
				{
					kdFree ( callback );										
				}
				else
				{
					ret = 0;
				}				
			}
			else
			{
				kdSetError ( KD_ENOMEM );	
			}
		}
	}
	
	return ret;
}

// kdCreateEvent : Create an event for posting.
KD_API KDEvent* KD_APIENTRY kdCreateEvent ( KDvoid )
{
	KDEvent*        event = KD_NULL;
	
	if ( ( event = (KDEvent *) kdMalloc ( sizeof ( KDEvent ) ) ) )
	{
		kdMemset ( event, 0, sizeof ( KDEvent ) );
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}
		
	return event;
}

// kdPostEvent, kdPostThreadEvent : Post an event into a queue.
KD_API KDint KD_APIENTRY kdPostEvent ( KDEvent* event )
{				
	return kdPostThreadEvent ( event, kdThreadSelf ( ) );
}

KD_API KDint KD_APIENTRY kdPostThreadEvent ( KDEvent* event, KDThread* thread )
{
	XMContext*      context = 0;
	KDint           ret	    = -1;

	if ( ( context = xmFindContext ( thread ) ) )
	{
		if ( !xmQueuePushRear ( context->events, event ) )
		{
			ret = 0;

			if ( event->timestamp == 0 )
			{
				event->timestamp = kdGetTimeUST ( );
			}

			if ( event->userptr == 0 )
			{
				event->userptr = context->userptr;
			}
		}
	}
		
	return ret;
}

// kdFreeEvent : Abandon an event instead of posting it. 
KD_API KDvoid KD_APIENTRY kdFreeEvent ( KDEvent* event )
{
	if ( event->type == KD_EVENT_NAME_LOOKUP_COMPLETE )
	{
		if ( event->data.namelookup.result )
		{
			kdFree ( (KDvoid*) event->data.namelookup.result );
		}
	}

	kdFree ( event );
}


