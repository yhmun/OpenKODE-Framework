/* --------------------------------------------------------------------------
 *
 *      File            LevelEditor.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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
#include "LevelEditor.h"
#include "LevelFileHandler.h"
#include "Constants.h"
#include "GameScene.h"
#include "Model/PineappleModel.h"
#include "Model/RopeModel.h"
#include "Utils/CoordinateHelper.h"
#include "RopeSprite.h"
#include "LevelSelectionScene.h"


LevelEditor::LevelEditor ( LevelFileHelper* pLevelHelper )
{
	m_pFileHandler				= pLevelHelper;
	m_pBackground				= KD_NULL;
	m_pPineapplesSpriteSheet	= KD_NULL;
	m_pRopeSpriteSheet			= KD_NULL;
	m_pPopupMenu				= KD_NULL;
	m_pRopeSpritesArray			= KD_NULL;
	m_pNewRope					= KD_NULL;
	m_eMode						= kEditMode;
	m_pSelectedObject			= KD_NULL;
	m_pConnectedRopes			= KD_NULL;
}

LevelEditor::~LevelEditor ( KDvoid )
{
    CC_SAFE_RELEASE	( m_pRopeSpritesArray );
    CC_SAFE_DELETE	( m_pPopupMenu );
    CC_SAFE_DELETE	( m_pNewRope );
    CC_SAFE_RELEASE	( m_pConnectedRopes );
}

CCScene* LevelEditor::createWithLevel ( LevelFileHelper* pLevelHelper )
{
    CCScene*		pScene = CCScene::create ( );
    LevelEditor*	pLevel = new LevelEditor ( pLevelHelper );

    if ( pLevel ) 
	{
        pLevel->init ( );
        pScene->addChild ( pLevel );
        
        return pScene;
    }

    return KD_NULL;
}

KDbool LevelEditor::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}       
        
	this->createMenu ( );

	m_pBackground = CCSprite::createWithSpriteFrameName ( "bg.png" );	
	m_pBackground->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pBackground->setColor ( kDefaultBackgroundColor );
	this->addChild ( m_pBackground );

	// Load the sprite sheet into the sprite cache
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "CutTheVerlet.plist" );
	m_pPineapplesSpriteSheet = CCSpriteBatchNode::create ( "CutTheVerlet.pvr.ccz" );
	this->addChild ( m_pPineapplesSpriteSheet );

	m_pRopeSpriteSheet = CCSpriteBatchNode::create ( "rope_texture.png" );
	this->addChild ( m_pRopeSpriteSheet );

	this->drawLoadedLevel ( );

	this->setTouchEnabled ( KD_TRUE );
 
    return KD_TRUE;
}

KDvoid LevelEditor::createMenu ( KDvoid )
{    
	const CCSize&		tLyrSize = this->getContentSize ( );

    CCLabelTTF*			pSaveLabel = CCLabelTTF::create ( "Save", "font/Marker Felt.ttf", 24 );
    CCMenuItem*			pSaveItem  = CCMenuItemLabel::create ( pSaveLabel, this, menu_selector ( LevelEditor::saveLevel ) );
    
    CCLabelTTF*			pResetLabel = CCLabelTTF::create ( "Select Level", "font/Marker Felt.ttf", 24 );
    CCMenuItemLabel*	pResetItem  = CCMenuItemLabel::create ( pResetLabel, this, menu_selector ( LevelEditor::resetLevel ) );
    
    CCLabelTTF*			pPlayLabel = CCLabelTTF::create("Play Level", "font/Marker Felt.ttf", 24);
    CCMenuItemLabel*	pPlayLevelItem = CCMenuItemLabel::create ( pPlayLabel, this, menu_selector ( LevelEditor::playLevel ) );
    CCMenu*				pMenu = CCMenu::create ( pSaveItem, pResetItem, pPlayLevelItem, KD_NULL );
    pMenu->setPosition ( tLyrSize.cx / 2, pSaveItem->getContentSize ( ).cy / 2 );
    this->addChild ( pMenu, 100 ); 
    pMenu->alignItemsHorizontallyWithPadding ( 20 );        
}

KDvoid LevelEditor::resetLevel ( CCObject* pSender )
{    
    m_pPineapplesSpriteSheet->removeAllChildrenWithCleanup ( KD_TRUE );
    m_pRopeSpriteSheet->removeAllChildrenWithCleanup ( KD_TRUE );
    if ( m_pRopeSpritesArray )
	{
        m_pRopeSpritesArray->removeAllObjects ( );
    }
    
    if ( m_pConnectedRopes ) 
	{
        m_pConnectedRopes->removeAllObjects ( );
    }
    
    m_pSelectedObject = KD_NULL;
    
    // go to menu scene
    CCDirector::sharedDirector()->replaceScene ( LevelSelectionScene::scene ( ) );
}


KDvoid LevelEditor::playLevel ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->replaceScene ( GameScene::GameSceneWithLevelHandler ( m_pFileHandler ) );
}

KDvoid LevelEditor::saveLevel ( CCObject* pSender )
{
    m_pFileHandler->saveFile ( );
}

KDvoid LevelEditor::drawLoadedLevel ( KDvoid )
{
    // Draw pineapple
    CCArray*	pPienapple = m_pFileHandler->m_pPineapples;
    CCObject*	pObject;
    CCARRAY_FOREACH ( pPienapple, pObject )
    {
        PineappleModel*		pPineappleModel = (PineappleModel*) pObject;
        this->createPineappleSpriteFromModel ( pPineappleModel );
    }

    // Draw ropes
    m_pRopeSpritesArray = CCArray::createWithCapacity ( 5 );  
    m_pRopeSpritesArray->retain ( );
    CCARRAY_FOREACH ( m_pFileHandler->m_pRopes, pObject )
    {
        this->createRopeSpriteFromModel ( (RopeModel*) pObject );
    }
    
}

KDvoid LevelEditor::createRopeSpriteFromModel ( RopeModel* pRM )
{
    CCPoint		tAnchorA;
    if ( pRM->m_nBodyAId == -1 )
	{        
        tAnchorA = CoordinateHelper::levelPositioinToScreenPosition ( pRM->m_tAchorA );
    }
	else 
	{
        PineappleModel* pineappleWithID = m_pFileHandler->getPineappleWithID ( pRM->m_nBodyAId );
        tAnchorA = CoordinateHelper::levelPositioinToScreenPosition ( pineappleWithID->m_tPosition );
    }
    
    CCPoint		tAnchorB;

	if ( pRM->m_nBodyBId == -1 )
	{
        tAnchorB = CoordinateHelper::levelPositioinToScreenPosition( pRM->m_tAchorB);
    } 
	else 
	{        
        PineappleModel*		pPineappleWithID = m_pFileHandler->getPineappleWithID ( pRM->m_nBodyBId );
        tAnchorB = CoordinateHelper::levelPositioinToScreenPosition ( pPineappleWithID->m_tPosition );
    }
    

    RopeSprite*		pRopeSprite = new RopeSprite ( m_pRopeSpriteSheet, pRM );
    m_pRopeSpritesArray->addObject ( pRopeSprite );
    pRopeSprite->release ( );
}

void LevelEditor::createPineappleSpriteFromModel ( PineappleModel* pPM )
{
    CCSprite*	pPineappleSprite = CCSprite::createWithSpriteFrameName ( "pineapple.png" );    
    pPineappleSprite->setTag( pPM->m_nId );

    CCPoint		tPosition = CoordinateHelper::levelPositioinToScreenPosition ( pPM->m_tPosition );
    pPineappleSprite->setPosition ( tPosition );
    m_pPineapplesSpriteSheet->addChild ( pPineappleSprite );
}

KDvoid LevelEditor::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
    CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tTouchLocation = pTouch->getLocation ( );
       
    if ( pTouches->count ( ) == 2 )
	{
        this->longPress ( tTouchLocation );
    }
	else
	{
        switch ( m_eMode )
		{
            case kEditMode :                
                this->selectRopeAnchor ( tTouchLocation );
                if ( !m_pSelectedObject )
				{
                    this->selectPineapple ( tTouchLocation );
                }
                if ( !m_pSelectedObject && !this->ropeAtPosition ( tTouchLocation ) ) 
				{
                    this->togglePopupMenu ( tTouchLocation );
                }                
                break;

            case kRopeAnchorPineappleMode :
                this->selectFirstAnchorPoint ( tTouchLocation );
                break;

            case kRopeAnchorAnyMode :
                this->selectSecondAnchorPoint ( tTouchLocation );
                break;
        }
    }
        
}

KDvoid LevelEditor::longPress ( CCPoint tPos )
{
    this->removeRopeAtPosition ( tPos );
    this->removePineappleAtPosition ( tPos );
}

KDvoid LevelEditor::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{    
    if ( m_pSelectedObject && m_eMode == kEditMode )
	{
        CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
        CCPoint		tTouchLocation = pTouch->getLocation ( );        
        CCPoint		tOldTouchLocation = pTouch->getPreviousLocation ( );
             
        RopeSprite*		pRopeSprite = dynamic_cast<RopeSprite*> ( m_pSelectedObject );
        if ( pRopeSprite != KD_NULL )
		{
            pRopeSprite->moveSelectedAnchorTo ( tTouchLocation );
        }
        
        // TODO: move pineapple
        CCPoint		tMovementVector = ccpSub ( tTouchLocation, tOldTouchLocation );
        CCSprite*	pSelectedSprite = dynamic_cast<CCSprite*> ( m_pSelectedObject );
        if ( pSelectedSprite != KD_NULL ) 
		{            
            CCPoint		tNewPosition = ccpAdd ( pSelectedSprite->getPosition ( ), tMovementVector );
            pSelectedSprite->setPosition ( tNewPosition );
            // remember which ropes are tied to the selectedSprite
            CCObject*	pObject;
            CCARRAY_FOREACH ( m_pConnectedRopes, pObject )
            {
                RopeSprite*		pRope = (RopeSprite*) pObject;
                pRope->moveSelectedAnchorTo ( tNewPosition );
            }
        }
    }
}

KDvoid LevelEditor::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( m_eMode == kEditMode )
	{
        RopeSprite*		pRope = dynamic_cast<RopeSprite*> ( m_pSelectedObject );
        if ( KD_NULL != pRope )
		{            
            CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
            CCPoint		tTouchLocation = pTouch->getLocation ( );
            
            // snap to pineapple if rope ends up on top of it
            KDint		nNewAnchorID = -1;
            CCSprite*	pPineappleAtTouchLocation = this->pineappleAtPosition ( tTouchLocation );
            if ( pPineappleAtTouchLocation ) 
			{
                pRope->moveSelectedAnchorTo ( pPineappleAtTouchLocation->getPosition ( ) );
                // get id of pineapple to properly set anchor in connected data model
                nNewAnchorID = pPineappleAtTouchLocation->getTag ( );
            }
            
            KDbool		bIsValidNewAnchor = pRope->isValideNewAnchorID ( nNewAnchorID );
            if ( bIsValidNewAnchor ) 
			{             
                m_pFileHandler->moveRopeWithId ( pRope->getID ( ), pRope->getSelectedAnchorType ( ), nNewAnchorID,
                                             CoordinateHelper::screenPositionToLevelPosition ( tTouchLocation ) );
            } 
			else 
			{
                m_pFileHandler->moveRopeWithId ( pRope->getID ( ), pRope->getSelectedAnchorType ( ),
                                             pRope->getSelectedAnchorType ( ),
                                             CoordinateHelper::screenPositionToLevelPosition ( m_tOriginalRopeAnchor ) );
                
                pRope->moveSelectedAnchorTo ( m_tOriginalRopeAnchor );
            }
            
            // set selected anchor to none
            pRope->setSelectedAnchorType ( kAnchorNone );
        }
        
        // TODO: end moving pineapple
        CCSprite*	pPineappleSprite = dynamic_cast<CCSprite*> ( m_pSelectedObject );
        if ( pPineappleSprite != KD_NULL )
		{
            KDint		nOineappleID = pPineappleSprite->getTag ( );
            CCPoint		tNewPineapplePosition = CoordinateHelper::screenPositionToLevelPosition ( pPineappleSprite->getPosition ( ) );
            
            m_pFileHandler->movePineappleWithID ( nOineappleID, tNewPineapplePosition );
        }
        if ( m_pConnectedRopes != KD_NULL ) 
		{
            m_pConnectedRopes->removeAllObjects ( );
        }
        
        m_pSelectedObject = KD_NULL;
    }
}

KDvoid LevelEditor::togglePopupMenu ( CCPoint tTouchLocation )
{
    if ( !m_pPopupMenu ) 
	{
        m_pPopupMenu = new PopupMenu ( this );
        m_pPopupMenu->m_pDelegate = this;
    }
    
    if ( m_pPopupMenu->isEnabled ( ) )
	{
        m_pPopupMenu->setMenuEnabled ( KD_FALSE );
    }
	else
	{
        m_pPopupMenu->setPopupPositioin ( tTouchLocation );
        m_pPopupMenu->setMenuEnabled ( KD_TRUE );
        if ( m_pPineapplesSpriteSheet->getChildren ( )->count ( ) < 1 )
		{
            m_pPopupMenu->setRopeItemEnabled ( KD_FALSE );
        }
    }
}

KDvoid LevelEditor::createPineappleAt ( CCPoint tPosition )
{    
    const CCSize&	tLyrSize = this->getContentSize ( );
    PineappleModel*	pPM = m_pFileHandler->addPineappleAt ( ccp ( tPosition.x / tLyrSize.cx, tPosition.y / tLyrSize.cy ) );
    
    this->createPineappleSpriteFromModel ( pPM );
    m_pPopupMenu->setMenuEnabled ( KD_FALSE );

}

KDvoid LevelEditor::createRopeAt ( CCPoint tPosition )
{
    this->setMode ( kRopeAnchorPineappleMode );
    m_pNewRope = new RopeModel;
    m_pPopupMenu->setMenuEnabled ( KD_FALSE );
}

KDvoid LevelEditor::setMode ( EEditorMode eNewMode )
{
    m_eMode = eNewMode;
    CCObject*	pObject;
    
    switch ( m_eMode ) 
	{
        case kRopeAnchorPineappleMode :
            m_pBackground->setColor ( kDefaultDisabledColor );            
            CCARRAY_FOREACH ( m_pPineapplesSpriteSheet->getChildren ( ), pObject )
            {
                CCSprite*	pPineapple = (CCSprite*) pObject;
                pPineapple->setColor ( kDefaultSelectableColor );
            }                        
            break;

        case kRopeAnchorAnyMode :
            m_pBackground->setColor ( kDefaultSelectableColor );            
            CCARRAY_FOREACH ( m_pPineapplesSpriteSheet->getChildren ( ), pObject )
            {
                CCSprite *	pPineapple = (CCSprite*) pObject;
                if ( pPineapple->getTag ( ) == m_pNewRope->m_nBodyAId )
				{
                    pPineapple->setColor ( kDefaultDisabledColor );
                }
				else
				{
                    pPineapple->setColor( kDefaultSelectableColor );
                }
            }
            break;

        case kEditMode :
        default :
            m_pBackground->setColor ( kDefaultBackgroundColor );
            CCARRAY_FOREACH ( m_pPineapplesSpriteSheet->getChildren ( ), pObject )
            {
                CCSprite*	pPineapple = (CCSprite*) pObject;
                pPineapple->setColor ( ccc3 ( 255, 255, 255 ) );
            }            
            break;
    }
}

CCSprite* LevelEditor::pineappleAtPosition ( CCPoint tPosition )
{
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pPineapplesSpriteSheet->getChildren ( ), pObject )
    {
        CCSprite*	pPineapple = (CCSprite*) pObject;
        if ( pPineapple->boundingBox ( ).containsPoint ( tPosition ) ) 
		{
            return pPineapple;
        }
    }
    return KD_NULL;
}

KDvoid LevelEditor::selectFirstAnchorPoint ( CCPoint tTouchLocation )
{
    // if user tapped on pineapple set it as anchor of the new rope
    CCSprite*	pTappedPineapple = this->pineappleAtPosition ( tTouchLocation );
    
    if ( pTappedPineapple != NULL ) 
	{
        m_pNewRope->m_tAchorA  = CoordinateHelper::screenPositionToLevelPosition ( pTappedPineapple->getPosition ( ) );
        m_pNewRope->m_nBodyAId = pTappedPineapple->getTag ( );
        this->setMode ( kRopeAnchorAnyMode );
    }
}


KDvoid LevelEditor::selectSecondAnchorPoint ( CCPoint tTouchLocation )
{
    // set second end of rope, can be either background or other pinapple, but not same pinapple as first one

    CCSprite*	pTappedPineapple = this->pineappleAtPosition ( tTouchLocation );
    if ( pTappedPineapple && pTappedPineapple->getTag ( ) != m_pNewRope->m_nBodyAId ) 
	{
        m_pNewRope->m_tAchorB  = CoordinateHelper::screenPositionToLevelPosition ( pTappedPineapple->getPosition ( ) );
        m_pNewRope->m_nBodyBId = pTappedPineapple->getTag ( );
    }
    
    if ( !pTappedPineapple )
	{
        m_pNewRope->m_tAchorB  = CoordinateHelper::screenPositionToLevelPosition ( tTouchLocation );
        m_pNewRope->m_nBodyBId = -1;
    }
    
    this->createRopeSpriteFromModel  ( m_pNewRope );
    m_pFileHandler->addRopeFromModel ( m_pNewRope );
    
    this->setMode ( kEditMode );
}

RopeSprite* LevelEditor::ropeAtPosition ( CCPoint tPosition )
{
    KDfloat			fMinDistanceToRope = 5;
    RopeSprite*		pClosestRope = KD_NULL;
    CCObject*		pObject;
    CCARRAY_FOREACH ( m_pRopeSpritesArray, pObject )
    {
        RopeSprite*		pRopeSprite = (RopeSprite*) pObject;
        if ( pRopeSprite->getBoundingBox ( ).containsPoint ( tPosition ) && pRopeSprite->distanceToPoint ( tPosition ) )
		{
            pClosestRope = pRopeSprite;
            fMinDistanceToRope = pRopeSprite->distanceToPoint ( tPosition );
        }
    }
    return pClosestRope;
}

KDvoid LevelEditor::removeRopeAtPosition ( CCPoint tPosition )
{
    RopeSprite*		pSelectedRope = this->ropeAtPosition ( tPosition );
    if ( pSelectedRope ) 
	{
        pSelectedRope->cleanupSprite ( );
        m_pFileHandler->removeRopeWithID ( pSelectedRope->getID ( ) );
        m_pRopeSpritesArray->removeObject ( pSelectedRope );
        return;
    }
}

KDvoid LevelEditor::removePineappleAtPosition ( CCPoint tPosition )
{
    CCSprite*	pSelectedPineapple = this->pineappleAtPosition ( tPosition );
    if ( pSelectedPineapple != KD_NULL )
	{
        m_pFileHandler->removePineappleWithID ( pSelectedPineapple->getTag ( ) );
        
        CCArray*	pConnectedRopesToBeRemoved = this->getAllRopesConnectedToPineappleWithID ( pSelectedPineapple->getTag ( ) );
        
        CCObject*	pObject;
        CCARRAY_FOREACH ( pConnectedRopesToBeRemoved, pObject )
        {
            RopeSprite*		pRope = (RopeSprite*) pObject;
            m_pFileHandler->removeRopeWithID ( pRope->getID ( ) );
            pRope->cleanupSprite ( );
            m_pRopeSpritesArray->removeObject ( pRope );
        }
        
        pSelectedPineapple->removeFromParentAndCleanup ( KD_TRUE );        
    }
}

CCArray* LevelEditor::getAllRopesConnectedToPineappleWithID ( KDint nPineappleID )
{
    return this->getAllRopesConnectedToPineappleWithID ( nPineappleID, KD_FALSE );
}

KDvoid LevelEditor::selectRopeAnchor ( CCPoint tTouchLocation )
{
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pRopeSpritesArray, pObject )
    {
        RopeSprite*		pRope = (RopeSprite*) pObject;
        KDfloat			fDistanceToAnchorA = ccpDistance ( pRope->getAnchorA ( ), tTouchLocation );
        if ( fDistanceToAnchorA < 10 )
		{
            m_pSelectedObject = pRope;
            pRope->setSelectedAnchorType ( kAnchorA );
            m_tOriginalRopeAnchor = pRope->getAnchorA ( );
        }
        
        KDfloat			fDistanceToAnchorB = ccpDistance ( pRope->getAnchorB ( ), tTouchLocation );
        if ( fDistanceToAnchorB < 10 )
		{
            m_pSelectedObject = pRope;
            pRope->setSelectedAnchorType ( kAnchorB );
            m_tOriginalRopeAnchor = pRope->getAnchorB ( );
        }
        
        if ( m_pSelectedObject )
		{
            break;
        }
    }
}

KDvoid LevelEditor::selectPineapple ( CCPoint tTouchLocation )
{
    CCSprite*	pSprite = this->pineappleAtPosition ( tTouchLocation );
    if ( pSprite )
	{
        m_pSelectedObject = pSprite;
        // remember which ropes are tied to the selectedSprite
        m_pConnectedRopes = this->getAllRopesConnectedToPineappleWithID ( pSprite->getTag ( ), KD_TRUE );
        m_pConnectedRopes->retain ( );
    }
}

CCArray* LevelEditor::getAllRopesConnectedToPineappleWithID ( KDint nPineappleID, KDbool bSetSelectedAnchor )
{
    CCArray*	pTempRopes = CCArray::create ( );
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pRopeSpritesArray, pObject )
    {
        RopeSprite*  pRope = (RopeSprite*) pObject;
        KDint		 nBodyAID = pRope->getBodyAID ( );
        KDint		 nBodyBID = pRope->getBodyBID ( );
        
        if ( nPineappleID == nBodyAID || nPineappleID == nBodyBID )
		{
            pTempRopes->addObject ( pRope );
        }
        
        if ( bSetSelectedAnchor )
		{
            if ( nPineappleID == nBodyAID ) 
			{
                pRope->setSelectedAnchorType ( kAnchorA );
            }
			else if ( nPineappleID == nBodyBID )
			{
                pRope->setSelectedAnchorType ( kAnchorB );
            }
        }
    }
    
    return pTempRopes;
}

