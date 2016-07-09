/* --------------------------------------------------------------------------
 *
 *      File            KWStateManager.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/23
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "KWStateManager.h"
#include "KWState.h"

KWStateManager*	create ( KWState* pState )
{
	KWStateManager*		pRet = new KWStateManager ( );

	if ( pRet && pRet->initWithInitialState ( pState ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWStateManager*	create ( KWState* pState, CCDictionary* pUserData )
{
	KWStateManager*		pRet = new KWStateManager ( );

	if ( pRet && pRet->initWithInitialState ( pState, pUserData ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWStateManager::KWStateManager ( KDvoid )
{
	m_pRunningState = KD_NULL;
	m_pStateStack   = KD_NULL;
}

KWStateManager::~KWStateManager ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pRunningState );
	CC_SAFE_RELEASE ( m_pStateStack );
}

KDbool KWStateManager::init ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pRunningState );
	CC_SAFE_RELEASE ( m_pStateStack );

	m_pStateStack = CCArray::create ( );

	return KD_TRUE;
}

KDbool KWStateManager::initWithInitialState ( KWState* pState )
{
	return this->initWithInitialState ( pState, CCDictionary::create ( ) );
}

KDbool KWStateManager::initWithInitialState ( KWState* pState, CCDictionary* pUserData )
{
	if ( !this->init ( ) )
	{
		return KD_FALSE;
	}

	this->pushState ( pState, pUserData );

	return KD_TRUE;
}

KDvoid KWStateManager::pushState ( KWState* pState )
{
	this->pushState ( pState, CCDictionary::create ( ) );
}

KDvoid KWStateManager::pushState ( KWState* pState, CCDictionary* pUserData )
{
	CC_SAFE_RELEASE ( m_pRunningState );
	m_pRunningState = pState;
	CC_SAFE_RETAIN  ( m_pRunningState );

	m_pStateStack->addObject ( pState );

	pState->setUp ( pUserData );
}

KDvoid KWStateManager::replaceState ( KWState* pState )
{
	this->replaceState ( pState, CCDictionary::create ( ) );
}

KDvoid KWStateManager::replaceState ( KWState* pState, CCDictionary* pUserData )
{
	m_pRunningState->tearDown ( );

	CC_SAFE_RELEASE ( m_pRunningState );
	m_pRunningState = pState;
	CC_SAFE_RETAIN  ( m_pRunningState );

	m_pStateStack->removeLastObject ( );
	m_pStateStack->addObject ( pState );

	m_pRunningState->setUp ( pUserData );
}

KDvoid KWStateManager::popState ( KDvoid )
{
	m_pRunningState->tearDown ( );

	m_pStateStack->removeLastObject ( );

	CC_SAFE_RELEASE ( m_pRunningState );
	m_pRunningState = (KWState*) m_pStateStack->lastObject ( );
	CC_SAFE_RETAIN  ( m_pRunningState );
}
