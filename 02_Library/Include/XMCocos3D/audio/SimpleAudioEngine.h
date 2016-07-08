/* -----------------------------------------------------------------------------------
 *
 *      File            SimpleAudioEngine.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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

#include "../ccMacros.h"
#include "Export.h"
#include <typeinfo>
#include <functional>

namespace CocosDenshion 
{

/**
 *	@class          SimpleAudioEngine
 *	@brief          Offers a VERY simple interface to play background music & sound effects.
 *	@note           Make sure to call SimpleAudioEngine::end() when the sound engine is not needed anymore
 *                  to release allocated resources.
 */

class CC_DLL SimpleAudioEngine
{
	public :

		/**
		 *	@brief Get the shared Engine object,it will new one when first time be called
		 */
		static SimpleAudioEngine*	getInstance ( KDvoid );

		/**
		 *	@brief Release the shared Engine object
		 *	@warning It must be called before the application exit, or a memory leak will be casued.
		 */
		static KDvoid				end ( KDvoid );

	protected :

				 SimpleAudioEngine ( KDvoid );
		virtual ~SimpleAudioEngine ( KDvoid );

	public:

		/**
		 *	@brief Preload background music
		 *	@param pszFilePath The path of the background music file.
		 *	@js preloadMusic
		 *	@lua preloadMusic
		 */
		virtual KDvoid				preloadBackgroundMusic ( const KDchar* szFilePath );
    
		/**
		 *	@brief Play background music
		 *	@param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
		 *	@param bLoop Whether the background music loop or not
		 *	@js playMusic
		 *	@lua playMusic
		 */
		virtual KDvoid				playBackgroundMusic ( const KDchar* szFilePath, KDbool bLoop = KD_FALSE );

		/**
		 *	@brief Stop playing background music
		 *	@param bReleaseData If release the background music data or not.As default value is false
		 *	@js stopMusic
		 *	@lua stopMusic
		 */
		virtual KDvoid				stopBackgroundMusic ( KDbool bReleaseData = KD_FALSE );

		/**
		 *	@brief Pause playing background music
		 *	@js pauseMusic
		 *	@lua pauseMusic
		 */
		virtual KDvoid				pauseBackgroundMusic ( KDvoid );

		/**
		 *	@brief Resume playing background music
		 *	@js resumeMusic
		 *	@lua resumeMusic
		 */
		virtual KDvoid				resumeBackgroundMusic ( KDvoid );

		/**
		 *	@brief Rewind playing background music
		 *	@js rewindMusic
		 *	 @lua rewindMusic
		 */
		virtual KDvoid				rewindBackgroundMusic ( KDvoid );

		/**
		 *	@brief Indicates whether any background music can be played or not.
		 *	@return <i>true</i> if background music can be played, otherwise <i>false</i>.
		 *	@js willPlayMusic
		 *	@lua willPlayMusic
		 */
		virtual KDbool				willPlayBackgroundMusic ( KDvoid );

		/**
		 *	@brief Indicates whether the background music is playing
		 *	@return <i>true</i> if the background music is playing, otherwise <i>false</i>
		 *	@js isMusicPlaying
		 *	@lua isMusicPlaying
		 */
		virtual KDbool				isBackgroundMusicPlaying ( KDvoid );

		// 
		// properties
		//

		/**
		 *	@brief The volume of the background music within the range of 0.0 as the minimum and 1.0 as the maximum.
		 *	@js getMusicVolume
		 *	@lua getMusicVolume
		 */
		virtual KDfloat				getBackgroundMusicVolume ( KDvoid );

		/**
		 *	@brief Set the volume of background music
		 *	@param volume must be within the range of 0.0 as the minimum and 1.0 as the maximum.
		 *	@js setMusicVolume
		 *	@lua setMusicVolume
		 */
		virtual KDvoid				setBackgroundMusicVolume ( KDfloat fVolume );

		/**
		 *	@brief The volume of the effects within the range of 0.0 as the minimum and 1.0 as the maximum.
		 */
		virtual KDfloat				getEffectsVolume ( KDvoid );

		/**
		 *	@brief Set the volume of sound effects
		 *	@param volume must be within the range of 0.0 as the minimum and 1.0 as the maximum.
		 */
		virtual KDvoid				setEffectsVolume ( KDfloat fVolume );

		virtual KDfloat				getEffectVolume ( KDuint uID );
		virtual KDvoid				setEffectVolume ( KDuint uID, KDfloat fVolume );

		virtual KDfloat				getEffectsPitch ( KDvoid );
		virtual KDvoid				setEffectsPitch ( KDfloat fPitch );

		virtual KDfloat				getEffectPitch ( KDuint uID );
		virtual KDvoid				setEffectPitch ( KDuint uID, KDfloat fPitch );

		// 
		// for sound effects

		/**
		 *	@brief Play sound effect with a file path, pitch, pan and gain
		 *	@param pszFilePath The path of the effect file.
		 *	@param bLoop Determines whether to loop the effect playing or not. The default value is false.
		 *	@param pitch Frequency, normal value is 1.0. Will also change effect play time.
		 *	@param pan   Stereo effect, in the range of [-1..1] where -1 enables only left channel.
		 *	@param gain  Volume, in the range of [0..1]. The normal value is 1.
		 *	@return the OpenAL source id
		 *
		 *	@note Full support is under development, now there are limitations:
		 *		- no pitch effect on Samsung Galaxy S2 with OpenSL backend enabled;
		 *		- no pitch/pan/gain on emscrippten, win32, marmalade.
		 */
		virtual KDuint				playEffect ( const KDchar* szFilePath, KDbool bLoop = KD_FALSE, KDfloat fPitch = 1.0f, KDfloat fPan = 0.0f, KDfloat fGain = 1.0f );
		virtual KDvoid				playEffect ( KDuint uSoundId, KDbool bLoop = KD_FALSE, KDfloat fPitch = 1.0f, KDfloat fPan = 0.0f, KDfloat fGain = 1.0f );

		/**
		 *	@brief Pause playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		virtual KDvoid				pauseEffect ( KDuint uSoundId );
			
		/**
		 *	@brief Pause all playing sound effect
		 */
		virtual KDvoid				pauseAllEffects ( KDvoid );

		/**
		 *	@brief Resume playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		virtual KDvoid				resumeEffect ( KDuint nSoundId );

		/**
		 *	@brief Resume all playing sound effect
		 */
		virtual KDvoid				resumeAllEffects ( KDvoid );

		/**
		 *	@brief Stop playing sound effect
		 *	@param nSoundId The return value of function playEffect
		 */
		virtual KDvoid				stopEffect ( KDuint uSoundId );

		/**
		 *	@brief Stop all playing sound effects
		 */
		virtual KDvoid				stopAllEffects ( KDvoid );

		/**
		 *	@brief          preload a compressed audio file
		 *	@details        the compressed audio will be decoded to wave, then written into an internal buffer in SimpleAudioEngine
		 *	@param pszFilePath The path of the effect file
		 */
		virtual KDuint				preloadEffect ( const KDchar* szFilePath, const KDchar* szKey = KD_NULL );

		/**
		 *	@brief          unload the preloaded effect from internal buffer
		 *	@param pszFilePath The path of the effect file
		 */
		virtual KDvoid				unloadEffect ( const KDchar* szFilePath );
		virtual KDvoid				unloadEffect ( KDuint uSoundId );

		// Added By XMSoft
		KDvoid						unloadMusic ( const KDchar* szFilePath );

		KDvoid						setBackgroundMusicMusicCompletionListener ( const std::function<KDvoid()>& pFunc );
};

} // end of namespace CocosDenshion

#define NS_CD_BEGIN								namespace CocosDenshion {
#define NS_CD_END								}
#define USING_NS_CD								using namespace CocosDenshion

#endif	// __SimpleAudioEngine_h__
