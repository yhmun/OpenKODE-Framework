/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventDispatcher.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCEventDispatcher_h__
#define __CCEventDispatcher_h__

#include "../../platform/CCPlatformMacros.h"
#include "CCEventListener.h"
#include "CCEvent.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>

NS_CC_BEGIN

class Event;
class EventTouch;
class Node;

/**
 *	This class manages event listener subscriptions
 *	and event dispatching.
 *
 *	The EventListener list is managed in such a way that
 *	event listeners can be added and removed even
 *	from within an EventListener, while events are being
 *	dispatched.
 */
class EventDispatcher : public Object
{
	public :

		/** 
		 *	Adds a event listener for a specified event with the priority of scene graph.
		 *  @param listener The listener of a specified event.
		 *  @param node The priority of the listener is based on the draw order of this node.
		 *  @note  The priority of scene graph will be fixed value 0. So the order of listener item
		 *         in the vector will be ' <0, scene graph (0 priority), >0'.
		 */
		KDvoid					addEventListenerWithSceneGraphPriority ( EventListener* pListener, Node* pNode );

		/**
		 *	Adds a event listener for a specified event with the fixed priority.
		 *  @param listener The listener of a specified event.
		 *  @param fixedPriority The fixed priority of the listener.
		 *  @note A lower priority will be called before the ones that have a higher value.
		 *        0 priority is forbidden for fixed priority since it's used for scene graph based priority.
		 */
		KDvoid					addEventListenerWithFixedPriority ( EventListener* pListener, KDint nFixedPriority );

		/**
		 *	Remove a listener 
		 *  @param listener The specified event listener which needs to be removed.
		 */
		KDvoid					removeEventListener ( EventListener* pListener );

		/** Removes all listeners with the same event listener type */
		KDvoid					removeEventListeners ( EventListener::Type eListenerType );
    
		/** Removes all custom listeners with the same event name */
		KDvoid					removeCustomEventListeners ( const std::string& sCustomEventName );
    
		/** Removes all listeners */
		KDvoid					removeAllEventListeners ( KDvoid );

		/** Sets listener's priority with fixed value. */
		KDvoid					setPriority ( EventListener* pListener, KDint nFixedPriority );

		/** Whether to enable dispatching events */
		KDvoid					setEnabled ( KDbool bIsEnabled );

		/** Checks whether dispatching events is enabled */
		KDbool					isEnabled ( KDvoid ) const;

		/**
		 *	Dispatches the event
		 *  Also removes all EventListeners marked for deletion from the
		 *  event dispatcher list.
		 */
		KDvoid					dispatchEvent ( Event* pEvent );
    
		/** Constructor of EventDispatcher */
		EventDispatcher ( KDvoid );

		/** Destructor of EventDispatcher */
		~EventDispatcher ( KDvoid );

	private :

		friend class Node;
    
		/** Sets the dirty flag for a node. */
		KDvoid					setDirtyForNode ( Node* pNode );
    
		/** Notifys event dispatcher that the node has been paused. */
		KDvoid					pauseTarget ( Node* pNode );
    
		/** Notifys event dispatcher that the node has been resumed. */
		KDvoid					resumeTarget ( Node* pNode );
    
		/** Notifys event dispatcher that the node has been deleted. */
		KDvoid					cleanTarget ( Node* pNode );
    
		/**
		 *  The vector to store event listeners with scene graph based priority and fixed priority.
		 */
		class EventListenerVector
		{
			public :

				EventListenerVector ( KDvoid );
				~EventListenerVector ( KDvoid );

				KDsize									size ( KDvoid ) const;
				KDbool									empty ( KDvoid ) const;
        
				KDvoid									push_back ( EventListener* pItem );
				KDvoid									clearSceneGraphListeners ( KDvoid );
				KDvoid									clearFixedListeners ( KDvoid );
				KDvoid									clear ( KDvoid );
        
				inline std::vector<EventListener*>*		getFixedPriorityListeners ( KDvoid ) const { return m_pFixedListeners; }
				inline std::vector<EventListener*>*		getSceneGraphPriorityListeners ( KDvoid ) const { return m_pSceneGraphListeners; }
				inline KDint32							getGt0Index ( KDvoid ) const { return m_nGt0Index; }
				inline KDvoid							setGt0Index ( KDint32 nIndex ) { m_nGt0Index = nIndex; }

			private :

				std::vector<EventListener*>*			m_pFixedListeners;
				std::vector<EventListener*>*			m_pSceneGraphListeners;
				KDint32									m_nGt0Index;	
		};
    
		/** Adds event listener with item */
		KDvoid					addEventListener ( EventListener* pListener );
    
		/** Gets event the listener list for the event listener type. */
		EventListenerVector*	getListeners ( EventListener::ListenerID eListenerID );
    
		/** Update dirty flag */
		KDvoid					updateDirtyFlagForSceneGraph ( KDvoid );
    
		/** Removes all listeners with the same event listener ID */
		KDvoid					removeEventListenersForListenerID ( EventListener::ListenerID eListenerID );
    
		/** Sort event listener */
		KDvoid					sortEventListeners ( EventListener::ListenerID eListenerID );
    
		/** Sorts the listeners of specified type by scene graph priority */
		KDvoid					sortEventListenersOfSceneGraphPriority ( EventListener::ListenerID eListenerID );
    
		/** Sorts the listeners of specified type by fixed priority */
		KDvoid					sortEventListenersOfFixedPriority ( EventListener::ListenerID eListenerID );
    
		/** 
		 *	Updates all listeners
		 *		1) Removes all listener items that have been marked as 'removed' when dispatching event.
		 *		2) Adds all listener items that have been marked as 'added' when dispatching event.
		 */
		KDvoid					updateListeners ( Event* pEvent );

		/** Touch event needs to be processed different with other events since it needs support ALL_AT_ONCE and ONE_BY_NONE mode. */
		KDvoid					dispatchTouchEvent ( EventTouch* pEvent );
    
		/** Associates node with event listener */
		KDvoid					associateNodeAndEventListener ( Node* pNode, EventListener* pListener );
    
		/** Dissociates node with event listener */
		KDvoid					dissociateNodeAndEventListener ( Node* pNode, EventListener* pListener );
    
		/** Dispatches event to listeners with a specified listener type */
		KDvoid					dispatchEventToListeners ( EventListenerVector* pListeners, std::function<KDbool(EventListener*)> onEvent );
    
		/// Priority dirty flag
		enum class DirtyFlag
		{
			NONE = 0,
			FIXED_PRITORY = 1 << 0,
			SCENE_GRAPH_PRIORITY = 1 << 1,
			ALL = FIXED_PRITORY | SCENE_GRAPH_PRIORITY
		};
    
		/** Sets the dirty flag for a specified listener ID */
		KDvoid					setDirty ( EventListener::ListenerID eListenerID, DirtyFlag eFlag );
    
		/** Walks though scene graph to get the draw order for each node, it's called before sorting event listener with scene graph priority */
		KDvoid					visitTarget ( Node* pNode );
    
	private :

		/** Listeners map */
		std::unordered_map<EventListener::ListenerID, EventListenerVector*>		m_aListeners;
    
		/** The map of dirty flag */
		std::unordered_map<EventListener::ListenerID, DirtyFlag>				m_aPriorityDirtyFlagMap;
    
		/** The map of node and event listeners */
		std::unordered_map<Node*, std::vector<EventListener*>*>					m_aNodeListenersMap;
    
		/** The map of node and its event priority */
		std::unordered_map<Node*, KDint>										m_aNodePriorityMap;
    
		/** The listeners to be added after dispatching event */
		std::vector<EventListener*>												m_aToAddedListeners;
    
		/** The nodes were associated with scene graph based priority listeners */
		std::set<Node*>															m_aDirtyNodes;
    
		/** Whether the dispatcher is dispatching event */
		KDint																	m_nInDispatch;
    
		/** Whether to enable dispatching event */
		KDbool																	m_bIsEnabled;
    
		KDint																	m_nNodePriorityIndex;
};

NS_CC_END

#endif	// __CCEventDispatcher_h__
