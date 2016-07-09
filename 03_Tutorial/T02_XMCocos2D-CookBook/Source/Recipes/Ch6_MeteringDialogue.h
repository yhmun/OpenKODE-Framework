/* --------------------------------------------------------------------------
 *
 *      File            Ch6_MeteringDialogue.h
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

#ifndef __Ch6_MeteringDialogue_h__
#define __Ch6_MeteringDialogue_h__

class Ch6_MeteringDialogue : public Recipe
{
	public :

		SCENE_FUNC ( Ch6_MeteringDialogue );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			onExit ( KDvoid );

		KDvoid					startRecipe ( KDvoid );
		
		KDvoid					step ( KDfloat fDelta );

		KDvoid					blink ( KDvoid );
		
		KDvoid					unblink ( KDvoid );
		
		KDvoid					resetPupils ( KDvoid );
		
		CCMenuItemSprite*		menuItemFromSpriteFile ( const KDchar* szFile, KDint nTag );
		
		KDvoid					loadLongAudioSource ( const KDchar* szFile );
		
		KDvoid					loadBackgroundMusic ( const KDchar* szFile );
		
		KDvoid					playBackgroundMusic ( const KDchar* szFile );
		
		KDvoid					playSoundNumber ( CCObject* pSender );

		KDvoid					playLongAudioSource ( const KDchar* szFile );
		
		KDvoid					setPeakAndAveragePower ( KDvoid );

		KDvoid					animateClaghorn ( KDvoid );

	private :

		SimpleAudioEngine*		m_pSAE;
		CCDictionary*			m_pSoundSources;
		CCSprite*				m_pClaghornBase;
		CCSprite*				m_pClaghornPupils;
		CCSprite*				m_pClaghornEyebrows;
		CCSprite*				m_pClaghornMouth;
		CCSprite*				m_pClaghornMoustache;
		KDfloat					m_fLastAudioLevel;
		KDbool					m_bIsBlinking;
		KDfloat					m_fPeakPower;
		KDfloat					m_fAvgPower;
};

#endif	// __Ch6_MeteringDialogue_h__
