/* --------------------------------------------------------------------------
 *
 *      File            XMXMCharacter.cpp
 *      Description     XMCharacter
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
#include "XMCharacter.h"

XMCharacter::XMCharacter ( KDvoid )
{
	m_pCharacterName = KD_NULL;
	m_pCharacterDict = KD_NULL;
}
XMCharacter::~XMCharacter ( KDvoid )
{	
	m_pParentWorld->DestroyBody ( this->m_pArmature->getB2Body ( ) );

	CC_SAFE_RELEASE ( m_pCharacterName );
	CC_SAFE_RELEASE ( m_pCharacterDict );
}

KDbool XMCharacter::initWithContentsOfFile ( const std::string& filename )
{
	if ( !Layer::init ( ) )
	{
		return KD_FALSE;
	}

	if ( !this->loadContentsOfFile ( filename ) )
	{
		return KD_FALSE;
	}

	this->initVariable ( );
	this->initArmature ( );
	
	MoveBy*		pMoveBy = MoveBy::create ( 2.0f, Point ( 0, 10 ) );
	this->m_pArmature->runAction
	(
		RepeatForever::create ( Sequence::create ( pMoveBy, pMoveBy->reverse ( ), nullptr ) )
	);

	return KD_TRUE;
}

KDvoid XMCharacter::initVariable ( KDvoid )
{
	m_pCharacterName	= (String*) m_pCharacterDict->objectForKey ( "Name" );
	m_bIsActive			= m_pCharacterDict->valueForKey ( "IsActive" )->boolValue ( );
	m_fMaxHP			= m_pCharacterDict->valueForKey ( "MaxHP" )->intValue ( );
	m_fCurrentHP		= m_pCharacterDict->valueForKey ( "CurrentHP" )->intValue ( );
	m_fMoveSpeed		= m_pCharacterDict->valueForKey ( "MoveSpeed" )->floatValue ( );
	m_fShootDamage		= m_pCharacterDict->valueForKey ( "ShootDamage" )->intValue ( );
	m_fSlashDamage		= m_pCharacterDict->valueForKey ( "SlashDamage" )->intValue ( );
	m_nSlashVariety		= m_pCharacterDict->valueForKey ( "SlashVariety" )->intValue ( );
	m_bIsPossibleShoot	= m_pCharacterDict->valueForKey ( "PossibleShoot" )->boolValue ( );
	m_bIsPossibleSlash	= m_pCharacterDict->valueForKey ( "PossibleSlash" )->boolValue ( );
	m_bIsSlashNow		= KD_FALSE;
	m_bIsSlashActive	= KD_FALSE;

	m_pCharacterName->retain ( );
}

KDvoid XMCharacter::initArmature ( KDvoid )
{
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( ccszf ( "%s/%s.ExportJson", m_pCharacterName->getCString ( ), m_pCharacterName->getCString ( ) ) );
	
	m_pArmature = Armature::create ( m_pCharacterName->getCString ( ) );
	m_pArmature->getAnimation ( )->setMovementEventCallFunc ( this, movementEvent_selector ( XMCharacter::onMovementEvent ) );
	m_pArmature->getAnimation ( )->setFrameEventCallFunc ( this, frameEvent_selector ( XMCharacter::onFrameEvent ) );
	this->addChild ( m_pArmature );

	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	b2Body*			pBody = m_pParentWorld->CreateBody ( &tBodyDef );

	m_pArmature->setB2Body ( pBody );
	
	this->setFilterInfo ( );
}

KDbool XMCharacter::loadContentsOfFile ( const std::string& filename )
{
	m_pCharacterDict = Dictionary::createWithContentsOfFile ( filename.c_str ( ) );

	if ( !m_pCharacterDict )
	{
		return KD_FALSE;
	}
	m_pCharacterDict->retain ( );

	return KD_TRUE;
}

KDvoid XMCharacter::playIdleAnimation ( KDvoid )
{
	m_bIsSlashNow = KD_FALSE;
	m_bIsSlashActive = KD_FALSE;
	m_eCharacterState = CHARACTERSTATE_IDLE;
	m_pArmature->getAnimation ( )->setSpeedScale ( 1.0f );
	m_pArmature->getAnimation ( )->play ( "IDLE" );
}

KDvoid XMCharacter::playShootAnimation ( KDint nShootState )
{
	m_eCharacterState = CHARACTERSTATE_SHOOT;
	m_pArmature->getAnimation ( )->play ( ccszf ( "SHOOT_%d", nShootState ) );
}

KDvoid XMCharacter::playSlashAnimation ( KDint nSlashState )
{	
	m_pArmature->getAnimation ( )->play ( ccszf ( "SLASH_%d", nSlashState ) );
	m_eCharacterState = CHARACTERSTATE_SLASH;
	m_bIsSlashNow = KD_TRUE;
}

// 이벤트 시작과 끝에 호출됨.
KDvoid XMCharacter::onMovementEvent ( Armature* pArmature, MovementEventType eMovementType, const KDchar* pMovementID )
{
	if ( kdStrcmp ( pMovementID, "SLASH_1" ) == 0 )
	{
		switch ( eMovementType )
		{
			case cocostudio::START:
				break;
			case cocostudio::COMPLETE:
			case cocostudio::LOOP_COMPLETE:
				playIdleAnimation ( );
				break;
			default:
				break;
		}
	}
	else if ( kdStrcmp ( pMovementID, "SLASH_2" ) == 0 )
	{
		switch ( eMovementType )
		{
			case cocostudio::START:
				break;
			case cocostudio::COMPLETE:
			case cocostudio::LOOP_COMPLETE:
				playIdleAnimation ( );
				break;
			default:
				break;
		}
	}
	else if ( kdStrcmp ( pMovementID, "SLASH_3" ) == 0 )
	{
		switch ( eMovementType )
		{
			case cocostudio::START:
				break;
			case cocostudio::COMPLETE:
			case cocostudio::LOOP_COMPLETE:
				playIdleAnimation ( );
				break;
			default:
				break;
		}
	}
	else if ( kdStrcmp ( pMovementID, "SHOOT_1" ) == 0 )
	{
		switch ( eMovementType )
		{
			case cocostudio::START:
				break;
			case cocostudio::COMPLETE:
			case cocostudio::LOOP_COMPLETE:
				playIdleAnimation ( );
				break;
			default:
				break;
		}
	}
	else if ( kdStrcmp ( pMovementID, "SHOOT_2" ) == 0 )
	{
		switch ( eMovementType )
		{
			case cocostudio::START:
				break;
			case cocostudio::COMPLETE:
			case cocostudio::LOOP_COMPLETE:
				playIdleAnimation ( );
				break;
			default:
				break;
		}
	}
	
}

// 툴에서 적용해놓은 프레임에서 호출됨
KDvoid XMCharacter::onFrameEvent ( Bone* pBone, const KDchar* szEvt, KDint nOriginFrameIndex, KDint nCurrentFrameIndex )
{
	if ( kdStrcmp ( szEvt, "START_SLASH" ) == 0 )
	{
		m_bIsSlashActive = KD_TRUE;
	}
	else if ( kdStrcmp ( szEvt, "END_SLASH" ) == 0 )
	{
		waitSlash ( );
		m_bIsSlashActive = KD_FALSE;
	}
	else if ( kdStrcmp ( szEvt, "MOVE_FORWARD" ) == 0 )
	{
		if ( m_pArmature->getPosition ( ).x + 10 < m_tContentSize.width )
		{
			this->getArmature ( )->runAction ( MoveBy::create ( 0.2f, Point ( 10, 0 ) ) );
		}
		
	}
}

KDvoid XMCharacter::waitSlash ( KDvoid )
{
	m_eCharacterState = CHARACTERSTATE_RECOVERY;
	m_bIsSlashNow = KD_FALSE;
}

KDbool XMCharacter::isActive ( KDvoid )
{
	return m_bIsActive;
}

KDvoid XMCharacter::setActive ( KDbool bActive )
{
	m_bIsActive = bActive;
}

KDfloat XMCharacter::getMaxHP ( KDvoid )
{
	return m_fMaxHP;
}

KDvoid XMCharacter::setMaxHP ( KDfloat nMaxHP )
{
	m_fMaxHP = nMaxHP;
}

KDfloat XMCharacter::getCurrentHP ( KDvoid )
{
	return m_fCurrentHP;
}

KDvoid XMCharacter::setCurrentHP ( KDfloat nCurrentHP )
{
	m_fCurrentHP = nCurrentHP;
}

KDfloat XMCharacter::getMoveSpeed ( KDvoid )
{
	return m_fMoveSpeed;
}

KDvoid XMCharacter::setMoveSpeed ( KDfloat fMoveSpeed )
{
	m_fMoveSpeed = fMoveSpeed;
}

KDfloat XMCharacter::getShootDamage ( KDvoid )
{
	return m_fShootDamage;
}

KDvoid XMCharacter::setShootDamage ( KDfloat nShootDamage )
{
	m_fShootDamage = nShootDamage;
}

KDfloat XMCharacter::getSlashDamage ( KDvoid )
{
	return m_fSlashDamage;
}

KDvoid XMCharacter::setSlashDamage ( KDfloat nSlashDamage )
{
	m_fSlashDamage = nSlashDamage;
}

KDbool XMCharacter::isSlashNow ( KDvoid )
{
	return m_bIsSlashNow;
}

KDvoid XMCharacter::setSlashNow ( KDbool bIsSlashNow )
{
	m_bIsSlashNow = bIsSlashNow;
}

KDbool XMCharacter::isSlashActive ( KDvoid )
{
	return m_bIsSlashActive;
}

KDbool XMCharacter::isPossibleShoot ( KDvoid )
{
	return m_bIsPossibleShoot;
}

KDvoid XMCharacter::setPossibleShoot ( KDbool bPossibleShoot )
{
	m_bIsPossibleShoot = bPossibleShoot;
}

KDbool XMCharacter::isPossibleSlash ( KDvoid )
{
	return m_bIsPossibleSlash;
}

KDvoid XMCharacter::setPossibleSlash ( KDbool bPossibleSlash )
{
	m_bIsPossibleSlash = bPossibleSlash;
}

KDvoid XMCharacter::move ( KDint nDirection )
{
	KDfloat		fX = 0, fY = 0;
	KDfloat		fMoveSpeed = m_fMoveSpeed;

	switch ( nDirection )
	{
	case LEFT:
		fX = -fMoveSpeed;
		break;
	case RIGHT:
		fX = fMoveSpeed;
		break;
	case UP:
		fY = fMoveSpeed;
		break;
	case DOWN:
		fY = -fMoveSpeed;
		break;
	case UP_LEFT:
		fX = -fMoveSpeed / kdSqrt ( 2 );
		fY = fMoveSpeed / kdSqrt ( 2 );
		break;
	case DOWN_LEFT:
		fX = -fMoveSpeed / kdSqrt ( 2 );
		fY = -fMoveSpeed / kdSqrt ( 2 );
		break;
	case UP_RIGHT:
		fX = fMoveSpeed / kdSqrt ( 2 );
		fY = fMoveSpeed / kdSqrt ( 2 );
		break;
	case DOWN_RIGHT:
		fX = fMoveSpeed / kdSqrt ( 2 );
		fY = -fMoveSpeed / kdSqrt ( 2 );
		break;
	default:
		return;
	}

	Point		tPosition = m_pArmature->getPosition ( );

	tPosition.x += fX;
	tPosition.y += fY;

	if ( tPosition.x < 0 )
	{
		tPosition.x = 0;
	}
	else if ( tPosition.x > m_tContentSize.width )
	{
		tPosition.x = m_tContentSize.width;
	}

	if ( tPosition.y < 0 )
	{
		tPosition.y = 0;
	}
	else if ( tPosition.y > m_tContentSize.height )
	{
		tPosition.y = m_tContentSize.height;
	}

	m_pArmature->setPosition ( tPosition );
}

KDbool XMCharacter::shoot ( KDvoid )
{
	return m_bIsPossibleShoot;
}

KDbool XMCharacter::slash ( KDvoid )
{
	return m_bIsPossibleSlash;
}

KDvoid XMCharacter::hitted ( KDvoid )
{
	m_pArmature->getAnimation ( )->play ( "HITTED" );
	m_pArmature->setOpacity ( 255 );
}

Armature* XMCharacter::getArmature ( KDvoid )
{
	return m_pArmature;
}

KDvoid XMCharacter::damaged ( KDint nDamage )
{
	m_fCurrentHP -= nDamage;
	
	KDint		nPercent = (KDint) ( (KDfloat) m_fCurrentHP / (KDfloat) m_fMaxHP * 100.0f );
	Color3B		tColor = Color3B::WHITE;

	if ( nPercent <= 100 && nPercent > 70 )
	{
		tColor = Color3B::WHITE;
	}
	else if ( nPercent <= 70 && nPercent > 30 )
	{
		tColor = Color3B::YELLOW;
	}
	else if ( nPercent <= 30 )
	{
		tColor = Color3B::RED;
	}

	this->getArmature ( )->runAction
	(
		Sequence::create
		(
			TintTo::create ( 0.05f, tColor.r, tColor.g, tColor.b ),
			TintTo::create ( 0.05f, 255, 255, 255 ),
			nullptr
		)
	);
	
}

String* XMCharacter::getPlayerName ( KDvoid )
{
	return m_pCharacterName;
}

b2World* XMCharacter::getb2World ( )
{
	return m_pParentWorld;
}