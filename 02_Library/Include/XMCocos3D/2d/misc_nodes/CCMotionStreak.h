/* -----------------------------------------------------------------------------------
 *
 *      File            CCMotionStreak.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCMotionStreak_h__
#define __CCMotionStreak_h__

#include "../../ccTypes.h"
#include "../textures/CCTexture2D.h"
#include "../base_nodes/CCNode.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

/**
 *	@addtogroup misc_nodes
 *	@{
 */

/** 
 *	MotionStreak.
 *	Creates a trailing path.
 */
class CC_DLL MotionStreak : public NodeRGBA, public TextureProtocol
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture filename */
		static MotionStreak*		create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const Color3B& tColor, const KDchar* pszPath );
		
		/** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture */
		static MotionStreak*		create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const Color3B& tColor, Texture2D* pTexture );
		
		/**
		 *	@js ctor
		 */
		MotionStreak ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MotionStreak ( KDvoid );

		/** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture filename */
		KDbool					initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const Color3B& tColor, const KDchar* pszPath );

		/** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture  */
		KDbool						initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const Color3B& tColor, Texture2D* pTexture );

		/** color used for the tint */
		KDvoid						tintWithColor ( const Color3B& tColors );

		/** Remove all living segments of the ribbon */
		KDvoid						reset ( KDvoid );

		/** When fast mode is enabled, new points are added faster but with lower precision */
		inline KDbool				 isFastMode ( KDvoid ) const { return m_bFastMode; }
		inline KDvoid				setFastMode ( KDbool bFastMode ) { m_bFastMode = bFastMode; }

		inline KDbool				 isStartingPositionInitialized ( KDvoid ) const { return m_bStartingPositionInitialized; }
		inline KDvoid				setStartingPositionInitialized ( KDbool bStartingPositionInitialized ) 
		{	
			m_bStartingPositionInitialized = bStartingPositionInitialized; 
		}

		// Overrides
		virtual KDvoid				setPosition ( const Point& tPosition ) override;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				draw ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				update ( KDfloat fDelta ) override;
		virtual Texture2D*			getTexture ( KDvoid ) const override;
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setBlendFunc(const BlendFunc &blendFunc) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual const BlendFunc&	getBlendFunc ( KDvoid ) const override;
		virtual GLubyte				getOpacity ( KDvoid ) const override;
		virtual KDvoid				setOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue ) override;
		virtual KDbool				 isOpacityModifyRGB ( KDvoid ) const override;

	protected :

		KDbool						m_bFastMode;
		KDbool						m_bStartingPositionInitialized;

	private :

		/** texture used for the motion streak */
		Texture2D*					m_pTexture;
		BlendFunc					m_tBlendFunc;
		Point						m_tPositionR;

		KDfloat						m_fStroke;
		KDfloat						m_fFadeDelta;
		KDfloat						m_fMinSeg;

		KDuint						m_uMaxPoints;
		KDuint						m_uNuPoints;
		KDuint						m_uPreviousNuPoints;

		/** Pointers */
		Point*						m_pPointVertexes;
		KDfloat*					m_pPointState;

		// Opengl
		Vertex2F*					m_pVertices;
		GLubyte*					m_pColorPointer;
		Tex2F*						m_pTexCoords;
};

//	end of misc_nodes group
/// @}

NS_CC_END

#endif	// __CCMotionStreak_h__
