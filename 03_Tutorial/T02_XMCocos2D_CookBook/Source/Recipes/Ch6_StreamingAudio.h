/* --------------------------------------------------------------------------
 *
 *      File            Ch6_StreamingAudio.h
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

#ifndef __Ch6_StreamingAudio_h__
#define __Ch6_StreamingAudio_h__

class Ch6_StreamingAudio : public Recipe
{
	public :

		SCENE_FUNC ( Ch6_StreamingAudio );

	protected :		

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				onExit ( KDvoid );

		CCMenuItemSprite*			menuItemFromSpriteFile ( const KDchar* szFile, CCObject* pListener, SEL_MenuHandler pSelector );

		KDvoid						playMusic	 ( CCObject* pSender );
		KDvoid						pauseMusic	 ( CCObject* pSender );
		KDvoid						stopMusic	 ( CCObject* pSender );
		KDvoid						nextSong	 ( CCObject* pSender );
		KDvoid						previousSong ( CCObject* pSender );

		KDvoid						setIsPlaying ( KDvoid );

		KDvoid						setStreamSource ( KDvoid );

	private :

//		MPMoviePlayerController*	m_pMoviePlayer;
		CCArray*					m_pStreamingSources;
		KDint						m_nSourceIndex;
		KDbool						m_bIsPlaying;
};

#endif	// __Ch6_StreamingAudio_h__
