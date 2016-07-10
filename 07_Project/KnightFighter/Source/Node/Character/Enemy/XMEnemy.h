/* --------------------------------------------------------------------------
 *
 *      File            XMEnemy.h
 *      Description     XMEnemy
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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

#ifndef __XMEnemy_h__
#define __XMEnemy_h__

#include "Node/Character/XMCharacter.h"

enum BULLET_TYPE
{
	BULLET_SMALL			=	0,
	BULLET_GOBLINRIDER			 ,
};

class XMPlayer;
class XMEnemyBullet;

class XMEnemy : public XMCharacter
{
	public :
				 XMEnemy ( KDvoid );
		virtual ~XMEnemy ( KDvoid );

		virtual KDvoid					playAI ( XMPlayer* pPlayer ) = 0;
		virtual KDvoid					homingPlayer ( KDfloat fDelta );

		KDint							getScore ( KDvoid );

	protected :

		virtual	KDbool					initWithContentsOfFile ( const std::string& filename );
		virtual KDvoid					initVariable ( KDvoid );

		virtual KDvoid					setFilterInfo ( KDvoid );

		virtual KDbool					slash ( KDvoid );

		virtual CallFunc*				makeLastAction ( KDvoid );
		
		virtual FiniteTimeAction*		makeMoveRandom	( KDfloat fTime = 1.0f );
		virtual FiniteTimeAction*		makeMoveLinear	( Point tPoint, KDfloat fTime = 1.0f );
		virtual FiniteTimeAction*		makeMoveEvade	( KDfloat fTime = 0.5f );
		virtual FiniteTimeAction*		makeMoveFlash	( Point tPoint, KDfloat fTime = 1.0f );

		virtual FiniteTimeAction*		makeMoveLeft	( Point tEnemyPoint, KDfloat fTime = 1.0f );
		virtual FiniteTimeAction*		makeMoveRight	( Point tEnemyPoint, KDfloat fTime = 1.0f );
		virtual FiniteTimeAction*		makeMoveUp		( Point tEnemyPoint, KDfloat fTime = 1.0f );
		virtual FiniteTimeAction*		makeMoveDown	( Point tEnemyPoint, KDfloat fTime = 1.0f );

		virtual Point					makeRandomPoint ( KDvoid );

		virtual KDfloat					getAngle ( Point tPoint1, Point tPoint2 );
		virtual KDfloat					getRadius ( Point tPoint1, Point tPoint2 );
		
		XMPlayer*						getPlayer ( );

		virtual KDvoid					runActionByArray ( Array* pArray );

		XMEnemyBullet*					makeBullet ( BULLET_TYPE eBulletType );

		KDvoid							shootLinear ( BULLET_TYPE eBulletType, KDfloat fSpeed = 480.0f );
		KDvoid							shootAiming ( BULLET_TYPE eBulletType, KDfloat fSpeed = 480.0f );

		KDvoid							shootMulti ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dDegree = 15.0f );
		KDvoid							shootMultiDelay ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dDegree = 15.0f, KDfloat fDelay = 0.5f );

		KDvoid							shootDivide ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dTotalDegree );
		KDvoid							shootDivideDelay ( BULLET_TYPE eBulletType, KDint nBulletCount, KDdouble dTotalDegree, KDfloat fDelay );

	protected :

		Sequence*						m_pSlashAction;

		KDint							m_nSlashState;
		KDint							m_nScore;

		XMPlayer*						m_pPlayer;

		KDbool							m_bIsPlayAI;

		KDchar*							m_sType;

	public :

		// m_nMaxHP Getter/Setter
		virtual KDfloat					getMaxHP ( KDvoid );
		virtual KDvoid					setMaxHP ( KDfloat nMaxHP );

		// m_nCurrentHP Getter/Setter
		virtual KDfloat					getCurrentHP ( KDvoid );
		virtual KDvoid					setCurrentHP ( KDfloat nCurrentHP );

		// m_fMoveSpeed Getter/Setter
		virtual KDfloat					getMoveSpeed ( KDvoid );
		virtual KDvoid					setMoveSpeed ( KDfloat fMoveSpeed );

		// m_nShootDamage Getter/Setter
		virtual KDfloat					getShootDamage ( KDvoid );
		virtual KDvoid					setShootDamage ( KDfloat nShootDamage );

		// m_nSlashDamage Getter/Setter
		virtual KDfloat					getSlashDamage ( KDvoid );
		virtual KDvoid					setSlashDamage ( KDfloat nSlashDamage );
};

#endif  // __XMEnemy_h__