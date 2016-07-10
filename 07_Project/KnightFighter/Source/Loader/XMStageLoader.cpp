/* --------------------------------------------------------------------------
 *
 *      File            XMStageLoader.cpp
 *      Description     XMStageLoader
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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
#include "XMStageLoader.h"

XMStageLoader::XMStageLoader ( KDvoid )
{
	m_pLoadArray = KD_NULL;
	m_pObject = KD_NULL;

	m_nSequence = 0;
	m_nLoaderState = LOADERSTATE_NONE;
	m_nStageState = STAGESTATE_NONE;
}

XMStageLoader::~XMStageLoader ( KDvoid )
{
	//m_pLoadArray->removeAllObjects ( );
	CC_SAFE_RELEASE ( m_pLoadArray );
}

KDvoid XMStageLoader::loadStage ( KDint nStage )
{
	CC_SAFE_RELEASE ( m_pLoadArray );

	const KDchar* sFileName = ccszf ( "Stage/Stage_%d.plist", nStage );
	m_pLoadArray = Array::createWithContentsOfFile ( sFileName );
	m_pLoadArray->retain ( );
}

KDvoid XMStageLoader::update ( KDvoid )
{
	if ( m_pLoadArray == KD_NULL )
	{
		return;
	}

	if ( m_nLoaderState == LOADERSTATE_NONE )
	{
		m_nSequence = 0;
		m_nLoaderState = LOADERSTATE_ROLL;
	}
	else if ( m_nLoaderState == LOADERSTATE_ROLL )
	{
		Dictionary* pDict = (Dictionary*) m_pLoadArray->getObjectAtIndex ( m_nSequence++ );

		String*		pTodo = (String*) pDict->objectForKey ( "TODO" );
		
		if ( pTodo->isEqual ( String::create ( "START" ) ) )
		{
			m_nStageState = STAGESTATE_START;
		}
		else if ( pTodo->isEqual ( String::create ( "ADD ENEMY" ) ) )
		{
			m_nStageState = STAGESTATE_ADDENEMY;

			m_pObject = (Dictionary*) pDict->objectForKey ( "OBJECT" );
		}
		else if ( pTodo->isEqual ( String::create ( "END" ) ) )
		{
			m_nStageState = STAGESTATE_END;
		}
	}
	else if ( m_nLoaderState == LOADERSTATE_WAIT )
	{
		m_nStageState = STAGESTATE_WAIT;
	}
}

KDvoid XMStageLoader::setSequence ( KDint nSequence )
{
	m_nSequence = nSequence;
}

KDint XMStageLoader::getSequence ( KDvoid )
{
	return m_nSequence;
}

KDvoid XMStageLoader::setLoaderState ( KDint nLoaderState )
{
	m_nLoaderState = nLoaderState;
}

KDint XMStageLoader::getLoaderState ( KDvoid )
{
	return m_nLoaderState;
}

KDvoid XMStageLoader::setStageState ( KDint nStageState )
{
	m_nStageState = nStageState;
}

KDint XMStageLoader::getStageState ( KDvoid )
{
	return m_nStageState;
}

Object* XMStageLoader::getObject ( KDvoid )
{
	return m_pObject;
}