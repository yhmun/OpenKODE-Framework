/* --------------------------------------------------------------------------
 *
 *      File            TestNode.cpp
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
#include "TestNode.h"

enum 
{
	kTagSprite1 = 1,
	kTagSprite2 = 2,
	kTagSprite3 = 3,
	kTagSlider,
};

TestNode* TestNode::create ( KDvoid )
{
	TestNode*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 :	pLayer = new CameraCenterTest	( );	break;
		case  1 :	pLayer = new Test2				( );	break;
		case  2 :	pLayer = new Test4				( );	break;
		case  3 :	pLayer = new Test5				( );	break;
		case  4 :	pLayer = new Test6				( );	break;
		case  5 :	pLayer = new StressTest1		( );	break;
		case  6 :	pLayer = new StressTest2		( );	break;
		case  7 :	pLayer = new NodeToWorld		( );	break;
		case  8 :	pLayer = new SchedulerTest1		( );	break;
		case  9 :	pLayer = new CameraOrbitTest	( );	break;
		case 10 :	pLayer = new CameraZoomTest		( );	break;
		case 11 :	pLayer = new ConvertToNode		( );	break;
		case 12 :	pLayer = new NodeOpaqueTest		( );	break;
		case 13 :	pLayer = new NodeNonOpaqueTest	( );	break;
		case 14 :	pLayer = new ClippingNode		( );	break;
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

KDuint TestNode::count ( KDvoid )
{
	return 15;
}

//------------------------------------------------------------------
//
// Test2
//
//------------------------------------------------------------------
KDvoid Test2::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
	
	CCSprite*  sp1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp1->setPosition ( ccp ( s.cx / 3 , s.cy / 2 ) );

	CCSprite*  sp3 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp3->setScale ( 0.25f );
	sp1->addChild ( sp3 );

	this->addChild ( sp1 );

	CCSprite*  sp2 = CCSprite::create ( "Images/grossinis_sister2.png" );
	sp2->setPosition ( ccp ( s.cx / 3 * 2, s.cy /2 ) );
	sp2->setAnchorPoint ( ccp ( 0, 0 ) );

	CCSprite*  sp4 = CCSprite::create ( "Images/grossinis_sister2.png" );
	sp4->setScale ( 0.25f );
	sp2->addChild ( sp4 );

	this->addChild ( sp2 );
		
	CCActionInterval*  a1 = CCRotateBy::create ( 2, 360 );
	CCActionInterval*  a2 = CCScaleBy ::create ( 2, 2 );

	CCAction*  action1 = CCRepeatForever::create
	(
		CCSequence::create ( a1, a2, a2->reverse ( ), KD_NULL )
	);

	CCAction*  action2 = CCRepeatForever::create
	(
		CCSequence::create
		( 
			(CCActionInterval*) ( a1->copy ( )->autorelease ( ) ),
			(CCActionInterval*) ( a2->copy ( )->autorelease ( ) ), 
			a2->reverse ( ), 
			KD_NULL 
		)
	);	

	sp1->runAction ( action1 );
	sp2->runAction ( action2 );
}

const KDchar* Test2::title ( KDvoid )
{
	return "anchorPoint and children";
}

//------------------------------------------------------------------
//
// Test4
//
//------------------------------------------------------------------
#define SID_DELAY2		1
#define SID_DELAY4		2

KDvoid Test4::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*  sp1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp1->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	this->addChild ( sp1, 0, 2 );

	CCSprite*  sp2 = CCSprite::create ( "Images/grossinis_sister2.png" );
	sp2->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	this->addChild ( sp2, 0, 3 );

	this->schedule ( schedule_selector ( Test4::delay2 ), 2.0f ); 
	this->schedule ( schedule_selector ( Test4::delay4 ), 4.0f ); 
}

KDvoid Test4::delay2 ( KDfloat dt )
{
	CCSprite*  node = (CCSprite*) ( getChildByTag ( 2 ) );
	node->runAction ( CCRotateBy::create ( 1, 360 ) );
}

KDvoid Test4::delay4 ( KDfloat dt )
{
	this->unschedule ( schedule_selector ( Test4::delay4 ) ); 
	this->removeChildByTag ( 3, KD_FALSE );
}

const KDchar* Test4::title ( KDvoid )
{
	return "tags";
}

//------------------------------------------------------------------
//
// Test5
//
//------------------------------------------------------------------
KDvoid Test5::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*  sp1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp1->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	this->addChild ( sp1, 0, kTagSprite1 );

	CCSprite*  sp2 = CCSprite::create ( "Images/grossinis_sister2.png" );
	sp2->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	this->addChild ( sp2, 0, kTagSprite2 );

	CCRotateBy*  rot = CCRotateBy::create ( 2, 360 );
	CCAction*  forever = CCRepeatForever::create ( CCSequence::create ( rot, rot->reverse ( ), KD_NULL ) );

	forever->setTag ( 101 );
	sp1->runAction ( forever );

	CCAction*  forever2 = (CCAction*) ( forever->copy ( )->autorelease ( ) );
	forever2->setTag ( 102 );
	sp2->runAction ( forever2 );
	
	this->schedule ( schedule_selector ( Test5::addAndRemove ), 2.0f );
}

KDvoid Test5::addAndRemove ( KDfloat dt )
{
	CCNode*  sp1 = getChildByTag ( kTagSprite1 );

	sp1->retain ( );
	this->removeChild ( sp1, KD_FALSE );
	this->addChild ( sp1, 0, kTagSprite1 );
	sp1->release ( );

	CCNode*  sp2 = getChildByTag ( kTagSprite2 );

	sp2->retain ( );
	this->removeChild ( sp2, KD_TRUE );	
	this->addChild ( sp2, 0, kTagSprite2 );
	sp2->release ( );	
}

const KDchar* Test5::title ( KDvoid )
{
	return "remove and cleanup";
}

//------------------------------------------------------------------
//
// Test6
//
//------------------------------------------------------------------
KDvoid Test6::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCActionInterval*  rot = CCRotateBy::create ( 2, 360 );
	CCActionInterval*  rot_back = rot->reverse ( );

    CCAction*  forever1  = CCRepeatForever::create ( CCSequence::create ( rot, rot_back, KD_NULL ) );
    CCAction*  forever11 = (CCAction*) ( forever1->copy ( )->autorelease ( ) );
    CCAction*  forever2  = (CCAction*) ( forever1->copy ( )->autorelease ( ) );
    CCAction*  forever21 = (CCAction*) ( forever1->copy ( )->autorelease ( ) );

	CCSprite*  sp1  = CCSprite::create ( "Images/grossinis_sister1.png" );
	CCSprite*  sp11 = CCSprite::create ( "Images/grossinis_sister1.png" );

	CCSprite*  sp2  = CCSprite::create ( "Images/grossinis_sister2.png" );
	CCSprite*  sp21 = CCSprite::create ( "Images/grossinis_sister2.png" );

	sp1->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	sp2->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );

	sp1->addChild ( sp11 );
	sp2->addChild ( sp21 );

	this->addChild ( sp1, 0, kTagSprite1 );
	this->addChild ( sp2, 0, kTagSprite2 );

    sp1 ->runAction ( forever1  );
    sp11->runAction ( forever11 );
    sp2 ->runAction ( forever2  );
    sp21->runAction ( forever21 );

	schedule ( schedule_selector ( Test6::addAndRemove ), 2.0f );
}

KDvoid Test6::addAndRemove ( KDfloat dt )
{
	CCNode*  sp1 = getChildByTag ( kTagSprite1 );

	sp1->retain ( );
	this->removeChild ( sp1, KD_FALSE );
	this->addChild ( sp1, 0, kTagSprite1 );
	sp1->release ( );

	CCNode*  sp2 = getChildByTag ( kTagSprite2 );

	sp2->retain ( );
	this->removeChild ( sp2, KD_TRUE );	
	this->addChild ( sp2, 0, kTagSprite2 );
	sp2->release ( );
}

const KDchar* Test6::title ( KDvoid )
{
	return "remove/cleanup with children";
}

//------------------------------------------------------------------
//
// StressTest1
//
//------------------------------------------------------------------
KDvoid StressTest1::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCSprite*  sp1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp1->setPosition ( ccpMid ( s ) );	
	this->addChild ( sp1, 0, kTagSprite1 );

	this->schedule ( schedule_selector ( StressTest1::shouldNotCrash ), 1.0f );
}

KDvoid StressTest1::shouldNotCrash ( KDfloat dt )
{
	this->unschedule ( schedule_selector ( StressTest1::shouldNotCrash ) );

	const CCSize&  s = this->getContentSize ( );

	// if the node has timers, it crashes
	CCParticleSun*  explosion = CCParticleSun::create ( );
	explosion->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	explosion->setPosition ( ccpMid ( s ) );
	this->addChild ( explosion );

	this->runAction ( CCSequence::create
	( 
		CCRotateBy ::create ( 2, 360 ),
		CCCallFuncN::create ( this, callfuncN_selector ( StressTest1::removeMe ) ), 
		KD_NULL) 
	);
}

// remove
KDvoid StressTest1::removeMe ( CCNode* node )
{	
	getParent ( )->removeChild ( node, KD_TRUE );
	this->onNext ( this );
}

const KDchar* StressTest1::title ( KDvoid )
{
	return "stress test #1 : no crashes";
}

//------------------------------------------------------------------
//
// StressTest2
//
//------------------------------------------------------------------
KDvoid StressTest2::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
	
	CCLayer*  sublayer = CCLayer::create ( );

	CCActionInterval*  move = CCMoveBy::create ( 3, ccp ( 350, 0 ) );
	CCActionInterval*  move_ease_inout3 = CCEaseInOut::create ( (CCActionInterval *) ( move->copy ( )->autorelease ( ) ), 2.0f );
	CCActionInterval*  move_ease_inout_back3 = move_ease_inout3->reverse ( );

	CCSequence*  seq3 = CCSequence::create ( move_ease_inout3, move_ease_inout_back3, KD_NULL );

	CCSprite*  sp1 = CCSprite::create ( "Images/grossinis_sister1.png" );
	sp1->setPosition ( ccp ( 80, s.cy / 2 ) );
	sp1->runAction ( CCRepeatForever::create ( seq3 ) );
	sublayer->addChild ( sp1, 1 );

	CCParticleFire*  fire = CCParticleFire::create ( );
	fire->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	fire->setPosition ( ccp ( 80, s.cy / 2 - 50 ) );
	fire->runAction ( CCRepeatForever::create ( (CCActionInterval *) ( seq3->copy ( )->autorelease ( ) ) ) );
	sublayer->addChild ( fire, 2 );

	this->addChild ( sublayer, 0, kTagSprite1 );
		
	this->schedule ( schedule_selector ( StressTest2::shouldNotLeak ), 6.0f );
}

KDvoid StressTest2::shouldNotLeak ( KDfloat dt )
{
	this->unschedule ( schedule_selector ( StressTest2::shouldNotLeak ) );

	CCLayer*  sublayer = (CCLayer*) getChildByTag ( kTagSprite1 );
	sublayer->removeAllChildrenWithCleanup ( KD_TRUE ); 
}

const KDchar* StressTest2::title ( KDvoid )
{
	return "stress test #2 : no leaks";
}

//------------------------------------------------------------------
//
// SchedulerTest1
//
//------------------------------------------------------------------
KDvoid SchedulerTest1::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	CCLayer*  layer = CCLayer::create ( );
	layer->schedule (  schedule_selector ( SchedulerTest1::doSomething ) );	
	layer->unschedule ( schedule_selector ( SchedulerTest1::doSomething ) );

	this->addChild ( layer, 0 );
}

KDvoid SchedulerTest1::doSomething ( KDfloat dt )
{
	CCLOG ( "SchedulerTest1 - doSomething ( )" );
}

const KDchar* SchedulerTest1::title ( KDvoid )
{
	return "cocosnode scheduler test #1";
}

//------------------------------------------------------------------
//
// NodeToWorld
//
//------------------------------------------------------------------
KDvoid NodeToWorld::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	//
	// This code tests that nodeToParent works OK:
	//  - It tests different anchor Points
	//  - It tests different children anchor points

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*  pBG = CCSprite::create ( "Images/background3.png" );
	pBG->setOpacity  ( 127 );
	pBG->setPosition ( ccpMid ( tLyrSize ) );

	CCMenuItemImage*  item = CCMenuItemImage::create ( "Images/btn-play-normal.png", "Images/btn-play-selected.png" );
	item->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 5, 360 ) ) );

	CCMenu*  menu = CCMenu::createWithItem ( item );
	menu->alignItemsVertically ( );
	menu->setPosition ( ccpMid ( pBG->getContentSize ( ) ) );
	pBG->addChild ( menu );

	CCActionInterval*  move = CCMoveBy::create ( 3, CCPointMake ( 200,0 ) );
	CCSequence*  seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );
	pBG->runAction ( CCRepeatForever::create ( seq ) );

	this->addChild ( pBG );
}

const KDchar* NodeToWorld::title ( KDvoid )
{
	return "nodeToParent transform";
}

//------------------------------------------------------------------
//
// CameraOrbitTest
//
//------------------------------------------------------------------
KDvoid CameraOrbitTest::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCSprite*  bg = CCSprite::create ( "Images/background3.png" );

	const CCSize&  t = bg->getContentSize ( );

	bg->setScaleX ( s.cx / t.cx );
	bg->setScaleY ( s.cy / t.cy );
	bg->setPosition ( ccpMid ( s ) );
	bg->setOpacity ( 128 );
	bg->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 90 ) ) );

	CCSprite*  left = CCSprite::create ( "Images/grossini.png" );
	left->setScale ( 0.5f );
	left->setPosition ( ccp ( t.cx / 3 , t.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 2, 1, 0, 0, 360, 0, 0 ) ) );
	bg->addChild ( left, 0 );		

	CCSprite*  center = CCSprite::create ( "Images/grossini.png" );
	center->setScale ( 1.0f );
	center->setPosition ( ccp ( t.cx / 2, t.cy / 2 ) );
	center->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 2, 1, 0, 0, 360, 45, 0 ) ) );
	bg->addChild ( center, 0 );

	CCSprite*  right = CCSprite::create ( "Images/grossini.png" );
	right->setScale ( 2.0f );
	right->setPosition ( ccp ( t.cx / 3 * 2, t.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 2, 1, 0, 0, 360, 90, -45 ) ) );
	bg->addChild ( right, 0 );	

	this->addChild ( bg );
}

const KDchar* CameraOrbitTest::title ( KDvoid )
{
	return "Camera Orbit test";
}

//------------------------------------------------------------------
//
// CameraZoomTest
//
//------------------------------------------------------------------
KDvoid CameraZoomTest::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  s = this->getContentSize ( );
	
	CCSprite*  left = CCSprite::create ( "Images/grossini.png" );
	left->setPosition ( ccp ( s.cx / 4 * 1, s.cy / 2 ) );
	left->getCamera ( )->setEyeXYZ ( 0, 0, 415 );
	this->addChild ( left, 0 );	

	CCSprite*  center = CCSprite::create ( "Images/grossini.png" );
	center->setPosition ( ccp ( s.cx / 4 * 2, s.cy / 2 ) );
	this->addChild ( center, 0, 40 );

	CCSprite*  right = CCSprite::create ( "Images/grossini.png" );
	right->setPosition ( ccp ( s.cx / 4 * 3, s.cy / 2 ) );
	this->addChild ( right, 0, 20 );
	
	this->schedule ( schedule_selector ( CameraZoomTest::updateZoom ) );

	m_z = 0;	
}

KDvoid CameraZoomTest::updateZoom ( KDfloat dt )
{	
	m_z += dt * 100;
	
	CCNode*  right = getChildByTag ( 20 );
	right->getCamera ( )->setEyeXYZ ( 0, 0, m_z * 2 );

	CCNode*  center = getChildByTag ( 40 );
	center->getCamera ( )->setEyeXYZ ( 0, 0, m_z );
}

const KDchar* CameraZoomTest::title ( KDvoid )
{
	return "Camera Zoom test";
}

//------------------------------------------------------------------
//
// CameraCenterTest
//
//------------------------------------------------------------------
KDvoid CameraCenterTest::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&   s = this->getContentSize ( );
	
	CCSprite*  left_top = CCSprite::create ( "Images/white-512x512.png" );
	left_top->setPosition ( ccp ( s.cx / 5 * 1, s.cy / 5 * 1 ) );
	left_top->setColor ( ccRED );
	left_top->setTextureRect ( CCRectMake ( 0, 0, 120, 50 ) );
	left_top->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) ) );
	this->addChild ( left_top );
	
	CCSprite*  left_bottom = CCSprite::create ( "Images/white-512x512.png" );
	left_bottom->setPosition ( ccp ( s.cx / 5 * 1, s.cy / 5 * 4 ) );
	left_bottom->setColor ( ccBLUE );
	left_bottom->setTextureRect ( CCRectMake ( 0, 0, 120, 50 ) );
	left_bottom->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) ) );
	this->addChild ( left_bottom );

	CCSprite*  right_top = CCSprite::create ( "Images/white-512x512.png" );
	right_top->setPosition ( ccp ( s.cx / 5 * 4, s.cy / 5 * 1 ) );
	right_top->setColor ( ccYELLOW );
	right_top->setTextureRect ( CCRectMake ( 0, 0, 120, 50 ) );
	right_top->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) ) );
	this->addChild ( right_top );
	
	CCSprite*  right_bottom = CCSprite::create ( "Images/white-512x512.png" );
	right_bottom->setPosition ( ccp ( s.cx / 5 * 4, s.cy / 5 * 4 ) );
	right_bottom->setColor ( ccGREEN );
	right_bottom->setTextureRect ( CCRectMake ( 0, 0, 120, 50 ) );
	right_bottom->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) ) );
	this->addChild ( right_bottom );

	CCSprite*  center = CCSprite::create ( "Images/white-512x512.png" );
	center->setPosition ( ccpMid ( s ) );
	center->setColor ( ccWHITE );
	center->setTextureRect ( CCRectMake ( 0, 0, 120, 50 ) );
	center->runAction ( CCRepeatForever::create ( CCOrbitCamera::create ( 10, 1, 0, 0, 360, 0, 0 ) ) );
	this->addChild ( center );
}

const KDchar* CameraCenterTest::title ( KDvoid )
{
	return "Camera Center test";
}

const KDchar* CameraCenterTest::subtitle ( KDvoid )
{
	return "Sprites should rotate at the same speed";
}

//------------------------------------------------------------------
//
// ConvertToNode
//
//------------------------------------------------------------------
KDvoid ConvertToNode::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCRepeatForever*  action = CCRepeatForever::create ( CCRotateBy::create ( 10, 360 ) );

	for ( KDint i = 0; i < 3; i++ )
	{
		CCSprite*  sprite = CCSprite::create ( "Images/grossini.png" );
		sprite->setPosition ( ccp ( s.cx / 4 * ( i + 1 ), s.cy / 2 ) );
		sprite->runAction ( (CCRepeatForever*) action->copy ( )->autorelease ( ) );

		switch ( i )
		{
			case 0 : sprite->setAnchorPoint ( ccp ( 0.0f, 0.0f ) ); break;
			case 1 : sprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) ); break;
			case 2 : sprite->setAnchorPoint ( ccp ( 1.0f, 1.0f ) ); break;
		}

		CCSprite*  point = CCSprite::create ( "Images/r1.png" );
		point->setScale ( 0.25f );
		point->setPosition ( sprite->getPosition ( ) );

		this->addChild ( point, 10, 100 + i );

		this->addChild ( sprite, i );
	}

	this->setTouchEnabled ( KD_TRUE );
}

KDvoid ConvertToNode::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
    for ( CCSetIterator it = touches->begin ( ); it != touches->end ( ); ++it )
    {
        CCTouch*     touch = (CCTouch*)(*it);
        CCPoint   location = touch->getLocation ( );

        for ( KDint i = 0; i < 3; i++ )
        {
            CCNode*  node = getChildByTag ( 100 + i );

			CCPoint  p1 = node->convertToNodeSpaceAR ( location );
			CCPoint  p2 = node->convertToNodeSpace ( location );

			CCLOG ( "AR ( %d ) x = %4d, y = %4d -- Not AR : x = %4d, y = %4d", i, (KDint) p1.x, (KDint) p1.y, (KDint) p2.x, (KDint) p2.y );   
		}
    }	
}

const KDchar* ConvertToNode::title ( KDvoid )
{
    return "Convert To Node Space";
}

const KDchar* ConvertToNode::subtitle ( KDvoid )
{
    return "testing convertToNodeSpace / AR.\nTouch and see console";
}

//------------------------------------------------------------------
//
// NodeOpaqueTest
//
//------------------------------------------------------------------
KDvoid NodeOpaqueTest::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

    const CCSize&  tLyrSize = this->getContentSize ( );

    for ( KDint i = 0; i < 50; i++)
    {
        CCSprite*  pBG = CCSprite::create ( "xm_supports/Back1.jpg" );
        pBG->setGLServerState ( (ccGLServerState) ( pBG->getGLServerState ( ) & ( ~CC_GL_BLEND ) ) );
		pBG->setPosition ( ccpMid ( tLyrSize ) );
        this->addChild ( pBG );
    }
}

const KDchar* NodeOpaqueTest::title ( KDvoid )
{
	return "Node Opaque Test";
}

const KDchar* NodeOpaqueTest::subtitle ( KDvoid )
{
	return "Node rendered with GL_BLEND disabled";
}

//------------------------------------------------------------------
//
// NodeNonOpaqueTest
//
//------------------------------------------------------------------
KDvoid NodeNonOpaqueTest::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );
	
    for ( KDint i = 0; i < 50; i++ )
    {
        CCSprite*  pBG = CCSprite::create ( "xm_supports/Back1.jpg" );
        pBG->setGLServerState ( (ccGLServerState) ( pBG->getGLServerState ( ) | CC_GL_BLEND ) );
		pBG->setPosition ( ccpMid ( tLyrSize ) );
        this->addChild ( pBG );
    }
}

const KDchar* NodeNonOpaqueTest::title ( KDvoid )
{
	return "Node Non Opaque Test";
}

const KDchar* NodeNonOpaqueTest::subtitle ( KDvoid )
{
	return "Node rendered with GL_BLEND enabled";
}

//------------------------------------------------------------------
//
// ClippingNode
//
//------------------------------------------------------------------
KDvoid ClippingNode::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*  pBG = CCSprite::create ( "xm_supports/Back1.jpg" );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	pBG->setClipping ( KD_TRUE );
	this->addChild ( pBG );

	CCSprite*  pGrossini = CCSprite::create ( "Images/grossini.png" ); 
	pGrossini->setPosition ( ccpMid ( pBG->getContentSize ( ) ) );
	pBG->addChild ( pGrossini );

	CCActionInterval*  pMoveBy = CCMoveBy::create ( 1.5f, pBG->getContentSize ( ) );
	pGrossini->runAction 
	(
		CCRepeatForever::create ( (CCActionInterval *) 
		(
			CCSequence::create ( pMoveBy, pMoveBy->reverse ( ), pMoveBy->reverse ( ), pMoveBy->copy ( )->autorelease ( ), KD_NULL ) ) 
		) 
	);
}

const KDchar* ClippingNode::title ( KDvoid )
{
	return "Clipping node test";
}
