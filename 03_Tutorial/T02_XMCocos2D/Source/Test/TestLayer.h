/* --------------------------------------------------------------------------
 *
 *      File            TestLayer.h
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

#ifndef __TestLayer_h__
#define __TestLayer_h__

#include "../TestBasic.h"

class TestLayer : public TestBasic
{
	public :

		static  TestLayer*		create ( KDvoid );

	protected :		

		virtual KDuint			count ( KDvoid );
};

class LayerTestCascadingOpacityA : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTestCascadingOpacityB : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTestCascadingOpacityC : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTestCascadingColorA : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTestCascadingColorB : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTestCascadingColorC : public TestLayer
{
	public :

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerTest1 : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
		
		KDvoid					updateSize ( const CCPoint& tLocation );
};

class LayerTest2 : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
};

class LayerGradient : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );

	protected :

		KDvoid					toggleItem ( CCObject* sender );
};

class LayerTestBlend : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );

	protected :

		KDvoid					newBlend ( KDfloat dt );
};

class LayerIgnoreAnchorPointPos : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onToggle ( CCObject* sender );
};

class LayerIgnoreAnchorPointRot : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onToggle ( CCObject* sender );
};

class LayerIgnoreAnchorPointScale : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onToggle ( CCObject* sender );
};

class LayerExtendedBlendOpacityTest : public TestLayer
{
	public :

		virtual KDbool			init	 ( KDvoid );
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
};

#endif	// __TestLayer_h__
