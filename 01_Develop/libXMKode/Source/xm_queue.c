/* --------------------------------------------------------------------------
 *
 *      File            xm_queue.c
 *      Description     Safety queue function.
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

typedef struct XMQueueNode      XMQueueNode;
typedef struct XMQueueFind      XMQueueFind;

#define KD_FORWARD              0
#define XM_REVERSE              1

struct XMQueueNode
{
	KDvoid*             data;	
	XMQueueNode*        prev;
	XMQueueNode*        next;
};

struct XMQueueFind
{		
	KDint               dir;
	XMQueueNode*        node;		
	XMQueueFindFunc*    func;		
	KDvoid*             data;	
};

struct XMQueue
{		
	KDsize              size;
	KDThreadSem*        sem;
	XMQueueNode*        front;
	XMQueueNode*        rear;		
	XMQueueFind         find;	
};

static XMQueueNode* xmQueueNodeCreate ( KDvoid* data )
{
	XMQueueNode*    node = 0;

	if ( ( node = (XMQueueNode *) kdMalloc ( sizeof ( XMQueueNode ) ) ) )
	{
		node->data = data;
		node->prev = 0;
		node->next = 0;
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}

	return node;
}

static KDvoid xmQueueNodeErase ( XMQueue* queue, XMQueueNode* node )
{
	if ( queue->size )
	{
		if ( queue->front == queue->rear )
		{
			queue->front	   = 0;
			queue->rear		   = 0;			
		}
		else if ( node == queue->front )
		{
			queue->front	   = queue->front->next;
			queue->front->prev = 0;			
		}
		else if ( node == queue->rear )
		{
			queue->rear		   = queue->rear->prev;
			queue->rear->next  = 0;			
		}
		else
		{
			node->prev->next   = node->next;
			node->next->prev   = node->prev;				
		}

		queue->size--;
			
		kdFree ( node );		
	}
}

KD_API XMQueue* KD_APIENTRY xmQueueCreate ( KDvoid )
{
	XMQueue*    queue = 0;		

	if ( ( queue = (XMQueue *) kdMalloc ( sizeof ( XMQueue ) ) ) )
	{
		kdMemset ( queue, 0, sizeof ( XMQueue ) );

		if ( !( queue->sem = kdThreadSemCreate ( 1 ) ) )
		{		
			kdSetError ( KD_ENOMEM );
			kdFree ( queue );
			queue = 0;
		}
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}
	
	return queue;
}

KD_API KDvoid KD_APIENTRY xmQueueFree ( XMQueue* queue, XMQueueFreeFunc* func )
{
	kdThreadSemWait ( queue->sem );	

	while ( queue->rear ) 
	{
		if ( func )
		{
			func ( queue->rear->data );
		}
		else
		{
			if ( queue->rear->data )
			{
				kdFree ( queue->rear->data );
			}
		}

		queue->rear = queue->rear->prev;		
	}

	kdThreadSemPost ( queue->sem );	

	kdThreadSemFree ( queue->sem );
	kdFree ( queue );
}

KD_API KDint KD_APIENTRY xmQueuePushFront ( XMQueue* queue, KDvoid* data ) 
{
	XMQueueNode*    node = 0;
	KDint           ret  = -1;

	if ( ( node = xmQueueNodeCreate ( data ) ) )
	{
		kdThreadSemWait ( queue->sem );	

		if ( queue->size )
		{			
			        node->next = queue->front;
			queue->front->prev = node;		
			queue->front	   = node;
		}
		else
		{
			queue->front = node;
			queue->rear	 = node;
		}

		queue->size++;

		kdThreadSemPost ( queue->sem );	
		
		ret = 0;
	}
	
	return ret;
}

KD_API KDint KD_APIENTRY xmQueuePushRear ( XMQueue* queue, KDvoid* data )
{
	XMQueueNode*    node = 0;
	KDint           ret  = -1;

	if ( ( node = xmQueueNodeCreate ( data ) ) )
	{
		kdThreadSemWait ( queue->sem );	

		if ( queue->size )
		{
			       node->prev = queue->rear;
			queue->rear->next = node;		
			queue->rear		  = node;
		}
		else
		{			
			queue->front = node;
			queue->rear	 = node;
		}

		queue->size++;

		kdThreadSemPost ( queue->sem );	

		ret = 0;
	}
		
	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueuePopFront ( XMQueue* queue )
{
	XMQueueNode*    node = 0;
	KDvoid*         ret  = 0;
	
	kdThreadSemWait ( queue->sem );	

	if ( queue->size )
	{
		ret  = queue->front->data;
		node = queue->front;

		if ( queue->front->next == 0 )
		{
			queue->front = 0;
			queue->rear  = 0;
		}
		else
		{
			queue->front	   = queue->front->next;
			queue->front->prev = 0;
		}
		
		kdFree ( node );	

		queue->size--;
	}		

	kdThreadSemPost ( queue->sem );	
	
	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueuePopRear ( XMQueue* queue )
{
	XMQueueNode*    node = 0;
	KDvoid*         ret  = 0;
	
	kdThreadSemWait ( queue->sem );	

	if ( queue->size )
	{
		ret  = queue->rear->data;
		node = queue->rear;

		if ( queue->rear->prev == 0 )
		{
			queue->front = 0;
			queue->rear  = 0;
		}
		else
		{
			queue->rear		  = queue->rear->prev;
			queue->rear->next = 0;
		}
		
		kdFree ( node );	

		queue->size--;
	}		

	kdThreadSemPost ( queue->sem );	
		
	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueuePopFind ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data )
{
	KDvoid*     ret = 0;
	
	kdThreadSemWait ( queue->sem );	
			
	if ( queue->size )
	{				
		queue->find.node = queue->front;

		while ( queue->find.node )
		{
			if ( !func || !func ( queue->find.node->data, data ) )
			{						
				ret = queue->find.node->data;	
				xmQueueNodeErase ( queue, queue->find.node );					
				break;
			}

			queue->find.node = queue->find.node->next;				
		}			
	}		

	kdThreadSemPost ( queue->sem );	

	return ret;
}

KD_API KDsize KD_APIENTRY xmQueueSize ( XMQueue* queue )
{
	return queue->size;
}

KD_API KDvoid* KD_APIENTRY xmQueueGet ( XMQueue* queue, KDuint index )
{
	XMQueueNode*    node = 0;	
	KDuint          i;

	kdThreadSemWait ( queue->sem );	

	if ( index < queue->size  )
	{
		node = queue->front;

		for ( i = 0; i < index; i++ )
		{		
			node = node->next;
		}		
	}

	kdThreadSemPost ( queue->sem );	
	
	return node->data;
}

KD_API KDvoid* KD_APIENTRY xmQueueErase ( XMQueue* queue, KDuint index )
{
	XMQueueNode*    node = 0;	
	KDvoid*         ret  = 0;
	KDuint          i;
	
	kdThreadSemWait ( queue->sem );	

	node = queue->front;

	for ( i = 0; i < index; i++ )
	{		
		node = node->next;
	}

	ret = node->data;
	xmQueueNodeErase ( queue, node );

	kdThreadSemPost ( queue->sem );	

	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueueFind ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data )
{	
	KDvoid*     ret = 0;
	
	//kdThreadSemWait ( queue->sem );	
			
	if ( queue->size )
	{			
		queue->find.dir  = KD_FORWARD;
		queue->find.node = queue->front;	
		queue->find.data = data;
		queue->find.func = func;

		while ( queue->find.node )
		{
			if ( !queue->find.func || !queue->find.func ( queue->find.node->data, queue->find.data ) )
			{	
				ret = queue->find.node->data;	
				break;
			}

			queue->find.node = queue->find.node->next;				
		}			
	}		

	//kdThreadSemPost ( queue->sem );	

	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueueReverseFind ( XMQueue* queue, XMQueueFindFunc* func, KDvoid* data )
{		
	KDvoid*     ret = 0;
	
	kdThreadSemWait ( queue->sem );	

	if ( queue->size )
	{	
		queue->find.dir  = XM_REVERSE;
		queue->find.node = queue->rear;	
		queue->find.data = data;
		queue->find.func = func;

		while ( queue->find.node )
		{
			if ( !queue->find.func || !queue->find.func ( queue->find.node->data, queue->find.data ) )
			{	
				ret = queue->find.node->data;	
				break;
			}

			queue->find.node = queue->find.node->prev;								
		}
	}

	kdThreadSemPost ( queue->sem );	

	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueueFindNext ( XMQueue* queue )
{	
	KDvoid*     ret = 0;

	kdThreadSemWait ( queue->sem );	

	if ( queue->size )
	{	
		if ( queue->find.node )
		{
			queue->find.node = queue->find.dir == KD_FORWARD ? queue->find.node->next : queue->find.node->prev;
		}
		else
		{
			queue->find.node = queue->find.dir == KD_FORWARD ? queue->front : queue->rear;
		}

		while ( queue->find.node ) 
		{		
			if ( !queue->find.func || !queue->find.func ( queue->find.node->data, queue->find.data ) )
			{						
				ret = queue->find.node->data;	
				break;
			}		

			queue->find.node = queue->find.dir == KD_FORWARD ? queue->find.node->next : queue->find.node->prev;					
		} 		
	}

	kdThreadSemPost ( queue->sem );	
	
	return ret;
}

KD_API KDvoid* KD_APIENTRY xmQueueEraseFound ( XMQueue* queue )
{		
	XMQueueNode*    node = 0;
	KDvoid*         ret  = 0;

	kdThreadSemWait ( queue->sem );	

	node = queue->find.node;
	ret  = queue->find.node->data;

	if ( queue->size )
	{
		if ( queue->front == queue->rear )
		{
			queue->front	 = 0;
			queue->rear		 = 0;
			queue->find.node = 0;
		}
		else if ( queue->find.node == queue->front )
		{
			queue->front	   = queue->front->next;
			queue->front->prev = 0;
			queue->find.node   = queue->find.dir == KD_FORWARD ? 0 : queue->front;
		}
		else if ( queue->find.node == queue->rear )
		{
			queue->rear		   = queue->rear->prev;
			queue->rear->next  = 0;
			queue->find.node   = queue->find.dir == XM_REVERSE ? 0 : queue->rear;
		}
		else
		{
			queue->find.node->prev->next = queue->find.node->next;
			queue->find.node->next->prev = queue->find.node->prev;	
			queue->find.node = queue->find.dir == KD_FORWARD ? queue->find.node->prev :queue->find.node->next; 
		}

		queue->size--;
		
		kdFree ( node );							
	}

	kdThreadSemPost ( queue->sem );	
	
	return ret;
}
