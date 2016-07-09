/* --------------------------------------------------------------------------
 *
 *      File            EnemySprite.h
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

#ifndef __EnemySprite_h__
#define __EnemySprite_h__

class GameLayer;

class EnemySprite : public CCSprite 
{
	public :
				 EnemySprite ( KDvoid );
		virtual ~EnemySprite ( KDvoid );

		static   EnemySprite*	create ( const KDchar* szFrameName, const CCPoint& tPos, KDfloat fSpeed, GameLayer* pLayer );

	public :

		virtual KDbool			initWithSpriteFrameName ( const KDchar* szFrameName, const CCPoint& tPos, KDfloat fSpeed, GameLayer* pLayer );

		virtual KDvoid			onEnter					( KDvoid );

		virtual CCAnimate*		getWalkAnimate			( KDvoid );
		virtual KDvoid			setWalkAnimate			( CCAnimate* pAnimate );

		virtual CCAnimate*		getAttackAnimate		( KDvoid );
		virtual KDvoid			setAttackAnimate		( CCAnimate* pAnimate );

		KDvoid					adjustPosition			( KDvoid );

	protected :

		KDvoid					walking					( KDfloat fDelta );

		KDvoid					startWalking			( KDvoid );
		KDvoid					stopWalking				( KDvoid );

		KDvoid					attackPrince			( KDvoid );
		KDvoid					attackCompleteHandler	( KDvoid );

	protected :

		GameLayer*				m_pGameLayer;
		CCAnimate*				m_pWalkAnimate;
		CCAnimate*				m_pAttackAnimate;
		KDfloat					m_fWalkingSpeed;
};

#endif	// __EnemySprite_h__