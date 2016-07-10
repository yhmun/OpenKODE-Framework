/* --------------------------------------------------------------------------
 *
 *      File            XMGoblinRider.cpp
 *      Description     XMGoblinRider
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
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
#include "XMGoblinRider.h"
#include "Node/Character/Player/XMPlayer.h"
#include "Node/Bullet/Enemy/XMGoblinRiderBullet.h"

XMGoblinRider::XMGoblinRider ( b2World* pWorld, String* sType )
{
	m_pParentWorld = pWorld;
	if ( sType )
	{
		kdStrcpy ( m_sType, sType->getCString ( ) );
	}
}

XMGoblinRider::~XMGoblinRider ( KDvoid )
{

}

XMGoblinRider* XMGoblinRider::create ( b2World* pWorld, String* sType )
{
	XMGoblinRider* pRet = new XMGoblinRider ( pWorld, sType );

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

KDbool XMGoblinRider::init ( KDvoid )
{
	if ( !XMEnemy::initWithContentsOfFile ( "GoblinRider/GoblinRider.plist" ) )
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


KDvoid XMGoblinRider::playAI ( XMPlayer* pPlayer )
{
	m_pPlayer = pPlayer;

	this->schedule ( schedule_selector ( XMGoblinRider::doAI ), 0.2f );
	
}

KDvoid XMGoblinRider::doAI ( KDfloat fDelta )
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
	
	if ( isPossibleShoot ( ) && isPossibleSlash ( ) )
	{
		KDint nRand = kdRand ( ) % 100;

		if ( nRand < 70 )
		{
			shoot ( );
		}
		else
		{
			slash ( );
		}
	}
	else if ( isPossibleShoot ( ) )
	{
		shoot ( );
	}
	else if ( isPossibleSlash ( ) )
	{	
		slash ( );
	}
	else 
	{
		// 공격을 하지 않고 지나가버린다.
		m_bIsPlayAI = KD_TRUE;
		
		KDint nRand = kdRand ( ) % (KDint) m_tContentSize.height;

		this->m_pArmature->runAction
		( 
			Sequence::create
			(
				EaseIn::create ( MoveTo::create ( 2, Point ( -200, nRand ) ), 2 ),
				CallFunc::create
				(
					[ & ] ( )
					{ 
						GLOBAL->getEnemyContainer ( )->removeObject ( this );
						removeFromParent ( );
					}
				),
				nullptr
			)
		);	
	}
}

KDbool XMGoblinRider::shoot ( KDvoid )
{
	KDint nRand    = kdRand ( ) % 100;
	KDint nRandSub = kdRand ( ) % 2;

	if ( nRand > 0 && nRand <= 25 )
	{
		if ( nRandSub == 1 )
		{
			this->ShootDiagonalFromTopToBottom ( );
		}
		else
		{
			this->ShootDiagonalFromTopToBottomDelay ( );
		}
	}
	else if ( nRand > 25 && nRand <=50 )
	{
		if ( nRandSub == 1 )
		{
			this->ShootDiagonalFromBottomToTop ( );
		}
		else
		{
			this->ShootDiagonalFromBottomToTopDelay ( );
		}
	} 
	else if ( nRand > 50 && nRand <=75 )
	{
		if ( nRandSub == 1 )
		{
			this->ShootLinear ( );
		}
		else
		{
			this->ShootDelay ( );
		}
	}
	else
	{

	}

	return KD_TRUE;
}

KDbool XMGoblinRider::slash ( KDvoid )
{
	KDint nRand = kdRand ( ) % 100;

	if ( nRand > 0 && nRand <= 30 )
	{
		this->slashWithLinearMove ( );
	}
	else if ( nRand > 30, nRand <= 60 )
	{
		this->slashWithMvoeAndThreehit ( );
	} 
	else
	{
		this->slashWithNotMove ( );
	}

	return KD_TRUE;
}

KDvoid XMGoblinRider::ShootLinear ( KDvoid )
{
	Array*		pActionArray = Array::create ( );

	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 1 );
			}
		),
		nullptr
	);
	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	CallFunc*	pCallFunc2 = CallFunc::create
	(
		[ & ] ( )
		{
			Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );

			for ( KDint i = -2; i <= 2; i++ )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				Point					tBulletPoint = Point ( 1300, tPlayerPoint.y + ( -100 * i ) );
				
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( 90 );

				pNode->addChild ( pBullet );

				pBullet->shootLinear ( SHOOTDIRECTION_LEFT, 960.0f );
			}
		}
	);
	pActionArray->addObject ( pCallFunc2 );

	DelayTime*	pDelayTime2 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime2 );

	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::ShootDelay ( KDvoid )
{
	Array*		pActionArray = Array::create ( );

	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 2 );
			}
		),
		nullptr
	);

	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	CallFunc*	pCallFunc2 = CallFunc::create
	(
		[ & ] ( )
		{
			Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );

			for ( KDint i = -2; i <= 2; i++ )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				Point					tBulletPoint = Point ( 1300, tPlayerPoint.y + ( -100 * i ) );
				
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( 90 );

				pNode->addChild ( pBullet );

				pBullet->shootLinear ( SHOOTDIRECTION_LEFT, 960.0f );
			}
		}
	);
	pActionArray->addObject ( pCallFunc2 );

	DelayTime*	pDelayTime2 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime2 );

	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::ShootDiagonalFromTopToBottom ( KDvoid )
{
	Array*		pActionArray = Array::create ( );
	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 1 );
			}
		),
		nullptr
	);

	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );
	Point		tStartPoint ;

	tStartPoint.x = m_tContentSize.width;
	tStartPoint.y = m_tContentSize.height;			
			
	KDdouble	dAngle   = getAngle  ( tStartPoint, tPlayerPoint );
	KDdouble	dDiameta = getRadius ( tStartPoint, tPlayerPoint );
	KDdouble	dRadian	 = dAngle * KD_PI_F / 180.0f;

	tStartPoint.x -= 200;
	tStartPoint.y += 200;

	Point		tBulletPoint;

	for ( KDint i = 0; i <= 4; i++ )
	{
		tBulletPoint = Point ( tStartPoint.x + ( 100 * i ), tStartPoint.y - ( 100 * i ) );

		CallFunc*	pCallFunc2 = CallFunc::create
		(
			[ &,tStartPoint, dAngle, dDiameta, dRadian, tBulletPoint ] ( )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( dAngle - 90 );
				pNode->addChild ( pBullet );

				Point tDestPoint = Point ( 0, 0 );
				tDestPoint.x = dDiameta * kdCos ( dRadian ) + tBulletPoint.x;
				tDestPoint.y = dDiameta * kdSin ( dRadian ) + tBulletPoint.y;
				pBullet->shootAiming ( tDestPoint, 960.0F );
			}
		);
		pActionArray->addObject ( pCallFunc2 );
	}	
	
	pActionArray->addObject ( pDelayTime1 );

	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::ShootDiagonalFromTopToBottomDelay	( KDvoid )
{
	Array*		pActionArray = Array::create ( );
	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 2 );
			}
		),
		nullptr
	);

	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );
	Point		tStartPoint ;

	tStartPoint.x = m_tContentSize.width;
	tStartPoint.y = m_tContentSize.height;			
			
	KDdouble	dAngle   = getAngle  ( tStartPoint, tPlayerPoint );
	KDdouble	dDiameta = getRadius ( tStartPoint, tPlayerPoint );
	KDdouble	dRadian	 = dAngle * KD_PI_F / 180.0f;

	tStartPoint.x -= 200;
	tStartPoint.y += 200;

	Point		tBulletPoint;

	for ( KDint i = 0; i <= 4; i++ )
	{
		tBulletPoint = Point ( tStartPoint.x + ( 100 * i ), tStartPoint.y - ( 100 * i ) );

		CallFunc*	pCallFunc2 = CallFunc::create
		(
			[ &,tStartPoint, dAngle, dDiameta, dRadian, tBulletPoint ] ( )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( dAngle - 90 );
				pNode->addChild ( pBullet );

				Point tDestPoint = Point ( 0, 0 );
				tDestPoint.x = dDiameta * kdCos ( dRadian ) + tBulletPoint.x;
				tDestPoint.y = dDiameta * kdSin ( dRadian ) + tBulletPoint.y;
				pBullet->shootAiming ( tDestPoint, 960.0F );
			}
		);

		pActionArray->addObject ( pCallFunc2 );
		pActionArray->addObject ( DelayTime::create ( 0.5f ) );
	}

	DelayTime*	pDelayTime2 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime2 );

	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::ShootDiagonalFromBottomToTop ( KDvoid )
{
	Array*		pActionArray = Array::create ( );
	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 1 );
			}
		),
		nullptr
	);

	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );
	Point		tStartPoint ;

	tStartPoint.x = m_tContentSize.width;
	tStartPoint.y = 0;			
			
	KDdouble	dAngle   = getAngle  ( tStartPoint, tPlayerPoint );
	KDdouble	dDiameta = getRadius ( tStartPoint, tPlayerPoint );
	KDdouble	dRadian	 = dAngle * KD_PI_F / 180.0f;

	tStartPoint.x -= 200;
	tStartPoint.y -= 200;

	Point		tBulletPoint;

	for ( KDint i = 0; i <= 4; i++ )
	{
		tBulletPoint = Point ( tStartPoint.x + ( 100 * i ), tStartPoint.y + ( 100 * i ) );

		CallFunc*	pCallFunc2 = CallFunc::create
		(
			[ &,tStartPoint, dAngle, dDiameta, dRadian, tBulletPoint ] ( )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( dAngle - 90 );
				pNode->addChild ( pBullet );

				Point tDestPoint = Point ( 0, 0 );
				tDestPoint.x = dDiameta * kdCos ( dRadian ) + tBulletPoint.x;
				tDestPoint.y = dDiameta * kdSin ( dRadian ) + tBulletPoint.y;
				pBullet->shootAiming ( tDestPoint, 960.0F );
			}
		);
		pActionArray->addObject ( pCallFunc2 );
	}	
	
	pActionArray->addObject ( pDelayTime1 );
	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::ShootDiagonalFromBottomToTopDelay	( KDvoid )
{
	Array*		pActionArray = Array::create ( );
	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playShootAnimation ( 1 );
			}
		),
		nullptr
	);

	pActionArray->addObject ( pSpawn );

	DelayTime*	pDelayTime1 = DelayTime::create ( 1.0f );
	pActionArray->addObject ( pDelayTime1 );

	Point		tPlayerPoint = this->getPlayer ( )->getArmature ( )->getPosition ( );
	Point		tStartPoint ;

	tStartPoint.x = m_tContentSize.width;
	tStartPoint.y = 0;			
			
	KDdouble	dAngle   = getAngle  ( tStartPoint, tPlayerPoint );
	KDdouble	dDiameta = getRadius ( tStartPoint, tPlayerPoint );
	KDdouble	dRadian	 = dAngle * KD_PI_F / 180.0f;

	tStartPoint.x -= 200;
	tStartPoint.y -= 200;

	Point		tBulletPoint;

	for ( KDint i = 0; i <= 4; i++ )
	{
		tBulletPoint = Point ( tStartPoint.x + ( 100 * i ), tStartPoint.y + ( 100 * i ) );

		CallFunc*	pCallFunc2 = CallFunc::create
		(
			[ &,tStartPoint, dAngle, dDiameta, dRadian, tBulletPoint ] ( )
			{
				Node*					pNode = this->getParent ( );
				XMGoblinRiderBullet*	pBullet = XMGoblinRiderBullet::create ( m_pParentWorld );
				pBullet->setPosition ( tBulletPoint );
				pBullet->setRotation ( dAngle - 90 );
				pNode->addChild ( pBullet );

				Point tDestPoint = Point ( 0, 0 );
				tDestPoint.x = dDiameta * kdCos ( dRadian ) + tBulletPoint.x;
				tDestPoint.y = dDiameta * kdSin ( dRadian ) + tBulletPoint.y;
				pBullet->shootAiming ( tDestPoint, 960.0F );
			}
		);
		pActionArray->addObject ( pCallFunc2 );
		pActionArray->addObject ( pDelayTime1 );
	}	
	
	pActionArray->addObject ( pDelayTime1 );
	this->runActionByArray ( pActionArray );
}
KDvoid XMGoblinRider::slashWithNotMove ( KDfloat fTime )
{
	Array*		pActionArray = Array::create ( );

	Spawn*		pSpawn = Spawn::create
	(
		DelayTime::create ( 1.5f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playSlashAnimation ( 1 );
			}
		),
		nullptr
	);
	pActionArray->addObject ( pSpawn );
	
	this->runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::slashWithLinearMove ( KDfloat fTime )
{
	Point				tPoint = Point ( m_pPlayer->getArmature ( )->getPosition ( ).x + 100, m_pArmature->getPosition ( ).y );
	FiniteTimeAction*	pLinearAction = makeMoveLinear ( tPoint, fTime );

	Spawn*		pSpawnAction = Spawn::create 
	(
		DelayTime::create ( 2.0f ),
		pLinearAction,
		CallFunc::create
		(
			[ & ] ( )
			{
				playSlashAnimation ( 1 );
			}
		),
		nullptr
	);

	Array*				pActionArray = Array::create ( );
	pActionArray->addObject ( pSpawnAction );
	pActionArray->addObject ( makeMoveLinear ( Point ( m_tContentSize.width * 0.8 , m_pArmature->getPosition ( ).y ) ) );

	runActionByArray ( pActionArray );
}

KDvoid XMGoblinRider::slashWithMvoeAndThreehit ( KDfloat fTime )
{
	Point				tPoint = Point ( m_pPlayer->getArmature ( )->getPosition ( ).x + 100, m_pArmature->getPosition ( ).y );
	FiniteTimeAction*	pLinearAction = makeMoveLinear ( tPoint, fTime );
	
	Spawn*		pSpawnAction = Spawn::create 
	(
		DelayTime::create ( 1.0f ),
		pLinearAction,
		Sequence::create
		(
			CallFunc::create
			(
				[ & ] ( )
				{
					playSlashAnimation ( 1 );
				}
			),
			DelayTime::create ( 0.5f ),
			CallFunc::create
			(
				[ & ] ( )
				{
					playSlashAnimation ( 1 );
				}
			),
			DelayTime::create ( 0.5f ),
			CallFunc::create
			(
				[ & ] ( )
				{
					playSlashAnimation ( 1 );
				}
			),
			nullptr
		),
		nullptr
	);
	
	Array*		pActionArray = Array::create ( );

	pActionArray->addObject ( pSpawnAction );
	pActionArray->addObject ( makeMoveLinear ( Point ( m_tContentSize.width * 0.8 , m_pArmature->getPosition ( ).y ) ) );

	runActionByArray ( pActionArray );
}
