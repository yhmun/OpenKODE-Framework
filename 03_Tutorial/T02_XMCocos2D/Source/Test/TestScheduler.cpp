/* --------------------------------------------------------------------------
 *
 *      File            TestScheduler.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestScheduler.h"

enum
{
	kTagAnimationDance = 1,
};

TestScheduler* TestScheduler::create ( KDvoid )
{
	TestScheduler*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new SchedulerTimeScale				( ); break;
		case  1 : pLayer = new TwoSchedulers					( ); break;
		case  2 : pLayer = new SchedulerAutoremove				( ); break;
		case  3 : pLayer = new SchedulerPauseResume				( ); break;
		case  4 : pLayer = new SchedulerPauseResumeAll			( ); break;
		case  5 : pLayer = new SchedulerPauseResumeAllUser		( ); break;
		case  6 : pLayer = new SchedulerUnscheduleAll			( ); break;
		case  7 : pLayer = new SchedulerUnscheduleAllHard		( ); break;
		case  8 : pLayer = new SchedulerUnscheduleAllUserLevel	( ); break;
		case  9 : pLayer = new SchedulerSchedulesAndRemove		( ); break;
		case 10 : pLayer = new SchedulerUpdate					( ); break;
		case 11 : pLayer = new SchedulerUpdateAndCustom			( ); break;
		case 12 : pLayer = new SchedulerUpdateFromCustom		( ); break;
		case 13 : pLayer = new RescheduleSelector				( ); break;
		case 14 : pLayer = new SchedulerDelayAndRepeat			( ); break;
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

KDuint TestScheduler::count ( KDvoid )
{
	return 15;
}

//------------------------------------------------------------------
//
// SchedulerAutoremove
//
//------------------------------------------------------------------
KDvoid SchedulerAutoremove::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    schedule ( schedule_selector ( SchedulerAutoremove::autoremove ), 0.5f );
    schedule ( schedule_selector ( SchedulerAutoremove::tick ), 0.5f );

    m_fAccum = 0;
}

KDvoid SchedulerAutoremove::autoremove ( KDfloat fDelta )
{
    m_fAccum += fDelta;

	CCLOG ( "Time : %2.1f", m_fAccum );

    if ( m_fAccum > 3 )
    {
        unschedule ( schedule_selector ( SchedulerAutoremove::autoremove ) );

		CCLOG ( "SchedulerAutoremove - This scheduler removed" );
    }

	CCLOG ( "SchedulerAutoremove - This scheduler should not be removed" );
}

KDvoid SchedulerAutoremove::tick ( KDfloat fDelta )
{
	CCLOG ( "SchedulerAutoremove - tick : %2.1f", fDelta );
}

const KDchar* SchedulerAutoremove::title ( KDvoid )
{
    return "Self-remove an scheduler";
}

const KDchar* SchedulerAutoremove::subtitle ( KDvoid )
{
    return "1 scheduler will be autoremoved in 3 seconds.\nSee console.";
}

//------------------------------------------------------------------
//
// SchedulerPauseResume
//
//------------------------------------------------------------------
KDvoid SchedulerPauseResume::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    schedule ( schedule_selector ( SchedulerPauseResume::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerPauseResume::tick2 ), 0.5f );
    schedule ( schedule_selector ( SchedulerPauseResume::pause ), 0.5f );
}

KDvoid SchedulerPauseResume::tick1 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerPauseResume - tick1 : %2.1f", fDelta );
}

KDvoid SchedulerPauseResume::tick2 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerPauseResume - tick2 : %2.1f", fDelta );
}

KDvoid SchedulerPauseResume::pause ( KDfloat fDelta )
{
	CCLOG ( "SchedulerPauseResume - pause : %2.1f", fDelta );

    CCDirector::sharedDirector ( )->getScheduler ( )->pauseTarget ( this );
}

const KDchar* SchedulerPauseResume::title ( KDvoid )
{
    return "Pause / Resume";
}

const KDchar* SchedulerPauseResume::subtitle ( KDvoid )
{
    return "Scheduler should be paused after 3 seconds.\nSee console.";
}

//------------------------------------------------------------------
//
// SchedulerPauseResumeAll
//
//------------------------------------------------------------------

KDvoid SchedulerPauseResumeAll::onEnter ( KDvoid )
{
	m_pPausedTargets = KD_NULL;

    TestScheduler::onEnter ( );

    CCSprite*   pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    this->addChild ( pSprite );
    pSprite->runAction(CCRepeatForever::create ( CCRotateBy::create ( 3.0, 360 ) ) );

    scheduleUpdate ( );

    schedule ( schedule_selector ( SchedulerPauseResumeAll::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerPauseResumeAll::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerPauseResumeAll::pause ), 3.0f, KD_FALSE, 0 );
}

KDvoid SchedulerPauseResumeAll::update ( KDfloat fDelta )
{
    // do nothing
}

KDvoid SchedulerPauseResumeAll::onExit ( KDvoid )
{
    if ( m_pPausedTargets )
    {
        CCDirector::sharedDirector ( )->getScheduler ( )->resumeTargets ( m_pPausedTargets );
		CC_SAFE_RELEASE ( m_pPausedTargets );
    }
}

KDvoid SchedulerPauseResumeAll::tick1 ( KDfloat fDelta )
{
    CCLOG ( "tick1" );
}

KDvoid SchedulerPauseResumeAll::tick2 ( KDfloat fDelta )
{
    CCLOG ( "tick2" );
}

KDvoid SchedulerPauseResumeAll::pause ( KDfloat fDelta )
{
    CCLOG ( "Pausing" );

    m_pPausedTargets = CCDirector::sharedDirector ( )->getScheduler ( )->pauseAllTargets ( );
    CC_SAFE_RETAIN ( m_pPausedTargets );
    
    KDuint  c = m_pPausedTargets->count ( );
    
    if ( c > 2 )
    {
        // should have only 2 items: CCActionManager, self
        CCLOG ( "Error: pausedTargets should have only 2 items, and not %u", (KDuint) c );
    }
}

KDvoid SchedulerPauseResumeAll::resume ( KDfloat fDelta )
{
    CCLOG ( "Resuming" );

    CCDirector::sharedDirector ( )->getScheduler ( )->resumeTargets ( m_pPausedTargets );
    CC_SAFE_RELEASE ( m_pPausedTargets );
}

const KDchar* SchedulerPauseResumeAll::title ( KDvoid )
{
    return "Pause / Resume";
}

const KDchar* SchedulerPauseResumeAll::subtitle ( KDvoid )
{
    return "Everything will pause after 3s, then resume at 5s. See console";
}

//------------------------------------------------------------------
//
// SchedulerPauseResumeAllUser
//
//------------------------------------------------------------------
KDvoid SchedulerPauseResumeAllUser::onEnter ( KDvoid )
{
	m_pPausedTargets = KD_NULL;

    TestScheduler::onEnter ( );

    CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    this->addChild ( pSprite );
    pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3.0, 360 ) ) );

    schedule ( schedule_selector ( SchedulerPauseResumeAllUser::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerPauseResumeAllUser::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerPauseResumeAllUser::pause ), 3.0f, KD_FALSE, 0 );
}

KDvoid SchedulerPauseResumeAllUser::onExit ( KDvoid )
{
    if ( m_pPausedTargets )
    {
        CCDirector::sharedDirector ( )->getScheduler ( )->resumeTargets ( m_pPausedTargets );
		CC_SAFE_RELEASE ( m_pPausedTargets );
    }
}

KDvoid SchedulerPauseResumeAllUser::tick1 ( KDfloat fDelta )
{
    CCLOG ( "tick1" );
}

KDvoid SchedulerPauseResumeAllUser::tick2 ( KDfloat fDelta )
{
    CCLOG ( "tick2" );
}

KDvoid SchedulerPauseResumeAllUser::pause ( KDfloat fDelta )
{
    CCLOG ( "Pausing" );

    m_pPausedTargets = CCDirector::sharedDirector ( )->getScheduler ( )->pauseAllTargetsWithMinPriority ( kCCPriorityNonSystemMin );
    CC_SAFE_RETAIN ( m_pPausedTargets );
}

KDvoid SchedulerPauseResumeAllUser::resume ( KDfloat fDelta )
{
    CCLOG ("Resuming" );
  
    CCDirector::sharedDirector ( )->getScheduler ( )->resumeTargets ( m_pPausedTargets );
    CC_SAFE_RELEASE ( m_pPausedTargets );
}

const KDchar* SchedulerPauseResumeAllUser::title ( KDvoid )
{
    return "Pause / Resume";
}

const KDchar* SchedulerPauseResumeAllUser::subtitle ( KDvoid )
{
    return "Everything will pause after 3s, then resume at 5s.\nSee console";
}

//------------------------------------------------------------------
//
// SchedulerUnscheduleAll
//
//------------------------------------------------------------------
KDvoid SchedulerUnscheduleAll::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    schedule ( schedule_selector ( SchedulerUnscheduleAll::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAll::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerUnscheduleAll::tick3 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAll::tick4 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAll::unscheduleAll ), 4 );
}

KDvoid SchedulerUnscheduleAll::tick1 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAll - tick1 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAll::tick2 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAll - tick2 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAll::tick3 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAll - tick3 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAll::tick4 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAll - tick4 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAll::unscheduleAll ( KDfloat fDelta )
{
    unscheduleAllSelectors ( );
}

const KDchar* SchedulerUnscheduleAll::title ( KDvoid )
{
    return "Unschedule All selectors";
}

const KDchar* SchedulerUnscheduleAll::subtitle ( KDvoid )
{
    return "All scheduled selectors will be unscheduled in 4 seconds.\nSee console.";
}

//------------------------------------------------------------------
//
// SchedulerUnscheduleAllHard
//
//------------------------------------------------------------------
KDvoid SchedulerUnscheduleAllHard::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    this->addChild ( pSprite );
    pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3.0, 360 ) ) );

	m_bActionManagerActive = KD_TRUE;

    schedule ( schedule_selector ( SchedulerUnscheduleAllHard::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllHard::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllHard::tick3 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllHard::tick4 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllHard::unscheduleAll ), 4 );
}

KDvoid SchedulerUnscheduleAllHard::onExit ( KDvoid )
{
    if ( !m_bActionManagerActive )
	{
        // Restore the director's action manager.
        CCDirector*  pDirector = CCDirector::sharedDirector ( );
        pDirector->getScheduler ( )->scheduleUpdateForTarget ( pDirector->getActionManager ( ), kCCPrioritySystem, KD_FALSE );
    }

	TestScheduler::onExit ( );
}

KDvoid SchedulerUnscheduleAllHard::tick1 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAllHard - tick1 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAllHard::tick2 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAllHard - tick2 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAllHard::tick3 ( KDfloat fDelta )
{
 	CCLOG ( "SchedulerUnscheduleAllHard - tick3 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAllHard::tick4 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUnscheduleAllHard - tick4 : %2.1f", fDelta );
}

KDvoid SchedulerUnscheduleAllHard::unscheduleAll ( KDfloat fDelta )
{
    CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleAll ( );

	m_bActionManagerActive = KD_FALSE;
}

const KDchar* SchedulerUnscheduleAllHard::title ( KDvoid )
{
    return "Unschedule All selectors (HARD)";
}

const KDchar* SchedulerUnscheduleAllHard::subtitle ( KDvoid )
{
    return "Unschedules all selectors after 4s. Uses CCScheduler.\nSee console.";
}

//------------------------------------------------------------------
//
// SchedulerUnscheduleAllUserLevel
//
//------------------------------------------------------------------
KDvoid SchedulerUnscheduleAllUserLevel::onEnter ( KDvoid )
{
    TestScheduler::onEnter ( );

    CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    this->addChild ( pSprite );
    pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3.0, 360 ) ) );

    schedule ( schedule_selector ( SchedulerUnscheduleAllUserLevel::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllUserLevel::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllUserLevel::tick3 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllUserLevel::tick4 ), 1.5f );
    schedule ( schedule_selector ( SchedulerUnscheduleAllUserLevel::unscheduleAll ), 4 );
}

void SchedulerUnscheduleAllUserLevel::tick1 ( KDfloat fDelta )
{
    CCLOG ( "tick1" );
}

void SchedulerUnscheduleAllUserLevel::tick2 ( KDfloat fDelta )
{
    CCLOG ( "tick2" );
}

void SchedulerUnscheduleAllUserLevel::tick3 ( KDfloat fDelta )
{
    CCLOG ( "tick3" );
}

void SchedulerUnscheduleAllUserLevel::tick4 ( KDfloat fDelta )
{
    CCLOG ( "tick4" );
}

void SchedulerUnscheduleAllUserLevel::unscheduleAll ( KDfloat fDelta )
{
    CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleAllWithMinPriority ( kCCPriorityNonSystemMin );
}

const KDchar* SchedulerUnscheduleAllUserLevel::title ( KDvoid )
{
    return "Unschedule All user selectors";
}

const KDchar* SchedulerUnscheduleAllUserLevel::subtitle ( KDvoid )
{
    return "Unschedules all user selectors after 4s. Action should not stop.\nSee console";
}

//------------------------------------------------------------------
//
// SchedulerSchedulesAndRemove
//
//------------------------------------------------------------------
KDvoid SchedulerSchedulesAndRemove::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    schedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick1 ), 0.5f );
    schedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick2 ), 1.0f );
    schedule ( schedule_selector ( SchedulerSchedulesAndRemove::scheduleAndUnschedule ), 4.0f );
}

KDvoid SchedulerSchedulesAndRemove::tick1 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerSchedulesAndRemove - tick1 : %2.1f", fDelta );
}

KDvoid SchedulerSchedulesAndRemove::tick2 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerSchedulesAndRemove - tick2 : %2.1f", fDelta );
}

KDvoid SchedulerSchedulesAndRemove::tick3 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerSchedulesAndRemove - tick3 : %2.1f", fDelta );
}

KDvoid SchedulerSchedulesAndRemove::tick4 ( KDfloat fDelta )
{
	CCLOG ( "SchedulerSchedulesAndRemove - tick4 : %2.1f", fDelta );
}

const KDchar* SchedulerSchedulesAndRemove::title ( KDvoid )
{
    return "Schedule from Schedule";
}

const KDchar* SchedulerSchedulesAndRemove::subtitle ( KDvoid )
{
    return "Will unschedule and schedule selectors in 4s.\nSee console.";
}

KDvoid SchedulerSchedulesAndRemove::scheduleAndUnschedule ( KDfloat fDelta )
{
    unschedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick1 ) );
    unschedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick2 ) );
    unschedule ( schedule_selector ( SchedulerSchedulesAndRemove::scheduleAndUnschedule ) );

    schedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick3 ), 1.0f );
    schedule ( schedule_selector ( SchedulerSchedulesAndRemove::tick4 ), 1.0f );
}

//------------------------------------------------------------------
//
// TempNode
//
//------------------------------------------------------------------
TempNode::~TempNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pString );
}

KDvoid TempNode::initWithString ( CCString* pStr, KDint nPriority )
{
    m_pString = pStr;
    m_pString->retain ( );

    scheduleUpdateWithPriority ( nPriority );
}

//------------------------------------------------------------------
//
// SchedulerUpdate
//
//------------------------------------------------------------------
KDvoid SchedulerUpdate::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

	for ( KDuint i = 0; i < 6; i++ )
	{
		KDint			nPriority;
		const KDchar*	szStr;

		switch ( i )
		{
			case 0 : szStr = "---"; nPriority =  50; break;
			case 1 : szStr = "3rd"; nPriority =   0; break;
			case 2 : szStr = "1st"; nPriority = -10; break;
			case 3 : szStr = "4th"; nPriority =  10; break;
			case 4 : szStr = "5th"; nPriority =  20; break;
			case 5 : szStr = "2nd"; nPriority =  -5; break;
		}

		TempNode*	pTest = new TempNode ( );
		CCString*	pStr  = new CCString ( szStr );		

		pTest->initWithString ( pStr, nPriority );
		addChild ( pTest );

		pStr->release ( );
		pTest->release ( );
	}

    schedule ( schedule_selector ( SchedulerUpdate::removeUpdates ), 4.0f );
}

KDvoid SchedulerUpdate::removeUpdates ( KDfloat fDelta )
{    
    CCObject*  pObject;
	CCArray*   pChildren = getChildren ( );
    CCARRAY_FOREACH ( pChildren, pObject )
    {
		CCNode*  pNode = (CCNode*) pObject;

		CC_BREAK_IF ( !pNode );

        pNode->unscheduleAllSelectors ( );
    }
}

const KDchar* SchedulerUpdate::title ( KDvoid )
{
    return "Schedule update with priority";
}

const KDchar* SchedulerUpdate::subtitle ( KDvoid )
{
    return "3 scheduled updates. Priority should work. Stops in 4s.";
}

//------------------------------------------------------------------
//
// SchedulerUpdateAndCustom
//
//------------------------------------------------------------------
KDvoid SchedulerUpdateAndCustom::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    scheduleUpdate ( );
    schedule ( schedule_selector ( SchedulerUpdateAndCustom::tick ) );
    schedule ( schedule_selector ( SchedulerUpdateAndCustom::stopSelectors ), 0.4f );
}

KDvoid SchedulerUpdateAndCustom::update ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUpdateAndCustom - update : %2.1f", fDelta );
}

KDvoid SchedulerUpdateAndCustom::tick ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUpdateAndCustom - tick : %2.1f", fDelta );
}

KDvoid SchedulerUpdateAndCustom::stopSelectors ( KDfloat fDelta )
{
    unscheduleAllSelectors ( );
}

const KDchar* SchedulerUpdateAndCustom::title ( KDvoid )
{
    return "Schedule Update + custom selector";
}

const KDchar* SchedulerUpdateAndCustom::subtitle ( KDvoid )
{
    return "Update + custom selector at the same time. Stops in 4s.\nSee console.";
}

//------------------------------------------------------------------
//
// SchedulerUpdateFromCustom
//
//------------------------------------------------------------------
KDvoid SchedulerUpdateFromCustom::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    schedule ( schedule_selector ( SchedulerUpdateFromCustom::schedUpdate ), 2.0f );
}

KDvoid SchedulerUpdateFromCustom::update ( KDfloat fDelta )
{
	CCLOG ( "SchedulerUpdateFromCustom - update : %2.1f", fDelta );
}

KDvoid SchedulerUpdateFromCustom::schedUpdate ( KDfloat fDelta )
{
    unschedule ( schedule_selector ( SchedulerUpdateFromCustom::schedUpdate ) );
    scheduleUpdate ( );
    schedule ( schedule_selector ( SchedulerUpdateFromCustom::stopUpdate ), 2.0f );
}

KDvoid SchedulerUpdateFromCustom::stopUpdate ( KDfloat fDelta )
{
    unscheduleUpdate ( );
    unschedule ( schedule_selector ( SchedulerUpdateFromCustom::stopUpdate ) );
}

const KDchar* SchedulerUpdateFromCustom::title ( KDvoid )
{
    return "Schedule Update in 2 sec";
}

const KDchar* SchedulerUpdateFromCustom::subtitle ( KDvoid )
{
    return "Update schedules in 2 secs. Stops 2 sec later.\nSee console.";
}

//------------------------------------------------------------------
//
// RescheduleSelector
//
//------------------------------------------------------------------
KDvoid RescheduleSelector::onEnter ( KDvoid )
{
	TestScheduler::onEnter ( );

    m_fInterval = 1.0f;
    m_nTicks = 0;

    schedule ( schedule_selector ( RescheduleSelector::schedUpdate ), m_fInterval );
}

const KDchar* RescheduleSelector::title ( KDvoid )
{
    return "Reschedule Selector";
}

const KDchar* RescheduleSelector::subtitle ( KDvoid )
{
    return "Interval is 1 second, then 2, then 3...\nSee console.";
}

KDvoid RescheduleSelector::schedUpdate ( KDfloat fDelta )
{
    m_nTicks++;

	CCLOG ( "RescheduleSelector - schedUpdate : %2.1f", fDelta );

    if ( m_nTicks > 3 )
    {
        m_fInterval += 1.0f;
        schedule ( schedule_selector ( RescheduleSelector::schedUpdate ), m_fInterval );
        m_nTicks = 0;
    }
}

//------------------------------------------------------------------
//
// RescheduleSelector
//
//------------------------------------------------------------------
KDvoid SchedulerDelayAndRepeat::onEnter ( KDvoid )
{
    TestScheduler::onEnter ( );
    schedule ( schedule_selector ( SchedulerDelayAndRepeat::update ), 0, 4 , 3.f );
    CCLOG ( "update is scheduled should begin after 3 seconds" );
}

const KDchar* SchedulerDelayAndRepeat::title ( KDvoid )
{
    return "Schedule with delay of 3 sec, repeat 4 times";
}

const KDchar* SchedulerDelayAndRepeat::subtitle ( KDvoid )
{
    return "After 5 x executed, method unscheduled.\nSee console";
}

void SchedulerDelayAndRepeat::update ( KDfloat fDelta )
{
    CCLOG ( "update called:%f", fDelta );
}

//------------------------------------------------------------------
//
// SchedulerTimeScale
//
//------------------------------------------------------------------
CCControlSlider* SchedulerTimeScale::sliderCtl ( KDvoid )
{
    CCControlSlider*  pSlider = CCControlSlider::create ( "extensions/sliderTrack2.png","extensions/sliderProgress2.png" ,"extensions/sliderThumb.png" );

    pSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( SchedulerTimeScale::sliderAction ), kCCControlEventValueChanged );

    pSlider->setMinimumValue ( -3.0f );
    pSlider->setMaximumValue (  3.0f );
    pSlider->setValue ( 1.0f );

    return pSlider;
}

KDvoid SchedulerTimeScale::sliderAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
    CCControlSlider*  pSliderCtl = (CCControlSlider*) pSender;
    KDfloat  fScale;
    fScale = pSliderCtl->getValue ( );

    CCDirector::sharedDirector ( )->getScheduler ( )->setTimeScale ( fScale );
}

KDvoid SchedulerTimeScale::onEnter ( KDvoid )
{
    TestScheduler::onEnter ( );

    const CCSize&  tSize = this->getContentSize ( );

    // rotate and jump
    CCActionInterval*  jump1 = CCJumpBy::create ( 4, ccp ( -tSize.cx + 80, 0 ), 100, 4 );
    CCActionInterval*  jump2 = jump1->reverse ( );
    CCActionInterval*  rot1  = CCRotateBy::create ( 4, 360 * 2 );
    CCActionInterval*  rot2  = rot1->reverse ( );

    CCSequence*  seq3_1 = CCSequence::create ( jump2, jump1, KD_NULL );
    CCSequence*  seq3_2 = CCSequence::create ( rot1, rot2, KD_NULL );
    CCSpawn*     spawn  = CCSpawn::create ( seq3_1, seq3_2, KD_NULL );
    CCRepeat*	 action = CCRepeat::create ( spawn, 50 );

    CCRepeat*   action2 = (CCRepeat*) action->copy ( )->autorelease ( );
    CCRepeat*   action3 = (CCRepeat*) action->copy ( )->autorelease ( );

    CCSprite*   grossini = CCSprite::create ( "Images/grossini.png" );
    CCSprite*   tamara = CCSprite::create ( "Images/grossinis_sister1.png" );
    CCSprite*   kathia = CCSprite::create ( "Images/grossinis_sister2.png" );

    grossini->setPosition ( ccp ( 40,80 ) );
    tamara->setPosition   ( ccp ( 40,80 ) );
    kathia->setPosition   ( ccp ( 40,80 ) );

    addChild ( grossini );
    addChild ( tamara );
    addChild ( kathia );

    grossini->runAction ( CCSpeed::create ( action , 0.5f ) );
      tamara->runAction ( CCSpeed::create ( action2, 1.5f ) );
      kathia->runAction ( CCSpeed::create ( action3, 1.0f ) );

    CCParticleSystem*  pEmitter = CCParticleFireworks::create ( );
    pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
    addChild ( pEmitter );

    m_pSliderCtl = sliderCtl ( );
    m_pSliderCtl->setPosition ( ccp ( tSize.cx / 2.0f, tSize.cy / 3.0f ) );

    addChild ( m_pSliderCtl );
}

KDvoid SchedulerTimeScale::onExit ( KDvoid )
{
    // restore scale
    CCDirector::sharedDirector ( )->getScheduler ( )->setTimeScale ( 1 );

    TestScheduler::onExit ( );
}

const KDchar* SchedulerTimeScale::title ( KDvoid )
{
    return "Scheduler timeScale Test";
}

const KDchar* SchedulerTimeScale::subtitle ( KDvoid )
{
    return "Fast-forward and rewind using scheduler.timeScale";
}

//------------------------------------------------------------------
//
// TwoSchedulers
//
//------------------------------------------------------------------
CCControlSlider* TwoSchedulers::sliderCtl ( KDvoid )
{
	CCControlSlider*  pSlider = CCControlSlider::create
	(
		"extensions/sliderTrack2.png","extensions/sliderProgress2.png" ,"extensions/sliderThumb.png" 
	);

	pSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( TwoSchedulers::sliderAction ), kCCControlEventValueChanged );

	// in case the parent view draws with a custom color or gradient, use a transparent color
	pSlider->setMinimumValue ( 0.0f );
	pSlider->setMaximumValue ( 2.0f );
	pSlider->setValue ( 1.0f );

    return pSlider;
}

KDvoid TwoSchedulers::sliderAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
    CCControlSlider*  pSlider = (CCControlSlider*) pSender;
    KDfloat  fScale = pSlider->getValue ( );

    if ( pSender == m_pSliderCtl1 )
	{
        m_pSched1->setTimeScale ( fScale );
	}
    else
	{
        m_pSched2->setTimeScale ( fScale );
	}
}

KDvoid TwoSchedulers::onEnter ( KDvoid )
{
    TestScheduler::onEnter ( );

    const CCSize&  tSize = this->getContentSize ( );

	// rotate and jump
	CCActionInterval*  pJump1 = CCJumpBy::create ( 4, ccp ( 0, 0 ), 100, 4 );
	CCActionInterval*  pJump2 = pJump1->reverse ( );

	CCSequence*  pSeq = CCSequence::create ( pJump2, pJump1, KD_NULL );
	CCRepeatForever*  pAction = CCRepeatForever::create ( pSeq );

	//
	// Center
	//
	CCSprite*  pGrossini = CCSprite::create ( "Images/grossini.png" );
	addChild ( pGrossini );
	pGrossini->setPosition ( ccp ( tSize.cx / 2, tSize.cy / 2 ) );
	pGrossini->runAction ( (CCAction*) pAction->copy ( )->autorelease ( ) );

	CCScheduler*  pDefaultScheduler = CCDirector::sharedDirector ( )->getScheduler ( );

	//
	// Left:
	//

	// Create a new scheduler, and link it to the main scheduler
	m_pSched1 = new CCScheduler ( );

	pDefaultScheduler->scheduleUpdateForTarget ( m_pSched1, 0, KD_FALSE );

	// Create a new ActionManager, and link it to the new scheudler
	m_pActionManager1 = new CCActionManager ( );
	m_pSched1->scheduleUpdateForTarget ( m_pActionManager1, 0, KD_FALSE );

	for( KDuint i = 0; i < 10; i++ ) 
	{
		CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );

		// IMPORTANT: Set the actionManager running any action
		pSprite->setActionManager ( m_pActionManager1 );

		addChild ( pSprite );
		pSprite->setPosition ( ccp ( 30 + 15 * i, tSize.cy / 2 ) );

		pSprite->runAction ( (CCAction*) pAction->copy ( )->autorelease ( ) );
	}


	//
	// Right:
	//

	// Create a new scheduler, and link it to the main scheduler
	m_pSched2 = new CCScheduler ( );;
	pDefaultScheduler->scheduleUpdateForTarget ( m_pSched2, 0, KD_FALSE );

	// Create a new ActionManager, and link it to the new scheudler
	m_pActionManager2 = new CCActionManager ( );
	m_pSched2->scheduleUpdateForTarget ( m_pActionManager2, 0, KD_FALSE );

	for ( KDuint i = 0; i < 10; i++ ) 
	{
		CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister2.png" );

		// IMPORTANT: Set the actionManager running any action
		pSprite->setActionManager ( m_pActionManager2 );

		addChild ( pSprite );
		pSprite->setPosition ( ccp ( tSize.cx - 30 - 15 * i, tSize.cy / 2 ) );

		pSprite->runAction ( (CCAction*) pAction->copy ( )->autorelease ( ) );
	}

	m_pSliderCtl1 = sliderCtl ( );
	addChild ( m_pSliderCtl1 );
	m_pSliderCtl1->retain ( );
	m_pSliderCtl1->setPosition ( ccp ( tSize.cx / 4.0f, tSize.cy / 2 - 150 ) );

	m_pSliderCtl2 = sliderCtl ( );
	addChild ( m_pSliderCtl2 );
	m_pSliderCtl2->retain ( );
	m_pSliderCtl2->setPosition ( ccp ( tSize.cx / 4.0f * 3.0f, tSize.cy / 2 - 150 ) );
}

KDvoid TwoSchedulers::onExit ( KDvoid )
{
    CCScheduler*  pDefaultScheduler = CCDirector::sharedDirector ( )->getScheduler ( );

    pDefaultScheduler->unscheduleAllForTarget ( m_pSched1 );
    pDefaultScheduler->unscheduleAllForTarget ( m_pSched2 );

    m_pSliderCtl1->release ( );
    m_pSliderCtl2->release ( );

    m_pSched1->release ( );
    m_pSched2->release ( );

    m_pActionManager1->release ( );
    m_pActionManager2->release ( );

	TestScheduler::onExit ( );
}

const KDchar* TwoSchedulers::title ( KDvoid )
{
    return "Two custom schedulers";
}

const KDchar* TwoSchedulers::subtitle ( KDvoid )
{
    return "Three schedulers. 2 custom + 1 default.\nTwo different time scales";
}
