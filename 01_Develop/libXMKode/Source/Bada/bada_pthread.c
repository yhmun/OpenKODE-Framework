/* --------------------------------------------------------------------------
 *
 *      File            bada_pthread.c
 *      Description     PThread functions.
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

#define XM_LOC_KEY_COUNT        256

static KDvoid*    l_loc_key[ XM_LOC_KEY_COUNT ];

struct XMPthreadAttr
{
	KDsize        stacksize;
	KDint         detachstate;
};

class XMPthread : public Thread
{
public :

	XMPthread ( void ) : m_routine( 0 ), m_arg( 0 ), m_ret( 0 )
	{

	}

	~XMPthread ( void )
	{

	}

	result	Construct ( pthread_attr_t attr, KDvoid* ( *routine )( KDvoid * ), KDvoid* arg )
	{
		m_routine = routine;
		m_arg     = arg;

		Thread::Construct ( THREAD_TYPE_WORKER, attr ? attr->stacksize : DEFAULT_STACK_SIZE, THREAD_PRIORITY_HIGH );

		Start ( );

		return E_SUCCESS;
	}

	Object*	Run ( KDvoid )
	{
		m_ret = m_routine ( m_arg );

		return null;
	}

	KDvoid*    ( *m_routine )( KDvoid * );
	KDvoid*    m_arg;
	KDvoid*    m_ret;
};

KDvoid xmBadaPthreadInit ( KDvoid )
{
	kdMemset ( l_loc_key, 0, sizeof ( KDvoid* ) * XM_LOC_KEY_COUNT );
}

KDvoid xmBadaPthreadFree ( KDvoid )
{
	KDint    i;

	for ( i = 0 ; i < XM_LOC_KEY_COUNT; i++ )
	{
		if ( l_loc_key[ i ] != 0 )
		{
			kdFree ( l_loc_key[ i ] );
		}
	}
}

KDint xmBadaPthreadAttrInit ( pthread_attr_t* attr )
{
	if ( ( *attr = ( XMPthreadAttr * ) kdMalloc( sizeof ( XMPthreadAttr ) ) ) )
	{
		( *attr )->stacksize   = Thread::DEFAULT_STACK_SIZE;
		( *attr )->detachstate = PTHREAD_CREATE_JOINABLE;
	}
	else
	{
		errno = ENOMEM;
		return -1;
	}

	return 0;
}

KDint xmBadaPthreadAttrDestroy ( pthread_attr_t* attr )
{
	kdFree ( *attr );

	return 0;
}

KDint xmBadaPthreadAttrSetdetachstate ( pthread_attr_t* attr, KDint detachstate )
{
	( *attr )->detachstate = detachstate;

	return 0;
}

KDint xmBadaPthreadAttrSetstacksize ( pthread_attr_t* attr, KDsize stacksize )
{
	( *attr )->stacksize = stacksize;

	return 0;
}

KDint xmBadaPthreadCreate ( pthread_t* tid, const pthread_attr_t* attr, KDvoid* ( *start ) (KDvoid *), KDvoid* arg )
{
	if ( ( *tid = new XMPthread ) )
	{
		(*tid)->Construct ( attr ? *attr : 0, start, arg );
	}
	else
	{
		errno = ENOMEM;
		return -1;
	}

	return 0;
}

KDint xmBadaPthreadDetach ( pthread_t tid )
{
	tid->Stop ( );
	delete tid;

	return 0;
}

KDint xmBadaPthreadEqual ( pthread_t t1, pthread_t t2 )
{
	return t1 == t2 ? 1 : 0;
}

KDvoid xmBadaPthreadExit ( KDvoid* value_ptr )
{
	XMPthread*    thread = (XMPthread *) Thread::GetCurrentThread ( );

	thread->m_ret = value_ptr;

	Thread::Exit ( 0 );
}

KDint xmBadaPthreadJoin ( pthread_t thread, KDvoid** value_ptr )
{
	result r = E_SUCCESS;

	r = thread->Join ( );

	if ( value_ptr )
	{
		*value_ptr = thread->m_ret;
	}

	thread->Stop ( );
	delete thread;

	return IsFailed ( r ) ? -1 : 0;
}

pthread_t xmBadaPthreadSelf ( KDvoid )
{
	return (pthread_t) Thread::GetCurrentThread ( );
}

KDint xmBadaPthreadOnce ( pthread_once_t* once_control, KDvoid ( *init_routine ) ( KDvoid ) )
{
	if ( *once_control == 0 )
	{
		*once_control = new void*;

		init_routine ( );
	}

	return 0;
}

KDint xmBadaPthreadMutexInit ( pthread_mutex_t* mutex, const pthread_mutexattr_t* attr )
{
	result    r;

	if ( ( *mutex = new Mutex ) )
	{
		r = ( *mutex )->Create ( );

		if ( IsFailed ( r ) )
		{
			errno = EAGAIN;
			delete *mutex;
			*mutex = 0;
		}
	}
	else
	{
		errno = ENOMEM;

		return -1;
	}

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadMutexDestroy ( pthread_mutex_t* mutex )
{
	delete *mutex;

	return 0;
}

KDint xmBadaPthreadMutexLock ( pthread_mutex_t* mutex )
{
	result    r;

	r = ( *mutex )->Acquire ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadMutexUnlock ( pthread_mutex_t* mutex )
{
	result    r;

	r = ( *mutex )->Release ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadCondInit ( pthread_cond_t* cond, const pthread_condattr_t* attr )
{
	result    r;

	if ( ( *cond = new Monitor ) )
	{
		r = ( *cond )->Construct ( );

		if ( r == E_SUCCESS )
		{
			r = ( *cond )->Enter ( );
		}

		if ( IsFailed ( r ) )
		{
			errno = EAGAIN;
			delete *cond;
			*cond = 0;
		}
	}
	else
	{
		errno = ENOMEM;

		return -1;
	}

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadCondDestroy ( pthread_cond_t* cond )
{
	( *cond )->Exit ( );

	delete *cond;

	return 0;
}

KDint xmBadaPthreadCondSignal ( pthread_cond_t* cond )
{
	result    r;

	r = ( *cond )->Notify ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadCondBroadcast ( pthread_cond_t* cond )
{
	result    r;

	r = ( *cond )->NotifyAll ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadCondWait ( pthread_cond_t* cond, pthread_mutex_t* mutex )
{
	result    r;

	pthread_mutex_unlock ( mutex );

	r = ( *cond )->Wait ( );

	pthread_mutex_lock ( mutex );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaSemInit ( sem_t* sem, KDint pshared, KDuint value )
{
	result    r;

	if ( ( *sem = new Semaphore ) )
	{
		r = ( *sem )->Create ( value );

		if ( IsFailed ( r ) )
		{
			errno = EINVAL;
			delete *sem;
			*sem = 0;
		}
	}
	else
	{
		errno = EINVAL;

		return -1;
	}

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaSemDestroy ( sem_t* sem )
{
	delete *sem;

	return 0;
}

KDint xmBadaSemWait ( sem_t* sem )
{
	result    r;

	r = ( *sem )->Acquire ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaSemPost ( sem_t* sem )
{
	result    r;

	r = ( *sem )->Release ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmBadaPthreadKeyCreate ( pthread_key_t* key, KDvoid ( *destructor ) (KDvoid *) )
{
	KDint    i;

	for ( i = 1; i < XM_LOC_KEY_COUNT; i++ )
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

KDint xmBadaPthreadKeyDelete ( pthread_key_t key )
{
	l_loc_key[ key ] = 0;

	return 0;
}

KDint xmBadaPthreadSetspecific ( pthread_key_t key, const KDvoid* value )
{
	l_loc_key[ key ] = (KDvoid *) value;

	return 0;
}

KDvoid* xmBadaPthreadGetspecific ( pthread_key_t key )
{
	return l_loc_key[ key ];
}


