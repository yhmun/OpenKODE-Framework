/* --------------------------------------------------------------------------
 *
 *      File            APSAudioHolder.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSAudioHolder_h__
#define __APSAudioHolder_h__

#include "APSMedium.h"

NS_APS_BEGIN

class APSDevice;
class APSAction;
class APSAction;

class APSAudioHolder : public APSMedium
{
	public :

		APSAudioHolder ( const KDchar* szCode = "", APSResourceManager* pManager = KD_NULL );
		APSAudioHolder ( APSDictionary* pProperties );

		virtual ~APSAudioHolder ( KDvoid );
	    
		APS_CLASS_NAME ( APSAudioHolder );

	public :

		virtual KDbool			initWithProperties ( APSDictionary* pProperties );
	        
		/** Plays the audio for a given action. */
		KDvoid					playForAction ( APSAction* pAction );
	    
		/** 
		 *	Stops the currently playing audio for a given action.                     
		 *	stopForAction ( ) is canceled if the given action is not the same one that 
		 *	has triggered playing audio.
		 */
		KDvoid					stopForAction ( APSAction* pAction );
	    
		/** Minimizes memory usage. */
		virtual KDvoid			clearCachedData ( KDvoid );
	    
		/** preloads the audio before it is used. */
		virtual KDvoid			preloadData ( KDvoid );
	 
	protected :

		inline KDvoid			setEffectId ( KDuint uSoundID ) { this->m_uSoundID = uSoundID; }
		inline KDvoid			setRunningAction ( APSAction* pRunningAction ) { this->m_pRunningAction = pRunningAction; }
	    
		const std::string*		getFilePath ( KDvoid );

		/** 
		 *	isBackground is true if the audio specified by APSAudioHolder is
		 *	relatively long and used for background music.
		 */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bIsBackground, IsBackground );
	    
		/** effectId property is a value returned by playEffect ( ) of SimpleAudioEngine. */
		APS_SYNTHESIZE_READONLY ( KDuint, m_uSoundID, EffectId );

		/** An action that currently running audio of APSAudioHolder. */
		APS_SYNTHESIZE_READONLY ( APSAction*, m_pRunningAction, RunningAction );

	protected :

		const std::string*		m_pFilePath;
};

NS_APS_END

#endif	// __APSAudioHolder_h__
