/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystemQuad.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada 
 *      Copyright (c) 2009      Leonardo Kasperavičius
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCParticleSystemQuad_h__
#define __CCParticleSystemQuad_h__

#include "CCParticleSystem.h"
#include "../ccConfig.h"

NS_CC_BEGIN
    
class CCSpriteFrame;

/**
 * @addtogroup particle_nodes
 * @{
 */

/**
 *	@brief	CCParticleSystemQuad is a subclass of CCParticleSystem
 *
 *	It includes all the features of ParticleSystem.
 *
 *	Special features and Limitations:	
 *		- Particle size can be any float number.
 *		- The system can be scaled
 *		- The particles can be rotated
 *		- It supports subrects
 *		- It supports batched rendering since 1.1
 *	@since	v0.8
 */
class CCParticleSystemQuad : public CCParticleSystem
{
	public :
	             CCParticleSystemQuad ( KDvoid );
		virtual ~CCParticleSystemQuad ( KDvoid );

		/**
		 *	creates an initializes a CCParticleSystemQuad from a plist file.
		 *	This plist files can be created manually or with Particle Designer:  
		 */
		static   CCParticleSystemQuad*		create ( const KDchar* szPlistPath );

		static   CCParticleSystemQuad*		create ( KDvoid );
		static   CCParticleSystemQuad*		createWithTotalParticles ( KDuint uNumberOfParticles );

	public :

		/** initializes the indices for the vertices*/
		KDvoid				initIndices ( KDvoid );
 
		/** initializes the texture with a rectangle measured Points */
		KDvoid				initTexCoordsWithRect ( const CCRect& tRect );

		/**
		 *	Sets a new CCSpriteFrame as particle.
		 *	WARNING: this method is experimental. Use setTexture:withRect instead.
		 *	@since	v0.99.4
		 */
		KDvoid				setDisplayFrame ( CCSpriteFrame* pSpriteFrame );

		/**
		 *	Sets a new texture with a rect. The rect is in Points.
		 *	@since	v0.99.4
		 */
		KDvoid				setTextureWithRect ( CCTexture2D* pTexture, const CCRect& tRect );

		// super methods
		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );

		virtual KDvoid		setTexture ( CCTexture2D* pTexture );

		virtual KDvoid		updateQuadWithParticle ( tCCParticle* pParticle, const CCPoint& tNewPosition );

		virtual KDvoid		postStep ( KDvoid );

		virtual KDvoid		draw ( KDvoid );
		
		virtual KDvoid		setBatchNode ( CCParticleBatchNode* pBatchNode );

		virtual KDvoid		setTotalParticles ( KDuint uTotal );
		    
		/**
		 *	listen the event that coming to foreground on Android
		 */
		KDvoid				listenBackToForeground ( CCObject* pObject );

	private :

	#if CC_TEXTURE_ATLAS_USE_VAO
		KDvoid				setupVBOandVAO ( KDvoid );
	#else
		KDvoid				setupVBO ( KDvoid );
	#endif
		
		KDbool				allocMemory ( KDvoid );

	protected :
		
		ccV3F_C4B_T2F_Quad*     m_pQuads;			// quads to be rendered			
		GLushort*               m_pIndices;			// indices    		
		GLuint                  m_pBuffersVBO[2];	// 0: vertex  1: indices

	#if CC_TEXTURE_ATLAS_USE_VAO
		GLuint					m_uVAOname;
	#endif
};

// end of particle_nodes group
/// @}

NS_CC_END

#endif // __CCParticleSystemQuad_h__

