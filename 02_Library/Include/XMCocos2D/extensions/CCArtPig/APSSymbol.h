/* --------------------------------------------------------------------------
 *
 *      File            APSSymbol.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSSymbol_h__
#define __APSSymbol_h__

#include "APSGraphicGroup.h"
#include "APSDeviceInfo.h"
#include "APSActionObserver.h"
#include "APSTouchEventObserver.h"
#include "APSTouch.h"
#include "../../actions/CCAction.h"
#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_APS_BEGIN

class APSResource;
class APSAction;
class APSActionGroup;
class APSResourceProvider;
class APSNotificationCenter;
class APSArray;
class APSResourceManager;

typedef std::map<const std::string, std::set<APSActionObserver*> *> APSActionObserverTable;

extern const KDchar*   kAPSDefaultSymbolTag;
extern const KDchar*   kAPSDefaultActionGroupTag;
extern const KDchar*   kAPSAutoTriggerActionGroupTag;

/**
 *	@brief	APSSymbol 
 *	Symbol represents a character such as person or car, and it consists of many
 *	movable parts. Those movable parts are called graphic group because they can
 *	consist of several small graphics. For example, if a symbol represents person, 
 *	it has head, body, arms, legs, and so on. Head is composed of eyes, hair, lips,
 *	and so on, so we call head part a graphic group. 
 *	
 *	In addition, symbol can have actions such as walk, run, sleep, and etc. 
 *	In order to walk, each part ( =graphic group ) of a person should have individual 
 *	small actions, such as moving arms and legs.
 *	Walk action can be called action group in ArtPig system because it involves 
 *	several sub-actions. 
 *
 *	APSSymbol can be created by APSResourceManager or by APSSymbol constructors.
 *	If APSSymbol object has any chance to access images or audio, APSSymbol object
 *	should be created by APSResourceManager, or resourceManager property should be
 *	assigned.
 */
class APSSymbol : public APSGraphicGroup
{	    
	public :

		/** Constructor of APSSymbol. If 'properties' parameter is not NULL, instance is initialized by the properties. */
		APSSymbol ( APSDictionary* pProperties = KD_NULL );
	    
		/** Constructor of APSSymbol with project id and tag. */
		APSSymbol ( const KDchar* szProjectID, const KDchar* szSymbolTag = kAPSDefaultSymbolTag );
	    
		virtual ~APSSymbol ( KDvoid );

		APS_CLASS_NAME ( APSSymbol );

	public :

		/** Returns symbol's filename without extension with given projectId and symbolTag */
		static std::string  getXMLFilename ( const KDchar* szProjectID, const KDchar* szSymbolTag = kAPSDefaultSymbolTag );
	    
		/** Initializes symbol with properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns duration of prologue action groups. */
		KDfloat	 getPrologueDuration ( KDvoid );
	    
		/** Returns a resource at a given index in resources list. If there is no resource with the index, NULL is returned. */
		APSResource*  getResourceAtIndex ( APSIndex uIndex ) const;
	    
		/** Triggers a given targetAction to run. */
		KDvoid  triggerAction ( APSAction* pTargetAction );

		/** Interrupts a given targetAction to stop */
		KDvoid  interruptAction ( APSAction* pTargetAction );
	    
		/** 
		 *	Triggers all of prologue action groups. Although any action can be
		 *	triggered by triggerAction ( ) method, prologue action groups are assumed
		 *	to be triggered before any other action group starts.
		 */
		KDvoid  triggerPrologueActionGroups ( KDvoid );
	    
		/** Triggers action group specified by a given tag. */
		KDvoid  triggerAction ( const std::string& sActionTag );
	        
		/** Interrupts action group specified by a given tag. */
		KDvoid  interruptAction ( const std::string& sActionTag );
	    
		/** Interrupts all running actions in symbol. */
		KDvoid  interruptAllActions ( KDvoid );
	    
		/** Disables action group specified by a given tag. */
		KDvoid  disableAction ( const std::string& sActionTag );

		/** Enables action group specified by a given tag. */
		KDvoid  enableAction ( const std::string& sActionTag );

		/** Initializes CCNode objects in symbol. initNode ( ) can be called multiple
		 times to reset CCNode objects' states back to original.
		 */
		virtual KDbool  initNode ( KDvoid );
	    
		/** Returns a CCLayer object which is the root of graphics tree. */
		virtual CCLayer*  getRootNode ( KDvoid );
	    
		/** Returns an APSResource object for a given tag. */
		APSResource*  getResourceForTag ( const std::string& sTag );

		/** Releases cached data in symbol. */
		virtual KDvoid  clearCachedData ( KDvoid );
	    
		/** Preloads data to perform actions without delay. */
		virtual KDvoid  preloadData ( KDvoid );      

		/** Returns currently running action groups */
		const std::set<APSActionGroup *>&  getRunningActionGroups ( KDvoid ) const;
	    
		/** 
		 *	Fills actions parameter with APSAction objects related to 
		 *	targetGraphicTag and actionTag. The list of actions is sorted by startTime 
		 *	APSAction property.
		 */
		KDvoid  getActions ( std::vector<APSAction*>* pActions, const std::string& sTargetGraphicTag, const std::string& sActionGroupTag );
	    
		/** 
		 *	Fills actions parameter with APSAction objects related to 
		 *	targetGraphic and action. The list of actions is sorted by startTime 
		 *	APSAction property.
		 */
		KDvoid  getActions ( std::vector<APSAction*>* pActions, APSGraphicGroup* pTargetGraphic, APSActionGroup* pActionGroup );
	    
		/**
		 *	Creates an autoreleased CCFiniteTimeAction object, for specific target 
		 *	graphic and action group, which is applicable to none ArtPig layer 
		 *	structures. Target graphic and action group are identified by given 
		 *	targetGraphicTag and actionGroupTag, respectively. 
		 *	Position coordinates are scaled by a given scale value.
		 */
		CCFiniteTimeAction*  universalCCAction ( const std::string& sTargetGraphicTag, const std::string& sActionGroupTag, KDfloat fScale = 1.0f );
	    
		/**
		 *	Creates an autoreleased CCFiniteTimeAction object, for specific target 
		 *	graphic and action group, which is applicable to none ArtPig layer 
		 *	structures.
		 *	Position coordinates are scaled by a given scale value.
		 */
		CCFiniteTimeAction*  universalCCAction ( APSGraphicGroup* pTargetGraphic, APSActionGroup* pActionGroup, KDfloat fScale = 1.0f );
	    
		// #### Events ####
	    
		KDvoid  dispatchTouch ( const APSTouch& tTouch );

		// #### Touch Observers ####
	    
		KDvoid  addTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, APSGraphic* pGraphic );
	    
		KDvoid  addTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, const std::string& sGraphicTag );
	    
		KDvoid  removeTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, APSGraphic* pGraphic );
	    
		KDvoid  removeTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, const std::string& sGraphicTag );
	    
		// #### Action Observers ####
	    
		/** Adds an observer that implements APSActionObserver. The observer is to receive start & stop messages from action specified by a tag. */
		KDvoid  addActionObserver ( APSActionObserver* pObserver, const std::string& sActionTag );
	    
		/** Removes an observer that implements APSActionObserver. */
		KDvoid  removeActionObserver ( APSActionObserver* pObserver, const std::string& sActionTag );
	    
		/** Removes all observation entries for a given observer */
		KDvoid  removeActionObserver ( APSActionObserver* pObserver );
	    
		/** Removes all observation entries in symbol. */
		KDvoid  removeAllActionObservers ( KDvoid );
	    
		/** parent graphic ( APSSymbolSprite ) is assigned when this symbol is used as a child of another symbol. */
		KDvoid  setParent ( APSGraphic* pParent );
	    
	protected :

		KDvoid  initialize ( APSDictionary* pProperties = KD_NULL );
	    
		virtual CCNode*  createNode ( KDvoid );

		/** 
		 *	notifyStart ( ) is called at the beginning of action run. 
		 *	With calling notifyStart ( ) and notifyStop ( ) methods, symbol can keep track 
		 *	of actions' activities.
		 */
		KDvoid  notifyStart ( APSAction* pAction );
	    
		/** notifyStop ( ) is called when an action finishes.
		 With calling notifyStart ( ) and notifyStop ( ) methods, symbol can keep track 
		 of actions' activities.
		 */
		KDvoid  notifyStop ( APSAction* pAction );
	    
		APSGraphic*  searchGraphic ( const CCPoint& tPositionInSymbol, APSEventType uType, KDbool bFilterFirstTouch = KD_TRUE );
		
		APSResourceArray*  getTouchListenersForEventType ( APSEventType uType ) const;

		APSResourceArray*  getTouchListenersForEventTypeWithCreation ( APSEventType uType );
	    
		std::set<APSActionObserver*>*  getActionObservers ( const std::string& sTag, KDbool bCreation = KD_FALSE );

	protected :

		/** If APSSymbol object has any chance to access images or audio, resourceManager property must be assigned. */
		APS_SYNTHESIZE ( APSResourceManager*, m_pResourceManager, ResourceManager );
	    
		/** Background color */
		APS_SYNTHESIZE_WITH_KEY ( ccColor4B, m_tBackgroundColor, BackgroundColor );
	    
		/** YES if this symbol is not a child of another symbol. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bIsRoot, IsRoot );
	    
		APS_SYNTHESIZE_WITH_KEY ( APSOrientation, m_uOrientation, Orientation );
	    
		APS_PROPERTY_KEY ( TouchBeginListeners );
		APS_PROPERTY_KEY ( TouchMovedListeners );
		APS_PROPERTY_KEY ( TouchEndListeners   );
	    
		APS_SYNTHESIZE_READONLY_WITH_KEY ( APSResourceArray*, m_pPrologues, Prologues );
	    
		APS_PROPERTY_KEY ( Resources );
		APS_PROPERTY_KEY ( NResources );

	protected :

		KDfloat										m_fPrologueDuration;
		std::vector<APSResource*>					m_aResources;
		std::set<APSActionGroup*>					m_aRunningActionGroups;
		std::map<const std::string, APSResource*>	m_aTaggedResources;
	    
		/** a map table of observers that receive start & stop message for actions specified by tags. */
		APSActionObserverTable						m_aActionObservers;       
	     
		std::vector<APSResourceArray*>*				m_pTouchListeners;
	    
		APSGraphic*									m_pFirstTouchGraphic;
		APSGraphic*									m_pTouchOwner;
				
		friend class APSAction;
};

typedef std::vector<std::pair<APSActionObserver*, APSAction*> > APSPendingPostStorage;
class APSCCSymbolLayer : public CCLayerColor 
{
	public :

		KDvoid	addFinishedActionObserver ( APSActionObserver* pObserver, APSAction* pAction );

		KDvoid	postAll ( KDvoid );

	protected :

		APSPendingPostStorage						m_aFinishedActions;
};

NS_APS_END

#endif	// __APSSymbol_h__
