/* --------------------------------------------------------------------------
 *
 *      File            khr_thread_storage.h
 *      Description     Thread-local storage based on unique identifier functions.
 *      Version         Version 5, 2008-06-20
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __kd_KHR_thread_storage_h_
#define __kd_KHR_thread_storage_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

// KDThreadStorageKeyKHR : The representation of a thread storage key.
typedef KDuint32                KDThreadStorageKeyKHR;

// kdMapThreadStorageKHR : Maps an arbitrary pointer to a global thread storage key.
KD_API KDThreadStorageKeyKHR KD_APIENTRY KD_APIENTRY kdMapThreadStorageKHR ( const KDvoid* id );

// kdSetThreadStorageKHR : Stores thread-local data.
KD_API KDint KD_APIENTRY KD_APIENTRY kdSetThreadStorageKHR ( KDThreadStorageKeyKHR key, KDvoid* data );

// kdGetThreadStorageKHR : Retrieves previously stored thread-local data.
KD_API KDvoid* KD_APIENTRY KD_APIENTRY kdGetThreadStorageKHR ( KDThreadStorageKeyKHR key );

#ifdef __cplusplus
}
#endif

#endif 

