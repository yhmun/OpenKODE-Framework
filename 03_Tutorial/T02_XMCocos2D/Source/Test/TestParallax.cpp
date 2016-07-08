/* --------------------------------------------------------------------------
 *
 *      File            TestParallax.cpp
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
#include "TestParallax.h"

enum 
{
	kTagNode,
};

TestParallax* TestParallax::create ( KDvoid )
{
	TestParallax*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0 : pLayer = new Parallax1 ( ); break;
		case 1 : pLayer = new Parallax2 ( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestParallax::count ( KDvoid )
{
	return 2;
}

KDbool TestParallax::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	CCParallaxNode*  node = CCParallaxNode::create ( );

	CCSprite*  bg = CCSprite::create ( "Images/background.png" );
	bg->setScale ( 1.5f );
	bg->setAnchorPoint ( ccp ( 0, 0 ) );
	node->addChild ( bg, -1, ccp ( 0.4f, 0.5f ), ccp ( 0, 0 ) );

	CCTileMapAtlas*  tile = CCTileMapAtlas::create ( "TileMaps/tiles.png", "TileMaps/levelmap.tga", 16, 16 );
	tile->releaseMap ( );
	tile->setAnchorPoint ( ccp ( 0, 0 ) );
	node->addChild ( tile, 1, ccp ( 2.2f, 1.0f ), ccp ( 0, -200 ) );

	CCSprite*  cocos = CCSprite::create ( "Images/powered.png" );
	cocos->setScale ( 2.5f );
	cocos->setAnchorPoint ( ccp ( 0, 0 ) );
	node->addChild ( cocos, 2, ccp ( 3.0f, 2.5f ), ccp ( 200, 800 ) );

	this->addChild ( node, 0, kTagNode );

	return KD_TRUE;
}

//------------------------------------------------------------------
//
// Parallax1
//
//------------------------------------------------------------------
KDvoid Parallax1::onEnter ( KDvoid )
{
	TestParallax::onEnter ( );

	CCNode*  node = getChildByTag ( kTagNode );

	CCActionInterval*   move1 = CCMoveBy::create ( 4, ccp ( 0, -500 ) );
	CCActionInterval*   move2 = CCMoveBy::create ( 8, ccp ( -1000, 0 ) );

	node->runAction ( ( CCRepeatForever::create 
	(
		CCSequence::create ( move1, move2, move1->reverse ( ), move2->reverse ( ), KD_NULL ) 
	) ) );
}

const KDchar* Parallax1::subtitle ( KDvoid )
{
	return "parent and 3 children";
}

//------------------------------------------------------------------
//
// Parallax2
//
//------------------------------------------------------------------
KDvoid Parallax2::onEnter ( KDvoid )
{
	TestParallax::onEnter ( );

	this->setTouchEnabled ( KD_TRUE );
}

const KDchar* Parallax2::subtitle ( KDvoid )
{
	return "drag screen";
}

KDvoid Parallax2::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint	  tDiff  = pTouch->getDelta ( );	

	CCNode*   pNode  = getChildByTag ( kTagNode );
	pNode->setPosition ( ccpAdd ( pNode->getPosition ( ), tDiff ) );
}
