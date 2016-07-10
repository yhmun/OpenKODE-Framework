/* --------------------------------------------------------------------------
 *
 *      File            XMContactFilter.cpp
 *      Description     XMContactFilter
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
#include "XMContactFilter.h"
#include "Node/Character/Player/XMPlayer.h"
#include "Node/Character/Enemy/XMEnemy.h"
#include "Node/Bullet/Player/XMPlayerBullet.h"
#include "Node/Bullet/Enemy/XMEnemyBullet.h"

KDbool XMContactFilter::ShouldCollide ( b2Fixture* pFixtureA, b2Fixture* pFixtureB )
{
	m_pFixtureA = pFixtureA;
	m_pFixtureB = pFixtureB;

	m_tFilterA = m_pFixtureA->GetFilterData ( );
	m_tFilterB = m_pFixtureB->GetFilterData ( );

	XMPlayer*		pPlayer = KD_NULL;
	XMEnemy*		pEnemy  = KD_NULL;
	XMEnemyBullet*	pEBullet = KD_NULL;
	
	Node* FixtureAUserData = (Node*) m_pFixtureA->GetUserData ( );
	Node* FixtureBUserData = (Node*) m_pFixtureB->GetUserData ( );

	// 같은 그룹에 속해 있으면 충돌 아님 ( 아군-아군무기-아군총알 / 적군-적군무기-적군총알 )
	if ( m_tFilterA.groupIndex == m_tFilterB.groupIndex )
	{
		return KD_FALSE;
	}

	// 아군 무기 - 적군 총알
	if ( ( m_tFilterA.categoryBits == CATEGORY_PWEAPON && m_tFilterB.categoryBits == CATEGORY_EBULLET ) ||
		 ( m_tFilterA.categoryBits == CATEGORY_EBULLET && m_tFilterB.categoryBits == CATEGORY_PWEAPON ) )
	{
		return KD_FALSE;
	}

	// 적군 무기 - 아군 총알
	if ( ( m_tFilterA.categoryBits == CATEGORY_EWEAPON && m_tFilterB.categoryBits == CATEGORY_PBULLET ) ||
		 ( m_tFilterA.categoryBits == CATEGORY_PBULLET && m_tFilterB.categoryBits == CATEGORY_EWEAPON ) )
	{
		return KD_FALSE;
	}

	// 아군무기 - 적군 무기
	if ( ( m_tFilterA.categoryBits == CATEGORY_EWEAPON && m_tFilterB.categoryBits == CATEGORY_PWEAPON ) ||
		 ( m_tFilterA.categoryBits == CATEGORY_PWEAPON && m_tFilterB.categoryBits == CATEGORY_EWEAPON ) )
	{
		return KD_FALSE;
	}

	Bone*			pBone1 = KD_NULL;
	Bone*			pBone2 = KD_NULL;
	XMBullet*		pBullet = KD_NULL;

	if ( ( pBone1 = dynamic_cast< Bone* > ( FixtureAUserData ) ) && ( pBone2 = dynamic_cast< Bone* > ( FixtureBUserData ) ) )
	{
		//근접 공격
		if ( ( ( pPlayer = dynamic_cast< XMPlayer* > ( pBone1->getArmature ( )->getParent ( ) ) ) && ( pEnemy = dynamic_cast< XMEnemy* > ( pBone2->getArmature ( )->getParent ( ) ) ) ) || 
			 ( ( pPlayer = dynamic_cast< XMPlayer* > ( pBone2->getArmature ( )->getParent ( ) ) ) && ( pEnemy = dynamic_cast< XMEnemy* > ( pBone1->getArmature ( )->getParent ( ) ) ) ) )

		{
			if ( ( m_tFilterA.categoryBits == CATEGORY_PWEAPON && m_tFilterB.categoryBits == CATEGORY_ENEMY ) ||
				 ( m_tFilterA.categoryBits == CATEGORY_ENEMY && m_tFilterB.categoryBits == CATEGORY_PWEAPON ) )
			{
				if ( pPlayer->isSlashActive ( ) && pEnemy->isActive ( ) )
				{
					return KD_TRUE;
				}
			}

			if ( ( m_tFilterA.categoryBits == CATEGORY_EWEAPON && m_tFilterB.categoryBits == CATEGORY_PLAYER ) ||
				 ( m_tFilterA.categoryBits == CATEGORY_PLAYER && m_tFilterB.categoryBits == CATEGORY_EWEAPON ) )
			{
				if ( pEnemy->isSlashActive ( ) && pPlayer->isActive ( ) )
				{
					return KD_TRUE;
				}
			}
		}
	}
	else if ( ( pBone1 = dynamic_cast< Bone* > ( FixtureAUserData ) ) && ( pBullet = dynamic_cast< XMBullet* > ( FixtureBUserData ) ) ||
			  ( pBone1 = dynamic_cast< Bone* > ( FixtureBUserData ) ) && ( pBullet = dynamic_cast< XMBullet* > ( FixtureAUserData ) ) )
	{
		// 원거리 공격
		return KD_TRUE;
	}

	return KD_FALSE;
}