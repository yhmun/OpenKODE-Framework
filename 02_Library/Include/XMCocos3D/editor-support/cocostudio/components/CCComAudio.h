/* -----------------------------------------------------------------------------------
 *
 *      File            CCComAudio.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCComAudio_h__
#define __CCComAudio_h__

#include "../../../support/component/CCComponent.h"

namespace cocostudio {

class ComAudio : public cocos2d::Component
{
	protected :

		/**
		 *	@js ctor
		 */
		ComAudio ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ComAudio ( KDvoid );
    
	public :

		virtual KDbool			init ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onEnter ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onExit ( KDvoid );

		virtual KDbool			 isEnabled ( KDvoid ) const;
		virtual KDvoid			setEnabled ( KDbool b );

		static ComAudio*		create ( KDvoid );
   
	public :

		KDvoid					end ( KDvoid );

		KDvoid					preloadBackgroundMusic ( const KDchar* pszFilePath );
		
		KDvoid					playBackgroundMusic ( const KDchar* pszFilePath, KDbool bLoop );
		KDvoid					playBackgroundMusic ( const KDchar* pszFilePath );
		
		KDvoid					stopBackgroundMusic ( KDbool bReleaseData );
		KDvoid					stopBackgroundMusic ( KDvoid );
		
		KDvoid					pauseBackgroundMusic ( KDvoid );
		
		KDvoid					resumeBackgroundMusic ( KDvoid );
		
		KDvoid					rewindBackgroundMusic ( KDvoid );
		
		KDbool					willPlayBackgroundMusic ( KDvoid );
		
		KDbool					 isBackgroundMusicPlaying ( KDvoid );

		KDfloat					getBackgroundMusicVolume ( KDvoid );
		KDvoid					setBackgroundMusicVolume ( KDfloat fVolume );
		
		KDfloat					getEffectsVolume ( KDvoid );
		KDvoid					setEffectsVolume ( KDfloat fVolume );

		KDuint					playEffect ( const KDchar* pszFilePath, KDbool bLoop );
		KDuint					playEffect ( const KDchar* pszFilePath);

		KDvoid 					pauseEffect ( KDuint nSoundId );
		KDvoid 					pauseAllEffects ( KDvoid );

		KDvoid 					resumeEffect ( KDuint nSoundId );
		KDvoid 					resumeAllEffects ( KDvoid );

		KDvoid 					stopEffect ( KDuint nSoundId );
		KDvoid 					stopAllEffects ( KDvoid );

		KDvoid 					preloadEffect ( const KDchar* pszFilePath );
		KDvoid 					unloadEffect ( const KDchar* pszFilePath );
		
		KDvoid 					setFile ( const KDchar* pszFilePath );
		const KDchar*			getFile ( KDvoid );

		KDvoid					setLoop ( KDbool bLoop );
		KDbool					 isLoop ( KDvoid );

	private :

		std::string				m_sFilePath;
		KDbool					m_bLoop;
};

}

#endif  // __CCComAudio_h__
