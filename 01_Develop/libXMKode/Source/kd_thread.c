/* --------------------------------------------------------------------------
 *
 *      File            kd_thread.c
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

#include "kd_library.h"

struct KDThreadAttr
{
	pthread_attr_t      pattr;
};

struct KDThreadMutex
{	
	pthread_mutex_t     pmutex;
};

struct KDThreadSem
{
	sem_t               psem;
};

struct KDThreadCond
{
	pthread_cond_t      pcond;
};

static KDint _xmFindPthread ( KDvoid* d, KDvoid* f )
{
	XMContext*      data = (XMContext *) d;
	pthread_t*      find = (pthread_t *) f;

	return pthread_equal ( data->thread->pthread, *find ) ? 0 : -1;
}

static KDint _xmFindContext ( KDvoid* d, KDvoid* f )
{		
	XMContext* 	    data = (XMContext *) d;
	KDThread*	    find = (KDThread  *) f;

	return data->thread == find ? 0 : -1;
}

// kdThreadAttrCreate : Create a thread attribute object. 
KD_API KDThreadAttr* KD_APIENTRY kdThreadAttrCreate ( KDvoid )
{	
	KDThreadAttr*   attr = 0;

	if ( ( attr = (KDThreadAttr *) kdMalloc ( sizeof ( KDThreadAttr ) ) ) )
	{
		if ( ( pthread_attr_init ( &attr->pattr ) ) )
		{
			kdSetError ( KD_ENOMEM );	
			kdFree ( attr );
			attr = 0;
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}

	return attr;	
}

// kdThreadAttrFree : Free a thread attribute object. 
KD_API KDint KD_APIENTRY kdThreadAttrFree ( KDThreadAttr* attr )
{	 
	return pthread_attr_destroy ( &attr->pattr );	
}

// kdThreadAttrSetDetachState : Set detachstate attribute.
KD_API KDint KD_APIENTRY kdThreadAttrSetDetachState ( KDThreadAttr* attr, KDint detachstate )
{	
	KDint          state = 0;
	
	switch ( detachstate )
	{
		case KD_THREAD_CREATE_JOINABLE : state = PTHREAD_CREATE_JOINABLE; break;
		case KD_THREAD_CREATE_DETACHED : state = PTHREAD_CREATE_DETACHED; break;
	}	

	return pthread_attr_setdetachstate ( &attr->pattr, state );	
}

// kdThreadAttrSetStackSize : Set stacksize attribute.
KD_API KDint KD_APIENTRY kdThreadAttrSetStackSize ( KDThreadAttr* attr, KDsize stacksize )
{
	KDint          ret = 0;
	
	if ( !pthread_attr_setstacksize ( &attr->pattr, stacksize ) )
	{
		kdSetError ( KD_EINVAL );
		ret = -1;
	}

	return ret;	
}

// kdThreadCreate : Create a new thread. 
KD_API KDThread* KD_APIENTRY kdThreadCreate ( const KDThreadAttr* attr, KDvoid* (* start_routine)(KDvoid *), KDvoid* arg )
{		
	XMContext*      context = 0;
	KDint           ret     = -1;

	if ( ( context = xmCreateContext ( ) ) )
	{
		if ( !xmQueuePushRear ( xmGetContexts ( ), context ) )
		{	
			if ( !pthread_create ( &context->thread->pthread, attr ? &attr->pattr : 0, start_routine, arg ) )
			{
				ret = 0;				
			}
			else
			{					
				xmQueuePopRear ( xmGetContexts ( ) );
				xmFreeContext ( context );					
			}			
		}
		else
		{
			xmFreeContext ( context );			
		}
	}
	else 
	{
		kdSetError ( KD_ENOMEM );
	}	

	return ret ? 0 : context->thread;	
}

// kdThreadExit : Terminate this thread.
KD_API KD_NORETURN KDvoid KD_APIENTRY kdThreadExit ( KDvoid* retval )
{	
	XMQueue*       contexts = 0;
	XMContext*     context  = 0;
	KDThread*      thread   = 0;
	
	contexts = xmGetContexts ( );
	context  = (XMContext *) xmQueueGet ( contexts , 0 );
	thread   = kdThreadSelf ( );

	if ( context->thread == thread )
	{
		kdExit ( 0 );
	}
	else
	{
		pthread_exit ( retval );
	}
}

// kdThreadJoin : Wait for termination of another thread. 
KD_API KDint KD_APIENTRY kdThreadJoin ( KDThread* thread, KDvoid** retval )
{		
	XMContext*     context = 0;
	KDint          ret     = -1;	
		
	if ( !( ret = pthread_join ( thread->pthread, retval ) ) )
	{

	}
	else	
	{			
		kdSetError ( ret == EDEADLK ? KD_EDEADLK : KD_EINVAL );
		ret = -1;						
	}
	
	if ( ( context = (XMContext *) xmQueuePopFind ( xmGetContexts ( ), _xmFindContext, thread ) ) )
	{
		xmFreeContext ( context );	
	}

	return ret;
}

// kdThreadDetach : Allow resources to be freed as soon as a thread terminates.
KD_API KDint KD_APIENTRY kdThreadDetach ( KDThread* thread )
{
	XMContext*     context = 0;
	KDint          ret     = -1;	

	if ( !( ret = pthread_detach ( thread->pthread ) ) )
	{

	}
	else
	{
		kdSetError ( KD_EINVAL );
		ret = -1;	
	}

	if ( ( context = (XMContext *) xmQueuePopFind ( xmGetContexts ( ), _xmFindContext, thread ) ) )
	{
		xmFreeContext ( context );	
	}

	return ret;
}

// kdThreadSelf : Return calling thread's ID. 
KD_API KDThread* KD_APIENTRY kdThreadSelf ( KDvoid )
{		
	XMContext*      context = 0;	
	pthread_t       pthread;
	
	pthread = pthread_self ( );
	context = (XMContext *) xmQueueFind ( xmGetContexts ( ), _xmFindPthread, &pthread );
	
	return context ? context->thread : 0;
}

// kdThreadOnce : Wrap initialization code so it is executed only once.
KD_API KDint KD_APIENTRY kdThreadOnce ( KDThreadOnce *once_control, KDvoid (* init_routine) ( KDvoid ) )
{			
	if ( !once_control->impl )
	{
		pthread_once_t  ponce = PTHREAD_ONCE_INIT; 

		once_control->impl = kdMalloc ( sizeof ( pthread_once_t ) );
		kdMemcpy ( once_control->impl, (const KDvoid *) &ponce, sizeof ( pthread_once_t ) );
	}
	
	return pthread_once ( (pthread_once_t *) once_control->impl, init_routine );	
} 
 
// kdThreadMutexCreate : Create a mutex.
KD_API KDThreadMutex* KD_APIENTRY kdThreadMutexCreate ( const KDvoid* mutexattr )
{
	pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;
	KDThreadMutex*  mutex  = 0;	
	KDint           ret	   = 0;	
	
	if ( ( mutex = (KDThreadMutex *) kdMalloc ( sizeof ( KDThreadMutex ) ) ) )
	{
		kdMemcpy ( &mutex->pmutex, &pmutex, sizeof ( pthread_mutex_t ) );

		if ( ( ret = pthread_mutex_init ( &mutex->pmutex, (const pthread_mutexattr_t *) mutexattr ) ) )
		{
			kdSetError ( ret == ENOMEM ? KD_ENOMEM : KD_EAGAIN );
			kdFree ( mutex );
			mutex = 0;
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}	
	
	return mutex;	
}

// kdThreadMutexFree : Free a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexFree ( KDThreadMutex* mutex )
{	
	KDint          ret = pthread_mutex_destroy ( &mutex->pmutex );

	kdFree ( mutex );

	return ret;
}

// kdThreadMutexLock : Lock a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexLock ( KDThreadMutex* mutex )
{			
	return pthread_mutex_lock ( &mutex->pmutex );	
}

// kdThreadMutexUnlock : Unlock a mutex.
KD_API KDint KD_APIENTRY kdThreadMutexUnlock ( KDThreadMutex* mutex )
{	
	return pthread_mutex_unlock ( &mutex->pmutex );	
}

// kdThreadCondCreate : Create a condition variable.
KD_API KDThreadCond* KD_APIENTRY kdThreadCondCreate ( const KDvoid* attr )
{
	pthread_cond_t  pcond = PTHREAD_COND_INITIALIZER;
	KDThreadCond*   cond  = 0;
	KDint           ret	  = 0;	

	if ( ( cond = (KDThreadCond *) kdMalloc ( sizeof ( KDThreadCond ) ) ) )
	{
		kdMemcpy ( &cond->pcond, &pcond, sizeof ( pthread_cond_t ) );

		if ( ( ret = pthread_cond_init ( &cond->pcond, (const pthread_condattr_t *) attr) ) )
		{
			kdSetError ( ret == ENOMEM ? KD_ENOMEM : KD_EAGAIN );
			kdFree ( cond );
			cond = 0;
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}

	return cond;
}

// kdThreadCondFree: Free a condition variable. 
KD_API KDint KD_APIENTRY kdThreadCondFree ( KDThreadCond* cond )
{
	KDint          ret = pthread_cond_destroy ( &cond->pcond );	

	kdFree ( cond );

	return ret;
}

// kdThreadCondSignal, kdThreadCondBroadcast : Signal a condition variable. 
KD_API KDint KD_APIENTRY kdThreadCondSignal ( KDThreadCond* cond )
{
	return pthread_cond_signal ( &cond->pcond );		
}

KD_API KDint KD_APIENTRY kdThreadCondBroadcast ( KDThreadCond* cond )
{
	return pthread_cond_broadcast ( &cond->pcond );		
}

// kdThreadCondWait : Wait for a condition variable to be signalled. 
KD_API KDint KD_APIENTRY kdThreadCondWait ( KDThreadCond* cond, KDThreadMutex* mutex )
{
	return pthread_cond_wait ( &cond->pcond, &mutex->pmutex );		
}

// kdThreadSemCreate: Create a semaphore. 
KD_API KDThreadSem *KD_APIENTRY kdThreadSemCreate ( KDuint value )
{
	KDThreadSem*    sem = 0;
	
	if ( ( sem = (KDThreadSem *) kdMalloc ( sizeof ( KDThreadSem ) ) ) )
	{
		if ( ( sem_init ( &sem->psem, 0, value ) ) )
		{
			kdSetError ( xmGetErrno ( ) == EINVAL ? KD_EINVAL : KD_ENOSPC );
			kdFree ( sem );
			sem = 0;
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}		

	return sem;
}

// kdThreadSemFree : Free a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemFree ( KDThreadSem* sem )
{	
	KDint          ret = sem_destroy ( &sem->psem );		

	kdFree ( sem );

	return ret;
}

// kdThreadSemWait : Lock a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemWait ( KDThreadSem* sem )
{	
	return sem_wait ( &sem->psem );		
}

// kdThreadSemPost : Unlock a semaphore. 
KD_API KDint KD_APIENTRY kdThreadSemPost ( KDThreadSem* sem )
{	
	return sem_post ( &sem->psem );		
}

