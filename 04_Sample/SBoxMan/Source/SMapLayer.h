/* --------------------------------------------------------------------------
 *
 *      File            SMapLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
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

#ifndef __SMapLayer_h__
#define __SMapLayer_h__

#include "SBoxMan.h"

class SLevel;

enum SMapElementType 
{
    SMapElementRedHouse         = 10,    
    SMapElementYellowHouse      = 11,    
    SMapElementBlueHouse        = 12,    
    SMapElementBluePoolUp       = 13,    
    SMapElementBluePoolDown     = 14,    
    SMapElementBluePoolLeft     = 15,    
    SMapElementBluePoolRight    = 16,    
    
    SMapElementGreenTree        = 20,    
    SMapElementScenryTree       = 21,    
    SMapElementGreenShrub       = 22,    
    SMapElementYellowFlower     = 23,    
    SMapElementRedWall          = 30,    
    SMapElementRedBarricade     = 40,    
    SMapElementBox              = 60,    
    SMapElementGreenRoad        = 70,    
    SMapElementMan              = 80,    
    SMapElementDst              = 90,     
};

enum SMoveType 
{
    SMoveDisabled					,   
    SMoveEnabledWithMan				,    
    SMoveEnabledWithBoxMan			,  
};

typedef struct SIndexPath
{
    KDint		nRow;   
    KDint		nCol;		
} SIndexPath;

extern const SIndexPath			SIndexPathZero;

#define kMoveDuration			0.3f

class SGameDelegate
{
	public :

		virtual KDvoid		gameDidStart ( KDvoid ) = 0;

		virtual KDvoid		gameDidFinish ( KDvoid ) = 0;

		virtual KDvoid		boxManDidMovedWithBox ( KDbool bWithBox ) { };
};

class SMapLayer : public CCLayer
{
	public :	
				 SMapLayer ( KDvoid );
		virtual ~SMapLayer ( KDvoid );

		CREATE_FUNC ( SMapLayer );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				loadMapWithLevel ( SLevel* pLevel );

		KDvoid				reloadMap ( KDvoid );

		KDvoid				loadMap ( KDvoid );

		KDvoid				setDelegate ( SGameDelegate* pDelegate );

	private :

		SIndexPath			indexPathAtPoint ( const CCPoint& tPoint );

		CCPoint				pointAtIndexPath ( SIndexPath tIndexPath );

		SDirection			directionFromPoint ( const CCPoint& tStart, const CCPoint& tFinish ); 

		SMoveType			canMoveWithDirection ( SDirection eDirection );

		KDvoid				pushManWithDirection ( SDirection eDirection, SIndexPath tIndexPath );

		KDvoid				pushBoxWithDirection ( SDirection eDirection, SIndexPath tIndexPath );

		KDvoid				win ( KDfloat fDelta );

		KDvoid				enabledTouch ( KDfloat fDelta );

	private :

		SGameDelegate*		m_pDelegate;
		SLevel*				m_pLevel;
		CCArray*			m_pMapElements;
		KDint				m_nRowCount;
		KDint				m_nColumnCount;
		SBoxMan*			m_pBoxMan;
		CCArray*			m_pBoxes;
		CCArray*			m_pBoxArray;
		CCArray*			m_pDestArray;
		CCPoint				m_tStart;
		CCPoint				m_tFinish;
};

#endif	// __SMapLayer_h__