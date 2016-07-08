/* --------------------------------------------------------------------------
 *
 *      File            win32_utility.c
 *      Description     Utility library functions
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

#ifdef _WIN32_WCE

void* xmWinBsearch ( const void* key, const void* base0, size_t nmemb, size_t size, int ( *compar ) ( const void*, const void* ) )
{
	register const char*  base = (const char*) base0;
	register size_t       lim;
	register int          cmp;
	register const void*  p;

	for ( lim = nmemb; lim != 0; lim >>= 1 )
	{
		p = base + (lim >> 1) * size;
		cmp = ( *compar )( key, p );
		if ( cmp == 0 )
		{
			return ( (void *) p );
		}
		if ( cmp > 0 ) 
		{	
			base = (char *) p + size;
			lim--;
		}		
	}

	return KD_NULL;
}

#endif
