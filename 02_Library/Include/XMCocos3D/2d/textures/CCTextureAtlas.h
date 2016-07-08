/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureAtlas.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __CCTextureAtlas_h__
#define __CCTextureAtlas_h__

#include "../../ccTypes.h"
#include "../../base/CCObject.h"
#include "../../ccConfig.h"
#include <string>

NS_CC_BEGIN

class Texture2D;

/**
 *	@addtogroup textures
 *	@{
 */

/** 
 *	@brief A class that implements a Texture Atlas.
 *	Supported features:
 *	The atlas file can be a PVRTC, PNG or any other format supported by Texture2D
 *		- Quads can be updated in runtime
 *		- Quads can be added in runtime
 *		- Quads can be removed in runtime
 *		- Quads can be re-ordered in runtime
 *	The TextureAtlas capacity can be increased or decreased in runtime
 *	OpenGL component: V3F, C4B, T2F.
 *		- The quads are rendered using an OpenGL ES VBO.
 *		- To render the quads using an interleaved vertex array list, you should modify the ccConfig.h file 
 */
class CC_DLL TextureAtlas : public Object 
{
	public :

		/**
		 *	creates a TextureAtlas with an filename and with an initial capacity for Quads.
		 *	The TextureAtlas capacity can be increased in runtime.
		 */
		static TextureAtlas*	create ( const KDchar* pFile, KDint32 nCapacity );

		/**
		 *	creates a TextureAtlas with a previously initialized Texture2D object, and
		 *	with an initial capacity for n Quads.
		 *	The TextureAtlas capacity can be increased in runtime.
		 */
		static TextureAtlas*	createWithTexture ( Texture2D* pTexture, KDint32 nCapacity );
		
		/**
		 *	@js ctor
		 */
		TextureAtlas ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TextureAtlas ( KDvoid );

		/**
		 *	initializes a TextureAtlas with a filename and with a certain capacity for Quads.
		 *	The TextureAtlas capacity can be increased in runtime.
		 *
		 *	WARNING: Do not reinitialize the TextureAtlas because it will leak memory (issue #706)
		 */
		KDbool					initWithFile ( const KDchar* pFile, KDint32 nCapacity );

		/** 
		 *	initializes a TextureAtlas with a previously initialized Texture2D object, and
		 *	with an initial capacity for Quads. 
		 *	The TextureAtlas capacity can be increased in runtime.
		 *
		 *	WARNING: Do not reinitialize the TextureAtlas because it will leak memory (issue #706)
		 */
		KDbool					initWithTexture ( Texture2D* pTexture, KDint32 nCapacity );

		/** 
		 *	updates a Quad (texture, vertex and color) at a certain index
		 *	index must be between 0 and the atlas capacity - 1
		 *	@since v0.8
		 */
		KDvoid					updateQuad ( V3F_C4B_T2F_Quad* pQuad, KDint32 nIndex );

		/**
		 *	Inserts a Quad (texture, vertex and color) at a certain index
		 *	index must be between 0 and the atlas capacity - 1
		 *	@since v0.8
		 */
		KDvoid					insertQuad ( V3F_C4B_T2F_Quad* pQuad, KDint32 nIndex );

		/** 
		 *	Inserts a c array of quads at a given index
		 *	index must be between 0 and the atlas capacity - 1
		 *	this method doesn't enlarge the array when amount + index > totalQuads
		 *	@since v1.1
		 */
		KDvoid					insertQuads ( V3F_C4B_T2F_Quad* pQuads, KDint32 nIndex, KDint32 nAmount );

		/**
		 *	Removes the quad that is located at a certain index and inserts it at a new index
		 *	This operation is faster than removing and inserting in a quad in 2 different steps
		 *	@since v0.7.2
		 */
		KDvoid					insertQuadFromIndex ( KDint32 nFromIndex, KDint32 nNewIndex );

		/** 
		 *	removes a quad at a given index number.
		 *	The capacity remains the same, but the total number of quads to be drawn is reduced in 1
		 *	@since v0.7.2
		 */
		KDvoid					removeQuadAtIndex ( KDint32 nIndex );

		/**
		 *	removes a amount of quads starting from index
		 *	@since 1.1
		 */
		KDvoid					removeQuadsAtIndex ( KDint32 nIndex, KDint32 nAmount );

		/**
		 *	removes all Quads.
		 *	The TextureAtlas capacity remains untouched. No memory is freed.
		 *	The total number of quads to be drawn will be 0
		 *	@since v0.7.2
		 */
		KDvoid					removeAllQuads ( KDvoid );

		/**
		 *	resize the capacity of the TextureAtlas.
		 *	The new capacity can be lower or higher than the current one
		 *	It returns true if the resize was successful.
		 *	If it fails to resize the capacity it will return false with a new capacity of 0.
		 */
		KDbool					resizeCapacity ( KDint32 nCapacity );

		/**
		 *	Used internally by ParticleBatchNode
		 *	don't use this unless you know what you're doing
		 *	@since 1.1
		 */
		KDvoid					increaseTotalQuadsWith ( KDint32 nAmount );

		/** 
		 *	Moves an amount of quads from oldIndex at newIndex
		 *	@since v1.1
		 */
		KDvoid					moveQuadsFromIndex ( KDint32 nOldIndex, KDint32 nAmount, KDint32 nNewIndex );

		/**
		 *	Moves quads from index till totalQuads to the newIndex
		 *	Used internally by ParticleBatchNode
		 *	This method doesn't enlarge the array if newIndex + quads to be moved > capacity
		 *	@since 1.1
		 */
		KDvoid					moveQuadsFromIndex ( KDint32 nIndex, KDint32 nNewIndex );

		/**
		 *	Ensures that after a realloc quads are still empty
		 *	Used internally by ParticleBatchNode
		 *	@since 1.1
		 */
		KDvoid					fillWithEmptyQuadsFromIndex ( KDint32 nIndex, KDint32 nAmount );

		/** 
		 *	draws n quads
		 *	n can't be greater than the capacity of the Atlas
		 */
		KDvoid					drawNumberOfQuads ( KDint32 n );

		/** 
		 *	draws n quads from an index (offset).
		 *	n + start can't be greater than the capacity of the atlas
		 *
		 *	@since v1.0
		 */
		KDvoid					drawNumberOfQuads ( KDint32 nNumberOfQuads, KDint32 nStart );

		/** 
		 *	draws all the Atlas's Quads
		 */
		KDvoid					drawQuads ( KDvoid );

		/**
		 *	listen the event that coming to foreground on Android
		 */
		KDvoid					listenBackToForeground ( Object* pObj );

		/** whether or not the array buffer of the VBO needs to be updated*/
		inline KDbool			isDirty ( KDvoid ) { return m_bDirty; }

		/** specify if the array buffer of the VBO needs to be updated */
		inline KDvoid			setDirty ( KDbool bDirty ) { m_bDirty = bDirty; }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*			description ( KDvoid ) const;

		/** Gets the quantity of quads that are going to be drawn */
		KDint32					getTotalQuads ( KDvoid ) const;
    
		/** Gets the quantity of quads that can be stored with the current texture atlas size */
		KDint32					getCapacity ( KDvoid ) const;
    
		/** Gets the texture of the texture atlas */
		Texture2D*				getTexture ( KDvoid ) const;
    
		/** Sets the texture for the texture atlas */
		KDvoid					setTexture ( Texture2D* pTexture );
    
		/** Gets the quads that are going to be rendered */
		V3F_C4B_T2F_Quad*		getQuads ( KDvoid );
    
		/** Sets the quads that are going to be rendered */
		KDvoid					setQuads ( V3F_C4B_T2F_Quad* pQuads );
    
	private :

		KDvoid					setupIndices ( KDvoid );
		KDvoid					mapBuffers ( KDvoid );
		KDvoid					setupVBOandVAO ( KDvoid );
		KDvoid					setupVBO ( KDvoid );

	protected :

		GLushort*				m_pIndices;
		GLuint					m_uVAOname;
		GLuint					m_aBuffersVBO [ 2 ];			// 0: vertex  1: indices
		KDbool					m_bDirty;						// indicates whether or not the array buffer of the VBO needs to be updated
		/** quantity of quads that are going to be drawn */
		KDint32					m_nTotalQuads;
		/** quantity of quads that can be stored with the current texture atlas size */
		KDint32					m_nCapacity;
		/** Texture of the texture atlas */
		Texture2D*				m_pTexture;
		/** Quads that are going to be rendered */
		V3F_C4B_T2F_Quad*		m_pQuads;
};

//	end of textures group
/// @}

NS_CC_END

#endif	// __CCTextureAtlas_h__


