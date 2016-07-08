/* --------------------------------------------------------------------------
 *
 *      File            win32_string.c
 *      Description     String manipulation functions.
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

// xmWinMtoWStr, xmWinWtoMStr : Convert between Multi-Byte and Wide-Char string.
wchar_t* xmWinMtoWStr ( const char* mstr )
{
	int         len;
	wchar_t*    wstr;

	len = MultiByteToWideChar ( CP_UTF8, 0, mstr, strlen ( mstr ), 0, 0 );
	if ( !len )
	{
		return 0;
	}
	
	wstr = (wchar_t *) malloc ( sizeof ( wchar_t ) * ( len + 1 ) );
	kdAssert ( wstr );

	MultiByteToWideChar ( CP_UTF8, 0, mstr, -1, wstr, len );
	wstr[ len ] = L'\0';
	
	return wstr;
}

char* xmWinWtoMStr ( const wchar_t* wstr )
{
	int         len;
	char*       mstr;

	len = WideCharToMultiByte ( CP_UTF8, 0, wstr, -1, 0, 0, NULL, NULL );
	if ( !len )
	{
		return 0;
	}

	mstr = (char *) malloc ( sizeof ( char ) * ( len + 1 ) );
	kdAssert ( mstr );		

	WideCharToMultiByte ( CP_UTF8, 0, wstr, -1, mstr, len, NULL, NULL );
	mstr[ len ] = '\0';	

	return mstr;
}