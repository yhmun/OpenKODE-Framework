/* --------------------------------------------------------------------------
 *
 *      File            XMGlobalManager.cpp
 *      Description     XMGlobalManager
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
#include "XMGlobalManager.h"
#include "Node/Character/Enemy/XMEnemy.h"
#include "Node/Character/Enemy/XMGoblin.h"
#include "Node/Character/Enemy/XMGoblinRider.h"
#include "Node/Character/Enemy/XMJaco.h"

XMGlobalManager*		GLOBAL = KD_NULL;

XMGlobalManager::XMGlobalManager ( KDvoid )
{
	m_pEnemyContainer = Array::create ( );
	m_pEnemyContainer->retain ( );

	m_nStage = 0;
	m_nLevel = 0;
	m_nScore = 0;
}

XMGlobalManager::~XMGlobalManager ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pEnemyContainer );
}

KDvoid XMGlobalManager::create ( KDvoid )
{
	if ( GLOBAL == KD_NULL )
	{
		GLOBAL = new XMGlobalManager ( );
	}


	// 미리 로딩...
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "Knight/Knight.ExportJson" );
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "Goblin/Goblin.ExportJson" );
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "Jaco/Jaco.ExportJson" );
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "GoblinRider/GoblinRider.ExportJson" );


	GLOBAL->initEnemyMap ( );	
}

KDvoid XMGlobalManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( GLOBAL );
}

KDvoid XMGlobalManager::resetData ( KDvoid )
{
	m_nStage = 1;
	m_nLevel = 1;
	m_nScore = 0;

	if ( m_pEnemyContainer )
	{
		m_pEnemyContainer->removeAllObjects ( );
	}
}

KDvoid XMGlobalManager::initEnemyMap ( KDvoid )
{
	m_AenemyMap [ "Goblin" ]		= CL ( XMGoblin );
	m_AenemyMap [ "Jaco" ]			= CL ( XMJaco );
	m_AenemyMap [ "GoblinRider" ]	= CL ( XMGoblinRider );
}

Array* XMGlobalManager::getEnemyContainer ( KDvoid )
{
	return m_pEnemyContainer;
}

std::map <std::string, std::function<XMEnemy*( b2World*, String* )>> XMGlobalManager::getEnemyMap ( KDvoid )
{
	return m_AenemyMap;
}

KDvoid XMGlobalManager::setStage ( KDint nStage )
{
	m_nStage = nStage;
}
KDint XMGlobalManager::getStage ( KDvoid )
{
	return m_nStage;
}

KDvoid XMGlobalManager::setLevel ( KDint nLevel )
{
	m_nLevel = nLevel;
}

KDint XMGlobalManager::getLevel ( KDvoid )
{
	return m_nLevel;
}

KDvoid XMGlobalManager::setScore ( KDint nScore )
{
	m_nScore = nScore;
}

KDint XMGlobalManager::getScore ( KDvoid )
{
	return m_nScore;
}

KDvoid XMGlobalManager::addScore ( KDint nAddScore )
{
	m_nScore += nAddScore;
}