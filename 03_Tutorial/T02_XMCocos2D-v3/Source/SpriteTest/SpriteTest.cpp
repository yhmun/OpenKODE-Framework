#include "SpriteTest.h"

#include <algorithm>
#include "../testResource.h"


enum 
{
    kTagTileMap = 1,
    kTagSpriteBatchNode = 1,
    kTagNode = 2,
    kTagAnimation1 = 1,
    kTagSpriteLeft,
    kTagSpriteRight,
};

enum 
{
    kTagSprite1,
    kTagSprite2,
    kTagSprite3,
    kTagSprite4,
    kTagSprite5,
    kTagSprite6,
    kTagSprite7,
    kTagSprite8,
};

enum
{
    IDC_NEXT = 100,
    IDC_BACK,
    IDC_RESTART
};

static int sceneIdx = -1; 


Layer* nextSpriteTestAction();
Layer* backSpriteTestAction();
Layer* restartSpriteTestAction();


static std::function<Layer*()> createFunctions[] =
{
	CL(Sprite1),
	CL(SpriteBatchNode1),
	CL(SpriteFrameTest),
	CL(SpriteFrameAliasNameTest),
	CL(SpriteAnchorPoint),
	CL(SpriteBatchNodeAnchorPoint),
	CL(SpriteOffsetAnchorRotation),
	CL(SpriteBatchNodeOffsetAnchorRotation),
	CL(SpriteOffsetAnchorScale),
	CL(SpriteBatchNodeOffsetAnchorScale),
	CL(SpriteOffsetAnchorSkew),
	CL(SpriteOffsetAnchorRotationalSkew),
	CL(SpriteBatchNodeOffsetAnchorSkew),
	CL(SpriteBatchNodeOffsetAnchorRotationalSkew),
	CL(SpriteOffsetAnchorSkewScale),
	CL(SpriteOffsetAnchorRotationalSkewScale),
	CL(SpriteBatchNodeOffsetAnchorSkewScale),
	CL(SpriteBatchNodeOffsetAnchorRotationalSkewScale),
	CL(SpriteOffsetAnchorFlip),
	CL(SpriteBatchNodeOffsetAnchorFlip),
	CL(SpriteAnimationSplit),
	CL(SpriteColorOpacity),
	CL(SpriteBatchNodeColorOpacity),
	CL(SpriteZOrder),
	CL(SpriteBatchNodeZOrder),
	CL(SpriteBatchNodeReorder),
	CL(SpriteBatchNodeReorderIssue744),
	CL(SpriteBatchNodeReorderIssue766),
	CL(SpriteBatchNodeReorderIssue767),
	CL(SpriteBatchNodeReorderSameIndex),
	CL(SpriteBatchNodeReorderOneChild),
	CL(NodeSort),
	CL(SpriteZVertex),
	CL(SpriteBatchNodeZVertex),
	CL(Sprite6),
	CL(SpriteFlip),
	CL(SpriteBatchNodeFlip),
	CL(SpriteAliased),
	CL(SpriteBatchNodeAliased),
	CL(SpriteNewTexture),
	CL(SpriteBatchNodeNewTexture),
	CL(SpriteHybrid),
	CL(SpriteBatchNodeChildren),
	CL(SpriteBatchNodeChildrenZ),
	CL(SpriteChildrenVisibility),
	CL(SpriteChildrenVisibilityIssue665),
	CL(SpriteChildrenAnchorPoint),
	CL(SpriteBatchNodeChildrenAnchorPoint),
	CL(SpriteBatchNodeChildrenScale),
	CL(SpriteChildrenChildren),
	CL(SpriteBatchNodeChildrenChildren),
	CL(SpriteSkewNegativeScaleChildren),
	CL(SpriteRotationalSkewNegativeScaleChildren),
	CL(SpriteBatchNodeSkewNegativeScaleChildren),
	CL(SpriteBatchNodeRotationalSkewNegativeScaleChildren),
	CL(SpriteNilTexture),
	CL(SpriteSubclass),
	CL(SpriteDoubleResolution),
	CL(SpriteBatchBug1217),
	CL(AnimationCacheTest),
	CL(AnimationCacheFile),
};

#define MAX_LAYER    (sizeof(createFunctions) / sizeof(createFunctions[0]))

Layer* nextSpriteTestAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;

    auto layer = (createFunctions[sceneIdx])();
    layer->autorelease();

    return layer;
}

Layer* backSpriteTestAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;    
    
    auto layer = (createFunctions[sceneIdx])();
    layer->autorelease();

    return layer;
}

Layer* restartSpriteTestAction()
{
    auto layer = (createFunctions[sceneIdx])();
    layer->autorelease();

    return layer;
} 

//------------------------------------------------------------------
//
// SpriteTestDemo
//
//------------------------------------------------------------------

SpriteTestDemo::SpriteTestDemo(void)
: BaseTest()
{
}

SpriteTestDemo::~SpriteTestDemo(void)
{
}

std::string SpriteTestDemo::title()
{
    return "No title";
}

std::string SpriteTestDemo::subtitle()
{
    return "";
}

void SpriteTestDemo::onEnter()
{
    BaseTest::onEnter();  
}

void SpriteTestDemo::restartCallback(Object* sender)
{
    auto s = new SpriteTestScene();
    s->addChild(restartSpriteTestAction()); 

    Director::getInstance()->replaceScene(s);
    s->release();
}

void SpriteTestDemo::nextCallback(Object* sender)
{
    auto s = new SpriteTestScene();
    s->addChild( nextSpriteTestAction() );
    Director::getInstance()->replaceScene(s);
    s->release();
}

void SpriteTestDemo::backCallback(Object* sender)
{
    auto s = new SpriteTestScene();
    s->addChild( backSpriteTestAction() );
    Director::getInstance()->replaceScene(s);
    s->release();
} 


//------------------------------------------------------------------
//
// Sprite1
//
//------------------------------------------------------------------

Sprite1::Sprite1()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(Sprite1::onTouchesEnded, this);
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto s = Director::getInstance()->getWinSize();
    addNewSpriteWithCoords( Point(s.width/2, s.height/2) );
}

void Sprite1::addNewSpriteWithCoords(Point p)
{
    int idx = (int)(CCRANDOM_0_1() * 1400.0f / 100.0f);
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(x,y,85,121) );
    addChild( sprite );
    
    sprite->setPosition( Point( p.x, p.y) );
    
    ActionInterval* action;
    float kdRandom = CCRANDOM_0_1();
    
    if( kdRandom < 0.20 )
        action = ScaleBy::create(3, 2);
    else if(kdRandom < 0.40)
        action = RotateBy::create(3, 360);
    else if( kdRandom < 0.60)
        action = Blink::create(1, 3);
    else if( kdRandom < 0.8 )
        action = TintBy::create(2, 0, -255, -255);
    else 
        action = FadeOut::create(2);
    auto action_back = action->reverse();
    auto seq = Sequence::create( action, action_back, NULL );
    
    sprite->runAction( RepeatForever::create(seq) );
}

void Sprite1::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    for (auto touch: touches)
    {
        auto location = touch->getLocation();
    
        addNewSpriteWithCoords( location );
    }
}

std::string Sprite1::title()
{
    return "Sprite (tap screen)";
}

//------------------------------------------------------------------
//
// SpriteBatchNode1
//
//------------------------------------------------------------------

SpriteBatchNode1::SpriteBatchNode1()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteBatchNode1::onTouchesEnded, this);
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto BatchNode = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 50);
    addChild(BatchNode, 0, kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();
    addNewSpriteWithCoords( Point(s.width/2, s.height/2) );
}

void SpriteBatchNode1::addNewSpriteWithCoords(Point p)
{
    auto BatchNode = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    
    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    

    auto sprite = Sprite::createWithTexture(BatchNode->getTexture(), Rect(x,y,85,121));
    BatchNode->addChild(sprite);

    sprite->setPosition( Point( p.x, p.y) );

    ActionInterval* action;
    float kdRandom = CCRANDOM_0_1();
    
    if( kdRandom < 0.20 )
        action = ScaleBy::create(3, 2);
    else if(kdRandom < 0.40)
        action = RotateBy::create(3, 360);
    else if( kdRandom < 0.60)
        action = Blink::create(1, 3);
    else if( kdRandom < 0.8 )
        action = TintBy::create(2, 0, -255, -255);
    else 
        action = FadeOut::create(2);

    auto action_back = action->reverse();
    auto seq = Sequence::create(action, action_back, NULL);
    
    sprite->runAction( RepeatForever::create(seq));
}

void SpriteBatchNode1::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    for (auto &touch: touches)
    {
        auto location = touch->getLocation();
            
        addNewSpriteWithCoords( location );
    }

}

std::string SpriteBatchNode1::title()
{
    return "SpriteBatchNode (tap screen)";
}


//------------------------------------------------------------------
//
// SpriteColorOpacity
//
//------------------------------------------------------------------

SpriteColorOpacity::SpriteColorOpacity()
{
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    auto sprite3 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*2, 121*1, 85, 121));
    auto sprite4 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*1, 85, 121));
    
    auto sprite5 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
    auto sprite6 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    auto sprite7 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*2, 121*1, 85, 121));
    auto sprite8 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*1, 85, 121));
    
    auto s = Director::getInstance()->getWinSize();
    sprite1->setPosition( Point( (s.width/5)*1, (s.height/3)*1) );
    sprite2->setPosition( Point( (s.width/5)*2, (s.height/3)*1) );
    sprite3->setPosition( Point( (s.width/5)*3, (s.height/3)*1) );
    sprite4->setPosition( Point( (s.width/5)*4, (s.height/3)*1) );
    sprite5->setPosition( Point( (s.width/5)*1, (s.height/3)*2) );
    sprite6->setPosition( Point( (s.width/5)*2, (s.height/3)*2) );
    sprite7->setPosition( Point( (s.width/5)*3, (s.height/3)*2) );
    sprite8->setPosition( Point( (s.width/5)*4, (s.height/3)*2) );
    
    auto action = FadeIn::create(2);
    auto action_back = action->reverse();
    auto fade = RepeatForever::create( Sequence::create( action, action_back, NULL) );
    
    auto tintred = TintBy::create(2, 0, -255, -255);
    auto tintred_back = tintred->reverse();
    auto red = RepeatForever::create( Sequence::create( tintred, tintred_back, NULL) );
    
    auto tintgreen = TintBy::create(2, -255, 0, -255);
    auto tintgreen_back = tintgreen->reverse();
    auto green = RepeatForever::create( Sequence::create( tintgreen, tintgreen_back, NULL) );
    
    auto tintblue = TintBy::create(2, -255, -255, 0);
    auto tintblue_back = tintblue->reverse();
    auto blue = RepeatForever::create( Sequence::create( tintblue, tintblue_back, NULL) );
    
    sprite5->runAction(red);
    sprite6->runAction(green);
    sprite7->runAction(blue);
    sprite8->runAction(fade);
    
    // late add: test dirtyColor and dirtyPosition
    addChild(sprite1, 0, kTagSprite1);
    addChild(sprite2, 0, kTagSprite2);
    addChild(sprite3, 0, kTagSprite3);
    addChild(sprite4, 0, kTagSprite4);
    addChild(sprite5, 0, kTagSprite5);
    addChild(sprite6, 0, kTagSprite6);
    addChild(sprite7, 0, kTagSprite7);
    addChild(sprite8, 0, kTagSprite8);
    
    schedule( schedule_selector(SpriteColorOpacity::removeAndAddSprite), 2 );
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
void SpriteColorOpacity::removeAndAddSprite(float dt)
{
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite5) );
    sprite->retain();
    
    removeChild(sprite, false);
    addChild(sprite, 0, kTagSprite5);
    
    sprite->release();
}

std::string SpriteColorOpacity::title()
{
    return "Sprite: Color & Opacity";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeColorOpacity
//
//------------------------------------------------------------------

SpriteBatchNodeColorOpacity::SpriteBatchNodeColorOpacity()
{
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto sprite1 = Sprite::createWithTexture(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
    auto sprite2 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    auto sprite3 = Sprite::createWithTexture(batch->getTexture(), Rect(85*2, 121*1, 85, 121));
    auto sprite4 = Sprite::createWithTexture(batch->getTexture(), Rect(85*3, 121*1, 85, 121));
    
    auto sprite5 = Sprite::createWithTexture(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
    auto sprite6 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    auto sprite7 = Sprite::createWithTexture(batch->getTexture(), Rect(85*2, 121*1, 85, 121));
    auto sprite8 = Sprite::createWithTexture(batch->getTexture(), Rect(85*3, 121*1, 85, 121));
    
    
    auto s = Director::getInstance()->getWinSize();
    sprite1->setPosition( Point( (s.width/5)*1, (s.height/3)*1) );
    sprite2->setPosition( Point( (s.width/5)*2, (s.height/3)*1) );
    sprite3->setPosition( Point( (s.width/5)*3, (s.height/3)*1) );
    sprite4->setPosition( Point( (s.width/5)*4, (s.height/3)*1) );
    sprite5->setPosition( Point( (s.width/5)*1, (s.height/3)*2) );
    sprite6->setPosition( Point( (s.width/5)*2, (s.height/3)*2) );
    sprite7->setPosition( Point( (s.width/5)*3, (s.height/3)*2) );
    sprite8->setPosition( Point( (s.width/5)*4, (s.height/3)*2) );

    auto action = FadeIn::create(2);
    auto action_back = action->reverse();
    auto fade = RepeatForever::create( Sequence::create( action, action_back,NULL) );

    auto tintred = TintBy::create(2, 0, -255, -255);
    auto tintred_back = tintred->reverse();
    auto red = RepeatForever::create( Sequence::create( tintred, tintred_back,NULL) );

    auto tintgreen = TintBy::create(2, -255, 0, -255);
    auto tintgreen_back = tintgreen->reverse();
    auto green = RepeatForever::create( Sequence::create( tintgreen, tintgreen_back,NULL) );

    auto tintblue = TintBy::create(2, -255, -255, 0);
    auto tintblue_back = tintblue->reverse();
    auto blue = RepeatForever::create( Sequence::create( tintblue, tintblue_back,NULL) );
    
    
    sprite5->runAction(red);
    sprite6->runAction(green);
    sprite7->runAction(blue);
    sprite8->runAction(fade);
    
    // late add: test dirtyColor and dirtyPosition
    batch->addChild(sprite1, 0, kTagSprite1);
    batch->addChild(sprite2, 0, kTagSprite2);
    batch->addChild(sprite3, 0, kTagSprite3);
    batch->addChild(sprite4, 0, kTagSprite4);
    batch->addChild(sprite5, 0, kTagSprite5);
    batch->addChild(sprite6, 0, kTagSprite6);
    batch->addChild(sprite7, 0, kTagSprite7);
    batch->addChild(sprite8, 0, kTagSprite8);
    
    
    schedule( schedule_selector(SpriteBatchNodeColorOpacity::removeAndAddSprite), 2);
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
void SpriteBatchNodeColorOpacity::removeAndAddSprite(float dt)
{
    auto batch= static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    auto sprite = static_cast<Sprite*>( batch->getChildByTag(kTagSprite5) );
    
    sprite->retain();

    batch->removeChild(sprite, false);
    batch->addChild(sprite, 0, kTagSprite5);
    
    sprite->release();
}

std::string SpriteBatchNodeColorOpacity::title()
{
    return "SpriteBatchNode: Color & Opacity";
}

//------------------------------------------------------------------
//
// SpriteZOrder
//
//------------------------------------------------------------------

SpriteZOrder::SpriteZOrder()
{
    _dir = 1;
            
    auto s = Director::getInstance()->getWinSize();
    
    float step = s.width/11;
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        addChild(sprite, i);
    }
    
    for(int i=5;i<10;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        addChild(sprite, 14-i);
    }
    
    auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*0, 85, 121));
    addChild(sprite, -1, kTagSprite1);
    sprite->setPosition( Point(s.width/2, s.height/2 - 20) );
    sprite->setScaleX( 6 );
    sprite->setColor(Color3B::RED);
    
    schedule( schedule_selector(SpriteZOrder::reorderSprite), 1);        
}

void SpriteZOrder::reorderSprite(float dt)
{
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    
    int z = sprite->getZOrder();
    
    if( z < -1 )
        _dir = 1;
    if( z > 10 )
        _dir = -1;
    
    z += _dir * 3;
    
    reorderChild(sprite, z);
    
}

std::string SpriteZOrder::title()
{
    return "Sprite: Z order";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZOrder
//
//------------------------------------------------------------------

SpriteBatchNodeZOrder::SpriteBatchNodeZOrder()
{
    _dir = 1;
    
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto s = Director::getInstance()->getWinSize();

    float step = s.width/11;
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        batch->addChild(sprite, i);
    }
    
    for(int i=5;i<10;i++) 
    {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        batch->addChild(sprite, 14-i);
    }
    
    auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*3, 121*0, 85, 121));
    batch->addChild(sprite, -1, kTagSprite1);
    sprite->setPosition( Point(s.width/2, s.height/2 - 20) );
    sprite->setScaleX( 6 );
    sprite->setColor(Color3B::RED);
    
    schedule( schedule_selector(SpriteBatchNodeZOrder::reorderSprite), 1);        
}

void SpriteBatchNodeZOrder::reorderSprite(float dt)
{
    auto batch= static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode ));
    auto sprite = static_cast<Sprite*>(batch->getChildByTag(kTagSprite1));
    
    int z = sprite->getZOrder();
    
    if( z < -1 )
        _dir = 1;
    if( z > 10 )
        _dir = -1;
    
    z += _dir * 3;

    batch->reorderChild(sprite, z);
}

std::string SpriteBatchNodeZOrder::title()
{
    return "SpriteBatchNode: Z order";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorder
//
//------------------------------------------------------------------

SpriteBatchNodeReorder::SpriteBatchNodeReorder()
{
    auto a = Array::createWithCapacity(10);
    auto asmtest = SpriteBatchNode::create("animations/ghosts.png");
    
    for(int i=0; i<10; i++)
    {
        auto s1 = Sprite::createWithTexture(asmtest->getTexture(), Rect(0, 0, 50, 50));
        a->addObject(s1);
        asmtest->addChild(s1, 10);
    }
    
    for(int i=0; i<10; i++)
    {
        if(i!=5)
        {
            asmtest->reorderChild( static_cast<Node*>(a->getObjectAtIndex(i)), 9 );
        }
    }
    
    int CC_UNUSED prev = -1;
    auto children = asmtest->getChildren();
    Sprite* child;
    Object* pObject = NULL;
    CCARRAY_FOREACH(children, pObject)
    {
        child = static_cast<Sprite*>(pObject);
        if(! child )
            break;

        int currentIndex = child->getAtlasIndex();
        CCASSERT( prev == currentIndex-1, "Child order failed");
        ////----CCLOG("children %x - atlasIndex:%d", child, currentIndex);
        prev = currentIndex;
    }
    
    prev = -1;
    auto descendants = asmtest->getDescendants();
    std::for_each(descendants.begin(), descendants.end(), [&](Sprite* sprite) {
        int currentIndex = sprite->getAtlasIndex();
        CCASSERT( prev == currentIndex-1, "Child order failed");
        ////----CCLOG("descendant %x - atlasIndex:%d", child, currentIndex);
        prev = currentIndex;
    });
}

std::string SpriteBatchNodeReorder::title()
{
    return "SpriteBatchNode: reorder #1";
}

std::string SpriteBatchNodeReorder::subtitle()
{
    return "Should not crash";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue744
//
//------------------------------------------------------------------

SpriteBatchNodeReorderIssue744::SpriteBatchNodeReorderIssue744()
{
    auto s = Director::getInstance()->getWinSize();
    

    // Testing issue #744
    // http://code.google.com/p/cocos2d-iphone/issues/detail?id=744
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
    addChild(batch, 0, kTagSpriteBatchNode);        

    auto sprite = Sprite::createWithTexture(batch->getTexture(),Rect(0, 0, 85, 121));
    sprite->setPosition( Point(s.width/2, s.height/2) );
    batch->addChild(sprite, 3);
    batch->reorderChild(sprite, 1);
}

std::string SpriteBatchNodeReorderIssue744::title()
{
    return "SpriteBatchNode: reorder issue #744";
}

std::string SpriteBatchNodeReorderIssue744::subtitle()
{
    return "Should not crash";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue766
//
//------------------------------------------------------------------

Sprite* SpriteBatchNodeReorderIssue766::makeSpriteZ(int aZ)
{
    auto sprite = Sprite::createWithTexture(batchNode->getTexture(), Rect(128,0,64,64));
    batchNode->addChild(sprite, aZ+1, 0);

    //children
    auto spriteShadow = Sprite::createWithTexture(batchNode->getTexture(), Rect(0,0,64,64));
    spriteShadow->setOpacity(128);
    sprite->addChild(spriteShadow, aZ, 3);

    auto spriteTop = Sprite::createWithTexture(batchNode->getTexture(), Rect(64,0,64,64));
    sprite->addChild(spriteTop, aZ+2, 3);

    return sprite;
}

void SpriteBatchNodeReorderIssue766::reorderSprite(float dt)
{
    unschedule(schedule_selector(SpriteBatchNodeReorderIssue766::reorderSprite));

    batchNode->reorderChild(sprite1, 4);
}

// on "init" you need to initialize your instance
SpriteBatchNodeReorderIssue766::SpriteBatchNodeReorderIssue766()
{
    batchNode = SpriteBatchNode::create("Images/piece.png", 15);
    addChild(batchNode, 1, 0);

    sprite1 = makeSpriteZ(2);
    sprite1->setPosition(Point(200,160));

    sprite2 = makeSpriteZ(3);
    sprite2->setPosition(Point(264,160));

    sprite3 = makeSpriteZ(4);
    sprite3->setPosition(Point(328,160));

    schedule(schedule_selector(SpriteBatchNodeReorderIssue766::reorderSprite), 2);
}

std::string SpriteBatchNodeReorderIssue766::title()
{
    return "SpriteBatchNode: reorder issue #766";
}

std::string SpriteBatchNodeReorderIssue766::subtitle()
{
    return "In 2 seconds 1 sprite will be reordered";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue767
//
//------------------------------------------------------------------
SpriteBatchNodeReorderIssue767::SpriteBatchNodeReorderIssue767()
{
    auto s = Director::getInstance()->getWinSize();        

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/ghosts.plist", "animations/ghosts.png");
    Node *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;

    //
    // SpriteBatchNode: 3 levels of children
    //
    aParent = SpriteBatchNode::create("animations/ghosts.png");
    addChild(aParent, 0, kTagSprite1);

    // parent
    l1 = Sprite::createWithSpriteFrameName("father.gif");
    l1->setPosition(Point( s.width/2, s.height/2));
    aParent->addChild(l1, 0, kTagSprite2);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition(Point( -25 + l1Size.width/2, 0 + l1Size.height/2));
    l1->addChild(l2a, -1, kTagSpriteLeft);
    auto l2aSize = l2a->getContentSize();        


    // child right
    l2b = Sprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition(Point( +25 + l1Size.width/2, 0 + l1Size.height/2));
    l1->addChild(l2b, 1, kTagSpriteRight);
    auto l2bSize = l2a->getContentSize();


    // child left bottom
    l3a1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale(0.65f);
    l3a1->setPosition(Point(0+l2aSize.width/2,-50+l2aSize.height/2));
    l2a->addChild(l3a1, -1);

    // child left top
    l3a2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale(0.65f);
    l3a2->setPosition(Point(0+l2aSize.width/2,+50+l2aSize.height/2));
    l2a->addChild(l3a2, 1);

    // child right bottom
    l3b1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale(0.65f);
    l3b1->setPosition(Point(0+l2bSize.width/2,-50+l2bSize.height/2));
    l2b->addChild(l3b1, -1);

    // child right top
    l3b2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale(0.65f);
    l3b2->setPosition(Point(0+l2bSize.width/2,+50+l2bSize.height/2));
    l2b->addChild(l3b2, 1);

    schedule(schedule_selector(SpriteBatchNodeReorderIssue767::reorderSprites), 1);
}

std::string SpriteBatchNodeReorderIssue767::title()
{
    return "SpriteBatchNode: reorder issue #767";
}

std::string SpriteBatchNodeReorderIssue767::subtitle()
{
    return "Should not crash";
}

void SpriteBatchNodeReorderIssue767::reorderSprites(float dt)
{
    auto spritebatch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSprite1) );
    auto father = static_cast<Sprite*>( spritebatch->getChildByTag(kTagSprite2) );
    auto left = static_cast<Sprite*>( father->getChildByTag(kTagSpriteLeft) );
    auto right = static_cast<Sprite*>( father->getChildByTag(kTagSpriteRight) );

    int newZLeft = 1;

    if( left->getZOrder() == 1 )
        newZLeft = -1;

    father->reorderChild(left, newZLeft);
    father->reorderChild(right, -newZLeft);
}

//------------------------------------------------------------------
//
// SpriteZVertex
//
//------------------------------------------------------------------

void SpriteZVertex::onEnter()
{
    SpriteTestDemo::onEnter();
    
    Director::getInstance()->setProjection(Director::Projection::_3D);
}

void SpriteZVertex::onExit()
{
    Director::getInstance()->setProjection(Director::Projection::_2D);
    SpriteTestDemo::onExit();
}

SpriteZVertex::SpriteZVertex()
{
    //
    // This test tests z-order
    // If you are going to use it is better to use a 3D projection
    //
    // WARNING:
    // The developer is resposible for ordering its sprites according to its Z if the sprite has
    // transparent parts.
    //

    //
    // Configure shader to mimic glAlphaTest
    //
    auto alphaTestShader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);
    GLint alphaValueLocation = glGetUniformLocation(alphaTestShader->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

    // set alpha test value
    // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
    if (getShaderProgram())
    {
        getShaderProgram()->setUniformLocationWith1f(alphaValueLocation, 0.0f);
    }
    
    
    _dir = 1;
    _time = 0;

    auto s = Director::getInstance()->getWinSize();
    float step = s.width/12;
    
    auto node = Node::create();
    // camera uses the center of the image as the pivoting point
    node->setContentSize( Size(s.width,s.height) );
    node->setAnchorPoint( Point::ANCHOR_MIDDLE);
    node->setPosition( Point(s.width/2, s.height/2));

    addChild(node, 0);

    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Point((i+1)*step, s.height/2) );
        sprite->setVertexZ( 10 + i*40 );
        sprite->setShaderProgram(alphaTestShader);
        node->addChild(sprite, 0);
        
    }
    
    for(int i=5;i<11;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        sprite->setVertexZ( 10 + (10-i)*40 );
        sprite->setShaderProgram(alphaTestShader);
        node->addChild(sprite, 0);
    }

    node->runAction( OrbitCamera::create(10, 1, 0, 0, 360, 0, 0) );
}

std::string SpriteZVertex::title()
{
    return "Sprite: openGL Z vertex";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZVertex
//
//------------------------------------------------------------------

void SpriteBatchNodeZVertex::onEnter()
{
    SpriteTestDemo::onEnter();

    Director::getInstance()->setProjection(Director::Projection::_3D);
}

void SpriteBatchNodeZVertex::onExit()
{
    Director::getInstance()->setProjection(Director::Projection::_2D);
    SpriteTestDemo::onExit();
}

SpriteBatchNodeZVertex::SpriteBatchNodeZVertex()
{
    //
    // This test tests z-order
    // If you are going to use it is better to use a 3D projection
    //
    // WARNING:
    // The developer is resposible for ordering its sprites according to its Z if the sprite has
    // transparent parts.
    //

    //
    // Configure shader to mimic glAlphaTest
    //
    auto alphaTestShader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);
    GLint alphaValueLocation = glGetUniformLocation(alphaTestShader->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

    // set alpha test value
    // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
    if (getShaderProgram())
    {
        getShaderProgram()->setUniformLocationWith1f(alphaValueLocation, 0.0f);
    }
    
    auto s = Director::getInstance()->getWinSize();
    float step = s.width/12;
    
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    // camera uses the center of the image as the pivoting point
    batch->setContentSize( Size(s.width,s.height));
    batch->setAnchorPoint( Point::ANCHOR_MIDDLE);
    batch->setPosition( Point(s.width/2, s.height/2));
    
    batch->setShaderProgram(alphaTestShader);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        sprite->setVertexZ(  10 + i*40 );
        batch->addChild(sprite, 0);
        
    }
    
    for(int i=5;i<11;i++) {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );
        sprite->setVertexZ(  10 + (10-i)*40 );
        batch->addChild(sprite, 0);
    }
    
    batch->runAction(OrbitCamera::create(10, 1, 0, 0, 360, 0, 0) );
}

std::string SpriteBatchNodeZVertex::title()
{
    return "SpriteBatchNode: openGL Z vertex";
}


//------------------------------------------------------------------
//
// SpriteAnchorPoint
//
//------------------------------------------------------------------

SpriteAnchorPoint::SpriteAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    
    auto rotate = RotateBy::create(10, 360);
    auto action = RepeatForever::create(rotate);
    
    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*i, 121*1, 85, 121) );
        sprite->setPosition( Point( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 10);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );

        sprite->runAction( action->clone() );
        addChild(sprite, i);
    }        
}

std::string SpriteAnchorPoint::title()
{
    return "Sprite: anchor point";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeAnchorPoint
//
//------------------------------------------------------------------
SpriteBatchNodeAnchorPoint::SpriteBatchNodeAnchorPoint()
{
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto s = Director::getInstance()->getWinSize();
    
    
    auto rotate = RotateBy::create(10, 360);
    auto action = RepeatForever::create(rotate);
    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*i, 121*1, 85, 121));
        sprite->setPosition( Point( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);

        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }

        point->setPosition( sprite->getPosition() );

        sprite->runAction( action->clone() );
        batch->addChild(sprite, i);
    }
}

std::string SpriteBatchNodeAnchorPoint::title()
{
    return "SpriteBatchNode: anchor point";
}

//------------------------------------------------------------------
//
// Sprite6
//
//------------------------------------------------------------------

Sprite6::Sprite6()
{
    // small capacity. Testing resizing
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);
    batch->ignoreAnchorPointForPosition( true );

    auto s = Director::getInstance()->getWinSize();

    batch->setAnchorPoint( Point::ANCHOR_MIDDLE );
    batch->setContentSize( Size(s.width, s.height) );
    
    
    // SpriteBatchNode actions
    auto rotate = RotateBy::create(5, 360);
    auto action = RepeatForever::create(rotate);

    // SpriteBatchNode actions
    auto rotate_back = rotate->reverse();
    auto rotate_seq = Sequence::create(rotate, rotate_back, NULL);
    auto rotate_forever = RepeatForever::create(rotate_seq);
    
    auto scale = ScaleBy::create(5, 1.5f);
    auto scale_back = scale->reverse();
    auto scale_seq = Sequence::create( scale, scale_back, NULL);
    auto scale_forever = RepeatForever::create(scale_seq);

    float step = s.width/4;

    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(85*i, 121*1, 85, 121));
        sprite->setPosition( Point( (i+1)*step, s.height/2) );

        sprite->runAction( action->clone());
        batch->addChild(sprite, i);
    }
    
    batch->runAction(scale_forever);
    batch->runAction(rotate_forever);
}

std::string Sprite6::title()
{
    return "SpriteBatchNode transformation";
}

//------------------------------------------------------------------
//
// SpriteFlip
//
//------------------------------------------------------------------
SpriteFlip::SpriteFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Point( s.width/2 - 100, s.height/2 ) );
    addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Point( s.width/2 + 100, s.height/2 ) );
    addChild(sprite2, 0, kTagSprite2);
    
    schedule( schedule_selector(SpriteFlip::flipSprites), 1);
}

void SpriteFlip::flipSprites(float dt)
{
    auto sprite1 = static_cast<Sprite*>(getChildByTag(kTagSprite1));
    auto sprite2 = static_cast<Sprite*>(getChildByTag(kTagSprite2));
    
    bool x = sprite1->isFlippedX();
    bool y = sprite2->isFlippedY();
    
    CCLOG("Pre: %f", sprite1->getContentSize().height);
    sprite1->setFlippedX(!x);
    sprite2->setFlippedY(!y);
    CCLOG("Post: %f", sprite1->getContentSize().height);
}

std::string SpriteFlip::title()
{
    return "Sprite Flip X & Y";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeFlip
//
//------------------------------------------------------------------
SpriteBatchNodeFlip::SpriteBatchNodeFlip()
{
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 10);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Point( s.width/2 - 100, s.height/2 ) );
    batch->addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Point( s.width/2 + 100, s.height/2 ) );
    batch->addChild(sprite2, 0, kTagSprite2);
    
    schedule( schedule_selector(SpriteBatchNodeFlip::flipSprites), 1);
}

void SpriteBatchNodeFlip::flipSprites(float dt)
{
    auto batch= static_cast<SpriteBatchNode*>(getChildByTag( kTagSpriteBatchNode ));
    auto sprite1 = static_cast<Sprite*>(batch->getChildByTag(kTagSprite1));
    auto sprite2 = static_cast<Sprite*>(batch->getChildByTag(kTagSprite2));
    
    bool x = sprite1->isFlippedX();
    bool y = sprite2->isFlippedY();
    
    CCLOG("Pre: %f", sprite1->getContentSize().height);
    sprite1->setFlippedX(!x);
    sprite2->setFlippedY(!y);
    CCLOG("Post: %f", sprite1->getContentSize().height);
}

std::string SpriteBatchNodeFlip::title()
{
    return "SpriteBatchNode Flip X & Y";
}

//------------------------------------------------------------------
//
// SpriteAliased
//
//------------------------------------------------------------------

SpriteAliased::SpriteAliased()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Point( s.width/2 - 100, s.height/2 ) );
    addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Point( s.width/2 + 100, s.height/2 ) );
    addChild(sprite2, 0, kTagSprite2);
    
    auto scale = ScaleBy::create(2, 5);
    auto scale_back = scale->reverse();
    auto seq = Sequence::create( scale, scale_back, NULL);
    auto repeat = RepeatForever::create(seq);
    
    auto repeat2 = repeat->clone();
    
    sprite1->runAction(repeat);
    sprite2->runAction(repeat2);
    
}

void SpriteAliased::onEnter()
{
    SpriteTestDemo::onEnter();
    
    //
    // IMPORTANT:
    // This change will affect every sprite that uses the same texture
    // So sprite1 and sprite2 will be affected by this change
    //
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    sprite->getTexture()->setAliasTexParameters();
}

void SpriteAliased::onExit()
{
    // restore the tex parameter to AntiAliased.
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    sprite->getTexture()->setAntiAliasTexParameters();
    SpriteTestDemo::onExit();
}

std::string SpriteAliased::title()
{
    return "Sprite Aliased";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeAliased
//
//------------------------------------------------------------------

SpriteBatchNodeAliased::SpriteBatchNodeAliased()
{
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 10);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();

    auto sprite1 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Point( s.width/2 - 100, s.height/2 ) );
    batch->addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::createWithTexture(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Point( s.width/2 + 100, s.height/2 ) );
    batch->addChild(sprite2, 0, kTagSprite2);
    
    auto scale = ScaleBy::create(2, 5);
    auto scale_back = scale->reverse();
    auto seq = Sequence::create( scale, scale_back, NULL);
    auto repeat = RepeatForever::create(seq);
    
    auto repeat2 = repeat->clone();
    
    sprite1->runAction(repeat);
    sprite2->runAction(repeat2);
        
}
void SpriteBatchNodeAliased::onEnter()
{
    SpriteTestDemo::onEnter();
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    batch->getTexture()->setAliasTexParameters();
}

void SpriteBatchNodeAliased::onExit()
{
    // restore the tex parameter to AntiAliased.
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    batch->getTexture()->setAntiAliasTexParameters();
    SpriteTestDemo::onExit();
}

std::string SpriteBatchNodeAliased::title()
{
    return "SpriteBatchNode Aliased";
}

//------------------------------------------------------------------
//
// SpriteNewTexture
//
//------------------------------------------------------------------

SpriteNewTexture::SpriteNewTexture()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteNewTexture::onTouchesEnded, this);
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto node = Node::create();
    addChild(node, 0, kTagSpriteBatchNode);

    _texture1 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas.png");
    _texture1->retain();
    _texture2 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas-mono.png");
    _texture2->retain();
    
    _usingTexture1 = true;

    for(int i=0;i<30;i++)
        addNewSprite();
}

SpriteNewTexture::~SpriteNewTexture()
{
    _texture1->release();
    _texture2->release();
}

void SpriteNewTexture::addNewSprite()
{
    auto s = Director::getInstance()->getWinSize();

    auto p = Point( CCRANDOM_0_1() * s.width, CCRANDOM_0_1() * s.height);

    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto node = getChildByTag( kTagSpriteBatchNode );
    auto sprite = Sprite::createWithTexture(_texture1, Rect(x,y,85,121));
    node->addChild(sprite);
    
    sprite->setPosition( Point( p.x, p.y) );
    
    ActionInterval* action;
    float kdRandom = CCRANDOM_0_1();
    
    if( kdRandom < 0.20 )
        action = ScaleBy::create(3, 2);
    else if(kdRandom < 0.40)
        action = RotateBy::create(3, 360);
    else if( kdRandom < 0.60)
        action = Blink::create(1, 3);
    else if( kdRandom < 0.8 )
        action = TintBy::create(2, 0, -255, -255);
    else 
        action = FadeOut::create(2);

    auto action_back = action->reverse();
    auto seq = Sequence::create(action, action_back, NULL);
    
    sprite->runAction( RepeatForever::create(seq) );
}

void SpriteNewTexture::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{

    auto node = getChildByTag( kTagSpriteBatchNode );

    auto children = node->getChildren();
    Sprite* sprite;
    Object* pObject;

    if( _usingTexture1 )                          //--> win32 : Let's it make just simple sentence
    {
        CCARRAY_FOREACH(children, pObject)
        {
            sprite = static_cast<Sprite*>( pObject );
            if(! sprite)
                break;

            sprite->setTexture(_texture2);
        }

        _usingTexture1 = false;
    } 
    else 
    {
        CCARRAY_FOREACH(children, pObject)
        {
            sprite = static_cast<Sprite*>( pObject );
            if(! sprite)
                break;

            sprite->setTexture(_texture1);
        }

        _usingTexture1 = true;
    }
}

std::string SpriteNewTexture::title()
{
    return "Sprite New texture (tap)";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeNewTexture
//
//------------------------------------------------------------------

SpriteBatchNodeNewTexture::SpriteBatchNodeNewTexture()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteBatchNodeNewTexture::onTouchesEnded, this);
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    _texture1 = batch->getTexture(); _texture1->retain();
    _texture2 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas-mono.png");
    _texture2->retain();
    
    for(int i=0;i<30;i++)
        addNewSprite();
}

SpriteBatchNodeNewTexture::~SpriteBatchNodeNewTexture()
{
    _texture1->release();
    _texture2->release();
}

void SpriteBatchNodeNewTexture::addNewSprite()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto p = Point( CCRANDOM_0_1() * s.width, CCRANDOM_0_1() * s.height);
    
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode ) );
    
    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto sprite = Sprite::createWithTexture(batch->getTexture(), Rect(x,y,85,121));
    batch->addChild(sprite);
    
    sprite->setPosition( Point( p.x, p.y) );
    
    ActionInterval* action;
    float kdRandom = CCRANDOM_0_1();
    
    if( kdRandom < 0.20 )
        action = ScaleBy::create(3, 2);
    else if(kdRandom < 0.40)
        action = RotateBy::create(3, 360);
    else if( kdRandom < 0.60)
        action = Blink::create(1, 3);
    else if( kdRandom < 0.8 )
        action = TintBy::create(2, 0, -255, -255);
    else 
        action = FadeOut::create(2);
    auto action_back = action->reverse();
    auto seq = Sequence::create(action, action_back, NULL);
    
    sprite->runAction( RepeatForever::create(seq) );
}

void SpriteBatchNodeNewTexture::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode) );
    
    if( batch->getTexture() == _texture1 )
        batch->setTexture(_texture2);
    else
        batch->setTexture(_texture1);    
}

std::string SpriteBatchNodeNewTexture::title()
{
    return "SpriteBatchNode new texture (tap)";
}


//------------------------------------------------------------------
//
// SpriteFrameTest
//
//------------------------------------------------------------------

void SpriteFrameTest::onEnter()
{
    SpriteTestDemo::onEnter();
    auto s = Director::getInstance()->getWinSize();

    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     SpriteFrameCache::getInstance()->removeUnusedSpriteFrames);
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    cache->addSpriteFramesWithFile("animations/grossini_blue.plist", "animations/grossini_blue.png");

    //
    // Animation using Sprite BatchNode
    //
    _sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    _sprite1->setPosition( Point( s.width/2-80, s.height/2) );

    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    spritebatch->addChild(_sprite1);
    addChild(spritebatch);

    auto animFrames = Array::createWithCapacity(15);

    char str[100] = {0};
    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_%02d.png", i);
        auto frame = cache->getSpriteFrameByName( str );
        animFrames->addObject(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
    _sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );

    // to test issue #732, uncomment the following line
    _sprite1->setFlippedX(false);
    _sprite1->setFlippedY(false);

    //
    // Animation using standard Sprite
    //
    _sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    _sprite2->setPosition( Point( s.width/2 + 80, s.height/2) );
    addChild(_sprite2);


    auto moreFrames = Array::createWithCapacity(20);
    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_gray_%02d.png",i);
        auto frame = cache->getSpriteFrameByName(str);
        moreFrames->addObject(frame);
    }


    for( int i = 1; i < 5; i++) {
        sprintf(str, "grossini_blue_%02d.png",i);
        auto frame = cache->getSpriteFrameByName(str);
        moreFrames->addObject(frame);
    }

    // append frames from another batch
    moreFrames->addObjectsFromArray(animFrames);
    auto animMixed = Animation::createWithSpriteFrames(moreFrames, 0.3f);


    _sprite2->runAction(RepeatForever::create( Animate::create(animMixed) ) );


    // to test issue #732, uncomment the following line
    _sprite2->setFlippedX(false);
    _sprite2->setFlippedY(false);

    schedule(schedule_selector(SpriteFrameTest::startIn05Secs), 0.5f);
    _counter = 0;
}

void SpriteFrameTest::onExit()
{
    SpriteTestDemo::onExit();
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_blue.plist");
}

std::string SpriteFrameTest::title()
{
    return "Sprite vs. SpriteBatchNode animation";
}

std::string SpriteFrameTest::subtitle()
{
    return "Testing issue #792";
}

void SpriteFrameTest::startIn05Secs(float dt)
{
    unschedule(schedule_selector(SpriteFrameTest::startIn05Secs));
    schedule(schedule_selector(SpriteFrameTest::flipSprites), 1.0f);
}

void SpriteFrameTest::flipSprites(float dt)
{
    _counter++;

    bool fx = false;
    bool fy = false;
    int  i  = _counter % 4;

    switch ( i ) {
        case 0:
            fx = false;
            fy = false;
            break;
        case 1:
            fx = true;
            fy = false;
            break;
        case 2:
            fx = false;
            fy = true;
            break;
        case 3:
            fx = true;
            fy = true;
            break;
    }

    _sprite1->setFlippedX(fx);
    _sprite1->setFlippedY(fy);
    _sprite2->setFlippedX(fx);
    _sprite2->setFlippedY(fy);
    //NSLog(@"flipX:%d, flipY:%d", fx, fy);
}

//------------------------------------------------------------------
//
// SpriteFrameAliasNameTest
//
//------------------------------------------------------------------
void SpriteFrameAliasNameTest::onEnter()
{
    SpriteTestDemo::onEnter();
    auto s = Director::getInstance()->getWinSize();

    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    //
    // SpriteFrameCache is a cache of SpriteFrames
    // SpriteFrames each contain a texture id and a rect (frame).

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini-aliases.plist", "animations/grossini-aliases.png");

    //
    // Animation using Sprite batch
    //
    // A SpriteBatchNode can reference one and only one texture (one .png file)
    // Sprites that are contained in that texture can be instantiatied as Sprites and then added to the SpriteBatchNode
    // All Sprites added to a SpriteBatchNode are drawn in one OpenGL ES draw call
    // If the Sprites are not added to a SpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient
    //
    // When you animate a sprite, Animation changes the frame of the sprite using setDisplayFrame: (this is why the animation must be in the same texture)
    // When setDisplayFrame: is used in the Animation it changes the frame to one specified by the SpriteFrames that were added to the animation,
    // but texture id is still the same and so the sprite is still a child of the SpriteBatchNode, 
    // and therefore all the animation sprites are also drawn as part of the SpriteBatchNode
    //

    auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite->setPosition(Point(s.width * 0.5f, s.height * 0.5f));

    auto spriteBatch = SpriteBatchNode::create("animations/grossini-aliases.png");
    spriteBatch->addChild(sprite);
    addChild(spriteBatch);

    auto animFrames = Array::createWithCapacity(15);
    char str[100] = {0};
    for(int i = 1; i < 15; i++)
    {
        // Obtain frames by alias name
        sprintf(str, "dance_%02d", i);
        auto frame = cache->getSpriteFrameByName(str);
        animFrames->addObject(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
    // 14 frames * 1sec = 14 seconds
    sprite->runAction(RepeatForever::create(Animate::create(animation)));
}

void SpriteFrameAliasNameTest::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("animations/grossini-aliases.plist");
}

std::string SpriteFrameAliasNameTest::title()
{
    return "SpriteFrame Alias Name";
}

std::string SpriteFrameAliasNameTest::subtitle()
{
    return "SpriteFrames are obtained using the alias name";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorRotation
//
//------------------------------------------------------------------
SpriteOffsetAnchorRotation::SpriteOffsetAnchorRotation()
{
    auto s = Director::getInstance()->getWinSize();        
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point( s.width/4*(i+1), s.height/2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);

        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        auto animFrames = Array::createWithCapacity(14);
        char str[100] = {0};
        for(int i = 0; i < 14; i++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(i+1));
            auto frame = cache->getSpriteFrameByName(str);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create( Animate::create(animation) ) );            
        sprite->runAction(RepeatForever::create(RotateBy::create(10, 360) ) );

        addChild(sprite, 0);

    }        
}


void SpriteOffsetAnchorRotation::onExit()
{
    SpriteTestDemo::onExit();
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorRotation::title()
{
    return "Sprite offset + anchor + rot";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorRotation
//
//------------------------------------------------------------------

SpriteBatchNodeOffsetAnchorRotation::SpriteBatchNodeOffsetAnchorRotation()
{
    auto s = Director::getInstance()->getWinSize(); 
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( Point( s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        auto animFrames = Array::createWithCapacity(14);
        char str[100] = {0};
        for(int k = 0; k < 14; k++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(k+1));
            auto frame = cache->getSpriteFrameByName(str);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create( Animate::create(animation) ));
        sprite->runAction(RepeatForever::create(RotateBy::create(10, 360) ));
        
        spritebatch->addChild(sprite, i);
    }        
}


void SpriteBatchNodeOffsetAnchorRotation::onExit()
{
    SpriteTestDemo::onExit();
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorRotation::title()
{
    return "SpriteBatchNode offset + anchor + rot";
}


//------------------------------------------------------------------
//
// SpriteOffsetAnchorScale
//
//------------------------------------------------------------------


SpriteOffsetAnchorScale::SpriteOffsetAnchorScale()
{
    auto s = Director::getInstance()->getWinSize();   
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( Point( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        auto animFrames = Array::createWithCapacity(14);
        char str[100] = {0};
        for(int i = 0; i < 14; i++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(i+1));
            auto frame = cache->getSpriteFrameByName(str);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create( Animate::create(animation) ));            
        
        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale));
        
        addChild(sprite, 0);
    }        
}

void SpriteOffsetAnchorScale::onExit()
{
    SpriteTestDemo::onExit();
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorScale::title()
{
    return "Sprite offset + anchor + scale";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorScale
//
//------------------------------------------------------------------
SpriteBatchNodeOffsetAnchorScale::SpriteBatchNodeOffsetAnchorScale()
{
    auto s = Director::getInstance()->getWinSize(); 
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritesheet = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritesheet);
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition( Point( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) {
            case 0:
                sprite->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Point::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        auto animFrames = Array::createWithCapacity(14);
        char str[100] = {0};
        for(int k = 0; k < 14; k++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(k+1));
            auto frame = cache->getSpriteFrameByName(str);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create( Animate::create(animation) ) );

        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale) );
        
        spritesheet->addChild(sprite, i);
    }        
}

void SpriteBatchNodeOffsetAnchorScale::onExit()
{
    SpriteTestDemo::onExit();
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorScale::title()
{
    return "SpriteBatchNode offset + anchor + scale";
}

//------------------------------------------------------------------
//
// SpriteAnimationSplit
//
//------------------------------------------------------------------

SpriteAnimationSplit::SpriteAnimationSplit()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto texture = Director::getInstance()->getTextureCache()->addImage("animations/dragon_animation.png");
    
    // manually add frames to the frame cache
    auto frame0 = SpriteFrame::createWithTexture(texture, Rect(132*0, 132*0, 132, 132));
    auto frame1 = SpriteFrame::createWithTexture(texture, Rect(132*1, 132*0, 132, 132));
    auto frame2 = SpriteFrame::createWithTexture(texture, Rect(132*2, 132*0, 132, 132));
    auto frame3 = SpriteFrame::createWithTexture(texture, Rect(132*3, 132*0, 132, 132));
    auto frame4 = SpriteFrame::createWithTexture(texture, Rect(132*0, 132*1, 132, 132));
    auto frame5 = SpriteFrame::createWithTexture(texture, Rect(132*1, 132*1, 132, 132));
    
    
    //
    // Animation using Sprite BatchNode
    //
    auto sprite = Sprite::createWithSpriteFrame(frame0);
    sprite->setPosition( Point( s.width/2-80, s.height/2) );
    addChild(sprite);
            
    auto animFrames = Array::createWithCapacity(6);
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    animFrames->addObject(frame2);
    animFrames->addObject(frame3);
    animFrames->addObject(frame4);
    animFrames->addObject(frame5);
            
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    auto animate = Animate::create(animation);
	auto seq = Sequence::create(animate,
								  FlipX::create(true),
								  animate->clone(),
								  FlipX::create(false),
								  NULL);
    
    sprite->runAction(RepeatForever::create( seq ) );
}

void SpriteAnimationSplit::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteAnimationSplit::title()
{
    return "Sprite: Animation + flip";
}

//------------------------------------------------------------------
//
// SpriteHybrid
//
//------------------------------------------------------------------
SpriteHybrid::SpriteHybrid()
{
    auto s = Director::getInstance()->getWinSize();

    // parents
    auto parent1 = Node::create();
    auto parent2 = SpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(parent1, 0, kTagNode);
    addChild(parent2, 0, kTagSpriteBatchNode);
    
    
    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     SpriteFrameCache::getInstance()->removeUnusedSpriteFrames);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");
    
    
    // create 250 sprites
    // only show 80% of them
    for(int i = 0; i < 250; i++) 
    {
        int spriteIdx = CCRANDOM_0_1() * 14;
        char str[25] = {0};
        sprintf(str, "grossini_dance_%02d.png", (spriteIdx+1));
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        auto sprite = Sprite::createWithSpriteFrame(frame);
        parent1->addChild(sprite, i, i);
        
        float x=-1000;
        float y=-1000;
        if( CCRANDOM_0_1() < 0.2f ) 
        {
            x = CCRANDOM_0_1() * s.width;
            y = CCRANDOM_0_1() * s.height;
        }
        sprite->setPosition( Point(x,y) );
            
        auto action = RotateBy::create(4, 360);
        sprite->runAction( RepeatForever::create(action) );
    }
    
    _usingSpriteBatchNode = false;
    
    schedule( schedule_selector(SpriteHybrid::reparentSprite), 2);
}

void SpriteHybrid::reparentSprite(float dt)
{
    auto p1 = getChildByTag(kTagNode);
    auto p2 = getChildByTag( kTagSpriteBatchNode );
    
    auto retArray = Array::createWithCapacity(250);

    if( _usingSpriteBatchNode )
        CC_SWAP(p1,p2, Node*);

    ////----CCLOG("New parent is: %x", p2);
    
    Node* node;
    Object* pObject;
    auto children = p1->getChildren();
    CCARRAY_FOREACH(children, pObject)
    {
        node = static_cast<Node*>( pObject );
        if(! node )
            break;

        retArray->addObject(node);
    }

    int i=0;
    p1->removeAllChildrenWithCleanup(false);

    CCARRAY_FOREACH(retArray, pObject)
    {
        node = static_cast<Node*>( pObject );
        if(! node)
            break;

        p2->addChild(node, i, i);
        i++;
    }

    _usingSpriteBatchNode = ! _usingSpriteBatchNode;
}

void SpriteHybrid::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("animations/grossini.plist");
}

std::string SpriteHybrid::title()
{
    return "HybrSprite* sprite Test";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildren
//
//------------------------------------------------------------------

SpriteBatchNodeChildren::SpriteBatchNodeChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    // parents
    auto batch = SpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(batch, 0, kTagSpriteBatchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");
    
    auto sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point( s.width/3, s.height/2));
    
    auto sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(50,50));
    
    auto sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-50,-50));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2);
    sprite1->addChild(sprite3);
    
    // BEGIN NEW CODE
    auto animFrames = Array::create();
    char str[100] = {0};
    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_%02d.png",i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames->addObject(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    sprite1->runAction(RepeatForever::create( Animate::create(animation) ) );
    // END NEW CODE
    
    auto action = MoveBy::create(2, Point(200,0));
    auto action_back = action->reverse();
    auto action_rot = RotateBy::create(2, 360);
    auto action_s = ScaleBy::create(2, 2);
    auto action_s_back = action_s->reverse();
    
    auto seq2 = action_rot->reverse();
    sprite2->runAction( RepeatForever::create(seq2) );
    
    sprite1->runAction( RepeatForever::create(action_rot));
    sprite1->runAction( RepeatForever::create(Sequence::create(action, action_back,NULL)) );
    sprite1->runAction( RepeatForever::create(Sequence::create(action_s, action_s_back,NULL)) );

}

void SpriteBatchNodeChildren::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildren::title()
{
    return "SpriteBatchNode GkdRand Children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenZ
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenZ::SpriteBatchNodeChildrenZ()
{
    auto s = Director::getInstance()->getWinSize();
    
    // parents
    SpriteBatchNode* batch;
    Sprite* sprite1, *sprite2, *sprite3;

    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");
    
    // test 1
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point( s.width/3, s.height/2));
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, 2);
    sprite1->addChild(sprite3, -2);
    
    // test 2
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point( 2*s.width/3, s.height/2));
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    // test 3
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point( s.width/2 - 90, s.height/4));
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point( s.width/2 - 60,s.height/4));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point( s.width/2 - 30, s.height/4));
    
    batch->addChild(sprite1, 10);
    batch->addChild(sprite2, -10);
    batch->addChild(sprite3, -5);

    // test 4
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point( s.width/2 +30, s.height/4));
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point( s.width/2 +60,s.height/4));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point( s.width/2 +90, s.height/4));
    
    batch->addChild(sprite1, -10);
    batch->addChild(sprite2, -5);
    batch->addChild(sprite3, -2);
}

void SpriteBatchNodeChildrenZ::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildrenZ::title()
{
    return "SpriteBatchNode Children Z";
}

//------------------------------------------------------------------
//
// SpriteChildrenVisibility
//
//------------------------------------------------------------------

SpriteChildrenVisibility::SpriteChildrenVisibility()
{
    auto s = Director::getInstance()->getWinSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");

    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3;
    //
    // SpriteBatchNode
    //
    // parents
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    aParent->setPosition( Point(s.width/3, s.height/2) );
    addChild(aParent, 0);
    
    
    
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point(0,0));
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(Blink::create(5, 10));
    
    //
    // Sprite
    //
    aParent = Node::create();
    aParent->setPosition( Point(2*s.width/3, s.height/2) );
    addChild(aParent, 0);

    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point(0,0));
            
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(Blink::create(5, 10));
}

void SpriteChildrenVisibility::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteChildrenVisibility::title()
{
    return "Sprite & SpriteBatchNode Visibility";
}

//------------------------------------------------------------------
//
// SpriteChildrenVisibilityIssue665
//
//------------------------------------------------------------------
SpriteChildrenVisibilityIssue665::SpriteChildrenVisibilityIssue665()
{
    auto s = Director::getInstance()->getWinSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");

    Node *aParent;
    Sprite *sprite1, *sprite2, *sprite3;
    //
    // SpriteBatchNode
    //
    // parents
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    aParent->setPosition(Point(s.width/3, s.height/2));
    addChild(aParent, 0);

    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point(0,0));

    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));

    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));

    // test issue #665
    sprite1->setVisible(false);

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);

    //
    // Sprite
    //
    aParent = Node::create();
    aParent->setPosition(Point(2*s.width/3, s.height/2));
    addChild(aParent, 0);

    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
    sprite1->setPosition(Point(0,0));

    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));

    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));

    // test issue #665
    sprite1->setVisible(false);

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
}

SpriteChildrenVisibilityIssue665::~SpriteChildrenVisibilityIssue665()
{
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteChildrenVisibilityIssue665::title()
{
    return "Sprite & SpriteBatchNode Visibility";
}

std::string SpriteChildrenVisibilityIssue665::subtitle()
{
    return "No sprites should be visible";
}

//------------------------------------------------------------------
//
// SpriteChildrenAnchorPoint
//
//------------------------------------------------------------------
SpriteChildrenAnchorPoint::SpriteChildrenAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");
    
    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = Node::create();
    addChild(aParent, 0);
    
    // anchor (0,0)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/4,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );

    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );

    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    
    // anchor (0.5, 0.5)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/2,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_MIDDLE );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));

    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));

    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );        

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/2+s.width/4,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );

    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

void SpriteChildrenAnchorPoint::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteChildrenAnchorPoint::title()
{
    return "Sprite: children + anchor";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenAnchorPoint
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenAnchorPoint::SpriteBatchNodeChildrenAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini.plist");
    
    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(aParent, 0);
    
    // anchor (0,0)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/4,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_BOTTOM_LEFT );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    
    // anchor (0.5, 0.5)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/2,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_MIDDLE );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = Sprite::createWithSpriteFrameName("grossini_dance_08.png");
    sprite1->setPosition(Point(s.width/2+s.width/4,s.height/2));
    sprite1->setAnchorPoint( Point::ANCHOR_TOP_RIGHT );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossini_dance_02.png");
    sprite2->setPosition(Point(20,30));
    
    sprite3 = Sprite::createWithSpriteFrameName("grossini_dance_03.png");
    sprite3->setPosition(Point(-20,30));
    
    sprite4 = Sprite::createWithSpriteFrameName("grossini_dance_04.png");
    sprite4->setPosition(Point(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

void SpriteBatchNodeChildrenAnchorPoint::onExit()
{
    SpriteTestDemo::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildrenAnchorPoint::title()
{
    return "SpriteBatchNode: children + anchor";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenScale
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenScale::SpriteBatchNodeChildrenScale()
{
    auto s = Director::getInstance()->getWinSize();        
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/grossini_family.plist");

    Node *aParent;
    Sprite* sprite1, *sprite2;
    auto rot = RotateBy::create(10, 360);
    auto seq = RepeatForever::create(rot);
    
    //
    // Children + Scale using Sprite
    // Test 1
    //
    aParent = Node::create();
    sprite1 = Sprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( Point( s.width/4, s.height/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction(seq);
    
    
    sprite2 = Sprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( Point( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using SpriteBatchNode
    // Test 2
    //
    
    aParent = SpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = Sprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( Point( 3*s.width/4, s.height/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction( seq->clone() );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( Point( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using Sprite
    // Test 3
    //
    
    aParent = Node::create();
    sprite1 = Sprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( Point( s.width/4, 2*s.height/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f );
    sprite1->runAction( seq->clone() );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( Point( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
    
    //
    // Children + Scale using Sprite
    // Test 4
    //
    
    aParent = SpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = Sprite::createWithSpriteFrameName("grossinis_sister1.png");
    sprite1->setPosition( Point( 3*s.width/4, 2*s.height/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f);
    sprite1->runAction( seq->clone() );
    
    sprite2 = Sprite::createWithSpriteFrameName("grossinis_sister2.png");
    sprite2->setPosition( Point( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
    
}

std::string SpriteBatchNodeChildrenScale::title()
{
    return "Sprite/BatchNode + child + scale + rot";
}

//------------------------------------------------------------------
//
// SpriteChildrenChildren
//
//------------------------------------------------------------------
SpriteChildrenChildren::SpriteChildrenChildren()
{
    auto s = Director::getInstance()->getWinSize();        
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/ghosts.plist");
    
    Node *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    auto rot = RotateBy::create(10, 360);
    auto seq = RepeatForever::create(rot);
    
    auto rot_back = rot->reverse();
    auto rot_back_fe = RepeatForever::create(rot_back);
    
    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = Node::create();
    addChild(aParent);
    
    // parent
    l1 = Sprite::createWithSpriteFrameName("father.gif");
    l1->setPosition( Point( s.width/2, s.height/2) );
    l1->runAction( seq->clone() );
    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();
    
    // child left
    l2a = Sprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition( Point( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( rot_back_fe->clone() );
    l1->addChild(l2a);
    auto l2aSize = l2a->getContentSize();        
    
    
    // child right
    l2b = Sprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition( Point( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( rot_back_fe->clone() );
    l1->addChild(l2b);
    auto l2bSize = l2a->getContentSize();        
    
    
    // child left bottom
    l3a1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale( 0.45f );
    l3a1->setPosition( Point(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale( 0.45f );
    l3a1->setPosition( Point(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale( 0.45f);
    l3b1->setFlippedY( true );
    l3b1->setPosition( Point(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);
    
    // child right top
    l3b2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale( 0.45f );
    l3b2->setFlippedY( true );
    l3b1->setPosition( Point(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
}

std::string SpriteChildrenChildren::title()
{
    return "Sprite multiple levels of children";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenChildren
//
//------------------------------------------------------------------

SpriteBatchNodeChildrenChildren::SpriteBatchNodeChildrenChildren()
{
    auto s = Director::getInstance()->getWinSize();        
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/ghosts.plist");
    
    SpriteBatchNode *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    auto rot = RotateBy::create(10, 360);
    auto seq = RepeatForever::create(rot);
    
    auto rot_back = rot->reverse();
    auto rot_back_fe = RepeatForever::create(rot_back);
    
    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = SpriteBatchNode::create("animations/ghosts.png");
    aParent->getTexture()->generateMipmap();
    addChild(aParent);
    
    // parent
    l1 = Sprite::createWithSpriteFrameName("father.gif");
    l1->setPosition( Point( s.width/2, s.height/2) );
    l1->runAction( seq->clone() );
    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition( Point( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( rot_back_fe->clone() );
    l1->addChild(l2a);
    auto l2aSize = l2a->getContentSize();        


    // child right
    l2b = Sprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition( Point( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( rot_back_fe->clone() );
    l1->addChild(l2b);
    auto l2bSize = l2a->getContentSize();        

    
    // child left bottom
    l3a1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale( 0.45f );
    l3a1->setPosition( Point(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale( 0.45f );
    l3a1->setPosition( Point(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale( 0.45f );
    l3b1->setFlippedY( true );
    l3b1->setPosition( Point(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);

    // child right top
    l3b2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale( 0.45f );
    l3b2->setFlippedY( true );
    l3b1->setPosition( Point(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
    
}

std::string SpriteBatchNodeChildrenChildren::title()
{
    return "SpriteBatchNode multiple levels of children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeSkewNegativeScaleChildren
//
//------------------------------------------------------------------

SpriteBatchNodeSkewNegativeScaleChildren::SpriteBatchNodeSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for(int i=0;i<2;i++) {
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point( s.width/4*(i+1), s.height/2));

        // Skew
        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        auto child1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        child1->setPosition(Point(sprite->getContentSize().width / 2.0f, sprite->getContentSize().height / 2.0f));

        child1->setScale(0.8f);

        sprite->addChild(child1);

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeSkewNegativeScaleChildren::~SpriteBatchNodeSkewNegativeScaleChildren()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeSkewNegativeScaleChildren::title()
{
    return "SpriteBatchNode + children + skew";
}

std::string SpriteBatchNodeSkewNegativeScaleChildren::subtitle()
{
    return "SpriteBatchNode skew + negative scale with children";
}

// SpriteSkewNegativeScaleChildren

SpriteSkewNegativeScaleChildren::SpriteSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    auto parent = Node::create();
    addChild(parent);

    for(int i=0;i<2;i++) {
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point( s.width/4*(i+1), s.height/2));

        // Skew
        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        auto child1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        child1->setPosition(Point(sprite->getContentSize().width / 2.0f, sprite->getContentSize().height / 2.0f));

        sprite->addChild(child1);

        child1->setScale(0.8f);

        parent->addChild(sprite, i);
    }
}

SpriteSkewNegativeScaleChildren::~SpriteSkewNegativeScaleChildren()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteSkewNegativeScaleChildren::title()
{
    return "Sprite + children + skew";
}

std::string SpriteSkewNegativeScaleChildren::subtitle()
{
    return "Sprite skew + negative scale with children";
}

//------------------------------------------------------------------
//
// SpriteNilTexture
//
//------------------------------------------------------------------

SpriteNilTexture::SpriteNilTexture()
{
    auto s = Director::getInstance()->getWinSize();

    Sprite* sprite = NULL;
    
    // TEST: If no texture is given, then Opacity + Color should work.

    sprite = new Sprite();
    sprite->init();
    sprite->setTextureRect( Rect(0, 0, 300,300) );
    sprite->setColor(Color3B::RED);
    sprite->setOpacity(128);
    sprite->setPosition(Point(3*s.width/4, s.height/2));
    addChild(sprite, 100);
    sprite->release();

    sprite = new Sprite();
    sprite->init();
    sprite->setTextureRect(Rect(0, 0, 300,300));
    sprite->setColor(Color3B::BLUE);
    sprite->setOpacity(128);
    sprite->setPosition(Point(1*s.width/4, s.height/2));
    addChild(sprite, 100);
    sprite->release();
}

std::string SpriteNilTexture::title()
{
    return "Sprite without texture";
}

std::string SpriteNilTexture::subtitle()
{
    return "opacity and color should work";
}

class MySprite1 : public Sprite
{
public:
    MySprite1() : ivar(10) {}
    static MySprite1* createWithSpriteFrameName(const char *pszSpriteFrameName)
    {
        auto pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);
        MySprite1 *pobSprite = new MySprite1();
        pobSprite->initWithSpriteFrame(pFrame);
        pobSprite->autorelease();

        return pobSprite;
    }

private:
    int ivar;
};

class MySprite2 : public Sprite
{
public:
    MySprite2() : ivar(10) {}
    static MySprite2* create(const char *pszName)
    {
        MySprite2 *pobSprite = new MySprite2();
        pobSprite->initWithFile(pszName);
        pobSprite->autorelease();

        return pobSprite;
    }

private:
    int ivar;
};

//------------------------------------------------------------------
//
// SpriteSubclass
//
//------------------------------------------------------------------
SpriteSubclass::SpriteSubclass()
{
    auto s = Director::getInstance()->getWinSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/ghosts.plist");
    auto aParent = SpriteBatchNode::create("animations/ghosts.png");

    // MySprite1
    MySprite1 *sprite = MySprite1::createWithSpriteFrameName("father.gif");
    sprite->setPosition(Point( s.width/4*1, s.height/2));
    aParent->addChild(sprite);
    addChild(aParent);

    // MySprite2
    MySprite2 *sprite2 = MySprite2::create("Images/grossini.png");
    addChild(sprite2);
    sprite2->setPosition(Point(s.width/4*3, s.height/2));
}

std::string SpriteSubclass::title()
{
    return "Sprite subclass";
}

std::string SpriteSubclass::subtitle()
{
    return "Testing initWithTexture:rect method";
}


class DoubleSprite : public Sprite
{
public:
    DoubleSprite() {_HD = false;}
    virtual bool initWithTexture(Texture2D* texture, const Rect& rect);
    // Called everytime the vertex needs to be updated.
    virtual void setContentSize(const Size& size);
    // rect used only for the vertex. Called everytime the vertex needs to be updated.
    virtual void setVertexRect(const Rect& rect);

    static DoubleSprite* create(const char* pszFileName);
    bool _HD;
};

DoubleSprite* DoubleSprite::create(const char* pszFileName)
{
    auto pSp = new DoubleSprite();
    pSp->initWithFile(pszFileName);
    pSp->autorelease();
    return pSp;
}

bool DoubleSprite::initWithTexture(Texture2D* texture, const Rect& rect)
{
    if( Sprite::initWithTexture(texture, rect)) 
    {
        return true;
    }

    return false;
}

// Called everytime the vertex needs to be updated.
void DoubleSprite::setContentSize(const Size& size)
{
    auto s = size;
    // If Retina Display and Texture is in HD then scale the vertex rect
    if( CC_CONTENT_SCALE_FACTOR() == 2 && ! _HD ) {
        s.width *= 2;
        s.height *= 2;
    }

    Sprite::setContentSize(s);
}

// rect used only for the vertex. Called everytime the vertex needs to be updated.
void DoubleSprite::setVertexRect(const Rect& rect)
{
    auto tmpRect = rect;
    // If Retina Display and Texture is in HD then scale the vertex rect
    if( CC_CONTENT_SCALE_FACTOR() == 2 && ! _HD ) {
        tmpRect.size.width *= 2;
        tmpRect.size.height *= 2;
    }

    Sprite::setVertexRect(tmpRect);
}


// SpriteDoubleResolution

SpriteDoubleResolution::SpriteDoubleResolution()
{
    auto s = Director::getInstance()->getWinSize();

    //
    // LEFT: SD sprite
    //
    // there is no HD resolution file of grossini_dance_08.
    auto spriteSD = DoubleSprite::create("Images/grossini_dance_08.png");
    addChild(spriteSD);
    spriteSD->setPosition(Point(s.width/4*1,s.height/2));

    auto child1_left = DoubleSprite::create("Images/grossini_dance_08.png");
    spriteSD->addChild(child1_left);
    child1_left->setPosition(Point(-30,0));

    auto child1_right = Sprite::create("Images/grossini.png");
    spriteSD->addChild(child1_right);
    child1_left->setPosition(Point( spriteSD->getContentSize().height, 0));



    //
    // RIGHT: HD sprite
    //
    // there is an HD version of grossini.png
    auto spriteHD = Sprite::create("Images/grossini.png");
    addChild(spriteHD);
    spriteHD->setPosition(Point(s.width/4*3,s.height/2));

    auto child2_left = DoubleSprite::create("Images/grossini_dance_08.png");
    spriteHD->addChild(child2_left);
    child2_left->setPosition(Point(-30,0));

    auto child2_right = Sprite::create("Images/grossini.png");
    spriteHD->addChild(child2_right);
    child2_left->setPosition(Point( spriteHD->getContentSize().height, 0));



    // Actions
    auto scale = ScaleBy::create(2, 0.5);
    auto scale_back = scale->reverse();
    auto seq = Sequence::create(scale, scale_back, NULL);

    auto seq_copy = seq->clone();

    spriteSD->runAction(seq);
    spriteHD->runAction(seq_copy);

}

std::string SpriteDoubleResolution::title()
{
    return "Sprite Double resolution";
}

std::string SpriteDoubleResolution::subtitle()
{
    return "Retina Display. SD (left) should be equal to HD (right)";
}

//------------------------------------------------------------------
//
// AnimationCache
//
//------------------------------------------------------------------
AnimationCacheTest::AnimationCacheTest()
{
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("animations/grossini.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_blue.plist");

    //
    // create animation "dance"
    //
    auto animFrames = Array::createWithCapacity(15);
    char str[100] = {0};
    for(int i = 1; i < 15; i++)
    {
        sprintf(str, "grossini_dance_%02d.png",i);
        auto frame = frameCache->getSpriteFrameByName(str);
        animFrames->addObject(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation(animation, "dance");

    //
    // create animation "dance gray"
    //
    animFrames->removeAllObjects();

    for(int i = 1; i < 15; i++)
    {
        sprintf(str, "grossini_dance_gray_%02d.png",i);
        auto frame = frameCache->getSpriteFrameByName(str);
        animFrames->addObject(frame);
    }

    animation = Animation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation(animation, "dance_gray");

    //
    // create animation "dance blue"
    //
    animFrames->removeAllObjects();

    for(int i = 1; i < 4; i++)
    {
        sprintf(str, "grossini_blue_%02d.png",i);
        auto frame = frameCache->getSpriteFrameByName(str);
        animFrames->addObject(frame);
    }

    animation = Animation::createWithSpriteFrames(animFrames, 0.2f);

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation(animation, "dance_blue");


    auto animCache = AnimationCache::getInstance();

    auto normal = animCache->getAnimation("dance");
    normal->setRestoreOriginalFrame(true);
    auto dance_grey = animCache->getAnimation("dance_gray");
    dance_grey->setRestoreOriginalFrame(true);
    auto dance_blue = animCache->getAnimation("dance_blue");
    dance_blue->setRestoreOriginalFrame(true);

    auto animN = Animate::create(normal);
    auto animG = Animate::create(dance_grey);
    auto animB = Animate::create(dance_blue);

    auto seq = Sequence::create(animN, animG, animB, NULL);

    // create an sprite without texture
    auto grossini = Sprite::create();
    auto frame = frameCache->getSpriteFrameByName("grossini_dance_01.png");
    grossini->setDisplayFrame(frame);

    auto winSize = Director::getInstance()->getWinSize();
    grossini->setPosition(Point(winSize.width/2, winSize.height/2));
    addChild(grossini);

    // run the animation
    grossini->runAction(seq);
}

std::string AnimationCacheTest::title()
{
    return "AnimationCache";
}

std::string AnimationCacheTest::subtitle()
{
    return "Sprite should be animated";
}

// AnimationCacheFile


AnimationCacheFile::AnimationCacheFile()
{
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("animations/grossini.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache->addSpriteFramesWithFile("animations/grossini_blue.plist");


    // Purge previously loaded animation
    AnimationCache::destroyInstance();

    auto animCache = AnimationCache::getInstance();

    // Add an animation to the Cache
    animCache->addAnimationsWithFile("animations/animations.plist");


    auto normal = animCache->getAnimation("dance_1");
    normal->setRestoreOriginalFrame(true);
    auto dance_grey = animCache->getAnimation("dance_2");
    dance_grey->setRestoreOriginalFrame(true);
    auto dance_blue = animCache->getAnimation("dance_3");
    dance_blue->setRestoreOriginalFrame(true);

    auto animN = Animate::create(normal);
    auto animG = Animate::create(dance_grey);
    auto animB = Animate::create(dance_blue);

    auto seq = Sequence::create(animN, animG, animB, NULL);

    // create an sprite without texture
    auto grossini = Sprite::create();

    auto frame = frameCache->getSpriteFrameByName("grossini_dance_01.png");
    grossini->setDisplayFrame(frame);

    auto winSize = Director::getInstance()->getWinSize();

    grossini->setPosition(Point(winSize.width/2, winSize.height/2));

    addChild(grossini);


    // run the animation
    grossini->runAction(seq);
}

std::string AnimationCacheFile::title()
{
    return "AnimationCache - Load file";
}

std::string AnimationCacheFile::subtitle()
{
    return "Sprite should be animated";
}

// SpriteBatchBug1217

SpriteBatchBug1217::SpriteBatchBug1217()
{
    auto bn = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);

    auto s1 = Sprite::createWithTexture(bn->getTexture(), Rect(0, 0, 57, 57));
    auto s2 = Sprite::createWithTexture(bn->getTexture(), Rect(0, 0, 57, 57));
    auto s3 = Sprite::createWithTexture(bn->getTexture(), Rect(0, 0, 57, 57));

    s1->setColor(Color3B(255, 0, 0));
    s2->setColor(Color3B(0, 255, 0));
    s3->setColor(Color3B(0, 0, 255));

    s1->setPosition(Point(20,200));
    s2->setPosition(Point(100,0));
    s3->setPosition(Point(100,0));

    bn->setPosition(Point(0,0));

    //!!!!!
    s1->addChild(s2);
    s2->addChild(s3);
    bn->addChild(s1);

    addChild(bn);
}

std::string SpriteBatchBug1217::title()
{
    return "SpriteBatch - Bug 1217";
}

std::string SpriteBatchBug1217::subtitle()
{
    return "Adding big family to spritebatch. You shall see 3 heads";
}


void SpriteTestScene::runThisTest()
{
    auto layer = nextSpriteTestAction();
    addChild(layer);

    Director::getInstance()->replaceScene(this);
}

//
// SpriteOffsetAnchorSkew
// 
SpriteOffsetAnchorSkew::SpriteOffsetAnchorSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
            break;
        }

        point->setPosition(sprite->getPosition());

        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", j + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorSkew::~SpriteOffsetAnchorSkew()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteOffsetAnchorSkew::title()
{
    return "Sprite offset + anchor + scale";
}

//
// SpriteBatchNodeOffsetAnchorSkew
// 
SpriteBatchNodeOffsetAnchorSkew::SpriteBatchNodeOffsetAnchorSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());
        
        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", j + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorSkew::~SpriteBatchNodeOffsetAnchorSkew()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteBatchNodeOffsetAnchorSkew::title()
{
    return "SpriteBatchNode offset + anchor + skew";
}

//
// SpriteOffsetAnchorSkewScale
//
SpriteOffsetAnchorSkewScale::SpriteOffsetAnchorSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());

        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", j + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        // Skew
        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        // Scale
        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorSkewScale::~SpriteOffsetAnchorSkewScale()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteOffsetAnchorSkewScale::title()
{
    return "Sprite anchor + skew + scale";
}

SpriteBatchNodeOffsetAnchorSkewScale::SpriteBatchNodeOffsetAnchorSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());        

        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", j + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        // skew
        auto skewX = SkewBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = SkewBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();

        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));

        // scale 
        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorSkewScale::~SpriteBatchNodeOffsetAnchorSkewScale()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteBatchNodeOffsetAnchorSkewScale::title()
{
    return "SpriteBatchNode anchor + skew + scale";
}

//
// SpriteOffsetAnchorFlip
// 
SpriteOffsetAnchorFlip::SpriteOffsetAnchorFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());

        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", i + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        auto flip = FlipY::create(true);
        auto flip_back = FlipY::create(false);
        auto delay = DelayTime::create(1);
        auto seq = Sequence::create(delay, flip, delay->clone(), flip_back, NULL);
        sprite->runAction(RepeatForever::create(seq));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorFlip::~SpriteOffsetAnchorFlip()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteOffsetAnchorFlip::title()
{
    return "Sprite offset + anchor + flip";
}

string SpriteOffsetAnchorFlip::subtitle()
{
    return "issue #1078";
}

//
// SpriteBatchNodeOffsetAnchorFlip
// 

SpriteBatchNodeOffsetAnchorFlip::SpriteBatchNodeOffsetAnchorFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());        

        auto animFrames = Array::create();
        char tmp[50];
        for (int j = 0; j < 14; j++)
        {            
            sprintf(tmp, "grossini_dance_%02d.png", i + 1);
            auto frame = cache->getSpriteFrameByName(tmp);
            animFrames->addObject(frame);
        }

        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));

        auto flip = FlipY::create(true);
        auto flip_back = FlipY::create(false);
        auto delay = DelayTime::create(1);
        auto seq = Sequence::create(delay, flip, delay->clone(), flip_back, NULL);
        sprite->runAction(RepeatForever::create(seq));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorFlip::~SpriteBatchNodeOffsetAnchorFlip()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteBatchNodeOffsetAnchorFlip::title()
{
    return "SpriteBatchNode offset + anchor + flip";
}

string SpriteBatchNodeOffsetAnchorFlip::subtitle()
{
    return "issue #1078";
}


/// NodeSort

NodeSort::NodeSort()
{
    _node = Node::create();
    addChild(_node, 0, 0);

    _sprite1 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite1->setPosition(Point(100, 160));
    _node->addChild(_sprite1, -6, 1);

    _sprite2 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite2->setPosition(Point(164, 160));
    _node->addChild(_sprite2, -6, 2);

    _sprite4 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite4->setPosition(Point(292, 160));
    _node->addChild(_sprite4, -3, 4);

    _sprite3 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite3->setPosition(Point(228, 160));
    _node->addChild(_sprite3, -4, 3);

    _sprite5 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite5->setPosition(Point(356, 160));
    _node->addChild(_sprite5, -3, 5);

    schedule(schedule_selector(NodeSort::reorderSprite));
}

std::string NodeSort::title()
{
    return "node sort same index";
}

std::string NodeSort::subtitle()
{
    return "tag order in console should be 2,1,3,4,5";
}

void NodeSort::reorderSprite(float dt)
{
    unschedule(schedule_selector(NodeSort::reorderSprite));

    log("Before reorder--");
    
    Object* pObj = NULL;
    CCARRAY_FOREACH(_node->getChildren(), pObj)
    {
        auto child = static_cast<Sprite*>( pObj );
        log("tag %i z %i",(int)child->getTag(),(int)child->getZOrder());
    }
    //z-4
    _node->reorderChild( static_cast<Node*>( _node->getChildren()->getObjectAtIndex(0) ), -6);

    _node->sortAllChildren();
    log("After reorder--");
    CCARRAY_FOREACH(_node->getChildren(), pObj)
    {
        auto child = static_cast<Sprite*>( pObj );
        log("tag %i z %i",(int)child->getTag(),(int)child->getZOrder());
    }
}

/// SpriteBatchNodeReorderSameIndex
SpriteBatchNodeReorderSameIndex::SpriteBatchNodeReorderSameIndex()
{
    _batchNode = SpriteBatchNode::create("Images/piece.png", 15);
    addChild(_batchNode, 1, 0);

    _sprite1 = Sprite::createWithTexture(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite1->setPosition(Point(100,160));
    _batchNode->addChild(_sprite1, 3, 1);

    _sprite2= Sprite::createWithTexture(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite2->setPosition(Point(164,160));
    _batchNode->addChild(_sprite2, 4, 2);

    _sprite3 = Sprite::createWithTexture(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite3->setPosition(Point(228,160));
    _batchNode->addChild(_sprite3, 4, 3);

    _sprite4 = Sprite::createWithTexture(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite4->setPosition(Point(292,160));
    _batchNode->addChild(_sprite4, 5, 4);

    _sprite5 = Sprite::createWithTexture(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite5->setPosition(Point(356,160));
    _batchNode->addChild(_sprite5, 6, 5);


    scheduleOnce(schedule_selector(SpriteBatchNodeReorderSameIndex::reorderSprite), 2);
}

std::string SpriteBatchNodeReorderSameIndex::title()
{
    return "SpriteBatchNodeReorder same index";
}

std::string SpriteBatchNodeReorderSameIndex::subtitle()
{
    return "tag order in console should be 2,3,4,5,1";
}

void SpriteBatchNodeReorderSameIndex::reorderSprite(float dt)
{
    _batchNode->reorderChild(_sprite4, 4);
    _batchNode->reorderChild(_sprite5, 4);
    _batchNode->reorderChild(_sprite1, 4);

    _batchNode->sortAllChildren();

    std::for_each(_batchNode->getDescendants().begin(), _batchNode->getDescendants().end(), [&](Sprite* sprite) {
        log("tag %i", sprite->getTag() );
    });
}

/// SpriteBatchNodeReorderOneChild
SpriteBatchNodeReorderOneChild::SpriteBatchNodeReorderOneChild()
{
    auto s = Director::getInstance()->getWinSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/ghosts.plist");

    SpriteBatchNode *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;

    //
    // SpriteBatchNode: 3 levels of children
    //

    aParent = SpriteBatchNode::create("animations/ghosts.png");

    _batchNode = aParent;
    //[[aParent texture] generateMipmap];
    addChild(aParent);

    // parent
    l1 = Sprite::createWithSpriteFrameName("father.gif");
    l1->setPosition(Point( s.width/2, s.height/2));

    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::createWithSpriteFrameName("sister1.gif");
    l2a->setPosition(Point( -10 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2a, 1);
    auto l2aSize = l2a->getContentSize();


    // child right
    l2b = Sprite::createWithSpriteFrameName("sister2.gif");
    l2b->setPosition(Point( +50 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2b, 2);
    auto l2bSize = l2a->getContentSize();


    // child left bottom
    l3a1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a1->setScale(0.45f);
    l3a1->setPosition(Point(0+l2aSize.width/2,-50+l2aSize.height/2));
    l2a->addChild(l3a1, 1);

    // child left top
    l3a2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3a2->setScale(0.45f);
    l3a2->setPosition(Point(0+l2aSize.width/2,+50+l2aSize.height/2));
    l2a->addChild(l3a2, 2);

    _reorderSprite = l2a;

    // child right bottom
    l3b1 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b1->setScale(0.45f);
    l3b1->setFlippedY(true);
    l3b1->setPosition(Point(0+l2bSize.width/2,-50+l2bSize.height/2));
    l2b->addChild(l3b1);

    // child right top
    l3b2 = Sprite::createWithSpriteFrameName("child1.gif");
    l3b2->setScale(0.45f);
    l3b2->setFlippedY(true);
    l3b2->setPosition(Point(0+l2bSize.width/2,+50+l2bSize.height/2));
    l2b->addChild(l3b2);

    scheduleOnce(schedule_selector(SpriteBatchNodeReorderOneChild::reorderSprite), 2.0f);
}

void SpriteBatchNodeReorderOneChild::reorderSprite(float dt)
{
    _reorderSprite->getParent()->reorderChild(_reorderSprite, -1);

    _batchNode->sortAllChildren();
    //CCSprite* child;
    //CCARRAY_FOREACH(batchNode.descendants,child) NSLog(@"tag %i",child.tag);
}

std::string SpriteBatchNodeReorderOneChild::title()
{
    return "SpriteBatchNode reorder 1 child";
}

// SpriteOffsetAnchorRotationalSkew
SpriteOffsetAnchorRotationalSkew::SpriteOffsetAnchorRotationalSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
                            
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        auto animFrames = Array::create();
        for(int i = 0; i < 14; i++)
        {
            char pngName[30];
            kdSnprintf(pngName, 30, "grossini_dance_%02d.png", (i+1));
            auto frame = cache->getSpriteFrameByName(pngName);
            animFrames->addObject(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));
        
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorRotationalSkew::~SpriteOffsetAnchorRotationalSkew()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteOffsetAnchorRotationalSkew::title()
{
    return "Sprite offset + anchor + rotational skew";
}

// SpriteBatchNodeOffsetAnchorRotationalSkew
SpriteBatchNodeOffsetAnchorRotationalSkew::SpriteBatchNodeOffsetAnchorRotationalSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        auto animFrames = Array::create();
        for(int j = 0; j < 14; j++)
        {
            char pngName[30];
            kdSnprintf(pngName, 30, "grossini_dance_%02d.png", (j+1));
            auto frame = cache->getSpriteFrameByName(pngName);
            animFrames->addObject(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));
        
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorRotationalSkew::~SpriteBatchNodeOffsetAnchorRotationalSkew()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteBatchNodeOffsetAnchorRotationalSkew::title()
{
    return "SpriteBatchNode offset + anchor + rot skew";
}

// SpriteOffsetAnchorRotationalSkewScale
SpriteOffsetAnchorRotationalSkewScale::SpriteOffsetAnchorRotationalSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        auto animFrames = Array::create();
        for(int j = 0; j < 14; j++)
        {
            char pngName[30];
            kdSnprintf(pngName, 30, "grossini_dance_%02d.png", (j+1));
            auto frame = cache->getSpriteFrameByName(pngName);
            animFrames->addObject(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));
        
        // Skew
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        // Scale
        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale));
        
        addChild(sprite, i);
    }
}

SpriteOffsetAnchorRotationalSkewScale::~SpriteOffsetAnchorRotationalSkewScale()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteOffsetAnchorRotationalSkewScale::title()
{
    return "Sprite anchor + rot skew + scale";
}

// SpriteBatchNodeOffsetAnchorRotationalSkewScale
SpriteBatchNodeOffsetAnchorRotationalSkewScale::SpriteBatchNodeOffsetAnchorRotationalSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        auto animFrames = Array::create();
        for(int j = 0; j < 14; j++)
        {
            char pngName[30];
            kdSnprintf(pngName, 30, "grossini_dance_%02d.png", (j+1));
            auto frame = cache->getSpriteFrameByName(pngName);
            animFrames->addObject(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        sprite->runAction(RepeatForever::create(Animate::create(animation)));
        
        // Skew
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        // Scale
        auto scale = ScaleBy::create(2, 2);
        auto scale_back = scale->reverse();
        auto seq_scale = Sequence::create(scale, scale_back, NULL);
        sprite->runAction(RepeatForever::create(seq_scale));
        
        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorRotationalSkewScale::~SpriteBatchNodeOffsetAnchorRotationalSkewScale()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteBatchNodeOffsetAnchorRotationalSkewScale::title()
{
    return "SpriteBatchNode anchor + rot skew + scale";
}

// SpriteRotationalSkewNegativeScaleChildren
SpriteRotationalSkewNegativeScaleChildren::SpriteRotationalSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto parent = Node::create();
    addChild(parent);
    
    for(int i=0;i<2;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        // Skew
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        auto child1 = Sprite::create("Images/grossini_dance_01.png");
        child1->setPosition(Point(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));
        
        sprite->addChild(child1);
        
        child1->setScale(0.8f);
        
        parent->addChild(sprite, i);
    }
}

SpriteRotationalSkewNegativeScaleChildren::~SpriteRotationalSkewNegativeScaleChildren()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("animations/grossini.plist");
    cache->removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

string SpriteRotationalSkewNegativeScaleChildren::title()
{
    return "Sprite rot skew + negative scale with children";
}

// SpriteBatchNodeRotationalSkewNegativeScaleChildren
SpriteBatchNodeRotationalSkewNegativeScaleChildren::SpriteBatchNodeRotationalSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("animations/grossini.plist");
    cache->addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<2;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
        sprite->setPosition(Point(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);
        
        // Skew
        auto skewX = RotateBy::create(2, 45, 0);
        auto skewX_back = skewX->reverse();
        auto skewY = RotateBy::create(2, 0, 45);
        auto skewY_back = skewY->reverse();
        
        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }
        
        auto seq_skew = Sequence::create(skewX, skewX_back, skewY, skewY_back, NULL);
        sprite->runAction(RepeatForever::create(seq_skew));
        
        auto child1 = Sprite::create("Images/grossini_dance_01.png");
        child1->setPosition(Point(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));
        
        sprite->addChild(child1);
        
        child1->setScale(0.8f);
        
        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeRotationalSkewNegativeScaleChildren::~SpriteBatchNodeRotationalSkewNegativeScaleChildren()
{
    
}

string SpriteBatchNodeRotationalSkewNegativeScaleChildren::title()
{
    return "SpriteBatchNode + children + rot skew";
}
