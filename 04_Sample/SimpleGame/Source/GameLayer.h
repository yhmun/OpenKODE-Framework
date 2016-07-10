/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2010      Ray Wenderlich
 *
 *         http://www.cocos2d-x.org  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __GameLayer_h__
#define __GameLayer_h__

class GameLayer : public CCLayerColor
{
	public :

		         GameLayer ( KDvoid );
		virtual ~GameLayer ( KDvoid );

		CREATE_FUNC ( GameLayer );

	public :

		virtual KDbool  init ( KDvoid );  

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid  ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid  onClose ( CCObject* pSender );
		
		KDvoid  spriteMoveFinished ( CCNode* pSender );

		KDvoid  gameLogic ( KDfloat fDelta );

		KDvoid  updateGame ( KDfloat fDelta );		

	private :

		KDvoid  addTarget ( KDvoid );

	protected :

		CCArray*	m_pTargets;
		CCArray*	m_pProjectiles;
		KDint		m_nProjectilesDestroyed;
		KDuint		m_uProjectileEffect;

};

#endif  // __GameLayer_h__