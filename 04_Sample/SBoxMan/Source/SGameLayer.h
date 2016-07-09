/* --------------------------------------------------------------------------
 *
 *      File            SGameLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-08
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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

#ifndef __SGameLayer_h__
#define __SGameLayer_h__

#include "SMapLayer.h"

class SGameLayer : public CCLayer, public SGameDelegate
{
	public :

		SCENE_FUNC ( SGameLayer );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		gameDidStart ( KDvoid );

		virtual KDvoid		gameDidFinish ( KDvoid );

		virtual KDvoid		boxManDidMovedWithBox ( KDbool bWithBox );

		KDvoid				setLevelCount ( KDint nCount );

		KDvoid				setStepCount ( KDint nCount );

		KDvoid				nextLevel ( KDfloat fDelta );

		KDvoid				onPrev ( CCObject* pSender );

		KDvoid				onNext ( CCObject* pSender );
		
		KDvoid				onReset ( CCObject* pSender );

	private :

		SMapLayer*			m_pMapLayer;
		CCLabelTTF*			m_pLevelLabel;
		CCLabelTTF*			m_pStepLabel;
		KDint				m_nLevelCount;
		KDint				m_nStepCount;
};
/*
//
//  SGameLayer.h
//  SBoxMan
//
//  Created by SunJiangting on 12-12-8.
//
//

#import "CCLayer.h"
#import "cocos2d.h"
#import "SMapLayer.h"

@interface SGameLayer : CCLayer <SGameDelegate>

+ (CCScene *) scene;

@property (nonatomic, assign) NSInteger level;

@end
*/

#endif	// __SGameLayer_h__