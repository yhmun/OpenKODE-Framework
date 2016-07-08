/* --------------------------------------------------------------------------
 *
 *      File            TestArtPig.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com
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
#include "TestArtPig.h"

TestArtPig* TestArtPig::create ( KDvoid )
{
	TestArtPig*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new ArtPigSample			( ); break;
		case  1 : pLayer = new HappyPrince			( ); break;
		case  2 : pLayer = new HappyPrincePacked	( ); break;
		case  3 : pLayer = new PigSpriteSheet		( ); break;
		case  4 : pLayer = new TouchJewel			( ); break;
		case  5 : pLayer = new ObserveActions		( ); break;
		case  6 : pLayer = new JewelControl			( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

TestArtPig::~TestArtPig ( KDvoid )
{
	CC_SAFE_DELETE ( m_pSymbol );
}

KDbool TestArtPig::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	std::string  sLocation = "xm_artpig/";

	if ( this->getProjectLocation ( ) )
	{
		sLocation += this->getProjectLocation ( );
	}
	else
	{		
		sLocation += this->getProjectID ( );
	}

    // load resource manager with project ID.
	APSResourceManager*  pManager = APSResourceManager::getResourceManagerForProjectId ( this->getProjectID ( ), sLocation.c_str ( ) );
    
    // allocate new symbol
    m_pSymbol = pManager->newSymbol ( );
    
    // preload data to run animation without delay.
    m_pSymbol->preloadData ( );
    
    // add the symbol's root node as a child of HelloWorldLayer
    this->addChild ( m_pSymbol->getRootNode ( ), 0 );
    
    // register an action observer to the symbol so that didFinishAction() is called when the animation is over.
    m_pSymbol->addActionObserver ( this, kAPSDefaultActionGroupTag );
    
    // start animation by triggering prologue action groups.
    m_pSymbol->triggerPrologueActionGroups ( );

	return KD_TRUE;
}

KDuint TestArtPig::count ( KDvoid )
{
	return 7;
}

const KDchar* TestArtPig::getProjectID ( KDvoid )
{
	return KD_NULL;
}

const KDchar* TestArtPig::getProjectLocation ( KDvoid )
{
	return KD_NULL;
}

KDvoid TestArtPig::willStartAction ( APSAction* pAction )
{
	CCLOG ( "The animation will be started." );
}	

KDvoid TestArtPig::didFinishAction ( APSAction* pAction )
{
	CCLOG ( "The animation has just finished." );
}

//------------------------------------------------------------------
//
// ArtPigSample
//
//------------------------------------------------------------------

const KDchar* ArtPigSample::subtitle ( KDvoid )
{
	return "ArtPig Sample";
}

const KDchar* ArtPigSample::getProjectID ( KDvoid )
{
	return "APP10054";
}

//------------------------------------------------------------------
//
// HappyPrince
//
//------------------------------------------------------------------

const KDchar* HappyPrince::subtitle ( KDvoid )
{
	return "Happy Prince";
}

const KDchar* HappyPrince::getProjectID ( KDvoid )
{
	return "APP10099";
}

//------------------------------------------------------------------
//
// HappyPrincePacked
//
//------------------------------------------------------------------

KDbool HappyPrincePacked::init ( KDvoid )
{
	// Images are packed as sprite sheets.
    CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "xm_artpig/APP10099_Packed/HappyPrinceSheet.plist" );

	if ( !TestArtPig::init ( ) )
	{
		return KD_FALSE;
	}

	return KD_TRUE;
}

const KDchar* HappyPrincePacked::subtitle ( KDvoid )
{
	return "Happy Prince : Images are packed as sprite sheets";
}

const KDchar* HappyPrincePacked::getProjectID ( KDvoid )
{
	return "APP10099";
}

const KDchar* HappyPrincePacked::getProjectLocation ( KDvoid )
{
	return "APP10099_Packed";
}

//------------------------------------------------------------------
//
// PigSpriteSheet
//
//------------------------------------------------------------------

const KDchar* PigSpriteSheet::subtitle ( KDvoid )
{
	return "Pig Sprite Sheet : Animate action";
}

const KDchar* PigSpriteSheet::getProjectID ( KDvoid )
{
	return "APP10013";
}

//------------------------------------------------------------------
//
// TouchJewel
//
//------------------------------------------------------------------

KDbool TouchJewel::init ( KDvoid )
{
	if ( !TestArtPig::init ( ) )
	{
		return KD_FALSE;
	}

	m_pSymbol->setPosition ( ccpAdd ( m_pSymbol->getPosition ( ), ccp ( 0, 50 ) ) );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* TouchJewel::subtitle ( KDvoid )
{
	return "Touch Jewel : User touch and linker";
}

const KDchar* TouchJewel::getProjectID ( KDvoid )
{
	return "APP10193";
}

KDvoid TouchJewel::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDbool TouchJewel::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchBegan, pTouch->getLocation ( ), 1 ) );
	return KD_TRUE;
}

KDvoid TouchJewel::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchMoved, pTouch->getLocation ( ), 1 ) );
}

KDvoid TouchJewel::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchEnded, pTouch->getLocation ( ), 1 ) );
}

//------------------------------------------------------------------
//
// ObserveActions
//
//------------------------------------------------------------------

KDvoid MyActionObserver::willStartAction ( APSAction* pAction )
{
	 CCLOG ( "%s is about to start.", pAction->getTag ( ) );
}

KDvoid MyActionObserver::didFinishAction ( APSAction* pAction )
{
	CCLOG ( "%s has finished running.", pAction->getTag ( ) );
}

ObserveActions::~ObserveActions ( KDvoid )
{
	// unregister all observations related to _observer.
//  m_pSymbol->removeActionObserver ( m_pObserver );
	CC_SAFE_DELETE ( m_pObserver );
}

KDbool ObserveActions::init ( KDvoid )
{
	if ( !TestArtPig::init ( ) )
	{
		return KD_FALSE;
	}

	m_pObserver = new MyActionObserver ( );

    m_pSymbol->addActionObserver ( m_pObserver, "ActionGroup1" );
    m_pSymbol->addActionObserver ( m_pObserver, "ActionGroup2" );
    m_pSymbol->addActionObserver ( m_pObserver, "RotateJewel"  );
    m_pSymbol->addActionObserver ( m_pObserver, "MoveJewel"    );

	return KD_TRUE;
}

const KDchar* ObserveActions::subtitle ( KDvoid )
{
	return "Observe Actions";
}

const KDchar* ObserveActions::getProjectID ( KDvoid )
{
	return "APP10026";
}

//------------------------------------------------------------------
//
// JewelControl
//
//------------------------------------------------------------------

JewelController::JewelController ( APSSymbol* pSymbol )
{
	m_pSymbol     = pSymbol;
    m_pBoyGraphic = (APSGraphic*) pSymbol->getResourceForTag ( "Boy" );
    
    APSGraphic*  pJewelGraphic = (APSGraphic*) pSymbol->getResourceForTag ( "Jewel" );

    // Once touch has begun with jewel graphic, it receives all proceeding touch move events 
    pJewelGraphic->setRetainsTouch ( KD_TRUE );
    
    pSymbol->addActionObserver ( this, "JewelBlink" );

    pSymbol->addTouchObserver ( this, kAPSEventTypeTouchBegan, "Jewel" );
    pSymbol->addTouchObserver ( this, kAPSEventTypeTouchMoved, "Jewel" );
    pSymbol->addTouchObserver ( this, kAPSEventTypeTouchEnded, "Jewel" );	
}

JewelController::~JewelController ( KDvoid ) 
{
//	m_pSymbol->removeActionObserver ( this );

    m_pSymbol->removeTouchObserver ( this, kAPSEventTypeTouchBegan, "Jewel" );
    m_pSymbol->removeTouchObserver ( this, kAPSEventTypeTouchMoved, "Jewel" );
    m_pSymbol->removeTouchObserver ( this, kAPSEventTypeTouchEnded, "Jewel" );
}

//
// APSActionObserver
//
KDvoid JewelController::willStartAction ( APSAction* pAction )
{

}

KDvoid JewelController::didFinishAction ( APSAction* pAction )
{
	// restart 'JewelBlink' action group
	pAction->trigger ( );
}

//
// APSTouchEventObserver
//
KDvoid JewelController::touchBegan ( APSGraphic* pGraphic, const APSTouch& tTouch ) 
{
    // calculate touched position in symbol
    CCPoint  tPosition = m_pSymbol->convertPositionFromParentNode ( tTouch.getPosition ( ) );
    
    // change jewel graphic node position to touched position.
    pGraphic->getNode ( )->setPosition ( tPosition );
}

KDvoid JewelController::touchMoved ( APSGraphic* pGraphic, const APSTouch& tTouch ) 
{
    // calculate touched position in symbol
    CCPoint  tPosition  = m_pSymbol->convertPositionFromParentNode ( tTouch.getPosition ( ) );    
    CCNode*  pJewelNode = pGraphic->getNode ( );
    
    // change jewel graphic node position to touched position.
    pJewelNode->setPosition ( tPosition );
    
    // get the rectangular box of jewel node
    CCRect  tJewelBox = pJewelNode->boundingBox ( );
    
    // get the rectangular box of boy node.
    CCRect  tBoyBox = m_pBoyGraphic->getNode ( )->boundingBox ( );
    
    // check intersection of the two boxes.
	if (  tJewelBox.intersectsRect ( tBoyBox ) )
	{
        APSActionGroup*  pShakeBoyAction =  (APSActionGroup*) m_pSymbol->getResourceForTag ( "ShakeBoy" );
        // trigger 'ShakeBoy' action group only if it is currently not running.
        if ( !pShakeBoyAction->getRunning ( ) )
		{
            pShakeBoyAction->trigger ( );
        }
    }
}

KDvoid JewelController::touchEnded ( APSGraphic* pGraphic, const APSTouch& tTouch ) 
{
    // restore jewel graphic node the original position.
    pGraphic->getNode ( )->setPosition ( pGraphic->getPosition ( ) );
}

JewelControl::~JewelControl ( KDvoid )
{
	CC_SAFE_DELETE ( m_pJewelController );
}

KDbool JewelControl::init ( KDvoid )
{
	if ( !TestArtPig::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTouchEnabled ( KD_TRUE );
	m_pJewelController = new JewelController ( m_pSymbol );
	m_pSymbol->setPosition ( ccpAdd ( m_pSymbol->getPosition ( ), ccp ( 0, 50 ) ) );	

	return KD_TRUE;
}

const KDchar* JewelControl::subtitle ( KDvoid )
{
	return "Control Graphics";
}

const KDchar* JewelControl::getProjectID ( KDvoid )
{
	return "APP10028";
}

KDvoid JewelControl::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDbool JewelControl::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchBegan, pTouch->getLocation ( ), 1 ) );
	return KD_TRUE;
}

KDvoid JewelControl::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchMoved, pTouch->getLocation ( ), 1 ) );
}

KDvoid JewelControl::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pSymbol->dispatchTouch ( APSTouch ( kAPSEventTypeTouchEnded, pTouch->getLocation ( ), 1 ) );
}