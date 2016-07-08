/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleBatchNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Matt Oswald
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *      Copyright (c) 2011      Marco Tillemans
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

#ifndef __CCParticleBatchNode_h__
#define __CCParticleBatchNode_h__

#include "../base_nodes/CCNode.h"

NS_CC_BEGIN

class Texture2D;
class TextureAtlas;
class ParticleSystem;

/**
 *	@addtogroup particle_nodes
 *	@{
 */

#define kParticleDefaultCapacity	500

/** 
 *	ParticleBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
 *	(often known as "batch draw").
 *
 *	A ParticleBatchNode can reference one and only one texture (one image file, one texture atlas).
 *	Only the ParticleSystems that are contained in that texture can be added to the SpriteBatchNode.
 *	All ParticleSystems added to a SpriteBatchNode are drawn in one OpenGL ES draw call.
 *	If the ParticleSystems are not added to a ParticleBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
 *
 *
 *	Limitations:
 *		- At the moment only ParticleSystemQuad is supported
 *		- All systems need to be drawn with the same parameters, blend function, aliasing, texture
 *
 *	Most efficient usage
 *		- Initialize the ParticleBatchNode with the texture and enough capacity for all the particle systems
 *		- Initialize all particle systems and add them as child to the batch node
 *	@since v1.1
 */

class CC_DLL ParticleBatchNode : public Node, public TextureProtocol
{
	public :

		/** initializes the particle system with Texture2D, a capacity of particles, which particle system to use */
		static ParticleBatchNode*	createWithTexture ( Texture2D* pTex,  KDuint uCapacity = kParticleDefaultCapacity );

		/** initializes the particle system with the name of a file on disk (for a list of supported formats look at the Texture2D class), a capacity of particles */
		static ParticleBatchNode*	create ( const KDchar* pFileImage,  KDuint uCapacity = kParticleDefaultCapacity );
		
		/**
		 *	@js ctor
		 */
		ParticleBatchNode ( KDvoid );
		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~ParticleBatchNode ( KDvoid );

		/** initializes the particle system with Texture2D, a capacity of particles */
		KDbool						initWithTexture ( Texture2D* pTex, KDuint uCapacity );

		/** initializes the particle system with the name of a file on disk (for a list of supported formats look at the Texture2D class), a capacity of particles */
		KDbool						initWithFile ( const KDchar* pFileImage, KDuint uCapacity );

		/** Inserts a child into the ParticleBatchNode */
		KDvoid						insertChild ( ParticleSystem* pSystem, KDint nIndex );

		KDvoid						removeChildAtIndex ( KDuint uIndex, KDbool bDoCleanup );
		KDvoid						removeAllChildrenWithCleanup ( KDbool bDoCleanup );

		/** disables a particle by inserting a 0'd quad into the texture atlas */
		KDvoid						disableParticle ( KDuint uParticleIndex );

		/** Gets the texture atlas used for drawing the quads */
		inline TextureAtlas*		getTextureAtlas ( KDvoid ) const { return m_pTextureAtlas; }
    
		/** Sets the texture atlas used for drawing the quads */
		inline KDvoid				setTextureAtlas ( TextureAtlas* pAtlas ) { m_pTextureAtlas = pAtlas; }
    
		// Overrides
		KDvoid						visit ( KDvoid );
		virtual KDvoid				addChild ( Node * pChild ) override;
		virtual KDvoid				addChild ( Node * pChild, KDint zOrder ) override;
		virtual KDvoid				addChild ( Node * pChild, KDint zOrder, KDint nTag ) override;
		virtual KDvoid				removeChild ( Node* pChild, KDbool bCleanup ) override;
		virtual KDvoid				reorderChild ( Node* pChild, KDint zOrder ) override;
		virtual KDvoid				draw ( KDvoid ) override;
		virtual Texture2D*			getTexture ( KDvoid ) const override;
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;
		
		/**
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var setBlendFunc(var src, var dst)
		 *	@endcode
		 *	@lua NA
		 */
		virtual KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual const BlendFunc&	getBlendFunc ( KDvoid ) const override;

	private :

		KDvoid						updateAllAtlasIndexes ( KDvoid );
		KDvoid						increaseAtlasCapacityTo ( KDuint uQuantity );
		KDuint						searchNewPositionInChildrenForZ ( KDint z );
		KDvoid						getCurrentIndex ( KDuint* pOldIndex, KDuint* pNewIndex, Node* pChild, KDint z );
		KDuint						addChildHelper ( ParticleSystem* pChild, KDint z, KDint nTag );
		KDvoid						updateBlendFunc ( KDvoid );
		/** the texture atlas used for drawing the quads */
		TextureAtlas*				m_pTextureAtlas;

	private :

		/** the blend function used for drawing the quads */
		BlendFunc					m_tBlendFunc;
};

//	end of particle_nodes group
/// @}

NS_CC_END

#endif	// __CCParticleBatchNode_h__

