//
//  towerEntranceNode.h
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 Sapus Media. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "cocos2d.h"
#include <vector>
#include "RefPointee.h"


@class Sprite;
DefineIntrusivePtr(cNodeChangeImgs);
@interface towerEntranceNode : CCLayer
{
	//time
	double m_tickCount;
	double m_beforeTickCount;

	//node change
	bool m_isNodeChangeIn;
	bool m_isNodeChangeOut;
	cNodeChangeImgsPtr m_pNodeChangeImg;

	//back sprite
	CCSprite* m_sprBack;

	//btns
	CCMenu* m_btnNew;
	CCMenu* m_btnContinue;
	CCMenu* m_btnExit;

	//switch
	bool m_isContinue;
}

+ (id)scene;

- (bool)updateNodeChage:(double)leftTIme;


@end
