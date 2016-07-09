/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      海锋 周 on 12-04-11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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
#include "GameLayer.h"

#define MAX_ENEMY			15
#define MOVESPEED			5

#define KProgressTag		100

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_nEnergy = 0;
	m_nMaxEnergy = 1000;

	this->setTouchEnabled ( KD_TRUE );

	this->loadTexture ( );

	this->initUITab ( );

	this->schedule ( schedule_selector ( GameLayer::updateGame ), 0.05f );

	return KD_TRUE;
}

KDvoid GameLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator  pIter = pTouches->begin ( ); pIter != pTouches->end ( ); pIter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *pIter );
		CCPoint		tPoint = pTouch->getLocation ( );

		m_pGun->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "actor_cannon1_72.png" ) );

		KDfloat		fAngle = ( tPoint.y - m_pGun->getPosition ( ).y ) / ( tPoint.x - m_pGun->getPosition ( ).x );		
		fAngle = kdAtanf ( fAngle ) / KD_PI_F * 180;

		if ( fAngle < 0 )
		{
			m_pGun->setRotation ( -( 90 + fAngle ) );
		}		
		else if ( fAngle > 0  )
		{
			m_pGun->setRotation ( 90 - fAngle );
		}
	}
}

KDvoid GameLayer::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator  pIter = pTouches->begin ( ); pIter != pTouches->end ( ); pIter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *pIter );
		CCPoint		tPoint = pTouch->getLocation ( );

		m_pGun->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "actor_cannon1_71.png" ) );

		m_pScore1->setNumber ( m_pScore1->getNumber ( ) - kdRand ( ) % 20 - 2 );
		
		CCNet*		pNet = CCNet::createWithSpriteFrameName ( "bullet01.png" );
        pNet->setPosition ( ccp ( 512, 50 ) );
		pNet->setCatching ( KD_TRUE );
        
		pNet->setRotation ( m_pGun->getRotation ( ) );
		pNet->runAction 
		(
			CCSequence::create 
			(
				CCMoveTo::create ( 1.0f, tPoint ),
				CCCallFuncO::create ( this, callfuncO_selector ( GameLayer::showNet ), pNet ),
				KD_NULL 
			)
		);
		m_pFish2Sheet->addChild ( pNet );

		this->updateEnergy ( kdRand ( ) % 20 );
	}
}

KDvoid GameLayer::loadTexture ( KDvoid )
{
	CCSprite*		pBg = CCSprite::create ( "bj01.jpg" );   
    pBg->setPosition ( ccp ( 512, 368 ) );
	this->addChild ( pBg );
    
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	pCache->addSpriteFramesWithFile ( "fish.plist" );
	m_pFishSheet = CCSpriteBatchNode::create ( "fish.png" );
	this->addChild ( m_pFishSheet );
    
	pCache->addSpriteFramesWithFile ( "fish2.plist" );
	m_pFish2Sheet = CCSpriteBatchNode::create ( "fish2.png" );
	this->addChild ( m_pFish2Sheet );

	pCache->addSpriteFramesWithFile ( "fish3.plist" );
	m_pNetSheet = CCSpriteBatchNode::create ( "fish3.png" );
	this->addChild ( m_pNetSheet );
}

KDvoid GameLayer::initUITab ( KDvoid )
{
	CCSprite*	pEnergyBox = CCSprite::create ( "ui_2p_004.png" );
	pEnergyBox->setAnchorPoint ( ccp ( 0.5, 0.5 ) );
	pEnergyBox->setPosition ( ccp ( 520, 30 ) );
	this->addChild ( pEnergyBox );

	m_pEnergyPointer = CCSprite::create ( "ui_2p_005.png" );
	m_pEnergyPointer->setPosition ( ccp ( 520, 30 ) );
	this->addChild ( m_pEnergyPointer );

	CCSprite*	pBgExp = CCSprite::create ( "ui_box_01.png" );
	pBgExp->setPosition ( ccp ( 500, 700 ) );
	this->addChild ( pBgExp );

	CCSprite*	pBgNum = CCSprite::create ( "ui_box_02.png" );
	pBgNum->setPosition ( ccp ( 440, 90 ) );
	this->addChild ( pBgNum );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "cannon.plist" );
	m_pCannonSheet = CCSpriteBatchNode::create ( "cannon.png" );
	this->addChild ( m_pCannonSheet );

	m_pScore1 = UIRollNum::create ( );
	m_pScore1->setNumber ( 10000 );
	m_pScore1->setPosition ( ccp ( 365, 17 ) );
	this->addChild ( m_pScore1, 100 );

	m_pGun = CCSprite::createWithSpriteFrameName ( "actor_cannon1_71.png" );
	m_pGun->setPosition ( ccp ( 520, 50 ) );
	m_pCannonSheet->addChild ( m_pGun );
}

KDvoid GameLayer::addFish ( KDvoid )
{
	KDint	nType = kdRand ( ) % 8 + 1;

	CCArray*	pFish01 = CCArray::create ( );
	for ( KDint i = 1; i < 10; i++ )
	{
		pFish01->addObject ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "fish0%d_0%d.png", nType, i ) ) );
	}

	CCFish*		pFish = CCFish::createWithSpriteFrameName ( ccszf ( "fish0%d_0%d.png", nType, 1 ) );
	pFish->setScale ( 1.2f );
	pFish->setTag ( nType );
	pFish->setCatch ( KD_FALSE );
	pFish->runAction ( CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFish01, 0.2f ) ) ) );
	pFish->addPath ( );
	m_pFishSheet->addChild ( pFish );
}

KDvoid GameLayer::updateGame ( KDfloat fDelta )
{
	CCObject*	pObject;

	CCARRAY_FOREACH ( m_pFishSheet->getChildren ( ), pObject )
	{
		CCFish*		pFish = (CCFish*) pObject;

		if ( pFish->isCatch ( ) )
		{
			continue;
		}

		CCObject*	pObject2;
		CCARRAY_FOREACH ( m_pFish2Sheet->getChildren ( ), pObject2 )
		{
			CCNet*		pNet = (CCNet*) pObject2;
			
			if ( pFish->boundingBox ( ).containsPoint ( pNet->getPosition ( ) ) )
			{
				if ( !pFish->randomCatch ( pFish->getTag ( ) ) )
				{
					pNet->setCatching ( KD_FALSE );					
					break;
				}
				else
				{
					pNet->setCatching ( KD_FALSE );	
					pFish->setCatch ( KD_TRUE );

					CCArray*	pFish01 = CCArray::create ( );
					for ( KDint i = 1; i < 3; i++ )
					{
						pFish01->addObject ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "fish0%d_catch_0%d.png", pFish->getTag ( ), i ) ) );
					}

					pFish->stopAllActions ( );
					pFish->runAction 
					(
						CCSequence::create 
						(
							CCRepeat::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFish01, 0.2f ) ), 2 ),
							CCCallFuncO::create ( this, callfuncO_selector ( GameLayer::afterCatch ), pFish ),
							KD_NULL
						)
					);
    
					CCSprite*	pGold = CCSprite::create ( "+5.png" );
					pGold->setPosition ( pFish->getPosition ( ) );
	                
					pGold->runAction 
					(
						CCSequence::create 
						(
							CCScaleTo::create ( 0.3f, 1.1f ),
							CCScaleTo::create ( 0.3f, 0.9f ),
							CCScaleTo::create ( 0.3f, 1.1f ),
							CCScaleTo::create ( 0.3f, 0.9f ),
							CCCallFuncO::create ( this, callfuncO_selector ( GameLayer::afterShow ), pGold ),
							KD_NULL 
						)
					);

					this->addChild ( pGold );					
				} 
			}				
		}	   
	}

	CCARRAY_FOREACH ( m_pFish2Sheet->getChildren ( ), pObject )
	{
		CCNet*		pNet = (CCNet*) pObject;

		if ( pNet->isCatching ( ) )
		{
			continue;
		}

		m_pFish2Sheet->removeChild ( pNet, KD_FALSE );

		CCNet*		pTapNet = CCNet::createWithSpriteFrameName ( "net01.png" );
		pTapNet->setPosition ( pNet->getPosition ( ) );

		pTapNet->runAction 
		(
			CCSequence::create 
			(
				CCScaleTo::create ( 0.3f, 1.1f ),
				CCScaleTo::create ( 0.3f, 0.9f ),
				CCScaleTo::create ( 0.3f, 1.1f ),
				CCScaleTo::create ( 0.3f, 0.9f ),
				CCCallFuncO::create ( this, callfuncO_selector ( GameLayer::afterShowNet ), pTapNet ),
				KD_NULL 
			)
		);

		m_pNetSheet->addChild ( pTapNet );

		m_pScore1->setNumber ( m_pScore1->getNumber ( ) + 5 );
	}

	while ( m_pFishSheet->getChildren ( )->count ( ) < MAX_ENEMY )
	{
		this->addFish ( );		
	}
}

KDvoid GameLayer::showNet ( CCObject* pSender )
{
    CCSprite*	pSprite = (CCSprite*) pSender;
 
    m_pFish2Sheet->removeChild ( pSprite, KD_FALSE );
    
	pSprite->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "net01.png" ) );
 
	pSprite->runAction 
	(
		CCSequence::create 
		(
			CCScaleTo::create ( 0.3f, 1.1f ),
			CCScaleTo::create ( 0.3f, 0.9f ),
			CCScaleTo::create ( 0.3f, 1.1f ),
			CCScaleTo::create ( 0.3f, 0.9f ),
			CCCallFuncO::create ( this, callfuncO_selector ( GameLayer::afterShowNet ), pSprite ),
			KD_NULL 
		)
	);

	m_pNetSheet->addChild ( pSprite );
}

KDvoid GameLayer::updateEnergy ( KDint nEnergy )
{
    m_nEnergy += nEnergy;
    
	if ( m_nEnergy >= m_nMaxEnergy )
	{
        m_nEnergy = m_nMaxEnergy;
    }

    KDfloat		fRotation = 180.0f * m_nEnergy / m_nMaxEnergy;
    m_pEnergyPointer->setRotation ( fRotation );
}

KDvoid GameLayer::afterShowNet ( CCObject* pSender )
{
    CCSprite*	pSprite = (CCSprite*) pSender;
	m_pNetSheet->removeChild ( pSprite, KD_FALSE );
}

KDvoid GameLayer::afterShow ( CCObject* pSender )
{
    CCSprite*	pSprite = (CCSprite*) pSender;
	this->removeChild ( pSprite, KD_FALSE );
}

KDvoid GameLayer::afterCatch ( CCObject* pSender )
{
    CCSprite*	pSprite = (CCSprite*) pSender;
	m_pFishSheet->removeChild ( pSprite, KD_FALSE );
}
