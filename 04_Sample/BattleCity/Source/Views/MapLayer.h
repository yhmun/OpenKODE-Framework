/* --------------------------------------------------------------------------
 *
 *      File            MapLayer.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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

#ifndef __MapLayer_h__
#define __MapLayer_h__

#include "../Models/Tank90Sprite.h"

class MapLayerDelegate;

class MapLayer : public CCLayerColor, public Tank90SpriteDelegate
{
	public :

		static MapLayer*	create ( KDint nLevel, KDint nStatus, KDint nLife );

	protected :

				 MapLayer ( KDvoid );
		virtual ~MapLayer ( KDvoid );

		virtual KDbool			initWithMap ( KDint nLevel, KDint nKind, KDint nLife );

		KDvoid					initEnemys ( KDfloat fDelta );

	public :

		KDvoid					setDelegate ( MapLayerDelegate* pDelegate );

		Tank90Sprite*			getTank ( KDvoid );

	protected :

		virtual KDvoid			initButtlesDidFinish ( Tank90Sprite* pTank, CCSprite* pButtle1, CCSprite* pButtle2 );

		virtual KDvoid			initButtleDidFinish  ( Tank90Sprite* pTank, CCSprite* pButtle );

		virtual KDint			tileIDFromPosition ( const CCPoint& tPos, Tank90Sprite* pTank );

		virtual KDint			tileIDFromPositionLayer2 ( const CCPoint& tPos, Tank90Sprite* pTank );

		virtual CCArray*		enemyArray ( Tank90Sprite* pTank );

		virtual KDvoid			gameOver ( Tank90Sprite* pTank );

		virtual KDvoid			createTool ( KDvoid );

		virtual CCArray*		toolsArray ( Tank90Sprite* pTank );

		virtual KDvoid			removeTool ( ToolSprite* pTool );

		virtual KDvoid			destroyTile ( const CCPoint& tPos, Tank90Sprite* pTank );

		virtual KDvoid			removeSprite ( Tank90Sprite* pTank );

		virtual CCSprite*		home ( KDvoid );

		virtual KDvoid			homeProtect ( KDbool bIsProtect, Tank90Sprite* pTank );

		virtual KDvoid			plusBoon ( Tank90Sprite* pTank );

		virtual KDvoid			changeLife ( Tank90Sprite* pTank );

		virtual KDvoid			plusPass ( Tank90Sprite* pTank );

		virtual Tank90Sprite*	tankFromMap ( Tank90Sprite* pTank );

	private :

		KDvoid					initAIPlistFile ( KDvoid );

		KDvoid					upQutoRemoveTool ( KDvoid );

		KDvoid					autoRemoveTool ( KDfloat fDelta );

		CCPoint					objectPosition ( CCTMXObjectGroup* pGroup, const KDchar* szObject );
	
		KDvoid					gotoScoreScene ( KDfloat fDelta );

		KDvoid					returnMainScene ( KDfloat fDelta );

		CCPoint					tileCoordinateFromPosition ( const CCPoint& tPos );

		KDuint					tileIDFromPosition ( const CCPoint& tPos );

		KDuint					tileIDFromPositionLayer2 ( const CCPoint& tPos );		

		KDvoid					destroyTile ( const CCPoint& tPos );		

	private :

		MapLayerDelegate*		m_pDelegate;

		CCTMXTiledMap*			m_pMap;
		CCTMXLayer*				m_pBGLayer1;
		CCTMXLayer*				m_pBGLayer2;
		CCTMXObjectGroup*		m_pObjects;

		Tank90Sprite*			m_pTank;
		CCSprite*				m_pHome;

		CCDictionary*			m_pRemoteSpheres;

		KDint					m_nEnemyNum;
		CCArray*				m_pEnemyArray;

		CCArray*				m_pToolArray;

		CCDictionary*			m_pAiDict;

		KDint					m_nSlowTankCount;
		KDint					m_nQuikeTankCount;
		KDint					m_nStrongTankCount;
		
		KDint					m_nRodamPoint;
		CCArray*				m_pPointArray;
		CCPoint					m_tTmpPoint;
		CCArray*				m_pPropArray;

		KDint					m_nSlow;
		KDint					m_nQuike;
		KDint					m_nStrong;
		KDint					m_nStrongYe;
		KDint					m_nStrongG;
		KDint					m_nLevel;
		KDint					m_nBornNum;
		KDbool					m_bIsGameOver; 
		CCRect					m_tHomeRect;
};

class MapLayerDelegate
{
	public :

		virtual KDvoid			gameOver ( KDvoid )	= 0;

		virtual KDvoid			bornEnmey ( KDvoid ) = 0;

		virtual KDvoid			changeTankLife ( KDint nLife ) = 0;
};

#endif	// __MapLayer_h__