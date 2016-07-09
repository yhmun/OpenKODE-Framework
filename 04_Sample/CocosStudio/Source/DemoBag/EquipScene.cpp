/* -----------------------------------------------------------------------------------
 *
 *      File            EquipScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "Precompiled.h"
#include "EquipScene.h"

EquipScene::EquipScene ( KDvoid )
{
    Scene::init ( );
}

EquipScene::~EquipScene ( KDvoid )
{
    
}

KDvoid EquipScene::onEnter ( KDvoid )
{
    Scene::onEnter ( );
    
    m_pUILayer = UILayer::create ( );
    m_pUILayer->scheduleUpdate ( );
    this->addChild ( m_pUILayer );
    
    // equip initialize
    EquipInit ( );       
    
    // exit button
    UIButton*	pExitButton = UIButton::create ( );
    pExitButton->setTouchEnabled ( true );
    pExitButton->loadTextures ( "CloseNormal.png", "CloseSelected.png", "" );
    pExitButton->setPosition ( Point ( m_pUILayer->getContentSize ( ) - pExitButton->getContentSize ( ) ) );
	pExitButton->addReleaseEvent ( this, coco_releaseselector ( EquipScene::menuCloseCallback ) );
    m_pUILayer->addWidget ( pExitButton );   
}

KDvoid EquipScene::onExit ( KDvoid )
{
    m_pUILayer->removeFromParent ( );

    SceneReader::getInstance ( )->purgeSceneReader ( );
	GUIReader::shareReader ( )->purgeGUIReader ( );
	ActionManagerEx::purgeActionManager ( );

    Scene::onExit ( );
}

KDvoid EquipScene::menuCloseCallback ( Object* pSender )
{
	Director::getInstance ( )->end ( );    
}

// equip
KDvoid EquipScene::EquipInit ( KDvoid )
{
    m_eEquipType = EQUIP_TYPE_CLOTHES;
    
    m_pDicBeUsedSlot = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicBeUsedSlot );

    m_pDicClothesSlot = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicClothesSlot );

    m_pDicWeaponsSlot = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicWeaponsSlot );

    m_pDicPetsSlot = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicPetsSlot );
    
    m_pDicClothes = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicClothes );
    m_pDicWeapons = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicWeapons );
    m_pDicPets = Dictionary::create ( );
    CC_SAFE_RETAIN ( m_pDicPets );
    
    m_nContainer1Zorder = 0;
    m_nContainer2Zorder = 0;
    m_nContainer3Zorder = 0;
    
	m_tContainer1Position = Point::ZERO;
    m_tContainer2Position = Point::ZERO;
    m_tContainer3Position = Point::ZERO;

    m_tMovePoint = Point::ZERO;
    m_tLastPoint = Point::ZERO;
    
    m_tWidgetLastWorldSpace = Point::ZERO;
    m_tWidgetLastNodeSpace	= Point::ZERO;
    m_pLastWidgetParent		= KD_NULL;
    //        

    // equip root from json
    auto		pEquipeRoot = GUIReader::shareReader ( )->widgetFromJsonFile ( "cocosgui/gui_examples/equip_1/equip_1.json" );
    pEquipeRoot->setTag ( EQUIP_LAYOUT_TAG_ROOT );
    m_pUILayer->addWidget ( pEquipeRoot );        

    // title layout
    auto		pTitleLayout = pEquipeRoot->getChildByName ( "title_panel" );

    // close button
	auto		pCloseBtn = pTitleLayout->getChildByName ( "close_button" );
    pCloseBtn->setVisible ( false );

    // up layout
    auto		pUpLayout = pEquipeRoot->getChildByName ( "up_panel" );
    pUpLayout->setTag ( EQUIP_LAYOUT_TAG_UP );

    // switch button
    // close button
    auto		pClothesBtn = pUpLayout->getChildByName ( "clothes_button" );
    pClothesBtn->addReleaseEvent ( this, coco_releaseselector ( EquipScene::switchBtnCallBack ) );
    pClothesBtn->setTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_CLOTHES );
	pClothesBtn->setBright ( false );
    

    // weapons button
    auto		pWeaponsBtn = pUpLayout->getChildByName ( "weapons_button" );
    pWeaponsBtn->addReleaseEvent ( this, coco_releaseselector ( EquipScene::switchBtnCallBack ) );
    pWeaponsBtn->setTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_WEAPONS );
    
    // pets button
    auto		pPetsBtn = pUpLayout->getChildByName ( "pets_button" );
    pPetsBtn->addReleaseEvent ( this, coco_releaseselector ( EquipScene::switchBtnCallBack ) );
    pPetsBtn->setTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_PETS );

    // repertories
    // clothes layout
	auto		pClothesLayout = pEquipeRoot->getChildByName ( "clothes_panel" );
    pClothesLayout->setTag ( EQUIP_LAYOUT_TAG_CLOTHES );

    // weapons layout
    auto		pWeaponsLayout = pEquipeRoot->getChildByName ( "weapons_panel" );
    pWeaponsLayout->setTag ( EQUIP_LAYOUT_TAG_WEAPONS );

    // pets layout
    auto		pPetsLayout = pEquipeRoot->getChildByName ( "pets_panel" );
    pPetsLayout->setTag ( EQUIP_LAYOUT_TAG_PETS );

    m_nContainer1Zorder = pClothesLayout->getZOrder ( );
    m_nContainer2Zorder = pWeaponsLayout->getZOrder ( );
    m_nContainer3Zorder = pPetsLayout->getZOrder ( );

    m_tContainer1Position = pClothesLayout->getPosition ( );
    m_tContainer2Position = pWeaponsLayout->getPosition ( );
    m_tContainer3Position = pPetsLayout->getPosition ( );
    
    // clothes repertory slot
    for ( auto pObj : *pClothesLayout->getChildren ( ) )
    {
        auto	pChild = dynamic_cast<UIWidget*> ( pObj );
        m_pDicClothesSlot->setObject ( pChild, pChild->getName ( ) );
    }

    // weapons repertory slot
    for ( auto pObj : *pWeaponsLayout->getChildren ( ) )
    {
        auto	pChild = dynamic_cast<UIWidget*> ( pObj );
        m_pDicWeaponsSlot->setObject ( pChild, pChild->getName ( ) );
    }
    // pets repertory slot
    for ( auto pObj : *pPetsLayout->getChildren ( ) )
    {
        auto	pChild = dynamic_cast<UIWidget*> ( pObj );
        m_pDicPetsSlot->setObject ( pChild, pChild->getName ( ) );
    }

    // actor
    auto		pWallBGIv = pUpLayout->getChildByName ( "wall" );
    auto		pWalIv = UIImageView::create ( );
    pWalIv->loadTexture ( "cocosgui/gui_examples/equip_1/equip/eg/1.png" );
    pWalIv->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
    KDfloat		fWalX = pWallBGIv->getSize ( ).cx / 2.04f;
    KDfloat		fWalY = pWallBGIv->getSize ( ).cy / 2.4f;
    pWalIv->setPosition ( Point ( -pWallBGIv->getSize ( ).cx / 2 + fWalX, -pWallBGIv->getSize ( ).cy / 2 + fWalY ) );
    pWallBGIv->addChild ( pWalIv );

    // original clothes be used slot
    auto		pOriginalClothesSlotIv = pUpLayout->getChildByName ( "1" );
    auto		pOriginalClothesIv = UIImageView::create ( );
    pOriginalClothesIv->loadTexture ( "cocosgui/gui_examples/equip_1/equip/eg/6.png" );
    pOriginalClothesSlotIv->addChild ( pOriginalClothesIv );
    m_pDicBeUsedSlot->setObject ( pOriginalClothesSlotIv, pOriginalClothesSlotIv->getName ( ) );

    // other be used slot
    auto		pSlot2Iv = pUpLayout->getChildByName ( "2" );
    m_pDicBeUsedSlot->setObject(pSlot2Iv, pSlot2Iv->getName ( ) );
    auto		pSlot3Iv = pUpLayout->getChildByName ( "3" );
    m_pDicBeUsedSlot->setObject(pSlot3Iv, pSlot3Iv->getName ( ) );
    auto		pSlot4Iv = pUpLayout->getChildByName ( "4" );
    m_pDicBeUsedSlot->setObject(pSlot4Iv, pSlot4Iv->getName ( ) );
    auto		pSlot5Iv = pUpLayout->getChildByName ( "5" );
    m_pDicBeUsedSlot->setObject(pSlot5Iv, pSlot5Iv->getName ( ) );
    auto		pSlot6Iv = pUpLayout->getChildByName ( "6" );
    m_pDicBeUsedSlot->setObject(pSlot6Iv, pSlot6Iv->getName ( ) );
                                    	
    // equip create
    create ( );
    
    // initialize touch able and influence children
    DictElement*	pElement = KD_NULL;    

    pClothesLayout->setTouchEnabled ( true );

	CCDICT_FOREACH ( m_pDicClothesSlot, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( true );
	}

	CCDICT_FOREACH ( m_pDicClothes, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( true );
	}

	pWeaponsLayout->setTouchEnabled ( false );

	CCDICT_FOREACH ( m_pDicWeaponsSlot, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( false );
	}

	CCDICT_FOREACH ( m_pDicWeapons, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( false );
	}

    pPetsLayout->setTouchEnabled ( false );

	CCDICT_FOREACH ( m_pDicPetsSlot, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( false );
	}

	CCDICT_FOREACH ( m_pDicPets, pElement )
	{
		auto		pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        pChild->setTouchEnabled ( false );
	}
}

KDvoid EquipScene::create ( KDvoid )
{
    createClothes ( );
    createWeapons ( );
    createPets ( );
}

KDvoid EquipScene::createClothes ( KDvoid )
{
    auto			pClothesLayout = m_pUILayer->getWidgetByTag ( EQUIP_LAYOUT_TAG_CLOTHES );
    
    // clothes
    KDfloat			fParentW = pClothesLayout->getSize ( ).width;
    KDfloat			fParentH = pClothesLayout->getSize ( ).height;
    KDfloat			fOffestX = fParentW / 10;
    KDfloat			fOffestY = fParentH / 6;
    
    const KDshort	nColumnMax = 3;
    
    // jacket
    const KDchar*	aJacketPng [ nColumnMax ] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/3.png",
        "cocosgui/gui_examples/equip_1/equip/eg/4.png",
        "cocosgui/gui_examples/equip_1/equip/eg/5.png",
    };
    const KDchar*	aJacketName [ nColumnMax ] =
    {
        "jacket_taenia",
        "jacket_suit",
        "jacket_shoes",
    };

    // jacket imageview add to clothes slot
    for ( KDint i = 0; i < nColumnMax; ++i )
    {
        auto		pJacketIv = UIImageView::create ( );
        pJacketIv->loadTexture ( aJacketPng [ i ] );
        pJacketIv->setAnchorPoint ( Point ( 0.5, 0.5 ) );
        pJacketIv->setPosition ( Point ( fOffestX + i * fOffestX * 4, fParentH - fOffestY ) );
        auto		pLastChild = dynamic_cast<UIWidget*> ( pClothesLayout->getChildren()->getLastObject ( ) );
        if ( pLastChild )
        {
            pJacketIv->setZOrder ( pLastChild->getZOrder ( ) + 1 );
        }
        pJacketIv->setName ( aJacketName [ i ] );
        pJacketIv->setTouchEnabled ( true );
        pJacketIv->addPushDownEvent ( this, coco_pushselector ( EquipScene::touch ) );
        pJacketIv->addMoveEvent ( this, coco_moveselector ( EquipScene::move ) );
        pJacketIv->addReleaseEvent ( this, coco_releaseselector ( EquipScene::drop ) );
        
        for ( auto pObj : *pClothesLayout->getChildren ( ) )
        {
            auto*	pSlot = dynamic_cast<UIWidget*> ( pObj );
            pSlot->setCascadeColorEnabled ( false );
            if ( pSlot->getRenderer ( )->getBoundingBox ( ).containsPoint ( pJacketIv->getPosition ( ) ) )
            {
                pJacketIv->setPosition ( Point::ZERO );
                pSlot->addChild ( pJacketIv );
                break;
            }
        }
        
        m_pDicClothes->setObject ( pJacketIv, pJacketIv->getName ( ) );
    }
    
    // kimono
    const KDchar*	aKimonoPng [ nColumnMax ] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/8.png",
        "cocosgui/gui_examples/equip_1/equip/eg/10.png",
        "cocosgui/gui_examples/equip_1/equip/eg/9.png",
    };
    const KDchar*	aKimonoName [ nColumnMax ] =
    {
        "kimono_strawhat",
        "kimono_suit",
        "kimono_shoes",
    };
    // kimono imageview add to clothes slot
    for ( KDint i = 0; i < nColumnMax; ++i )
    {
        auto	pKimonoIv = UIImageView::create ( );
        pKimonoIv->loadTexture ( aKimonoPng [ i ] );
        pKimonoIv->setPosition ( Point ( fOffestX + i * fOffestX * 4, fParentH - fOffestY * 3 ) );
        auto	pLastChild = dynamic_cast<UIWidget*> ( pClothesLayout->getChildren ( )->getLastObject ( ) );
        if ( pLastChild )
        {
            pKimonoIv->setZOrder ( pLastChild->getZOrder ( ) + 1 );
        }
        pKimonoIv->setName ( aKimonoName [ i ] );
        pKimonoIv->setTouchEnabled ( true );
        pKimonoIv->addPushDownEvent ( this, coco_pushselector ( EquipScene::touch ) );
        pKimonoIv->addMoveEvent ( this, coco_moveselector ( EquipScene::move ) );
        pKimonoIv->addReleaseEvent ( this, coco_releaseselector ( EquipScene::drop ) );
        
        for ( auto pObj : *pClothesLayout->getChildren ( ) )
        {
            auto	pSlot = dynamic_cast<UIWidget*> ( pObj );
            pSlot->setCascadeColorEnabled ( false );
            if ( pSlot->getRenderer()->getBoundingBox ( ).containsPoint ( pKimonoIv->getPosition ( ) ) )
            {
                pKimonoIv->setPosition ( Point::ZERO );
                pSlot->addChild ( pKimonoIv );
                break;
            }            
        }
        
        m_pDicClothes->setObject ( pKimonoIv, pKimonoIv->getName ( ) );
    }
}

KDvoid EquipScene::createWeapons ( KDvoid )
{
	/*
    Layout* weapons_panel = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(EQUIP_LAYOUT_TAG_WEAPONS));
    
    // weapons
    float parent_w = weapons_panel->getSize().width;
    float parent_h = weapons_panel->getSize().height;
    float offest_x = parent_w / 10;
    float offest_y = parent_h / 6;
    
    // sword
    const short swordAmount = 3;
    const char* sword_png[swordAmount] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/train.png",
        "cocosgui/gui_examples/equip_1/equip/eg/research.png",
        "cocosgui/gui_examples/equip_1/equip/eg/upgrade.png",
    };
    const char* sword_name[swordAmount] =
    {
        "sword_sword",
        "sword_plus",
        "sword_hammer",
    };
    // sword imageview add to weapons slot
    for (int i = 0; i < swordAmount; ++i)
    {
        UIImageView* sword_iv = UIImageView::create();
        sword_iv->setTexture(sword_png[i]);
        sword_iv->setPosition(ccp(offest_x, parent_h - offest_y - i * (offest_y * 2)));
        UIWidget* lastChild = dynamic_cast<UIWidget*>(weapons_panel->getChildren()->lastObject ( ) );
        if (lastChild)
        {
            sword_iv->setZOrder ( lastpChild->getZOrder() + 1);
        }
        sword_iv->setName(sword_name[i]);
        sword_iv->addPushDownEvent(this, coco_pushselector(EquipScene::touch));
        sword_iv->addMoveEvent(this, coco_moveselector(EquipScene::move));
        sword_iv->addReleaseEvent(this, coco_releaseselector(EquipScene::drop));
        
        Object* obj = NULL;
        CCARRAY_FOREACH(weapons_panel->getChildren(), obj)
        {
            UIWidget* slot = dynamic_cast<UIWidget*>(obj);
            slot->setCascadeColorEnabled ( false );
            if (slot->getRenderer()->boundingBox().containsPoint(sword_iv->getPosition ( ) ))
            {
                sword_iv->setPosition(PointZero);
                slot->addChild(sword_iv);
                break;
            }
        }
        
        m_pDicWeapons->setObject(sword_iv, sword_iv->getName ( ) );
    }
    
    // arrow
    const short arrowAmount = 3;
    const char* arrow_png[arrowAmount] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/shop_shield_1.png",
        "cocosgui/gui_examples/equip_1/equip/eg/shop_shield_2.png",
        "cocosgui/gui_examples/equip_1/equip/eg/shop_shield_3.png",
    };
    const char* arrow_name[arrowAmount] =
    {
        "arraw_normal",
        "arraw_hard",
        "arraw_devil",
    };
    // arrow imageview add to weapons slot
    for (int i = 0; i < arrowAmount; ++i)
    {
        UIImageView* arrow_iv = UIImageView::create();
        arrow_iv->setTexture(arrow_png[i]);
        arrow_iv->setPosition(ccp(offest_x * 5, parent_h - offest_y - i * (offest_y * 2)));
        UIWidget* lastChild = dynamic_cast<UIWidget*>(weapons_panel->getChildren()->lastObject ( ) );
        if (lastChild)
        {
            arrow_iv->setZOrder ( lastpChild->getZOrder() + 1);
        }
        arrow_iv->setName(arrow_name[i]);
        arrow_iv->addPushDownEvent(this, coco_pushselector(EquipScene::touch));
        arrow_iv->addMoveEvent(this, coco_moveselector(EquipScene::move));
        arrow_iv->addReleaseEvent(this, coco_releaseselector(EquipScene::drop));
        
        Object* obj = NULL;
        CCARRAY_FOREACH(weapons_panel->getChildren(), obj)
        {
            UIWidget* slot = dynamic_cast<UIWidget*>(obj);
            slot->setCascadeColorEnabled ( false );
            if (slot->getRenderer()->boundingBox().containsPoint(arrow_iv->getPosition ( ) ))
            {
                arrow_iv->setPosition(PointZero);
                slot->addChild(arrow_iv);
                break;
            }
        }
        
        m_pDicWeapons->setObject(arrow_iv, arrow_iv->getName ( ) );
    }
    
    // bomb
    const short bombAmount = 1;
    const char* bomb_png[bombAmount] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/sell.png",
    };
    const char* bomb_name[bombAmount] =
    {
        "bomb_normal",
    };
    // bomo imageview add to weapons slot
    for (int i = 0; i < bombAmount; ++i)
    {
        UIImageView* bomb_iv = UIImageView::create();
        bomb_iv->setTexture(bomb_png[i]);
        bomb_iv->setPosition(ccp(offest_x * 9, parent_h - offest_y - i * (offest_y * 2)));
        UIWidget* lastChild = dynamic_cast<UIWidget*>(weapons_panel->getChildren()->lastObject ( ) );
        if (lastChild)
        {
            bomb_iv->setZOrder ( lastpChild->getZOrder() + 1);
        }
        bomb_iv->setName(bomb_name[i]);
        bomb_iv->addPushDownEvent(this, coco_pushselector(EquipScene::touch));
        bomb_iv->addMoveEvent(this, coco_moveselector(EquipScene::move));
        bomb_iv->addReleaseEvent(this, coco_releaseselector(EquipScene::drop));
        
        Object* obj = NULL;
        CCARRAY_FOREACH(weapons_panel->getChildren(), obj)
        {
            UIWidget* slot = dynamic_cast<UIWidget*>(obj);
            slot->setCascadeColorEnabled ( false );
            if (slot->getRenderer()->boundingBox().containsPoint(bomb_iv->getPosition ( ) ))
            {
                bomb_iv->setPosition(PointZero);
                slot->addChild(bomb_iv);
                break;
            }
        }
        
        m_pDicWeapons->setObject(bomb_iv, bomb_iv->getName ( ) );
    }
	*/
}

KDvoid EquipScene::createPets ( KDvoid )
{
	/*
    Layout* pets_layout = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(EQUIP_LAYOUT_TAG_PETS));
    
    // pets
    float parent_w = pets_layout->getSize().width;
    float parent_h = pets_layout->getSize().height;
    float offest_x = parent_w / 10;
    float offest_y = parent_h / 6;
    
    // dragon
    const short dragonAmount = 1;
    const char* dragon_png[dragonAmount] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/7.png",
    };
    const char* dragon_name[dragonAmount] =
    {
        "pet_dragon",
    };
    // dragon imageview add to pets slot
    for (int i = 0; i < dragonAmount; ++i)
    {
        UIImageView* dragon_iv = UIImageView::create();
        dragon_iv->setTexture(dragon_png[i]);
        dragon_iv->setPosition(ccp(offest_x, parent_h - offest_y - i * (offest_y * 2)));
        UIWidget* lastChild = dynamic_cast<UIWidget*>(pets_layout->getChildren()->lastObject ( ) );
        if (lastChild)
        {
            dragon_iv->setZOrder ( lastpChild->getZOrder() + 1);
        }
        dragon_iv->setName(dragon_name[i]);
        dragon_iv->addPushDownEvent(this, coco_pushselector(EquipScene::touch));
        dragon_iv->addMoveEvent(this, coco_moveselector(EquipScene::move));
        dragon_iv->addReleaseEvent(this, coco_releaseselector(EquipScene::drop));
        
        Object* obj = NULL;
        CCARRAY_FOREACH(pets_layout->getChildren(), obj)
        {
            UIWidget* slot = dynamic_cast<UIWidget*>(obj);
            slot->setCascadeColorEnabled ( false );            
            if (slot->getRenderer()->boundingBox().containsPoint(dragon_iv->getPosition ( ) ))
            {
                dragon_iv->setPosition(PointZero);
                slot->addChild(dragon_iv);
                break;
            }
        }
        
        m_pDicPets->setObject(dragon_iv, dragon_iv->getName ( ) );
    }
    
    // crab
    const short crabAmount = 1;
    const char* crab_png[crabAmount] =
    {
        "cocosgui/gui_examples/equip_1/equip/eg/crab.png",
    };
    const char* crab_name[crabAmount] =
    {
        "pet_crab",
    };
    // crab imageview add to pets slot
    for (int i = 0; i < crabAmount; ++i)
    {
        UIImageView* crab_iv = UIImageView::create();
        crab_iv->setTexture(crab_png[i]);
        crab_iv->setPosition(ccp(offest_x * 5, parent_h - offest_y - i * (offest_y * 2)));
        UIWidget* lastChild = dynamic_cast<UIWidget*>(pets_layout->getChildren()->lastObject ( ) );
        if (lastChild)
        {
            crab_iv->setZOrder ( lastpChild->getZOrder() + 1);
        }
        crab_iv->setName(crab_name[i]);
        crab_iv->addPushDownEvent(this, coco_pushselector(EquipScene::touch));
        crab_iv->addMoveEvent(this, coco_moveselector(EquipScene::move));
        crab_iv->addReleaseEvent(this, coco_releaseselector(EquipScene::drop));
        
        Object* obj = NULL;
        CCARRAY_FOREACH(pets_layout->getChildren(), obj)
        {
            UIWidget* slot = dynamic_cast<UIWidget*>(obj);
            slot->setCascadeColorEnabled ( false );
            if (slot->getRenderer()->boundingBox().containsPoint(crab_iv->getPosition ( ) ))
            {
                crab_iv->setPosition(PointZero);
                slot->addChild(crab_iv);
                break;
            }                        
        }
        
        m_pDicPets->setObject(crab_iv, crab_iv->getName ( ) );
    }
    */
}

KDvoid EquipScene::switchBtnCallBack ( Object* pSender )
{
    // switch button
    auto	pButton = dynamic_cast<UIButton*> ( pSender );
    
    // equip root
    auto	pRoot = m_pUILayer->getWidgetByTag ( EQUIP_LAYOUT_TAG_ROOT ) ;
    
    // equip repertory
    auto	pClothesLayout	= pRoot->getChildByTag ( EQUIP_LAYOUT_TAG_CLOTHES );
    auto	pWeaponsLayout	= pRoot->getChildByTag ( EQUIP_LAYOUT_TAG_WEAPONS );
    auto	pPetsLayout		= pRoot->getChildByTag ( EQUIP_LAYOUT_TAG_PETS );
    
    auto	pUpLayout		= pRoot->getChildByTag ( EQUIP_LAYOUT_TAG_UP );
    auto	pClothesBtn		= pUpLayout->getChildByTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_CLOTHES );
    auto	pWeaponsBtn		= pUpLayout->getChildByTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_WEAPONS );
    auto	pPetsBtn		= pUpLayout->getChildByTag ( EQUIP_SWITCH_LAYOUT_BUTTON_TAG_PETS );
    
    pClothesBtn->setActive ( true );
    pWeaponsBtn->setActive ( true );
    pPetsBtn->setActive ( true );
    
    DictElement*	pElement = KD_NULL;    
    // switch slot
    switch ( pButton->getTag ( ) )
    {
        case EQUIP_SWITCH_LAYOUT_BUTTON_TAG_CLOTHES :
        {
            m_eEquipType = EQUIP_TYPE_CLOTHES;
            
            pClothesBtn->setActive ( false );
            
            pClothesLayout->setZOrder ( m_nContainer1Zorder );
            pWeaponsLayout->setZOrder ( m_nContainer2Zorder );
            pPetsLayout->setZOrder ( m_nContainer3Zorder );
            
            pClothesLayout->setPosition ( m_tContainer1Position );
            pWeaponsLayout->setPosition ( m_tContainer2Position );
            pPetsLayout->setPosition ( m_tContainer3Position );
            
            // equip slot touch able
            pClothesLayout->setTouchEnabled ( true );
            CCDICT_FOREACH ( m_pDicClothesSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            CCDICT_FOREACH ( m_pDicClothes, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            pWeaponsLayout->setTouchEnabled ( false );
            CCDICT_FOREACH ( m_pDicWeaponsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicWeapons, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            pPetsLayout->setTouchEnabled ( false );
            CCDICT_FOREACH ( m_pDicPetsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicPets, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
        }
            break;
            
        case EQUIP_SWITCH_LAYOUT_BUTTON_TAG_WEAPONS :
        {
            m_eEquipType = EQUIP_TYPE_WEAPONS;
            
            pWeaponsBtn->setActive ( false );
            
            pClothesLayout->setZOrder ( m_nContainer3Zorder );
            pWeaponsLayout->setZOrder ( m_nContainer1Zorder );
            pPetsLayout->setZOrder ( m_nContainer2Zorder );
            
            pWeaponsLayout->setPosition ( m_tContainer1Position );
            pPetsLayout->setPosition ( m_tContainer2Position );
            pClothesLayout->setPosition ( m_tContainer3Position );
            
            // equip slot touch able
            pWeaponsLayout->setTouchEnabled ( true );
            CCDICT_FOREACH ( m_pDicWeaponsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            CCDICT_FOREACH ( m_pDicWeapons, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            pClothesLayout->setTouchEnabled ( false );
   
            CCDICT_FOREACH ( m_pDicClothesSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicClothes, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }            
            pPetsLayout->setTouchEnabled ( false );
            CCDICT_FOREACH ( m_pDicPetsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicPets, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
        }
            break;
            
        case EQUIP_SWITCH_LAYOUT_BUTTON_TAG_PETS :
        {
            m_eEquipType = EQUIP_TYPE_PETS;
            
            pPetsBtn->setActive ( false );
            
            pPetsLayout->setZOrder ( m_nContainer1Zorder );
            pClothesLayout->setZOrder ( m_nContainer2Zorder );
            pWeaponsLayout->setZOrder ( m_nContainer3Zorder );
            
            pPetsLayout->setPosition ( m_tContainer1Position );
            pClothesLayout->setPosition ( m_tContainer2Position );
            pWeaponsLayout->setPosition ( m_tContainer3Position );
            
            // equip slot touch able
            pPetsLayout->setTouchEnabled ( true );
            CCDICT_FOREACH ( m_pDicPetsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            CCDICT_FOREACH ( m_pDicPets, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( true );
            }
            pClothesLayout->setTouchEnabled ( false );
            CCDICT_FOREACH ( m_pDicClothesSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicClothes, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            pWeaponsLayout->setTouchEnabled ( false );
            CCDICT_FOREACH ( m_pDicWeaponsSlot, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }
            CCDICT_FOREACH ( m_pDicWeapons, pElement )
            {
                auto	pChild = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
                pChild->setTouchEnabled ( false );
            }            
        }
            break;
            
        default :
            break;
    }
}

KDvoid EquipScene::touch ( Object* pSender )
{
    auto	pWidget = dynamic_cast<UIWidget*> ( pSender );
    Point	tWorldSpace = pWidget->getRenderer ( )->convertToWorldSpace ( Point::ZERO );
    
    m_tWidgetLastWorldSpace = tWorldSpace;
    m_tWidgetLastNodeSpace  = pWidget->getPosition ( );
    
    m_pLastWidgetParent = pWidget->getParent ( );
    pWidget->removeFromParent ( );
    
    m_pUILayer->addWidget ( pWidget );
    
    pWidget->setPosition ( pWidget->getTouchStartPos ( ) );
    m_tMovePoint = pWidget->getTouchStartPos ( );
}

KDvoid EquipScene::move ( Object* pSender )
{
    UIWidget*	pWidget = dynamic_cast<UIWidget*> ( pSender );
    
    m_tLastPoint = m_tMovePoint;
    m_tMovePoint = pWidget->getTouchMovePos ( );

    Point		tOffset  = m_tMovePoint - m_tLastPoint;
    Point		tToPoint = pWidget->getPosition ( ) + tOffset;
    pWidget->setPosition ( tToPoint );
}

KDvoid EquipScene::drop ( Object* pSender )
{
/*
    bool isInUsedSlot = false;
    bool isInEquipSlot = false;
    
    UIWidget* widget = dynamic_cast<UIWidget*>(pSender);
    
    CCDictElement* element = NULL;
    
    // drop into used slot
    CCDICT_FOREACH ( m_pDicBeUsedSlot, pElement )
    {
        UIWidget* usedSlot_wigt = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        if (usedSlot_wigt->getChildren()->count() > 0)
        {
            continue;
        }
                
        if (usedSlot_wigt->hitTest(widget->getPosition ( ) ))
        {
            widget->removeFromParentAndCleanup ( false );
            widget->setPosition(PointZero);
            usedSlot_wigt->addChild(widget);
            
            isInUsedSlot = true;
            break;
        }
    }
    
    // drop into equip slot
    Dictionary* equipSlot_dic = NULL;
    Dictionary* equip_dic = NULL;
    switch (m_eEquipType)
    {
        case EQUIP_TYPE_CLOTHES:
            equipSlot_dic = m_pDicClothesSlot;
            equip_dic = m_pDicClothes;
            break;
            
        case EQUIP_TYPE_WEAPONS:
            equipSlot_dic = m_pDicWeaponsSlot;
            equip_dic = m_pDicWeapons;
            break;
            
        case EQUIP_TYPE_PETS:
            equipSlot_dic = m_pDicPetsSlot;
            equip_dic = m_pDicPets;
            break;
            
        default:
            break;
    }
    
    CCDICT_FOREACH ( equipSlot_dic, pElement )
    {
        UIWidget* equipSlot = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        if (equipSlot->getChildren()->count() > 0)
        {
            continue;
        }
        
        if (equipSlot->hitTest(widget->getPosition ( ) ))
        {
            Object* obj = equip_dic->objectForKey(widget->getName ( ) );
            if (obj)
            {
                widget->removeFromParentAndCleanup ( false );
                widget->setPosition(PointZero);
                equipSlot->addChild(widget);
                
                isInEquipSlot = true;
            }
            break;
        }
    }
    
    // back to last position if cannot drop other slot
    if (!isInUsedSlot && !isInEquipSlot)
    {
        Point point = m_tWidgetLastWorldSpace;
        CCMoveTo* moveTo = CCMoveTo::create(1.0f, point);
        CCEaseExponentialOut* ease = CCEaseExponentialOut::create(moveTo);
        CCCallFuncO* calllFunc0 = CCCallFuncO::create(this, callfuncO_selector(EquipScene::backOver), widget);
        CCSequence* seq = CCSequence::create(ease, calllFunc0, NULL);
        widget->runAction(seq);
        
        // widget in equip slot
        CCDICT_FOREACH ( equip_dic, pElement )
        {
            UIWidget* widget = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
            widget->setTouchEnabled ( false );
        }
        
        // equip up layout
        Layout* equipe_root = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(EQUIP_LAYOUT_TAG_ROOT));
        Layout* up_layout = dynamic_cast<Layout*>(equipe_root->getChildByName("up_panel"));
        Object* obj = NULL;
        up_layout->setTouchEnabled ( false );
        CCARRAY_FOREACH(up_layout->getChildren(), obj)
        {
            auto pChild = dynamic_cast<UIWidget*>(obj);
            pChild->setTouchEnabled ( false );
        }
    }
*/
}

void EquipScene::backOver(Object *pObject)
{
/*
    UIWidget* widget = dynamic_cast<UIWidget*>(pObject);
    
    widget->removeFromParentAndCleanup ( false );
    m_pLastWidgetParent->addChild(widget);
    widget->setPosition ( m_tWidgetLastNodeSpace);
    
    CCDictElement* element = NULL;
    
    // equip
    Dictionary* equip_dic = NULL;
    switch (m_eEquipType)
    {
        case EQUIP_TYPE_CLOTHES:
            equip_dic = m_pDicClothes;
            break;
            
        case EQUIP_TYPE_WEAPONS:
            equip_dic = m_pDicWeapons;
            break;
            
        case EQUIP_TYPE_PETS:
            equip_dic = m_pDicPets;
            break;
            
        default:
            break;
    }
    CCDICT_FOREACH ( equip_dic, pElement )
    {
        UIWidget* widget = dynamic_cast<UIWidget*> ( pElement->getObject ( ) );
        widget->setTouchEnabled ( true );
    }
    
    // equip up layout
    Layout* equip_root = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(EQUIP_LAYOUT_TAG_ROOT));
    Layout* up_layout = dynamic_cast<Layout*>(equip_root->getChildByName("up_panel"));
    up_layout->setTouchEnabled ( true );
    Object* obj = NULL;
    CCARRAY_FOREACH(up_layout->getChildren(), obj)
    {
        auto pChild = dynamic_cast<UIWidget*>(obj);
        pChild->setTouchEnabled ( true );
    }
*/
}

KDvoid EquipScene::close ( Object* pObject )
{
    // equip
    auto	pEquipRoot = m_pUILayer->getWidgetByTag ( EQUIP_LAYOUT_TAG_ROOT );
    pEquipRoot->setVisible ( false );
}