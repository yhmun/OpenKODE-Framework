/* --------------------------------------------------------------------------
 *
 *      File            TestActionManager.cpp
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
#include "TestActionManager.h"

enum 
{
	kTagNode,
	kTagGrossini,
    kTagSequence,
}; 

TestActionManager* TestActionManager::create ( KDvoid )
{
	TestActionManager*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new CrashTest  ( ); break;
		case  1 : pLayer = new LogicTest  ( ); break;
		case  2 : pLayer = new PauseTest  ( ); break;
		case  3 : pLayer = new RemoveTest ( ); break;
		case  4 : pLayer = new ResumeTest ( ); break;
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

KDuint TestActionManager::count ( KDvoid )
{
	return 5;
}

//------------------------------------------------------------------
//
// Test1
//
//------------------------------------------------------------------
KDvoid CrashTest::onEnter ( KDvoid )
{
	TestActionManager::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCSprite*  child = CCSprite::create ( "Images/grossini.png" );
	this->addChild ( child, 1 );

	child->setPosition ( ccpMid ( s ) );

	// Sum of all action's duration is 1.5 second.
	child->runAction ( CCRotateBy::create ( 1.5f, 90 ) );
	child->runAction ( CCSequence::create ( CCDelayTime::create ( 1.4f ), CCFadeOut::create ( 1.1f ), KD_NULL ) );	

	//After 1.5 second, self will be removed.
	this->runAction ( CCSequence::create 
	(
		CCDelayTime::create ( 1.4f ),
		CCCallFunc ::create ( this, callfunc_selector ( CrashTest::removeThis ) ),
		KD_NULL
	) );
}

const KDchar* CrashTest::subtitle ( KDvoid )
{
	return "Test 1. Should not crash";
}

KDvoid CrashTest::removeThis ( KDvoid )
{
	m_pParent->removeChild ( this, KD_TRUE );
	
	onNext ( this );
}


//------------------------------------------------------------------
//
// Test2
//
//------------------------------------------------------------------
KDvoid LogicTest::onEnter ( KDvoid )
{
	TestActionManager::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCSprite*  grossini = CCSprite::create ( "Images/grossini.png" );
	
	this->addChild ( grossini, 0, 2 );

	grossini->setPosition ( ccpMid ( s ) );
	grossini->runAction (  CCSequence::create
	( 
		CCMoveBy   ::create ( 1, ccp ( 150, 0 ) ),
		CCCallFuncN::create ( this, callfuncN_selector ( LogicTest::bugMe ) ),
		KD_NULL
	) );
}

const KDchar* LogicTest::subtitle ( KDvoid )
{
	return "Logic test"; 
}

KDvoid LogicTest::bugMe ( CCNode* node )
{
	node->stopAllActions ( ); //After this stop next action not working, if remove this stop everything is working
	node->runAction ( CCScaleTo::create ( 2, 2 ) );
}



//------------------------------------------------------------------
//
// PauseTest
//
//------------------------------------------------------------------
KDvoid PauseTest::onEnter ( KDvoid )
{
	TestActionManager::onEnter ( );

	//
	// This test MUST be done in 'onEnter' and not on 'init'
	// otherwise the paused action will be resumed at 'onEnter' time
	//
	
	const CCSize&  s = this->getContentSize ( );

	CCLabelTTF*  l = CCLabelTTF::create ( "After 5 seconds grossini should move", "fonts/Marker Felt.ttf", 16 );

	l->setPosition ( ccp ( s.cx / 2, 230 ) );
	this->addChild ( l );

	//
	// Also, this test MUST be done, after [super onEnter]
	//
	CCSprite*  grossini = CCSprite::create ( "Images/grossini.png" );

	grossini->setPosition ( ccpMid ( s ) );

	this->addChild ( grossini, 0, kTagGrossini );

	CCDirector::sharedDirector ( )->getActionManager ( )->addAction ( CCMoveBy::create ( 1, ccp ( 150, 0 ) ), grossini, KD_TRUE );
	this->schedule ( schedule_selector ( PauseTest::unpause ), 3 ); 
}

const KDchar* PauseTest::subtitle ( KDvoid )
{
	return "Pause Test";
}

KDvoid PauseTest::unpause ( KDfloat dt )
{
	this->unschedule ( schedule_selector ( PauseTest::unpause ) );

	CCNode*  node = getChildByTag ( kTagGrossini );

	CCDirector::sharedDirector ( )->getActionManager ( )->resumeTarget ( node );
}


//------------------------------------------------------------------
//
// RemoveTest
//
//------------------------------------------------------------------
KDvoid RemoveTest::onEnter ( KDvoid )
{
	TestActionManager::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  l = CCLabelTTF::create ( "Should not crash", "fonts/Marker Felt.ttf", 16 );
	l->setPosition ( ccp ( s.cx / 2, 230 ) );
	this->addChild ( l );

   
	CCSprite*  pChild = CCSprite::create ( "Images/grossini.png" );

	pChild->setPosition ( ccpMid ( s ) );

	CCActionInterval*  pSequence = CCSequence::create 
	(
		CCMoveBy  ::create ( 2, ccp ( 200, 0 ) ),
		CCCallFunc::create ( this, callfunc_selector ( RemoveTest::stopAction ) ),
		KD_NULL
	);

	pSequence->setTag ( kTagSequence );
	pChild->runAction ( pSequence );
		
	this->addChild ( pChild, 1, kTagGrossini );
}

const KDchar* RemoveTest::subtitle ( KDvoid )
{
    return "Remove Test";
}

KDvoid RemoveTest::stopAction ( KDvoid )
{
    CCNode*  pSprite = getChildByTag ( kTagGrossini );

	pSprite->stopActionByTag ( kTagSequence );
}



//------------------------------------------------------------------
//
// ResumeTest
//
//------------------------------------------------------------------
KDvoid ResumeTest::onEnter ( KDvoid )
{
	TestActionManager::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  l = CCLabelTTF::create ( "Grossini only rotate/scale in 3 seconds", "fonts/Marker Felt.ttf", 16 );
	l->setPosition ( ccp ( s.cx / 2, 230 ) );
	this->addChild ( l );


    CCSprite*  pGrossini = CCSprite::create ( "Images/grossini.png" );

	this->addChild ( pGrossini, 0, kTagGrossini );

	pGrossini->setPosition ( ccpMid ( s ) );
	pGrossini->runAction ( CCScaleBy::create ( 2, 2 ) );

	CCDirector::sharedDirector ( )->getActionManager ( )->pauseTarget ( pGrossini );
	pGrossini->runAction ( CCRotateBy::create ( 2, 360 ) );
    
    this->schedule ( schedule_selector ( ResumeTest::resumeGrossini ), 3.0f );
}

const KDchar* ResumeTest::subtitle ( KDvoid )
{
    return "Resume Test";
}

KDvoid ResumeTest::resumeGrossini ( KDfloat time )
{
    this->unschedule ( schedule_selector ( ResumeTest::resumeGrossini ) );

    CCNode*  pGrossini = getChildByTag ( kTagGrossini );
	CCDirector::sharedDirector ( )->getActionManager ( )->resumeTarget ( pGrossini );
}
