/* --------------------------------------------------------------------------
 *
 *      File            CoordinateFunctions.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 05/05/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "CoordinateFunctions.h"
#include "Bullet.h"

static KDfloat arctan2 ( KDfloat y, KDfloat x )
{
	const KDfloat	ONEQTR_PI = 0.78539816339f;
	const KDfloat	THRQTR_PI = 2.35619449019f;
	KDfloat			r, angle;
	KDfloat			abs_y = kdFabsf ( y ) + 1e-10f;
	if ( x < 0.0f )
	{
		r = ( x + abs_y ) / ( abs_y - x );
		angle = THRQTR_PI;
	}
	else 
	{
		r = ( x - abs_y ) / ( x + abs_y );
		angle = ONEQTR_PI;
	}
	angle += ( 0.1963f * r * r - 0.9817f ) * r;
	return ( y < 0.0f ) ? -angle : angle;
}

KDbool CoordinateFunctions::init ( KDvoid )
{
	return KD_TRUE;
}

CCPoint CoordinateFunctions::tilePosFromLocation ( const CCPoint& tLocation )
{
	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );

	// Tilemap position must be added as an offset, in case the tilemap position is not at 0,0 due to scrolling
	CCPoint			tPos = ccpSub ( tLocation, pTileMap->getPosition ( ) );
	
	KDfloat			fHalfMapWidth = pTileMap->getMapSize ( ).cx * 0.5f;
	KDfloat			fMapHeight	  = pTileMap->getMapSize ( ).cy;
	KDfloat			fTileWidth	  = pTileMap->getTileSize ( ).cx;
	KDfloat			fTileHeight	  = pTileMap->getTileSize ( ).cy;
	
	CCPoint			tTilePosDiv    = ccp ( tPos.x / fTileWidth, tPos.y / fTileHeight );
	KDfloat			fMapHeightDiff = fMapHeight - tTilePosDiv.y;
	
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	KDint			nPosX = (KDint) ( fMapHeightDiff + tTilePosDiv.x - fHalfMapWidth );
	KDint			nPosY = (KDint) ( fMapHeightDiff - tTilePosDiv.x + fHalfMapWidth );
	
	return ccp ( nPosX, nPosY );
}

CCPoint CoordinateFunctions::locationFromTilePos ( const CCPoint& tTilePos )
{
	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );
	CCTMXLayer*		pGrass = pTileMap->layerNamed ( "Grass" );
	CCSprite*		pTile = pGrass->tileAt ( tTilePos );
	KDfloat			x = -pTile->getPosition ( ).x - pTileMap->getTileSize ( ).cx + 32;
	KDfloat			y = -pTile->getPosition ( ).y - pTileMap->getTileSize ( ).cy;

	return ccp ( x, y );
}

CCPoint CoordinateFunctions::pointRelativeToCentreFromLocation ( const CCPoint& tLocation )
{
	return ccpSub ( GameMgr->getTileMap ( )->getPosition ( ), tLocation );
}

KDbool CoordinateFunctions::spritesCollided ( GameSprite* pSprite1, GameSprite* pSprite2 )
{
	CCRect		tSpriteRect1;
	CCRect		tSpriteRect2;
	
	// Do normal collision detection for bullet.
	// Make allowances for huge transparent border for everything else.
	if ( dynamic_cast <Bullet*> ( pSprite1 ) ) 
	{
		tSpriteRect1 = CCRect (	pSprite1->getPosition ( ).x - pSprite1->getContentSize ( ).cx / 2, 
								pSprite1->getPosition ( ).y - pSprite1->getContentSize ( ).cy / 2, 
								pSprite1->getContentSize ( ).cx, 
								pSprite1->getContentSize ( ).cy );
	} 
	else 
	{
		tSpriteRect1 = CCRect ( pSprite1->getPosition ( ).x - pSprite1->getContentSize ( ).cx / 4, 
								pSprite1->getPosition ( ).y - pSprite1->getContentSize ( ).cy / 4, 
								pSprite1->getContentSize ( ).cx / 2, 
								pSprite1->getContentSize ( ).cy / 2 );
	}

	if ( dynamic_cast <Bullet*> ( pSprite2 ) ) 
	{
		tSpriteRect2 = CCRect (	pSprite2->getPosition ( ).x - pSprite2->getContentSize ( ).cx / 2, 
								pSprite2->getPosition ( ).y - pSprite2->getContentSize ( ).cy / 2, 
								pSprite2->getContentSize ( ).cx, 
								pSprite2->getContentSize ( ).cy );
	}
	else 
	{
		tSpriteRect2 = CCRect ( pSprite2->getPosition ( ).x - pSprite2->getContentSize ( ).cx / 4, 
								pSprite2->getPosition ( ).y - pSprite2->getContentSize ( ).cy / 4, 
								pSprite2->getContentSize ( ).cx / 2, 
								pSprite2->getContentSize ( ).cy / 2 );
	}

	return tSpriteRect1.intersectsRect ( tSpriteRect2 );
}

KDbool CoordinateFunctions::atDoor ( const CCPoint& tTilePos )
{
	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );
	CCTMXLayer*		pMetaLayer = pTileMap->layerNamed ( "Meta" );
	KDuint			uMetaTileGID = pMetaLayer->tileGIDAt ( tTilePos );
	if ( uMetaTileGID > 0 )
	{
		CCDictionary*	pProperties = pTileMap->propertiesForGID ( uMetaTileGID );
		if ( pProperties ) 
		{
			const CCString*		pCollision = pProperties->valueForKey ( "Door" );
			if ( !pCollision->compare ( "True" ) )
			{
				return KD_TRUE;
			} 
		}
	}
	
	return KD_FALSE;
}

KDbool CoordinateFunctions::isTilePosBlocked ( const CCPoint& tTilePos )
{
	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );
	CCTMXLayer*		pMetaLayer = pTileMap->layerNamed ( "Meta" );

	if ( !this->isTilePosIsWithinBounds ( tTilePos ) )
	{
		CCLOG ( "Off the map! Tile pos %f %f is not within bounds", tTilePos.x, tTilePos.y );
		return KD_TRUE;
	}
	
	KDuint			uMetaTileGID = pMetaLayer->tileGIDAt ( tTilePos );
	if ( uMetaTileGID > 0 )
	{
		CCDictionary*	pProperties = pTileMap->propertiesForGID ( uMetaTileGID );
		if ( pProperties ) 
		{
			const CCString*		pCollision = pProperties->valueForKey ( "Collidable" );
			if ( !pCollision->compare ( "True" ) )
			{
				return KD_TRUE;
			} 
		}
	}
	
	return KD_FALSE;
}

CCPoint CoordinateFunctions::ensureTilePosIsWithinBounds ( const CCPoint& tTilePos )
{
	CCPoint		tTilePos2;

	// make sure coordinates are within bounds of the playable area, correcting them if not
	tTilePos2.x = KD_MAX ( m_tPlayableAreaMin.x, tTilePos.x );
	tTilePos2.x = KD_MIN ( m_tPlayableAreaMax.x, tTilePos.x );
	tTilePos2.y = KD_MAX ( m_tPlayableAreaMin.y, tTilePos.y );
	tTilePos2.y = KD_MIN ( m_tPlayableAreaMax.y, tTilePos.y );
	
	return tTilePos2;
}

KDbool CoordinateFunctions::isTilePosIsWithinBounds ( const CCPoint& tTilePos )
{
	if ( ( tTilePos.x < m_tPlayableAreaMin.x ) ||
		 ( tTilePos.x > m_tPlayableAreaMax.x ) ||
		 ( tTilePos.y < m_tPlayableAreaMin.y ) ||
		 ( tTilePos.y > m_tPlayableAreaMax.y ) ) 
	{
		return KD_FALSE;
	}

	return KD_TRUE;
}

KDfloat CoordinateFunctions::getAngleBetweenPoints ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	KDfloat		dx, dy;
	dx = tPoint1.x - tPoint2.x;
	dy = tPoint1.y - tPoint2.y;

	KDfloat		fRadians = arctan2 ( dy, dx );
	KDfloat		fAngle = CC_RADIANS_TO_DEGREES ( fRadians );
	if ( fAngle < 0 )
	{
		fAngle = ( 360.0f + fAngle );
	}
	return fAngle;
}

CCPoint CoordinateFunctions::pointFromAngleAndDistance ( KDfloat fAngle, const CCPoint& tStartPosition, KDfloat fDistance )
{
	KDfloat		fAngleInRadians = CC_DEGREES_TO_RADIANS ( fAngle );
	CCPoint		tNewPoint = ccp ( tStartPosition.x + fDistance * -kdCosf ( fAngleInRadians ),
								  tStartPosition.y + fDistance * -kdSinf ( fAngleInRadians ) );
	return tNewPoint;
}

KDvoid CoordinateFunctions::removeTile ( const CCPoint& tTilePos )
{
	
}

KDvoid CoordinateFunctions::debugTile ( const CCPoint& tTilePos )
{
	CCTMXLayer*		pGrassLayer = GameMgr->getTileMap ( )->layerNamed ( "Grass" );
	pGrassLayer->setTileGID ( 230, tTilePos );	
}

/*
static CoordinateFunctions *coordinateFunctions = nil;

@implementation CoordinateFunctions

-(void) debugTile:(CGPoint)tilePos {
	CCTMXLayer* grassLayer = [UIAppDelegate.tileMap layerNamed:@"Grass"];
	[grassLayer setTileGID:230 at:tilePos];
}

-(BOOL)spritesCollided:(GameSprite *)sprite1 sprite2:(GameSprite *)sprite2 {

}

#pragma mark Singleton Methods
+ (id)coordinateFunctions {
	@synchronized(self) {
		if(coordinateFunctions == nil)
			coordinateFunctions = [[super allocWithZone:NULL] init];
	}
	return coordinateFunctions;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self coordinateFunctions] retain];
}
- (id)copyWithZone:(NSZone *)zone {
	return self;
}
- (id)retain {
	return self;
}
- (unsigned)retainCount {
	return UINT_MAX; //denotes an object that cannot be released
}
- (void)release {
	// never release
}
- (id)autorelease {
	return self;
}
- (id)init {
	if( (self=[super init] )) {
	}
	return self;
}
- (void)dealloc {
	[super dealloc];
}

@end
*/