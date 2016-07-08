/* --------------------------------------------------------------------------
 *
 *      File            kd_thread_local_storage.c
 *      Description     Thread-local storage.
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

// kdGetTLS : Get the thread-local storage pointer. 
KD_API KDvoid* KD_APIENTRY kdGetTLS ( KDvoid )
{
	XMContext*  context = 0;
	KDvoid*     ret		= 0;

	if ( ( context = xmFindContext ( kdThreadSelf ( ) ) ) )
	{		
		ret = kdGetThreadStorageKHR( context->key );
	}	

	return ret;
}

// kdSetTLS : Set the thread-local storage pointer. 
KD_API KDvoid KD_APIENTRY kdSetTLS ( KDvoid* ptr )
{
	XMContext*  context = 0;

	if ( ( context = xmFindContext ( kdThreadSelf() ) ) )
	{
		if ( context->key == 0 )
		{
			context->key = kdMapThreadStorageKHR ( context );
		}
		
		kdSetThreadStorageKHR ( context->key, ptr );		
	}	
}