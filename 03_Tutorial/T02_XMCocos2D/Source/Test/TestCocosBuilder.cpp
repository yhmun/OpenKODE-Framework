/* --------------------------------------------------------------------------
 *
 *      File            TestCocosBuilder.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#include "Precompiled.h"
#include "TestCocosBuilder.h"

//------------------------------------------------------------------
//
// TestCocosBuilder
//
//------------------------------------------------------------------

KDbool TestCocosBuilder::init ( KDvoid )
{	
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Create an autorelease CCNodeLoaderLibrary. 
    CCNodeLoaderLibrary*  pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary ( );
	pCCNodeLoaderLibrary->registerCCNodeLoader ( "HelloCocosBuilderLayer", HelloCocosBuilderLayerLoader::loader ( ) );

    // Create an autorelease CCBReader. 
    CCBReader*  pCCBReader = new CCBReader ( pCCNodeLoaderLibrary );
    
    // Read a ccbi file. 
    CCNode*  pNode = pCCBReader->readNodeGraphFromFile ( "ccb/HelloCocosBuilder.ccbi", this );
	this->addChild ( pNode );

    pCCBReader->release ( );

	return KD_TRUE;
}

//------------------------------------------------------------------
//
// HelloCocosBuilderLayer
//
//------------------------------------------------------------------

HelloCocosBuilderLayer::HelloCocosBuilderLayer ( KDvoid )
{
	m_pBurstSprite		 = KD_NULL;
	m_pTestTitleLabelTTF = KD_NULL;
}

HelloCocosBuilderLayer::~HelloCocosBuilderLayer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pBurstSprite );
    CC_SAFE_RELEASE ( m_pTestTitleLabelTTF );
}

KDvoid HelloCocosBuilderLayer::openTest ( const KDchar* szCCBFileName, const KDchar* szCCNodeName, CCNodeLoader* pCCNodeLoader )
{
    // Create an autorelease CCNodeLoaderLibrary. 
    CCNodeLoaderLibrary*   pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary ( );

    pCCNodeLoaderLibrary->registerCCNodeLoader ( "TestHeaderLayer", TestHeaderLayerLoader::loader ( ) );
    
	if ( szCCNodeName != KD_NULL && pCCNodeLoader != KD_NULL ) 
	{
        pCCNodeLoaderLibrary->registerCCNodeLoader ( szCCNodeName, pCCNodeLoader );
    }

    // Create an autorelease CCBReader. 
    CCBReader*  pCCBReader = new CCBReader ( pCCNodeLoaderLibrary );
    pCCBReader->autorelease ( );

    // Read a ccbi file. 
    // Load the scene from the ccbi-file, setting this class as
    // the owner will cause lblTestTitle to be set by the CCBReader.
    // lblTestTitle is in the TestHeader.ccbi, which is referenced
    // from each of the test scenes.
    CCNode*   pNode = pCCBReader->readNodeGraphFromFile ( szCCBFileName, this );
    m_pTestTitleLabelTTF->setString ( szCCBFileName );

    CCScene*  pScene = CCScene::create ( );
    if ( pNode != KD_NULL )
	{
        pScene->addChild ( pNode );
    }	
    
    CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( 0.5f, pScene, ccBLACK ) );
}

KDvoid HelloCocosBuilderLayer::onNodeLoaded ( CCNode* pNode, CCNodeLoader* pNodeLoader ) 
{
    CCRotateBy*       pCCRotateBy = CCRotateBy::create ( 20.0f, 360 );
    CCRepeatForever*  pCCRepeatForever = CCRepeatForever::create ( pCCRotateBy );

    this->m_pBurstSprite->runAction ( pCCRepeatForever );
}

SEL_MenuHandler HelloCocosBuilderLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    return KD_NULL;    
}

SEL_CCControlHandler HelloCocosBuilderLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onMenuTestClicked"				, HelloCocosBuilderLayer::onMenuTestClicked				 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onSpriteTestClicked"			, HelloCocosBuilderLayer::onSpriteTestClicked			 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onButtonTestClicked"			, HelloCocosBuilderLayer::onButtonTestClicked			 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onAnimationsTestClicked"		, HelloCocosBuilderLayer::onAnimationsTestClicked		 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onParticleSystemTestClicked"	, HelloCocosBuilderLayer::onParticleSystemTestClicked	 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onScrollViewTestClicked"		, HelloCocosBuilderLayer::onScrollViewTestClicked		 );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onTimelineCallbackSoundClicked", HelloCocosBuilderLayer::onTimelineCallbackSoundClicked );

    return KD_NULL;
}

KDbool HelloCocosBuilderLayer::onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode )
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "mBurstSprite"		, CCSprite*  , this->m_pBurstSprite		  );
    CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "mTestTitleLabelTTF", CCLabelTTF*, this->m_pTestTitleLabelTTF );

    return KD_FALSE;
}

KDbool HelloCocosBuilderLayer::onAssignCCBCustomProperty ( CCObject* pTarget, const KDchar* pMemberVariableName, CCBValue* pCCBValue )
{
    KDbool  bRet = KD_FALSE;
    if ( pTarget == this )
    {
        if ( 0 == kdStrcmp ( pMemberVariableName, "mCustomPropertyInt" ) )
        {
            this->m_nCustomPropertyInt = pCCBValue->getIntValue ( );
            CCLOG ( "mCustomPropertyInt = %d", m_nCustomPropertyInt );
            bRet = KD_TRUE;
        }
        else if ( 0 == kdStrcmp ( pMemberVariableName, "mCustomPropertyFloat" ) )
        {
            this->m_fCustomPropertyFloat = pCCBValue->getFloatValue ( );
            CCLOG ( "mCustomPropertyFloat = %f", m_fCustomPropertyFloat );
            bRet = KD_TRUE;
        }
        else if ( 0  == kdStrcmp ( pMemberVariableName, "mCustomPropertyBoolean" ) )
        {
            this->m_bCustomPropertyBoolean = pCCBValue->getBoolValue ( );
            CCLOG ( "mCustomPropertyBoolean = %d", m_bCustomPropertyBoolean );
            bRet = KD_TRUE;
        }
        else if ( 0  == kdStrcmp ( pMemberVariableName, "mCustomPropertyString" ) )
        {
            this->m_sCustomPropertyString = pCCBValue->getStringValue();
            CCLOG ( "mCustomPropertyString = %s", m_sCustomPropertyString.c_str ( ) );
            bRet = KD_TRUE;
        }
        
    }

    return bRet;
}

KDvoid HelloCocosBuilderLayer::onMenuTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    this->openTest ( "ccb/ccb/TestMenus.ccbi", "TestMenusLayer", MenuTestLayerLoader::loader ( ) );
}

KDvoid HelloCocosBuilderLayer::onSpriteTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent ) 
{
    this->openTest ( "ccb/ccb/TestSprites.ccbi", "TestSpritesLayer", SpriteTestLayerLoader::loader ( ) );
}

KDvoid HelloCocosBuilderLayer::onButtonTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent ) 
{
    this->openTest ( "ccb/ccb/TestButtons.ccbi", "TestButtonsLayer", ButtonTestLayerLoader::loader ( ) );
}

KDvoid HelloCocosBuilderLayer::onAnimationsTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    // Create an autorelease CCNodeLoaderLibrary. 
    CCNodeLoaderLibrary*  pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary ( );
    
    pCCNodeLoaderLibrary->registerCCNodeLoader ( "TestHeaderLayer"    , TestHeaderLayerLoader    ::loader ( ) );
    pCCNodeLoaderLibrary->registerCCNodeLoader ( "TestAnimationsLayer", AnimationsTestLayerLoader::loader ( ) );
    
    // Create an autorelease CCBReader. 
    CCBReader*  pCCBReader = new CCBReader ( pCCNodeLoaderLibrary );
    pCCBReader->autorelease ( );
    
    // Read a ccbi file. 
    // Load the scene from the ccbi-file, setting this class as
    // the owner will cause lblTestTitle to be set by the CCBReader.
    // lblTestTitle is in the TestHeader.ccbi, which is referenced
    // from each of the test scenes.
    CCNode*  pAnimationsTest = pCCBReader->readNodeGraphFromFile ( "ccb/ccb/TestAnimations.ccbi", this );
	// Load node graph (TestAnimations is a sub class of CCLayer) and retrieve the ccb action manager
    ( (AnimationsTestLayer*) pAnimationsTest )->setAnimationManager ( pCCBReader->getAnimationManager ( ) );
    
    m_pTestTitleLabelTTF->setString ( "TestAnimations.ccbi" );
    
    CCScene*  pScene = CCScene::create ( );
    if ( pAnimationsTest != KD_NULL )
	{
        pScene->addChild ( pAnimationsTest );
    }
    
    CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( 0.5f, pScene, ccBLACK ) );
}

KDvoid HelloCocosBuilderLayer::onParticleSystemTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent ) 
{
    this->openTest ( "ccb/ccb/TestParticleSystems.ccbi", "TestParticleSystemsLayer", ParticleSystemTestLayerLoader::loader ( ) );
}

KDvoid HelloCocosBuilderLayer::onScrollViewTestClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    this->openTest ( "ccb/ccb/TestScrollViews.ccbi", "TestScrollViewsLayer", ScrollViewTestLayerLoader::loader ( ) );
}

KDvoid HelloCocosBuilderLayer::onTimelineCallbackSoundClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    this->openTest ( "ccb/ccb/TestTimelineCallback.ccbi", "TimelineCallbackTestLayer", TimelineCallbackTestLayerLoader::loader ( ) );
}

//------------------------------------------------------------------
//
// TestHeaderLayer
//
//------------------------------------------------------------------

SEL_MenuHandler TestHeaderLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE ( this, "onBackClicked", TestHeaderLayer::onBackClicked );

    return KD_NULL;    
}

SEL_CCControlHandler TestHeaderLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    return KD_NULL;
}

KDvoid TestHeaderLayer::onBackClicked ( CCObject* pSender )
{
    CCDirector::sharedDirector ( )->popScene ( );
}

//------------------------------------------------------------------
//
// MenuTestLayer
//
//------------------------------------------------------------------

MenuTestLayer::MenuTestLayer ( KDvoid )
{
	m_pMenuItemStatusLabelBMFont = KD_NULL;
}

MenuTestLayer::~MenuTestLayer ( )
{
    CC_SAFE_RELEASE ( m_pMenuItemStatusLabelBMFont );
}

SEL_MenuHandler MenuTestLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE ( this, "onMenuItemAClicked", MenuTestLayer::onMenuItemAClicked );
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE ( this, "onMenuItemBClicked", MenuTestLayer::onMenuItemBClicked );
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE ( this, "onMenuItemCClicked", MenuTestLayer::onMenuItemCClicked );

    return KD_NULL;
}

SEL_CCControlHandler MenuTestLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    return KD_NULL;
}

KDbool MenuTestLayer::onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode )
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "mMenuItemStatusLabelBMFont", CCLabelBMFont*, m_pMenuItemStatusLabelBMFont );

    return KD_FALSE;
}

KDvoid MenuTestLayer::onMenuItemAClicked ( CCObject* pSender )
{
    m_pMenuItemStatusLabelBMFont->setString ( "Menu Item A clicked." );
}

KDvoid MenuTestLayer::onMenuItemBClicked ( CCObject* pSender )
{
    m_pMenuItemStatusLabelBMFont->setString ( "Menu Item B clicked." );
}

KDvoid MenuTestLayer::onMenuItemCClicked ( CCObject* pSender )
{
    m_pMenuItemStatusLabelBMFont->setString ( "Menu Item C clicked." );
}

//------------------------------------------------------------------
//
// ButtonTestLayer
//
//------------------------------------------------------------------

ButtonTestLayer::ButtonTestLayer ( KDvoid )
{
	m_pCCControlEventLabel = KD_NULL;
}

ButtonTestLayer::~ButtonTestLayer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pCCControlEventLabel );
}

SEL_MenuHandler ButtonTestLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    return KD_NULL;    
}

SEL_CCControlHandler ButtonTestLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onCCControlButtonClicked", ButtonTestLayer::onCCControlButtonClicked );

    return KD_NULL;
}

KDbool ButtonTestLayer::onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode )
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "mCCControlEventLabel", CCLabelBMFont*, m_pCCControlEventLabel );

    return KD_FALSE;
}

KDvoid ButtonTestLayer::onCCControlButtonClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    switch ( uCCControlEvent ) 
	{
        case kCCControlEventTouchDown			:	m_pCCControlEventLabel->setString ( "Touch Down."		 );	break;
        case kCCControlEventTouchDragInside		:	m_pCCControlEventLabel->setString ( "Touch Drag Inside." );	break;
        case kCCControlEventTouchDragOutside	:	m_pCCControlEventLabel->setString ( "Touch Drag Outside.");	break;
        case kCCControlEventTouchDragEnter		:	m_pCCControlEventLabel->setString ( "Touch Drag Enter."	 );	break;
        case kCCControlEventTouchDragExit		:	m_pCCControlEventLabel->setString ( "Touch Drag Exit."	 );	break;
        case kCCControlEventTouchUpInside		:	m_pCCControlEventLabel->setString ( "Touch Up Inside."	 );	break;
        case kCCControlEventTouchUpOutside		:	m_pCCControlEventLabel->setString ( "Touch Up Outside."	 );	break;
        case kCCControlEventValueChanged		:	m_pCCControlEventLabel->setString ( "Value Changed."	 );	break;

        default :
            CCAssert ( KD_FALSE, "" ); // OH SHIT!
    }
}

//------------------------------------------------------------------
//
// AnimationsTestLayer
//
//------------------------------------------------------------------

AnimationsTestLayer::AnimationsTestLayer ( KDvoid )
{
	m_pAnimationManager = KD_NULL;
}

AnimationsTestLayer::~AnimationsTestLayer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pAnimationManager );
}

SEL_MenuHandler AnimationsTestLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    return KD_NULL;
}

SEL_CCControlHandler AnimationsTestLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onCCControlButtonIdleClicked" , AnimationsTestLayer::onCCControlButtonIdleClicked  );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onCCControlButtonWaveClicked" , AnimationsTestLayer::onCCControlButtonWaveClicked  );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onCCControlButtonJumpClicked" , AnimationsTestLayer::onCCControlButtonJumpClicked  );
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE ( this, "onCCControlButtonFunkyClicked", AnimationsTestLayer::onCCControlButtonFunkyClicked );

    return KD_NULL;
}

KDbool AnimationsTestLayer::onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "mAnimationManager", CCBAnimationManager*, m_pAnimationManager );
    
    return KD_FALSE;
}

KDvoid AnimationsTestLayer::setAnimationManager ( CCBAnimationManager* pAnimationManager )
{
    CC_SAFE_RELEASE ( m_pAnimationManager );
    m_pAnimationManager = pAnimationManager;
    CC_SAFE_RETAIN  ( m_pAnimationManager );
}

KDvoid AnimationsTestLayer::onCCControlButtonIdleClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    m_pAnimationManager->runAnimationsForSequenceNamedTweenDuration ( "Idle", 0.3f );
}

KDvoid AnimationsTestLayer::onCCControlButtonWaveClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    m_pAnimationManager->runAnimationsForSequenceNamedTweenDuration ( "Wave", 0.3f );
}

KDvoid AnimationsTestLayer::onCCControlButtonJumpClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    m_pAnimationManager->runAnimationsForSequenceNamedTweenDuration ( "Jump", 0.3f );
}

KDvoid AnimationsTestLayer::onCCControlButtonFunkyClicked ( CCObject* pSender, CCControlEvent uCCControlEvent )
{
    m_pAnimationManager->runAnimationsForSequenceNamedTweenDuration ( "Funky", 0.3f );
}


//------------------------------------------------------------------
//
// TimelineCallbackTestLayer
//
//------------------------------------------------------------------

TimelineCallbackTestLayer::TimelineCallbackTestLayer ( KDvoid )
{
	m_pHelloLabel = KD_NULL;
}

TimelineCallbackTestLayer::~TimelineCallbackTestLayer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pHelloLabel );
}

SEL_MenuHandler TimelineCallbackTestLayer::onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName ) 
{
    return KD_NULL;
}

SEL_CCControlHandler TimelineCallbackTestLayer::onResolveCCBCCControlSelector ( CCObject* pTarget, const KDchar* pSelectorName ) 
{
    return KD_NULL;
}

SEL_CallFuncN TimelineCallbackTestLayer::onResolveCCBCCCallFuncSelector ( CCObject* pTarget, const KDchar* pSelectorName )
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE ( this, "onCallback1", TimelineCallbackTestLayer::onCallback1 );
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE ( this, "onCallback2", TimelineCallbackTestLayer::onCallback2 );
    return KD_NULL;
}

KDbool TimelineCallbackTestLayer::onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode )
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE ( this, "helloLabel", CCLabelTTF*, this->m_pHelloLabel );
    
    return KD_FALSE;
}

KDvoid TimelineCallbackTestLayer::onCallback1 ( CCNode* pSender )
{
    // Rotate the label when the button is pressed
    this->m_pHelloLabel->runAction ( CCRotateBy::create ( 1, 360 ) );
    this->m_pHelloLabel->setString ( "Callback 1");
}

KDvoid TimelineCallbackTestLayer::onCallback2 ( CCNode* pSender )
{
    // Rotate the label when the button is pressed
    this->m_pHelloLabel->runAction ( CCRotateBy::create ( 1, -360 ) );
    this->m_pHelloLabel->setString ( "Callback 2" );
}
