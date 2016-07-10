//
//  towerBattleNode.h
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 Sapus Media. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "cocos2d.h"
#include <vector>
#include <list>
#include "RefPointee.h"
#include "define.h"


DefineIntrusivePtr(cSprite);
DefineIntrusivePtr(cSprAni);
DefineIntrusivePtr(cSkillBtn);
class cSkillBtn : public RefPointee
{
public :
	cSkillBtn(CCNode* node, int z, CGPoint pt, int unitID, int type, int agl, float delay, eTargetType targetType, eRangeType rangeType);
	virtual ~cSkillBtn();

	void update(double leftTIme);

	void setVisible   (bool isVisible);
	void setIsTouch	  (bool isTouch);
	void set_agl	  (int agl);
	void setDarkMode  (bool isDark);
	void setActiveHero(bool isActive);
	void setLeftTime  (double time);

	bool getActive ();
	bool getIsTouch();
	inline int		   getUnitID()		{	return m_unitID;		}
	inline int		   getSkillType()	{	return m_type;			}
	inline double	   getDelay()		{	return m_delay;			}
	inline eTargetType getTargetType()	{	return m_targetType;	}
	inline eRangeType  getRangeType	()	{	return m_rangeType;		}
	inline CGPoint	   getPt()			{	return m_pt;			}


private :
	int			m_unitID;
	int			m_type;
	int			m_agl;
	double		m_delay;
	eTargetType m_targetType;
	eRangeType	m_rangeType;
	double		m_leftTime;
	bool		m_isVisible;
	bool		m_isTouch;
	bool		m_isAlphaAdd;
	bool		m_isDarkMode;
	bool		m_isActiveHero;
	CGPoint		m_pt;
	float		m_alpha;
	float		m_alphaSub;

	cSpritePtr	m_skill_Back;
	cSpritePtr	m_skill_BackNoTouch;
	cSprAniPtr	m_skill_Icon;
	cSpritePtr	m_skill_Name;
	cSpritePtr	m_skill_CT;
};


@class Sprite;
DefineIntrusivePtr(cUnit);
DefineIntrusivePtr(cUnitCreateInfo);
DefineIntrusivePtr(cUnitCommanderTower);
DefineIntrusivePtr(cSprEffects);
DefineIntrusivePtr(cLabelAtlasCostom);
DefineIntrusivePtr(cNodeChangeImgs);
@interface towerBattleNode : CCLayer
{
	//touch
	int m_tidCount;
	std::list<sTouch> m_listTouch;

	//time
	double m_tickCount;
	double m_beforeTickCount;

	//node change
	bool m_isNodeChangeIn;
	bool m_isNodeChangeOut;
	cNodeChangeImgsPtr m_pNodeChangeImg;

	//back sprite
	CCSprite* m_sprBack;

	//effects
	std::vector<cSprEffectsPtr> m_listEffects;

	//money
	cLabelAtlasCostomPtr m_UI_money;

	//unit
	std::vector<cUnitPtr> m_heros;
	std::vector<cUnitPtr> m_mobs;

	//unit commander
	cUnitCommanderTowerPtr m_unitCommander;

	//status BG
	cSpritePtr m_sprStatusBG;

	//hero hp
	std::vector<cSpritePtr			> m_sprHP_Gage_BGs;
	std::vector<cSpritePtr			> m_sprHP_Gages;
	std::vector<cSpritePtr			> m_sprHP_Dangers;
	std::vector<cLabelAtlasCostomPtr> m_labelHP;

	//mob hp
	cSpritePtr m_sprMob_HP_BG;
	cSpritePtr m_sprMob_HP;

	//hero select
	std::vector<cSpritePtr> m_sprSelectHeros;
	double m_selectHeroCurVisibleTime;

	//mob select
	cSpritePtr	m_sprSelectMob_1;
	cSpritePtr	m_sprSelectMob_2;
	cSpritePtr	m_sprSelectMob_3;
	int			m_selectMobIndex;

	//hero skill
	std::vector<cSkillBtnPtr> m_skillBtns_1;
	std::vector<cSkillBtnPtr> m_skillBtns_2;

	//floor
	int m_floor;

	//exercise skill
	bool m_isSelectHeroMode;
	int	 m_selectSkillUnitID;
	int	 m_selectSkillType;
}

+ (id)scene:(bool)isContinue;

- (void)initWithIsContinue:(bool)isContinue;

- (void)touchSkill	   :(CGPoint)ptTouch;
- (void)touchHeroSelect:(CGPoint)ptTouch;

- (bool)updateNodeChage:(double)leftTIme;
- (void)updateHP;
- (void)update_mobHP;
- (void)update_skillBtn:(double)leftTime;
- (void)update_selectHeroMode:(double)leftTime;
- (void)updateMoney;
- (void)setMobSelect:(int)select;
- (void)setSelectHeroMode:(bool)isSelectHeroMode skillUnitID:(int)skillUnitID skillType:(int)skillType;



@end
