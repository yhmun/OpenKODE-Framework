/* --------------------------------------------------------------------------
 *
 *      File            TestActions.h
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

#ifndef __TestActions_h__
#define __TestActions_h__

#include "../TestBasic.h"

class TestActions : public TestBasic
{
	public :

		static  TestActions*	create ( KDvoid );

	protected :		

		virtual KDbool			init  ( KDvoid );	
		virtual KDuint			count ( KDvoid );		
	
		KDvoid					centerSprites    ( KDuint uNumberOfSprites );
		KDvoid					alignSpritesLeft ( KDuint uNumberOfSprites );

	protected :


		CCSprite*				m_pGrossini;
		CCSprite*				m_pTamara;
		CCSprite*				m_pKathia;		
};

class ActionManual : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionMove : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionScale : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionSkew : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRotationalSkew : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRotationalSkewVSStandardSkew : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionSkewRotateScale : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRotate : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionJump : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionBezier : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionBlink : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionFade : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionTint : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionAnimate : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionSequence : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionSequence2 : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onCallback1 ( KDvoid );
		KDvoid					onCallback2 ( CCNode* sender );
		KDvoid					onCallback3 ( CCNode* sender, KDvoid* data );
};

class ActionSpawn : public TestActions
{
	public :

		virtual KDbool			init ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionReverse : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRepeat : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionDelayTime : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionReverseSequence : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionReverseSequence2 : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionOrbit : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRemoveSelf : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRepeatForever : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onRepeatForever ( CCNode* pTarget );
};

class ActionRotateToRepeat : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionRotateJerk : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionCallFunc : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onCallback1 ( KDvoid );
		KDvoid					onCallback2 ( CCNode* pTarget );
		KDvoid					onCallback3 ( CCNode* pTarget, KDvoid* pData );
};

class ActionCallFuncND : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onCallback ( CCNode* pTarget, KDvoid* pData );		
};

class ActionFollow : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionStacked : public TestActions
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			addNewSpriteWithCoords ( CCPoint tPos );
		virtual KDvoid			runActionsInSprite ( CCSprite* pSprite );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
};

class ActionMoveStacked : public ActionStacked
{
	public :

		virtual const KDchar*	title	 ( KDvoid );

		virtual KDvoid			runActionsInSprite ( CCSprite* pSprite );
};

class ActionMoveJumpStacked : public ActionStacked
{
	public :

		virtual const KDchar*	title	 ( KDvoid );

		virtual KDvoid			runActionsInSprite ( CCSprite* pSprite );
};

class ActionMoveBezierStacked : public ActionStacked
{
	public :

		virtual const KDchar*	title	 ( KDvoid );

		virtual KDvoid			runActionsInSprite ( CCSprite* pSprite );
};

class ActionCatmullRomStacked : public TestActions
{
	public :

		virtual ~ActionCatmullRomStacked ( KDvoid );

		virtual KDvoid			draw	 ( KDvoid );
		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	private :

		CCPointArray*			m_pArray1;
		CCPointArray*			m_pArray2;
};

class ActionCardinalSplineStacked : public TestActions
{
	public:
		virtual ~ActionCardinalSplineStacked ( KDvoid );

		virtual KDvoid			draw	 ( KDvoid );
		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	private :

		CCPointArray*			m_pArray;
};

class ActionTargeted : public TestActions
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

class ActionCatmullRom : public TestActions
{
	public :

		virtual ~ActionCatmullRom ( KDvoid );

	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );

	private :

		CCPointArray*			m_pArray1;
		CCPointArray*			m_pArray2;
};

class ActionCardinalSpline : public TestActions
{
	public :

		virtual ~ActionCardinalSpline ( KDvoid );

	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );

	private :

		CCPointArray*		m_pArray;
};

class PauseResumeActions : public TestActions
{
	public :
				 PauseResumeActions ( KDvoid );
		virtual ~PauseResumeActions ( KDvoid );

	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	public :

		KDvoid					pause  ( KDfloat fDelta );
		KDvoid					resume ( KDfloat fDelta );

	private :

		CCSet*					m_pPausedTargets;
};

#endif	// __TestActions_h__
