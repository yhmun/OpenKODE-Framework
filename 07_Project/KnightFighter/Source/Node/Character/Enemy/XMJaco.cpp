/* --------------------------------------------------------------------------
 *
 *      File            XMJaco.cpp
 *      Description     XMJaco
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
#include "XMJaco.h"
#include "Node/Character/Player/XMPlayer.h"
#include "Node/Bullet/Enemy/XMSmallBullet.h"

XMJaco::XMJaco ( b2World* pWorld, String* sType )
{
	m_pParentWorld = pWorld;
	if ( sType )
	{
		kdStrcpy ( m_sType, sType->getCString ( ) );
	}
}

XMJaco::~XMJaco ( KDvoid )
{

}

XMJaco* XMJaco::create ( b2World* pWorld, String* sType )
{
	XMJaco* pRet = new XMJaco ( pWorld, sType );

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

KDbool XMJaco::init ( KDvoid )
{
	if ( !XMEnemy::initWithContentsOfFile ( "Jaco/Jaco.plist" ) )
	{
		return KD_FALSE;
	}

	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		m_pArmature->setScale ( 0.5f );
	}
	else
	{
		m_pArmature->setScale ( 0.3f );
	}

	return KD_TRUE;
}


KDvoid XMJaco::playAI ( XMPlayer* pPlayer )
{
	m_pPlayer = pPlayer;

	this->schedule ( schedule_selector ( XMJaco::doAI ), 0.2f );
	
}

KDvoid XMJaco::doAI ( KDfloat fDelta )
{
	m_pArmature->getB2Body ( )->SetActive ( KD_TRUE );

	if ( m_bIsPlayAI == KD_TRUE )
	{
		return;
	}

	if ( m_pPlayer->isActive ( ) == KD_FALSE )
	{
		unscheduleAllSelectors ( );
		return;
	}

	// 20%의 확률로 공격을 하고 지나가버린다.
	m_bIsPlayAI = KD_TRUE;

	Array*			pActionArray = Array::create ( );
	KDint			nRand = kdRand ( ) % 100;
	KDint			nRandDestY = kdRand ( ) % (KDint) m_tContentSize.height;
	KDint			nPercentage = 20 + ( GLOBAL->getStage ( ) * 10 ) + GLOBAL->getLevel ( ); 
	
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		nPercentage *= 2;
	}

	if ( nRand < nPercentage )
	{
		Spawn*		pSpawn = Spawn::create
		(
			Sequence::create
			(
				CallFunc::create
				(
					[ & ] ( )
					{
						XMSmallBullet*		pBullet = XMSmallBullet::create ( m_pParentWorld );
						pBullet->setPosition ( m_pArmature->getPosition ( ) );
						this->getParent ( )->addChild ( pBullet );

						pBullet->shootAiming ( m_pPlayer->getArmature ( )->getPosition ( ) );
						playShootAnimation ( );
					}
				),
				nullptr
			),
			EaseIn::create ( MoveTo::create ( 2, Point ( -200, nRandDestY ) ), 2 ),
			nullptr
		);

		pActionArray->addObject ( pSpawn );
	}
	else
	{
		EaseIn*		pEaseIn = EaseIn::create ( MoveTo::create ( 2, Point ( -200, nRandDestY ) ), 2 );

		pActionArray->addObject ( pEaseIn );
	}

	CallFunc*		pCallFunc = CallFunc::create
	(
		[ & ] ( )
		{ 
			GLOBAL->getEnemyContainer ( )->removeObject ( this );
			removeFromParent ( );
		}
	);

	pActionArray->addObject ( pCallFunc );

	this->m_pArmature->runAction
	(
		Sequence::create ( pActionArray )
	);
}

