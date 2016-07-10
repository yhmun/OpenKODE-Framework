/* -----------------------------------------------------------------------------------
 *
 *      File            MainNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __MainNode_h__
#define __MainNode_h__

#include "Object/Sprite/Sprite.h"
#include "Object/NodeChageImgs/NodeChangeImgs.h"

class MainNode : public Scene
{
	public :
				 MainNode ( KDvoid );
		virtual	~MainNode ( KDvoid );

		static MainNode*	create ( KDbool bIsNodeChangeIn );

	public :

		KDbool				init ( KDbool bIsNodeChangeIn );		

		KDvoid				removeObject ( KDvoid );

		KDvoid				step ( KDfloat fDelta );

		KDbool				updateNodeChage ( KDdouble dLeftTIme );

		KDvoid				setNodeChangeIn ( KDvoid );

		KDvoid				onTouchesBagan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		KDvoid				gameCasinoCallback	( Object* pSender );
		KDvoid				myRoomCallback		( Object* pSender );
		KDvoid				rankCallback		( Object* pSender );
		KDvoid				manualCallback		( Object* pSender );
		KDvoid				creditCallback		( Object* pSender );

		KDvoid				saveDataAskYesCallback ( Object* pSender );
		KDvoid				saveDataAskNoCallback  ( Object* pSender );

	private :

		// time
		KDdouble			m_dTickCount;
		KDdouble			m_dBeforeTickCount;

		// node change
		KDbool				m_bIsNodeChangeIn;
		KDbool				m_bIsNodeChangeOut;
		cNodeChangeImgs*	m_pNodeChangeImg;

		Sprite*				m_pSprBack;
		cSprite*			m_pSprSoundOn;
		cSprite*			m_pSprSoundOff;
		cSprite*			m_pSprCredit;
		Menu*				m_pMenuCasino;
		Menu*				m_pMenuMyRoom;
		Menu*				m_pMenuRank;
		Menu*				m_pMenuManual;
		Menu*				m_pMenuCredit;

		// save data ask
		cSprite*			m_pSprSaveDataAsk;
		Menu*				m_pMenuSaveDataAskYes;
		Menu*				m_pMenuSaveDataAskNo;

		// switch
		KDbool				m_bIsGoCasinoBattle;
		KDbool				m_bIsGoCasinoResult;
		KDbool				m_bIsGoMyRoomBattle;
		KDbool				m_bIsGoMyRoomResult;
		KDbool				m_bIsGoMyRoom;

		// credit
		KDbool				m_bIsCredit;
		KDint				m_dCreditStep;
		KDdouble			m_dCreditStepTime;
};
/*
@class Sprite;
@class GradientLayer;
DefineIntrusivePtr(cSprite);
DefineIntrusivePtr(cNodeChangeImgs);
@interface mainNode : CCLayer
{

}

+ (id)scene:(bool)isNodeChangeIn;

- (bool)updateNodeChage:(double)leftTIme;
-(void)updateBack:(double)leftTime;

@end
*/

#endif	// __MainNode_h__