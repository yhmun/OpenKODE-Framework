/* --------------------------------------------------------------------------
 *
 *      File            mac_semaphore.c
 *      Description     Semaphore functions
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

KDint xmMacSemInit ( sem_t* sem, KDint pshared, KDuint value )
{
	kern_return_t  kret = 0;
	KDint          ret  = -1;
	
	kret = semaphore_create( mach_task_self ( ), sem, SYNC_POLICY_FIFO, value );

	switch ( kret )
	{ 		
		case KERN_RESOURCE_SHORTAGE	: errno = ENOSPC; break;
		case KERN_INVALID_ARGUMENT  : errno = EINVAL; break; 
		case KERN_SUCCESS           : ret   = 0;      break;
	}
	
	return ret;
}

KDint xmMacSemDestroy ( sem_t* sem )
{
	kern_return_t  kret = 0;
	KDint          ret  = -1;
	
	kret = semaphore_destroy ( mach_task_self ( ), *sem );
	
	switch ( kret )
	{ 		
		case KERN_INVALID_RIGHT    :
		case KERN_TERMINATED       :	
		case KERN_INVALID_ARGUMENT : errno = EINVAL; break; 
		case KERN_SUCCESS          : ret   = 0;	     break;
	}
	
	return ret;
}

KDint xmMacSemWait ( sem_t* sem )
{
	kern_return_t  kret = 0;
	KDint          ret  = -1;
	
	kret = semaphore_wait ( *sem );
	
	switch ( kret )
	{ 		
		case KERN_ABORTED          :	
		case KERN_TERMINATED       :	
		case KERN_INVALID_ARGUMENT : errno = EINVAL; break; 
		case KERN_SUCCESS          : ret   = 0;	     break;
	}
	
	return ret;
}

KDint xmMacSemPost ( sem_t* sem )
{
	kern_return_t  kret = 0;
	KDint          ret  = -1;
	
	kret = semaphore_signal ( *sem );
	
	switch ( kret )
	{		
		case KERN_TERMINATED       :	
		case KERN_INVALID_ARGUMENT : errno = EINVAL; break; 
		case KERN_SUCCESS          : ret   = 0;	     break;
	}
	
	return ret;
}