//
//  shopNode.m
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//


#import "formationNode.h"
#import "mapNode.h"
#import "CocosDenshion.h"
#import "chipmunk.h"
#import "SimpleAudioEngine.h"

#include "LayerDefine.h"
#include "myData.h"
#include "labels.h"
#include "sprAni.h"
#include "sprite.h"
#include "unit.h"
#include "unitCreateInfos.h"



@implementation formationNode



//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
cLabelParas::cLabelParas(cLabelsPtr pLabel, CGPoint _pt, int z)
: labelName(nil)
, labelHp  (nil)
, labelAtk (nil)
, labelDef (nil)
, labelAgl (nil)
, labelCrt (nil)
{
	CGRect rcName = CGRectMake(_pt.x +  44.5f, _pt.y + 113.5f, 88, 15);
	CGRect rcHp	  = CGRectMake(_pt.x +  69.5f, _pt.y +  59.5f, 88, 15);
	CGRect rcAtk  = CGRectMake(_pt.x +  69.5f, _pt.y +  46.5f, 88, 15);
	CGRect rcDef  = CGRectMake(_pt.x + 113.5f, _pt.y +  46.5f, 88, 15);
	CGRect rcAgl  = CGRectMake(_pt.x +  69.5f, _pt.y +  33.5f, 88, 15);
	CGRect rcCrt  = CGRectMake(_pt.x + 113.5f, _pt.y +  33.5f, 88, 15);

	labelName = pLabel->add( @"gaeul9", 12, UITextAlignmentCenter, rcName, @"", LAYER_FORMATION_UI_2);
	labelHp	  = pLabel->add( @"gaeul9", 12, UITextAlignmentLeft	 , rcHp	 , @"", LAYER_FORMATION_UI_2);
	labelAtk  = pLabel->add( @"gaeul9", 12, UITextAlignmentLeft	 , rcAtk , @"", LAYER_FORMATION_UI_2);
	labelDef  = pLabel->add( @"gaeul9", 12, UITextAlignmentLeft	 , rcDef , @"", LAYER_FORMATION_UI_2);
	labelAgl  = pLabel->add( @"gaeul9", 12, UITextAlignmentLeft	 , rcAgl , @"", LAYER_FORMATION_UI_2);
	labelCrt  = pLabel->add( @"gaeul9", 12, UITextAlignmentLeft	 , rcCrt , @"", LAYER_FORMATION_UI_2);
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
cLabelParas::~cLabelParas()
{
	labelName = nil;
	labelHp	  = nil;
	labelAtk  = nil;
	labelDef  = nil;
	labelAgl  = nil;
	labelCrt  = nil;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cLabelParas::setString(cLabelsPtr pLabel, NSString* strName, NSString* strHp, NSString* strAtk, NSString* strDef, NSString* strAgl, NSString* strCrt)
{
	pLabel->setStr(labelName, strName);
	pLabel->setStr(labelHp	, strHp	 );
	pLabel->setStr(labelAtk	, strAtk );
	pLabel->setStr(labelDef	, strDef );
	pLabel->setStr(labelAgl	, strAgl );
	pLabel->setStr(labelCrt	, strCrt );
}




//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
+ (id)scene
{
	CCScene *s = [CCScene node];
	id node = [formationNode node];
	[s addChild:node];
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

	//touch
	m_tidCount = 1;

	//time
	m_tickCount		  = 0;
	m_beforeTickCount = 0;

	//back
	m_sprBack = [CCSprite spriteWithFile:@"formation_bg.png"];
	m_sprBack.position	  = cpvzero;
	m_sprBack.anchorPoint = cpvzero;
	[self addChild:m_sprBack z:LAYER_BACK];

	//labels
	m_labels = new cLabels(self);

	//hero create info
	m_heroCreateInfos = new cUnitCreateInfos("heroCreateInfos.par");
	
	//heros
	for(int i = 0; i < (int)cMyData::getObj()->m_heroSaveDatas.size(); i++)
	{
		const cHeroSaveData& saveData = cMyData::getObj()->m_heroSaveDatas[i];

		cUnitPtr hero = new cUnit(self, m_heroCreateInfos->getUnitInfo(saveData.createIndex), eCreateInfoFileType_Hero, LAYER_FORMATION_HERO, saveData.heroID, saveData.createIndex, saveData.seatIndex);
		hero->texLoad(eUnitAniType_Portrait);
		hero->texLoad(eUnitAniType_Normal  );
		hero->load(saveData.seatIndex, (ePositionType)saveData.positionType, saveData.level, saveData.hp, saveData.use_hp, saveData.atk, saveData.def, saveData.agl, saveData.crt, saveData.skills);
		hero->changeAni(eUnitAniType_Normal);
		hero->setVisiblePortrait(false);
		hero->setVisible(false);

		m_heros.push_back(hero);
	}

	//label papa
	CGPoint pt = CGPointMake(50, 0);
	for(int i = 0; i < 4; i++)
	{
		if(i == 1)	pt.x += 0.5f;
		if(i == 3)	pt.x -= 0.5f;

		cLabelParasPtr pLabelParas = new cLabelParas(m_labels, pt, LAYER_FORMATION_UI_2);
		m_labelParas.push_back(pLabelParas);

		pt.x += 95;
	}

	//btns
	//btns
	CCMenuItemImage* itemExit	= [ CCMenuItemImage itemFromNormalImage:@"formation_btn_exit.png"	selectedImage:@"formation_btn_exit_push.png"	target:self selector:@selector( exitCallback  : ) ];
	CCMenuItemImage* itemLeft	= [ CCMenuItemImage itemFromNormalImage:@"formation_btn_left.png"	selectedImage:@"formation_btn_left_push.png"	target:self selector:@selector( leftCallback  : ) ];
	CCMenuItemImage* itemRight	= [ CCMenuItemImage itemFromNormalImage:@"formation_btn_right.png"	selectedImage:@"formation_btn_right_push.png"	target:self selector:@selector( rightCallback : ) ];

	m_btnExit  = [CCMenu menuWithItems:itemExit  , nil];
	m_btnLeft  = [CCMenu menuWithItems:itemLeft	 , nil];
	m_btnRight = [CCMenu menuWithItems:itemRight , nil];

	m_btnExit .position = cpv(431, 270);
	m_btnLeft .position = cpv( 20,  60);
	m_btnRight.position = cpv(459,  60);

	m_btnExit .visible = YES;
	m_btnLeft .visible = YES;
	m_btnRight.visible = YES;

	m_btnExit .isTouchEnabled = YES;
	m_btnLeft .isTouchEnabled = YES;
	m_btnRight.isTouchEnabled = YES;

	[self addChild:m_btnExit  z:LAYER_MENE];
	[self addChild:m_btnLeft  z:LAYER_MENE];
	[self addChild:m_btnRight z:LAYER_MENE];

	//char back img
	for(int i = 0; i < 4; i++)
	{
		cSpritePtr pSprite = NULL;
		if( i < (int)m_heros.size() )	pSprite = new cSprite( self, @"formation_cha_bg_1.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(50 + i*95, 0) );
		else							pSprite = new cSprite( self, @"formation_cha_bg_2.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(50 + i*95, 0) );

		cSpritePtr pSpriteError = new cSprite( self, @"formation_cha_error.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(50 + i*95, 0) );

		pSprite		->get().visible = true;
		pSpriteError->get().visible = false;

		m_sprCharBGs		.push_back(pSprite		);
		m_sprCharErrors		.push_back(pSpriteError	);
		m_charErrorLeftTimes.push_back(0			);
	}

	//skill sprite
	for(int i = 0; i < 4; i++)
	{
		cSpritePtr sprBG_1 = new cSprite( self, @"formation_skill_bg.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(51 + i*95, 0) );
		cSpritePtr sprBG_2 = new cSprite( self, @"formation_skill_bg.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(95 + i*95, 0) );

		cSprAniPtr aniIcon_1 = new cSprAni( self, @"skill_icons.png", LAYER_FORMATION_UI_2, CGPointMake( 63 + i*95, 25) );
		cSprAniPtr aniIcon_2 = new cSprAni( self, @"skill_icons.png", LAYER_FORMATION_UI_2, CGPointMake(108 + i*95, 25) );

		for(int a = 0; a < 11; a++)
		{
			aniIcon_1->addFrame( CGRectMake(a * 20, 0, 20, 20), 0, false, false, CGPointZero, CGRectZero, CGRectZero );
			aniIcon_2->addFrame( CGRectMake(a * 20, 0, 20, 20), 0, false, false, CGPointZero, CGRectZero, CGRectZero );
		}

		aniIcon_1->setLoop(false);
		aniIcon_2->setLoop(false);
		aniIcon_1->stop();
		aniIcon_2->stop();

		m_sprSkillBGs.push_back(sprBG_1);
		m_sprSkillBGs.push_back(sprBG_2);

		m_aniSkillIcons.push_back(aniIcon_1);
		m_aniSkillIcons.push_back(aniIcon_2);
	}

	//label position types
	for(int i = 0; i < 3; i++)
	{
		cSpritePtr pSprFront = new cSprite( self, @"formation_front.png", LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(96 + i*120, 130) );
		cSpritePtr pSprBack	 = new cSprite( self, @"formation_back.png"	, LAYER_FORMATION_UI_1, CGPointZero, CGPointMake(96 + i*120, 130) );

		pSprFront->get().visible = false;
		pSprBack ->get().visible = false;

		m_spPositionFronts.push_back(pSprFront);
		m_spPositionBacks .push_back(pSprBack );
	}

	//scroll
	[self setScrollPos:0];

	//schedule
	[self schedule: @selector(step:)];

	return self;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void)onExit
{
	//unschedule
	[self unschedule: @selector(step:)];

	//touch
	m_listTouch.clear();

	//back img
	if(m_sprBack != nil)
	{
		[self removeChild:m_sprBack cleanup:YES];
		m_sprBack = nil;
	}

	//labels
	m_labels = NULL;

	//hero
	m_heroCreateInfos = NULL;
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i] = NULL;
	m_heros.clear();

	//label papa
	for(int i = 0; i < (int)m_labelParas.size(); i++)	m_labelParas[i] = NULL;
	m_labelParas.clear();

	//btns
	[self removeChild:m_btnExit	 cleanup:YES];
	[self removeChild:m_btnLeft	 cleanup:YES];
	[self removeChild:m_btnRight cleanup:YES];

	m_btnExit  = nil;
	m_btnLeft  = nil;
	m_btnRight = nil;

	//char back img
	for(int i = 0; i < (int)m_sprCharBGs   .size(); i++)	m_sprCharBGs   [i] = NULL;
	for(int i = 0; i < (int)m_sprCharErrors.size(); i++)	m_sprCharErrors[i] = NULL;
	m_sprCharBGs   .clear();
	m_sprCharErrors.clear();

	//skill sprite
	for(int i = 0; i < (int)m_sprSkillBGs  .size(); i++)	m_sprSkillBGs  [i] = NULL;
	for(int i = 0; i < (int)m_aniSkillIcons.size(); i++)	m_aniSkillIcons[i] = NULL;
	m_sprSkillBGs  .clear();
	m_aniSkillIcons.clear();

	//label position types
	for(int i = 0; i < (int)m_spPositionFronts.size(); i++)	m_spPositionFronts[i] = NULL;
	for(int i = 0; i < (int)m_spPositionBacks .size(); i++)	m_spPositionBacks [i] = NULL;
	m_spPositionFronts.clear();
	m_spPositionBacks .clear();
	
	//super exit
	[super onExit];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : input
//----------------------------------------------------------------------------------------------
- (void)ccTouchesBegan:(NSMutableSet *)touches withEvent:(UIEvent *)event
{
	for(UITouch *touch in touches)
	{
		CGPoint ptTouch = [touch locationInView:touch.view];

		//현재 화면에 맞게 컨버트.
		float x	  = ptTouch.x;
		ptTouch.x = _SCR_W - ptTouch.y;
		ptTouch.y = _SCR_H - x;

		//list add
		sTouch touchInfo;
		touchInfo.tid = ++m_tidCount;
		touchInfo.pt  = ptTouch;

		m_listTouch.push_back(touchInfo);

		//touch enable 처리.
		[self touchBegan:ptTouch];
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : input
//----------------------------------------------------------------------------------------------
- (void)ccTouchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(UITouch *touch in touches)
	{
		CGPoint ptTouch			= [touch locationInView:touch.view];
		int		tidCompare		= -1;
		float   compareDistance	= 0;

		//현재 화면에 맞게 컨버트.
		float x	  = ptTouch.x;
		ptTouch.x = _SCR_W - ptTouch.y;
		ptTouch.y = _SCR_H - x;

		//list를 검색하여 가장 좌표가 가까운 node를 찾는다.
		for(std::list<sTouch>::iterator node = m_listTouch.begin(); node != m_listTouch.end(); ++node)
		{
			sTouch loopTouch = *node;
			float  distance  = cpvdist(loopTouch.pt, ptTouch);

			if(tidCompare == -1 || distance < compareDistance)
			{
				tidCompare		= loopTouch.tid;
				compareDistance = distance;
			}
		}

		//가장 좌표가 가까운 곳의 값을 바꾼다.
		for(std::list<sTouch>::iterator node = m_listTouch.begin(); node != m_listTouch.end(); ++node)
		{
			if( (* node).tid == tidCompare )	(* node).pt = ptTouch;
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : input
//----------------------------------------------------------------------------------------------
- (void)ccTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(UITouch *touch in touches)
	{
		CGPoint ptTouch			= [touch locationInView:touch.view];
		int		tidCompare		= -1;
		float   compareDistance	= 0;

		//현재 화면에 맞게 컨버트.
		float x	  = ptTouch.x;
		ptTouch.x = _SCR_W - ptTouch.y;
		ptTouch.y = _SCR_H - x;

		//list를 검색하여 가장 좌표가 가까운 node를 찾는다.
		for(std::list<sTouch>::iterator node = m_listTouch.begin(); node != m_listTouch.end(); ++node)
		{
			sTouch loopTouch = *node;
			float  distance  = cpvdist(loopTouch.pt, ptTouch);

			if(tidCompare == -1 || distance < compareDistance)
			{
				tidCompare		= loopTouch.tid;
				compareDistance = distance;
			}
		}

		//가장 좌표가 가까운 node를 삭제한다.
		for(std::list<sTouch>::iterator node = m_listTouch.begin(); node != m_listTouch.end(); ++node)
		{
			if( (* node).tid == tidCompare )
			{
				m_listTouch.erase(node);

				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)touchBegan:(CGPoint)pt
{
	//position type
	for(int i = 0; i < 3; i++)
	{
		if( RectInPt( CGRectMake(75 + i*120, 125, 90, 23), pt ) == true )
		{
			for(std::vector<cUnitPtr>::iterator node = m_heros.begin(); node != m_heros.end(); ++node)
			{
				if( (* node)->getSeatIndex() == i )
				{
					if( (* node)->getPositionType() == ePositionType_Front )	(* node)->setPositionType(ePositionType_Back );
					else														(* node)->setPositionType(ePositionType_Front);

					cMyData::getObj()->updateHeroSaveDatas(m_heros);

					break;
				}
			}

			break;
		}
	}

	//insert or erase hero
	for(int i = 0; i < 4; i++)
	{
		if( RectInPt( CGRectMake(50 + i*95, 0, 92, 125), pt ) == true )
		{
			//is seat
			bool isSeat[3]	= {false, };
			int  isSetCount = 0;

			for(std::vector<cUnitPtr>::iterator node = m_heros.begin(); node != m_heros.end(); ++node)
			{
				if( (* node)->getSeatIndex() < 0 || (* node)->getSeatIndex() >= 3 )		continue;

				isSeat[ (* node)->getSeatIndex() ] = true;
				++isSetCount;
			}

			//seat set
			if(m_heros[m_scrollPos+i]->getSeatIndex() != -1)
			{
				if(isSetCount > 1)
				{
					m_heros[m_scrollPos+i]->setSeatIndex(-1);

					cMyData::getObj()->updateHeroSaveDatas(m_heros);
				}
				else if( i >= 0 && i < (int)m_charErrorLeftTimes.size() )
				{
					m_charErrorLeftTimes[i] = 0.5f;
				}
			}
			else
			{
				if( m_scrollPos+i < (int)m_heros.size() )
				{
					bool isSet = false;
					for(int a = 0; a < 3; a++)
					{
						if(isSeat[a] == false)
						{
							m_heros[m_scrollPos+i]->setSeatIndex(a);
							isSet = true;

							break;
						}
					}
					
					//세팅할 자리가 없다면 마지막 유저와 교체한다.
					if(isSet == false)
					{
						for(std::vector<cUnitPtr>::iterator node = m_heros.begin(); node != m_heros.end(); ++node)
						{
							if( (* node)->getSeatIndex() == 2 )
							{
								(* node)->setSeatIndex(-1);

								break;
							}
						}

						m_heros[m_scrollPos+i]->setSeatIndex(2);
					}

					cMyData::getObj()->updateHeroSaveDatas(m_heros);
				}
			}

			break;
		}
	}
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

	if(leftTickCount > 0.1f)	leftTickCount = 0.1f;

	//heros
	[self updateHero:leftTickCount];

	//char back img error
	for(int i = 0; i < (int)m_sprCharErrors.size(); i++)
	{
		m_charErrorLeftTimes[i] -= leftTickCount;
		if(m_charErrorLeftTimes[i] > 0)
		{
			m_sprCharErrors[i]->get().visible = true;
		}
		else
		{
			m_sprCharErrors[i]->get().visible = false;
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)updateHero:(double)leftTime
{
	//label position types
	for(int i = 0; i < (int)m_spPositionFronts.size(); i++)	m_spPositionFronts[i]->get().visible = false;
	for(int i = 0; i < (int)m_spPositionBacks .size(); i++)	m_spPositionBacks [i]->get().visible = false;

	//loop
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		int seat = m_heros[i]->getSeatIndex();
		//is seat
		if(seat == -1)
		{
			//portrait
			m_heros[i]->setPortraitColor(255, 255, 255);

			//normal
			m_heros[i]->setVisible(false);
		}
		else
		{
			//portrait
			m_heros[i]->setPortraitColor(100, 100, 100);

			//normal
			m_heros[i]->setVisible(true);

			switch( m_heros[i]->getSeatIndex() )
			{
			case 0	:	if(m_heros[i]->getPositionType() == ePositionType_Front)	m_heros[i]->setPt( CGPointMake(127, 159) );
						else														m_heros[i]->setPt( CGPointMake(144, 186) );
						break;
			case 1	:	if(m_heros[i]->getPositionType() == ePositionType_Front)	m_heros[i]->setPt( CGPointMake(240, 159) );
						else														m_heros[i]->setPt( CGPointMake(240, 186) );
						break;
			case 2	:	if(m_heros[i]->getPositionType() == ePositionType_Front)	m_heros[i]->setPt( CGPointMake(353, 159) );
						else														m_heros[i]->setPt( CGPointMake(342, 186) );
						break;
			}

			//label position types
			if(seat >= 0 && seat < 3)
			{
				if(m_heros[i]->getPositionType() == ePositionType_Front)	m_spPositionFronts[seat]->get().visible = true;
				else														m_spPositionBacks [seat]->get().visible = true;
			}
		}

		//update
		m_heros[i]->update(leftTime);
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)setScrollPos:(int)scrollPos
{
	if(scrollPos != 0)
	{
		if(scrollPos < 0 || scrollPos >= (int)m_heros.size() - 3)	return;
	}

	//scroll pos
	m_scrollPos = scrollPos;

	//portraits visible false
	for(int i = m_scrollPos; i < (int)m_heros.size(); i++)
	{
		m_heros[i]->setVisiblePortrait(false);
	}

	//skill sprite
	for(int i = 0; i < (int)m_sprSkillBGs  .size(); i++)	m_sprSkillBGs  [i]->get().visible = false;
	for(int i = 0; i < (int)m_aniSkillIcons.size(); i++)	m_aniSkillIcons[i]->setVisible(false);

	//loop
	int x = 50;
	for(int i = 0, loopScrollPos = m_scrollPos; i < 4 && loopScrollPos < (int)m_heros.size(); i++, loopScrollPos++)
	{
		//portrait
		m_heros[loopScrollPos]->setVisiblePortrait(true);
		m_heros[loopScrollPos]->setPtPortrait( CGPointMake(x + 13, 108) );

		//paras
		NSString* strName = [ NSString stringWithUTF8String:m_heros[loopScrollPos]->getName().c_str() ];

		NSMutableString* strHp = [NSMutableString stringWithFormat:@""];
			 if(m_heros[loopScrollPos]->getHP() >= 1000)	[ strHp appendString:[NSString stringWithFormat:@"%d / "   ,m_heros[loopScrollPos]->getHP()] ];
		else if(m_heros[loopScrollPos]->getHP() >=  100)	[ strHp appendString:[NSString stringWithFormat:@"%d  / "  ,m_heros[loopScrollPos]->getHP()] ];
		else if(m_heros[loopScrollPos]->getHP() >=   10)	[ strHp appendString:[NSString stringWithFormat:@" %d  / " ,m_heros[loopScrollPos]->getHP()] ];
		else												[ strHp appendString:[NSString stringWithFormat:@"  %d  / ",m_heros[loopScrollPos]->getHP()] ];
			 if(m_heros[loopScrollPos]->get_use_hp() >= 1000)	[ strHp appendString:[NSString stringWithFormat:@"%d"	, m_heros[loopScrollPos]->get_use_hp()] ];
		else if(m_heros[loopScrollPos]->get_use_hp() >=  100)	[ strHp appendString:[NSString stringWithFormat:@" %d"	, m_heros[loopScrollPos]->get_use_hp()] ];
		else if(m_heros[loopScrollPos]->get_use_hp() >=   10)	[ strHp appendString:[NSString stringWithFormat:@"  %d"	, m_heros[loopScrollPos]->get_use_hp()] ];
		else													[ strHp appendString:[NSString stringWithFormat:@"   %d", m_heros[loopScrollPos]->get_use_hp()] ];

		NSString* strAtk = NULL;
		NSString* strDef = NULL;
		NSString* strAgl = NULL;
		NSString* strCrt = NULL;
		if(m_heros[loopScrollPos]->getATK() < 10)	strAtk = [ NSString stringWithFormat:@" %d", m_heros[loopScrollPos]->getATK() ];
		else										strAtk = [ NSString stringWithFormat:@"%d" , m_heros[loopScrollPos]->getATK() ];
		if(m_heros[loopScrollPos]->getDEF() < 10)	strDef = [ NSString stringWithFormat:@" %d", m_heros[loopScrollPos]->getDEF() ];
		else										strDef = [ NSString stringWithFormat:@"%d" , m_heros[loopScrollPos]->getDEF() ];
		if(m_heros[loopScrollPos]->getAGL() < 10)	strAgl = [ NSString stringWithFormat:@" %d", m_heros[loopScrollPos]->getAGL() ];
		else										strAgl = [ NSString stringWithFormat:@"%d" , m_heros[loopScrollPos]->getAGL() ];
		if(m_heros[loopScrollPos]->getCRT() < 10)	strCrt = [ NSString stringWithFormat:@" %d", m_heros[loopScrollPos]->getCRT() ];
		else										strCrt = [ NSString stringWithFormat:@"%d" , m_heros[loopScrollPos]->getCRT() ];
		m_labelParas[i]->setString(m_labels, strName, strHp, strAtk, strDef, strAgl, strCrt);

		//skill sprite
		m_sprSkillBGs  [i*2  ]->get().visible = true;
		m_sprSkillBGs  [i*2+1]->get().visible = true;
		m_aniSkillIcons[i*2  ]->setVisible(true);
		m_aniSkillIcons[i*2+1]->setVisible(true);

		m_aniSkillIcons[i*2  ]->setFrame_init( m_heros[loopScrollPos]->getSkillType(0) );
		m_aniSkillIcons[i*2+1]->setFrame_init( m_heros[loopScrollPos]->getSkillType(1) );

		//point
		x += 95;
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) exitCallback:(id)sender
{
	m_btnExit .isTouchEnabled = false;
	m_btnLeft .isTouchEnabled = false;
	m_btnRight.isTouchEnabled = false;

	//schedule
	[self unschedule: @selector(step:)];

	[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.5f scene:[mapNode scene:false] ] ];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) leftCallback:(id)sender
{
	[self setScrollPos:m_scrollPos-1];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) rightCallback:(id)sender
{
	[self setScrollPos:m_scrollPos+1];
}



@end
