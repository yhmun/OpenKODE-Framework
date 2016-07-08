/* --------------------------------------------------------------------------
 *
 *      File            linux_time.c
 *      Description     Time functions
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

// xmPlatformGetTimeUST : Get the current unadjusted system time in platform specific.
KDust xmPlatformGetTimeUST ( KDvoid )
{	
	struct timeval    time;

    gettimeofday ( &time, NULL );

	return (KDust) ( time.tv_sec * 1000000000LL + time.tv_usec * 1000LL );
}

// xmPlatformUSTAtEpoch : Get the UST corresponding to KDtime 0 in platform specific.
KDust xmPlatformUSTAtEpoch ( KDvoid )
{			
	return xmPlatformGetTimeUST ( );
}
