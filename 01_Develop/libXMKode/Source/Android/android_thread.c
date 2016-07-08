/* --------------------------------------------------------------------------
 *
 *      File            android_thread.c
 *      Description     Threads and synchronization
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

static KDvoid*     l_loc_key[256];

KDvoid xmAndroidPthreadKeyInit ( KDvoid )
{
	kdMemset ( l_loc_key, 0, sizeof ( KDvoid* ) * 256 );
}

KDint xmAndroidPthreadKeyCreate ( pthread_key_t* key, KDvoid ( *destructor ) (KDvoid *) )
{
	KDint    i;
	
	for ( i = 1; i < 256; i++ )
	{
		if ( l_loc_key[ i ] == 0 )
		{
			*key = i;
			return 0;
		}
	}
	
	errno = ENOMEM;

	return -1;
}

KDint xmAndroidPthreadKeyDelete ( pthread_key_t key )
{
	l_loc_key[ key ] = 0;

	return 0;
}

KDint xmAndroidPthreadSetSpecific ( pthread_key_t key, const KDvoid* value )
{
	l_loc_key[ key ] = (KDvoid *) value;

	return 0;
}

KDvoid* xmAndroidPthreadGetSpecific ( pthread_key_t key )
{
	return l_loc_key[ key ];
}