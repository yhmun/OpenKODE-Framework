/* -----------------------------------------------------------------------------------
 *
 *      File            CCAtlasNode.h
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

#ifndef __CCAtlasNode_h__
#define __CCAtlasNode_h__

#include "CCNode.h"
#include "../../CCProtocols.h"

NS_CC_BEGIN

/**
 *	@addtogroup base_nodes
 *	@{
 */

class TextureAtlas;

/** 
 *	@brief AtlasNode is a subclass of Node that implements the RGBAProtocol and TextureProtocol protocol
 *
 *	It knows how to render a TextureAtlas object.
 *	If you are going to render a TextureAtlas consider subclassing AtlasNode (or a subclass of AtlasNode)
 *
 *	All features from Node are valid, plus the following features:
 *		- opacity and RGB colors
 */
class CC_DLL AtlasNode : public NodeRGBA, public TextureProtocol
{    
	public :

		/** creates a AtlasNode  with an Atlas file the width and height of each item and the quantity of items to render*/
		static AtlasNode*		create ( const std::string& sTile, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender );

		/**
		 *	@js ctor
		 */
		AtlasNode ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~AtlasNode ( KDvoid );

		/** initializes an AtlasNode  with an Atlas file the width and height of each item and the quantity of items to render*/
		KDbool						initWithTileFile ( const std::string& sTile, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender );

		/** initializes an AtlasNode  with a texture the width and height of each item measured in points and the quantity of items to render*/
		KDbool						initWithTexture ( Texture2D* pTexture, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender );
    
		/** 
		 *	updates the Atlas (indexed vertex array).
		 *	Shall be overridden in subclasses
		 */
		virtual KDvoid				updateAtlasValues ( KDvoid );

		KDvoid						setTextureAtlas ( TextureAtlas* pTextureAtlas );
		TextureAtlas*				getTextureAtlas ( KDvoid ) const;
    
		KDvoid						setQuadsToDraw ( KDint32 nQuadsToDraw );
		KDint32						getQuadsToDraw ( KDvoid ) const;

    
		// Overrides
		virtual KDvoid				draw ( KDvoid ) override;
		virtual Texture2D*			getTexture ( KDvoid ) const override;
		virtual KDvoid				setTexture ( Texture2D* pTexture ) override;
		virtual KDbool				isOpacityModifyRGB ( KDvoid ) const override;
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB ) override;
		virtual const Color3B&		getColor ( KDvoid ) const override;
		virtual KDvoid				setColor ( const Color3B& tColor ) override;
		virtual KDvoid				setOpacity ( GLubyte cOpacity ) override;
		
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

		KDvoid						calculateMaxItems ( KDvoid );
		KDvoid						updateBlendFunc ( KDvoid );
		KDvoid						updateOpacityModifyRGB ( KDvoid );
    
		friend class Director;

		KDvoid						setIgnoreContentScaleFactor ( KDbool bIgnoreContentScaleFactor );

	protected :

		//! chars per row
		KDint32						m_nItemsPerRow;
		//! chars per column
		KDint32						m_nItemsPerColumn;

		//! width of each char
		KDint32						m_nItemWidth;
		//! height of each char
		KDint32						m_nItemHeight;
    
		Color3B						m_tColorUnmodified;
    
		TextureAtlas*				m_pTextureAtlas;

		// protocol variables
		KDbool						m_bIsOpacityModifyRGB;
		BlendFunc					m_tBlendFunc;
		// quads to draw
		KDint32						m_nQuadsToDraw;
		// color uniform
		GLint						m_nUniformColor;
		// This varible is only used for LabelAtlas FPS display. So plz don't modify its value.
		KDbool						m_bIgnoreContentScaleFactor;
};

//	end of base_node group
/// @}

NS_CC_END

#endif	// __CCAtlasNode_h__


