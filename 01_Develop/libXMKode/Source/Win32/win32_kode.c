/* --------------------------------------------------------------------------
 *
 *      File            win32_kode.c
 *      Description     Win32-specific definitions and functions for XMKode
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

static KDint         l_load_pthread = 0;
       HINSTANCE     g_inst = 0;

// xmPlatformInitLibrary : Initialize XMKode library in platform specific.
KDvoid xmPlatformInitLibrary ( KDvoid )
{
	WSADATA  wsaData;  
    if ( WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData ) != 0 )  
    {  
		kdLogMessage ( "* Winsock failed.\n\n" );
        kdExit ( 3 );
    }  

	kdLogMessage ( "* Pthread win32 attaching.\n\n" );

	if ( pthread_win32_process_attach_np ( ) && pthread_win32_thread_attach_np ( ) )
	{
		l_load_pthread = 1;
	}
	else
	{
		kdLogMessage ( "Pthread win32 attach failed." );
		kdExit ( 3 );
	}

	timeBeginPeriod ( 1 );
}

// xmPlatformFreeLibrary : Release XMKode library's resources in platform specific.
KDvoid xmPlatformFreeLibrary ( KDvoid )
{
	if ( l_load_pthread )
	{
		kdLogMessage ( "\n* Pthread win32 detaching.\n\n" );

		pthread_win32_thread_detach_np ( );
		pthread_win32_process_detach_np ( );
	}

	WSACleanup ( ); 
}

static KDchar* xmGetCommandLine ( LPTSTR cmd_line )
{	
	KDchar*     cmdline = 0;

	#ifdef _WIN32_WCE
	
	wchar_t*    wcmdline = 0;
	wchar_t     wpath[MAX_PATH];

	if ( GetModuleFileName ( NULL, wpath, MAX_PATH ) )
	{
		if ( wcmdline = malloc ( sizeof ( wchar_t ) * ( wcslen ( wpath ) + wcslen ( cmd_line ) + 4 ) ) )
		{
			wsprintf ( wcmdline, L"\"%s\" %s", wpath, cmd_line );
			cmdline = xmWinWtoMStr ( wcmdline );
			free ( wcmdline );
		}		
		else
		{
			kdLogMessage ( "Memory allocation failed." );
			kdExit ( 3 );
		}
	}

	#else

	XM_SET_STR ( cmdline, GetCommandLine ( ) );
	
	#endif

	return cmdline;
}

static KDvoid xmUnEscapeQuotes ( KDchar* arg )
{
	KDchar*	    last = 0;

	while ( *arg )
	{
		if ( *arg == '"' && *last == '\\' )
		{
			KDchar*	    c_curr = arg;
			KDchar*     c_last = last;

			while ( *c_curr )
			{
				*c_last = *c_curr;
				 c_last =  c_curr;
				 c_curr++;
			}

			*c_last = '\0';
		}

		last = arg;
		arg++;
	}
}

// xmParseCommandLine : Parse a command line buffer into arguments 
static KDint xmParseCommandLine ( KDchar *cmdline, KDchar **argv )
{
	KDint      argc      = 0;
	KDint      last_argc = 0;
	KDchar*    lastp     = 0;
	KDchar*    bufp      = 0;	

	for ( bufp = cmdline; *bufp; ) 
	{
		// Skip leading whitespace 
		while ( isspace ( *bufp ) )
		{
			++bufp;
		}

		// Skip over argument 
		if ( *bufp == '"' )
		{
			++bufp;

			if ( *bufp ) 
			{
				if ( argv )
				{
					argv[ argc ] = bufp;
				}

				++argc;
			}

			// Skip over word 
			while ( *bufp && ( *bufp != '"' || *lastp == '\\' ) ) 
			{
				lastp = bufp;
				++bufp;
			}
		} 
		else
		{
			if ( *bufp ) 
			{
				if ( argv )
				{
					argv[ argc ] = bufp;
				}

				++argc;
			}

			// Skip over word 
			while ( *bufp && ! isspace ( *bufp ) )
			{
				++bufp;
			}
		}

		if ( *bufp ) 
		{
			if ( argv ) 
			{
				*bufp = '\0';
			}

			++bufp;
		}

		// Strip out \ from \" sequences 
		if ( argv && last_argc != argc )
		{
			xmUnEscapeQuotes ( argv[ last_argc ] );	
		}

		last_argc = argc;	
	}

	if ( argv )
	{
		argv[ argc ] = NULL;
	}

	return argc;
}

// xmInitWin32 : Initialize XMKode library in "Win32" platform. 
KD_API KDvoid KD_APIENTRY xmWinInitLibrary ( HINSTANCE inst, HINSTANCE prev_inst, LPTSTR cmd_line, KDint cmd_show )
{
	KDint       argc = 0;
	KDchar**    argv = 0;
	KDchar*     cmdline = 0;

	g_inst = inst;

	cmdline = xmGetCommandLine ( cmd_line );
	if ( !cmdline )
	{
		kdLogMessage ( "Get command line failed." );
		kdExit ( 3 );
	}

	// Parse it into argv and argc 
	argc = xmParseCommandLine ( cmdline, NULL );
	argv = malloc ( sizeof ( KDchar* ) * argc + 1 ); 

	if  ( !argv )
	{
		kdLogMessage ( "Memory allocation failed." );
		kdExit ( 3 );
	}

	xmParseCommandLine ( cmdline, argv );

	xmInitLibrary ( argc, argv );
}


