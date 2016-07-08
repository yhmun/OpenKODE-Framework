/* --------------------------------------------------------------------------
 *
 *      File            TestBox2DBed.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2011 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __TestBox2DBed_h__
#define __TestBox2DBed_h__

#include "../TestBasic.h"

struct TestEntry;
class  Test;

class TestBox2DBed : public TestBasic
{
	public :

		CREATE_FUNC ( TestBox2DBed );

	protected :		

		virtual KDuint			count	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );		

		virtual KDvoid			onEnter	 ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid					tick ( KDfloat fDelta );

	private :

		TestEntry*				m_pEntry;
		Test*					m_pTest;
		KDint					m_nEntryID;
};


#endif
