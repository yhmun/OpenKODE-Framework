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

#ifndef __GameLayer_h__
#define __GameLayer_h__

// 적이 쓰러질 방향
typedef enum 
{
    kFallRight		,
    kFallLeft		,
} FallDirection;

// 현재 게임 진행 사항
typedef enum 
{
    kGamePlaying	,
    kGamePaused		,
} GameStatus;

class EnergyBar;

class GameLayer : public CCLayer 
{
	protected :
				 GameLayer ( KDvoid );
		virtual ~GameLayer ( KDvoid );

	public :

		CREATE_FUNC ( GameLayer );

	protected :

		virtual KDbool			init							( KDvoid );

		virtual KDvoid			onEnter							( KDvoid );
		virtual KDvoid			onExit							( KDvoid );

		virtual KDbool			ccTouchBegan					( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved					( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded					( CCTouch* pTouch, CCEvent* pEvent );

		KDbool					isTouchInside					( CCSprite* pSprite, CCTouch* pTouch );

		KDvoid					createBackgroundParallax		( KDvoid );
		KDvoid					createLables					( KDvoid );
		KDvoid					createArrowButtons				( KDvoid );
		KDvoid					createPrinceSpriteAndAnimation	( KDvoid );
		KDvoid					createEnemyAndAnimation			( KDvoid );
		KDvoid					createEnergyBar					( KDvoid );
		KDvoid					createGamePauseResumeMenu		( KDvoid );

		KDvoid					addNewEnemy						( KDfloat fDelta );

		KDvoid					displayScore					( KDvoid );
		KDvoid					displayLife						( KDvoid );

		KDvoid					playPauseMenuCallback			( CCObject* pSender );

		KDvoid					kickAnimateCompleteHandler		( KDvoid );

		KDvoid					moveBackground					( KDfloat fDelta );

		KDvoid					startMovingBackground			( KDvoid );
		KDvoid					stopMovingBackground			( KDvoid );

		KDvoid					startPrinceWalking				( KDvoid );
		KDvoid					stopPrinceWalking				( KDvoid );

		KDvoid					handleKickHit					( const CCPoint& tEffectPoint, CCSprite* pEnemy, FallDirection eFallDirection );

		KDvoid					hitEffectCompleteHander			( CCNode* pNode );
		KDvoid					enemyFallCompleteHander			( CCNode* pNode, KDvoid* pData );

		KDvoid					princeAttackedCompleteHandler	( KDvoid );

	public :

		KDvoid					princeAttacked					( FallDirection eDirection );

		virtual CCSprite*		getPrinceSprite					( KDvoid );

		virtual CCAnimate*		getPrinceWalkAnimate			( KDvoid );
		virtual KDvoid			setPrinceWalkAnimate			( CCAnimate* pAnimate );

		virtual CCAnimate*		getPrinceKickAnimate			( KDvoid );
		virtual KDvoid			setPrinceKickAnimate			( CCAnimate* pAnimate );

		virtual CCAnimation*	getEnemyWalkAnmation			( KDvoid );
		virtual KDvoid			setEnemyWalkAnmation			( CCAnimation* pAnimation );

		virtual CCAnimation*	getEnemyAttackAnimation			( KDvoid );
		virtual KDvoid			setEnemyAttackAnimation			( CCAnimation* pAnimation );
		
	protected :

		CCLabelTTF*				m_pScoreLabel;
		CCLabelTTF*				m_pLifeLabel;

		// 방향 전환에 쓰일 버튼
		// 눌리기 전과 눌렸을 때에 쓸 수 있도록 반향별로 두개씩 만듭니다.
		CCSprite*				m_pLeftSprite;
		CCSprite*				m_pLeftPressedSprite;
		CCSprite*				m_pRightSprite;
		CCSprite*				m_pRightPressedSprite;
	
		KDbool					m_bIsLeftPresssed;
		KDbool					m_bIsRightPresssed;

		// 발차기 버튼
		CCSprite*				m_pKickSprite;
		CCSprite*				m_pKickPressedSprite;

		// 주인공 케릭터 - 여자 이미지를 사용하지만 prince라고 부릅니다.
		CCSprite*				m_pPrinceSprite;

		// 주인공 케릭터의 걷기 애니메이션
		CCAnimate*				m_pPrinceWalkAnimate;

		// 주인공 캐릭터의 발차기 애니메이션
		CCAnimate*				m_pPrinceKickAnimate;

		// 적 케릭터
		CCSpriteBatchNode*		m_pEnemySpriteSheet;

		// 적 케릭터 애니메이션
		CCAnimation*			m_pEnemyWalkAnmation;
		CCAnimation*			m_pEnemyAttackAnimation;		

		// 주인공 에너지 변수와 바
		KDint					m_nEnergyValue;
		EnergyBar*				m_pPrinceEnergyBar;

		// 게임을 일시정지/플레이하기 위해 사용하는 메뉴 아이템(버튼)
		CCMenuItemImage*		m_pPauseMenuItem;
		CCMenuItemImage*		m_pPlayMenuItem;

		// 점수와 라이프 값을 담을 변수와 화면에 표시할 레이블
		KDint					m_nScoreValue;
		KDint					m_nLifeValue;

		// 게임이 일시정지 또는 플레이 중인지 나타냅니다.
		GameStatus				m_eGameStatus;

		// 발차기 애니메이션이 진행 중인지 검사하는 데 사용합니다.
		KDbool					m_bIsAnimating;
};

#endif	// __GameLayer_h__