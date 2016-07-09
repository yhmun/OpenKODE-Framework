/* --------------------------------------------------------------------------
 *
 *      File            Hero.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-26
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
#include "Hero.h"

static Hero*	l_pInstance = KD_NULL;

Hero* Hero::getInstance ( KDvoid )
{
    if ( l_pInstance == KD_NULL )
    {
        l_pInstance = new Hero ( );
		l_pInstance->init ( );
		l_pInstance->autorelease ( );
    }

    return l_pInstance;	
}

Hero::~Hero ( KDvoid )
{
	m_pActRun ->release ( );
	m_pActIdle->release ( );
	m_pActJump->release ( );
	m_pActSit ->release ( );
	m_pActFire->release ( );
}

KDbool Hero::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_bIsRun  = KD_FALSE;
	m_bIsJump = KD_FALSE;

	m_pHead = CCSprite::create ( "Image 577 at frame 10.png" );
	m_pHead->setPosition ( ccp ( 0, 7 ) );

	m_pLeg = CCSprite::create ( "Image 523 at frame 10.png" );
	m_pLeg->setPosition ( ccp ( 3, -7 ) );

	this->addChild ( m_pLeg  );
	this->addChild ( m_pHead );

	CCArray*	pFrames = KD_NULL;

	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 544 at frame 10.png", CCRect ( 0, 0, 30.5f, 30 ) ),
		CCSpriteFrame::create ( "Image 545 at frame 10.png", CCRect ( 0, 0, 31, 29 ) ),
		CCSpriteFrame::create ( "Image 546 at frame 10.png", CCRect ( 0, 0, 31, 28 ) ),
		KD_NULL
	);

	m_pActIdle = CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.2f ) ) );
	m_pActIdle->retain ( );
	m_pHead->runAction ( m_pActIdle );

	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 523 at frame 10.png", CCRect ( 0, 0, 16.5f, 16    ) ),
		CCSpriteFrame::create ( "Image 524 at frame 10.png", CCRect ( 0, 0, 12.5f, 16.5f ) ),
		CCSpriteFrame::create ( "Image 525 at frame 10.png", CCRect ( 0, 0, 26.5f, 20    ) ),
		CCSpriteFrame::create ( "Image 526 at frame 10.png", CCRect ( 0, 0, 30.5f, 16.5f ) ),
//		CCSpriteFrame::create ( "Image 527 at frame 10.png", CCRect ( 0, 0, 21   , 18    ) ),
//		CCSpriteFrame::create ( "Image 528 at frame 10.png", CCRect ( 0, 0, 15.5f, 17    ) ),
//		CCSpriteFrame::create ( "Image 529 at frame 10.png", CCRect ( 0, 0, 12.5f, 16.5f ) ),
//		CCSpriteFrame::create ( "Image 530 at frame 10.png", CCRect ( 0, 0, 25.5f, 18    ) ),
//		CCSpriteFrame::create ( "Image 531 at frame 10.png", CCRect ( 0, 0, 32.5f, 16.5f ) ),
//		CCSpriteFrame::create ( "Image 532 at frame 10.png", CCRect ( 0, 0, 21.5f, 18.5f ) ),
		CCSpriteFrame::create ( "Image 523 at frame 10.png", CCRect ( 0, 0, 16.5f, 16    ) ),
		KD_NULL
	);

	m_pActRun = CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.05f ) ) );
	m_pActRun->retain ( );

	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 539 at frame 10.png", CCRect ( 0, 0, 19   , 22.5f ) ),
		CCSpriteFrame::create ( "Image 540 at frame 10.png", CCRect ( 0, 0, 19.3f, 22    ) ),
		CCSpriteFrame::create ( "Image 541 at frame 10.png", CCRect ( 0, 0, 20.5f, 22    ) ),
		CCSpriteFrame::create ( "Image 542 at frame 10.png", CCRect ( 0, 0, 22   , 19.5f ) ),
		CCSpriteFrame::create ( "Image 543 at frame 10.png", CCRect ( 0, 0, 21.5f, 20    ) ),
		CCSpriteFrame::create ( "Image 523 at frame 10.png", CCRect ( 0, 0, 16.5f, 16    ) ),
		KD_NULL
	);

	m_pActJump = CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.1f ) ) );
	m_pActJump->retain ( );

	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 533 at frame 10.png", CCRect ( 0, 0, 17   , 16.5f ) ),
		CCSpriteFrame::create ( "Image 534 at frame 10.png", CCRect ( 0, 0, 17.5f, 16.5f ) ),
		CCSpriteFrame::create ( "Image 535 at frame 10.png", CCRect ( 0, 0, 17   , 16.5f ) ),
		CCSpriteFrame::create ( "Image 536 at frame 10.png", CCRect ( 0, 0, 17.5f, 17    ) ),
		CCSpriteFrame::create ( "Image 537 at frame 10.png", CCRect ( 0, 0, 17   , 16.5f ) ),
		CCSpriteFrame::create ( "Image 538 at frame 10.png", CCRect ( 0, 0, 19   , 16.5f ) ),
		CCSpriteFrame::create ( "Image 523 at frame 10.png", CCRect ( 0, 0, 16.5f, 16    ) ),
		KD_NULL
	);

	m_pActSit = CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.05f ) ) );
	m_pActSit->retain ( );

	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 635 at frame 10.png", CCRect ( 0, 0, 43   , 26 ) ),
		CCSpriteFrame::create ( "Image 637 at frame 10.png", CCRect ( 0, 0, 43   , 25 ) ),
		CCSpriteFrame::create ( "Image 638 at frame 10.png", CCRect ( 0, 0, 40.5f, 27 ) ),
		CCSpriteFrame::create ( "Image 544 at frame 10.png", CCRect ( 0, 0, 30.5f, 30 ) ),
		KD_NULL
	);

	m_pActFire = CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.1f ) );
	m_pActFire->retain ( );

	return KD_TRUE;
}

KDvoid Hero::setOpacity ( GLubyte cOpacity )
{
    m_pHead->setOpacity ( cOpacity );
    m_pLeg ->setOpacity ( cOpacity );

	CCSprite::setOpacity ( cOpacity );
}

KDvoid Hero::jumpDownFromTruck ( KDvoid )
{
    this->stopAllActions ( );
    this->setScaleX ( -IOSSCALE );
}

KDbool Hero::isJumping ( KDvoid )
{
	return m_bIsJump;
}		

KDvoid Hero::run ( KDvoid )
{
    if ( !m_bIsJump && !m_bIsRun )
	{
        m_bIsRun = KD_TRUE;
		m_pLeg->runAction ( m_pActRun );
    }
}

KDvoid Hero::stop ( KDvoid )
{
	if ( !m_bIsJump && m_bIsRun ) 
	{
		m_bIsRun = KD_FALSE;
		m_pLeg->stopAction ( m_pActRun );
	}
}

KDvoid Hero::jump ( KDfloat* pGroundPlane )
{
    if ( !m_bIsJump )
    {
        if ( m_bIsRun )
		{
            m_bIsRun = KD_FALSE;
			m_pLeg->stopAction ( m_pActRun );
        }

		m_bIsJump = KD_TRUE;
		m_pLeg->stopAction ( m_pActJump );
		m_pLeg->runAction ( m_pActJump );       

        CCPoint		tSPos = this->getParent ( )->getPosition ( );
        CCPoint		tPos  = this->getPosition ( );
        KDfloat		fOffX = this->getScaleX ( ) / 1.5f * -85;

        if ( tPos.x + tSPos.x + fOffX < 15 )
		{
            fOffX = 15 - ( tPos.x + tSPos.x );
        }

		this->runAction
		(	
			CCSequence::create 
			(
				CCJumpY		::create ( 0.7f, ccp ( 0, 0 ), 80, 1, pGroundPlane ),
				CCCallFunc	::create ( this, callfunc_selector ( Hero::selfJumpDone ) ),

				KD_NULL 
			)
		);
	}
}

KDvoid Hero::selfJumpDone ( KDvoid )
{
	m_bIsJump = KD_FALSE;
}

KDvoid Hero::fire ( KDvoid )
{
	m_pHead->stopAction ( m_pActFire );
	m_pHead->runAction ( m_pActFire );
}
