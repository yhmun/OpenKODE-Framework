/* --------------------------------------------------------------------------
 *
 *      File            GameObject.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __GameObject_h__
#define __GameObject_h__

enum 
{
	// Game Object Class Types (used for collision response handling)
	GO_TYPE_NONE		= 0,
	GO_TYPE_SENSOR		= 1,
	GO_TYPE_PROJECTILE	= 2,
	GO_TYPE_ACTOR		= 3,
	GO_TYPE_MISC		= 4,
	GO_TYPE_ISO			= 5,
};

class GameArea2D;

class GameObject : public CCNode
{
	public :
				 GameObject ( KDvoid );
		virtual ~GameObject ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				initBox2D ( KDvoid );

		virtual GameArea2D*			getGameArea ( KDvoid );
		virtual KDvoid				setGameArea ( GameArea2D* pGameArea );

		virtual b2Body*				getBody ( KDvoid );
		virtual KDvoid				setBody ( b2Body* pBody );

		virtual b2BodyDef*			getBodyDef ( KDvoid );
		virtual KDvoid				setBodyDef ( b2BodyDef* pBodyDef );

		virtual b2FixtureDef*		getFixtureDef ( KDvoid );
		virtual KDvoid				setFixtureDef ( b2FixtureDef* pFixtureDef );

		virtual b2PolygonShape*		getPolygonShape ( KDvoid );
		virtual KDvoid				setPolygonShape ( b2PolygonShape* pPolygonShape );

		virtual b2CircleShape*		getCircleShape ( KDvoid );
		virtual KDvoid				setCircleShape ( b2CircleShape* pCircleShape );

		virtual CCSprite*			getSprite ( KDvoid );
		virtual KDvoid				setSprite ( CCSprite* pSprite );

		virtual KDint				getTypeTag ( KDvoid );
		virtual KDvoid				setTypeTag ( KDint nTypeTag );

		virtual KDbool				 isMarkedForDestruction ( KDvoid );
		virtual KDvoid				setMarkedForDestruction ( KDbool bMarkedForDestruction );

		virtual KDint				getType ( KDvoid );

	protected :

		GameArea2D*					m_pGameArea;
		b2Body*						m_pBody;
		b2BodyDef*					m_pBodyDef;
		b2FixtureDef*				m_pFixtureDef;
		b2PolygonShape*				m_pPolygonShape;
		b2CircleShape*				m_pCircleShape;
		CCSprite*					m_pSprite;
		KDint						m_nTypeTag;
		KDbool						m_bMarkedForDestruction;
};

#endif	// __GameObject_h__
