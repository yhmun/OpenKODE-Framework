/* --------------------------------------------------------------------------
 *
 *      File            CCControl.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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

#include "Precompiled.h"
#include "extensions/CCControl/CCControl.h"
#include "menu_nodes/CCMenu.h"
#include "touch_dispatcher/CCTouch.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "support/CCDirector.h"

NS_CC_BEGIN

CCControl::CCControl ( KDvoid )
{
	m_bIsOpacityModifyRGB	= KD_FALSE;
	m_eState				= kCCControlStateNormal;
	m_bHasVisibleParents	= KD_FALSE;
	m_bEnabled				= KD_FALSE;
	m_bSelected				= KD_FALSE;
	m_bHighlighted			= KD_FALSE;
	m_pDispatchTable		= KD_NULL;
}

CCControl::~CCControl ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pDispatchTable );
}

CCControl* CCControl::create ( KDvoid )
{
	CCControl*  pRet = new CCControl ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;

}

KDbool CCControl::init ( KDvoid )
{
	if ( CCLayer::init ( ) )
	{
	    // Initialise instance variables
		m_eState = kCCControlStateNormal;

		setEnabled	   ( KD_TRUE );
        setSelected	   ( KD_FALSE );
        setHighlighted ( KD_FALSE );

		// Set the touch dispatcher priority by default to 1
		this->setTouchPriority ( 1 );

        // Initialise the tables
		m_pDispatchTable = new CCDictionary ( );	

		return KD_TRUE;
    }
	
	return KD_FALSE;
}

// Menu - Events
KDvoid CCControl::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, getTouchPriority ( ), KD_TRUE );
}

KDvoid CCControl::onEnter ( KDvoid )
{
    CCLayer::onEnter ( );
}

KDvoid CCControl::onExit ( KDvoid )
{
    CCLayer::onExit ( );
}

KDvoid CCControl::sendActionsForControlEvents ( CCControlEvent uControlEvents )
{
	// For each control events
    for ( KDint i = 0; i < kCCControlEventTotalNumber; i++)
    {
        // If the given uControlEvents bitmask contains the curent event
        if ( ( uControlEvents & ( 1 << i ) ) )
        {
            // Call invocations
			CCArray*   pInvocationList = this->dispatchListforControlEvent ( 1 << i );
            CCObject*  pObject = KD_NULL;
            CCARRAY_FOREACH ( pInvocationList, pObject )
            {
                CCInvocation*  pInvocation = (CCInvocation*) pObject;
                pInvocation->invoke ( this );
            }                
        }
    }
}

KDvoid CCControl::addTargetWithActionForControlEvents ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvents )
{
    // For each control events
    for ( KDint i = 0; i < kCCControlEventTotalNumber; i++ )
    {
        // If the given uControlEvents bitmask contains the curent event
        if ( ( uControlEvents & (1 << i) ) )
        {
			this->addTargetWithActionForControlEvent ( pTarget, pAction, 1 << i );            
        }
    }
}

KDvoid CCControl::addTargetWithActionForControlEvent ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent )
{	
    // Create the invocation object
	CCInvocation*  pInvocation = CCInvocation::create ( pTarget, pAction, uControlEvent );

    // Add the invocation into the dispatch list for the given control event
	CCArray*  pEventInvocationList = this->dispatchListforControlEvent ( uControlEvent );
	pEventInvocationList->addObject ( pInvocation );    
}

KDvoid CCControl::removeTargetWithActionForControlEvents ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvents )
{
	 // For each control events
    for ( KDint i = 0; i < kCCControlEventTotalNumber; i++ )
    {
        // If the given uControlEvents bitmask contains the curent event
        if ( ( uControlEvents & (1 << i) ) )
        {
			this->removeTargetWithActionForControlEvent ( pTarget, pAction, 1 << i );
        }
    }
}

KDvoid CCControl::removeTargetWithActionForControlEvent ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent )
{
    // Retrieve all invocations for the given control event
	CCArray*  pEventInvocationList = this->dispatchListforControlEvent ( uControlEvent );
    
	//remove all invocations if the target and action are null
	//TODO: should the invocations be deleted, or just removed from the array? Won't that cause issues if you add a single invocation for multiple events?
	KDbool  bDeleteObjects = KD_TRUE;
    if ( !pTarget && !pAction )
    {
		//remove objects
		pEventInvocationList->removeAllObjects ( );
    } 
	else
    {
		//normally we would use a predicate, but this won't work here. Have to do it manually			
		CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pEventInvocationList, pObject )
		{
			CCInvocation*  pInvocation = (CCInvocation*) pObject;
			KDbool  bShouldBeRemoved = KD_TRUE;
			if ( pTarget )
			{
				bShouldBeRemoved = ( pTarget == pInvocation->getTarget ( ) );				
			}

			if ( pAction )
			{
				bShouldBeRemoved = ( bShouldBeRemoved && ( pAction == pInvocation->getAction ( )  ) );
			}

			// Remove the corresponding invocation object
			if ( bShouldBeRemoved )
			{
				pEventInvocationList->removeObject ( pInvocation, bDeleteObjects );
			}
		}
	}
}

KDvoid CCControl::setOpacityModifyRGB ( KDbool bOpacityModifyRGB )
{
	m_bIsOpacityModifyRGB = bOpacityModifyRGB;

	CCObject*  pChild;
	CCArray*   pChildren = getChildren ( );
	CCARRAY_FOREACH ( pChildren, pChild )
	{
		CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*> ( pChild );		
		if ( pNode )
		{
			pNode->setOpacityModifyRGB ( bOpacityModifyRGB );
		}
	}
}

KDbool CCControl::isOpacityModifyRGB ( KDvoid )
{
	return m_bIsOpacityModifyRGB;
}

CCPoint CCControl::getTouchLocation ( CCTouch* pTouch )
{    
	CCPoint  tLocation = pTouch->getLocation ( );           // Get the touch position
    tLocation = this->convertToNodeSpace ( tLocation );		// Convert to the node space of this class
    
    return tLocation;
}

KDbool CCControl::isTouchInside ( CCTouch* pTouch )
{
    CCPoint  tLocation = pTouch->getLocation ( );			// Get the touch position
    tLocation = this->getParent ( )->convertToNodeSpace ( tLocation );

    CCRect tBox = boundingBox ( );
    return tBox.containsPoint ( tLocation );
}

CCArray* CCControl::dispatchListforControlEvent ( CCControlEvent uControlEvent )
{
	CCArray*  pInvocationList = (CCArray*) m_pDispatchTable->objectForKey ( uControlEvent );

    // If the invocation list does not exist for the  dispatch table, we create it
    if ( pInvocationList == KD_NULL )
    {
		pInvocationList = CCArray::createWithCapacity ( 1 );
		m_pDispatchTable->setObject ( pInvocationList, uControlEvent );
    }   

    return pInvocationList;
}

KDvoid CCControl::needsLayout ( KDvoid )
{

}

KDvoid CCControl::setEnabled ( KDbool bEnabled )
{
    m_bEnabled = bEnabled;

	if ( m_bEnabled )
	{
        m_eState = kCCControlStateNormal;
    } 
	else
	{
        m_eState = kCCControlStateDisabled;
    }

    this->needsLayout ( );
}

KDbool CCControl::isEnabled()
{
    return m_bEnabled;
}

KDvoid CCControl::setSelected ( KDbool bSelected )
{
    m_bSelected = bSelected;
	this->needsLayout ( );
}

KDbool CCControl::isSelected()
{
    return m_bSelected;
}

KDvoid CCControl::setHighlighted ( KDbool bHighlighted )
{
    m_bHighlighted = bHighlighted;
	this->needsLayout ( );
}

KDbool CCControl::isHighlighted ( KDvoid )
{
    return m_bHighlighted;
}

KDbool CCControl::hasVisibleParents ( KDvoid )
{
    CCNode*  pParent = this->getParent ( );
    
	for ( CCNode* pNode = pParent; pNode != KD_NULL; pNode = pNode->getParent ( ) )
    {
        if( !pNode->isVisible ( ) )
        {
            return KD_FALSE;
        }
    }

    return KD_TRUE;
}

NS_CC_END