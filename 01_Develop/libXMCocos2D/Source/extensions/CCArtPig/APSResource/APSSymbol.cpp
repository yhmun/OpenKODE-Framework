/* --------------------------------------------------------------------------
 *
 *      File            APSSymbol.cpp
 *      Author          Y.H Mun
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

#include "Precompiled.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSActionGroup.h"
#include "extensions/CCArtPig/APSGraphic.h"
#include "extensions/CCArtPig/APSElements.h"
#include "extensions/CCArtPig/APSXMLParser.h"
#include "extensions/CCArtPig/APSAudioHolder.h"
#include "extensions/CCArtPig/APSImageHolder.h"
#include "extensions/CCArtPig/APSXMLParser.h"
#include "extensions/CCArtPig/APSResourceManager.h"
#include "extensions/CCArtPig/APSGraphicAction.h"
#include "extensions/CCArtPig/APSLinkerAction.h"
#include "extensions/CCArtPig/APSTouchEventNotificationLinker.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "support/CCFileUtils.h"

NS_APS_BEGIN

const KDchar*  kAPSDefaultSymbolTag				= "Symbol1";
const KDchar*  kAPSDefaultActionGroupTag		= "ActionGroup1";
const KDchar*  kAPSAutoTriggerActionGroupTag	= "AutoTriggerActionGroup";
const KDbool   kAPSSymbol_defaultIsRoot			= KD_TRUE;

const std::string& APSSymbol::getBackgroundColorKey ( KDvoid )
{
    static const std::string  sKey = "bgColor";
    return sKey;
}

const std::string& APSSymbol::getProloguesKey ( KDvoid )
{
    static const std::string  sKey = "prologues";
    return sKey;
}

const std::string& APSSymbol::getIsRootKey ( KDvoid )
{
    static const std::string  sKey = "isRoot";
    return sKey;
}

const std::string& APSSymbol::getOrientationKey ( KDvoid )
{
    static const std::string  sKey = "orientation";
    return sKey;
}

const std::string& APSSymbol::getResourcesKey ( KDvoid )
{
    static const std::string  sKey = "resources";
    return sKey;
}

const std::string& APSSymbol::getNResourcesKey ( KDvoid )
{
    static const std::string  sKey = "nResources";
    return sKey;
}

const std::string& APSSymbol::getTouchBeginListenersKey ( KDvoid )
{
    static const std::string  sKey = "tBeginLns";
    return sKey;
}

const std::string& APSSymbol::getTouchMovedListenersKey ( KDvoid )
{
    static const std::string  sKey = "tMovedLns";
    return sKey;
}

const std::string& APSSymbol::getTouchEndListenersKey ( KDvoid )
{
    static const std::string  sKey = "tEndLns";
    return sKey;
}


APSSymbol::APSSymbol ( APSDictionary* pProperties )
{
	m_pResourceManager	= KD_NULL;
	m_tBackgroundColor  = ccc4 ( 0, 0, 0, 0 );
	m_fPrologueDuration	= -1;
	m_pTouchListeners	= KD_NULL;
	m_bIsRoot			= kAPSSymbol_defaultIsRoot;
	m_pTouchOwner		= KD_NULL;

    this->initialize ( pProperties );
}

APSSymbol::APSSymbol ( const KDchar* szProjectId, const KDchar* szSymbolTag )
{
	m_pResourceManager	= KD_NULL;
	m_tBackgroundColor	= ccc4 ( 0, 0, 0, 0 );
	m_fPrologueDuration = -1;
	m_pTouchListeners	= KD_NULL;
	m_bIsRoot			= kAPSSymbol_defaultIsRoot;
	m_uOrientation		= kAPSOrientationLandscapeRight;
	m_pTouchOwner		= KD_NULL;

    this->initialize ( );
    
	std::string  sFileName = APSSymbol::getXMLFilename ( szProjectId, szSymbolTag );
	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sFileName.c_str ( ) );
    
    APSXMLParser*  pParser = new APSXMLParser ( );
    if ( pParser->loadFile ( sPath.c_str ( ) ) )
	{
        APSDictionary*  pSymProp = pParser->getDictionary ( );
        if ( pSymProp )
		{
            this->initWithProperties ( pSymProp );
        }
    }
    APS_SAFE_DELETE ( pParser );
}

KDvoid APSSymbol::initialize ( APSDictionary* pProperties )
{
    this->setParentSymbol ( this );
    
    m_uIndex = 0;

    m_pPrologues = new APSResourceArray ( this );
            
    m_aResources.resize ( APS_PREDEFINED_RESOURCE_INDEXES_LENGTH );
    
    // first item is reserved for this(APSSymbol)
    m_aResources [ kAPSSymbolIndex ] = this;
	if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSSymbol::~APSSymbol ( KDvoid )
{   
    if ( m_pTouchListeners )
	{
		APS_FOREACH ( std::vector<APSResourceArray*>, *m_pTouchListeners, iter )
		{
            APS_SAFE_DELETE ( *iter );
        }
        APS_SAFE_DELETE ( m_pTouchListeners );
    }

    APS_SAFE_DELETE(m_pPrologues)
    
    this->interruptAllActions ( );
    this->removeAllActionObservers ( );
    
    KDbool  bFirst = KD_TRUE;
	APS_FOREACH ( std::vector<APSResource*>, this->m_aResources, iter )
	{
        if ( bFirst )
		{
            bFirst = KD_FALSE;
        } 
		else 
		{
            APS_SAFE_DELETE ( (*iter) );
        }
    }
}

std::string APSSymbol::getXMLFilename ( const KDchar* szProjectId, const KDchar* szSymbolTag )
{
    //  szProjectId + "_SYM_" + szSymbolTag + ".xml";
	std::string  sStr;
    sStr.reserve ( kdStrlen ( szProjectId ) + kdStrlen ( szSymbolTag ) + 10 );
    sStr.append ( szProjectId );
    sStr.append ( "_SYM_" );
    sStr.append ( szSymbolTag );
    sStr.append ( ".xml" );
    return sStr;
    
}

KDbool APSSymbol::initWithProperties ( APSDictionary* pProperties )
{
    KDbool  bOk = KD_TRUE;
        
    // set size
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getNResourcesKey ( ) );
    this->m_aResources.resize ( pNumber->getUnsignedInteger ( ) );
    
    if ( !APSGraphicGroup::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
    }
    
    APSString*  pBackgroundColor = (APSString*) pProperties->getObjectForKey ( this->getBackgroundColorKey ( ) );
    if ( pBackgroundColor ) 
	{
        this->setBackgroundColor ( pBackgroundColor->getColorValue ( ) );
    }
    
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIsRootKey ( ) );
    if ( pNumber )
	{
        this->setIsRoot ( pNumber->getBoolean ( ) );
    }

    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getOrientationKey ( ) );
    if ( pNumber ) 
	{
        this->setOrientation ( pNumber->getInteger ( ) );
    }

    // set resources
    APSArray*  pArray = (APSArray*) pProperties->getObjectForKey ( this->getResourcesKey ( ) );
    if ( pArray ) 
	{      
        KDbool  bFirst = KD_TRUE;
        APS_FOREACH ( APSArrayStorage, *pArray, iter ) 
		{            
            if ( bFirst )
			{
                // first resource item is reserved for this APSSymbol.
                bFirst = KD_FALSE;
            }
			else 
			{                
                // Allocate a resource object with the class name 
                APSResource*  pRS = APSResourceManager::newResourceWithProperties ( (APSDictionary*)* iter, this );
                if ( pRS ) 
				{                    
                    this->m_aResources [ pRS->getIndex ( ) ] = pRS;
                    
                    // If the resource has a tag, insert it into taggedResources table.
                    if ( pRS->getTag ( ) )
					{
                        this->m_aTaggedResources [ pRS->getTag ( ) ] = pRS;
                    }
                }
            }
        }
    }
	else
	{
        bOk = KD_FALSE;
	}
    
    if ( bOk )
	{
        APSString*  pIndexes = (APSString*) pProperties->getObjectForKey ( this->getProloguesKey ( ) );
        pIndexes->getResourceArray ( this->m_pPrologues );
        
        //
        // Set touch listeners
        //
        APSString*  pListeners = (APSString*) pProperties->getObjectForKey ( this->getTouchBeginListenersKey ( ) );
        if ( pListeners )
		{
            pListeners->getResourceArray ( this->getTouchListenersForEventTypeWithCreation ( kAPSEventTypeTouchBegan ) );
        }
        
        pListeners = (APSString*) pProperties->getObjectForKey ( this->getTouchMovedListenersKey());
        if ( pListeners )
		{
            pListeners->getResourceArray ( this->getTouchListenersForEventTypeWithCreation ( kAPSEventTypeTouchMoved ) );
        }
        
        pListeners = (APSString*) pProperties->getObjectForKey ( this->getTouchEndListenersKey ( ) );
        if ( pListeners ) 
		{
            pListeners->getResourceArray ( this->getTouchListenersForEventTypeWithCreation ( kAPSEventTypeTouchEnded ) );
        }       
    } 
	else 
	{
        APSLOG ( "Failed initializing APSSymbol." );
    }

    return bOk;
}

KDfloat APSSymbol::getPrologueDuration ( KDvoid )
{
    if ( this->m_fPrologueDuration < 0 )
	{
		this->m_fPrologueDuration = 0;
		this->getPrologues ( )->preloadAll ( );
		APS_FOREACH ( APSResourceArray, *this->getPrologues ( ), iter )
		{
            this->m_fPrologueDuration = KD_MAX ( m_fPrologueDuration, ( (APSActionGroup*) iter->second )->getDuration ( ) );
        }
    }
    return this->m_fPrologueDuration;
}

APSResource* APSSymbol::getResourceAtIndex ( APSIndex uIndex ) const
{
    if ( this->m_aResources.size ( ) > uIndex )
	{
        return this->m_aResources [ uIndex ];
    }
    return KD_NULL;
}

KDvoid APSSymbol::clearCachedData ( KDvoid ) 
{
	APS_FOREACH ( std::vector<APSResource*>, this->m_aResources, iter )
	{
        if ( ( *iter ) && ( *iter ) != this ) 
		{
             ( *iter )->clearCachedData ( );
        }
    }
}

KDvoid APSSymbol::preloadData ( KDvoid )
{
	APS_FOREACH ( std::vector<APSResource*>, this->m_aResources, iter )
	{
        if ( ( *iter ) && ( *iter ) != this )
		{
             ( *iter )->preloadData();
        }
    }
}

KDvoid APSSymbol::triggerAction ( APSAction* pTargetAction )
{
    // An action can be triggered only if it is enabled.
    if ( pTargetAction ) 
	{
        APSGraphicGroup*  pGraphic = pTargetAction->getTargetGraphic ( );
        if ( !pGraphic )
		{
            pGraphic = this;
        }
                
        KDbool  bShouldTrigger = KD_TRUE;
        
        if ( pTargetAction->getEnabled ( ) )
		{
            KDint  nType = pTargetAction->getActionType ( );
            if ( nType == kAPSActionTypeGroup )
			{
                // The same action group is already running, so interrupt it to restart.
				std::set<APSActionGroup*>::iterator  it = this->m_aRunningActionGroups.find ( (APSActionGroup*) pTargetAction );
                if ( it != this->m_aRunningActionGroups.end ( ) )
				{
                    pTargetAction->interrupt ( );
                }
            } 
			else 
			{
                APSAction*  pOldAction = pGraphic->getRunningActionForType ( nType );
                // If target graphic is running for an action that has higher priority than 'targetAction', then 'targetAction' cannot be triggered.
                if ( pOldAction ) 
				{
                    if ( pOldAction->getActionGroup ( ) == pTargetAction->getActionGroup ( ) )
					{
                        // Complete the old action.
                        pOldAction->getCCAction ( )->update ( 1.f );
                    }
					else 
					{
                        if ( pOldAction->getPriority ( ) > pTargetAction->getPriority ( ) )
						{
                            bShouldTrigger = KD_FALSE;
                        }
						else 
						{
                            // Before triggering, force to stop the currently running action.
                            APSActionGroup*  pActionGroup = pOldAction->getActionGroup ( );
                            if ( pActionGroup && pActionGroup->getIntact ( ) ) 
							{
                                // If target action's group is intact, stop all actions in the group.
                                pActionGroup->interrupt ( );
                            } 
							else 
							{
                                pOldAction->interrupt ( );
                            }
                        }
                    }
                }
            }
        }
		else 
		{
            bShouldTrigger = KD_FALSE;
        }
        
        if ( bShouldTrigger ) 
		{
            pTargetAction->_trigger ( );
        }
		else 
		{
            APSActionGroup*  pActionGroup = pTargetAction->getActionGroup ( );
            if ( pActionGroup )
			{
                if ( pTargetAction->getEnabled ( ) && pActionGroup->getIntact ( ) )
				{
                    // If target action is enabled but cannot run, all actions in the action group are interrupted.
                    pActionGroup->interrupt ( );
                } else 
				{
                    // Since action is never triggered, decreaseRunningActionCount should be called here.
                    pActionGroup->decreaseRunningActionCount ( );
                }
            }
        }
    }
}

KDvoid APSSymbol::interruptAction ( APSAction* pTargetAction )
{
    pTargetAction->_interrupt ( );
}

KDvoid APSSymbol::interruptAllActions ( KDvoid ) 
{
    // send interrupt messages to all running actions. !!!Caution: we cannot use fast iteration because the set is chaning by interrupt actions.
    while ( this->m_aRunningActionGroups.size ( ) > 0 )
	{
        this->interruptAction ( *this->m_aRunningActionGroups.begin ( ) );
    }
}

CCLayer* APSSymbol::getRootNode ( KDvoid ) 
{
    return (CCLayer*) APSGraphic::getNode ( );
}

APSResource* APSSymbol::getResourceForTag ( const std::string& sTag )
{
    return this->m_aTaggedResources [ sTag ];
}

KDvoid APSSymbol::getActions ( std::vector<APSAction*>* pActions, const std::string& sTargetGraphicTag, const std::string& sActionGroupTag )
{
    APSActionGroup*   pActionGroup = (APSActionGroup*)  this->getResourceForTag ( sActionGroupTag );
    APSGraphicGroup*  pTarget = (APSGraphicGroup*) this->getResourceForTag ( sTargetGraphicTag );
    this->getActions ( pActions, pTarget, pActionGroup );
}

KDvoid APSSymbol::getActions ( std::vector<APSAction*>* pActions, APSGraphicGroup* pTargetGraphic, APSActionGroup* pActionGroup )
{
    if ( pActions && pActionGroup && pTargetGraphic ) 
	{
        pActionGroup->getActionsForTargetGraphic ( pActions, pTargetGraphic );
    }
}

CCFiniteTimeAction* APSSymbol::universalCCAction ( const std::string& sTargetGraphicTag, const std::string& sActionGroupTag, KDfloat fScale )
{
    APSGraphicGroup*  pGraphic = (APSGraphicGroup*) this->getResourceForTag ( sTargetGraphicTag );
    APSActionGroup*   pActionGroup = (APSActionGroup*) this->getResourceForTag ( sActionGroupTag );
    
    return this->universalCCAction ( pGraphic, pActionGroup );
}

CCFiniteTimeAction* APSSymbol::universalCCAction ( APSGraphicGroup* pTargetGraphic, APSActionGroup* pActionGroup, KDfloat fScale )
{
	std::vector<APSAction*>  aActions;
    this->getActions ( &aActions, pTargetGraphic, pActionGroup );
    
    KDbool  bFirst = KD_TRUE;
    CCFiniteTimeAction*  pSpawn = KD_NULL;

	APS_FOREACH ( std::vector<APSAction*>, aActions, iter) 
	{
        APSGraphicAction*  pGraphicAction = dynamic_cast<APSGraphicAction*> ( *iter );
        if ( pGraphicAction ) 
		{
            CCFiniteTimeAction*  pAction = pGraphicAction->universalCCAction ( fScale );
            if ( pAction ) 
			{
                if (!isZeroF ( pGraphicAction->getStartTime ( ) ) ) 
				{
                    pAction = CCSequence::createWithTwoActions ( CCDelayTime::create ( pGraphicAction->getStartTime ( ) ), pAction );
                }
                
                if ( bFirst )
				{
                    pSpawn = pAction;
                    bFirst = KD_FALSE;
                }
				else 
				{
                    pSpawn = CCSpawn::createWithTwoActions ( pSpawn, pAction );
                }
            }
        }
    }

    return pSpawn;
}

APSResourceArray* APSSymbol::getTouchListenersForEventType ( APSEventType uType ) const
{
    if ( !m_pTouchListeners )
	{
        return KD_NULL;
    }
    
    switch ( uType ) 
	{
        case kAPSEventTypeTouchBegan : return m_pTouchListeners->at ( 0 );
        case kAPSEventTypeTouchMoved : return m_pTouchListeners->at ( 1 );
        case kAPSEventTypeTouchEnded : return m_pTouchListeners->at ( 2 );
    }
    
    return KD_NULL;
}

APSResourceArray* APSSymbol::getTouchListenersForEventTypeWithCreation ( APSEventType uType )
{
	std::vector<APSResourceArray*>*  pListeners = m_pTouchListeners;
    
    if ( !pListeners )
	{
		pListeners = m_pTouchListeners = new std::vector<APSResourceArray*> ( 4, (APSResourceArray*) KD_NULL );
    }
    
    KDuint  uPos;
    
	switch ( uType ) 
	{
        case kAPSEventTypeTouchBegan :	uPos = 0;	break;
        case kAPSEventTypeTouchMoved :	uPos = 1;	break;
        case kAPSEventTypeTouchEnded :	uPos = 2;	break;
        default :	return KD_NULL;
    }
    
    if ( !pListeners->at ( uPos ) ) 
	{
        ( *pListeners ) [ uPos ] = new APSResourceArray ( this );
    }
    return pListeners->at ( uPos );
}

APSGraphic* APSSymbol::searchGraphic ( const CCPoint& tPositionInSymbol, APSEventType uType, KDbool bFilterFirstTouch )
{
    APSResourceArray*  pListeners = this->getTouchListenersForEventType ( uType );
    if ( !pListeners ) 
	{
        return KD_NULL;
    }
    
    pListeners->preloadAll ( );
    
    APS_FOREACH ( APSResourceArrayStorage, *pListeners, iter )
	{
        APSGraphic*  pGraphic = (APSGraphic*) iter->second;
        APSResourceArray*  pLinkers = pGraphic->getLinkersForEvent ( uType );
        if ( pLinkers && pLinkers->size ( ) > 0 )
		{
            if ( bFilterFirstTouch && uType != kAPSEventTypeTouchBegan && !pGraphic->getAcceptsTouchMoveEnter ( ) && m_pFirstTouchGraphic != pGraphic ) 
			{
                // Graphic does not accept touches that has begun external position.
                continue;
            }
            
            if ( pGraphic->getAcceptsTouchOnlyVisible ( ) && !pGraphic->isNodeVisible ( ) )
			{
                continue;
            }
            
            // Calculate if the graphic is touched.
            CCAffineTransform   t = pGraphic->getSymbolToNodeTransform ( );
            CCPoint  tPositionInGraphic = CCPointApplyAffineTransform ( tPositionInSymbol, t );
            
            CCRect  tRect;
            tRect.origin = CCPointZero;
            tRect.size = pGraphic->getSize ( );

			KDbool  bContainsPoint = tRect.containsPoint ( tPositionInGraphic );            
            if ( bContainsPoint )
			{
                return pGraphic;
            }
        }
    }
    
    return KD_NULL;
}

KDvoid APSSymbol::dispatchTouch ( const APSTouch& tTouch )
{
    if ( tTouch.getType ( ) == kAPSEventTypeTouchBegan )
	{
        this->m_pTouchOwner = KD_NULL;
    }

    APSGraphic*  pTouchedGraphic = this->m_pTouchOwner;
    
    // If there is no touch owner, find new owner.
    if ( !pTouchedGraphic )
	{
        CCPoint  tPositionInSymbol = this->convertPositionFromParentNode ( tTouch.getPosition ( ) );
        
        pTouchedGraphic = this->searchGraphic ( tPositionInSymbol, tTouch.getType ( ) );
        
        if ( tTouch.getType ( ) == kAPSEventTypeTouchBegan )
		{
            if ( pTouchedGraphic )
			{
                this->m_pFirstTouchGraphic = pTouchedGraphic;
            }
			else 
			{
                // Since touched graphic for touch begin type is not found, search for the other types.
                APSGraphic*  pGraphic = KD_NULL;
                pGraphic = this->searchGraphic ( tPositionInSymbol, kAPSEventTypeTouchMoved, KD_FALSE );
                if ( !pGraphic )
				{
                    pGraphic = this->searchGraphic ( tPositionInSymbol, kAPSEventTypeTouchEnded, KD_FALSE );         
                }
                this->m_pFirstTouchGraphic = pGraphic;
            }
        }
     }

    // Touch owner has been found.
    if ( pTouchedGraphic )
	{
        APSResourceArray*  pLinkers = pTouchedGraphic->getLinkersForEvent ( tTouch.getType ( ) );
        if ( pLinkers )
		{
            pLinkers->preloadAll ( );
            
            // Trigger linker actions in the target graphic.
            APS_FOREACH ( APSResourceArray, *pLinkers, iter2 )
			{
                APSLinkerAction*  pAction = (APSLinkerAction*) iter2->second;
                if ( pAction->getType ( ) == kAPSLinkerTypeNotifyEvent )
				{
                    // Notify touch events to the registered observers.
                    APSTouchEventNotificationLinker*  pNotifier = (APSTouchEventNotificationLinker*) pAction;
                    switch ( tTouch.getType ( ) )
					{
                        case kAPSEventTypeTouchBegan :
                            pNotifier->getObserver ( )->touchBegan ( pTouchedGraphic, tTouch );
                            break;
                        case kAPSEventTypeTouchMoved :
                            pNotifier->getObserver ( )->touchMoved ( pTouchedGraphic, tTouch );
                            break;
                        case kAPSEventTypeTouchEnded :
                            pNotifier->getObserver ( )->touchEnded ( pTouchedGraphic, tTouch );
                            break;     
                    }
                } 
				else
				{
                    if ( !pAction->getRunning ( ) )
					{
                        this->triggerAction ( pAction );
                    }
                }
            }
        }
        
        if ( tTouch.getType ( ) == kAPSEventTypeTouchBegan )
		{
            if ( pTouchedGraphic->getRetainsTouch ( ) ) 
			{
                this->m_pTouchOwner = pTouchedGraphic;
            } 
			else 
			{
                this->m_pTouchOwner = KD_NULL;
            }
        }
    } 
	else
	{
        this->m_pTouchOwner = KD_NULL;
    }
    
    switch ( tTouch.getType ( ) )
	{
        case kAPSEventTypeTouchEnded :
            this->m_pTouchOwner = this->m_pFirstTouchGraphic = KD_NULL;
    }
}

//
// #### Protected ####
//
KDvoid APSSymbol::notifyStart ( APSAction* pAction )
{
    if ( pAction )
	{
        // Notify start to observers
        const KDchar*  szTag = pAction->getTag ( );
        if ( *szTag ) 
		{
			std::set<APSActionObserver*>*  pObservers = this->getActionObservers ( szTag );
            if ( pObservers )
			{
				APS_FOREACH ( std::set<APSActionObserver*>, *pObservers, iter )
				{
                    ( *iter )->willStartAction ( pAction );
                }
            }
        }

        if ( pAction->getActionType ( ) == kAPSActionTypeGroup )
		{        
            this->m_aRunningActionGroups.insert ( (APSActionGroup*) pAction );
        }
    }
}

KDvoid APSSymbol::notifyStop ( APSAction* pAction )
{
    if ( pAction )
	{
        if ( pAction->getActionType ( ) == kAPSActionTypeGroup )
		{
            this->m_aRunningActionGroups.erase ( (APSActionGroup*) pAction );
        }
        
        // notify observers for 'didFinishAction'
        const KDchar*  szTag = pAction->getTag ( );
        if ( *szTag ) 
		{
			std::set<APSActionObserver*>*  pObservers = this->getActionObservers ( szTag );
            if ( pObservers )
			{
				APS_FOREACH ( std::set<APSActionObserver*>, *pObservers, iter )
				{
                    ( (APSCCSymbolLayer*) this->m_pNode )->addFinishedActionObserver ( *iter, pAction );
                }
            }
        }
        
        if ( pAction->getActionType ( ) != kAPSActionTypeGroup )
		{
            APSActionGroup*  pActionGroup = pAction->getActionGroup ( );
            if ( pActionGroup )
			{
                pActionGroup->decreaseRunningActionCount ( );
            }
        }
    }
}

CCNode* APSSymbol::createNode ( KDvoid )
{
    if ( !m_pNode )
	{
        m_pNode = new APSCCSymbolLayer ( );
        ( (APSCCSymbolLayer*) m_pNode )->initWithColor ( this->getBackgroundColor ( ) );
    }

    APSGraphicGroup::createNode ( );
    
    return m_pNode;
}

KDbool APSSymbol::initNode ( KDvoid )
{
    if ( APSGraphicGroup::initNode ( ) )
	{
        m_pNode->ignoreAnchorPointForPosition ( KD_FALSE );

        // Update decendant graphics' opacities.
        this->updateAccOpacity ( );
        
        return KD_TRUE;
    }
    return KD_FALSE;
}

KDvoid APSSymbol::triggerPrologueActionGroups ( KDvoid )
{    
    this->getPrologues ( )->preloadAll ( );
    APS_FOREACH ( APSResourceArray, *this->getPrologues ( ), iter )
	{
        this->triggerAction ( (APSAction*) iter->second );
    }
}

KDvoid APSSymbol::triggerAction ( const std::string& sTag ) 
{
    this->triggerAction ( (APSActionGroup*) this->m_aTaggedResources [ sTag ] );
}

KDvoid APSSymbol::interruptAction ( const std::string& sTag )
{
    this->interruptAction ( (APSAction*) this->m_aTaggedResources [ sTag ] );
}

KDvoid APSSymbol::enableAction ( const std::string& sTag )
{
    APSAction*  pAction = (APSAction*) this->m_aTaggedResources [ sTag ];
    if ( pAction )
	{
        pAction->setEnabled ( KD_TRUE );
    }
}

KDvoid APSSymbol::disableAction ( const std::string& sTag )
{
    APSActionGroup*  pAction = (APSActionGroup*) this->m_aTaggedResources [ sTag ];
    if ( pAction )
	{
        pAction->setEnabled ( KD_FALSE );
    }
}

const std::set<APSActionGroup*>& APSSymbol::getRunningActionGroups ( KDvoid ) const
{
    return this->m_aRunningActionGroups;
}


//
// #### Touch Observers ####
//
KDvoid APSSymbol::addTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, APSGraphic* pGraphic )
{
    // Obtain the array of linker actions to be triggered by touch event.
    APSResourceArray*  pLinkers = pGraphic->getLinkersForEventWithCreation ( uEventType );
    
    // When the touch event occurs, linker action is triggered to notify to touch event observers.
    APSTouchEventNotificationLinker*  pLinkerAction = new APSTouchEventNotificationLinker ( );
    pLinkerAction->setParentSymbol ( this );
    pLinkerAction->setType ( kAPSLinkerTypeNotifyEvent );
    pLinkerAction->setTargetActionIndex ( pGraphic->getIndex ( ) );
    pLinkerAction->setObserver ( pObserver );
    
    // Add the linker in symbol's resources.
    pLinkerAction->setIndex ( (KDuint) this->m_aResources.size ( ) );
    this->m_aResources.push_back ( pLinkerAction );
    
    // Add the linker in event array of graphic.
    pLinkers->pushBack ( pLinkerAction );
    
    APSResourceArray*  pListeners = this->getTouchListenersForEventTypeWithCreation ( uEventType );
    
    // Register the graphic as a touch listener.
    if ( pListeners->getIndexOfApsIndex ( pGraphic->getIndex ( ) ) == APS_NIL_INDEX )
	{
        KDbool  bInserted = KD_FALSE;
        pListeners->preloadAll ( );
        
        APS_FOREACH ( APSResourceArray, *pListeners, iter )
		{
            if ( ( (APSGraphic*) iter->second )->isAbove ( pGraphic ) )
			{
                bInserted = KD_TRUE;
				pListeners->insert ( iter, std::pair<APSIndex, APSResource*> ( pGraphic->getIndex ( ), pGraphic ) );
                break;
            }
        }
        
        if ( !bInserted ) 
		{
            pListeners->pushBack ( pGraphic );
        }
    }
}

KDvoid APSSymbol::addTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, const std::string& sGraphicTag )
{
    // Aquire graphic from the tag.
    APSGraphic*  pGraphic = dynamic_cast<APSGraphic*> ( this->getResourceForTag ( sGraphicTag ) );
    if ( pGraphic )
	{
        this->addTouchObserver ( pObserver, uEventType, pGraphic );
    }
}

KDvoid APSSymbol::removeTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, APSGraphic* pGraphic )
{
    APSResourceArray*  pLinkers = pGraphic->getLinkersForEvent ( uEventType );
    if ( pLinkers )
	{
        pLinkers->preloadAll ( );
        APS_FOREACH ( APSResourceArray, *pLinkers, iter )
		{
            if ( ( (APSLinkerAction*) this->m_aResources [ iter->first ] )->getType ( ) == kAPSLinkerTypeNotifyEvent )
			{
                APS_SAFE_DELETE ( this->m_aResources [ iter->first ] );
                this->m_aResources [ iter->first ] = KD_NULL;
            }
        }
        
        pLinkers->clear ( );
    }
}

KDvoid APSSymbol::removeTouchObserver ( APSTouchEventObserver* pObserver, APSEventType uEventType, const std::string& sGraphicTag )
{
    // Aquire graphic from the tag.
    APSGraphic*  pGraphic = dynamic_cast<APSGraphic*> ( this->getResourceForTag ( sGraphicTag ) );
    if ( pGraphic )
	{
        this->removeTouchObserver ( pObserver, uEventType, pGraphic );
    }
}

KDvoid APSSymbol::addActionObserver ( APSActionObserver* pObserver, const std::string& sActionTag )
{
    if ( pObserver )
	{
        this->getActionObservers ( sActionTag, KD_TRUE )->insert ( pObserver );
    }
}

KDvoid APSSymbol::removeActionObserver ( APSActionObserver* pObserver, const std::string& sActionTag )
{
    if ( pObserver ) 
	{
		std::set<APSActionObserver*>*  pObserserSet = this->getActionObservers ( sActionTag );
        if ( pObserserSet )
		{
            pObserserSet->erase ( pObserver );
        }
    }
}

KDvoid APSSymbol::removeActionObserver ( APSActionObserver* pObserver )
{
    if ( pObserver )
	{
        APS_FOREACH ( APSActionObserverTable, this->m_aActionObservers, iter )
		{
			std::set<APSActionObserver*>*  pObserserSet = iter->second;
            pObserserSet->erase ( pObserver );
        }
    }
}

KDvoid APSSymbol::removeAllActionObservers ( KDvoid )
{    
    APS_FOREACH ( APSActionObserverTable, this->m_aActionObservers, mapIter )
	{
        delete mapIter->second;
    }
    this->m_aActionObservers.clear ( );
}

KDvoid APSSymbol::setParent ( APSGraphic* pParent ) 
{
    m_pParent = pParent;
}

//
// #### Protected ####
//
std::set<APSActionObserver*>* APSSymbol::getActionObservers ( const std::string& sTag, KDbool bCreateNew )
{
	std::set<APSActionObserver*>*  pObservers = this->m_aActionObservers [ sTag ];
    if ( !pObservers && bCreateNew )
	{
		pObservers = new std::set<APSActionObserver*>;
        this->m_aActionObservers [ sTag ] = pObservers;
    }
    return pObservers;
}

KDvoid APSCCSymbolLayer::addFinishedActionObserver ( APSActionObserver* pObserver, APSAction* pAction )
{
    if ( m_aFinishedActions.size ( ) == 0 )
	{
        this->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0 ), CCCallFunc::create ( this, callfunc_selector ( APSCCSymbolLayer::postAll ) ) ) );
    }
	m_aFinishedActions.push_back ( std::pair<APSActionObserver*, APSAction*> ( pObserver, pAction ) );
}

KDvoid APSCCSymbolLayer::postAll ( KDvoid )
{
    APS_FOREACH ( APSPendingPostStorage, m_aFinishedActions, iter )
	{
        iter->first->didFinishAction ( iter->second );
    }
    m_aFinishedActions.clear ( );
}

NS_APS_END



