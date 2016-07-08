/* --------------------------------------------------------------------------
 *
 *      File            TestEaseActions.cpp
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
#include "TestEaseActions.h"

enum 
{
	kTagAction1 = 1,
	kTagAction2 = 2,
	kTagSlider  = 1,
};

TestEaseActions* TestEaseActions::create ( KDvoid )
{
	TestEaseActions*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new SpriteEase                 ( ); break;
		case  1 : pLayer = new SpriteEaseInOut            ( ); break;
		case  2 : pLayer = new SpriteEaseExponential      ( ); break;
		case  3 : pLayer = new SpriteEaseExponentialInOut ( ); break;
		case  4 : pLayer = new SpriteEaseSine             ( ); break;
		case  5 : pLayer = new SpriteEaseSineInOut        ( ); break;
		case  6 : pLayer = new SpriteEaseElastic          ( ); break;
		case  7 : pLayer = new SpriteEaseElasticInOut     ( ); break;
		case  8 : pLayer = new SpriteEaseBounce           ( ); break;
		case  9 : pLayer = new SpriteEaseBounceInOut      ( ); break;
		case 10 : pLayer = new SpriteEaseBack             ( ); break;
		case 11 : pLayer = new SpriteEaseBackInOut        ( ); break;
		case 12 : pLayer = new SpeedTest                  ( ); break;
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

KDuint TestEaseActions::count ( KDvoid )
{
	return 13;
}

KDbool TestEaseActions::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	m_grossini = CCSprite::create ( "Images/grossini.png" );
	m_grossini->setPosition ( ccp ( 60, tLyrSize.cy / 5 * 1 ) );
	this->addChild ( m_grossini, 3 );

	m_tamara = CCSprite::create ( "Images/grossinis_sister1.png" ); 
	m_tamara->setPosition ( ccp ( 60, tLyrSize.cy / 5 * 4 ) );
	this->addChild ( m_tamara, 1 );

	m_kathia = CCSprite::create ( "Images/grossinis_sister2.png" );
	m_kathia->setPosition ( ccp ( 60, tLyrSize.cy / 2 ) );
	this->addChild ( m_kathia, 2 );

	return KD_TRUE;
}

KDvoid TestEaseActions::positionForTwo ( KDvoid )
{	
	const CCSize&  tLyrSize = this->getContentSize ( );

	m_grossini->setPosition ( ccp ( 60, tLyrSize.cy / 3 * 1 ) );
	m_tamara  ->setPosition ( ccp ( 60, tLyrSize.cy / 3 * 2 ) );
	m_kathia  ->setVisible ( KD_FALSE );
}

#define CCCA(x)   ( (CCActionInterval*) x->copy ( )->autorelease ( ) )

//------------------------------------------------------------------
//
// SpriteEase
//
//------------------------------------------------------------------
KDvoid SpriteEase::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );
	
    const CCSize&  s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130,0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseIn::create ( CCCA ( move ), 2.5f);
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseOut::create ( CCCA ( move ), 2.5f);
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    
    
    CCAction *a2 = m_grossini->runAction(CCRepeatForever::create ( seq1 ) );
    a2->setTag(1);

    CCAction *a1 = m_tamara->runAction(CCRepeatForever::create ( seq2 ) );
    a1->setTag(1);

    CCAction *a = m_kathia->runAction(CCRepeatForever::create ( seq3 ) );
    a->setTag(1);

    schedule(schedule_selector(SpriteEase::testStopAction), 6.25f);

}

KDvoid SpriteEase::testStopAction ( KDfloat dt )
{
	unschedule ( schedule_selector ( SpriteEase::testStopAction ) );

	m_tamara  ->stopActionByTag ( 1 );
	m_kathia  ->stopActionByTag ( 1 );
	m_grossini->stopActionByTag ( 1 );
}

const KDchar* SpriteEase::title ( KDvoid )
{
	return "EaseIn - EaseOut - Stop";
}

//------------------------------------------------------------------
//
// SpriteEaseInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval*  move = CCMoveBy::create(3, ccp(s.cx-130,0 ) );
//    id move_back = move->reverse();
    
    CCActionInterval*  move_ease_inout1 = CCEaseInOut::create ( CCCA ( move ), 0.65f);
    CCActionInterval*  move_ease_inout_back1 = move_ease_inout1->reverse();
    
    CCActionInterval*  move_ease_inout2 = CCEaseInOut::create ( CCCA ( move ), 1.35f);
    CCActionInterval*  move_ease_inout_back2 = move_ease_inout2->reverse();

    CCActionInterval*  move_ease_inout3 = CCEaseInOut::create ( CCCA ( move ), 1.0f);
    CCActionInterval*  move_ease_inout_back3 = move_ease_inout3->reverse();
    
    CCDelayTime*  delay = CCDelayTime::create(0.25f);

    CCSequence*  seq1 = CCSequence::create( move_ease_inout1, delay, move_ease_inout_back1, CCCA(delay), KD_NULL);
    CCSequence*  seq2 = CCSequence::create( move_ease_inout2, CCCA(delay), move_ease_inout_back2, CCCA(delay), KD_NULL);
    CCSequence*  seq3 = CCSequence::create( move_ease_inout3, CCCA(delay), move_ease_inout_back3, CCCA(delay), KD_NULL);
        
    m_tamara->runAction(CCRepeatForever::create ( seq1 ) );
    m_kathia->runAction(CCRepeatForever::create ( seq2 ) );
    m_grossini->runAction(CCRepeatForever::create ( seq3 ) );

}

const KDchar* SpriteEaseInOut::title ( KDvoid )
{
	return "EaseInOut and rates";
}

//------------------------------------------------------------------
//
// SpriteEaseExponential
//
//------------------------------------------------------------------
KDvoid SpriteEaseExponential::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130,0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseExponentialIn::create ( CCCA ( move ) );
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseExponentialOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    

    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );
    m_kathia->runAction( CCRepeatForever::create ( seq3 ) );
}

const KDchar* SpriteEaseExponential::title ( KDvoid )
{
	return "ExpIn - ExpOut actions";
}

//------------------------------------------------------------------
//
// SpriteEaseExponentialInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseExponentialInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease = CCEaseExponentialInOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_back = move_ease->reverse();    //--> reverse()
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create( move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create( move_ease, delay, move_ease_back, CCCA(delay), KD_NULL);
    
    this->positionForTwo();
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );

}

const KDchar* SpriteEaseExponentialInOut::title ( KDvoid )
{
	return "EaseExponentialInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseSine
//
//------------------------------------------------------------------
KDvoid SpriteEaseSine::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseSineIn::create ( CCCA ( move ) );
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseSineOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime*  delay = CCDelayTime::create(0.25f);
        
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );
    m_kathia->runAction( CCRepeatForever::create ( seq3 ) );   

}

const KDchar* SpriteEaseSine::title ( KDvoid )
{
	return "EaseSineIn - EaseSineOut";
}

//------------------------------------------------------------------
//
// SpriteEaseSineInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseSineInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130,0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease = CCEaseSineInOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_back = move_ease->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease, CCCA(delay), move_ease_back, CCCA(delay), KD_NULL);

    this->positionForTwo();

    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );

}

const KDchar* SpriteEaseSineInOut::title ( KDvoid )
{
	return "EaseSineInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseElastic
//
//------------------------------------------------------------------
KDvoid SpriteEaseElastic::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseElasticIn::create ( CCCA ( move ) );
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseElasticOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);

    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );
    m_kathia->runAction( CCRepeatForever::create ( seq3 ) );    

}

const KDchar* SpriteEaseElastic::title ( KDvoid )
{
	return "Elastic In - Out actions";
}

//------------------------------------------------------------------
//
// SpriteEaseElasticInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseElasticInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );
	
    CCSize s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );

    CCActionInterval* move_ease_inout1 = CCEaseElasticInOut::create ( CCCA ( move ), 0.3f);
    CCActionInterval* move_ease_inout_back1 = move_ease_inout1->reverse();
    
    CCActionInterval* move_ease_inout2 = CCEaseElasticInOut::create ( CCCA ( move ), 0.45f);
    CCActionInterval* move_ease_inout_back2 = move_ease_inout2->reverse();
    
    CCActionInterval* move_ease_inout3 = CCEaseElasticInOut::create ( CCCA ( move ), 0.6f);
    CCActionInterval* move_ease_inout_back3 = move_ease_inout3->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
        
    CCSequence* seq1 = CCSequence::create(move_ease_inout1, delay, move_ease_inout_back1, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_inout2, CCCA(delay), move_ease_inout_back2, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_inout3, CCCA(delay), move_ease_inout_back3, CCCA(delay), KD_NULL);
    
    m_tamara->runAction( CCRepeatForever::create ( seq1 ) );
    m_kathia->runAction( CCRepeatForever::create ( seq2 ) );
    m_grossini->runAction( CCRepeatForever::create ( seq3 ) ); 
}

const KDchar* SpriteEaseElasticInOut::title ( KDvoid )
{
	return "EaseElasticInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseBounce
//
//------------------------------------------------------------------
KDvoid SpriteEaseBounce::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseBounceIn::create ( CCCA ( move ) );
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseBounceOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );
    m_kathia->runAction( CCRepeatForever::create ( seq3 ) );    

}

const KDchar* SpriteEaseBounce::title ( KDvoid )
{
	return "Bounce In - Out actions";
}

//------------------------------------------------------------------
//
// SpriteEaseBounceInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseBounceInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease = CCEaseBounceInOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_back = move_ease->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease, CCCA(delay), move_ease_back, CCCA(delay), KD_NULL);
    
    this->positionForTwo();
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) );
}

const KDchar* SpriteEaseBounceInOut::title ( KDvoid )
{
	return "EaseBounceInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseBack
//
//------------------------------------------------------------------
KDvoid SpriteEaseBack::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease_in = CCEaseBackIn::create ( CCCA ( move ) );
    CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    
    CCActionInterval* move_ease_out = CCEaseBackOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_out_back = move_ease_out->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease_in, CCCA(delay), move_ease_in_back, CCCA(delay), KD_NULL);
    CCSequence* seq3 = CCSequence::create(move_ease_out, CCCA(delay), move_ease_out_back, CCCA(delay), KD_NULL);
    
    m_grossini->runAction(CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction(CCRepeatForever::create ( seq2 ) );
    m_kathia->runAction(CCRepeatForever::create ( seq3 ) ); 
}

const KDchar* SpriteEaseBack::title ( KDvoid )
{
	return "Back In - Out actions";
}

//------------------------------------------------------------------
//
// SpriteEaseBackInOut
//
//------------------------------------------------------------------
KDvoid SpriteEaseBackInOut::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );
    
    CCActionInterval* move = CCMoveBy::create(3, ccp(s.cx-130, 0 ) );
    CCActionInterval* move_back = move->reverse();
    
    CCActionInterval* move_ease = CCEaseBackInOut::create ( CCCA ( move ) );
    CCActionInterval* move_ease_back = move_ease->reverse();
    
    CCDelayTime *delay = CCDelayTime::create(0.25f);
    
    CCSequence* seq1 = CCSequence::create(move, delay, move_back, CCCA(delay), KD_NULL);
    CCSequence* seq2 = CCSequence::create(move_ease, CCCA(delay), move_ease_back, CCCA(delay), KD_NULL);
    
    this->positionForTwo();
    
    m_grossini->runAction( CCRepeatForever::create ( seq1 ) );
    m_tamara->runAction( CCRepeatForever::create ( seq2 ) ); 
}

const KDchar* SpriteEaseBackInOut::title ( KDvoid )
{
	return "EaseBackInOut action";
}

//------------------------------------------------------------------
//
// SpeedTest
//
//------------------------------------------------------------------

KDvoid SpeedTest::onEnter ( KDvoid )
{
	TestEaseActions::onEnter ( );

    CCSize s = this->getContentSize ( );

    // rotate and jump
    CCActionInterval *jump1 = CCJumpBy::create(4, ccp(-s.cx+80, 0), 100, 4);
    CCActionInterval *jump2 = jump1->reverse();
    CCActionInterval *rot1 = CCRotateBy::create(4, 360*2);
    CCActionInterval *rot2 = rot1->reverse();
    
    CCSequence* seq3_1 = CCSequence::create(jump2, jump1, KD_NULL);
    CCSequence* seq3_2 = CCSequence::create( rot1, rot2, KD_NULL);
    CCSpawn* spawn = CCSpawn::create(seq3_1, seq3_2, KD_NULL);
    CCSpeed* action = CCSpeed::create(CCRepeatForever::create ( spawn), 1.0f);
    action->setTag(kTagAction1);
    
    CCAction* action2 = (CCAction*)(action->copy()->autorelease( ) );
    CCAction* action3 = (CCAction*)(action->copy()->autorelease( ) );

    action2->setTag(kTagAction1);
    action3->setTag(kTagAction1);
    
    m_grossini->runAction(action2);
    m_tamara->runAction(action3);
    m_kathia->runAction(action);
    
    this->schedule(schedule_selector(SpeedTest::altertime), 1.0f);
}

KDvoid SpeedTest::altertime ( KDfloat dt )
{	
	CCSpeed*  action1 = (CCSpeed*) ( m_grossini->getActionByTag ( kTagAction1 ) );
	CCSpeed*  action2 = (CCSpeed*) ( m_tamara  ->getActionByTag ( kTagAction1 ) );
	CCSpeed*  action3 = (CCSpeed*) ( m_kathia  ->getActionByTag ( kTagAction1 ) );

	action1->setSpeed ( CCRANDOM_0_1 ( ) * 2 );
	action2->setSpeed ( CCRANDOM_0_1 ( ) * 2 );
	action3->setSpeed ( CCRANDOM_0_1 ( ) * 2 );
}

const KDchar* SpeedTest::title ( KDvoid )
{
	return "Speed action";
}
