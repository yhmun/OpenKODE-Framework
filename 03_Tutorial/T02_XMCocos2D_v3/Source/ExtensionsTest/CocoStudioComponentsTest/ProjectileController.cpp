#include "ProjectileController.h"
#include "SceneController.h"
#include "EnemyController.h"

using namespace cocos2d;
using namespace cocostudio;

ProjectileController::ProjectileController(void)
{
    m_sName = "ProjectileController";
}

ProjectileController::~ProjectileController(void)
{
}

bool ProjectileController::init()
{
    return true;
}

void ProjectileController::onEnter()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    m_pOwner->setPosition( Point(origin.x+20, origin.y+winSize.height/2) );
	m_pOwner->setTag(3);
    auto com = m_pOwner->getParent()->getComponent("SceneController");
    ((SceneController*)com)->getProjectiles()->addObject(m_pOwner);
}

void ProjectileController::onExit()
{

}

void ProjectileController::update(float delta)
{
    auto com = m_pOwner->getParent()->getComponent("SceneController");
    auto _targets = ((SceneController*)com)->getTargets();
    
    auto projectile = dynamic_cast<Sprite*>(m_pOwner);
    auto projectileRect = Rect(
			projectile->getPosition().x - (projectile->getContentSize().width/2),
			projectile->getPosition().y - (projectile->getContentSize().height/2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

    auto targetsToDelete = Array::createWithCapacity(20);
    Object* jt = NULL;
    CCARRAY_FOREACH(_targets, jt)
    {
        auto target = dynamic_cast<Sprite*>(jt);
        auto targetRect = Rect(
            target->getPosition().x - (target->getContentSize().width/2),
            target->getPosition().y - (target->getContentSize().height/2),
            target->getContentSize().width,
            target->getContentSize().height);

        // if (Rect::RectIntersectsRect(projectileRect, targetRect))
        if (projectileRect.intersectsRect(targetRect))
        {
            targetsToDelete->addObject(target);
        }
    }
    
    CCARRAY_FOREACH(targetsToDelete, jt)
    {
        auto target = dynamic_cast<Sprite*>(jt);
        static_cast<EnemyController*>(target->getComponent("EnemyController"))->die();
    }
    
    bool isDied = targetsToDelete->count() > 0;
   
    if (isDied)
    {
        die();
    }
}

ProjectileController* ProjectileController::create(void)
{
    ProjectileController * pRet = new ProjectileController();
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

void freeFunction( Node *ignore )
{
    log("hello");
}

void ProjectileController::move(float flocationX, float flocationY)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    // Determinie offset of location to projectile
	float offX = flocationX - m_pOwner->getPosition().x;
	float offY = flocationY - m_pOwner->getPosition().y;

	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;

	// Ok to add now - we've double checked position
	

	// Determine where we wish to shoot the projectile to
	float realX = origin.x + winSize.width + (m_pOwner->getContentSize().width/2);
	float ratio = offY / offX;
	float realY = (realX * ratio) + m_pOwner->getPosition().y;
	Point realDest = Point(realX, realY);

	// Determine the length of how far we're shooting
	float offRealX = realX - m_pOwner->getPosition().x;
	float offRealY = realY - m_pOwner->getPosition().y;
	float length = kdSqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;

    auto callfunc = CallFuncN::create(
          CC_CALLBACK_1(
                SceneController::spriteMoveFinished,
                static_cast<SceneController*>( getOwner()->getParent()->getComponent("SceneController")
      ) ) );

	// Move projectile to actual endpoint
	m_pOwner->runAction(
          Sequence::create(
               MoveTo::create(realMoveDuration, realDest),
               callfunc,
               NULL)
          );
}

void ProjectileController::die()
{
    auto com = m_pOwner->getParent()->getComponent("SceneController");
    auto _projectiles = static_cast<SceneController*>(com)->getProjectiles();
    _projectiles->removeObject(m_pOwner);
    m_pOwner->removeFromParentAndCleanup(true);
}
