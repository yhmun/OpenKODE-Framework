/* --------------------------------------------------------------------------
 *
 *      File            XMEnemy.cpp
 *      Description     XMEnemy
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
#include "XMEnemy.h"
#include "Node/Character/Player/XMPlayer.h"

#include "Node/Bullet/Enemy/XMSmallBullet.h"
#include "Node/Bullet/Enemy/XMGoblinRiderBullet.h"

XMEnemy::XMEnemy ( KDvoid )
{
	m_sType = new char[64];
}

XMEnemy::~XMEnemy ( KDvoid )
{
	delete m_sType;
}

KDbool XMEnemy::initWithContentsOfFile ( const std::string& filename )
{
	if ( !XMCharacter::initWithContentsOfFile ( filename ) )
	{
		return KD_FALSE;
	}

	this->playIdleAnimation ( );

	return KD_TRUE;
}

KDvoid XMEnemy::initVariable ( KDvoid )
{
	XMCharacter::initVariable ( );

	m_nScore = m_pCharacterDict->valueForKey ( "Score" )->intValue ( );
	m_bIsPlayAI = KD_FALSE;
}

KDvoid XMEnemy::setFilterInfo ( KDvoid )
{
	ColliderFilter			pColliderFilter;
	pColliderFilter.setCategoryBits ( CATEGORY_ENEMY );
	pColliderFilter.setGroupIndex ( GROUPINDEX_ENEMY );
	m_pArmature->setColliderFilter ( &pColliderFilter );

	if ( m_pArmature->getBone ( "WEAPON" ) != KD_NULL )
	{
		ColliderFilter		pColliderFilter2;
		pColliderFilter2.setCategoryBits ( CATEGORY_EWEAPON );
		pColliderFilter2.setGroupIndex ( GROUPINDEX_ENEMY );
		m_pArmature->getBone ( "WEAPON" )->setColliderFilter ( &pColliderFilter2 );
	}
}

KDvoid XMEnemy::homingPlayer ( KDfloat fDelta )
{
	m_bIsPlayAI = KD_TRUE;

	Point		tPlayerPosition = m_pPlayer->getArmature ( )->getPosition ( );
	Point		tMyPosition = this->getArmature ( )->getPosition ( );

	if ( tPlayerPosition.x + 100 >= tMyPosition.x && tPlayerPosition.x < tMyPosition.x &&
		 tPlayerPosition.y + 50 >= tMyPosition.y && tPlayerPosition.y - 100 < tMyPosition.y )
	{
		this->playSlashAnimation ( 1 );
		m_bIsPlayAI = KD_FALSE;
		unschedule ( schedule_selector ( XMEnemy::homingPlayer ) );
		return;
	}

	if ( tPlayerPosition.x + 100 < tMyPosition.x )
	{
		tMyPosition.x -= 5;
	}
	else
	{
		tMyPosition.x += 5;
	}

	if ( tPlayerPosition.y < tMyPosition.y )
	{
		if ( tPlayerPosition.y + 5 <= tMyPosition.y )
		{
			tMyPosition.y -= 5;
		}
		else
		{
			tMyPosition.y -= tMyPosition.y - tPlayerPosition.y;
		}
		
	}
	else if ( tPlayerPosition.y > tMyPosition.y )
	{
		if ( tPlayerPosition.y - 5 >= tMyPosition.y )
		{
			tMyPosition.y += 5;
		}
		else
		{
			tMyPosition.y += tPlayerPosition.y - tMyPosition.y;
		}
		
	}

	this->getArmature ( )->setPosition ( tMyPosition );
}

KDint XMEnemy::getScore ( KDvoid )
{
	return m_nScore;
}

KDbool XMEnemy::slash ( KDvoid )
{
	playSlashAnimation ( 1 );

	return KD_TRUE;
}

CallFunc* XMEnemy::makeLastAction ( KDvoid )
{
	return CallFunc::create 
	(
		[ & ] ( )
		{
			m_bIsPlayAI = KD_FALSE;
		}
	);
}

FiniteTimeAction* XMEnemy::makeMoveRandom ( KDfloat fTime )
{
	KDint		nX =  CCRANDOM_BETWEEN ( 200,	m_tContentSize.width );
	KDint		nY =  CCRANDOM_BETWEEN (   0,	m_tContentSize.height );
	
	return EaseInOut::create ( MoveTo::create ( fTime, makeRandomPoint ( ) ), 2 );
}

FiniteTimeAction* XMEnemy::makeMoveLinear ( Point tPoint, KDfloat fTime )
{
	return EaseInOut::create ( MoveTo::create ( fTime, tPoint ), 2 );
}

FiniteTimeAction* XMEnemy::makeMoveEvade ( KDfloat fTime )
{
	KDint		nReturnX = CCRANDOM_BETWEEN ( m_tContentSize.width / 2,	m_tContentSize.width );
	KDint		nReturnY = kdRand ( ) % (KDint) m_tContentSize.height;
	Point		tReturnPoint = Point ( nReturnX, nReturnY );

	return JumpTo::create ( fTime, tReturnPoint, 50, 1 );
}

FiniteTimeAction* XMEnemy::makeMoveFlash ( Point tPoint, KDfloat fTime )
{
	FadeOut*	pFadeOutAction = FadeOut::create ( 0.3f );

	CallFunc*	pCallFunc1 = CallFunc::create
	( 
		[ & ] ( )
		{
			m_pArmature->getB2Body ( )->SetActive ( KD_FALSE );
		}
	);

	DelayTime*	pDelayTime = DelayTime::create ( fTime );

	MoveTo*		pMoveTo = MoveTo::create ( 0, tPoint );

	CallFunc*	pCallFunc2 = CallFunc::create
	( 
		[ & ] ( )
		{
			m_pArmature->getB2Body ( )->SetActive ( KD_TRUE );
		}
	);

	return Sequence::create ( pFadeOutAction, pCallFunc1, pDelayTime, pMoveTo, pCallFunc2, pFadeOutAction->reverse ( ), nullptr );
}

FiniteTimeAction* XMEnemy::makeMoveLeft ( Point tEnemyPoint, KDfloat fTime )
{
	KDfloat fRand = CCRANDOM_BETWEEN ( 0, tEnemyPoint.x ) ;
	tEnemyPoint.x = fRand;

	return EaseInOut::create ( MoveTo::create ( fTime, tEnemyPoint ), 1 );
}

FiniteTimeAction* XMEnemy::makeMoveRight ( Point tEnemyPoint, KDfloat fTime )
{
	KDfloat fRand = CCRANDOM_BETWEEN ( tEnemyPoint.x, m_tContentSize.width ) ;
	tEnemyPoint.x = fRand;

	return EaseInOut::create ( MoveTo::create ( fTime, tEnemyPoint ), 1 );
}

FiniteTimeAction* XMEnemy::makeMoveUp ( Point tEnemyPoint, KDfloat fTime )
{
	KDfloat fRand = CCRANDOM_BETWEEN ( tEnemyPoint.y, m_tContentSize.height ) ;
	tEnemyPoint.y = fRand;

	return EaseInOut::create ( MoveTo::create ( fTime, tEnemyPoint ), 1 );
}

FiniteTimeAction* XMEnemy::makeMoveDown	( Point tEnemyPoint, KDfloat fTime )
{
	KDfloat fRand = CCRANDOM_BETWEEN ( 0, tEnemyPoint.y ) ;
	tEnemyPoint.y = fRand;
	return EaseInOut::create ( MoveTo::create ( fTime, tEnemyPoint ), 1 );
}

Point XMEnemy::makeRandomPoint ( KDvoid )
{
	KDint		nX =  CCRANDOM_BETWEEN ( 200,	m_tContentSize.width );
	KDint		nY =  CCRANDOM_BETWEEN (   0,	m_tContentSize.height );

	return Point ( nX, nY );
}

XMPlayer* XMEnemy::getPlayer ( )
{
	return m_pPlayer;
}


KDfloat XMEnemy::getAngle ( Point tPoint1, Point tPoint2 )
{
	KDfloat			dx = tPoint2.x - tPoint1.x;
	KDfloat			dy = tPoint2.y - tPoint1.y;

	KDdouble		dRad = kdAtan2 ( dy, dx );
	KDdouble		dDegree = dRad * ( 180 / KD_PI_F );

	if ( dDegree < 0 )
	{
		dDegree += 360;
	}

	return dDegree;
}

KDfloat XMEnemy::getRadius ( Point tPoint1, Point tPoint2 )
{
	return kdSqrt ( kdPow ( tPoint2.x - tPoint1.x, 2 ) + kdPow ( tPoint2.y - tPoint1.y, 2 ) );
}

KDvoid XMEnemy::runActionByArray ( Array* pArray )
{
	kdAssert ( pArray != KD_NULL );

	for ( KDint i = 0; i < pArray->count ( ); i++ )
	{
		if ( FiniteTimeAction* pAction = dynamic_cast< FiniteTimeAction* > ( pArray->getObjectAtIndex ( i ) ) )
		{
			kdAssert ( pAction != KD_NULL );
		}
	}

	m_bIsPlayAI = KD_TRUE;

	pArray->addObject ( makeLastAction ( ) );
	
	m_pArmature->runAction ( Sequence::create ( pArray ) );
}

KDfloat XMEnemy::getMaxHP ( KDvoid )
{
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		return m_fMaxHP * BOSS_STATE_RATE;
	}
	return m_fMaxHP;
}

KDvoid XMEnemy::setMaxHP ( KDfloat nMaxHP )
{
	m_fMaxHP = nMaxHP;
}

KDfloat XMEnemy::getCurrentHP ( KDvoid )
{
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		return m_fCurrentHP * BOSS_STATE_RATE;
	}
	return m_fCurrentHP;
}
KDvoid XMEnemy::setCurrentHP ( KDfloat nCurrentHP )
{
	m_fCurrentHP = nCurrentHP;
}

KDfloat XMEnemy::getMoveSpeed ( KDvoid )
{
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		return m_fMoveSpeed * BOSS_STATE_RATE;
	}
	return m_fMoveSpeed;
}

KDvoid XMEnemy::setMoveSpeed ( KDfloat fMoveSpeed )
{
	m_fMoveSpeed = fMoveSpeed;
}

KDfloat XMEnemy::getShootDamage ( KDvoid )
{
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		return m_fShootDamage * BOSS_STATE_RATE;
	}
	return m_fShootDamage;
}

KDvoid XMEnemy::setShootDamage ( KDfloat nShootDamage )
{
	m_fShootDamage = nShootDamage;
}

KDfloat XMEnemy::getSlashDamage ( KDvoid )
{
	if ( kdStrcmp ( m_sType, "BOSS" ) == 0 )
	{
		return m_fSlashDamage * BOSS_STATE_RATE;
	}
	return m_fSlashDamage;
}

KDvoid XMEnemy::setSlashDamage ( KDfloat nSlashDamage )
{
	m_fSlashDamage = nSlashDamage;
}

XMEnemyBullet* XMEnemy::makeBullet ( BULLET_TYPE eBulletType )
{
	XMEnemyBullet*	pBullet = nullptr;
	switch ( eBulletType )
	{
		case BULLET_SMALL		: pBullet = XMSmallBullet::create ( m_pParentWorld );				break;
		case BULLET_GOBLINRIDER : pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );			break;

		default					: pBullet = XMSmallBullet::create ( m_pParentWorld );				break;
	}

	return pBullet;
}

KDvoid XMEnemy::shootLinear ( BULLET_TYPE eBulletType, KDfloat fSpeed )
{
	XMEnemyBullet*		pBullet = makeBullet ( eBulletType );
	pBullet->setPosition ( m_pArmature->getPosition ( ) );
	this->getParent ( )->addChild ( pBullet );

	pBullet->shootLinear ( SHOOTDIRECTION_LEFT, fSpeed );
	playShootAnimation ( );
}

KDvoid XMEnemy::shootAiming ( BULLET_TYPE eBulletType, KDfloat fSpeed )
{
	Point				tPoint = m_pPlayer->getArmature ( )->getPosition ( );

	XMEnemyBullet*		pBullet = makeBullet ( eBulletType );
	pBullet->setPosition ( m_pArmature->getPosition ( ) );
	this->getParent ( )->addChild ( pBullet );

	pBullet->shootAiming ( tPoint, fSpeed );
	playShootAnimation ( );
}

KDvoid XMEnemy::shootMulti ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dDegree )
{
	Point		tPoint1 = m_pArmature->getPosition ( );
	Point		tPoint2 = m_pPlayer->getArmature ( )->getPosition ( );

	KDdouble	dAngle = getAngle ( tPoint1, tPoint2 );
	KDint		nHalfBullet = nBulletCount / 2;
	KDdouble	r = getRadius ( tPoint1, tPoint2 );
	KDdouble	dMaxDegree;

	if ( nBulletCount % 2 == 0 )	// 礎熱
	{
		dMaxDegree = dAngle + ( dDegree * ( nHalfBullet - 1 ) ) + ( dDegree / 2.0f );
	}
	else	// �汝�
	{
		dMaxDegree = dAngle + ( dDegree * nHalfBullet );
	}

	Array*		pArray = Array::create ( );

	for ( KDint i = 0; i < nBulletCount; i++ )
	{
		CallFunc*		pCallFunc = CallFunc::create
		(
			[ &, tPoint1, r, dMaxDegree, dDegree, i ] ( )
			{
				Node*			pNode = this->getParent ( );
				XMEnemyBullet*	pBullet = makeBullet ( BULLET_SMALL );
		
				Point tPoint1 = m_pArmature->getPosition ( );
				Point tPoint2 = m_pPlayer->getArmature ( )->getPosition ( );

				pBullet->setPosition ( tPoint1 );
				pBullet->setDamage ( this->getShootDamage ( ) );

				pNode->addChild ( pBullet, ZORDER_BULLET );

				KDdouble		dCurrentDegree = dMaxDegree - ( dDegree * i );
				KDdouble		rad = dCurrentDegree * KD_PI_F / 180.0f;

				Point			tDestPoint = Point ( 0, 0 );
				tDestPoint.x = r * kdCos ( rad ) + tPoint1.x;
				tDestPoint.y = r * kdSin ( rad ) + tPoint1.y;

				pBullet->shootAiming ( tDestPoint );
			}
		);

		pArray->addObject ( pCallFunc );
	}

	playShootAnimation ( );

	this->runActionByArray ( pArray );
}

KDvoid XMEnemy::shootMultiDelay ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dDegree, KDfloat fDelay )
{
	Point		tPoint1 = m_pArmature->getPosition ( );
	Point		tPoint2 = m_pPlayer->getArmature ( )->getPosition ( );

	KDdouble	dAngle = getAngle ( tPoint1, tPoint2 );
	KDint		nHalfBullet = nBulletCount / 2;
	KDdouble	r = getRadius ( tPoint1, tPoint2 );
	KDdouble	dMaxDegree;

	if ( nBulletCount % 2 == 0 )	// 礎熱
	{
		dMaxDegree = dAngle + ( dDegree * ( nHalfBullet - 1 ) ) + ( dDegree / 2.0f );
	}
	else	// �汝�
	{
		dMaxDegree = dAngle + ( dDegree * nHalfBullet );
	}

	Array*		pActionArray = Array::create ( );

	for ( KDint i = 0; i < nBulletCount; i++ )
	{
		CallFunc* pCallFunc = CallFunc::create
		(
			[ &, tPoint1, r, dMaxDegree, dDegree, i ] ( )
			{
				Node*			pNode = this->getParent ( );
				XMEnemyBullet*	pBullet = makeBullet ( BULLET_SMALL );

				pBullet->setPosition ( tPoint1 );
				pBullet->setDamage ( this->getShootDamage ( ) );

				pNode->addChild ( pBullet, ZORDER_BULLET );

				KDdouble		dCurrentDegree = dMaxDegree - ( dDegree * i );
				KDdouble		rad = dCurrentDegree * KD_PI_F / 180.0f;

				Point			tDestPoint = Point ( 0, 0 );

				tDestPoint.x = r * kdCos ( rad ) + tPoint1.x;
				tDestPoint.y = r * kdSin ( rad ) + tPoint1.y;

				pBullet->shootAiming ( tDestPoint );
			}
		);

		pActionArray->addObject ( pCallFunc );
		pActionArray->addObject ( DelayTime::create ( fDelay ) );
	}
	playShootAnimation ( );

	this->runActionByArray ( pActionArray );
}

KDvoid XMEnemy::shootDivide ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dTotalDegree )
{
	Point		tPoint1 = m_pArmature->getPosition ( );
	Point		tPoint2 = m_pPlayer->getArmature ( )->getPosition ( );

	KDdouble	dAngle = getAngle ( tPoint1, tPoint2 );
	KDint		nHalfBullet = nBulletCount / 2;
	KDdouble	r = getRadius ( tPoint1, tPoint2 );
	
	KDdouble	dStartDegree;
	KDdouble	dIntervalDegree = dTotalDegree / nBulletCount;
	KDint		nI = 1;

	if ( nBulletCount % 2 == 0 )	// 礎熱
	{
		dStartDegree = dAngle + ( dIntervalDegree / 2 );
	}
	else
	{
		dStartDegree = dAngle;
	}

	Array*		pActionArray = Array::create ( );

	for ( KDint i = 0; i < nBulletCount; i++ )
	{
		dStartDegree = dStartDegree + ( dIntervalDegree * nI * i );
		nI = nI * -1;

		CallFunc*	pCallFunc = CallFunc::create
		(
			[ &, tPoint1, r, dStartDegree, dIntervalDegree, nI ] ( )
			{
				Node*				pNode = this->getParent ( );
				XMEnemyBullet*		pBullet = makeBullet ( BULLET_SMALL );

				pBullet->setPosition ( tPoint1 );
				pBullet->setDamage ( this->getShootDamage ( ) );

				pNode->addChild ( pBullet, ZORDER_BULLET );
		
				Point tDestPoint = Point ( 0, 0 );

				KDdouble rad = dStartDegree * KD_PI_F / 180.0f;

				tDestPoint.x = r * kdCos ( rad ) + tPoint1.x;
				tDestPoint.y = r * kdSin ( rad ) + tPoint1.y;

				pBullet->shootAiming ( tDestPoint );
			}
		);

		pActionArray->addObject ( pCallFunc );
	}

	playShootAnimation ( );

	this->runActionByArray ( pActionArray );
}

KDvoid XMEnemy::shootDivideDelay ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dTotalDegree, KDfloat fDelay )
{
	Point		tPoint1 = m_pArmature->getPosition ( );
	Point		tPoint2 = m_pPlayer->getArmature ( )->getPosition ( );

	KDdouble	dAngle = getAngle ( tPoint1, tPoint2 );
	KDint		nHalfBullet = nBulletCount / 2;
	KDdouble	r = getRadius ( tPoint1, tPoint2 );
	
	KDdouble	dStartDegree;
	KDdouble	dIntervalDegree = dTotalDegree / nBulletCount;
	KDint		nI = 1;

	if ( nBulletCount % 2 == 0 )	// 礎熱
	{
		dStartDegree = dAngle + ( dIntervalDegree / 2 );
	}
	else
	{
		dStartDegree = dAngle;
	}

	Array*		pActionArray = Array::create ( );

	for ( KDint i = 0; i < nBulletCount; i++ )
	{
		dStartDegree = dStartDegree + ( dIntervalDegree * nI * i );
		nI = nI * -1;

		CallFunc*	pCallFunc = CallFunc::create
		(
			[ &, tPoint1, r, dStartDegree, dIntervalDegree, nI ] ( )
			{
				Node*				pNode = this->getParent ( );
				XMEnemyBullet*		pBullet = makeBullet ( BULLET_SMALL );

				pBullet->setPosition ( tPoint1 );
				pBullet->setDamage ( this->getShootDamage ( ) );

				pNode->addChild ( pBullet, ZORDER_BULLET );
		
				Point tDestPoint = Point ( 0, 0 );

				KDdouble rad = dStartDegree * KD_PI_F / 180.0f;

				tDestPoint.x = r * kdCos ( rad ) + tPoint1.x;
				tDestPoint.y = r * kdSin ( rad ) + tPoint1.y;

				pBullet->shootAiming ( tDestPoint );
			}
		);

		pActionArray->addObject ( pCallFunc );
		pActionArray->addObject ( DelayTime::create ( fDelay ) );
	}

	playShootAnimation ( );

	this->runActionByArray ( pActionArray );
}