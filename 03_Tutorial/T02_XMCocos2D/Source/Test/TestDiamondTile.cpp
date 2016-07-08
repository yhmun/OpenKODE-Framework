/* --------------------------------------------------------------------------
 *
 *      File            TestDiamondTile.cpp
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

#include "Precompiled.h"
#include "TestDiamondTile.h"

KDbool TestDiamondTile::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
		  
	m_map = CCDiamondTiledLayer::create ( ccg ( 16, 16 ), ccg ( 48, 24 ), 4 );
	m_map->loadFrame ( 0, "xm_supports/default_tile.png" );
	m_map->loadFrame ( 1, "xm_supports/default_tile2.png" );
	m_map->loadFrame ( 2, "xm_supports/default_tile3.png" );
	m_map->loadFrame ( 3, "xm_supports/default_tile4.png" );	

	m_map->ignoreAnchorPointForPosition ( KD_FALSE );
	m_map->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );		
	m_map->setPosition ( ccp ( tLyrSize.cx * 0.5f, tLyrSize.cy * 0.5f - 25 ) );
	
	this->addChild ( m_map );

	this->setTouchEnabled ( KD_TRUE );
	this->schedule ( schedule_selector ( TestDiamondTile::updateTile ), 0.5f ); 

	return KD_TRUE;
}

const KDchar* TestDiamondTile::subtitle ( KDvoid )
{
	return "Tap map tile or Drag background";
}

KDvoid TestDiamondTile::updateTile ( KDfloat dt )
{
	ccGridSize    gridSize = m_map->getGridSize ( );
	KDuint      frameCount = m_map->getFrameCount ( );

	for ( KDint y = 0; y < gridSize.y; y++ )
	{
		for ( KDint x = 0; x < gridSize.x; x++ )
		{
			m_map->setFrame ( ccg ( x, y ), kdRand ( ) % frameCount );
			m_map->setOpacity ( ccg ( x, y ), kdRand ( ) % 128 + 127 );
		}
	}
}

KDvoid TestDiamondTile::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, -1, KD_TRUE );
}

KDbool TestDiamondTile::ccTouchBegan ( CCTouch* touch, CCEvent* event )
{
	CCPoint  location = touch->getLocation ( );

	ccGridSize  grid = m_map->getIndex ( m_map->convertToNodeSpace ( location ) );
	if ( grid.x != -1 && grid.y != -1 )
	{
		this->setSelect ( grid, KD_TRUE );

		m_grid = grid;

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid TestDiamondTile::ccTouchMoved ( CCTouch* touch, CCEvent* event )
{
	CCPoint  location = touch->getLocation ( );

	ccGridSize  grid = m_map->getIndex ( m_map->convertToNodeSpace ( location ) );

	this->setSelect ( m_grid, KD_FALSE );

	if ( grid.x != -1 && grid.y != -1 )
	{			
		this->setSelect ( grid, KD_TRUE );
		m_grid = grid;
	}
}

KDvoid TestDiamondTile::ccTouchEnded ( CCTouch* touch, CCEvent* event )
{
	this->setSelect ( m_grid, KD_FALSE );
}

KDvoid TestDiamondTile::setSelect ( ccGridSize grid, KDbool on )
{
	for ( KDint y = grid.y - 1; y < ( grid.y + 2 ); y++ )
	{
		for ( KDint x = grid.x - 1; x < ( grid.x + 2 ); x++ )
		{
			if ( on )
			{
				m_map->setColor ( ccg ( x, y ), ccRED );
			}
			else
			{
				m_map->setColor ( ccg ( x, y ), ccWHITE );
			}
		}
	}
}