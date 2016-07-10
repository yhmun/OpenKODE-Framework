/* --------------------------------------------------------------------------
 *
 *      File            XMPlayer.cpp
 *      Description     XMPlayer
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
#include "XMPlayer.h"
#include "Node/Character/Enemy/XMEnemy.h"

XMPlayer::XMPlayer ( KDvoid )
{
	m_eCharacterState = CHARACTERSTATE_IDLE;
	m_nDirection = NO_DIRECTION;
	m_tResetTime = 0;
}

XMPlayer::~XMPlayer ( KDvoid )
{

}

KDbool XMPlayer::initWithContentsOfFile ( const std::string& filename )
{
	if ( !XMCharacter::initWithContentsOfFile ( filename ) )
	{
		return KD_FALSE;
	}

	this->playIdleAnimation ( );

	return KD_TRUE;
}

KDbool XMPlayer::shoot ( KDvoid )
{
	if ( !XMCharacter::shoot ( ) )
	{
		return KD_FALSE;
	}

	if ( m_eCharacterState != CHARACTERSTATE_IDLE )
	{
		if ( m_eCharacterState != CHARACTERSTATE_SHOOT )
		{
			return KD_FALSE;
		}
	}

	if ( m_eCharacterState != CHARACTERSTATE_SHOOT )
	{
		playShootAnimation ( );
	}

	KDfloat tCurrentTime = kdGetMilliseconds ( );

	if ( m_tResetTime + 200 < tCurrentTime )
	{
		m_tResetTime = kdGetMilliseconds ( );
		shootBullet ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid XMPlayer::setFilterInfo ( KDvoid )
{
	ColliderFilter		pColliderFilter;
	pColliderFilter.setCategoryBits ( CATEGORY_PLAYER );
	pColliderFilter.setGroupIndex ( GROUPINDEX_PLAYER );
	m_pArmature->setColliderFilter ( &pColliderFilter );

	ColliderFilter*		pColliderFilter2 = m_pArmature->getBone ( "WEAPON" )->getColliderFilter ( );
	pColliderFilter2->setCategoryBits ( CATEGORY_PWEAPON );
	m_pArmature->getBone ( "WEAPON" )->setColliderFilter ( pColliderFilter2 );


	// 충돌대 위치 재정렬
	Array*	pDisplayList = m_pArmature->getBone ( "WEAPON" )->getDisplayManager ( )->getDecorativeDisplayList ( );

    for ( auto pDisplayObject: *pDisplayList )
    {
        ColliderDetector*	pDetector = static_cast<DecorativeDisplay*> ( pDisplayObject )->getColliderDetector ( );
        if ( pDetector != nullptr )
        {
			pDetector->setActive ( true );
            pDetector->setActive ( false );
        }
    }
}

KDvoid XMPlayer::playIdleAnimation ( KDvoid )
{
	XMCharacter::playIdleAnimation ( );

	Object*				pObject = KD_NULL;
	CCARRAY_FOREACH ( GLOBAL->getEnemyContainer ( ), pObject )
	{
		XMEnemy*		pEnemy = ( XMEnemy* ) pObject;
		if ( pEnemy->getArmature ( )->getAnimation ( )->getCurrentMovementID ( ).find ( "IDLE" ) == string::npos )
		{
			if ( pEnemy->getArmature ( )->getAnimation ( )->getCurrentMovementID ( ).find ( "SLASH_1" ) == string::npos )
			{
				if ( pEnemy->getArmature ( )->getB2Body ( )->IsActive ( ) == KD_FALSE )
				{
					pEnemy->getArmature ( )->getB2Body ( )->SetActive ( KD_TRUE );
					pEnemy->getArmature ( )->getAnimation ( )->play ( "IDLE" );
				}
				
			}
		}
	}
}

KDvoid XMPlayer::waitSlash ( KDvoid )
{
	XMCharacter::waitSlash ( );

	Object* pObject = KD_NULL;
	CCARRAY_FOREACH ( GLOBAL->getEnemyContainer ( ), pObject )
	{
		XMEnemy* pEnemy = ( XMEnemy* ) pObject;
		
		if ( pEnemy->getArmature ( )->getAnimation ( )->getCurrentMovementID ( ).find ( "IDLE" ) == string::npos )
		{
			if ( pEnemy->getArmature ( )->getAnimation ( )->getCurrentMovementID ( ).find ( "SLASH_1" ) == string::npos )
			{
				if ( pEnemy->getArmature ( )->getB2Body ( )->IsActive ( ) == KD_FALSE )
				{
					pEnemy->getArmature ( )->getB2Body ( )->SetActive ( KD_TRUE );
					pEnemy->getArmature ( )->getAnimation ( )->play ( "IDLE" );
				}
			}
		}
	}
}

KDbool XMPlayer::slash ( KDvoid )
{
	if ( !XMCharacter::slash ( ) )
	{
		return KD_FALSE;
	}
	
	if ( m_bIsSlashNow == KD_TRUE )
	{
		return KD_FALSE;
	}

	if ( m_eCharacterState == CHARACTERSTATE_RECOVERY )
	{
		m_nSlashState = m_nSlashVariety < m_nSlashState + 1 ? SLASHSTATE_SLASH1 : m_nSlashState + 1;
	}
	else if ( m_eCharacterState == CHARACTERSTATE_IDLE )
	{
		m_nSlashState = SLASHSTATE_SLASH1;
	}
	else if ( m_eCharacterState == CHARACTERSTATE_SHOOT )
	{
		m_nSlashState = SLASHSTATE_SLASH1;
	}

	playSlashAnimation ( m_nSlashState );

	return KD_TRUE;
}

KDvoid XMPlayer::move ( KDint nDirection )
{
	XMCharacter::move ( nDirection );

	if ( m_eCharacterState != CHARACTERSTATE_IDLE )
	{
		return;
	}
	
	switch ( nDirection )
	{
		case RIGHT :
		case UP_RIGHT :
		case DOWN_RIGHT :
			m_pArmature->getAnimation ( )->play ( "FORWARD" );
			break;
		case LEFT :
		case UP_LEFT :
		case DOWN_LEFT :
			m_pArmature->getAnimation ( )->play ( "BACKWARD" );
			break;
		default:
			if ( m_nDirection != nDirection )
			{
				playIdleAnimation ( );
			}

			
			break;
	}

	m_nDirection = nDirection;
}

KDvoid XMPlayer::hitted ( KDvoid )
{
	XMCharacter::hitted ( );
	this->scheduleOnce ( schedule_selector ( XMPlayer::recovery ), 1 );
}

KDvoid XMPlayer::recovery ( KDfloat fDelta )
{
	this->getArmature ( )->getB2Body ( )->SetActive ( KD_TRUE );
	this->playIdleAnimation ( );
}