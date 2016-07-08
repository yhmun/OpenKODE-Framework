/* --------------------------------------------------------------------------
 *
 *      File            TestNode.h
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

#ifndef __TestNode_h__
#define __TestNode_h__

#include "../TestBasic.h"

class TestNode : public TestBasic
{
	public :

		static  TestNode*		create ( KDvoid );

	protected :		

		virtual KDuint			count ( KDvoid );
};

class Test2 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class Test4 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					delay2 ( KDfloat dt );
		KDvoid					delay4 ( KDfloat dt );
};

class Test5 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					addAndRemove ( KDfloat dt );
};

class Test6 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					addAndRemove ( KDfloat dt );
};

class StressTest1 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					shouldNotCrash ( KDfloat dt );
		KDvoid					removeMe ( CCNode* node );
};

class StressTest2 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					shouldNotLeak ( KDfloat dt );
};

class SchedulerTest1 : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					doSomething ( KDfloat dt );
};

class NodeToWorld : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class CameraOrbitTest : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class CameraZoomTest : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					updateZoom ( KDfloat dt );

	protected :

		KDfloat		m_z;
};

class CameraCenterTest : public TestNode
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};


class ConvertToNode : public TestNode
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
};

class NodeOpaqueTest : public TestNode
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class NodeNonOpaqueTest : public TestNode
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class ClippingNode : public TestNode
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

#endif // __TestNode_h__
