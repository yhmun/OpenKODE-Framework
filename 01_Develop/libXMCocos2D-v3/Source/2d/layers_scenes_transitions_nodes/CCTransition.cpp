/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransition.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#include "2d/layers_scenes_transitions_nodes/CCTransition.h"
#include "2d/CCCamera.h"
#include "2d/CCDirector.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/actions/CCActionInstant.h"
#include "2d/actions/CCActionEase.h"
#include "2d/actions/CCActionCamera.h"
#include "2d/actions/CCActionTiledGrid.h"
#include "2d/actions/CCActionGrid.h"
#include "2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "2d/misc_nodes/CCRenderTexture.h"


NS_CC_BEGIN

const unsigned int kSceneFade = 0xFADEFADE;

TransitionScene::TransitionScene()
{
}
TransitionScene::~TransitionScene()
{
    m_pInScene->release();
    m_pOutScene->release();
}

TransitionScene * TransitionScene::create(float t, Scene *scene)
{
    TransitionScene * pScene = new TransitionScene();
    if(pScene && pScene->initWithDuration(t,scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

bool TransitionScene::initWithDuration(float t, Scene *scene)
{
    CCASSERT( scene != NULL, "Argument scene must be non-nil");

    if (Scene::init())
    {
        m_fDuration = t;

        // retain
        m_pInScene = scene;
        m_pInScene->retain();
        m_pOutScene = Director::getInstance()->getRunningScene();
        if (m_pOutScene == NULL)
        {
            m_pOutScene = Scene::create();
            m_pOutScene->init();
        }
        m_pOutScene->retain();

        CCASSERT( m_pInScene != m_pOutScene, "Incoming scene must be different from the outgoing scene" );
        
        sceneOrder();

        return true;
    }
    else
    {
        return false;
    }
}

void TransitionScene::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void TransitionScene::draw()
{
    Scene::draw();

    if( m_bIsInSceneOnTop ) {
        m_pOutScene->visit();
        m_pInScene->visit();
    } else {
        m_pInScene->visit();
        m_pOutScene->visit();
    }
}

void TransitionScene::finish()
{
    // clean up     
     m_pInScene->setVisible(true);
     m_pInScene->setPosition(Point(0,0));
     m_pInScene->setScale(1.0f);
     m_pInScene->setRotation(0.0f);
     m_pInScene->getCamera()->restore();
 
     m_pOutScene->setVisible(false);
     m_pOutScene->setPosition(Point(0,0));
     m_pOutScene->setScale(1.0f);
     m_pOutScene->setRotation(0.0f);
     m_pOutScene->getCamera()->restore();

    //[self schedule:@selector(setNewScene:) interval:0];
    this->schedule(schedule_selector(TransitionScene::setNewScene), 0);

}

void TransitionScene::setNewScene(float dt)
{    
    CC_UNUSED_PARAM(dt);

    this->unschedule(schedule_selector(TransitionScene::setNewScene));
    
    // Before replacing, save the "send cleanup to scene"
    Director *director = Director::getInstance();
    m_bIsSendCleanupToScene = director->isSendCleanupToScene();
    
    director->replaceScene(m_pInScene);
    
    // issue #267
    m_pOutScene->setVisible(true);
}

void TransitionScene::hideOutShowIn()
{
    m_pInScene->setVisible(true);
    m_pOutScene->setVisible(false);
}


// custom onEnter
void TransitionScene::onEnter()
{
    Scene::onEnter();
    
    // disable events while transitions
    m_pEventDispatcher->setEnabled(false);
    
    // outScene should not receive the onEnter callback
    // only the onExitTransitionDidStart
    m_pOutScene->onExitTransitionDidStart();
    
    m_pInScene->onEnter();
}

// custom onExit
void TransitionScene::onExit()
{
    Scene::onExit();
    
    // enable events while transitions
    m_pEventDispatcher->setEnabled(true);
    m_pOutScene->onExit();

    // _inScene should not receive the onEnter callback
    // only the onEnterTransitionDidFinish
    m_pInScene->onEnterTransitionDidFinish();
}

// custom cleanup
void TransitionScene::cleanup()
{
    Scene::cleanup();

    if( m_bIsSendCleanupToScene )
        m_pOutScene->cleanup();
}

//
// Oriented Transition
//

TransitionSceneOriented::TransitionSceneOriented()
{
}

TransitionSceneOriented::~TransitionSceneOriented()
{
}

TransitionSceneOriented * TransitionSceneOriented::create(float t, Scene *scene, Orientation orientation)
{
    TransitionSceneOriented * pScene = new TransitionSceneOriented();
    pScene->initWithDuration(t,scene,orientation);
    pScene->autorelease();
    return pScene;
}

bool TransitionSceneOriented::initWithDuration(float t, Scene *scene, Orientation orientation)
{
    if ( TransitionScene::initWithDuration(t, scene) )
    {
        m_eOrientation = orientation;
    }
    return true;
}

//
// RotoZoom
//
TransitionRotoZoom::TransitionRotoZoom()
{
}

TransitionRotoZoom* TransitionRotoZoom::create(float t, Scene* scene)                   
{                                                               
    TransitionRotoZoom* pScene = new TransitionRotoZoom();                                
    if(pScene && pScene->initWithDuration(t, scene))            
    {                                                           
        pScene->autorelease();                                  
        return pScene;                                          
    }                                                           
    CC_SAFE_DELETE(pScene);                                     
    return NULL;                                                
}

TransitionRotoZoom::~TransitionRotoZoom()
{
}

void TransitionRotoZoom:: onEnter()
{
    TransitionScene::onEnter();

    m_pInScene->setScale(0.001f);
    m_pOutScene->setScale(1.0f);

    m_pInScene->setAnchorPoint(Point(0.5f, 0.5f));
    m_pOutScene->setAnchorPoint(Point(0.5f, 0.5f));

    ActionInterval *rotozoom = (ActionInterval*)(Sequence::create
    (
        Spawn::create
        (
            ScaleBy::create(m_fDuration/2, 0.001f),
            RotateBy::create(m_fDuration/2, 360 * 2),
            NULL
        ),
        DelayTime::create(m_fDuration/2),
        NULL
    ));

    m_pOutScene->runAction(rotozoom);
    m_pInScene->runAction
    (
        Sequence::create
        (
            rotozoom->reverse(),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        )
    );
}

//
// JumpZoom
//
TransitionJumpZoom::TransitionJumpZoom()
{
}
TransitionJumpZoom::~TransitionJumpZoom()
{
}

TransitionJumpZoom* TransitionJumpZoom::create(float t, Scene* scene)
{
    TransitionJumpZoom* pScene = new TransitionJumpZoom();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionJumpZoom::onEnter()
{
    TransitionScene::onEnter();
    Size s = Director::getInstance()->getWinSize();

    m_pInScene->setScale(0.5f);
    m_pInScene->setPosition(Point(s.width, 0));
    m_pInScene->setAnchorPoint(Point(0.5f, 0.5f));
    m_pOutScene->setAnchorPoint(Point(0.5f, 0.5f));

    ActionInterval *jump = JumpBy::create(m_fDuration/4, Point(-s.width,0), s.width/4, 2);
    ActionInterval *scaleIn = ScaleTo::create(m_fDuration/4, 1.0f);
    ActionInterval *scaleOut = ScaleTo::create(m_fDuration/4, 0.5f);

    ActionInterval *jumpZoomOut = (ActionInterval*)(Sequence::create(scaleOut, jump, NULL));
    ActionInterval *jumpZoomIn = (ActionInterval*)(Sequence::create(jump, scaleIn, NULL));

    ActionInterval *delay = DelayTime::create(m_fDuration/2);

    m_pOutScene->runAction(jumpZoomOut);
    m_pInScene->runAction
    (
        Sequence::create
        (
            delay,
            jumpZoomIn,
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        )
    );
}

//
// MoveInL
//
TransitionMoveInL::TransitionMoveInL()
{
}

TransitionMoveInL::~TransitionMoveInL()
{
}

TransitionMoveInL* TransitionMoveInL::create(float t, Scene* scene)
{
    TransitionMoveInL* pScene = new TransitionMoveInL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionMoveInL::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();

    ActionInterval *a = this->action();

    m_pInScene->runAction
    (
        Sequence::create
        (
            this->easeActionWithAction(a),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        )
    );
}
 
ActionInterval* TransitionMoveInL::action()
{
    return MoveTo::create(m_fDuration, Point(0,0));
}

ActionInterval* TransitionMoveInL::easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
//    return [EaseElasticOut actionWithAction:action period:0.4f];
}

void TransitionMoveInL::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition(Point(-s.width,0));
}

//
// MoveInR
//
TransitionMoveInR::TransitionMoveInR()
{
}
TransitionMoveInR::~TransitionMoveInR()
{
}

TransitionMoveInR* TransitionMoveInR::create(float t, Scene* scene)
{
    TransitionMoveInR* pScene = new TransitionMoveInR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionMoveInR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(s.width,0) );
}

//
// MoveInT
//
TransitionMoveInT::TransitionMoveInT()
{
}
TransitionMoveInT::~TransitionMoveInT()
{
}

TransitionMoveInT* TransitionMoveInT::create(float t, Scene* scene)
{
    TransitionMoveInT* pScene = new TransitionMoveInT();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionMoveInT::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(0,s.height) );
}

//
// MoveInB
//
TransitionMoveInB::TransitionMoveInB()
{
}
TransitionMoveInB::~TransitionMoveInB()
{
}

TransitionMoveInB* TransitionMoveInB::create(float t, Scene* scene)
{
    TransitionMoveInB* pScene = new TransitionMoveInB();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionMoveInB::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(0,-s.height) );
}


//
// SlideInL
//

// The adjust factor is needed to prevent issue #442
// One solution is to use DONT_RENDER_IN_SUBPIXELS images, but NO
// The other issue is that in some transitions (and I don't know why)
// the order should be reversed (In in top of Out or vice-versa).
#define ADJUST_FACTOR 0.5f
TransitionSlideInL::TransitionSlideInL()
{
}

TransitionSlideInL::~TransitionSlideInL()
{
}

void TransitionSlideInL::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();

    ActionInterval *in = this->action();
    ActionInterval *out = this->action();

    ActionInterval* inAction = easeActionWithAction(in);
    ActionInterval* outAction = (ActionInterval*)Sequence::create
    (
        easeActionWithAction(out),
        CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
        NULL
    );
    m_pInScene->runAction(inAction);
    m_pOutScene->runAction(outAction);
}

void TransitionSlideInL::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void TransitionSlideInL:: initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(-(s.width-ADJUST_FACTOR),0) );
}

ActionInterval* TransitionSlideInL::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(m_fDuration, Point(s.width-ADJUST_FACTOR,0));
}

ActionInterval* TransitionSlideInL::easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
}

TransitionSlideInL* TransitionSlideInL::create(float t, Scene* scene)
{
    TransitionSlideInL* pScene = new TransitionSlideInL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// SlideInR
//
TransitionSlideInR::TransitionSlideInR()
{
}
TransitionSlideInR::~TransitionSlideInR()
{
}

TransitionSlideInR* TransitionSlideInR::create(float t, Scene* scene)
{
    TransitionSlideInR* pScene = new TransitionSlideInR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionSlideInR::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void TransitionSlideInR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(s.width-ADJUST_FACTOR,0) );
}


ActionInterval* TransitionSlideInR:: action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(m_fDuration, Point(-(s.width-ADJUST_FACTOR),0));
}


//
// SlideInT
//
TransitionSlideInT::TransitionSlideInT()
{
}
TransitionSlideInT::~TransitionSlideInT()
{
}

TransitionSlideInT* TransitionSlideInT::create(float t, Scene* scene)
{
    TransitionSlideInT* pScene = new TransitionSlideInT();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionSlideInT::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void TransitionSlideInT::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(0,s.height-ADJUST_FACTOR) );
}


ActionInterval* TransitionSlideInT::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(m_fDuration, Point(0,-(s.height-ADJUST_FACTOR)));
}

//
// SlideInB
//
TransitionSlideInB::TransitionSlideInB()
{
}
TransitionSlideInB::~TransitionSlideInB()
{
}

TransitionSlideInB* TransitionSlideInB::create(float t, Scene* scene)
{
    TransitionSlideInB* pScene = new TransitionSlideInB();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionSlideInB::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void TransitionSlideInB:: initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    m_pInScene->setPosition( Point(0,-(s.height-ADJUST_FACTOR)) );
}


ActionInterval* TransitionSlideInB:: action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(m_fDuration, Point(0,s.height-ADJUST_FACTOR));
}

//
// ShrinkGrow Transition
//
TransitionShrinkGrow::TransitionShrinkGrow()
{
}
TransitionShrinkGrow::~TransitionShrinkGrow()
{
}

TransitionShrinkGrow* TransitionShrinkGrow::create(float t, Scene* scene)
{
    TransitionShrinkGrow* pScene = new TransitionShrinkGrow();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionShrinkGrow::onEnter()
{
    TransitionScene::onEnter();

    m_pInScene->setScale(0.001f);
    m_pOutScene->setScale(1.0f);

    m_pInScene->setAnchorPoint(Point(2/3.0f,0.5f));
    m_pOutScene->setAnchorPoint(Point(1/3.0f,0.5f));    

    ActionInterval* scaleOut = ScaleTo::create(m_fDuration, 0.01f);
    ActionInterval* scaleIn = ScaleTo::create(m_fDuration, 1.0f);

    m_pInScene->runAction(this->easeActionWithAction(scaleIn));
    m_pOutScene->runAction
    (
        Sequence::create
        (
            this->easeActionWithAction(scaleOut),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        )
    );
}
ActionInterval* TransitionShrinkGrow:: easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
//    return [EaseElasticOut actionWithAction:action period:0.3f];
}

//
// FlipX Transition
//
TransitionFlipX::TransitionFlipX()
{
}
TransitionFlipX::~TransitionFlipX()
{
}

void TransitionFlipX::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation == TransitionScene::Orientation::RIGHT_OVER )
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    }
    else
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    inA = (ActionInterval*)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Show::create(),
            OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );

    outA = (ActionInterval *)Sequence::create
        (
            OrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
            Hide::create(),
            DelayTime::create(m_fDuration/2),                            
            NULL 
        );

    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);
}

TransitionFlipX* TransitionFlipX::create(float t, Scene* s, Orientation o)
{
    TransitionFlipX* pScene = new TransitionFlipX();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionFlipX* TransitionFlipX::create(float t, Scene* s)
{
    return TransitionFlipX::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// FlipY Transition
//
TransitionFlipY::TransitionFlipY()
{
}
TransitionFlipY::~TransitionFlipY()
{
}

void TransitionFlipY::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation == TransitionScene::Orientation::UP_OVER ) 
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    inA = (ActionInterval*)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Show::create(),
            OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );
    outA = (ActionInterval*)Sequence::create
        (
            OrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0),
            Hide::create(),
            DelayTime::create(m_fDuration/2),                            
            NULL
        );

    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);

}

TransitionFlipY* TransitionFlipY::create(float t, Scene* s, Orientation o)
{
    TransitionFlipY* pScene = new TransitionFlipY();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionFlipY* TransitionFlipY::create(float t, Scene* s)
{
    return TransitionFlipY::create(t, s, TransitionScene::Orientation::UP_OVER);
}

//
// FlipAngular Transition
//
TransitionFlipAngular::TransitionFlipAngular()
{
}

TransitionFlipAngular::~TransitionFlipAngular()
{
}

void TransitionFlipAngular::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation == TransitionScene::Orientation::RIGHT_OVER ) 
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    inA = (ActionInterval *)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Show::create(),
            OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );
    outA = (ActionInterval *)Sequence::create
        (
            OrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 45, 0),
            Hide::create(),
            DelayTime::create(m_fDuration/2),                            
            NULL
        );

    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);
}

TransitionFlipAngular* TransitionFlipAngular::create(float t, Scene* s, Orientation o)
{
    TransitionFlipAngular* pScene = new TransitionFlipAngular();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionFlipAngular* TransitionFlipAngular::create(float t, Scene* s)
{
    return TransitionFlipAngular::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// ZoomFlipX Transition
//
TransitionZoomFlipX::TransitionZoomFlipX()
{
}
TransitionZoomFlipX::~TransitionZoomFlipX()
{
}

void TransitionZoomFlipX::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation == TransitionScene::Orientation::RIGHT_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }
    inA = (ActionInterval *)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),
                ScaleTo::create(m_fDuration/2, 1),
                Show::create(),
                NULL
            ),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );
    outA = (ActionInterval *)Sequence::create
        (
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
                ScaleTo::create(m_fDuration/2, 0.5f),
                NULL
            ),
            Hide::create(),
            DelayTime::create(m_fDuration/2),                            
            NULL
        );

    m_pInScene->setScale(0.5f);
    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);
}

TransitionZoomFlipX* TransitionZoomFlipX::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipX* pScene = new TransitionZoomFlipX();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionZoomFlipX* TransitionZoomFlipX::create(float t, Scene* s)
{
    return TransitionZoomFlipX::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// ZoomFlipY Transition
//
TransitionZoomFlipY::TransitionZoomFlipY()
{
}

TransitionZoomFlipY::~TransitionZoomFlipY()
{
}

void TransitionZoomFlipY::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation== TransitionScene::Orientation::UP_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } else {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    inA = (ActionInterval *)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0),
                ScaleTo::create(m_fDuration/2, 1),
                Show::create(),
                NULL
            ),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );

    outA = (ActionInterval *)Sequence::create
        (
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0),
                ScaleTo::create(m_fDuration/2, 0.5f),
                NULL
            ),                            
            Hide::create(),
            DelayTime::create(m_fDuration/2),
            NULL
        );

    m_pInScene->setScale(0.5f);
    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);
}

TransitionZoomFlipY* TransitionZoomFlipY::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipY* pScene = new TransitionZoomFlipY();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionZoomFlipY* TransitionZoomFlipY::create(float t, Scene* s)
{
    return TransitionZoomFlipY::create(t, s, TransitionScene::Orientation::UP_OVER);
}

//
// ZoomFlipAngular Transition
//
TransitionZoomFlipAngular::TransitionZoomFlipAngular()
{
}
TransitionZoomFlipAngular::~TransitionZoomFlipAngular()
{
}


void TransitionZoomFlipAngular::onEnter()
{
    TransitionSceneOriented::onEnter();

    ActionInterval *inA, *outA;
    m_pInScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( m_eOrientation == TransitionScene::Orientation::RIGHT_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    inA = (ActionInterval *)Sequence::create
        (
            DelayTime::create(m_fDuration/2),
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0),
                ScaleTo::create(m_fDuration/2, 1),
                Show::create(),
                NULL
            ),
            Show::create(),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            NULL
        );
    outA = (ActionInterval *)Sequence::create
        (
            Spawn::create
            (
                OrbitCamera::create(m_fDuration/2, 1, 0 , outAngleZ, outDeltaZ, 45, 0),
                ScaleTo::create(m_fDuration/2, 0.5f),
                NULL
            ),                            
            Hide::create(),
            DelayTime::create(m_fDuration/2),                            
            NULL
        );

    m_pInScene->setScale(0.5f);
    m_pInScene->runAction(inA);
    m_pOutScene->runAction(outA);
}

TransitionZoomFlipAngular* TransitionZoomFlipAngular::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipAngular* pScene = new TransitionZoomFlipAngular();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

TransitionZoomFlipAngular* TransitionZoomFlipAngular::create(float t, Scene* s)
{
    return TransitionZoomFlipAngular::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// Fade Transition
//
TransitionFade::TransitionFade()
{
}
TransitionFade::~TransitionFade()
{
}

TransitionFade * TransitionFade::create(float duration, Scene *scene, const Color3B& color)
{
    TransitionFade * pTransition = new TransitionFade();
    pTransition->initWithDuration(duration, scene, color);
    pTransition->autorelease();
    return pTransition;
}

TransitionFade* TransitionFade::create(float duration,Scene* scene)
{
    return TransitionFade::create(duration, scene, Color3B::BLACK);
}

bool TransitionFade::initWithDuration(float duration, Scene *scene, const Color3B& color)
{
    if (TransitionScene::initWithDuration(duration, scene))
    {
        m_tColor.r = color.r;
        m_tColor.g = color.g;
        m_tColor.b = color.b;
        m_tColor.a = 0;
    }
    return true;
}

bool TransitionFade::initWithDuration(float t, Scene *scene)
{
    this->initWithDuration(t, scene, Color3B::BLACK);
    return true;
}

void TransitionFade :: onEnter()
{
    TransitionScene::onEnter();

    LayerColor* l = LayerColor::create(m_tColor);
    m_pInScene->setVisible(false);

    addChild(l, 2, kSceneFade);
    Node* f = getChildByTag(kSceneFade);

    ActionInterval* a = (ActionInterval *)Sequence::create
        (
            FadeIn::create(m_fDuration/2),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this)),
            FadeOut::create(m_fDuration/2),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),

         NULL
        );
    f->runAction(a);
}

void TransitionFade::onExit()
{
    TransitionScene::onExit();
    this->removeChildByTag(kSceneFade, false);
}

//
// Cross Fade Transition
//
TransitionCrossFade::TransitionCrossFade()
{
}
TransitionCrossFade::~TransitionCrossFade()
{
}

TransitionCrossFade* TransitionCrossFade::create(float t, Scene* scene)
{
    TransitionCrossFade* pScene = new TransitionCrossFade();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionCrossFade:: draw()
{
    // override draw since both scenes (textures) are rendered in 1 scene
}

void TransitionCrossFade::onEnter()
{
    TransitionScene::onEnter();

    // create a transparent color layer
    // in which we are going to add our rendertextures
    Color4B  color(0,0,0,0);
    Size size = Director::getInstance()->getWinSize();
    LayerColor* layer = LayerColor::create(color);

    // create the first render texture for inScene
    RenderTexture* inTexture = RenderTexture::create((int)size.width, (int)size.height);

    if (NULL == inTexture)
    {
        return;
    }

    inTexture->getSprite()->setAnchorPoint( Point(0.5f,0.5f) );
    inTexture->setPosition( Point(size.width/2, size.height/2) );
    inTexture->setAnchorPoint( Point(0.5f,0.5f) );

    // render inScene to its texturebuffer
    inTexture->begin();
    m_pInScene->visit();
    inTexture->end();

    // create the second render texture for outScene
    RenderTexture* outTexture = RenderTexture::create((int)size.width, (int)size.height);
    outTexture->getSprite()->setAnchorPoint( Point(0.5f,0.5f) );
    outTexture->setPosition( Point(size.width/2, size.height/2) );
    outTexture->setAnchorPoint( Point(0.5f,0.5f) );

    // render outScene to its texturebuffer
    outTexture->begin();
    m_pOutScene->visit();
    outTexture->end();

    // create blend functions

    BlendFunc blend1 = {GL_ONE, GL_ONE}; // inScene will lay on background and will not be used with alpha
    BlendFunc blend2 = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}; // we are going to blend outScene via alpha 

    // set blendfunctions
    inTexture->getSprite()->setBlendFunc(blend1);
    outTexture->getSprite()->setBlendFunc(blend2);    

    // add render textures to the layer
    layer->addChild(inTexture);
    layer->addChild(outTexture);

    // initial opacity:
    inTexture->getSprite()->setOpacity(255);
    outTexture->getSprite()->setOpacity(255);

    // create the blend action
    Action* layerAction = Sequence::create
    (
        FadeTo::create(m_fDuration, 0),
        CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this)),
        CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
        NULL
    );


    // run the blend action
    outTexture->getSprite()->runAction( layerAction );

    // add the layer (which contains our two rendertextures) to the scene
    addChild(layer, 2, kSceneFade);
}

// clean up on exit
void TransitionCrossFade::onExit()
{
    // remove our layer and release all containing objects 
    this->removeChildByTag(kSceneFade, false);
    TransitionScene::onExit();
}

//
// TurnOffTilesTransition
//
TransitionTurnOffTiles::TransitionTurnOffTiles()
{
}

TransitionTurnOffTiles::~TransitionTurnOffTiles()
{
}

TransitionTurnOffTiles* TransitionTurnOffTiles::create(float t, Scene* scene)
{
    TransitionTurnOffTiles* pScene = new TransitionTurnOffTiles();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

// override addScenes, and change the order
void TransitionTurnOffTiles::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void TransitionTurnOffTiles::onEnter()
{
    TransitionScene::onEnter();
    Size s = Director::getInstance()->getWinSize();
    float aspect = s.width / s.height;
    int x = (int)(12 * aspect);
    int y = 12;

    TurnOffTiles* toff = TurnOffTiles::create(m_fDuration, Size(x,y));
    ActionInterval* action = easeActionWithAction(toff);
    m_pOutScene->runAction
    (
        Sequence::create
        (
            action,
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            StopGrid::create(),
            NULL
        )
    );
}


ActionInterval* TransitionTurnOffTiles:: easeActionWithAction(ActionInterval* action)
{
    return action;
}

//
// SplitCols Transition
//
TransitionSplitCols::TransitionSplitCols()
{
}
TransitionSplitCols::~TransitionSplitCols()
{
}

TransitionSplitCols* TransitionSplitCols::create(float t, Scene* scene)
{
    TransitionSplitCols* pScene = new TransitionSplitCols();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionSplitCols::onEnter()
{
    TransitionScene::onEnter();
    m_pInScene->setVisible(false);

    ActionInterval* split = action();
    ActionInterval* seq = (ActionInterval*)Sequence::create
    (
        split,
        CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this)),
        split->reverse(),
        NULL
    );

    this->runAction
    ( 
        Sequence::create
        (
            easeActionWithAction(seq),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            StopGrid::create(),
            NULL
        )
    );
}

ActionInterval* TransitionSplitCols:: action()
{
    return SplitCols::create(m_fDuration/2.0f, 3);
}


ActionInterval* TransitionSplitCols::easeActionWithAction(ActionInterval * action)
{
    return EaseInOut::create(action, 3.0f);
}


//
// SplitRows Transition
//
TransitionSplitRows::TransitionSplitRows()
{
}

TransitionSplitRows::~TransitionSplitRows()
{
}

ActionInterval* TransitionSplitRows::action()
{
    return SplitRows::create(m_fDuration/2.0f, 3);
}

TransitionSplitRows* TransitionSplitRows::create(float t, Scene* scene)
{
    TransitionSplitRows* pScene = new TransitionSplitRows();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// FadeTR Transition
//
TransitionFadeTR::TransitionFadeTR()
{
}
TransitionFadeTR::~TransitionFadeTR()
{
}

TransitionFadeTR* TransitionFadeTR::create(float t, Scene* scene)
{
    TransitionFadeTR* pScene = new TransitionFadeTR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void TransitionFadeTR::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void TransitionFadeTR::onEnter()
{
    TransitionScene::onEnter();

    Size s = Director::getInstance()->getWinSize();
    float aspect = s.width / s.height;
    int x = (int)(12 * aspect);
    int y = 12;

    ActionInterval* action  = actionWithSize(Size(x,y));

    m_pOutScene->runAction
    (
        Sequence::create
        (
            easeActionWithAction(action),
            CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
            StopGrid::create(),
            NULL
        )
    );
}


ActionInterval*  TransitionFadeTR::actionWithSize(const Size& size)
{
    return FadeOutTRTiles::create(m_fDuration, size);
}

ActionInterval* TransitionFadeTR:: easeActionWithAction(ActionInterval* action)
{
    return action;
}


//
// FadeBL Transition
//

TransitionFadeBL::TransitionFadeBL()
{
}
TransitionFadeBL::~TransitionFadeBL()
{
}

TransitionFadeBL* TransitionFadeBL::create(float t, Scene* scene)
{
    TransitionFadeBL* pScene = new TransitionFadeBL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

ActionInterval*  TransitionFadeBL::actionWithSize(const Size& size)
{
    return FadeOutBLTiles::create(m_fDuration, size);
}

//
// FadeUp Transition
//
TransitionFadeUp::TransitionFadeUp()
{
}

TransitionFadeUp::~TransitionFadeUp()
{
}

TransitionFadeUp* TransitionFadeUp::create(float t, Scene* scene)
{
    TransitionFadeUp* pScene = new TransitionFadeUp();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

ActionInterval* TransitionFadeUp::actionWithSize(const Size& size)
{
    return FadeOutUpTiles::create(m_fDuration, size);
}

//
// FadeDown Transition
//
TransitionFadeDown::TransitionFadeDown()
{
}
TransitionFadeDown::~TransitionFadeDown()
{
}

TransitionFadeDown* TransitionFadeDown::create(float t, Scene* scene)
{
    TransitionFadeDown* pScene = new TransitionFadeDown();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

ActionInterval* TransitionFadeDown::actionWithSize(const Size& size)
{
    return FadeOutDownTiles::create(m_fDuration, size);
}

NS_CC_END
