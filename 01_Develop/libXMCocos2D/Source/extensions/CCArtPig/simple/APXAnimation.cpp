/* --------------------------------------------------------------------------
 *
 *      File            APXAnimation.cpp
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
#include "extensions/CCArtPig/APXAnimation.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSResourceManager.h"

NS_APS_BEGIN

const std::string  kAPXAnimation_defaultSymbolTag		= "Symbol1";
const std::string  kAPXAnimation_defaultActionGroupTag	= "ActionGroup1";
const std::string  kAPXAnimation_isPlayingKey			= "isPlaying";

static std::map<const std::string, std::set<APXAnimation*>* > l_aAllocatedAnimations;

std::set<APXAnimation *>*  animationsForProjectId ( const KDchar* szPID );
KDvoid  removeDataWithProjectId ( const KDchar* szPID );

APXAnimation::APXAnimation ( const KDchar* szProjectID, KDbool bShouldScaleModelToDevice ) 
{
	m_bIsPlaying = KD_FALSE;
	m_pSymbol	 = KD_NULL;
	m_bShouldScaleModelToDevice = bShouldScaleModelToDevice;

    APSResourceManager*  pManager = APSResourceManager::getResourceManagerForProjectId ( szProjectID );
    
    if ( pManager ) 
	{
        m_pSymbol = pManager->newSymbol ( kAPXAnimation_defaultSymbolTag.c_str ( ) );
        if ( m_pSymbol )
		{                
            if ( bShouldScaleModelToDevice ) 
			{
                // Scale symbol with regard to active device.
                m_pSymbol->setScale ( pManager->getScaleModelToDevice ( KD_FALSE ) );
            }
            
            m_pSymbol->preloadData ( );
            
        }
        
    }

    animationsForProjectId ( szProjectID )->insert ( this );
}

APXAnimation::~APXAnimation ( KDvoid )
{
	std::set<APXAnimation*>*  pAnimations = animationsForProjectId ( this->getProjectId ( ) );
    pAnimations->erase ( this );
    
    const KDchar*  szPID = m_pSymbol->getResourceManager ( )->getProjectId ( );
    
    // Deleting symbol must be done before remving project because symbol's APSSprite and APSAudioAction objects access resources to decrease reference counts.
    CC_SAFE_RELEASE ( m_pSymbol );
    
    // Clean up the project if there is no more instance.
    if ( pAnimations->size ( ) == 0 )
	{
        removeDataWithProjectId ( szPID );
    }
}

APXAnimation* APXAnimation::animationWithProjectId ( const KDchar* szProjectID, KDbool bShouldScaleModelToDevice )
{
    APXAnimation*  pAnimation = new APXAnimation ( szProjectID, bShouldScaleModelToDevice );
    pAnimation->autorelease();
    return pAnimation;
}

const KDchar* APXAnimation::getProjectId ( KDvoid )
{
    APSResourceManager*  pRM = m_pSymbol->getResourceManager ( );
    if ( pRM ) 
	{
        return pRM->getProjectId ( );
    }
    return KD_NULL;
}

CCLayer* APXAnimation::getRootNode ( KDvoid )
{
    return m_pSymbol->getRootNode ( );
}

// Resets all CCNode objects in the symbol to become original state.
KDvoid APXAnimation::resetNodes ( KDvoid ) 
{
    m_pSymbol->initNode ( );
}

KDvoid APXAnimation::play ( KDvoid ) 
{
    if ( this->m_bHasPlayed )
	{
        this->resetNodes ( );
    }
    m_pSymbol->triggerPrologueActionGroups ( );
    m_bHasPlayed = KD_TRUE;
}

KDvoid APXAnimation::stop ( KDvoid )
{
    m_pSymbol->interruptAllActions ( );
}

KDfloat APXAnimation::getDuration ( KDvoid )
{
    return m_pSymbol->getPrologueDuration ( );
}

KDbool APXAnimation::getShouldScaleModelToDevice ( KDvoid ) const 
{
    return m_bShouldScaleModelToDevice;
}

KDvoid APXAnimation::setShouldScaleModelToDevice ( KDbool bShouldScale ) 
{
    m_bShouldScaleModelToDevice = bShouldScale;
}

KDbool APXAnimation::isPlaying ( KDvoid ) 
{
    return m_bIsPlaying;
}

KDvoid APXAnimation::setIsPlaying ( KDbool bIsPlaying ) 
{
    if ( bIsPlaying ) 
	{
        m_bHasPlayed = KD_TRUE;
    }
    m_bIsPlaying = bIsPlaying;
}

//
// #### APActionGroupObserver ####
//

KDvoid APXAnimation::addObserver ( APXAnimationObserver* pObserver ) 
{
    m_pSymbol->addActionObserver ( this, kAPXAnimation_defaultActionGroupTag );

    m_aObservers.insert ( pObserver );
}

KDvoid APXAnimation::removeObserver ( APXAnimationObserver* pObserver )
{
    m_aObservers.erase ( pObserver );
}

// Notifies the observer action group specified by a tag is about to start running.
KDvoid APXAnimation::willStartAction ( APSAction* pAction ) 
{
    this->setIsPlaying ( KD_TRUE );
	for ( std::set<APXAnimationObserver*>::iterator iter = m_aObservers.begin ( ); iter != m_aObservers.end ( ); iter++ )
	{
        ( *iter )->willStartAnimation ( this );
    }
}

// Notifies the observer action group specified by a tag finished running.
KDvoid APXAnimation::didFinishAction ( APSAction* pAction ) 
{
    this->notifyObservers ( );
}

KDvoid APXAnimation::notifyObservers ( KDvoid ) 
{       
    this->setIsPlaying ( KD_FALSE );
    
	for ( std::set<APXAnimationObserver*>::iterator iter = m_aObservers.begin ( ); iter != m_aObservers.end ( ); iter++ )
	{
        ( *iter )->didFinishAnimation ( this );
    }
}

std::set<APXAnimation*>* animationsForProjectId ( const KDchar* szPID )
{
    std::set<APXAnimation*>*  pAnimations = l_aAllocatedAnimations [ szPID ];
    if ( !pAnimations )
	{
        pAnimations = new std::set<APXAnimation*>;
        l_aAllocatedAnimations [ szPID ] = pAnimations;
    }
    return pAnimations;
}

KDvoid removeDataWithProjectId ( const KDchar* szPID )
{
    // Stop all animation
    std::set<APXAnimation*>*  pAnimations = animationsForProjectId ( szPID );
    for ( std::set<APXAnimation*>::iterator iter = pAnimations->begin ( ); iter != pAnimations->end ( ); iter++ )
	{
        ( *iter )->stop ( );
    }
    l_aAllocatedAnimations.erase ( szPID );
    
    APSResourceManager::removeResourceManagerForProjectId ( szPID );
}

NS_APS_END