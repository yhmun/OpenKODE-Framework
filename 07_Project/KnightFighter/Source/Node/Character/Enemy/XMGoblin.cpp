/* --------------------------------------------------------------------------
 *
 *      File            XMGoblin.cpp
 *      Description     XMGoblin
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
#include "XMGoblin.h"
#include "Node/Character/Player/XMPlayer.h"
#include "Node/Bullet/Enemy/XMEnemyBullet.h"

XMGoblin::XMGoblin ( b2World* pWorld, String* sType )
{
	m_pParentWorld = pWorld;
	if ( sType )
	{
		kdStrcpy ( m_sType, sType->getCString ( ) );
	}
}

XMGoblin::~XMGoblin ( KDvoid )
{

}

XMGoblin* XMGoblin::create ( b2World* pWorld, String* sType )
{
	XMGoblin* pRet = new XMGoblin ( pWorld, sType );

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

KDbool XMGoblin::init ( KDvoid )
{
	if ( !XMEnemy::initWithContentsOfFile ( "Goblin/Goblin.plist" ) )
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

	if ( GLOBAL->getStage ( ) <= 1 )
	{
		setPossibleSlash ( KD_FALSE );
	}
	
	return KD_TRUE;
}


KDvoid XMGoblin::playAI ( XMPlayer* pPlayer )
{
	m_pPlayer = pPlayer;

	this->schedule ( schedule_selector ( XMGoblin::doAI ), 1.0f );
	
}

KDvoid XMGoblin::doAI ( KDfloat fDelta )
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

	if ( m_pPlayer->getArmature ( )->getBoundingBox ( ).intersectsRect ( this->getArmature ( )->getBoundingBox ( ) ) )
	{
		if ( kdRand ( ) % 100 < 50 )
		{
			Array* pArray = Array::create ( );
			pArray->addObject ( makeMoveEvade ( ) );
			runActionByArray ( pArray );

			return;
		}
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
}

KDbool XMGoblin::shoot ( KDvoid )
{
	KDint		nRand = kdRand ( ) % 100;

	Point		tPoint = Point ( m_pPlayer->getArmature ( )->getPosition ( ).x, m_pPlayer->getArmature ( )->getPosition ( ).y );

	if ( GLOBAL->getStage ( ) <= 1 )
	{
		if ( GLOBAL->getLevel ( ) <= 2 )
		{
			KDint		nRand = kdRand ( ) % 100;

			if ( nRand < 50 )
			{
				shootLinear ( BULLET_SMALL );
			}
			else if ( nRand >= 50 && nRand < 80 )
			{
				Array*		pActionArray = Array::create ( );
	
				if ( kdRand ( ) % 2 == 0 )
				{
					pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
				}
				else
				{
					pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
				}

				runActionByArray ( pActionArray );
				// 위 아래로 이동
			}
			else
			{
				// do Nothing
			}
		}
		else
		{
			KDint		nRand = kdRand ( ) % 100;

			if ( nRand < 50 )
			{
				KDint		nRand = kdRand ( ) % 100;

				if ( nRand < 30 )
				{
					shootLinear ( BULLET_SMALL );
				}
				else if ( nRand >= 30 && nRand < 60 )
				{
					shootAiming ( BULLET_SMALL );
				}
				else if ( nRand >= 60 && nRand < 80 )
				{
					Array*		pActionArray = Array::create ( );
					KDint		nSubRand = kdRand ( ) % 2;

					if ( kdRand ( ) % 2 == 0 )
					{
						pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
					}
					else
					{
						pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
					}

					runActionByArray ( pActionArray );
						// 위 아래로 이동
				}
				else
				{
					// do Nothing
				}
			}
		}
	}
	else if ( GLOBAL->getStage ( ) <= 2 )
	{
		if ( GLOBAL->getLevel ( ) <= 2 )
		{
			KDint		nRand = kdRand ( ) % 100;

			if ( nRand < 50 )
			{
				KDint		nRand = kdRand ( ) % 100;

				if ( nRand < 30 )
				{
					shootLinear ( BULLET_SMALL, 480.0f + ( 50 * GLOBAL->getLevel ( ) ) );
				}
				else if ( nRand >= 30 && nRand < 60 )
				{
					shootAiming ( BULLET_SMALL, 480.0f + ( 50 * GLOBAL->getLevel ( ) ) );
				}
				else if ( nRand >= 60 && nRand < 80 )
				{

					Array*		pActionArray = Array::create ( );				
					KDint		nSubRand = kdRand ( ) % 2;

					if ( kdRand ( ) % 2 == 0)
					{
						if ( kdRand ( ) % 2 == 0 )
						{
							pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
						}
						else
						{
							pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
						}

						runActionByArray ( pActionArray );
						// 위 아래로 이동
					}
					else
					{
						if ( kdRand ( ) % 2 == 0 )
						{
							pActionArray->addObject ( makeMoveLeft   ( m_pArmature->getPosition ( ) ) );
						}
						else
						{
							pActionArray->addObject ( makeMoveRight ( m_pArmature->getPosition ( ) ) );
						}

						runActionByArray ( pActionArray );
						// 좌우로 이동
					}
				}
				else
				{
					// do Notghing
				}
			}
		}
		else
		{
			KDint		nRand = kdRand ( ) % 100;

			if ( nRand < 30 )
			{
				shootMulti ( BULLET_SMALL, GLOBAL->getStage ( ), 15.0f );
			}
			else if ( nRand >= 30 && nRand < 60 )
			{
				shootMultiDelay ( BULLET_SMALL, GLOBAL->getStage ( ), 15.0f, 0.5f );
			}
			else if ( nRand >= 60 && nRand < 80 )
			{
				Array*		pActionArray = Array::create ( );				

				if ( kdRand ( ) % 2 == 0)
				{
					FiniteTimeAction*	pMoveUpAction   = makeMoveUp   ( m_pArmature->getPosition ( ) );
					FiniteTimeAction*	pMoveDownAction = makeMoveDown ( m_pArmature->getPosition ( ) );

					if ( kdRand ( ) % 2 == 0 )
					{
						pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
					}
					else
					{
						pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
					}

					runActionByArray ( pActionArray );
					// 위 아래로 이동
				}
				else
				{
					if ( kdRand ( ) % 2 == 0 )
					{
						pActionArray->addObject ( makeMoveLeft   ( m_pArmature->getPosition ( ) ) );
					}
					else
					{
						pActionArray->addObject ( makeMoveRight ( m_pArmature->getPosition ( ) ) );
					}

					runActionByArray ( pActionArray );
					// 좌우로 이동
				}
			}
			else
			{
				// do Notghing
			}
		}
	}
	else
	{
		KDint		nRand = kdRand ( ) % 100;

		if ( nRand < 80 )
		{
			KDint		nRand = kdRand ( ) % 100;

			if ( nRand < 30 )
			{
				shootMulti ( BULLET_SMALL, GLOBAL->getStage ( ), 15.0f );
			}
			else if ( nRand >= 30 && nRand < 60 )
			{
				shootMultiDelay ( BULLET_SMALL, GLOBAL->getStage ( ), 15.0f, 0.5f );
			}
			else if ( nRand >= 60 && nRand < 80 )
			{
				Array*		pActionArray = Array::create ( );				

				if ( kdRand ( ) % 2 == 0)
				{
					if ( kdRand ( ) % 2 == 0 )
					{
						pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
					}
					else
					{
						pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
					}

					runActionByArray ( pActionArray );
					// 위 아래로 이동
				}
				else
				{
					if ( kdRand ( ) % 2 == 0 )
					{
						pActionArray->addObject ( makeMoveLeft   ( m_pArmature->getPosition ( ) ) );
					}
					else
					{
						pActionArray->addObject ( makeMoveRight ( m_pArmature->getPosition ( ) ) );
					}

					runActionByArray ( pActionArray );
					// 좌우로 이동
				}
			}
			else
			{
				// do Nothing
			}
		}
	}

	return KD_TRUE;
}

KDbool XMGoblin::slash ( KDvoid )
{
	if ( GLOBAL->getStage ( ) <= 2 )
	{
		KDint		nRand = kdRand ( ) % 100;
		KDint		nPercentage = 50 + ( GLOBAL->getLevel ( ) * 5 );
		if ( nRand < nPercentage )
		{
			slashWithLinearMove ( );
		}
		else if ( nRand >= nPercentage && nRand < 80 )
		{
			Array*		pActionArray = Array::create ( );

			if ( kdRand ( ) % 2 == 0)
			{
				pActionArray->addObject ( makeMoveUp   ( m_pArmature->getPosition ( ) ) );
			}
			else
			{
				pActionArray->addObject ( makeMoveDown ( m_pArmature->getPosition ( ) ) );
			}

			runActionByArray ( pActionArray );
			// 위 아래로 이동
		}
		else
		{
			// do Nothing
		}
	}
	else
	{
		KDint		nRand = kdRand ( ) % 100;
		KDint		nPercentage = 50 - ( GLOBAL->getLevel ( ) * 5 );
		if ( nRand < nPercentage )
		{
			slashWithLinearMove ( );
		}
		else if ( nRand >= nPercentage && nRand < nPercentage + 30 )
		{
			slashWithFlashMove ( );
		}
		else
		{
			// do Nothing
		}

	}

	return KD_TRUE;

	KDint nRand = kdRand ( ) % 100;

	if ( nRand < 20 )
	{
		schedule ( schedule_selector ( XMEnemy::homingPlayer ) );
	}
	else if ( nRand >= 20 && nRand < 50 )
	{
		
	}
	else if ( nRand >= 50 && nRand <= 80 )
	{
		
	}
	else
	{
		// Do Nothing
	}

	return KD_TRUE;
}



KDvoid  XMGoblin::slashWithLinearMove ( KDfloat fTime )
{
	Point				tPoint = Point ( m_pPlayer->getArmature ( )->getPosition ( ).x + 100, m_pPlayer->getArmature ( )->getPosition ( ).y );
	FiniteTimeAction*	pLinearAction = makeMoveLinear ( tPoint, fTime );

	Spawn*		pSpawnAction = Spawn::create 
	(
		DelayTime::create ( 1.0f ),
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

	pActionArray->addObject ( pLinearAction );
	pActionArray->addObject ( pSpawnAction );
	pActionArray->addObject ( makeMoveEvade ( ) );

	runActionByArray ( pActionArray );
}

KDvoid XMGoblin::slashWithFlashMove ( KDfloat fTime )
{
	FiniteTimeAction*	pFlashAction = makeMoveFlash ( Point ( m_pPlayer->getArmature ( )->getPosition ( ).x + 100, m_pPlayer->getArmature ( )->getPosition ( ).y ), 0.5 );
	
	Spawn*		pSpawnAction = Spawn::create 
	(
		DelayTime::create ( 1.0f ),
		CallFunc::create
		(
			[ & ] ( )
			{
				playSlashAnimation ( 1 );
			}
		),
		nullptr
	);
	
	Array*		pActionArray = Array::create ( );

	pActionArray->addObject ( pFlashAction );
	pActionArray->addObject ( pSpawnAction );
	pActionArray->addObject ( makeMoveEvade ( ) );

	runActionByArray ( pActionArray );
}