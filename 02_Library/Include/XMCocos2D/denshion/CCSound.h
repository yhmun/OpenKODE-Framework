/* -----------------------------------------------------------------------------------
 *
 *      File            CCSound.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCSound_h__
#define __CCSound_h__

#include "../cocoa/CCObject.h"

NS_CC_BEGIN
 
enum
{
	CCSoundNormal        = XM_SOUND_NORMAL,
	CCSoundEffect        = XM_SOUND_EFFECT,
	CCSoundRepeat        = XM_SOUND_REPEAT,
	CCSoundPlay          = XM_SOUND_PLAY,
	CCSoundAutoPauseOff  = XM_SOUND_AUTO_PAUSE_OFF,
};

class CCSound
{
	protected :
				 CCSound ( KDvoid );
		virtual ~CCSound ( KDvoid );

	public :

		 static CCSound*  sharedSound ( KDvoid );
	     static KDvoid     purgeSound ( KDvoid );

	public :

		 KDuint		Open      ( const KDchar* szFilePath, KDuint nMode = 0 );
		 KDint		Close     ( KDuint uSoundID );

		 KDint		Play      ( KDuint uSoundID );
		 KDint		Stop      ( KDuint uSoundID );

		 KDint		Pause     ( KDuint uSoundID );
		 KDint		Resume    ( KDuint uSoundID );

		 KDint		Rewind    ( KDuint uSoundID );

		 KDint		setRepeat ( KDuint uSoundID, KDbool bOn );

		 KDfloat	getVolume ( KDuint uSoundID );
		 KDint		setVolume ( KDuint uSoundID, KDfloat fVolume );

		 KDfloat	getVolumes ( KDuint nMode );
		 KDint		setVolumes ( KDuint nMode, KDfloat fVolume );
		 
		 /////////////////////////////////////////////////////////////////////////////////////
		 // Effect Mode : Not worked.
		 KDint		getDuration ( KDuint uSoundID );

		 KDint		getPosition ( KDuint uSoundID );
		 KDint		setPosition ( KDuint uSoundID, KDint nPos );

		 KDint		setCallFunc ( KDuint uSoundID, CCObject* pTarget, SEL_SOUND pSelector );
};

NS_CC_END

#endif // __CCSound_h__
