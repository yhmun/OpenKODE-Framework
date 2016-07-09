#include "Precompiled.h"
#include "WinnerScene.h"
#include "MainScene.h"
 
KDbool WinnerScene::init ( KDvoid )
{
	if( CCScene::init ( ) )
    {
		this->m_pLayer = WinnerLayer::create ( );
        this->m_pLayer->retain ( );
        this->addChild ( m_pLayer );

        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}


WinnerScene::WinnerScene ( )
{
	m_pLayer = KD_NULL;
}

WinnerScene::~WinnerScene ( )
{
   CC_SAFE_RELEASE ( m_pLayer );
}

KDbool WinnerLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

    if ( CCLayerColor::initWithColor( ccc4 ( 255,255,255,255 ) ) )
    {
       
		
		CCSize tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

		CCSprite* pBg = CCSprite::create ( "Bgempty.png" );
		pBg->setPosition ( ccp ( tWinSize.cx/2, tWinSize.cy/2 ) );
		this->addChild ( pBg,0 );


		CCSprite* pSprite = CCSprite::create ( "youwin.png" );
		pSprite->setPosition ( ccp ( tWinSize.cx/2, tWinSize.cy/2+50 ) );
		this->addChild ( pSprite,0 );

    
		ExplosionAnimation ( ccp ( tWinSize.cx/2, tWinSize.cy/2+50 ), "balloonBlue.png" );
		ExplosionAnimation ( ccp ( tWinSize.cx/2-100, tWinSize.cy/2+50 ), "balloonRed.png" );
		ExplosionAnimation ( ccp ( tWinSize.cx/2+100, tWinSize.cy/2+50 ), "balloonYellow.png" );

        this->runAction( CCSequence::create(
								CCDelayTime::create(5),
								CCCallFunc::create(
									this, 
									callfunc_selector ( WinnerLayer::WinnerDone ) ),
									KD_NULL ) );

        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}

KDvoid WinnerLayer::WinnerDone ( KDvoid )
{
	CCScene* pScene = CCScene::create ( );
    pScene->addChild ( MainScene::create ( ) );
    CCDirector::sharedDirector()->replaceScene ( pScene );
}

WinnerLayer::WinnerLayer ( )
{
	m_pLabel = KD_NULL;
}
WinnerLayer::~WinnerLayer ( )
{
    CC_SAFE_RELEASE ( m_pLabel );
}

KDvoid WinnerLayer::ExplosionAnimation ( CCPoint tPos, const KDchar* szImgName )
{

	CCParticleExplosion* emitter = new CCParticleExplosion ( );
	emitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szImgName ) );
	emitter->initWithTotalParticles ( 20 );
	emitter->setStartSize ( 20.0f );
	emitter->setSpeed ( 50.0f );
	emitter->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );
	emitter->setPosition ( tPos );
	emitter->setDuration ( 0.5f );
	ccColor4F startColor = { 219.5f, 147.5f, 29.5f, 1.0f };
	emitter->setStartColor ( startColor );
	
	ccColor4F startColorVar = { 219.5f, 147.5f, 29.5f, 1.0f };
	emitter->setStartColorVar ( startColorVar );
	
	ccColor4F endColor = { 167.1f, 91.1f, 39.1f, 1.0f };
	emitter->setEndColor ( endColor );
	
	ccColor4F endColorVar = {167.1f, 91.1f, 39.1f, 1.0f};    
	emitter->setEndColorVar ( endColorVar );
	emitter->setAutoRemoveOnFinish ( KD_TRUE );
	emitter->retain ( );
	addChild ( emitter, 1) ;
	emitter->release ( );

}