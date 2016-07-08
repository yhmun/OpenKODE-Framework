/* --------------------------------------------------------------------------
 *
 *      File            APSSpriteSheetHolder.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Created by Kim Kiyoung on 5/7/12.
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
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"
#include "extensions/CCArtPig/APSResourceManager.h"
#include "sprite_nodes/CCSpriteFrameCache.h"

NS_APS_BEGIN

const std::string& APSSpriteSheetHolder::getFramesKey ( KDvoid )
{
	static const std::string  sKey = "frames";
    return sKey;
}

APSSpriteSheetHolder::APSSpriteSheetHolder ( const KDchar* szCode, APSResourceManager* pManager ) 
: APSImageHolder ( szCode, pManager )
{	
	m_pRuntimePlPath = KD_NULL;
	m_bIsCached		 = KD_FALSE;
}

APSSpriteSheetHolder::APSSpriteSheetHolder ( APSDictionary* pProperties ) 
{
	m_pRuntimePlPath = KD_NULL;
	m_bIsCached		 = KD_FALSE;

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSSpriteSheetHolder::~APSSpriteSheetHolder ( KDvoid )
{
    this->clearCachedData ( );

	APS_FOREACH ( std::vector<APSSpriteFrame*>, this->m_aFrames, iter )
	{
        APS_SAFE_DELETE ( *iter );
    }
}

KDbool APSSpriteSheetHolder::initWithProperties ( APSDictionary* pProperties ) 
{
    KDbool  bOk = APSImageHolder::initWithProperties ( pProperties );
        
    APSArray*  pArray = (APSArray*) pProperties->getObjectForKey ( this->getFramesKey ( ) );
    if ( pArray ) 
	{
        APS_FOREACH ( APSArrayStorage, *pArray, iter ) 
		{
            APSSpriteFrame*  pFrame = new APSSpriteFrame ( (APSDictionary*) *iter, this );
            this->m_aFrames.push_back ( pFrame );
        }
    }
    
    return bOk;
}

KDvoid APSSpriteSheetHolder::preloadData ( KDvoid )
{
    this->getFrames ( );
}

KDvoid APSSpriteSheetHolder::clearCachedData ( KDvoid )
{
    APSImageHolder::clearCachedData ( );
	APS_FOREACH ( std::vector<APSSpriteFrame*>, this->m_aFrames, iter )
	{
        ( *iter )->setCCSpriteFrame ( KD_NULL );
    }
    this->setIsCached ( KD_FALSE );
}

CCSpriteFrame* APSSpriteSheetHolder::getCCSpriteFrameAtIndex ( KDuint uIndex )
{
	std::vector<APSSpriteFrame*>*  pFrames = this->getFrames ( );
    
    KDuint  uCount = pFrames->size ( );
    
    if ( uIndex >= uCount )
	{
        uIndex = uCount - 1;
    }
    
    return pFrames->at ( uIndex )->getCCSpriteFrame ( );
}

const KDchar* APSSpriteSheetHolder::getRuntimePlPath ( KDvoid )
{
    if ( !m_pRuntimePlPath ) 
	{
		m_pRuntimePlPath = new std::string ( );
        this->getPlFilenameWithImageFilename ( m_pRuntimePlPath, this->getRuntimeFullPath ( )->c_str ( ) );
    }
    return m_pRuntimePlPath->c_str ( );
}

KDvoid APSSpriteSheetHolder::getPlFilenameWithImageFilename ( std::string* pPlFilename, const KDchar* szImageFile )
{
    *pPlFilename = szImageFile;

    KDint  nIdx  = pPlFilename->rfind ( "." );
    KDint  nSize = pPlFilename->size ( );
    pPlFilename->replace ( nIdx + 1, nSize - nIdx - 1, APSSpriteSheetHolder::getPlFileExtension ( ) );
}

std::vector<APSSpriteFrame*>* APSSpriteSheetHolder::getFrames ( KDbool bLoadCache )
{
    if ( bLoadCache && !this->getIsCached ( ) )
	{
        CCSpriteFrameCache*  pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
        pSpriteFrameCache->addSpriteFramesWithFile ( this->getRuntimePlPath ( ) );
        
		APS_FOREACH ( std::vector<APSSpriteFrame*>, this->m_aFrames, iter )
		{
            ( *iter )->setCCSpriteFrame ( pSpriteFrameCache->spriteFrameByName ( ( *iter )->getInternalName ( ) ) );
        }
        this->setIsCached ( KD_TRUE );
    }

    return &this->m_aFrames;
}

NS_APS_END
