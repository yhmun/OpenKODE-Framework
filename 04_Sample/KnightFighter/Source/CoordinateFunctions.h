/* --------------------------------------------------------------------------
 *
 *      File            CoordinateFunctions.h
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

#ifndef __CoordinateFunctions_h__
#define __CoordinateFunctions_h__

class GameSprite;

class CoordinateFunctions : public CCObject 
{
	public :

		CREATE_FUNC ( CoordinateFunctions );

	public :

		virtual KDbool		init ( KDvoid );

		CCPoint				tilePosFromLocation ( const CCPoint& tLocation );

		CCPoint				locationFromTilePos ( const CCPoint& tTilePos );

		CCPoint				pointRelativeToCentreFromLocation ( const CCPoint& tLocation );

		KDbool				spritesCollided ( GameSprite* pSprite1, GameSprite* pSprite2 );

		KDbool				atDoor ( const CCPoint& tTilePos );

		KDbool				isTilePosBlocked ( const CCPoint& tTilePos );

		CCPoint				ensureTilePosIsWithinBounds ( const CCPoint& tTilePos );

		KDbool				isTilePosIsWithinBounds ( const CCPoint& tTilePos );

		KDfloat				getAngleBetweenPoints ( const CCPoint& tPoint1, const CCPoint& tPoint2 );

		CCPoint				pointFromAngleAndDistance ( KDfloat fAngle, const CCPoint& tStartPosition, KDfloat fDistance );

		KDvoid				removeTile ( const CCPoint& tTilePos );

		KDvoid				debugTile ( const CCPoint& tTilePos );

	public :

		CC_SYNTHESIZE ( CCPoint, m_tPlayableAreaMin, PlayableAreaMin );
		CC_SYNTHESIZE ( CCPoint, m_tPlayableAreaMax, PlayableAreaMax );
};

#endif	// __CoordinateFunctions_h__
