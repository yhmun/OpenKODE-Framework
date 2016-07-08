/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingSimpleData.h
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

#ifndef __Ch3_SavingSimpleData_h__
#define __Ch3_SavingSimpleData_h__

class Ch3_SavingSimpleData : public Recipe
{
	public :

		SCENE_FUNC ( Ch3_SavingSimpleData );

	protected :		

		virtual KDbool		init ( KDvoid );

		KDvoid				loadDifficulty ( KDvoid );
		KDvoid				saveDifficulty ( KDvoid );

		KDvoid				resetMenuColors ( KDvoid );

		KDvoid				setDifficultyFromValue ( KDvoid );

		KDvoid				chooseEasy   ( CCObject* pSender );
		KDvoid				chooseMedium ( CCObject* pSender );
		KDvoid				chooseHard	 ( CCObject* pSender );
		KDvoid				chooseInsane ( CCObject* pSender );

	public :

		std::string			m_sDifficulty;
		CCMenuItemFont*		m_pEasyMIF;
		CCMenuItemFont*		m_pMediumMIF;
		CCMenuItemFont*		m_pHardMIF;
		CCMenuItemFont*		m_pInsaneMIF;
};

#endif	// __Ch3_SavingSimpleData_h__
