/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransition.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "layers_scenes_transitions_nodes/CCTransition.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
#include "actions/CCActionCamera.h"
#include "actions/CCActionGrid.h"
#include "actions/CCActionTiledGrid.h"
#include "misc_nodes/CCRenderTexture.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "support/CCCamera.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
const KDuint kSceneFade = 0xFADEFADE;

CCTransitionScene::CCTransitionScene ( KDvoid )
{
	m_pInScene  = KD_NULL;
	m_pOutScene = KD_NULL;
}

CCTransitionScene::~CCTransitionScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pInScene );
	CC_SAFE_RELEASE ( m_pOutScene );
}

CCTransitionScene* CCTransitionScene::create ( KDfloat fDuration, CCScene* pScene )
{
	CCTransitionScene*  pRet = new CCTransitionScene ( );

	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTransitionScene::initWithDuration ( KDfloat fDuration, CCScene* pScene )
{
	CCAssert ( pScene != KD_NULL, "Argument scene must be non-nil" );

	if ( CCScene::init ( ) )
	{
		m_fDuration = fDuration;

		// retain
		m_pInScene = pScene;
		m_pInScene->retain ( );

		m_pOutScene = CCDirector::sharedDirector ( )->getRunningScene ( );
		if ( m_pOutScene == KD_NULL )
        {
            m_pOutScene = CCScene::create ( );
            m_pOutScene->init ( );
        }
		m_pOutScene->retain ( );

		CCAssert ( m_pInScene != m_pOutScene, "Incoming scene must be different from the outgoing scene" );

		sceneOrder ( );

		return KD_TRUE;
	}
	
	return KD_FALSE;	
}

KDvoid CCTransitionScene::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_TRUE;
}

KDvoid CCTransitionScene::draw ( KDvoid )
{
	CCScene::draw ( );

	if ( m_bIsInSceneOnTop )
	{
		m_pOutScene->visit ( );
		m_pInScene ->visit ( );
	} 
	else 
	{
		m_pInScene ->visit ( );
		m_pOutScene->visit ( );
	}
}

KDvoid CCTransitionScene::finish ( KDvoid )
{
	// clean up 	
 	m_pInScene->setVisible ( KD_TRUE );
 	m_pInScene->setPosition ( ccp ( 0, 0 ) );
 	m_pInScene->setScale ( 1.0f );
 	m_pInScene->setRotation ( 0.0f );
 	m_pInScene->getCamera ( )->restore ( );
 
 	m_pOutScene->setVisible ( KD_FALSE );
 	m_pOutScene->setPosition ( ccp ( 0, 0 ) );
 	m_pOutScene->setScale ( 1.0f );
 	m_pOutScene->setRotation ( 0.0f );
 	m_pOutScene->getCamera ( )->restore ( );

	this->schedule ( schedule_selector ( CCTransitionScene::setNewScene ), 0 );
}

KDvoid CCTransitionScene::setNewScene ( KDfloat fDelta )
{	
	CC_UNUSED_PARAM ( fDelta );

	this->unschedule ( schedule_selector ( CCTransitionScene::setNewScene ) );
	
	// Before replacing, save the "send cleanup to scene"
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	m_bIsSendCleanupToScene = pDirector->isSendCleanupToScene ( );
	pDirector->replaceScene ( m_pInScene );

	// issue #267
	m_pOutScene->setVisible ( KD_TRUE );
}

KDvoid CCTransitionScene::hideOutShowIn ( KDvoid )
{
	m_pInScene ->setVisible ( KD_TRUE );
	m_pOutScene->setVisible ( KD_FALSE );
}

KDvoid CCTransitionScene::onEnter ( KDvoid )
{	
	CCScene::onEnter ( );

	// disable events while transitions
    CCDirector::sharedDirector ( )->getTouchDispatcher ( )->setDispatchEvents ( KD_FALSE );

	// only the onExitTransitionDidStart
	m_pOutScene->onExitTransitionDidStart ( );

	m_pInScene->onEnter ( );
}

KDvoid CCTransitionScene::onExit ( KDvoid )
{
	CCScene::onExit ( );

	// enable events while transitions
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->setDispatchEvents ( KD_TRUE );

	m_pOutScene->onExit ( );

	// m_pInScene should not receive the onExit callback
	// only the onEnterTransitionDidFinish
	m_pInScene->onEnterTransitionDidFinish ( );
}

KDvoid CCTransitionScene::cleanup ( KDvoid )
{
	CCScene::cleanup ( );

	if ( m_bIsSendCleanupToScene )
	{
		m_pOutScene->cleanup ( );
	}
}


//
// Oriented Transition
//

CCTransitionSceneOriented* CCTransitionSceneOriented::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
	CCTransitionSceneOriented*  pRet = new CCTransitionSceneOriented ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCTransitionSceneOriented::initWithDuration ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
	if ( CCTransitionScene::initWithDuration ( fDuration, pScene ) )
	{
		m_eOrientation = eOrientation;
	}
    
	return KD_TRUE;
}


//
// RotoZoom
//

CCTransitionRotoZoom* CCTransitionRotoZoom::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionRotoZoom*  pRet = new CCTransitionRotoZoom ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionRotoZoom:: onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	m_pInScene ->setScale ( 0.001f );
	m_pOutScene->setScale ( 1.0f );

	m_pInScene ->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	m_pOutScene->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

	CCActionInterval*  pRotZoom = (CCActionInterval *) ( CCSequence::create
    (
        CCSpawn::create
        (
            CCScaleBy ::create ( m_fDuration / 2, 0.001f ),
            CCRotateBy::create ( m_fDuration / 2, 360 * 2 ),
            KD_NULL
        ),
        CCDelayTime::create ( m_fDuration / 2 ),
        KD_NULL
    ) );

	m_pOutScene->runAction ( pRotZoom );
	m_pInScene ->runAction
	(
		CCSequence::create
		(
			pRotZoom->reverse ( ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
			KD_NULL
		)
	);
}


//
// JumpZoom
//

CCTransitionJumpZoom* CCTransitionJumpZoom::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionJumpZoom*  pRet = new CCTransitionJumpZoom ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionJumpZoom::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );

	m_pInScene ->setScale ( 0.5f );
	m_pInScene ->setPosition ( ccp ( tSize.cx, 0 ) );
	m_pInScene ->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	m_pOutScene->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

   	m_pInScene->runAction
	(
        CCSequence::create
        (
            CCDelayTime	::create ( m_fDuration / 2 ),
            CCJumpBy	::create ( m_fDuration / 4, ccp ( -tSize.cx, 0 ), tSize.cx / 4, 2 ),
            CCScaleTo	::create ( m_fDuration / 4, 1.0f ),
            CCCallFunc	::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
            KD_NULL
        )
    );
    
	m_pOutScene->runAction 
    (
        CCSequence::create 
        (
            CCScaleTo::create ( m_fDuration / 4, 0.5f ), 
            CCJumpBy ::create ( m_fDuration / 4, ccp ( -tSize.cx, 0 ), tSize.cx / 4, 2 ),
            KD_NULL
        )
    );
}


//
// MoveInL
//

CCTransitionMoveInL* CCTransitionMoveInL::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionMoveInL*  pRet = new CCTransitionMoveInL ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionMoveInL::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	this->initScenes ( );

	m_pInScene->runAction
	(
		CCSequence::create
		(
			this->easeActionWithAction ( this->action ( ) ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
			KD_NULL
		)
	);
}
 
CCActionInterval* CCTransitionMoveInL::action ( KDvoid )
{
	return CCMoveTo::create ( m_fDuration, ccp ( 0, 0 ) );
}

CCActionInterval* CCTransitionMoveInL::easeActionWithAction ( CCActionInterval* pAction )
{
	return CCEaseOut::create ( pAction, 2.0f );
}

KDvoid CCTransitionMoveInL::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( -CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}


//
// MoveInR
//

CCTransitionMoveInR* CCTransitionMoveInR::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionMoveInR*  pRet = new CCTransitionMoveInR ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionMoveInR::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}


//
// MoveInT
//

CCTransitionMoveInT* CCTransitionMoveInT::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionMoveInT*  pRet = new CCTransitionMoveInT ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionMoveInT::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( 0, CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}


//
// MoveInB
//

CCTransitionMoveInB* CCTransitionMoveInB::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionMoveInB*  pRet = new CCTransitionMoveInB ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionMoveInB::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( 0, -CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}


//
// SlideInL
//

CCTransitionSlideInL* CCTransitionSlideInL::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSlideInL*  pRet = new CCTransitionSlideInL ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionSlideInL::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );
	
    this->initScenes ( );

	m_pInScene ->runAction 
    (
        easeActionWithAction ( this->action ( ) )
    );
    
	m_pOutScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            easeActionWithAction ( this->action ( ) ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
            KD_NULL
        )
    );
}

KDvoid CCTransitionSlideInL::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionSlideInL::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( -CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}

CCActionInterval* CCTransitionSlideInL::action ( KDvoid )
{
	return CCMoveBy::create ( m_fDuration, ccp ( CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}

CCActionInterval* CCTransitionSlideInL::easeActionWithAction ( CCActionInterval* pAction )
{
	return CCEaseOut::create ( pAction, 2.0f );
}


//
// SlideInR
//

CCTransitionSlideInR* CCTransitionSlideInR::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSlideInR*  pRet = new CCTransitionSlideInR ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionSlideInR::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_TRUE;
}

KDvoid CCTransitionSlideInR::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}

CCActionInterval* CCTransitionSlideInR::action ( KDvoid )
{
	return CCMoveBy::create ( m_fDuration, ccp ( -CCDirector::sharedDirector ( )->getWinSize ( ).cx, 0 ) );
}

//
// SlideInT
//

CCTransitionSlideInT* CCTransitionSlideInT::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSlideInT*  pRet = new CCTransitionSlideInT ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionSlideInT::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionSlideInT::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( 0, CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}

CCActionInterval* CCTransitionSlideInT::action ( KDvoid )
{
	return CCMoveBy::create ( m_fDuration, ccp ( 0, -CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}

//
// SlideInB
//

CCTransitionSlideInB* CCTransitionSlideInB::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSlideInB*  pRet = new CCTransitionSlideInB ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionSlideInB::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_TRUE;
}

KDvoid CCTransitionSlideInB::initScenes ( KDvoid )
{
	m_pInScene->setPosition ( ccp ( 0, -CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}

CCActionInterval* CCTransitionSlideInB::action ( KDvoid )
{
	return CCMoveBy::create ( m_fDuration, ccp ( 0, CCDirector::sharedDirector ( )->getWinSize ( ).cy ) );
}

//
// ShrinkGrow Transition
//

CCTransitionShrinkGrow* CCTransitionShrinkGrow::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionShrinkGrow*  pRet = new CCTransitionShrinkGrow ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionShrinkGrow::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	m_pInScene ->setScale ( 0.001f );
	m_pOutScene->setScale ( 1.0f );

	m_pInScene ->setAnchorPoint ( ccp ( 2 / 3.0f, 0.5f ) );
	m_pOutScene->setAnchorPoint ( ccp ( 1 / 3.0f, 0.5f ) );	

	m_pInScene ->runAction 
    (
        this->easeActionWithAction ( CCScaleTo::create ( m_fDuration, 1.0f ) )
    );
    
	m_pOutScene->runAction
	(
		CCSequence::create
		(
			this->easeActionWithAction ( CCScaleTo::create ( m_fDuration, 0.01f ) ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
			KD_NULL
		)
	);
}

CCActionInterval* CCTransitionShrinkGrow::easeActionWithAction ( CCActionInterval* pAction )
{
	return CCEaseOut::create ( pAction, 2.0f );
}

//
// FlipX Transition
//

KDvoid CCTransitionFlipX::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat   inDeltaZ,  inAngleZ;
	KDfloat  outDeltaZ, outAngleZ;

	if ( m_eOrientation == kCCTransitionOrientationRightOver )
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	}
	else
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}

    m_pInScene->setVisible ( KD_FALSE );
	m_pInScene->runAction 
    (
        (CCActionInterval*) CCSequence::create
        (
            CCDelayTime	 ::create ( m_fDuration / 2 ),
            CCShow		 ::create ( ),
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, 0, 0 ),
            CCCallFunc	 ::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
            KD_NULL
        )
	);

	m_pOutScene->runAction
    (
        (CCActionInterval *) CCSequence::create
        (
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, outAngleZ, outDeltaZ, 0, 0 ),
            CCHide		 ::create ( ),
            CCDelayTime	 ::create ( m_fDuration / 2 ),							
            KD_NULL 
        )
	);
}

CCTransitionFlipX* CCTransitionFlipX::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionFlipX::create ( fDuration, pScene, kCCTransitionOrientationRightOver );
}

CCTransitionFlipX* CCTransitionFlipX::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionFlipX*  pRet = new CCTransitionFlipX ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

//
// FlipY Transition
//

KDvoid CCTransitionFlipY::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat   inDeltaZ,  inAngleZ;
	KDfloat  outDeltaZ, outAngleZ;

	if ( m_eOrientation == kCCTransitionOrientationUpOver ) 
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	} 
	else 
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}

    m_pInScene->setVisible ( KD_FALSE );
	m_pInScene->runAction 
    ( 
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime	 ::create ( m_fDuration / 2 ),
            CCShow		 ::create ( ),
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, 90, 0 ),
            CCCallFunc	 ::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
            KD_NULL
        )
	);
    
	m_pOutScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, outAngleZ, outDeltaZ, 90, 0 ),
            CCHide		 ::create ( ),
            CCDelayTime	 ::create ( m_fDuration / 2 ),							
            KD_NULL
        )
	);
}

CCTransitionFlipY* CCTransitionFlipY::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionFlipY::create ( fDuration, pScene, kCCTransitionOrientationUpOver );
}

CCTransitionFlipY* CCTransitionFlipY::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionFlipY*  pRet = new CCTransitionFlipY ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

//
// FlipAngular Transition
//

KDvoid CCTransitionFlipAngular::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat   inDeltaZ,  inAngleZ;
	KDfloat  outDeltaZ, outAngleZ;

	if ( m_eOrientation == kCCTransitionOrientationRightOver ) 
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	} 
	else 
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}

    m_pInScene->setVisible ( KD_FALSE );
	m_pInScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime	 ::create ( m_fDuration / 2 ),
            CCShow		 ::create ( ),
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, -45, 0 ),
            CCCallFunc	 ::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
            KD_NULL
        )
    );
    
	m_pOutScene->runAction 
    (  
        (CCActionInterval *) CCSequence::create
        (
            CCOrbitCamera::create ( m_fDuration / 2, 1, 0, outAngleZ, outDeltaZ, 45, 0 ),
            CCHide		 ::create ( ),
            CCDelayTime	 ::create ( m_fDuration / 2 ),							
            KD_NULL
        )
	);
}

CCTransitionFlipAngular* CCTransitionFlipAngular::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionFlipAngular::create ( fDuration, pScene, kCCTransitionOrientationRightOver );
}

CCTransitionFlipAngular* CCTransitionFlipAngular::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionFlipAngular*  pRet = new CCTransitionFlipAngular ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

//
// ZoomFlipX Transition
//

KDvoid CCTransitionZoomFlipX::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat   inDeltaZ,  inAngleZ;
	KDfloat  outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationRightOver ) 
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	} 
	else 
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}
    
    m_pInScene->setVisible ( KD_FALSE );
    m_pInScene->setScale ( 0.5f );
	m_pInScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime::create ( m_fDuration / 2 ),
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, 0, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 1 ),
                CCShow		 ::create ( ),
                KD_NULL
             ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
            KD_NULL
        )
	);
    
	m_pOutScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0, outAngleZ, outDeltaZ, 0, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 0.5f ),
                KD_NULL
            ),
            CCHide::create ( ),
            CCDelayTime::create ( m_fDuration / 2 ),							
            KD_NULL
        )
	);
}

CCTransitionZoomFlipX* CCTransitionZoomFlipX::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionZoomFlipX::create ( fDuration, pScene, kCCTransitionOrientationRightOver );
}

CCTransitionZoomFlipX* CCTransitionZoomFlipX::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionZoomFlipX*  pRet = new CCTransitionZoomFlipX ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

//
// ZoomFlipY Transition
//

KDvoid CCTransitionZoomFlipY::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat  inDeltaZ,  inAngleZ;
	KDfloat outDeltaZ, outAngleZ;

	if( m_eOrientation== kCCTransitionOrientationUpOver ) 
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	} 
	else
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}

    m_pInScene->setVisible ( KD_FALSE );
    m_pInScene->setScale ( 0.5f );
	m_pInScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime::create ( m_fDuration / 2),
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, 90, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 1 ),
                CCShow		 ::create ( ),
                KD_NULL
            ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
            KD_NULL
        )
	);

	m_pOutScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0, outAngleZ, outDeltaZ, 90, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 0.5f ),
                KD_NULL
            ),							
            CCHide::create ( ),
            CCDelayTime::create ( m_fDuration / 2 ),
            KD_NULL
        )
	);
}

CCTransitionZoomFlipY* CCTransitionZoomFlipY::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionZoomFlipY::create ( fDuration, pScene, kCCTransitionOrientationUpOver );
}

CCTransitionZoomFlipY* CCTransitionZoomFlipY::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionZoomFlipY*  pRet = new CCTransitionZoomFlipY ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

//
// ZoomFlipAngular Transition
//

KDvoid CCTransitionZoomFlipAngular::onEnter ( KDvoid )
{
	CCTransitionSceneOriented::onEnter ( );

	KDfloat   inDeltaZ,  inAngleZ;
	KDfloat  outDeltaZ, outAngleZ;

	if ( m_eOrientation == kCCTransitionOrientationRightOver )
	{
		 inDeltaZ =  90;
		 inAngleZ = 270;
		outDeltaZ =  90;
		outAngleZ =   0;
	} 
	else 
	{
		 inDeltaZ = -90;
		 inAngleZ =  90;
		outDeltaZ = -90;
		outAngleZ =   0;
	}

    m_pInScene->setVisible ( KD_FALSE );
    m_pInScene->setScale ( 0.5f );
	m_pInScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime::create ( m_fDuration / 2 ),
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0, inAngleZ, inDeltaZ, -45, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 1 ),
                CCShow		 ::create ( ),
                KD_NULL
            ),
            CCShow::create ( ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
            KD_NULL
        )
	);
    
	m_pOutScene->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCSpawn::create
            (
                CCOrbitCamera::create ( m_fDuration / 2, 1, 0 , outAngleZ, outDeltaZ, 45, 0 ),
                CCScaleTo	 ::create ( m_fDuration / 2, 0.5f ),
                KD_NULL
            ),							
            CCHide::create ( ),
            CCDelayTime::create ( m_fDuration / 2 ),							
            KD_NULL
        )
	);
}

CCTransitionZoomFlipAngular* CCTransitionZoomFlipAngular::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionZoomFlipAngular::create ( fDuration, pScene, kCCTransitionOrientationRightOver );
}

CCTransitionZoomFlipAngular* CCTransitionZoomFlipAngular::create ( KDfloat fDuration, CCScene* pScene, tOrientation eOrientation )
{
    CCTransitionZoomFlipAngular*  pRet = new CCTransitionZoomFlipAngular ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, eOrientation ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

//
// Fade Transition
//

CCTransitionFade* CCTransitionFade::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
	return CCTransitionFade::create ( fDuration, pScene, ccBLACK );
}

CCTransitionFade* CCTransitionFade::create ( KDfloat fDuration, CCScene* pScene, const ccColor3B& tColor )
{
	CCTransitionFade*  pRet = new CCTransitionFade ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, tColor ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDbool CCTransitionFade::initWithDuration ( KDfloat fDuration, CCScene* pScene, const ccColor3B& tColor )
{
	if ( CCTransitionScene::initWithDuration ( fDuration, pScene ) )
	{
		m_tColor.r = tColor.r;
		m_tColor.g = tColor.g;
		m_tColor.b = tColor.b;
		m_tColor.a = 0;
	}
    
	return KD_TRUE;
}

KDbool CCTransitionFade::initWithDuration ( KDfloat fDuration, CCScene* pScene )
{
	this->initWithDuration ( fDuration, pScene, ccBLACK );

	return KD_TRUE;
}

KDvoid CCTransitionFade :: onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );
    
	m_pInScene->setVisible ( KD_FALSE );
	
    CCLayerColor*  pLayer = CCLayerColor::create ( m_tColor );

	pLayer->runAction 
    (
        (CCActionInterval *) CCSequence::create
        (
            CCFadeIn  ::create ( m_fDuration / 2 ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::hideOutShowIn ) ),
            CCFadeOut ::create ( m_fDuration / 2 ),
            CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
            KD_NULL
         )
	);
    
    this->addChild ( pLayer, 2, kSceneFade );
}

KDvoid CCTransitionFade::onExit ( KDvoid )
{
	CCTransitionScene::onExit ( );

	this->removeChildByTag ( kSceneFade, KD_FALSE );
}

//
// Cross Fade Transition
//

CCTransitionCrossFade* CCTransitionCrossFade::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionCrossFade*  pRet = new CCTransitionCrossFade ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionCrossFade:: draw ( KDvoid )
{
	// override draw since both scenes (textures) are rendered in 1 scene
}

KDvoid CCTransitionCrossFade::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	// create a transparent color layer
	// in which we are going to add our rendertextures
    const CCSize&   tSize = CCDirector::sharedDirector ( )->getWinSize ( );
	ccColor4B      tColor = { 0, 0, 0, 0 };
	CCLayerColor*  pLayer = CCLayerColor::create ( tColor );

	// create the first render texture for inScene
	CCRenderTexture*  inTexture = CCRenderTexture::create ( (KDint) tSize.cx, (KDint) tSize.cy );
	if ( !inTexture  )
	{
		return;
	}

	inTexture->getSprite ( )->setAnchorPoint ( ccp ( 0.5f,0.5f ) );
	inTexture->setPosition ( ccpMid ( tSize ) );
	inTexture->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

	// render inScene to its texturebuffer
	inTexture->begin ( );
	m_pInScene->visit ( );
	inTexture->end ( );

	// create the second render texture for outScene
	CCRenderTexture* outTexture = CCRenderTexture::create ( (KDint) tSize.cx, (KDint) tSize.cy );
	outTexture->getSprite ( )->setAnchorPoint ( ccp ( 0.5f,0.5f ) );
	outTexture->setPosition ( ccpMid ( tSize ) );
	outTexture->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

	// render outScene to its texturebuffer
	outTexture->begin ( );
	m_pOutScene->visit ( );
	outTexture->end ( );

	// create blend functions
	ccBlendFunc  tBlend1 = { GL_ONE, GL_ONE }; // inScene will lay on background and will not be used with alpha
	ccBlendFunc  tBlend2 = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA }; // we are going to blend outScene via alpha 

	// set blendfunctions
	inTexture->getSprite ( )->setBlendFunc ( tBlend1 );
	outTexture->getSprite ( )->setBlendFunc ( tBlend2 );	

	// add render textures to the layer
	pLayer->addChild ( inTexture );
	pLayer->addChild ( outTexture );

	// initial opacity:
	inTexture->getSprite ( )->setOpacity ( 255 );
	outTexture->getSprite ( )->setOpacity ( 255 );

	// create the blend action
	CCAction*  pActions = CCSequence::create
	(
		CCFadeTo  ::create ( m_fDuration, 0 ),
		CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::hideOutShowIn ) ),
		CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
		KD_NULL
	);

	// run the blend action
	outTexture->getSprite ( )->runAction ( pActions );

	// add the layer (which contains our two rendertextures) to the scene
	this->addChild ( pLayer, 2, kSceneFade );
}

KDvoid CCTransitionCrossFade::onExit ( KDvoid )
{
	// remove our layer and release all containing objects 
	this->removeChildByTag ( kSceneFade, KD_FALSE );
    
	CCTransitionScene::onExit ( );
}

//
// TurnOffTilesTransition
//

CCTransitionTurnOffTiles* CCTransitionTurnOffTiles::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionTurnOffTiles*  pRet = new CCTransitionTurnOffTiles ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionTurnOffTiles::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionTurnOffTiles::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );
    
	const CCSize&   tSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDfloat			fAspect = tSize.cx / tSize.cy;
    KDint			x = (KDint) ( 12 * fAspect );
    KDint			y = 12;

	m_pOutScene->runAction
	(
		CCSequence::create
		(
			easeActionWithAction ( CCTurnOffTiles::create ( m_fDuration, ccs ( x, y ) ) ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
			CCStopGrid::create ( ),
			KD_NULL
		)
	);
}

CCActionInterval* CCTransitionTurnOffTiles:: easeActionWithAction ( CCActionInterval* pAction )
{
	return pAction;
}


//
// SplitCols Transition
//

CCTransitionSplitCols* CCTransitionSplitCols::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSplitCols*  pRet = new CCTransitionSplitCols ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionSplitCols::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );
    
	m_pInScene->setVisible ( KD_FALSE );

	CCActionInterval*  pSplit = action ( );
	CCActionInterval*  pSequence = (CCActionInterval *) CCSequence::create
	(
		pSplit,
		CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::hideOutShowIn ) ),
		pSplit->reverse ( ),
		KD_NULL
	);

	this->runAction
	( 
		CCSequence::create
		(
			easeActionWithAction ( pSequence ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
			CCStopGrid::create ( ),
			KD_NULL
		)
	);
}

CCActionInterval* CCTransitionSplitCols::action ( KDvoid )
{
	return CCSplitCols::create ( m_fDuration / 2.0f, 3 );
}

CCActionInterval* CCTransitionSplitCols::easeActionWithAction ( CCActionInterval* pAction )
{
	return CCEaseInOut::create ( pAction, 3.0f );
}


//
// SplitRows Transition
//

CCTransitionSplitRows* CCTransitionSplitRows::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionSplitRows*  pRet = new CCTransitionSplitRows ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCActionInterval* CCTransitionSplitRows::action ( KDvoid )
{
	return CCSplitRows::create ( m_fDuration / 2.0f, 3 );
}


//
// FadeTR Transition
//

CCTransitionFadeTR* CCTransitionFadeTR::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionFadeTR*  pRet = new CCTransitionFadeTR ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDvoid CCTransitionFadeTR::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionFadeTR::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	const CCSize&   tSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDfloat			fAspect = tSize.cx / tSize.cy;
	KDint			x = (KDint) ( 12 * fAspect );
    KDint			y = 12;

	m_pOutScene->runAction
	(
		CCSequence::create
		(
			easeActionWithAction ( actionWithSize ( ccs ( x, y ) ) ),
			CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ), 
			CCStopGrid::create ( ),
			KD_NULL
		)
	);
}

CCActionInterval*  CCTransitionFadeTR::actionWithSize ( const CCSize& tSize )
{
	return CCFadeOutTRTiles::create ( m_fDuration, tSize );
}

CCActionInterval* CCTransitionFadeTR::easeActionWithAction ( CCActionInterval* pAction )
{
	return pAction;
}


//
// FadeBL Transition
//

CCTransitionFadeBL* CCTransitionFadeBL::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionFadeBL*  pRet = new CCTransitionFadeBL ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCActionInterval* CCTransitionFadeBL::actionWithSize ( const CCSize& tSize )
{
	return CCFadeOutBLTiles::create ( m_fDuration, tSize );
}


//
// FadeUp Transition
//

CCTransitionFadeUp* CCTransitionFadeUp::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionFadeUp*  pRet = new CCTransitionFadeUp ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCActionInterval* CCTransitionFadeUp::actionWithSize ( const CCSize& tSize )
{
	return CCFadeOutUpTiles::create ( m_fDuration, tSize );
}


//
// FadeDown Transition
//

CCTransitionFadeDown* CCTransitionFadeDown::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionFadeDown*  pRet = new CCTransitionFadeDown ( );
    
	if ( pRet && pRet->initWithDuration ( fDuration, pScene ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

CCActionInterval* CCTransitionFadeDown::actionWithSize ( const CCSize& tSize )
{
	return CCFadeOutDownTiles::create ( m_fDuration, tSize );
}

NS_CC_END