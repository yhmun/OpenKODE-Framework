/* --------------------------------------------------------------------------
 *
 *      File            SLevelManager.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-07
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "SLevelManager.h"
#include "SLevel.h"

static SLevelManager*	l_pLevelManager = KD_NULL;

SLevelManager* SLevelManager::sharedLevelManager ( KDvoid )
{
	if ( !l_pLevelManager )
	{
		l_pLevelManager = new SLevelManager ( );
		l_pLevelManager->init ( );
	}

	return l_pLevelManager;
}

SLevelManager::SLevelManager ( KDvoid )
{
	m_pLevelArray   = KD_NULL;
	m_pLevel		= KD_NULL;
	m_nCurrentLevel = 0;
}

SLevelManager::~SLevelManager ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLevelArray );
}

KDbool SLevelManager::init ( KDvoid )
{
	m_pLevelArray = CCArray::createWithCapacity ( 5 );
	m_pLevelArray->retain ( ); 

	CCDictionary*	pDictionary = CCDictionary::createWithContentsOfFile ( "boxman.plist" );
	CCArray*		pLevels = (CCArray*) pDictionary->objectForKey ( "Levels" );

	CCObject*		pObject;
	CCARRAY_FOREACH ( pLevels, pObject )
	{
		CCDictionary*	pDict  = (CCDictionary*) pObject;
		SLevel*			pLevel = SLevel::create ( pDict );

		m_pLevelArray->addObject ( pLevel );
	}

	m_nCurrentLevel = CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( "level", 0 );
	m_pLevel = (SLevel*) m_pLevelArray->objectAtIndex ( m_nCurrentLevel );

	return KD_TRUE;
}

CCArray* SLevelManager::getLevelArray ( KDvoid )
{
	return m_pLevelArray;
}

KDbool SLevelManager::hasPrevLevel ( KDvoid )
{
	 return m_nCurrentLevel > 0;
}

KDbool SLevelManager::hasNextLevel ( KDvoid )
{
	return m_nCurrentLevel < (KDint) m_pLevelArray->count ( ) - 1;
}

SLevel* SLevelManager::prevLevel ( KDvoid )
{
	KDint	nPrevLevel = m_nCurrentLevel - 1;
    if ( nPrevLevel >= 0 )
	{
        m_nCurrentLevel -= 1;
		m_pLevel = (SLevel*) m_pLevelArray->objectAtIndex ( nPrevLevel );
        return m_pLevel;
    }

	return KD_NULL;
}

SLevel* SLevelManager::nextLevel ( KDvoid )
{
    KDint	nNextLevel = m_nCurrentLevel + 1;
    if ( nNextLevel < (KDint) m_pLevelArray->count ( ) )
	{
        m_nCurrentLevel += 1;
		m_pLevel = (SLevel*) m_pLevelArray->objectAtIndex ( nNextLevel );
        return m_pLevel;
    } 

	return KD_NULL;
}

SLevel* SLevelManager::getLevel ( KDvoid )
{
	return m_pLevel;
}

KDint SLevelManager::getCurrentLevel ( KDvoid )
{
	return m_nCurrentLevel;
}

KDvoid SLevelManager::setCurrentLevel ( KDint nCurrentLevel )
{
    if ( m_pLevelArray->count ( ) == 0 )
	{
        return;
    }

    m_nCurrentLevel = KD_MAX ( 0, nCurrentLevel );
    m_nCurrentLevel = KD_MIN ( (KDint) m_pLevelArray->count ( ) - 1, nCurrentLevel );
	m_pLevel = (SLevel*) m_pLevelArray->objectAtIndex ( m_nCurrentLevel );
}
