/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      海锋 周 on 12-04-11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __GameLayer_h__
#define __GameLayer_h__

#include "CCFish.h"
#include "CCNet.h"
#include "UIRollNum.h"

class GameLayer : public CCLayer
{
	public :

		SCENE_FUNC ( GameLayer );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					loadTexture ( KDvoid );
		
		KDvoid					initUITab ( KDvoid );
		
		KDvoid					addFish ( KDvoid );
		
		KDvoid					updateGame ( KDfloat fDelta );

		KDvoid					showNet ( CCObject* pSender );

		KDvoid					updateEnergy ( KDint nEnergy );

		KDvoid					afterShowNet ( CCObject* pSender );

		KDvoid					afterShow ( CCObject* pSender );

		KDvoid					afterCatch ( CCObject* pSender );

	private :

		CCSpriteBatchNode*		m_pNetSheet;
		CCSpriteBatchNode*		m_pFishSheet;
		CCSpriteBatchNode*		m_pFish2Sheet;
		CCSpriteBatchNode*		m_pCannonSheet;
		CCAction*				m_pFish01Act;
		UIRollNum*				m_pScore1;
		CCSprite*				m_pGun;
		KDint					m_nEnergy;
		KDint					m_nMaxEnergy;
		CCSprite*				m_pEnergyPointer;
};

/*
#import  "CCFish.h"
#import  "CCNet.h"
#import  "UIRollNum.h"


// HelloWorldLayer
@interface HelloWorldLayer : CCLayer
{

    
    
}

// returns a CCScene that contains the HelloWorldLayer as the only child
+(CCScene *) scene;

@end
*/

#endif	// __GameLayer_h__