//
//  shopNode.h
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
DefineIntrusivePtr(cUnitCreateInfos);
DefineIntrusivePtr(cUnit);
DefineIntrusivePtr(cLabelParas);
DefineIntrusivePtr(cLabels);
@interface formationNode : CCLayer
{
	class cLabelParas : public RefPointee
	{
	public :
		cLabelParas(cLabelsPtr pLabel, CGPoint _pt, int z);
		virtual ~cLabelParas();

		void setString(cLabelsPtr pLabel, NSString* strName, NSString* strHp, NSString* strAtk, NSString* strDef, NSString* strAgl, NSString* strCrt);

	private :
		id labelName;
		id labelHp;
		id labelAtk;
		id labelDef;
		id labelAgl;
		id labelCrt;
	};

	//touch
	int m_tidCount;
	std::list<sTouch> m_listTouch;

	//time
	double m_tickCount;
	double m_beforeTickCount;

	//back
	CCSprite* m_sprBack;

	//labels
	cLabelsPtr m_labels;

	//hero
	cUnitCreateInfosPtr		m_heroCreateInfos;
	std::vector<cUnitPtr>	m_heros;

	//label papa
	std::vector<cLabelParasPtr> m_labelParas;

	//btns
	CCMenu* m_btnExit;
	CCMenu* m_btnLeft;
	CCMenu* m_btnRight;

	//char back img
	std::vector<cSpritePtr> m_sprCharBGs;
	std::vector<cSpritePtr> m_sprCharErrors;
	std::vector<double	  > m_charErrorLeftTimes;

	//skill sprite
	std::vector<cSpritePtr> m_sprSkillBGs;
	std::vector<cSprAniPtr>	m_aniSkillIcons;

	//label position types
	std::vector<cSpritePtr> m_spPositionFronts;
	std::vector<cSpritePtr> m_spPositionBacks;

	//scroll
	int m_scrollPos;
}


+ (id)scene;

- (void)touchBegan:(CGPoint)pt;

- (void)setScrollPos:(int)scrollPos;
- (void)updateHero:(double)leftTime;


@end
