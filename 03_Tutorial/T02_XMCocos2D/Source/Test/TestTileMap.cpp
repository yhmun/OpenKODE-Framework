/* --------------------------------------------------------------------------
 *
 *      File            TestTileMap.cpp
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
 *      version 2 of the License, or ( at your option) any later version.
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
#include "TestTileMap.h"

enum 
{
	kTagTileMap = 1,
};

TestTileMap* TestTileMap::create ( KDvoid )
{
	TestTileMap*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new TMXIsoZorder				( ); break;
		case  1 : pLayer = new TMXOrthoZorder			( ); break;
		case  2 : pLayer = new TMXIsoVertexZ			( ); break;
		case  3 : pLayer = new TMXOrthoVertexZ			( ); break;
		case  4 : pLayer = new TMXOrthoTest				( ); break;
		case  5 : pLayer = new TMXOrthoTest2			( ); break;
		case  6 : pLayer = new TMXOrthoTest3			( ); break;
		case  7 : pLayer = new TMXOrthoTest4			( ); break;
		case  8 : pLayer = new TMXIsoTest				( ); break;
		case  9 : pLayer = new TMXIsoTest1				( ); break;
		case 10 : pLayer = new TMXIsoTest2				( ); break;
		case 11 : pLayer = new TMXUncompressedTest		( ); break;
		case 12 : pLayer = new TMXHexTest				( ); break;
		case 13 : pLayer = new TMXReadWriteTest			( ); break;
		case 14 : pLayer = new TMXTilesetTest			( ); break;
		case 15 : pLayer = new TMXOrthoObjectsTest		( ); break;
		case 16 : pLayer = new TMXIsoObjectsTest		( ); break;
		case 17 : pLayer = new TMXResizeTest			( ); break;
		case 18 : pLayer = new TMXIsoMoveLayer			( ); break;
		case 19 : pLayer = new TMXOrthoMoveLayer		( ); break;
		case 20 : pLayer = new TMXTilePropertyTest		( ); break;
		case 21 : pLayer = new TMXOrthoFlipTest			( ); break;
		case 22 : pLayer = new TMXOrthoFlipRunTimeTest	( ); break;
		case 23 : pLayer = new TMXOrthoFromXMLTest		( ); break;
		case 24 : pLayer = new TileMapTest				( ); break;
		case 25 : pLayer = new TileMapEditTest			( ); break;
		case 26 : pLayer = new TMXGIDObjectsTest		( ); break;
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

KDuint TestTileMap::count ( KDvoid )
{
	return 27;
}

KDbool TestTileMap::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	//CCDirector::sharedDirector ( )->setDepthTest ( KD_TRUE );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* TestTileMap::subtitle ( KDvoid )
{
	return "drag the screen";
}

KDvoid TestTileMap::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint   tDiff  = pTouch->getDelta ( );
	
	CCNode*   pNode  = getChildByTag ( kTagTileMap );
	pNode->setPosition ( ccpAdd ( pNode->getPosition ( ), tDiff ) );
}

//------------------------------------------------------------------
//
// TileMapTest
//
//------------------------------------------------------------------
KDvoid TileMapTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTileMapAtlas*  map = CCTileMapAtlas::create ( "TileMaps/tiles.png",  "TileMaps/levelmap.tga", 16, 16 );

	this->addChild ( map, 0, kTagTileMap );

	// Convert it to "alias" ( GL_LINEAR filtering )
	map->getTexture ( )->setAliasTexParameters ( );

	// If you are not going to use the Map, you can free it now
	// NEW since v0.7
	map->releaseMap ( );

	map->setAnchorPoint ( ccp ( 0, 0.5f ) );

	CCScaleBy*   scale = CCScaleBy::create ( 4, 0.8f );
	CCSequence*  seq = CCSequence::create ( scale, scale->reverse ( ), KD_NULL );
	map->runAction ( CCRepeatForever::create ( seq ) );		 
}

const KDchar*  TileMapTest::title ( KDvoid )
{
	return "TileMapAtlas";
}

//------------------------------------------------------------------
//
// TileMapEditTest
//
//------------------------------------------------------------------
KDvoid TileMapEditTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTileMapAtlas*  map = CCTileMapAtlas::create ( "TileMaps/tiles.png", "TileMaps/levelmap.tga", 16, 16 );

	this->addChild ( map, 0, kTagTileMap );

	// Create an Aliased Atlas
	map->getTexture ( )->setAliasTexParameters ( );

	map->setAnchorPoint ( ccp ( 0, 0 ) );
	map->setPosition ( ccp ( -20, -200 ) );
	
	// If you are not going to use the Map, you can free it now
	// [tilemap releaseMap );
	// And if you are going to use, it you can access the data with:
	this->schedule ( schedule_selector ( TileMapEditTest::updateMap ), 0.2f );
}

KDvoid TileMapEditTest::updateMap ( KDfloat fDelta )
{
	// IMPORTANT
	//   The only limitation is that you cannot change an empty, or assign an empty tile to a tile
	//   The value 0 not rendered so don't assign or change a tile with value 0

	CCTileMapAtlas*  map = ( CCTileMapAtlas*) getChildByTag ( kTagTileMap );

	//
	// For example you can iterate over all the tiles
	// using this code, but try to avoid the iteration
	// over all your tiles in every frame. It's very expensive
	//	for ( int x=0; x < tilemap.tgaInfo->cx; x++) {
	//		for ( int y=0; y < tilemap.tgaInfo->height; y++) {
	//			ccColor3B c =[tilemap tileAt:ccs ( x,y ) );
	//			if ( c.r != 0 ) {
	//				////----UXLOG ( "%d,%d = %d", x,y,c.r );
	//			}
	//		}
	//	}
	
	// NEW since v0.7
	ccColor3B c = map->tileAt ( ccp ( 13, 21 ) );		
	c.r++;
	c.r %= 50;
	if ( c.r == 0 )
	{
		c.r=1;
	}
	
	// NEW since v0.7
	map->setTile ( c, ccp ( 13, 21 ) );		
}

const KDchar*  TileMapEditTest::title ( KDvoid )
{
	return "Editable TileMapAtlas";
}

//------------------------------------------------------------------
//
// TMXOrthoTest
//
//------------------------------------------------------------------
KDvoid TMXOrthoTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	//
	// Test orthogonal with 3d camera and anti-alias textures
	//
	// it should not flicker. No artifacts should appear
	//

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test2.tmx" );

	this->addChild ( map, 0, kTagTileMap );

	CCArray*   pChildrenArray = map->getChildren ( );
	CCSpriteBatchNode*  child = KD_NULL;
	CCObject*         pObject = KD_NULL;
	CCARRAY_FOREACH ( pChildrenArray, pObject )
	{
		child = (CCSpriteBatchNode*) pObject;

		CC_BREAK_IF ( !child );

		child->getTexture ( )->setAntiAliasTexParameters ( );
	}

	KDfloat x, y, z;
	map->getCamera ( )->getEyeXYZ ( &x, &y, &z );
	map->getCamera ( )->setEyeXYZ ( x - 200, y, z + 50 );	
}

const KDchar* TMXOrthoTest::title ( KDvoid )
{
	return "TMX Orthogonal test";
}

//------------------------------------------------------------------
//
// TMXOrthoTest2
//
//------------------------------------------------------------------
KDvoid TMXOrthoTest2::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test1.tmx" );

	this->addChild ( map, 0, kTagTileMap );

	CCArray*   pChildrenArray = map->getChildren ( );
	CCSpriteBatchNode*  child = KD_NULL;
	CCObject*         pObject = KD_NULL;
	CCARRAY_FOREACH ( pChildrenArray, pObject )
	{
		child = (CCSpriteBatchNode*) pObject;

		CC_BREAK_IF ( !child );

		child->getTexture ( )->setAntiAliasTexParameters ( );
	}

	map->runAction ( CCScaleBy::create ( 2, 0.5f ) );
}

const KDchar* TMXOrthoTest2::title ( KDvoid )
{
	return "TMX Ortho test2";
}

//------------------------------------------------------------------
//
// TMXOrthoTest3
//
//------------------------------------------------------------------
KDvoid TMXOrthoTest3::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test3.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	map->setScale ( 0.2f );
	map->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
}

const KDchar* TMXOrthoTest3::title ( KDvoid )
{
	return "TMX anchorPoint test";
}

//------------------------------------------------------------------
//
// TMXOrthoTest4
//
//------------------------------------------------------------------
KDvoid TMXOrthoTest4::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test4.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	map->setAnchorPoint ( ccp ( 0, 0 ) );

	CCTMXLayer*  layer = map->layerNamed ( "Layer 0" );
	CCSize  t = layer->getLayerSize ( );
	
	CCSprite*  sprite;

	sprite = layer->tileAt ( ccp ( 0, 0 ) );
	sprite->setScale ( 2 );

	sprite = layer->tileAt ( ccp ( t.cx - 1,0 ) );
	sprite->setScale ( 2 );

	sprite = layer->tileAt ( ccp ( 0, t.cy - 1 ) );
	sprite->setScale ( 2 );

	sprite = layer->tileAt ( ccp ( t.cx - 1, t.cy - 1 ) );
	sprite->setScale ( 2 );

	this->schedule ( schedule_selector ( TMXOrthoTest4::removeSprite ), 2 );
}

KDvoid TMXOrthoTest4::removeSprite ( KDfloat fDelta )
{
	this->unschedule ( schedule_selector ( TMXOrthoTest4::removeSprite ) );

	CCTMXTiledMap*  map = ( CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXLayer*  layer = map->layerNamed ( "Layer 0" );

	CCSize  t = layer->getLayerSize ( );
	CCSprite* sprite = layer->tileAt ( ccp ( t.cx - 1,0 ) );
	
	layer->removeChild ( sprite, KD_TRUE );
}

const KDchar* TMXOrthoTest4::title ( KDvoid )
{
	return "TMX cx / height test";
}

//------------------------------------------------------------------
//
// TMXReadWriteTest
//
//------------------------------------------------------------------
enum
{
	SID_UPDATECOL = 100,
	SID_REPAINTWITHGID,
	SID_REMOVETILES
};

KDvoid TMXReadWriteTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	m_uGID  = 0;
	m_uGID2 = 0;
	
	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test2.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	CCTMXLayer*  layer = map->layerNamed ( "Layer 0" );
	layer->getTexture ( )->setAntiAliasTexParameters ( );
	
	map->setScale (  1 );

	CCSprite*  tile0 = layer->tileAt ( ccp ( 1, 63 ) ); 
	CCSprite*  tile1 = layer->tileAt ( ccp ( 2, 63 ) ); 
	CCSprite*  tile2 = layer->tileAt ( ccp ( 3, 62 ) ); 
	CCSprite*  tile3 = layer->tileAt ( ccp ( 2, 62 ) ); 

	tile0->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	tile1->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	tile2->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	tile3->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

	CCActionInterval*       move = CCMoveBy::create ( 0.5f, ccp ( 0, 160 ) );
	CCActionInterval*     rotate = CCRotateBy::create ( 2, 360 );
	CCActionInterval*      scale = CCScaleBy::create ( 2, 5 );
	CCActionInterval*    opacity = CCFadeOut::create ( 2 );
	CCActionInterval*     fadein = CCFadeIn::create ( 2 );
	CCActionInterval*  scaleback = CCScaleTo::create ( 1, 1 );
	CCActionInstant*      finish = CCCallFuncN::create ( this, callfuncN_selector ( TMXReadWriteTest::removeSprite ) );

	CCSequence*				seq0 = CCSequence::create ( move, rotate, scale, opacity, fadein, scaleback, finish, KD_NULL );
	CCActionInterval*       seq1 = (CCActionInterval*) ( seq0->copy ( )->autorelease ( ) );
	CCActionInterval*       seq2 = (CCActionInterval*) ( seq0->copy ( )->autorelease ( ) );
	CCActionInterval*       seq3 = (CCActionInterval*) ( seq0->copy ( )->autorelease ( ) );
	
	tile0->runAction ( seq0 );
	tile1->runAction ( seq1 );
	tile2->runAction ( seq2 );
	tile3->runAction ( seq3 );

	m_uGID = layer->tileGIDAt ( ccp ( 0, 63 ) );

	this->schedule ( schedule_selector ( TMXReadWriteTest::updateCol ), 2.0f ); 
	this->schedule ( schedule_selector ( TMXReadWriteTest::repaintWithGID ), 2.0f ); 
	this->schedule ( schedule_selector ( TMXReadWriteTest::removeTiles ), 1.0f ); 
}

KDvoid TMXReadWriteTest::removeSprite ( CCNode* sender )
{
	CCNode*   node = ( (CCNode*) sender)->getParent ( );

	node->removeChild ( (CCNode*) sender, KD_TRUE );
}

KDvoid TMXReadWriteTest::updateCol ( KDfloat fDelta )
{	
	CCTMXTiledMap*  map = (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXLayer*   layer = (CCTMXLayer*) map->getChildByTag ( 0 );

	CCSize  t = layer->getLayerSize ( );
	for ( KDint y = 0; y < t.cy; y++ ) 
	{
		layer->setTileGID ( m_uGID2, ccp ( 3, y ) );
	}
	
	m_uGID2 = ( m_uGID2 + 1 ) % 80;
}

KDvoid TMXReadWriteTest::repaintWithGID ( KDfloat fDelta )
{
	CCTMXTiledMap*  map = (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXLayer*   layer = (CCTMXLayer*) map->getChildByTag ( 0 );

	CCSize  t = layer->getLayerSize ( );
	for ( KDint x = 0; x < t.cx; x++ ) 
	{
		KDint        y = (KDint) t.cy - 1;
		KDuint  tmpgid = layer->tileGIDAt ( ccp ( x, y ) );

		layer->setTileGID ( tmpgid + 1, ccp ( x, y ) );
	}
}

KDvoid TMXReadWriteTest::removeTiles ( KDfloat fDelta )
{
	this->unschedule ( schedule_selector ( TMXReadWriteTest::removeTiles ) );

	CCTMXTiledMap*  map = (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXLayer*   layer = (CCTMXLayer*) map->getChildByTag ( 0 );

	CCSize  t = layer->getLayerSize ( );
	for ( KDint y = 0; y < t.cy; y++ ) 
	{
		layer->removeTileAt ( ccp ( 5.0, y ) );
	}
}

const KDchar* TMXReadWriteTest::title ( KDvoid )
{
	return "TMX Read / Write test";
}

//------------------------------------------------------------------
//
// TMXHexTest
//
//------------------------------------------------------------------
KDvoid TMXHexTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );
	
	CCTMXTiledMap* map = CCTMXTiledMap::create ( "TileMaps/hexa-test.tmx" );
	addChild ( map, 0, kTagTileMap );
	
	CCSize s = map->getContentSize ( );
}

const KDchar* TMXHexTest::title ( KDvoid )
{
	return "TMX Hex tes";
}

//------------------------------------------------------------------
//
// TMXIsoTest
//
//------------------------------------------------------------------
KDvoid TMXIsoTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	// move map to the center of the screen
	CCSize  ms = map->getMapSize ( );
	CCSize  ts = map->getTileSize ( );
	
	map->runAction ( CCMoveTo::create ( 1.0f, ccp ( -ms.cx * ts.cx / 2, -ms.cy * ts.cy / 2 ) ) ); 
}

const KDchar* TMXIsoTest::title ( KDvoid )
{
	return "TMX Isometric test 0";
}

//------------------------------------------------------------------
//
// TMXIsoTest1
//
//------------------------------------------------------------------
KDvoid TMXIsoTest1::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test1.tmx" );

	this->addChild ( map, 0, kTagTileMap );
	map->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
}

const KDchar* TMXIsoTest1::title ( KDvoid )
{
	return "TMX Isometric test + anchorPoint";
}

//------------------------------------------------------------------
//
// TMXIsoTest2
//
//------------------------------------------------------------------
KDvoid TMXIsoTest2::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test2.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	// move map to the center of the screen
	CCSize  ms = map->getMapSize ( );
	CCSize  ts = map->getTileSize ( );
	map->runAction ( CCMoveTo::create ( 1.0f, ccp ( -ms.cx * ts.cx / 2, -ms.cy * ts.cy / 2 ) ) );
}

const KDchar* TMXIsoTest2::title ( KDvoid )
{
	return "TMX Isometric test 2";
}

//------------------------------------------------------------------
//
// TMXUncompressedTest
//
//------------------------------------------------------------------
KDvoid TMXUncompressedTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test2-uncompressed.tmx" );
	this->addChild ( map, 0, kTagTileMap );
	
	// move map to the center of the screen
	CCSize  ms = map->getMapSize ( );
	CCSize  ts = map->getTileSize ( );
	map->runAction ( CCMoveTo::create ( 1.0f, ccp ( -ms.cx * ts.cx / 2, -ms.cy * ts.cy / 2 ) ) );

	// testing release map
	CCArray*  pChildrenArray = map->getChildren ( );
	CCObject*        pObject = KD_NULL;
	CCARRAY_FOREACH ( pChildrenArray, pObject )
	{
		CCTMXLayer*  layer = (CCTMXLayer*) pObject;

		CC_BREAK_IF ( !layer );

		layer->releaseMap ( );
	}
}

const KDchar* TMXUncompressedTest::title ( KDvoid )
{
	return "TMX Uncompressed test";
}

//------------------------------------------------------------------
//
// TMXTilesetTest
//
//------------------------------------------------------------------
KDvoid TMXTilesetTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap* map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test5.tmx" );
	addChild ( map, 0, kTagTileMap );
	
	CCSize s = map->getContentSize ( );
	
	CCTMXLayer* layer;
	layer = map->layerNamed ( "Layer 0" );
	layer->getTexture ( )->setAntiAliasTexParameters ( );
	
	layer = map->layerNamed ( "Layer 1" );
	layer->getTexture ( )->setAntiAliasTexParameters ( );

	layer = map->layerNamed ( "Layer 2" );
	layer->getTexture ( )->setAntiAliasTexParameters ( );
}

const KDchar* TMXTilesetTest::title ( KDvoid )
{
	return "TMX Tileset test";
}

//------------------------------------------------------------------
//
// TMXOrthoObjectsTest
//
//------------------------------------------------------------------
KDvoid TMXOrthoObjectsTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap* map = CCTMXTiledMap::create ( "TileMaps/ortho-objects.tmx" );
	addChild ( map, -1, kTagTileMap );
	
	CCSize s = map->getContentSize ( );
	
	////----UXLOG ( "----> Iterating over all the group objets" );
	CCTMXObjectGroup* group = map->objectGroupNamed ( "Object Group 1" );
	CCArray*          objects = group->getObjects ( );

	CCDictionary*  pDict = KD_NULL;
    CCObject*      pObj  = KD_NULL;
    CCARRAY_FOREACH ( objects, pObj )
	{
        pDict = (CCDictionary*) pObj;

        if ( !pDict )
		{
            break;
		}
	}
}

KDvoid TMXOrthoObjectsTest::draw ( KDvoid )
{
	CCTMXTiledMap*      map		= (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXObjectGroup*	group	= map->objectGroupNamed ( "Object Group 1" );

	CCArray*			objects = group->getObjects ( );
    CCDictionary*		dict	= KD_NULL;
    CCObject*			pObj	= KD_NULL;
    CCARRAY_FOREACH ( objects, pObj )
    {
        dict = (CCDictionary*) pObj;
		
		if ( !dict )
		{
			break;
		}

		const KDchar*   key = "x";
		KDint  x = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "y";
		KDint  y = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "width";
		KDint cx = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "height";
		KDint cy = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		
		ccLineWidth ( 3 );
		
		ccDrawLine ( ccp ( x     , y      ), ccp ( x + cx, y      ) );
		ccDrawLine ( ccp ( x + cx, y      ), ccp ( x + cx, y + cy ) );
		ccDrawLine ( ccp ( x + cx, y + cy ), ccp ( x     , y + cy ) );
		ccDrawLine ( ccp ( x     , y + cy ), ccp ( x     , y      ) );
		
		ccLineWidth ( 1 );
	}
}

const KDchar* TMXOrthoObjectsTest::title ( KDvoid )
{
	return "TMX Ortho object test";
}

const KDchar* TMXOrthoObjectsTest::subtitle ( KDvoid )
{
	return "You should see a white box around the 3 platforms";
}

//------------------------------------------------------------------
//
// TMXIsoObjectsTest
//
//------------------------------------------------------------------
KDvoid TMXIsoObjectsTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test-objectgroup.tmx" );
	addChild ( map, -1, kTagTileMap );
	
	CCSize s = map->getContentSize ( );

	CCTMXObjectGroup* group = map->objectGroupNamed ( "Object Group 1" );

	CCArray*       objects = group->getObjects ( );
    CCDictionary*  dict;
    CCObject*      pObj = KD_NULL;
    CCARRAY_FOREACH ( objects, pObj )
    {
        dict = (CCDictionary*) pObj;

		if ( !dict )
		{
			break;
		}
	}		
}

KDvoid TMXIsoObjectsTest::draw ( KDvoid )
{
	CCTMXTiledMap*     map = (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXObjectGroup*  group = map->objectGroupNamed ( "Object Group 1" );

	CCArray*       objects = group->getObjects ( );
    CCDictionary*  dict;
    CCObject*      pObj = KD_NULL;
    CCARRAY_FOREACH ( objects, pObj )
    {
        dict = (CCDictionary*) pObj;
		if ( !dict )
		{
			break;
		}

		const KDchar*  key = "x";
		KDint  x = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "y";
		KDint  y = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "width";
		KDint cx = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		key = "height";
		KDint cy = ( (CCString*) dict->objectForKey ( key ) )->intValue ( );
		
		ccLineWidth ( 3 );
		
		ccDrawLine ( ccp ( x     , y      ), ccp ( x + cx, y      ) );
		ccDrawLine ( ccp ( x + cx, y      ), ccp ( x + cx, y + cy ) );
		ccDrawLine ( ccp ( x + cx, y + cy ), ccp ( x     , y + cy ) );
		ccDrawLine ( ccp ( x     , y + cy ), ccp ( x     , y      ) );
		
		ccLineWidth ( 1 );
	}
}

const KDchar* TMXIsoObjectsTest::title ( KDvoid )
{
	return "TMX Iso object test";
}

const KDchar* TMXIsoObjectsTest::subtitle ( KDvoid )
{
	return "You need to parse them manually. See bug #810";
}

//------------------------------------------------------------------
//
// TMXResizeTest
//
//------------------------------------------------------------------
KDvoid TMXResizeTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test5.tmx" );
	addChild ( map, 0, kTagTileMap );
	
	CCSize  s = map->getContentSize ( );
	////----UXLOG ( "ContentSize: %f, %f", s.cx,s.cy );

	CCTMXLayer*  layer;
	layer = map->layerNamed ( "Layer 0" );

	CCSize  ls = layer->getLayerSize ( );
	for ( KDuint y = 0; y < ls.cy; y++ ) 
	{
		for ( KDuint x = 0; x < ls.cx; x++ ) 
		{
			layer->setTileGID ( 1, ccp ( x, y ) );
		}
	}		
}

const KDchar* TMXResizeTest::title ( KDvoid )
{
	return "TMX resize test";
}

const KDchar* TMXResizeTest::subtitle ( KDvoid )
{
	return "Should not crash. Testing issue #740";
}

//------------------------------------------------------------------
//
// TMXIsoZorder
//
//------------------------------------------------------------------
KDvoid TMXIsoZorder::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  pMap = CCTMXTiledMap::create ( "TileMaps/iso-test-zorder.tmx" );
	this->addChild ( pMap, 0, kTagTileMap );

	CCSize  t = pMap->getContentSize ( );

	pMap->setPosition ( ccp ( -t.cx / 2, 0 ) );
		
	m_pTamara = CCSprite::create ( "Images/grossinis_sister1.png" );
	pMap->addChild ( m_pTamara, pMap->getChildren ( )->count ( ) );

	m_pTamara->setPosition ( ccp ( (KDint) ( pMap->getMapSize ( ).cx * pMap->getTileSize ( ).cx ) / 2.f, 0 ) );
	m_pTamara->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	
	CCActionInterval*   move = CCMoveBy::create ( 10, ccp ( 300, 250 ) );
	CCSequence*			 seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );
	
	m_pTamara->runAction ( CCRepeatForever::create ( seq ) );

	this->schedule ( schedule_selector ( TMXIsoZorder::repositionSprite ) );
}

KDvoid TMXIsoZorder::onExit ( KDvoid )
{
    this->unschedule ( schedule_selector ( TMXIsoZorder::repositionSprite ) );

	TestTileMap::onExit ( );
}

KDvoid TMXIsoZorder::repositionSprite ( KDfloat fDelta )
{	
	CCNode*  map = getChildByTag ( kTagTileMap );

	CCPoint  p = m_pTamara->getPosition ( );

	// there are only 4 layers. ( grass and 3 trees layers)
	// if tamara < 48, z=4
	// if tamara < 96, z=3
	// if tamara < 144,z=2
	
	KDint  newZ = (KDint) ( 4 - ( p.y / 48 ) );
	newZ = KD_MAX ( newZ, 0 );
	
	map->reorderChild ( m_pTamara, newZ );
}

const KDchar* TMXIsoZorder::title ( KDvoid )
{
	return "TMX Iso Zorder";
}

const KDchar* TMXIsoZorder::subtitle ( KDvoid )
{
	return "Sprite should hide behind the trees";
}

//------------------------------------------------------------------
//
// TMXOrthoZorder
//
//------------------------------------------------------------------
KDvoid TMXOrthoZorder::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test-zorder.tmx" );
	this->addChild ( map, 0, kTagTileMap );
				
	m_pTamara = CCSprite::create ( "Images/grossinis_sister1.png" );
	map->addChild ( m_pTamara, map->getChildren ( )->count ( ) );

	m_pTamara->setAnchorPoint ( ccp ( 0.5f, 0 ) );

	CCActionInterval*   move = CCMoveBy::create ( 10, ccp ( 400, 450 ) );
	CCSequence*			 seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );

	m_pTamara->runAction ( CCRepeatForever::create ( seq ) );
	
	this->schedule ( schedule_selector ( TMXOrthoZorder::repositionSprite ) );
}

KDvoid TMXOrthoZorder::repositionSprite ( KDfloat fDelta )
{
	CCNode*  map = getChildByTag ( kTagTileMap );
	CCPoint  p = m_pTamara->getPosition ( );

	// there are only 4 layers. ( grass and 3 trees layers)
	// if tamara <  81, z=4
	// if tamara < 162, z=3
	// if tamara < 243, z=2

	// -10: customization for this particular sample
	KDint  newZ = (KDint)  ( 4 - ( (  p.y - 10 ) / 81 ) );
	newZ = KD_MAX ( newZ,0 );

	map->reorderChild ( m_pTamara, newZ );
}

const KDchar* TMXOrthoZorder::title ( KDvoid )
{
	return "TMX Ortho Zorder";
}

const KDchar* TMXOrthoZorder::subtitle ( KDvoid )
{
	return "Sprite should hide behind the trees";
}

//------------------------------------------------------------------
//
// TMXIsoVertexZ
//
//------------------------------------------------------------------
KDvoid TMXIsoVertexZ::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test-vertexz.tmx" );

	this->addChild ( map, 0, kTagTileMap );
	
	CCSize  t = map->getContentSize ( );
	map->setPosition ( ccp ( -t.cx / 2, 0) );
	
	// because I'm lazy, I'm reusing a tile as an sprite, but since this method uses vertexZ, you
	// can use any CCSprite and it will work OK.
	CCTMXLayer*  layer = map->layerNamed ( "Trees" );

	m_pTamara = layer->tileAt ( ccp ( 29, 29 ) );
	
	CCActionInterval*  move = CCMoveBy::create ( 10, ccp ( 300, 250 ) );
	CCSequence*			seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );

	m_pTamara->runAction ( CCRepeatForever::create ( seq ) );
	
	this->schedule ( schedule_selector ( TMXIsoVertexZ::repositionSprite ) );

	// TIP: 2d projection should be used
	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection2D );
}

KDvoid TMXIsoVertexZ::repositionSprite ( KDfloat fDelta )
{
	// tile height is 64x32
	// map size: 30x30
	m_pTamara->setVertexZ ( - ( ( m_pTamara->getPosition ( ).y + 32 ) / 16 ) );
}

KDvoid TMXIsoVertexZ::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection3D );

	TestTileMap::onExit ( );
}

const KDchar* TMXIsoVertexZ::title ( KDvoid )
{
	return "TMX Iso VertexZ";
}

const KDchar* TMXIsoVertexZ::subtitle ( KDvoid )
{
	return "Sprite should hide behind the trees";
}

//------------------------------------------------------------------
//
// TMXOrthoVertexZ
//
//------------------------------------------------------------------
KDvoid TMXOrthoVertexZ::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test-vertexz.tmx" );
	this->addChild ( map, 0, kTagTileMap );

	// because I'm lazy, I'm reusing a tile as an sprite, but since this method uses vertexZ, you
	// can use any CCSprite and it will work OK.
	CCTMXLayer*  layer = map->layerNamed ( "trees" );
	m_pTamara = layer->tileAt ( ccp ( 0, 11 ) );

	CCActionInterval*   move = CCMoveBy::create ( 10, ccp ( 400, 450 ) );
	CCSequence*			 seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );

	m_pTamara->runAction ( CCRepeatForever::create ( seq ) );

	this->schedule ( schedule_selector ( TMXOrthoVertexZ::repositionSprite ) );	

	// TIP: 2d projection should be used
	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection2D );
}

KDvoid TMXOrthoVertexZ::repositionSprite ( KDfloat fDelta )
{
	// tile height is 101x81
	// map size: 12x12

	m_pTamara->setVertexZ ( - ( ( m_pTamara->getPosition ( ).y + 81 ) / 81 ) );
}

KDvoid TMXOrthoVertexZ::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection3D );

	TestTileMap::onExit ( );
}

const KDchar* TMXOrthoVertexZ::title ( KDvoid )
{
	return "TMX Ortho vertexZ";
}

const KDchar* TMXOrthoVertexZ::subtitle ( KDvoid )
{
	return "Sprite should hide behind the trees";
}

//------------------------------------------------------------------
//
// TMXIsoMoveLayer
//
//------------------------------------------------------------------
KDvoid TMXIsoMoveLayer::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/iso-test-movelayer.tmx" );
	this->addChild ( map, -1, kTagTileMap );

	map->setPosition ( ccp ( -700, -50 ) );
}

const KDchar* TMXIsoMoveLayer::title ( KDvoid )
{
	return "TMX Iso Move Layer";
}

const KDchar* TMXIsoMoveLayer::subtitle ( KDvoid )
{
	return "Trees should be horizontally aligned";
}

//------------------------------------------------------------------
//
// TMXOrthoMoveLayer
//
//------------------------------------------------------------------
KDvoid TMXOrthoMoveLayer::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/orthogonal-test-movelayer.tmx" );
	this->addChild ( map, -1, kTagTileMap );
}

const KDchar* TMXOrthoMoveLayer::title ( KDvoid )
{
	return "TMX Ortho Move Layer";
}

const KDchar* TMXOrthoMoveLayer::subtitle ( KDvoid )
{
	return "Trees should be horizontally aligned";
}

//------------------------------------------------------------------
//
// TMXTilePropertyTest
//
//------------------------------------------------------------------
KDvoid TMXTilePropertyTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

    CCTMXTiledMap*  map = CCTMXTiledMap::create ( "TileMaps/ortho-tile-property.tmx" );
    addChild ( map ,0, kTagTileMap );

    for ( KDint i = 1; i <= 20;i++ )
	{
        CCLOG ( "GID:%i, Properties:%p", i, map->propertiesForGID ( i ) );
    }
}

const KDchar* TMXTilePropertyTest::title ( KDvoid )
{
    return "TMX Tile Property Test";
}

const KDchar* TMXTilePropertyTest::subtitle ( KDvoid )
{
    return "In the console you should see tile properties";
}

//------------------------------------------------------------------
//
// TMXOrthoFlipTest
//
//------------------------------------------------------------------
KDvoid TMXOrthoFlipTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

    CCTMXTiledMap *map = CCTMXTiledMap::create ( "TileMaps/ortho-rotation-test.tmx" );
    addChild(map, 0, kTagTileMap);

    CCSize  s = map->getContentSize ( );
    CCLOG ( "ContentSize: %f, %f", s.cx, s.cy);

    CCObject*  pObj = KD_NULL;
    CCARRAY_FOREACH(map->getChildren(), pObj)
    {
        CCSpriteBatchNode* child = (CCSpriteBatchNode*)pObj;
        child->getTexture()->setAntiAliasTexParameters ( );
    }

    CCScaleBy* action = CCScaleBy::create(2, 0.5f);
    map->runAction(action);
}

const KDchar* TMXOrthoFlipTest::title ( KDvoid )
{
    return "TMX tile flip test";
}

//------------------------------------------------------------------
//
// TMXOrthoFlipRunTimeTest
//
//------------------------------------------------------------------
KDvoid TMXOrthoFlipRunTimeTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

    CCTMXTiledMap *map = CCTMXTiledMap::create("TileMaps/ortho-rotation-test.tmx");
    addChild(map, 0, kTagTileMap);

    CCSize s = map->getContentSize ( );
 
    CCObject* pObj = KD_NULL;
    CCARRAY_FOREACH(map->getChildren(), pObj)
    {
        CCSpriteBatchNode* child = (CCSpriteBatchNode*)pObj;
        child->getTexture()->setAntiAliasTexParameters ( );
    }

    CCScaleBy* action = CCScaleBy::create(2, 0.5f);
    map->runAction(action);

    schedule(schedule_selector(TMXOrthoFlipRunTimeTest::flipIt), 1.0f);
}

const KDchar* TMXOrthoFlipRunTimeTest::title ( KDvoid )
{
    return "TMX tile flip run time test";
}

const KDchar* TMXOrthoFlipRunTimeTest::subtitle ( KDvoid )
{
    return "in 2 sec bottom left tiles will flip";
}

KDvoid TMXOrthoFlipRunTimeTest::flipIt ( KDfloat fDelta )
{
    CCTMXTiledMap *map = (CCTMXTiledMap*) getChildByTag(kTagTileMap); 
    CCTMXLayer *layer = map->layerNamed("Layer 0"); 

    //blue diamond 
    CCPoint tileCoord = ccp(1,10);
    KDint flags;
    KDuint GID = layer->tileGIDAt(tileCoord, (ccTMXTileFlags*)&flags);
    // Vertical
    if ( flags & kCCTMXTileVerticalFlag )
	{
        flags &= ~kCCTMXTileVerticalFlag;
	}
    else
	{
        flags |= kCCTMXTileVerticalFlag;
	}
    layer->setTileGID(GID ,tileCoord, (ccTMXTileFlags)flags);


    tileCoord = ccp(1,8);    
    GID = layer->tileGIDAt(tileCoord, (ccTMXTileFlags*)&flags);
    // Vertical
    if ( flags & kCCTMXTileVerticalFlag )
	{
        flags &= ~kCCTMXTileVerticalFlag;
	}
    else
	{
        flags |= kCCTMXTileVerticalFlag;    
	}
    layer->setTileGID(GID ,tileCoord, (ccTMXTileFlags)flags);


    tileCoord = ccp(2,8);
    GID = layer->tileGIDAt(tileCoord, (ccTMXTileFlags*)&flags);
    // Horizontal
    if( flags & kCCTMXTileHorizontalFlag )
	{
        flags &= ~kCCTMXTileHorizontalFlag;
	}
    else
	{
        flags |= kCCTMXTileHorizontalFlag;    
	}
    layer->setTileGID(GID, tileCoord, (ccTMXTileFlags)flags);    
}

//------------------------------------------------------------------
//
// TMXOrthoFromXMLTest
//
//------------------------------------------------------------------
KDvoid TMXOrthoFromXMLTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	std::string resources = "TileMaps";        // partial paths are OK as resource paths.
	std::string file = resources + "/orthogonal-test1.tmx";

	CCString*  str = CCString::createWithContentsOfFile ( CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( file.c_str ( ) ).c_str ( ) );
    CCAssert ( str != KD_NULL, "Unable to open file");

    CCTMXTiledMap *map = CCTMXTiledMap::createWithXML(str->getCString() ,resources.c_str());
    addChild(map, 0, kTagTileMap);

    CCSize s = map->getContentSize ( );
    CCLOG("ContentSize: %f, %f", s.cx,s.cy);

    CCObject* pObj = KD_NULL;
    CCARRAY_FOREACH(map->getChildren(), pObj)
    {
        CCSpriteBatchNode* child = (CCSpriteBatchNode*)pObj;
        child->getTexture()->setAntiAliasTexParameters ( );
    }

    CCScaleBy* action = CCScaleBy::create(2, 0.5f);
    map->runAction(action);
}

const KDchar* TMXOrthoFromXMLTest::title ( KDvoid )
{
    return "TMX created from XML test";
}

//------------------------------------------------------------------
//
// TMXGIDObjectsTest
//
//------------------------------------------------------------------
KDvoid TMXGIDObjectsTest::onEnter ( KDvoid )
{
	TestTileMap::onEnter ( );

	CCTMXTiledMap*   pMap = CCTMXTiledMap::create ( "TileMaps/test-object-layer.tmx" );
	this->addChild ( pMap, -1, kTagTileMap );
}

KDvoid TMXGIDObjectsTest::draw ( KDvoid )
{
	CCTMXTiledMap*     pMap   = (CCTMXTiledMap*) getChildByTag ( kTagTileMap );
	CCTMXObjectGroup*  pGroup = pMap->objectGroupNamed ( "Object Layer 1" );

	CCArray*       pArray  = pGroup->getObjects ( );
	CCObject*      pObject = KD_NULL;
	CCARRAY_FOREACH ( pArray, pObject )
	{
		CCDictionary*  pDict = (CCDictionary*) pObject;

		KDint   x = ( (CCString*) pDict->objectForKey ( "x" ) )->intValue ( );
		KDint   y = ( (CCString*) pDict->objectForKey ( "y" ) )->intValue ( );
		KDint  cx = ( (CCString*) pDict->objectForKey ( "width"  ) )->intValue ( );
		KDint  cy = ( (CCString*) pDict->objectForKey ( "height" ) )->intValue ( );

		ccLineWidth ( 3 );

		ccDrawLine ( ccp ( x     , y      ), ccp ( x + cx, y      ) );
		ccDrawLine ( ccp ( x + cx, y      ), ccp ( x + cx, y + cy ) );
		ccDrawLine ( ccp ( x + cx, y + cy ), ccp ( x     , y + cy ) );
		ccDrawLine ( ccp ( x     , y + cy ), ccp ( x     , y      ) );

		ccLineWidth ( 1 );
	}
}

const KDchar* TMXGIDObjectsTest::title ( KDvoid )
{
	return "TMX GID objects";
}

const KDchar* TMXGIDObjectsTest::subtitle ( KDvoid )
{
	return "Tiles are created from an object group";
}