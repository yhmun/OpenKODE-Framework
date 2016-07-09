/* -----------------------------------------------------------------------------------
 *
 *      File            Player.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Player.h"

enum
{
	DEFAULT		= -1,
	STAND		=  0,		// default state
	MOVING			,
	STAND_ATTACK	,
	MOVING_ATTACK	,
	BE_HITTED		,
	DEAD
};

Player* Player::create ( KDvoid )
{
	Player*		pRet = new Player ( );

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

KDbool Player::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	initArmature ( );
	
	m_pPlayerStateMachine = new PlayerStateMachine ( );

	setGameOver ( false );
	m_nLifeNum = 2;
	m_nPlayerState = DEFAULT;
	playerRebirth ( );
	Size tSize = this->getContentSize ( );
	scheduleUpdate ( );
	return true;
}

KDvoid Player::initArmature ( KDvoid )
{
	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "armature/player/player0.png", "armature/player/player0.plist","armature/player/player.ExportJson" );
	
	m_pPlayerArmature = Armature::create ( "player" );
	m_pPlayerArmature->setScaleX ( -0.1f );
	m_pPlayerArmature->setScaleY (  0.1f );
	m_pPlayerArmature->getAnimation ( )->play ( "stand" );
	this->addChild ( m_pPlayerArmature );
	m_pPlayerArmature->getAnimation ( )->setMovementEventCallFunc ( this, movementEvent_selector ( Player::animationEvent ) );
}

KDvoid Player::playerRebirth ( KDvoid )
{
	m_nHitPonts = PLAYER_MAX_HP;
	m_pPlayerArmature->setPosition ( Point ( 50, 200 ) );
	m_pPlayerStateMachine->reset ( );
}

KDbool Player::isLock ( KDvoid )
{
	KDbool	bLock = ( ( m_nPlayerState == BE_HITTED )|| ( m_nPlayerState == DEAD ) );
	return bLock;
}

KDvoid Player::update ( KDfloat fDelta )
{
	updatePosition ( );

	KDint	nState = m_pPlayerStateMachine->getState ( );
	if ( ( m_nPlayerState == nState ) || isLock ( ) )
	{
		return;
	}
	else
	{
		m_nPlayerState = nState;
	}

	updateAnimation ( );
}

KDvoid Player::updatePosition ( KDvoid )
{
	if ( isLock ( ) )
	{
		return;
	}

	// update face direction
	if ( m_pPlayerStateMachine->getMovePosition ( ).x > 0 ) 
	{
		m_pPlayerArmature->setScaleX ( -0.1f );
	}
	else if ( m_pPlayerStateMachine->getMovePosition ( ).x < 0 )
	{
		m_pPlayerArmature->setScaleX ( 0.1f );
	}
	//update position
	Point	tPosition = m_pPlayerArmature->getPosition ( );
	Point	tDeltaPosition = m_pPlayerStateMachine->getMovePosition ( );
	//the edge of screen
	if ( ( ( tPosition.x >= 20 ) && ( tDeltaPosition.x < 0 ) ) ||
		 ( ( tPosition.x <= 720 ) && ( tDeltaPosition.x > 0 ) ) )
	{
		tPosition.x += tDeltaPosition.x * 2;
	}
		
	if ( ( ( tPosition.y <= 200 ) && ( tDeltaPosition.y > 0 ) ) ||
		 ( ( tPosition.y >= 50 ) && ( tDeltaPosition.y < 0 ) ) )
	{
		tPosition.y += m_pPlayerStateMachine->getMovePosition ( ).y * 2;
	}
	
	m_pPlayerArmature->setPosition ( tPosition );
	this->setZOrder ( (KDint) Director::getInstance( )->getVisibleSize ( ).height - tPosition.y );
}

KDvoid Player::updateAnimation ( KDvoid )
{
	switch ( m_nPlayerState )
	{
		case STAND :
			m_pPlayerArmature->getAnimation ( )->play ( "stand" );
			break;

		case MOVING :
			m_pPlayerArmature->getAnimation ( )->play ( "walk" );
			break;

		case STAND_ATTACK :
			m_pPlayerArmature->getAnimation ( )->play ( "stand_fire" );
			break;

		case MOVING_ATTACK :
			m_pPlayerArmature->getAnimation ( )->play ( "walk_fire" );
			break;

		default :
			break;
	}
}

KDvoid Player::beHitted ( KDvoid )
{
	// player is locked or still in default init
	if ( isLock ( ) || ( m_nPlayerState == DEFAULT ) )
	{
		return;
	}
	
	m_nHitPonts -= 20;
	if ( m_nHitPonts > 0 )
	{
		m_nPlayerState = BE_HITTED;
		m_pPlayerArmature->getAnimation ( )->play ( "hitted" );
	}
	else
	{
		m_nPlayerState = DEAD;
		m_pPlayerArmature->getAnimation ( )->play ( "dead" );
	}
}

KDvoid Player::animationEvent ( Armature* pArmature, MovementEventType eMovementType, const KDchar* pMovementID )
{
	std::string		sId = pMovementID;
	if ( eMovementType == LOOP_COMPLETE )
	{
		if ( sId.compare ( "hitted" ) == 0 )
		{
			m_nPlayerState = DEFAULT;
		}
		
		if ( sId.compare ( "dead" ) == 0 )
		{
			m_nPlayerState = DEFAULT;
			if ( m_nLifeNum > 0 )  
			{
				m_nLifeNum--;
				playerRebirth ( );
			}
			else
			{
				setGameOver ( true );
			}
		}
	}
}


Player::~Player ( KDvoid )
{
	CC_SAFE_DELETE ( m_pPlayerStateMachine );
}

PlayerStateMachine::PlayerStateMachine ( KDvoid )
{
	m_nPlayerState   = 0;
	m_bPressJoystick = 0;
	m_bPressButton   = 0;
	m_tMovePosition  = Point::ZERO; 
}

KDint PlayerStateMachine::getState ( KDvoid )
{
	if ( m_bPressJoystick )
	{
		if ( m_bPressButton )
		{
			m_nPlayerState = MOVING_ATTACK;
		}
		else 
		{
			m_nPlayerState = MOVING;
		}
	}
	else
	{
		if ( m_bPressButton )
		{
			m_nPlayerState = STAND_ATTACK;
		}
		else
		{
			m_nPlayerState = STAND;
		}
	}

	return m_nPlayerState;
}


PlayerStateMachine::~PlayerStateMachine ( KDvoid )
{

}


