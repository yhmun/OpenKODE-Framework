/* --------------------------------------------------------------------------
 *
 *      File            QueuedAction.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "QueuedAction.h"

QueuedAction* QueuedAction::create ( KDvoid )
{
	QueuedAction*	pRet = new QueuedAction ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

GameObject* QueuedAction::getGameObject ( KDvoid )
{
	return m_pGameObject;
}

KDvoid QueuedAction::setGameObject ( GameObject* pObject )
{
	m_pGameObject = pObject;
}

CCAction* QueuedAction::getAction ( KDvoid )
{
	return m_pAction;
}

KDvoid QueuedAction::setAction ( CCAction* pAction )
{
	m_pAction = pAction;
}