/* --------------------------------------------------------------------------
 *
 *      File            Global.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors   
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

#ifndef __Global_h__
#define __Global_h__

#include "Resource.h"

enum EKeyID
{
	eKeyUp				= 0	,
	eKeyLeft				,
	eKeyRight				,
	eKeyDown				,
};

enum EBulletType
{
	eBulletShip			= 1 ,
	eBulletEnemy			,
};

enum EWeaponType
{
	eWeaponOne			= 1	,
};

enum ETag
{
	eTagEnemyBullet		= 900,
	eTagEnemy				 ,
	eTagShip				 ,
	eTagShipBullet		     ,
};

enum EMoveType
{
	eMoveAttack			= 0 ,
	eMoveVertical			,
	eMoveHorizontal			,
	eMoveOverlap			,
};

enum EAttackMode
{
	eAttackNormal		= 1	,
	eAttackTsuihikidan		,
};

#define DELTA_X			-100
#define OFFSET_X		-24
#define ROT				-5.625

class Bullet;

class Sprite : public CCSprite
{
	public :

		virtual  KDvoid  destroy ( KDvoid ) = 0;

		virtual  KDvoid  update ( KDfloat fDelta ) = 0;

		virtual	 KDvoid  hurt ( KDvoid ) = 0;

		virtual	 CCRect  collideRect ( KDvoid ) = 0;

		virtual  KDbool  active ( KDvoid ) = 0;
};

class Global
{
	public :

		static KDvoid  playBGM ( const KDchar* szPath );
		static KDvoid  playEffect ( const KDchar* szPath );

		static KDvoid  setSound ( KDbool bOn );

	public :

		static KDbool		bSound;
		static KDuint		uBGM;

		static KDbool		bKeys[4];

		static KDint		nScore;
		static KDint		nLife;
		static KDint		nLevel;
		
		static KDint		nBulletNum;
		static KDint		nEnemyNum;

		static KDint		nLifeUpScores[6];

		static CCArray*		pEnemyContainer;
		static CCArray*		pEBulletContainer;
		static CCArray*		pSBulletContainer;
};

#endif // __Global_h__