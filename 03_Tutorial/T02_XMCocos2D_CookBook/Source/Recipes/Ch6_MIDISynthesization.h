/* --------------------------------------------------------------------------
 *
 *      File            Ch6_MIDISynthesization.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Ch6_MIDISynthesization_h__
#define __Ch6_MIDISynthesization_h__

#include "Libraries/MIDISampleGenerator.h"

class Ch6_MIDISynthesization : public Recipe
{
	public :

		SCENE_FUNC ( Ch6_MIDISynthesization );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			onExit ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					initKeyboard ( KDvoid );
		
		KDvoid					randomize ( CCObject* pSender );
		
		KDvoid					slideLeft ( CCObject* pSender );
		
		KDvoid					slideRight ( CCObject* pSender );
		
		KDbool					keyPressed ( CCSprite* pKey, KDint nHashKey );

		KDbool					keyReleased ( KDint nHashKey, KDbool bRemove );

		KDbool					touchedSprite ( CCSprite* pSprite, const CCPoint& tTouch );

	private :

		MIDISampleGenerator*	m_pSampleGenerator;
  
		CCNode*					m_pKeyboard;
		KDfloat					m_fKeySpriteScale;
		CCArray*				m_pWhiteKeys;
		CCArray*				m_pBlackKeys;
		CCDictionary*			m_pKeyTouches;
};

#endif	// __Ch6_MIDISynthesization_h__
