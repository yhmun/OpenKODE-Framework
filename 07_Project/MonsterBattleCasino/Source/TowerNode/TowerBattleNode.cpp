//
//  towerBattleNode.m
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "towerBattleNode.h"
#import "mapNode.h"
#import "chipmunk.h"
#import "SimpleAudioEngine.h"
#include "LayerDefine.h"
#include "nodeChangeImgs.h"
#include "unit_commander_tower.h"
#include "sprite.h"
#include "sprAni.h"
#include "myData.h"
#include "unitCreateInfos.h"
#include "sprEffects.h"
#include "labelAtlasCostom.h"
#include "unit.h"
#include "ticpp.h"


//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : carSort
//-----------------------------------------------------------
bool unitSeatSort(const cUnitPtr lhs, const cUnitPtr rhs)
{
	return lhs->getSeatIndex() < rhs->getSeatIndex();
}



//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
cSkillBtn::cSkillBtn(CCNode* node, int z, CGPoint pt, int unitID, int type, int agl, float delay, eTargetType targetType, eRangeType rangeType)
: m_unitID			 (unitID	)
, m_type			 (type		)
, m_agl				 (agl		)
, m_delay			 (delay		)
, m_targetType		 (targetType)
, m_rangeType		 (rangeType )
, m_leftTime		 (delay		)
, m_isVisible		 (true		)
, m_isTouch			 (true		)
, m_isAlphaAdd		 (false		)
, m_isDarkMode		 (false		)
, m_isActiveHero	 (false		)
, m_pt				 (pt		)
, m_alpha			 (0			)
, m_alphaSub		 (0			)
, m_skill_Back		 (NULL		)
, m_skill_BackNoTouch(NULL		)
, m_skill_Icon		 (NULL		)
, m_skill_Name		 (NULL		)
, m_skill_CT		 (NULL		)
{
	if(type != -1)
	{
		NSString* strName = [NSString stringWithFormat:@"skill_name_%d.png", m_type];

		m_skill_Back		= new cSprite( node, @"towerBattle_skill_BG.png" , z, CGPointZero, CGPointMake(pt.x		, pt.y	   ) );
		m_skill_BackNoTouch	= new cSprite( node, @"towerBattle_skill_BG2.png", z, CGPointZero, CGPointMake(pt.x		, pt.y	   ) );
		m_skill_Icon		= new cSprAni( node, @"skill_icons.png"			 , z,			   CGPointMake(pt.x + 29, pt.y + 35) );
		m_skill_Name		= new cSprite( node, strName					 , z, CGPointZero, CGPointMake(pt.x + 39, pt.y +  2) );
		m_skill_CT			= new cSprite( node, @"towerBattle_skill_CT.png" , z, CGPointZero, CGPointMake(pt.x +  2, pt.y +  2) );

		for(int a = 0; a < 11; a++)	m_skill_Icon->addFrame( CGRectMake(a * 20, 0, 20, 20), 0, false, false, CGPointZero, CGRectZero, CGRectZero );

		m_skill_Icon->setFrame_init(m_type);
		m_skill_Icon->setVisible(true);
		m_skill_Icon->setLoop(false);
		m_skill_Icon->stop();

		if(m_skill_Name != NULL)
		{
			CGPoint pt = CGPointMake(m_skill_Name->get().position.x - (int)(m_skill_Name->get().contentSize.width/2), m_skill_Name->get().position.y) ;
			m_skill_Name->get().position = pt;
		}

		update(0);
	}
	else
	{
		m_skill_Back = new cSprite( node, @"towerBattle_skill_BG.png", z, CGPointZero, CGPointMake(pt.x, pt.y) );

		m_isVisible = false;
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
cSkillBtn::~cSkillBtn()
{
	m_skill_Back = NULL;
	m_skill_Icon = NULL;
	m_skill_Name = NULL;
	m_skill_CT	 = NULL;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::update(double leftTIme)
{
	if(m_type == -1)	return;

	if(m_isActiveHero == true)	leftTIme = 0;

	if(m_isTouch == false || m_isActiveHero == true)
	{
		ccColor3B color;
		if(m_isDarkMode == true)	{	color.r = 60;	color.g = 60;	color.b = 60;	}
		else						{	color.r = 120;	color.g = 120;	color.b = 120;	}

		m_skill_Name->get().color = color;
		if(m_isDarkMode == true)	m_skill_Icon->setColor( 50,  50,  50);
		else						m_skill_Icon->setColor(100, 100, 100);

		if(m_isDarkMode == true)
		{
			ccColor3B color2;
			color2.r = 120;	color2.g = 120;	color2.b = 120;

			m_skill_Back	   ->get().color = color2;
			m_skill_BackNoTouch->get().color = color2;
		}
		else
		{
			ccColor3B color2;
			color2.r = 255;	color2.g = 255;	color2.b = 255;

			m_skill_Back	   ->get().color = color2;
			m_skill_BackNoTouch->get().color = color2;
		}

		m_skill_Back	   ->get().visible = false;
		m_skill_BackNoTouch->get().visible = true;
	}
	else
	{
		ccColor3B color;
		if(m_isDarkMode == true)	{	color.r = 120;	color.g = 120;	color.b = 120;	}
		else						{	color.r = 255;	color.g = 255;	color.b = 255;	}

		m_skill_Name->get().color = color;
		if(m_isDarkMode == true)	m_skill_Icon->setColor(120, 120, 120);
		else						m_skill_Icon->setColor(255, 255, 255);

		if(m_isDarkMode == true)
		{
			ccColor3B color2;
			color2.r = 120;	color2.g = 120;	color2.b = 120;

			m_skill_Back	   ->get().color = color2;
			m_skill_BackNoTouch->get().color = color2;
		}
		else
		{
			ccColor3B color2;
			color2.r = 255;	color2.g = 255;	color2.b = 255;

			m_skill_Back	   ->get().color = color2;
			m_skill_BackNoTouch->get().color = color2;
		}

		m_skill_Back	   ->get().visible = true;
		m_skill_BackNoTouch->get().visible = false;
	}

	if(m_leftTime <= 0)
	{
		if(m_isAlphaAdd == false)
		{
			m_alpha -= m_alphaSub;
			if(m_alpha < 0)	{	m_alpha = 0;	m_alphaSub = 1;	m_isAlphaAdd = true;	}
		}
		else
		{
			m_alpha += m_alphaSub;
			if(m_alpha > 50)	{	m_alpha = 50;	m_alphaSub = 1;	m_isAlphaAdd = false;	}
		}

		ccColor3B color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		m_skill_CT->get().color	  = color;
		m_skill_CT->get().opacity = (int)m_alpha;

		[ m_skill_CT->get() setTextureRect:CGRectMake(0, 0, 74, 35) ];
		m_skill_CT->get().position = CGPointMake(m_pt.x + 2, m_pt.y + 2);

		if(m_isDarkMode == true)	m_skill_CT->get().visible = false;
	}
	else
	{
		m_skill_CT->get().visible = true;

		m_leftTime -= leftTIme + ( leftTIme * ( (float)m_agl/200 ) );

		if(m_leftTime < 0)
		{
			m_leftTime	 = 0;
			m_isAlphaAdd = false;
			m_alpha		 = 255;
			m_alphaSub	 = 10;

			ccColor3B color;
			color.r = 255;
			color.g = 255;
			color.b = 255;
			m_skill_CT->get().color	  = color;
			m_skill_CT->get().opacity = m_alpha;

			[ m_skill_CT->get() setTextureRect:CGRectMake(0, 0, 74, 35) ];
			m_skill_CT->get().position = CGPointMake(m_pt.x + 2, m_pt.y + 2);

			if(m_isDarkMode == true)	m_skill_CT->get().visible = false;
		}
		else
		{
			ccColor3B color;
			color.r = 0;
			color.g = 0;
			color.b = 0;
			m_skill_CT->get().color	  = color;
			m_skill_CT->get().opacity = 200;

			float height = (m_leftTime * 35) / m_delay;
			[ m_skill_CT->get() setTextureRect:CGRectMake(0, 0, 74, height) ];
			m_skill_CT->get().position = CGPointMake(m_pt.x + 2, m_pt.y + 2 + 35-height);
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::setIsTouch(bool isTouch)
{
	m_isTouch = isTouch;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::setVisible(bool isVisible)
{
	if(m_type == -1)	return;

	m_isVisible = true;

	m_skill_Back	   ->get().visible = isVisible;
	m_skill_BackNoTouch->get().visible = isVisible;
	m_skill_Name	   ->get().visible = isVisible;
	m_skill_CT		   ->get().visible = isVisible;
	m_skill_Icon	   ->setVisible(isVisible);
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
bool cSkillBtn::getActive()
{
	if(m_type == -1)	return false;

	if(m_leftTime <= 0)	return true;
	else				return false;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
bool cSkillBtn::getIsTouch()
{
	return m_isTouch == true || m_isActiveHero == true;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::set_agl(int agl)
{
	m_agl = agl;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::setDarkMode(bool isDark)
{
	m_isDarkMode = isDark;

	update(0);
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::setActiveHero(bool isActive)
{
	
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
void cSkillBtn::setLeftTime(double time)
{
	m_leftTime = time;
}





@implementation towerBattleNode


//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
+ (id)scene:(bool)isContinue
{
	CCScene *s = [CCScene node];
	id node = [towerBattleNode node];
	[node initWithIsContinue:isContinue];
	[s addChild:node];

	return s;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)initWithIsContinue:(bool)isContinue;
{
	//switch---------------------------------------------
	isTouchEnabled = YES;

	//time-----------------------------------------------
	m_tickCount		  = 0;
	m_beforeTickCount = 0;

	//node change----------------------------------------
	m_isNodeChangeIn  = true;
	m_isNodeChangeOut = false;
	m_pNodeChangeImg  = new cNodeChangeImgs(self, false);

	//back-----------------------------------------------
	m_sprBack = [CCSprite spriteWithFile:@"towerBattle_bg.png"];
	m_sprBack.position	  = cpvzero;
	m_sprBack.anchorPoint = cpvzero;
	[self addChild:m_sprBack z:LAYER_BACK];

	//money----------------------------------------------
	m_UI_money = new cLabelAtlasCostom(self, @"coin_nums_s.png", 15, LAYER_MAP_MONEY);
	m_UI_money->addCharInfo(',',  7);	m_UI_money->addCharInfo('0', 15);	m_UI_money->addCharInfo('1', 15);
	m_UI_money->addCharInfo('2', 15);	m_UI_money->addCharInfo('3', 15);	m_UI_money->addCharInfo('4', 15);
	m_UI_money->addCharInfo('5', 15);	m_UI_money->addCharInfo('6', 15);	m_UI_money->addCharInfo('7', 15);
	m_UI_money->addCharInfo('8', 15);	m_UI_money->addCharInfo('9', 15);

	[self updateMoney];

	//floor----------------------------------------------
	m_floor = 1;

	//continue-------------------------------------------

	//heros----------------------------------------------
	cUnitCreateInfosPtr heroCreateInfos = new cUnitCreateInfos("heroCreateInfos.par");

	//seat index 가 있는 유닛만 골라서 가져오고 포메이션 정보를 알아온다.
	int positionFrontCount	= 0;
	int positionBackCount	= 0;
	for(int i = 0; i < (int)cMyData::getObj()->m_heroSaveDatas.size(); i++)
	{
		//init
		const cHeroSaveData& saveData = cMyData::getObj()->m_heroSaveDatas[i];
		if(saveData.seatIndex == -1)	continue;

		//hero create
		cUnitPtr hero = new cUnit(self, heroCreateInfos->getUnitInfo(saveData.createIndex), eCreateInfoFileType_Hero, LAYER_TOWER_BATTLE_UNIT_1, saveData.heroID, saveData.createIndex, saveData.seatIndex);
		hero->load(saveData.seatIndex, (ePositionType)saveData.positionType, saveData.level, saveData.hp, saveData.use_hp, saveData.atk, saveData.def, saveData.agl, saveData.crt, saveData.skills);

		//unit type
		hero->setUnitType(eUnitType_Hero);

		//position counts
		if(hero->getPositionType() == ePositionType_Front)	++positionFrontCount;
		else												++positionBackCount;

		//visible
		hero->setVisible(true);
		if(hero->get_use_hp() == 0)	{	hero->setVisiblePortrait(false);	hero->die(6);	}
		else							hero->setVisiblePortrait(true );

		//push
		m_heros.push_back(hero);
	}

	//release create info
	heroCreateInfos = NULL;

	//총 인원이 1명일 경우.
	if(m_heros.size() == 1)
	{
		m_heros[0]->setPtPortrait( CGPointMake(206,  85) );
		m_heros[0]->setPt		 ( CGPointMake(120, 110) );

		//tex laod
		m_heros[0]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
		m_heros[0]->texLoad(eUnitAniType_Normal	);
		m_heros[0]->texLoad(eUnitAniType_Attack	);
		m_heros[0]->texLoad(eUnitAniType_Special);
		m_heros[0]->texLoad(eUnitAniType_Damage	);

		//animation
		m_heros[0]->changeAni(eUnitAniType_Normal);

		//seatIndex
		m_heros[0]->setSeatIndex(0);
		
	}
	//총 인원이 2명일 경우.
	else if(m_heros.size() == 2)
	{
		//같은 포지션일 경우.
		if(positionFrontCount == 2 || positionBackCount == 2)
		{
			for(int i = 0; i < (int)m_heros.size(); i++)
			{
				m_heros[i]->setPtPortrait( CGPointMake(46 + i*320,  85) );
				m_heros[i]->setPt		 ( CGPointMake(70 + i*100, 110) );

				//tex laod
				m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
				m_heros[i]->texLoad(eUnitAniType_Normal	);
				m_heros[i]->texLoad(eUnitAniType_Attack	);
				m_heros[i]->texLoad(eUnitAniType_Special);
				m_heros[i]->texLoad(eUnitAniType_Damage	);

				//animation
				m_heros[i]->changeAni(eUnitAniType_Normal);

				//seatIndex
				m_heros[i]->setSeatIndex(i);
			}
		}
		//다른 포지션일 경우.
		else
		{
			for(int i = 0; i < (int)m_heros.size(); i++)
			{
				if(m_heros[i]->getPositionType() == ePositionType_Front)
				{
					m_heros[i]->setPtPortrait( CGPointMake(46, 85) );
					m_heros[i]->setPt		 ( CGPointMake(90, 80) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	);
					m_heros[i]->texLoad(eUnitAniType_Attack	);
					m_heros[i]->texLoad(eUnitAniType_Special);
					m_heros[i]->texLoad(eUnitAniType_Damage	);

					//seatIndex
					m_heros[i]->setSeatIndex(0);
				}
				else
				{
					m_heros[i]->setPtPortrait( CGPointMake(366,  85) );
					m_heros[i]->setPt		 ( CGPointMake(150, 125) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Attack	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Special , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Damage	 , LAYER_TOWER_BATTLE_UNIT_2	   );

					//seatIndex
					m_heros[i]->setSeatIndex(1);
				}

				//animation
				m_heros[i]->changeAni(eUnitAniType_Normal);
			}
		}
	}
	//총 인원이 3명일 경우.
	else if(m_heros.size() == 3)
	{
		//전부 같은 포지션일 경우.
		if( positionFrontCount == 3 || positionBackCount == 3 )
		{
			for(int i = 0; i < (int)m_heros.size(); i++)
			{
				m_heros[i]->setPtPortrait( CGPointMake(46 + i*160,  85) );
				m_heros[i]->setPt		 ( CGPointMake(40 + i* 80, 110) );

				//tex load
				m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
				m_heros[i]->texLoad(eUnitAniType_Normal	);
				m_heros[i]->texLoad(eUnitAniType_Attack	);
				m_heros[i]->texLoad(eUnitAniType_Special);
				m_heros[i]->texLoad(eUnitAniType_Damage	);

				//animation
				m_heros[i]->changeAni(eUnitAniType_Normal);

				//seatIndex
				m_heros[i]->setSeatIndex(i);
			}
		}
		//front 타입이 2명일 경우.
		else if(positionFrontCount == 2)
		{
			for(int i = 0, setFrontCount = 0; i < (int)m_heros.size(); i++)
			{
				if(m_heros[i]->getPositionType() == ePositionType_Front)
				{
					m_heros[i]->setPtPortrait( CGPointMake(46 + setFrontCount*320, 85) );
					m_heros[i]->setPt		 ( CGPointMake(65 + setFrontCount*110, 80) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	);
					m_heros[i]->texLoad(eUnitAniType_Attack	);
					m_heros[i]->texLoad(eUnitAniType_Special);
					m_heros[i]->texLoad(eUnitAniType_Damage	);

					//seatIndex
					m_heros[i]->setSeatIndex(setFrontCount);

					++setFrontCount;
				}
				else
				{
					m_heros[i]->setPtPortrait( CGPointMake(206,  85) );
					m_heros[i]->setPt		 ( CGPointMake(120, 125) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Attack	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Special , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Damage	 , LAYER_TOWER_BATTLE_UNIT_2	   );

					//seatIndex
					m_heros[i]->setSeatIndex(2);
				}

				//animation
				m_heros[i]->changeAni(eUnitAniType_Normal);
			}
		}
		//front 타입이 1명인 경우.
		else
		{
			for(int i = 0, setBackCount = 0; i < (int)m_heros.size(); i++)
			{
				if(m_heros[i]->getPositionType() == ePositionType_Back)
				{
					m_heros[i]->setPtPortrait( CGPointMake(46 + setBackCount*320,  85) );
					m_heros[i]->setPt		 ( CGPointMake(65 + setBackCount*110, 125) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Attack	 , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Special , LAYER_TOWER_BATTLE_UNIT_2	   );
					m_heros[i]->texLoad(eUnitAniType_Damage	 , LAYER_TOWER_BATTLE_UNIT_2	   );

					//seatIndex
					m_heros[i]->setSeatIndex(1 + setBackCount);

					++setBackCount;
				}
				else
				{
					m_heros[i]->setPtPortrait( CGPointMake(206, 85) );
					m_heros[i]->setPt		 ( CGPointMake(120, 80) );

					//tex load
					m_heros[i]->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
					m_heros[i]->texLoad(eUnitAniType_Normal	);
					m_heros[i]->texLoad(eUnitAniType_Attack	);
					m_heros[i]->texLoad(eUnitAniType_Special);
					m_heros[i]->texLoad(eUnitAniType_Damage	);

					//seatIndex
					m_heros[i]->setSeatIndex(0);
				}

				//animation
				m_heros[i]->changeAni(eUnitAniType_Normal);
			}
		}
	}

	//seat sort
	std::list<cUnitPtr> sortHeros;
	for(int i = 0; i < (int)m_heros.size(); i++)	sortHeros.push_back( m_heros[i] );
	sortHeros.sort(unitSeatSort);
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i] = NULL;
	m_heros.clear();
	for(std::list<cUnitPtr>::iterator node = sortHeros.begin(); node != sortHeros.end(); ++node)	m_heros.push_back( *node );


	//mobs-----------------------------------------------
	cUnitCreateInfosPtr mobCreateInfos = new cUnitCreateInfos("towerMobCreateInfos.par");

	//xmlLoad
	NSString* xmlPath = [ NSString stringWithFormat:@"%@/towerFloorMob.xml", [ [NSBundle mainBundle] bundlePath ] ];
	ticpp::Document root( [xmlPath UTF8String] );
	root.LoadFile();

	//base info
	ticpp::Element* pXmlBaseInfo = root.FirstChildElement("TowerMobInfo");

	//floor max
	int floorMax = 0;
	ticpp::Element* pFloorMaxInfo = (ticpp::Element*)pXmlBaseInfo->FirstChildElement("floor_max");
	pFloorMaxInfo->GetAttribute( "max", &floorMax );

	//error test
	std::ostringstream ostrFloorMaxTest;
	ostrFloorMaxTest << "floor_" << floorMax;
	std::string adsfaa = ostrFloorMaxTest.str();
	(ticpp::Element*) pXmlBaseInfo->FirstChildElement( ostrFloorMaxTest.str() );

	//floor info
	std::ostringstream ostr;
	if(m_floor > floorMax)	ostr << "floor_" << floorMax;
	else					ostr << "floor_" << m_floor;
	ticpp::Element* pFloorInfo = (ticpp::Element*) pXmlBaseInfo->FirstChildElement( ostr.str() );

	//몹 배치 타입 count
	int PositionTypeCount = 0;
	ticpp::Element* pPositionTypeInfo = (ticpp::Element *)pFloorInfo->FirstChildElement("info_0");
	while(pPositionTypeInfo != NULL)
	{
		++PositionTypeCount;

		//next
		pPositionTypeInfo = (ticpp::Element *)pPositionTypeInfo->NextSibling(false);
	}

	//랜덤하게 하나의 몹 배치 타입을 선택한다.
	std::ostringstream ostr_info;
	ostr_info << "info_" << random() % PositionTypeCount;
	pPositionTypeInfo = (ticpp::Element *)pFloorInfo->FirstChildElement( ostr_info.str() );

	//xml 의 데이타를 토대로 몹을 생성한다.
	for(int i = 0, seatIndex = 0; i < 9; i++)
	{
		//init
		std::string name;

		std::ostringstream ostr;
		ostr << "mob_" << i;
		std::string asdf2 = ostr.str();
		pPositionTypeInfo->GetAttribute( ostr.str(), &name );

		if(name.length() == 0)	continue;

		//create info
		cUnitCreateInfoPtr createInfo = mobCreateInfos->getUnitInfo(name);
		if(createInfo == NULL)	continue;

		//create mob
		cUnitPtr mob = NULL;

			 if( (i/3) == 0 )	mob = new cUnit( self, mobCreateInfos->getUnitInfo(name), eCreateInfoFileType_Tower, LAYER_TOWER_BATTLE_UNIT_1, -1, -1, seatIndex );
		else if( (i/3) == 1 )	mob = new cUnit( self, mobCreateInfos->getUnitInfo(name), eCreateInfoFileType_Tower, LAYER_TOWER_BATTLE_UNIT_2, -1, -1, seatIndex );
		else					mob = new cUnit( self, mobCreateInfos->getUnitInfo(name), eCreateInfoFileType_Tower, LAYER_TOWER_BATTLE_UNIT_3, -1, -1, seatIndex );

		//unit type
		mob->setUnitType(eUnitType_Mob);

		//set
		mob->texLoad(eUnitAniType_Portrait, LAYER_TOWER_BATTLE_UNIT_PORTRAIT);
		mob->texLoad(eUnitAniType_Normal  );
		mob->texLoad(eUnitAniType_Attack  );
		mob->texLoad(eUnitAniType_Special );
		mob->texLoad(eUnitAniType_Damage  );

		mob->setPtPortrait( CGPointMake(414, 297) );

			 if( (i/3) == 0 )	{	mob->setPt( CGPointMake(280 + (i%3)*80,  80) );	mob->setMobLineIndex(0);	}
		else if( (i/3) == 1 )	{	mob->setPt( CGPointMake(290 + (i%3)*70, 112) );	mob->setMobLineIndex(1);	}
		else					{	mob->setPt( CGPointMake(300 + (i%3)*60, 145) );	mob->setMobLineIndex(2);	}

		mob->setVisible		   (true );
		mob->setVisiblePortrait(false);
		mob->changeAni(eUnitAniType_Normal);

		//push
		m_mobs.push_back(mob);

		//seat index
		++seatIndex;
	}

	//release create info
	mobCreateInfos = NULL;

	//effect---------------------------------------------
	m_listEffects.push_back( new cSprEffects(self, LAYER_TOWER_BATTLE_EFFECT_1_1, LAYER_TOWER_BATTLE_EFFECT_1_2) );
	m_listEffects.push_back( new cSprEffects(self, LAYER_TOWER_BATTLE_EFFECT_2_1, LAYER_TOWER_BATTLE_EFFECT_2_2) );
	m_listEffects.push_back( new cSprEffects(self, LAYER_TOWER_BATTLE_EFFECT_3_1, LAYER_TOWER_BATTLE_EFFECT_3_2) );

	//unit commander-------------------------------------
	m_unitCommander = new cUnitCommanderTower(self, m_listEffects, m_heros, m_mobs);
	m_unitCommander->setSelectMob(m_selectMobIndex);

	//status BG------------------------------------------
	m_sprStatusBG = new cSprite(self, @"towerBattle_status_bg.png", LAYER_TOWER_BATTLE_STATUS_1, CGPointZero, CGPointZero);

	//mob hp---------------------------------------------
	m_sprMob_HP_BG	= new cSprite( self, @"towerBattle_status_mob_hp_BG.png", LAYER_TOWER_BATTLE_STATUS_1, CGPointZero, CGPointMake(1, 285) );
	m_sprMob_HP		= new cSprite( self, @"towerBattle_status_mob_hp.png"   , LAYER_TOWER_BATTLE_STATUS_1, CGPointZero, CGPointMake(4, 287) );

	//hero hp & skill------------------------------------
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		//int hp
		cSpritePtr sprGageBG = NULL;
		cSpritePtr sprGage	 = NULL;
		cSpritePtr sprDanger = NULL;

		//get skill
		sSkill skill_1;
		sSkill skill_2;
		skill_1.type		= -1;
		skill_2.type		= -1;
		skill_1.waitTime	= 0;
		skill_2.waitTime	= 0;
		skill_1.targetType	= eTargetType_End;
		skill_2.targetType	= eTargetType_End;
		skill_1.rangeType	= eRangeType_End;
		skill_2.rangeType	= eRangeType_End;

		//position type
		for(int a = 0; a < m_heros[i]->getSkillCount(); a++)
		{
			sSkill loopSkill = m_heros[i]->getSkill(a);

			if( loopSkill.positonType == m_heros[i]->getPositionType() )
			{
				if(skill_1.type == -1)	skill_1 = loopSkill;
				else					skill_2 = loopSkill;
			}
		}

		//btn create
		cSkillBtnPtr pSkillBtn_1 = NULL;
		cSkillBtnPtr pSkillBtn_2 = NULL;

		if(m_heros.size() == 1)
		{
			sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 160, 41) );
			sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 160, 42) );
			sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 160, 62) );

			pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(162, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
			pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(240, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
		}
		else if(m_heros.size() == 2)
		{
			if(positionFrontCount == 2 || positionBackCount == 2)
			{
				sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + i*320, 41) );
				sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + i*320, 42) );
				sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + i*320, 62) );

				pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake( 2 + i*320, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
				pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(80 + i*320, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
			}
			else
			{
				sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 160 + i*160, 41) );
				sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 160 + i*160, 42) );
				sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 160 + i*160, 62) );

				pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(162 + i*160, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
				pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(240 + i*160, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
			}
		}
		else if(m_heros.size() == 3)
		{
			//전부 같은 포지션일 경우.
			if( positionFrontCount == 3 || positionBackCount == 3 )
			{
				sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + i*160, 41) );
				sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + i*160, 42) );
				sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + i*160, 62) );

				pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake( 2 + i*160, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
				pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(80 + i*160, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
			}
			//front 타입이 2명일 경우.
			else if(positionFrontCount == 2)
			{
				if(i == 0)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake( 2, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(80, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
				else if(i == 1)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 320, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 320, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 320, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(322, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(380, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
				else if(i == 2)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 160, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 160, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 160, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(162, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(240, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
			}
			//front 타입이 1명인 경우.
			else
			{
				if(i == 0)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 160, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 160, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 160, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(162, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(240, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
				else if(i == 1)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake( 2, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(80, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
				else if(i == 2)
				{
					sprGageBG = new cSprite( self, @"towerBattle_hp_bg.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(46 + 320, 41) );
					sprGage	  = new cSprite( self, @"towerBattle_hp.png"	   , LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(47 + 320, 42) );
					sprDanger = new cSprite( self, @"towerBattle_hp_danger.png", LAYER_TOWER_BATTLE_STATUS_2, CGPointZero, CGPointMake(48 + 320, 62) );

					pSkillBtn_1 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(322, 2), m_heros[i]->getUnitID(), skill_1.type, m_heros[i]->get_use_agl(), skill_1.waitTime, skill_1.targetType, skill_1.rangeType);
					pSkillBtn_2 = new cSkillBtn(self, LAYER_TOWER_BATTLE_STATUS_2, CGPointMake(380, 2), m_heros[i]->getUnitID(), skill_2.type, m_heros[i]->get_use_agl(), skill_2.waitTime, skill_2.targetType, skill_2.rangeType);
				}
			}
		}

		//hp push
		m_sprHP_Gage_BGs.push_back(sprGageBG);
		m_sprHP_Gages	.push_back(sprGage	);
		m_sprHP_Dangers	.push_back(sprDanger);

		cLabelAtlasCostomPtr labelHP = new cLabelAtlasCostom(self, @"towerBattle_hp_num.png", 13, LAYER_TOWER_BATTLE_STATUS_2);
		labelHP->addCharInfo('0', 11);	labelHP->addCharInfo('1', 10);	labelHP->addCharInfo('2', 11);
		labelHP->addCharInfo('3', 11);	labelHP->addCharInfo('4', 11);	labelHP->addCharInfo('5', 11);
		labelHP->addCharInfo('6', 11);	labelHP->addCharInfo('7', 11);	labelHP->addCharInfo('8', 11);
		labelHP->addCharInfo('9', 11);	labelHP->addCharInfo('/', 11);	labelHP->addCharInfo(' ',  6);

		m_labelHP.push_back(labelHP);

		//skill push
		m_skillBtns_1.push_back(pSkillBtn_1);
		m_skillBtns_2.push_back(pSkillBtn_2);
	}

	[self updateHP];

	//hero select----------------------------------------
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		//init
		cSpritePtr pSpr = NULL;

		//create
		if(m_heros[i]->getPositionType() == ePositionType_Front)	pSpr = new cSprite(self, @"towerBattle_mobCursor.png", LAYER_TOWER_BATTLE_UNIT_CUR_1, CGPointZero, CGPointZero);
		else														pSpr = new cSprite(self, @"towerBattle_mobCursor.png", LAYER_TOWER_BATTLE_UNIT_CUR_2, CGPointZero, CGPointZero);

		//set
		CGPoint heroPt		  = m_heros[i]->getPt();
		CGPoint heroSpecialPt = m_heros[i]->getPtSpecial();
		pSpr->get().position  = CGPointMake(heroPt.x + heroSpecialPt.x - 34, heroPt.y + heroSpecialPt.y);
		pSpr->get().visible	  = false;

		//push
		m_sprSelectHeros.push_back(pSpr);
	}

	m_selectHeroCurVisibleTime = 0;

	//select mob-----------------------------------------
	m_sprSelectMob_1 = new cSprite(self, @"towerBattle_mobCursor.png", LAYER_TOWER_BATTLE_UNIT_CUR_1, CGPointZero, CGPointZero);
	m_sprSelectMob_2 = new cSprite(self, @"towerBattle_mobCursor.png", LAYER_TOWER_BATTLE_UNIT_CUR_2, CGPointZero, CGPointZero);
	m_sprSelectMob_3 = new cSprite(self, @"towerBattle_mobCursor.png", LAYER_TOWER_BATTLE_UNIT_CUR_3, CGPointZero, CGPointZero);
	[self setMobSelect:0];

	//exercise skill-------------------------------------
	m_isSelectHeroMode	= false;
	m_selectSkillUnitID	= -1;
	m_selectSkillType	= -1;

	//schedule-------------------------------------------
	[ self schedule: @selector(step :) ];
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)onExit
{
	//back
	[self removeChild:m_sprBack cleanup:YES];
	m_sprBack = nil;

	//effects
	for(int i = 0; i < (int)m_listEffects.size(); i++)	m_listEffects[i] = NULL;

	//money
	m_UI_money = NULL;

	//mob commander
	m_unitCommander = NULL;

	//unit
	for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i] = NULL;
	for(int i = 0; i < (int)m_mobs .size(); i++)	m_mobs [i] = NULL;
	m_heros.clear();
	m_mobs .clear();

	//status BG
	m_sprStatusBG = NULL;

	//hero hp
	for(int i = 0; i < (int)m_sprHP_Gage_BGs.size(); i++)	m_sprHP_Gage_BGs[i] = NULL;
	for(int i = 0; i < (int)m_sprHP_Gages	.size(); i++)	m_sprHP_Gages	[i] = NULL;
	for(int i = 0; i < (int)m_sprHP_Dangers	.size(); i++)	m_sprHP_Dangers	[i] = NULL;
	for(int i = 0; i < (int)m_labelHP		.size(); i++)	m_labelHP		[i] = NULL;
	m_sprHP_Gage_BGs.clear();
	m_sprHP_Gages	.clear();
	m_labelHP		.clear();

	//mob hp
	m_sprMob_HP_BG	= NULL;
	m_sprMob_HP		= NULL;

	//hero select
	for(int i = 0; i < (int)m_sprSelectHeros.size(); i++)	m_sprSelectHeros[i] = NULL;
	m_sprSelectHeros.clear();

	//mob select
	m_sprSelectMob_1 = NULL;
	m_sprSelectMob_2 = NULL;
	m_sprSelectMob_3 = NULL;

	//hero skill
	for(int i = 0; i < (int)m_skillBtns_1 .size(); i++)	m_skillBtns_1 [i] = NULL;
	for(int i = 0; i < (int)m_skillBtns_2 .size(); i++)	m_skillBtns_2 [i] = NULL;
	m_skillBtns_1 .clear();
	m_skillBtns_2 .clear();

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

		//mob 선택.
		for(int i = 0; i < (int)m_mobs.size(); i++)
		{
			if(m_mobs[i]->isRectInPt(ptTouch) == true)
			{
				[self setMobSelect:i];
				break;
			}
		}

		//영웅 선택.
		if(m_isSelectHeroMode == true)
		{
			[self touchHeroSelect:ptTouch];
		}
		else
		{
			//스킬 선택.
			[self touchSkill:ptTouch];

			//포션 선택.
		}

		//mob commander
		m_unitCommander->setSelectMob(m_selectMobIndex);
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

		//mob commander
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

		//mob commander
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)touchSkill:(CGPoint)ptTouch
{
	for(int i = 0; i < (int)m_skillBtns_1.size(); i++)
	{
		if(m_skillBtns_1[i]->getIsTouch() == false || m_skillBtns_1[i]->getActive() == false)	continue;

		if(RectInPt(CGRectMake(m_skillBtns_1[i]->getPt().x, m_skillBtns_1[i]->getPt().y, 78, 39), ptTouch) == true)
		{
			if(m_skillBtns_1[i]->getTargetType() == eTargetType_Ally_One)
			{
				[ self setSelectHeroMode:true skillUnitID:m_skillBtns_1[i]->getUnitID() skillType:m_skillBtns_1[i]->getSkillType() ];
			}
			else
			{
				m_unitCommander->exerciseSkill(m_skillBtns_1[i]->getUnitID(), m_skillBtns_1[i]->getSkillType(), -1, m_selectMobIndex);
				m_skillBtns_1[i]->setLeftTime( m_skillBtns_1[i]->getDelay() );
			}

			return;
		}
	}
	for(int i = 0; i < (int)m_skillBtns_2.size(); i++)
	{
		if(m_skillBtns_2[i]->getIsTouch() == false || m_skillBtns_2[i]->getActive() == false)	continue;

		if(RectInPt(CGRectMake(m_skillBtns_2[i]->getPt().x, m_skillBtns_2[i]->getPt().y, 78, 39), ptTouch) == true)
		{
			if(m_skillBtns_2[i]->getTargetType() == eTargetType_Ally_One)
			{
				[ self setSelectHeroMode:true skillUnitID:m_skillBtns_2[i]->getUnitID() skillType:m_skillBtns_2[i]->getSkillType() ];
			}
			else
			{
				m_unitCommander->exerciseSkill(m_skillBtns_2[i]->getUnitID(), m_skillBtns_2[i]->getSkillType(), -1, m_selectMobIndex);
				m_skillBtns_2[i]->setLeftTime( m_skillBtns_2[i]->getDelay() );
			}

			return;
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)touchHeroSelect:(CGPoint)ptTouch
{
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		if(m_heros[i]->isRectInPt(ptTouch) == true)
		{
			m_unitCommander->exerciseSkill(m_selectSkillUnitID, m_selectSkillType, m_heros[i]->getUnitID(), m_selectMobIndex);

			for(int a = 0; a < (int)m_heros.size(); a++)
			{
				if(m_heros[a]->getUnitID() == m_selectSkillUnitID)
				{
						 if(m_skillBtns_1[a]->getSkillType() == m_selectSkillType)	m_skillBtns_1[a]->setLeftTime( m_skillBtns_1[a]->getDelay() );
					else if(m_skillBtns_2[a]->getSkillType() == m_selectSkillType)	m_skillBtns_2[a]->setLeftTime( m_skillBtns_2[a]->getDelay() );
				}
			}

			break;
		}
	}

	[self setSelectHeroMode:false skillUnitID:-1 skillType:-1];
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

	//node chage
	if( [self updateNodeChage:leftTickCount] == true )	return;

	//mob commander
	m_unitCommander->update(leftTickCount);

	//hp
	[self updateHP];

	//mob select
	[self update_mobHP];

	//hero skill
	[self update_skillBtn:leftTickCount];

	//select hero mode
	[self update_selectHeroMode:leftTickCount];

	//update Effect
	for(int i = 0; i < (int)m_listEffects.size(); i++)	m_listEffects[i]->update(leftTickCount);

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
				m_isNodeChangeIn = false;
				m_pNodeChangeImg = nil;
			}
			else if(m_isNodeChangeOut == true)
			{
				m_isNodeChangeOut = false;
				m_pNodeChangeImg  = nil;

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
- (void)updateHP
{
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		//gage
		float width = (m_heros[i]->get_use_hp() * 64) / m_heros[i]->getHP();
		[ m_sprHP_Gages[i]->get() setTextureRect:CGRectMake(0, 0, width, 4) ];

		//danger
		if( m_heros[i]->get_use_hp() < m_heros[i]->getHP() / 6 )	m_sprHP_Dangers[i]->get().visible = true;
		else														m_sprHP_Dangers[i]->get().visible = false;

		//get BG position
		CGPoint ptBG = m_sprHP_Gage_BGs[i]->get().position;

		//label
		NSMutableString* str = NULL;
			 if(m_heros[i]->get_use_hp() <  10)	str = [ NSMutableString stringWithFormat:@"%d/", m_heros[i]->get_use_hp() ];
		else if(m_heros[i]->get_use_hp() < 100)	str = [ NSMutableString stringWithFormat:@"%d/", m_heros[i]->get_use_hp() ];
		else									str = [ NSMutableString stringWithFormat:@"%d/", m_heros[i]->get_use_hp() ];

			 if(m_heros[i]->getHP() <  10)	[str appendFormat:@"%d", m_heros[i]->getHP() ];
		else if(m_heros[i]->getHP() < 100)	[str appendFormat:@"%d", m_heros[i]->getHP() ];
		else								[str appendFormat:@"%d", m_heros[i]->getHP() ];

		m_labelHP[i]->setString(str);
		m_labelHP[i]->setVisible(true);
		m_labelHP[i]->setPosition( CGPointMake( (ptBG.x + 32) - (int)(m_labelHP[i]->getSize().width/2), 42) );
	}
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
- (void)update_mobHP
{
	//hp gage
	if(m_selectMobIndex != -1 && m_mobs[m_selectMobIndex]->getDie() == false)
	{
		m_sprMob_HP_BG->get().visible = true;
		m_sprMob_HP	  ->get().visible = true;

		float width = 409 - (m_mobs[m_selectMobIndex]->get_use_hp() * 409) / m_mobs[m_selectMobIndex]->getHP();
		[ m_sprMob_HP->get() setTextureRect:CGRectMake(width, 0, 409-width, 8) ];
		m_sprMob_HP->get().position = CGPointMake(4 + width, 287);
	}
	else
	{
		//hp gage
		m_sprMob_HP_BG->get().visible = false;
		m_sprMob_HP	  ->get().visible = false;

		//portrait
		for(int i = 0; i < (int)m_mobs.size(); i++)	m_mobs[i]->setVisiblePortrait(false);

		//select mob cursor
		m_sprSelectMob_1->get().visible = false;
		m_sprSelectMob_2->get().visible = false;
		m_sprSelectMob_3->get().visible = false;
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)update_skillBtn:(double)leftTime
{
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		if(m_heros[i]->getDie() == true)
		{
			m_skillBtns_1[i]->setVisible(true);
			m_skillBtns_2[i]->setVisible(true);

			continue;
		}

		m_skillBtns_1[i]->set_agl( m_heros[i]->get_use_agl() );
		m_skillBtns_2[i]->set_agl( m_heros[i]->get_use_agl() );

		m_skillBtns_1[i]->update(leftTime);
		m_skillBtns_2[i]->update(leftTime);

		if(m_skillBtns_1[i]->getTargetType() == eTargetType_Enemy_One)
		{
			if(m_selectMobIndex == -1 || m_mobs[m_selectMobIndex]->getDie() == true)
			{
				m_skillBtns_1[i]->setIsTouch(false);
			}
		}
		if(m_skillBtns_2[i]->getTargetType() == eTargetType_Enemy_One)
		{
			if(m_selectMobIndex == -1 || m_mobs[m_selectMobIndex]->getDie() == true)
			{
				m_skillBtns_2[i]->setIsTouch(false);
			}
		}
		if(m_unitCommander->getIsActiveSkill(  m_heros[i]->getUnitID() ) == true)
		{
			m_skillBtns_1[i]->setActiveHero(true);
			m_skillBtns_2[i]->setActiveHero(true);
		}
		else
		{
			m_skillBtns_1[i]->setActiveHero(false);
			m_skillBtns_2[i]->setActiveHero(false);
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)update_selectHeroMode:(double)leftTime
{
	if(m_isSelectHeroMode == false)	return;

	m_selectHeroCurVisibleTime -= leftTime;

	if(m_selectHeroCurVisibleTime < 0)
	{
		for(int i = 0; i < (int)m_sprSelectHeros.size(); i++)
		{
			if(m_sprSelectHeros[i]->get().visible == false)	m_sprSelectHeros[i]->get().visible = true;
			else											m_sprSelectHeros[i]->get().visible = false;
		}

		m_selectHeroCurVisibleTime = 0.5f;
	}
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
- (void)setMobSelect:(int)select
{
	m_selectMobIndex = select;

	if(m_selectMobIndex == -1)
	{
		//cursor
		m_sprSelectMob_1->get().visible = false;
		m_sprSelectMob_2->get().visible = false;
		m_sprSelectMob_3->get().visible = false;

		//portrait
		for(int i = 0; i < (int)m_mobs.size(); i++)	m_mobs[i]->setVisiblePortrait(false);

		return;
	}

	//cursor
	CGPoint mobPt		 = m_mobs[m_selectMobIndex]->getPt();
	CGPoint mobSpecialPt = m_mobs[m_selectMobIndex]->getPtSpecial();

	m_sprSelectMob_1->get().visible = false;
	m_sprSelectMob_2->get().visible = false;
	m_sprSelectMob_3->get().visible = false;

	if(m_mobs[m_selectMobIndex]->getMobLineIndex() == 0)
	{
		m_sprSelectMob_1->get().visible	 = true;
		m_sprSelectMob_1->get().position = CGPointMake(mobPt.x + mobSpecialPt.x - 34, mobPt.y + mobSpecialPt.y);
	}
	else if(m_mobs[m_selectMobIndex]->getMobLineIndex() == 1)
	{
		m_sprSelectMob_2->get().visible	 = true;
		m_sprSelectMob_2->get().position = CGPointMake(mobPt.x + mobSpecialPt.x - 34, mobPt.y + mobSpecialPt.y);
	}
	else
	{
		m_sprSelectMob_3->get().visible	 = true;
		m_sprSelectMob_3->get().position = CGPointMake(mobPt.x + mobSpecialPt.x - 34, mobPt.y + mobSpecialPt.y);
	}

	//portrait
	for(int i = 0; i < (int)m_mobs.size(); i++)
	{
		if(i == m_selectMobIndex)	m_mobs[i]->setVisiblePortrait(true );
		else						m_mobs[i]->setVisiblePortrait(false);
	}

	//mob line index
	int mobLineIndex	= m_mobs[m_selectMobIndex]->getMobLineIndex();
	int mobLine_1_count = 0;
	int mobLine_2_count = 0;

	for(int i = 0; i < (int)m_mobs.size(); i++)
	{
		if(m_mobs[i]->getDie() == true)	continue;

		if(m_mobs[i]->getMobLineIndex() == 0)	++mobLine_1_count;
		if(m_mobs[i]->getMobLineIndex() == 1)	++mobLine_2_count;
	}

	if(mobLineIndex == 1 &&  mobLine_1_count == 0)								mobLineIndex = 0;
	if(mobLineIndex == 2 &&  mobLine_1_count == 0 && mobLine_2_count == 0)		mobLineIndex = 0;
	if(mobLineIndex == 2 && (mobLine_1_count == 0 || mobLine_2_count == 0) )	mobLineIndex = 1;

	//hero front count
	int heroFroutCount = 0;
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		if(m_heros[i]->getDie() == true)	continue;

		if(m_mobs[i]->getPositionType() == ePositionType_Front)	++heroFroutCount;
	}

	//skill
	for(int i = 0; i < (int)m_heros.size(); i++)
	{
		if(m_heros[i]->getDie() == true)	continue;

		//타겟과의 line 수
		int heroLineIndex = 0;
		if(m_heros[i]->getPositionType() == ePositionType_Back && heroFroutCount > 0)	++heroLineIndex;

		// _1_
		if( m_skillBtns_1[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_1[i]->getRangeType() == eRangeType_Short )
		{
			if(heroLineIndex == 0 && mobLineIndex == 0)	m_skillBtns_1[i]->setIsTouch(true );
			else										m_skillBtns_1[i]->setIsTouch(false);
		}
		else if( m_skillBtns_1[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_1[i]->getRangeType() == eRangeType_Long )
		{
			if(heroLineIndex == 1 && mobLineIndex != 0)	m_skillBtns_1[i]->setIsTouch(false);
			else										m_skillBtns_1[i]->setIsTouch(true );
		}
		else if( m_skillBtns_1[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_1[i]->getRangeType() == eRangeType_Back )
		{
			if( heroLineIndex == 0 && (mobLineIndex == 0 || mobLineIndex == 1) )	m_skillBtns_1[i]->setIsTouch(true );
			else																	m_skillBtns_1[i]->setIsTouch(false);
		}
		else
		{
			m_skillBtns_1[i]->setIsTouch(true);
		}

		// _2_
		if( m_skillBtns_2[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_2[i]->getRangeType() == eRangeType_Short )
		{
			if(heroLineIndex == 0 && mobLineIndex == 0)	m_skillBtns_2[i]->setIsTouch(true );
			else										m_skillBtns_2[i]->setIsTouch(false);
		}
		else if( m_skillBtns_2[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_2[i]->getRangeType() == eRangeType_Long )
		{
			if(heroLineIndex == 1 && mobLineIndex != 0)	m_skillBtns_2[i]->setIsTouch(false);
			else										m_skillBtns_2[i]->setIsTouch(true );
		}
		else if( m_skillBtns_2[i]->getTargetType() == eTargetType_Enemy_One && m_skillBtns_2[i]->getRangeType() == eRangeType_Back )
		{
			if( heroLineIndex == 0 && (mobLineIndex == 0 || mobLineIndex == 2) )	m_skillBtns_2[i]->setIsTouch(true );
			else																	m_skillBtns_2[i]->setIsTouch(false);
		}
		else
		{
			m_skillBtns_2[i]->setIsTouch(true);
		}
	}
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
- (void)setSelectHeroMode:(bool)isSelectHeroMode skillUnitID:(int)skillUnitID skillType:(int)skillType
{
	//set
	m_isSelectHeroMode	= isSelectHeroMode;
	m_selectSkillUnitID	= skillUnitID;
	m_selectSkillType	= skillType;

	if(m_isSelectHeroMode == true)
	{
		//select cursor
		for(int i = 0; i < (int)m_sprSelectHeros.size(); i++)	m_sprSelectHeros[i]->get().visible = true;
		m_selectHeroCurVisibleTime = 0.5f;

		//color
		ccColor3B color;
		color.r = 120;	color.g = 120;	color.b = 120;

		//back
		m_sprBack.color = color;

		//effect
		for(int i = 0; i < (int)m_listEffects.size(); i++)	m_listEffects[i]->setColor(120, 120, 120);

		//money
		m_UI_money->setColor(255, 120, 120, 120);

		//heros
		for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i]->setPortraitColor(120, 120, 120);

		//mobs
		for(int i = 0; i < (int)m_mobs.size(); i++)
		{
			m_mobs[i]->setColor		   (255, 120, 120, 120);
			m_mobs[i]->setPortraitColor(	 120, 120, 120);
		}

		//unit commander
		m_unitCommander->setDamageLabelColor (120, 120, 120);
		m_unitCommander->setStatusEffectColor(120, 120, 120);
		m_unitCommander->setSignIconColor	 (120, 120, 120);

		//status BG
		m_sprStatusBG->get().color = color;

		//hero hp
		for(int i = 0; i < (int)m_sprHP_Gage_BGs.size(); i++)	m_sprHP_Gage_BGs[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Gages	.size(); i++)	m_sprHP_Gages	[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Dangers	.size(); i++)	m_sprHP_Dangers	[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Dangers	.size(); i++)	m_labelHP[i]->setColor(255, 120, 120, 120);

		//mob hp
		m_sprMob_HP_BG->get().color = color;
		m_sprMob_HP	  ->get().color = color;

		//mob select
		m_sprSelectMob_1->get().color = color;
		m_sprSelectMob_2->get().color = color;
		m_sprSelectMob_3->get().color = color;

		//skill
		for(int i = 0; i < (int)m_skillBtns_1.size(); i++)	m_skillBtns_1[i]->setDarkMode(true);
		for(int i = 0; i < (int)m_skillBtns_2.size(); i++)	m_skillBtns_2[i]->setDarkMode(true);
	}
	else
	{
		//select cursor
		for(int i = 0; i < (int)m_sprSelectHeros.size(); i++)	m_sprSelectHeros[i]->get().visible = false;
		m_selectHeroCurVisibleTime = 0;

		//color
		ccColor3B color;
		color.r = 255;	color.g = 255;	color.b = 255;

		//back
		m_sprBack.color = color;

		//effect
		for(int i = 0; i < (int)m_listEffects.size(); i++)	m_listEffects[i]->setColor(255, 255, 255);

		//money
		m_UI_money->setColor(255, 255, 255, 255);

		//heros
		for(int i = 0; i < (int)m_heros.size(); i++)	m_heros[i]->setPortraitColor(255, 255, 255);

		//mobs
		for(int i = 0; i < (int)m_mobs.size(); i++)
		{
			m_mobs[i]->setColor		   (255, 255, 255, 255);
			m_mobs[i]->setPortraitColor(	 255, 255, 255);
		}

		//unit commander
		m_unitCommander->setDamageLabelColor (255, 255, 255);
		m_unitCommander->setStatusEffectColor(255, 255, 255);
		m_unitCommander->setSignIconColor	 (255, 255, 255);

		//status BG
		m_sprStatusBG->get().color = color;

		//hero hp
		for(int i = 0; i < (int)m_sprHP_Gage_BGs.size(); i++)	m_sprHP_Gage_BGs[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Gages	.size(); i++)	m_sprHP_Gages	[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Dangers	.size(); i++)	m_sprHP_Dangers	[i]->get().color = color;
		for(int i = 0; i < (int)m_sprHP_Dangers	.size(); i++)	m_labelHP[i]->setColor(255, 255, 255, 255);

		//mob hp
		m_sprMob_HP_BG->get().color = color;
		m_sprMob_HP	  ->get().color = color;

		//mob select
		m_sprSelectMob_1->get().color = color;
		m_sprSelectMob_2->get().color = color;
		m_sprSelectMob_3->get().color = color;

		//skill
		for(int i = 0; i < (int)m_skillBtns_1.size(); i++)	m_skillBtns_1[i]->setDarkMode(false);
		for(int i = 0; i < (int)m_skillBtns_2.size(); i++)	m_skillBtns_2[i]->setDarkMode(false);
	}
}



@end
