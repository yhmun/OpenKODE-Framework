/* --------------------------------------------------------------------------
 *
 *      File            kd_assert.c
 *      Description     Assertions and logging
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

// kdHandleAssertion : Handle assertion failure.
KD_API KDvoid KD_APIENTRY kdHandleAssertion ( const KDchar* condition, const KDchar* filename, KDint linenumber )
{
	kdLogMessagefKHR ( "*** XMKode Assertion Failure ***" );
	kdLogMessagefKHR ( "Condition   : %s", condition );
	kdLogMessagefKHR ( "File Name   : %s", filename );
	kdLogMessagefKHR ( "Line Number : %d", linenumber );

	kdExit ( 3 );
}

// kdLogMessage : Output a log message.
KD_API KDvoid KD_APIENTRY kdLogMessage ( const KDchar* string )
{
	unsigned int    length  = (unsigned int) strlen ( string );
	char*           message = 0;
	
	if ( length > 0 )
	{				
		if ( strcmp ( &string[ length - 1 ], "\n" ) )
		{
			message = (char *) malloc ( length + 2 );
			strcpy( message, string );
			strcat( message, "\n" );
		}
		else
		{
			message = (char *) string;
		}

		#ifdef XM_HAS_PLATFORM_LOG

		xmPlatformLogMessage ( message );

		#else
		
		printf ( "%s", message );
	
		#endif

		#ifdef XM_HAS_PLATFORM_FILE_LOG

		xmPlatformFileLog ( message );

		#endif

		if ( strcmp ( message, string ) )
		{
			free ( message );
		}
	}
}
