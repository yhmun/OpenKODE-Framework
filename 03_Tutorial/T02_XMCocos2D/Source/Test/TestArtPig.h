/* --------------------------------------------------------------------------
 *
 *      File            TestArtPig.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com
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

#ifndef __TestArtPig_h__
#define __TestArtPig_h__

#include "../TestBasic.h"

class TestArtPig : public TestBasic, APSActionObserver
{
	public :

		virtual ~TestArtPig ( KDvoid );

		static   TestArtPig*	create ( KDvoid );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual KDuint			count ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );
		virtual const KDchar*	getProjectLocation ( KDvoid );

		virtual KDvoid			willStartAction ( APSAction* pAction );
		virtual KDvoid			didFinishAction ( APSAction* pAction );

	protected :	

		APSSymbol*				m_pSymbol;
};

class ArtPigSample : public TestArtPig
{
	public :

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );
};

class HappyPrince : public TestArtPig
{
	public :

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );
};

class HappyPrincePacked : public TestArtPig
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );
		virtual const KDchar*	getProjectLocation ( KDvoid );
};

class PigSpriteSheet : public TestArtPig
{
	public :

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );
};

class TouchJewel : public TestArtPig
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );

		virtual KDvoid			registerWithTouchDispatcher ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );
};

class MyActionObserver : public APSActionObserver
{
	public :

		virtual KDvoid			willStartAction ( APSAction* pAction );
		virtual KDvoid			didFinishAction ( APSAction* pAction );
};

class ObserveActions : public TestArtPig
{
	public :

		virtual ~ObserveActions ( KDvoid );

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );

	protected :

		MyActionObserver*		m_pObserver;
};

class JewelController : public APSActionObserver, APSTouchEventObserver 
{
	public :
				 JewelController ( APSSymbol* pSymbol );
		virtual ~JewelController ( KDvoid );
	    
	public :

		//
		// APSActionObserver
		//
		virtual KDvoid			willStartAction ( APSAction* pAction );
		virtual KDvoid			didFinishAction ( APSAction* pAction );
	    
		//
		// APSTouchEventObserver
		//
		virtual KDvoid			touchBegan ( APSGraphic* pGraphic, const APSTouch& tTouch );
		virtual KDvoid			touchMoved ( APSGraphic* pGraphic, const APSTouch& tTouch );
		virtual KDvoid			touchEnded ( APSGraphic* pGraphic, const APSTouch& tTouch );
	    
	protected :

		APSSymbol*				m_pSymbol;
		APSGraphic*				m_pBoyGraphic;
};

class JewelControl : public TestArtPig
{
	public :

		virtual ~JewelControl ( KDvoid );

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

		virtual const KDchar*	getProjectID ( KDvoid );

		virtual KDvoid			registerWithTouchDispatcher ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

	protected :

		JewelController*		m_pJewelController;
};

#endif	// __TestArtPig_h__
