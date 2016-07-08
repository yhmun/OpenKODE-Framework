/* --------------------------------------------------------------------------
 *
 *      File            APSActionGroup.cpp
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
#include "extensions/CCArtPig/APSActionGroup.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"

NS_APS_BEGIN

const KDbool kAPSActionGroup_defaultPrologue = KD_FALSE;
const KDbool kAPSActionGroup_defaultIntact   = KD_FALSE;


const std::string& APSActionGroup::getActionIndexesKey ( KDvoid )
{
    static const std::string  sKey = "actionIndexes";
    return sKey;
}

const std::string& APSActionGroup::getPrologueKey ( KDvoid )
{
    static const std::string  sKey = "prologue";
    return sKey;
}

const std::string& APSActionGroup::getIntactKey ( KDvoid )
{
    static const std::string  sKey = "intact";
    return sKey;
}

APSActionGroup::APSActionGroup ( APSDictionary* pProperties ) 
{
	m_bPrologue = kAPSActionGroup_defaultPrologue;
	m_bIntact   = kAPSActionGroup_defaultIntact;
	m_uRunningActionCount = 0;

    m_pActions = new APSResourceArray ( this );    
    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSActionGroup::~APSActionGroup ( KDvoid )
{
    APS_SAFE_DELETE ( m_pActions )
}

KDvoid APSActionGroup::decreaseRunningActionCount ( KDvoid )
{
    this->m_uRunningActionCount--;
    if ( this->m_uRunningActionCount == 0 )
	{
        this->stopFunc ( );
    }
}

KDbool APSActionGroup::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set prologue
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getPrologueKey ( ) );
    if ( pNumber )
	{
        this->setPrologue ( pNumber->getBoolean ( ) );
    }
    
    // set intact
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIntactKey ( ) );
    if ( pNumber )
	{
        this->setIntact ( pNumber->getBoolean());
    }
    
    // set actionIndexes
    APSString*  pIndexes = (APSString*) pProperties->getObjectForKey ( this->getActionIndexesKey ( ) );
    pIndexes->getResourceArray ( this->getActions ( ) );
    
    return KD_TRUE;
}

KDvoid APSActionGroup::getActionsForTargetGraphic ( std::vector<APSAction*>* pActions, const APSGraphicGroup* pTargetGraphic ) const
{
    if ( pActions && pTargetGraphic )
	{
        this->getActions ( )->preloadAll ( );
        APS_FOREACH ( APSResourceArray, *this->getActions ( ), iter )
		{
            APSAction*  pAct = (APSAction*) iter->second;
            if ( pAct && pAct->getTargetGraphic ( ) == pTargetGraphic )
			{
                pActions->push_back ( pAct );
            }
        }
        
    }
}

KDvoid APSActionGroup::startFunc ( KDvoid )
{
    // runningActionCount begins with whole actions count, and it is reduced on every sub-action finishing.
    this->m_uRunningActionCount = this->getActions ( )->size ( );
    
    APSAction::startFunc ( );
    
    // If there is no action to run, stop action group immediately.
    if ( this->m_uRunningActionCount == 0 )
	{
        this->stopFunc ( );
    }
}

KDvoid APSActionGroup::_interrupt ( KDvoid )
{
    // interrupt all sub-actions.
    APSResourceArray*  pActions = this->getActions ( );
    pActions->preloadAll ( );
    APS_FOREACH ( APSResourceArray, *pActions, iter )
	{
        APSAction*  pAction = (APSAction*) iter->second;
        if ( pAction->getRunning ( ) ) 
		{
            pAction->interrupt ( );
        }
    }
    
    APSAction::_interrupt ( );
}

KDvoid APSActionGroup::_trigger ( KDvoid ) 
{
    // !!! Fixme: sometimes target node has this action group as running action even when running==NO
    this->getTargetGraphic ( )->stopAction ( this );
    
    APSAction::_trigger ( );
}

CCFiniteTimeAction* APSActionGroup::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    //
    // Group actions with startTime property.
    //
    KDint  nCnt = this->getActions ( )->size ( );
    if ( nCnt > 0 )
	{
		std::vector<std::vector<std::pair<APSAction*, CCFiniteTimeAction*> >* > aActionPairs;
        
		std::vector<std::pair<APSAction*, CCFiniteTimeAction*> >* pPairs = KD_NULL;
        
        KDfloat  fStartTime = -1.f;
        for ( KDint i = 0; i < nCnt; i++ )
		{
			std::pair<APSAction*, CCFiniteTimeAction*>  aPair = this->getActionPair ( i );
            if ( aPair.first && aPair.second )
			{
                // if next action does not start at the same time as previous action's start time, create a new pairs list.
                if ( !isZeroF ( aPair.first->getStartTime ( ) - fStartTime ) )
				{
                    if ( pPairs )
					{
                        aActionPairs.push_back ( pPairs );
                    }
					pPairs = new std::vector<std::pair<APSAction*, CCFiniteTimeAction*> >;
                    fStartTime = aPair.first->getStartTime ( );
                }
                
                pPairs->push_back ( aPair );
            }
        }
        
        aActionPairs.push_back ( pPairs );
        
        fStartTime = ( *aActionPairs [ 0 ] ) [ 0 ].first->getStartTime ( );
        
        CCFiniteTimeAction*  pAction = CCSequence::createWithTwoActions ( CCDelayTime::create ( fStartTime ), this->spawnWithActions ( *aActionPairs [ 0 ] ) );
                
        for ( KDuint i = 1; i < aActionPairs.size ( ); i++ )
		{
            KDfloat  fNextStartTime = ( *aActionPairs [ i ] ) [ 0 ].first->getStartTime ( );
            KDfloat  fDuration = fNextStartTime - fStartTime;
            fStartTime = fNextStartTime;

            pAction = CCSequence::create ( pAction, CCDelayTime::create ( fDuration ), this->spawnWithActions ( *aActionPairs [ i ] ), KD_NULL );
		}
        
        // deallocate actionPairs
        for ( KDuint i = 0; i < aActionPairs.size ( ); i++ )
		{
            delete aActionPairs [ i ];
        }
        
        return pAction;
    }
    
    return KD_NULL;
}

CCFiniteTimeAction * APSActionGroup::makePostCCAction ( KDvoid ) 
{
    // Although this CCMoveBy action does nothing, sequence of sub-actions in action goup works properly when the action group does not have any graphic action.
    return CCMoveBy::create ( KD_FLT_EPSILON, CCPointZero );   
}

CCFiniteTimeAction* APSActionGroup::spawnWithActions ( std::vector<std::pair<APSAction*, CCFiniteTimeAction*> >& aActionPairs )
{
    KDsize  uSize = aActionPairs.size ( );
    CCFiniteTimeAction*  pAction = aActionPairs [ 0 ].second;
    
    for ( KDuint i = 1; i < uSize; i++ )
	{
        pAction = CCSpawn::createWithTwoActions ( pAction, aActionPairs [ i ].second );
	}

    return pAction;
}

std::pair<APSAction*, CCFiniteTimeAction*> APSActionGroup::getActionPair ( KDint nIndex )
{
    APSAction*  pAction = (APSAction*) this->getActions ( )->getResourceAtIndex ( nIndex );

    CCFiniteTimeAction*  pCCAction = KD_NULL;
    if ( pAction )
	{
        pCCAction = CCCallFuncND::create ( this, callfuncND_selector ( APSActionGroup::notifyTrigger ), pAction );    
	}

	if ( !pAction || !pCCAction ) 
	{
        APSLOG ( "Failed to create action for action index %d.", this->getActions ( )->getApsIndexAtIndex ( nIndex ) );
    }
	return std::pair<APSAction*, CCFiniteTimeAction*> ( pAction, pCCAction );
}

KDvoid APSActionGroup::notifyTrigger ( CCNode* pSender, KDvoid* pTargetAction )
{
    APSSymbol*  pSymbol = this->getParentSymbol ( );
    if ( pSymbol )
	{
        pSymbol->triggerAction ( (APSAction*) pTargetAction );
    }
}

NS_APS_END


