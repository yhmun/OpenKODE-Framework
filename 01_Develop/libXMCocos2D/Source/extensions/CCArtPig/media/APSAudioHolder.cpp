/* --------------------------------------------------------------------------
 *
 *      File            APSAudioHolder.cpp
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
#include "extensions/CCArtPig/APSAudioHolder.h"
#include "extensions/CCArtPig/APSResourceManager.h"
#include "extensions/CCArtPig/APSAudioAction.h"
#include "denshion/CCSound.h"

NS_APS_BEGIN

const std::string& APSAudioHolder::getIsBackgroundKey ( KDvoid )
{
	static const std::string  sKey = "isBackground";
    return sKey;
}

APSAudioHolder::APSAudioHolder ( const KDchar* szCode, APSResourceManager* pManager )
: APSMedium ( szCode, pManager )
{
	m_pRunningAction	= KD_NULL;
	m_uSoundID			= 0;
	m_pFilePath			= KD_NULL;
}

APSAudioHolder::APSAudioHolder ( APSDictionary* pProperties ) 
{
	m_pRunningAction	= KD_NULL;
	m_uSoundID			= 0;
	m_pFilePath			= KD_NULL;

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSAudioHolder::~APSAudioHolder ( KDvoid )
{    
	if ( m_uSoundID )
	{
		CCSound::sharedSound ( )->Close ( m_uSoundID );
	}

    APS_SAFE_DELETE ( m_pFilePath );
}

KDbool APSAudioHolder::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSMedium::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
        
    // isBackground
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIsBackgroundKey ( ) );
    if ( pNumber ) 
	{
        this->setIsBackground ( pNumber->getBoolean ( ) );
    }
    
    return KD_TRUE;
}

KDvoid APSAudioHolder::clearCachedData ( KDvoid )
{ 
    if ( !this->getRunningAction ( ) )
	{
        if ( this->getIsBackground ( ) ) 
		{

        }
		else
		{
			CCSound::sharedSound ( )->Close ( m_uSoundID );
		}
    }    
	APSMedium::clearCachedData ( );
}

KDvoid APSAudioHolder::preloadData ( KDvoid ) 
{
	m_uSoundID = CCSound::sharedSound ( )->Open ( this->getFilePath ( )->c_str ( ), this->getIsBackground ( ) ? CCSoundRepeat : 0 );
}

KDvoid APSAudioHolder::playForAction ( APSAction* pAction )
{
    if ( this->getRunningAction ( ) )
	{
        this->stopForAction ( this->getRunningAction ( ) );
    }

	if ( this->getIsBackground ( ) )
	{
		CCSound::sharedSound ( )->Play ( m_uSoundID );
	}
	else
	{
		CCSound::sharedSound ( )->Rewind ( m_uSoundID );
	}

    this->setRunningAction ( pAction );
}

KDvoid APSAudioHolder::stopForAction ( APSAction* pAction ) 
{
    // Only the action that ran currently playing sound can stop it.
    if ( this->getRunningAction ( ) == pAction )
	{
        CCSound::sharedSound ( )->Stop ( m_uSoundID );
        
        this->setRunningAction ( KD_NULL );
    }
}

const std::string* APSAudioHolder::getFilePath ( KDvoid )
{
    if ( !m_pFilePath )
	{
		m_pFilePath = new std::string ( this->getResourceManager ( )->getPathForFilename ( this->getFilename ( ) ) );
    }
    return m_pFilePath;
}

NS_APS_END
