/* --------------------------------------------------------------------------
 *
 *      File            xm_queue.h
 *      Description     Safety queue function.
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

#ifndef __xm_queue_h__
#define __xm_queue_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XMQueue XMQueue;

typedef KDint  ( KD_APIENTRY XMQueueFindFunc ) ( KDvoid* data, KDvoid* find );
typedef KDvoid ( KD_APIENTRY XMQueueFreeFunc ) ( KDvoid* data);

KD_API XMQueue* KD_APIENTRY xmQueueCreate ( KDvoid);
KD_API KDvoid   KD_APIENTRY xmQueueFree   ( XMQueue* queue, XMQueueFreeFunc* func );

KD_API KDint    KD_APIENTRY xmQueuePushFront ( XMQueue* queue, KDvoid* data );
KD_API KDint    KD_APIENTRY xmQueuePushRear  ( XMQueue* queue, KDvoid* data );

KD_API KDvoid*  KD_APIENTRY xmQueuePopFront ( XMQueue* queue );
KD_API KDvoid*  KD_APIENTRY xmQueuePopRear  ( XMQueue* queue );
KD_API KDvoid*  KD_APIENTRY xmQueuePopFind  ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data );

KD_API KDsize   KD_APIENTRY xmQueueSize  ( XMQueue* queue );
KD_API KDvoid*	KD_APIENTRY xmQueueGet   ( XMQueue* queue, KDuint index );
KD_API KDvoid*	KD_APIENTRY xmQueueErase ( XMQueue* queue, KDuint index );

KD_API KDvoid*	KD_APIENTRY xmQueueFind	       ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data );
KD_API KDvoid*	KD_APIENTRY xmQueueReverseFind ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data );
KD_API KDvoid*	KD_APIENTRY xmQueueFindNext    ( XMQueue* queue);
KD_API KDvoid*	KD_APIENTRY xmQueueEraseFound  ( XMQueue* queue);

#ifdef __cplusplus
}
#endif

#endif 



