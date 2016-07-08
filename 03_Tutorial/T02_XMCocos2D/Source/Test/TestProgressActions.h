/* --------------------------------------------------------------------------
 *
 *      File            TestProgressActions.h
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

#ifndef __TestProgressActions_h__
#define __TestProgressActions_h__

#include "../TestBasic.h"

class TestProgressActions : public TestBasic
{
	public :

		static  TestProgressActions*	create ( KDvoid );

	protected :		

		virtual KDuint					count ( KDvoid );
};

class SpriteProgressToRadial : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressToHorizontal : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressToVertical : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressToRadialMidpointChanged : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressBarVarious : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressBarTintAndFade : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class SpriteProgressWithSpriteFrame : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class ProgressHorizontally : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class ProgressVertically : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class ProgressBoth : public TestProgressActions
{
	public :

		virtual KDvoid					onEnter  ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );
};

class ProgressSize : public TestProgressActions
{
	public :

		ProgressSize ( KDvoid );

	public :

		virtual KDvoid					onEnter  ( KDvoid );
		virtual KDvoid					onExit   ( KDvoid );

		virtual const KDchar*			subtitle ( KDvoid );

		KDvoid							onUpdate ( KDfloat dt );

	protected :

		CCProgress*						m_left;
		CCProgress*						m_right;
		CCSize							m_size;
		KDbool							m_dir_x;
		KDbool							m_dir_y;
};

#endif	// __TestProgressActions_h__
