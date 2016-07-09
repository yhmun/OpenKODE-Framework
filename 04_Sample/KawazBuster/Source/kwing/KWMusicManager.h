/* --------------------------------------------------------------------------
 *
 *      File            KWMusicManager.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/21 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWMusicManager_h__
#define __KWMusicManager_h__

class KWMusicManager : public CCObject
{
	public :

		static KWMusicManager*		sharedManager ( KDvoid );

	protected :

		KDbool		init ( KDvoid );

	public :

		KDvoid		playMusic ( const KDchar* szFile, const KDchar* szIntroFile, KDbool bLoop );
		KDvoid		playMusic ( const KDchar* szFile, KDbool bLoop );

		KDvoid		stopMusic ( KDvoid );

		KDvoid		fadeout ( KDfloat s );

		KDvoid		setIntroMusicCompletionListener ( CCObject* pListener, SEL_CallFunc pSelector );

		KDvoid		changeMusic ( const KDchar* szFile, const KDchar* szIntroFile, KDbool bLoop, KDfloat s );

	private :

		KDvoid		onSound ( KDuint uID, KDint uParam );

		KDvoid		playNextMusic ( KDfloat fDelta );

		KDvoid		volumeDown ( KDfloat fDelta );

	private :
		
		KDuint				m_uIntroMusic;
		KDuint				m_uLoopMusic;

		std::string			m_sNextIntroMusic;
		std::string			m_sNextLoopMusic;
		KDbool				m_bNextLoop;

		SEL_CallFunc		m_pIntroMusicCompletionSelector;
		CCObject*			m_pIntroMusicCompletionListener;
};

#endif	// __KWMusicManager_h__