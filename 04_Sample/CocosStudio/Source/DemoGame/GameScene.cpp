/* -----------------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "GameScene.h"
#include "MyController.h"

GameScene* GameScene::create ( KDint nStage )
{
	GameScene*	pRet = new GameScene ( );

    if ( pRet && pRet->init ( nStage ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool GameScene::init ( KDint nStage )
{
	KDbool	bRet = false;
    do
    {
        this->setContentSize ( Director::getInstance ( )->getWinSize ( ) );

        //add stage by stage number
        if ( nStage == 1 )
        {
			this->addChild ( StageOne::create ( ) );
        }
        else
		{
			this->addChild ( StageTwo::create ( ) );
		}

        // success
        bRet = true;

    } while ( 0 );

    return bRet;
}

KDvoid GameScene::menuPopSceneCallback ( Object* pSender )
{
	Director::getInstance ( )->popScene ( );
}

KDbool StageBasic::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	m_pStageLayer = Layer::create ( );

	//create exit stage button
	MenuItemImage*	pCloseItem = MenuItemImage::create
	(
		"CloseNormal.png","CloseSelected.png", CC_CALLBACK_1 ( StageBasic::menuTestCallback, this ) 
	);
    
	pCloseItem->setPosition ( Point ( Director::getInstance ( )->getVisibleSize ( ).width * 0.5f - pCloseItem->getContentSize ( ).width / 2, 
									  Director::getInstance ( )->getVisibleSize ( ).height * 0.5f - pCloseItem->getContentSize ( ).height / 2 ) );
    this->addChild ( Menu::create ( pCloseItem, nullptr ), 4 );
	
	return true;
}

KDvoid StageBasic::menuTestCallback ( Object* pSender )
{
	overStage ( false );
}

KDbool StageBasic::initWorld ( KDvoid )
{
	m_pPhysicalWorld = PhysicalWorld::create ( m_pEnemyBuilder );
	m_pStageLayer->addChild ( m_pPhysicalWorld, 2 );
	//set stageLayer follow player's armature
	m_pStageLayer->runAction ( Follow::create ( m_pPhysicalWorld->getPlayer ( )->getArmature ( ), Rect ( 0, 0, 720, 320 ) ) );

	//create controller layer
    MyController*	pController = MyController::create ( );
    this->addChild ( pController, 4 );
	pController->bindPlayer ( m_pPhysicalWorld->getPlayer ( ) );

	this->addChild ( m_pStageLayer );

	scheduleUpdate ( );
	
	return true;
}

KDvoid StageBasic::update ( KDfloat fDelta )
{
	if ( m_pPhysicalWorld->isGameOver ( ) ) 
	{
		overStage ( true );
	}

	if ( m_pPhysicalWorld->isPassStage ( ) ) 
	{
		overStage ( false );
	}
}

KDvoid StageBasic::overStage ( KDbool bPlayerDied )
{
	if ( bPlayerDied )	
	{
		changeToOverScene ( );
	}
	else
	{
		changeToNextStage ( );
	}
}

KDvoid StageBasic::changeToOverScene ( KDvoid )
{
	unscheduleUpdate ( );

	GameOverScene*		pOverScene = GameOverScene::create ( );
	Director::getInstance ( )->replaceScene ( pOverScene );
}

StageOne* StageOne::create ( KDvoid )
{
	StageOne*	pRet = new StageOne ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

	return pRet;
}

KDbool StageOne::init ( KDvoid )
{
	if ( !StageBasic::init ( ) )
	{
		return false;
	}    

    Sprite*		pSprite = Sprite::create ( "stage1.png" );
    
    pSprite->setPosition ( Point ( Director::getInstance ( )->getVisibleSize ( ).width * 0.5,
								   Director::getInstance ( )->getVisibleSize ( ).height * 0.5 ) );
    //m_pStageLayer->addChild ( pSprite, -1 );
	
	m_pEnemyBuilder = new StageOneEnemyList ( );
    
	if ( !StageBasic::initWorld ( ) )
	{
		return false;
	}

    return true;
}

KDvoid StageOne::changeToNextStage ( KDvoid )
{
	VictoryScene*	pVictoryScene = VictoryScene::create ( );
	Director::getInstance()->replaceScene ( pVictoryScene );
}

KDvoid StageOne::menuNextStageCallback ( Object* pSender )
{
	GameScene*		pStageTwoScene = GameScene::create ( 2 );
	Director::getInstance ( )->replaceScene ( pStageTwoScene );
}


StageTwo* StageTwo::create ( KDvoid )
{
	StageTwo*	pRet = new StageTwo ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );
        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool StageTwo::init ( KDvoid )
{
   	if ( !StageBasic::init ( ) )
	{
		return false; 
	}

    Sprite*		pSprite = Sprite::create ( "stage2.png" );
    
    pSprite->setPosition ( Point ( Director::getInstance ( )->getVisibleSize ( ).width * 0.5,
								   Director::getInstance ( )->getVisibleSize ( ).height * 0.5 ) );
    m_pStageLayer->addChild ( pSprite, -1 );
    
	m_pEnemyBuilder = new StageTwoEnemyList ( );

	if ( !StageBasic::initWorld ( ) )
	{
		return false;
	}

    return true;
}

KDvoid StageTwo::changeToNextStage ( KDvoid )
{
	VictoryScene*	pVictoryScene = VictoryScene::create ( );
	Director::getInstance ( )->replaceScene ( pVictoryScene );
}

GameOverScene* GameOverScene::create ( KDvoid )
{
	GameOverScene*	pRet = new GameOverScene ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool GameOverScene::init ( KDvoid )
{
	KDbool			bRet = false;
    do
    {
        Director*	pDirector;
        CC_BREAK_IF ( !( pDirector = Director::getInstance ( ) ) );
        this->setContentSize ( pDirector->getWinSize ( ) );
        
        Sprite*		pBg = Sprite::create ( "gameover.png" );
        pBg->setPosition ( Point(pDirector->getWinSize ( ).width * 0.5f, pDirector->getWinSize ( ).height * 0.5 ) );
        this->addChild ( pBg );
 
        MenuItemFont*	pMainMenu = MenuItemFont::create ( "Back To Menu", CC_CALLBACK_1 ( GameOverScene::menuPopSceneCallback, this )  ); 
        MenuItemFont*	pClose = MenuItemFont::create ( "Exit Game", CC_CALLBACK_1 ( GameOverScene::menuCloseCallback, this ) );
        Menu*			pMenu = Menu::create ( pMainMenu, pClose, nullptr );
        pMenu->setPosition ( Point ( pDirector->getWinSize ( ).width * 0.5f, pDirector->getWinSize ( ).height * 0.3f ) );
        pMenu->alignItemsVertically ( );
        this->addChild ( pMenu, 1 );

        // success
        bRet = true;
    } while ( 0 );
    return bRet;
}

KDvoid GameOverScene::menuPopSceneCallback ( Object* pSender )
{
	Director::getInstance ( )->popScene ( );
}

KDvoid GameOverScene::menuCloseCallback ( Object* pSender )
{
	ArmatureDataManager::destoryInstance ( );
    SceneReader::getInstance ( )->purgeSceneReader ( );
	ActionManagerEx::purgeActionManager ( );
	GUIReader::shareReader ( )->purgeGUIReader ( );
	
    Director::getInstance ( )->end ( );
}

VictoryScene* VictoryScene::create ( KDvoid )
{
	VictoryScene*	pRet = new VictoryScene ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool VictoryScene::init ( KDvoid )
{
	KDbool			bRet = false;
    do
    {
        Director*	pDirector;
        CC_BREAK_IF ( !( pDirector = Director::getInstance ( ) ) );
        this->setContentSize ( pDirector->getWinSize ( ) );
        Sprite*		pSprite = Sprite::create ( "victory.png" );
        
        // position the sprite on the center of the screen
        pSprite->setPosition ( Point ( Director::getInstance ( )->getVisibleSize ( ).width * 0.5f, Director::getInstance ( )->getVisibleSize ( ).height * 0.5f ) );
        // add the sprite as a child to this layer
        addChild ( pSprite, -1 );
        
		// create menu for game victory
		MenuItemFont*	pBackMenu = MenuItemFont::create ( "Back To Menu", CC_CALLBACK_1 ( VictoryScene::menuPopSceneCallback, this ) );
		MenuItemFont*	pExit = MenuItemFont::create ( "Exit", CC_CALLBACK_1 ( VictoryScene::menuCloseCallback, this ) );
		Menu*			pMenu = Menu::create ( pBackMenu, pExit, nullptr );
		
		pMenu->alignItemsVertically ( );
		pMenu->setColor ( Color3B ( 0, 0, 0 ) );
		addChild ( pMenu, 3 );

        // success
        bRet = true;
    } while ( 0 );

    return bRet;
}

KDvoid VictoryScene::menuPopSceneCallback ( Object* pSender )
{
	Director::getInstance ( )->popScene ( );
}

KDvoid VictoryScene::menuCloseCallback ( Object* pSender )
{
	ArmatureDataManager::destoryInstance ( );
    SceneReader::getInstance ( )->purgeSceneReader ( );
	ActionManagerEx::purgeActionManager ( );
	GUIReader::shareReader ( )->purgeGUIReader ( );
	
    Director::getInstance ( )->end ( );
}