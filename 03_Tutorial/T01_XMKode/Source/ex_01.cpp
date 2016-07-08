/* --------------------------------------------------------------------------
 *
 *      File            ex_01.cpp
 *      Description     01. Application startup and exit
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

#include "main.h"

KDvoid xmExample_01 ( KDvoid )
{
	KDint        argc = xmGetArgc ( );
	KDchar**     argv = xmGetArgv ( );

	KDint        i;

	kdLogMessage ( "Example 01. Application startup and exit\n\n" );

	// 어플리케이션 진입점으로 들어오는 인자를 확인한다.
	if ( argc > 1 && !kdStrcmp ( argv[ 1 ], "exit" ) )
	{
		kdLogMessagefKHR ( "Case 2 : argv = %s\n", argv[ 1 ] );
		kdExit ( 0 );
		kdLogMessagefKHR ( "kdExit : Failed.\n" );
	}
	
	kdLogMessagefKHR ( "Case 1 : argc = %d\n", argc );
	for ( i = 0; i < argc; i++ )
	{
		kdLogMessagefKHR ( "Case 1 : argv [ %d ] = %s\n", i, argv[ i ] );
	}
}
