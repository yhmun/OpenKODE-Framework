/* --------------------------------------------------------------------------
 *
 *      File            StandardMoveComponent.h
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

#ifndef __StandardMoveComponent_h__
#define __StandardMoveComponent_h__

class GameLayer;

// Why is it derived from CCSprite? Because enemies use a SpriteBatch, and CCSpriteBatchNode requires that all
// child nodes added to it are CCSprite. Under other circumstances I would use a CCNode as parent class of course, since
// the components won't have a texture and everything else that a Sprite has.

class StandardMoveComponent : public CCSprite
{
	public :

		static StandardMoveComponent*  create ( GameLayer* pGameLayer );

	public :

		virtual KDbool    init   ( GameLayer* pGameLayer );
		KDvoid            update ( KDfloat fDelta );

	private :

		GameLayer*        m_pGameLayer;
		CCPoint           m_tVelocity;
};

#endif // __StandardMoveComponent_h__
