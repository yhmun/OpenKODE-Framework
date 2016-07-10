/* --------------------------------------------------------------------------
 *
 *      File            XMCharacter.h
 *      Description     XMCharacter
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

#ifndef __XMCharacter_h__
#define __XMCharacter_h__



class XMCharacter : public Layer
{
	public :

				 XMCharacter ( KDvoid );
		virtual ~XMCharacter ( KDvoid );

		virtual KDvoid		move ( KDint nDirection );
		virtual KDbool		shoot ( KDvoid );
		virtual KDbool		slash ( KDvoid );
		virtual KDvoid		hitted ( KDvoid );

		Armature*			getArmature ( KDvoid );

		KDvoid				damaged ( KDint nDamage );

		b2World*			getb2World ( );

	protected :

		virtual	KDbool		initWithContentsOfFile ( const std::string& filename );
		
		virtual	KDvoid		initVariable ( KDvoid );		// 변수 초기화
		virtual KDvoid		initArmature ( KDvoid );
		
		virtual	KDvoid		setFilterInfo ( KDvoid ) = 0;

		virtual KDvoid		playIdleAnimation ( KDvoid );
		virtual KDvoid		playShootAnimation ( KDint nShootState = 1 );
		virtual KDvoid		playSlashAnimation ( KDint nSlashState = 1 );

		virtual KDvoid		onMovementEvent ( Armature* pArmature, MovementEventType eMovementType, const KDchar* pMovementID );
		virtual KDvoid		onFrameEvent ( Bone* pBone, const KDchar* szEvt, KDint nOriginFrameIndex, KDint nCurrentFrameIndex );
		
		virtual KDvoid		waitSlash ( KDvoid );

		

		String*				m_pCharacterName;				// 캐릭터이름
		KDbool				m_bIsActive;					// 활성화 상태
		KDfloat				m_fMaxHP;						// 최대 체력
		KDfloat				m_fCurrentHP;					// 현재 체력
		KDfloat				m_fMoveSpeed;					// 캐릭터 이동 속도
		KDfloat				m_fShootDamage;					// 원거리 공격 데미지 
		KDfloat				m_fSlashDamage;					// 근거리 공격 데미지
		KDint				m_nSlashVariety;				// 근거리 공격 종류

		KDbool				m_bIsSlashNow;					// 근거리 공격중인지 여부
		KDbool				m_bIsSlashActive;
		KDbool				m_bIsPossibleShoot;				// 원거리 공격 가능 여부
		KDbool				m_bIsPossibleSlash;				// 근거리 공격 가능 여부

		
		Armature*			m_pArmature;

		Dictionary*			m_pCharacterDict;


		XMCharacterState	m_eCharacterState;

		b2World*			m_pParentWorld;


	private :

		KDbool				loadContentsOfFile ( const std::string& filename );

	public :				// Getter/Setter

		// m_bActive Getter/Setter
		virtual KDbool		isActive ( KDvoid );
		virtual KDvoid		setActive ( KDbool bActive );

		// m_nMaxHP Getter/Setter
		virtual KDfloat		getMaxHP ( KDvoid );
		virtual KDvoid		setMaxHP ( KDfloat nMaxHP );

		// m_nCurrentHP Getter/Setter
		virtual KDfloat		getCurrentHP ( KDvoid );
		virtual KDvoid		setCurrentHP ( KDfloat nCurrentHP );

		// m_fMoveSpeed Getter/Setter
		virtual KDfloat		getMoveSpeed ( KDvoid );
		virtual KDvoid		setMoveSpeed ( KDfloat fMoveSpeed );

		// m_nShootDamage Getter/Setter
		virtual KDfloat		getShootDamage ( KDvoid );
		virtual KDvoid		setShootDamage ( KDfloat nShootDamage );

		// m_nSlashDamage Getter/Setter
		virtual KDfloat		getSlashDamage ( KDvoid );
		virtual KDvoid		setSlashDamage ( KDfloat nSlashDamage );

		// m_bIsSlash Getter/Setter
		virtual KDbool		isSlashNow ( KDvoid );
		virtual KDvoid		setSlashNow ( KDbool bIsSlashNow );

		// m_bIsSlashActive Getter
		virtual KDbool		isSlashActive ( KDvoid );

		// m_bIsPossibleShoot Getter/Setter
		virtual KDbool		isPossibleShoot ( KDvoid );
		virtual KDvoid		setPossibleShoot ( KDbool bPossibleShoot );

		// m_bIsPossibleSlash Getter/Setter
		virtual KDbool		isPossibleSlash ( KDvoid );
		virtual KDvoid		setPossibleSlash ( KDbool bPossibleSlash );

		virtual	String*		getPlayerName ( KDvoid );
};

#endif  // __XMCharacter_h__