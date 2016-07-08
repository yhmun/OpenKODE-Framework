/* -----------------------------------------------------------------------------------
 *
 *      File            SimpleAudioEngine.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Steve Oldmeadow
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

#ifndef __SimpleAudioEngine_h__
#define __SimpleAudioEngine_h__

#include "../ccTypeInfo.h"

NS_CC_BEGIN

/**
 *	@class	SimpleAudioEngine
 *	@brief	offer a VERY simple interface to play background music & sound effect
 */

class SimpleAudioEngine : public TypeInfo
{
	protected :
				 SimpleAudioEngine ( KDvoid );
		virtual ~SimpleAudioEngine ( KDvoid );

	public :

		/**
		 *	@brief Get the shared Engine object,it will new one when first time be called
		 */
		static SimpleAudioEngine*	sharedEngine ( KDvoid );

		/**
		 *	@brief Release the shared Engine object
		 *	@warning It must be called before the application exit, or a memroy leak will be casued.
		 */
		static KDvoid				end ( KDvoid );

	public :

		virtual KDint	getClassTypeInfo ( KDvoid );

		/**
		 *	@brief Preload background music
		 *	@param szFilePath The path of the background music file,or the FileName of T_SoundResInfo
		 */
		KDvoid		preloadBackgroundMusic ( const KDchar* szFilePath );
	    
		/**
		 *	@brief Play background music
		 *	@param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
		 *	@param bLoop Whether the background music loop or not
		 */
		KDvoid		playBackgroundMusic ( const KDchar* szFilePath, KDbool bLoop );
		KDvoid		playBackgroundMusic ( const KDchar* szFilePath );
		
		/**
		 *	@brief Stop playing background music
		 *	@param bReleaseData If release the background music data or not.As default value is false
		 */
		KDvoid		stopBackgroundMusic ( KDbool bReleaseData );
		KDvoid		stopBackgroundMusic ( KDvoid );

		/**
		 *	@brief Pause playing background music
		 */
		KDvoid		pauseBackgroundMusic ( KDvoid );

		/**
		 *	@brief Resume playing background music
		 */
		KDvoid		resumeBackgroundMusic ( KDvoid );

		/**
		 *	@brief Rewind playing background music
		 */
		KDvoid		rewindBackgroundMusic ( KDvoid );

		KDbool		willPlayBackgroundMusic ( KDvoid );

		/**
		 *	@brief Whether the background music is playing
		 *	@return If is playing return true,or return false
		 */
		KDbool		isBackgroundMusicPlaying ( KDvoid );

		//
		// properties
		//

		/**
		 *	@brief The volume of the background music max value is 1.0,the min value is 0.0
		 */
		KDfloat		getBackgroundMusicVolume ( KDvoid );

		/**
		 *	@brief set the volume of background music
		 *	@param volume must be in 0.0~1.0
		 */
		KDvoid		setBackgroundMusicVolume ( KDfloat fVolume );

		/**
		 *	@brief The volume of the effects max value is 1.0,the min value is 0.0
		 */
		KDfloat		getEffectsVolume ( KDvoid );

		/**
		 *	@brief set the volume of sound effecs
		 *	@param volume must be in 0.0~1.0
		 */
		KDvoid		setEffectsVolume ( KDfloat fVolume );

		KDfloat		getEffectVolume ( KDuint uID );
		KDvoid		setEffectVolume ( KDuint uID, KDfloat fVolume );

		KDfloat		getEffectsPitch ( KDvoid );
		KDvoid		setEffectsPitch ( KDfloat fPitch );

		KDfloat		getEffectPitch ( KDuint uID );
		KDvoid		setEffectPitch ( KDuint uID, KDfloat fPitch );

		//
		// for sound effects
		//

		/**
		 *	@brief Play sound effect
		 *	@param szFilePath The path of the effect file,or the FileName of T_SoundResInfo
		 *	@bLoop Whether to loop the effect playing, default value is false
		 */
		KDuint		playEffect ( const KDchar* szFilePath, KDbool bLoop );
		KDuint		playEffect ( const KDchar* szFilePath );
		KDvoid		playEffect ( KDuint uSoundId, KDbool bLoop = KD_FALSE );

		/**
		 *	@brief Pause playing sound effect
		 *	@param uSoundId The return value of function playEffect
		 */
		KDvoid		pauseEffect ( KDuint uSoundId );

		/**
		 *	@brief Pause all playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		KDvoid		pauseAllEffects ( KDvoid );

		/**
		 *	@brief Resume playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		KDvoid		resumeEffect ( KDuint uSoundId );

		/**
		 *	@brief Resume all playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		KDvoid		resumeAllEffects ( KDvoid );

		/**
		 *	@brief Stop playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		KDvoid		stopEffect ( KDuint uSoundId );

		/**
		 *	@brief Stop all playing sound effects
		 */
		KDvoid		stopAllEffects ( KDvoid );

		/**
		 *	@brief	 preload a compressed audio file
		 *	@details the compressed audio will be decode to wave, then write into an 
		 *	internal buffer in SimpleaudioEngine
		 */
		KDuint		preloadEffect ( const KDchar* szFilePath );

		KDuint		preloadEffect ( const KDchar* szFilePath, const KDchar* szKey );

		/**
		 *	@brief		unload the preloaded effect from internal buffer
		 *	@param[in]	szFilePath The path of the effect file,or the FileName of T_SoundResInfo
		 */
		KDvoid		unloadEffect ( const KDchar* szFilePath );

		KDvoid		unloadEffect ( KDuint uSoundId );
};

NS_CC_END

#endif	// __SimpleAudioEngine_h__

