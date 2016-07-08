/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionProgress.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Lam Pham
 *      Copyright (c) 2012      Ricardo Quesada
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
#include "layers_scenes_transitions_nodes/CCTransitionProgress.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionProgressTimer.h"
#include "misc_nodes/CCRenderTexture.h"
#include "misc_nodes/CCProgressTimer.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

enum 
{
    kCCSceneRadial = 0xc001,
};

//
// CCTransitionProgress
//

CCTransitionProgress::CCTransitionProgress ( KDvoid )
{
	m_fTo	= 0.0f;
	m_fFrom = 0.0f;
	m_pSceneToBeModified = KD_NULL;
}

CCTransitionProgress* CCTransitionProgress::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgress*  pRet = new CCTransitionProgress ( );
    
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

KDvoid CCTransitionProgress::onEnter ( KDvoid )
{
    CCTransitionScene::onEnter ( );

    setupTransition ( );
    
    // create a transparent color layer
    // in which we are going to add our rendertextures
	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );

    // create the second render texture for outScene
    CCRenderTexture*  pTexture = CCRenderTexture::create ( (KDint) tSize.cx, (KDint) tSize.cy );
    pTexture->getSprite ( )->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pTexture->setPosition ( ccpMid ( tSize ) );
    pTexture->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

    // render outScene to its texturebuffer
    pTexture->clear ( 0, 0, 0, 1 );
    pTexture->begin ( );
    m_pSceneToBeModified->visit ( );
    pTexture->end ( );

    //    Since we've passed the outScene to the texture we don't need it.
    if ( m_pSceneToBeModified == m_pOutScene )
    {
        hideOutShowIn ( );
    }

    //    We need the texture in RenderTexture.
    CCProgressTimer*  pNode = progressTimerNodeWithRenderTexture ( pTexture );

    // create the blend action
    CCActionInterval*  pLayerAction = (CCActionInterval*) CCSequence::create
	( 
        CCProgressFromTo::create ( m_fDuration, m_fFrom, m_fTo ),
        CCCallFunc::create ( this, callfunc_selector ( CCTransitionProgress::finish ) ), 
        KD_NULL 
	);

    // run the blend action
    pNode->runAction ( pLayerAction );

    // add the layer ( which contains our two rendertextures ) to the scene
    addChild ( pNode, 2, kCCSceneRadial );
}

// clean up on exit
KDvoid CCTransitionProgress::onExit ( KDvoid )
{
    // remove our layer and release all containing objects
    removeChildByTag ( kCCSceneRadial, KD_TRUE );
    CCTransitionScene::onExit ( );
}

KDvoid CCTransitionProgress::sceneOrder ( KDvoid )
{
    m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionProgress::setupTransition ( KDvoid )
{
    m_pSceneToBeModified = m_pOutScene;
    m_fFrom = 100;
    m_fTo = 0;
}

CCProgressTimer* CCTransitionProgress::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{
    CCAssert ( KD_FALSE, "override me - abstract class" );
    return KD_NULL;
}

//
// CCTransitionProgressRadialCCW
//

CCTransitionProgressRadialCCW* CCTransitionProgressRadialCCW::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressRadialCCW*  pRet = new CCTransitionProgressRadialCCW ( );
    
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

CCProgressTimer* CCTransitionProgressRadialCCW::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );

    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeRadial );

    //    Return the radial type that we want to use
    pNode->setReverseDirection ( KD_FALSE );
    pNode->setPercentage ( 100 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return pNode;
}

//
// CCTransitionProgressRadialCW
//

CCTransitionProgressRadialCW* CCTransitionProgressRadialCW::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressRadialCW*  pRet = new CCTransitionProgressRadialCW ( );
    
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

CCProgressTimer* CCTransitionProgressRadialCW::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{  
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );
    
    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeRadial );
    
    //    Return the radial type that we want to use
    pNode->setReverseDirection ( KD_TRUE );
    pNode->setPercentage ( 100 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return pNode;
}

//
// CCTransitionProgressHorizontal
//

CCTransitionProgressHorizontal* CCTransitionProgressHorizontal::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressHorizontal*  pRet = new CCTransitionProgressHorizontal ( );
    
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

CCProgressTimer* CCTransitionProgressHorizontal::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{    
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );
    
    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeBar );
    
    pNode->setMidpoint ( ccp ( 1, 0 ) );
    pNode->setBarChangeRate ( ccp ( 1, 0 ) );
    
    pNode->setPercentage ( 100 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

    return pNode;
}

//
// CCTransitionProgressVertical
//

CCTransitionProgressVertical* CCTransitionProgressVertical::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressVertical*  pRet = new CCTransitionProgressVertical ( );
    
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

CCProgressTimer* CCTransitionProgressVertical::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{        
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );
    
    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeBar );
    
    pNode->setMidpoint ( ccp ( 0, 0 ) );
    pNode->setBarChangeRate ( ccp ( 0, 1 ) );
    
    pNode->setPercentage ( 100 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return pNode;
}

//
// CCTransitionProgressInOut
//

CCTransitionProgressInOut* CCTransitionProgressInOut::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressInOut*  pRet = new CCTransitionProgressInOut ( );
    
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

KDvoid CCTransitionProgressInOut::sceneOrder ( KDvoid )
{
    m_bIsInSceneOnTop = KD_FALSE;
}

KDvoid CCTransitionProgressInOut::setupTransition ( KDvoid )
{
    m_pSceneToBeModified = m_pInScene;
    m_fFrom = 0;
    m_fTo = 100;    
}

CCProgressTimer* CCTransitionProgressInOut::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{        
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );
    
    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeBar );
    
    pNode->setMidpoint ( ccp ( 0.5f, 0.5f ) );
    pNode->setBarChangeRate ( ccp ( 1, 1 ) );
    
    pNode->setPercentage ( 0 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return pNode;
}

//
// CCTransitionProgressOutIn
//

CCTransitionProgressOutIn* CCTransitionProgressOutIn::create ( KDfloat fDuration, CCScene* pScene )
{                                                               
    CCTransitionProgressOutIn*  pRet = new CCTransitionProgressOutIn ( );
    
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

CCProgressTimer* CCTransitionProgressOutIn::progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture )
{        
    CCProgressTimer*  pNode = CCProgressTimer::create ( pTexture->getSprite ( ) );
    
    // but it is flipped upside down so we flip the sprite
    pNode->getSprite ( )->setFlipY ( KD_TRUE );
    pNode->setType ( kCCProgressTimerTypeBar );
    
    pNode->setMidpoint ( ccp ( 0.5f, 0.5f ) );
    pNode->setBarChangeRate ( ccp ( 1, 1 ) );
    
    pNode->setPercentage ( 100 );
    pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return pNode;
}

NS_CC_END
