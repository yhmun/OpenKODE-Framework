#include "EnemyController.h"
#include "SceneController.h"

using namespace cocos2d;
using namespace cocostudio;

EnemyController::EnemyController(void)
{
    m_sName = "EnemyController";
}

EnemyController::~EnemyController(void)
{
}

bool EnemyController::init()
{
    return true;
}

void EnemyController::onEnter()
{
   // Determine where to spawn the target along the Y axis
	Size winSize = Director::getInstance()->getVisibleSize();
	float minY = getOwner()->getContentSize().height/2;
	float maxY = winSize.height -  getOwner()->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// kdSrand( TimGetTicks() );
	int actualY = ( kdRand() % rangeY ) + (int)minY;

	// Create the target slightly off-screen along the right edge,
	// and along a kdRandom position along the Y axis as calculated
	m_pOwner->setPosition(
		Point(winSize.width + (getOwner()->getContentSize().width/2), 
            Director::getInstance()->getVisibleOrigin().y + actualY) );
	

	// Determine speed of the target
	int minDuration = 2;
	int maxDuration = 4;
	int rangeDuration = maxDuration - minDuration;
	// kdSrand( TimGetTicks() );
	int actualDuration = ( kdRand() % rangeDuration ) + minDuration;

	// Create the actions
	FiniteTimeAction* actionMove = MoveTo::create( actualDuration,
                                            Point(0 - getOwner()->getContentSize().width/2, actualY) );
	FiniteTimeAction* actionMoveDone = CallFuncN::create(
                                         CC_CALLBACK_1(SceneController::spriteMoveFinished, static_cast<SceneController*>( getOwner()->getParent()->getComponent("SceneController") )));

	m_pOwner->runAction( Sequence::create(actionMove, actionMoveDone, nullptr) );
}

void EnemyController::onExit()
{
}

void EnemyController::update(float delta)
{

}

EnemyController* EnemyController::create(void)
{
    EnemyController * pRet = new EnemyController();
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

void EnemyController::die()
{
    auto com = m_pOwner->getParent()->getComponent("SceneController");
    auto _targets = ((SceneController*)com)->getTargets();
    _targets->removeObject(m_pOwner);
    m_pOwner->removeFromParentAndCleanup(true);
    ((SceneController*)com)->increaseKillCount();
}

