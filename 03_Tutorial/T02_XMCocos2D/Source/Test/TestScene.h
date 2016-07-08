/* --------------------------------------------------------------------------
 *
 *      File            TestScene.h
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

#ifndef __TestScene_h__
#define __TestScene_h__

#include "../TestBasic.h"

class TestScene : public TestBasic
{
	public :

		CREATE_FUNC ( TestScene );

	public :

		virtual KDbool			init     ( KDvoid );
		virtual	const KDchar*	subtitle ( KDvoid );

		KDvoid					onPushScene			( CCObject* pSender );
		KDvoid					onPushSceneTran		( CCObject* pSender );
		KDvoid					onQuit				( CCObject* pSender );
};

class TestScene2 : public TestBasic
{
	public :

		CREATE_FUNC ( TestScene2 );

	public :

		virtual KDbool			init     ( KDvoid );
		virtual	const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onGoBack			( CCObject* pSender );
		KDvoid					onReplaceScene		( CCObject* pSender );
		KDvoid					onReplaceSceneTran	( CCObject* pSender );
};

class TestScene3 : public TestBasic
{
	public :

		CREATE_FUNC ( TestScene3 );

	public :

		virtual KDbool			init     ( KDvoid );
		virtual	const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onPushScene      ( CCObject* pSender );
		KDvoid					onPopScene       ( CCObject* pSender );
		KDvoid					onPopToRootScene ( CCObject* pSender );
};

#endif	// __TestScene_h__
