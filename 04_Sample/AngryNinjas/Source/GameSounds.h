/* --------------------------------------------------------------------------
 *
 *      File            GameSounds.h
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

#ifndef __GameSounds_h__
#define __GameSounds_h__

class GameSounds
{
	public :
		
		GameSounds ( KDvoid );

		static GameSounds*		sharedGameSounds ( KDvoid );

	public :

		KDvoid					preloadSounds ( KDvoid );

		KDbool					areSoundFXMuted ( KDvoid );
		KDvoid					setSoundFXMuted ( KDbool bValue );

		KDbool					areVoiceFXMuted ( KDvoid );
		KDvoid					setVoiceFXMuted ( KDbool bValue );

		KDvoid					playSoundFX ( const KDchar* szFileToPlay );

		KDvoid					introTag ( KDvoid );		

		KDvoid					playStackImpactSound ( KDvoid );

		KDvoid					playBreakSound ( KDvoid );

		KDvoid					releaseSlingSounds ( KDvoid );

		KDvoid					playBackgroundMusic ( AmbientFXSounds eTrack );

		KDvoid					stopBackgroundMusic ( KDvoid );

		KDvoid					restartBackgroundMusic ( KDvoid );

	private :

		KDbool					m_bSoundFXTurnedOff;
		KDbool					m_bVoiceFXTurnedOff;
		KDbool					m_bAmbientFXTurnedOff;			// ambient or background music
		
		std::string				m_sDelayedSoundName;
		
		AmbientFXSounds			m_eMusicChoice;
};

#endif	// __GameSounds_h__