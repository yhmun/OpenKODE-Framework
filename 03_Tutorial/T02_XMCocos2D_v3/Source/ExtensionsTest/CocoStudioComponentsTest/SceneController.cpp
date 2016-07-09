#include "SceneController.h"
#include "ComponentsTestScene.h"
#include "PlayerController.h"
#include "GameOverScene.h"
#include "EnemyController.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;

SceneController::SceneController(void)
: _fAddTargetTime(0.0f)
, _fElapsedTime(0.0f)
, _targets(nullptr)
, _projectiles(nullptr)
{
    m_sName = "SceneController";
}

SceneController::~SceneController(void)
{
    CC_SAFE_RELEASE(_targets);
    CC_SAFE_RELEASE(_projectiles);
}

bool SceneController::init()
{
    return true;
}

void SceneController::onEnter()
{
    _fAddTargetTime = 1.0f;
    
    _targets = Array::createWithCapacity(10);
    _projectiles = Array::createWithCapacity(10);

    _targets->retain();
    _projectiles->retain();
   
    ((ComAudio*)(m_pOwner->getComponent("Audio")))->playBackgroundMusic("background-music-aac.wav", true);
    ((ComAttribute*)(m_pOwner->getComponent("ComAttribute")))->setInt("KillCount", 0);

}

void SceneController::onExit()
{
}

void SceneController::update(float delta)
{
//    return;
    _fElapsedTime += delta;
    if (_fElapsedTime > _fAddTargetTime)
    {
        addTarget();
        _fElapsedTime = 0.0f;
    }
}

SceneController* SceneController::create(void)
{
    SceneController * pRet = new SceneController();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void SceneController::addTarget()
{
	Sprite *target = Sprite::create("components/Target.png", Rect(0,0,27,40));
    m_pOwner->addChild(target, 1, 2);
    
    target->addComponent(EnemyController::create());
	target->setTag(2);
    _targets->addObject(target);
}

void SceneController::spriteMoveFinished(Node* sender)
{
	Sprite *sprite = (Sprite *)sender;
	m_pOwner->removeChild(sprite, true);
    
	if (sprite->getTag() == 2)  // target
	{
        _targets->removeObject(sprite);
		auto gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
		Director::getInstance()->replaceScene(gameOverScene);
	}
	else if (sprite->getTag() == 3) 
	{
        _projectiles->removeObject(sprite);
	}
    
}

void SceneController::increaseKillCount()
{
    int nProjectilesDestroyed = ((ComAttribute*)(m_pOwner->getComponent("ComAttribute")))->getInt("KillCount");
    
    ComAttribute *p = (ComAttribute*)(m_pOwner->getComponent("ComAttribute"));
    p->setInt("KillCount", ++nProjectilesDestroyed);

    if (nProjectilesDestroyed >= 5)
    {
            auto gameOverScene = GameOverScene::create();
            gameOverScene->getLayer()->getLabel()->setString("You Win!");
            Director::getInstance()->replaceScene(gameOverScene);
    }
}
