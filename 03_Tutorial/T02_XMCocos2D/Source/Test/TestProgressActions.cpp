/* --------------------------------------------------------------------------
 *
 *      File            TestProgressActions.cpp
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
 *      Lesser General Public License for more detailtLyrSize.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#include "Precompiled.h"
#include "TestProgressActions.h"

TestProgressActions* TestProgressActions::create ( KDvoid )
{
	TestProgressActions*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new SpriteProgressToRadial				 ( ); break;
		case  1 : pLayer = new SpriteProgressToHorizontal			 ( ); break;
		case  2 : pLayer = new SpriteProgressToVertical				 ( ); break;
        case  3 : pLayer = new SpriteProgressToRadialMidpointChanged ( ); break;
        case  4 : pLayer = new SpriteProgressBarVarious				 ( ); break;
        case  5 : pLayer = new SpriteProgressBarTintAndFade			 ( ); break;
        case  6 : pLayer = new SpriteProgressWithSpriteFrame		 ( ); break;
		case  7 : pLayer = new ProgressHorizontally					 ( ); break;
		case  8 : pLayer = new ProgressVertically					 ( ); break;
		case  9 : pLayer = new ProgressBoth							 ( ); break;
		case 10 : pLayer = new ProgressSize							 ( ); break;
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

KDuint TestProgressActions::count ( KDvoid )
{
	return 11;
}

//------------------------------------------------------------------
//
// SpriteProgressToRadial
//
//------------------------------------------------------------------
KDvoid SpriteProgressToRadial::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );

	CCProgressTimer*  left = CCProgressTimer::create ( "Images/grossinis_sister1.png" );

	left->setType ( kCCProgressTimerTypeRadial );
	left->setPosition ( CCPointMake ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );

	
	CCProgressTimer*  right = CCProgressTimer::create ( "Images/blocks.png" );

	right->setType ( kCCProgressTimerTypeRadial );		
	right->setReverseProgress ( KD_TRUE );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );
}

const KDchar* SpriteProgressToRadial::subtitle ( KDvoid )
{
	return "ProgressTo Radial";
}

//------------------------------------------------------------------
//
// SpriteProgressToHorizontal
//
//------------------------------------------------------------------
KDvoid SpriteProgressToHorizontal::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCProgressTimer*  left = CCProgressTimer::create ( "Images/grossinis_sister1.png" );

	left->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the left since the midpoint is 0 for the x
	left->setMidpoint ( ccp ( 0, 0 ) );
	//    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
	left->setBarChangeRate ( ccp ( 1, 0 ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );

	
	CCProgressTimer*  right = CCProgressTimer::create ( "Images/grossinis_sister2.png" );

	right->setType ( kCCProgressTimerTypeBar );		
	//    Setup for a bar starting from the left since the midpoint is 1 for the x
	right->setMidpoint ( ccp ( 1, 0 ) );
	//    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
	right->setBarChangeRate ( ccp ( 1, 0 ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );

}

const KDchar* SpriteProgressToHorizontal::subtitle ( KDvoid )
{
	return "ProgressTo Horizontal";
}

//------------------------------------------------------------------
//
// SpriteProgressToVertical
//
//------------------------------------------------------------------
KDvoid SpriteProgressToVertical::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCProgressTimer*  left = CCProgressTimer::create ( "Images/grossinis_sister1.png" );

	left->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	left->setMidpoint ( ccp ( 0, 0 ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	left->setBarChangeRate ( ccp ( 0, 1 ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );

	
	CCProgressTimer*  right = CCProgressTimer::create ( "Images/grossinis_sister2.png" );

	right->setType ( kCCProgressTimerTypeBar );	
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	right->setMidpoint ( ccp ( 0, 1 ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	right->setBarChangeRate ( ccp ( 0, 1 ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );
}

const KDchar* SpriteProgressToVertical::subtitle ( KDvoid )
{
	return "ProgressTo Vertical";
}

//------------------------------------------------------------------
//
// SpriteProgressToRadialMidpointChanged
//
//------------------------------------------------------------------
KDvoid SpriteProgressToRadialMidpointChanged::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	// Our image on the left should be a radial progress indicator, clockwise
	CCProgressTimer*  left = CCProgressTimer::create ( "Images/blocks.png" );

	left->setType ( kCCProgressTimerTypeRadial );
	left->setMidpoint ( ccp ( 0.25f, 0.75f ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );


	// Our image on the left should be a radial progress indicator, counter clockwise
	CCProgressTimer*  right = CCProgressTimer::create ( "Images/blocks.png" );

	right->setType ( kCCProgressTimerTypeRadial );		
	right->setMidpoint ( ccp ( 0.75f, 0.25f ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );
}

const KDchar* SpriteProgressToRadialMidpointChanged::subtitle ( KDvoid )
{
    return "Radial w/ Different Midpoints";
}

//------------------------------------------------------------------
//
// SpriteProgressBarVarious
//
//------------------------------------------------------------------
KDvoid SpriteProgressBarVarious::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCProgressTimer*  left = CCProgressTimer::create ( "Images/grossinis_sister1.png" );

	left->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	left->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	left->setBarChangeRate ( ccp ( 1, 0 ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );
	

	CCProgressTimer*  middle = CCProgressTimer::create ( "Images/grossini.png" );

	middle->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	middle->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	middle->setBarChangeRate ( ccp ( 1, 1 ) );
	middle->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 ) );
	middle->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( middle );

	CCProgressTimer*  right = CCProgressTimer::create ( "Images/grossinis_sister2.png" );

	right->setType ( kCCProgressTimerTypeBar );		
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	right->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	right->setBarChangeRate ( ccp ( 0, 1 ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );
}

const KDchar* SpriteProgressBarVarious::subtitle ( KDvoid )
{
    return "ProgressTo Bar Mid";
}

//------------------------------------------------------------------
//
// SpriteProgressBarTintAndFade
//
//------------------------------------------------------------------
KDvoid SpriteProgressBarTintAndFade::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCLabelTTF*  pText;

    CCProgressTo*   to = CCProgressTo::create ( 6, 100 );
    CCAction*     tint = CCSequence::create
	(
		CCTintTo::create ( 1, 255,   0,   0 ),
		CCTintTo::create ( 1,   0, 255,   0 ),
		CCTintTo::create ( 1,   0,   0, 255 ),
		KD_NULL
	);
    CCAction*     fade = CCSequence::create
	(
		CCFadeTo::create ( 1.0f,   0 ),
		CCFadeTo::create ( 1.0f, 255 ),
		KD_NULL 
	);

	CCProgressTimer*  left = CCProgressTimer::create ( "Images/grossinis_sister1.png" );

	left->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	left->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	left->setBarChangeRate ( ccp ( 1, 0 ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );

	left->runAction ( CCRepeatForever::create ( (CCActionInterval*)   to->copy ( )->autorelease ( ) ) );
	left->runAction ( CCRepeatForever::create ( (CCActionInterval*) tint->copy ( )->autorelease ( ) ) );

	this->addChild ( left );

	pText = CCLabelTTF::create ( "Tint", "fonts/Marker Felt.ttf", 14.0f );
	pText->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pText );

	
	CCProgressTimer*  middle = CCProgressTimer::create ( "Images/grossini.png" );

	middle->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	middle->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	middle->setBarChangeRate ( ccp ( 1, 1 ) );
	middle->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 ) );

	this->addChild ( middle );

	middle->runAction ( CCRepeatForever::create ( (CCActionInterval*)   to->copy ( )->autorelease ( ) ) );
	middle->runAction ( CCRepeatForever::create ( (CCActionInterval*) fade->copy ( )->autorelease ( ) ) );

	pText = CCLabelTTF::create ( "Fade", "fonts/Marker Felt.ttf", 14.0f );
	pText->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pText );


	CCProgressTimer*  right = CCProgressTimer::create ( "Images/grossinis_sister2.png" );

	right->setType ( kCCProgressTimerTypeBar );		
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	right->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	right->setBarChangeRate ( ccp ( 0, 1 ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );

	this->addChild ( right );

	right->runAction ( CCRepeatForever::create ( (CCActionInterval*)   to->copy ( )->autorelease ( ) ) );
	right->runAction ( CCRepeatForever::create ( (CCActionInterval*) tint->copy ( )->autorelease ( ) ) );
	right->runAction ( CCRepeatForever::create ( (CCActionInterval*) fade->copy ( )->autorelease ( ) ) );

	pText = CCLabelTTF::create ( "Tint and Fade", "fonts/Marker Felt.ttf", 14.0f );
	pText->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pText );
}

const KDchar* SpriteProgressBarTintAndFade::subtitle ( KDvoid )
{
    return "ProgressTo Bar Mid";
}

//------------------------------------------------------------------
//
// SpriteProgressWithSpriteFrame
//
//------------------------------------------------------------------
KDvoid SpriteProgressWithSpriteFrame::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "animations/grossini.plist" );

	CCProgressTimer*  left = CCProgressTimer::create ( CCSprite::createWithSpriteFrameName ( "grossini_dance_01.png" ) );

	left->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	left->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	left->setBarChangeRate ( ccp ( 1, 0 ) );
	left->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );
	left->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( left );
	
	CCProgressTimer*  middle = CCProgressTimer::create ( CCSprite::createWithSpriteFrameName ( "grossini_dance_02.png" ) );

	middle->setType ( kCCProgressTimerTypeBar );
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	middle->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	middle->setBarChangeRate ( ccp ( 1, 1 ) );
	middle->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 ) );
	middle->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( middle );

	CCProgressTimer*  right = CCProgressTimer::create ( CCSprite::createWithSpriteFrameName ( "grossini_dance_04.png" ) );

	right->setType ( kCCProgressTimerTypeBar );		
	//    Setup for a bar starting from the bottom since the midpoint is 0 for the y
	right->setMidpoint ( ccp ( 0.5f, 0.5f ) );
	//    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
	right->setBarChangeRate ( ccp ( 0, 1 ) );
	right->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );
	right->runAction ( CCRepeatForever::create ( CCProgressTo::create ( 2, 100 ) ) );

	this->addChild ( right );
}

const KDchar* SpriteProgressWithSpriteFrame::subtitle ( KDvoid )
{
    return "Progress With Sprite Frame";
}

//------------------------------------------------------------------
//
// ProgressHorizontally
//
//------------------------------------------------------------------
KDvoid ProgressHorizontally::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
	CCProgressFromTo*  to;

	CCProgress*  left = CCProgress::create ( "xm_supports/progress.png" );

	left->setType ( kCCProgressTypeToRight );
	left->setPosition ( ccp ( tLyrSize.cx / 4, tLyrSize.cy / 2 - 20 ) );	  
	left->setContentSize ( CCSize ( 150, 80 ) );	

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	left->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );

	this->addChild ( left );
	

	CCProgress*  right = CCProgress::create ( "xm_supports/list_484_nor.9.png" );

	right->setType ( kCCProgressTypeToLeft );
	right->setPosition ( ccp ( tLyrSize.cx * 3 / 4, tLyrSize.cy / 2 - 20 ) );	
	right->setContentSize ( CCSize ( 150, 80 ) );

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	right->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );

	this->addChild ( right );
}

const KDchar* ProgressHorizontally::subtitle ( KDvoid )
{
	return "Progress Horizontally ( Android 9 Patch )";
}

//------------------------------------------------------------------
//
// ProgressVertically
//
//------------------------------------------------------------------
KDvoid ProgressVertically::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
	CCProgressFromTo*  to;
	
	CCProgress*  left = CCProgress::create ( "xm_supports/progress.png" );

	left->setType ( kCCProgressTypeToTop );
	left->setPosition ( ccp ( tLyrSize.cx / 4, tLyrSize.cy / 2 - 20 ) );	  
	left->setContentSize ( CCSize ( 80, 120 ) );	

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	left->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );
	this->addChild ( left );

	
	CCProgress*  right = CCProgress::create ( "xm_supports/list_484_nor.9.png" );

	right->setType ( kCCProgressTypeToBottom );
	right->setPosition ( ccp ( tLyrSize.cx * 3 / 4, tLyrSize.cy / 2 - 20 ) );	
	right->setContentSize ( CCSize ( 80, 120 ) );

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	right->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );
	this->addChild ( right );
}

const KDchar* ProgressVertically::subtitle ( KDvoid )
{
	return "Progress Vertically ( Android 9 Patch )";
}

//------------------------------------------------------------------
//
// ProgressBoth
//
//------------------------------------------------------------------
KDvoid ProgressBoth::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
	CCProgressFromTo*  to;
	
	CCProgress*  left = CCProgress::create ( "xm_supports/progress.png" );

	left->setType ( kCCProgressTypeToRightBottom );
	left->setPosition ( ccp ( tLyrSize.cx / 4, tLyrSize.cy / 2 - 20 ) );	  
	left->setContentSize ( CCSize ( 150, 80 ) );	

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	left->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );
	this->addChild ( left );
	

	CCProgress*  right = CCProgress::create ( "xm_supports/list_484_nor.9.png" );

	right->setType ( kCCProgressTypeToLeftTop );
	right->setPosition ( ccp ( tLyrSize.cx * 3 / 4, tLyrSize.cy / 2 - 20 ) );	
	right->setContentSize ( CCSize ( 150, 80 ) );

	to = CCProgressFromTo::create ( 1.5f, 0, 100 );
	right->runAction ( CCRepeatForever::create ( (CCActionInterval *) CCSequence::create ( to, CCDelayTime::create ( 1 ), to->reverse ( ), CCDelayTime::create ( 1 ), KD_NULL ) ) );
	this->addChild ( right );

}

const KDchar* ProgressBoth::subtitle ( KDvoid )
{
	return "Progress Horizontally and Vertically ( Android 9 Patch )";
}

//------------------------------------------------------------------
//
// ProgressSize
//
//------------------------------------------------------------------
ProgressSize::ProgressSize ( KDvoid )
: m_left ( KD_NULL )
, m_right ( KD_NULL )
, m_size ( CCSizeZero )
, m_dir_x ( KD_FALSE )
, m_dir_y ( KD_FALSE )
{

}

KDvoid ProgressSize::onEnter ( KDvoid )
{
	TestProgressActions::onEnter ( );
	
	const CCSize&  tLyrSize = this->getContentSize ( );
		
	m_left = CCProgress::create ( "xm_supports/progress.png" );
	m_left->setType ( kCCProgressTypeToRightBottom );
	m_left->setPosition ( ccp ( tLyrSize.cx / 4, tLyrSize.cy / 2 - 20 ) );	
	this->addChild ( m_left );


	m_right = CCProgress::create ( "xm_supports/list_484_nor.9.png" );
	m_right->setType ( kCCProgressTypeToLeftTop );
	m_right->setPosition ( ccp ( tLyrSize.cx * 3 / 4, tLyrSize.cy / 2 - 20 ) );	
	this->addChild ( m_right );

	schedule ( schedule_selector ( ProgressSize::onUpdate ), 0.02f );

	m_size  = CCSize ( 150, 80 );
	m_dir_x = KD_FALSE;
	m_dir_y = KD_FALSE;
}

KDvoid ProgressSize::onExit ( KDvoid )
{
	unschedule ( schedule_selector ( ProgressSize::onUpdate ) );
}

const KDchar* ProgressSize::subtitle ( KDvoid )
{
	return "Progress setting content size ( Android 9 Patch )";
}

KDvoid ProgressSize::onUpdate ( KDfloat dt )
{
	m_size.cx += m_dir_x ? 3 : -3;
	m_size.cy += m_dir_y ? 3 : -3;

	if ( m_size.cx > 150.f || m_size.cx < 0 )
	{
		m_dir_x = !m_dir_x;
	}

	if ( m_size.cy > 80.f || m_size.cy < 0 )
	{
		m_dir_y = !m_dir_y;
	}

	m_left ->setContentSize ( m_size );
	m_right->setContentSize ( CCSize ( m_size.cy, m_size.cx ) );
}