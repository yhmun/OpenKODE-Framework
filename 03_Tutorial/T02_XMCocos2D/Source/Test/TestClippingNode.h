/* --------------------------------------------------------------------------
 *
 *      File            TestClippingNode.h
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

#ifndef __TestClippingNode_h__
#define __TestClippingNode_h__

#include "../TestBasic.h"

class TestClippingNode : public TestBasic
{
	public :

		static TestClippingNode*	create ( KDvoid );

	protected :		

		virtual KDbool				init  ( KDvoid );
		virtual	KDuint				count ( KDvoid );
		virtual KDvoid				setup ( KDvoid );
};

class BasicTest : public TestClippingNode
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );

		virtual CCAction*			actionRotate ( KDvoid );
		virtual CCAction*			actionScale  ( KDvoid );

		virtual CCDrawNode*			shape    ( KDvoid );
		virtual CCSprite*			grossini ( KDvoid );

		virtual CCNode*				stencil  ( KDvoid );
		virtual CCClippingNode*		clipper  ( KDvoid );
		virtual CCNode*				content  ( KDvoid );
};

class ShapeTest : public BasicTest
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual CCNode*				stencil  ( KDvoid );
		virtual CCNode*				content  ( KDvoid );
};

class ShapeInvertedTest : public ShapeTest
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual CCClippingNode*		clipper  ( KDvoid );
};

class SpriteTest : public BasicTest
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual CCNode*				stencil  ( KDvoid );
		virtual CCClippingNode*		clipper  ( KDvoid );
		virtual CCNode*				content  ( KDvoid );
};

class SpriteNoAlphaTest : public SpriteTest
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual CCClippingNode*		clipper  ( KDvoid );
};

class SpriteInvertedTest : public SpriteTest
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual CCClippingNode*		clipper  ( KDvoid );
};

class NestedTest : public TestClippingNode
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );
};

class HoleDemo : public TestClippingNode
{
	public :

		virtual KDvoid				onExit	 ( KDvoid );

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );

		virtual KDvoid				ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid						pokeHoleAtPoint ( CCPoint tPoint );

	private :

		CCClippingNode*				m_pOuterClipper;
		CCNode*						m_pHoles;
		CCNode*						m_pHolesStencil;
};

class ScrollViewDemo : public TestClippingNode
{
	public :

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );

		virtual KDvoid				ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid				ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid				ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

	private :

		KDbool						m_bScrolling;
		CCPoint						m_tLastPoint;
};

class RawStencilBufferTest : public TestClippingNode
{
	public :

		virtual KDvoid				onExit   ( KDvoid );

		virtual const KDchar*		title    ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );
		virtual KDvoid				draw	 ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );

	protected :

		CCSprite*					m_pSprite;
};

class RawStencilBufferTest2 : public RawStencilBufferTest
{
	public :

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );
};

class RawStencilBufferTest3 : public RawStencilBufferTest
{
	public :

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );
};

class RawStencilBufferTest4 : public RawStencilBufferTest
{
	public :

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );
};

class RawStencilBufferTest5 : public RawStencilBufferTest
{
	public :

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );
};

class RawStencilBufferTest6 : public RawStencilBufferTest
{
	public :

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDvoid				setup	 ( KDvoid );

		virtual KDvoid				setupStencilForClippingOnPlane ( GLint nPlane );
		virtual KDvoid				setupStencilForDrawingOnPlane  ( GLint nPlane );
};

#endif	// __TestClippingNode_h__
