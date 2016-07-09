/* --------------------------------------------------------------------------
 *
 *      File            BodyNode.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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

#ifndef __BodyNode_h__
#define __BodyNode_h__

class BodyNode : public CCNode
{
	public :
				 BodyNode ( KDvoid );
		virtual ~BodyNode ( KDvoid );

	public :

		virtual KDvoid		createBodyInWorld ( b2World* pWorld, b2BodyDef* pBodyDef, b2FixtureDef* pFixtureDef, const KDchar* szSpriteName ); 
		
		virtual KDvoid		removeSprite ( KDvoid );

		virtual KDvoid		removeBody ( KDvoid );

	public :

		CC_SYNTHESIZE ( b2Body*, m_pBody, Body );
		CC_SYNTHESIZE ( CCSprite*, m_pSprite, Sprite );
};

#endif	// __BodyNode_h__
