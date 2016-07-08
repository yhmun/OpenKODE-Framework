/* -----------------------------------------------------------------------------------
 *
 *      File            CCMotionStreak.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      ForzeField Studios S.L.
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

#ifndef __CCMotionStreak_h__
#define __CCMotionStreak_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"
#include "../textures/CCTexture2D.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

/**
 * @addtogroup misc_nodes
 * @{
 */

/** 
 *	MotionStreak.
 *	Creates a trailing path.
 */
class CCMotionStreak : public CCNodeRGBA, public CCTextureProtocol
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
		         CCMotionStreak ( KDvoid );
		virtual ~CCMotionStreak ( KDvoid );

		/** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture filename */
		static   CCMotionStreak*	create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, const KDchar* szPath );

		/** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture */
		static   CCMotionStreak*	create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, CCTexture2D* pTexture );

	public :

		/** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture filename */
		virtual KDbool				initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, const KDchar* szPath );

		/** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture  */
		virtual KDbool				initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, CCTexture2D* pTexture );

		/** color used for the tint */
		KDvoid						tintWithColor ( const ccColor3B& tColor );

		/** Remove all living segments of the ribbon */
		KDvoid						reset ( KDvoid );

		/** Override super methods */
		virtual KDvoid				setPosition ( const CCPoint& tPosition );
	
		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				update ( KDfloat fDelta );

		/* Implement interfaces */
		virtual CCTexture2D*		getTexture ( KDvoid );
		virtual KDvoid				setTexture ( CCTexture2D* pTexture );
		
		virtual KDvoid				setBlendFunc ( ccBlendFunc tBlendFunc );
		virtual ccBlendFunc			getBlendFunc ( KDvoid );

		virtual GLubyte				getOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue );
		virtual KDbool				 isOpacityModifyRGB ( KDvoid );

		/** When fast mode is enabled, new points are added faster but with lower precision */
		inline  KDbool				 isFastMode ( KDvoid ) { return m_bFastMode; }
		inline  KDvoid				setFastMode ( KDbool bFastMode ) { m_bFastMode = bFastMode; }

		inline  KDbool				 isStartingPositionInitialized ( KDvoid ) { return m_bStartingPositionInitialized; }
		inline  KDvoid				setStartingPositionInitialized ( KDbool bStartingPositionInitialized ) 
		{ 
			m_bStartingPositionInitialized = bStartingPositionInitialized; 
		}

	protected :

		KDbool						m_bFastMode;
		KDbool						m_bStartingPositionInitialized;

		/** texture used for the motion streak */
		CCTexture2D*				m_pTexture;
		ccBlendFunc					m_tBlendFunc;
		CCPoint						m_tPositionR;

		KDfloat						m_fStroke;
		KDfloat						m_fFadeDelta;
		KDfloat						m_fMinSeg;

		KDuint						m_uMaxPoints;
		KDuint						m_uNuPoints;
		KDuint						m_uPreviousNuPoints;

		/** Pointers */
		CCPoint*					m_pPointVertexes;
		KDfloat*					m_pPointState;

		// Opengl
		ccVertex2F*					m_pVertices;
		GLubyte*					m_pColorPointer;
		ccTex2F*					m_pTexCoords;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif // __CCMotionStreak_h__
