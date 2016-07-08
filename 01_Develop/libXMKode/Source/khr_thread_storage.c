/* --------------------------------------------------------------------------
 *
 *      File            khr_thread_storage.c
 *      Description     Thread-local storage based on unique identifier functions.
 *      Version         5, 2008-06-20
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

typedef	struct XMThreadStorage  XMThreadStorage;

struct XMThreadStorage 
{
	const KDvoid*           id;
	KDThreadStorageKeyKHR   key;
	pthread_key_t           pkey;
};

static XMQueue*             l_tss = 0;

static KDint _xmFindTSbyID ( KDvoid* d, KDvoid* f )
{
	XMThreadStorage*       data = (XMThreadStorage *) d;
	KDvoid*                find = (KDvoid *) f;

	return data->id == find ? 0 : -1;
}

static KDint _xmFindTSByKey ( KDvoid* d, KDvoid* f)
{
	XMThreadStorage*        data = (XMThreadStorage *) d;
	KDThreadStorageKeyKHR*  key  = (KDThreadStorageKeyKHR *) f;
		
	return data->key == *key ? 0 : -1;
}

static KDvoid _xmFreeTS ( KDvoid* f )
{
	XMThreadStorage*        ts = (XMThreadStorage *) f;
	KDvoid*                 value = 0;

	if ( ts )
	{
		value = (KDvoid *) pthread_getspecific ( ts->pkey );

		if ( value )
		{
			kdFree ( value );
			pthread_setspecific ( ts->pkey, 0 );
		}
		
		pthread_key_delete ( ts->pkey );
		kdFree ( ts );
	}
}

KDvoid xmCreateTSS ( KDvoid )
{
	l_tss = xmQueueCreate ( );

	if ( !l_tss )
	{
		kdLogMessage ( "Thread storage key manager create failed." );
		kdExit ( 3 );
	}
}

KDvoid xmFreeTSS ( KDvoid )
{
	if ( l_tss )
	{
		xmQueueFree ( l_tss, _xmFreeTS );
		l_tss = 0;
	}	
}

XMThreadStorage* xmCreateTS ( const KDvoid* id )
{
	XMThreadStorage*          ts = 0;

	if ( ( ts = (XMThreadStorage *) kdMalloc ( sizeof ( XMThreadStorage ) ) ) )
	{
		if ( pthread_key_create ( &ts->pkey, 0 ) == 0 )
		{
			if ( !xmQueuePushRear ( l_tss, ts ) )
			{
				ts->id  = id;
				ts->key = (KDThreadStorageKeyKHR) ts->pkey;
			}
			else
			{
				pthread_key_delete ( ts->pkey );
				kdFree ( ts );
				ts = 0;	
			}
		}
		else
		{
			kdSetError ( KD_ENOMEM );
			kdFree ( ts );
			ts = 0;	
		}
	}

	return ts;
}

KDvoid xmFreeTS ( KDThreadStorageKeyKHR key )
{
	XMThreadStorage*         ts  = 0;

	if ( ( ts = (XMThreadStorage *) xmQueuePopFind ( l_tss, _xmFindTSByKey, &key ) ) )
	{
		_xmFreeTS ( ts );
	}	
}

// kdMapThreadStorageKHR : Maps an arbitrary pointer to a global thread storage key.
KD_API KDThreadStorageKeyKHR KD_APIENTRY KD_APIENTRY kdMapThreadStorageKHR ( const KDvoid* id )
{
	XMThreadStorage*        ts  = 0;
	KDThreadStorageKeyKHR   key = 0;

	if ( ( ts = (XMThreadStorage *) xmQueueFind ( l_tss, _xmFindTSbyID, (KDvoid *) id ) ) )
	{
		key = ts->key;
	}
	else
	{
		if ( ( ts = xmCreateTS ( id ) ) )
		{
			key = ts->key;
		}
	}
	
	return key;
}

// kdSetThreadStorageKHR : Stores thread-local data.
KD_API KDint KD_APIENTRY KD_APIENTRY kdSetThreadStorageKHR ( KDThreadStorageKeyKHR key, KDvoid* data )
{
	XMThreadStorage*        ts  = 0;
	KDint                   ret = KD_ENOMEM;
	
	if ( ( ts = (XMThreadStorage *) xmQueueFind ( l_tss, _xmFindTSByKey, &key ) ) )
	{
		if ( pthread_setspecific ( ts->pkey, data ) == 0 )
		{
			ret = 0;
		}
	}
	
	return ret;
}

// kdGetThreadStorageKHR : Retrieves previously stored thread-local data.
KD_API KDvoid* KD_APIENTRY KD_APIENTRY kdGetThreadStorageKHR ( KDThreadStorageKeyKHR key )
{	
	XMThreadStorage*        ts  = 0;
	KDvoid*                 ret = 0;
	
	if ( ( ts = (XMThreadStorage *) xmQueueFind ( l_tss, _xmFindTSByKey, &key ) ) )
	{		
		ret = (void *) pthread_getspecific ( ts->pkey );
	}
	
	return ret;
}