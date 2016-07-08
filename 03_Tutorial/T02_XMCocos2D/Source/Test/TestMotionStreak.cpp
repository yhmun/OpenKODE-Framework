/* --------------------------------------------------------------------------
 *
 *      File            TestMotionStreak.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "TestMotionStreak.h"

TestMotionStreak* TestMotionStreak::create ( KDvoid )
{
	TestMotionStreak*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0 : pLayer = new MotionStreakTest1 ( ); break;
		case 1 : pLayer = new MotionStreakTest2 ( ); break;
		case 2 : pLayer = new MotionStreakTest3 ( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestMotionStreak::count ( KDvoid )
{
	return 3;
}

KDvoid TestMotionStreak::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );

    CCMenuItemToggle*  pMode = CCMenuItemToggle::createWithTarget
	( 
		this, menu_selector ( TestMotionStreak::onMode ),
        CCMenuItemFont::create ( "Use High Quality Mode" ),
        CCMenuItemFont::create ( "Use Fast Mode" ),
        KD_NULL
	);
	pMode->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 4 ) );

    this->addChild ( CCMenu::create ( pMode, KD_NULL ) );
}

KDvoid TestMotionStreak::onMode ( CCObject* pSender )
{
    m_pStreak->setFastMode ( !m_pStreak->isFastMode ( ) );
}

//------------------------------------------------------------------
//
// MotionStreakTest1
//
//------------------------------------------------------------------
KDvoid MotionStreakTest1::onEnter ( KDvoid )
{
	TestMotionStreak::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
  
	// the root object just rotates around
	CCSprite*  pRoot = CCSprite::create ( "Images/r1.png" );
	pRoot->setPosition ( ccpMid ( s ) );

	CCActionInterval*  motion = CCMoveBy::create ( 2, ccp ( 100, 0 ) );
	pRoot->runAction ( CCRepeatForever::create ( CCSequence::create ( motion, motion->reverse ( ), KD_NULL ) ) );
	pRoot->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 2, 360 ) ) );

	// the target object is offset from root, and the streak is moved to follow it
	m_pTarget = CCSprite::create ( "Images/r1.png" );
	m_pTarget->setPosition ( ccp ( 100, 0 ) );
	pRoot->addChild ( m_pTarget );

	this->addChild ( pRoot, 1 );
	
	// create the streak object and add it to the scene
	m_pStreak = CCMotionStreak::create ( 2, 3, 32, ccGREEN, "Images/streak.png" ); 
	this->addChild ( m_pStreak );

	CCActionInterval*  pColorAction = CCRepeatForever::create
	(
		CCSequence::create
		(
			CCTintTo::create ( 0.2f, 255,   0,   0 ),
			CCTintTo::create ( 0.2f,   0, 255,   0 ),
			CCTintTo::create ( 0.2f,   0,   0, 255 ),
			CCTintTo::create ( 0.2f,   0, 255, 255 ),
			CCTintTo::create ( 0.2f, 255, 255,   0 ),
			CCTintTo::create ( 0.2f, 255,   0, 255 ),
			CCTintTo::create ( 0.2f, 255, 255, 255 ),
			KD_NULL
		)
	);

	m_pStreak->runAction ( pColorAction );

	// schedule an update on each frame so we can syncronize the streak with the target
	this->schedule ( schedule_selector ( MotionStreakTest1::onUpdate ) );
}

KDvoid MotionStreakTest1::onUpdate ( KDfloat fElapsed )
{
	m_pStreak->setPosition ( ccpSub ( m_pTarget->convertToWorldSpace ( CCPointZero ), this->getPosition ( ) ) );
}

const KDchar* MotionStreakTest1::title ( KDvoid )
{
	return "MotionStreak test 1";
}

//------------------------------------------------------------------
//
// MotionStreakTest2
//
//------------------------------------------------------------------

KDvoid MotionStreakTest2::onEnter ( KDvoid )
{
	TestMotionStreak::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );
		
	// create the streak object and add it to the scene
	m_pStreak = CCMotionStreak::create ( 1, 5, 50, ccWHITE, "Images/Icon.png" ); 
	m_pStreak->setPosition ( ccpMid ( tLyrSize ) ); 
	this->addChild ( m_pStreak );
	
	this->setTouchEnabled ( KD_TRUE );
}

KDvoid MotionStreakTest2::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint   tLocation  = this->convertTouchToNodeSpace ( pTouch );
	
	m_pStreak->setPosition ( tLocation );
}

const KDchar* MotionStreakTest2::subtitle ( KDvoid )
{
	return "( tap screen )";
}

//------------------------------------------------------------------
//
// MotionStreakTest3
//
//------------------------------------------------------------------
KDvoid MotionStreakTest3::onEnter ( KDvoid )
{
    TestMotionStreak::onEnter ( );
    
    // ask director the the window size
    const CCSize&  tLyrSize = this->getContentSize ( );
    
	m_pStreak = CCMotionStreak::create ( 2, 1.f, 50.f, ccc3 ( 255, 255, 0 ), "Images/Icon.png" ); 
    addChild ( m_pStreak );
    
    m_tCenter = ccpMid ( tLyrSize );
    m_fRadius = tLyrSize.cy / 3;
    m_fAngle  = 0.0f;
    
    schedule ( schedule_selector ( MotionStreakTest3::update ), 1 / 30.f );
}

const KDchar* MotionStreakTest3::subtitle ( KDvoid )
{
	return "The tail should use the texture";
}

KDvoid MotionStreakTest3::update ( KDfloat fDelta )
{
    m_fAngle += 2.0f;
    m_pStreak->setPosition ( ccp ( m_tCenter.x + kdCosf ( m_fAngle / 180 * KD_PI_F ) * m_fRadius,
                                   m_tCenter.y + kdSinf ( m_fAngle / 180 * KD_PI_F ) * m_fRadius ) );
}