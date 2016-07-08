/* -----------------------------------------------------------------------------------
 *
 *      File            CCAtlasNode.h
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

#ifndef __CCAtlasNode_h__
#define __CCAtlasNode_h__

#include "CCNode.h"
#include "../CCProtocols.h"

NS_CC_BEGIN
    
/**
 * @addtogroup base_nodes
 * @{
 */

class CCTextureAtlas;

/** 
 *	@brief	CCAtlasNode is a subclass of CCNode that implements the CCRGBAProtocol and CCTextureProtocol protocol
 *
 *	It knows how to render a TextureAtlas object.
 *	If you are going to render a TextureAtlas consider subclassing CCAtlasNode (or a subclass of CCAtlasNode)
 *
 *	All features from CCNode are valid, plus the following features:
 *		- opacity and RGB colors
 */
class CCAtlasNode : public CCNodeRGBA, public CCTextureProtocol
{
	public :
		         CCAtlasNode ( KDvoid );
		virtual ~CCAtlasNode ( KDvoid );

		/** creates a CCAtlasNode  with an Atlas file the width and height of each item and the quantity of items to render */
		static   CCAtlasNode*		create ( const KDchar* szFilePath, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender );

	public :

		/** initializes an CCAtlasNode  with an Atlas file the width and height of each item and the quantity of items to render */
		virtual KDbool				initWithTileFile ( const KDchar* szFilePath, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender );

		/** initializes an CCAtlasNode  with a texture the width and height of each item measured in points and the quantity of items to render*/
		virtual KDbool				initWithTexture ( CCTexture2D* pTexture, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender );

		/** 
		 *	updates the Atlas (indexed vertex array).
		 *	Shall be overridden in subclasses
		 */
		virtual KDvoid				updateAtlasValues ( KDvoid );

		virtual KDvoid				draw ( KDvoid );

		// CC Texture protocol

		/** returns the used texture */
		virtual CCTexture2D*		getTexture ( KDvoid );

		/** sets a new texture. it will be retained */
		virtual KDvoid				setTexture ( CCTexture2D* pTexture );

		virtual KDbool				 isOpacityModifyRGB ( KDvoid );
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB );

		virtual const ccColor3B&	getColor ( KDvoid );
		virtual KDvoid				setColor ( const ccColor3B& tColor );

		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		CC_PROPERTY ( CCTextureAtlas*, m_pTextureAtlas, TextureAtlas );
		
		CC_PROPERTY ( ccBlendFunc, m_tBlendFunc, BlendFunc );
		
		// quads to draw
		CC_PROPERTY ( KDuint, m_uQuadsToDraw, QuadsToDraw );

	private :

		KDvoid						calculateMaxItems ( KDvoid );
		KDvoid						updateBlendFunc ( KDvoid );
		KDvoid						updateOpacityModifyRGB ( KDvoid );

		friend class CCDirector;

		KDvoid						setIgnoreContentScaleFactor ( KDbool bIgnoreContentScaleFactor );

	protected :

		//! chars per row
		KDuint						m_uItemsPerRow;

		//! chars per column
		KDuint						m_uItemsPerColumn;

		//! width of each char
		KDuint						m_uItemWidth;

		//! height of each char
		KDuint						m_uItemHeight;

		ccColor3B					m_tColorUnmodified;

		// protocol variables
		KDbool						m_bIsOpacityModifyRGB;

		// color uniform
		GLint						m_nUniformColor;

		// This varible is only used for CCLabelAtlas FPS display. So plz don't modify its value.
		KDbool						m_bIgnoreContentScaleFactor;    
};

// end of base_node group
/// @}

NS_CC_END

#endif // __CCAtlasNode_h__


