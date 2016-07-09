/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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

#ifndef __FlyingPlane__GameLayer__
#define __FlyingPlane__GameLayer__

class ShipSprite;

class GameLayer : public CCLayer
{
    public :

		GameLayer ( KDvoid );

		CREATE_FUNC ( GameLayer );

	public :
	    
		virtual KDbool          init                 ( KDvoid );

		ShipSprite*             getShip              ( KDvoid );
		CCSize                  getScreenSize        ( KDvoid );
	    
		KDvoid                  doGameOver           ( KDvoid );
		KDvoid                  showGameOver         ( KDvoid );
		KDvoid                  doResetGame          ( KDvoid );
	    
		KDvoid                  update               ( KDfloat fDelta );
	    
		virtual KDvoid          ccTouchesBegan       ( CCSet *pTouches, CCEvent *pEvent );
		virtual KDvoid          ccTouchesEnded       ( CCSet *pTouches, CCEvent *pEvent );
		virtual KDvoid          ccTouchesCancelled   ( CCSet *pTouches, CCEvent *pEvent );

	private :

		KDbool                  m_bGameOver;
		KDfloat                 m_fTotalTime;
		KDint                   m_nScore;
		CCLabelTTF*             m_pScoreBest;
		CCLabelTTF*             m_pScoreDistance;
		CCRect                  m_tScreenRect;
};

#endif /* defined(__FlyingPlane__GameLayer__) */
