//
//  SapusIntroNode.m
//  SapusTongue
//
//  Created by Ricardo Quesada on 23/09/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "towerEntranceNode.h"
#import "towerBattleNode.h"
#import "mapNode.h"
#import "chipmunk.h"
#import "SimpleAudioEngine.h"
#include "LayerDefine.h"
#include "nodeChangeImgs.h"
#include "sprite.h"
#include "myData.h"



@implementation towerEntranceNode



//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
+ (id)scene
{
	CCScene *s = [CCScene node];
	id node = [towerEntranceNode node];
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

	//time
	m_tickCount		  = 0;
	m_beforeTickCount = 0;

	//back
	m_sprBack = [CCSprite spriteWithFile:@"towerEntrance_bg.png"];
	m_sprBack.position	  = cpvzero;
	m_sprBack.anchorPoint = cpvzero;
	[self addChild:m_sprBack z:LAYER_BACK];

	//btns
	CCMenuItemImage* itemNew	  = [ CCMenuItemImage itemFromNormalImage:@"towerEntrance_bt_new.png"		selectedImage:@"towerEntrance_bt_new_push.png"		 target:self selector:@selector( newCallback	  : ) ];
	CCMenuItemImage* itemContinue = [ CCMenuItemImage itemFromNormalImage:@"towerEntrance_btn_continue.png"	selectedImage:@"towerEntrance_btn_continue_push.png" target:self selector:@selector( continueCallback : ) ];
	CCMenuItemImage* itemExit	  = [ CCMenuItemImage itemFromNormalImage:@"towerEntrance_btn_exit.png"		selectedImage:@"towerEntrance_btn_exit_push.png"	 target:self selector:@selector( exitCallback	  : ) ];

	m_btnNew	  = [CCMenu menuWithItems:itemNew	  , nil];
	m_btnContinue = [CCMenu menuWithItems:itemContinue, nil];
	m_btnExit	  = [CCMenu menuWithItems:itemExit	  , nil];

	m_btnNew	 .position = cpv(135, 160);
	m_btnContinue.position = cpv(135, 100);
	m_btnExit	 .position = cpv(135,  40);

	m_btnNew	 .visible = YES;
	m_btnContinue.visible = YES;
	m_btnExit	 .visible = YES;

	m_btnNew	 .isTouchEnabled = YES;
	m_btnContinue.isTouchEnabled = YES;
	m_btnExit	 .isTouchEnabled = YES;

	[self addChild:m_btnNew		 z:LAYER_MENE];
	[self addChild:m_btnContinue z:LAYER_MENE];
	[self addChild:m_btnExit	 z:LAYER_MENE];

	//switch
	m_isContinue = false;

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
	//back
	[self removeChild:m_sprBack cleanup:YES];
	m_sprBack = nil;

	//btns
	[self removeChild:m_btnNew		cleanup:YES];
	[self removeChild:m_btnContinue	cleanup:YES];
	[self removeChild:m_btnExit		cleanup:YES];

	m_btnNew	  = nil;
	m_btnContinue = nil;
	m_btnExit	  = nil;

	//unschedule
	[self unschedule: @selector(step:)];

	//super exit
	[super onExit];
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

				//object release
				[self onExit];

				//cenece change
				[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.0f scene:[towerBattleNode scene:m_isContinue] ] ];

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
-(void) newCallback:(id)sender
{
	m_btnNew	 .isTouchEnabled = false;
	m_btnContinue.isTouchEnabled = false;
	m_btnExit	 .isTouchEnabled = false;

	m_isNodeChangeOut = true;
	m_pNodeChangeImg  = new cNodeChangeImgs(self, true);

	m_isContinue = false;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) continueCallback:(id)sender
{
	m_btnNew	 .isTouchEnabled = false;
	m_btnContinue.isTouchEnabled = false;
	m_btnExit	 .isTouchEnabled = false;

	m_isNodeChangeOut = true;
	m_pNodeChangeImg  = new cNodeChangeImgs(self, true);

	m_isContinue = false;
}

//----------------------------------------------------------------------------------------------
// Name : JINE
// Dest : 
//----------------------------------------------------------------------------------------------
-(void) exitCallback:(id)sender
{
	m_btnNew	 .isTouchEnabled = false;
	m_btnContinue.isTouchEnabled = false;
	m_btnExit	 .isTouchEnabled = false;

	//schedule
	[self unschedule: @selector(step:)];

	[ [CCDirector sharedDirector] replaceScene:[ CCFadeTransition transitionWithDuration:0.5f scene:[mapNode scene:false] ] ];
}


@end
