/* --------------------------------------------------------------------------
 *
 *      File            PauseScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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

#ifndef __PauseScene_h__
#define __PauseScene_h__

class PlayingScene;

class PauseScene : public CCScene
{
	public :

		CREATE_FUNC ( PauseScene );

	public :

		virtual	KDbool		init ( KDvoid );

		KDvoid				resumeButtonPressed ( CCObject* pSender );

		KDvoid				optionsButtonPressed ( CCObject* pSender );

		KDvoid				menuButtonPressed ( CCObject* pSender );

		KDvoid				howToPlayButtonPressed ( CCObject* pSender );

		KDvoid				setPlayingScene ( PlayingScene* pScene );

	private :

		PlayingScene*		m_pPlayingScene;	// used to inform it that pause is going to be resumed just before finishing this object
};

#endif	// __PauseScene_h__
