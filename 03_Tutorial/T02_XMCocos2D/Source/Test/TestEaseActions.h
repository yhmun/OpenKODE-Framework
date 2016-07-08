/* --------------------------------------------------------------------------
 *
 *      File            TestEaseActions.h
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

#ifndef __TestEaseActions_h__
#define __TestEaseActions_h__

#include "../TestBasic.h"

class TestEaseActions : public TestBasic
{
	public :

		static  TestEaseActions*	create ( KDvoid );

	protected :		

		virtual KDbool			init ( KDvoid );	

		virtual KDuint			count ( KDvoid );

		KDvoid					positionForTwo ( KDvoid );

	protected :

		CCSprite*				m_grossini;
		CCSprite*				m_tamara;
		CCSprite*				m_kathia;
};

class SpriteEase : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );

		KDvoid					testStopAction ( KDfloat dt );
};

class SpriteEaseInOut : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseExponential : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseExponentialInOut : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseSine : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseSineInOut : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseElastic : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseElasticInOut : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseBounce : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseBounceInOut : public TestEaseActions
{
	public :

		virtual KDvoid  onEnter ( KDvoid );

		virtual const KDchar*  title ( KDvoid );
};

class SpriteEaseBack : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpriteEaseBackInOut : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );
};

class SpeedTest : public TestEaseActions
{
	public :

		virtual KDvoid			onEnter ( KDvoid );

		virtual const KDchar*	title ( KDvoid );

	    KDvoid					altertime ( KDfloat dt );
};

#endif	// __TestEaseActions_h__
