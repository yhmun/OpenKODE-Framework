/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
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

#include "Precompiled.h"
#include "GameLayer.h"
#include "EnemySprite.h"
#include "EnergyBar.h"

enum 
{
    kTag_Parallax					,
    kTag_Enemy						,
    kTag_Prince						,
    kTag_Effect						,
    kTag_Label						,
    kTag_PrinceSpriteSheet			,
    kTag_ArrowButtonPressed			,
    kTag_ArrowButton				,
    kTag_ControlButtonPressed		,
    kTag_ControlButton				,
    kTag_PauseResumeMenu			,
};

#define IMG_WIDTH					1600
#define INIT_NUM_LIFE				3
#define INIT_NUM_ENERGY				10

GameLayer::GameLayer ( KDvoid )
{
	m_pPrinceWalkAnimate	= KD_NULL;
	m_pPrinceKickAnimate	= KD_NULL;
	m_pEnemyWalkAnmation	= KD_NULL;
	m_pEnemyAttackAnimation = KD_NULL;
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPrinceWalkAnimate		);
	CC_SAFE_RELEASE ( m_pPrinceKickAnimate		);
	CC_SAFE_RELEASE ( m_pEnemyWalkAnmation		);
	CC_SAFE_RELEASE ( m_pEnemyAttackAnimation	);
}

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

    // 초기값 설정
	m_eGameStatus		= kGamePlaying;
	m_nScoreValue		= 0;
	m_nLifeValue		= INIT_NUM_LIFE;
	m_nEnergyValue		= INIT_NUM_ENERGY;
	m_bIsAnimating		= KD_FALSE;
	m_bIsLeftPresssed	= KD_FALSE;
	m_bIsRightPresssed	= KD_FALSE;

    this->createBackgroundParallax		 ( );
    this->createLables					 ( );
    this->createArrowButtons			 ( );
    this->createPrinceSpriteAndAnimation ( );
    this->createEnemyAndAnimation		 ( );
	this->createEnergyBar				 ( );
    this->createGamePauseResumeMenu		 ( );

    // CCLayer가 터치 이벤트를 처리할 수 있게 활성화합니다.
	this->setTouchMode ( kCCTouchesOneByOne );
    this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}		

KDvoid GameLayer::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );
    
    // 배경음악 연주를 시작합니다.
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "Sounds/audioBG.wav" );
    
    // 0.5초 간격으로 적 케릭터를 만들어냅니다.
	this->schedule ( schedule_selector ( GameLayer::addNewEnemy ), 0.5f );
}

KDvoid GameLayer::onExit ( KDvoid )
{
    this->unschedule ( schedule_selector ( GameLayer::addNewEnemy ) );
    
    // sprite sheet texture 캐시를 모두 지웁니다.
    // 더 이상 사용하지 않는 캐시를 반드시 지워주세요.
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->removeSpriteFrames ( );
    
    // 배경음악 연주를 중지합니다.
	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( KD_TRUE );

	CCLayer::onExit ( );
}

KDvoid GameLayer::createBackgroundParallax ( KDvoid )
{    
	const CCSize	tLyrSize = this->getContentSize ( );

    // 이미지로 백그라운드에 쓰일 CCSprite를 만듭니다.
	CCSprite*		pBGSprite1   = CCSprite::create ( "Images/background1.png" );
	CCSprite*		pBGSprite2_1 = CCSprite::create ( "Images/background2_1.png" );
	CCSprite*		pBGSprite2_2 = CCSprite::create ( "Images/background2_2.png" );
    
    // Transform할 때 사용하는 anchorPoint를 왼쪽 아래 귀퉁이 (0, 0)으로 잡습니다.
    pBGSprite1	->setAnchorPoint ( ccpz );
    pBGSprite2_1->setAnchorPoint ( ccpz );
    pBGSprite2_2->setAnchorPoint ( ccpz );
    
    // 이미지가 만나는 가장자리(edge)에 검은선이 생기는 현상을 방지하기 위하여
    // Anti-Aliasing을 끕니다.
    pBGSprite2_1->getTexture ( )->setAliasTexParameters ( );
    pBGSprite2_2->getTexture ( )->setAliasTexParameters ( );
    
    // 위에서 만든 sprite를 담을 parent로 CCParallaxNode를 만듭니다.
	CCParallaxNode*		pVoidNode = CCParallaxNode::create ( );
    
    // 배경 sprite를 Parallox에 넣습니다.
    // parallaxRatio는 가로/세로로 움직이는 속도로 보면 됩니다.
    // 우리는 가로로만 움직이므로 y값을 0으로 줍니다.
    // backgroud1.png 파일의 세로 크기가 160픽셀이기 때문에 positionOffset을 이용하여
    // 화면 위에 위치하도록 좌표를 조정합니다.
    // 뒤쪽에 놓인 배경 background1.png는 좀 더 천천히 움직이도록 parallaxRatio의 x값을 1보다 작은 0.4로 설정합니다.
    pVoidNode->addChild ( pBGSprite1  , 0, ccp ( 0.4f, 0 ), ccp ( 0, tLyrSize.cy / 2 ) );
    pVoidNode->addChild ( pBGSprite2_1, 1, ccp ( 1.0f, 0 ), ccp ( 0, 0 ) );
    pVoidNode->addChild ( pBGSprite2_2, 1, ccp ( 1.0f, 0 ), ccp ( IMG_WIDTH / 2, 0 ) );
     
	this->addChild ( pVoidNode, kTag_Parallax, kTag_Parallax );
}

KDvoid GameLayer::createLables ( KDvoid )
{    
	const CCSize	tLyrSize = this->getContentSize ( );

	CCLabelTTF*		pScore = m_pScoreLabel = CCLabelTTF::create ( "Score: 0", "Font/arial.ttf", 18, ccs ( 120, 20 ), kCCTextAlignmentLeft );
	pScore->setAnchorPoint ( ccp ( 0, 1 ) );
	pScore->setPosition ( ccp ( 25, tLyrSize.cy - 10 ) );
	this->addChild ( pScore, kTag_Label );
	this->displayScore ( );


	CCLabelTTF*		pLife = m_pLifeLabel = CCLabelTTF::create ( "Life: 0", "Font/arial.ttf", 18, ccs ( 120, 20 ), kCCTextAlignmentLeft );
	pLife->setAnchorPoint ( ccp ( 0, 1 ) );
	pLife->setPosition ( ccp ( tLyrSize.cx / 2 + 15, tLyrSize.cy - 10 ) );
	this->addChild ( pLife, kTag_Label );
	this->displayLife ( );	
}

KDvoid GameLayer::createArrowButtons ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	CCSprite*	pSprite	= KD_NULL;
	CCSize		tSize   = ccsz;

    // 왼쪽 화살표 -------------------------------------------------------------
	pSprite = m_pLeftSprite = CCSprite::create ( "Images/arrow_left.png" );
	tSize   = pSprite->getContentSize ( );
    
    // 기본 anchorPoint가 가운데, 즉 (0.5, 0.5)이므로 이미지의 가로 세로 크기의 반에
    // 5필셀의 여유를 두고 화면 아래에 표시합니다.
    pSprite->setPosition ( ccp ( 10 + tSize.cx / 2, tSize.cy / 2 + 5 ) );
    this->addChild ( pSprite, kTag_ArrowButton );
    
    // 눌렸을 때 쓰일 왼쪽 화살표 ------------------------------------------------
    pSprite = m_pLeftPressedSprite = CCSprite::create ( "Images/arrow_left_s.png" );    
    
    // m_pLeftSprite와 같은 위치에 표시합니다.
    pSprite->setPosition ( m_pLeftSprite->getPosition ( ) );
    
    // 눌렸을 때의 화살표를 하위 z-order로 넣습니다. 그럼, 위에 같은 크기의 화살표가
    // 같은 위치에 있으니까 가려서 화면상에 보이지 않게 됩니다.
    this->addChild ( pSprite, kTag_ArrowButtonPressed );


    // 오른쪽 화살표 -------------------------------------------------------------
    pSprite = m_pRightSprite = CCSprite::create ( "Images/arrow_right.png" );
	tSize   = pSprite->getContentSize ( );
    
    // 왼쪽 화살표에서 15필셀 오른쪽에 위치시킵니다.
    pSprite->setPosition ( ccp ( m_pLeftSprite->getPosition ( ).x + tSize.cx + 15, m_pLeftSprite->getPosition ( ).y ) );
    this->addChild ( pSprite, kTag_ArrowButton );


    // 눌렸을 때 쓰일 오른쪽 화살표 ------------------------------------------------
	pSprite = m_pRightPressedSprite = CCSprite::create ( "Images/arrow_right_s.png" );
    
    // self.leftSprite와 같은 위치에 표시합니다.
	pSprite->setPosition ( m_pRightSprite->getPosition ( ) );
    
    // 눌렸을 때의 화살표를 하위 z-order로 넣습니다. 그럼, 위에 같은 크기의 화살표가
    // 같은 위치에 있으니까 가려서 화면상에 보이지 않게 됩니다.
    this->addChild ( pSprite, kTag_ArrowButtonPressed );
    
    // 발차기 버튼 --------------------------------------------------------------
    pSprite = m_pKickSprite = CCSprite::create ( "Images/kick.png" );
	tSize   = pSprite->getContentSize ( );

    pSprite->setPosition ( ccp ( tLyrSize.cx - tSize.cx / 2 - 5, m_pLeftSprite->getPosition ( ).y ) );
    this->addChild ( pSprite, kTag_ControlButton );
    
    // 발차기 버튼 눌렸을 때 ------------------------------------------------------
    pSprite = m_pKickPressedSprite = CCSprite::create ( "Images/kick_s.png" );

    pSprite->setPosition ( m_pKickSprite->getPosition ( ) );
    this->addChild ( pSprite, kTag_ControlButtonPressed );
}

KDvoid GameLayer::createPrinceSpriteAndAnimation ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    // prince_walk_notrim.png 파일로 CCSpriteSheet를 만든 후 layer에 넣습니다.
	CCSpriteBatchNode*		pSpriteSheet = CCSpriteBatchNode::create ( "Images/prince_walk_notrim.png" );
    this->addChild ( pSpriteSheet, kTag_PrinceSpriteSheet, kTag_PrinceSpriteSheet );
    
    // 위치정보 파일을 읽어 들여 바로 CCSpriteFrame을 만들어 캐시에 저장합니다.
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/prince_walk_trim.plist" );
    
    // 프레임을 담을 Array를 만듭니다.
	CCArray*	pAniFrames = CCArray::create ( );
    
    // sprite sheet안의 프레임 크기는 88 * 146픽셀입니다.
    CCSize		tFrameSize = CCSize ( 88, 146 );
    
    // 모두 15개의 이미지로 프레임을 만든 후 array에 담습니다.
    // 첫 번째 이미지의 rect는 (0, 0, 88, 146), 두 번째는 (88, 0, 88, 146),
    // 세 번째는 (176, 0, 88, 146), ...
    // 그러므로 n번째 이미지의 x 좌표는 88 * (n-1)이 됩니다.
    for ( KDint nIdx = 0; nIdx < 15; nIdx++ )
	{
        // 첫째 줄에 11개의 프레임이 있고, 12번째부터는 두 번째 줄에 있으므로,
        // 12번째(idx==1)부터는 y좌표의 값을 증가시켜야 합니다.
        KDint	nIndex = nIdx % 11;
        KDint	nRowIndex = nIdx / 11;
        
		CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pSpriteSheet->getTexture ( ), CCRect ( tFrameSize.cx * nIndex, tFrameSize.cy * nRowIndex, tFrameSize.cx, tFrameSize.cy ) );

        pAniFrames->addObject ( pFrame );
    }
    
    // 아주 간단히 프레임을 만들어 배열에 저장할 수 있습니다. 스프라이트 시트를 만들 때 사용한
    // 개별 이미지의 파일 이름을 사용하여 개별 프레임을 읽어 들입니다.
    for ( KDint nIdx = 1; nIdx <= 15; nIdx++ )
	{
		CCSpriteFrame*	pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "prince_walk_%02d.png", nIdx ) );
        pAniFrames->addObject ( pFrame );
    }
    
    
    // 프레임으로 CCAnimation을 만듭니다. 각 프레임당 시간을 0.05초로 정해줍니다.
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pAniFrames, 0.05f );
	pAnimation->setRestoreOriginalFrame ( KD_FALSE );

	CCAnimate*		pAnimate = CCAnimate::create ( pAnimation );
	this->setPrinceWalkAnimate ( pAnimate );
    
    // 첫 번째 프레임으로 주인공 스프라이트를 만듭니다.
	CCSprite*		pSprite = m_pPrinceSprite = CCSprite::createWithSpriteFrameName ( "prince_walk_01.png" ); 
    this->addChild ( pSprite, kTag_Prince, kTag_Prince );
    
    // 위치를 계산하기 편하게 주인공의 anchorPoint를 가운데 맨 아래로 잡습니다.
    pSprite->setAnchorPoint ( ccp ( 0.5, 0) );
    
    // 주인공을 화면 가운데 맨 아래에서 12픽셀 위에 위치시킵니다.
    pSprite->setPosition ( ccp ( tLyrSize.cx / 2, 13 ) );
        
    
    // 발차기 애니메이션에 쓰일 스프라이트 시트의 위치정보 파일을 읽어들입니다.
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/prince_kick_notrim.plist" );
    
    pAniFrames->removeAllObjects ( );
    for ( KDint nIdx = 1; nIdx <= 15; nIdx++ )
	{
		CCSpriteFrame*	pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "prince_kick_%02d.png", nIdx ) );
        pAniFrames->addObject ( pFrame );
    }
    
    // 프레임으로 CCAnimation을 만듭니다. 프레임당 시간을 0.04초로 정해줍니다.
	pAnimation = CCAnimation::createWithSpriteFrames ( pAniFrames, 0.04f );
	pAnimation->setRestoreOriginalFrame ( KD_FALSE );
    
    // CCAnimation에 action을 줄 CCAnimate를 만듭니다.
	pAnimate = CCAnimate::create ( pAnimation );
	this->setPrinceKickAnimate ( pAnimate );
}

KDvoid GameLayer::createEnemyAndAnimation ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    // 위치정보 파일을 읽어들여 바로 CCSpriteFrame을 만들어 캐시에 저장합니다.
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/enemy.plist" );
    
    // 위치정보 파일과 같은 이름을 가진 스프라이트 시트로 CCSpriteSheet객체를 만듭니다.
	
	m_pEnemySpriteSheet = CCSpriteBatchNode::create ( "Images/enemy.png" );
	this->addChild ( m_pEnemySpriteSheet, kTag_Enemy );

	// 프레임을 담을 Array를 만듭니다.
	CCArray*	pAniFrames = CCArray::create ( );
    
    // 아주 간단히 프레임을 만들어 배열에 저장할 수 있습니다. 스프라이트 시트를 만들 때 사용한
    // 개별 이미지의 파일 이름을 사용하여 개별 프레임을 읽어들입니다.
    for ( KDint nIdx = 1; nIdx <= 15; nIdx++ ) 
	{
		CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "zombie_walk_%04d.png", nIdx ) );
        pAniFrames->addObject ( pFrame );
    }
    
    // 프레임으로 CCAnimation을 만듭니다. 프레임당 시간을 0.15초로 정해줍니다.
	CCAnimation*	pWalkAnimation = CCAnimation::createWithSpriteFrames ( pAniFrames, 0.15f );
	pWalkAnimation->setRestoreOriginalFrame ( KD_FALSE );
	this->setEnemyWalkAnmation ( pWalkAnimation );
    
	// 공격에 해당하는 이미지로 프레임을 만듭니다.
    CCArray*		pAttackFrames = CCArray::create ( );
    for ( KDint nIdx = 1; nIdx <= 15; nIdx++ )
	{
		CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "zombie_attack_%04d.png", nIdx ) );
		pAttackFrames->addObject ( pFrame );
    }

    // 프레임으로 CCAnimation을 만듭니다. 각 프레임당 시간을 0.15초로 정해줍니다.
    CCAnimation*	pAttackAnimation = CCAnimation::createWithSpriteFrames ( pAttackFrames, 0.07f ); 
	pAttackAnimation->setRestoreOriginalFrame ( KD_FALSE );
	this->setEnemyAttackAnimation ( pAttackAnimation );

    // 적 케릭터 1
	EnemySprite*	pEnemy = EnemySprite::create ( "zombie_walk_0001.png", ccp ( tLyrSize.cx / 2 + 100, 13 ), 0.5f, this );
	m_pEnemySpriteSheet->addChild ( pEnemy );

    // 적 케릭터 1
	pEnemy = EnemySprite::create ( "zombie_walk_0001.png", ccp ( tLyrSize.cx / 2 - 150, 13 ), 0.3f, this );
	m_pEnemySpriteSheet->addChild ( pEnemy );

    // 적 케릭터 1
	pEnemy = EnemySprite::create ( "zombie_walk_0001.png", ccp ( -100, 13 ), 0.7f, this );
	m_pEnemySpriteSheet->addChild ( pEnemy );
}

KDvoid GameLayer::createEnergyBar ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    // 에너지 바의 가로*세로 크기를 정하고, 최고 에너지 값을 정해줍니다.
	EnergyBar*	pBar = m_pPrinceEnergyBar = EnergyBar::create ( CCSize ( 120, 10), INIT_NUM_ENERGY );
    
    pBar->setPosition ( ccp ( tLyrSize.cx / 2 + 75, tLyrSize.cy - 22 ) );
    this->addChild ( pBar, kTag_Label );
}

KDvoid GameLayer::createGamePauseResumeMenu ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );
	CCSize			tSize = ccsz;
	CCPoint			tPos  = ccpz;

	m_pPauseMenuItem = CCMenuItemImage::create ( "Images/btn_pause.png", "Images/btn_pause_s.png", this, menu_selector ( GameLayer::playPauseMenuCallback ) );
	m_pPlayMenuItem  = CCMenuItemImage::create ( "Images/btn_play.png" , "Images/btn_play_s.png" , this, menu_selector ( GameLayer::playPauseMenuCallback ) );
    
	tSize = m_pPauseMenuItem->getContentSize ( );
	tPos  = ccp ( tLyrSize.cx - tSize.cx / 2 - 5, tLyrSize.cy - tSize.cy / 2 - 5 );
    m_pPauseMenuItem->setPosition ( tPos );
	m_pPlayMenuItem ->setPosition ( tPos );
    
    // 처음에는 게임이 플레이 모드이므로 플레이 메뉴 아이템을 보이지 않게 합니다.
    m_pPlayMenuItem->setVisible ( KD_FALSE );
    
	CCMenu*		pMenu = CCMenu::create ( m_pPauseMenuItem, m_pPlayMenuItem, KD_NULL );
	this->addChild ( pMenu, kTag_PauseResumeMenu, kTag_PauseResumeMenu );
}

KDvoid GameLayer::addNewEnemy ( KDfloat fDelta )
{
    // 만약 현재 적 케릭터의 수가 4개면 더 이상 만들지 않습니다.
    if ( m_pEnemySpriteSheet->getChildren ( )->count ( ) >= 4 )
	{
        return;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	// 움직이는 속도와 위치를 다르게 하여 적 케릭터를 만듭니다.
	KDfloat		fPosX = tLyrSize.cx + 100.f;
	if ( kdRand ( ) % 100 < 50 )
	{
		fPosX = -100.f;
	}

	KDfloat		fSpeed = 0.30f + ( ( kdRand ( ) % 70 ) / 100 );

	EnemySprite*	pEnemy = EnemySprite::create ( "zombie_walk_0001.png", ccp ( fPosX, 13 ), fSpeed, this );    
    m_pEnemySpriteSheet->addChild ( pEnemy );
}

KDbool GameLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    // pause시켜도 계속해서 터치 이벤트를 받기 때문에, 방향 버튼과 공격 버튼의
    // 터치 이벤트를 처리하면 안 됩니다. 이를 방지하기 위하여 GameStatus 값을 사용합니다.
    if ( m_eGameStatus == kGamePaused || m_bIsAnimating == KD_TRUE ) 
	{
        CCLOG ( "Touch Began ignored" );
        return KD_FALSE;
    }

    // 발차기 버튼이 터치되었는지 검사
    if ( this->isTouchInside ( m_pKickSprite, pTouch ) == KD_TRUE ) 
	{
		const CCPoint&		tPrincePos = m_pPrinceSprite->getPosition ( );

		m_pKickSprite->setVisible ( KD_FALSE );

        // 발차기 공격의 유효 타격거리 안에 들어와 있는 적이 있는지 확인합니다.
        // 주인공 sprite 중심에서 40픽셀, 적 sprite의 중심에서 30픽셀 되는 거리를 발차기 유효거리로 정합니다.
        // 위에서 사용된 수치는 현재 사용 중인 이미지의 모양과 크기에 따라서 정한 숫자이므로 자신이 사용할 이미지에 맞게 조정합니다.
        KDuint		uNumHits = 0;

		CCArray*	pEnemies = m_pEnemySpriteSheet->getChildren ( );
		CCObject*	pObject  = KD_NULL;
		CCARRAY_FOREACH ( pEnemies, pObject )
		{
			EnemySprite*	pEnemy = (EnemySprite*) pObject;
			pEnemy->adjustPosition ( );

			const CCPoint&	tEnemyPos = pEnemy->getPosition ( );

			if ( m_pPrinceSprite->isFlipX ( ) == KD_FALSE && 
				 tPrincePos.x + 40 >= tEnemyPos.x - 30 &&
				 tPrincePos.x + 40 <= tEnemyPos.x + 30 )
			{				
				this->handleKickHit ( ccp ( tPrincePos.x + 55, tPrincePos.y + 113 ), pEnemy, kFallRight );
                uNumHits++;                
            }
			else if ( m_pPrinceSprite->isFlipX ( ) == KD_TRUE && 
					  tPrincePos.x - 40 <= tEnemyPos.x + 30 && 
					  tPrincePos.x - 40 >= tEnemyPos.x - 30 )
			{
				this->handleKickHit ( ccp ( tPrincePos.x - 55, tPrincePos.y + 113 ), pEnemy, kFallLeft );
				uNumHits++;
            }
		}

		// 발차기 애니메이션이 진행 중
        m_bIsAnimating = KD_TRUE; 
        
		m_pPrinceSprite->runAction 
		(
			CCSequence::create 
			(
				m_pPrinceKickAnimate, 
				CCCallFunc::create ( this, callfunc_selector ( GameLayer::kickAnimateCompleteHandler ) ),
				KD_NULL
			)
		);
  
		m_nScoreValue += uNumHits * 10;
        this->displayScore ( );
    }
    
	KDbool		bRet = KD_FALSE;

	m_bIsLeftPresssed  = KD_FALSE;
	m_bIsRightPresssed = KD_FALSE;

    // 터치가 왼쪽 또는 오른쪽 화살표 안에 들어왔는지 확인합니다.
    if ( this->isTouchInside ( m_pLeftSprite, pTouch ) == KD_TRUE )
	{
        // 왼쪽 화살표를 안보이게 합니다. 그럼 아래에 있던 눌릴 때
        // 보여지는 이미지가 나탈 것입니다.
        m_pLeftSprite->setVisible ( KD_FALSE );

		m_bIsLeftPresssed = KD_TRUE;
    } 
	else if ( this->isTouchInside ( m_pRightSprite, pTouch ) == KD_TRUE )
	{
        // 오른쪽 화살표를 안보이게 합니다.
        m_pRightSprite->setVisible ( KD_FALSE ); 
		
		m_bIsRightPresssed = KD_TRUE;
    }
    
    // 버튼이 눌려졌으면 화면을 움직입니다.
	if ( m_bIsLeftPresssed == KD_TRUE || m_bIsRightPresssed == KD_TRUE ) 
	{
        this->startMovingBackground ( );
        bRet = KD_TRUE;
    }

	return bRet;
}

KDvoid GameLayer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( m_eGameStatus == kGamePaused )
	{
        return;
	}

    // 손가락이 버튼을 벗어나면 움직임이 중단됩니다.
    if ( m_bIsLeftPresssed == KD_TRUE && this->isTouchInside ( m_pLeftSprite, pTouch ) == KD_FALSE ) 
	{
        m_pLeftSprite->setVisible ( KD_TRUE );
        this->stopMovingBackground ( );
    } 
	else if ( m_bIsRightPresssed == KD_TRUE && this->isTouchInside ( m_pRightSprite, pTouch ) == KD_FALSE ) 
	{
        m_pRightSprite->setVisible ( KD_TRUE );
        this->stopMovingBackground ( );
    }
}

KDvoid GameLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( m_eGameStatus == kGamePaused )
	{
        return;
	}
    
    // 배경 화면을 멈춥니다.
    if ( m_bIsLeftPresssed == KD_TRUE || m_bIsRightPresssed == KD_TRUE )
	{
        this->stopMovingBackground ( );
    }
    
    // 감춰졌던 버튼 이미지를 다시 보이게 합니다.
    if ( m_bIsLeftPresssed == KD_TRUE ) 
	{
        m_pLeftSprite->setVisible ( KD_TRUE );
    }
    
    if ( m_bIsRightPresssed == KD_TRUE ) 
	{
        m_pRightSprite->setVisible ( KD_TRUE );
    }
}

KDbool GameLayer::isTouchInside ( CCSprite* pSprite, CCTouch* pTouch )
{
	CCRect		tRect;
	tRect.size = pSprite->getContentSize ( );

	return tRect.containsPoint ( pSprite->convertTouchToNodeSpace ( pTouch ) );
}

KDvoid GameLayer::displayScore ( KDvoid )
{
	m_pScoreLabel->setString ( ccszf ( "Score: %d", m_nScoreValue ) );
}

KDvoid GameLayer::displayLife ( KDvoid )
{
	m_pLifeLabel->setString ( ccszf ( "Life: %d", m_nLifeValue ) );
}

KDvoid GameLayer::playPauseMenuCallback ( CCObject* pSender )
{
    // 플레이 메뉴 아이템이 보이지 않는 상태라면, 게임을 일시정지시킵니다.
    if ( m_pPlayMenuItem->isVisible ( ) == KD_FALSE )
	{
        m_eGameStatus = kGamePaused;
        
        // 모든 애니메이션을 일시정지시킵니다.
        // pause시켜도 계속해서 터치 이벤트를 받습니다.
		CCDirector::sharedDirector ( )->pause ( );
        
        // 플레이 메뉴 아이템을 보여줍니다.
        m_pPlayMenuItem->setVisible ( KD_TRUE );
    } 
	else 
	{
        m_eGameStatus = kGamePlaying;
        
        m_pPlayMenuItem->setVisible ( KD_FALSE );
        
        // 모든 애니메이션을 다시 작동시킵니다.
        CCDirector::sharedDirector ( )->resume ( );
    }
}

KDvoid GameLayer::kickAnimateCompleteHandler ( KDvoid )
{
    // 버튼을 다시 보이게 합니다.
    m_pKickSprite->setVisible ( KD_TRUE );
    
    m_pPrinceSprite->stopAllActions ( );
    m_bIsAnimating = KD_FALSE;
    
    // 주인 스프라이트를 원래 색으로 돌립니다.
    m_pPrinceSprite->setColor ( ccc3 ( 255, 255, 255 ) );
}

KDvoid GameLayer::moveBackground ( KDfloat fDelta )
{
	const CCSize&		tLyrSize = this->getContentSize ( );

    // gameLayer에 들어 있는 parallax node를 받습니다.
    CCNode*		pVoidNode = this->getChildByTag ( kTag_Parallax );
	CCPoint		tVoidPos  = pVoidNode->getPosition ( );

    // 매 프레임마다 움직일 거리
    CCPoint		tMoveStep = ccp ( 3, 0 );
    
    // 오른쪽 버튼이 눌려졌을 때는 반대로 움직입니다.
	if ( m_bIsRightPresssed )
	{
        tMoveStep.x = -tMoveStep.x;
	}    

    KDfloat		fBGParallaxRatio = 1.0f;
    
    CCPoint		tNewPos = ccp ( tVoidPos.x + tMoveStep.x, tVoidPos.y );
    
    // 배경이 양쪽 끝에 도달하면 더 이상 움직이지 않습니다.
	if ( m_bIsLeftPresssed == KD_TRUE && tNewPos.x > 0 )
	{
        tNewPos.x = 0;
	}
    else if ( m_bIsRightPresssed == KD_TRUE && tNewPos.x < -( IMG_WIDTH - tLyrSize.cx ) / fBGParallaxRatio )
	{
        tNewPos.x = -( IMG_WIDTH - tLyrSize.cx ) / fBGParallaxRatio;
	}
  
    // 주인공이 화면 가운데 있을 때만 배경을 움직입니다.
    KDfloat		fHalfWinWidth = tLyrSize.cx / 2;
    if ( m_pPrinceSprite->getPosition ( ).x == fHalfWinWidth )
	{
        pVoidNode->setPosition ( tNewPos );
    }
    
    // 주인공의 방향을 정합니다.
	if ( m_bIsRightPresssed == KD_TRUE )
	{
        m_pPrinceSprite->setFlipX ( KD_FALSE );
	}
    else
	{
		m_pPrinceSprite->setFlipX ( KD_TRUE );
	}

    // 만약 주인공이 화면 가운데 있지 않으면 주인공을 화면 가운데로 이동시킵니다.
    if ( m_bIsRightPresssed == KD_TRUE && m_pPrinceSprite->getPosition ( ).x < fHalfWinWidth )
	{
        // moveStep.x의 부호를 바꾼 이유는 배경과 주인공의 움직임 방향이
        // 서로 반대이기 때문입니다.
		m_pPrinceSprite->setPosition ( ccpAdd ( m_pPrinceSprite->getPosition ( ), ccp ( tMoveStep.x * -1, 0 ) ) );
        
        // 가운데 이상 움직이지 않도록 체크합니다.
        if ( m_pPrinceSprite->getPosition ( ).x > fHalfWinWidth )
		{
			m_pPrinceSprite->setPosition ( ccp ( fHalfWinWidth, m_pPrinceSprite->getPosition ( ).y ) );
        }
    } 
	else if ( m_bIsLeftPresssed == KD_TRUE && m_pPrinceSprite->getPosition ( ).x > fHalfWinWidth )
	{
        // moveStep.x의 부호를 바꾼 이유는 배경과 주인공의 움직임 방향이 서로 반대이기 때문입니다.
        m_pPrinceSprite->setPosition ( ccpAdd (  m_pPrinceSprite->getPosition ( ), ccp ( tMoveStep.x * -1, 0 ) ) );
        
        // 가운데 이상 움직이지 않도록 체크합니다.
        if ( m_pPrinceSprite->getPosition ( ).x < fHalfWinWidth ) 
		{
            m_pPrinceSprite->setPosition ( ccp ( fHalfWinWidth, m_pPrinceSprite->getPosition ( ).y ) );
        }
    }
    
    // 배경의 끝에 도달하면 배경은 움직이지 않고 주인공을 화면 끝까지 이동시킵니다.
    if ( tNewPos.x == 0 || tNewPos.x == -( IMG_WIDTH - tLyrSize.cx ) ) 
	{
        CCPoint		tNewPrincePos = ccpAdd ( m_pPrinceSprite->getPosition ( ), ccp ( tMoveStep.x * -1, 0 ) );
        
        // 주인공이 화면 왼쪽 또는 오른쪽 끝까지 도달했을 때는 더 이상 움직이지 않습니다.
        KDfloat		fHalfWidth = m_pPrinceSprite->getContentSize ( ).cx / 2;
        if ( tNewPrincePos.x <= fHalfWidth )
		{
            tNewPrincePos.x = fHalfWidth;
		}
        else if ( tNewPrincePos.x >= tLyrSize.cx - fHalfWidth )
		{
            tNewPrincePos.x = tLyrSize.cx - fHalfWidth;
		}
        
        m_pPrinceSprite->setPosition ( tNewPrincePos );
    } 
	else 
	{
        // 주인공의 움직임에 맞쳐 적의 위치를 조정합니다.
		CCArray*	pEnemies = m_pEnemySpriteSheet->getChildren ( );
		CCObject*	pObject  = KD_NULL;
		CCARRAY_FOREACH ( pEnemies, pObject )
		{
			EnemySprite*	pEnemy = (EnemySprite*) pObject;
			pEnemy->adjustPosition ( );
		}
    }
}

KDvoid GameLayer::startMovingBackground ( KDvoid )
{
    // 만약 버튼 두 개가 눌려졌으면 화면을 이동시키지 않습니다.
	if ( m_bIsLeftPresssed == KD_TRUE && m_bIsRightPresssed == KD_TRUE )
	{
		return;
	}

    // schedule을 이용하여 매 프레임마다 배경 화면을 움직입니다.
	this->schedule ( schedule_selector ( GameLayer::moveBackground ) );
    
    // 걷기 애니메이션을 시작합니다.
	this->startPrinceWalking ( );
}

KDvoid GameLayer::stopMovingBackground ( KDvoid )
{
	this->unschedule ( schedule_selector ( GameLayer::moveBackground ) );
    
    //걷기 애니메이션을 멈춤니다.
    this->stopPrinceWalking ( );
}

KDvoid GameLayer::startPrinceWalking ( KDvoid )
{    
	m_pPrinceSprite->runAction ( CCRepeatForever::create ( m_pPrinceWalkAnimate ) );        
}

KDvoid GameLayer::stopPrinceWalking ( KDvoid )
{
	m_pPrinceSprite->stopAllActions ( );
}	

KDvoid GameLayer::handleKickHit ( const CCPoint& tEffectPoint, CCSprite* pEnemy, FallDirection eFallDirection )
{
    // 발차기 공격 이펙트
	CCSprite*	pHitEffectSprite = CCSprite::create ( "Images/hit_effect.png" );
    pHitEffectSprite->setPosition ( tEffectPoint );
	this->addChild ( pHitEffectSprite, kTag_Effect );
    
    // 발차기 이펙트 애니메이션
    pHitEffectSprite->setScale ( 0.3f );
    
    // CCSequence를 사용하면 여러 개의 action을 순서대로 쓸 수 있습니다.
    pHitEffectSprite->runAction
	(
		CCSequence::create 
		(
            // 주인공 발차기 애니메이션이 이루어지는 시간 동안 잠시 기다립니다.
			CCDelayTime::create ( 0.05f ),
			CCScaleTo  ::create ( 0.4f, 1.0f ),
            // 위 Scale 액션이 끝나면 hitEffectCompleteHandler 매서드를 호출합니다.
			CCCallFuncN::create ( this, callfuncN_selector ( GameLayer::hitEffectCompleteHander ) ),
            KD_NULL 
		)
	);

    // 적 케릭터 넘어지는 애니메이션
    KDfloat		fDiffX = 120;
    KDfloat		fAngle = 90;
    if ( eFallDirection == kFallLeft )
	{
        fDiffX = -120;
        fAngle = -90;
    }
    
    CCPoint		tTargetPoint = ccp ( tEffectPoint.x + fDiffX, pEnemy->getPosition ( ).y );
    
	// CCSpawn을 사용하면 여러 종류의 action을 동시에 쓸 수 있습니다.
	CCSpawn*	pSpawn = CCSpawn::create
	(
		CCJumpTo  ::create ( 0.7f, tTargetPoint, 80, 1 ),
		CCRotateBy::create ( 0.5f, fAngle ),
		KD_NULL
	);

	pEnemy->runAction
	(
		CCSequence::create 
		(
			CCEaseIn::create ( pSpawn, 2.0f ),
			CCCallFuncND::create ( this, callfuncND_selector ( GameLayer::enemyFallCompleteHander ), pEnemy ),
			KD_NULL
		)
	);
    
    // 애니메이션을 처리하는 코드 다음에 소리를 내는 코드를 넣는 것이 좋습니다.
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/soundKick.wav" );
}

KDvoid GameLayer::hitEffectCompleteHander ( CCNode* pNode )
{
    if ( pNode != KD_NULL )
	{
		this->removeChild ( pNode );
    }
}

KDvoid GameLayer::enemyFallCompleteHander ( CCNode* pNode, KDvoid* pData )
{
	CCSprite*	pEnemy = (CCSprite*) pData;

    m_pEnemySpriteSheet->stopAllActions ( );
    
    if ( pEnemy != KD_NULL )
	{
        m_pEnemySpriteSheet->removeChild ( pEnemy, KD_TRUE );
    }
}

KDvoid GameLayer::princeAttacked ( FallDirection eDirection )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    // 에너지를 감소시킵니다.
    m_nEnergyValue--;
    
    if ( m_nEnergyValue <= 0 )
	{
        // 라이프를 감소시킵니다.
        m_nLifeValue--;
        
        if ( m_nLifeValue <= 0 )
		{
            CCLOG ( "GAME OVER" );
            
            // 임시로 다시 원래값으로 리셋합니다.
            m_nLifeValue = INIT_NUM_LIFE;
        }
        
        this->displayLife ( );
        
        // 에너지를 재충전시킵니다.
        m_nEnergyValue = INIT_NUM_ENERGY;
    }

    // 에너지 바를 업데이트합니다.
    m_pPrinceEnergyBar->updateBar ( (KDfloat) m_nEnergyValue );

    // 좌우로 60픽셀만큰 밀려나게 합니다.
    KDfloat		fDiffX = 60.0;
    if ( eDirection == kFallLeft )
	{
        fDiffX *= -1.0;
    }

    // x좌표의 값이 화면 바깥으로 나가지 않게 합니다.
    KDfloat		fPosX = m_pPrinceSprite->getPosition ( ).x + fDiffX;
    if ( fPosX < 0 )
	{
        fPosX = 0;
	}
    else if ( fPosX > tLyrSize.cx )
	{
        fPosX = tLyrSize.cx;
	}
    
    CCPoint		tTargetPoint = ccp ( fPosX, m_pPrinceSprite->getPosition ( ).y );
    
    KDfloat		fDuration = 0.5f;

	m_pPrinceSprite->runAction 
	(
		CCSequence::create 
		(
			CCSpawn::create 
			(
				// 빨간색으로 바꿉니다.
				CCTintBy::create ( fDuration / 2.f, 0, -255, -255 ),
				// CCEaseExponentialOut을 사용하여 속도감이 있게 합니다.
				CCEaseExponentialOut::create ( CCMoveTo::create ( fDuration, tTargetPoint ) ),
				KD_NULL
			),
			CCCallFunc::create ( this, callfunc_selector ( GameLayer::princeAttackedCompleteHandler ) ),
			KD_NULL
		)
	);
}

KDvoid GameLayer::princeAttackedCompleteHandler ( KDvoid )
{
    // Tint로 바뀐 색을 원래 색으로 돌립니다.
    m_pPrinceSprite->setColor ( ccc3 ( 255, 255, 255 ) );
}

CCSprite* GameLayer::getPrinceSprite ( KDvoid )
{
	return m_pPrinceSprite;
}

CCAnimate* GameLayer::getPrinceWalkAnimate ( KDvoid )
{
	return m_pPrinceWalkAnimate;
}

KDvoid GameLayer::setPrinceWalkAnimate ( CCAnimate* pAnimate )
{
	CC_SAFE_RELEASE ( m_pPrinceWalkAnimate );
	m_pPrinceWalkAnimate = pAnimate;
	CC_SAFE_RETAIN  ( m_pPrinceWalkAnimate );
}

CCAnimate* GameLayer::getPrinceKickAnimate ( KDvoid )
{
	return m_pPrinceKickAnimate;
}

KDvoid GameLayer::setPrinceKickAnimate ( CCAnimate* pAnimate )
{
	CC_SAFE_RELEASE ( m_pPrinceKickAnimate );
	m_pPrinceKickAnimate = pAnimate;
	CC_SAFE_RETAIN  ( m_pPrinceKickAnimate );
}

CCAnimation* GameLayer::getEnemyWalkAnmation ( KDvoid )
{
	return m_pEnemyWalkAnmation;
}

KDvoid GameLayer::setEnemyWalkAnmation ( CCAnimation* pAnimation )
{
	CC_SAFE_RELEASE ( m_pEnemyWalkAnmation );
	m_pEnemyWalkAnmation = pAnimation;
	CC_SAFE_RETAIN  ( m_pEnemyWalkAnmation );
}

CCAnimation* GameLayer::getEnemyAttackAnimation ( KDvoid )
{
	return m_pEnemyAttackAnimation;
}

KDvoid GameLayer::setEnemyAttackAnimation ( CCAnimation* pAnimation )
{
	CC_SAFE_RELEASE ( m_pEnemyAttackAnimation );
	m_pEnemyAttackAnimation = pAnimation;
	CC_SAFE_RETAIN  ( m_pEnemyAttackAnimation );
}
