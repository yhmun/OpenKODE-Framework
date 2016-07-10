//
//  SapusIntroNode.m
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "mapNode.h"
#import "mainNode.h"
#import "shopNode.h"
#import "formationNode.h"
#import "towerEntranceNode.h"
#import "casinoBetNode.h"
#import "SimpleAudioEngine.h"
#include "LayerDefine.h"
#include "nodeChangeImgs.h"
#include "sprite.h"
#include "labelAtlasCostom.h"
#include "myData.h"
#include "unitCreateInfos.h"
#include "unit.h"



@implementation mapNode



//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
+ (id)scene:(bool)isNodeChangeIn
{
	CCScene *s = [CCScene node];
	id node = [mapNode node];
	[s addChild:node];

	if(isNodeChangeIn == true)
	{
		[node setNodeChangeIn];
	}

	return s;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (id)init
{
	if( ![super init])
		return nil;

	//switch
	isTouchEnabled = YES;

	//time
	m_tickCount		  = 0;
	m_beforeTickCount = 0;

	//node
	m_pShopNode = nil;

	//logo
	m_sprLogo = new cSprite( self, @"map_logo.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(-300, 240) );
	m_sprLogo->setPtTime( CGPointMake(0, 240), 1.0f );

	//back
	m_sprBack = [CCSprite spriteWithFile:@"map_back.png"];
	m_sprBack.position	  = CGPointZero;
	m_sprBack.anchorPoint = CGPointZero;
	[self addChild:m_sprBack z:LAYER_BACK];

	//hero create info
	m_heroCreateInfos = new cUnitCreateInfos("heroCreateInfos.par");

	//heros
	if(cMyData::getObj()->m_heroSaveDatas.size() == 0)
	{
		cUnitPtr hero = new cUnit(self, m_heroCreateInfos->getUnitInfo(eHeroCreateType_Worrier), eCreateInfoFileType_Hero, LAYER_MAP_HERO, cMyData::getObj()->getNewHeroID(), eHeroCreateType_Worrier, 0);
		hero->setPtPortrait(CGPointMake(393, 282) );
		hero->setVisiblePortrait(true);
		hero->texLoad(eUnitAniType_Portrait);
		m_heros.push_back(hero);

		cMyData::getObj()->updateHeroSaveDatas(m_heros);
	}
	else
	{
		for(int i = 0; i < (int)cMyData::getObj()->m_heroSaveDatas.size(); i++)
		{
			const cHeroSaveData& saveData = cMyData::getObj()->m_heroSaveDatas[i];

			cUnitPtr hero = new cUnit(self, m_heroCreateInfos->getUnitInfo(saveData.createIndex), eCreateInfoFileType_Hero, LAYER_MAP_HERO, saveData.heroID, saveData.createIndex, saveData.seatIndex);
			hero->texLoad(eUnitAniType_Portrait);
			hero->load(saveData.seatIndex, (ePositionType)saveData.positionType, saveData.level, saveData.hp, saveData.use_hp, saveData.atk, saveData.def, saveData.agl, saveData.crt, saveData.skills);
			hero->setVisiblePortrait(false);

			switch( hero->getSeatIndex() )
			{
			case 0	:	hero->setPtPortrait(CGPointMake(393, 282) );	hero->setVisiblePortrait(true);		break;
			case 1	:	hero->setPtPortrait(CGPointMake(393, 225) );	hero->setVisiblePortrait(true);		break;
			case 2	:	hero->setPtPortrait(CGPointMake(393, 168) );	hero->setVisiblePortrait(true);		break;
			}

			m_heros.push_back(hero);
		}
	}

	//money
	m_UI_moneyBack = new cSprite( self, @"map_money_back.png", LAYER_MAP_MONEY, CGPointMake(0, 0), CGPointMake(0, 295) );

	m_UI_money = new cLabelAtlasCostom(self, @"coin_nums_s.png", 15, LAYER_MAP_MONEY);
	m_UI_money->addCharInfo(',',  7);	m_UI_money->addCharInfo('0', 15);	m_UI_money->addCharInfo('1', 15);
	m_UI_money->addCharInfo('2', 15);	m_UI_money->addCharInfo('3', 15);	m_UI_money->addCharInfo('4', 15);
	m_UI_money->addCharInfo('5', 15);	m_UI_money->addCharInfo('6', 15);	m_UI_money->addCharInfo('7', 15);
	m_UI_money->addCharInfo('8', 15);	m_UI_money->addCharInfo('9', 15);

	[self updateMoney];

	//status
	m_status_back = new cSprite( self, @"map_ui_back.png", LAYER_STATUS_BACK, CGPointMake(0, 0), CGPointMake(375, 64) );

	m_status_LevelBack_1 = new cSprite( self, @"map_ui_level_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(422, 243) );
	m_status_LevelBack_2 = new cSprite( self, @"map_ui_level_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(422, 186) );
	m_status_LevelBack_3 = new cSprite( self, @"map_ui_level_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(422, 129) );

	for(int i = 0; i < 3; i++)
	{
		cLabelAtlasCostomPtr loopLabel = NULL;

			 if(i == 0)	loopLabel = m_status_LevelLabel_1 = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 1)	loopLabel = m_status_LevelLabel_2 = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 2)	loopLabel = m_status_LevelLabel_3 = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);

		loopLabel->addCharInfo('1', 5);		loopLabel->addCharInfo('2', 6);		loopLabel->addCharInfo('3', 6);
		loopLabel->addCharInfo('4', 6);		loopLabel->addCharInfo('5', 6);		loopLabel->addCharInfo('6', 6);
		loopLabel->addCharInfo('7', 6);		loopLabel->addCharInfo('8', 6);		loopLabel->addCharInfo('9', 6);
		loopLabel->addCharInfo('0', 6);

		loopLabel->setColor(255, 0, 0, 0);
	}		

	m_status_hpBarBack_1 = new cSprite( self, @"map_ui_hpbar_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(382, 227) );
	m_status_hpBarBack_2 = new cSprite( self, @"map_ui_hpbar_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(382, 170) );
	m_status_hpBarBack_3 = new cSprite( self, @"map_ui_hpbar_back.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(382, 113) );
	m_status_hpBarGage_1 = new cSprite( self, @"map_ui_hpbar.png"	  , LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(383, 228) );
	m_status_hpBarGage_2 = new cSprite( self, @"map_ui_hpbar.png"	  , LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(383, 171) );
	m_status_hpBarGage_3 = new cSprite( self, @"map_ui_hpbar.png"	  , LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(383, 114) );

	m_status_labelHP_Back_1 = new cSprite( self, @"map_ui_hpLabelBack.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(392, 229) );
	m_status_labelHP_Back_2 = new cSprite( self, @"map_ui_hpLabelBack.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(392, 172) );
	m_status_labelHP_Back_3 = new cSprite( self, @"map_ui_hpLabelBack.png", LAYER_STATUS_UI, CGPointMake(0, 0), CGPointMake(392, 115) );

	for(int i = 0; i < 6; i++)
	{
		cLabelAtlasCostomPtr loopLabel = NULL;

			 if(i == 0)	loopLabel = m_status_labelHP_1[0] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 1)	loopLabel = m_status_labelHP_2[0] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 2)	loopLabel = m_status_labelHP_3[0] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 3)	loopLabel = m_status_labelHP_1[1] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 4)	loopLabel = m_status_labelHP_2[1] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);
		else if(i == 5)	loopLabel = m_status_labelHP_3[1] = new cLabelAtlasCostom(self, @"map_ui_level_num.png", 7, LAYER_STATUS_UI);

		loopLabel->addCharInfo('1', 5);		loopLabel->addCharInfo('2', 6);		loopLabel->addCharInfo('3', 6);
		loopLabel->addCharInfo('4', 6);		loopLabel->addCharInfo('5', 6);		loopLabel->addCharInfo('6', 6);
		loopLabel->addCharInfo('7', 6);		loopLabel->addCharInfo('8', 6);		loopLabel->addCharInfo('9', 6);
		loopLabel->addCharInfo('0', 6);
	}		

	[self updateStatus];

	//label potion
	m_labelPotionCountNum = new cLabelAtlasCostom(self, @"shop_item_num2.png", 12, LAYER_STATUS_UI_2);
	m_labelPotionCountNum->addCharInfo('1',  9);	m_labelPotionCountNum->addCharInfo('2', 13);	m_labelPotionCountNum->addCharInfo('3', 13);
	m_labelPotionCountNum->addCharInfo('4', 13);	m_labelPotionCountNum->addCharInfo('5', 13);	m_labelPotionCountNum->addCharInfo('6', 13);
	m_labelPotionCountNum->addCharInfo('7', 13);	m_labelPotionCountNum->addCharInfo('8', 13);	m_labelPotionCountNum->addCharInfo('9', 13);
	m_labelPotionCountNum->addCharInfo('0', 13);	m_labelPotionCountNum->addCharInfo('X', 14);

	m_labelPotionCountNum->setString( [NSMutableString stringWithFormat:@"X %d", cMyData::getObj()->m_potion] );
	m_labelPotionCountNum->setPosition( CGPointMake(460 - m_labelPotionCountNum->getSize().width, 76) );

	//btns
	CCMenuItemImage* itemExit		= [ CCMenuItemImage itemFromNormalImage:@"map_btn_exit.png"		 selectedImage:@"map_btn_exit_push.png"		 target:self selector:@selector( exitCallback	   : ) ];
	CCMenuItemImage* itemParty		= [ CCMenuItemImage itemFromNormalImage:@"map_btn_party.png"	 selectedImage:@"map_btn_party_push.png"	 target:self selector:@selector( partyCallback	   : ) ];
	CCMenuItemImage* itemHeal		= [ CCMenuItemImage itemFromNormalImage:@"map_ui_heal.png"		 selectedImage:@"map_ui_heal_over.png"		 target:self selector:@selector( healCallback	   : ) ];
	CCMenuItemImage* itemTower		= [ CCMenuItemImage itemFromNormalImage:@"map_btn_tower.png"	 selectedImage:@"map_btn_tower_push.png"	 target:self selector:@selector( towerCallback	   : ) ];
	CCMenuItemImage* itemColoseum	= [ CCMenuItemImage itemFromNormalImage:@"map_btn_coloseum.png"	 selectedImage:@"map_btn_coloseum_push.png"	 target:self selector:@selector( coloseumCallback  : ) ];
	CCMenuItemImage* itemReinforme	= [ CCMenuItemImage itemFromNormalImage:@"map_btn_reinforme.png" selectedImage:@"map_btn_reinforme_push.png" target:self selector:@selector( reinformeCallback : ) ];
	CCMenuItemImage* itemShop		= [ CCMenuItemImage itemFromNormalImage:@"map_btn_shop.png"		 selectedImage:@"map_btn_shop_push.png"		 target:self selector:@selector( shopCallback	   : ) ];
	CCMenuItemImage* itemShopExit	= [ CCMenuItemImage itemFromNormalImage:@"shop_exit.png"		 selectedImage:@"shop_exit_push.png"		 target:self selector:@selector( shopExitCallback  : ) ];

	m_btnExit		= [CCMenu menuWithItems:itemExit	 , nil];
	m_btnParty		= [CCMenu menuWithItems:itemParty	 , nil];
	m_btnHeal		= [CCMenu menuWithItems:itemHeal	 , nil];
	m_btnTower		= [CCMenu menuWithItems:itemTower	 , nil];
	m_btnColoseum	= [CCMenu menuWithItems:itemColoseum , nil];
	m_btnReinforme	= [CCMenu menuWithItems:itemReinforme, nil];
	m_btnShop		= [CCMenu menuWithItems:itemShop	 , nil];
	m_btnShopExit	= [CCMenu menuWithItems:itemShopExit , nil];

	m_btnExit	  .position = CGPointMake(	  44,	  19);
	m_btnParty	  .position = CGPointMake(	 425,	  33);
	m_btnHeal	  .position = CGPointMake(	 425,	  91);
	m_btnTower	  .position = CGPointMake(	 255,	 257);
	m_btnColoseum .position = CGPointMake(278.5f, 106.5f);
	m_btnReinforme.position = CGPointMake(	  58,	 202);
	m_btnShop	  .position = CGPointMake( 85.5f, 125.5f);
	m_btnShopExit .position = CGPointMake(	  52,	  37);

	m_btnExit	  .visible = YES;
	m_btnParty	  .visible = YES;
	m_btnHeal	  .visible = YES;
	m_btnTower	  .visible = YES;
	m_btnColoseum .visible = YES;
	m_btnReinforme.visible = YES;
	m_btnShop	  .visible = YES;
	m_btnShopExit .visible = NO;

	m_btnExit	  .isTouchEnabled = YES;
	m_btnParty	  .isTouchEnabled = YES;
	m_btnHeal	  .isTouchEnabled = YES;
	m_btnTower	  .isTouchEnabled = YES;
	m_btnColoseum .isTouchEnabled = YES;
	m_btnReinforme.isTouchEnabled = YES;
	m_btnShop	  .isTouchEnabled = YES;
	m_btnShopExit .isTouchEnabled = NO;

	[self addChild:m_btnExit	  z:LAYER_MENE];
	[self addChild:m_btnParty	  z:LAYER_MENE];
	[self addChild:m_btnHeal	  z:LAYER_MENE];
	[self addChild:m_btnTower	  z:LAYER_MENE];
	[self addChild:m_btnColoseum  z:LAYER_MENE];
	[self addChild:m_btnReinforme z:LAYER_MENE];
	[self addChild:m_btnShop	  z:LAYER_MENE];
	[self addChild:m_btnShopExit  z:LAYER_MENE];

	//switchs
	m_isGoTower	 = false;
	m_isGoCasino = false;

	//schedule
	[self schedule: @selector(step:)];

	//return
	return self;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)onExit
{
	//node
	if(m_pShopNode != nil)
	{
		[self removeChild:m_pShopNode cleanup:YES]; 
		m_pShopNode = nil;
	}

	//logo
	m_sprLogo = NULL;

	//back
	[self removeChild:m_sprBack cleanup:YES];
	m_sprBack = nil;

	//hero
	m_heroCreateInfos = NULL;
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i] = NULL;
	m_heros.clear();

	//UI money
	m_UI_moneyBack = NULL;
	m_UI_money	   = NULL;

	//status
	m_status_back			= NULL;
	m_status_LevelBack_1	= NULL;
	m_status_LevelBack_2	= NULL;
	m_status_LevelBack_3	= NULL;
	m_status_LevelLabel_1	= NULL;
	m_status_LevelLabel_2	= NULL;
	m_status_LevelLabel_3	= NULL;
	m_status_hpBarBack_1	= NULL;
	m_status_hpBarBack_2	= NULL;
	m_status_hpBarBack_3	= NULL;
	m_status_hpBarGage_1	= NULL;
	m_status_hpBarGage_2	= NULL;
	m_status_hpBarGage_3	= NULL;
	m_status_labelHP_Back_1	= NULL;
	m_status_labelHP_Back_2	= NULL;
	m_status_labelHP_Back_3	= NULL;
	m_status_labelHP_1[0]	= NULL;
	m_status_labelHP_1[1]	= NULL;
	m_status_labelHP_2[0]	= NULL;
	m_status_labelHP_2[1]	= NULL;
	m_status_labelHP_3[0]	= NULL;
	m_status_labelHP_3[1]	= NULL;

	//label potion
	m_labelPotionCountNum = NULL;

	//btns
	[self removeChild:m_btnExit		 cleanup:YES];
	[self removeChild:m_btnParty	 cleanup:YES];
	[self removeChild:m_btnHeal		 cleanup:YES];
	[self removeChild:m_btnTower	 cleanup:YES];
	[self removeChild:m_btnColoseum	 cleanup:YES];
	[self removeChild:m_btnReinforme cleanup:YES];
	[self removeChild:m_btnShop		 cleanup:YES];
	[self removeChild:m_btnShopExit	 cleanup:YES];

	m_btnExit		= nil;
	m_btnParty		= nil;
	m_btnHeal		= nil;
	m_btnTower		= nil;
	m_btnColoseum	= nil;
	m_btnReinforme	= nil;
	m_btnShop		= nil;
	m_btnShopExit	= nil;

	//unschedule
	[self unschedule: @selector(step:)];

	//super exit
	[super onExit];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : input
//----------------------------------------------------------------------------------------------
- (void)ccTouchesBegan:(NSMutableSet *)touches withEvent:(UIEvent *)event
{
#ifdef COCOS2D_DEBUG
	if( [ [touches anyObject] tapCount ] > 4 )
	{
		cMyData::getObj()->m_money += 1000LL;
		cMyData::getObj()->save();
	}
#endif
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)setNodeChangeIn
{
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = false;

	m_isNodeChangeIn = true;
	m_pNodeChangeImg = new cNodeChangeImgs(self, false);
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) step:(ccTime)delta
{
	//tickCount
	m_tickCount = CFAbsoluteTimeGetCurrent();

	if(m_beforeTickCount == 0)	m_beforeTickCount = m_tickCount;
	double leftTickCount = m_tickCount - m_beforeTickCount;
	m_beforeTickCount	 = m_tickCount;

	//node chage
	if( [self updateNodeChage:leftTickCount] == true )	return;

	//logo
	m_sprLogo->update(leftTickCount);

	//money
	[self updateMoney];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (bool)updateNodeChage:(double)leftTIme
{
	if(m_pNodeChangeImg != nil)
	{
		if( m_pNodeChangeImg->update(leftTIme) == true )
		{
			if(m_isNodeChangeIn == true)
			{
				m_btnExit	  .isTouchEnabled = true;
				m_btnParty	  .isTouchEnabled = true;
				m_btnHeal	  .isTouchEnabled = true;
				m_btnTower	  .isTouchEnabled = true;
				m_btnColoseum .isTouchEnabled = true;
				m_btnReinforme.isTouchEnabled = true;
				m_btnShop	  .isTouchEnabled = true;
				m_btnShopExit .isTouchEnabled = true;

				m_isNodeChangeIn = false;
				m_pNodeChangeImg = nil;
			}
			else if(m_isNodeChangeOut == true)
			{
				m_isNodeChangeOut = false;
				m_pNodeChangeImg  = nil;

				//object release
				[self onExit];

				//cenece change
				if(m_isGoCasino == true)
				{
					[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.0f scene:[casinoBetNode scene] ] ];
				}

				return true;
			}
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)updateMoney
{
	NSMutableString* strMoney = [NSMutableString stringWithFormat:@"%lld", cMyData::getObj()->m_money];
	for(int i = strMoney.length-3; i > 0; i -= 3)	[strMoney insertString:@"," atIndex:(NSUInteger)i];

	m_UI_money->setString(strMoney);
	m_UI_money->setPosition( CGPointMake(459 - m_UI_money->getSize().width, 300) );
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)updateStatus
{
	m_status_LevelBack_1   ->get().visible = false;
	m_status_LevelBack_2   ->get().visible = false;
	m_status_LevelBack_3   ->get().visible = false;
	m_status_hpBarBack_1   ->get().visible = false;
	m_status_hpBarBack_2   ->get().visible = false;
	m_status_hpBarBack_3   ->get().visible = false;
	m_status_hpBarGage_1   ->get().visible = false;
	m_status_hpBarGage_2   ->get().visible = false;
	m_status_hpBarGage_3   ->get().visible = false;
	m_status_labelHP_Back_1->get().visible = false;
	m_status_labelHP_Back_2->get().visible = false;
	m_status_labelHP_Back_3->get().visible = false;
	m_status_LevelLabel_1->setString(@"");
	m_status_LevelLabel_2->setString(@"");
	m_status_LevelLabel_3->setString(@"");
	m_status_labelHP_1[0]->setString(@"");
	m_status_labelHP_2[0]->setString(@"");
	m_status_labelHP_3[0]->setString(@"");
	m_status_labelHP_1[1]->setString(@"");
	m_status_labelHP_2[1]->setString(@"");
	m_status_labelHP_3[1]->setString(@"");

	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		int seatIndex = m_heros[i]->getSeatIndex();
		if(seatIndex == -1)	continue;

		int hp		= m_heros[i]->getHP();
		int use_hp	= m_heros[i]->get_use_hp();
		int gage	= (use_hp*84) / hp;
		if(gage > 84)	gage = 84;

		if(seatIndex == 0)
		{
			m_status_LevelBack_1   ->get().visible = true;
			m_status_hpBarBack_1   ->get().visible = true;
			m_status_hpBarGage_1   ->get().visible = true;
			m_status_labelHP_Back_1->get().visible = true;

			[ m_status_hpBarGage_1->get() setTextureRect:CGRectMake(0, 0, gage, 11) ];

			m_status_LevelLabel_1->setVisible(true);
			m_status_LevelLabel_1->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getLevel() ] );
			m_status_LevelLabel_1->setPosition( CGPointMake(456 - m_status_LevelLabel_1->getSize().width, 244) );

			m_status_labelHP_1[0]->setVisible(true);
			m_status_labelHP_1[1]->setVisible(true);
			m_status_labelHP_1[0]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->get_use_hp() ] );
			m_status_labelHP_1[0]->setPosition( CGPointMake(420 - (int)(m_status_labelHP_1[0]->getSize().width/2), 230) );
			m_status_labelHP_1[1]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getHP() ] );
			m_status_labelHP_1[1]->setPosition( CGPointMake(449 - (int)(m_status_labelHP_1[0]->getSize().width/2), 230) );
		}
		else if(seatIndex == 1)
		{
			m_status_LevelBack_2   ->get().visible = true;
			m_status_hpBarBack_2   ->get().visible = true;
			m_status_hpBarGage_2   ->get().visible = true;
			m_status_labelHP_Back_2->get().visible = true;

			[ m_status_hpBarGage_2->get() setTextureRect:CGRectMake(0, 0, gage, 11) ];

			m_status_LevelLabel_2->setVisible(true);
			m_status_LevelLabel_2->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getLevel() ] );
			m_status_LevelLabel_2->setPosition( CGPointMake(456 - m_status_LevelLabel_2->getSize().width, 187) );

			m_status_labelHP_2[0]->setVisible(true);
			m_status_labelHP_2[1]->setVisible(true);
			m_status_labelHP_2[0]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->get_use_hp() ] );
			m_status_labelHP_2[0]->setPosition( CGPointMake(420 - (int)(m_status_labelHP_1[0]->getSize().width/2), 173) );
			m_status_labelHP_2[1]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getHP() ] );
			m_status_labelHP_2[1]->setPosition( CGPointMake(449 - (int)(m_status_labelHP_1[0]->getSize().width/2), 173) );
		}
		else if(seatIndex == 2)
		{
			m_status_LevelBack_3   ->get().visible = true;
			m_status_hpBarBack_3   ->get().visible = true;
			m_status_hpBarGage_3   ->get().visible = true;
			m_status_labelHP_Back_3->get().visible = true;

			[ m_status_hpBarGage_3->get() setTextureRect:CGRectMake(0, 0, gage, 11) ];

			m_status_LevelLabel_3->setVisible(true);
			m_status_LevelLabel_3->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getLevel() ] );
			m_status_LevelLabel_3->setPosition( CGPointMake(456 - m_status_LevelLabel_3->getSize().width, 130) );

			m_status_labelHP_3[0]->setVisible(true);
			m_status_labelHP_3[1]->setVisible(true);
			m_status_labelHP_3[0]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->get_use_hp() ] );
			m_status_labelHP_3[0]->setPosition( CGPointMake(420 - (int)(m_status_labelHP_1[0]->getSize().width/2), 116) );
			m_status_labelHP_3[1]->setString( [ NSMutableString stringWithFormat:@"%d", m_heros[i]->getHP() ] );
			m_status_labelHP_3[1]->setPosition( CGPointMake(449 - (int)(m_status_labelHP_1[0]->getSize().width/2), 116) );
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) exitCallback:(id)sender
{
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = false;

	//schedule
	[self unschedule: @selector(step:)];

	[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.5f scene:[mainNode scene:false] ] ];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) partyCallback:(id)sender
{
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = false;

	//schedule
	[self unschedule: @selector(step:)];

	[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.5f scene:[formationNode scene] ] ];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) healCallback:(id)sender
{
	if(cMyData::getObj()->m_potion <= 0)	return;

	bool isUse = false;
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		if( m_heros[i]->getSeatIndex() != -1 && m_heros[i]->get_use_hp() < m_heros[i]->getHP() )
		{
			isUse = true;

			break;
		}
	}

	if(isUse == true)
	{
		--cMyData::getObj()->m_potion;

		for(int i = 0; i < (int)m_heros.size(); i++)
		{
			if(m_heros[i]->getSeatIndex() != -1)
			{
				m_heros[i]->set_use_hp( m_heros[i]->get_use_hp() + _POSION_HP_UP );
			}
		}

		cMyData::getObj()->updateHeroSaveDatas(m_heros);

		m_labelPotionCountNum->setString( [NSMutableString stringWithFormat:@"X %d", cMyData::getObj()->m_potion] );
		m_labelPotionCountNum->setPosition( CGPointMake(460 - m_labelPotionCountNum->getSize().width, 76) );

		[self updateStatus];
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) towerCallback:(id)sender
{
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = false;

	//schedule
	[self unschedule: @selector(step:)];

	[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.5f scene:[towerEntranceNode scene] ] ];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) coloseumCallback:(id)sender
{
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = false;
	
	m_isNodeChangeOut = true;
	m_pNodeChangeImg  = new cNodeChangeImgs(self, true);

	m_isGoCasino = true;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) reinformeCallback:(id)sender
{
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) shopCallback:(id)sender
{
	if(m_pShopNode != nil)	return;

	isTouchEnabled = NO;

	m_btnExit	  .visible = false;
	m_btnParty	  .visible = false;
	m_btnHeal	  .visible = false;
	m_btnTower	  .visible = true;
	m_btnColoseum .visible = true;
	m_btnReinforme.visible = true;
	m_btnShop	  .visible = true;
	m_btnShopExit .visible = true;
	m_btnExit	  .isTouchEnabled = false;
	m_btnParty	  .isTouchEnabled = false;
	m_btnHeal	  .isTouchEnabled = false;
	m_btnTower	  .isTouchEnabled = false;
	m_btnColoseum .isTouchEnabled = false;
	m_btnReinforme.isTouchEnabled = false;
	m_btnShop	  .isTouchEnabled = false;
	m_btnShopExit .isTouchEnabled = true;

	//status
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i]->setVisiblePortrait(false);

	m_status_back			->get().visible = false;
	m_status_LevelBack_1	->get().visible = false;
	m_status_LevelBack_2	->get().visible = false;
	m_status_LevelBack_3	->get().visible = false;
	m_status_LevelLabel_1	->setVisible(false);
	m_status_LevelLabel_2	->setVisible(false);
	m_status_LevelLabel_3	->setVisible(false);
	m_status_hpBarBack_1	->get().visible = false;
	m_status_hpBarBack_2	->get().visible = false;
	m_status_hpBarBack_3	->get().visible = false;
	m_status_hpBarGage_1	->get().visible = false;
	m_status_hpBarGage_2	->get().visible = false;
	m_status_hpBarGage_3	->get().visible = false;
	m_status_labelHP_Back_1	->get().visible = false;
	m_status_labelHP_Back_2	->get().visible = false;
	m_status_labelHP_Back_3	->get().visible = false;
	m_status_labelHP_1[0]	->setVisible(false);
	m_status_labelHP_2[0]	->setVisible(false);
	m_status_labelHP_3[0]	->setVisible(false);
	m_status_labelHP_1[1]	->setVisible(false);
	m_status_labelHP_2[1]	->setVisible(false);
	m_status_labelHP_3[1]	->setVisible(false);

	//label potion
	m_labelPotionCountNum->setVisible(false);

	id fadeTo = [CCFadeTo actionWithDuration:0.25f opacity:255];
	id seq	  = [CCSequence actions:fadeTo, nil];

	m_btnShopExit.opacity = 0;
	[m_btnShopExit runAction:seq];

	m_pShopNode = [ [shopNode alloc] init];
	[self addChild:m_pShopNode z:LAYER_SHOP_NODE];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) shopExitCallback:(id)sender
{
/*
	if(m_pShopNode != nil)
	{
	}

	isTouchEnabled = YES;

	m_btnExit	  .visible = true;
	m_btnParty	  .visible = true;
	m_btnHeal	  .visible = true;
	m_btnTower	  .visible = true;
	m_btnColoseum .visible = true;
	m_btnReinforme.visible = true;
	m_btnShop	  .visible = true;
	m_btnShopExit .visible = true;
	m_btnExit	  .isTouchEnabled = true;
	m_btnParty	  .isTouchEnabled = true;
	m_btnHeal	  .isTouchEnabled = true;
	m_btnTower	  .isTouchEnabled = true;
	m_btnColoseum .isTouchEnabled = true;
	m_btnReinforme.isTouchEnabled = true;
	m_btnShop	  .isTouchEnabled = true;
	m_btnShopExit .isTouchEnabled = false;

	//heros
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i] = NULL;
	m_heros.clear();

	for(int i = 0; i < (int)cMyData::getObj()->m_heroSaveDatas.size(); i++)
	{
		const cHeroSaveData& saveData = cMyData::getObj()->m_heroSaveDatas[i];

		cUnitPtr hero = new cUnit(self, m_heroCreateInfos->getUnitInfo(saveData.createIndex), LAYER_MAP_HERO, saveData.heroID, saveData.createIndex, saveData.seatIndex);
		hero->texLoad(eUnitAniType_Portrait);
		hero->load(saveData.seatIndex, (ePositionType)saveData.positionType, saveData.level, saveData.hp, saveData.use_hp, saveData.atk, saveData.def, saveData.agl, saveData.crt, saveData.skills);
		hero->setVisiblePortrait(false);

		switch( hero->getSeatIndex() )
		{
		case 0	:	hero->setPtPortrait(CGPointMake(393, 282) );	hero->setVisiblePortrait(true);		break;
		case 1	:	hero->setPtPortrait(CGPointMake(393, 225) );	hero->setVisiblePortrait(true);		break;
		case 2	:	hero->setPtPortrait(CGPointMake(393, 168) );	hero->setVisiblePortrait(true);		break;
		}

		m_heros.push_back(hero);
	}

	//status
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i]->setVisiblePortrait(true);
	m_status_back->get().visible = true;

	//label potion
	m_labelPotionCountNum->setVisible(true);

	m_labelPotionCountNum->setString( [NSMutableString stringWithFormat:@"X %d", cMyData::getObj()->m_potion] );
	m_labelPotionCountNum->setPosition( CGPointMake(460 - m_labelPotionCountNum->getSize().width, 76) );

	[self updateStatus];

	id fadeTo = [CCFadeTo actionWithDuration:0.25f opacity:0];
	id seq	  = [CCSequence actions:fadeTo, nil];

	[m_btnShopExit runAction:seq];
*/
}


@end
