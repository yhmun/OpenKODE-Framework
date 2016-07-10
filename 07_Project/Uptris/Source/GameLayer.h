/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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

class Board;
class Tetromino;

class GameLayer : public CCLayer
{
	public :

		GameLayer ( KDvoid );

		CREATE_FUNC ( GameLayer );

	public :

		virtual KDbool		init			( KDvoid );

		virtual KDvoid		onEnter			( KDvoid );								// 화면진입시

		virtual KDvoid		onEvent			( const KDEvent* pEvent );				// 이벤트수신 (포커스)

		virtual KDvoid		ccTouchesBegan	( CCSet* pTouches, CCEvent* pEvent );	// 터치 입력 수신
		virtual KDvoid		ccTouchesEnded	( CCSet* pTouches, CCEvent* pEvent );	// 터치 종료 수신

		virtual KDvoid		keyPressed		( KDint nId );							// 키보드 입력 수신
		virtual KDvoid		keyReleased		( KDint nId );							// 키보드 종료 수신
		
		KDvoid				onMenuCallback	( CCObject* pSender );	// 메뉴 터치시 콜백
		KDvoid				onNextTetromino ( KDint nType );		// 다음블록 노출
		KDvoid				onPressedButton	( KDfloat fDelta );		// 이동키가 눌렸을때
		KDvoid				onClearLine		( KDint nCount );		// 블럭라인 제거 콜백
		KDvoid				onGameOver		( KDvoid );				// 게임오버 콜백

		KDvoid				shakeScreen		( KDvoid );				// 화면 흔들기
		KDvoid				showLevelUpAni	( KDvoid );				// 레벨업 이미지 노출
		KDvoid				hideLevelUpAni	( KDfloat fDelta );		// 레벨업 이미지 숨김

		KDint				getLevel		( KDvoid );				// 게임레벨 반환

	protected:

		Board*				m_pBoard;			// 보드
		Tetromino*			m_pNextTetromino;	// 다음블럭
		KDint				m_nTouchedTarget;	// 터치중인대상

		CCLabelTTF*			m_pLevel;			// 레벨 레이블
		CCLabelTTF*			m_pScore;			// 점수 레이블
		CCLabelTTF*			m_pBlockLine;		// 제거라인 레이블
		
		CCSprite*			m_pVibrateCursor;	// 진동 토글 버튼
		CCSprite*			m_pSoundCursor;		// 소리 토글 버튼

		CCSprite*			m_pLevelupSprite;	// 레벨업 이미지
		CCAnimation*		m_pLevelupAnimation;// 레벨업 애니메이션

		KDint				m_nLevelValue;		// 레벨
		KDint				m_nScoreValue;		// 점수
		KDint				m_nBlockLineValue;	// 제거라인

		KDbool				m_bIsSound;			// 소리 상태
		KDbool				m_bIsVibrate;		// 진동 상태
};

#endif	//__GameLayer_h__