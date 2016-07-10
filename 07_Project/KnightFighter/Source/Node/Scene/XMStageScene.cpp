/* --------------------------------------------------------------------------
 *
 *      File            XMStageScene.cpp
 *      Description     XMStageScene
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
#include "XMStageScene.h"
#include "Node/Background/XMBackground.h"
#include "Node/Character/Player/XMKnight.h"
#include "Node/Character/Enemy/XMGoblin.h"
#include "Node/Bullet/XMBullet.h"
#include "Node/Effect/Slash/XMSlashEffect.h"
#include "Loader/XMStageLoader.h"
#include "ColliderToolkit/XMContactListener.h"
#include "ColliderToolkit/XMContactFilter.h"
#include "Node/Effect/Die/XMDieEffect.h"
#include "Node/UI/XMStageUi.h"
#include "Node/Layer/XMMissionComplete.h"
#include "Node/Layer/XMMissionFailed.h"


XMStageScene::XMStageScene ( KDvoid )
{
	m_pStageLoader = KD_NULL;

	m_pXMStageUi = KD_NULL;
}

XMStageScene::~XMStageScene ( KDvoid )
{
	unscheduleUpdate ( );

	m_aButtons.clear ( );

	CC_SAFE_DELETE ( m_pStageLoader );

	CC_SAFE_DELETE ( m_pContactListener );
	CC_SAFE_DELETE ( m_pContactFilter );
	CC_SAFE_DELETE ( m_pDebugDraw );
	CC_SAFE_DELETE ( m_pWorld );
}

KDbool XMStageScene::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return KD_FALSE;
	}

	this->setContentSize ( EGLView::getInstance ( )->getVisibleSize ( ) );
	this->setPosition ( EGLView::getInstance ( )->getVisibleOrigin ( ) );
	this->initPhysics ( );
	this->initVariable ( );
	this->initBackground ( 1 );
	this->initDPadAndButtons ( );
	this->initTouchListener ( );
	this->initStageLoader ( 1 );
	this->initPlayer ( );
	this->initStageUi ( );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid XMStageScene::initVariable ( KDvoid )
{
	m_bIsStart = KD_FALSE;
}

KDvoid XMStageScene::initPhysics ( KDvoid )
{
	// create world without gravity
	b2Vec2		tGravity ( 0, 0 );
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( KD_TRUE );

	// owner contact listener
	m_pContactListener = new XMContactListener ( );
	m_pWorld->SetContactListener ( m_pContactListener );

	m_pContactFilter = new XMContactFilter ( );
	m_pWorld->SetContactFilter ( m_pContactFilter );

	m_pDebugDraw = new Box2DDebugDraw ( PT_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	KDuint32	uFlags = 0;

	uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;

	m_pDebugDraw->SetFlags ( uFlags );
}

KDvoid XMStageScene::initBackground ( KDint nStage )
{
	m_pBackground = XMBackground::create ( nStage );

	this->addChild ( m_pBackground, ZORDER_BACKGROUND );
}

KDvoid XMStageScene::initDPadAndButtons ( KDvoid )
{
	const Size&		tLyrSize = this->getContentSize ( );
	SpriteFrameCache::getInstance ( )->addSpriteFramesWithFile ( "dpad_buttons.plist" );

	m_pDPad = GameDPad::create ( "d_pad_normal.png", "d_pad_horizontal.png", "d_pad_diagonal.png" );
	m_pDPad->setOpacity ( 100 );
	m_pDPad->setScale ( 2 );
	m_pDPad->setPosition ( Point ( 150, 150 ) );
	this->addChild ( m_pDPad, ZORDER_DPAD );

	this->createButtonWithPosition ( Point ( tLyrSize.width - 250, 100 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( Point ( tLyrSize.width - 100, 100 ), "a_button_up.png", "a_button_down.png", "A" );
}

KDvoid XMStageScene::initTouchListener ( KDvoid )
{
	EventListenerTouchAllAtOnce*	pListner = EventListenerTouchAllAtOnce::create ( );
	
	pListner->onTouchesBegan = CC_CALLBACK_2 ( XMStageScene::onTouchesBegan, this );
	pListner->onTouchesMoved = CC_CALLBACK_2 ( XMStageScene::onTouchesMoved, this );
	pListner->onTouchesEnded = CC_CALLBACK_2 ( XMStageScene::onTouchesEnded, this );

	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListner, this );
}

KDvoid XMStageScene::initStageLoader ( KDint nStage )
{
	m_pStageLoader = new XMStageLoader ( );
	m_pStageLoader->loadStage ( nStage );
}

KDvoid XMStageScene::initPlayer ( KDvoid )
{
	m_pPlayer = XMKnight::create ( m_pWorld );
	this->addChild ( m_pPlayer, ZORDER_PLAYER );
}

KDvoid XMStageScene::initStageUi ( )
{
	m_pXMStageUi = XMStageUi::create ( );
	this->addChild ( m_pXMStageUi, ZORDER_STAGEUI );
}

KDvoid XMStageScene::onExit ( KDvoid )
{
	Scene::onExit ( );

	this->removeAllChildrenWithCleanup ( KD_TRUE );
	GLOBAL->getEnemyContainer ( )->removeAllObjects ( );
}

KDvoid XMStageScene::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pDPad->onTouchesBegan ( aTouches, pEvent );

	for ( KDuint i = 0; i < m_aButtons.size ( ); i++ )
	{
		m_aButtons [ i ]->onTouchesBegan ( aTouches, pEvent );
	}
}

KDvoid XMStageScene::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	if ( m_bIsStart == KD_FALSE )
	{
		return;
	}

	m_pDPad->onTouchesMoved ( aTouches, pEvent );

	for ( KDuint i = 0; i < m_aButtons.size ( ); i++ )
	{
		m_aButtons [ i ]->onTouchesMoved ( aTouches, pEvent );
	}
}

KDvoid XMStageScene::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pDPad->onTouchesEnded ( aTouches, pEvent );

	for ( KDuint i = 0; i < m_aButtons.size ( ); i++ )
	{
		m_aButtons [ i ]->onTouchesEnded ( aTouches, pEvent );
	}
}

KDvoid XMStageScene::createButtonWithPosition ( const Point& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName )
{
	GameButton*		pButton = GameButton::create ( szUpFrame, szDownFrame );

	pButton->setOpacity ( 100 );
	pButton->setScale ( 2 );
	pButton->setPosition ( tPosition );
	pButton->setName ( szName );
	this->addChild ( pButton, ZORDER_BUTTON );

	m_aButtons.push_back ( pButton );
}

KDvoid XMStageScene::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid XMStageScene::update ( KDfloat fDelta )
{
	m_pWorld->Step ( fDelta, 0, 0 );
	
	this->updateStageLoader ( );
	this->operatePlayer ( );
	this->processContactList ( fDelta );
}

KDvoid XMStageScene::updateStageLoader ( KDvoid )
{
	m_pStageLoader->update ( );

	KDint		nStageState = m_pStageLoader->getStageState ( );
	Object*		pObject = m_pStageLoader->getObject ( );

	switch ( nStageState )
	{
		case STAGESTATE_START :
			startGame ( );
			break;
		case STAGESTATE_ADDENEMY :
			addEnemy ( pObject );
			break;
		case STAGESTATE_END :
			endGame ( );
			break;
	}
}

KDvoid XMStageScene::operatePlayer ( KDvoid )
{
	m_pPlayer->move ( m_pDPad->getDirection ( ) );

	for ( KDuint i = 0; i < m_aButtons.size ( ); i++ )
	{
		GameButton*		pButton = m_aButtons [ i ];

		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "B" ) )
		{
			m_pPlayer->slash ( );
			return;
		}
		else if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			m_pPlayer->shoot ( );
			return;
		}
	}
}

KDvoid XMStageScene::processContactList ( KDfloat fDelta )
{
	if ( m_pContactListener->isContactListEmpty ( ) )
	{
		return;
	}
	
	std::list<Contact> tList = m_pContactListener->getContactList ( );

	for ( std::list<Contact>::iterator it = tList.begin ( ); it != tList.end ( ); it++ )
	{
		Contact		tContact = *it;
		b2Filter	aFilter [ 2 ];

		for ( KDint i = 0; i < 2; i++ )
		{
			aFilter [ i ] = tContact.fixture [ i ]->GetFilterData ( );
		}

		Bone*			pBone1 = KD_NULL;
		Bone*			pBone2 = KD_NULL;
		XMBullet*		pBullet = KD_NULL;
		XMEnemy*		pEnemy = KD_NULL;
		XMPlayer*		pPlayer = KD_NULL;
		XMCharacter*	pCharacter = KD_NULL;

		Node*			pFixture0UserData = ( Node* ) tContact.fixture [ 0 ]->GetUserData ( );
		Node*			pFixture1UserData = ( Node* ) tContact.fixture [ 1 ]->GetUserData ( );

		if ( ( pBone1 = dynamic_cast< Bone* > ( pFixture0UserData ) ) && ( pBone2 = dynamic_cast< Bone* > ( pFixture1UserData ) ) )
		{
			//근접 공격
			Node*		pNode1 = pBone1->getArmature ( )->getParent ( );
			Node*		pNode2 = pBone2->getArmature ( )->getParent ( );

			if ( ( ( pPlayer = dynamic_cast< XMPlayer* > ( pNode1 ) ) && ( pEnemy = dynamic_cast< XMEnemy* > ( pNode2 ) ) ) ||
				 ( ( pPlayer = dynamic_cast< XMPlayer* > ( pNode2 ) ) && ( pEnemy = dynamic_cast< XMEnemy* > ( pNode1 ) ) ) )
			{
				if ( ( aFilter [ 0 ].categoryBits == CATEGORY_PWEAPON && aFilter [ 1 ].categoryBits == CATEGORY_ENEMY ) ||
					 ( aFilter [ 0 ].categoryBits == CATEGORY_ENEMY && aFilter [ 1 ].categoryBits == CATEGORY_PWEAPON ) )
				{
					if ( pEnemy->getArmature ( )->getB2Body ( )->IsActive ( ) )
					{
						pEnemy->damaged ( m_pPlayer->getSlashDamage ( ) );

						pEnemy->getArmature ( )->getB2Body ( )->SetActive ( KD_FALSE );
						pEnemy->hitted ( );

						XMSlashEffect*		pEffect = XMSlashEffect::create ( m_pPlayer->getPlayerName ( )->getCString ( ) );

						KDint				nRotation = kdRand ( ) % 360;
						pEffect->setRotation ( nRotation );
						pEffect->setPosition ( pEnemy->getArmature ( )->getPosition ( ) );
						this->addChild ( pEffect, ZORDER_EFFECT );
					}
				}

				if ( ( aFilter [ 0 ].categoryBits == CATEGORY_EWEAPON && aFilter [ 1 ].categoryBits == CATEGORY_PLAYER ) ||
					 ( aFilter [ 0 ].categoryBits == CATEGORY_PLAYER && aFilter [ 1 ].categoryBits == CATEGORY_EWEAPON ) )
				{
					if ( pPlayer->getArmature ( )->getB2Body ( )->IsActive ( ) )
					{
						pPlayer->damaged ( pEnemy->getSlashDamage ( ) );
						m_pXMStageUi->setMaxHp ( m_pPlayer->getMaxHP ( ) );
						m_pXMStageUi->setCurrentHp ( m_pPlayer->getCurrentHP ( ) );
						pPlayer->getArmature ( )->getB2Body ( )->SetActive ( KD_FALSE );
						pPlayer->hitted ( );

						XMSlashEffect*		pEffect = XMSlashEffect::create ( m_pPlayer->getPlayerName ( )->getCString ( ) );

						pEffect->setPosition ( pPlayer->getArmature ( )->getPosition ( ) );
						this->addChild ( pEffect, ZORDER_EFFECT );
					}
				}
			}
		}
		else if ( ( ( pBullet = dynamic_cast< XMBullet* > ( pFixture0UserData ) ) && ( pBone1 = dynamic_cast< Bone* > ( pFixture1UserData ) ) ) || 
				  ( ( pBullet = dynamic_cast< XMBullet* > ( pFixture1UserData ) ) && ( pBone1 = dynamic_cast< Bone* > ( pFixture0UserData ) ) ) )
		{
			// 원거리 공격
			pCharacter = (XMCharacter*) pBone1->getArmature ( )->getParent ( );

			pCharacter->damaged ( pBullet->getDamage ( ) );

			if ( XMPlayer* pPlayer = dynamic_cast<XMPlayer*> ( pCharacter ) )
			{
				m_pXMStageUi->setCurrentHp ( pPlayer->getCurrentHP ( ) );
			}

			pBullet->setVisible ( KD_FALSE );
			pBullet->getB2Body ( )->SetActive ( KD_FALSE );
		}
	}	


	m_pContactListener->clearContactList ( );

	Array*			pDeleteArray = Array::create ( );

	Object*			pObject = KD_NULL;
	CCARRAY_FOREACH ( GLOBAL->getEnemyContainer ( ), pObject )
	{
		XMEnemy*		pEnemy = (XMEnemy*) pObject;
		if ( pEnemy->getCurrentHP ( ) <= 0 )
		{
			pDeleteArray->addObject ( pEnemy );
		}
	}

	GLOBAL->getEnemyContainer ( )->removeObjectsInArray ( pDeleteArray );

	CCARRAY_FOREACH ( pDeleteArray, pObject )
	{
		XMEnemy*		pEnemy = (XMEnemy*) pObject;
		XMDieEffect*	pDieEffect = XMDieEffect::create ( );

		pDieEffect->setPosition ( pEnemy->getArmature ( )->getPosition ( ) );
		this->addChild ( pDieEffect );

		GLOBAL->addScore ( pEnemy->getScore ( ) );

		this->removeChild ( pEnemy, KD_TRUE );
	}

	if ( m_pPlayer->getCurrentHP ( ) <= 0 )
	{
		Object*			pObject = KD_NULL;
		CCARRAY_FOREACH ( GLOBAL->getEnemyContainer ( ), pObject )
		{
			XMEnemy*		pEnemy = ( XMEnemy* ) pObject;
			pEnemy->unscheduleAllSelectors ( );
		}

		unscheduleAllSelectors ( );

		// 게임오버

		//m_pPlayer->setActive ( KD_FALSE );
		m_pPlayer->setVisible ( KD_FALSE );

		//this->removeChild ( m_pPlayer );

		m_bIsStart = KD_FALSE;

		XMMissionFailed*		pMissionFailed = XMMissionFailed::create ( );
		this->addChild ( pMissionFailed, ZORDER_STAGEUI_END );


		return;
	}

	if ( m_pStageLoader->getLoaderState ( ) == LOADERSTATE_WAIT && GLOBAL->getEnemyContainer ( )->count ( ) == 0 )
	{
		m_pStageLoader->setLoaderState ( LOADERSTATE_ROLL );
	}
}

KDvoid XMStageScene::startGame ( KDvoid )
{
	m_pStageLoader->setLoaderState ( LOADERSTATE_WAIT );
	
	const Size &tLyrSize = this->getContentSize ( );
	m_pPlayer->getArmature ( )->setPosition ( Point ( -300, tLyrSize.height / 2 ) );
	m_pPlayer->getArmature ( )->setVisible ( KD_TRUE );

	m_pPlayer->getArmature ( )->runAction
	(
		Sequence::create 
		(
			MoveTo::create ( 2, Point ( 200, tLyrSize.height / 2 ) ),
			CallFunc::create 
			(
				[ & ]( )
					{
						m_bIsStart = KD_TRUE;
						m_pStageLoader->setLoaderState ( LOADERSTATE_ROLL );
					}
			),
			nullptr
		)
	);	

	if ( m_pXMStageUi != KD_NULL )
	{
		m_pXMStageUi->setMaxHp ( m_pPlayer->getMaxHP ( ) );
		m_pXMStageUi->setCurrentHp ( m_pPlayer->getCurrentHP ( ) );
		m_pXMStageUi->setPlayerIconName ( m_pPlayer->getPlayerName ( )->getCString ( ) );
	}
}

KDvoid XMStageScene::endGame ( KDvoid )
{
	m_bIsStart = KD_FALSE;
	m_pStageLoader->setLoaderState ( LOADERSTATE_WAIT );
	unscheduleUpdate ( );

	const Size &tLyrSize = this->getContentSize ( );

	m_pPlayer->getArmature ( )->runAction
	(
		Sequence::create
		(
			MoveTo::create ( 2, Point ( tLyrSize / 2 ) ),
			CallFunc::create
			(
				[ & ] ( )
				{
					// 게임 승리
					XMMissionComplete*		pComplete = XMMissionComplete::create ( );
					this->addChild ( pComplete, ZORDER_STAGEUI_END );
				}
			),
			nullptr
		)
	);
}

KDvoid XMStageScene::addEnemy ( Object* pObject )
{
	m_pStageLoader->setLoaderState ( LOADERSTATE_WAIT );
	
	Dictionary*		pDict = (Dictionary*) pObject;

	Array*			pArray = (Array*) pDict->objectForKey ( "ENEMYDATA" );
	Object*			pObject1 = KD_NULL;

	CCARRAY_FOREACH ( pArray, pObject1 )
	{
		Dictionary*	pInfo = (Dictionary*) pObject1;

		String*		sName = (String*) pInfo->objectForKey ( "NAME" );
		Point		tStartPosition = PointFromString ( pInfo->valueForKey ( "START_POSITION" )->getCString ( ) );
		Point		tEndPosition = PointFromString ( pInfo->valueForKey ( "END_POSITION" )->getCString ( ) );
		KDfloat		fDuration = pInfo->valueForKey ( "DURATION" )->floatValue ( );
		KDfloat		fDelay = pInfo->valueForKey ( "DELAY" )->floatValue ( );
		String*		sType = (String*) pInfo->objectForKey ( "TYPE" );

		XMEnemy*	pEnemy = GLOBAL->getEnemyMap ( ) [ sName->getCString ( ) ] ( m_pWorld, sType );
		pEnemy->getArmature ( )->setPosition ( tStartPosition );
		this->addChild ( pEnemy, ZORDER_ENEMY );
		GLOBAL->getEnemyContainer ( )->addObject ( pEnemy );

		pEnemy->getArmature ( )->runAction
		(
			Sequence::create
			(
				DelayTime::create ( fDelay ),
				EaseExponentialOut::create ( MoveTo::create ( fDuration, tEndPosition ) ),
				CallFuncN::create 
				(
					[ & ] ( Node* pNode )
					{
						XMEnemy* pEnemy = (XMEnemy*) pNode->getParent ( );
						pEnemy->playAI ( m_pPlayer );
					}
				),
				nullptr
			)
		);
	}
}
