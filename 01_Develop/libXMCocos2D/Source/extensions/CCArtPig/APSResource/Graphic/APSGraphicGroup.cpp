/* --------------------------------------------------------------------------
 *
 *      File            APSGraphicGroup.cpp
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
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSResourceArray.h"
#include "extensions/CCArtPig/APSSprite.h"
#include "extensions/CCArtPig/APSGraphicAction.h"

NS_APS_BEGIN

const KDbool	kAPSGraphicGroup_defaultHasAnimatableChild  = KD_FALSE;
const KDuint	kAPSGraphicGroup_defaultDisplayedFrameIndex = 0;

const std::string& APSGraphicGroup::getChildIndexesKey ( KDvoid ) 
{
    static const std::string  sKey = "childIndexes";
    return sKey;
}

const std::string& APSGraphicGroup::getHasAnimatableChildKey ( KDvoid )
{
    static const std::string  sKey = "hasAnimChlid";
    return sKey;
}

const std::string& APSGraphicGroup::getDisplayedFrameIndexKey ( KDvoid )
{
    static const std::string  sKey = "frameIdx";
    return sKey;
}


APSGraphicGroup::APSGraphicGroup ( APSDictionary* pProperties )
{
	m_uDisplayedFrameIndex	= kAPSGraphicGroup_defaultDisplayedFrameIndex;
	m_uRunningFrameIndex	= kAPSGraphicGroup_defaultDisplayedFrameIndex;
	m_bHasAnimatableChild	= kAPSGraphicGroup_defaultHasAnimatableChild;

    for ( KDint i=0; i < APS_GRAPHIC_ACTION_TYPES_COUNT; i++ )
	{
        m_aRunningActions [ i ] = KD_NULL;
    }

    m_pChildren = new APSResourceArray ( this );
    
    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSGraphicGroup::~APSGraphicGroup ( KDvoid )
{
    APS_SAFE_DELETE ( m_pChildren );
}

KDbool APSGraphicGroup::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSGraphic::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getHasAnimatableChildKey ( ) );
    if ( pNumber ) 
	{
        this->setHasAnimatableChild ( pNumber->getBoolean ( ) );
    }
    
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getDisplayedFrameIndexKey ( ) );
    if ( pNumber ) 
	{
        this->setDisplayedFrameIndex ( pNumber->getUnsignedInteger ( ) );
    }
    
    // set childIndexes
    APSString*  pIndexes = (APSString *) pProperties->getObjectForKey ( this->getChildIndexesKey ( ) );
    if ( pIndexes ) 
	{
        pIndexes->getResourceArray ( this->getChildren ( ) );
    }
  
    return KD_TRUE;
}

CCNode* APSGraphicGroup::createNode ( KDvoid ) 
{
    CCNode*  pNode = APSGraphic::createNode ( );
    
    this->getChildren ( )->preloadAll ( );
    APS_FOREACH ( APSResourceArray, *this->getChildren ( ), iter )
	{
        APSGraphic*  pChild = (APSGraphic*) iter->second;
        pNode->addChild ( pChild->createNode ( ) );
    }

    return pNode;
}

KDbool APSGraphicGroup::initNode ( KDvoid ) 
{
    if ( APSGraphic::initNode ( ) )
	{  
        if ( this->getHasAnimatableChild ( ) )
		{
            this->setRunningFrameIndex ( this->getDisplayedFrameIndex ( ) );
            this->updateChildSpriteFrames ( );
        }
        
        this->getChildren ( )->preloadAll ( );
        APS_FOREACH ( APSResourceArray, *this->getChildren ( ), iter )
		{
            ( (APSGraphic*) iter->second )->initNode ( );
        }
        
        return KD_TRUE;
    }
    return KD_FALSE;
}


APSSpriteSheetHolder* APSGraphicGroup::getFirstSpriteSheetHolder ( KDvoid ) 
{
    if ( this->getHasAnimatableChild ( ) )
	{
        this->getChildren ( )->preloadAll ( );
        APS_FOREACH ( APSResourceArray, *this->getChildren ( ), iter )
		{
            APSSpriteSheetHolder*  pSH = ( (APSSprite*) iter->second )->getSpriteSheetHolder ( );
            if ( pSH ) 
			{
                return pSH;
            }
        }
    }
    return KD_NULL;
}

KDvoid APSGraphicGroup::runAction ( APSAction* pAction )
{
    if ( this->m_pNode )
	{
        this->m_pNode->runAction ( pAction->getCCAction ( ) );
        
        this->setRunningAction ( pAction );
    }
}

KDvoid APSGraphicGroup::stopAction ( APSAction* pAction )
{
    if ( this->m_pNode )
	{
        
        this->m_pNode->stopAction ( pAction->getCCAction ( ) );
        
        if ( this->getRunningActionForType ( pAction->getActionType ( ) ) == pAction )
		{
            // Assign NULL for the type.
            this->setRunningAction ( KD_NULL, pAction->getActionType ( ) );
        }
    }
}

KDvoid APSGraphicGroup::updateAccOpacity ( KDvoid )
{
    APSGraphic::updateAccOpacity ( );
    this->getChildren ( )->preloadAll ( );
    APS_FOREACH ( APSResourceArray, *this->getChildren ( ), iter )
	{
        ( (APSSprite*) iter->second )->updateAccOpacity ( );
    }
}

KDvoid APSGraphicGroup::updateChildSpriteFrames ( KDvoid ) 
{
    if ( this->getHasAnimatableChild ( ) )
	{
        this->getChildren ( )->preloadAll ( );
        // If a child is animatable, all children are sprite.
        APS_FOREACH ( APSResourceArray, *this->getChildren ( ), iter ) 
		{
            ( (APSSprite*) iter->second )->updateSpriteFrame ( );
        }
    }
}

APSAction* APSGraphicGroup::getRunningActionForType ( APSActionType uType ) const 
{
    KDint  nPos = this->getIndexForActionType ( uType );
    if ( nPos >= 0 )
	{
        return this->m_aRunningActions [ nPos ];
    }
    return KD_NULL;
}

KDvoid APSGraphicGroup::setRunningAction ( APSAction* pAction, APSActionType uType )
{
    if ( uType == kAPSActionTypeNone )
	{
        uType = pAction->getActionType ( );
    }
    
    KDint  nPos = this->getIndexForActionType ( uType );
    if ( nPos >= 0 )
	{
        this->m_aRunningActions [ nPos ] = pAction;
    }
}

KDvoid APSGraphicGroup::setEnableActionForType ( KDbool bEnable, APSActionType uType )
{
    KDint  nPos = this->getIndexForActionType ( uType );
    if ( nPos >= 0 ) 
	{
        if ( bEnable ) 
		{
            if (!this->isActionEnabledForType ( uType ) )
			{
                this->m_aRunningActions [ nPos ] = KD_NULL;
            }
        } 
		else 
		{
            if ( this->isActionEnabledForType ( uType ) )
			{
                if ( this->m_aRunningActions [ nPos ] )
				{
                    // Interrupt currently running action.
                    this->m_aRunningActions [ nPos ]->interrupt ( );
                }
                // By setting rigid action, no other actions are allowed to run in this graphic group.
                this->m_aRunningActions [ nPos ] = this->getRigidAction ( );
            }
        }
    }
}

KDbool APSGraphicGroup::isActionEnabledForType ( APSActionType uType ) const 
{
    KDint  nPos = this->getIndexForActionType ( uType );
    if ( nPos >= 0 )
	{
        return this->m_aRunningActions [ nPos ] != this->getRigidAction ( );
    }
    return KD_TRUE;
}

KDvoid APSGraphicGroup::setEnableAllActions ( KDbool bEnable ) 
{
    this->setEnableActionForType ( bEnable, kAPSActionTypeMove		);
    this->setEnableActionForType ( bEnable, kAPSActionTypeRotation	);
    this->setEnableActionForType ( bEnable, kAPSActionTypeScale		);
    this->setEnableActionForType ( bEnable, kAPSActionTypeFade		);
    this->setEnableActionForType ( bEnable, kAPSActionTypeAnimate	);
}

KDint APSGraphicGroup::getIndexForActionType ( APSActionType uType )
{
    switch ( uType )
	{
        case kAPSActionTypeMove		:	return 0;
        case kAPSActionTypeRotation	:	return 1;
        case kAPSActionTypeScale	:	return 2;
        case kAPSActionTypeFade		:	return 3;
        case kAPSActionTypeAnimate	:	return 4;
        default						:	return -1;
    }
}

APSAction* APSGraphicGroup::getRigidAction ( KDvoid ) 
{
    static APSAction*  pAction = KD_NULL;
    if ( !pAction ) 
	{
        pAction = new APSAction ( );
        pAction->setPriority ( APS_MAX_PRIORITY );
    }
    return pAction;
}

NS_APS_END






