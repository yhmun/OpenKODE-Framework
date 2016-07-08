/* --------------------------------------------------------------------------
 *
 *      File            APSAnimateAction.cpp
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
#include "extensions/CCArtPig/APSAnimateAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"
#include "extensions/CCArtPig/APSSpriteFrame.h"
#include "sprite_nodes/CCSpriteFrameCache.h"

NS_APS_BEGIN

const KDbool   kAPSAnimateAction_defaultIsWholeRange = KD_FALSE;
const KDfloat  kAPSAnimateAction_defaultInterval     = 0.1f;

const std::string& APSAnimateAction::getIsWholeRangeKey ( KDvoid )
{
    static const std::string  sKey = "wholeRange";
    return sKey;
}

const std::string& APSAnimateAction::getIntervalKey ( KDvoid )
{
    static const std::string  sKey = "interval";
    return sKey;
}

APSAnimateAction::APSAnimateAction ( APSDictionary* pProperties ) 
{
	m_bIsWholeRange = kAPSAnimateAction_defaultIsWholeRange;
	m_fInterval	    = kAPSAnimateAction_defaultInterval;

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSAnimateAction::initWithProperties ( APSDictionary* pProperties )
{
    KDbool  bOk = APSIntervalGraphicAction::initWithProperties ( pProperties );
    
    // set wholeRange
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIsWholeRangeKey ( ) );
    if ( pNumber ) 
	{
        this->m_bIsWholeRange = pNumber->getBoolean ( );
    }
    
    // set interval
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIntervalKey ( ) );
    if ( pNumber ) 
	{
        this->m_fInterval = pNumber->getFloat ( );
    }
        
    return bOk;
}

//
// #### Protected ####
//
KDvoid APSAnimateAction::saveNodeState ( KDvoid ) 
{    
    APSGraphicGroup*  pGraphic = (APSGraphicGroup*) this->getTargetGraphic ( );
    if ( pGraphic )
	{
        this->m_uStartFrameIndex = pGraphic->getRunningFrameIndex ( );
    }
}

KDvoid APSAnimateAction::restoreNodeState ( KDvoid )
{
    APSGraphicGroup*  pGraphic = (APSGraphicGroup*) this->getTargetGraphic ( );
    if ( pGraphic )
	{
        pGraphic->setRunningFrameIndex ( this->m_uStartFrameIndex );
        pGraphic->updateChildSpriteFrames ( );
    }
}

CCActionInterval* APSAnimateAction::makeGraphicCCAction ( KDbool bUniversal, KDfloat fScale ) 
{
    APSGraphicGroup*  pGraphic = (APSGraphicGroup*) this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        APSSpriteSheetHolder*  pSH = pGraphic->getFirstSpriteSheetHolder ( );
        if ( pSH ) 
		{
            KDuint  uCount = pSH->getCount ( );

            if ( bUniversal ) 
			{
                CCArray*  pFrames = CCArray::create ( );
                for ( KDuint i = 0; i < uCount; i++ )
				{
					std::vector<APSSpriteFrame*>*  pApsFrames = pSH->getFrames ( KD_FALSE );
                    pFrames->addObject ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( pApsFrames->at ( i )->getName ( ) ) );
                }
                
                CCAnimation*  pAnimation = CCAnimation::createWithSpriteFrames ( pFrames );
                
                if ( this->getIsWholeRange ( ) )
				{
                    pAnimation->setRestoreOriginalFrame ( this->isRestoringAction ( ) );
                    return CCAnimate::create ( pAnimation );
                } 
				else 
				{
                    return APSCCAnimate::actionWithDuration ( this->getDuration ( ), pAnimation, this->isRestoringAction ( ), this->getInterval ( ) );
                }
            } 
			else 
			{
                if ( this->getIsWholeRange ( ) )
				{
                    return APSCCAnimateChildren::actionWithDuration ( this->getDuration ( ), pSH->getCount ( ), pGraphic );
                } 
				else 
				{
                    return APSCCAnimateChildren::actionWithDuration ( this->getDuration ( ), pSH->getCount ( ), pGraphic, this->getInterval ( ) );
                }
            }
        }
    }

    return KD_NULL;
}

NS_APS_END
