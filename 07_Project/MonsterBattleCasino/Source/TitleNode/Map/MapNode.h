//
//  SapusIntroNode.h
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 Sapus Media. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "cocos2d.h"
#include <vector>
#include "RefPointee.h"


@class shopNode;
@class Sprite;
DefineIntrusivePtr(cLabelAtlasCostom);
DefineIntrusivePtr(cSprite);
DefineIntrusivePtr(cUnit);
DefineIntrusivePtr(cUnitCreateInfos);
DefineIntrusivePtr(cNodeChangeImgs);
@interface mapNode : CCLayer
{
	//time
	double m_tickCount;
	double m_beforeTickCount;

	//node change
	bool m_isNodeChangeIn;
	bool m_isNodeChangeOut;
	cNodeChangeImgsPtr m_pNodeChangeImg;

	//node
	shopNode* m_pShopNode;

	//logo
	cSpritePtr m_sprLogo;

	//back sprite
	CCSprite* m_sprBack;

	//hero
	cUnitCreateInfosPtr		m_heroCreateInfos;
	std::vector<cUnitPtr>	m_heros;

	//money
	cSpritePtr			 m_UI_moneyBack;
	cLabelAtlasCostomPtr m_UI_money;

	//status
	cSpritePtr			 m_status_back;
	cSpritePtr			 m_status_LevelBack_1;
	cSpritePtr			 m_status_LevelBack_2;
	cSpritePtr			 m_status_LevelBack_3;
	cLabelAtlasCostomPtr m_status_LevelLabel_1;
	cLabelAtlasCostomPtr m_status_LevelLabel_2;
	cLabelAtlasCostomPtr m_status_LevelLabel_3;
	cSpritePtr			 m_status_hpBarBack_1;
	cSpritePtr			 m_status_hpBarBack_2;
	cSpritePtr			 m_status_hpBarBack_3;
	cSpritePtr			 m_status_hpBarGage_1;
	cSpritePtr			 m_status_hpBarGage_2;
	cSpritePtr			 m_status_hpBarGage_3;
	cSpritePtr			 m_status_labelHP_Back_1;
	cSpritePtr			 m_status_labelHP_Back_2;
	cSpritePtr			 m_status_labelHP_Back_3;
	cLabelAtlasCostomPtr m_status_labelHP_1[2];
	cLabelAtlasCostomPtr m_status_labelHP_2[2];
	cLabelAtlasCostomPtr m_status_labelHP_3[2];

	//label potion
	cLabelAtlasCostomPtr m_labelPotionCountNum;

	//btns
	CCMenu* m_btnExit;
	CCMenu* m_btnParty;
	CCMenu* m_btnHeal;
	CCMenu* m_btnTower;
	CCMenu* m_btnColoseum;
	CCMenu* m_btnReinforme;
	CCMenu* m_btnShop;
	CCMenu* m_btnShopExit;

	//switchs
	bool m_isGoTower;
	bool m_isGoCasino;
}

+ (id)scene:(bool)isNodeChangeIn;

- (void)setNodeChangeIn;
- (bool)updateNodeChage:(double)leftTIme;
- (void)updateMoney;
- (void)updateStatus;


@end
