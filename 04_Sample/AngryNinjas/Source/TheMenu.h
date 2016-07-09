/* --------------------------------------------------------------------------
 *
 *      File            TheMenu.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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

#ifndef __TheMenu_h__
#define __TheMenu_h__

class TheMenu : public CCLayer
{
	public :

		SCENE_FUNC ( TheMenu );

	protected :

		virtual KDbool		init ( KDvoid );

		KDvoid				goToFirstLevelSection ( CCObject* pSender );

		KDvoid				playNegativeSound ( CCObject* pSender );

		KDvoid				popAndTransition ( KDvoid );

		// Sound FX
		KDvoid				setSoundFXMenuActive ( KDbool bValue );

		KDvoid				turnSoundFXOn  ( CCObject* pSender );
		KDvoid				turnSoundFXOff ( CCObject* pSender );

		// VOICE FX
		KDvoid				setVoiceFXMenuActive ( KDbool bValue );

		KDvoid				turnVoiceFXOn  ( CCObject* pSender );
		KDvoid				turnVoiceFXOff ( CCObject* pSender );

		// Ambient FX
		KDvoid				setAmbientFXMenuActive ( KDbool bValue );

		KDvoid				turnAmbientFXOn  ( CCObject* pSender );
		KDvoid				turnAmbientFXOff ( CCObject* pSender );

	private :

		CCMenu*				m_pVoiceFXMenu;
		CCMenu*				m_pSoundFXMenu;
		CCMenu*				m_pAmbientFXMenu;
		
		CCPoint				m_tVoiceFXMenuLocation;
		CCPoint 			m_tSoundFXMenuLocation;
		CCPoint 			m_tAmbientFXMenuLocation;
		
		std::string			m_sMenuBackgroundName;
		
		std::string			m_aLvlButtonNames		[ 10 ];
		std::string			m_aLvlLockedButtonNames [ 10 ];
		
		std::string			m_sVoiceButtonName;
		std::string			m_sVoiceButtonNameDim;
		
		std::string			m_sSoundButtonName;
		std::string			m_sSoundButtonNameDim;
		
		std::string			m_sAmbientButtonName;
		std::string			m_sAmbientButtonNameDim;
};

#endif	// __TheMenu_h__
