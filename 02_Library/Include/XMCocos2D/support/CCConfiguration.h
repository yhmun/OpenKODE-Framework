/* -----------------------------------------------------------------------------------
 *
 *      File            CCConfiguration.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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

#ifndef __CCConfiguration_h__
#define __CCConfiguration_h__

#include "../cocoa/CCString.h"
#include "../cocoa/CCDictionary.h"

NS_CC_BEGIN

typedef enum _ccConfigurationType
{
	ConfigurationError	,
	ConfigurationString	,
	ConfigurationInt	,
	ConfigurationDouble	,
	ConfigurationBoolean,
} ccConfigurationType;

/**
 * @addtogroup global
 * @{
 */

/**
 *	@brief	CCConfiguration contains some openGL variables
 *	@since	v0.99.0
 */
class CCConfiguration : public CCObject
{
	protected :
				 CCConfiguration ( KDvoid );
		virtual ~CCConfiguration ( KDvoid );

		virtual KDbool				init ( KDvoid );

	public :

		/** returns a shared instance of the CCConfiguration */
		static  CCConfiguration*	sharedConfiguration ( KDvoid );

		/** purge the shared instance of CCConfiguration */
		static  KDvoid				purgeConfiguration ( KDvoid );

	public :

		/** OpenGL Max texture size. */
		KDint						getMaxTextureSize ( KDvoid ) const;

		/** OpenGL Max Modelview Stack Depth */
		KDint						getMaxModelviewStackDepth ( KDvoid ) const;

		/** 
		 *	returns the maximum texture units
		 *	@since v2.0.0
		 */
		KDint						getMaxTextureUnits ( KDvoid ) const;

		/**
		 *	Whether or not the GPU supports NPOT (Non Power Of Two) textures.
		 *	NPOT textures have the following limitations:
		 *		- They can't have mipmaps
		 *		- They only accept GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}
		 *
		 *	@since	v0.99.2
		 */
		KDbool						supportsNPOT ( KDvoid ) const;

		/** Whether or not PVR Texture Compressed is supported */
		KDbool						supportsPVRTC ( KDvoid ) const;

		/**
		 *	Whether or not BGRA8888 textures are supported.
		 *	@since	v0.99.2
		 */
		KDbool						supportsBGRA8888 ( KDvoid ) const; 

		/**
		 *	Whether or not glDiscardFramebufferEXT is supported
		 *	@since	v0.99.2
		 */
		KDbool						supportsDiscardFramebuffer ( KDvoid ) const;

		/** 
		 *	Whether or not shareable VAOs are supported.
		 *	@since v2.0.0
		 */
		KDbool						supportsShareableVAO ( KDvoid ) const;

		/** returns whether or not an OpenGL is supported */
		KDbool						checkForGLExtension ( const KDchar* szSearchName ) const;

		/**
		 *	returns the value of a given key as a string.
		 *	If the key is not found, it will return the default value 
		 */
		const KDchar*				getCString ( const KDchar* szKey, const KDchar* szDefaultValue = KD_NULL ) const;

		/**
		 *	returns the value of a given key as a boolean.
		 *	If the key is not found, it will return the default value 
		 */
		KDbool						getBool ( const KDchar* szKey, KDbool bDefaultValue = KD_FALSE ) const;

		/**
		 *	returns the value of a given key as a double.
		 *	If the key is not found, it will return the default value 
		 */
		KDdouble					getNumber ( const KDchar* szKey, KDdouble dDefaultValue = 0.0 ) const;

		/** returns the value of a given key as a double */
		CCObject*					getObject ( const KDchar* szKey ) const;

		/** sets a new key/value pair  in the configuration dictionary */
		KDvoid						setObject ( const KDchar* szKey, CCObject* pValue );

		/** dumps the current configuration on the console */
		KDvoid						dumpInfo ( KDvoid ) const;

		/** gathers OpenGL / GPU information */
		KDvoid						gatherGPUInfo ( KDvoid );

		/** Loads a config file. If the keys are already present, then they are going to be replaced. Otherwise the new keys are added. */
		KDvoid						loadConfigFile ( const KDchar* szFilename );

	protected :

		KDint						m_nMaxTextureSize;
		KDint						m_nMaxModelviewStackDepth;
		KDbool						m_bSupportsPVRTC;
		KDbool						m_bSupportsNPOT;
		KDbool						m_bSupportsBGRA8888;
		KDbool						m_bSupportsDiscardFramebuffer;
		KDbool						m_bSupportsShareableVAO;
		KDint						m_nMaxSamplesAllowed;
		KDint						m_nMaxTextureUnits;
		KDchar*						m_pGlExtensions;
		CCDictionary*				m_pValueDict;
};

// end of global group
/// @}

NS_CC_END

#endif	// __CCConfiguration_h__