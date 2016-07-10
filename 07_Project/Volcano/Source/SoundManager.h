/* --------------------------------------------------------------------------
 *
 *      File            SoundManager.h
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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

#ifndef __SoundManager_h__
#define __SoundManager_h__

class SoundManager : public CCObject
{
	private :
				 SoundManager ( KDvoid );
		virtual ~SoundManager ( KDvoid );

	public : 

		static SoundManager*	sharedSoundManager ( KDvoid );

	public : 

		KDvoid		soundOnOff  ( CCMenuItemImage* pSoundOnMenuItem, CCMenuItemImage* pSoundOffMenuItem );
		KDvoid		soundOnOff  ( CCMenuItemImage* pSoundOnMenuItem, CCMenuItemImage* pSoundOffMenuItem, KDbool bSoundOff );

		KDvoid		playEffectDie  ( KDvoid );

};

#endif // __SoundManager_h__
