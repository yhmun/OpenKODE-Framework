/* --------------------------------------------------------------------------
 *
 *      File            APSAudioAction.cpp
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
#include "extensions/CCArtPig/APSAudioAction.h"
#include "extensions/CCArtPig/APSAudioHolder.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"

NS_APS_BEGIN

const KDbool  kAPSAudioAction_defaultTrimmed = KD_FALSE;

// const float kAPSAudioAction_defaultPlayTimeInTrack = 0.f;

const std::string& APSAudioAction::getTrimmedKey ( KDvoid )
{
	static const std::string  sKey = "trimmed";
    return sKey;
}

const std::string& APSAudioAction::getAudioHolderCodeKey ( KDvoid ) 
{
    static const std::string  sKey = "audioHolderCode";
    return sKey;
}

APSAudioAction::APSAudioAction ( APSDictionary* pProperties ) 
{
	m_pAudioHolder		= KD_NULL;
	m_sAudioHolderCode  = "";
	m_bTrimmed			= kAPSAudioAction_defaultTrimmed;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

KDbool APSAudioAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set audioHolderCode
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getAudioHolderCodeKey ( ) );
    if ( pStr ) 
	{
        this->setAudioHolderCode ( pStr->c_str ( ) );
    }
    
    // set trimmed
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTrimmedKey ( ) );
    if ( pNumber ) 
	{
        this->setTrimmed ( pNumber->getBoolean ( ) );
    }
    
    return KD_TRUE;
}

KDvoid APSAudioAction::preloadData ( KDvoid ) 
{
    APSAudioHolder*  pHolder = this->getAudioHolder ( );
    if ( pHolder )
	{
        pHolder->preloadData ( );
    }
}

CCFiniteTimeAction* APSAudioAction::makeMidCCAction ( KDbool bUniversal, KDfloat fScale )
{
    return CCSequence::createWithTwoActions ( CCCallFunc::create ( this, callfunc_selector ( APSAudioAction::play ) ), CCDelayTime::create ( m_fDuration ) );
}

KDvoid APSAudioAction::stopFunc ( KDvoid )
{
    if ( this->getTrimmed ( ) )
	{
        // Stop playing audio only if it is trimmed because playing audio may be delayed in Android.
        this->getAudioHolder ( )->stopForAction ( this );
    }
    APSAction::stopFunc ( );
}

KDvoid APSAudioAction::_interrupt ( KDvoid )
{
    this->getAudioHolder ( )->stopForAction ( this );
    APSAction::_interrupt ( );
}

KDvoid APSAudioAction::play ( KDvoid )
{
    this->getAudioHolder ( )->playForAction ( this );
}

APSAudioHolder* APSAudioAction::getAudioHolder ( KDvoid )
{
    if ( !m_pAudioHolder ) 
	{
        m_pAudioHolder = (APSAudioHolder*) this->getMediumForCode ( this->getAudioHolderCode ( ) );
    }
    return m_pAudioHolder;
}

NS_APS_END


