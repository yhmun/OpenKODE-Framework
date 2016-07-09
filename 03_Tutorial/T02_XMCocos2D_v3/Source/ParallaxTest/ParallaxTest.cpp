#include "ParallaxTest.h"
#include "../testResource.h"

enum 
{
    kTagNode,
    kTagGrossini,
};

Layer* nextParallaxAction();
Layer* backParallaxAction();
Layer* restartParallaxAction();

//------------------------------------------------------------------
//
// Parallax1
//
//------------------------------------------------------------------

Parallax1::Parallax1()
{
    // Top Layer, a simple image
    auto cocosImage = Sprite::create(s_Power);
    // scale the image (optional)
    cocosImage->setScale( 2.5f );
    // change the transform anchor point to 0,0 (optional)
    cocosImage->setAnchorPoint( Point(0,0) );
    

    // Middle layer: a Tile map atlas
    auto tilemap = TileMapAtlas::create(s_TilesPng, s_LevelMapTga, 16, 16);
    tilemap->releaseMap();
    
    // change the transform anchor to 0,0 (optional)
    tilemap->setAnchorPoint( Point(0, 0) );

    // Anti Aliased images
    tilemap->getTexture()->setAntiAliasTexParameters();
    

    // background layer: another image
    auto background = Sprite::create(s_back);
    // scale the image (optional)
    background->setScale( 1.5f );
    // change the transform anchor point (optional)
    background->setAnchorPoint( Point(0,0) );

    
    // create a void node, a parent node
    auto voidNode = ParallaxNode::create();
    
    // NOW add the 3 layers to the 'void' node

    // background image is moved at a ratio of 0.4x, 0.5y
    voidNode->addChild(background, -1, Point(0.4f,0.5f), Point::ZERO);
    
    // tiles are moved at a ratio of 2.2x, 1.0y
    voidNode->addChild(tilemap, 1, Point(2.2f,1.0f), Point(0,-200) );
    
    // top image is moved at a ratio of 3.0x, 2.5y
    voidNode->addChild(cocosImage, 2, Point(3.0f,2.5f), Point(200,800) );
    
    
    // now create some actions that will move the 'void' node
    // and the children of the 'void' node will move at different
    // speed, thus, simulation the 3D environment
    auto goUp = MoveBy::create(4, Point(0,-500) );
    auto goDown = goUp->reverse();
    auto go = MoveBy::create(8, Point(-1000,0) );
    auto goBack = go->reverse();
    auto seq = Sequence::create(goUp, go, goDown, goBack, NULL);
    voidNode->runAction( (RepeatForever::create(seq) ));
    
    addChild( voidNode );
}

std::string Parallax1::title()
{
    return "Parallax: parent and 3 children";
}

//------------------------------------------------------------------
//
// Parallax2
//
//------------------------------------------------------------------

Parallax2::Parallax2()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(Parallax2::onTouchesMoved, this);
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Top Layer, a simple image
    auto cocosImage = Sprite::create(s_Power);
    // scale the image (optional)
    cocosImage->setScale( 2.5f );
    // change the transform anchor point to 0,0 (optional)
    cocosImage->setAnchorPoint( Point(0,0) );
    
    
    // Middle layer: a Tile map atlas
    auto tilemap = TileMapAtlas::create(s_TilesPng, s_LevelMapTga, 16, 16);
    tilemap->releaseMap();
    
    // change the transform anchor to 0,0 (optional)
    tilemap->setAnchorPoint( Point(0, 0) );
    
    // Anti Aliased images
    tilemap->getTexture()->setAntiAliasTexParameters();
    
    
    // background layer: another image
    auto background = Sprite::create(s_back);
    // scale the image (optional)
    background->setScale( 1.5f );
    // change the transform anchor point (optional)
    background->setAnchorPoint( Point(0,0) );
    
    
    // create a void node, a parent node
    auto voidNode = ParallaxNode::create();
    
    // NOW add the 3 layers to the 'void' node
    
    // background image is moved at a ratio of 0.4x, 0.5y
    voidNode->addChild(background, -1, Point(0.4f,0.5f), Point::ZERO);
    
    // tiles are moved at a ratio of 1.0, 1.0y
    voidNode->addChild(tilemap, 1, Point(1.0f,1.0f), Point(0,-200) );
    
    // top image is moved at a ratio of 3.0x, 2.5y
    voidNode->addChild( cocosImage, 2, Point(3.0f,2.5f), Point(200,1000) );
    addChild(voidNode, 0, kTagNode);
}

void Parallax2::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    auto diff = touches[0]->getDelta();
    
    auto node = getChildByTag(kTagNode);
    auto currentPos = node->getPosition();
    node->setPosition(currentPos + diff);
}

std::string Parallax2::title()
{
    return "Parallax: drag screen";
}

//------------------------------------------------------------------
//
// ParallaxScroll
//
//------------------------------------------------------------------

ParallaxScroll::ParallaxScroll ( KDvoid )
{
	auto	tWinSize = Director::getInstance ( )->getWinSize ( );

	auto*	pSky = Sprite::create ( "supports/sky_evening.png" );
	pSky->setPosition ( Point ( tWinSize / 2 ) );
	this->addChild ( pSky );

	auto	tSkySize = pSky->getContentSize ( );
    m_pParallax = ParallaxScrollNode::create ( );
	m_pParallax->setContentSize ( tSkySize );
	pSky->addChild ( m_pParallax );

    auto	pLand1 = Sprite::create ( "supports/land_green.png" );
    auto	pLand2 = Sprite::create ( "supports/land_green.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 0, Point ( 0.5f, 0.2f ), Point ( 0, 0 ), pLand1, pLand2, KD_NULL );
    
    auto	pLand3 = Sprite::create ( "supports/land_grey.png" );
    auto	pLand4 = Sprite::create ( "supports/land_grey.png" );
    m_pParallax->addInfiniteScrollXWithZ ( -2, Point ( 0.05f, 0.1f ), Point ( 0, 60 ), pLand3, pLand4, KD_NULL );
    
    auto	pClouds1 = Sprite::create ( "supports/clouds.png" );
    auto	pClouds2 = Sprite::create ( "supports/clouds.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 1, Point ( 0.1f, 0.1f ), Point ( 0, tSkySize.cy - pClouds1->getContentSize ( ).cy ), pClouds1, pClouds2, KD_NULL );

    for ( KDint i = 0; i < 10; i++ )
	{
        auto	pMountain = Sprite::create ( "supports/mountain_grey.png" );
        auto	tPos = Point ( CCRANDOM_BETWEEN ( 0, pLand1->getContentSize ( ).cx ), CCRANDOM_BETWEEN ( tSkySize.cy * 0.1f, tSkySize.cy * 0.25f ) );        
        auto	fSpeedMountainX = CCRANDOM_BETWEEN ( 0.15f, 0.25f );
        m_pParallax->addChild ( pMountain, -1, Point ( fSpeedMountainX, 0.015f ), tPos, Point ( pLand1->getContentSize ( ).cx * 2, 0 ) );
        
        pMountain->setScale ( CCRANDOM_BETWEEN ( 0.6f, 1 ) );
    }	
   
    this->scheduleUpdate ( );
}

KDvoid ParallaxScroll::update ( KDfloat fDelta )
{
	m_pParallax->updateWithVelocity ( Point ( -5, 0 ), fDelta );
}

//------------------------------------------------------------------
//
// ParallaxDemo
//
//------------------------------------------------------------------

static int sceneIdx = -1; 

#define MAX_LAYER    3

Layer* createParallaxTestLayer ( KDint nIndex )
{
    switch(nIndex)
    {
        case 0 : return new Parallax1 ( );
        case 1 : return new Parallax2 ( );
		case 2 : return new ParallaxScroll ( );
    }

    return NULL;
}

Layer* nextParallaxAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;

    auto layer = createParallaxTestLayer(sceneIdx);
    layer->autorelease();

    return layer;
}

Layer* backParallaxAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;    
    
    auto layer = createParallaxTestLayer(sceneIdx);
    layer->autorelease();

    return layer;
}

Layer* restartParallaxAction()
{
    auto layer = createParallaxTestLayer(sceneIdx);
    layer->autorelease();

    return layer;
} 


ParallaxDemo::ParallaxDemo(void)
{
}

ParallaxDemo::~ParallaxDemo(void)
{
}

std::string ParallaxDemo::title()
{
    return "No title";
}

void ParallaxDemo::onEnter()
{
    BaseTest::onEnter();
}

void ParallaxDemo::restartCallback(Object* sender)
{
    auto s = new ParallaxTestScene();
    s->addChild(restartParallaxAction()); 

    Director::getInstance()->replaceScene(s);
    s->release();
}

void ParallaxDemo::nextCallback(Object* sender)
{
    auto s = new ParallaxTestScene();
    s->addChild( nextParallaxAction() );
    Director::getInstance()->replaceScene(s);
    s->release();
}

void ParallaxDemo::backCallback(Object* sender)
{
    auto s = new ParallaxTestScene();
    s->addChild( backParallaxAction() );
    Director::getInstance()->replaceScene(s);
    s->release();
} 

void ParallaxTestScene::runThisTest()
{
    auto layer = nextParallaxAction();

    addChild(layer);
    Director::getInstance()->replaceScene(this);
}
