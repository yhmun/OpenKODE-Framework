/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystemQuad.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCParticleSystemQuad_h__
#define __CCParticleSystemQuad_h__

#include  "CCParticleSystem.h"

NS_CC_BEGIN

class SpriteFrame;

/**
 *	@addtogroup particle_nodes
 *	@{
 */

/** 
 *	@brief ParticleSystemQuad is a subclass of ParticleSystem
 *
 *	It includes all the features of ParticleSystem.
 *
 *	Special features and Limitations:    
 *		- Particle size can be any float number.
 *		- The system can be scaled
 *		- The particles can be rotated
 *		- It supports subrects
 *		- It supports batched rendering since 1.1
 *	@since v0.8
 */
class CC_DLL ParticleSystemQuad : public ParticleSystem
{
	public :

		/** creates a Particle Emitter */
		static ParticleSystemQuad*	create ( KDvoid );

		/** creates a Particle Emitter with a number of particles */
		static ParticleSystemQuad*	createWithTotalParticles ( KDuint uNumberOfParticles );
		
		/**
		 *	creates an initializes a ParticleSystemQuad from a plist file.
		 *	This plist files can be created manually or with Particle Designer:
		 */
		static ParticleSystemQuad*	create ( const KDchar* szPlistFile );
		
		/**
		 *	@js ctor
		 */
		ParticleSystemQuad ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleSystemQuad ( KDvoid );

		/** initializes the indices for the vertices*/
		KDvoid						initIndices ( KDvoid );

		/** initializes the texture with a rectangle measured Points */
		KDvoid						initTexCoordsWithRect ( const Rect& tRect );

		/** 
		 *	Sets a new SpriteFrame as particle.
		 *	WARNING: this method is experimental. Use setTextureWithRect instead.
		 *	@since v0.99.4
		 */
		KDvoid						setDisplayFrame ( SpriteFrame* pSpriteFrame );

		/**
		 *	Sets a new texture with a rect. The rect is in Points.
		 *	@since v0.99.4
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						setTextureWithRect ( Texture2D* pTexture, const Rect& tRect );

		/**
		 *	listen the event that coming to foreground on Android
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						listenBackToForeground ( Object* pObj );

		// Overrides
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				updateQuadWithParticle ( tParticle* pTarticle, const Point& tNewPosition ) override;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				postStep ( KDvoid ) override;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				draw ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setBatchNode ( ParticleBatchNode* pBatchNode ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setTotalParticles ( KDint nTp ) override;

	private :

		KDvoid						setupVBOandVAO ( KDvoid );
		KDvoid						setupVBO ( KDvoid );
		KDbool						allocMemory ( KDvoid );
    
	protected :

		V3F_C4B_T2F_Quad*			m_pQuads;				// quads to be rendered
		GLushort*					m_pIndices;				// indices
    
		GLuint						m_uVAOname;
    
		GLuint						m_aBuffersVBO [ 2 ];	// 0: vertex  1: indices
};

//	end of particle_nodes group
/// @}

NS_CC_END

#endif	// __CCParticleSystemQuad_h__

