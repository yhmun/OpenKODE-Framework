/* --------------------------------------------------------------------------
 *
 *      File            CCNodeLoaderLibrary.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
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
#include "extensions/CCBReader/CCNodeLoaderLibrary.h"
#include "extensions/CCBReader/CCLayerLoader.h"
#include "extensions/CCBReader/CCLayerColorLoader.h"
#include "extensions/CCBReader/CCLayerGradientLoader.h"
#include "extensions/CCBReader/CCLabelBMFontLoader.h"
#include "extensions/CCBReader/CCLabelTTFLoader.h"
#include "extensions/CCBReader/CCSpriteLoader.h"
#include "extensions/CCBReader/CCScale9SpriteLoader.h"
#include "extensions/CCBReader/CCBFileLoader.h"
#include "extensions/CCBReader/CCMenuLoader.h"
#include "extensions/CCBReader/CCMenuItemLoader.h"
#include "extensions/CCBReader/CCMenuItemImageLoader.h"
#include "extensions/CCBReader/CCControlButtonLoader.h"
#include "extensions/CCBReader/CCParticleSystemQuadLoader.h"
#include "extensions/CCBReader/CCScrollViewLoader.h"

NS_CC_BEGIN

CCNodeLoaderLibrary::~CCNodeLoaderLibrary ( KDvoid ) 
{
    this->purge ( KD_TRUE );
}

KDvoid CCNodeLoaderLibrary::registerDefaultCCNodeLoaders ( KDvoid )
{
    this->registerCCNodeLoader ( "CCNode"				, CCNodeLoader				::loader ( ) );
    this->registerCCNodeLoader ( "CCLayer"				, CCLayerLoader				::loader ( ) );
    this->registerCCNodeLoader ( "CCLayerColor"			, CCLayerColorLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCLayerGradient"		, CCLayerGradientLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCSprite"				, CCSpriteLoader			::loader ( ) );
    this->registerCCNodeLoader ( "CCLabelBMFont"		, CCLabelBMFontLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCLabelTTF"			, CCLabelTTFLoader			::loader ( ) );
    this->registerCCNodeLoader ( "CCScale9Sprite"		, CCScale9SpriteLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCScrollView"			, CCScrollViewLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCBFile"				, CCBFileLoader				::loader ( ) );
    this->registerCCNodeLoader ( "CCMenu"				, CCMenuLoader				::loader ( ) );
    this->registerCCNodeLoader ( "CCMenuItemImage"		, CCMenuItemImageLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCControlButton"		, CCControlButtonLoader		::loader ( ) );
    this->registerCCNodeLoader ( "CCParticleSystemQuad"	, CCParticleSystemQuadLoader::loader ( ) );
}

KDvoid CCNodeLoaderLibrary::registerCCNodeLoader ( const KDchar* szClassName, CCNodeLoader* pCCNodeLoader ) 
{
    pCCNodeLoader->retain ( );
    this->m_aCCNodeLoaders.insert ( CCNodeLoaderMapEntry ( szClassName, pCCNodeLoader ) );
}

KDvoid CCNodeLoaderLibrary::unregisterCCNodeLoader ( const KDchar* szClassName ) 
{
    CCNodeLoaderMap::iterator  ccNodeLoadersIterator = this->m_aCCNodeLoaders.find ( szClassName );
    if ( ccNodeLoadersIterator != this->m_aCCNodeLoaders.end ( ) )
	{
		ccNodeLoadersIterator->second->release ( );
		m_aCCNodeLoaders.erase ( ccNodeLoadersIterator );
	}
	else
    {
        CCLOG ( "The loader (%s) doesn't exist", szClassName );
    }
}

CCNodeLoader* CCNodeLoaderLibrary::getCCNodeLoader ( const KDchar* szClassName )
{
    CCNodeLoaderMap::iterator  ccNodeLoadersIterator = this->m_aCCNodeLoaders.find ( szClassName );
    CCAssert ( ccNodeLoadersIterator != this->m_aCCNodeLoaders.end ( ), "" );
    return ccNodeLoadersIterator->second;
}

KDvoid CCNodeLoaderLibrary::purge ( KDbool pReleaseCCNodeLoaders )
{
    if ( pReleaseCCNodeLoaders ) 
	{
        for ( CCNodeLoaderMap::iterator it = this->m_aCCNodeLoaders.begin ( ); it != this->m_aCCNodeLoaders.end ( ); it++ )
		{
            it->second->release ( );
        }
    }
    this->m_aCCNodeLoaders.clear ( );
}



static CCNodeLoaderLibrary*   l_pSharedCCNodeLoaderLibrary = KD_NULL;

CCNodeLoaderLibrary* CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary ( KDvoid ) 
{
    if ( l_pSharedCCNodeLoaderLibrary == KD_NULL ) 
	{
        l_pSharedCCNodeLoaderLibrary = new CCNodeLoaderLibrary ( );
        l_pSharedCCNodeLoaderLibrary->registerDefaultCCNodeLoaders ( );
    }

    return l_pSharedCCNodeLoaderLibrary;
}

KDvoid CCNodeLoaderLibrary::purgeSharedCCNodeLoaderLibrary ( KDvoid )
{
    CC_SAFE_DELETE ( l_pSharedCCNodeLoaderLibrary );
}

CCNodeLoaderLibrary* CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary ( KDvoid )
{
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::library ( );
    
    ccNodeLoaderLibrary->registerDefaultCCNodeLoaders ( );

    return ccNodeLoaderLibrary;
}

NS_CC_END