/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleBatchNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCParticleBatchNode_h__
#define __CCParticleBatchNode_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"

NS_CC_BEGIN

class CCTexture2D;
class CCTextureAtlas;
class CCParticleSystem;

/**
 * @addtogroup particle_nodes
 * @{
 */

#define kCCParticleDefaultCapacity	500

/** 
 *	CCParticleBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
 *	(often known as "batch draw").
 *
 *	A CCParticleBatchNode can reference one and only one texture (one image file, one texture atlas).
 *	Only the CCParticleSystems that are contained in that texture can be added to the CCSpriteBatchNode.
 *	All CCParticleSystems added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call.
 *	If the CCParticleSystems are not added to a CCParticleBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
 *
 *
 *	Limitations:
 *		- At the moment only CCParticleSystemQuad is supported
 *		- All systems need to be drawn with the same parameters, blend function, aliasing, texture
 *
 *	Most efficient usage
 *		- Initialize the ParticleBatchNode with the texture and enough capacity for all the particle systems
 *		- Initialize all particle systems and add them as child to the batch node
 *	@since v1.1
 */

class CCParticleBatchNode : public CCNode, public CCTextureProtocol
{
	public :

				 CCParticleBatchNode ( KDvoid );
		virtual ~CCParticleBatchNode ( KDvoid );

		/** initializes the particle system with CCTexture2D, a capacity of particles, which particle system to use */
		static   CCParticleBatchNode*	createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity = kCCParticleDefaultCapacity );

		/** initializes the particle system with the name of a file on disk (for a list of supported formats look at the CCTexture2D class), a capacity of particles */
		static   CCParticleBatchNode*	create ( const KDchar* szFilePath, KDuint uCapacity = kCCParticleDefaultCapacity );

	public :

		/** initializes the particle system with CCTexture2D, a capacity of particles */
		virtual KDbool			initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity = kCCParticleDefaultCapacity );

		/** initializes the particle system with the name of a file on disk (for a list of supported formats look at the CCTexture2D class), a capacity of particles */
		virtual KDbool			initWithFile ( const KDchar* szFilePath, KDuint uCapacity = kCCParticleDefaultCapacity );

		/** Add a child into the CCParticleBatchNode */
		virtual KDvoid			addChild ( CCNode* pChild);
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );

		/** Inserts a child into the CCParticleBatchNode */
		KDvoid					insertChild ( CCParticleSystem* pSystem, KDuint uIndex );

		/** remove child from the CCParticleBatchNode */
		virtual KDvoid			removeChild  ( CCNode* pChild, KDbool bCleanup );
		virtual KDvoid			reorderChild ( CCNode* pChild, KDint nZOrder );
		KDvoid					removeChildAtIndex ( KDuint index, KDbool bCleanup );
		KDvoid					removeAllChildrenWithCleanup ( KDbool bCleanup );

		/** disables a particle by inserting a 0'd quad into the texture atlas */
		KDvoid					disableParticle ( KDuint uParticleIndex );

		virtual KDvoid			draw ( KDvoid );

		// returns the used texture
		virtual CCTexture2D*	getTexture ( KDvoid );
		
		// sets a new texture. it will be retained
		virtual KDvoid			setTexture ( CCTexture2D* pTexture );
		virtual KDvoid			setBlendFunc ( ccBlendFunc tBlendFunc );
		
		// returns the blending function used for the texture
		virtual ccBlendFunc		getBlendFunc ( KDvoid );

		KDvoid					visit ( KDvoid );

	private :

		KDvoid					updateAllAtlasIndexes ( KDvoid );
		KDvoid					increaseAtlasCapacityTo ( KDuint nQuantity );
		KDuint					searchNewPositionInChildrenForZ ( KDint nZOrder );
		KDvoid					getCurrentIndex ( KDuint* pOldIndex, KDuint* pNewIndex, CCNode* pChild, KDint nZOrder );
		KDuint					addChildHelper ( CCParticleSystem* pChild, KDint nZOrder, KDint nTag );
		KDvoid					updateBlendFunc ( KDvoid );

		/** the texture atlas used for drawing the quads */
		CC_SYNTHESIZE ( CCTextureAtlas*, m_pTextureAtlas, TextureAtlas );

	private :
		/** the blend function used for drawing the quads */
		ccBlendFunc				m_tBlendFunc;
};

// end of particle_nodes group
/// @}

NS_CC_END

#endif // __CCParticleBatchNode_h__

