#include "Precompiled.h"
#include "GameOverScene.h"
#include "MainScene.h"

KDbool GameOverScene::init()
{
	if( CCScene::init() )
    {
        this->m_pLayer = GameOverLayer::create ( );
        this->m_pLayer->retain ( );
        this->addChild ( m_pLayer );

        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}

GameOverScene::GameOverScene ( KDvoid )
{
	m_pLayer = KD_NULL;
}

GameOverScene::~GameOverScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLayer );
}

KDbool GameOverLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

    if ( CCLayerColor::initWithColor ( ccc4 ( 255,255,255,255 ) ) )
    {
		CCSize tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

		CCSprite* pBg = CCSprite::create ( "bgempty.png" );

		pBg->setPosition ( ccp ( tWinSize.cx/2, tWinSize.cy/2 ) );
		this->addChild ( pBg,0 );

		CCSprite* pSprite = CCSprite::create("gameover.png");

		pSprite->setPosition ( ccp ( tWinSize.cx/2, tWinSize.cy/2+50 ) );
		this->addChild ( pSprite,0 );


        this->runAction( 
			CCSequence::create(
				CCDelayTime::create(3),
				CCCallFunc::create(
					this, 
					callfunc_selector ( GameOverLayer::gameOverDone ) ),
				NULL ) );

        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}

KDvoid GameOverLayer::gameOverDone ( KDvoid )
{
    CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( MainScene::create ( ) );
    CCDirector::sharedDirector()->replaceScene ( pScene );
}

GameOverLayer::GameOverLayer ( )
{
	m_pLabel = KD_NULL;
}
GameOverLayer::~GameOverLayer ( )
{
	CC_SAFE_RELEASE ( m_pLabel );
}