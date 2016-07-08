/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureAtlas.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCTextureAtlas_h__
#define __CCTextureAtlas_h__

#include "../cocoa/CCObject.h"
#include "../ccTypes.h"
#include "../ccConfig.h"

NS_CC_BEGIN
    
class CCTexture2D;

/**
 * @addtogroup textures
 * @{
 */

/** 
 *	@brief	A class that implements a Texture Atlas.
 *	Supported features:
 *	The atlas file can be a PVRTC, PNG or any other format supported by Texture2D
 *		- Quads can be updated in runtime
 *		- Quads can be added in runtime
 *		- Quads can be removed in runtime
 *		-  Quads can be re-ordered in runtime
 *		- The TextureAtlas capacity can be increased or decreased in runtime
 *		- OpenGL component: V3F, C4B, T2F.
 *	The quads are rendered using an OpenGL ES VBO.
 *	To render the quads using an interleaved vertex array list, you should modify the ccConfig.h file 
 */
class CCTextureAtlas : public CCObject 
{
	public :
		         CCTextureAtlas ( KDvoid );
		virtual ~CCTextureAtlas ( KDvoid );	
    
	public :
    
		/**
		 *	creates a TextureAtlas with an filename and with an initial capacity for Quads.
		 *	The TextureAtlas capacity can be increased in runtime.
		 */
		static  CCTextureAtlas*		create ( const KDchar* szFilePath, KDuint uCapacity );

		/** 
		 *	creates a TextureAtlas with a previously initialized Texture2D object, and
		 *	with an initial capacity for n Quads. 
		 *	The TextureAtlas capacity can be increased in runtime.
		 */
		static  CCTextureAtlas*		createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity );

	public :

		const KDchar*		description ( KDvoid );

		/**
		 *	initializes a TextureAtlas with a filename and with a certain capacity for Quads.
		 *	The TextureAtlas capacity can be increased in runtime.
		 *
		 *	WARNING: Do not reinitialize the TextureAtlas because it will leak memory (issue #706)
		 */
		virtual KDbool		initWithFile ( const KDchar* szFilePath, KDuint uCapacity );

		/** 
		 *	initializes a TextureAtlas with a previously initialized Texture2D object, and
		 *	with an initial capacity for Quads. 
		 *	The TextureAtlas capacity can be increased in runtime.
		 *
		 *	WARNING: Do not reinitialize the TextureAtlas because it will leak memory (issue #706)
		 */
		virtual KDbool		initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity );

		/**
		 *	updates a Quad (texture, vertex and color) at a certain index
		 *	index must be between 0 and the atlas capacity - 1
		 *	@since	v0.8
		 */
		KDvoid				updateQuad ( ccV3F_C4B_T2F_Quad* pQuad, KDuint uIndex );

		/** 
		 *	Inserts a Quad (texture, vertex and color) at a certain index
		 *	index must be between 0 and the atlas capacity - 1
		 *	@since	v0.8
		 */
		KDvoid				insertQuad ( ccV3F_C4B_T2F_Quad* pQuad, KDuint uIndex );

		/** 
		 *	Inserts a c array of quads at a given index
		 *	index must be between 0 and the atlas capacity - 1
		 *	this method doesn't enlarge the array when amount + index > totalQuads
		 *	@since v1.1
		 */
		KDvoid				insertQuads ( ccV3F_C4B_T2F_Quad* pQuads, KDuint uIndex, KDuint uAmount );

		/**
		 *	Removes the quad that is located at a certain index and inserts it at a new index
		 *	This operation is faster than removing and inserting in a quad in 2 different steps
		 *	@since	v0.7.2
		 */
		KDvoid				insertQuadFromIndex ( KDuint uOldIndex, KDuint uNewIndex );

		/**
		 *	removes a quad at a given index number.
		 *	The capacity remains the same, but the total number of quads to be drawn is reduced in 1
		 *	@since	v0.7.2
		 */
		KDvoid				removeQuadAtIndex ( KDuint uIndex );

		/**
		 *	removes a amount of quads starting from index
		 *	@since 1.1
		 */
		KDvoid				removeQuadsAtIndex ( KDuint uIndex, KDuint uAmount );

		/** 
		 *	removes all Quads.
		 *	The TextureAtlas capacity remains untouched. No memory is freed.
		 *	The total number of quads to be drawn will be 0
		 *	@since	v0.7.2
		 */
		KDvoid				removeAllQuads ( KDvoid );

		/**
		 *	resize the capacity of the CCTextureAtlas.
		 *	The new capacity can be lower or higher than the current one
		 *	It returns YES if the resize was successful.
		 *	If it fails to resize the capacity it will return NO with a new capacity of 0.
		 */
		KDbool				resizeCapacity ( KDuint uCapacity );

		/**
		 *	Used internally by CCParticleBatchNode
		 *	don't use this unless you know what you're doing
		 *	@since 1.1
		 */
		KDvoid				increaseTotalQuadsWith ( KDuint uAmount );

		/**
		 *	Moves an amount of quads from oldIndex at newIndex
		 *	@since v1.1
		 */
		KDvoid				moveQuadsFromIndex ( KDuint uOldIndex, KDuint uAmount, KDuint uNewIndex );

		/**
		 *	Moves quads from index till totalQuads to the newIndex
		 *	Used internally by CCParticleBatchNode
		 *	This method doesn't enlarge the array if newIndex + quads to be moved > capacity
		 *	@since 1.1
		 */
		KDvoid				moveQuadsFromIndex ( KDuint uIndex, KDuint uNewIndex );

		/**
		 *	Ensures that after a realloc quads are still empty
		 *	Used internally by CCParticleBatchNode
		 *	@since 1.1
		 */
		KDvoid				fillWithEmptyQuadsFromIndex ( KDuint uIndex, KDuint uAmount );

		/**
		 *	draws n quads
		 *	n can't be greater than the capacity of the Atlas
		 */
		KDvoid				drawNumberOfQuads ( KDuint uNumber );

		/** 
		 *	draws n quads from an index (offset).
		 *	n + start can't be greater than the capacity of the atlas
		 *
		 *	@since	v1.0
		 */
		KDvoid				drawNumberOfQuads ( KDuint uNumber, KDuint uStart );

		/** draws all the Atlas's Quads */
		KDvoid				drawQuads ( KDvoid );

		/**
		 *	listen the event that coming to foreground on Android
		 */
		KDvoid				listenBackToForeground ( CCObject* pObject );

		/** whether or not the array buffer of the VBO needs to be updated*/
		inline KDbool		isDirty ( KDvoid ) { return m_bDirty; }
		
		/** specify if the array buffer of the VBO needs to be updated */
		inline KDvoid		setDirty ( KDbool bDirty ) { m_bDirty = bDirty; }

	public :

		/** quantity of quads that are going to be drawn */
		CC_PROPERTY_READONLY ( KDuint, m_uTotalQuads, TotalQuads );

		/** quantity of quads that can be stored with the current texture atlas size */
		CC_PROPERTY_READONLY ( KDuint, m_uCapacity, Capacity );

		/** Texture of the texture atlas */
		CC_PROPERTY ( CCTexture2D*, m_pTexture, Texture );

		/** Quads that are going to be rendered */
		CC_PROPERTY ( ccV3F_C4B_T2F_Quad*, m_pQuads, Quads );

	private :

		KDvoid				setupIndices ( KDvoid );
		KDvoid				mapBuffers ( KDvoid );

	#if CC_TEXTURE_ATLAS_USE_VAO
		KDvoid				setupVBOandVAO ( KDvoid );
	#else
		KDvoid				setupVBO ( KDvoid );
	#endif

	protected :

	#if CC_TEXTURE_ATLAS_USE_VAO
		GLuint				m_uVAOname;
	#endif

		GLushort*			m_pIndices;
		GLuint				m_pBuffersVBO [ 2 ];	// 0: vertex  1: indices		
		KDbool				m_bDirty;				// indicates whether or not the array buffer of the VBO needs to be updated
};

// end of textures group
/// @}

NS_CC_END

#endif	// __CCTextureAtlas_h__

