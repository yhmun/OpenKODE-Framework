/* --------------------------------------------------------------------------
 *
 *      File            kd_library.c
 *      Description     Library-specific definitions and functions for XMKode
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

static XMQueue*     l_contexts = 0;	

KDint         g_argc   = 0;
KDchar**      g_argv   = 0;
KDint         g_crypto = 0;

static KDvoid xmCreateContexts ( KDvoid )
{
	XMContext*  context = 0;
	
	l_contexts = xmQueueCreate ( );
	if ( !l_contexts )
	{
		kdLogMessage ( "Contexts create failed." );		
		kdExit ( 3 );
	}

	context = xmCreateContext ( );
	context->thread->pthread = pthread_self ( );

	if ( xmQueuePushRear ( l_contexts, context ) != 0 )
	{
		kdLogMessage ( "Contexts create failed." );		
		kdExit ( 3 );
	}	
}

static KDvoid xmFreeContexts ( KDvoid )
{
	XMContext*    context = 0;

	if ( l_contexts )
	{
		while ( ( context = (XMContext *) xmQueuePopRear ( l_contexts ) ) )
		{
			xmFreeContext ( context );
		}
		
		l_contexts = 0;	
	}
}

// xmGetContexts : Get context queue.
XMQueue* xmGetContexts ( KDvoid )
{
	return l_contexts;
}

// xmCreateContext : Create context.
XMContext* xmCreateContext ( KDvoid )
{
	XMContext*  context = 0;

	if ( ( context = (XMContext *) kdMalloc ( sizeof ( XMContext ) ) ) )
	{
		context->key       = 0;
		context->userptr   = 0;
		context->thread    = (KDThread *) kdMalloc ( sizeof ( KDThread ) );
		context->events    = xmQueueCreate ( );
		context->timers    = xmQueueCreate ( );
		context->callbacks = xmQueueCreate ( );

		if ( !context->thread || !context->events || !context->timers || !context->callbacks ) // || !context->windows )
		{
			if ( context->thread	) kdFree ( context->thread );
			if ( context->events	) xmQueueFree ( context->events   , 0 );
			if ( context->timers	) xmQueueFree ( context->timers   , 0 );
			if ( context->callbacks ) xmQueueFree ( context->callbacks, 0 );

			kdFree ( context );
			goto fail;
		}
	}
	else
	{
		goto fail;
	}

	return context;

fail :

	kdLogMessage ( "Context create failed." );	
	kdExit ( 3 );

	return 0;
}

// xmFreeContext : Free context.
KDvoid xmFreeContext ( XMContext* context )
{
	xmQueueFree ( context->callbacks, 0 );
	xmQueueFree ( context->timers   , 0 );
	xmQueueFree ( context->events   , 0 );	

	if ( context->thread )
	{		
		kdFree ( context->thread );		
	}

	if ( context->key )
	{
		xmFreeTS ( context->key );
	}

	kdFree( context );
}

static KDint _xmFindContext ( KDvoid* d, KDvoid* f )
{		
	XMContext*  data = (XMContext *) d;
	KDThread*   find = (KDThread  *) f;

	return data->thread == find ? 0 : -1;
}

// xmFindContext : Find context with thread.
XMContext* xmFindContext ( KDThread* thread )
{
	XMContext*   context = 0;

	context = (XMContext *) xmQueueFind ( l_contexts, _xmFindContext, thread );

	return context;
}

// xmInitLibrary : Initialize XMKode library.
KD_API KDvoid KD_APIENTRY xmInitLibrary ( KDint argc, KDchar *argv[] )
{
	kdLogMessage     ( "\n\n" );
	kdLogMessage     ( "**********************************************************" );
	kdLogMessagefKHR ( "* XMKode Library v%s initializing...              *", XM_LIBRARY_VERSION );
	kdLogMessage     ( "**********************************************************" );
	kdLogMessage     ( "\n\n" );

	g_argc = argc;
	g_argv = argv;

	xmPlatformInitLibrary ( );

	#ifdef XM_HAS_PLATFORM_CRYPTO
		
	kdLogMessage ( "* Crypto initializing...\n\n" );
	
	g_crypto = xmPlatformCryptoInit ( );
	if ( !g_crypto )
	{
		kdLogMessage ( "* Crypto failed.\n\n" );
	}

	kdLogMessage( "\n" );
	
	#endif

	xmCreateTSS ( );

	xmCreateContexts ( );

	xmInitDirectory ( );

	xmInitNMEA ( );

	xmCreateSounds ( );

	xmInitTickCount ( );

	kdSrand ( (KDuint) kdTime ( 0 ) );
}

// xmFreeLibrary : Release XMKode library's resources.
KD_API KDvoid KD_APIENTRY xmFreeLibrary ( KDvoid )
{
	xmFreeNMEA ( );

	xmPlatformFreeWindow ( );

	xmFreeDirectory ( );

	xmFreeContexts ( );

	xmFreeTSS ( );

	xmFreeSounds ( );

	#ifdef XM_HAS_PLATFORM_CRYPTO
		
	if ( g_crypto )
	{
		kdLogMessage ( "* Crypto releasing...\n\n" );

		xmPlatformCryptoFree ( );
	}
	
	#endif

	kdLogMessage     ( "\n\n" );	
	kdLogMessage     ( "**********************************************************" );
	kdLogMessagefKHR ( "* XMKode Library v%s released.                    *", XM_LIBRARY_VERSION );
	kdLogMessage     ( "**********************************************************" );
	kdLogMessage     ( "\n\n" );

	xmPlatformFreeLibrary ( );
}