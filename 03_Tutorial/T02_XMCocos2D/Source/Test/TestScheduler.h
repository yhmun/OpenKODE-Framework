/* --------------------------------------------------------------------------
 *
 *      File            TestScheduler.h
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

#ifndef __TestScheduler_h__
#define __TestScheduler_h__

#include "../TestBasic.h"

class TestScheduler : public TestBasic
{
	public :

		static  TestScheduler*	create ( KDvoid );

	protected :		

		virtual KDuint			count  ( KDvoid );
};

class SchedulerAutoremove : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					autoremove	( KDfloat fDelta );
		KDvoid					tick		( KDfloat fDelta );

	protected :

		KDfloat					m_fAccum;
};

class SchedulerPauseResume : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					tick1 ( KDfloat fDelta );
		KDvoid					tick2 ( KDfloat fDelta );
		KDvoid					pause ( KDfloat fDelta );
};

class SchedulerPauseResumeAll : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual	KDvoid			onExit	 ( KDvoid );
		virtual KDvoid			update	 ( KDfloat fDelta );

	protected :

		KDvoid					tick1  ( KDfloat fDelta );
		KDvoid					tick2  ( KDfloat fDelta );
		KDvoid					pause  ( KDfloat fDelta );
		KDvoid					resume ( KDfloat fDelta );

	private :

		CCSet*					m_pPausedTargets;
};

class SchedulerPauseResumeAllUser : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual	KDvoid			onExit	 ( KDvoid );

	protected :

		KDvoid					tick1  ( KDfloat fDelta );
		KDvoid					tick2  ( KDfloat fDelta );
		KDvoid					pause  ( KDfloat fDelta );
		KDvoid					resume ( KDfloat fDelta );

	private :

		CCSet*					m_pPausedTargets;
};

class SchedulerUnscheduleAll : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					tick1 ( KDfloat fDelta );
		KDvoid					tick2 ( KDfloat fDelta );
		KDvoid					tick3 ( KDfloat fDelta );
		KDvoid					tick4 ( KDfloat fDelta );

		KDvoid					unscheduleAll ( KDfloat fDelta );
};

class SchedulerUnscheduleAllHard : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

	protected :

		KDvoid					tick1 ( KDfloat fDelta );
		KDvoid					tick2 ( KDfloat fDelta );
		KDvoid					tick3 ( KDfloat fDelta );
		KDvoid					tick4 ( KDfloat fDelta );

		KDvoid					unscheduleAll ( KDfloat fDelta );

	private :

		KDbool					m_bActionManagerActive;
};

class SchedulerUnscheduleAllUserLevel : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					tick1 ( KDfloat fDelta );
		KDvoid					tick2 ( KDfloat fDelta );
		KDvoid					tick3 ( KDfloat fDelta );
		KDvoid					tick4 ( KDfloat fDelta );

		KDvoid					unscheduleAll ( KDfloat fDelta );
};

class SchedulerSchedulesAndRemove : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					tick1 ( KDfloat fDelta );
		KDvoid					tick2 ( KDfloat fDelta );
		KDvoid					tick3 ( KDfloat fDelta );
		KDvoid					tick4 ( KDfloat fDelta );

		KDvoid					scheduleAndUnschedule ( KDfloat fDelta );
};

class SchedulerUpdate : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					removeUpdates ( KDfloat fDelta );
};

class SchedulerUpdateAndCustom : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					update		  ( KDfloat fDelta );
		KDvoid					tick		  ( KDfloat fDelta );
		KDvoid					stopSelectors ( KDfloat fDelta );
};

class SchedulerUpdateFromCustom : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					update		( KDfloat fDelta );
		KDvoid					schedUpdate	( KDfloat fDelta );
		KDvoid					stopUpdate	( KDfloat fDelta );
};

class TempNode : public CCNode
{
	public :

		virtual	 ~TempNode ( KDvoid );

	public :

		virtual	KDvoid			initWithString ( CCString* pStr, KDint nPriority );

	protected :

		CCString*				m_pString;
};

class RescheduleSelector : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					schedUpdate ( KDfloat fDelta );

	protected :

		KDfloat					m_fInterval;
		KDint					m_nTicks;
};

class SchedulerDelayAndRepeat : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	protected :

		KDvoid					update ( KDfloat fDelta );
};

class SchedulerTimeScale : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

	protected :

		CCControlSlider*		sliderCtl    ( KDvoid );
		KDvoid					sliderAction ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCControlSlider*		m_pSliderCtl;
};

class TwoSchedulers : public TestScheduler
{
	protected :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

	protected :

		CCControlSlider*		sliderCtl    ( KDvoid );
		KDvoid					sliderAction ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCScheduler*			m_pSched1;
		CCScheduler*			m_pSched2;
		CCActionManager*		m_pActionManager1;
		CCActionManager*		m_pActionManager2;
		CCControlSlider*		m_pSliderCtl1;
		CCControlSlider*		m_pSliderCtl2;
};

#endif	// __TestScheduler_h__
