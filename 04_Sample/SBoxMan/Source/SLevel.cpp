/* --------------------------------------------------------------------------
 *
 *      File            SLevel.cpp
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
#include "SLevel.h"

SLevel* SLevel::create ( CCDictionary* pDictionary )
{
	SLevel*		pRet = new SLevel ( );

	if ( pRet && pRet->initWithDictionary ( pDictionary ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SLevel::SLevel ( KDvoid )
{
	m_pMapElements = KD_NULL;
	m_pMapString   = KD_NULL;
}

SLevel::~SLevel ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pMapElements );
	CC_SAFE_RELEASE ( m_pMapString );
}

KDbool SLevel::initWithDictionary ( CCDictionary* pDictionary )
{
	CCString*	pTemp;

	pTemp = (CCString*) pDictionary->objectForKey ( "map" );
	m_sMapName = pTemp->getCString ( );

	pTemp = (CCString*) pDictionary->objectForKey ( "level" );
	m_nLevel = pTemp->intValue ( );

	pTemp = (CCString*) pDictionary->objectForKey ( "backgroundMusic" );
	m_sBackgroundMusic = pTemp ? pTemp->getCString ( ) : "background.wav";
	
	pTemp = (CCString*) pDictionary->objectForKey ( "backgroundThumb" );
	m_sBackgroundThumb = pTemp ? pTemp->getCString ( ) : "thumb_level.png";

	pTemp = (CCString*) pDictionary->objectForKey ( "pushEffect" );
	m_sPushEffect = pTemp ? pTemp->getCString ( ) : "push_box.wav";

	pTemp = (CCString*) pDictionary->objectForKey ( "winEffect" );
	m_sWinEffect = pTemp ? pTemp->getCString ( ) : "win.wav";

	m_pMapElements = CCArray::createWithCapacity ( 10 );
	m_pMapElements->retain ( );

	m_pMapString = CCString::createWithContentsOfFile ( m_sMapName.c_str ( ) );
	m_pMapString->retain ( );

	this->resetMapElements ( );

	return KD_TRUE;
}

KDvoid SLevel::resetMapElements ( KDvoid )
{
	m_pMapElements->removeAllObjects ( );

	KDchar*		szTemp = kdStrdup ( m_pMapString->getCString ( ) );
	CCArray*	pRows  = CCArray::create ( );
	KDchar*		szPtr  = kdStrtok ( szTemp, "\n" );

	do
	{
		pRows->addObject ( CCString::create ( szPtr ) );

	} while ( ( szPtr = kdStrtok ( KD_NULL, "\n" ) ) );

	kdFree ( szTemp );
	
	CCObject*	pObject;
	CCARRAY_FOREACH ( pRows, pObject )
	{
		szTemp = kdStrdup ( ( (CCString*) pObject )->getCString ( ) );
		szPtr  = kdStrtok ( szTemp, "," );

		CCArray*	pColumns = CCArray::createWithCapacity ( 10 );
		do
		{
			pColumns->addObject ( CCString::create ( szPtr ) );

		} while ( ( szPtr = kdStrtok ( KD_NULL, "," ) ) );

		m_pMapElements->addObject ( pColumns );

		kdFree ( szTemp );
	}
}

CCDictionary* SLevel::toDictionary ( KDvoid )
{
	CCDictionary*	pDict = CCDictionary::create ( ); 

	pDict->setObject ( CCString::create ( m_sBackgroundMusic		), "backgroundMusic" );
	pDict->setObject ( CCString::create ( m_sPushEffect				), "pushEffect" );
//	pDict->setObject ( CCString::create ( m_sWinEffect ) );
	pDict->setObject ( CCString::createWithFormat ( "%d", m_nLevel	), "level" );
	pDict->setObject ( CCString::create ( m_sMapName				), "map" );

	return pDict;
}

const KDchar* SLevel::getBackgroundMusic ( KDvoid )
{
	return m_sBackgroundMusic.c_str ( );
}

const KDchar* SLevel::getPushEffect ( KDvoid )
{
	return m_sPushEffect.c_str ( );
}

const KDchar* SLevel::getWinEffect ( KDvoid )
{
	return m_sWinEffect.c_str ( );
}

const KDchar* SLevel::getBackgroundThumb ( KDvoid )
{
	return m_sBackgroundThumb.c_str ( );
}

KDint SLevel::getLevel ( KDvoid )
{
	return m_nLevel;
}

CCArray* SLevel::getMapElements ( KDvoid )
{
	return m_pMapElements;
}