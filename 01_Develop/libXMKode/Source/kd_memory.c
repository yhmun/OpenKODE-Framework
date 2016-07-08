/* --------------------------------------------------------------------------
 *
 *      File            kd_memory.c
 *      Description     Memory allocation
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

// kdMalloc : Allocate memory.
KD_API KDvoid* KD_APIENTRY kdMalloc ( KDsize size )
{
	KDvoid*     mem; 

	mem = malloc ( size );
	if ( !mem )
	{
		kdSetError ( KD_ENOMEM );
	}

	return mem;
}

// kdFree : Free allocated memory block. 
KD_API KDvoid KD_APIENTRY kdFree ( KDvoid* ptr )
{
	free ( ptr );
}

// kdRealloc : Resize memory block. 
KD_API KDvoid* KD_APIENTRY kdRealloc ( KDvoid* ptr, KDsize size )
{
	KDvoid*     mem; 

	mem = realloc( ptr, size );
	if ( !mem )
	{
		kdSetError ( KD_ENOMEM );
	}

	return mem;
}

// kdCalloc : Allocate space for array in memory
//            Allocates a block of memory for an array of num elements,
//            each of them size bytes long, and initializes all its bits to zero.
// XMSoft's revision : Added API.
KD_API KDvoid* KD_APIENTRY kdCalloc ( KDsize num, KDsize size )
{
	KDvoid*     mem; 

	mem = calloc ( num, size );
	if ( !mem )
	{
		kdSetError ( KD_ENOMEM );
	}

	return mem;
}