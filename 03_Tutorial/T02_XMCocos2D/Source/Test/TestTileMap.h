/* --------------------------------------------------------------------------
 *
 *      File            TestTileMap.h
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

#ifndef __TestTileMap_h__
#define __TestTileMap_h__

#include "../TestBasic.h"

class TestTileMap : public TestBasic
{
	public :

		static  TestTileMap*	create ( KDvoid );

	protected :		

		virtual KDbool			init	 ( KDvoid );
		virtual KDuint			count	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );	
};

class TileMapTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TileMapEditTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid		updateMap ( KDfloat fDelta );
};

class TMXOrthoTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXOrthoTest2 : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXOrthoTest3 : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXOrthoTest4 : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid		removeSprite ( KDfloat fDelta );
};

class TMXReadWriteTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );

		KDvoid					removeSprite   ( CCNode* pSender );

		KDvoid					updateCol	   ( KDfloat fDelta );
		KDvoid					repaintWithGID ( KDfloat fDelta );
		KDvoid					removeTiles    ( KDfloat fDelta );

	protected :

		KDuint					m_uGID;
		KDuint					m_uGID2;
};

class TMXHexTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXIsoTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXIsoTest1 : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXIsoTest2 : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXUncompressedTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXTilesetTest : public TestTileMap
{
	public :

		virtual const KDchar*	title   ( KDvoid );
		virtual KDvoid			onEnter ( KDvoid );
};

class TMXOrthoObjectsTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );
};

class TMXIsoObjectsTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );
};

class TMXResizeTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXIsoZorder : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual	KDvoid			onExit	 ( KDvoid );

		KDvoid					repositionSprite ( KDfloat fDelta );

	protected :

		CCSprite*				m_pTamara;
};

class TMXOrthoZorder : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					repositionSprite ( KDfloat fDelta );

	protected :

		CCSprite*				m_pTamara;
};

class TMXIsoVertexZ : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		KDvoid					repositionSprite ( KDfloat fDelta );

	protected :

		CCSprite*				m_pTamara;
};

class TMXOrthoVertexZ : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		KDvoid					repositionSprite ( KDfloat dt );

	protected :

		CCSprite*				m_pTamara;
};

class TMXIsoMoveLayer : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXOrthoMoveLayer : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXTilePropertyTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXOrthoFlipTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXOrthoFlipRunTimeTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					flipIt ( KDfloat fDelta );
};

class TMXOrthoFromXMLTest : public TestTileMap 
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TMXGIDObjectsTest : public TestTileMap
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );
};

#endif	// __TestTileMap_h__
