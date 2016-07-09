/* --------------------------------------------------------------------------
 *
 *      File            Minimap.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Minimap.h"

Minimap* Minimap::create ( KDvoid )
{
	Minimap*	pRet = new Minimap ( );

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

Minimap::Minimap ( KDvoid )
{
	m_pWallVertices1	= KD_NULL;
	m_pWallVertices2	= KD_NULL;
	m_pProjectiles		= KD_NULL;
	m_pStaticObjects	= KD_NULL;
}

Minimap::~Minimap ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pWallVertices1 );
	CC_SAFE_RELEASE ( m_pWallVertices2 );
	CC_SAFE_RELEASE ( m_pProjectiles   );
	CC_SAFE_RELEASE ( m_pStaticObjects );
}

KDbool Minimap::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->setWallVertices1 ( CCArray::create ( ) );
	this->setWallVertices2 ( CCArray::create ( ) );
	this->setStaticObjects ( CCArray::create ( ) );
	this->setProjectiles   ( CCDictionary::create ( ) );

	m_fScale = 2.5f;

	return KD_TRUE;
}

KDvoid Minimap::draw ( KDvoid )
{			
	// Draw walls
	ccLineWidth ( 2.0f );
	
	for ( KDuint i = 0; i < m_pWallVertices1->count ( ); i++ )
	{	
		CCPoint		tPoint1 = ( (CCPointValue*) m_pWallVertices1->objectAtIndex ( i ) )->getValue ( );
		CCPoint		tPoint2 = ( (CCPointValue*) m_pWallVertices2->objectAtIndex ( i ) )->getValue ( );
		
		ccDrawColor4B ( 96, 96, 96, 0 );
		ccDrawLine ( ccpMult ( tPoint1, m_fScale ), ccpMult ( tPoint2, m_fScale ) );
	}

	// Draw projectiles
	ccPointSize ( 4 );
	CCArray*	pKeys = m_pProjectiles->allKeys ( );
	CCObject*	pObject;
	CCARRAY_FOREACH ( pKeys, pObject )
	{
		CCString*		pKey   = (CCString*) pObject;
		CCPointValue*	pValue = (CCPointValue*) m_pProjectiles->objectForKey ( pKey->getCString ( ) );
		
		ccDrawColor4B ( 255, 128, 0, 128 );
		ccDrawPoint ( ccpMult ( pValue->getValue ( ), m_fScale ) );	

	}

	// Draw static objects
	ccPointSize ( 4 );
	for ( KDuint i = 0; i < m_pStaticObjects->count ( ); i++ )
	{	
		CCPoint		tPoint = ( (CCPointValue*) m_pStaticObjects->objectAtIndex ( i ) )->getValue ( );
		
		ccDrawColor4B ( 96, 96, 96, 0 );
		ccDrawPoint ( ccpMult ( tPoint, m_fScale ) );	
	}

	// Draw Actor
	ccPointSize ( 6 );
	ccDrawColor4B ( 0, 64, 255, 255 );
	ccDrawPoint ( ccpMult ( m_tActor, m_fScale ) );	
}

KDvoid Minimap::addWallWithVertex1 ( const CCPoint& tVertex1, const CCPoint& tVertex2 )
{
	m_pWallVertices1->addObject ( CCPointValue::create ( tVertex1 ) ); 
	m_pWallVertices2->addObject ( CCPointValue::create ( tVertex2 ) ); 
}

KDvoid Minimap::addStaticObject ( const CCPoint& tPoint )
{
	m_pStaticObjects->addObject ( CCPointValue::create ( tPoint ) ); 
}

KDvoid Minimap::setProjectile ( const CCPoint& tPoint, const KDchar* szKey )
{
	m_pProjectiles->setObject ( CCPointValue::create ( tPoint ), szKey ); 
}

const CCPoint& Minimap::getActor ( KDvoid )
{
	return m_tActor;
}

KDvoid Minimap::setActor ( const CCPoint& tPoint )
{
	m_tActor = tPoint;
}

CCArray* Minimap::getWallVertices1 ( KDvoid )
{
	return m_pWallVertices1;
}

KDvoid Minimap::setWallVertices1 ( CCArray* pWallVertices1 )
{
	CC_SAFE_RELEASE ( m_pWallVertices1 );
	m_pWallVertices1 = pWallVertices1;
	CC_SAFE_RETAIN  ( m_pWallVertices1 );
}

CCArray* Minimap::getWallVertices2 ( KDvoid )
{
	return m_pWallVertices2;
}

KDvoid Minimap::setWallVertices2 ( CCArray* pWallVertices2 )
{
	CC_SAFE_RELEASE ( m_pWallVertices2 );
	m_pWallVertices2 = pWallVertices2;
	CC_SAFE_RETAIN  ( m_pWallVertices2 );
}

CCDictionary* Minimap::getProjectiles ( KDvoid )
{
	return m_pProjectiles;
}

KDvoid Minimap::setProjectiles ( CCDictionary* pProjectiles )
{
	CC_SAFE_RELEASE ( m_pProjectiles );
	m_pProjectiles = pProjectiles;
	CC_SAFE_RETAIN  ( m_pProjectiles );
}

CCArray* Minimap::getStaticObjects ( KDvoid )
{
	return m_pStaticObjects;
}

KDvoid Minimap::setStaticObjects ( CCArray* pStaticObjects )
{
	CC_SAFE_RELEASE ( m_pStaticObjects );
	m_pStaticObjects = pStaticObjects;
	CC_SAFE_RETAIN  ( m_pStaticObjects );
}

KDfloat Minimap::getScale ( KDvoid )
{
	return m_fScale;
}

KDvoid Minimap::setScale ( KDfloat fScale )
{
	m_fScale = fScale;
}